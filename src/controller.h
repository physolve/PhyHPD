#pragma once

#include "settingsdialog.h"
#include <QSerialPort>
#include <QElapsedTimer>
#include <QTimer>

class SettingsDialog;

struct Data {
    Data() {}
    Data( const QString& name, QList<quint64> x, QList<double> y)
        : name(name), x(x), y(y) {}
    void clearPoints(){
        x.clear();
        y.clear();
    }
    void addPoint(const quint64 &val_x, const double &val_y){
        if(x.count() > 120){
            x.removeFirst();
            y.removeFirst();
        }
        x.append(val_x);
        y.append(val_y);        
    }
    QString name;
    QList<quint64> x; // one second data
    QList<double> y; // one second data
};

class Controller : public QObject
{
    Q_OBJECT
    
public:
    Controller(const SettingsDialog::Settings &settings, QObject *parent = nullptr);
    virtual ~Controller();
    void shuttingOff();
    void startReading();
    void stopReading();
    void openSerialPort();
    void closeSerialPort();
    
private slots:
    virtual void readData();
    void handleError(QSerialPort::SerialPortError error);
    void processEvents();

signals:
    void mySettingsChanged();
    void logChanged(QString);
protected:
    void setLogText(const QString &text);
    QSerialPort *m_serial = nullptr;
    QElapsedTimer m_timePassed;
    QTimer* m_timer;
    uint8_t threshold;
private:
    virtual void writeData() ;
    SettingsDialog::Settings m_settings;
    QString logText;
};

class PressureController : public Controller
{
    Q_OBJECT
public:
    PressureController(const SettingsDialog::Settings &settings, QObject *parent = nullptr);
    QMap<QString,double> getLastChanged();
    void stopReading();
signals:
    void pointsPressureChanged(const QList<quint64>& x, const QList<double>& y);
    void pointsVacuumChanged(const QList<quint64>& x, const QList<double>& y);
    void lastPressureChanged(double y);
    void lastVacuumChanged(double y);
private slots:
    void readData() override;
private:
    void writeData();
    const double filterData_pr(double voltage);
    const double filterData_vac(double voltage);
    const QString query;
    Data m_pressure;
    Data m_vacuum;
    QString m_bufferData;
};