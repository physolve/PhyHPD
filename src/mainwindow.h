#pragma once

#include <QApplication>
#include <QQmlApplicationEngine>

#include "settingsdialog.h"
#include "controller.h"
#include "demodata.h"
#include "writelogfile.h"

class MainWindow : public QApplication
{
    Q_OBJECT
    Q_PROPERTY(QString logText READ getLogText NOTIFY logChanged)
public:
    explicit MainWindow(int &argc, char **argv);
    ~MainWindow();
    //void setLogText(const QString &text);
    void initController();
    QString getLogText() const;

    Q_INVOKABLE void onReadButtonClicked(bool s);
    Q_INVOKABLE void openSerialPort();
    Q_INVOKABLE void closeSerialPort();
signals:
    void logChanged(QString);

private slots:
    void setLogText(const QString &text);
    void processEvents();

private:
    QQmlApplicationEngine m_engine;

    SettingsDialog *m_settings = nullptr;

    PressureController *m_pressure;

    VacuumController *m_vacuum;

    DemoData *m_demoPressure;
    DemoData *m_demoVacuum;

    WriteLogFile m_writeLog;
    QTimer *m_logTimer;
    
    QElapsedTimer m_programmTime;
    QString logText;
};