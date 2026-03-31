#ifndef DATABASEINITIALIZER_H
#define DATABASEINITIALIZER_H

class DatabaseInitializer
{
public:
    bool initialize();

private:
    bool createTables();
    bool seedDefaultDataIfNeeded();

    bool isTableEmpty(const char* tableName);

    bool seedUsers();
    bool seedMarketDates();
    bool seedComplianceDocuments();
};

#endif // DATABASEINITIALIZER_H
