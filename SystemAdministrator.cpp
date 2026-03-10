#include "SystemAdministrator.h"

// --------------------------------------------------
// Default Constructor
// --------------------------------------------------

SystemAdministrator::SystemAdministrator()
    : User()
{
}

// --------------------------------------------------
// Parameterized Constructor
// --------------------------------------------------

SystemAdministrator::SystemAdministrator(const std::string& id,
                                         const std::string& name,
                                         const std::string& email,
                                         const std::string& phone,
                                         const std::string& address)
    : User(id, name, email, phone, address)
{
}

// --------------------------------------------------
// Role Identification
// --------------------------------------------------

std::string SystemAdministrator::getRoleName() const
{
    return "SystemAdministrator";
}