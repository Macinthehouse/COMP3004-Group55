#include "NotificationRepository.h"
#include "DatabaseManager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

std::vector<Notification> NotificationRepository::loadNotificationsForVendor(const std::string& vendorId)
{
    std::vector<Notification> notifications;

    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT message "
        "FROM notifications "
        "WHERE vendor_id = :vendor_id "
        "ORDER BY id"
    );
    query.bindValue(":vendor_id", QString::fromStdString(vendorId));

    if (!query.exec()) {
        qDebug() << "NotificationRepository::loadNotificationsForVendor failed:"
                 << query.lastError().text();
        return notifications;
    }

    while (query.next()) {
        notifications.emplace_back(
            query.value("message").toString().toStdString()
        );
    }

    return notifications;
}

bool NotificationRepository::addNotification(const std::string& vendorId,
                                             const std::string& message)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "INSERT INTO notifications (vendor_id, message) "
        "VALUES (:vendor_id, :message)"
    );

    query.bindValue(":vendor_id", QString::fromStdString(vendorId));
    query.bindValue(":message", QString::fromStdString(message));

    if (!query.exec()) {
        qDebug() << "NotificationRepository::addNotification failed:"
                 << query.lastError().text();
        return false;
    }

    return true;
}

bool NotificationRepository::clearNotificationsForVendor(const std::string& vendorId)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "DELETE FROM notifications "
        "WHERE vendor_id = :vendor_id"
    );

    query.bindValue(":vendor_id", QString::fromStdString(vendorId));

    if (!query.exec()) {
        qDebug() << "NotificationRepository::clearNotificationsForVendor failed:"
                 << query.lastError().text();
        return false;
    }

    return true;
}
