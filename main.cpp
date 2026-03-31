#include <QApplication>
#include <QCoreApplication>
#include <iostream>

#include "DatabaseManager.h"
#include "DatabaseInitializer.h"
#include "InMemoryStorageManager.h"
#include "BookingController.h"
#include "WaitlistController.h"
#include "LoginController.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString dbPath = QCoreApplication::applicationDirPath() + "/hintonMarket.sqlite3";

    if (!DatabaseManager::instance().openDatabase(dbPath)) {
        std::cerr << "Failed to open SQLite database: "
                  << DatabaseManager::instance().lastError().toStdString()
                  << std::endl;
        return 1;
    }

    DatabaseInitializer initializer;
    if (!initializer.initialize()) {
        std::cerr << "Failed to initialize database." << std::endl;
        return 1;
    }

    InMemoryStorageManager storage;

    // New D2 behavior:
    // Load persisted users, market dates, waitlists, bookings, etc.
    // from SQLite into in-memory objects.
    if (!storage.loadFromDatabase()) {
        std::cerr << "Failed to load data from database into memory." << std::endl;
        return 1;
    }

    WaitlistController waitlistController(storage);
    BookingController bookingController(storage, waitlistController);
    LoginController loginController(&storage);

    std::cout << "HintonMarket backend initialized successfully with SQLite persistence."
              << std::endl;

    return 0;
}
