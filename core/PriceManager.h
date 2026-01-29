#ifndef PRICEMANAGER_H
#define PRICEMANAGER_H

#include "Room.h"
#include "Date.h"

class PriceManager {
public:
    static double calculateOccupancyFactor(int availableRooms, int totalRooms);
    static double calculateBookingFactor(Date checkIn, Date currentDate, int availableRooms);
    static double calculateSeasonFactor(Date date);
    static double calculateLengthOfStayFactor(int nights);

    static double calculatePrice(const Room& room, Date checkIn, Date checkOut,
                                 Date currentDate, int availableRooms, int totalRooms);

    static bool isSummer(Date date);
    static bool isHoliday(Date date);
};

#endif // PRICEMANAGER_H
