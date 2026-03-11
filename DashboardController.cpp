#include "DashboardController.h"

#include <algorithm>

#include "User.h"
#include "Vendor.h"
#include "MarketDate.h"
#include "Waitlist.h"

static std::string categoryToString(VendorCategory c) {
    return (c == VendorCategory::FOOD) ? "Food" : "Artisan";
}

DashboardController::DashboardController(InMemoryStorageManager& storage)
    : storage(storage) {}

std::vector<MarketDateSummary> DashboardController::listMarketDates() {
    std::vector<MarketDateSummary> out;

    const auto dates = storage.getAllMarketDates();
    out.reserve(dates.size());

    for (MarketDate* md : dates) {
        if (!md) continue;

        MarketDateSummary row;
        row.marketDateId = md->getDate();
        row.dateIso = md->getDate();
        row.foodHasSpace = md->hasAvailableStall(VendorCategory::FOOD);
        row.artisanHasSpace = md->hasAvailableStall(VendorCategory::ARTISAN);
        out.push_back(row);
    }

    std::sort(out.begin(), out.end(),
              [](const MarketDateSummary& a, const MarketDateSummary& b) {
                  return a.dateIso < b.dateIso;
              });

    return out;
}

VendorDashboardData DashboardController::getVendorDashboard(const std::string& userId) {
    VendorDashboardData data;

    User* user = storage.getUser(userId);
    Vendor* vendor = dynamic_cast<Vendor*>(user);

    if (!vendor) {
        return data; // empty
    }

    data.confirmedBookings = vendor->getBookings();
    data.notifications = vendor->getNotifications();

    const VendorCategory cat = vendor->getCategory();

    const auto dates = storage.getAllMarketDates();
    for (MarketDate* md : dates) {
        if (!md) continue;

        const std::string marketDateId = md->getDate();
        Waitlist* wl = storage.getWaitlist(marketDateId, cat);
        if (!wl) continue;

        if (wl->containsVendor(userId)) {
            WaitlistStatus ws;
            ws.marketDateId = marketDateId;
            ws.dateIso = marketDateId;
            ws.position = wl->getPosition(userId); // 1-based per Waitlist.h
            ws.category = categoryToString(cat);
            data.activeWaitlists.push_back(ws);
        }
    }

    std::sort(data.activeWaitlists.begin(), data.activeWaitlists.end(),
              [](const WaitlistStatus& a, const WaitlistStatus& b) {
                  return a.dateIso < b.dateIso;
              });

    return data;
}
