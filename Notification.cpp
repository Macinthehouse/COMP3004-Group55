#include "Notification.h"

// Default Constructor
// Required for use in STL containers (e.g., vector).

Notification::Notification()
{
}

// Parameterized Constructor
Notification::Notification(const std::string& message)
    : message(message)
{
}

// Getter
std::string Notification::getMessage() const
{
    return message;
}
