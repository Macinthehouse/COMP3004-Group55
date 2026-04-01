#include "BookingRepository.h"
#include "DatabaseManager.h"
#include "VendorCategory.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

namespace
{
    VendorCategory parseVendorCategory(const QString& categoryText)
    {
        const QString normalized = categoryText.trimmed().toUpper();

        if (normalized == "FOOD") {
            return VendorCategory::FOOD;
        }

        if (normalized == "ARTISAN") {
            return VendorCategory::ARTISAN;
        }

        qDebug() << "Unknown vendor category in bookings query:" << categoryText;
        return VendorCategory::FOOD;
    }
}

std::vector<Booking> BookingRepository::loadAllBookings()
{
    std::vector<Booking> bookings;

    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT b.vendor_id, b.market_date_id, u.vendor_category "
        "FROM bookings b "
        "JOIN users u ON b.vendor_id = u.id "
        "ORDER BY b.id"
    );

    if (!query.exec()) {
        qDebug() << "BookingRepository::loadAllBookings failed:" << query.lastError().text();
        return bookings;
    }

    while (query.next()) {
        bookings.emplace_back(
            query.value("vendor_id").toString().toStdString(),
            query.value("market_date_id").toString().toStdString(),
            parseVendorCategory(query.value("vendor_category").toString())
        );
    }

    return bookings;
}
