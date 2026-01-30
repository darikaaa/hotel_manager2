

#include "Booking.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

Booking::Booking(int id, Room *_room, Client *_client,
                 Date checkInDate, Date checkOutDate, double _price,
                 BookingStatus initialStatus)
        : bookingId(id), room(_room), client(_client),
          checkedIn(checkInDate), checkedOut(checkOutDate),
          totalPrice(_price), status(initialStatus) {
}

int Booking::getBookingId() const {
    return bookingId;
}

Room* Booking::getRoom() const {
    return room;
}

Client* Booking::getClient() const {
    return client;
}

Date Booking::getCheckedIn() const {
    return checkedIn;
}

Date Booking::getCheckedOut() const {
    return checkedOut;
}

double Booking::getTotalPrice() const {
    return totalPrice;
}

BookingStatus Booking::getStatus() const {
    return status;
}

int Booking::getRoomId() const {
    return room ? room->getRoomNumber() : 0;
}

int Booking::getClientId() const {
    return client ? client->getId() : 0;
}

void Booking::setStatus(BookingStatus newStatus) {
    status = newStatus;
}

void Booking::setRoom(Room* newRoom) {
    room = newRoom;
}

void Booking::setClient(Client* clientPtr) {
    client = clientPtr;
}

int Booking::calculateNights() const {
    return checkedIn.calculateDaysBetween(checkedOut);
}

double Booking::calculatePricePerNight() const {
    int nights = calculateNights();
    if (nights <= 0) {
        throw std::runtime_error("Staying nights value = " + std::to_string(nights));
    }
    return totalPrice / static_cast<double>(nights);
}

void Booking::checkIn() {
    status = BookingStatus::CheckedIn;
}

void Booking::checkOut() {
    status = BookingStatus::CheckedOut;
}

void Booking::saveToFile(const std::vector<Booking>& bookings, const std::string& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw "Cannot open file";
    }

    for (const Booking& booking : bookings) {
        int safeRoomId = booking.getRoom() ? booking.getRoom()->getRoomNumber() : 0;
        int safeClientId = booking.getClient() ? booking.getClient()->getId() : 0;
        file << booking.getBookingId() << "|" << safeRoomId << "|" << safeClientId << "|"
             << booking.getCheckedIn().ConvertToString() << "|"
             << booking.getCheckedOut().ConvertToString() << "|"
             << booking.getTotalPrice() << "|";

        if (booking.getStatus() == BookingStatus::Booked) file << "Booked";
        else if (booking.getStatus() == BookingStatus::CheckedIn) file << "CheckedIn";
        else if (booking.getStatus() == BookingStatus::CheckedOut) file << "CheckedOut";
        else file << "Canceled";

        file << "\n";
    }

    file.close();
}

std::vector<Booking> Booking::loadFromFile(const std::string& filename) {
    std::vector<Booking> bookings;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw "Cannot open file";
    }

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        lineNumber++;
        if (line.empty()) continue;

        std::vector<std::string> parts;
        std::string part;
        std::stringstream ss(line);

        while (getline(ss, part, '|')) {
            parts.push_back(part);
        }

        if (parts.size() != 7) {
            std::cerr << "Warning: Invalid line format on line " << lineNumber
                      << ". Expected 7 parts, got " << parts.size() << std::endl;
            continue;
        }

        try {
            int id = std::stoi(parts[0]);

            std::string checkInStr = parts[3];
            size_t dot1 = checkInStr.find('.');
            size_t dot2 = checkInStr.rfind('.');

            if (dot1 == std::string::npos || dot2 == std::string::npos || dot1 == dot2) {
                throw std::runtime_error("Invalid date format for check-in");
            }

            int checkInDay = std::stoi(checkInStr.substr(0, dot1));
            int checkInMonth = std::stoi(checkInStr.substr(dot1 + 1, dot2 - dot1 - 1));
            int checkInYear = std::stoi(checkInStr.substr(dot2 + 1));

            std::string checkOutStr = parts[4];
            dot1 = checkOutStr.find('.');
            dot2 = checkOutStr.rfind('.');

            if (dot1 == std::string::npos || dot2 == std::string::npos || dot1 == dot2) {
                throw std::runtime_error("Invalid date format for check-out");
            }

            int checkOutDay = std::stoi(checkOutStr.substr(0, dot1));
            int checkOutMonth = std::stoi(checkOutStr.substr(dot1 + 1, dot2 - dot1 - 1));
            int checkOutYear = std::stoi(checkOutStr.substr(dot2 + 1));

            double price = std::stod(parts[5]);
            std::string statusStr = parts[6];

            Date checkInDate(checkInDay, checkInMonth, checkInYear);
            Date checkOutDate(checkOutDay, checkOutMonth, checkOutYear);

            BookingStatus bookingStatus = BookingStatus::Booked;
            if (statusStr == "Booked") bookingStatus = BookingStatus::Booked;
            else if (statusStr == "CheckedIn") bookingStatus = BookingStatus::CheckedIn;
            else if (statusStr == "CheckedOut") bookingStatus = BookingStatus::CheckedOut;
            else if (statusStr == "Canceled") bookingStatus = BookingStatus::Canceled;

            Booking booking(id,
                            nullptr,
                            nullptr,
                            checkInDate,
                            checkOutDate,
                            price,
                            bookingStatus);

            bookings.push_back(booking);

        } catch (const std::exception& e) {
            std::cerr << "Error on line " << lineNumber << ": " << e.what()
                      << "\nLine content: " << line << std::endl;
        }
    }
    file.close();
    return bookings;
}