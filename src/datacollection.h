#pragma once
#include <QString>
#include <QMap>

class DataCollection //: public QObject
{
    //Q_OBJECT
public:
    DataCollection(const QString &name);
    ~DataCollection();
    void clearPoints();
    void addPoint(const qreal &val_x, const double &val_y);
    QVector<qreal> getTime() const;
    QVector<double> getValue() const;
    qreal getCurTime() const;
    double getCurValue() const;
    QString m_name;
protected:
    virtual const double filterData(const double &voltage);
    QList<qreal> m_x; // one second data
    QList<double> m_y; // one second data

};

class ControllerData : public DataCollection
{
public:
    ControllerData(const QString &name, const double &k = 1, const double &c = 0);
private:
    const double filterData(const double &voltage) override;
    double m_k;
    double m_c;
};

class ExpData : public DataCollection
{
public:
    ExpData(const QString &name, const int &setId);
    void setData(const QVector<qreal> &val_x, const QVector<double> &val_y);
private:
    const double filterData(const double &data) override;
    unsigned int m_setId;
};