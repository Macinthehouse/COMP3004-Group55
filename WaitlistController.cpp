#include "WaitlistController.h"

#include "InMemoryStorageManager.h"
#include "User.h"
#include "Vendor.h"
#include "MarketDate.h"
#include "Waitlist.h"
#include "Booking.h"
#include "Notification.h"

#include "WaitlistRepository.h"
#include "NotificationRepository.h"

#include <string>
#include <vector>

// Constructor

WaitlistController::WaitlistController(InMemoryStorageManager& storageManager)
    : storage(storageManager)
{
}

// joinWaitlist()

WaitlistResult WaitlistController::joinWaitlist(const std::string& userId,
                                                const std::string& marketDateId)
{
    User* user = storage.getUser(userId);
    if (!user) {
        return { WaitlistResultType::NOT_FOUND,
                 "User not found.",
                 -1 };
    }

    Vendor* vendor = dynamic_cast<Vendor*>(user);
    if (!vendor) {
        return { WaitlistResultType::NOT_VENDOR,
                 "Only vendors may join waitlists.",
                 -1 };
    }

    MarketDate* marketDate = storage.getMarketDate(marketDateId);
    if (!marketDate) {
        return { WaitlistResultType::NOT_FOUND,
                 "Market date not found.",
                 -1 };
    }

    VendorCategory category = vendor->getCategory();

    if (vendor->hasBookingForDate(marketDateId)) {
        return { WaitlistResultType::ALREADY_BOOKED,
                 "Vendor already booked for this date.",
                 -1 };
    }

    if (marketDate->hasAvailableStall(category)) {
        return { WaitlistResultType::MARKET_NOT_FULL,
                 "Stalls still available. Booking required instead.",
                 -1 };
    }

    Waitlist* waitlist = storage.getWaitlist(marketDateId, category);
    if (!waitlist) {
        return { WaitlistResultType::ERROR,
                 "Waitlist not found.",
                 -1 };
    }

    if (waitlist->containsVendor(userId)) {
        return { WaitlistResultType::ALREADY_IN_WAITLIST,
                 "Vendor already in waitlist.",
                 -1 };
    }

    Waitlist updatedWaitlist = *waitlist;
    updatedWaitlist.enqueue(userId);
    int position = updatedWaitlist.getPosition(userId);

    WaitlistRepository waitlistRepository;
    if (!waitlistRepository.replaceWaitlist(updatedWaitlist)) {
        return { WaitlistResultType::ERROR,
                 "Failed to save waitlist to the database.",
                 -1 };
    }

    *waitlist = updatedWaitlist;

    std::string message =
        "You have been added to the waitlist for market date "
        + marketDateId +
        ". Current position: " + std::to_string(position);

    Notification notification(message);
    vendor->addNotification(notification);

    NotificationRepository notificationRepository;
    notificationRepository.addNotification(userId, message);

    return { WaitlistResultType::SUCCESS,
             "Added to waitlist.",
             position };
}

// leaveWaitlist()

WaitlistResult WaitlistController::leaveWaitlist(const std::string& userId,
                                                 const std::string& marketDateId)
{
    User* user = storage.getUser(userId);
    if (!user) {
        return { WaitlistResultType::NOT_FOUND,
                 "User not found.",
                 -1 };
    }

    Vendor* vendor = dynamic_cast<Vendor*>(user);
    if (!vendor) {
        return { WaitlistResultType::NOT_VENDOR,
                 "Only vendors may leave waitlists.",
                 -1 };
    }

    MarketDate* marketDate = storage.getMarketDate(marketDateId);
    if (!marketDate) {
        return { WaitlistResultType::NOT_FOUND,
                 "Market date not found.",
                 -1 };
    }

    VendorCategory category = vendor->getCategory();

    Waitlist* waitlist = storage.getWaitlist(marketDateId, category);
    if (!waitlist) {
        return { WaitlistResultType::ERROR,
                 "Waitlist not found.",
                 -1 };
    }

    if (!waitlist->containsVendor(userId)) {
        return { WaitlistResultType::NOT_FOUND,
                 "Vendor not in waitlist.",
                 -1 };
    }

    const int removedPos = waitlist->getPosition(userId); // 1-based

    Waitlist updatedWaitlist = *waitlist;
    updatedWaitlist.remove(userId);

    WaitlistRepository waitlistRepository;
    if (!waitlistRepository.replaceWaitlist(updatedWaitlist)) {
        return { WaitlistResultType::ERROR,
                 "Failed to update waitlist in the database.",
                 -1 };
    }

    *waitlist = updatedWaitlist;

    // everyone who was behind removedPos moves up by 1
    notifyVendorsMovedUp(marketDateId, category, removedPos);

    std::string message =
        "You have been removed from the waitlist for market date " + marketDateId;

    Notification notification(message);
    vendor->addNotification(notification);

    NotificationRepository notificationRepository;
    notificationRepository.addNotification(userId, message);

    return { WaitlistResultType::SUCCESS,
             "Removed from waitlist.",
             -1 };
}

