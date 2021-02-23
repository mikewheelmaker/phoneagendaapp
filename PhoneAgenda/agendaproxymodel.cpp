#include "agendaproxymodel.h"
#include "agendamodel.h"

AgendaProxyModel::AgendaProxyModel()
{

}

AgendaProxyModel::AgendaProxyModel(AgendaModel *model) : QSortFilterProxyModel(), m_currentFilter(AgendaModel::RoleName)
{
    Q_UNUSED(model);
}

int AgendaProxyModel::getFilter() const
{
    return static_cast<int>(m_currentFilter);
}

void AgendaProxyModel::setFilter(int filter)
{
    auto filterType = AgendaModel::RoleName;
    if(filter == 1)
    {
        filterType = AgendaModel::RolePhoneNumber;
    }

    m_currentFilter = filterType;
    this->setFilterRole(m_currentFilter);
    emit filterChanged();
}
