#include "WaitlistController.h"

#include "InMemoryStorageManager.h"
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

WaitlistResult WaitlistController::joinWaitlist(const std::string& vendorId,
                                                const std::string& marketDateId)
{
    // Retrieve Vendor
    Vendor* vendor = storage.getVendor(vendorId);
    if (!vendor) {
        return { WaitlistResultType::NOT_FOUND,
                 "Vendor not found.",
                 -1 };
    }

    // Retrieve MarketDate
    MarketDate* marketDate = storage.getMarketDate(marketDateId);
    if (!marketDate) {
        return { WaitlistResultType::NOT_FOUND,
                 "Market date not found.",
                 -1 };
    }

    VendorCategory category = vendor->getCategory();

    // Prevent joining if already booked
    if (vendor->hasBookingForDate(marketDateId)) {
        return { WaitlistResultType::ALREADY_BOOKED,
                 "Vendor already booked for this date.",
                 -1 };
    }

    // Only allow waitlist if market is FULL
    if (marketDate->hasAvailableStall(category)) {
        return { WaitlistResultType::MARKET_NOT_FULL,
                 "Stalls still available. Booking required instead.",
                 -1 };
    }

    // Retrieve correct waitlist
    Waitlist* waitlist = storage.getWaitlist(marketDateId, category);
    if (!waitlist) {
        return { WaitlistResultType::ERROR,
                 "Waitlist not found.",
                 -1 };
    }

    // Prevent duplicate waitlist entry
    if (waitlist->containsVendor(vendorId)) {
        return { WaitlistResultType::ALREADY_IN_WAITLIST,
                 "Vendor already in waitlist.",
                 -1 };
    }

    // Add vendor to waitlist
    waitlist->enqueue(vendorId);
    int position = waitlist->getPosition(vendorId);

    // Add notification
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

WaitlistResult WaitlistController::leaveWaitlist(const std::string& vendorId,
                                                 const std::string& marketDateId)
{
    Vendor* vendor = storage.getVendor(vendorId);
    if (!vendor) {
        return { WaitlistResultType::NOT_FOUND,
                 "Vendor not found.",
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

    if (!waitlist->containsVendor(vendorId)) {
        return { WaitlistResultType::NOT_FOUND,
                 "Vendor not in waitlist.",
                 -1 };
    }

    waitlist->remove(vendorId);

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
// Automatically completes booking when stall frees.
// ---------------------------------------------

void WaitlistController::handlePromotionIfNeeded(const std::string& marketDateId,
                                                 VendorCategory category)
{
    MarketDate* marketDate = storage.getMarketDate(marketDateId);
    if (!marketDate) return;

    // Stall must be available
    if (!marketDate->hasAvailableStall(category)) return;

    Waitlist* waitlist = storage.getWaitlist(marketDateId, category);
    if (!waitlist || waitlist->isEmpty()) return;

    // Remove next vendor (FIFO)
    std::string promotedVendorId = waitlist->dequeue();
    if (promotedVendorId.empty()) return;

    Vendor* vendor = storage.getVendor(promotedVendorId);
    if (!vendor) return;

    // Safety check
    if (vendor->hasBookingForDate(marketDateId)) return;

    // Create booking
    Booking booking(promotedVendorId, marketDateId, category);

    marketDate->addBooking(booking);
    vendor->addBooking(booking);

    // Notify vendor of automatic booking
    Notification notification(
        "You have been automatically booked from the waitlist for market date "
        + marketDateId
    );

    vendor->addNotification(notification);
}