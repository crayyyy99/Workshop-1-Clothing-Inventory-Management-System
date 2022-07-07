#include <string>
#include <list>
#include <iterator>
using namespace std;

#include "Database.h"

#ifndef TRANSACTIONDETAIL_H
#define TRANSACTIONDETAIL_H

class TransactionDetail
{
private:
	int id, itemNumber, transactionId, code, quantity, discount;
	string createdAt, updatedAt;

public:
	static string TABLE_NAME;

	TransactionDetail();
	~TransactionDetail();

	static int count();
	static TransactionDetail* get(int id);
	static TransactionDetail* gett(int transId, int itemNo);
	static list<TransactionDetail*> parseList(ResultSet* result);

	void parseItem(ResultSet* result, int row);
	void save();
	void resetAutoIncrement();
	void insert();
	void update();
	void erase();
	int displayDetails();


	void setId(int n);
	void setTransactionId(int n);
	void setItemNumber(int n);
	void setCode(int n);
	void setQuantity(int n);
	void setDiscount(int n);
	void setCreatedAt(string n);	
	void setUpdatedAt(string n);

	int getId();
	int getTransId();
	int getItemNumber();
	int getCode();	
	int getQuantity();
	int getDiscount();
	string getCreatedAt();
	string getUpdatedAt();
};

#endif // !PRODUCT_H



