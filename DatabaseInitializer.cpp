#include "DatabaseInitializer.h"
#include "DatabaseManager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QList>

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
    "legal_text TEXT NOT NULL,"
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
        // Food Vendors
        {"food_vendor_1", "Alice Smith", "alice@freshfarms.ca", "111-111-1111", "123 Farm Road", "Vendor", "Fresh Farms", "FOOD"},
        {"food_vendor_2", "Bob Green", "bob@greengardens.ca", "222-222-2222", "456 Garden Ave", "Vendor", "Green Gardens", "FOOD"},
        {"food_vendor_3", "Carol White", "carol@organic.ca", "333-333-3333", "789 Organic St", "Vendor", "Organic Delights", "FOOD"},
        {"food_vendor_4", "David Brown", "david@harvest.ca", "444-444-4444", "101 Harvest Blvd", "Vendor", "Harvest Kitchen", "FOOD"},

        // Artisan Vendors
        {"artisan_vendor_1", "Emma Stone", "emma@crafts.ca", "555-555-5555", "12 Craft Lane", "Vendor", "Handmade Crafts", "ARTISAN"},
        {"artisan_vendor_2", "Frank Miller", "frank@woodworks.ca", "666-666-6666", "34 Timber Rd", "Vendor", "WoodWorks", "ARTISAN"},
        {"artisan_vendor_3", "Grace Lee", "grace@clay.ca", "777-777-7777", "56 Pottery Ave", "Vendor", "Clay Creations", "ARTISAN"},
        {"artisan_vendor_4", "Henry Clark", "henry@designs.ca", "888-888-8888", "78 Design St", "Vendor", "Artisan Designs", "ARTISAN"},

        // Market Operator
        {"market_operator", "Market Admin", "operator@market.ca", "999-999-9999", "Market Office", "MarketOperator", "", ""},

        // System Administrator
        {"system_admin", "System Admin", "admin@market.ca", "000-000-0000", "Admin Office", "SystemAdministrator", "", ""}
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
        {"2026-04-01", 1, "2026-04-01", 2, 2},
        {"2026-04-08", 2, "2026-04-08", 2, 2},
        {"2026-04-15", 3, "2026-04-15", 2, 2},
        {"2026-04-22", 4, "2026-04-22", 2, 2}
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
        {"food_vendor_1", "Food Handling License", "FH-2026-001", "2027-12-31"},
        {"food_vendor_1", "Liability Insurance Policy", "LI-2026-034", "2027-05-15"},

        {"food_vendor_2", "Food Handling License", "FH-2026-002", "2027-12-31"},
        {"food_vendor_2", "Liability Insurance Policy", "LI-2026-035", "2027-05-15"},

        {"food_vendor_3", "Food Handling License", "FH-2026-003", "2027-12-31"},
        {"food_vendor_3", "Liability Insurance Policy", "LI-2026-036", "2027-05-15"},

        {"food_vendor_4", "Food Handling License", "FH-2026-004", "2027-12-31"},
        {"food_vendor_4", "Liability Insurance Policy", "LI-2026-037", "2027-05-15"},

        {"artisan_vendor_1", "Food Handling License", "FH-2026-005", "2027-12-31"},
        {"artisan_vendor_1", "Liability Insurance Policy", "LI-2026-038", "2027-05-15"},

        {"artisan_vendor_2", "Food Handling License", "FH-2026-006", "2027-12-31"},
        {"artisan_vendor_2", "Liability Insurance Policy", "LI-2026-039", "2027-05-15"},

        {"artisan_vendor_3", "Food Handling License", "FH-2026-007", "2027-12-31"},
        {"artisan_vendor_3", "Liability Insurance Policy", "LI-2026-040", "2027-05-15"},

        {"artisan_vendor_4", "Food Handling License", "FH-2026-008", "2027-12-31"},
        {"artisan_vendor_4", "Liability Insurance Policy", "LI-2026-041", "2027-05-15"}
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
