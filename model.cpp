#include "model.h"

Model::Model(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int Model::rowCount(const QModelIndex & /*parent*/) const
{
   return m_item.size();
}

int Model::columnCount(const QModelIndex & /*parent*/) const
{
    return m_item.values().first().size();
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    auto item = m_item.value(index);


    if (role == Qt::DisplayRole)
       return QString("Row%1, Column%2")
                   .arg(index.row() + 1)
                   .arg(index.column() +1);

    return QVariant();
}

void Model::loadData(QStringList list)
{

}
