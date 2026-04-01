#ifndef MARKETDATEREPOSITORY_H
#define MARKETDATEREPOSITORY_H

#include <memory>
#include <string>
#include <vector>

#include "MarketDate.h"

class MarketDateRepository
{
public:
    std::unique_ptr<MarketDate> findById(const std::string& id);
    std::vector<MarketDate> loadAllMarketDates();
};

#endif // MARKETDATEREPOSITORY_H
