#pragma once
#include <QDebug>
#include <QVariant>

static const double Rgas = 8.31446;

struct accumulationPoint{
    double p_p;
    double p_s;
    double absTemp;
    qreal t;
    //double q;
};

struct expParameters{
    // it might be linked to json for import and multi-result log
    double volume; // m3
    double thickness; // m
    double sampleArea; // m2
};

class ExpCalc : public QObject
{
	Q_OBJECT

public:
    ExpCalc(QObject *parent = 0);
	~ExpCalc();
    void addAccumulationPoint(const accumulationPoint& aP);

    bool steadyStateTrigger();
    double timeLagCalc(const int &leakEnd, const int &steadyStateStart);

    void collectValues(QVector<qreal> &time, QVector<double> &flux, QVector<double> &diffusivity, QVector<double> &modeledDiffus, QVector<double> &permeation);

    Q_INVOKABLE void setConstants(const QVariantMap &expParametersMap);

private:
    void calculateFlux(QVector<double> &flux, QVector<double> &diffusivity, QVector<double> &permeation); // change of pressure per time, current absolute temperature
    void diffusionFit(QVector<double> &modeledDiffus);
    //UI to property
    unsigned int steadyStatePoints;
    //unsigned int steadyStateSeconds; // close to 60?
    double steadyStateEps;
    //
    expParameters currentExp;
    QList<accumulationPoint> m_accumulation;

    double J_0_corr;
    double J_inf_corr;
};