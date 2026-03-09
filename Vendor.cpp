#include "Vendor.h"
#include "VendorCategory.h"
#include "Booking.h"
#include "Notification.h"

// --------------------------------------------------
// Default Constructor
// --------------------------------------------------
// Required for use in STL containers (e.g., unordered_map).
// --------------------------------------------------

Vendor::Vendor()
    : category(VendorCategory::FOOD) // Default category (can be overridden)
{
}

// --------------------------------------------------
// Parameterized Constructor
// --------------------------------------------------
// Initializes a vendor with full business information.
// --------------------------------------------------

Vendor::Vendor(const std::string& id,
               const std::string& businessName,
               const std::string& ownerName,
               const std::string& email,
               const std::string& phone,
               const std::string& address,
               VendorCategory category)
    : id(id),
      businessName(businessName),
      ownerName(ownerName),
      email(email),
      phone(phone),
      address(address),
      category(category)
{
}

// --------------------------------------------------
// Getters
// --------------------------------------------------

std::string Vendor::getId() const {
    return id;
}

std::string Vendor::getBusinessName() const {
    return businessName;
}

std::string Vendor::getOwnerName() const {
    return ownerName;
}

std::string Vendor::getEmail() const {
    return email;
}

std::string Vendor::getPhone() const {
    return phone;
}

std::string Vendor::getAddress() const {
    return address;
}

VendorCategory Vendor::getCategory() const {
    return category;
}

// --------------------------------------------------
// Booking Management
// --------------------------------------------------

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
    bookings.push_back(booking);
}

void Vendor::removeBooking(const std::string& marketDateId)
{
    for (auto it = bookings.begin(); it != bookings.end(); ++it) {
        if (it->getMarketDateId() == marketDateId) {
            bookings.erase(it);
            break; // Only one booking per date allowed
        }
    }
}

const std::vector<Booking>& Vendor::getBookings() const
{
    return bookings;
}

// --------------------------------------------------
// Notification Management
// --------------------------------------------------

void Vendor::addNotification(const Notification& notification)
{
    notifications.push_back(notification);
}

const std::vector<Notification>& Vendor::getNotifications() const
{
    return notifications;
}