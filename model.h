#include <QAbstractTableModel>

class Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    Model(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void loadData(QStringList list);

private:
    QHash<const QModelIndex, QStringList> m_item;

};
