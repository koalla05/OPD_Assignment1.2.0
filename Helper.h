#pragma once
#include "Airport.h"
#include "Ticket.h"

class Helper {
    Airport airport;
public:
    Helper (const Airport& myAirport);

    string getRowPrice(const string& date, const string& flight, const int& row) const;

    void check(const string& date, const string& flight) const;

    void book(const string& date, const string& flight, const string& place, const string& userName);

    void refund(const int& id);

    void view(const int& id);

    void view(const string& userName);

    void view(const string& date, const string& flight);

};