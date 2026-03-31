#ifndef MARKETSCHEDULE_H
#define MARKETSCHEDULE_H

#include <string>
#include <unordered_map>
#include <vector>

#include "MarketDate.h"

// MarketSchedule Class
//
// Represents the collection of market dates in the system.
//
// Responsibilities:
// - Store all MarketDate objects
// - Provide lookup by date ID
// - Provide access to all dates
// - Keep MarketDate grouped under a schedule entity

class MarketSchedule
{
public:
    MarketSchedule();

    void clear();

    void addMarketDate(const MarketDate& marketDate);

    MarketDate* getMarketDate(const std::string& marketDateId);
    const MarketDate* getMarketDate(const std::string& marketDateId) const;

    std::vector<MarketDate*> getAllMarketDates();
    std::vector<const MarketDate*> getAllMarketDates() const;

private:
    std::unordered_map<std::string, MarketDate> marketDates;
};

#endif // MARKETSCHEDULE_H
