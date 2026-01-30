
#include "Hotel.h"
#include <iostream>
#include <algorithm>
#include <fstream>

Hotel::Hotel(const std::string& hotelName, const std::string& hotelAddress){
    name = hotelName;
    address = hotelAddress;
}

std::string Hotel::getName() const {
    return name;
}
std::string Hotel::getAddress() const {
    return address;
}
int Hotel::getTotalRooms() const {
    return static_cast<int>(rooms.size());
}
int Hotel::getTotalClients() const {
    return static_cast<int>(clients.size());
}
int Hotel::getTotalBookings() const {
    return static_cast<int>(bookings.size());
}

void Hotel::loadAllData() {
    rooms = Room::loadFromFile("data/rooms.txt");
    clients = Client::loadFromFile("data/clients.txt");
    bookings = Booking::loadFromFile("data/bookings.txt");

    for (auto &booking : bookings) {
        Room* room = findRoomById(booking.getRoomId());
        Client* client = findClientById(booking.getClientId());
        if (room) booking.setRoom(room);
        if (client) booking.setClient(client);
    }
}

void Hotel::saveAllData() {
    try {
        Room::saveToFile(rooms, "data/rooms.txt");
        Client::saveToFile(clients, "data/clients.txt");
        Booking::saveToFile(bookings, "data/bookings.txt");
    } catch (const char* error) {
        std::cerr << "Data saving error :" << error << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Data saving error :" << e.what() << std::endl;
    }
}

void Hotel::addRoom(Room room) {
    for (Room existingRoom : rooms) {
        if (existingRoom.getRoomNumber() == room.getRoomNumber()) {
            throw "Room with that number alredy exists";
        }
    }
    rooms.push_back(room);
}
bool Hotel::isRoomAvailable(int roomId, Date checkIn, Date checkOut) const {
    for (Booking booking : bookings) {
        if (booking.getRoomId() == roomId && booking.getStatus() != BookingStatus::Canceled) {
            bool hasConflict = !(checkOut.getYear() < booking.getCheckedIn().getYear() ||
                                 (checkOut.getYear() == booking.getCheckedIn().getYear() &&
                                  checkOut.getMonth() < booking.getCheckedIn().getMonth()) ||
                                 (checkOut.getYear() == booking.getCheckedIn().getYear() &&
                                  checkOut.getMonth() == booking.getCheckedIn().getMonth() &&
                                  checkOut.getDay() <= booking.getCheckedIn().getDay()) ||
                                 checkIn.getYear() > booking.getCheckedOut().getYear() ||
                                 (checkIn.getYear() == booking.getCheckedOut().getYear() &&
                                  checkIn.getMonth() > booking.getCheckedOut().getMonth()) ||
                                 (checkIn.getYear() == booking.getCheckedOut().getYear() &&
                                  checkIn.getMonth() == booking.getCheckedOut().getMonth() &&
                                  checkIn.getDay() >= booking.getCheckedOut().getDay()));

            if (hasConflict) {
                return false;
            }
        }
    }
    return true;
}

bool Hotel::removeRoom(int roomNumber) {
    for (auto it = rooms.begin(); it != rooms.end(); ++it) {
        if (it->getRoomNumber() == roomNumber) {
            for (Booking booking : bookings) {
                if (booking.getRoomId() == roomNumber &&
                    (booking.getStatus() == BookingStatus::Booked ||
                     booking.getStatus() == BookingStatus::CheckedIn)) {
                    std::cerr << "Cannot delete room that already booked" << std::endl;
                    return false;
                }
            }
            rooms.erase(it);
            return true;
        }
    }
    std::cerr << "Room №" << roomNumber << " is not found" << std::endl;
    return false;
}
Room* Hotel::findRoomById(int roomNumber) {
    for (auto& room : rooms) {
        if (room.getRoomNumber() == roomNumber) {
            return &room;
        }
    }
    return nullptr;
}

std::vector<Room*> Hotel::getRoomsByType(RoomType type) {
    std::vector<Room*> result;
    for (auto& room : rooms) {
        if (room.getType() == type) {
            result.push_back(&room);
        }
    }
    return result;
}

std::vector<Room*> Hotel::getAvailableRooms(Date checkIn, Date checkOut) {
    std::vector<Room*> availableRooms;
    for (auto& room : rooms) {
        if (isRoomAvailable(room.getRoomNumber(), checkIn, checkOut)) {
            availableRooms.push_back(&room);
        }
    }
    return availableRooms;
}

