#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

#include "TransactionDetail.h"
#include "Database.h"

string TransactionDetail::TABLE_NAME = "transaction_details";

TransactionDetail::TransactionDetail()
{
	//Default Values
	id = 0;	
	transactionId = 0;
	itemNumber = 0;
	code = 0;
	quantity = 0;
	discount = 0;
	createdAt = "";
	updatedAt = "";
}

TransactionDetail::~TransactionDetail() {}

//Static Functions
int TransactionDetail::count()
{
	ResultSet* result = Database::query("SELECT COUNT(id) AS count FROM " + TABLE_NAME);
	result->next();
	return (int)result->getInt64("count");
}

TransactionDetail* TransactionDetail::get(int id) //READ (SELECT * FROM tablename WHERE code = ?)
{
	TransactionDetail* item = nullptr;
	string str = "SELECT * FROM " + TABLE_NAME + " WHERE id = ?";
	PreparedStatement* statement = Database::prepareStatement(str);
	statement->setInt(1, id);
	ResultSet* result = statement->executeQuery();
	if (result->rowsCount() >= 0)
	{
		item = new TransactionDetail();
		item->parseItem(result, 1);
	}
	return item;
}

TransactionDetail* TransactionDetail::gett(int transId, int itemNo) //READ (SELECT * FROM tablename WHERE code = ?)
{
	TransactionDetail* item = nullptr;
	string str = "SELECT * FROM " + TABLE_NAME + " WHERE transaction_id = ? and item_number = ?";
	PreparedStatement* statement = Database::prepareStatement(str);
	statement->setInt(1, transId);
	statement->setInt(2, itemNo);
	ResultSet* result = statement->executeQuery();
	if (result->rowsCount() >= 0)
	{
		item = new TransactionDetail();
		item->parseItem(result, 1);
	}
	return item;
}

list<TransactionDetail*> TransactionDetail::parseList(ResultSet* result)
{
	list<TransactionDetail*> list;
	int count = result->rowsCount();
	for (int i = 0; i < count; i++)
	{
		TransactionDetail* item = new TransactionDetail();
		item->parseItem(result, i + 1);
		list.push_back(item);
	}
	return list;
}

//Functions
void TransactionDetail::parseItem(ResultSet* result, int row)
{
	result->absolute(row);
	setId(result->getInt("id"));
	setTransactionId(result->getInt("transaction_id"));
	setItemNumber(result->getInt("item_number"));
	setCode(result->getInt("code"));
	setQuantity(result->getInt("quantity"));
	setDiscount(result->getDouble("discount"));
	setCreatedAt(result->getString("created_at"));
	setUpdatedAt(result->getString("updated_at"));
}

void TransactionDetail::save()
{
	if (id == 0)
		insert();
	else
		update();
}

void TransactionDetail::insert()
{
	setCreatedAt(Database::getDateTimeNow());
	setUpdatedAt(getCreatedAt());

	string str = "INSERT INTO " + TABLE_NAME
		+ "( transaction_id, item_number, code, quantity, discount, created_at, updated_at) "
		+ "VALUES (?, ?, ?, ?, ?, ?, ?)";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, getTransId());
	ps->setInt(2, getItemNumber());
	ps->setInt(3, getCode());
	ps->setInt(4, getQuantity());
	ps->setInt(5, getDiscount());
	ps->setString(6, getCreatedAt());
	ps->setString(7, getUpdatedAt());
	ps->execute();
	setId(Database::getLastInsertId());

}

void TransactionDetail::resetAutoIncrement()
{
	string str = "TRUNCATE TABLE " + TABLE_NAME;
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->execute();
}

void TransactionDetail::update()
{
	setUpdatedAt(Database::getDateTimeNow());
	string str = "UPDATE " + TABLE_NAME + " SET " +
		"transaction_id = ?, item_number = ? , code = ?, quantity = ?, discount = ?, created_at = ?" +
		", updated_at = ? WHERE ID = ?";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, getTransId());
	ps->setInt(2, getItemNumber());
	ps->setInt(3, getCode());
	ps->setInt(4, getQuantity());
	ps->setInt(5, getDiscount());
	ps->setString(6, getCreatedAt());
	ps->setString(7, getUpdatedAt());
	ps->setInt(8, getId());
	ps->execute();
}

void TransactionDetail::erase()
{
	if (getId() == 0) return;
	string str = "DELETE FROM " + TABLE_NAME + " WHERE ID = ?";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, getId());
	ps->execute();
	setId(0);
}

int TransactionDetail::displayDetails()
{
	cout << left << setw(23) << "\nItem No." << ": " << getItemNumber();
	cout << left << setw(23) << "\nProduct Code" << ": " << getCode();
	cout << left << setw(23) << "\nDiscount (%)" << ": " << getDiscount();
	cout << left << setw(23) << "\nQuantity" << ": " << getQuantity();
	cout << endl;
	return 0;
}


//Setters
void TransactionDetail::setId(int n) {
	id = n;
}

void TransactionDetail::setTransactionId(int n) {
	transactionId = n;
}

void TransactionDetail::setItemNumber(int n) {
	itemNumber = n;
}

void TransactionDetail::setCode(int n) {
	code = n;
}

void TransactionDetail::setQuantity(int n) {
	quantity = n;
}

void TransactionDetail::setDiscount(int n) {
	discount = n;
}

void TransactionDetail::setCreatedAt(string n) {
	createdAt = n;
}

void TransactionDetail::setUpdatedAt(string n) {
	updatedAt = n;
}

//Getters
int TransactionDetail::getId() {
	return id;
}

int TransactionDetail::getTransId() {
	return transactionId;
}

int TransactionDetail::getItemNumber() {
	return itemNumber;
}

int TransactionDetail::getCode() {
	return code;
}

int TransactionDetail::getQuantity() {
	return quantity;
}

int TransactionDetail::getDiscount() {
	return discount;
}



string TransactionDetail::getCreatedAt() {
	return createdAt;
}

string TransactionDetail::getUpdatedAt() {
	return updatedAt;
}