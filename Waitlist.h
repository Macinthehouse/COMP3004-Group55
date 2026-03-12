#ifndef WAITLIST_H
#define WAITLIST_H

#include <string>
#include <vector>
#include "VendorCategory.h"

// Waitlist Class

// Represents a waitlist for a specific:
//      - MarketDate
//      - VendorCategory (FOOD or ARTISAN)

// Responsibilities:
// - Maintain FIFO ordering
// - Allow enqueue (add to end)
// - Allow dequeue (remove from front)
// - Allow removal by vendorId
// - Provide queue position

class Waitlist {
public:
    // Constructors
    Waitlist();  

    Waitlist(const std::string& marketDateId,
             VendorCategory category);

    // Core Queue Operations
    void enqueue(const std::string& vendorId);
    std::string dequeue();
    void remove(const std::string& vendorId);

    // Utility Methods
    bool containsVendor(const std::string& vendorId) const;
    int getPosition(const std::string& vendorId) const;  // 1-based index
    bool isEmpty() const;
    std::string peekNextVendorId() const;
    std::vector<std::string> getQueueSnapshot() const;

    // Getters
    std::string getMarketDateId() const;
    VendorCategory getCategory() const;

private:
    std::string marketDateId;
    VendorCategory category;

    std::vector<std::string> queue;  // FIFO structure
};

#endif // WAITLIST_H
