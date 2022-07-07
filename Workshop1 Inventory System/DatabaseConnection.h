#include <mysql/jdbc.h>
using namespace sql;

#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

class DatabaseConnection
{
private:
	Connection* connection;


public:
	DatabaseConnection();
	~DatabaseConnection();

	PreparedStatement* prepareStatement(string query);
	Connection* getConnection();
};

#endif // !DATABASECONNECTION_H
