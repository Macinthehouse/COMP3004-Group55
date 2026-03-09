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
    ERROR
};

struct WaitlistResult {
    WaitlistResultType type;
    std::string message;
    int queuePosition;  // -1 if not applicable
};

// ----------------------------
// WaitlistController
// ----------------------------
// Responsibilities:
// - Manage waitlist placement
// - Handle removal from waitlist
// - Automatically promote vendors when stalls free
//
// IMPORTANT:
// - No UI logic
// - No storage logic
// - Coordinates between domain objects
// ----------------------------

class WaitlistController {
public:
    WaitlistController(InMemoryStorageManager& storageManager);

    WaitlistResult joinWaitlist(const std::string& vendorId,
                                const std::string& marketDateId);

    WaitlistResult leaveWaitlist(const std::string& vendorId,
                                 const std::string& marketDateId);

    void handlePromotionIfNeeded(const std::string& marketDateId,
                                 VendorCategory category);

private:
    InMemoryStorageManager& storage;
};

#endif // WAITLISTCONTROLLER_H