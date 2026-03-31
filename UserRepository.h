#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include <memory>
#include <string>
#include <vector>

class User;

class UserRepository
{
public:
    std::unique_ptr<User> findById(const std::string& id);
    std::vector<std::unique_ptr<User>> loadAllUsers();
};

#endif // USERREPOSITORY_H
