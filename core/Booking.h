//
// Created by Asus on 26.01.2026.
//

#ifndef HOTEL_MANAGAR_BOOKING_H
#define HOTEL_MANAGAR_BOOKING_H
#include "Room.h"
#include "Client.h"
#include "Date.h"
#include <string>

enum class BookingStatus{
    Booked, CheckedIn, CheckedOut, Canceled
};

class Booking {
private:
    int bookingId;
    Room* room;
    Client* client;
    Date checkedIn;
    Date checkedOut;
    double totalPrice;
    BookingStatus status;
public:
    Booking(int id, Room *_room, Client *_client,
            Date checkInDate, Date checkOutDate, double _price,
            BookingStatus initialStatus = BookingStatus::Booked);

    int getBookingId() const;
    Room* getRoom() const;
    Client* getClient() const;
    Date getCheckedIn() const;
    Date getCheckedOut() const;
    double getTotalPrice() const;
    BookingStatus getStatus() const;
    int getRoomId() const;
    int getClientId() const;

    int calculateNights() const;
    double calculatePricePerNight() const;

    void setStatus(BookingStatus newStatus);
    void setRoom(Room* newRoom);
    void setClient(Client* clientPtr);

    void checkIn();
    void checkOut();

    static void saveToFile(const std::vector<Booking>& bookings, const std::string& filename);
    static std::vector<Booking> loadFromFile(const std::string& filename);
};



#endif //HOTEL_MANAGAR_BOOKING_H
