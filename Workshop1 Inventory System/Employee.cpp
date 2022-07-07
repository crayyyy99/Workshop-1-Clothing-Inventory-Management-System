#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

#include "Employee.h"
#include "Database.h"

string Employee::TABLE_NAME = "employees";

Employee::Employee()
{
	//Default Values
	id = 0;
	name = "";
	ic = "";
	gender = "";
	dob = "";
	position = "Salesperson";
	contactNo = "";
	email = "";
	homeAddress = "";
	username = "";
	password = "";
	permission = "normaluser";
	status = "active";
	createdAt = "";
	updatedAt = "";
}

Employee::~Employee() {}

//Static Functions
int Employee::count()
{
	ResultSet* result = Database::query("SELECT COUNT(id) AS count FROM " + TABLE_NAME);
	result->next();
	return (int)result->getInt64("count");
}

Employee* Employee::get(int id) //READ (SELECT * FROM employees WHERE id = ?)
{
	Employee* item = nullptr;
	string str = "SELECT * FROM " + TABLE_NAME + " WHERE id = ?";
	PreparedStatement* statement = Database::prepareStatement(str);
	statement->setInt(1, id);
	ResultSet* result = statement->executeQuery();
	if (result->rowsCount() >= 0)
	{
		item = new Employee();
		item->parseItem(result, 1);
	}
	return item;
}

Employee* Employee::get(string username) //READ (SELECT * FROM employees WHERE username = ?)
{
	Employee* item = nullptr;
	string str = "SELECT * FROM " + TABLE_NAME + " WHERE username = ?";
	PreparedStatement* statement = Database::prepareStatement(str);
	statement->setString(1, username);
	ResultSet* result = statement->executeQuery();
	if (result->rowsCount() >= 0)
	{
		item = new Employee();
		item->parseItem(result, 1);
	}
	return item;
}

list<Employee*> Employee::parseList(ResultSet* result)
{
	list<Employee*> list;
	int count = result->rowsCount();
	for (int i = 0; i < count; i++)
	{
		Employee* item = new Employee();
		item->parseItem(result, i + 1);
		list.push_back(item);
	}
	return list;
}

//Functions
void Employee::parseItem(ResultSet* result, int row)
{
	result->absolute(row);
	setId(result->getInt("id"));
	setName(result->getString("name"));
	setIc(result->getString("ic"));
	setGender(result->getString("gender"));
	setDob(result->getString("dob"));
	setPosition(result->getString("position"));
	setContactNo(result->getString("contact_no"));
	setEmail(result->getString("email"));
	setHomeAddress(result->getString("home_address"));
	setUsername(result->getString("username"));
	setPassword(result->getString("password"));
	setPermission(result->getString("permission"));
	setStatus(result->getString("status"));
	setCreatedAt(result->getString("created_at"));
	setUpdatedAt(result->getString("updated_at"));
}

void Employee::save()
{
	if (id == 0)
		insert();
	else
		update();
}

void Employee::insert()
{
	setCreatedAt(Database::getDateTimeNow());
	setUpdatedAt(getCreatedAt());

	string str = "INSERT INTO " + TABLE_NAME
		+ "(name, ic, gender, dob, position, contact_no, email, home_address, username, password, permission, status, created_at, updated_at) "
		+ "VALUES (?, ?, ?, ?, ? ,? ,? ,?, ?, ?, ?, ?, ?, ?)";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setString(1, getName());
	ps->setString(2, getIc());
	ps->setString(3, getGender());
	ps->setString(4, getDob());
	ps->setString(5, getPosition());
	ps->setString(6, getContactNo());
	ps->setString(7, getEmail());
	ps->setString(8, getHomeAddress());
	ps->setString(9, getUsername());
	ps->setString(10, getPassword());
	ps->setString(11, getPermission());
	ps->setString(12, getStatus());
	ps->setString(13, getCreatedAt());
	ps->setString(14, getUpdatedAt());
	ps->execute();
	setId(Database::getLastInsertId());

}

void Employee::resetAutoIncrement()
{
	string str = "TRUNCATE TABLE " + TABLE_NAME;

	PreparedStatement* ps = Database::prepareStatement(str);
	
	ps->execute();
}

