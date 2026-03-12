#ifndef VENDOR_H
#define VENDOR_H

#include <string>
#include <vector>

#include "User.h"
#include "VendorCategory.h"
#include "Booking.h"
#include "Notification.h"
#include "ComplianceDocument.h"

// --------------------------------------------------
// Vendor Class (Inherits from User)
// --------------------------------------------------
// Represents a vendor user in the HintonMarket system.
//
// Responsibilities:
// - Store vendor business information
// - Track active stall bookings
// - Track system notifications
// - Track compliance documents
// - Provide booking-related validation
// --------------------------------------------------

class Vendor : public User {
public:
    // --------------------------------------------------
    // Constructors
    // --------------------------------------------------

    Vendor();

    Vendor(const std::string& id,
           const std::string& ownerName,
           const std::string& email,
           const std::string& phone,
           const std::string& address,
           const std::string& businessName,
           VendorCategory category);

    // --------------------------------------------------
    // Role Identification
    // --------------------------------------------------

    std::string getRoleName() const override;

    // --------------------------------------------------
    // Vendor-Specific Getters
    // --------------------------------------------------

    std::string getBusinessName() const;
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

    // --------------------------------------------------
    // Compliance Management
    // --------------------------------------------------

    void addComplianceDocument(const ComplianceDocument& document);
    const std::vector<ComplianceDocument>& getComplianceDocuments() const;

private:
    std::string businessName;
    VendorCategory category;

    std::vector<Booking> bookings;
    std::vector<Notification> notifications;
    std::vector<ComplianceDocument> complianceDocuments;
};

#endif // VENDOR_H
