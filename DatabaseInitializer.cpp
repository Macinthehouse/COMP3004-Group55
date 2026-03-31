#include "DatabaseInitializer.h"
#include "DatabaseManager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

bool DatabaseInitializer::initialize()
{
    if (!DatabaseManager::instance().isOpen()) {
        qDebug() << "Database is not open. Cannot initialize.";
        return false;
    }

    if (!createTables()) {
        qDebug() << "Failed to create tables.";
        return false;
    }

    if (!seedDefaultDataIfNeeded()) {
        qDebug() << "Failed to seed default data.";
        return false;
    }

    return true;
}

bool DatabaseInitializer::createTables()
{
    QSqlQuery query(DatabaseManager::instance().database());

    bool ok = true;

    ok = ok && query.exec(
        "CREATE TABLE IF NOT EXISTS users ("
        "id TEXT PRIMARY KEY,"
        "name TEXT NOT NULL,"
        "email TEXT NOT NULL,"
        "phone TEXT NOT NULL,"
        "address TEXT NOT NULL,"
        "role TEXT NOT NULL,"
        "business_name TEXT,"
        "vendor_category TEXT"
        ")"
    );

    ok = ok && query.exec(
        "CREATE TABLE IF NOT EXISTS market_dates ("
        "id TEXT PRIMARY KEY,"
        "week_number INTEGER NOT NULL,"
        "market_date TEXT NOT NULL,"
        "food_capacity INTEGER NOT NULL,"
        "artisan_capacity INTEGER NOT NULL"
        ")"
    );

    ok = ok && query.exec(
        "CREATE TABLE IF NOT EXISTS compliance_documents ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "vendor_id TEXT NOT NULL,"
        "document_type TEXT NOT NULL,"
        "document_number TEXT NOT NULL,"
        "expiry_date TEXT NOT NULL,"
        "FOREIGN KEY(vendor_id) REFERENCES users(id)"
        ")"
    );

    ok = ok && query.exec(
        "CREATE TABLE IF NOT EXISTS bookings ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "vendor_id TEXT NOT NULL,"
        "market_date_id TEXT NOT NULL,"
        "FOREIGN KEY(vendor_id) REFERENCES users(id),"
        "FOREIGN KEY(market_date_id) REFERENCES market_dates(id)"
        ")"
    );

    ok = ok && query.exec(
        "CREATE TABLE IF NOT EXISTS waitlists ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "vendor_id TEXT NOT NULL,"
        "market_date_id TEXT NOT NULL,"
        "category TEXT NOT NULL,"
        "queue_position INTEGER NOT NULL,"
        "FOREIGN KEY(vendor_id) REFERENCES users(id),"
        "FOREIGN KEY(market_date_id) REFERENCES market_dates(id)"
        ")"
    );

    ok = ok && query.exec(
        "CREATE TABLE IF NOT EXISTS notifications ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "vendor_id TEXT NOT NULL,"
        "message TEXT NOT NULL,"
        "FOREIGN KEY(vendor_id) REFERENCES users(id)"
        ")"
    );

    if (!ok) {
        qDebug() << "Table creation failed:" << query.lastError().text();
    }

    return ok;
}

bool DatabaseInitializer::seedDefaultDataIfNeeded()
{
    if (!isTableEmpty("users")) {
        qDebug() << "Database already contains default data. Skipping seed.";
        return true;
    }

    bool ok = true;
    ok = ok && seedUsers();
    ok = ok && seedMarketDates();
    ok = ok && seedComplianceDocuments();

    return ok;
}

bool DatabaseInitializer::isTableEmpty(const char* tableName)
{
    QSqlQuery query(DatabaseManager::instance().database());
    QString sql = QString("SELECT COUNT(*) FROM %1").arg(tableName);

    if (!query.exec(sql)) {
        qDebug() << "Failed to check table:" << tableName << query.lastError().text();
        return true;
    }

    if (query.next()) {
        return query.value(0).toInt() == 0;
    }

    return true;
}

