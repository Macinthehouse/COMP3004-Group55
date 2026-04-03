#include "UserRepository.h"

#include "DatabaseManager.h"
#include "User.h"
#include "Vendor.h"
#include "MarketOperator.h"
#include "SystemAdministrator.h"
#include "VendorCategory.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

namespace
{
    VendorCategory parseVendorCategory(const QString& categoryText)
    {
        if (categoryText.toUpper() == "ARTISAN") {
            return VendorCategory::ARTISAN;
        }

        return VendorCategory::FOOD;
    }

    std::unique_ptr<User> buildUserFromQuery(const QSqlQuery& query)
    {
        const std::string id = query.value("id").toString().toStdString();
        const std::string name = query.value("name").toString().toStdString();
        const std::string email = query.value("email").toString().toStdString();
        const std::string phone = query.value("phone").toString().toStdString();
        const std::string address = query.value("address").toString().toStdString();
        const QString role = query.value("role").toString();

        if (role == "Vendor") {
            const std::string businessName =
                query.value("business_name").toString().toStdString();

            VendorCategory category =
                parseVendorCategory(query.value("vendor_category").toString());

            return std::unique_ptr<User>(new Vendor(
                id,
                name,   // ownerName
                email,
                phone,
                address,
                businessName,
                category
            ));
        }

        if (role == "MarketOperator") {
            return std::unique_ptr<User>(new MarketOperator(
                id,
                name,
                email,
                phone,
                address
            ));
        }

        if (role == "SystemAdministrator") {
            return std::unique_ptr<User>(new SystemAdministrator(
                id,
                name,
                email,
                phone,
                address
            ));
        }

        qDebug() << "Unknown role found in users table:" << role;
        return nullptr;
    }
}

std::unique_ptr<User> UserRepository::findById(const std::string& id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT id, name, email, phone, address, role, business_name, vendor_category "
        "FROM users "
        "WHERE id = :id"
    );
    query.bindValue(":id", QString::fromStdString(id));

    if (!query.exec()) {
        qDebug() << "findById query failed:" << query.lastError().text();
        return nullptr;
    }

    if (!query.next()) {
        return nullptr;
    }

    return buildUserFromQuery(query);
}

std::vector<std::unique_ptr<User>> UserRepository::loadAllUsers()
{
    std::vector<std::unique_ptr<User>> users;

    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT id, name, email, phone, address, role, business_name, vendor_category "
        "FROM users"
    );

    if (!query.exec()) {
        qDebug() << "loadAllUsers query failed:" << query.lastError().text();
        return users;
    }

    while (query.next()) {
        std::unique_ptr<User> user = buildUserFromQuery(query);
        if (user) {
            users.push_back(std::move(user));
        }
    }

    return users;
}
