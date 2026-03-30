#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QString>

class DatabaseManager
{
public:
    static DatabaseManager& instance();

    bool openDatabase(const QString& dbPath);
    void closeDatabase();

    bool isOpen() const;
    QSqlDatabase database() const;
    QString lastError() const;

private:
    DatabaseManager();
    ~DatabaseManager();

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase m_database;
    QString m_lastError;
    const QString m_connectionName = "HintonMarketConnection";
};

#endif // DATABASEMANAGER_H
