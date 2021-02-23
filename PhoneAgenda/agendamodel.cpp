#include "agendamodel.h"

#include <QDebug>

AgendaModel::AgendaModel()
{

}

void AgendaModel::addItem(const QString &name, const QString &phoneNumber)
{
    beginResetModel();
    m_list.append({name, phoneNumber});
    endResetModel();
}

void AgendaModel::removeItem(int index)
{
    beginResetModel();
    m_list.removeAt(index);
    endResetModel();
}

void AgendaModel::clearList()
{
    beginResetModel();
    m_list.clear();
    endResetModel();
}

int AgendaModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.size();
}

QVariant AgendaModel::data(const QModelIndex &index, int role) const
{
    if(m_list.size() >= index.row())
    {
        if (role == RoleName)
        {
            return m_list[index.row()].name;
        }
        if (role == RolePhoneNumber)
        {
            return m_list[index.row()].phoneNumber;
        }
    }
    return QVariant();
}

QHash<int, QByteArray> AgendaModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(RoleName, "name");
    roles.insert(RolePhoneNumber, "phoneNumber");
    return roles;
}
