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

#include <QDebug>

// ... (Constructor and getAvailableMarketDates remain same) ...

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

/**
 * Orchestrates the booking of a market stall.
 * * This method enforces the D2 "One Active Booking" rule and handles 
 * the SQLite persistence mapping for the relational 'bookings' table.
 */
BookingResult BookingController::bookStall(const std::string& userId,
                                           const std::string& marketDateId)
{
    // Validation Logic
    // Checks if the user exists, is a vendor, and if the date is valid.
    User* user = storage.getUser(userId);
    if (!user) {
        return { BookingResultType::INVALID_USER,
                 "User not found." };
    }

    Vendor* vendor = dynamic_cast<Vendor*>(user);
    if (!vendor) {
        return { BookingResultType::NOT_VENDOR,
                 "Only vendors may book stalls." };
    }

    MarketDate* marketDate = storage.getMarketDate(marketDateId);
    if (!marketDate) {
        return { BookingResultType::INVALID_DATE,
                 "Market date not found." };
    }

    // D2 business rule: vendors can hold only one active booking total
    if (!vendor->getBookings().empty()) {
        return { BookingResultType::ALREADY_BOOKED,
                 "Vendor already has an active booking." };
    }

    VendorCategory category = vendor->getCategory();

    if (!marketDate->hasAvailableStall(category)) {
        return { BookingResultType::MARKET_FULL,
                 "No stalls available for this category." };
    }

    Waitlist* wl = storage.getWaitlist(marketDateId, category);
    if (wl && !wl->isEmpty()) {
        const std::string headId = wl->peekNextVendorId();
        if (!headId.empty() && headId != userId) {
            return { BookingResultType::ERROR,
                     "A waitlist exists for " + marketDateId +
                     ". Only the next vendor in line may book right now." };
        }
    }

    // Persistence Layer (Relational Storage)
    // D2 Requirement: Every database query must result in a corresponding object in memory.
    Booking booking(userId, marketDateId, category);

    BookingRepository bookingRepository;
    WaitlistRepository waitlistRepository;
    NotificationRepository notificationRepository;

    // 1. Persist booking
    if (!bookingRepository.createBooking(booking)) {
        return { BookingResultType::ERROR,
                 "Failed to save booking to the database." };
    }

    // 2. If this vendor was first in waitlist, persist waitlist dequeue
    bool removedFromWaitlist = false;
    Waitlist updatedWaitlist;

    if (wl && !wl->isEmpty() && wl->peekNextVendorId() == userId) {
        updatedWaitlist = *wl;
        updatedWaitlist.dequeue();

        if (!waitlistRepository.replaceWaitlist(updatedWaitlist)) {
            return { BookingResultType::ERROR,
                     "Failed to update waitlist in the database." };
        }

        removedFromWaitlist = true;
    }

    // 3. Update in-memory state
    marketDate->addBooking(booking);
    vendor->addBooking(booking);

    if (removedFromWaitlist && wl) {
        *wl = updatedWaitlist;
        waitlistController.notifyVendorsMovedUp(marketDateId, category, 1);
    }

    // 4. Notifications are best-effort
    const std::string notificationMessage =
        "Booking confirmed for market date " + marketDateId;

    vendor->addNotification(Notification(notificationMessage));

    if (!notificationRepository.addNotification(userId, notificationMessage)) {
        qDebug() << "Booking notification could not be saved for vendor:"
                 << QString::fromStdString(userId);
    }

    return { BookingResultType::SUCCESS,
             "Booking successful." };
}

/**
 * Handles stall cancellation and triggers the FIFO promotion sequence.
 */
BookingResult BookingController::cancelBooking(const std::string& userId,
                                               const std::string& marketDateId)
{
    User* user = storage.getUser(userId);
    if (!user) {
        return { BookingResultType::INVALID_USER,
                 "User not found." };
    }

    Vendor* vendor = dynamic_cast<Vendor*>(user);
    if (!vendor) {
        return { BookingResultType::NOT_VENDOR,
                 "Only vendors may cancel bookings." };
    }

    MarketDate* marketDate = storage.getMarketDate(marketDateId);
    if (!marketDate) {
        return { BookingResultType::INVALID_DATE,
                 "Market date not found." };
    }

    if (!vendor->hasBookingForDate(marketDateId)) {
        return { BookingResultType::ERROR,
                 "No booking found for this date." };
    }

    BookingRepository bookingRepository;
    NotificationRepository notificationRepository;

    // 1. Persist removal to SQLite to ensure data is not lost on restart.
    if (!bookingRepository.removeBooking(userId, marketDateId)) {
        return { BookingResultType::ERROR,
                 "Failed to remove booking from the database." };
    }

    // 2. Update In-Memory state (Object Mapping).
    marketDate->removeBooking(userId);
    vendor->removeBooking(marketDateId);

    // 3. Notifications are best-effort
    const std::string notificationMessage =
        "Booking cancelled for market date " + marketDateId;

    vendor->addNotification(Notification(notificationMessage));

    if (!notificationRepository.addNotification(userId, notificationMessage)) {
        qDebug() << "Cancellation notification could not be saved for vendor:"
                 << QString::fromStdString(userId);
    }

    // 4. Trigger waitlist promotion logic
    const VendorCategory category = vendor->getCategory();
    // FIFO TRIGGER: D2 requires that when a stall opens, the next vendor 
    // in the waitlist is notified.
    waitlistController.handlePromotionIfNeeded(marketDateId, category);

    return { BookingResultType::SUCCESS,
             "Booking cancelled successfully." };
}
