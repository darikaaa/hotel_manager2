//
// Created by Asus on 25.01.2026.
//

#ifndef HOTEL_MANAGAR_DATE_H
#define HOTEL_MANAGAR_DATE_H
#include <string>


class Date {
private:
    int _day;
    int _month;
    int _year;
public:
    Date(int day, int month, int year);
    void setDate(int d, int m, int y);
    void setDay(int d);
    void setMonth(int m);
    void setYear(int y);

    int getDay() const;
    int getMonth() const;
    int getYear() const;

    bool isLeapYear(int year) const;
    int DaysInMonth(int month, int year) const;

    bool isValid() const;
    bool isDayValidForCurrent(int d) const;

    std::string ConvertToString() const;

    int calculateDays() const;
    int calculateDaysBetween(Date secondDate) const;
};


#endif //HOTEL_MANAGAR_DATE_H
