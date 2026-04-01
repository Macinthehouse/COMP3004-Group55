#include "LoginController.h"

LoginController::LoginController(InMemoryStorageManager* storage)
    : storage(storage)
{
}

User* LoginController::login(const std::string& userId)
{
    if (!storage) {
        return nullptr;
    }

    return storage->getUser(userId);
}
