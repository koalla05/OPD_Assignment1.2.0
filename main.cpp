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
    const map<int, string>& pricing;

public:
    Airplane(const int& inNumSeat, const int& inMaxRow, const map<int, string>& inPricing): numSeat(inNumSeat), maxRow(inMaxRow), pricing(inPricing) {
        places.resize(maxRow * numSeat, true);
    }

    void check() {
        for (int i = 0; i < maxRow * numSeat; ++i) {
            if (places[i]) {
                cout << i / numSeat + 1 << static_cast<char>(i % numSeat + 'A') << endl;
            }
        }
    }

    shared_ptr<Ticket> book(const string& flight, const string& date, const string &seat, const string& userName, const int id) {
        shared_ptr<Ticket> ticket;
        char place = seat.back();
        int row = stoi(seat.substr(0, seat.size() - 1));
        int index = row * numSeat - 'A' + place - numSeat;
        if (places[index]) {
            places[index] = false;
            ticket = make_shared<Ticket>(flight, date, row, place, userName);
            return ticket;
        }
        return ticket;
    }

    void refundTicket(const int& row, const char&  place) {
        int index = row * numSeat - 'A' + place - numSeat;
        places[index] = true;
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

    void bookTicket(shared_ptr<Ticket>& ticket) {
        booked.push_back(ticket);
    }

    void refundTicket(const int& id) {
        shared_ptr<Ticket> ticket = booked[id - 1];
        if (ticket != nullptr) {
            planes[make_pair(ticket->date, ticket->flight)]->refundTicket(ticket->row, ticket->seat);
            booked[id - 1] = nullptr;
            cout << "Confirmed refund for id " << id << endl;
        }
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
    map<int, string> pricing;
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
            pricing.clear(); // Clear the map for each new flight entry

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
                shared_ptr<Airplane> plane = make_shared<Airplane>(numSeat, stoi(endSeat), pricing);
                airport.addPlane(plane, date, flightNo );
            }
        }

        inputFile.close();
    }
};

class Helper {
    //unordered_map<string, vector<long>> userIds;
    Airport airport;
    long id;
public:
    Helper (Airport& myAirport): airport(myAirport), id(1){}

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
        if (plane==NULL)
            cout << "Sorry, no airplane in this day with this flight number to book a ticket :(" << endl;
        else {
            shared_ptr<Ticket> ticket = plane->book(flight, date, place, userName, id);
            if (ticket != nullptr) {
                airport.bookTicket(ticket);
            }
            id++;
        }
    }

    void refund(const int& id) {
        airport.refundTicket(id);
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
    helper.book("01.03.2023", "TI678", "2B", "Oliver");
    helper.refund(1);
    cout << "Hello, World!" << endl;
    return 0;
}
