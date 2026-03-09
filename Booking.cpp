#include "Booking.h"
#include "VendorCategory.h"

// --------------------------------------------------
// Default Constructor
// --------------------------------------------------
// Required for use in STL containers (e.g., vector).
// --------------------------------------------------

Booking::Booking()
    : category(VendorCategory::FOOD) // Default value
{
}

// --------------------------------------------------
// Parameterized Constructor
// --------------------------------------------------

Booking::Booking(const std::string& vendorId,
                 const std::string& marketDateId,
                 VendorCategory category)
    : vendorId(vendorId),
      marketDateId(marketDateId),
      category(category)
{
}

// --------------------------------------------------
// Getters
// --------------------------------------------------

std::string Booking::getVendorId() const
{
    return vendorId;
}

std::string Booking::getMarketDateId() const
{
    return marketDateId;
}

VendorCategory Booking::getCategory() const
{
    return category;
}