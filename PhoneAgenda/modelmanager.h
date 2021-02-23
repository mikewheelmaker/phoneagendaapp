#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QObject>
#include <QFile>
#include <QAbstractItemModel>
#include "agendamodel.h"
#include "agendaproxymodel.h"
#include "databasemanager.h"
#include "networkrequest.h"

class ModelManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QAbstractListModel* agendaModel READ model CONSTANT)
    Q_PROPERTY(QAbstractItemModel* proxyModel READ proxy CONSTANT)
    Q_PROPERTY(bool isRequestStarted READ getIsRequestStarted WRITE setIsRequestStarted NOTIFY isRequestStartedChanged)
public:
    explicit ModelManager(QObject *parent = nullptr);
    ~ModelManager();

    AgendaModel* model() const;
    QAbstractItemModel* proxy() const;
    bool getIsRequestStarted() const;

    Q_INVOKABLE void addItem(const QString& name, const QString& phoneNumber);
    Q_INVOKABLE void removeItem(int index);
    Q_INVOKABLE void setProxyFilter(int filter);
    Q_INVOKABLE void sortProxy(int column, bool isAscendingOrder = true);
    Q_INVOKABLE void searchProxy(int role, const QString& string);
    Q_INVOKABLE void clearCache();
    Q_INVOKABLE void importDB();
    Q_INVOKABLE void exportDB();
    Q_INVOKABLE void refresh();

    void linkIsGetDoneToIsRequestStarted();
    void linkIsPostDoneToIsRequestStarted();

    void writeToDatabaseLog(const QString &message);
    void writeToNetworkLog(const QString &message);

signals:
    void isRequestStartedChanged();

public slots:
    void setIsRequestStarted(bool value);
    void setIsGetDone(bool newIsGetDone);
    void setIsPostDone(bool newIsPostDone);

private:
    AgendaModel* m_model;
    AgendaProxyModel* m_proxy;
    DatabaseManager* m_dbManager;
    NetworkRequest* m_networkRequest;
    bool m_isRequestStarted = true;
    QFile* m_databaseLog;
    QFile* m_networkLog;
};

#endif // MODELMANAGER_H
