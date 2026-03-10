#ifndef MARKETOPERATOR_H
#define MARKETOPERATOR_H

#include "User.h"

// --------------------------------------------------
// MarketOperator Class
// --------------------------------------------------
// Represents a Market Operator user.
//
// Responsibilities:
// - Identify as Market Operator
// - Provide role name
//
// NOTE:
// - No booking logic
// - No compliance logic
// - No waitlist logic
// - Pure role-based user type
// --------------------------------------------------

class MarketOperator : public User {
public:
    // --------------------------------------------------
    // Constructors
    // --------------------------------------------------

    MarketOperator();

    MarketOperator(const std::string& id,
                   const std::string& name,
                   const std::string& email,
                   const std::string& phone,
                   const std::string& address);

    // --------------------------------------------------
    // Role Identification
    // --------------------------------------------------

    std::string getRoleName() const override;
};

#endif // MARKETOPERATOR_H