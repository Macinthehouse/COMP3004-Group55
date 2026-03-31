#include "MarketSchedule.h"

MarketSchedule::MarketSchedule()
{
}

void MarketSchedule::clear()
{
    marketDates.clear();
}

void MarketSchedule::addMarketDate(const MarketDate& marketDate)
{
    marketDates[marketDate.getDate()] = marketDate;
}

MarketDate* MarketSchedule::getMarketDate(const std::string& marketDateId)
{
    auto it = marketDates.find(marketDateId);
    return (it != marketDates.end()) ? &it->second : nullptr;
}

const MarketDate* MarketSchedule::getMarketDate(const std::string& marketDateId) const
{
    auto it = marketDates.find(marketDateId);
    return (it != marketDates.end()) ? &it->second : nullptr;
}

std::vector<MarketDate*> MarketSchedule::getAllMarketDates()
{
    std::vector<MarketDate*> dates;
    for (auto& pair : marketDates) {
        dates.push_back(&pair.second);
    }
    return dates;
}

std::vector<const MarketDate*> MarketSchedule::getAllMarketDates() const
{
    std::vector<const MarketDate*> dates;
    for (const auto& pair : marketDates) {
        dates.push_back(&pair.second);
    }
    return dates;
}
