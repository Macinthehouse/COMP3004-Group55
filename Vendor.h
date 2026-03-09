#ifndef VENDOR_H
#define VENDOR_H

#include <string>
#include <vector>
#include "VendorCategory.h"

// Forward declarations to avoid circular dependencies
class Booking;
class Notification;

// --------------------------------------------------
// Vendor Class
// --------------------------------------------------
// Represents a vendor user in the HintonMarket system.
//
// Responsibilities:
// - Store vendor identity and business information
// - Track active stall bookings
// - Track system notifications
// - Provide booking-related validation
// --------------------------------------------------

class Vendor {
public:
    // --------------------------------------------------
    // Constructors
    // --------------------------------------------------

    Vendor();  // Default constructor (required for containers)

    Vendor(const std::string& id,
           const std::string& businessName,
           const std::string& ownerName,
           const std::string& email,
           const std::string& phone,
           const std::string& address,
           VendorCategory category);

    // --------------------------------------------------
    // Getters
    // --------------------------------------------------

    std::string getId() const;
    std::string getBusinessName() const;
    std::string getOwnerName() const;
    std::string getEmail() const;
    std::string getPhone() const;
    std::string getAddress() const;
    VendorCategory getCategory() const;

    // --------------------------------------------------
    // Booking Management
    // --------------------------------------------------

    bool hasBookingForDate(const std::string& marketDateId) const;

    void addBooking(const Booking& booking);

    void removeBooking(const std::string& marketDateId);

    const std::vector<Booking>& getBookings() const;

    // --------------------------------------------------
    // Notification Management
    // --------------------------------------------------

    void addNotification(const Notification& notification);

    const std::vector<Notification>& getNotifications() const;

private:
    // --------------------------------------------------
    // Identity Information
    // --------------------------------------------------

    std::string id;
    std::string businessName;
    std::string ownerName;
    std::string email;
    std::string phone;
    std::string address;

    VendorCategory category;

    // --------------------------------------------------
    // Vendor State
    // --------------------------------------------------

    std::vector<Booking> bookings;
    std::vector<Notification> notifications;
};

#endif // VENDOR_H