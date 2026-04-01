#ifndef BOOKINGCONTROLLER_H
#define BOOKINGCONTROLLER_H

#include <string>
#include <vector>

#include "MarketDate.h"

class InMemoryStorageManager;
class WaitlistController;

// Booking Result Definitions

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

// BookingController
//
// Responsibilities:
// - Coordinates booking operations
// - Books/cancels stalls (only Vendors are allowed)
// - Persists booking-related changes for D2

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
