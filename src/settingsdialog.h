#pragma once

#include <QSerialPort>
#include <QElapsedTimer>
#include <QVariant>

class SettingsDialog : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap serialPortList READ serialPortListRead CONSTANT)
public:
    struct Settings {
        QString name;
        QString description;
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

    Settings settings(const QString &c_name) const;

    QVariantMap serialPortListRead() const;

    Q_INVOKABLE void apply(const QVariantMap &map, const QString &c_name);

private slots:

private:
    void fillPortsInfo();
    void updateSettings();

    QMap<QString,Settings> m_SettingsMap;

    QVariantMap m_serialPortList; 
};