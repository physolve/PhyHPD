#include "chartmodel.h"

ChartModel::ChartModel(QList<QSharedPointer<ControllerData>> dataStorage, QList<QSharedPointer<ExpData>> expStorage, QObject *parent) : QObject(parent){
    // timeData = dataStorage[0];
    // pressure = dataStorage[1];
    // vacuum = dataStorage[2];
    
    // timeExp = expStorage[0];
    // fluxExp = expStorage[1];
    // diffusivityExp = expStorage[2];
    // modeldiffusExp = expStorage[3];
    // permeationExp = expStorage[4];
    timePressure = { dataStorage[0].staticCast<DataCollection>(), dataStorage[1].staticCast<DataCollection>()};
    timeVacuum = { dataStorage[0].staticCast<DataCollection>(), dataStorage[2].staticCast<DataCollection>()};
    
    timeFlux = { expStorage[0].staticCast<DataCollection>(), expStorage[1].staticCast<DataCollection>()};
    timeDiffModelDiff = { expStorage[0].staticCast<DataCollection>(), expStorage[2].staticCast<DataCollection>(), expStorage[3].staticCast<DataCollection>()};
    timePermeation = { expStorage[0].staticCast<DataCollection>(), expStorage[4].staticCast<DataCollection>()};
}

QList<QSharedPointer<DataCollection>> ChartModel::getChartPtr(QString chartName){
    if(chartName == "timePressure"){
        return timePressure;
    }
    else if(chartName == "timeVacuum"){
        return timeVacuum;
    }
    else if(chartName == "timeFlux"){
        return timeFlux;
    }
    else if(chartName == "timeDiffModelDiff"){
        return timeDiffModelDiff;
    }
    else if(chartName == "timePermeation"){
        return timePermeation;
    }
    return timePressure; //default
}