//
// Created by Asus on 25.01.2026.
//
#include <algorithm>
#include "Room.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

Room::Room(int number, RoomType roomType, double price, int roomCapacity)
        : RoomNumber(number), type(roomType), BasePriceForNight(price),
          capacity(roomCapacity), status(RoomStatus::Available) {
    if (price < 0) {
        throw std::invalid_argument("Price cannot be negative");
    }
    if (capacity <= 0) {
        throw std::invalid_argument("Capacity must be positive");
    }
    if (number <= 0) {
        throw std::invalid_argument("Room number must be positive");
    }
}

int Room::getRoomNumber() const {
    return RoomNumber;
}

RoomType Room::getType() const {
    return type;
}

double Room::getBasePrice() const {
    return BasePriceForNight;
}

RoomStatus Room::getStatus() const {
    return status;
}

int Room::getCapacity() const {
    return capacity;
}

const std::vector<std::string>& Room::getAdditionalServices() const {
    return AdditionalServices;
}

void Room::setBasePrice(double price) {
    if (price < 0) {
        throw std::invalid_argument("Invalid price");
    }
    BasePriceForNight = price;
}

void Room::setStatus(RoomStatus newStatus) {
    status = newStatus;
}

void Room::addService(const std::string& service) {
    if (!hasService(service)) {
        AdditionalServices.push_back(service);
    }
}

void Room::removeService(std::string& service) {
    auto it = std::remove(AdditionalServices.begin(), AdditionalServices.end(), service);
    AdditionalServices.erase(it, AdditionalServices.end());
}

bool Room::hasService(const std::string& service) const {
    return std::find(AdditionalServices.begin(), AdditionalServices.end(), service)
           != AdditionalServices.end();
}

bool Room::isAvailable() const {
    return status == RoomStatus::Available;
}

void Room::saveToFile(const std::vector<Room>& rooms, const std::string& filename) {
    std::ofstream file(filename);

    if (!file) {
        throw "File is not opened";
    }

    for (const Room& room : rooms) {
        file << room.getRoomNumber() << "|"
             << static_cast<int>(room.getType()) << "|"
             << room.getBasePrice() << "|"
             << room.getCapacity() << "|"
             << static_cast<int>(room.getStatus()) << "|";

        const auto& services = room.getAdditionalServices();
        for (size_t i = 0; i < services.size(); ++i) {
            file << services[i];
            if (i != services.size() - 1) {
                file << ",";
            }
        }

        file << "\n";
    }

    file.close();
}

RoomType Room::stringToRoomType(const std::string& str) {
    if (str == "Single") return RoomType::Single;
    if (str == "Double") return RoomType::Double;
    if (str == "Family") return RoomType::Family;
    if (str == "Suite") return RoomType::Suite;
    throw std::invalid_argument("Unknown room type: " + str);
}

RoomStatus Room::stringToRoomStatus(const std::string& str) {
    if (str == "Available") return RoomStatus::Available;
    if (str == "Occupied") return RoomStatus::Occupied;
    if (str == "Maintenance") return RoomStatus::Maintenance;
    if (str == "Cleaning") return RoomStatus::Cleaning;
    throw std::invalid_argument("Unknown room status: " + str);
}

std::vector<Room> Room::loadFromFile(const std::string& filename) {
    std::vector<Room> rooms;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        lineNumber++;

        if (line.empty()) {
            continue;
        }

        std::vector<std::string> parts;
        std::stringstream ss(line);
        std::string part;

        while (std::getline(ss, part, '|')) {
            parts.push_back(part);
        }

        if (parts.size() < 5) {
            std::cerr << "Line " << lineNumber << ": Invalid format, skipping\n";
            continue;
        }

        try {
            int roomNumber = std::stoi(parts[0]);
            RoomType type = static_cast<RoomType>(std::stoi(parts[1]));
            double price = std::stod(parts[2]);
            int capacity = std::stoi(parts[3]);
            RoomStatus status = static_cast<RoomStatus>(std::stoi(parts[4]));

            Room room(roomNumber, type, price, capacity);
            room.setStatus(status);

            if (parts.size() > 5 && !parts[5].empty()) {
                std::stringstream servicesStream(parts[5]);
                std::string service;

                while (std::getline(servicesStream, service, ',')) {
                    size_t start = service.find_first_not_of(' ');
                    size_t end = service.find_last_not_of(' ');
                    if (start != std::string::npos && end != std::string::npos) {
                        room.addService(service.substr(start, end - start + 1));
                    }
                }
            }
            rooms.push_back(room);
        } catch (const std::exception& e) {
            std::cerr << "Line " << lineNumber << ": Error - " << e.what() << "\n";
        }
    }
    return rooms;
}