#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

#include "PurchaseOrder.h"
#include "Database.h"

string PurchaseOrder::TABLE_NAME = "purchase_orders";

PurchaseOrder::PurchaseOrder()
{
	//Default Values
	id = 0;
	employeeId = 0;
	supplierId = 0;
	createdAt = "";
	updatedAt = "";
}

PurchaseOrder::~PurchaseOrder() {}

//Static Functions
int PurchaseOrder::count()
{
	ResultSet* result = Database::query("SELECT COUNT(id) AS count FROM " + TABLE_NAME);
	result->next();
	return (int)result->getInt64("count");
}

PurchaseOrder* PurchaseOrder::get(int id) //READ (SELECT * FROM tablename WHERE id = ?)
{
	PurchaseOrder* item = nullptr;
	string str = "SELECT * FROM " + TABLE_NAME + " WHERE id = ?";
	PreparedStatement* statement = Database::prepareStatement(str);
	statement->setInt(1, id);
	ResultSet* result = statement->executeQuery();
	if (result->rowsCount() >= 0)
	{
		item = new PurchaseOrder();
		item->parseItem(result, 1);
	}
	return item;
}

list<PurchaseOrder*> PurchaseOrder::parseList(ResultSet* result)
{
	list<PurchaseOrder*> list;
	int count = result->rowsCount();
	for (int i = 0; i < count; i++)
	{
		PurchaseOrder* item = new PurchaseOrder();
		item->parseItem(result, i + 1);
		list.push_back(item);
	}
	return list;
}

//Functions
void PurchaseOrder::parseItem(ResultSet* result, int row)
{
	result->absolute(row);
	setId(result->getInt("id"));
	setSupplierId(result->getInt("supplier_id"));
	setEmployeeId(result->getInt("employee_id"));
	setCreatedAt(result->getString("created_at"));
	setUpdatedAt(result->getString("updated_at"));
}

void PurchaseOrder::save()
{
	if (id == 0)
		insert();
	else
		update();
}

void PurchaseOrder::insert()
{
	setCreatedAt(Database::getDateTimeNow());
	setUpdatedAt(getCreatedAt());

	string str = "INSERT INTO " + TABLE_NAME
		+ "(supplier_id, employee_id, created_at, updated_at) "
		+ "VALUES (?, ?, ?, ?)";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, getSupplierId());
	ps->setInt(2, getEmployeeId());
	ps->setString(3, getCreatedAt());
	ps->setString(4, getUpdatedAt());
	ps->execute();
	setId(Database::getLastInsertId());

}

void PurchaseOrder::resetAutoIncrement()
{
	string str = "TRUNCATE TABLE " + TABLE_NAME;

	PreparedStatement* ps = Database::prepareStatement(str);

	ps->execute();
}

void PurchaseOrder::update()
{
	setUpdatedAt(Database::getDateTimeNow());
	string str = "UPDATE " + TABLE_NAME + " SET " +
		" supplier_id = ?, employee_id = ?, created_at = ?" +
		", updated_at = ? WHERE ID = ?";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, getSupplierId());
	ps->setInt(2, getEmployeeId());
	ps->setString(3, getCreatedAt());
	ps->setString(4, getUpdatedAt());
	ps->setInt(5, getId());
	ps->execute();
}

void PurchaseOrder::erase()
{
	if (getId() == 0) return;
	string str = "DELETE FROM " + TABLE_NAME + " WHERE ID = ?";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, getId());
	ps->execute();
	setId(0);
}

string PurchaseOrder::displayDetails()
{
	stringstream str;

	str << "\nID\t\t: " << getId();
	str << "\nDate\t\t: " << getCreatedAt();
	str << "\nEmployee ID\t\t: " << getEmployeeId();
	str << "\nSupplier ID\t\t: " << getSupplierId();

	return str.str();
}


//Setters
void PurchaseOrder::setId(int n) {
	id = n;
}

void PurchaseOrder::setSupplierId(int n) {
	supplierId = n;
}

void PurchaseOrder::setEmployeeId(int n) {
	employeeId = n;
}

void PurchaseOrder::setCreatedAt(string n) {
	createdAt = n;
}

void PurchaseOrder::setUpdatedAt(string n) {
	updatedAt = n;
}

//Getters
int PurchaseOrder::getId() {
	return id;
}

int PurchaseOrder::getSupplierId() {
	return supplierId;
}

int PurchaseOrder::getEmployeeId() {
	return employeeId;
}

string PurchaseOrder::getCreatedAt() {
	return createdAt;
}

string PurchaseOrder::getUpdatedAt() {
	return updatedAt;
}