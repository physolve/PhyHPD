#pragma once

#include "datacollection.h"
#include "customplotitem.h"

class ChartModel : public QObject
{
    Q_OBJECT
public:
    explicit ChartModel(QList<QSharedPointer<ControllerData>> dataStorage, QList<QSharedPointer<ExpData>> expStorage, QObject *parent = nullptr);
    // theoretically it could be properties as list of chart pointers
    Q_INVOKABLE QList<QSharedPointer<DataCollection>> getChartPtr(QString chartName);
private:
    QList<QSharedPointer<DataCollection>> timePressure;
    QList<QSharedPointer<DataCollection>> timeVacuum;
    
    QList<QSharedPointer<DataCollection>> timeFlux;
    QList<QSharedPointer<DataCollection>> timeDiffModelDiff;
    QList<QSharedPointer<DataCollection>> timePermeation;
};