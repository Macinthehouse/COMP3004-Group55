#ifndef VENDOR_H
#define VENDOR_H

#include <string>
#include <vector>

// Forward declarations to avoid circular dependencies
class Booking;
class Notification;

// --------------------------------------------------
// VendorCategory Enum
// --------------------------------------------------
// Represents the type of vendor allowed in the market.
// Used for stall capacity rules and waitlist separation.
// --------------------------------------------------

enum class VendorCategory {
    FOOD,
    ARTISAN
};

// --------------------------------------------------
// Vendor Class
// --------------------------------------------------
// Represents a vendor user in the HintonMarket system.
//
// Responsibilities:
// - Store vendor identity and business information
// - Track active stall bookings
// - Track system notifications
// - Provide booking-related validation (e.g., duplicate booking check)
//
// IMPORTANT:
// - No UI logic here
// - No storage logic here
// - No controller logic here
// - Pure domain entity
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

    // Checks whether the vendor already has a booking
    // for a specific market date.
    bool hasBookingForDate(const std::string& marketDateId) const;

    // Adds a new booking to the vendor.
    // Assumes controller has validated business rules.
    void addBooking(const Booking& booking);

    // Removes an existing booking for a specific market date.
    void removeBooking(const std::string& marketDateId);

    // Returns read-only access to bookings (for dashboard display).
    const std::vector<Booking>& getBookings() const;

    // --------------------------------------------------
    // Notification Management
    // --------------------------------------------------

    // Adds a system notification to the vendor.
    void addNotification(const Notification& notification);

    // Returns read-only access to notifications.
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

    std::vector<Booking> bookings;          // Active stall bookings
    std::vector<Notification> notifications; // System notifications
};

#endif // VENDOR_H
