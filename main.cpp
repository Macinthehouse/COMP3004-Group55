#include <QApplication>
#include <iostream>

#include "InMemoryStorageManager.h"
#include "BookingController.h"
#include "WaitlistController.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // ------------------------------------------------
    // 1️⃣ Create Central In-Memory Storage
    // ------------------------------------------------
    // Stores:
    // - Polymorphic Users (Vendor, MarketOperator, SystemAdministrator)
    // - MarketDates
    // - Waitlists
    // ------------------------------------------------
    InMemoryStorageManager storage;

    // ------------------------------------------------
    // 2️⃣ Load Default Data (D1 Requirement)
    // ------------------------------------------------
    // Creates:
    // - 4 Market Dates (2 Food + 2 Artisan capacity)
    // - 8 Waitlists (4 dates × 2 categories)
    // - 4 Food Vendors
    // - 4 Artisan Vendors
    // - 1 MarketOperator
    // - 1 SystemAdministrator
    //
    // All stored polymorphically as std::unique_ptr<User>
    // ------------------------------------------------
    storage.initializeDefaultData();

    // ------------------------------------------------
    // 3️⃣ Create Controllers (Dependency Injection)
    // ------------------------------------------------
    // Controllers depend on storage
    // BookingController depends on WaitlistController
    // ------------------------------------------------
    WaitlistController waitlistController(storage);
    BookingController bookingController(storage, waitlistController);

    // ------------------------------------------------
    // Backend Ready
    // ------------------------------------------------
    std::cout << "HintonMarket backend initialized successfully." << std::endl;

    // UI handled separately (Person 3)
    // For now, backend-only initialization.

    return 0;
}
