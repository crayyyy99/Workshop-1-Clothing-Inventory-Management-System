#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

#include "Product.h"
#include "Database.h"

string Product::TABLE_NAME = "products";

Product::Product()
{
	//Default Values
	id = 0;
	name = "";
	colour = "";
	gender = "";
	type = "";
	description = "";
	unitPrice = 0.0;
	status = "newly added";
	createdAt = "";
	updatedAt = "";
}

Product::~Product() {}

//Static Functions
int Product::count()
{
	ResultSet* result = Database::query("SELECT COUNT(id) AS count FROM " + TABLE_NAME);
	result->next();
	return (int)result->getInt64("count");
}

Product* Product::get(int id) //READ (SELECT * FROM tablename WHERE id = ?)
{
	Product* item = nullptr;
	string str = "SELECT * FROM " + TABLE_NAME + " WHERE id = ?";
	PreparedStatement* statement = Database::prepareStatement(str);
	statement->setInt(1, id);
	ResultSet* result = statement->executeQuery();
	if (result->rowsCount() >= 0)
	{
		item = new Product();
		item->parseItem(result, 1);
	}
	return item;
}

list<Product*> Product::parseList(ResultSet* result)
{
	list<Product*> list;
	int count = result->rowsCount();
	for (int i = 0; i < count; i++)
	{
		Product* item = new Product();
		item->parseItem(result, i + 1);
		list.push_back(item);
	}
	return list;
}

//Functions
void Product::parseItem(ResultSet* result, int row)
{
	result->absolute(row);
	setId(result->getInt("id"));
	setName(result->getString("name"));
	setColour(result->getString("colour"));
	setGender(result->getString("gender"));
	setType(result->getString("type"));
	setDescription(result->getString("description"));
	setUnitPrice(result->getDouble("unit_price"));
	setStatus(result->getString("status"));
	setCreatedAt(result->getString("created_at"));
	setUpdatedAt(result->getString("updated_at"));
}

void Product::save()
{
	if (id == 0)
		insert();
	else
		update();
}

void Product::insert()
{
	setCreatedAt(Database::getDateTimeNow());
	setUpdatedAt(getCreatedAt());

	string str = "INSERT INTO " + TABLE_NAME
		+ "(name, colour, gender, type, description, unit_price, status, created_at, updated_at) "
		+ "VALUES (?, ?, ?, ?, ?, ?, ?, ? ,?)";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setString(1, getName());
	ps->setString(2, getColour());
	ps->setString(3, getGender());
	ps->setString(4, getType());
	ps->setString(5, getDescription());
	ps->setDouble(6, getUnitPrice());
	ps->setString(7, getStatus());
	ps->setString(8, getCreatedAt());
	ps->setString(9, getUpdatedAt());
	ps->execute();
	
	setId(Database::getLastInsertId());
}

void Product::resetAutoIncrement()
{
	string str = "TRUNCATE TABLE " + TABLE_NAME;

	PreparedStatement* ps = Database::prepareStatement(str);

	ps->execute();
}

void Product::update()
{
	setUpdatedAt(Database::getDateTimeNow());
	string str = "UPDATE " + TABLE_NAME + " SET " +
		" name = ?, colour = ?, gender = ?, type = ?, description = ?, unit_price = ?, status = ?, created_at = ?" +
		", updated_at = ? WHERE ID = ?";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setString(1, getName());
	ps->setString(2, getColour());
	ps->setString(3, getGender());
	ps->setString(4, getType());
	ps->setString(5, getDescription());
	ps->setDouble(6, getUnitPrice());
	ps->setString(7, getStatus());
	ps->setString(8, getCreatedAt());
	ps->setString(9, getUpdatedAt());
	ps->setInt(10, getId());
	ps->execute();
}

void Product::erase()
{
	if (getId() == 0) return;
	string str = "DELETE FROM " + TABLE_NAME + " WHERE ID = ?";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, getId());
	ps->execute();
	setId(0);
}

int Product::displayDetails()
{
	cout << left << setw(17) << "\nName" << ": " << getName();
	cout << left << setw(17) << "\nColour" << ": " << getColour();
	cout << left << setw(17) << "\nGender" << ": " << getGender();
	cout << left << setw(17) << "\nType" << ": " << getType();
	cout << left << setw(17) << "\nDescription" << ": " << getDescription();
	cout << left << setw(17) << "\nUnit Price (RM)" << ": " << fixed << showpoint << setprecision(2) << getUnitPrice();

	return 0;
}

int Product::display()
{
	cout << left << setw(17) << "\nName" << ": " << getName();
	cout << left << setw(17) << "\nColour" << ": " << getColour();
	cout << left << setw(17) << "\nGender" << ": " << getGender();
	cout << left << setw(17) << "\nType" << ": " << getType();
	cout << left << setw(17) << "\nDescription" << ": " << getDescription();
	cout << left << setw(17) << "\nUnit Price (RM)" << ": " << fixed << showpoint << setprecision(2) << getUnitPrice();
	cout << left << setw(17) << "\nStatus" << ": " << getStatus();
	cout << left << setw(17) << "\nCreated At" << ": " << getCreatedAt();
	cout << left << setw(17) << "\nUpdated At" << ": " << getUpdatedAt();
	return 0;
}


//Setters
void Product::setId(int n) {
	id = n;
}

void Product::setName(string n) {
	name = n;
}

void Product::setColour(string n) {
	colour = n;
}

void Product::setGender(string n) {
	gender = n;
}

void Product::setType(string n) {
	type = n;
}
void Product::setDescription(string n) {
	description = n;
}

void Product::setUnitPrice(double n) {
	unitPrice = n;
}

void Product::setStatus(string n) {
	status = n;
}

void Product::setCreatedAt(string n) {
	createdAt = n;
}

void Product::setUpdatedAt(string n) {
	updatedAt = n;
}

//Getters
int Product::getId() {
	return id;
}

string Product::getName() {
	return name;
}

string Product::getColour() {
	return colour;
}

string Product::getGender() {
	return gender;
}

string Product::getType() {
	return type;
}

string Product::getDescription() {
	return description;
}

double Product::getUnitPrice() {
	return unitPrice;
}

string Product::getStatus() {
	return status;
}

string Product::getCreatedAt() {
	return createdAt;
}

string Product::getUpdatedAt() {
	return updatedAt;
}