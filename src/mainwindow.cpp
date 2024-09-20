
#include "mainwindow.h"
#include "CustomPlotItem.h"

#include <QQmlContext>
#include <QtQuickControls2/QQuickStyle>

MainWindow::MainWindow(int &argc, char **argv)
    : QApplication(argc, argv), m_settings(new SettingsDialog), m_writeLog(), m_logTimer(new QTimer), expCalc(), m_mnemoState()
{
    QQuickStyle::setStyle("Material");
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
    //m_engine.rootContext()->setContextProperty("table_model", &m_expTable);
    m_engine.rootContext()->setContextProperty("mnemostate", &m_mnemoState);
    m_engine.rootContext()->setContextProperty("dataModel", &m_dataModel);
    m_engine.rootContext()->setContextProperty("expCalc", &expCalc);

    initController();

    initDataSet();
    
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

void MainWindow::initDataSet(){ // I don't like this 
    auto dataSetTest = new ExpTable();
    m_engine.rootContext()->setContextProperty("table_model", dataSetTest);
    m_expDataSet["dataSetTest"] = dataSetTest;
    currentDataSet = "dataSetTest";
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
    quint64 c_time = m_programmTime.elapsed()/1000; // qreal?
    double c_pressure = 0;
    double c_vacuum = 0;
    if(!m_settings->isPressureConnected()){
        return;
    }
    auto c_map = m_pressure->getLastChanged();
    c_pressure = c_map["pressure"];
    c_vacuum = c_map["vacuum"];
    m_dataModel.appendData(c_pressure, c_vacuum);
    bool doingExp = false;
    if(doingExp){
        // c_map["time"] = m_dataModel.getLastTime(); // change to larger map with additional info or regain timed data
        
        // currently feed m_expDataSet with log data
        
        // here make all the calculations using m_dataModel data in ExpCalc then to m_expDataSet
    }
    QString line = QString("%1\t%2\t%3").arg(c_time).arg(c_pressure).arg(c_vacuum);
    m_writeLog.writeLine(line);
}

void MainWindow::calcToPlot(){
    QVector<qreal> time;
    QVector<double> diffusivity, modeledDiffus, permeation{0}, flux;
    expCalc.collectValues(time, flux, diffusivity, modeledDiffus, permeation);
    m_expDataSet[currentDataSet]->appendDataExp(time);
    m_expDataSet[currentDataSet]->appendDataExp(flux, "flux");
    m_expDataSet[currentDataSet]->appendDataExp(diffusivity, "diffusivity");
    m_expDataSet[currentDataSet]->appendDataExp(modeledDiffus, "modeldiffus");
    m_expDataSet[currentDataSet]->appendDataExp(permeation, "permeation");
}

void MainWindow::reCalculateDiffusFit(double val){ // only timeLagChanged
    QVector<double> corrModeledDiffus;
    expCalc.reCalculateDiffusFit(corrModeledDiffus, val);
    m_expDataSet[currentDataSet]->appendDataExp(corrModeledDiffus, "modeldiffus");
}

void MainWindow::reCalculateNewData(){ // anything changed
    calcToPlot();
} //test


void MainWindow::preapreExpCalc(){
    setPointsFromFile();
    calcToPlot();
}

void MainWindow::setPointsFromFile(){ // log calc function
    QFile expData("data/pseudoData.txt");
	if (!expData.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
    QTextStream in(&expData);
    in.readLine(); // skip header
    
    while (!in.atEnd()) {
        const auto &lineList = in.readLine().split('\t');
        double p_p = lineList.at(2).toDouble() * pow(10,5); //pressure, bar to Pa
        double p_s = lineList.at(3).toDouble() * pow(10,5); //vacuum, bar to Pa
        double absTemp = 273 + 653; // K
        unsigned int t = lineList.at(1).toInt();
        expCalc.addAccumulationPoint(accumulationPoint(p_p,p_s,absTemp,t));
        // unsigned int n = 120;
        // while(n > 0 && !in.atEnd()){
        //     in.readLine();
        //     --n;
        // }
    }
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
