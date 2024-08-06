#include "datamodel.h"

#include <QByteArray>
#include <QTimer>
#include <QtCore/QRandomGenerator>
#include <QtCore/QtMath>
#include <cstdlib>

DataModel::DataModel(QObject *parent) :
    QAbstractListModel(parent)
{

    m_trData["pressure"] = QSharedPointer<ControllerData>::create("pressure", 2.6046, -2.4978);
    m_trData["vacuum"] = QSharedPointer<ControllerData>::create("vacuum", 0.1303, -0.1253);
}

void DataModel::initializeAcquisition(){
    // is there any other initializations?
    m_time.start(); // Elapsed timer
}

int DataModel::rowCount( const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return m_trData.count();
}

QVariant DataModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
        return QVariant();
    // QStringList allNames = m_controllersToSensors["pressure"] + m_controllersToSensors["temperature"];

    // auto sensor = this->m_sensors[allNames.at(index.row())];

    const auto it = m_trData.begin() + index.row();

    if ( role == NameRole ){
        return it.value()->m_name;
    }
    else if ( role == Time )
        return QVariant::fromValue(it.value()->getTime());
    else if ( role == Value )
        return QVariant::fromValue(it.value()->getValue());
    else if ( role == CurTime)
        return QVariant::fromValue(it.value()->getCurTime());
    else if ( role == CurValue)
        return QVariant::fromValue(it.value()->getCurValue());
    else
        return QVariant();
}

//--> slide
QHash<int, QByteArray> DataModel::roleNames() const
{
    static QHash<int, QByteArray> mapping {
        {NameRole, "name"},
        {Time, "x"},
        {Value, "y"},
        {CurTime, "ct"},
        {CurValue, "cv"}
    };
    return mapping;
}

QSharedPointer<ControllerData> DataModel::getSensor(const QString &name){
    return m_trData[name];
}

void DataModel::appendData(const double & pressureVoltage, const double & vacuumVoltage){ 
    if(m_trData.isEmpty())
        return;
    qreal time = m_time.elapsed()/1000.0;

    m_trData["pressure"]->addPoint(time, pressureVoltage);
    m_trData["vacuum"]->addPoint(time, vacuumVoltage);

    const QModelIndex startIndex = index(0, 0);
    const QModelIndex endIndex   = index(m_trData.count() - 1, 0);
    // ...but only the population field
    emit dataChanged(startIndex, endIndex, QVector<int>() << Time << Value << CurTime << CurValue );
}

double DataModel::getLastTime(){
    return data(index(m_trData.count()-1),DataModel::Roles::Time).toDouble();
}
