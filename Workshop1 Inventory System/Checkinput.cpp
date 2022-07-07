#include <string>
#include <string.h>

#include "CheckInput.h"

bool CheckInput::isNullOrWhiteSpace(string input)
{
	if (input == "")
		return false;
	else
		return true;
}

bool CheckInput::isValidContactNo(string input)
{
	string::size_type found = input.find_first_not_of("0123456789-+");
	if (found != input.npos) //if contains other than ()
		return false;
	else
	{
		size_t found1 = input.find("-");
		if (found1 != input.npos)
			return true;
		else
			return false;
	}
}

bool CheckInput::isValidIc(string input)
{
	 string::size_type found = input.find_first_not_of("0123456789");
	if (found != input.npos) 
		return false;
	else
	{
		if (input.length() == 12)
			return true;
		else
			return false;
	}
}

bool CheckInput::isValidGender(string input)
{
	if (input == "Male" || input == "Female")
		return true;
	else
		return false;
}

bool CheckInput::isValidPermission(string input)
{
	if (input == "admin" || input == "normaluser")
		return true;
	else
		return false;
}

bool CheckInput::isValidDate(string input)
{
	string::size_type found = input.find_first_not_of("0123456789-"); 
	if (found != input.npos)
		return false;	
	else
	{
		size_t found1 = input.find("-", 4, 1);
		if (found1 != input.npos)
		{
			if (input.length() == 10)
				return true;
			else
				return false;
		}
		else
			return false;
	}		
}

bool CheckInput::isValidEmail(string input)
{
	string::size_type found = input.find_first_of("@");
	if (found != input.npos) //if found @
		return true;
	else
		return false;
}

bool CheckInput::isDigit(string input)
{
	string::size_type found = input.find_first_not_of("0123456789.");
	if (found == input.npos)//if only contains digit
		return true;
	else
		return false;
}