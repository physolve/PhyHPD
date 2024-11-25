#include "expcalc.h"
#include <math.h>
#include <QFile>

ExpCalc::ExpCalc(QList<QSharedPointer<ControllerData>> dataStorage, QList<QSharedPointer<ExpData>> expStorage, QObject *parent) : QObject(parent),
currentExpResults{0, 0, 100}, currentExpParameters{"blank",1,1,1}, currentExpTiming{0,0,0}, currentExpInfo{"unknown", 0, 0, 27, false, "", "", -1}
{
    timeData = dataStorage[0];
    pressure = dataStorage[1];
    vacuum = dataStorage[2];

    timeExp = expStorage[0];
    fluxExp = expStorage[1];
    diffusivityExp = expStorage[2];
    modeldiffusExp = expStorage[3];
    permeationExp = expStorage[4];
}

ExpCalc::~ExpCalc(){
    
}

void ExpCalc::setExpParametersStruct(const expParameters &val){
    currentExpParameters = val;
    // emit expParametersStructChanged();
}
void ExpCalc::setExpTimingStruct(const expTiming &val){
    currentExpTiming = val;
    // emit expTimingStructChanged();
}

void ExpCalc::setExpInfoStruct(const expInfo &val){
    currentExpInfo = val;
}

expParameters ExpCalc::getExpParametersStruct() const{
    return currentExpParameters;
}
expTiming ExpCalc::getExpTimingStruct() const{
    return currentExpTiming;
}
expResults ExpCalc::getExpResultsStruct() const{
    return currentExpResults;
}
expInfo ExpCalc::getExpInfoStruct() const{
    return currentExpInfo;
}

QStringList ExpCalc::getExpNames() const{
    return jsonLoader.getExpList();
}

void ExpCalc::applyExpFromJSON(const QString &expName){
    currentExpParameters = jsonLoader.getExpParameters(expName);
    emit expParametersStructChanged();
    currentExpTiming = jsonLoader.getExpTiming(expName);
    emit expTimingStructChanged();
    currentExpInfo = jsonLoader.getExpInfo(expName);
    emit expInfoStructChanged();
}

bool ExpCalc::isLastDataAvailable(const QString &expName){
    return jsonLoader.checkLastSampleData(expName);
}

void ExpCalc::calculateLastData(){
    // cleaning data containers
    // if(setPointsFromFile(jsonLoader.getSampleDataFile(currentExpInfo.m_expName))){
    //     collectValues();
    // }
    if(currentExpInfo.m_lastExpDataFile.isEmpty())
        return;
    if(setPointsFromFile(currentExpInfo.m_lastExpDataFile)){
        collectValues();
    }
}

