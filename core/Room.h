
#ifndef HOTEL_MANAGAR_ROOM_H
#define HOTEL_MANAGAR_ROOM_H \

#include <vector>
#include <string>

enum class RoomType {
    Single, Double, Family, Suite
};

enum class RoomStatus {
    Available, Occupied, Maintenance, Cleaning
};

class Room {
private:
    int RoomNumber;
    RoomType type;
    int capacity;
    double BasePriceForNight;
    RoomStatus status;
    std::vector<std::string> AdditionalServices;

public:
    Room(int number, RoomType roomType, double price, int roomCapacity);

    int getRoomNumber() const;

    RoomType getType() const;

    double getBasePrice() const;

    RoomStatus getStatus() const;

    int getCapacity() const;

    const std::vector<std::string> &getAdditionalServices() const;

    void setBasePrice(double price);

    void setStatus(RoomStatus newStatus);

    void addService(const std::string& service);

    void removeService(std::string &service);

    bool hasService(const std::string& service) const;

    bool isAvailable() const;

    static void saveToFile(const std::vector<Room> &rooms, const std::string &filename);

    static RoomStatus stringToRoomStatus(const std::string& str);
    static RoomType stringToRoomType(const std::string& str);

    static std::vector<Room> loadFromFile(const std::string &filename);
};

#endif