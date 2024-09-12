#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;

struct Ticket {
    const string date;
    const string flight;
    const int row;
    char seat;
    const string userName;
    Ticket(const string& inFlight, const string& inDate, const int& inRow, const char& inSeat, const string& inUserName): row(inRow), seat(inSeat),  userName(inUserName), date(inDate), flight(inFlight){}
};

class Airplane {
    const int numSeat;
    const int maxRow;
    vector<bool> places;
    shared_ptr<const map<int, string>> pricing;

public:
    Airplane(const int& inNumSeat, const int& inMaxRow, const shared_ptr<map<int, string>> &inPricing): numSeat(inNumSeat), maxRow(inMaxRow), pricing(inPricing) {
        places.resize(maxRow * numSeat, true);
    }

    string getPrice(const int &row) const {
        string price = pricing->lower_bound(row)->second;
        return price;
    }

    void check() {
        for (int i = 0; i < maxRow * numSeat; ++i) {
            if (places[i]) {
                string price = getPrice(i / numSeat + 1);
                cout << i / numSeat + 1 << static_cast<char>(i % numSeat + 'A') << " " << price << endl;
            }
        }
    }

    shared_ptr<Ticket> book(const string& flight, const string& date, const string &seat, const string& userName) {
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

    string refundTicket(const int& row, const char&  place) {
        string price = getPrice(row);
        int index = row * numSeat - 'A' + place - numSeat;
        places[index] = true;
        return price;
    }
};

struct pair_hash {
    size_t operator () (const pair<string, string>& p) const {
        size_t h1 = hash<std::string>{}(p.first);
        size_t h2 = hash<std::string>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

class Airport {
public:
    vector<shared_ptr<Ticket>> booked = {};
    unordered_map<pair<string, string>, shared_ptr<Airplane>, pair_hash> planes;

    void addPlane(shared_ptr<Airplane> plane, const string& inDate, const string& inFlight) {
        planes[make_pair(inDate, inFlight)] = std::move(plane);
    }

    void bookTicket(const shared_ptr<Ticket>& ticket) {
        booked.push_back(ticket);
        cout << "Confirmed with id " << booked.size() << endl;
    }

    void refundTicket(const int& id) {
        if (id > booked.size() || id < 0) {
            cout << "Sorry, we couldn't have found a booked ticket with such an ID" <<endl;
            return;
        }
        shared_ptr<Ticket> ticket = booked[id - 1];
        if (ticket) {
            string price = planes[make_pair(ticket->date, ticket->flight)]->refundTicket(ticket->row, ticket->seat);
            cout << "Confirmed refund "<< price << " for " << ticket->userName << endl;
            booked[id - 1] = nullptr;
        }
        else {
            cout << "Sorry, seems like you already have refund" <<endl;
        }
    }

    shared_ptr<Ticket> view(const int& id) const {
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

    vector<shared_ptr<Ticket>> view(const string& userName) {
        vector<shared_ptr<Ticket>> tickets;
        for (const shared_ptr<Ticket>& ticket : booked) {
            if (ticket->userName == userName) {
                tickets.push_back(ticket);
            }
        }
        return tickets;
    }

    vector<shared_ptr<Ticket>> view(const string& date, const string& flight) {
        vector<shared_ptr<Ticket>> tickets;
        for (const shared_ptr<Ticket>& ticket : booked) {
            if (ticket->date == date && ticket->flight == flight) {
                tickets.push_back(ticket);
            }
        }
        return tickets;
    }
};

class FileReader {
    string path = "C:/Users/kovko/CLionProjects/OPD_Assignment1.2.0/config.txt";
    string date;
    string flightNo;
    int numSeat = 0;
    int numRow;
    string price, seats;
    string endSeat;
    Airport& airport;

public:
    FileReader(Airport& inAirport) : airport(inAirport) {}

    void read() {
        ifstream inputFile(path);

        if (!inputFile) {
            cerr << "No file has been found: " << path << endl;
            return;
        }

        string line;
        istringstream iss(line);
        getline(inputFile, line);

        // Process each line in the file
        while (getline(inputFile, line)) {
            istringstream iss(line);
            map<int, string> pricing;

            if (!(iss >> date >> flightNo >> numSeat)) {
                cerr << "Error reading date, flightNo, or numSeat from line: " << line << endl;
                continue;
            }

            // Process the seat and price pairs
            while (iss >> seats >> price) {
                size_t dashPos = seats.find('-');
                if (dashPos == string::npos) {
                    cerr << "Invalid seat range format in line: " << line << endl;
                    continue;
                }
                endSeat = seats.substr(dashPos + 1);

                int endSeatNumber = stoi(endSeat);
                pricing[endSeatNumber] = price;

            }

            if (!pricing.empty()) {
                shared_ptr<Airplane> plane = make_shared<Airplane>(numSeat, stoi(endSeat), make_shared<map<int, string>>(pricing));
                airport.addPlane(plane, date, flightNo);
            }
        }

        inputFile.close();
    }
};

class Helper {
    Airport airport;
    long id;
public:
    Helper (Airport& myAirport): airport(myAirport), id(1){}

    string getRowPrice(const string& date, const string& flight, const int& row) {
        for (const auto& pair : airport.planes) {
            const auto& key = pair.first;
            shared_ptr<Airplane> airplane = pair.second;
            if (key.first == date && key.second == flight) {
                return airplane->getPrice(row);
            }
        }
    }

    void check(const string& date, const string& flight) {
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

    void book(const string& date, const string& flight, const string& place, const string& userName) {
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

    void refund(const int& id) {
        airport.refundTicket(id);
    }

    void view(const int& id) {
        const shared_ptr<Ticket> ticket = airport.view(id);
        const string price = getRowPrice(ticket->date, ticket->flight, ticket->row);
        cout << ticket->date << " " << ticket->flight << " " << ticket->row << ticket->seat << " " << price << " " << ticket->userName << endl;
    }

    void view(const string& userName) {
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

    void view(const string& date, const string& flight) {
        vector<shared_ptr<Ticket>> tickets = airport.view(date, flight);
        if (tickets.empty()) {
            cout << "Sorry, no planes in that day and with that flight number" << endl;
            return;
        }
        for (auto& ticket: tickets) {
            const string price = getRowPrice(ticket->date, ticket->flight, ticket->row);
            cout << ticket-> row << ticket -> seat << " " << ticket ->userName << " " << price  << endl;
        }

    }

};

int main()
{
    Airport myAirport;
    FileReader file(myAirport);
    file.read();
    Helper helper(myAirport);
    //helper.check("01.03.2023", "TI678");
    helper.book("01.03.2023", "TI678", "1A", "Alla");
    helper.book("01.03.2023", "TI678", "2A", "Alla");

    //helper.book("01.03.2023", "TI678", "1A", "Alla");
    //helper.check("01.03.2023", "TI678");
    // helper.book("01.03.2023", "TI678", "2B", "Oliver");
    // helper.book("17.03.2023", "ZV456", "12C", "Alla");
    // helper.refund(1);
    // helper.refund(1);
    // helper.refund(3);
    // helper.book("01.03.2023", "TI678", "1A", "Oliver");
    //
    //helper.view(2);
    //helper.view(1);
    // helper.refund(1);
    // helper.view(1);

    // helper.view(3);
    //helper.view("All");
    helper.view("01.03.203", "TI678");
    return 0;
}
