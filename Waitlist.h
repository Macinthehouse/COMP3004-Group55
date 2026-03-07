#ifndef WAITLIST_H
#define WAITLIST_H

#include <string>
#include <vector>

#include "Vendor.h"   // For VendorCategory

// --------------------------------------------------
// Waitlist Class
// --------------------------------------------------
// Represents a waitlist for a specific:
//      - MarketDate
//      - VendorCategory (FOOD or ARTISAN)
//
// Responsibilities:
// - Maintain FIFO ordering
// - Allow enqueue (add to end)
// - Allow dequeue (remove from front)
// - Allow removal by vendorId
// - Provide queue position
//
// IMPORTANT:
// - No controller logic
// - No UI logic
// - Pure domain entity
// --------------------------------------------------

class Waitlist {
public:
    // --------------------------------------------------
    // Constructors
    // --------------------------------------------------

    Waitlist();  // Required for STL containers

    Waitlist(const std::string& marketDateId,
             VendorCategory category);

    // --------------------------------------------------
    // Core Queue Operations
    // --------------------------------------------------

    // Adds vendor to end of queue
    void enqueue(const std::string& vendorId);

    // Removes and returns first vendor (FIFO)
    std::string dequeue();

    // Removes specific vendor from queue
    void remove(const std::string& vendorId);

    // --------------------------------------------------
    // Utility Methods
    // --------------------------------------------------

    bool containsVendor(const std::string& vendorId) const;

    // Returns 1-based position in queue.
    // Returns -1 if not found.
    int getPosition(const std::string& vendorId) const;

    bool isEmpty() const;

    // Getters
    std::string getMarketDateId() const;
    VendorCategory getCategory() const;

private:
    std::string marketDateId;
    VendorCategory category;

    // FIFO queue storage
    std::vector<std::string> queue;
};

#endif // WAITLIST_H