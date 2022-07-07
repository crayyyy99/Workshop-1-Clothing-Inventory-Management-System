#pragma once
#include <cstring>
using namespace std;
#include "DatabaseConnection.h"

class Database
{
public:
	static DatabaseConnection* conn;
	static void connect();
	static void disconnect();
	static PreparedStatement* prepareStatement(string query);
	static int getLastInsertId();
	static void execute(string query);
	static ResultSet* query(string query);
	static string getDateTimeNow();
	static string getDateNow();
};