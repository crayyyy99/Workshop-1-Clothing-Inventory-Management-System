#include <string>
#include <list>
#include <iterator>
using namespace std;

#include "Database.h"

#ifndef TRANSACTION_H
#define TRANSACTION_H

class Transaction
{
private:
	int id, employeeId;
	string createdAt, updatedAt;

public:
	static string TABLE_NAME;

	Transaction();
	~Transaction();

	static int count();
	static Transaction* get(int id);
	static list<Transaction*> parseList(ResultSet* result);

	void parseItem(ResultSet* result, int row);
	void save();
	void resetAutoIncrement();
	void insert();
	void update();
	void erase();
	string displayDetails();


	void setId(int n);
	void setEmployeeId(int n);
	void setCreatedAt(string n);
	void setUpdatedAt(string n);

	int getId();
	int getEmployeeId();
	string getCreatedAt();
	string getUpdatedAt();
};

#endif // !PRODUCT_H



