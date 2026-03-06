#include "BookingController.h"
#include "InMemoryStorageManager.h"
#include "WaitlistController.h"
#include "Vendor.h"
#include "MarketDate.h"
#include "Booking.h"
#include "Notification.h"

// ---------------------------------------------
// Constructor
// ---------------------------------------------
BookingController::BookingController(InMemoryStorageManager& storageManager,
                                     WaitlistController& waitlistCtrl)
    : storage(storageManager),
      waitlistController(waitlistCtrl)
{
}

// ---------------------------------------------
// bookStall()
// ---------------------------------------------
BookingResult BookingController::bookStall(const std::string& vendorId,
                                           const std::string& marketDateId)
{
    // 1️⃣ Retrieve Vendor
    Vendor* vendor = storage.getVendor(vendorId);
    if (!vendor) {
        return { BookingResultType::INVALID_VENDOR,
                 "Vendor not found." };
    }

    // 2️⃣ Retrieve MarketDate
    MarketDate* marketDate = storage.getMarketDate(marketDateId);
    if (!marketDate) {
        return { BookingResultType::INVALID_DATE,
                 "Market date not found." };
    }

    // 3️⃣ Prevent duplicate booking
    if (vendor->hasBookingForDate(marketDateId)) {
        return { BookingResultType::ALREADY_BOOKED,
                 "Vendor already has a booking for this date." };
    }

    // 4️⃣ Check stall availability
    VendorCategory category = vendor->getCategory();

    if (!marketDate->hasAvailableStall(category)) {
        return { BookingResultType::MARKET_FULL,
                 "No stalls available for this category." };
    }

    // 5️⃣ Create Booking object
    Booking booking(vendorId, marketDateId, category);

    // 6️⃣ Update MarketDate and Vendor
    marketDate->addBooking(booking);
    vendor->addBooking(booking);

    // 7️⃣ Create confirmation notification
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
BookingResult BookingController::cancelBooking(const std::string& vendorId,
                                               const std::string& marketDateId)
{
    // 1️⃣ Retrieve Vendor
    Vendor* vendor = storage.getVendor(vendorId);
    if (!vendor) {
        return { BookingResultType::INVALID_VENDOR,
                 "Vendor not found." };
    }

    // 2️⃣ Retrieve MarketDate
    MarketDate* marketDate = storage.getMarketDate(marketDateId);
    if (!marketDate) {
        return { BookingResultType::INVALID_DATE,
                 "Market date not found." };
    }

    // 3️⃣ Ensure booking exists
    if (!vendor->hasBookingForDate(marketDateId)) {
        return { BookingResultType::ERROR,
                 "No booking found for this date." };
    }

    // 4️⃣ Store category before removal (needed for waitlist promotion)
    VendorCategory category = vendor->getCategory();

    // 5️⃣ Remove booking from both domain objects
    marketDate->removeBooking(vendorId);
    vendor->removeBooking(marketDateId);

    // 6️⃣ Create cancellation notification
    Notification notification(
        "Booking cancelled for market date " + marketDateId
    );

    vendor->addNotification(notification);

    // 7️⃣ Trigger automatic waitlist promotion
    waitlistController.handlePromotionIfNeeded(marketDateId, category);

    return { BookingResultType::SUCCESS,
             "Booking cancelled successfully." };
}