#pragma once

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QSerialPort>
#include <QElapsedTimer>
#include "settingsdialog.h"
#include "writelogfile.h"

class SettingsDialog;

struct Data {
    Data() {}
    Data( const QString& name, QList<quint64> x, QList<double> y)
        : name(name), x(x), y(y) {}
    void clearPoints(){
        x.clear();
        y.clear();
    }
    QString name;
    QList<quint64> x; // one second data
    QList<double> y; // one second data
};

class MainWindow : public QApplication
{
    Q_OBJECT
    
    Q_PROPERTY(QString logText READ getLogText NOTIFY logChanged)
    
public:
    explicit MainWindow(int &argc, char **argv);
    ~MainWindow();

    Q_INVOKABLE void onReadButtonClicked(bool s);
    Q_INVOKABLE void openSerialPort();
    Q_INVOKABLE void closeSerialPort();

    QString getLogText() const;

signals:
    void mySettingsChanged();
    void logChanged(QString);
    void dataChanged(const QStringList& data);
    void pointsChanged(const QList<quint64>& x, const QList<double>& y);


private slots:
    void readData();
    void handleError(QSerialPort::SerialPortError error);
    void processEvents();

private:
    void setLogText(const QString &text);
    void writeData(const QByteArray &data);

    SettingsDialog *m_settings = nullptr;
    QSerialPort *m_serial = nullptr;

    QQmlApplicationEngine m_engine;

    QString logText;

    QTimer* m_timer;
    QElapsedTimer m_timePassed;
    Data m_points;

    WriteLogFile m_writeLog;
};