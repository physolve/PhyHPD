#pragma once

#include <QSerialPort>
#include <QVariant>

class SettingsDialog : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap serialPortList READ serialPortListRead CONSTANT)
public:
    struct Settings {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        //QSerialPort::FlowControl flowControl;
        //QString stringFlowControl;
        //bool localEchoEnabled;
    };

    explicit SettingsDialog(QObject *parent = nullptr);
    ~SettingsDialog();

    Settings settings() const;

    QVariantMap serialPortListRead() const;

    Q_INVOKABLE void apply(QVariantMap map);

private slots:

private:
    void fillPortsInfo();
    void updateSettings();

private:
    Settings m_currentSettings;

    QVariantMap m_serialPortList; 
};