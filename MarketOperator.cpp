#include "MarketOperator.h"

// --------------------------------------------------
// Default Constructor
// --------------------------------------------------

MarketOperator::MarketOperator()
    : User()
{
}

// --------------------------------------------------
// Parameterized Constructor
// --------------------------------------------------

MarketOperator::MarketOperator(const std::string& id,
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

std::string MarketOperator::getRoleName() const
{
    return "MarketOperator";
}