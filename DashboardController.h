#ifndef DASHBOARDCONTROLLER_H
#define DASHBOARDCONTROLLER_H

#include <vector>
#include <string>

#include "InMemoryStorageManager.h"
#include "VendorCategory.h"
#include "Notification.h"
#include "Booking.h"


struct WaitlistStatus {
    std::string marketDateId;  // "YYYY-MM-DD"
    std::string dateIso;       // display
    int position = 0;          // 1-based
    std::string category;      // "Food" / "Artisan"
};

struct VendorDashboardData {
    std::vector<Booking> confirmedBookings;
    std::vector<Notification> notifications;
    std::vector<WaitlistStatus> activeWaitlists; // NEW
};

struct MarketDateSummary {
    std::string marketDateId;  // "YYYY-MM-DD"
    std::string dateIso;       // display
    bool foodHasSpace = false;
    bool artisanHasSpace = false;
};

class DashboardController {
public:
    DashboardController(InMemoryStorageManager& storage);

    VendorDashboardData getVendorDashboard(const std::string& userId);

    // NEW: schedule browsing for MarketScheduleUI (UI calls controller, not storage)
    std::vector<MarketDateSummary> listMarketDates();

private:
    InMemoryStorageManager& storage;
};

#endif
