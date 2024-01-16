#include <cmath>
#include "exptable.h"

ExpTable::ExpTable(QObject *parent)
    : QAbstractTableModel(parent), m_exp("Exp",{0},{0},{0})
{
    header << "Time" << "Flux" << "Permeation";
    for(int n = 0; n < rowCount(); ++n){
        m_exp.time.append(n+1);
        m_exp.flux.append(std::erf(n*4.0/rowCount()-2)+1);
        m_exp.permeation.append(m_exp.flux.last()/10);
    }

}

QVariant ExpTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole){
        if(orientation == Qt::Horizontal)
            return  header.value(section,"null");
        else
            return QString("%1").arg(section);
    }
    return QVariant();
}

int ExpTable::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return 10;
}

int ExpTable::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return 3;
}

QVariant ExpTable::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if(role == Qt::DisplayRole
            && index.row() >= 0 && index.row() < rowCount()
            && index.column() >= 0 && index.column() < columnCount()){
        switch(index.column()){
            case 0:
                return m_exp.time.at(index.row());
            case 1:
                return QString("%1").arg(m_exp.flux.at(index.row()), 0, 'f', 3);
            case 2:
                return QString::number(m_exp.permeation.at(index.row()), 'e', 3);
            default: break;
        }
    }
        //return QString("data %1-%2").arg(index.row()).arg(index.column());
    return QVariant();
}

QString ExpTable::getResultStr() const
{
    const QString &text = "Time offset 22 \n Hard stop duration 65 \n Data min: 7.998e-10; Data max 1.1933e-09 \n jnif 0.2442 Time: 14; jnif 0.5 Time: 5.016; jnif 0.6266 Time: 32.016 \n Diffusion_0.2442 Result: 1.63827e-11; Diffusion_0.5 Result: 6.92229e-11; \n Diffusion_0.6266 Result: 1.30143e-11 \n Average flux: 3.13293e-05. Average permeation: 4.48801e-09 \n Average flux deviation: 0.000969569. Average permeation deviation: 1.38841e-07";
    return text;
}
