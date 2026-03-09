#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>

// --------------------------------------------------
// Notification Class
// --------------------------------------------------
// Represents a system-generated notification for a vendor.
//
// Examples:
// - Booking confirmed
// - Booking cancelled
// - Added to waitlist
// - Automatically promoted from waitlist
//
// Responsibilities:
// - Store notification message
// - Provide read-only access to message
//
// IMPORTANT:
// - No UI logic
// - No business logic
// - No controller logic
// - Pure data object
// --------------------------------------------------

class Notification {
public:
    // --------------------------------------------------
    // Constructors
    // --------------------------------------------------

    Notification();  // Required for STL containers

    explicit Notification(const std::string& message);

    // --------------------------------------------------
    // Getter
    // --------------------------------------------------

    std::string getMessage() const;

private:
    std::string message;
};

#endif // NOTIFICATION_H