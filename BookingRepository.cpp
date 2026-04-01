#include "BookingRepository.h"
#include "DatabaseManager.h"
#include "VendorCategory.h"

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

bool BookingRepository::createBooking(const Booking& booking)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "INSERT INTO bookings (vendor_id, market_date_id) "
        "VALUES (:vendor_id, :market_date_id)"
    );

    query.bindValue(":vendor_id", QString::fromStdString(booking.getVendorId()));
    query.bindValue(":market_date_id", QString::fromStdString(booking.getMarketDateId()));

    if (!query.exec()) {
        qDebug() << "BookingRepository::createBooking failed:" << query.lastError().text();
        return false;
    }

    return true;
}

bool BookingRepository::removeBooking(const std::string& vendorId,
                                      const std::string& marketDateId)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "DELETE FROM bookings "
        "WHERE vendor_id = :vendor_id "
        "AND market_date_id = :market_date_id"
    );

    query.bindValue(":vendor_id", QString::fromStdString(vendorId));
    query.bindValue(":market_date_id", QString::fromStdString(marketDateId));

    if (!query.exec()) {
        qDebug() << "BookingRepository::removeBooking failed:" << query.lastError().text();
        return false;
    }

    return true;
}
