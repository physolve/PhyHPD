#pragma once

#include "settingsdialog.h"
#include "datacollection.h"
#include <QSerialPort>
#include <QElapsedTimer>
#include <QTimer>

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
    //void mySettingsChanged();
    void logChanged(QString);
protected:
    void setLogText(const QString &text);
    QSerialPort *m_serial = nullptr;
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
private slots:
    void readData() override;
private:
    void writeData();
    const double filterData_pr(double voltage);
    const double filterData_vac(double voltage);
    const QString query;
    double currentPressure;
    double currentVacuum;
    QString m_bufferData;
};