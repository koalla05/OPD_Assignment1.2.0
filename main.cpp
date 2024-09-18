#include <iostream>
#include <sstream>
#include <string>
#include "Airport.h"
#include "FileReader.h"
#include "Helper.h"


void proccessCommand(const string& input, Helper& helper) {
    std::istringstream iss(input);
    std::string command;
    iss >> command;

    if (command == "check") {
        std::string date, flight;
        iss >> date >> flight;
        if (!date.empty() && !flight.empty()) {
            helper.check(date, flight);
        }
        else {
            cout << "Something wrong with input" << endl;
        }
    }
    else if (command == "book"){
        std::string date, flight, place, userName;
        iss >> date >> flight >> place >> userName;
        if (!date.empty() && !flight.empty() && !place.empty() && !userName.empty()) {
            helper.book(date, flight, place, userName);
        }
        else {
            std::cout << "Something wrong with input" << endl;
        }
    }
    else if (command == "return") {
        int id;
        iss >> id;
        if (id != 0) {
            helper.refund(id);
        }
        else {
            std::cout << "Something wrong with input" << endl;
        }
    }
    else if (command == "view") {
        std::string arg1, arg2;
        iss >> arg1 >> arg2;

        if (!arg1.empty() && std::isdigit(arg1[0]) && arg2.empty()) {
            int id = std::stoi(arg1);
            helper.view(id);
        } else {
            std::string date = arg1;
            std::string flight = arg2;
            iss >> flight;

            if (!date.empty() && !flight.empty()) {
                helper.view(date, flight);
            } else if (!date.empty()) {
                helper.view(date);
            }else {
                std::cout << "Something wrong with input" << endl;
            }
        }
    }
}

int main()
{
    Airport myAirport;
    std::wstring filePath = L"C:/Users/kovko/CLionProjects/OPD_Assignment1.2.0/config.txt";
    FileReader fileReader(myAirport, filePath);
    fileReader.read();
    Helper helper(myAirport);
    string input;

    // helper.check("01.03.2023", "TI678");
    // helper.book("01.03.2023", "TI678", "1A", "Alla");
    // helper.book("01.03.2023", "TI678", "2A", "Alla");

    // helper.book("01.03.2023", "TI678", "1A", "Alla");
    // helper.check("01.03.2023", "TI678");
    // helper.check("07.03.2023", "PA345");
    // helper.book("07.03.2023", "PA345", "1A", "JK");
    // helper.book("01.03.2023", "TI678", "2B", "Oliver");
    // helper.refund(1);
    // helper.refund(4);
    // helper.book("01.03.2023", "TI678", "1A", "Alla");
    //
    // helper.view(2);
    // helper.view(1);
    // helper.refund(1);
    // helper.view(1);
    //
    // helper.view("All");
    // helper.view("Alla");
    // helper.view("Oliver");
    // helper.view("01.03.2023", "TI678");
    // helper.view("07.03.2023", "PA345");
    // helper.view("01.03.2023", "TI678");
    while (true) {
        std::cout << "Please enter what you want to do: " << endl;
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        proccessCommand(input, helper);
    }
    return 0;
}
