#include "User.h"

// --------------------------------------------------
// Default Constructor
// --------------------------------------------------

User::User()
{
}

// --------------------------------------------------
// Parameterized Constructor
// --------------------------------------------------

User::User(const std::string& id,
           const std::string& name,
           const std::string& email,
           const std::string& phone,
           const std::string& address)
    : id(id),
      name(name),
      email(email),
      phone(phone),
      address(address)
{
}

// --------------------------------------------------
// Getters
// --------------------------------------------------

std::string User::getId() const
{
    return id;
}

std::string User::getName() const
{
    return name;
}

std::string User::getEmail() const
{
    return email;
}

std::string User::getPhone() const
{
    return phone;
}

std::string User::getAddress() const
{
    return address;
}