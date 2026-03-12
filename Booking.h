#ifndef BOOKING_H
#define BOOKING_H

#include <string>
#include "VendorCategory.h"

// --------------------------------------------------
// Booking Class
// --------------------------------------------------
// Represents a confirmed stall booking for a vendor.
//
// Responsibilities:
// - Store vendorId
// - Store marketDateId
// - Store vendor category (Food or Artisan)
// --------------------------------------------------

class Booking {
public:
    // --------------------------------------------------
    // Constructors
    // --------------------------------------------------

    Booking();  // Required for STL containers

    Booking(const std::string& vendorId,
            const std::string& marketDateId,
            VendorCategory category);

    // --------------------------------------------------
    // Getters
    // --------------------------------------------------

    std::string getVendorId() const;
    std::string getMarketDateId() const;
    VendorCategory getCategory() const;

private:
    std::string vendorId;
    std::string marketDateId;
    VendorCategory category;
};

#endif // BOOKING_H
