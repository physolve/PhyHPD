#include "datacollection.h"
#include <QDebug>

DataCollection::DataCollection(const QString &name) : m_name(name)
{
    m_y.append(0);
}

DataCollection::~DataCollection(){

}
void DataCollection::clearPoints(){
    m_y.clear();
    m_y << 0;

}
void DataCollection::addPoint(const double &val_y){
    m_y.append(val_y);        
}

const double DataCollection::filterData(const double &voltage){
    return voltage;
}

QVector<double> DataCollection::getValue() const {
    return m_y;
}

QVector<double> DataCollection::getLastToChart() const {
    if(m_y.count() > 120)
        return m_y.last(120);
    return m_y;
}

double DataCollection::getCurValue() const{
    return m_y.last();
}


ControllerData::ControllerData(const QString &name, const double &k, const double &c) : DataCollection(name), m_k(k), m_c(c)
{
}

const double ControllerData::filterData(const double &voltage){
    const double point = m_k * voltage + m_c;
    return point;
}


ExpData::ExpData(const QString &name, const int &setId) : DataCollection(name), m_setId(setId)
{
}

void ExpData::setData(const QVector<double> &val_y){
    m_y = val_y;
}

const double ExpData::filterData(const double &data){
    const double point = data * 1;
    return point;
}

