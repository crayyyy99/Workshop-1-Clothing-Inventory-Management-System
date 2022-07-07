#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

#include "Supplier.h"
#include "Database.h"

string Supplier::TABLE_NAME = "suppliers";

Supplier::Supplier()
{
	//Default Values
	id = 0;
	compName = "";
	contactName = "";
	contactTitle = "";
	compEmail = "";
	compAddress = "";
	contactNo1 = "";
	contactNo2 = "";
	otherDetails = "";
	status = "newly added";
	createdAt = "";
	updatedAt = "";
}

Supplier::~Supplier() {}

//Static Functions
int Supplier::count()
{
	ResultSet* result = Database::query("SELECT COUNT(id) AS count FROM " + TABLE_NAME);
	result->next();
	return (int)result->getInt64("count");
}

Supplier* Supplier::get(int id) //READ (SELECT * FROM tablename WHERE id = ?)
{
	Supplier* item = nullptr;
	string str = "SELECT * FROM " + TABLE_NAME + " WHERE id = ?";
	PreparedStatement* statement = Database::prepareStatement(str);
	statement->setInt(1, id);
	ResultSet* result = statement->executeQuery();
	if (result->rowsCount() >= 0)
	{
		item = new Supplier();
		item->parseItem(result, 1);
	}
	return item;
}

list<Supplier*> Supplier::parseList(ResultSet* result)
{
	list<Supplier*> list;
	int count = result->rowsCount();
	for (int i = 0; i < count; i++)
	{
		Supplier* item = new Supplier();
		item->parseItem(result, i + 1);
		list.push_back(item);
	}
	return list;
}

//Functions
void Supplier::parseItem(ResultSet* result, int row)
{
	result->absolute(row);
	setId(result->getInt("id"));
	setCompName(result->getString("company_name"));
	setContactName(result->getString("contact_name"));
	setContactTitle(result->getString("contact_title"));
	setCompEmail(result->getString("company_email"));
	setCompAddress(result->getString("company_address"));
	setContactNo1(result->getString("contact_no1"));
	setContactNo2(result->getString("contact_no2"));
	setOtherDetails(result->getString("other_details"));
	setStatus(result->getString("status"));
	setCreatedAt(result->getString("created_at"));
	setUpdatedAt(result->getString("updated_at"));
}

void Supplier::save()
{
	if (id == 0)
		insert();
	else
		update();
}

void Supplier::insert()
{
	setCreatedAt(Database::getDateTimeNow());
	setUpdatedAt(getCreatedAt());

	string str = "INSERT INTO " + TABLE_NAME
		+ "(company_name, contact_name, contact_title, company_email, company_address, contact_no1, contact_no2, other_details, status, created_at, updated_at) "
		+ "VALUES (?, ?, ?, ?, ?, ? ,? ,? ,?, ?, ?)";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setString(1, getCompName());
	ps->setString(2, getContactName());
	ps->setString(3, getContactTitle());
	ps->setString(4, getCompEmail());
	ps->setString(5, getCompAddress());
	ps->setString(6, getContactNo1());
	ps->setString(7, getContactNo2());
	ps->setString(8, getOtherDetails());
	ps->setString(9, getStatus());
	ps->setString(10, getCreatedAt());
	ps->setString(11, getUpdatedAt());
	ps->execute();
	setId(Database::getLastInsertId());

}

void Supplier::resetAutoIncrement()
{
	string str = "TRUNCATE TABLE " + TABLE_NAME;

	PreparedStatement* ps = Database::prepareStatement(str);

	ps->execute();
}

void Supplier::update()
{
	setUpdatedAt(Database::getDateTimeNow());
	string str = "UPDATE " + TABLE_NAME + " SET " +
		" company_name = ?, contact_name = ?, contact_title = ?, company_email = ?," +
		" company_address = ?, contact_no1 = ?, contact_no2 = ?, other_details = ?, status = ?, " +
		" created_at = ?, updated_at = ? WHERE ID = ?";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setString(1, getCompName());
	ps->setString(2, getContactName());
	ps->setString(3, getContactTitle());
	ps->setString(4, getCompEmail());
	ps->setString(5, getCompAddress());
	ps->setString(6, getContactNo1());
	ps->setString(7, getContactNo2());
	ps->setString(8, getOtherDetails());
	ps->setString(9, getStatus());
	ps->setString(10, getCreatedAt());
	ps->setString(11, getUpdatedAt());
	ps->setInt(12, getId());
	ps->execute();
}

void Supplier::erase()
{
	if (getId() == 0) return;
	string str = "DELETE FROM " + TABLE_NAME + " WHERE ID = ?";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, getId());
	ps->execute();
	setId(0);
}

string Supplier::displayDetails()
{
	stringstream str;

	str << "\nCompany Name\t\t: " << getCompName();
	str << "\nContact Name\t\t: " << getContactName();
	str << "\nContact Title\t\t: " << getContactTitle();
	str << "\nCompany E-mail Address\t: " << getCompEmail();
	str << "\nCompany Address\t\t: " << getCompAddress();
	str << "\nContact Number 1\t: " << getContactNo1();
	str << "\nContact Number 2\t: " << getContactNo2();
	str << "\nOther Details\t\t: " << getOtherDetails();
	str << "\nStatus\t\t\t: " << getStatus();
	str << "\nCreated At\t\t: " << getCreatedAt();
	str << "\nUpdated At\t\t: " << getUpdatedAt();

	return str.str();
}

int Supplier::display(int b)
{
	cout << right << setw(5) << b + 1 << setw(5) << getId() << setw(4) << " " << left << setw(50) << getCompName();
	cout << setw(4) << " " << left << setw(40) << getContactName() << setw(4) << " " << getStatus();
	return 0;
}


//Setters
void Supplier::setId(int n) {
	id = n;
}

void Supplier::setCompName(string n) {
	compName = n;
}

void Supplier::setContactName(string n) {
	contactName = n;
}

void Supplier::setContactTitle(string n) {
	contactTitle = n;
}

void Supplier::setCompEmail(string n) {
	compEmail = n;
}

void Supplier::setCompAddress(string n) {
	compAddress = n;
}

void Supplier::setContactNo1(string n) {
	contactNo1 = n;
}

void Supplier::setContactNo2(string n) {
	contactNo2 = n;
}

void Supplier::setOtherDetails(string n) {
	otherDetails = n;
}

void Supplier::setStatus(string n) {
	status = n;
}

void Supplier::setCreatedAt(string n) {
	createdAt = n;
}

void Supplier::setUpdatedAt(string n) {
	updatedAt = n;
}

//Getters
int Supplier::getId() {
	return id;
}

string Supplier::getCompName() {
	return compName;
}

string Supplier::getContactName() {
	return contactName;
}

string Supplier::getContactTitle() {
	return contactTitle;
}

string Supplier::getCompEmail() {
	return compEmail;
}

string Supplier::getCompAddress() {
	return compAddress;
}

string Supplier::getContactNo1() {
	return contactNo1;
}

string Supplier::getContactNo2() {
	return contactNo2;
}

string Supplier::getOtherDetails() {
	return otherDetails;
}

string Supplier::getStatus() {
	return status;
}

string Supplier::getCreatedAt() {
	return createdAt;
}

string Supplier::getUpdatedAt() {
	return updatedAt;
}