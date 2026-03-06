#ifndef BOOKINGCONTROLLER_H
#define BOOKINGCONTROLLER_H

#include <string>

// Forward declarations
class InMemoryStorageManager;
class Vendor;
class MarketDate;
class WaitlistController; 

// ----------------------------
// Booking Result Definitions
// ----------------------------

enum class BookingResultType {
    SUCCESS,
    MARKET_FULL,
    ALREADY_BOOKED,
    INVALID_VENDOR,
    INVALID_DATE,
    ERROR
};

struct BookingResult {
    BookingResultType type;
    std::string message;
};

// ----------------------------
// BookingController Declaration
// ----------------------------

class BookingController {
public:
    // Constructor with dependency injection
    BookingController(InMemoryStorageManager& storageManager,
                      WaitlistController& waitlistController); 

    // Book a stall for a vendor on a specific market date
    BookingResult bookStall(const std::string& vendorId,
                            const std::string& marketDateId);

    // Cancel a stall booking
    BookingResult cancelBooking(const std::string& vendorId,
                                const std::string& marketDateId);

private:
    InMemoryStorageManager& storage;  // Reference to in-memory data manager
    WaitlistController& waitlistController;  
};

#endif // BOOKINGCONTROLLER_H