void Employee::update()
{
	setUpdatedAt(Database::getDateTimeNow());
	string str = "UPDATE " + TABLE_NAME + " SET " +
		" name = ?, ic = ?, gender = ?, dob = ?, position = ?, contact_no = ?," +
		"email = ?, home_address = ?, username = ?, password = ?, permission = ?, status = ?, " +
		"created_at = ?, updated_at = ? WHERE ID = ?";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setString(1, getName());
	ps->setString(2, getIc());
	ps->setString(3, getGender());
	ps->setString(4, getDob());
	ps->setString(5, getPosition());
	ps->setString(6, getContactNo());
	ps->setString(7, getEmail());
	ps->setString(8, getHomeAddress());
	ps->setString(9, getUsername());
	ps->setString(10, getPassword());
	ps->setString(11, getPermission());
	ps->setString(12, getStatus());
	ps->setString(13, getCreatedAt());
	ps->setString(14, getUpdatedAt());
	ps->setInt(15, getId());
	ps->execute();
}

void Employee::erase()
{
	if (getId() == 0) return;
	string str = "DELETE FROM " + TABLE_NAME + " WHERE ID = ?";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, getId());
	ps->execute();
	setId(0);
}

string Employee::displayAccDetails()
{
	stringstream str;
	str << "\nUsername\t: "<< getUsername();
	str << "\nName\t\t: "<< getName() ;
	str << "\nIC Number\t: " << getIc();
	str << "\nGender\t\t: " << getGender();
	str << "\nDate of Birth\t: " << getDob();
	str << "\nContact Number\t: " << getContactNo();
	str << "\nEmail Address\t: " << getEmail();
	str << "\nHome Address\t: " << getHomeAddress();

	return str.str();
}

string Employee::displayDetails()
{
	stringstream str;

	str << "\nName\t\t: " << getName();
	str << "\nIC Number\t: " << getIc();
	str << "\nGender\t\t: " << getGender();
	str << "\nDate of Birth\t: " << getDob();
	str << "\nPosition\t: " << getPosition();
	str << "\nContact Number\t: " << getContactNo();
	str << "\nE-mail Address\t: " << getEmail();
	str << "\nHome Address\t: " << getHomeAddress();
	str << "\nUsername\t: " << getUsername();
	str << "\nPermission\t: " << getPermission();
	str << "\nStatus\t\t: " << getStatus();
	str << "\nCreated At\t: " << getCreatedAt();
	str << "\nUpdated At\t: " << getUpdatedAt();

	return str.str();
}

int Employee::display(int b)
{
	
	cout << right << setw(5) << b+1 << setw(5) << getId() << setw(4) << " " << left << setw(40) << getName();
	cout << setw(4) << " " << left << setw(12) << getIc() << setw(4) << " " << left << setw(11) << getPosition() << setw(4) << " ";
	cout << left << setw(15) << getUsername() << setw(4) << " " << left << setw(10) << getStatus() << setw(4) << " " << getPermission();
	
	return 0;
}


//Setters
void Employee::setId(int n) {
	id = n;
}

void Employee::setName(string n) {
	name = n;
}

void Employee::setIc(string n) {
	ic = n;
}

void Employee::setGender(string n) {
	gender = n;
}

void Employee::setDob(string n) {
	dob = n;
}

void Employee::setPosition(string n) {
	position = n;
}

void Employee::setContactNo(string n) {
	contactNo = n;
}

void Employee::setEmail(string n) {
	email = n;
}

void Employee::setHomeAddress(string n) {
	homeAddress = n;
}


void Employee::setUsername(string n) {
	username = n;
}

void Employee::setPassword(string n) {
	password = n;
}

void Employee::setPermission(string n) {
	permission = n;
}

void Employee::setStatus(string n) {
	status = n;
}

void Employee::setCreatedAt(string n) {
	createdAt = n;
}

void Employee::setUpdatedAt(string n) {
	updatedAt = n;
}

//Getters
int Employee::getId() {
	return id;
}

string Employee::getName() {
	return name;
}
	
string Employee::getIc() {
	return ic;
}

string Employee::getGender() {
	return gender;
}

string Employee::getDob() {
	return dob;
}

string Employee::getPosition() {
	return position;
}

string Employee::getContactNo() {
	return contactNo;
}

string Employee::getEmail() {
	return email;
}

string Employee::getHomeAddress() {
	return homeAddress;
}


string Employee::getUsername() {
	return username;
}

string Employee::getPassword() {
	return password;
}

string Employee::getPermission() {
	return permission;
}

string Employee::getStatus() {
	return status;
}

string Employee::getCreatedAt() {
	return createdAt;
}

string Employee::getUpdatedAt() {
	return updatedAt;
}