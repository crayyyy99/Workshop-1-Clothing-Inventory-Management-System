#include <string>
#include <list>
#include <iterator>
using namespace std;

#include "Database.h"

#ifndef PRODUCTSTOCK_H
#define PRODUCTSTOCK_H

class ProductStock
{
private:
	int id, productId, quantity, code;
	string size, createdAt, updatedAt;

public:
	static string TABLE_NAME;

	ProductStock();
	~ProductStock();

	static int count();
	static ProductStock* get(int id);
	static ProductStock* gett(int code);
	static list<ProductStock*> parseList(ResultSet* result);

	void parseItem(ResultSet* result, int row);
	void save();
	void resetAutoIncrement();
	void insert();
	void update();
	void erase();
	int displayDetails();
	int display(int b);


	void setId(int n);
	void setCode(int n);
	void setSize(string n);
	void setQuantity(int n);
	void setProductId(int n);
	void setCreatedAt(string n);
	void setUpdatedAt(string n);

	int getId();
	int getCode();
	string getSize();
	int getQuantity();
	int getProductId();
	string getCreatedAt();
	string getUpdatedAt();
};

#endif // !PRODUCTSTOCK_H



