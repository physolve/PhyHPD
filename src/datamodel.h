#pragma once

#include <QAbstractListModel>
#include <QElapsedTimer>
#include "datacollection.h"

class DataModel : public QAbstractListModel
{
    Q_OBJECT 
public:
    enum Roles {
        NameRole = Qt::UserRole,
        Time,
        Value,
        CurTime,
        CurValue,
    };

    explicit DataModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex& parent) const override;
    QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;
    //bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QHash<int, QByteArray> roleNames() const override; // use QMultiHash
    Q_INVOKABLE QSharedPointer<ControllerData> getSensor(const QString &name);

    void initializeAcquisition();
    void appendData(const double & pressureVoltage, const double & vacuumVoltage);
    double getLastTime();
signals:
    void channelMapListChanged();

private:
    QMap<QString, QSharedPointer<ControllerData>> m_trData;
    //QList<Sensor*> m_sensors; // PURPOSE BASED Hash? pointer beacause we don't know in start?
    QElapsedTimer m_time;
};