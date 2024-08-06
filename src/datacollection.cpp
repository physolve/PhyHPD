#include "datacollection.h"
#include <QDebug>

DataCollection::DataCollection(const QString &name) : m_name(name)
{
    m_x.append(0);
    m_y.append(0);
}

DataCollection::~DataCollection(){

}
void DataCollection::clearPoints(){
    m_x.clear();
    m_y.clear();
    m_x << 0;
    m_x << 0;

}
void DataCollection::addPoint(const qreal &val_x, const double &val_y){
    if(m_x.count() > 120){
        m_x.removeFirst();
        m_y.removeFirst();
    }
    m_x.append(val_x);
    m_y.append(val_y);        
}

const double DataCollection::filterData(const double &voltage){
    return voltage;
}

QVector<qreal> DataCollection::getTime() const {
    return m_x;
}

QVector<double> DataCollection::getValue() const {
    return m_y;
}

qreal DataCollection::getCurTime() const{
    return m_x.last();
};
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

void ExpData::setData(const QVector<qreal> &val_x, const QVector<double> &val_y){
    m_x = val_x;
    m_y = val_y;
}

const double ExpData::filterData(const double &data){
    const double point = data * 1;
    return point;
}

