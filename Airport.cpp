#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "Airplane.h"
#include "Ticket.h"
#include "Airport.h"
#include <unordered_map>
#include <vector>

using namespace std;

void Airport::addPlane(shared_ptr<Airplane> plane, const string& inDate, const string& inFlight) {
    planes[make_pair(inDate, inFlight)] = std::move(plane);
}

void Airport::bookTicket(const shared_ptr<Ticket>& ticket) {
    booked.push_back(ticket);
    cout << "Confirmed with id " << booked.size() << endl;
}

void Airport::refundTicket(const int& id) {
    if (id > booked.size() || id < 0) {
        cout << "Sorry, we couldn't have found a booked ticket with such an ID" <<endl;
        return;
    }
    const shared_ptr<Ticket> ticket = booked[id - 1];
    if (ticket) {
        string price = planes[make_pair(ticket->date, ticket->flight)]->refundTicket(ticket->row, ticket->seat);
        cout << "Confirmed refund "<< price << " for " << ticket->userName << endl;
        booked[id - 1] = nullptr;
    }
    else {
        cout << "Sorry, seems like you already have refund" <<endl;
    }
}

shared_ptr<Ticket> Airport::view(const int& id) const {
    if (id > booked.size() || id < 0) {
        cout << "Sorry, we couldn't have found a booked ticket with such an ID" <<endl;
        return nullptr;
    }
    shared_ptr<Ticket> ticket = booked[id - 1];
    if (!ticket) {
        cout << "Sorry, no ticket by this id has been found :(" << endl;;
        return nullptr;
    }
    return ticket;
}

vector<shared_ptr<Ticket>> Airport::view(const string& userName) {
    vector<shared_ptr<Ticket>> tickets;
    for (const shared_ptr<Ticket>& ticket : booked) {
        if (ticket && ticket->userName == userName) {
            tickets.push_back(ticket);
        }
    }
    return tickets;
}

vector<shared_ptr<Ticket>> Airport::view(const string& date, const string& flight) {
    vector<shared_ptr<Ticket>> tickets;
    for (const shared_ptr<Ticket>& ticket : booked) {
        if (ticket && ticket->date == date && ticket->flight == flight) {
            tickets.push_back(ticket);
        }
    }
    return tickets;
}
