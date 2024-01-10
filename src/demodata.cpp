#include "demodata.h"
#include <QRandomGenerator>
DemoData::DemoData(QObject *parent) : QObject(parent), m_timer(new QTimer), m_points("Flow",{0},{0})
{
    connect(m_timer, &QTimer::timeout, this, &DemoData::processEvents);
}
DemoData::~DemoData(){
     m_timer->stop();
}

void DemoData::startDemo(){
    m_timer->start(1000);
    m_timePassed.start();

}
void DemoData::stopDemo(){
    m_timer->stop();
    m_timePassed.restart(); // ? 
}

double DemoData::getLastChanged(){
    return m_points.y.last();
}

void DemoData::processEvents(){
    //quint32 value = QRandomGenerator::global()->bounded(0, 1);
    const double point = sin(m_timePassed.elapsed()/1000)*2+4;
    m_points.y.append(point);
    m_points.x.append(m_timePassed.elapsed()/1000);
    emit pointsChanged(m_points.x, m_points.y);
    emit lastChanged(m_points.y.last());
}