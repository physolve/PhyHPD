#include "controller.h"

Controller::Controller(const SettingsDialog::Settings &settings, QObject *parent) : 
    QObject(parent), m_settings(settings), m_timer(new QTimer), m_serial(new QSerialPort(this))
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
    const QString query = "#01\r"; 
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
}
void Controller::stopReading(){
    m_timer->stop();
}

void Controller::processEvents(){
    //setLogText("");
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

PressureController::PressureController(const SettingsDialog::Settings &settings, QList<QSharedPointer<ControllerData>> dataStorage, QObject *parent) : 
    Controller(settings,parent), query("#01\r")//, currentPressure(0), currentVacuum(0)
{
    m_programmTime.start();
    timeData = dataStorage[0];
    pressure = dataStorage[1];
    vacuum = dataStorage[2];
}
// #010\r to read only first channel, #000\r to read all channels, but what is syntax? 
void PressureController::writeData(){
    m_serial->write(query.toLocal8Bit());
}

void PressureController::readData(){
    const QByteArray data = m_serial->readAll();
    //data format "+00.000\r" For ONE channel
    //data format: +000.00+000.00+000.00...+000.00\r

    // QString responce = QString::fromLocal8Bit(data);
    // responce.remove(0,1);
    // responce.chop(1);

    QString responce = QString::fromLocal8Bit(data);
    if(!responce.endsWith('\r')){
        m_bufferData = responce;
        return;
    }
    responce = m_bufferData + responce;
    responce.remove(0,1);
    responce.chop(1);
    QStringList channelsVoltage = responce.split('+', Qt::SkipEmptyParts);
    //qDebug() << channelsVoltage;
    bool ok = true;
    auto voltagePressure = channelsVoltage.at(0).toDouble(&ok);
    auto voltageVacuum = channelsVoltage.at(1).toDouble(&ok);
    auto point_pr = 0.0;
    if(voltagePressure != 0 && ok){
        point_pr = filterData_pr(voltagePressure);
        threshold = 0;
    }
    else{
        point_pr = 0.0;
        if(++threshold>3){
            shuttingOff();
        }
    }
    // currentPressure = point_pr;
    auto point_vac = 0.0;
    if(voltageVacuum != 0 && ok){
        point_vac = filterData_vac(voltageVacuum);
        threshold = 0;
    }
    else{
        point_vac = 0.0;
        if(++threshold>3){
            shuttingOff();
        }
    }
    // currentVacuum = point_vac;

    const auto &c_time = m_programmTime.elapsed()/1000;
    timeData->addPoint(c_time);
    pressure->addPoint(point_pr);
    // emit pressureValChanged();
    vacuum->addPoint(point_vac);
    // emit vacuumValChanged();
    emit pressureChanged();
}

const double PressureController::filterData_pr(double voltage){
    double point = 2.6046 * voltage - 2.4978;
    return point;
}

const double PressureController::filterData_vac(double voltage){
    double point = 0.1303 * voltage - 0.1253;
    return point;
}

// QMap<QString, double> PressureController::getLastChanged(){
//     QMap<QString, double> points;
//     points["pressure"] = currentPressure;
//     points["vacuum"] = currentVacuum;
//     return points;
// }

void PressureController::stopReading(){
    m_timer->stop();
    // currentPressure = 0;
    // currentVacuum = 0;
}

double PressureController::pressureVal() const{
    return pressure->getCurValue();
}
double PressureController::vacuumVal() const{
    return vacuum->getCurValue();
}