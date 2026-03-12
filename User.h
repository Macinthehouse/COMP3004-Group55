#ifndef USER_H
#define USER_H

#include <string>

// User (Abstract Base Class)

// Represents a generic system user.
//
// Responsibilities:
// - Store common identity information
// - Provide role identification

class User {
public:
    
// Constructors

    User();  // Required for containers (if needed)

    User(const std::string& id,
         const std::string& name,
         const std::string& email,
         const std::string& phone,
         const std::string& address);

    virtual ~User() = default;  // Required for polymorphism

    // Getters
    std::string getId() const;
    std::string getName() const;
    std::string getEmail() const;
    std::string getPhone() const;
    std::string getAddress() const;

    // Role Identification (Pure Virtual)
    virtual std::string getRoleName() const = 0;

protected:
    // Common Identity Information

    std::string id;
    std::string name;
    std::string email;
    std::string phone;
    std::string address;
};

#endif // USER_H
