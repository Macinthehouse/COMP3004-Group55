#ifndef BOOKINGCONTROLLER_H
#define BOOKINGCONTROLLER_H

#include <string>

// Forward declarations
class InMemoryStorageManager;
class WaitlistController;

// ----------------------------
// Booking Result Definitions
// ----------------------------

enum class BookingResultType {
    SUCCESS,
    MARKET_FULL,
    ALREADY_BOOKED,
    INVALID_USER,
    INVALID_DATE,
    NOT_VENDOR,      // NEW
    ERROR
};

struct BookingResult {
    BookingResultType type;
    std::string message;
};

// --------------------------------------------------
// BookingController
// --------------------------------------------------
// Coordinates booking operations.
// Only Vendors are allowed to book/cancel stalls.
// --------------------------------------------------

class BookingController {
public:
    BookingController(InMemoryStorageManager& storageManager,
                      WaitlistController& waitlistController);

    BookingResult bookStall(const std::string& userId,
                            const std::string& marketDateId);

    BookingResult cancelBooking(const std::string& userId,
                                const std::string& marketDateId);

private:
    InMemoryStorageManager& storage;
    WaitlistController& waitlistController;
};

#endif // BOOKINGCONTROLLER_H
