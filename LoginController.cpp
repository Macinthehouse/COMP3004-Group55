#include "LoginController.h"

/**
 * Constructor for LoginController.
 * * This controller manages the initial access to the system.
 * It strictly follows the "UI -> Controller -> Storage" layered dependency rule.
 */
LoginController::LoginController(InMemoryStorageManager* storage)
    : storage(storage)
{
}

/**
 * Validates a user login by their unique ID.
 * * This method acts as a gateway. Based on the returned User type (Vendor, Operator, Admin),
 * the UI Subsystem decides which dashboard or menu to display.
 * * @param userId The ID entered in the StartupUI.
 * @return A pointer to the User object if found, otherwise nullptr.
 */
User* LoginController::login(const std::string& userId)
{
    if (!storage) {
        return nullptr;
    }
    
    // Fetches the user from the Domain & Storage subsystem.
    // D2 Requirement: This user was reconstructed from SQLite during startup.
    return storage->getUser(userId);
}
