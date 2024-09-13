#include "Airport.h"
#include "Airplane.h"
#include "FileReader.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include <iostream>
FileReader::FileReader(Airport& inAirport): airport(inAirport) {
}

void FileReader::read() {
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
        map<int, string> pricing;
        string date;
        string flightNo;
        int numSeat;
        string seats;
        string price;
        string endSeat;

        if (!(iss >> date >> flightNo >> numSeat)) {
            cout << "Error reading date, flightNo, or numSeat from line: " << line << endl;
            exit(0);
        }

        while (iss >> seats >> price) {
            size_t dashPos = seats.find('-');
            if (dashPos == string::npos || dashPos==0) {
                cout << "Invalid seat range format in line: " << line << endl;
                exit(0);
            }
            if (price.back()!= '$') {
                cout << "Invalid price format in line: " << line << endl;
                exit(0);
            }
            endSeat = seats.substr(dashPos + 1);

            int endSeatNumber = stoi(endSeat);
            pricing[endSeatNumber] = price;
            seats = "";
            price = "";
        }
        if (!price.empty() || !seats.empty()) {
            cout << "Invalid format in line: " << line << endl;
            exit(0);
        }
        if (!pricing.empty()) {
            shared_ptr<Airplane> plane = make_shared<Airplane>(numSeat, stoi(endSeat), make_shared<map<int, string>>(pricing));
            airport.addPlane(plane, date, flightNo);
        }
        else exit(0);
    }

    inputFile.close();
}