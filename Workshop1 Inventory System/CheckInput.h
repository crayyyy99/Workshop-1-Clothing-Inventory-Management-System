#include <string>
#include <cstring>
using namespace std;

#ifndef CHECKINPUT_H
#define CHECKINPUT_H

class CheckInput
{
public:

	bool isNullOrWhiteSpace(string input);
	bool isValidContactNo(string input);
	bool isValidIc(string input);
	bool isValidGender(string input);
	bool isValidPermission(string input);
	bool isValidDate(string input);
	bool isValidEmail(string input);
	bool isDigit(string input);

	
};
#endif // !CHECKINPUT_H



