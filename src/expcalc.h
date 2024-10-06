#pragma once
#include <QDebug>
#include <QVariant>
#include <QElapsedTimer>

#include "datacollection.h"
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
    Q_PROPERTY (double volume MEMBER m_volume)
    Q_PROPERTY (double thickness MEMBER m_thickness)
    Q_PROPERTY (double diameter MEMBER m_diameter)
public:
    double m_volume; // m3
    double m_thickness; // m
    double m_diameter;
};

struct expTiming{
    Q_GADGET
public:    
    unsigned int m_leakStart; // second elapsed when valve open
    unsigned int m_leakEnd; // second elapsed after valve open
    unsigned int m_steadyStateStart;
    Q_PROPERTY (int leakStart MEMBER m_leakStart)
    Q_PROPERTY (unsigned int leakEnd MEMBER m_leakEnd)
    Q_PROPERTY (unsigned int steadyStateStart MEMBER m_steadyStateStart)
};

struct expInfo{
    Q_GADGET
public:
    QString m_expName;
    unsigned int m_expStart; // to expTiming
    unsigned int m_expEnd; // to expTiming
    bool isExpWorking;
    Q_PROPERTY (int expStart MEMBER m_expStart)
    Q_PROPERTY (int expEnd MEMBER m_expEnd)
};

struct expResults{
    Q_GADGET
public:
    double m_estPermeability;
    double m_estDiffusivity;
    double m_timeLagVal;
    // move time Lag here
    // Q_PROPERTY (double estPermeability MEMBER m_estPermeability)
    // Q_PROPERTY (double estDiffusivity MEMBER m_estDiffusivity)
    Q_PROPERTY(double timeLagVal MEMBER m_timeLagVal) //READ timeLagVal NOTIFY timeLagValChanged
};

class ExpCalc : public QObject
{
	Q_OBJECT
    Q_PROPERTY(expParameters expParametersStruct READ getExpParametersStruct WRITE setExpParametersStruct NOTIFY expParametersStructChanged) 
    Q_PROPERTY(expTiming expTimingStruct READ getExpTimingStruct WRITE setExpTimingStruct  NOTIFY expTimingStructChanged) //
    Q_PROPERTY(expResults expResultsStruct READ getExpResultsStruct NOTIFY expResultsStructChanged) //WRITE setExpTimingStruct
    Q_PROPERTY(expInfo expInfoStruct READ getExpInfoStruct NOTIFY expInfoStructChanged) //WRITE setExpTimingStruct 
    Q_PROPERTY(QString resultStr READ getResultStr NOTIFY resultChanged)
public:
    ExpCalc(QList<QSharedPointer<ControllerData>> dataStorage, QList<QSharedPointer<ExpData>> expStorage, QObject *parent = 0);
	~ExpCalc();
    Q_INVOKABLE void applyExpFromJSON();
    Q_INVOKABLE void reCalculateDiffusFit(const double &corrTimeLagVal); //test
    
    Q_INVOKABLE bool setLeakStart(bool s);
    Q_INVOKABLE bool setLeakEnd(bool s);
    Q_INVOKABLE bool setSteadyStateStart(bool s);
    
    void addAccumulationPoint(const accumulationPoint& aP);
    bool steadyStateTrigger();
    void collectValues();

    void startExpTime(bool s);

    // Q_INVOKABLE void setConstants(const QVariantMap &expParametersMap); // sample parameters ?
    expParameters getExpParametersStruct() const;
    void setExpParametersStruct(const expParameters &val);
    // Q_INVOKABLE void setTimings(const QVariantMap &expTimingsMap);
    expTiming getExpTimingStruct() const;
    void setExpTimingStruct(const expTiming &val);

    expResults getExpResultsStruct() const;
    // void setExpResultsStruct(expResults val);

    expInfo getExpInfoStruct() const;
    
    // void reCalculateDiffusFit(const double &corrTimeLagVal);
    QString getResultStr() const;
signals:
    void expParametersStructChanged();
    void expTimingStructChanged();
    void expResultsStructChanged();
    void expInfoStructChanged();
    void resultChanged();

    void fluxChanged();
private:
    QSharedPointer<ControllerData> timeData;
    QSharedPointer<ControllerData> pressure;
    QSharedPointer<ControllerData> vacuum;
    QSharedPointer<ExpData> timeExp;
    QSharedPointer<ExpData> fluxExp;
    QSharedPointer<ExpData> diffusivityExp;
    QSharedPointer<ExpData> modeldiffusExp;
    QSharedPointer<ExpData> permeationExp;

    QElapsedTimer m_expTime;

    double timeLagCalc();
    void calculateFlux(); // change of pressure per time, current absolute temperature
    void diffusionFit();
    // void limitedPermeability(const qreal &endTime);
    //UI to property
    unsigned int steadyStatePoints;
    //unsigned int steadyStateSeconds; // close to 60?
    double steadyStateEps;

    double J_0_corr;
    double J_inf_corr;

    expInfo currentExpInfo;
    
    expParameters currentExpParameters;
    expTiming currentExpTiming;
    expResults currentExpResults;
    QString resultStr;

    QList<QSharedPointer<ExpData>> expStorage;

    QList<accumulationPoint> m_accumulation;
};