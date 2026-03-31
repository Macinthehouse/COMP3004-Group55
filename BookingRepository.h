#ifndef BOOKINGREPOSITORY_H
#define BOOKINGREPOSITORY_H

#include <vector>

#include "Booking.h"

class BookingRepository
{
public:
    std::vector<Booking> loadAllBookings();
};

#endif // BOOKINGREPOSITORY_H
