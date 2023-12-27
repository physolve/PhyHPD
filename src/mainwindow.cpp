
#include "mainwindow.h"
#include "CustomPlotItem.h"

#include <QQmlContext>
#include <QtQuickControls2/QQuickStyle>

MainWindow::MainWindow(int &argc, char **argv)
    : QApplication(argc, argv), m_timer(new QTimer), m_points("Flow",{0},{0}), m_settings(new SettingsDialog),
    m_serial(new QSerialPort(this))
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
    m_engine.load(url);

    setLogText("Click connect");

    connect(m_serial, &QSerialPort::errorOccurred, this, &MainWindow::handleError);

    connect(m_serial, &QSerialPort::readyRead, this, &MainWindow::readData);

    connect(m_timer, &QTimer::timeout, this, &MainWindow::processEvents);
}

MainWindow::~MainWindow()
{
    m_timer->stop();
    delete m_settings;
}

void MainWindow::openSerialPort()
{
    const SettingsDialog::Settings p = m_settings->settings();
    m_serial->setPortName(p.name);
    m_serial->setBaudRate(p.baudRate);
    m_serial->setDataBits(p.dataBits);
    m_serial->setParity(p.parity);
    m_serial->setStopBits(p.stopBits);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    if (m_serial->open(QIODevice::ReadWrite)) {
        //m_console->setEnabled(true);
        //m_console->setLocalEchoEnabled(p.localEchoEnabled);
        //m_ui->actionConnect->setEnabled(false);
        //m_ui->actionDisconnect->setEnabled(true);
        //m_ui->actionConfigure->setEnabled(false);
        setLogText(tr("Connected to %1 : %2, %3, %4, %5")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits));
    } else {
        //QMessageBox::critical(this, tr("Error"), m_serial->errorString());

        setLogText(tr("Open error"));
    }
}

void MainWindow::closeSerialPort()
{
    if (m_serial->isOpen())
        m_serial->close();
    //m_console->setEnabled(false);
    //m_ui->actionConnect->setEnabled(true);
    //m_ui->actionDisconnect->setEnabled(false);
    //m_ui->actionConfigure->setEnabled(true);
    setLogText(tr("Disconnected"));
}

void MainWindow::writeData(const QByteArray &data)
{
    m_serial->write(data);
}

void MainWindow::readData()
{
    const QByteArray data = m_serial->readAll();
    qDebug() << QString::fromLocal8Bit(data);
    //data format "+00.000\r"
    // QString responce = QString::fromLocal8Bit(data);
    // responce.remove(0,1);
    // responce.chop(1);
    // bool ok = true;
    // auto result = responce.toDouble(&ok);
    // if(result != 0 && ok)
    //     m_points.y.append(result);
    // else{
    //     m_points.y.append(0);
    //     if(++threshold>3){
    //         shuttingOff();
    //     }
    // }
    // m_points.x.append(m_timePassed.elapsed()/1000);
    // emit pointsChanged(m_points.x, m_points.y);
}

void MainWindow::shuttingOff(){
    qDebug() << "Wrong data in sensor!";
    onReadButtonClicked(false);
    //emit to qml status about error   
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        //QMessageBox::critical(this, tr("Critical Error"), m_serial->errorString());
        closeSerialPort();
    }
}

void MainWindow::onReadButtonClicked(bool s)
{
    if(s){
        threshold = 0;
        m_timer->start(1000);
        m_timePassed.start();
    }
    else{
        m_timer->stop();
        m_timePassed.restart();
        m_points.clearPoints();
    }
}

void MainWindow::processEvents(){
    
    // if (!modbusDevice)
    //     return;
    
    // m_readValue.clear();
    const QString query = "001M^\r";//"#011\r";

    setLogText(""); // log

    writeData(query.toLocal8Bit());
    // if (auto *reply = modbusDevice->sendReadRequest(readRequest(), this->m_serverEdit)) { // ui server address
    //     if (!reply->isFinished())
    //         connect(reply, &QModbusReply::finished, this, &MainWindow::onReadReady);
    //     else
    //         delete reply; // broadcast replies return immediately
    // } else {
    //     setLogText(tr("Read error: ") + modbusDevice->errorString()); // log
    // }

}

QString MainWindow::getLogText() const
{
    return logText;
}
 
void MainWindow::setLogText(const QString &text)
{
    if (text != logText)
    {
        logText = text;
        emit logChanged(text);
    }
}
