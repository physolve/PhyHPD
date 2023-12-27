#include "settingsdialog.h"

#include <QSerialPortInfo>

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

SettingsDialog::SettingsDialog(QObject *parent) :
    QObject(parent)
{
    fillPortsInfo();

    QVariantMap map;
    map["serialPortInfo"] = m_serialPortList.keys().first();
    map["baudRate"] = QSerialPort::Baud9600;
    map["dataBitsBox"] = QSerialPort::Data8;
    map["parityBox"] = QSerialPort::NoParity;
    map["stopBitsBox"] = QSerialPort::OneStop;
    apply(map);
}

SettingsDialog::~SettingsDialog()
{
}

SettingsDialog::Settings SettingsDialog::settings() const
{
    return m_currentSettings;
}

void SettingsDialog::fillPortsInfo()
{
    m_serialPortList.clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);
        qDebug() << info.portName();
        m_serialPortList[info.portName()] = list;
    }
}

QVariantMap SettingsDialog::serialPortListRead() const{
    return m_serialPortList;
}

void SettingsDialog::apply(const QVariantMap& map)
{
    m_currentSettings.name = map["serialPortInfo"].toString();
    qDebug() << map["baudRate"].toInt();
    m_currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(map["baudRate"].toInt());
    m_currentSettings.stringBaudRate = QString::number(m_currentSettings.baudRate);
    qDebug() << map["dataBitsBox"].toInt();
    m_currentSettings.dataBits = static_cast<QSerialPort::DataBits>(map["dataBitsBox"].toInt());
    m_currentSettings.stringDataBits = map["dataBitsBox"].toString();

    m_currentSettings.parity = static_cast<QSerialPort::Parity>(map["parityBox"].toInt());
    qDebug() << "Parity box: " << m_currentSettings.parity;
    m_currentSettings.stringParity = map["parityBox"].toString();
    m_currentSettings.stopBits = static_cast<QSerialPort::StopBits>(map["stopBitsBox"].toInt());
    qDebug() << "stop bits: " <<  m_currentSettings.stopBits;
    m_currentSettings.stringStopBits = map["stopBitsBox"].toString();

    // m_currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(map["flowControlBox"].toInt());
    // m_currentSettings.stringFlowControl = map["flowControlBox"].toString();

    // m_currentSettings.localEchoEnabled = map["localEchoCheckBox"].toBool();
}
