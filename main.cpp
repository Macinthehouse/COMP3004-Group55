#include <QApplication>
#include <iostream>

#include "InMemoryStorageManager.h"
#include "BookingController.h"
#include "WaitlistController.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // ------------------------------------------------
    // 1️⃣ Create Single In-Memory Storage Instance
    // ------------------------------------------------
    InMemoryStorageManager storage;

    // ------------------------------------------------
    // 2️⃣ Load Default System Data (D1 Requirement)
    // ------------------------------------------------
    storage.initializeDefaultData();

    // ------------------------------------------------
    // 3️⃣ Create Controllers (Dependency Injection)
    // ------------------------------------------------
    WaitlistController waitlistController(storage);
    BookingController bookingController(storage, waitlistController);

    // ------------------------------------------------
    // Backend Ready
    // ------------------------------------------------
    std::cout << "HintonMarket backend initialized successfully." << std::endl;

    // No UI yet (handled by Person 3)
    return 0;
}