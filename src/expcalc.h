#pragma once
#include <QDebug>
#include <QVariant>
#include <QElapsedTimer>
#include "expstructures.h"
#include "jsonloader.h"
#include "datacollection.h"
static const double Rgas = 8.31446;

class ExpCalc : public QObject
{
	Q_OBJECT
    Q_PROPERTY(QStringList sampleNames READ getSampleNames NOTIFY sampleNamesChanged) // notify after new save
    Q_PROPERTY(expParameters expParametersStruct READ getExpParametersStruct WRITE setExpParametersStruct NOTIFY expParametersStructChanged) 
    Q_PROPERTY(expTiming expTimingStruct READ getExpTimingStruct WRITE setExpTimingStruct  NOTIFY expTimingStructChanged) //
    Q_PROPERTY(expResults expResultsStruct READ getExpResultsStruct NOTIFY expResultsStructChanged) //WRITE setExpTimingStruct
    Q_PROPERTY(expInfo expInfoStruct READ getExpInfoStruct NOTIFY expInfoStructChanged) //WRITE setExpTimingStruct 
    Q_PROPERTY(QString resultStr READ getResultStr NOTIFY resultChanged)
public:
    ExpCalc(QList<QSharedPointer<ControllerData>> dataStorage, QList<QSharedPointer<ExpData>> expStorage, QObject *parent = 0);
	~ExpCalc();
    Q_INVOKABLE void applyExpFromJSON(const QString &name);
    Q_INVOKABLE void applyExpToJSON(const QString &name);
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

    QStringList getSampleNames() const;
signals:
    void sampleNamesChanged();
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

    JsonLoader jsonLoader;

    QList<QSharedPointer<ExpData>> expStorage;

    QList<accumulationPoint> m_accumulation;
};