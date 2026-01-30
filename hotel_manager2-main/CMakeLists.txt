cmake_minimum_required(VERSION 4.0)
project(hotel_managar)

set(CMAKE_CXX_STANDARD 20)

add_executable(hotel_managar core/main.cpp
        core/Date.cpp
        core/Date.h
        core/Room.cpp
        core/Room.h
        core/Client.cpp
        core/Client.h
        core/Booking.cpp
        core/Booking.h
        core/Hotel.cpp
        core/Hotel.h
        core/PriceManager.cpp
        core/PriceManager.h)
