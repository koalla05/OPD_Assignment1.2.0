#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <unordered_map>
#include <vector>
using namespace std;

struct Ticket {
    int row;
    char seat;
    string userName;
};

class PayFolder {
    vector<Ticket> tickets;
    unordered_map<string, vector<int>> userIds;
};

class Airplane {
public:
    vector<bool> availability;
    Airplane(int numSeat, int maxRow) {
        for (int i=1; i < maxRow*numSeat; i++) {
            availability[i] = false;
        }
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
    unordered_map<pair<string, string>, Airplane*, pair_hash> planes;
    void addPlane(const string& inFlight, const string& inDate, const int& inSeatNum, const int& maxRow) {

        planes[make_pair(inDate, inFlight)] = new Airplane (inSeatNum, maxRow);
    }
};

class FileReader {
    string path = "C:/Users/kovko/CLionProjects/OPD_Assignment1.2.0/config.txt";
    string date;
    string flightNo;
    int numSeat;
    int numRow;
    string price, seats;
    string endSeat;
    map<int, string> pricing;
    Airport& airport;
public:
    FileReader(Airport& inAirport): airport(inAirport) {}
    void read() {
        ifstream inputFile(path);

        if (!inputFile) {
            cerr << "No file has been found: " << path << endl;
            return;
        }

        string line;
        istringstream iss(line);
        getline(inputFile, line);

        while (getline(inputFile, line)) {
            istringstream iss(line);
            pricing.clear();
            iss >> date >> flightNo >> numSeat;

            while (iss >> seats >> price) {
                endSeat = seats.substr(seats.find('-') + 1);
                pricing[stoi(endSeat)] = price;
            }
            airport.addPlane(flightNo, date, numSeat, pricing.rbegin()->first);
        }
        inputFile.close();
    }
};

class Helper {
    unordered_map<string, vector<long>> userIds;
    Airport airport;
    long id;
public:
    Helper (Airport& myAirport): airport(myAirport), id(1){}

    void check(){}
};

int main()
{
    Airport myAirport;
    FileReader file(myAirport);
    file.read();
    cout << "Hello, World!" << endl;
    return 0;
}
