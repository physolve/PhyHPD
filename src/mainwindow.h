#pragma once

#include <QApplication>
#include <QQmlApplicationEngine>

#include "settingsdialog.h"
#include "controller.h"
#include "writelogfile.h"
// #include "datamodel.h"

#include "expcalc.h"
// #include "exptable.h"

#include "chartmodel.h"

#include "mnemostate.h"

class MainWindow : public QApplication
{
    Q_OBJECT
    Q_PROPERTY(QString logText READ getLogText NOTIFY logChanged)

public:
    explicit MainWindow(int &argc, char **argv);
    ~MainWindow();
    //void setLogText(const QString &text);
    QString getLogText() const;

    Q_INVOKABLE void onReadButtonClicked(bool s);
    Q_INVOKABLE void openSerialPort();
    Q_INVOKABLE void closeSerialPort();
    Q_INVOKABLE void preapreExpCalc();

    Q_INVOKABLE void beginExp(const QString &runName, bool state);
signals:
    void logChanged(QString);

private slots:
    void setLogText(const QString &text);
    // void processEvents();
    void writeToLog();
private:

    void initController();
    void initCalc();
    void initChartModel();
    void initDataSet(); // database
    
    void feedExpValues();

    bool setPointsFromFile();
    void calcToPlot();

    WriteLogFile m_writeLog;
    // QTimer *m_logTimer;
    
    // QElapsedTimer m_programmTime;
    QString logText;
    // DataModel m_dataModel;
    
    QString currentDataSet;
    // QMap<QString, ExpTable*> m_expDataSet;
    SettingsDialog *m_settings = nullptr;
    QSharedPointer<ControllerData> timeData;
    QSharedPointer<ControllerData> pressure;
    QSharedPointer<ControllerData> vacuum;
    PressureController* m_pressure = nullptr;

    QSharedPointer<ExpData> timeExp;
    QSharedPointer<ExpData> flux;
    QSharedPointer<ExpData> diffusivity;
    QSharedPointer<ExpData> modeldiffus;
    QSharedPointer<ExpData> permeation;
    
    QStringList expNames;
    ExpCalc* expCalc;

    ChartModel* chartModel;

    MnemoState m_mnemoState;
    QQmlApplicationEngine m_engine;
};
