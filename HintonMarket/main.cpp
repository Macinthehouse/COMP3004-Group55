#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QFileInfo>
#include <QStringList>

#include <iostream>

#include "../DatabaseManager.h"
#include "../DatabaseInitializer.h"

namespace
{
    QString findDatabasePath()
    {
        const QString basePath = QCoreApplication::applicationDirPath();

        QStringList candidatePaths;
        candidatePaths << (basePath + "/hintonMarket.sqlite3")
                       << (basePath + "/../hintonMarket.sqlite3")
                       << (basePath + "/../../hintonMarket.sqlite3")
                       << (basePath + "/../../../hintonMarket.sqlite3");

        for (int i = 0; i < candidatePaths.size(); ++i) {
            const QString& path = candidatePaths[i];
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
    QApplication a(argc, argv);

    const QString dbPath = findDatabasePath();

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

    MainWindow w;
    w.show();

    const int rc = a.exec();

    DatabaseManager::instance().closeDatabase();
    return rc;
}
