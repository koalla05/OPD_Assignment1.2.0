#include "FileReader.h"
#include <iostream>
#include <sstream>
#include <map>
#include "FileHandle.h"

FileReader::FileReader(Airport& inAirport, const std::wstring& filePath)
    : airport(inAirport), path(filePath) {
}

void FileReader::read() {

    FileHandle fileHandle(path);

    HANDLE hFile = fileHandle.get();
    DWORD fileSize = GetFileSize(hFile, nullptr);
    if (fileSize == INVALID_FILE_SIZE) {
        throw std::runtime_error("Failed to get file size.");
    }

    std::string fileContent(fileSize, '\0');
    DWORD bytesRead;
    if (!ReadFile(hFile, &fileContent[0], fileSize, &bytesRead, nullptr)) {
        throw std::runtime_error("Failed to read file.");
    }

    std::istringstream inputFile(fileContent);
    std::string line;
    getline(inputFile, line);

    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::map<int, std::string> pricing;

        std::string date, flightNo, seats, price, endSeat;
        int numSeat = 0;

        if (!(iss >> date >> flightNo >> numSeat)) {
            std::cout << "Error reading date, flightNo, or numSeat from line: " << line << std::endl;
            continue;
        }

        while (iss >> seats >> price) {
            size_t dashPos = seats.find('-');
            if (dashPos == string::npos || dashPos == 0 || dashPos == seats.size()-1) {
                cout << "Invalid seat range format in line: " << line << endl;
                exit(0);
            }
            if (price.back()!= '$') {
                cout << "Invalid price format in line: " << line << endl;
                exit(0);
            }
            endSeat = seats.substr(dashPos + 1);
            int endSeatNumber = std::stoi(endSeat);
            pricing[endSeatNumber] = price;
            seats = "";
            price = "";
        }
        if (!price.empty() || !seats.empty()) {
            cout << "Invalid format in line: " << line << endl;
            exit(0);
        }

        if (!pricing.empty()) {
            auto plane = std::make_shared<Airplane>(numSeat, std::stoi(endSeat), std::make_shared<std::map<int, std::string>>(pricing));
            airport.addPlane(plane, date, flightNo);
        }
    }

}
