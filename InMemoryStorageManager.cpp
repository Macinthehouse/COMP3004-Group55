#include "InMemoryStorageManager.h"

#include "Vendor.h"
#include "MarketDate.h"
#include "Waitlist.h"

#include <utility>   // for std::pair
#include <iostream>  // remove if not debugging

// --------------------------------------------------
// Constructor
// --------------------------------------------------

InMemoryStorageManager::InMemoryStorageManager()
{
    // Nothing required here.
    // Data is loaded explicitly via initializeDefaultData()
}


// --------------------------------------------------
// initializeDefaultData()
// --------------------------------------------------

void InMemoryStorageManager::initializeDefaultData()
{
    // Clear any existing data (important if reused)
    vendors.clear();
    marketDates.clear();
    waitlists.clear();

    // --------------------------------------------------
    // 1️⃣ Create 4 Market Dates (YYYY-MM-DD format)
    // --------------------------------------------------

    std::vector<std::string> dates = {
        "2026-04-01",
        "2026-04-08",
        "2026-04-15",
        "2026-04-22"
    };

    for (const auto& date : dates) {
        MarketDate marketDate(date, 2, 2); 
        // Assumes MarketDate constructor:
        // MarketDate(std::string date,
        //            int maxFoodVendors,
        //            int maxArtisanVendors)

        marketDates.emplace(date, marketDate);

        // Create 2 waitlists per date (Food & Artisan)
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
    // 2️⃣ Create Vendors
    // --------------------------------------------------

    // 4 Food Vendors
    vendors.emplace("food_vendor_1",
        Vendor("food_vendor_1", "Fresh Farms", "Alice Smith",
               "alice@freshfarms.ca", "111-111-1111",
               "123 Farm Road", VendorCategory::FOOD));

    vendors.emplace("food_vendor_2",
        Vendor("food_vendor_2", "Green Gardens", "Bob Green",
               "bob@greengardens.ca", "222-222-2222",
               "456 Garden Ave", VendorCategory::FOOD));

    vendors.emplace("food_vendor_3",
        Vendor("food_vendor_3", "Organic Delights", "Carol White",
               "carol@organic.ca", "333-333-3333",
               "789 Organic St", VendorCategory::FOOD));

    vendors.emplace("food_vendor_4",
        Vendor("food_vendor_4", "Harvest Kitchen", "David Brown",
               "david@harvest.ca", "444-444-4444",
               "101 Harvest Blvd", VendorCategory::FOOD));


    // 4 Artisan Vendors
    vendors.emplace("artisan_vendor_1",
        Vendor("artisan_vendor_1", "Handmade Crafts", "Emma Stone",
               "emma@crafts.ca", "555-555-5555",
               "12 Craft Lane", VendorCategory::ARTISAN));

    vendors.emplace("artisan_vendor_2",
        Vendor("artisan_vendor_2", "WoodWorks", "Frank Miller",
               "frank@woodworks.ca", "666-666-6666",
               "34 Timber Rd", VendorCategory::ARTISAN));

    vendors.emplace("artisan_vendor_3",
        Vendor("artisan_vendor_3", "Clay Creations", "Grace Lee",
               "grace@clay.ca", "777-777-7777",
               "56 Pottery Ave", VendorCategory::ARTISAN));

    vendors.emplace("artisan_vendor_4",
        Vendor("artisan_vendor_4", "Artisan Designs", "Henry Clark",
               "henry@designs.ca", "888-888-8888",
               "78 Design St", VendorCategory::ARTISAN));

    // --------------------------------------------------
    // 3️⃣ Market Operator & System Admin
    // --------------------------------------------------

    vendors.emplace("market_operator",
        Vendor("market_operator", "Market Operator", "Operator User",
               "operator@market.ca", "999-999-9999",
               "Market Office", VendorCategory::FOOD)); 
    // Category may not matter for operator (depends on your design)

    vendors.emplace("system_admin",
        Vendor("system_admin", "System Admin", "Admin User",
               "admin@market.ca", "000-000-0000",
               "Admin Office", VendorCategory::FOOD));
}
    

// --------------------------------------------------
// getVendor()
// --------------------------------------------------

Vendor* InMemoryStorageManager::getVendor(const std::string& vendorId)
{
    auto it = vendors.find(vendorId);
    if (it != vendors.end()) {
        return &(it->second);
    }
    return nullptr;
}


// --------------------------------------------------
// getMarketDate()
// --------------------------------------------------

MarketDate* InMemoryStorageManager::getMarketDate(const std::string& marketDateId)
{
    auto it = marketDates.find(marketDateId);
    if (it != marketDates.end()) {
        return &(it->second);
    }
    return nullptr;
}


// --------------------------------------------------
// getWaitlist()
// --------------------------------------------------

Waitlist* InMemoryStorageManager::getWaitlist(const std::string& marketDateId,
                                              VendorCategory category)
{
    auto key = std::make_pair(marketDateId, category);
    auto it = waitlists.find(key);

    if (it != waitlists.end()) {
        return &(it->second);
    }

    return nullptr;
}