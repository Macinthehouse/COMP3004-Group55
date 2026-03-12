#include "BookingController.h"

#include "InMemoryStorageManager.h"
#include "WaitlistController.h"
#include "User.h"
#include "Vendor.h"
#include "MarketDate.h"
#include "Booking.h"
#include "Notification.h"
#include "VendorCategory.h"
#include "Waitlist.h"

// Constructor

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

// bookStall()

BookingResult BookingController::bookStall(const std::string& userId,
                                          const std::string& marketDateId)
{
    // Retrieve User
    User* user = storage.getUser(userId);
    if (!user) {
        return { BookingResultType::INVALID_USER,
                 "User not found." };
    }

    // Ensure user is Vendor
    Vendor* vendor = dynamic_cast<Vendor*>(user);
    if (!vendor) {
        return { BookingResultType::NOT_VENDOR,
                 "Only vendors may book stalls." };
    }

    // Retrieve MarketDate
    MarketDate* marketDate = storage.getMarketDate(marketDateId);
    if (!marketDate) {
        return { BookingResultType::INVALID_DATE,
                 "Market date not found." };
    }

    // Prevent duplicate booking
    if (vendor->hasBookingForDate(marketDateId)) {
        return { BookingResultType::ALREADY_BOOKED,
                 "Vendor already has a booking for this date." };
    }

    // Check stall availability
    VendorCategory category = vendor->getCategory();

    if (!marketDate->hasAvailableStall(category)) {
        return { BookingResultType::MARKET_FULL,
                 "No stalls available for this category." };
    }

    // Enforce waitlist priority (if a waitlist exists and is non-empty)
    Waitlist* wl = storage.getWaitlist(marketDateId, category);
    if (wl && !wl->isEmpty()) {
        const std::string headId = wl->peekNextVendorId();
        if (!headId.empty() && headId != userId) {
            return { BookingResultType::ERROR,
                     "A waitlist exists for " + marketDateId +
                     ". Only the next vendor in line may book right now." };
        }
    }

    // Create booking
    Booking booking(userId, marketDateId, category);

    marketDate->addBooking(booking);
    vendor->addBooking(booking);

    // If this vendor was next-in-line, remove them from the waitlist now
    if (wl && !wl->isEmpty() && wl->peekNextVendorId() == userId) {
        wl->dequeue();
        waitlistController.notifyVendorsMovedUp(marketDateId, category, 1);
    }

    // Confirmation notification
    Notification notification(
        "Booking confirmed for market date " + marketDateId
    );
    vendor->addNotification(notification);

    return { BookingResultType::SUCCESS,
             "Booking successful." };
}

// cancelBooking()

BookingResult BookingController::cancelBooking(const std::string& userId,
                                               const std::string& marketDateId)
{
    // Retrieve User
    User* user = storage.getUser(userId);
    if (!user) {
        return { BookingResultType::INVALID_USER,
                 "User not found." };
    }

    // Ensure user is Vendor
    Vendor* vendor = dynamic_cast<Vendor*>(user);
    if (!vendor) {
        return { BookingResultType::NOT_VENDOR,
                 "Only vendors may cancel bookings." };
    }

    // Retrieve MarketDate
    MarketDate* marketDate = storage.getMarketDate(marketDateId);
    if (!marketDate) {
        return { BookingResultType::INVALID_DATE,
                 "Market date not found." };
    }

    // Ensure booking exists
    if (!vendor->hasBookingForDate(marketDateId)) {
        return { BookingResultType::ERROR,
                 "No booking found for this date." };
    }

    VendorCategory category = vendor->getCategory();

    // Remove booking
    marketDate->removeBooking(userId);
    vendor->removeBooking(marketDateId);

    // Cancellation notification
    Notification notification(
        "Booking cancelled for market date " + marketDateId
    );

    vendor->addNotification(notification);

    // Trigger automatic waitlist promotion
    waitlistController.handlePromotionIfNeeded(marketDateId, category);

    return { BookingResultType::SUCCESS,
             "Booking cancelled successfully." };
}
