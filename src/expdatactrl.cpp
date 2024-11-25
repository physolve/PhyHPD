#include "expdatactrl.h"
#include <math.h>
#include <QFile>
#include <QDate>

ExpDataCtrl::ExpDataCtrl(QList<QSharedPointer<ControllerData>> dataStorage, QObject *parent) : QObject(parent), m_timer(new QTimer),
currentExpCtrl{0, 1}
{
    timeData = dataStorage[0];
    pressure = dataStorage[1];
    vacuum = dataStorage[2];

    // timeExp = expStorage[0];
    // fluxExp = expStorage[1];
    // diffusivityExp = expStorage[2];
    // modeldiffusExp = expStorage[3];
    // permeationExp = expStorage[4];

    setTimerInterval(1);

    connect(m_timer, &QTimer::timeout, this, &ExpDataCtrl::processEvents);
}

ExpDataCtrl::~ExpDataCtrl(){
    if(m_timer->isActive()) 
        m_timer->stop();
    expDataFile.close();

    delete m_timer;
}

void ExpDataCtrl::setTimerInterval(int interval){ // arg in seconds
    currentExpCtrl.m_writeDataInterval = interval;
    m_timer->setInterval(interval * 1000);
}

void ExpDataCtrl::fileCreation(const QString &expName){
    expDataFile.setFileName(QString("data/%1.txt").arg(expName));
    if (!expDataFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&expDataFile);
    out << "Time\t" << "Elapsed\t" << "Pressure\t" << "Vacuum\n";
}

void ExpDataCtrl::startExpCtrl(double relativeTimeStart){
    currentExpCtrl.m_relativeTimeStart = relativeTimeStart;

    m_timer->start();
}

void ExpDataCtrl::processEvents(){
    // write to file ASAP
    const auto &elapsed = timeData->getCurValue() - currentExpCtrl.m_relativeTimeStart; // 0?
    //process timeData

    QString line = QString("%1\t%2\t%3").arg(elapsed).arg(pressure->getCurValue()).arg(vacuum->getCurValue()); // time data relative!
    QTextStream out(&expDataFile);
    out << QTime::currentTime().toString() << "\t";
    out << line << "\n";
}

void ExpDataCtrl::endExpCtrl(){
    m_timer->stop();
    expDataFile.close();
}

QString ExpDataCtrl::getExpFilePath() const{
    return expDataFile.fileName();
}