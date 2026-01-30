
#ifndef HOTEL_MANAGAR_HOTEL_H
#define HOTEL_MANAGAR_HOTEL_H
#include "Room.h"
#include "Client.h"
#include "Booking.h"
#include "Date.h"
#include <vector>
#include <string>
#include <map>

class Hotel {
private:
    std::string name;
    std::string address;
    bool isRoomAvailable(int roomId, Date checkIn, Date checkOut) const;

public:
    std::vector<Room> rooms;
    std::vector<Client> clients;
    std::vector<Booking> bookings;

    std::string getName() const;
    std::string getAddress() const;
    int getTotalRooms() const;
    int getTotalClients() const;
    int getTotalBookings() const;

    Hotel(const std::string& hotelName, const std::string& hotelAddress);

    void loadAllData();
    void saveAllData();

    void addRoom(Room room);
    bool removeRoom(int roomNumber);
    Room* findRoomById(int roomNumber);
    std::vector<Room*> getAvailableRooms(Date checkIn, Date checkOut);
    std::vector<Room*> getRoomsByType(RoomType type);

    void addClient(const Client& client);
    Client* findClientById(int clientId);
    Client* findClientByPhone(const std::string& phone);

    bool createBooking(int clientId, int roomId, Date checkIn, Date checkOut, double price);
    bool cancelBooking(int bookingId);
    bool checkIn(int bookingId);
    bool checkOut(int bookingId);
    Booking* findBookingById(int bookingId);

    std::vector<Booking*> getBookingsForDate(Date date);
    std::vector<Booking*> getClientBookings(int clientId);
    std::vector<Booking*> getActiveBookings();
    std::vector<Booking*> getBookingsByStatus(BookingStatus status);
};


#endif //HOTEL_MANAGAR_HOTEL_H