bool DatabaseInitializer::seedUsers()
{
    QSqlQuery query(DatabaseManager::instance().database());

    struct UserSeed {
        QString id;
        QString name;
        QString email;
        QString phone;
        QString address;
        QString role;
        QString businessName;
        QString vendorCategory;
    };

    QList<UserSeed> users = {
        // 4 Food Vendors
        {"FV001", "Alice Brown", "alice@food.com", "613-111-0001", "101 Main St", "Vendor", "Alice Bakery", "FOOD"},
        {"FV002", "Bob Green", "bob@food.com", "613-111-0002", "102 Main St", "Vendor", "Bob BBQ", "FOOD"},
        {"FV003", "Carol White", "carol@food.com", "613-111-0003", "103 Main St", "Vendor", "Carol Catering", "FOOD"},
        {"FV004", "David Black", "david@food.com", "613-111-0004", "104 Main St", "Vendor", "David Desserts", "FOOD"},

        // 4 Artisan Vendors
        {"AV001", "Emma Stone", "emma@artisan.com", "613-222-0001", "201 Oak St", "Vendor", "Emma Crafts", "ARTISAN"},
        {"AV002", "Frank Wood", "frank@artisan.com", "613-222-0002", "202 Oak St", "Vendor", "Frank Pottery", "ARTISAN"},
        {"AV003", "Grace Hall", "grace@artisan.com", "613-222-0003", "203 Oak St", "Vendor", "Grace Handmade", "ARTISAN"},
        {"AV004", "Henry King", "henry@artisan.com", "613-222-0004", "204 Oak St", "Vendor", "Henry Designs", "ARTISAN"},

        // Market Operator
        {"MO001", "Market Operator", "operator@hintonmarket.com", "613-333-0001", "300 Market St", "MarketOperator", "", ""},

        // System Administrator
        {"SA001", "System Admin", "admin@hintonmarket.com", "613-333-0002", "301 Market St", "SystemAdministrator", "", ""}
    };

    query.prepare(
        "INSERT INTO users (id, name, email, phone, address, role, business_name, vendor_category) "
        "VALUES (:id, :name, :email, :phone, :address, :role, :business_name, :vendor_category)"
    );

    for (const UserSeed& user : users) {
        query.bindValue(":id", user.id);
        query.bindValue(":name", user.name);
        query.bindValue(":email", user.email);
        query.bindValue(":phone", user.phone);
        query.bindValue(":address", user.address);
        query.bindValue(":role", user.role);
        query.bindValue(":business_name", user.businessName);
        query.bindValue(":vendor_category", user.vendorCategory);

        if (!query.exec()) {
            qDebug() << "Failed to insert user:" << user.id << query.lastError().text();
            return false;
        }
    }

    return true;
}

bool DatabaseInitializer::seedMarketDates()
{
    QSqlQuery query(DatabaseManager::instance().database());

    struct MarketDateSeed {
        QString id;
        int weekNumber;
        QString marketDate;
        int foodCapacity;
        int artisanCapacity;
    };

    QList<MarketDateSeed> dates = {
        {"MD001", 1, "2026-04-04", 2, 2},
        {"MD002", 2, "2026-04-11", 2, 2},
        {"MD003", 3, "2026-04-18", 2, 2},
        {"MD004", 4, "2026-04-25", 2, 2}
    };

    query.prepare(
        "INSERT INTO market_dates (id, week_number, market_date, food_capacity, artisan_capacity) "
        "VALUES (:id, :week_number, :market_date, :food_capacity, :artisan_capacity)"
    );

    for (const MarketDateSeed& date : dates) {
        query.bindValue(":id", date.id);
        query.bindValue(":week_number", date.weekNumber);
        query.bindValue(":market_date", date.marketDate);
        query.bindValue(":food_capacity", date.foodCapacity);
        query.bindValue(":artisan_capacity", date.artisanCapacity);

        if (!query.exec()) {
            qDebug() << "Failed to insert market date:" << date.id << query.lastError().text();
            return false;
        }
    }

    return true;
}

bool DatabaseInitializer::seedComplianceDocuments()
{
    QSqlQuery query(DatabaseManager::instance().database());

    struct ComplianceSeed {
        QString vendorId;
        QString documentType;
        QString documentNumber;
        QString expiryDate;
    };

    QList<ComplianceSeed> docs = {
        {"FV001", "Food License", "FL-1001", "2027-12-31"},
        {"FV002", "Food License", "FL-1002", "2027-12-31"},
        {"FV003", "Food License", "FL-1003", "2027-12-31"},
        {"FV004", "Food License", "FL-1004", "2027-12-31"},
        {"AV001", "Artisan Policy", "AP-2001", "2027-12-31"},
        {"AV002", "Artisan Policy", "AP-2002", "2027-12-31"},
        {"AV003", "Artisan Policy", "AP-2003", "2027-12-31"},
        {"AV004", "Artisan Policy", "AP-2004", "2027-12-31"}
    };

    query.prepare(
        "INSERT INTO compliance_documents (vendor_id, document_type, document_number, expiry_date) "
        "VALUES (:vendor_id, :document_type, :document_number, :expiry_date)"
    );

    for (const ComplianceSeed& doc : docs) {
        query.bindValue(":vendor_id", doc.vendorId);
        query.bindValue(":document_type", doc.documentType);
        query.bindValue(":document_number", doc.documentNumber);
        query.bindValue(":expiry_date", doc.expiryDate);

        if (!query.exec()) {
            qDebug() << "Failed to insert compliance document for vendor:" 
                     << doc.vendorId << query.lastError().text();
            return false;
        }
    }

    return true;
}
