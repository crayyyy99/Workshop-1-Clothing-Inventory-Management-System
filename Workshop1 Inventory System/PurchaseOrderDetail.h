#include <string>
#include <list>
#include <iterator>
using namespace std;

#include "Database.h"

#ifndef PURCHASEORDERDETAIL_H
#define PURCHASEORDERDETAIL_H

class PurchaseOrderDetail
{
private:
	int id, itemNumber, purOrderId, code, quantity;
	double purPrice;
	string createdAt, updatedAt;

public:
	static string TABLE_NAME;

	PurchaseOrderDetail();
	~PurchaseOrderDetail();

	static int count();
	static PurchaseOrderDetail* get(int id);
	static PurchaseOrderDetail* gett(int purOrderid, int itemNo);
	static list<PurchaseOrderDetail*> parseList(ResultSet* result);

	void parseItem(ResultSet* result, int row);
	void save();
	void resetAutoIncrement();
	void insert();
	void update();
	void erase();
	int displayDetails();


	void setId(int n);
	void setItemNumber(int n);
	void setPurOrderId(int n);
	void setCode(int n);
	void setPurPrice(double n);
	void setQuantity(int n);
	void setCreatedAt(string n);
	void setUpdatedAt(string n);

	int getId();
	int getItemNumber();
	int getPurOrderId();
	int getCode();
	double getPurPrice();
	int getQuantity();
	string getCreatedAt();
	string getUpdatedAt();
};

#endif // !PRODUCT_H



