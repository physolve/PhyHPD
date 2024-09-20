#include <cmath>
#include "exptable.h"

ExpTable::ExpTable(QObject *parent)
    : QAbstractTableModel(parent), currentDataCount(0)
{
    header << "Time" << "Flux"<< "Diffusivity" << "Model Diffus" << "Permeation";
    m_expData["time"] = QSharedPointer<ExpData>::create("time", 0); // 0 - dataset id
    m_expData["flux"] = QSharedPointer<ExpData>::create("flux", 0); // 0 - dataset id

    m_expData["diffusivity"] = QSharedPointer<ExpData>::create("diffusivity", 0); // 0 - dataset id
    m_expData["modeldiffus"] = QSharedPointer<ExpData>::create("modeldiffus", 0); // 0 - dataset id
    
    m_expData["permeation"] = QSharedPointer<ExpData>::create("permeation", 0); // 0 - dataset id
    // for(int n = 0; n < rowCount(); ++n){
    //     m_exp.time.append(n+1);
    //     m_exp.flux.append(std::erf(n*4.0/rowCount()-2)+1);
    //     m_exp.permeation.append(m_exp.flux.last()/10);
    // }
}

QVariant ExpTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole){
        if(orientation == Qt::Horizontal)
            return  header.value(section,"null");
        else
            return QString("%1").arg(section);
    }
    return QVariant();
}

QSharedPointer<ExpData> ExpTable::getExpData(const QString &name){
    return m_expData[name];
}

int ExpTable::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return currentDataCount;
}

int ExpTable::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    
    return 3;//static_cast<int>(currentDataSet.count()+1);
}

QVariant ExpTable::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if(role == Qt::DisplayRole
            && index.row() >= 0 && index.row() < rowCount()
            && index.column() >= 0 && index.column() < columnCount()){
        switch(index.column()){
            case ExpTime:
                return m_expData["time"]->getValue().at(index.row());
                break;
            case Flux:
                return QString("%1").arg(m_expData["flux"]->getValue().at(index.row()), 0, 'f', 3);
                break;
            case Diffusivity:
                return QString("%1").arg(m_expData["diffusivity"]->getValue().at(index.row()), 0, 'f', 3);
                break;
            case ModelledDiffus:
                return QString("%1").arg(m_expData["modeldiffus"]->getValue().at(index.row()), 0, 'f', 3);
                break;
            case Permeation:
                return QString("%1").arg(m_expData["permeation"]->getValue().at(index.row()), 0, 'f', 3);
                break;
            default: break;
        }
        // THOSE ROLES FOR WHAT?
        //  Time,
        // Value,
        // CurTime,
        // CurValue,
    }
        //return QString("data %1-%2").arg(index.row()).arg(index.column());
    return QVariant();
}

QHash<int, QByteArray> ExpTable::roleNames() const
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

void ExpTable::appendDataExp(const QVector<qreal> &timeList){
    if(timeList.isEmpty())
        return;
    const auto &firstValue = timeList.first();
    QVector<qreal> corrTimeList;
    for(const auto &val : timeList){
        corrTimeList << val-firstValue;
    }
    m_expData["time"]->setData(corrTimeList, timeList);
}
//change to set data
void ExpTable::appendDataExp(const QVector<double> &dataList, const QString &dataName){ // not tested
    if(dataList.isEmpty())
        return;
    // m_expData: currentDataSet: "ZeroFlux", "ZeroPermeation"
    const auto &timeList = m_expData["time"]->getTime();
    m_expData[dataName]->setData(timeList, dataList);

    const QModelIndex startIndex = index(0, 0);
    const QModelIndex endIndex   = index(m_expData.count() - 1, 0);
    // current data count
    // ...but only the population field
    emit dataChanged(startIndex, endIndex, QVector<int>() << Time << Value << CurTime << CurValue );
}