void Hotel::addClient(const Client& client) {
    if (findClientById(client.getId())) {
        throw "Client already exists";
    }
    clients.push_back(client);
}

Client* Hotel::findClientById(int clientId) {
    for (auto& client : clients) {
        if (client.getId() == clientId) {
            return &client;
        }
    }
    return nullptr;
}

Client* Hotel::findClientByPhone(const std::string& phone) {
    for (auto& client : clients) {
        if (client.getPhone() == phone) {
            return &client;
        }
    }
    return nullptr;
}

bool Hotel::createBooking(int clientId, int roomId, Date checkIn, Date checkOut, double price) {
    Client* client = findClientById(clientId);
    Room* room = findRoomById(roomId);
    if (!client || !room) {
        std::cerr << "Client or room not found" << std::endl;
        return false;
    }
    if (!isRoomAvailable(roomId, checkIn, checkOut)) {
        std::cerr << "Room is not available" << std::endl;
        return false;
    }
    int newId = bookings.empty() ? 1 : bookings.back().getBookingId() + 1;
    try {
        bookings.emplace_back(newId, room, client, checkIn, checkOut, price);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Booking creation failed: " << e.what() << std::endl;
        return false;
    }
}

bool Hotel::cancelBooking(int bookingId) {
    auto* booking = findBookingById(bookingId);
    if (!booking || booking->getStatus() != BookingStatus::Booked) {
        throw "Cannot cancel booking";
    }
    booking->setStatus(BookingStatus::Canceled);
    return true;
}

bool Hotel::checkIn(int bookingId) {
    auto* booking = findBookingById(bookingId);
    if (!booking || booking->getStatus() != BookingStatus::Booked) {
        throw "Cannot check in";
    }

    booking->checkIn();
    if (booking->getRoom()) {
        booking->getRoom()->setStatus(RoomStatus::Occupied);
    }
    return true;
}

bool Hotel::checkOut(int bookingId) {
    auto* booking = findBookingById(bookingId);
    if (!booking || booking->getStatus() != BookingStatus::CheckedIn) {
        throw "Cannot check out";
    }

    booking->checkOut();
    if (booking->getRoom()) {
        booking->getRoom()->setStatus(RoomStatus::Available);
    }
    return true;
}

std::vector<Booking*> Hotel::getBookingsForDate(Date date) {
    std::vector<Booking*> result;
    for (auto& booking : bookings) {
        bool isActiveOnDate = !(date.getYear() < booking.getCheckedIn().getYear() ||
                                (date.getYear() == booking.getCheckedIn().getYear() &&
                                 date.getMonth() < booking.getCheckedIn().getMonth()) ||
                                (date.getYear() == booking.getCheckedIn().getYear() &&
                                 date.getMonth() == booking.getCheckedIn().getMonth() &&
                                 date.getDay() < booking.getCheckedIn().getDay())) &&
                              !(date.getYear() > booking.getCheckedOut().getYear() ||
                                (date.getYear() == booking.getCheckedOut().getYear() &&
                                 date.getMonth() > booking.getCheckedOut().getMonth()) ||
                                (date.getYear() == booking.getCheckedOut().getYear() &&
                                 date.getMonth() == booking.getCheckedOut().getMonth() &&
                                 date.getDay() > booking.getCheckedOut().getDay()));

        if (isActiveOnDate && booking.getStatus() != BookingStatus::Canceled) {
            result.push_back(&booking);
        }
    }
    return result;
}

Booking* Hotel::findBookingById(int bookingId) {
    auto it = std::find_if(bookings.begin(), bookings.end(),
                           [bookingId](Booking& b) { return b.getBookingId() == bookingId; });
    return it != bookings.end() ? &(*it) : nullptr;
}

std::vector<Booking*> Hotel::getClientBookings(int clientId) {
    std::vector<Booking*> result;
    for (auto& booking : bookings) {
        if (booking.getClientId() == clientId) {
            result.push_back(&booking);
        }
    }
    return result;
}

std::vector<Booking*> Hotel::getActiveBookings() {
    std::vector<Booking*> result;
    for (auto& booking : bookings) {
        if (booking.getStatus() == BookingStatus::Booked ||
            booking.getStatus() == BookingStatus::CheckedIn) {
            result.push_back(&booking);
        }
    }
    return result;
}

std::vector<Booking*> Hotel::getBookingsByStatus(BookingStatus status) {
    std::vector<Booking*> result;
    for (auto& booking : bookings) {
        if (booking.getStatus() == status) {
            result.push_back(&booking);
        }
    }
    return result;
}