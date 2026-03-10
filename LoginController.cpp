#include "LoginController.h"

LoginController::LoginController(InMemoryStorageManager* storage) 
    : storage(storage) {}

User* LoginController::login(const std::string& username) {
    // The Controller handles the logic of retrieval
    User* user = storage->getUser(username);
    
    if (user != nullptr) {
        // Here you could add logic for Deliverable 2 later (like password checks)
        // For D1, we just return the user if they exist in our seeded data
        return user;
    }
    
    return nullptr;
}