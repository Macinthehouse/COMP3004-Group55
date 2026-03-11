#ifndef DASHBOARDCONTROLLER_H
#define DASHBOARDCONTROLLER_H

#include <vector>
#include <string>
#include "MarketDate.h"
#include "InMemoryStorageManager.h"

#include "Notification.h"
#include "Booking.h"

struct VendorDashboardData {
    std::vector<Booking> confirmedBookings; 
    // Change this from std::string to Notification
    std::vector<Notification> notifications; 
};

class DashboardController {
public:
    DashboardController(InMemoryStorageManager& storage);

    // Main logic: Gather everything for a specific vendor
    VendorDashboardData getVendorDashboard(const std::string& userId);

private:
    InMemoryStorageManager& storage;
};

#endif