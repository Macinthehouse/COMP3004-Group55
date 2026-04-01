#ifndef LOGINCONTROLLER_H
#define LOGINCONTROLLER_H

#include <string>
#include "InMemoryStorageManager.h"
#include "User.h"

class LoginController {
public:
    LoginController(InMemoryStorageManager* storage);

    // The UI calls this to verify a user by ID
    User* login(const std::string& userId);

private:
    InMemoryStorageManager* storage;
};

#endif // LOGINCONTROLLER_H
