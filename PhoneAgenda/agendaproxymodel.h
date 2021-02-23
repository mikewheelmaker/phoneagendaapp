#ifndef AGENDAPROXYMODEL_H
#define AGENDAPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QObject>

#include "agendamodel.h"

class AgendaProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(int filter READ getFilter WRITE setFilter NOTIFY filterChanged)
public:
    AgendaProxyModel();
    AgendaProxyModel(AgendaModel* model);

    int getFilter() const;

signals:
    void filterChanged();

public slots:
    void setFilter(int filter);

    // QSortFilterProxyModel interface
private:
    AgendaModel::Roles m_currentFilter;
};

#endif // AGENDAPROXYMODEL_H
