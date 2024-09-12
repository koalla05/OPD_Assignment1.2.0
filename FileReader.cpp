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

        if (!(iss >> date >> flightNo >> numSeat)) {
            cerr << "Error reading date, flightNo, or numSeat from line: " << line << endl;
            continue;
        }

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
