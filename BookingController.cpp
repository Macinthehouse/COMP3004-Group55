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

#include "BookingRepository.h"
#include "WaitlistRepository.h"
#include "NotificationRepository.h"

// Constructor

BookingController::BookingController(InMemoryStorageManager& storageManager,
                                     WaitlistController& waitlistCtrl)
    : storage(storageManager),
      waitlistController(waitlistCtrl)
{
}

std::vector<MarketDate*> BookingController::getAvailableMarketDates()
{
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

    // Enforce one active booking total, not just same-date duplicate
    if (!vendor->getBookings().empty()) {
        return { BookingResultType::ALREADY_BOOKED,
                 "Vendor already has an active booking." };
    }

    VendorCategory category = vendor->getCategory();

    // Check stall availability
    if (!marketDate->hasAvailableStall(category)) {
        return { BookingResultType::MARKET_FULL,
                 "No stalls available for this category." };
    }

    // Enforce waitlist priority
    Waitlist* wl = storage.getWaitlist(marketDateId, category);
    if (wl && !wl->isEmpty()) {
        const std::string headId = wl->peekNextVendorId();
        if (!headId.empty() && headId != userId) {
            return { BookingResultType::ERROR,
                     "A waitlist exists for " + marketDateId +
                     ". Only the next vendor in line may book right now." };
        }
    }

    Booking booking(userId, marketDateId, category);

    BookingRepository bookingRepository;
    WaitlistRepository waitlistRepository;
    NotificationRepository notificationRepository;

    // Persist booking first
    if (!bookingRepository.createBooking(booking)) {
        return { BookingResultType::ERROR,
                 "Failed to save booking to the database." };
    }

    // If this vendor was next-in-line, persist waitlist update
    if (wl && !wl->isEmpty() && wl->peekNextVendorId() == userId) {
        Waitlist updatedWaitlist = *wl;
        updatedWaitlist.dequeue();

        if (!waitlistRepository.replaceWaitlist(updatedWaitlist)) {
            return { BookingResultType::ERROR,
                     "Failed to update waitlist in the database." };
        }

        *wl = updatedWaitlist;
        waitlistController.notifyVendorsMovedUp(marketDateId, category, 1);
    }

    // Persist notification
    const std::string notificationMessage =
        "Booking confirmed for market date " + marketDateId;

    if (!notificationRepository.addNotification(userId, notificationMessage)) {
        return { BookingResultType::ERROR,
                 "Failed to save booking notification to the database." };
    }

    // Update in-memory state
    marketDate->addBooking(booking);
    vendor->addBooking(booking);
    vendor->addNotification(Notification(notificationMessage));

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

    BookingRepository bookingRepository;
    NotificationRepository notificationRepository;

    // Persist booking removal first
    if (!bookingRepository.removeBooking(userId, marketDateId)) {
        return { BookingResultType::ERROR,
                 "Failed to remove booking from the database." };
    }

    const VendorCategory category = vendor->getCategory();

    // Persist notification
    const std::string notificationMessage =
        "Booking cancelled for market date " + marketDateId;

    if (!notificationRepository.addNotification(userId, notificationMessage)) {
        return { BookingResultType::ERROR,
                 "Failed to save cancellation notification to the database." };
    }

    // Update in-memory state
    marketDate->removeBooking(userId);
    vendor->removeBooking(marketDateId);
    vendor->addNotification(Notification(notificationMessage));

    // Trigger waitlist promotion logic
    waitlistController.handlePromotionIfNeeded(marketDateId, category);

    return { BookingResultType::SUCCESS,
             "Booking cancelled successfully." };
}
