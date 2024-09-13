#include <string>
#include "Airport.h"
#include "FileReader.h"
#include "Helper.h"

int main()
{
    Airport myAirport;
    std::wstring filePath = L"C:/Users/kovko/CLionProjects/OPD_Assignment1.2.0/config.txt";
    FileReader fileReader(myAirport, filePath);
    fileReader.read();
    Helper helper(myAirport);

    helper.check("01.03.2023", "TI678");
    helper.book("01.03.2023", "TI678", "1A", "Alla");
    helper.book("01.03.2023", "TI678", "2A", "Alla");

    helper.book("01.03.2023", "TI678", "1A", "Alla");
    helper.check("01.03.2023", "TI678");
    helper.check("07.03.2023", "PA345");
    helper.book("07.03.2023", "PA345", "1A", "JK");
    helper.book("01.03.2023", "TI678", "2B", "Oliver");
    helper.refund(1);
    helper.refund(4);
    helper.book("01.03.2023", "TI678", "1A", "Alla");

    helper.view(2);
    helper.view(1);
    helper.refund(1);
    helper.view(1);

    helper.view("All");
    helper.view("Alla");
    helper.view("Oliver");
    helper.view("01.03.203", "TI678");
    helper.view("07.03.2023", "PA345");
    helper.view("01.03.2023", "TI678");
    return 0;
}
