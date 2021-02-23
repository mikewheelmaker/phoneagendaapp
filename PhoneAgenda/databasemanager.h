#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QList>

struct PersonInfo {
    QString name;
    QString phoneNumber;
};

class DatabaseManager
{
public:
    enum SearchCategories {
        Name = 0,
        PhoneNumber
    };

public:
    DatabaseManager(const QString& path);
    ~DatabaseManager();

    bool createTable();
    bool insertPerson(const QString& name, const QString& phoneNumber);
    bool selectPerson(const QString& string, SearchCategories category);
    bool deletePerson(const QString& string, SearchCategories category);
    bool dropTable(const QString& tableName);
    QList<PersonInfo> getDbContent();

private:
    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
