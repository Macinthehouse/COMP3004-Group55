#include "DashboardController.h"

#include <algorithm>

#include "InMemoryStorageManager.h"
#include "User.h"
#include "Vendor.h"
#include "MarketDate.h"
#include "Waitlist.h"
#include "ComplianceDocument.h"

namespace
{
    std::string categoryToString(VendorCategory category)
    {
        return (category == VendorCategory::FOOD) ? "Food" : "Artisan";
    }
}

DashboardController::DashboardController(InMemoryStorageManager& storage)
    : storage(storage)
{
}

std::vector<MarketDateSummary> DashboardController::listMarketDates()
{
    std::vector<MarketDateSummary> out;

    const auto dates = storage.getAllMarketDates();
    out.reserve(dates.size());

    for (MarketDate* md : dates) {
        if (!md) {
            continue;
        }

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

/**
 * Prepares the complete data set required for the VendorDashboardUI.
 * * This method demonstrates high cohesion by focusing solely on 
 * data aggregation for the presentation layer.
 */
VendorDashboardData DashboardController::getVendorDashboard(const std::string& userId)
{
    VendorDashboardData data;

    User* user = storage.getUser(userId);
    Vendor* vendor = dynamic_cast<Vendor*>(user);

    if (!vendor) {
        return data;
    }

    // Vendor summary
    data.vendor.vendorId = vendor->getId();
    data.vendor.businessName = vendor->getBusinessName();
    data.vendor.ownerName = vendor->getName();
    data.vendor.email = vendor->getEmail();
    data.vendor.phone = vendor->getPhone();
    data.vendor.mailingAddress = vendor->getAddress();
    data.vendor.category = categoryToString(vendor->getCategory());

    // Existing dashboard sections
    data.confirmedBookings = vendor->getBookings();
    data.notifications = vendor->getNotifications();
    data.complianceDocuments = vendor->getComplianceDocuments();

    const VendorCategory category = vendor->getCategory();
    const auto dates = storage.getAllMarketDates();

    // Calculate waitlist positions for the vendor across all market dates.
    // This allows the UI to show "Position: 2" directly to the user.
    for (MarketDate* md : dates) {
        if (!md) {
            continue;
        }

        const std::string marketDateId = md->getDate();
        Waitlist* wl = storage.getWaitlist(marketDateId, category);
        if (!wl) {
            continue;
        }

        if (wl->containsVendor(userId)) {
            WaitlistStatus ws;
            ws.marketDateId = marketDateId;
            ws.dateIso = marketDateId;
            ws.position = wl->getPosition(userId);
            ws.category = categoryToString(category);
            data.activeWaitlists.push_back(ws);
        }
    }

    std::sort(data.activeWaitlists.begin(), data.activeWaitlists.end(),
              [](const WaitlistStatus& a, const WaitlistStatus& b) {
                  return a.dateIso < b.dateIso;
              });

    return data;
}
