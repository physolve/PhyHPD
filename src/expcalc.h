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
    Q_GADGET
    // it might be linked to json for import and multi-result log
public:
    double m_volume; // m3
    double m_thickness; // m
    double m_diameter;
    Q_PROPERTY (double volume MEMBER m_volume)
    Q_PROPERTY (double thickness MEMBER m_thickness)
    Q_PROPERTY (double diameter MEMBER m_diameter)
};

struct expTiming{
    Q_GADGET
public:
    unsigned int m_steadyStateStart;
    unsigned int m_leakStart; // second elapsed when valve open
    unsigned int m_leakEnd; // second elapsed after valve open
    Q_PROPERTY (unsigned int steadyStateStart MEMBER m_steadyStateStart)
    Q_PROPERTY (unsigned int leakStart MEMBER m_leakStart)
    Q_PROPERTY (unsigned int leakEnd MEMBER m_leakEnd)
};

struct expResults{
//     Q_GADGET
// public:
    double m_estPermeability;
    double m_estDiffusivity;
    // Q_PROPERTY (double estPermeability MEMBER m_estPermeability)
    // Q_PROPERTY (double estDiffusivity MEMBER m_estDiffusivity)
};

class ExpCalc : public QObject
{
	Q_OBJECT
    Q_PROPERTY(double timeLagVal READ timeLagVal NOTIFY timeLagValChanged)
    Q_PROPERTY(expParameters expParametersStruct READ getExpParametersStruct WRITE setExpParametersStruct NOTIFY expParametersStructChanged)
    Q_PROPERTY(expTiming expTimingStruct READ getExpTimingStruct WRITE setExpTimingStruct NOTIFY expTimingStructChanged)
    // Q_PROPERTY(expResults currentResults READ getExpResultsStruct WRITE setExpResultsStruct NOTIFY expResultsStructChanged)
    Q_PROPERTY(QString resultStr READ getResultStr NOTIFY resultChanged)
public:
    ExpCalc(QObject *parent = 0);
	~ExpCalc();
    void addAccumulationPoint(const accumulationPoint& aP);

    bool steadyStateTrigger();

    void collectValues(QVector<qreal> &time, QVector<double> &flux, QVector<double> &diffusivity, QVector<double> &modeledDiffus, QVector<double> &permeation);

    double timeLagVal() const;

    // Q_INVOKABLE void setConstants(const QVariantMap &expParametersMap); // sample parameters ?
    expParameters getExpParametersStruct() const;
    void setExpParametersStruct(expParameters val);
    // Q_INVOKABLE void setTimings(const QVariantMap &expTimingsMap);
    expTiming getExpTimingStruct() const;
    void setExpTimingStruct(expTiming val);

    // expResults getExpResultsStruct() const;
    // void setExpResultsStruct(expResults val);
    
    void reCalculateDiffusFit(QVector<double> &modeledDiffus, const double &corrTimeLagVal);
    QString getResultStr() const;
signals:
    void timeLagValChanged();
    void expParametersStructChanged();
    void expTimingStructChanged();
    // void expResultsStructChanged();
    void resultChanged();
private:
    double timeLagCalc();
    void calculateFlux(QVector<double> &flux, QVector<double> &diffusivity, QVector<double> &permeation); // change of pressure per time, current absolute temperature
    void diffusionFit(QVector<double> &modeledDiffus);
    void setTimeLagCalc(const double &val);
    // void limitedPermeability(const qreal &endTime);
    //UI to property
    unsigned int steadyStatePoints;
    //unsigned int steadyStateSeconds; // close to 60?
    double steadyStateEps;
    //
    expParameters currentExpParameters;
    expTiming currentExpTiming;

    expResults currentResults;
    QString resultStr;

    QList<accumulationPoint> m_accumulation;

    double J_0_corr;
    double J_inf_corr;

    double m_timeLagVal;
};