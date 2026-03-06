#ifndef WAITLISTCONTROLLER_H
#define WAITLISTCONTROLLER_H

#include <string>

// Forward declarations
class InMemoryStorageManager;

// Forward declaration for enum usage
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
// WaitlistController Declaration
// ----------------------------

class WaitlistController {
public:
    // Constructor with dependency injection
    WaitlistController(InMemoryStorageManager& storageManager);

    // Add vendor to waitlist
    WaitlistResult joinWaitlist(const std::string& vendorId,
                                const std::string& marketDateId);

    // Remove vendor from waitlist
    WaitlistResult leaveWaitlist(const std::string& vendorId,
                                 const std::string& marketDateId);

    // Automatic promotion when stall becomes available
    void handlePromotionIfNeeded(const std::string& marketDateId,
                                 VendorCategory category);

private:
    InMemoryStorageManager& storage;  // Shared in-memory storage
};

#endif // WAITLISTCONTROLLER_H