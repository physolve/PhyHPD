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
    Q_PROPERTY(double timeLagVal READ timeLagVal NOTIFY timeLagValChanged)
    Q_PROPERTY(unsigned int steadyStateStart READ steadyStateStart NOTIFY steadyStateStartChanged)
    Q_PROPERTY(unsigned int leakStart READ leakStart NOTIFY leakStartChanged)
    Q_PROPERTY(unsigned int leakEnd READ leakEnd NOTIFY leakEndChanged)
public:
    ExpCalc(QObject *parent = 0);
	~ExpCalc();
    void setFileTimes();
    void addAccumulationPoint(const accumulationPoint& aP);

    bool steadyStateTrigger();

    void collectValues(QVector<qreal> &time, QVector<double> &flux, QVector<double> &diffusivity, QVector<double> &modeledDiffus, QVector<double> &permeation);

    double timeLagVal() const;
    unsigned int steadyStateStart() const;
    unsigned int leakStart() const;
    unsigned int leakEnd() const;

    Q_INVOKABLE void setConstants(const QVariantMap &expParametersMap);

    void reCalculateDiffusFit(QVector<double> &modeledDiffus, const double &corrTimeLagVal);
    void reCalculateNewTime(unsigned int steadyStateStart, unsigned int leakStart, unsigned int leakEnd);

signals:
    void timeLagValChanged();
    void steadyStateStartChanged();
    void leakStartChanged();
    void leakEndChanged();
private:
    double timeLagCalc();
    void calculateFlux(QVector<double> &flux, QVector<double> &diffusivity, QVector<double> &permeation); // change of pressure per time, current absolute temperature
    void diffusionFit(QVector<double> &modeledDiffus);
    void setTimeLagCalc(const double &val);
    //UI to property
    unsigned int steadyStatePoints;
    //unsigned int steadyStateSeconds; // close to 60?
    double steadyStateEps;
    //
    expParameters currentExp;
    QList<accumulationPoint> m_accumulation;

    double J_0_corr;
    double J_inf_corr;

    // correct from qml
    unsigned int m_steadyStateStart;
    unsigned int m_leakStart; // second elapsed when valve open
    unsigned int m_leakEnd; // second elapsed after valve open
    double m_timeLagVal;
};