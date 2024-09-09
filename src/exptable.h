#pragma once
#include <QAbstractTableModel>
#include <QElapsedTimer>
#include "datacollection.h"
class ExpTable : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QString resultStr READ getResultStr NOTIFY resultChanged)
public:
    enum Column {
        ExpTime,
        Flux,
        Diffusivity,
        ModelledDiffus,
        Permeation
    };
    enum TableRole {
        NameRole = Qt::UserRole,
        Time,
        Value,
        CurTime,
        CurValue,
    };
    explicit ExpTable(QObject *parent = nullptr);
    Q_INVOKABLE QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE QSharedPointer<ExpData> getExpData(const QString &name);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    
    QHash<int, QByteArray> roleNames() const override;
    
    QString getResultStr() const;

    void appendDataExp(const QVector<qreal> &timeList);
    void appendDataExp(const QVector<double> &dataList, const QString &dataName);
signals:
    void resultChanged(QString);
private:
    QString resultStr;
    QStringList header;
    unsigned int currentDataCount;
    QMap<QString, QSharedPointer<ExpData>> m_expData;
};