// handlePromotionIfNeeded()

void WaitlistController::handlePromotionIfNeeded(const std::string& marketDateId,
                                                 VendorCategory category)
{
    MarketDate* marketDate = storage.getMarketDate(marketDateId);
    if (!marketDate) return;

    if (!marketDate->hasAvailableStall(category)) return;

    Waitlist* waitlist = storage.getWaitlist(marketDateId, category);
    if (!waitlist || waitlist->isEmpty()) return;

    WaitlistRepository waitlistRepository;
    NotificationRepository notificationRepository;

    // Find first eligible vendor at head of queue (skip invalid/stale entries)
    while (!waitlist->isEmpty()) {
        const std::string nextVendorId = waitlist->peekNextVendorId();
        if (nextVendorId.empty()) return;

        User* user = storage.getUser(nextVendorId);
        Vendor* vendor = dynamic_cast<Vendor*>(user);

        if (!vendor) {
            Waitlist updatedWaitlist = *waitlist;
            updatedWaitlist.dequeue();

            if (!waitlistRepository.replaceWaitlist(updatedWaitlist)) {
                return;
            }

            *waitlist = updatedWaitlist;
            notifyVendorsMovedUp(marketDateId, category, 1);
            continue;
        }

        // Skip vendors who already hold any active booking
        if (!vendor->getBookings().empty()) {
            Waitlist updatedWaitlist = *waitlist;
            updatedWaitlist.dequeue();

            if (!waitlistRepository.replaceWaitlist(updatedWaitlist)) {
                return;
            }

            *waitlist = updatedWaitlist;
            notifyVendorsMovedUp(marketDateId, category, 1);
            continue;
        }

        std::string message =
            "A stall is now available for " + marketDateId +
            ". You are next in line—please book.";

        Notification notification(message);
        vendor->addNotification(notification);
        notificationRepository.addNotification(nextVendorId, message);
        return;
    }
}

// getWaitlistPosition()

int WaitlistController::getWaitlistPosition(const std::string& userId,
                                            const std::string& marketDateId,
                                            VendorCategory category)
{
    Waitlist* wl = storage.getWaitlist(marketDateId, category);

    if (!wl) return -1;

    return wl->getPosition(userId);
}

// notifyVendorsMovedUp()

void WaitlistController::notifyVendorsMovedUp(const std::string& marketDateId,
                                              VendorCategory category,
                                              int fromPosition1Based)
{
    if (fromPosition1Based <= 0) return;

    Waitlist* wl = storage.getWaitlist(marketDateId, category);
    if (!wl) return;

    const std::vector<std::string> q = wl->getQueueSnapshot();
    const int startIdx = fromPosition1Based - 1;
    if (startIdx < 0 || startIdx >= static_cast<int>(q.size())) return;

    NotificationRepository notificationRepository;

    for (int i = startIdx; i < static_cast<int>(q.size()); ++i) {
        const std::string& vendorId = q[i];

        User* user = storage.getUser(vendorId);
        Vendor* vendor = dynamic_cast<Vendor*>(user);
        if (!vendor) continue;

        const int newPos = i + 1; // 1-based
        std::string message =
            "Your waitlist position for " + marketDateId +
            " is now " + std::to_string(newPos) + ".";

        Notification notification(message);
        vendor->addNotification(notification);
        notificationRepository.addNotification(vendorId, message);
    }
}
