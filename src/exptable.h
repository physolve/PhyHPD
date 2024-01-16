#pragma once
#include <QAbstractTableModel>
struct ExpData {
    ExpData() {}
    ExpData( const QString& name, QList<quint64> time, QList<double> flux, QList<double> permeation)
        : name(name), time(time), flux(flux), permeation(permeation) {}
    void clearPoints(){
        time.clear();
        flux.clear();
    }
    QString name;
    QList<quint64> time; // one second data
    QList<double> flux; // one second data
    QList<double> permeation;
};
class ExpTable : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QString resultStr READ getResultStr NOTIFY resultChanged)
public:
    explicit ExpTable(QObject *parent = nullptr);
    Q_INVOKABLE QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QString getResultStr() const;
signals:
    void resultChanged(QString);
private:
    QString resultStr;
    QStringList header;
    ExpData m_exp;
};