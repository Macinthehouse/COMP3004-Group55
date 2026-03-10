#ifndef WAITLISTCONTROLLER_H
#define WAITLISTCONTROLLER_H

#include <string>

// Forward declarations
class InMemoryStorageManager;
enum class VendorCategory;

// ----------------------------
// Waitlist Result Definitions
// ----------------------------

enum class WaitlistResultType {
    SUCCESS,
    ALREADY_BOOKED,
    ALREADY_IN_WAITLIST,
    NOT_FOUND,
    MARKET_NOT_FULL,
    NOT_VENDOR,      // NEW: prevents admin/operator misuse
    ERROR
};

struct WaitlistResult {
    WaitlistResultType type;
    std::string message;
    int queuePosition;  // -1 if not applicable
};

// --------------------------------------------------
// WaitlistController
// --------------------------------------------------
// Coordinates waitlist operations.
// Only Vendors are allowed to join/leave waitlists.
// --------------------------------------------------

class WaitlistController {
public:
    WaitlistController(InMemoryStorageManager& storageManager);

    WaitlistResult joinWaitlist(const std::string& userId,
                                const std::string& marketDateId);

    WaitlistResult leaveWaitlist(const std::string& userId,
                                 const std::string& marketDateId);

    void handlePromotionIfNeeded(const std::string& marketDateId,
                                 VendorCategory category);

private:
    InMemoryStorageManager& storage;
};

#endif // WAITLISTCONTROLLER_H
