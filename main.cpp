#include <QApplication>
#include <QCoreApplication>
#include <QFileInfo>
#include <QString>
#include <iostream>

#include "DatabaseManager.h"
#include "DatabaseInitializer.h"
#include "InMemoryStorageManager.h"
#include "BookingController.h"
#include "WaitlistController.h"
#include "LoginController.h"
#include "StartupUI.h"

namespace
{
    QString findDatabasePath()
    {
        const QString basePath = QCoreApplication::applicationDirPath();

        QStringList candidatePaths = {
            basePath + "/hintonMarket.sqlite3",
            basePath + "/../hintonMarket.sqlite3",
            basePath + "/../../hintonMarket.sqlite3",
            basePath + "/../../../hintonMarket.sqlite3"
        };

        for (const QString& path : candidatePaths) {
            if (QFileInfo::exists(path)) {
                return QFileInfo(path).absoluteFilePath();
            }
        }

        // If it does not exist yet, create it in the app directory.
        return basePath + "/hintonMarket.sqlite3";
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString dbPath = findDatabasePath();

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
    if (!storage.loadFromDatabase()) {
        std::cerr << "Failed to load data from database into memory." << std::endl;
        return 1;
    }

    WaitlistController waitlistController(storage);
    BookingController bookingController(storage, waitlistController);
    LoginController loginController(&storage);

    std::cout << "HintonMarket backend initialized successfully with SQLite persistence."
              << std::endl;

    StartupUI startup;
    startup.show();

    return app.exec();
}
