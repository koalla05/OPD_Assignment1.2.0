#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
#include "Airplane.h"
#include "Ticket.h"

using namespace std;

struct pair_hash {
    size_t operator () (const pair<string, string>& p) const {
        size_t h1 = hash<std::string>{}(p.first);
        size_t h2 = hash<std::string>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

class Airport {
    vector<shared_ptr<Ticket>> booked = {};

public:
    unordered_map<pair<string, string>, shared_ptr<Airplane>, pair_hash> planes;

    void addPlane(shared_ptr<Airplane> plane, const string& inDate, const string& inFlight);

    void bookTicket(const shared_ptr<Ticket> ticket);

    void refundTicket(const int& id);

    shared_ptr<Ticket> view(const int& id) const;

    vector<shared_ptr<Ticket>> view(const string& userName);

    vector<shared_ptr<Ticket>> view(const string& date, const string& flight);
};
