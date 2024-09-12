#pragma once
#include <map>
#include <memory>
#include <vector>
#include "Ticket.h"
using namespace std;

class Airplane {
    const int numSeat;
    const int maxRow;
    vector<bool> places;
    shared_ptr<const map<int, string>> pricing;

public:
    Airplane(const int& inNumSeat, const int& inMaxRow, const shared_ptr<map<int, string>> &inPricing);

    string getPrice(const int &row) const;

    void check();

    shared_ptr<Ticket> book(const string& flight, const string& date, const string &seat, const string& userName);

    string refundTicket(const int& row, const char&  place);
};