#ifndef INMEMORYSTORAGEMANAGER_H
#define INMEMORYSTORAGEMANAGER_H

#include <string>
#include <unordered_map>
#include <map>
#include <memory>
#include <vector>

#include "User.h"
#include "Vendor.h"
#include "MarketOperator.h"
#include "SystemAdministrator.h"
#include "VendorCategory.h"
#include "MarketSchedule.h"
#include "Waitlist.h"

// InMemoryStorageManager
//
// Central in-memory storage
// Responsible for storing:
// - Users (Vendor, MarketOperator, SystemAdministrator)
// - MarketSchedule
// - Waitlists

class InMemoryStorageManager {
public:
    InMemoryStorageManager();

    // Initialization
    void initializeDefaultData();
    bool loadFromDatabase();

    // Utility
    void clear();

    // Manual population helpers
    void addUser(std::unique_ptr<User> user);
    void addMarketDate(const MarketDate& marketDate);
    void addWaitlist(const std::string& marketDateId,
                     VendorCategory category,
                     const Waitlist& waitlist);

    // User Access
    User* getUser(const std::string& userId);
    Vendor* getVendor(const std::string& userId);

    // MarketDate Access
    MarketDate* getMarketDate(const std::string& marketDateId);
    std::vector<MarketDate*> getAllMarketDates();

    // Waitlist Access
    Waitlist* getWaitlist(const std::string& marketDateId,
                          VendorCategory category);

private:
    std::unordered_map<std::string, std::unique_ptr<User>> users;
    MarketSchedule marketSchedule;
    std::map<std::pair<std::string, VendorCategory>, Waitlist> waitlists;
};

#endif // INMEMORYSTORAGEMANAGER_H
