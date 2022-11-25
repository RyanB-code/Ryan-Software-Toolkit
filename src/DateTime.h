#ifndef DATE_TIME_H
#define DATE_TIME_H

#include "RST.h"

#include <string>
#include <iomanip>
#include <sstream>

#pragma warning(disable : 4996) // Disable warning for using time_t

struct Date {
	Date();

	int second, minute, hour;
	int day, month, year;

	// Will return in format [DATE]
	// \param True - Formatted as HH:MM:SS DD-MMM-YYYY \param False - Only HH:MM:SS
	std::string printDate(bool full);
};

#endif