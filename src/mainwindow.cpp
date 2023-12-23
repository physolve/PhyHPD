
#include "mainwindow.h"
#include "CustomPlotItem.h"
//#include "writeregistermodel.h"
#include <QQmlContext>
#include <QtQuickControls2/QQuickStyle>
#include <QModbusRtuSerialClient>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QUrl>


MainWindow::MainWindow(int &argc, char **argv)
    : QApplication(argc, argv), m_portEdit("COM"), m_serverEdit(1), m_startAddress(1), m_readSize(10),
    m_timer(new QTimer), m_points("Flow",{0},{0})
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
    m_engine.load(url);

    setLogText("Click connect");
    // only SERIAL
    if (modbusDevice) {
        modbusDevice->disconnectDevice();
        delete modbusDevice;
        modbusDevice = nullptr;
    }

    modbusDevice = new QModbusRtuSerialClient(this);

    connect(modbusDevice, &QModbusClient::errorOccurred, [this](QModbusDevice::Error) {
        setLogText(modbusDevice->errorString()); // log under
    });

    if (!modbusDevice) {
        //ui->connectButton->setDisabled(true); //?
        setLogText("Could not create Modbus client."); // log under
    } else {
        connect(modbusDevice, &QModbusClient::stateChanged,
                this, &MainWindow::onModbusStateChanged);
    }

    connect(m_timer, &QTimer::timeout, this, &MainWindow::processEvents);
}

MainWindow::~MainWindow()
{
    m_timer->stop();
    if (modbusDevice)
        modbusDevice->disconnectDevice();
    delete modbusDevice;
}

void MainWindow::onConnectButtonClicked()
{
    if (!modbusDevice)
        return;

    setLogText(""); // log
    if (modbusDevice->state() != QModbusDevice::ConnectedState) {
       
        modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter, this->m_portEdit);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter, setObj.m_parity); // from settings
        modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, setObj.m_baud); // from settings
        modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, setObj.m_dataBits); // from settings
        modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, setObj.m_stopBits); // from settings

        modbusDevice->setTimeout(setObj.m_responseTime);
        modbusDevice->setNumberOfRetries(setObj.m_numberOfRetries);
        if (!modbusDevice->connectDevice()) {
            setLogText(tr("Connect failed: ") + modbusDevice->errorString()); // log
        } else {
            //ui->actionConnect->setEnabled(false); // ui
            //ui->actionDisconnect->setEnabled(true); // ui
        }
    } else {
        modbusDevice->disconnectDevice();
        //ui->actionConnect->setEnabled(true); // ui
        //ui->actionDisconnect->setEnabled(false); // ui
    }
    setLogText("Connected to " + m_portEdit + "\nTemperature address: 41272 (x2)\nFlow address: 41216 (x2)");
}

void MainWindow::onModbusStateChanged(int state)
{
    bool connected = (state != QModbusDevice::UnconnectedState);
    //ui->actionConnect->setEnabled(!connected); // ui
    //ui->actionDisconnect->setEnabled(connected); // ui

    //if (state == QModbusDevice::UnconnectedState)
        //ui->connectButton->setText(tr("Connect")); // ui
    //else if (state == QModbusDevice::ConnectedState)
        //ui->connectButton->setText(tr("Disconnect")); // ui
}

void MainWindow::onReadButtonClicked(bool s)
{
    if(s){
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
    if (!modbusDevice)
        return;
    m_readValue.clear();
    setLogText(""); // log

    if (auto *reply = modbusDevice->sendReadRequest(readRequest(), this->m_serverEdit)) { // ui server address
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &MainWindow::onReadReady);
        else
            delete reply; // broadcast replies return immediately
    } else {
        setLogText(tr("Read error: ") + modbusDevice->errorString()); // log
    }
}

void MainWindow::onReadReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;
    m_points.x.append(m_timePassed.elapsed()/1000);
    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        for (qsizetype i = 0, total = unit.valueCount(); i < total; ++i) {
            const QString entry = tr("Address: %1, Value: %2").arg(unit.startAddress() + i)
                                     .arg(QString::number(unit.value(i))); //, 16
            m_readValue << entry;
        }
        if(unit.valueCount() == 2){
            //QVector<quint16> mm;
            //mm.append(unit.value(0));
            //mm.append(unit.value(1));

            float x;
            unsigned long *p;

            p = (unsigned long*)&x;

            *p = (unsigned long)unit.value(0)<<16 | unit.value(1);
            qDebug() << x;
            float fiteredVal = 0;
            if(unit.startAddress() == 41216)
                fiteredVal = x*55;
            else if(unit.startAddress() == 41272)
                fiteredVal = x*1;
            else fiteredVal = x;
            m_points.y.append(fiteredVal);
        }
        else{
            m_points.y.append(unit.value(0));
        }
        
        if(unit.startAddress() == 41272){
            setLogText("Читаю температуру: " + QString::number(m_points.y.last()) + " °C");   
        }
        if(unit.startAddress() == 41216){
            setLogText("Текущий поток: " + QString::number(m_points.y.last()) + " норм. л./мин");   
        }
        
    } else if (reply->error() == QModbusDevice::ProtocolError) {
        setLogText(tr("Read response error: %1 (Modbus exception: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->rawResult().exceptionCode(), -1, 16)); // log
        m_points.y.append(0);
    } else {
        setLogText(tr("Read response error: %1 (code: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->error(), -1, 16)); // log
        m_points.y.append(0);
    }
    m_writeLog.writeLine(QString("%1\t%2").arg(m_points.x.last()).arg(m_points.y.last()));
    setData(m_readValue);
    emit pointsChanged(m_points.x,m_points.y); 
    reply->deleteLater();
}

QModbusDataUnit MainWindow::readRequest() const
{
    const auto table = QModbusDataUnit::HoldingRegisters;

    int startAddress = m_startAddress;
    //Q_ASSERT(startAddress >= 0 && startAddress < 10); // check address 10

    // do not go beyond 10 entries
    quint16 numberOfEntries = quint16(m_readSize); // check address
    return QModbusDataUnit(table, startAddress, numberOfEntries);
}

QString MainWindow::getLogText() const
{
    return logText;
}
 
void MainWindow::setLogText(QString text)
{
    if (text != logText)
    {
        logText = text;
        emit logChanged(text);
    }
}

QStringList MainWindow::data() const
{
  return m_data;
}

void MainWindow::setData(const QStringList& data)
{
  if (m_data == data)
    return;
  m_data = data;
  emit dataChanged(m_data);
}

