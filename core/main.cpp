#include <iostream>
#include <fstream>
#include "Hotel.h"
#include "Date.h"

int main() {
    std::cout << "========================================\n";
    std::cout << "   HOTEL SYSTEM - FILE DEMONSTRATION\n";
    std::cout << "========================================\n\n";

    // 1. Создаем папку data если её нет
#ifdef _WIN32
    system("mkdir data 2>nul");
#else
    system("mkdir -p data 2>/dev/null");
#endif

    // 2. Создаем отель
    Hotel hotel("Grand Hotel", "Moscow, Tverskaya str. 1");

    std::cout << "STEP 1: Checking data files...\n";
    std::cout << "-----------------------------\n";

    // Проверяем существование файлов
    std::ifstream roomsFile("data/rooms.txt");
    std::ifstream clientsFile("data/clients.txt");

    if (!roomsFile.is_open()) {
        std::cout << "ERROR: data/rooms.txt not found!\n";
        std::cout << "Please create this file with room data.\n";
        return 1;
    }

    if (!clientsFile.is_open()) {
        std::cout << "ERROR: data/clients.txt not found!\n";
        std::cout << "Please create this file with client data.\n";
        return 1;
    }

    std::cout << "✓ data/rooms.txt exists\n";
    std::cout << "✓ data/clients.txt exists\n";

    roomsFile.close();
    clientsFile.close();

    std::cout << "\nSTEP 2: Loading rooms and clients from files...\n";
    std::cout << "-----------------------------------------------\n";

    try {
        hotel.loadAllData();
        std::cout << "✓ Data loaded successfully!\n";
        std::cout << "  Rooms loaded: " << hotel.getTotalRooms() << "\n";
        std::cout << "  Clients loaded: " << hotel.getTotalClients() << "\n";
        std::cout << "  Bookings loaded: " << hotel.getTotalBookings() << "\n";
    } catch (const std::exception& e) {
        std::cout << "✗ Error loading data: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "\nSTEP 3: Creating test bookings...\n";
    std::cout << "--------------------------------\n";

    try {
        // Создаем несколько бронирований
        Date checkIn1(20, 12, 2025);
        Date checkOut1(25, 12, 2025);
        Date checkIn2(26, 12, 2025);
        Date checkOut2(3, 1, 2026);
        Date checkIn3(10, 1, 2026);
        Date checkOut3(15, 1, 2026);

        // Бронирование 1: Клиент 1001, Комната 101
        std::cout << "Creating booking 1: Client 1001, Room 101... ";
        bool booking1 = hotel.createBooking(1001, 101, checkIn1, checkOut1, 15000.0);
        if (booking1) {
            std::cout << "SUCCESS\n";
            std::cout << "  Dates: " << checkIn1.ConvertToString() << " - "
                      << checkOut1.ConvertToString() << "\n";
            std::cout << "  Price: 15000.00 RUB\n";
        } else {
            std::cout << "FAILED (room may be unavailable)\n";
        }

        // Бронирование 2: Клиент 1002, Комната 103
        std::cout << "\nCreating booking 2: Client 1002, Room 103... ";
        bool booking2 = hotel.createBooking(1002, 103, checkIn2, checkOut2, 30000.0);
        if (booking2) {
            std::cout << "SUCCESS\n";
            std::cout << "  Dates: " << checkIn2.ConvertToString() << " - "
                      << checkOut2.ConvertToString() << "\n";
            std::cout << "  Price: 30000.00 RUB\n";
        } else {
            std::cout << "FAILED (room may be unavailable)\n";
        }

        // Бронирование 3: Клиент 1003, Комната 105
        std::cout << "\nCreating booking 3: Client 1003, Room 105... ";
        bool booking3 = hotel.createBooking(1003, 105, checkIn3, checkOut3, 20000.0);
        if (booking3) {
            std::cout << "SUCCESS\n";
            std::cout << "  Dates: " << checkIn3.ConvertToString() << " - "
                      << checkOut3.ConvertToString() << "\n";
            std::cout << "  Price: 20000.00 RUB\n";
        } else {
            std::cout << "FAILED (room may be unavailable)\n";
        }

        std::cout << "\nTotal bookings created: "
                  << (booking1 + booking2 + booking3) << "/3\n";

    } catch (const std::exception& e) {
        std::cout << "Error creating bookings: " << e.what() << std::endl;
    }

    std::cout << "\nSTEP 4: Saving all data to files...\n";
    std::cout << "-----------------------------------\n";

    try {
        hotel.saveAllData();
        std::cout << "✓ All data saved successfully!\n";
        std::cout << "  Rooms saved to: data/rooms.txt\n";
        std::cout << "  Clients saved to: data/clients.txt\n";
        std::cout << "  Bookings saved to: data/bookings.txt\n";
    } catch (const std::exception& e) {
        std::cout << "✗ Error saving data: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "\nSTEP 5: Showing content of bookings.txt...\n";
    std::cout << "-----------------------------------------\n";

    std::ifstream bookingsFile("data/bookings.txt");
    if (bookingsFile.is_open()) {
        std::cout << "Content of data/bookings.txt:\n";
        std::cout << "------------------------------\n";

        std::string line;
        int lineNum = 1;
        while (std::getline(bookingsFile, line)) {
            if (!line.empty()) {
                std::cout << "Booking " << lineNum << ": " << line << std::endl;
                lineNum++;
            }
        }

        if (lineNum == 1) {
            std::cout << "(File is empty)\n";
        }

        bookingsFile.close();
    } else {
        std::cout << "Could not open data/bookings.txt for reading\n";
    }

    std::cout << "\nSTEP 6: Final summary\n";
    std::cout << "--------------------\n";

    std::cout << "Hotel: " << hotel.getName() << "\n";
    std::cout << "Total rooms in system: " << hotel.getTotalRooms() << "\n";
    std::cout << "Total clients in system: " << hotel.getTotalClients() << "\n";
    std::cout << "Total bookings in system: " << hotel.getTotalBookings() << "\n";

    std::cout << "\n========================================\n";
    std::cout << "   DEMONSTRATION COMPLETED SUCCESSFULLY\n";
    std::cout << "========================================\n";

    return 0;
}