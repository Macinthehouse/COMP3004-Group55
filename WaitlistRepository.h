#ifndef WAITLISTREPOSITORY_H
#define WAITLISTREPOSITORY_H

#include <string>
#include <vector>

#include "Waitlist.h"
#include "VendorCategory.h"

class WaitlistRepository
{
public:
    std::vector<Waitlist> loadAllWaitlists();

    bool addVendorToWaitlist(const std::string& vendorId,
                             const std::string& marketDateId,
                             VendorCategory category,
                             int queuePosition);

    bool removeVendorFromWaitlist(const std::string& vendorId,
                                  const std::string& marketDateId,
                                  VendorCategory category);

    bool replaceWaitlist(const Waitlist& waitlist);
};

#endif // WAITLISTREPOSITORY_H
