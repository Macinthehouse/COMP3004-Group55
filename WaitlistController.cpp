#include "WaitlistController.h"

#include "InMemoryStorageManager.h"
#include "User.h"
#include "Vendor.h"
#include "MarketDate.h"
#include "Waitlist.h"
#include "Booking.h"
#include "Notification.h"

#include <string>

// Constructor

WaitlistController::WaitlistController(InMemoryStorageManager& storageManager)
    : storage(storageManager)
{
}

// joinWaitlist()

WaitlistResult WaitlistController::joinWaitlist(const std::string& userId,
                                                const std::string& marketDateId)
{
    // Retrieve User
    User* user = storage.getUser(userId);
    if (!user) {
        return { WaitlistResultType::NOT_FOUND,
                 "User not found.",
                 -1 };
    }

    // Ensure user is Vendor
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

    waitlist->enqueue(userId);
    int position = waitlist->getPosition(userId);

    Notification notification(
        "You have been added to the waitlist for market date "
        + marketDateId +
        ". Current position: " + std::to_string(position)
    );

    vendor->addNotification(notification);

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
    waitlist->remove(userId);

    // everyone who was behind removedPos moves up by 1
    notifyVendorsMovedUp(marketDateId, category, removedPos);

    Notification notification(
        "You have been removed from the waitlist for market date "
        + marketDateId
    );

    vendor->addNotification(notification);

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

    // Find first eligible vendor at head of queue (skip invalid/stale entries)
    while (!waitlist->isEmpty()) {
        const std::string nextVendorId = waitlist->peekNextVendorId();
        if (nextVendorId.empty()) return;

        User* user = storage.getUser(nextVendorId);
        Vendor* vendor = dynamic_cast<Vendor*>(user);
        if (!vendor) {
            // stale entry; drop it and check next
            waitlist->dequeue();
            notifyVendorsMovedUp(marketDateId, category, 1);
            continue;
        }

        if (vendor->hasBookingForDate(marketDateId)) {
            // stale entry; already booked; drop it and check next
            waitlist->dequeue();
            notifyVendorsMovedUp(marketDateId, category, 1);
            continue;
        }

        // Notify only (do not dequeue here)
        Notification notification(
            "A stall is now available for " + marketDateId +
            ". You are next in line—please book."
        );
        vendor->addNotification(notification);
        return;
    }
}

// getWaitlistPosition()
int WaitlistController::getWaitlistPosition(const std::string& userId, 
                                            const std::string& marketDateId, 
                                            VendorCategory category) {
    // Get the waitlist object from storage
    Waitlist* wl = storage.getWaitlist(marketDateId, category);
    
    // If it doesn't exist, the position is 0
    if (!wl) return 0;

    // This will return the 1-based index (1, 2, 3...) or 0 if not found.
    return wl->getPosition(userId);
}

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

    for (int i = startIdx; i < static_cast<int>(q.size()); ++i) {
        const std::string& vid = q[i];

        User* u = storage.getUser(vid);
        Vendor* v = dynamic_cast<Vendor*>(u);
        if (!v) continue;

        const int newPos = i + 1; // 1-based
        Notification n(
            "Your waitlist position for " + marketDateId +
            " is now " + std::to_string(newPos) + "."
        );
        v->addNotification(n);
    }
}


