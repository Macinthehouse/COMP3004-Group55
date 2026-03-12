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
