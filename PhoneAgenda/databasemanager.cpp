#include "databasemanager.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDriver>
#include <QSqlRecord>

const QString CREATE_TABLE("CREATE TABLE IF NOT EXISTS %1 (%2)");

DatabaseManager::DatabaseManager(const QString &path)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    if (!m_db.open())
    {
        qDebug() << "Error: connection with database failed";
    }
    else
    {
        qDebug() << "Database: connection ok";
    }
}

DatabaseManager::~DatabaseManager()
{
    if (m_db.isOpen())
    {
        m_db.close();
    }
}

bool DatabaseManager::createTable()
{
    QSqlQuery query;
    QString sql(CREATE_TABLE.arg("contacts", "ids integer primary key, name text, phoneNumber text"));

    if (!query.exec(sql))
    {
        qDebug() << "createTable: error on running" << query.lastError();
        return false;
    }
    else
    {
        qDebug() << "createTable: success";
    }

    return true;
}

bool DatabaseManager::insertPerson(const QString &name, const QString &phoneNumber)
{
    bool success = false;

    QSqlQuery query;
    query.prepare("INSERT INTO contacts (name, phoneNumber) VALUES (:name, :phoneNumber)");
    query.bindValue(":name", name);
    query.bindValue(":phoneNumber", phoneNumber);

    if(!selectPerson(name, Name))
    {
        if (query.exec())
        {
            qDebug() << "insertPerson: person was inserted successfully";
            success = true;
        }
        else
        {
            qDebug() << "insertPerson: error" << query.lastError();
        }
    }
    else
    {
        qDebug() << "insertPerson: person already exists";
    }

    return success;
}

bool DatabaseManager::selectPerson(const QString &string, SearchCategories category)
{
    QSqlQuery query;

    switch(category)
    {
        case Name:
        query.prepare("SELECT name FROM contacts WHERE name = (:name)");
        query.bindValue(":name", string);
        break;
    case PhoneNumber:
        query.prepare("SELECT phoneNumber FROM contacts WHERE phoneNumber = (:phoneNumber)");
        query.bindValue(":phoneNumber", string);
        break;
    default:
        query.prepare("SELECT name FROM contacts WHERE name = (:name)");
        query.bindValue(":name", string);
        break;
    }

    if (!query.exec())
    {
        qDebug() << "selectPerson: error" << query.lastError();
    }
    else
    {
        if (query.first())
        {
            qDebug() << "selectPerson: contact found" << query.record();
            return true;
        }
        else
        {
            qDebug() << "selectPerson: no such contact found";
        }
    }

    return false;
}

bool DatabaseManager::deletePerson(const QString &string, SearchCategories category)
{
    QSqlQuery query;

    switch(category)
    {
        case Name:
        query.prepare("DELETE FROM contacts WHERE name = (:name)");
        query.bindValue(":name", string);
        break;
    case PhoneNumber:
        query.prepare("DELETE FROM contacts WHERE phoneNumber = (:phoneNumber)");
        query.bindValue(":phoneNumber", string);
        break;
    default:
        query.prepare("DELETE FROM contacts WHERE name = (:name)");
        query.bindValue(":name", string);
        break;
    }

    if (query.exec())
    {
        qDebug() << "deletePerson: person successfully removed";
        return true;
    }
    else
    {
        qDebug() << "deletePerson: error" << query.lastError();
        return false;
    }
}

bool DatabaseManager::dropTable(const QString &tableName)
{
    QSqlQuery query;
    Q_UNUSED(tableName);
    query.prepare("DROP TABLE contacts");
    //query.prepare("DROP TABLE :tableName");
    //query.bindValue(":tableName", tableName);

    if(query.exec())
    {
        qDebug() << "dropTable: successfully dropped table";
        return true;
    }
    else
    {
        qDebug() << "dropTable: error" << query.lastError();
        return false;
    }
}

QList<PersonInfo> DatabaseManager::getDbContent()
{
    QList<PersonInfo> result;
    QSqlQuery query;

    query.prepare("SELECT * FROM contacts");

    if(query.exec())
    {
        qDebug() << "getDbContent: success";
        while(query.next())
        {
            result.append({query.value(1).toString(), query.value(2).toString()});
        }
    }
    else
    {
        qDebug() << "getDbContent: failed; " << query.lastError();
    }

    return result;
}
