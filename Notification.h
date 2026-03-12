#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>

// --------------------------------------------------
// Notification Class
// --------------------------------------------------
// Represents a system-generated notification for a vendor.
// Responsibilities:
// - Store notification message
// - Provide read-only access to message
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
