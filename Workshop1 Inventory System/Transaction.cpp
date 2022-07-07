#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

#include "Transaction.h"
#include "Database.h"

string Transaction::TABLE_NAME = "transactions";

Transaction::Transaction()
{
	//Default Values
	id = 0;
	employeeId = 0;
	createdAt = "";
	updatedAt = "";
}

Transaction::~Transaction() {}

//Static Functions
int Transaction::count()
{
	ResultSet* result = Database::query("SELECT COUNT(id) AS count FROM " + TABLE_NAME);
	result->next();
	return (int)result->getInt64("count");
}

Transaction* Transaction::get(int id) //READ (SELECT * FROM tablename WHERE id = ?)
{
	Transaction* item = nullptr;
	string str = "SELECT * FROM " + TABLE_NAME + " WHERE id = ?";
	PreparedStatement* statement = Database::prepareStatement(str);
	statement->setInt(1, id);
	ResultSet* result = statement->executeQuery();
	if (result->rowsCount() >= 0)
	{
		item = new Transaction();
		item->parseItem(result, 1);
	}
	return item;
}

list<Transaction*> Transaction::parseList(ResultSet* result)
{
	list<Transaction*> list;
	int count = result->rowsCount();
	for (int i = 0; i < count; i++)
	{
		Transaction* item = new Transaction();
		item->parseItem(result, i + 1);
		list.push_back(item);
	}
	return list;
}

//Functions
void Transaction::parseItem(ResultSet* result, int row)
{
	result->absolute(row);
	setId(result->getInt("id"));
	setEmployeeId(result->getInt("employee_id"));
	setCreatedAt(result->getString("created_at"));
	setUpdatedAt(result->getString("updated_at"));
}

void Transaction::save()
{
	if (id == 0)
		insert();
	else
		update();
}

void Transaction::insert()
{
	setCreatedAt(Database::getDateTimeNow());
	setUpdatedAt(getCreatedAt());

	string str = "INSERT INTO " + TABLE_NAME
		+ "(employee_id, created_at, updated_at) "
		+ "VALUES (?, ?, ?)";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, getEmployeeId());
	ps->setString(2, getCreatedAt());
	ps->setString(3, getUpdatedAt());
	ps->execute();
	setId(Database::getLastInsertId());

}

void Transaction::resetAutoIncrement()
{
	string str = "TRUNCATE TABLE " + TABLE_NAME;

	PreparedStatement* ps = Database::prepareStatement(str);

	ps->execute();
}

void Transaction::update()
{
	setUpdatedAt(Database::getDateTimeNow());
	string str = "UPDATE " + TABLE_NAME + " SET " +
		" employee_id = ?, created_at = ?" +
		", updated_at = ? WHERE ID = ?";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, getEmployeeId());
	ps->setString(2, getCreatedAt());
	ps->setString(3, getUpdatedAt());
	ps->setInt(4, getId());
	ps->execute();
}

void Transaction::erase()
{
	if (getId() == 0) return;
	string str = "DELETE FROM " + TABLE_NAME + " WHERE ID = ?";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, getId());
	ps->execute();
	setId(0);
}

string Transaction::displayDetails()
{
	stringstream str;

	str << "\nID\t\t: " << getId();
	str << "\nDate\t\t: " << getCreatedAt();
	str << "\nEmployee ID\t\t: " << getEmployeeId();

	return str.str();
}


//Setters
void Transaction::setId(int n) {
	id = n;
}

void Transaction::setEmployeeId(int n) {
	employeeId = n;
}

void Transaction::setCreatedAt(string n) {
	createdAt = n;
}

void Transaction::setUpdatedAt(string n) {
	updatedAt = n;
}

//Getters
int Transaction::getId() {
	return id;
}

int Transaction::getEmployeeId() {
	return employeeId;
}

string Transaction::getCreatedAt() {
	return createdAt;
}

string Transaction::getUpdatedAt() {
	return updatedAt;
}