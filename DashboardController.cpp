#include "DashboardController.h"
#include "User.h"
#include "Vendor.h"

DashboardController::DashboardController(InMemoryStorageManager& storage) 
    : storage(storage) {}

VendorDashboardData DashboardController::getVendorDashboard(const std::string& userId) {
    VendorDashboardData data;
    User* user = storage.getUser(userId);
    Vendor* vendor = dynamic_cast<Vendor*>(user);

    if (vendor != nullptr) {
        // Now it's vector<Booking> = vector<Booking>
        data.confirmedBookings = vendor->getBookings(); 
        
        // Now it's vector<Notification> = vector<Notification>
        data.notifications = vendor->getNotifications();
    }

    return data;
}