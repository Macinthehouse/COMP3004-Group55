#ifndef BOOKINGCONTROLLER_H
#define BOOKINGCONTROLLER_H

#include <string>
#include <vector>

#include "MarketDate.h"

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
    NOT_VENDOR,
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

    std::vector<MarketDate*> getAvailableMarketDates();

private:
    InMemoryStorageManager& storage;
    WaitlistController& waitlistController;
};

#endif // BOOKINGCONTROLLER_H
