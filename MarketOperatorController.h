#ifndef MARKETOPERATORCONTROLLER_H
#define MARKETOPERATORCONTROLLER_H

#include <string>
#include <vector>

class InMemoryStorageManager;
class BookingController;
class WaitlistController;
enum class BookingResultType;
enum class WaitlistResultType;

struct MarketOperatorResult {
    bool success;
    std::string message;
};

class MarketOperatorController {
public:
    MarketOperatorController(InMemoryStorageManager& storage, 
                             BookingController& bookingCtrl, 
                             WaitlistController& waitlistCtrl);

    // D2 Required Features 
    MarketOperatorResult bookForVendor(const std::string& vendorId, const std::string& marketDateId);
    MarketOperatorResult cancelForVendor(const std::string& vendorId, const std::string& marketDateId);
    MarketOperatorResult removeFromWaitlistForVendor(const std::string& vendorId, const std::string& marketDateId);

private:
    InMemoryStorageManager& storage;
    BookingController& bookingController;
    WaitlistController& waitlistController;
};

#endif