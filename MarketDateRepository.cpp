#include "MarketDateRepository.h"
#include "DatabaseManager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

MarketDate* MarketDateRepository::findById(const std::string& id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT id, food_capacity, artisan_capacity "
        "FROM market_dates "
        "WHERE id = :id"
    );
    query.bindValue(":id", QString::fromStdString(id));

    if (!query.exec()) {
        qDebug() << "MarketDateRepository::findById failed:" << query.lastError().text();
        return nullptr;
    }

    if (!query.next()) {
        return nullptr;
    }

    return new MarketDate(
        query.value("id").toString().toStdString(),
        query.value("food_capacity").toInt(),
        query.value("artisan_capacity").toInt()
    );
}

std::vector<MarketDate> MarketDateRepository::loadAllMarketDates()
{
    std::vector<MarketDate> dates;

    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT id, food_capacity, artisan_capacity "
        "FROM market_dates "
        "ORDER BY week_number"
    );

    if (!query.exec()) {
        qDebug() << "MarketDateRepository::loadAllMarketDates failed:" << query.lastError().text();
        return dates;
    }

    while (query.next()) {
        dates.emplace_back(
            query.value("id").toString().toStdString(),
            query.value("food_capacity").toInt(),
            query.value("artisan_capacity").toInt()
        );
    }

    return dates;
}
