#include "WaitlistRepository.h"
#include "DatabaseManager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

namespace
{
    QString categoryToQString(VendorCategory category)
    {
        return (category == VendorCategory::ARTISAN) ? "ARTISAN" : "FOOD";
    }

    VendorCategory parseVendorCategory(const QString& categoryText)
    {
        const QString normalized = categoryText.trimmed().toUpper();

        if (normalized == "FOOD") {
            return VendorCategory::FOOD;
        }

        if (normalized == "ARTISAN") {
            return VendorCategory::ARTISAN;
        }

        qDebug() << "Unknown vendor category in waitlists query:" << categoryText;
        return VendorCategory::FOOD;
    }
}

std::vector<Waitlist> WaitlistRepository::loadAllWaitlists()
{
    std::vector<Waitlist> waitlists;

    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT market_date_id, category, vendor_id, queue_position "
        "FROM waitlists "
        "ORDER BY market_date_id, category, queue_position"
    );

    if (!query.exec()) {
        qDebug() << "WaitlistRepository::loadAllWaitlists failed:"
                 << query.lastError().text();
        return waitlists;
    }

    QString currentMarketDateId;
    QString currentCategoryText;
    Waitlist currentWaitlist;
    bool hasCurrentWaitlist = false;

    while (query.next()) {
        const QString marketDateId = query.value("market_date_id").toString();
        const QString categoryText = query.value("category").toString();
        const QString vendorId = query.value("vendor_id").toString();

        if (!hasCurrentWaitlist ||
            marketDateId != currentMarketDateId ||
            categoryText != currentCategoryText) {

            if (hasCurrentWaitlist) {
                waitlists.push_back(currentWaitlist);
            }

            currentMarketDateId = marketDateId;
            currentCategoryText = categoryText;
            currentWaitlist = Waitlist(
                marketDateId.toStdString(),
                parseVendorCategory(categoryText)
            );
            hasCurrentWaitlist = true;
        }

        currentWaitlist.enqueue(vendorId.toStdString());
    }

    if (hasCurrentWaitlist) {
        waitlists.push_back(currentWaitlist);
    }

    return waitlists;
}

bool WaitlistRepository::addVendorToWaitlist(const std::string& vendorId,
                                             const std::string& marketDateId,
                                             VendorCategory category,
                                             int queuePosition)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "INSERT INTO waitlists (vendor_id, market_date_id, category, queue_position) "
        "VALUES (:vendor_id, :market_date_id, :category, :queue_position)"
    );

    query.bindValue(":vendor_id", QString::fromStdString(vendorId));
    query.bindValue(":market_date_id", QString::fromStdString(marketDateId));
    query.bindValue(":category", categoryToQString(category));
    query.bindValue(":queue_position", queuePosition);

    if (!query.exec()) {
        qDebug() << "WaitlistRepository::addVendorToWaitlist failed:"
                 << query.lastError().text();
        return false;
    }

    return true;
}

bool WaitlistRepository::removeVendorFromWaitlist(const std::string& vendorId,
                                                  const std::string& marketDateId,
                                                  VendorCategory category)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "DELETE FROM waitlists "
        "WHERE vendor_id = :vendor_id "
        "AND market_date_id = :market_date_id "
        "AND category = :category"
    );

    query.bindValue(":vendor_id", QString::fromStdString(vendorId));
    query.bindValue(":market_date_id", QString::fromStdString(marketDateId));
    query.bindValue(":category", categoryToQString(category));

    if (!query.exec()) {
        qDebug() << "WaitlistRepository::removeVendorFromWaitlist failed:"
                 << query.lastError().text();
        return false;
    }

    return true;
}

bool WaitlistRepository::replaceWaitlist(const Waitlist& waitlist)
{
    QSqlDatabase db = DatabaseManager::instance().database();

    if (!db.transaction()) {
        qDebug() << "WaitlistRepository::replaceWaitlist failed to start transaction:"
                 << db.lastError().text();
        return false;
    }

    QSqlQuery deleteQuery(db);
    deleteQuery.prepare(
        "DELETE FROM waitlists "
        "WHERE market_date_id = :market_date_id "
        "AND category = :category"
    );
    deleteQuery.bindValue(":market_date_id", QString::fromStdString(waitlist.getMarketDateId()));
    deleteQuery.bindValue(":category", categoryToQString(waitlist.getCategory()));

    if (!deleteQuery.exec()) {
        qDebug() << "WaitlistRepository::replaceWaitlist delete failed:"
                 << deleteQuery.lastError().text();
        db.rollback();
        return false;
    }

    const std::vector<std::string> queueSnapshot = waitlist.getQueueSnapshot();

    QSqlQuery insertQuery(db);
    insertQuery.prepare(
        "INSERT INTO waitlists (vendor_id, market_date_id, category, queue_position) "
        "VALUES (:vendor_id, :market_date_id, :category, :queue_position)"
    );

    for (size_t i = 0; i < queueSnapshot.size(); ++i) {
        insertQuery.bindValue(":vendor_id", QString::fromStdString(queueSnapshot[i]));
        insertQuery.bindValue(":market_date_id", QString::fromStdString(waitlist.getMarketDateId()));
        insertQuery.bindValue(":category", categoryToQString(waitlist.getCategory()));
        insertQuery.bindValue(":queue_position", static_cast<int>(i) + 1);

        if (!insertQuery.exec()) {
            qDebug() << "WaitlistRepository::replaceWaitlist insert failed:"
                     << insertQuery.lastError().text();
            db.rollback();
            return false;
        }
    }

    if (!db.commit()) {
        qDebug() << "WaitlistRepository::replaceWaitlist commit failed:"
                 << db.lastError().text();
        db.rollback();
        return false;
    }

    return true;
}
