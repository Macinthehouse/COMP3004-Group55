#include "MarketDate.h"

// --------------------------------------------------
// Default Constructor
// --------------------------------------------------
// Required for use in STL containers (e.g., unordered_map).
// --------------------------------------------------

MarketDate::MarketDate()
    : maxFoodVendors(2),
      maxArtisanVendors(2)
{
}

// --------------------------------------------------
// Parameterized Constructor
// --------------------------------------------------

MarketDate::MarketDate(const std::string& date,
                       int maxFoodVendors,
                       int maxArtisanVendors)
    : date(date),
      maxFoodVendors(maxFoodVendors),
      maxArtisanVendors(maxArtisanVendors)
{
}

// --------------------------------------------------
// Getter
// --------------------------------------------------

std::string MarketDate::getDate() const
{
    return date;
}

// --------------------------------------------------
// hasAvailableStall()
// --------------------------------------------------
// Enforces capacity rule:
// - Maximum 2 FOOD vendors
// - Maximum 2 ARTISAN vendors
//
// This is where the 2+2 rule lives.
// Controllers must NOT manually count bookings.
// --------------------------------------------------

bool MarketDate::hasAvailableStall(VendorCategory category) const
{
    int foodCount = 0;
    int artisanCount = 0;

    for (const auto& booking : bookings) {
        if (booking.getCategory() == VendorCategory::FOOD) {
            foodCount++;
        } 
        else if (booking.getCategory() == VendorCategory::ARTISAN) {
            artisanCount++;
        }
    }

    if (category == VendorCategory::FOOD) {
        return foodCount < maxFoodVendors;
    }

    if (category == VendorCategory::ARTISAN) {
        return artisanCount < maxArtisanVendors;
    }

    return false;
}

// --------------------------------------------------
// addBooking()
// --------------------------------------------------
// Defensive implementation:
// Even if controller forgets to validate capacity,
// this method will prevent exceeding stall limits.
// --------------------------------------------------

void MarketDate::addBooking(const Booking& booking)
{
    // Ensure capacity still allows booking
    if (hasAvailableStall(booking.getCategory())) {
        bookings.push_back(booking);
    }
}

// --------------------------------------------------
// removeBooking()
// --------------------------------------------------
// Removes booking associated with given vendorId.
// Assumes one booking per vendor per date.
// --------------------------------------------------

void MarketDate::removeBooking(const std::string& vendorId)
{
    for (auto it = bookings.begin(); it != bookings.end(); ++it) {
        if (it->getVendorId() == vendorId) {
            bookings.erase(it);
            break;  // Only one booking per vendor
        }
    }
}

// --------------------------------------------------
// getBookings()
// --------------------------------------------------

const std::vector<Booking>& MarketDate::getBookings() const
{
    return bookings;
}