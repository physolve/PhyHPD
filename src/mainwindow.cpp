
#include "mainwindow.h"
#include "CustomPlotItem.h"

#include <QQmlContext>
#include <QtQuickControls2/QQuickStyle>

MainWindow::MainWindow(int &argc, char **argv)
    : QApplication(argc, argv), m_settings(new SettingsDialog), m_writeLog(), m_logTimer(new QTimer), m_expTable(), m_mnemoState()
{
    QQuickStyle::setStyle("Material");
    QString applicationName = "MHgrph";
    
    m_engine.addImportPath(":/");

    qmlRegisterType<CustomPlotItem>("CustomPlot", 1, 0, "CustomPlotItem");

    const QUrl url(QString("qrc:/%1/qml/main.qml").arg(applicationName));
    QObject::connect(
                &m_engine, &QQmlApplicationEngine::objectCreated, this,
                [url](QObject *obj, const QUrl &objUrl) {
                    if(!obj && url == objUrl) QCoreApplication::exit(-1);
    },
    Qt::QueuedConnection);
    m_engine.rootContext()->setContextProperty("backend", this);
    m_engine.rootContext()->setContextProperty("settingsDialog", m_settings);
    m_engine.rootContext()->setContextProperty("table_model", &m_expTable);
    m_engine.rootContext()->setContextProperty("mnemostate", &m_mnemoState);
    
    initController();
    
    m_engine.load(url);

    connect(m_logTimer, &QTimer::timeout, this, &MainWindow::processEvents);

    m_programmTime.start();
}

MainWindow::~MainWindow()
{
    m_logTimer->stop();
    delete m_logTimer;
    delete m_settings;
}

void MainWindow::initController(){
    if(m_settings->isPressureConnected()){
        m_pressure = new PressureController(m_settings->settings("pressure"));
        connect(m_pressure, &Controller::logChanged, this, &MainWindow::logChanged);
        m_engine.rootContext()->setContextProperty("pressureBack", m_pressure);
    }
}

void MainWindow::onReadButtonClicked(bool s){
    if(!m_settings->isPressureConnected()){
        return;
    }
    if(s){
        m_pressure->startReading();
        m_logTimer->start(1000);
    }
    else{
        m_pressure->stopReading();
        m_logTimer->stop();
    }
    
}

void MainWindow::openSerialPort(){
    if(!m_settings->isPressureConnected())
        return;
    m_pressure->openSerialPort();
}

void MainWindow::closeSerialPort(){
    if(!m_settings->isPressureConnected())
        return;
    m_pressure->closeSerialPort();
}

void MainWindow::processEvents(){
    quint64 c_time = m_programmTime.elapsed()/1000;
    double c_pressure = 0;
    double c_vacuum = 0;
    if(!m_settings->isPressureConnected()){
        return;
    }
    
    auto c_map = m_pressure->getLastChanged();
    c_pressure = c_map["pressure"];
    c_vacuum = c_map["vacuum"];
    
    QString line = QString("%1\t%2\t%3").arg(c_time).arg(c_pressure).arg(c_vacuum);
    m_writeLog.writeLine(line);
}

void MainWindow::setLogText(const QString &text)
{
    if (text != logText)
    {
        logText = text;
        emit logChanged(text);
    }
}
QString MainWindow::getLogText() const
{
    return logText;
}
