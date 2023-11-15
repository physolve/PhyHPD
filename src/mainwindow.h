#pragma once

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QModbusDataUnit>
#include <QElapsedTimer>
#include "settingsdialog.h"

class QModbusClient;
class QModbusReply;

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
    Q_PROPERTY(Settings mySettings READ getMySettings
                WRITE setMySettings NOTIFY mySettingsChanged)
    Q_PROPERTY(QString portEdit MEMBER m_portEdit)
    Q_PROPERTY(int serverEdit MEMBER m_serverEdit)
    Q_PROPERTY(QStringList  data    READ data     WRITE setData     NOTIFY dataChanged)
    Q_PROPERTY(int startAddress MEMBER m_startAddress)
    Q_PROPERTY(int readSize MEMBER m_readSize)
    Q_PROPERTY(QString logText READ getLogText WRITE setLogText NOTIFY logChanged)
    
public:
    explicit MainWindow(int &argc, char **argv);
    ~MainWindow();
    Settings setObj;

    Settings getMySettings() const
    {
        return setObj;
    }

    void setMySettings(Settings val)
    {
        setObj = val;
        emit mySettingsChanged();
    }

    Q_INVOKABLE void onConnectButtonClicked();
    Q_INVOKABLE void onReadButtonClicked(bool s);
    Q_INVOKABLE QString getLogText() const;
    QStringList data () const;
signals:
    void mySettingsChanged();
    void logChanged(QString);
    void dataChanged(const QStringList& data);
    void pointsChanged(const QList<quint64>& x, const QList<double>& y);
private:
    QModbusDataUnit readRequest() const;
    QModbusDataUnit writeRequest() const;

private slots:
    void setLogText(QString text);
    //void onConnectButtonClicked();
    void onModbusStateChanged(int state);

    //void onReadButtonClicked();
    void onReadReady();

    //void onWriteButtonClicked();
    //void onReadWriteButtonClicked();

    //void onWriteTableChanged(int);
    void setData(const QStringList& data);
    void processEvents();
private:
    QQmlApplicationEngine m_engine;
    QStringList  m_data;
    QModbusReply *lastRequest = nullptr;
    QModbusClient *modbusDevice = nullptr;
    //SettingsDialog *m_settingsDialog = nullptr;
    //WriteRegisterModel *writeModel = nullptr;
    QString m_portEdit;
    int m_serverEdit;
    QStringList m_readValue;
    int m_startAddress;
    int m_readSize;
    QString logText;

    QTimer* m_timer;
    QElapsedTimer m_timePassed;
    Data m_points;
};

