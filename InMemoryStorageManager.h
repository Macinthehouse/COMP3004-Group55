#ifndef INMEMORYSTORAGEMANAGER_H
#define INMEMORYSTORAGEMANAGER_H

#include <string>
#include <unordered_map>
#include <map>
#include <memory>

#include "User.h"
#include "Vendor.h"
#include "MarketOperator.h"
#include "SystemAdministrator.h"
#include "VendorCategory.h"
#include "MarketDate.h"
#include "Waitlist.h"

// --------------------------------------------------
// InMemoryStorageManager
// --------------------------------------------------
// Central in-memory storage for Deliverable 1.
// Responsible for storing:
// - Users (Vendor, MarketOperator, SystemAdministrator)
// - MarketDates
// - Waitlists
//
// No business rules live here.
// No UI logic.
// Pure data storage.
// --------------------------------------------------

class InMemoryStorageManager {
public:
    InMemoryStorageManager();

    // ----------------------------
    // Initialization
    // ----------------------------
    void initializeDefaultData();

    // ----------------------------
    // User Access
    // ----------------------------
    User* getUser(const std::string& userId);

    Vendor* getVendor(const std::string& userId);

    // ----------------------------
    // MarketDate Access
    // ----------------------------
    MarketDate* getMarketDate(const std::string& marketDateId);

    // ----------------------------
    // Waitlist Access
    // ----------------------------
    Waitlist* getWaitlist(const std::string& marketDateId,
                          VendorCategory category);

private:
    // Polymorphic user storage
    std::unordered_map<std::string, std::unique_ptr<User>> users;

    // MarketDates stored by YYYY-MM-DD
    std::unordered_map<std::string, MarketDate> marketDates;

    // Waitlists stored by (date, category)
    std::map<std::pair<std::string, VendorCategory>, Waitlist> waitlists;
};

#endif // INMEMORYSTORAGEMANAGER_H
