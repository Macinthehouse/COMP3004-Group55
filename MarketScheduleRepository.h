#ifndef MARKETSCHEDULEREPOSITORY_H
#define MARKETSCHEDULEREPOSITORY_H

#include <memory>
#include <string>
#include <vector>

#include "MarketDate.h"

class MarketScheduleRepository
{
public:
    std::unique_ptr<MarketDate> findById(const std::string& id);
    std::vector<MarketDate> loadAllMarketDates();
};

#endif // MARKETSCHEDULEREPOSITORY_H
