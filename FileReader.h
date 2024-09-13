#pragma once
#include "Airport.h"
#include "Airplane.h"

class FileReader {
    string path = "C:/Users/kovko/CLionProjects/OPD_Assignment1.2.0/config.txt";
    Airport& airport;

public:
    FileReader(Airport& inAirport);

    void read();
};