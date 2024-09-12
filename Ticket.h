#pragma once
#include <string>
using namespace std;

struct Ticket {
    const string date;
    const string flight;
    const int row;
    char seat;
    const string userName;
    Ticket(const string& inFlight, const string& inDate, const int& inRow, const char& inSeat, const string& inUserName): row(inRow), seat(inSeat),  userName(inUserName), date(inDate), flight(inFlight){}
};
