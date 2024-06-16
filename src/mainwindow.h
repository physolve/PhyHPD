#pragma once

#include <QApplication>
#include <QQmlApplicationEngine>

#include "settingsdialog.h"
#include "controller.h"
#include "writelogfile.h"
#include "exptable.h"
#include "mnemostate.h"

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

    SettingsDialog *m_settings = nullptr;

    PressureController* m_pressure = nullptr;

    WriteLogFile m_writeLog;
    QTimer *m_logTimer;
    
    QElapsedTimer m_programmTime;
    QString logText;

    ExpTable m_expTable;
    MnemoState m_mnemoState;
    QQmlApplicationEngine m_engine;
};