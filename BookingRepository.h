#ifndef BOOKINGREPOSITORY_H
#define BOOKINGREPOSITORY_H

#include <string>
#include <vector>

#include "Booking.h"

class BookingRepository
{
public:
    std::vector<Booking> loadAllBookings();

    bool createBooking(const Booking& booking);

    bool removeBooking(const std::string& vendorId,
                       const std::string& marketDateId);
};

#endif // BOOKINGREPOSITORY_H
