#include "modelmanager.h"
#include <QDebug>

ModelManager::ModelManager(QObject *parent) : QObject(parent), m_model(new AgendaModel), m_proxy(new AgendaProxyModel), m_dbManager(new DatabaseManager("test.db")), m_networkRequest(new NetworkRequest), m_databaseLog(new QFile("databaseLog.txt")), m_networkLog(new QFile("networkLog.txt"))
{
    m_dbManager->createTable();
    m_proxy->setSourceModel(m_model);
    linkIsGetDoneToIsRequestStarted();
    linkIsPostDoneToIsRequestStarted();

    QList<PersonInfo> dbContent = m_dbManager->getDbContent();

    if(dbContent.size() == 0)
    {
        m_model->addItem("Radu Rotariu", "0711222333");
        m_dbManager->insertPerson("Radu Rotariu", "0711222333");
        m_model->addItem("Mihai Gadea", "0711252383");
        m_dbManager->insertPerson("Mihai Gadea", "0711252383");
        m_model->addItem("Adrian Paun", "0712233388");
        m_dbManager->insertPerson("Adrian Paun", "0712233388");
        m_model->addItem("Bogdan Chitac", "0711212313");
        m_dbManager->insertPerson("Bogdan Chitac", "0711212313");
        m_model->addItem("Toni Banana", "0711227733");
        m_dbManager->insertPerson("Toni Banana", "0711227733");
    }
    else
    {
        for(int i = 0; i < dbContent.size(); ++i)
        {
            m_model->addItem(dbContent[i].name, dbContent[i].phoneNumber);
        }
    }
}

ModelManager::~ModelManager()
{
    delete m_model;
    delete m_proxy;
    delete m_dbManager;
    delete m_networkRequest;
}

AgendaModel* ModelManager::model() const
{
    return m_model;
}

QAbstractItemModel *ModelManager::proxy() const
{
    return m_proxy;
}

bool ModelManager::getIsRequestStarted() const
{
    return m_isRequestStarted;
}

void ModelManager::addItem(const QString &name, const QString &phoneNumber)
{
    if(m_dbManager->insertPerson(name, phoneNumber))
    {
        m_model->addItem(name, phoneNumber);
    }
}

void ModelManager::removeItem(int index)
{
    QModelIndex proxyIndex = m_proxy->index(index, 0);
    QModelIndex sourceIndex = m_proxy->mapToSource(proxyIndex);
    m_dbManager->deletePerson(m_model->data(sourceIndex, AgendaModel::RoleName).toString(), DatabaseManager::Name);
    m_model->removeItem(sourceIndex.row());
}

void ModelManager::setProxyFilter(int filter)
{
    this->m_proxy->setFilter(filter);
}

void ModelManager::sortProxy(int column, bool isAscendingOrder)
{
    this->m_proxy->setSortRole(Qt::UserRole + 1 + column);
    if(isAscendingOrder)
        this->m_proxy->sort(0,  Qt::AscendingOrder);
    else
        this->m_proxy->sort(0, Qt::DescendingOrder);
}

void ModelManager::searchProxy(int role, const QString &string)
{
    this->m_proxy->setFilter(role);
    this->m_proxy->setFilterFixedString(string);
}

void ModelManager::clearCache()
{
    if(m_dbManager->dropTable("contacts"))
    {
        qDebug() << "table dropped succesfully";
        m_model->clearList();
        m_dbManager->createTable();
    }
    else
    {
        qDebug() << "error at table drop";
    }
}

void ModelManager::importDB()
{
    QList<JsonFields> dataFromGet = m_networkRequest->getData();
    clearCache();
    for(auto & user : dataFromGet)
    {
        addItem(user.name, user.phoneNumber);
    }
}

void ModelManager::exportDB()
{
    QList<PersonInfo> dbContents = m_dbManager->getDbContent();
    QList<JsonFields> dataToPost;
    for(auto & user : dbContents)
    {
        dataToPost.append({user.name, user.phoneNumber});
    }
    m_networkRequest->setData(dataToPost);
    m_networkRequest->post();
}

void ModelManager::refresh()
{
    m_networkRequest->get();
}

void ModelManager::linkIsGetDoneToIsRequestStarted()
{
    connect(m_networkRequest, &NetworkRequest::isGetDone, this, &ModelManager::setIsGetDone);
}

void ModelManager::linkIsPostDoneToIsRequestStarted()
{
    connect(m_networkRequest, &NetworkRequest::isPostDone, this, &ModelManager::setIsPostDone);
}

void ModelManager::writeToDatabaseLog(const QString &message)
{
    Q_UNUSED(message);
}

void ModelManager::writeToNetworkLog(const QString &message)
{
    Q_UNUSED(message);
}

void ModelManager::setIsRequestStarted(bool value)
{
    this->m_isRequestStarted = value;
    emit isRequestStartedChanged();
}

void ModelManager::setIsGetDone(bool newIsGetDone)
{
    setIsRequestStarted(newIsGetDone);
    QLoggingCategory appNetworking("networking.log");
    if(newIsGetDone)
    {
        qCDebug(appNetworking) << "Started get function";
    }
    else
    {
        qCDebug(appNetworking) << "Finished getting data";
    }
}

void ModelManager::setIsPostDone(bool newIsPostDone)
{
    setIsRequestStarted(newIsPostDone);
    QLoggingCategory appNetworking("networking.log");
    if(newIsPostDone)
    {
        qCDebug(appNetworking) << "Started post function";
    }
    else
    {
        qCDebug(appNetworking) << "Finished posting data";
    }
}
