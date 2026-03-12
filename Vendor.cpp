#include "Vendor.h"
#include "VendorCategory.h"
#include "Booking.h"
#include "Notification.h"
#include "ComplianceDocument.h"


// Default Constructor
// Required for use in STL containers.

Vendor::Vendor()
    : User(),  // Call base class default constructor
      category(VendorCategory::FOOD)
{
}

// Parameterized Constructor
// Initializes a vendor with full business information.
// Identity fields are initialized via User base class.

Vendor::Vendor(const std::string& id,
               const std::string& ownerName,
               const std::string& email,
               const std::string& phone,
               const std::string& address,
               const std::string& businessName,
               VendorCategory category)
    : User(id, ownerName, email, phone, address),
      businessName(businessName),
      category(category)
{
}

// Role Identification

std::string Vendor::getRoleName() const
{
    return "Vendor";
}

// Vendor-Specific Getters

std::string Vendor::getBusinessName() const
{
    return businessName;
}

VendorCategory Vendor::getCategory() const
{
    return category;
}

// Booking Management

bool Vendor::hasBookingForDate(const std::string& marketDateId) const
{
    for (const auto& booking : bookings) {
        if (booking.getMarketDateId() == marketDateId) {
            return true;
        }
    }
    return false;
}

void Vendor::addBooking(const Booking& booking)
{
    // Prevent duplicate booking for same date
    if (!hasBookingForDate(booking.getMarketDateId())) {
        bookings.push_back(booking);
    }
}

void Vendor::removeBooking(const std::string& marketDateId)
{
    for (auto it = bookings.begin(); it != bookings.end(); ++it) {
        if (it->getMarketDateId() == marketDateId) {
            bookings.erase(it);
            break;  // Only one booking per date allowed
        }
    }
}

const std::vector<Booking>& Vendor::getBookings() const
{
    return bookings;
}

// Notification Management

void Vendor::addNotification(const Notification& notification)
{
    notifications.push_back(notification);
}

const std::vector<Notification>& Vendor::getNotifications() const
{
    return notifications;
}

// Compliance Management

void Vendor::addComplianceDocument(const ComplianceDocument& document)
{
    complianceDocuments.push_back(document);
}

const std::vector<ComplianceDocument>& Vendor::getComplianceDocuments() const
{
    return complianceDocuments;
}
