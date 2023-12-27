
#include "mainwindow.h"
#include "CustomPlotItem.h"

#include <QQmlContext>
#include <QtQuickControls2/QQuickStyle>

MainWindow::MainWindow(int &argc, char **argv)
    : QApplication(argc, argv), m_settings(new SettingsDialog)
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
    
    initController();
    
    m_engine.load(url);
}

MainWindow::~MainWindow()
{
    delete m_settings;
}

void MainWindow::initController(){
    m_pressure = new PressureController(m_settings->settings("pressure"));
    connect(m_pressure, &Controller::logChanged, this, &MainWindow::logChanged);
    m_engine.rootContext()->setContextProperty("pressureBack", m_pressure);
    
    m_vacuum = new VacuumController(m_settings->settings("vacuum"));
    connect(m_vacuum, &Controller::logChanged, this, &MainWindow::logChanged);
    m_engine.rootContext()->setContextProperty("vacuumBack", m_vacuum);
}

void MainWindow::onReadButtonClicked(bool s){
    if(s){
        m_pressure->startReading();
        m_vacuum->startReading();
    }
    else{
        m_pressure->stopReading();
        m_vacuum->stopReading();
    }
}

void MainWindow::openSerialPort(){
    m_pressure->openSerialPort();
    m_vacuum->openSerialPort();
}

void MainWindow::closeSerialPort(){
    m_pressure->closeSerialPort();
    m_vacuum->closeSerialPort();
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
