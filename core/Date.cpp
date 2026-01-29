//
// Created by Asus on 25.01.2026.
//

#include "Date.h"
#include "string"

Date::Date(int day, int month, int year){
    _day = day;
    _month = month;
    _year = year;
    if (!isValid()){
        throw "Invalid date";
    }
}

int Date::getDay() const {
    return _day;
}
int Date::getMonth() const {
    return _month;
}
int Date::getYear() const {
    return _year;
}

void Date::setDay(int d) {
    if (isDayValidForCurrent(d)){
        _day = d;
    }
    else {
        throw "Invalid day";
    }
}
void Date::setMonth(int m) {
    if (m <= 12 && m >=1){
        _month = m;
    }
    else {
        throw "Invalid month";
    }
    if (!isDayValidForCurrent(_day)) {
        _day = DaysInMonth(_month, _year);
    }
}

void Date::setYear(int y) {
    if (y >= 2025 && y <= 2030){
        _year = y;
    }
    else{
        throw "Invalid year";
    }
}
void Date::setDate(int d, int m, int y) {
    Date date(d,m,y);
    if (date.isValid()){
        _day = d;
        _month = m;
        _year = y;
    }
    else{
        throw "Invalid date";
    }
}
bool Date::isLeapYear(int year) const {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
int Date::DaysInMonth(int month, int year) const {
    if (month < 1 || month > 12) {
        return 0;
    }
    switch (month) {
        case 2: if (isLeapYear(year)){
            return 29;
        }
        else  {
            return 28;
        }
        case 4: case 6: case 9: case 11: return 30;
        default: return 31;
    }
}

bool Date::isValid() const {
    return _day >= 1 && _day <= DaysInMonth(_month, _year) && _month >= 1 && _month <= 12 && _year >= 2025 && _year <= 2030;
}

bool Date::isDayValidForCurrent(int d) const {
    return d >= 1 && d <= DaysInMonth(_month, _year);
}

std::string Date::ConvertToString() const {
    return std::to_string(_day)+ "." + std::to_string(_month) + "." + std::to_string(_year);
}

int Date::calculateDays() const {
    int totalDays = 0;
    for (int year = 1; year < _year; year++) {
        totalDays += 365;
        if (isLeapYear(year)) {
            totalDays += 1;
        }
    }
    for (int month = 1; month < _month; month++) {
        totalDays += DaysInMonth(month, _year);
    }
    totalDays += _day;
    return totalDays;
}

int Date::calculateDaysBetween(Date secondDate) const {
    int daysFirstDate = calculateDays();
    int daysSecondDate = secondDate.calculateDays();
    if (daysFirstDate > daysSecondDate) {
        return daysFirstDate - daysSecondDate;
    } else {
        return daysSecondDate - daysFirstDate;
    }
}