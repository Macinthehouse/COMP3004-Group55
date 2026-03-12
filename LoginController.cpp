#include "LoginController.h"

LoginController::LoginController(InMemoryStorageManager* storage) 
    : storage(storage) {}

User* LoginController::login(const std::string& username) {
    // handles the logic of retrieval
    User* user = storage->getUser(username);
    
    if (user != nullptr) {
        // return the user if they exist in seeded data
        return user;
    }
    
    return nullptr;
}
