#include "BookingController.h"

#include "InMemoryStorageManager.h"
#include "WaitlistController.h"
#include "User.h"
#include "Vendor.h"
#include "MarketDate.h"
#include "Booking.h"
#include "Notification.h"
#include "VendorCategory.h"

// ---------------------------------------------
// Constructor
// ---------------------------------------------

BookingController::BookingController(InMemoryStorageManager& storageManager,
                                     WaitlistController& waitlistCtrl)
    : storage(storageManager),
      waitlistController(waitlistCtrl)
{
}

std::vector<MarketDate*> BookingController::getAvailableMarketDates() {
    // The Controller asks Storage for the list and hands it to the UI
    return storage.getAllMarketDates(); 
}

// ---------------------------------------------
// bookStall()
// ---------------------------------------------

BookingResult BookingController::bookStall(const std::string& userId,
                                           const std::string& marketDateId)
{
    // 1️⃣ Retrieve User
    User* user = storage.getUser(userId);
    if (!user) {
        return { BookingResultType::INVALID_USER,
                 "User not found." };
    }

    // 2️⃣ Ensure user is Vendor
    Vendor* vendor = dynamic_cast<Vendor*>(user);
    if (!vendor) {
        return { BookingResultType::NOT_VENDOR,
                 "Only vendors may book stalls." };
    }

    // 3️⃣ Retrieve MarketDate
    MarketDate* marketDate = storage.getMarketDate(marketDateId);
    if (!marketDate) {
        return { BookingResultType::INVALID_DATE,
                 "Market date not found." };
    }

    // 4️⃣ Prevent duplicate booking
    if (vendor->hasBookingForDate(marketDateId)) {
        return { BookingResultType::ALREADY_BOOKED,
                 "Vendor already has a booking for this date." };
    }

    // 5️⃣ Check stall availability
    VendorCategory category = vendor->getCategory();

    if (!marketDate->hasAvailableStall(category)) {
        return { BookingResultType::MARKET_FULL,
                 "No stalls available for this category." };
    }

    // 6️⃣ Create booking
    Booking booking(userId, marketDateId, category);

    marketDate->addBooking(booking);
    vendor->addBooking(booking);

    // 7️⃣ Confirmation notification
    Notification notification(
        "Booking confirmed for market date " + marketDateId
    );

    vendor->addNotification(notification);

    return { BookingResultType::SUCCESS,
             "Booking successful." };
}

// ---------------------------------------------
// cancelBooking()
// ---------------------------------------------

BookingResult BookingController::cancelBooking(const std::string& userId,
                                               const std::string& marketDateId)
{
    // 1️⃣ Retrieve User
    User* user = storage.getUser(userId);
    if (!user) {
        return { BookingResultType::INVALID_USER,
                 "User not found." };
    }

    // 2️⃣ Ensure user is Vendor
    Vendor* vendor = dynamic_cast<Vendor*>(user);
    if (!vendor) {
        return { BookingResultType::NOT_VENDOR,
                 "Only vendors may cancel bookings." };
    }

    // 3️⃣ Retrieve MarketDate
    MarketDate* marketDate = storage.getMarketDate(marketDateId);
    if (!marketDate) {
        return { BookingResultType::INVALID_DATE,
                 "Market date not found." };
    }

    // 4️⃣ Ensure booking exists
    if (!vendor->hasBookingForDate(marketDateId)) {
        return { BookingResultType::ERROR,
                 "No booking found for this date." };
    }

    VendorCategory category = vendor->getCategory();

    // 5️⃣ Remove booking
    marketDate->removeBooking(userId);
    vendor->removeBooking(marketDateId);

    // 6️⃣ Cancellation notification
    Notification notification(
        "Booking cancelled for market date " + marketDateId
    );

    vendor->addNotification(notification);

    // 7️⃣ Trigger automatic waitlist promotion
    waitlistController.handlePromotionIfNeeded(marketDateId, category);

    return { BookingResultType::SUCCESS,
             "Booking cancelled successfully." };
}