bool ExpCalc::setPointsFromFile(const QString &sampleDataFilePath){ // log calc function
    QFile expData(sampleDataFilePath); // path to current data file from JSON struct
	if (!expData.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream in(&expData);
    in.readLine(); // skip header
    
    while (!in.atEnd()) {
        const auto &lineList = in.readLine().split('\t');
        double p_p = lineList.at(2).toDouble() * pow(10,5); //pressure, bar to Pa
        double p_s = lineList.at(3).toDouble() * pow(10,5); //vacuum, bar to Pa
        double absTemp = 273 + currentExpInfo.m_temperature; // K 
        unsigned int t = lineList.at(1).toInt();

        m_accumulation << (accumulationPoint(p_p,p_s,absTemp,t));
        // unsigned int n = 120;
        // while(n > 0 && !in.atEnd()){
        //     in.readLine();
        //     --n;
        // }
    }
    return true;
}

void ExpCalc::applyExpToJSON(const QString &expName){
    // firstly save under new name?
    qDebug() << "Saving to JSON for " << expName; 
    // different logic for new and old?
    // if new then add to sampleNamesList
    // if old then 
    if(jsonLoader.addNewSample(currentExpInfo, currentExpParameters, currentExpTiming)){
        // make new as current
        emit expNamesChanged();
    }
}

void ExpCalc::setExpTime(bool s){
    if(s){
        currentExpInfo.m_expStart = timeData->getCurValue();
    }
    else{
        currentExpInfo.m_expEnd = timeData->getCurValue();
    }
    currentExpInfo.isExpWorking = s;
    emit expInfoStructChanged();
}

void ExpCalc::setExpDataPath(const QString &path){
    if(!currentExpInfo.m_currentExpDataFile.isEmpty())
        qDebug() << "Rewrite existing expDataPath";
    currentExpInfo.m_currentExpDataFile = path;
    emit expInfoStructChanged();
}

bool ExpCalc::setLeakStart(bool s){
    if(!currentExpInfo.isExpWorking)
        return false;
    if(s){
        currentExpTiming.m_leakStart = round(timeData->getCurValue());
        emit expTimingStructChanged();
    }
    return true;
}

bool ExpCalc::setLeakEnd(bool s){
    if(!currentExpInfo.isExpWorking)
        return false;
    if(s){
        currentExpTiming.m_leakEnd = round(timeData->getCurValue());
        emit expTimingStructChanged();
    }
    return true;
}

bool ExpCalc::setSteadyStateStart(bool s){
    if(!currentExpInfo.isExpWorking)
        return false;
    if(s){
        currentExpTiming.m_steadyStateStart = round(timeData->getCurValue());
        emit expTimingStructChanged();
    }
    return true;
}

bool ExpCalc::steadyStateTrigger(){ // try to use with pseudo data
    QListIterator<accumulationPoint> i (m_accumulation); // through ALL data 
    i.toBack();
    const auto& l = i.previous();
    double lastP = l.p_s;
    const unsigned int lastT = l.t;
    QVector<double> slopeList;
    double slopeSum = 0;
    while(i.hasPrevious()) {
        const auto& c = i.previous();     
        slopeList << (c.p_s - lastP)/(c.t - lastT);
        slopeSum += slopeList.last();
        // seconds?
        if(lastT - c.t > 60) break;
    }
    double slopeMean = slopeSum / slopeList.count(); 
    double sigmaSum = 0;
    for(const double& c_slope : slopeList){
        sigmaSum += pow(c_slope-slopeMean, 2);
    }
    double sd = sqrt(sigmaSum / slopeList.count()); // standard deviation
    double sv = sd / slopeMean; // coefficient of variation
    return sv < 0.1;
}

void ExpCalc::collectValues(){
    QVector<qreal> timeList;
    for(const auto &i : m_accumulation){ // diffusivity times starts from leak
        // time << i.t;
        timeList << i.t;
    }
    timeExp->setData(timeList);
    calculateFlux();

    // setTimeLagCalc(timeLagCalc());
    currentExpResults.m_timeLagVal = timeLagCalc();
    emit expResultsStructChanged();

    diffusionFit();
    emit resultChanged();
}

void ExpCalc::calculateFlux(){
    // to change m_accumulation with averaged values or different time range use argument of this function
    QVector<double> diffusivity, permeability{0};
    //dn_leak/dt -> moles change during valve open (as very low rate) 
    //  = V/RT*dPleak/dt
    //dn_inf/dt -> moles change during steady state 
    //  = V/RT*dPinf/dt
    QList<double> J_vals{0};
    QList<double> J_leak{0};
    QList<double> J_inf{0};
    // double p_buff = m_accumulation.first().p_s;
    // double t_buff = m_accumulation.first().t;
    // QVector<double> permeability;
    const auto &sampleArea = M_PI * pow(currentExpParameters.m_diameter,2)/4; // m2
    const auto &thickness = currentExpParameters.m_thickness * pow(10,-3); // m

    const qreal &endTime = m_accumulation.last().t; // for now
    int startAvg = 0;
    int endAvg = 0;

    for(int i = 1; i < m_accumulation.count(); ++i){ // trough selected data
        const auto& p = m_accumulation.at(i); // curr point
        const auto& o = m_accumulation.at(i-1); // prev point
        J_vals << currentExpParameters.m_volume / (Rgas * p.absTemp) * (p.p_s - o.p_s) / (p.t-o.t) * 1 / sampleArea; // actually J(t) basically flux
        if(p.t > currentExpTiming.m_leakStart && p.t < currentExpTiming.m_leakEnd){
            permeability << 0;
            J_leak << J_vals.last();
        }
        else{
            permeability << J_vals.last() * thickness / (sqrt(p.p_p) - sqrt(p.p_s)); //?
            if(p.t > currentExpTiming.m_steadyStateStart){
                J_inf << J_vals.last();
                if(p.t <= endTime) endAvg = i;
            }
            else startAvg = i;
        }
        // is it right?
        // p_buff = p.p_s;
        // t_buff = p.t;
    }
    //suppose we know start of steady stade
    //  than J_0 = 1/A*dn_leak/dt
    //  and J_inf = 1/A*dn_inf/dt
    //  permeation J = J_inf - J_0 as for permeation in peak

    // for now last time is last point of accumulation
    QVector<double> averagePermeabilityList = permeability.mid(startAvg,endAvg-startAvg+1);
    double sumPermeability = 0;
    for(const double &val : averagePermeabilityList){
        sumPermeability += val;
    }
    currentExpResults.m_estPermeability = sumPermeability/averagePermeabilityList.count();
    // split to another function 
    J_0_corr = J_leak.last(); // or not last
    J_inf_corr = J_inf.last(); // or not last! limit to exp end timing
    for(const auto &val : J_vals){
        // crutch
        auto curDiff = (val-J_0_corr)/(J_inf_corr - J_0_corr); // flux but normalized
        if(curDiff < 0)
            curDiff = 0;
        diffusivity << curDiff;
    }

    // argument save
    fluxExp->setData(J_vals);
    diffusivityExp->setData(diffusivity);
    permeationExp->setData(permeability);
}

// this function must be called ether manually or automatically by some trigger
double ExpCalc::timeLagCalc(){
    // when linear steady state is reached, calculate linear fit for the last steadyStateSeconds
    qreal xsum = 0, x2sum = 0;
    double ysum = 0, xysum = 0;
    QListIterator<accumulationPoint> i (m_accumulation); // trough selected data
    i.toBack();
    qreal lastT = m_accumulation.last().t; // last elapsed second
    unsigned int n = 0;
    while(i.hasPrevious()) {
        const auto& c = i.previous();     
        xsum += c.t;
        ysum += c.p_s; 
        x2sum += pow(c.t, 2);
        xysum += c.t * c.p_s;
        // seconds?
        n++;
        if(c.t < currentExpTiming.m_steadyStateStart) break;
    }
    double a = (n*xysum - xsum*ysum)/(n*x2sum - xsum*xsum);
    double b = (x2sum*ysum - xsum*xysum)/(x2sum*n - xsum*xsum);
    if(a<=0) {
        qDebug() << "Wrong slope for timeLag!";
        return 1;
    }
    return -b/a - currentExpTiming.m_leakEnd; 
}

// void ExpCalc::setTimeLagCalc(const double &val){
//     m_timeLagVal = val;
//     emit timeLagValChanged();
// }

void ExpCalc::diffusionFit(){
    QVector<double> modeledDiffus;
    // function to east squares method fit
    const auto &thickness = currentExpParameters.m_thickness * pow(10,-3); // m



    double initialD = pow(thickness, 2) / (6 * currentExpResults.m_timeLagVal);
    const double &L = thickness;
    
    auto f = [initialD, L] (unsigned int t){
        double sum = 0;
        for(int n = 1; n <= 20; ++n){
            sum += pow(-1, n) * exp(-(initialD * pow(n, 2) * pow(M_PI, 2))/pow(L, 2) * t);
        }
        return 1 + 2 * sum;
    };
    // aplly initialD as new value for Diffusivity
    currentExpResults.m_estDiffusivity = initialD; // update signal?
    // somewhere cause of error 
    // QVector<double> modeledFlux;
    for(const auto& p : m_accumulation){
        if(p.t <= currentExpTiming.m_leakEnd){
            modeledDiffus << 0;
            continue;
        }
        modeledDiffus << f(p.t-currentExpTiming.m_leakEnd);
    }

    modeldiffusExp->setData(modeledDiffus);
    // set modeledFlux to chart
}

// void MainWindow::reCalculateNewData(){ // anything changed
//     expCalc->collectValues();
//     // calcToPlot();
// } //test


//flux is calculated by two points
// flux returns in vector
// flux values is used for obtain permeation

void ExpCalc::reCalculateDiffusFit(const double &corrTimeLagVal){
    // setTimeLagCalc(corrTimeLagVal);
    currentExpResults.m_timeLagVal = corrTimeLagVal;
    emit expResultsStructChanged();
    diffusionFit();
    emit resultChanged();
}

// double ExpCalc::timeLagVal() const{
//     return m_timeLagVal;
// }

QString ExpCalc::getResultStr() const
{
    const QString &text = QString("Diffusivity: %1\n Permeability: %2").arg(currentExpResults.m_estDiffusivity).arg(currentExpResults.m_estPermeability);
    return text;
}