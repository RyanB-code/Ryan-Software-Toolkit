#include "DateTime.h"

Date::Date() {
	time_t now{ time(0) };
	tm* ltm = localtime(&now);

	hour = ltm->tm_hour;
	minute = ltm->tm_min;
	second = ltm->tm_sec;

	day = ltm->tm_mday;
	month = ltm->tm_mon;
	year = ltm->tm_year + 1900;

	return;
}

std::string Date::printDate(bool full) {
	std::ostringstream os;
	os << std::setw(2) << std::setfill('0') << hour << ':'
		<< std::setw(2) << std::setfill('0') << minute << ':'
		<< std::setw(2) << std::setfill('0') << second;
	if (!full) {
		os << ']';
		return os.str();
	}
	else {
		os << ' ';
		os << std::setw(2) << std::setfill('0') << day << '-';
		switch (month) {
		case 0:
			os << "JAN";
			break;
		case 1:
			os << "FEB";
			break;
		case 2:
			os << "MAR";
			break;
		case 3:
			os << "APR";
			break;
		case 4:
			os << "MAY";
			break;
		case 5:
			os << "JUN";
			break;
		case 6:
			os << "JUL";
			break;
		case 7:
			os << "AUG";
			break;
		case 8:
			os << "SEP";
			break;
		case 9:
			os << "OCT";
			break;
		case 10:
			os << "NOV";
			break;
		case 11:
			os << "DEC";
			break;
		default:
			os << "MMM";
		}

		os << '-' << year << ']';

		return os.str();
	}

}