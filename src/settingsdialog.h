#pragma once

#include <QtSerialBus/qtserialbusglobal.h>
#include <QSerialPort>

struct Settings {
    Q_GADGET
public:
    int m_parity = QSerialPort::EvenParity;
    int m_baud = QSerialPort::Baud19200;
    int m_dataBits = QSerialPort::Data8;
    int m_stopBits = QSerialPort::OneStop;
    int m_responseTime = 1000;
    int m_numberOfRetries = 3;
    Q_PROPERTY(int parity MEMBER m_parity)
    Q_PROPERTY(int baud MEMBER m_baud)
    Q_PROPERTY(int dataBits MEMBER m_dataBits)
    Q_PROPERTY(int stopBits MEMBER m_stopBits)
    Q_PROPERTY(int responseTime MEMBER m_responseTime)
    Q_PROPERTY(int numberOfRetries MEMBER m_numberOfRetries)
};
