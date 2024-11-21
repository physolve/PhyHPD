#include "mainwindow.h"
#include "CustomPlotItem.h"

#include <QQmlContext>
#include <QtQuickControls2/QQuickStyle>

MainWindow::MainWindow(int &argc, char **argv)
    : QApplication(argc, argv), m_settings(new SettingsDialog), m_writeLog(), m_mnemoState() //m_logTimer(new QTimer),
{
    QQuickStyle::setStyle("Material");
    // QQuickStyle::setStyle("Fusion");
    QString applicationName = "MHgrph";
    
    m_engine.addImportPath(":/");

    const QUrl url(QString("qrc:/%1/qml/main.qml").arg(applicationName));
    QObject::connect(
                &m_engine, &QQmlApplicationEngine::objectCreated, this,
                [url](QObject *obj, const QUrl &objUrl) {
                    if(!obj && url == objUrl) QCoreApplication::exit(-1);
    },
    Qt::QueuedConnection);
    qmlRegisterType<CustomPlotItem>("CustomPlot", 1, 0, "CustomPlotItem");
    m_engine.rootContext()->setContextProperty("backend", this);
    m_engine.rootContext()->setContextProperty("settingsDialog", m_settings);
    m_engine.rootContext()->setContextProperty("mnemostate", &m_mnemoState);
    // m_engine.rootContext()->setContextProperty("dataModel", &m_dataModel);
    initController();

    initCalc();

    initChartModel();
    //initDataSet(); will be database
    m_engine.load(url);

    //connect(m_logTimer, &QTimer::timeout, this, &MainWindow::processEvents);

    //m_programmTime.start();
}

MainWindow::~MainWindow()
{
    // m_logTimer->stop();
    // delete m_logTimer;
    delete m_settings;
    // other deletes like
    delete m_pressure;
    delete expCalc;
}

void MainWindow::initController(){
    timeData = QSharedPointer<ControllerData>::create("timeData", 1, 0); // as base class ok?
    pressure = QSharedPointer<ControllerData>::create("pressure", 2.6046, -2.4978);
    vacuum = QSharedPointer<ControllerData>::create("vacuum", 0.1303, -0.1253);
    m_engine.rootContext()->setContextProperty("pressureBack", nullptr);
    if(m_settings->isPressureConnected()){ // ?
        m_pressure = new PressureController(m_settings->settings("pressure"), {timeData, pressure, vacuum});
        connect(m_pressure, &Controller::logChanged, this, &MainWindow::logChanged);
        connect(m_pressure, &PressureController::pressureChanged, this, &MainWindow::writeToLog);
        m_engine.rootContext()->setContextProperty("pressureBack", m_pressure);
    }
}

void MainWindow::initCalc(){
    timeExp = QSharedPointer<ExpData>::create("time", 0); // 0 - dataset id
    flux = QSharedPointer<ExpData>::create("flux", 0); // 0 - dataset id
    diffusivity = QSharedPointer<ExpData>::create("diffusivity", 0); // 0 - dataset id
    modeldiffus = QSharedPointer<ExpData>::create("modeldiffus", 0); // 0 - dataset id
    permeation = QSharedPointer<ExpData>::create("permeation", 0); // 0 - dataset id
    expCalc = new ExpCalc({timeData, pressure, vacuum}, {timeExp, flux, diffusivity, modeldiffus, permeation});
    m_engine.rootContext()->setContextProperty("expCalc", expCalc);
}

void MainWindow::initChartModel(){
    chartModel = new ChartModel({timeData, pressure, vacuum}, {timeExp, flux, diffusivity, modeldiffus, permeation});
    m_engine.rootContext()->setContextProperty("chartModel", chartModel);
}

void MainWindow::initDataSet(){ // I don't like this 
    // auto dataSetTest = new ExpTable();
    // m_engine.rootContext()->setContextProperty("table_model", dataSetTest);
    //m_expDataSet["dataSetTest"] = dataSetTest;
    // currentDataSet = "dataSetTest";
}

void MainWindow::onReadButtonClicked(bool s){
    if(!m_settings->isPressureConnected()){
        return;
    }
    if(s){
        m_pressure->startReading();
        // m_logTimer->start(1000);
    }
    else{
        m_pressure->stopReading();
        // m_logTimer->stop();
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

// void MainWindow::processEvents(){
//     quint64 c_time = m_programmTime.elapsed()/1000; // qreal?
//     double c_pressure = 0;
//     double c_vacuum = 0;
//     if(!m_settings->isPressureConnected()){
//         return;
//     }
//     auto c_map = m_pressure->getLastChanged();
//     c_pressure = c_map["pressure"];
//     c_vacuum = c_map["vacuum"];
//     // m_dataModel.appendData(c_pressure, c_vacuum);
//     bool doingExp = false;
//     if(doingExp){
//         // c_map["time"] = m_dataModel.getLastTime(); // change to larger map with additional info or regain timed data     
//         // currently feed m_expDataSet with log data     
//         // here make all the calculations using m_dataModel data in ExpCalc then to m_expDataSet
//     }
//     QString line = QString("%1\t%2\t%3").arg(c_time).arg(c_pressure).arg(c_vacuum);
//     m_writeLog.writeLine(line);
// }

void MainWindow::beginExp(const QString &runName, bool state){
    expNames << runName;
    expCalc->startExpTime(state);
}

void MainWindow::writeToLog(){
    QString line = QString("%1\t%2\t%3").arg(timeData->getCurValue()).arg(pressure->getCurValue()).arg(vacuum->getCurValue());
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
