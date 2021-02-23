#ifndef AGENDAMODEL_H
#define AGENDAMODEL_H

#include <QAbstractListModel>
#include <QObject>

struct ContactDetails
{
    QString name;
    QString phoneNumber;
};

class AgendaModel : public QAbstractListModel
{
public:
    AgendaModel();

    enum Roles
    {
        RoleName = Qt::UserRole + 1,
        RolePhoneNumber
    };

    void addItem(const QString& name, const QString& phoneNumber);
    void removeItem(int index);
    void clearList();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

private:
    QList<ContactDetails> m_list;
};

#endif // AGENDAMODEL_H
