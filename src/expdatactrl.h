#pragma once

#include <QDebug>
#include <QFile>
#include <QTimer>
#include "expstructures.h"
#include "datacollection.h"

struct expCtrl { // for any other purpose
    double m_relativeTimeStart;
    int m_writeDataInterval;
};

class ExpDataCtrl : public QObject // class for writing separate Data info with special time period 
{
	Q_OBJECT
public:
    ExpDataCtrl(QList<QSharedPointer<ControllerData>> dataStorage, QObject *parent = 0);
	~ExpDataCtrl();

    void setTimerInterval(int interval);
    void startExpCtrl(double relativeTimeStart);
    void fileCreation(const QString &expName);
    void endExpCtrl();
    QString getExpFilePath() const;
private slots:
    void processEvents();

private:
    QSharedPointer<ControllerData> timeData;
    QSharedPointer<ControllerData> pressure;
    QSharedPointer<ControllerData> vacuum;
    // QSharedPointer<ExpData> timeExp;
    // QSharedPointer<ExpData> fluxExp;
    // QSharedPointer<ExpData> diffusivityExp;
    // QSharedPointer<ExpData> modeldiffusExp;
    // QSharedPointer<ExpData> permeationExp;

    expCtrl currentExpCtrl; // m_relativeTimeStart, m_writeDataInterval

    QTimer* m_timer;

    QFile expDataFile;
};