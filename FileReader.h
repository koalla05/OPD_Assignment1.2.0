#pragma once
#include "Airport.h"
#include <string>

class FileReader {
    std::wstring path;
    Airport& airport;

public:
    FileReader(Airport& inAirport, const std::wstring& filePath);

    void read();
};
