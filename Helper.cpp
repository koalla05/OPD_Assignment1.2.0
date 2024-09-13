#include "Helper.h"
#include <iostream>
#include "Airplane.h"
#include "Airport.h"
#include "Ticket.h"

Helper::Helper (Airport& myAirport): airport(myAirport){}

string Helper::getRowPrice(const string& date, const string& flight, const int& row) {
    for (const auto& pair : airport.planes) {
        const auto& key = pair.first;
        shared_ptr<Airplane> airplane = pair.second;
        if (key.first == date && key.second == flight) {
            return airplane->getPrice(row);
        }
    }
    return "";
}

void Helper::check(const string& date, const string& flight) const {
    bool found = false;
    for (const auto& pair : airport.planes) {
        const auto& key = pair.first;
        shared_ptr<Airplane> airplane = pair.second;
        if (key.first == date && key.second == flight) {
            airplane->check();
            found = true;
        }
    }
    if (!found)
        cout << "Sorry, no airplane in this day with this flight number :(" << endl;
}

void Helper::book(const string& date, const string& flight, const string& place, const string& userName) {
    auto plane = airport.planes[make_pair(date, flight)];
    if (plane== nullptr)
        cout << "Sorry, no airplane in this day with this flight number to book a ticket :(" << endl;
    else {
        const shared_ptr<Ticket> ticket = plane->book(flight, date, place, userName);
        if (ticket != nullptr) {
            airport.bookTicket(ticket);
        }
    }
}

void Helper::refund(const int& id) {
    airport.refundTicket(id);
}

void Helper::view(const int& id) {
    if (shared_ptr<Ticket> ticket = airport.view(id)) {
        const string price = getRowPrice(ticket->date, ticket->flight, ticket->row);
        cout << ticket->date << " " << ticket->flight << " " << ticket->row << ticket->seat << " " << price << " " << ticket->userName << endl;
    }
}

void Helper::view(const string& userName) {
    vector<shared_ptr<Ticket>> tickets = airport.view(userName);
    if (tickets.empty()) {
        cout << "Sorry, no one with such a name has booked a ticket" << endl;
        return;
    }
    for (auto& ticket: tickets) {
        const string price = getRowPrice(ticket->date, ticket->flight, ticket->row);
        cout << ticket->flight << " " << ticket ->date << " " << ticket-> row << ticket -> seat << " " << price << endl;
    }
}

void Helper::view(const string& date, const string& flight) {
    vector<shared_ptr<Ticket>> tickets = airport.view(date, flight);
    if (tickets.empty()) {
        cout << "Sorry, no tickets for that day and that flight number" << endl;
        return;
    }
    for (const auto& ticket: tickets) {
        const string price = getRowPrice(ticket->date, ticket->flight, ticket->row);
        cout << ticket-> row << ticket -> seat << " " << ticket ->userName << " " << price  << endl;
    }

}
