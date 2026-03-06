#include <QApplication>

#include "InMemoryStorageManager.h"
#include "BookingController.h"
#include "WaitlistController.h"
#include "StartupUI.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // ------------------------------------------------
    // Create Single In-Memory Storage Instance
    // ------------------------------------------------
    InMemoryStorageManager storage;

    // ------------------------------------------------
    // Load Default System Data (D1 Requirement)
    // ------------------------------------------------
    storage.initializeDefaultData();
    /*
        initializeDefaultData() must:
        - Create 4 MarketDate objects (YYYY-MM-DD format)
        - Set stall capacity rules (2 food, 2 artisan)
        - Create:
            • 4 food vendors
            • 4 artisan vendors
            • 1 market operator
            • 1 system administrator
        - Create 8 waitlists (4 dates × 2 categories)
        - Ensure all bookings = 0
        - Ensure all waitlists empty
    */

    // ------------------------------------------------
    // 3️⃣ Create Controllers (Dependency Injection)
    // ------------------------------------------------
    WaitlistController waitlistController(storage);
    BookingController bookingController(storage, waitlistController);

    // ------------------------------------------------
    // 4️⃣ Create Startup UI
    // ------------------------------------------------
    StartupUI startupUI(storage,
                        bookingController,
                        waitlistController);

    startupUI.show();

    // ------------------------------------------------
    // 5️⃣ Execute Application
    // ------------------------------------------------
    return app.exec();
}