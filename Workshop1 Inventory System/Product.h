#include <string>
#include <list>
#include <iterator>
using namespace std;

#include "Database.h"

#ifndef PRODUCT_H
#define PRODUCT_H

class Product
{
private:
	int id;
	double unitPrice;
	string name, colour, gender, type, description, status, createdAt, updatedAt;

public:
	static string TABLE_NAME;

	Product();
	~Product();

	static int count();
	static Product* get(int id);
	static list<Product*> parseList(ResultSet* result);

	void parseItem(ResultSet* result, int row);
	void save();
	void resetAutoIncrement();
	void insert();
	void update();
	void erase();
	int displayDetails();
	int display();


	void setId(int n);
	void setName(string n);
	void setColour(string n);
	void setGender(string n);
	void setType(string n);
	void setDescription(string n);
	void setUnitPrice(double n);
	void setStatus(string n);
	void setCreatedAt(string n);
	void setUpdatedAt(string n);

	int getId();
	string getName();
	string getColour();
	string getGender();
	string getType();
	string getDescription();
	double getUnitPrice();
	string getStatus();
	string getCreatedAt();
	string getUpdatedAt();
};

#endif // !PRODUCT_H



