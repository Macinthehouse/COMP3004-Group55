#include "MarketOperatorController.h"
#include "InMemoryStorageManager.h"
#include "BookingController.h"
#include "WaitlistController.h"
#include "Vendor.h"
#include "NotificationRepository.h"

/**
 * Constructor for MarketOperatorController.
 * * This controller coordinates actions performed by the Market Operator on behalf of vendors.
 * It strictly follows the 3-subsystem layered architecture by interacting with other 
 * Application Logic controllers and the Domain & Storage subsystem.
 */
MarketOperatorController::MarketOperatorController(InMemoryStorageManager& storage, 
                                                   BookingController& bookingCtrl, 
                                                   WaitlistController& waitlistCtrl)
    : storage(storage), 
      bookingController(bookingCtrl), 
      waitlistController(waitlistCtrl) 
{
}

/**
 * Books a stall for a specific market date on behalf of a vendor.
 * * This method delegates to BookingController::bookStall to ensure that core business rules—
 * specifically the D2 rule that vendors can hold only one active booking—are enforced 
 * consistently.
 * * @param vendorId The unique ID of the vendor receiving the booking.
 * @param marketDateId The target market date (YYYY-MM-DD).
 * @return MarketOperatorResult containing success status and descriptive message.
 */
MarketOperatorResult MarketOperatorController::bookForVendor(const std::string& vendorId, 
                                                             const std::string& marketDateId) {
    // Re-use existing booking logic. This automatically handles the SQLite persistence 
    // and updates the in-memory MarketDate and Vendor objects.
    auto result = bookingController.bookStall(vendorId, marketDateId);
    
    if (result.type == BookingResultType::SUCCESS) {
        // Create a record of this action for the vendor's dashboard.
        std::string msg = "A market operator has booked a stall for you on " + marketDateId;
        if (Vendor* v = storage.getVendor(vendorId)) {
            // Update in-memory notification list for immediate UI feedback.
            v->addNotification(Notification(msg));
            // Persist notification to SQLite so it remains after restart.
            NotificationRepository().addNotification(vendorId, msg);
        }
        return {true, "Successfully booked stall for vendor."};
    }
    
    // Return the specific error message (e.g., "Vendor already has an active booking").
    return {false, result.message};
}

/**
 * Cancels a stall booking on behalf of a vendor.
 * * This action ensures the stall becomes available for other vendors and triggers 
 * the FIFO waitlist promotion logic.
 * * @param vendorId The unique ID of the vendor whose booking is being cancelled.
 * @param marketDateId The market date associated with the booking.
 * @return MarketOperatorResult containing success status and descriptive message.
 */
MarketOperatorResult MarketOperatorController::cancelForVendor(const std::string& vendorId, 
                                                               const std::string& marketDateId) {
    // Delegate to BookingController to handle database removal and FIFO promotion 
    // for the next vendor in line.
    auto result = bookingController.cancelBooking(vendorId, marketDateId);
    
    if (result.type == BookingResultType::SUCCESS) {
        // Notify the vendor so the change is visible when they return to the system.
        std::string msg = "A market operator has cancelled your booking for " + marketDateId;
        if (Vendor* v = storage.getVendor(vendorId)) {
            v->addNotification(Notification(msg));
            NotificationRepository().addNotification(vendorId, msg);
        }
        return {true, "Successfully cancelled booking for vendor."};
    }
    return {false, result.message};
}

/**
 * Removes a vendor from a specific market date waiting list on their behalf.
 * * This ensures that if the vendor is no longer interested in a date, the Operator 
 * can manually manage the queue.
 * * @param vendorId The unique ID of the vendor to remove.
 * @param marketDateId The market date waitlist to modify.
 * @return MarketOperatorResult containing success status and descriptive message.
 */
MarketOperatorResult MarketOperatorController::removeFromWaitlistForVendor(const std::string& vendorId, 
                                                                           const std::string& marketDateId) {
    // Delegate to WaitlistController. This handles the SQLite 'waitlists' table update 
    // and notifies any vendors who moved up in position.
    auto result = waitlistController.leaveWaitlist(vendorId, marketDateId);
    
    if (result.type == WaitlistResultType::SUCCESS) {
        // Document the removal in the vendor's persistent notification history.
        std::string msg = "A market operator has removed you from the waitlist for " + marketDateId;
        if (Vendor* v = storage.getVendor(vendorId)) {
            v->addNotification(Notification(msg));
            NotificationRepository().addNotification(vendorId, msg);
        }
        return {true, "Successfully removed vendor from waitlist."};
    }
    return {false, result.message};
}