#include <string>
#include <list>
#include <iterator>
using namespace std;

#include "Database.h"

#ifndef PURCHASEORDER_H
#define PURCHASEORDER_H

class PurchaseOrder
{
private:
	int id, supplierId, employeeId;
	string createdAt, updatedAt;

public:
	static string TABLE_NAME;

	PurchaseOrder();
	~PurchaseOrder();

	static int count();
	static PurchaseOrder* get(int id);
	static list<PurchaseOrder*> parseList(ResultSet* result);

	void parseItem(ResultSet* result, int row);
	void save();
	void resetAutoIncrement();
	void insert();
	void update();
	void erase();
	string displayDetails();


	void setId(int n);
	void setSupplierId(int n);
	void setEmployeeId(int n);
	void setCreatedAt(string n);
	void setUpdatedAt(string n);

	int getId();
	int getSupplierId();
	int getEmployeeId();
	string getCreatedAt();
	string getUpdatedAt();
};

#endif // !PRODUCT_H



