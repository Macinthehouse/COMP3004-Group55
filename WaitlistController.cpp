#include "WaitlistController.h"

#include "InMemoryStorageManager.h"
#include "User.h"
#include "Vendor.h"
#include "MarketDate.h"
#include "Waitlist.h"
#include "Booking.h"
#include "Notification.h"

#include <string>

// ---------------------------------------------
// Constructor
// ---------------------------------------------

WaitlistController::WaitlistController(InMemoryStorageManager& storageManager)
    : storage(storageManager)
{
}

// ---------------------------------------------
// joinWaitlist()
// ---------------------------------------------

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


// ---------------------------------------------
// leaveWaitlist()
// ---------------------------------------------

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

    waitlist->remove(userId);

    Notification notification(
        "You have been removed from the waitlist for market date "
        + marketDateId
    );

    vendor->addNotification(notification);

    return { WaitlistResultType::SUCCESS,
             "Removed from waitlist.",
             -1 };
}


// ---------------------------------------------
// handlePromotionIfNeeded()
// ---------------------------------------------

void WaitlistController::handlePromotionIfNeeded(const std::string& marketDateId,
                                                 VendorCategory category)
{
    MarketDate* marketDate = storage.getMarketDate(marketDateId);
    if (!marketDate) return;

    if (!marketDate->hasAvailableStall(category)) return;

    Waitlist* waitlist = storage.getWaitlist(marketDateId, category);
    if (!waitlist || waitlist->isEmpty()) return;

    std::string promotedVendorId = waitlist->dequeue();
    if (promotedVendorId.empty()) return;

    User* user = storage.getUser(promotedVendorId);
    Vendor* vendor = dynamic_cast<Vendor*>(user);
    if (!vendor) return;

    if (vendor->hasBookingForDate(marketDateId)) return;

    Booking booking(promotedVendorId, marketDateId, category);

    marketDate->addBooking(booking);
    vendor->addBooking(booking);

    Notification notification(
        "You have been automatically booked from the waitlist for market date "
        + marketDateId
    );

    vendor->addNotification(notification);
}

// ---------------------------------------------
// getWaitlistPosition()
// ---------------------------------------------
int WaitlistController::getWaitlistPosition(const std::string& userId, 
                                            const std::string& marketDateId, 
                                            VendorCategory category) {
    // 1. Get the waitlist object from storage
    Waitlist* wl = storage.getWaitlist(marketDateId, category);
    
    // 2. If it doesn't exist, the position is 0
    if (!wl) return 0;

    // 3. Use the built-in domain method!
    // This will return the 1-based index (1, 2, 3...) or 0 if not found.
    return wl->getPosition(userId);
}