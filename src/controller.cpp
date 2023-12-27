#include "controller.h"

Controller::Controller(const SettingsDialog::Settings &settings, QObject *parent) : 
    QObject(parent), m_settings(settings), m_timer(new QTimer), m_points("Flow",{0},{0}),
    m_serial(new QSerialPort(this))
{
    connect(m_serial, &QSerialPort::errorOccurred, this, &Controller::handleError);

    connect(m_serial, &QSerialPort::readyRead, this, &Controller::readData);

    connect(m_timer, &QTimer::timeout, this, &Controller::processEvents);

    setLogText("Click connect");
}
Controller::~Controller(){
    m_timer->stop();
}
void Controller::openSerialPort()
{
    //const SettingsDialog::Settings p = m_settings->settings();
    m_serial->setPortName(m_settings.name);
    m_serial->setBaudRate(m_settings.baudRate);
    m_serial->setDataBits(m_settings.dataBits);
    m_serial->setParity(m_settings.parity);
    m_serial->setStopBits(m_settings.stopBits);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    if (m_serial->open(QIODevice::ReadWrite)) {
        //m_console->setEnabled(true);
        //m_console->setLocalEchoEnabled(p.localEchoEnabled);
        //m_ui->actionConnect->setEnabled(false);
        //m_ui->actionDisconnect->setEnabled(true);
        //m_ui->actionConfigure->setEnabled(false);
        setLogText(tr("Connected to %1 : %2, %3, %4, %5")
                          .arg(m_settings.name).arg(m_settings.stringBaudRate).arg(m_settings.stringDataBits)
                          .arg(m_settings.stringParity).arg(m_settings.stringStopBits));
    } else {
        //QMessageBox::critical(this, tr("Error"), m_serial->errorString());

        setLogText(tr("Open error"));
    }
}

void Controller::closeSerialPort()
{
    if (m_serial->isOpen())
        m_serial->close();
    //m_console->setEnabled(false);
    //m_ui->actionConnect->setEnabled(true);
    //m_ui->actionDisconnect->setEnabled(false);
    //m_ui->actionConfigure->setEnabled(true);
    setLogText(tr("Disconnected"));
}

void Controller::writeData()
{
    const QString query = "#011\r"; 
    m_serial->write(query.toLocal8Bit());
}

void Controller::readData()
{
    const QByteArray data = m_serial->readAll();
    qDebug() << "BASE CLASS" << QString::fromLocal8Bit(data);
}

void Controller::shuttingOff(){
    qDebug() << "Wrong data in sensor!";
    stopReading();
    //emit to qml status about error   
}

void Controller::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        //QMessageBox::critical(this, tr("Critical Error"), m_serial->errorString());
        closeSerialPort();
    }
}

void Controller::startReading()
{
    threshold = 0;
    m_timer->start(1000);
    m_timePassed.start();
}
void Controller::stopReading(){
    m_timer->stop();
    m_timePassed.restart();
    m_points.clearPoints();
}

void Controller::processEvents(){
    setLogText("");

    writeData();
}

void Controller::setLogText(const QString &text)
{
    if (text != logText)
    {
        logText = text;
        emit logChanged(text);
    }
}

PressureController::PressureController(const SettingsDialog::Settings &settings, QObject *parent) : 
    Controller(settings,parent), query("#011\r")
{
}

void PressureController::writeData(){
    m_serial->write(query.toLocal8Bit());
}

void PressureController::readData(){
    const QByteArray data = m_serial->readAll();
    //data format "+00.000\r"
    QString responce = QString::fromLocal8Bit(data);
    responce.remove(0,1);
    responce.chop(1);
    bool ok = true;
    auto result = responce.toDouble(&ok);
    if(result != 0 && ok)
        m_points.y.append(result);
    else{
        m_points.y.append(0);
        if(++threshold>3){
            shuttingOff();
        }
    }
    m_points.x.append(m_timePassed.elapsed()/1000);
    emit pointsChanged(m_points.x, m_points.y);
}

VacuumController::VacuumController(const SettingsDialog::Settings &settings, QObject *parent) : 
    Controller(settings,parent), query("001M^\r")
{
}

void VacuumController::writeData(){
    m_serial->write(query.toLocal8Bit());
}

void VacuumController::readData(){
    const QByteArray data = m_serial->readAll();
    //data format 001M100023D -> 1.000Ex (x = 23-20 = 3)
    QString responce = QString::fromLocal8Bit(data);
    responce.remove(0,4);
    responce.chop(1);
    bool ok = true;
    double result = responce.first(4).toDouble(&ok)/1000.0;
    int mantissa = responce.last(2).toInt();
    if(result != 0 && ok)
        m_points.y.append(result*pow(10,mantissa));
    else{
        m_points.y.append(0);
        if(++threshold>3){
            shuttingOff();
        }
    }
    m_points.x.append(m_timePassed.elapsed()/1000);
    emit pointsChanged(m_points.x, m_points.y);
}