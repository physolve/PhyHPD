#pragma once

//#include <QTimer>
//#include <QElapsedTimer>
#include "controller.h"

class DemoData : public QObject
{
    Q_OBJECT
    
public:
    DemoData(QObject *parent = nullptr);
    ~DemoData();
    void startDemo();
    void stopDemo();
signals:
    void logChanged(QString);
    void pointsChanged(const QList<quint64>& x, const QList<double>& y);
    void lastChanged(double y);
private slots:
    void processEvents();
private:
    QTimer *m_timer;
    Data m_points;
    QElapsedTimer m_timePassed;
};