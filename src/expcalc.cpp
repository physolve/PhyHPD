#include "expcalc.h"
#include <math.h>

ExpCalc::ExpCalc(QObject *parent) : QObject(parent), currentExp({1,1,1}), m_timeLagVal(0), m_leakEnd(0), m_leakStart(0), m_steadyStateStart(0)
{

}

ExpCalc::~ExpCalc(){
    
}

void ExpCalc::setConstants(const QVariantMap &expParametersMap){
    currentExp = expParameters{expParametersMap["sideVolume"].toDouble(), expParametersMap["thickness"].toDouble() * pow(10, -3), M_PI * pow(expParametersMap["diameter"].toDouble(),2)/4};
}

void ExpCalc::setFileTimes(){
    m_steadyStateStart = 600; // extern parameters
    emit steadyStateStartChanged();
    m_leakStart = 280; // second elapsed when valve open
    emit leakStartChanged();
    m_leakEnd = 330; // extern parameters
    emit leakEndChanged();
}

void ExpCalc::addAccumulationPoint(const accumulationPoint& aP){
    m_accumulation << aP;
}

bool ExpCalc::steadyStateTrigger(){
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
        if(c.t < m_steadyStateStart) break;
    }
    double a = (n*xysum - xsum*ysum)/(n*x2sum - xsum*xsum);
    double b = (x2sum*ysum - xsum*xysum)/(x2sum*n - xsum*xsum);
    if(a<=0) {
        qDebug() << "Wrong slope for timeLag!";
        return 1;
    }
    return -b/a - m_leakEnd; 
}

void ExpCalc::collectValues(QVector<qreal> &time, QVector<double> &flux, QVector<double> &diffusivity, QVector<double> &modeledDiffus, QVector<double> &permeation){

    for(const auto &i : m_accumulation){ // diffusivity times starts from leak
        time << i.t;
    }
    calculateFlux(flux, diffusivity, permeation);
    setTimeLagCalc(timeLagCalc());
    diffusionFit(modeledDiffus);
}

void ExpCalc::calculateFlux(QVector<double> &flux, QVector<double> &diffusivity, QVector<double> &permeability){
    // to change m_accumulation with averaged values or different time range use argument of this function
    
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
    for(int i = 1; i < m_accumulation.count(); ++i){ // trough selected data
        const auto& p = m_accumulation.at(i); // curr point
        const auto& o = m_accumulation.at(i-1); // prev point
        J_vals << currentExp.volume / (Rgas * p.absTemp) * (p.p_s - o.p_s) / (p.t-o.t) * 1 / currentExp.sampleArea; // actually J(t) basically flux
        if(p.t > m_leakStart && p.t < m_leakEnd){
            permeability << 0;
            J_leak << J_vals.last();
        }
        else{
            permeability << J_vals.last() * currentExp.thickness / (sqrt(p.p_p) - sqrt(p.p_s)); //?
            if(p.t > m_steadyStateStart){
                J_inf << J_vals.last();
            }
        }
        // is it right?
        // p_buff = p.p_s;
        // t_buff = p.t;
    }
    //suppose we know start of steady stade
    //  than J_0 = 1/A*dn_leak/dt
    //  and J_inf = 1/A*dn_inf/dt
    //  permeation J = J_inf - J_0 as for permeation in peak

    // split to another function 
    J_0_corr = J_leak.last(); // or not last
    J_inf_corr = J_inf.last(); // or not last
    for(const auto &val : J_vals){
        // crutch
        auto curDiff = (val-J_0_corr)/(J_inf_corr - J_0_corr);
        if(curDiff < 0)
            curDiff = 0;
        diffusivity << curDiff;
    }

    flux = J_vals;
}

double ExpCalc::timeLagVal() const{
    return m_timeLagVal;
}
unsigned int ExpCalc::steadyStateStart() const{
    return m_steadyStateStart;
}
unsigned int ExpCalc::leakStart() const{
    return m_leakStart;
}
unsigned int ExpCalc::leakEnd() const{
    return m_leakEnd;
}

void ExpCalc::reCalculateDiffusFit(QVector<double> &modeledDiffus, const double &corrTimeLagVal){
    setTimeLagCalc(corrTimeLagVal);
    diffusionFit(modeledDiffus);
}

void ExpCalc::setTimeLagCalc(const double &val){
    m_timeLagVal = val;
    emit timeLagValChanged();
}

void ExpCalc::reCalculateNewTime(unsigned int steadyStateStart, unsigned int leakStart, unsigned int leakEnd){
    m_steadyStateStart = steadyStateStart; // extern parameters
    emit steadyStateStartChanged();
    m_leakStart = leakStart; // second elapsed when valve open
    emit leakStartChanged();
    m_leakEnd = leakEnd; // extern parameters
    emit leakEndChanged();
}

void ExpCalc::diffusionFit(QVector<double> &modeledDiffus){
    // function to east squares method fit
    double initialD = pow(currentExp.thickness, 2) / (6 * m_timeLagVal);
    const double L = currentExp.thickness;
    auto f = [initialD, L] (unsigned int t){
        double sum = 0;
        for(int n = 1; n <= 20; ++n){
            sum += pow(-1, n) * exp(-(initialD * pow(n, 2) * pow(M_PI, 2))/pow(L, 2) * t);
        }
        return 1 + 2 * sum;
    };
    
    // somewhere cause of error 
    // QVector<double> modeledFlux;
    for(const auto& p : m_accumulation){
        if(p.t <= m_leakEnd){
            modeledDiffus << 0;
            continue;
        }
        modeledDiffus << f(p.t-m_leakEnd);
    }
    // set modeledFlux to chart
}
//flux is calculated by two points
// flux returns in vector
// flux values is used for obtain permeation