#pragma once
#include <QDebug>

static const double Rgas = 8.31446;

struct accumulationPoint{
    double p_p;
    double p_s;
    double absTemp;
    qreal t;
    //double q;
};

class ExpCalc : public QObject
{
	Q_OBJECT

public:
    ExpCalc(QObject *parent = 0);
	~ExpCalc();
    void setConstants(double volumeChamber, double areaSample);
    void addAccumulationPoint(const accumulationPoint& aP);

    bool steadyStateTrigger();
    double timeLagCalc(const int &leakEnd, const int &steadyStateStart);

    void collectValues(QVector<qreal> &time, QVector<double> &diffusivity, QVector<double> &modeledDiffus); 

private:
    void calculateFlux(QVector<double> &diffusivity); // change of pressure per time, current absolute temperature
    void diffusionFit(QVector<double> &modeledDiffus);
    //UI to property
    unsigned int steadyStatePoints;
    //unsigned int steadyStateSeconds; // close to 60?
    double steadyStateEps;
    //
    double m_volume; //cm3 to m3
    double m_sampleArea;
    double m_thickness; // mkm to m
    QList<accumulationPoint> m_accumulation;

    double J_0_corr;
    double J_inf_corr;
};