#include <iostream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>
using namespace std;

string getCurrentDate()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char dateStr[11];
    sprintf(dateStr, "%02d-%02d-%04d", ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
    return std::string(dateStr);
}

int inBetweenDays(const std::string &date1, const std::string &date2)
{
    // Parse day, month, and year manually
    int day1, month1, year1, day2, month2, year2;
    istringstream ss1(date1), ss2(date2);

    if (!(ss1 >> day1) || ss1.get() != '-' || !(ss1 >> month1) || ss1.get() != '-' || !(ss1 >> year1) ||
        !(ss2 >> day2) || ss2.get() != '-' || !(ss2 >> month2) || ss2.get() != '-' || !(ss2 >> year2))
    {
        return -1; // Error
    }

    // Convert to tm structure
    tm tm1 = {}, tm2 = {};
    tm1.tm_mday = day1;
    tm1.tm_mon = month1 - 1; // Months are 0-based
    tm1.tm_year = year1 - 1900;

    tm2.tm_mday = day2;
    tm2.tm_mon = month2 - 1;
    tm2.tm_year = year2 - 1900;

    // Convert to time_t (Local Time)
    time_t time1 = mktime(&tm1);
    time_t time2 = mktime(&tm2);

    if (time1 == -1 || time2 == -1)
    {
        cerr << "Error converting dates to time.\n";
        return -1;
    }

    // Return absolute difference in days
    return (difftime(time2, time1) / (60 * 60 * 24));
}