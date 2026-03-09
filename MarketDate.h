#ifndef MARKETDATE_H
#define MARKETDATE_H

#include <string>
#include <vector>

#include "VendorCategory.h"
#include "Booking.h"

// --------------------------------------------------
// MarketDate Class
// --------------------------------------------------
// Represents a single market date in the system.
//
// Responsibilities:
// - Store market date (YYYY-MM-DD format)
// - Enforce stall capacity rules:
//      • Max 2 FOOD vendors
//      • Max 2 ARTISAN vendors
// - Store confirmed bookings
//
// IMPORTANT:
// - Capacity rules enforced here
// - No controller logic
// - No UI logic
// - Pure domain entity
// --------------------------------------------------

class MarketDate {
public:
    // --------------------------------------------------
    // Constructors
    // --------------------------------------------------

    MarketDate();  // Required for STL containers

    MarketDate(const std::string& date,
               int maxFoodVendors,
               int maxArtisanVendors);

    // --------------------------------------------------
    // Getters
    // --------------------------------------------------

    std::string getDate() const;

    // --------------------------------------------------
    // Capacity & Booking Management
    // --------------------------------------------------

    // Returns true if a stall is available
    // for the given vendor category.
    bool hasAvailableStall(VendorCategory category) const;

    // Adds a confirmed booking.
    // Defensive capacity check inside implementation.
    void addBooking(const Booking& booking);

    // Removes booking for given vendorId.
    void removeBooking(const std::string& vendorId);

    // Returns read-only list of bookings.
    const std::vector<Booking>& getBookings() const;

private:
    std::string date;

    int maxFoodVendors;
    int maxArtisanVendors;

    std::vector<Booking> bookings;
};

#endif // MARKETDATE_H