#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

#include "PurchaseOrderDetail.h"
#include "Database.h"

string PurchaseOrderDetail::TABLE_NAME = "purchase_order_details";

PurchaseOrderDetail::PurchaseOrderDetail()
{
	//Default Values
	id = 0;
	itemNumber = 0;
	purOrderId = 0;
	code = 0;
	purPrice = 0.0;
	quantity = 0;
	createdAt = "";
	updatedAt = "";
}

PurchaseOrderDetail::~PurchaseOrderDetail() {}

//Static Functions
int PurchaseOrderDetail::count()
{
	ResultSet* result = Database::query("SELECT COUNT(id) AS count FROM " + TABLE_NAME);
	result->next();
	return (int)result->getInt64("count");
}

PurchaseOrderDetail* PurchaseOrderDetail::get(int id) 
{
	PurchaseOrderDetail* item = nullptr;
	string str = "SELECT * FROM " + TABLE_NAME + " WHERE id = ?";
	PreparedStatement* statement = Database::prepareStatement(str);
	statement->setInt(1, id);
	ResultSet* result = statement->executeQuery();
	if (result->rowsCount() >= 0)
	{
		item = new PurchaseOrderDetail();
		item->parseItem(result, 1);
	}
	return item;
}

PurchaseOrderDetail* PurchaseOrderDetail::gett(int purOrderid, int itemNo) 
{
	PurchaseOrderDetail* item = nullptr;
	string str = "SELECT * FROM " + TABLE_NAME + " WHERE purchase_order_id = ? and item_number = ?";
	PreparedStatement* statement = Database::prepareStatement(str);
	statement->setInt(1, purOrderid);
	statement->setInt(2, itemNo);
	ResultSet* result = statement->executeQuery();
	if (result->rowsCount() >= 0)
	{
		item = new PurchaseOrderDetail();
		item->parseItem(result, 1);
	}
	return item;
}

list<PurchaseOrderDetail*> PurchaseOrderDetail::parseList(ResultSet* result)
{
	list<PurchaseOrderDetail*> list;
	int count = result->rowsCount();
	for (int i = 0; i < count; i++)
	{
		PurchaseOrderDetail* item = new PurchaseOrderDetail();
		item->parseItem(result, i + 1);
		list.push_back(item);
	}
	return list;
}

//Functions
void PurchaseOrderDetail::parseItem(ResultSet* result, int row)
{
	result->absolute(row);
	setId(result->getInt("id"));
	setItemNumber(result->getInt("item_number"));
	setPurOrderId(result->getInt("purchase_order_id"));
	setCode(result->getInt("code"));
	setPurPrice(result->getDouble("purchasing_price"));
	setQuantity(result->getInt("quantity"));
	setCreatedAt(result->getString("created_at"));
	setUpdatedAt(result->getString("updated_at"));
}

void PurchaseOrderDetail::save()
{
	if (id == 0)
		insert();
	else
		update();
}

void PurchaseOrderDetail::insert()
{
	setCreatedAt(Database::getDateTimeNow());
	setUpdatedAt(getCreatedAt());

	string str = "INSERT INTO " + TABLE_NAME
		+ "(purchase_order_id, item_number, code, purchasing_price, quantity, created_at, updated_at) "
		+ "VALUES (?, ?, ?, ?, ?, ?, ?)";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, getPurOrderId());
	ps->setInt(2, getItemNumber());
	ps->setInt(3, getCode());
	ps->setDouble(4, getPurPrice());
	ps->setInt(5, getQuantity());
	ps->setString(6, getCreatedAt());
	ps->setString(7, getUpdatedAt());
	ps->execute();
	setId(Database::getLastInsertId());

}

void PurchaseOrderDetail::resetAutoIncrement()
{
	string str = "TRUNCATE TABLE " + TABLE_NAME;
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->execute();
}

void PurchaseOrderDetail::update()
{
	setUpdatedAt(Database::getDateTimeNow());
	string str = "UPDATE " + TABLE_NAME + " SET " +
		" purchase_order_id = ?, item_number = ?, code = ?, purchasing_price = ?, quantity = ?, created_at = ?" +
		", updated_at = ? WHERE ID = ?";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, getPurOrderId());
	ps->setInt(2, getItemNumber());
	ps->setInt(3, getCode());
	ps->setDouble(4, getPurPrice());
	ps->setInt(5, getQuantity());
	ps->setString(6, getCreatedAt());
	ps->setString(7, getUpdatedAt());
	ps->setInt(8, getId());
	ps->execute();
}

void PurchaseOrderDetail::erase()
{
	if (getId() == 0) return;
	string str = "DELETE FROM " + TABLE_NAME + " WHERE ID = ?";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, getId());
	ps->execute();
	setId(0);
}

int PurchaseOrderDetail::displayDetails()
{
	cout << left << setw(23) << "\nItem No." << ": " << getItemNumber();
	cout << left << setw(23) << "\nProduct Code" << ": " << getCode();
	cout << left << setw(23) << "\nPurchasing Price (RM)" << ": " << fixed << showpoint << setprecision(2) << getPurPrice();
	cout << left << setw(23) << "\nQuantity" << ": " << getQuantity();
	cout << endl;
	return 0;
}


//Setters
void PurchaseOrderDetail::setId(int n) {
	id = n;
}

void PurchaseOrderDetail::setItemNumber(int n) {
	itemNumber = n;
}

void PurchaseOrderDetail::setPurOrderId(int n) {
	purOrderId = n;
}

void PurchaseOrderDetail::setCode(int n) {
	code = n;
}

void PurchaseOrderDetail::setPurPrice(double n) {
	purPrice = n;
}

void PurchaseOrderDetail::setQuantity(int n) {
	quantity = n;
}

void PurchaseOrderDetail::setCreatedAt(string n) {
	createdAt = n;
}

void PurchaseOrderDetail::setUpdatedAt(string n) {
	updatedAt = n;
}

//Getters
int PurchaseOrderDetail::getId() {
	return id;
}

int PurchaseOrderDetail::getItemNumber() {
	return itemNumber;
}

int PurchaseOrderDetail::getPurOrderId() {
	return purOrderId;
}

int PurchaseOrderDetail::getCode() {
	return code;
}

double PurchaseOrderDetail::getPurPrice() {
	return purPrice;
}

int PurchaseOrderDetail::getQuantity() {
	return quantity;
}

string PurchaseOrderDetail::getCreatedAt() {
	return createdAt;
}

string PurchaseOrderDetail::getUpdatedAt() {
	return updatedAt;
}