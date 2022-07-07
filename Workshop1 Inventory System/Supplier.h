#include <string>
#include <list>
#include <iterator>
using namespace std;

#include "Database.h"

#ifndef SUPPLIER_H
#define SUPPLIER_H

class Supplier
{
private:
	int id;
	string compName, contactName, contactTitle, compEmail, compAddress, contactNo1, contactNo2, otherDetails, status, createdAt, updatedAt;

public:
	static string TABLE_NAME;

	Supplier();
	~Supplier();

	static int count();
	static Supplier* get(int id);
	static list<Supplier*> parseList(ResultSet* result);

	void parseItem(ResultSet* result, int row);
	void save();
	void resetAutoIncrement();
	void insert();
	void update();
	void erase();
	string displayDetails();
	int display(int b);


	void setId(int n);
	void setCompName(string n);
	void setContactName(string n);
	void setContactTitle(string n);
	void setCompEmail(string n);
	void setCompAddress(string n);
	void setContactNo1(string n);
	void setContactNo2(string n);
	void setOtherDetails(string n);
	void setStatus(string n);
	void setCreatedAt(string n);
	void setUpdatedAt(string n);

	int getId();
	string getCompName();
	string getContactName();
	string getContactTitle();
	string getCompEmail();
	string getCompAddress();
	string getContactNo1();
	string getContactNo2();
	string getOtherDetails();
	string getStatus();
	string getCreatedAt();
	string getUpdatedAt();
};

#endif // !SUPPLIER_H


