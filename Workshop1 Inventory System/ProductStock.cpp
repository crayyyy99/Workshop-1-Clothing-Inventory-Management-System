#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

#include "ProductStock.h"
#include "Database.h"

string ProductStock::TABLE_NAME = "product_stocks";

ProductStock::ProductStock()
{
	//Default Values
	id = 0;
	code = 0;
	size = "";
	quantity = 0;
	productId = 0;
	createdAt = "";
	updatedAt = "";
}

ProductStock::~ProductStock() {}

//Static Functions
int ProductStock::count()
{
	ResultSet* result = Database::query("SELECT COUNT(id) AS count FROM " + TABLE_NAME);
	result->next();
	return (int)result->getInt64("count");
}

ProductStock* ProductStock::get(int id) //READ (SELECT * FROM tablename WHERE id = ?)
{
	ProductStock* item = nullptr;
	string str = "SELECT * FROM " + TABLE_NAME + " WHERE id = ?";
	PreparedStatement* statement = Database::prepareStatement(str);
	statement->setInt(1, id);
	ResultSet* result = statement->executeQuery();
	if (result->rowsCount() >= 0)
	{
		item = new ProductStock();
		item->parseItem(result, 1);
	}
	return item;
}

ProductStock* ProductStock::gett(int code)
{
	ProductStock* item = nullptr;
	string str = "SELECT * FROM " + TABLE_NAME + " WHERE code = ?";
	PreparedStatement* statement = Database::prepareStatement(str);
	statement->setInt(1, code);
	ResultSet* result = statement->executeQuery();
	if (result->rowsCount() >= 0)
	{
		item = new ProductStock();
		item->parseItem(result, 1);
	}
	return item;
}
list<ProductStock*> ProductStock::parseList(ResultSet* result)
{
	list<ProductStock*> list;
	int count = result->rowsCount();
	for (int i = 0; i < count; i++)
	{
		ProductStock* item = new ProductStock();
		item->parseItem(result, i + 1);
		list.push_back(item);
	}
	return list;
}

//Functions
void ProductStock::parseItem(ResultSet* result, int row)
{
	result->absolute(row);
	setId(result->getInt("id"));
	setCode(result->getInt("code"));
	setSize(result->getString("size"));
	setQuantity(result->getInt("quantity"));
	setProductId(result->getInt("product_id"));
	setCreatedAt(result->getString("created_at"));
	setUpdatedAt(result->getString("updated_at"));
}

void ProductStock::save()
{
	if (id == 0)
		insert();
	else
		update();
}

void ProductStock::insert()
{
	setCreatedAt(Database::getDateTimeNow());
	setUpdatedAt(getCreatedAt());

	string str = "INSERT INTO " + TABLE_NAME
		+ "(code, size, quantity, product_id, created_at, updated_at) "
		+ "VALUES (?, ?, ?, ?, ?, ?)";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, getCode());
	ps->setString(2, getSize());
	ps->setInt(3, getQuantity());
	ps->setInt(4, getProductId());
	ps->setString(5, getCreatedAt());
	ps->setString(6, getUpdatedAt());
	ps->execute();
	setId(Database::getLastInsertId());
}

void ProductStock::resetAutoIncrement()
{
	string str = "TRUNCATE TABLE " + TABLE_NAME;

	PreparedStatement* ps = Database::prepareStatement(str);

	ps->execute();
}

void ProductStock::update()
{
	setUpdatedAt(Database::getDateTimeNow());
	string str = "UPDATE " + TABLE_NAME + " SET " +
		" code = ?, size = ?, quantity = ?, product_id = ?, created_at = ?, updated_at = ? WHERE ID = ?";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, getCode());
	ps->setString(2, getSize());
	ps->setInt(3, getQuantity());
	ps->setInt(4, getProductId());
	ps->setString(5, getCreatedAt());
	ps->setString(6, getUpdatedAt());
	ps->setInt(7, getId());
	ps->execute();
}

void ProductStock::erase()
{
	if (getId() == 0) return;
	string str = "DELETE FROM " + TABLE_NAME + " WHERE ID = ?";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, getId());
	ps->execute();
	setId(0);
}

int ProductStock::displayDetails()
{
	cout << left << setw(17) << "\nSize" << ": " << getSize();
	cout << left << setw(17) << "\nQuantity" << ": " << getQuantity();

	return 0;
}

int ProductStock::display(int b)
{
	cout << right << setw(5) << b + 1 << setw(5) << getProductId() << setw(4) << " " << left << setw(12) << getCode();
	cout << setw(4) << " " << left << setw(12) << getSize() << setw(4) << " ";
	return 0;
}


//Setters
void ProductStock::setId(int n) {
	id = n;
}

void ProductStock::setCode(int n) {
	code = n;
}

void ProductStock::setSize(string n) {
	size = n;
}

void ProductStock::setQuantity(int n) {
	quantity = n;
}

void ProductStock::setProductId(int n) {
	productId = n;
}

void ProductStock::setCreatedAt(string n) {
	createdAt = n;
}

void ProductStock::setUpdatedAt(string n) {
	updatedAt = n;
}

//Getters
int ProductStock::getId() {
	return id;
}

int ProductStock::getCode() {
	return code;
}

string ProductStock::getSize() {
	return size;
}

int ProductStock::getQuantity() {
	return quantity;
}

int ProductStock::getProductId() {
	return productId;
}

string ProductStock::getCreatedAt() {
	return createdAt;
}

string ProductStock::getUpdatedAt() {
	return updatedAt;
}