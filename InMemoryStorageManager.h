#ifndef INMEMORYSTORAGEMANAGER_H
#define INMEMORYSTORAGEMANAGER_H

#include <string>
#include <unordered_map>
#include <map>

// Forward declarations
class Vendor;
class MarketDate;
class Waitlist;

// Forward declaration for enum
enum class VendorCategory;

// --------------------------------------------------
// InMemoryStorageManager
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
    // ----------------------------
    // Internal Storage Containers
    // ----------------------------

    // Vendors stored by unique vendorId
    std::unordered_map<std::string, Vendor> vendors;

    // MarketDates stored by YYYY-MM-DD string
    std::unordered_map<std::string, MarketDate> marketDates;

    // Waitlists stored by (marketDateId, VendorCategory)
    std::map<std::pair<std::string, VendorCategory>, Waitlist> waitlists;
};

#endif // INMEMORYSTORAGEMANAGER_H