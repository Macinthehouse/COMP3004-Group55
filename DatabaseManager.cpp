#include "DatabaseManager.h"

#include <QSqlError>
#include <QDebug>

DatabaseManager::DatabaseManager()
{
}

DatabaseManager::~DatabaseManager()
{
    closeDatabase();
}

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::openDatabase(const QString& dbPath)
{
    m_lastError.clear();

    if (m_database.isOpen()) {
        return true;
    }

    if (QSqlDatabase::contains(m_connectionName)) {
        m_database = QSqlDatabase::database(m_connectionName);
    } else {
        m_database = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
    }

    if (!m_database.isValid()) {
        m_lastError = "Failed to create a valid SQLite database connection.";
        qDebug() << m_lastError;
        return false;
    }

    m_database.setDatabaseName(dbPath);

    if (!m_database.open()) {
        m_lastError = m_database.lastError().text();
        qDebug() << "Failed to open database:" << m_lastError;
        return false;
    }

    qDebug() << "Database opened successfully at:" << dbPath;
    return true;
}

void DatabaseManager::closeDatabase()
{
    if (m_database.isValid() && m_database.isOpen()) {
        m_database.close();
    }

    if (QSqlDatabase::contains(m_connectionName)) {
        m_database = QSqlDatabase();
        QSqlDatabase::removeDatabase(m_connectionName);
    }
}

bool DatabaseManager::isOpen() const
{
    return m_database.isOpen();
}

QSqlDatabase DatabaseManager::database() const
{
    return m_database;
}

QString DatabaseManager::lastError() const
{
    return m_lastError;
}
