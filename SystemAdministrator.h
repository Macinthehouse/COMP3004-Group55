#ifndef SYSTEMADMINISTRATOR_H
#define SYSTEMADMINISTRATOR_H

#include "User.h"

// SystemAdministrator Class

// Represents a System Administrator user.
//
// Responsibilities:
// - Identify as System Administrator
// - Provide role name

class SystemAdministrator : public User {
public:

    // Constructors
    SystemAdministrator();

    SystemAdministrator(const std::string& id,
                        const std::string& name,
                        const std::string& email,
                        const std::string& phone,
                        const std::string& address);

    // Role Identification
    std::string getRoleName() const override;
};

#endif // SYSTEMADMINISTRATOR_H
