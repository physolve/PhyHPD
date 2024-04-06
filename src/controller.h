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
    //void dataChanged(const QStringList& data);
    void pointsChanged(const QList<quint64>& x, const QList<double>& y);
    void lastChanged(double y);
protected:
    void setLogText(const QString &text);
    QSerialPort *m_serial = nullptr;
    QElapsedTimer m_timePassed;
    uint8_t threshold;
private:
    virtual void writeData() ;
    SettingsDialog::Settings m_settings;
    QTimer* m_timer;
    QString logText;
};

class PressureController : public Controller
{
    Q_OBJECT
public:
    PressureController(const SettingsDialog::Settings &settings, QObject *parent = nullptr);
    QMap<QString,double> getLastChanged();
private slots:
    void readData() override;
private:
    void writeData();
    const double filterData(double voltage);
    const QString query;
    Data m_pressure;
    Data m_vacuum;
};