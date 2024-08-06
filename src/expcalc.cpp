#include "expcalc.h"
#include <math.h>

ExpCalc::ExpCalc(QObject *parent) : QObject(parent), m_volume(1), m_sampleArea(1), m_thickness(1)
{

}

ExpCalc::~ExpCalc(){
    
}

void ExpCalc::setConstants(double volumeChamber, double areaSample){
    this->m_volume = volumeChamber;
    this->m_sampleArea = areaSample;
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
        if(lastT - c.t > steadyStateSeconds) break;
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
    unsigned int xsum = 0, x2sum = 0;
    double ysum = 0, xysum = 0;
    QListIterator<accumulationPoint> i (m_accumulation); // trough selected data
    i.toBack();
    unsigned int lastT = m_accumulation.last().t; // last elapsed second
    while(i.hasPrevious()) {
        const auto& c = i.previous();     
        xsum += c.t;
        ysum += c.p_s;
        x2sum += pow(c.t, 2);
        xysum += c.t * c.p_s;
        // seconds?
        if(lastT - c.t > steadyStateSeconds) break;
    }
    const auto& n = m_accumulation.count();
    double a = (n*xysum - xsum*ysum)/(n*x2sum - xsum*xsum);
    double b = (x2sum*ysum - xsum*xysum)/(x2sum*n - xsum*xsum);
    if(a<=0) {
        qDebug() << "Wrong slope for timeLag!";
        return 1;
    }
    return -b/a; 
}

void ExpCalc::collectValues(QVector<qreal> &time, QVector<double> &diffusivity, QVector<double> &modeledDiffus){

    for(const auto &i : m_accumulation){
        time << i.t;
    }
    calculateFlux(diffusivity);
    diffusionFit(modeledDiffus);
}

void ExpCalc::calculateFlux(QVector<double> &diffusivity){
    // to change m_accumulation with averaged values or different time range use argument of this function
    
    //dn_leak/dt -> moles change during valve open (as very low rate) 
    //  = V/RT*dPleak/dt
    //dn_inf/dt -> moles change during steady state 
    //  = V/RT*dPinf/dt

    //find near leak moment using time range from saved
    unsigned int leakStart = 46; // second elapsed when valve open
    unsigned int leakEnd = 55; // second elapsed after valve open

    unsigned int steadyStateStart = 4000;
    
    QList<double> J_vals{0};
    QList<double> J_leak{0};
    QList<double> J_inf{0};
    // double p_buff = m_accumulation.first().p_s;
    // double t_buff = m_accumulation.first().t;
    QVector<double> permeability;
    for(int i = 1; i < m_accumulation.count(); ++i){ // trough selected data
        const auto& p = m_accumulation.at(i); // curr point
        const auto& o = m_accumulation.at(i-1); // prev point
        J_vals << m_volume / (Rgas * p.absTemp) * (p.p_s - o.p_s) / (p.t-o.t) * 1 / m_sampleArea; // actually J(t) 
        if(p.t > leakStart && p.t < leakEnd){
            J_leak << J_vals.last();
        }
        else{
            permeability << J_vals.last() * m_thickness / (sqrt(p.p_p) - sqrt(p.p_s)); //?
            if(p.t > steadyStateStart){
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
    const double J_0_corr = J_leak.last(); // or not last
    const double J_inf_corr = J_inf.last(); // or not last
    for(const auto &val : J_vals){
        diffusivity << (val-J_0_corr)/(J_inf_corr - J_0_corr);
    }
}

void ExpCalc::diffusionFit(QVector<double> &modeledDiffus){
    //J_0_avg, J_inf_avg and J_t
    double J_0 = 1;
    double J_inf = 1;
    // function to east squares method fit
    double initialD = pow(m_thickness, 2) / (6 * timeLagCalc());
    const double L = m_thickness;
    auto f = [initialD, L, J_0, J_inf] (unsigned int t){
        double sum = 0;
        for(int n = 1; n <= 10; ++n){
            sum += pow(-1, n) * exp(-(initialD * pow(n, 2) * pow(M_PI, 2))/pow(L, 2) * t);
        }
        return (1 + 2 * sum)*(J_inf-J_0)+J_0;
    };
    // QVector<double> modeledFlux;
    for(const auto& p : m_accumulation){
        modeledDiffus << f(p.t);
    }
    // set modeledFlux to chart
}
//flux is calculated by two points
// flux returns in vector
// flux values is used for obtain permeation