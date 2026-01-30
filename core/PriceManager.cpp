#include "PriceManager.h"
#include <cmath>

double PriceManager::calculateOccupancyFactor(int availableRooms, int totalRooms) {
    if (totalRooms <= 0) return 1.0;

    double occupancyRate = static_cast<double>(totalRooms - availableRooms) / totalRooms;

    if (occupancyRate < 0.3) {
        return 0.85;
    }
    if (occupancyRate > 0.7) {
        return 1.35;
    }

    return 1.0;
}

double PriceManager::calculateBookingFactor(Date checkIn, Date currentDate, int availableRooms) {
    int daysUntilCheckIn = checkIn.calculateDays() - currentDate.calculateDays();
    if (daysUntilCheckIn < 0) daysUntilCheckIn = 0;

    if (daysUntilCheckIn >= 30) {
        return 0.8;
    }
    if (daysUntilCheckIn >= 7) {
        return 1.0;
    }
    if (daysUntilCheckIn >= 1) {
        if (availableRooms < 3) {
            return 1.5;
        }
        return 1.2;
    }
    else {
        return 2.0;
    }
}

double PriceManager::calculateSeasonFactor(Date date) {
    if (isSummer(date) && isHoliday(date)) {
        return 1.8;
    }
    if (isSummer(date)) {
        return 1.4;
    }
    if (isHoliday(date)) {
        return 1.4;
    }
    return 1.0;
}

double PriceManager::calculateLengthOfStayFactor(int nights) {
    if (nights >= 7) {
        return 0.8;
    }
    if (nights >= 3) {
        return 0.9;
    }
    return 1.0;
}

double PriceManager::calculatePrice(const Room& room, Date checkIn, Date checkOut,
                                    Date currentDate, int availableRooms, int totalRooms) {
    int nights = checkOut.calculateDays() - checkIn.calculateDays();
    if (nights <= 0) nights = 1;

    double basePrice = room.getBasePrice() * nights;

    double occupancyFactor = calculateOccupancyFactor(availableRooms, totalRooms);
    double bookingFactor = calculateBookingFactor(checkIn, currentDate, availableRooms);
    double seasonFactor = calculateSeasonFactor(checkIn);
    double lengthFactor = calculateLengthOfStayFactor(nights);

    double finalPrice = basePrice * occupancyFactor * bookingFactor * seasonFactor * lengthFactor;

    double minPrice = basePrice * 0.5;
    if (finalPrice < minPrice) {
        finalPrice = minPrice;
    }

    return std::round(finalPrice * 100) / 100;
}

bool PriceManager::isSummer(Date date) {
    int month = date.getMonth();
    return (month >= 6 && month <= 8);
}

bool PriceManager::isHoliday(Date date) {
    int day = date.getDay();
    int month = date.getMonth();

    if (month == 12 && day == 31) return true;
    if (month == 1 && day >= 1 && day <= 8) return true;
    if (month == 2 && day == 23) return true;
    if (month == 3 && day == 8) return true;
    if (month == 5 && day == 1) return true;
    if (month == 5 && day == 9) return true;
    if (month == 6 && day == 12) return true;
    if (month == 11 && day == 4) return true;

    return false;
}