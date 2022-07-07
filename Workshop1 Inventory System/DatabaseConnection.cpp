#include <string>
using namespace std;

#include <mysql/jdbc.h>
using namespace sql;

#include "DatabaseConnection.h"

string address = "tcp://127.0.0.1:3306";
string user = "root";
string password = "abc123";
string dbName = "clothinginventoryms";

DatabaseConnection::DatabaseConnection()
{

    mysql::MySQL_Driver* driver = mysql::get_mysql_driver_instance();
    connection = driver->connect(address, user, password); //IP, Port Number, UserName, Password

    connection->setSchema(dbName); //DatabaseName
}

DatabaseConnection::~DatabaseConnection()
{
    connection->close(); //close connection

    delete connection; //clear memory
}


PreparedStatement* DatabaseConnection::prepareStatement(string query)
{
    return connection->prepareStatement(query);
}

Connection* DatabaseConnection::getConnection()
{
    return connection;
}



