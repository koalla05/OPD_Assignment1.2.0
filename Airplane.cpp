#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include "Ticket.h"
#include "Airplane.h"
using namespace std;

Airplane::Airplane(const int& inNumSeat, const int& inMaxRow, const shared_ptr<map<int, string>> &inPricing): numSeat(inNumSeat), maxRow(inMaxRow), pricing(inPricing) {
    places.resize(maxRow * numSeat, true);
}

string Airplane::getPrice(const int &row) const {
    string price = pricing->lower_bound(row)->second;
    return price;
}

void Airplane::check() {
    for (int i = 0; i < maxRow * numSeat; ++i) {
        if (places[i]) {
            string price = getPrice(i / numSeat + 1);
            cout << i / numSeat + 1 << static_cast<char>(i % numSeat + 'A') << " " << price << endl;
        }
    }
}

shared_ptr<Ticket> Airplane::book(const string& flight, const string& date, const string &seat, const string& userName) {
    shared_ptr<Ticket> ticket;
    char place = seat.back();
    int row = stoi(seat.substr(0, seat.size() - 1));
    int index = row * numSeat - 'A' + place - numSeat;
    if (places[index]) {
        places[index] = false;
        ticket = make_shared<Ticket>(flight, date, row, place, userName);
        return ticket;
    }
    else {
        cout << "Sorry, this place is already booked, try again." << endl;
        return ticket;
    }
}

string Airplane::refundTicket(const int& row, const char&  place) {
    string price = getPrice(row);
    int index = row * numSeat - 'A' + place - numSeat;
    places[index] = true;
    return price;
}
