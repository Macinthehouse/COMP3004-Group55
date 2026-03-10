#include "InMemoryStorageManager.h"

#include "Vendor.h"
#include "MarketOperator.h"
#include "SystemAdministrator.h"
#include "ComplianceDocument.h"

#include <vector>
#include <utility>
#include <memory>

// --------------------------------------------------
// Constructor
// --------------------------------------------------

InMemoryStorageManager::InMemoryStorageManager()
{
}

// --------------------------------------------------
// initializeDefaultData()
// --------------------------------------------------

void InMemoryStorageManager::initializeDefaultData()
{
    users.clear();
    marketDates.clear();
    waitlists.clear();

    // --------------------------------------------------
    // 1️⃣ Create Market Dates
    // --------------------------------------------------

    std::vector<std::string> dates = {
        "2026-04-01",
        "2026-04-08",
        "2026-04-15",
        "2026-04-22"
    };

    for (const auto& date : dates) {

        marketDates.emplace(date, MarketDate(date, 2, 2));

        waitlists.emplace(
            std::make_pair(date, VendorCategory::FOOD),
            Waitlist(date, VendorCategory::FOOD)
        );

        waitlists.emplace(
            std::make_pair(date, VendorCategory::ARTISAN),
            Waitlist(date, VendorCategory::ARTISAN)
        );
    }

    // --------------------------------------------------
    // 2️⃣ Create Vendors (Polymorphic Storage)
    // --------------------------------------------------

    users["food_vendor_1"] = std::unique_ptr<User>(new Vendor(
        "food_vendor_1",
        "Alice Smith",
        "alice@freshfarms.ca",
        "111-111-1111",
        "123 Farm Road",
        "Fresh Farms",
        VendorCategory::FOOD
    ));

    users["food_vendor_2"] = std::unique_ptr<User>(new Vendor(
        "food_vendor_2",
        "Bob Green",
        "bob@greengardens.ca",
        "222-222-2222",
        "456 Garden Ave",
        "Green Gardens",
        VendorCategory::FOOD
    ));

    users["food_vendor_3"] = std::unique_ptr<User>(new Vendor(
        "food_vendor_3",
        "Carol White",
        "carol@organic.ca",
        "333-333-3333",
        "789 Organic St",
        "Organic Delights",
        VendorCategory::FOOD
    ));

    users["food_vendor_4"] = std::unique_ptr<User>(new Vendor(
        "food_vendor_4",
        "David Brown",
        "david@harvest.ca",
        "444-444-4444",
        "101 Harvest Blvd",
        "Harvest Kitchen",
        VendorCategory::FOOD
    ));

    users["artisan_vendor_1"] = std::unique_ptr<User>(new Vendor(
        "artisan_vendor_1",
        "Emma Stone",
        "emma@crafts.ca",
        "555-555-5555",
        "12 Craft Lane",
        "Handmade Crafts",
        VendorCategory::ARTISAN
    ));

    users["artisan_vendor_2"] = std::unique_ptr<User>(new Vendor(
        "artisan_vendor_2",
        "Frank Miller",
        "frank@woodworks.ca",
        "666-666-6666",
        "34 Timber Rd",
        "WoodWorks",
        VendorCategory::ARTISAN
    ));

    users["artisan_vendor_3"] = std::unique_ptr<User>(new Vendor(
        "artisan_vendor_3",
        "Grace Lee",
        "grace@clay.ca",
        "777-777-7777",
        "56 Pottery Ave",
        "Clay Creations",
        VendorCategory::ARTISAN
    ));

    users["artisan_vendor_4"] = std::unique_ptr<User>(new Vendor(
        "artisan_vendor_4",
        "Henry Clark",
        "henry@designs.ca",
        "888-888-8888",
        "78 Design St",
        "Artisan Designs",
        VendorCategory::ARTISAN
    ));

    // --------------------------------------------------
    // 3️⃣ Market Operator
    // --------------------------------------------------

    users["market_operator"] = std::unique_ptr<User>(new MarketOperator(
        "market_operator",
        "Market Admin",
        "operator@market.ca",
        "999-999-9999",
        "Market Office"
    ));

    // --------------------------------------------------
    // 4️⃣ System Administrator
    // --------------------------------------------------

    users["system_admin"] = std::unique_ptr<User>(new SystemAdministrator(
        "system_admin",
        "System Admin",
        "admin@market.ca",
        "000-000-0000",
        "Admin Office"
    ));

    // --------------------------------------------------
    // 5️⃣ Create Compliance Documents (Extended Legal Text)
    // --------------------------------------------------

    ComplianceDocument foodLicense(
        "Food Handling License",
        "FH-2026-001",
        "2027-12-31",
        "This Food Handling License certifies that the vendor has completed "
        "all required municipal and provincial food safety certification programs. "
        "The vendor agrees to comply with all applicable public health regulations, "
        "including but not limited to safe food storage temperatures, prevention of "
        "cross-contamination, proper sanitation procedures, hygienic food preparation "
        "standards, and use of approved food-grade equipment. Vendors must maintain "
        "clean and sanitary preparation areas at all times and permit unannounced "
        "inspections by authorized market personnel or public health officials. "
        "Non-compliance may result in suspension, revocation of stall privileges, "
        "financial penalties, or referral to governing health authorities."
    );

    ComplianceDocument liabilityInsurance(
        "Liability Insurance Policy",
        "LI-2026-034",
        "2027-05-15",
        "This Liability Insurance Policy confirms that the vendor maintains "
        "active commercial general liability insurance coverage sufficient "
        "to cover bodily injury, property damage, product liability claims, "
        "and any other risk arising from stall operations. The vendor assumes "
        "full legal and financial responsibility for any damages, losses, "
        "injuries, or claims resulting from their products, employees, "
        "equipment, or operational negligence. The HintonMarket organization "
        "shall not be held liable for vendor misconduct or regulatory violations. "
        "Proof of active insurance must be provided upon request and maintained "
        "continuously throughout the market season."
    );

    // --------------------------------------------------
    // 6️⃣ Attach Compliance Documents to All Vendors
    // --------------------------------------------------

    for (auto& pair : users) {

        Vendor* vendor = dynamic_cast<Vendor*>(pair.second.get());

        if (vendor) {
            vendor->addComplianceDocument(foodLicense);
            vendor->addComplianceDocument(liabilityInsurance);
        }
    }
}

// --------------------------------------------------
// getUser()
// --------------------------------------------------

User* InMemoryStorageManager::getUser(const std::string& userId)
{
    auto it = users.find(userId);
    return (it != users.end()) ? it->second.get() : nullptr;
}

// --------------------------------------------------
// getVendor()
// --------------------------------------------------

Vendor* InMemoryStorageManager::getVendor(const std::string& userId)
{
    User* user = getUser(userId);
    return dynamic_cast<Vendor*>(user);
}

// --------------------------------------------------
// getMarketDate()
// --------------------------------------------------

MarketDate* InMemoryStorageManager::getMarketDate(const std::string& marketDateId)
{
    auto it = marketDates.find(marketDateId);
    return (it != marketDates.end()) ? &(it->second) : nullptr;
}

// --------------------------------------------------
// getWaitlist()
// --------------------------------------------------

Waitlist* InMemoryStorageManager::getWaitlist(const std::string& marketDateId,
                                              VendorCategory category)
{
    auto key = std::make_pair(marketDateId, category);
    auto it = waitlists.find(key);

    return (it != waitlists.end()) ? &(it->second) : nullptr;
}
