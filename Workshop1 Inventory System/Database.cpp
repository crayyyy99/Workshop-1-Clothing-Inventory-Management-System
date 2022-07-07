#include "Database.h"

DatabaseConnection* Database::conn = nullptr;

void Database::connect()
{
	Database::conn = new DatabaseConnection();
}

void Database::disconnect()
{
	if (Database::conn == nullptr)
		return;
	else
	{
		delete Database::conn;
		Database::conn = nullptr;
	}
}

PreparedStatement* Database::prepareStatement(string query)
{
	PreparedStatement* statement = nullptr;
	try
	{
		statement = Database::conn->prepareStatement(query);
	}
	catch (exception e)
	{
		cout << e.what();
	}
	return statement;
}

void Database::execute(string query) //without return
{
	try
	{
		Statement* stmt = Database::conn->getConnection()->createStatement();
		stmt->executeQuery(query);
	}
	catch (exception e)
	{
		cout << e.what();
	}
}

ResultSet* Database::query(string query)
{
	ResultSet* result = nullptr;
	try
	{
		Statement* stmt = Database::conn->getConnection()->createStatement();
		result = stmt->executeQuery(query);
	}
	catch (exception e)
	{
		cout << e.what();
	}
	return result;
}

int Database::getLastInsertId()
{
	Statement* stmt = Database::conn->getConnection()->createStatement();
	ResultSet* res = stmt->executeQuery("SELECT LAST_INSERT_ID() AS id");
	res->next();
	uint64_t retVal = res->getInt64("id");
	return (int)retVal;
}

string Database::getDateTimeNow()
{
	Statement* stmt = Database::conn->getConnection()->createStatement();
	ResultSet* res = stmt->executeQuery("SELECT NOW() AS time");
	res->next();
	string retVal = res->getString("time");
	return retVal;
}

string Database::getDateNow()
{
	Statement* stmt = Database::conn->getConnection()->createStatement();
	ResultSet* res = stmt->executeQuery("SELECT CURDATE() AS date");
	res->next();
	string retVal = res->getString("date");
	return retVal;
}