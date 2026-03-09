#ifndef INMEMORYSTORAGEMANAGER_H
#define INMEMORYSTORAGEMANAGER_H

#include <string>
#include <unordered_map>
#include <map>

#include "VendorCategory.h"

// Forward declarations
class Vendor;
class MarketDate;
class Waitlist;

// --------------------------------------------------
// InMemoryStorageManager
// --------------------------------------------------
// Central in-memory storage for Deliverable 1.
// Responsible for storing:
// - Vendors
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
    // Vendor Access
    // ----------------------------
    Vendor* getVendor(const std::string& vendorId);

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
    // Vendors stored by vendorId
    std::unordered_map<std::string, Vendor> vendors;

    // MarketDates stored by YYYY-MM-DD
    std::unordered_map<std::string, MarketDate> marketDates;

    // Waitlists stored by (date, category)
    std::map<std::pair<std::string, VendorCategory>, Waitlist> waitlists;
};

#endif // INMEMORYSTORAGEMANAGER_H