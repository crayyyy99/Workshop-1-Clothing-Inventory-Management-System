#include <string>
#include <list>
#include <iterator>
using namespace std;

#include "Database.h"

#ifndef EMPLOYEE_H
#define EMPLOYEE_H

class Employee
{
private:
	int id;
	string name, ic, gender, dob, position, contactNo, email, homeAddress, username, password, permission, status, createdAt, updatedAt;

public:
	static string TABLE_NAME;

	Employee();
	~Employee();

	static int count();
	static Employee* get(int id);
	static Employee* get(string username);
	static list<Employee*> parseList(ResultSet* result);

	void parseItem(ResultSet* result, int row);
	void save();
	void resetAutoIncrement();
	void insert();
	void update();
	void erase();
	string displayAccDetails();
	string displayDetails();
	int display(int b);


	void setId(int n);
	void setName(string n);
	void setIc(string n);
	void setGender(string n);
	void setDob(string n);
	void setPosition(string n);
	void setContactNo(string n);
	void setEmail(string n);
	void setHomeAddress(string n);
	void setUsername(string n);
	void setPassword(string n);
	void setPermission(string n);
	void setStatus(string n);
	void setCreatedAt(string n);
	void setUpdatedAt(string n);

	int getId();
	string getName();
	string getIc();
	string getGender();
	string getDob();
	string getPosition();
	string getContactNo();
	string getEmail();
	string getHomeAddress();
	string getUsername();
	string getPassword();
	string getPermission();
	string getStatus();
	string getCreatedAt();
	string getUpdatedAt();
};

#endif // !EMPLOYEE_H

