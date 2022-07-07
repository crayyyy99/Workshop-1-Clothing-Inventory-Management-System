#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <iomanip>
#include <conio.h>
#include <cstdlib> 
#include <ctime> 
#include <cmath>

using namespace std;

#include <mysql/jdbc.h>
using namespace sql;

#include "TransactionController.h"
#include "ProductController.h"
#include "DatabaseConnection.h"
#include "Transaction.h"
#include "TransactionDetail.h"
#include "IndexPage.h"
#include "CheckInput.h"

void TransactionController::viewTransList(const int m)
{
	int n = m;

	int totalCount = Transaction::count();

	if (totalCount == 0)
	{
		page.color(15);
		cout << "\nNumber of transactions: " << totalCount << endl;
	}
	else
	{
		string str = "select transaction_id, employee_id, employees.`name`,transactions.created_at AS date, sum(transaction_details.quantity * (unit_price - (unit_price * discount / 100))) "
			+ std::string("AS totalSales, sum(transaction_details.quantity) AS itemSold from transaction_details inner JOIN transactions on(transaction_id = transactions.id)")
			+ "inner join employees on(transactions.employee_id = employees.id) inner join product_stocks on(transaction_details.`code`=product_stocks.`code`) "
			+ "inner join products on(products.id = product_id) group by transaction_id order by transaction_id LIMIT 10 offset "
			+ to_string(m);

		PreparedStatement* ps = Database::prepareStatement(str);
		ResultSet* result = ps->executeQuery();

		int SIZE = ((int)result->rowsCount() < totalCount) ? (int)result->rowsCount() : totalCount;

		page.color(15);
		cout << "\nNumber of transactions: " << totalCount << endl;

		cout << "\nShowing " << n + 1;

		if (m + 10 <= totalCount)
			cout << "~" << n + 10 << " out of " << totalCount << " results.\n" << endl;
		else
			cout << "~" << totalCount << " out of " << totalCount << " results.\n" << endl;

		page.color(7);
		cout << right << setw(5) << "No." << setw(5) << "ID" << setw(4) << " " << left << setw(19) << "Date" << setw(4) << " "<<left << setw(56) << "Employee" << setw(4) << " ";
		cout << left << setw(10) << "Item Count" << setw(4) << " " << left << setw(10) << "Sales (RM)" << endl;

		for (int i = 0; i < SIZE; i++)
		{
			result->next();
			cout << right << setw(5) << n + 1 << setw(5) << result->getInt("transaction_id") << setw(4) << " " << left << setw(19) << result->getString("date");
			cout << setw(4) << " " << setw(3) << left << result->getInt("employee_id") << " - " << left << setw(50) << result->getString("name") << setw(4) << " ";
			cout << left << setw(10) << result->getInt("itemSold") << setw(4) << " " << right << setw(10) << fixed << showpoint << setprecision(2) << result->getDouble("totalSales") << endl;
			n++;
		}
	}
}

int TransactionController::countSearchDate(string search)
{
	string str = "select COUNT(DISTINCT transactions.id) AS count from transactions where transactions.created_at like '%" + search + "%'";
	PreparedStatement* ps = Database::prepareStatement(str);
	ResultSet* result = ps->executeQuery();
	result->next();
	return (int)result->getInt64("count");
}
void TransactionController::viewTransListDate(string searchVal, const int m)
{
	int n = m;

	int totalCount = countSearchDate(searchVal);

	if (totalCount == 0)
	{
		page.color(15);
		cout << "\nNumber of transactions: " << totalCount << endl;
	}
	else
	{
		string str = "select transaction_id, employee_id, employees.`name`,transactions.created_at AS date, sum(transaction_details.quantity * (unit_price - (unit_price * discount / 100))) "
			+ std::string("AS totalSales, sum(transaction_details.quantity) AS itemSold from transaction_details inner JOIN transactions on(transaction_id = transactions.id)")
			+ "inner join employees on(transactions.employee_id = employees.id) inner join product_stocks on(transaction_details.`code`=product_stocks.`code`) "
			+ "inner join products on(products.id = product_id) where transactions.created_at LIKE ? group by transaction_id order by transaction_id LIMIT 10 offset "
			+ to_string(m);

		PreparedStatement* ps = Database::prepareStatement(str);
		ps->setString(1, "%" + searchVal + "%");
		ResultSet* result = ps->executeQuery();

		int SIZE = ((int)result->rowsCount() < totalCount) ? (int)result->rowsCount() : totalCount;

		page.color(15);
		cout << "\nNumber of transactions: " << totalCount << endl;

		cout << "\nShowing " << n + 1;

		if (m + 10 <= totalCount)
			cout << "~" << n + 10 << " out of " << totalCount << " results for \"" << searchVal << "\".\n" << endl;
		else
			cout << "~" << totalCount << " out of " << totalCount << " results for \"" << searchVal << "\".\n" << endl;

		page.color(7);
		cout << right << setw(5) << "No." << setw(5) << "ID" << setw(4) << " " << left << setw(19) << "Date" << setw(4) << " " << left << setw(56) << "Employee" << setw(4) << " ";
		cout << left << setw(10) << "Item Count" << setw(4) << " " << left << setw(10) << "Sales (RM)" << endl;

		for (int i = 0; i < SIZE; i++)
		{
			result->next();
			cout << right << setw(5) << n + 1 << setw(5) << result->getInt("transaction_id") << setw(4) << " " << left << setw(19) << result->getString("date");
			cout << setw(4) << " " << setw(3) << left << result->getInt("employee_id") << " - " << left << setw(50) << result->getString("name") << setw(4) << " ";
			cout << left << setw(10) << result->getInt("itemSold") << setw(4) << " " << right << setw(10) << fixed << showpoint << setprecision(2) << result->getDouble("totalSales") << endl;
			n++;
		}
	}
}
bool TransactionController::dateExist(string date)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("SELECT EXISTS (SELECT * FROM transactions WHERE created_at LIKE ?)");

	ps->setString(1, "%" + date + "%");

	ResultSet* rs = ps->executeQuery();
	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}

int TransactionController::countSearchEmployee(string search)
{
	string str = "select COUNT(DISTINCT transaction_id) AS count from transaction_details inner JOIN transactions on(transaction_id = transactions.id) "
		+ std::string("inner join employees on(transactions.employee_id = employees.id) inner join product_stocks on(transaction_details.`code`=product_stocks.`code`) ")
			+ "inner join products on (products.id = product_id) where employees.`name` like '%" + search + "%' ";
	PreparedStatement* ps = Database::prepareStatement(str);
	ResultSet* result = ps->executeQuery();
	result->next();
	return (int)result->getInt64("count");
}
void TransactionController::viewTransListEmployee(string searchVal, const int m)
{
	int n = m;

	int totalCount = countSearchEmployee(searchVal);

	if (totalCount == 0)
	{
		page.color(15);
		cout << "\nNumber of transactions: " << totalCount << endl;
	}
	else
	{
		string str = "select transaction_id, employee_id, employees.`name`,transactions.created_at AS date, sum(transaction_details.quantity * (unit_price - (unit_price * discount / 100))) "
			+ std::string("AS totalSales, sum(transaction_details.quantity) AS itemSold from transaction_details inner JOIN transactions on(transaction_id = transactions.id)")
			+ "inner join employees on(transactions.employee_id = employees.id) inner join product_stocks on(transaction_details.`code`=product_stocks.`code`) "
			+ "inner join products on(products.id = product_id) where employees.`name` LIKE ? group by transaction_id order by transaction_id LIMIT 10 offset "
			+ to_string(m);

		PreparedStatement* ps = Database::prepareStatement(str);
		ps->setString(1, "%" + searchVal + "%");
		ResultSet* result = ps->executeQuery();

		int SIZE = ((int)result->rowsCount() < totalCount) ? (int)result->rowsCount() : totalCount;

		page.color(15);
		cout << "\nNumber of transactions: " << totalCount << endl;

		cout << "\nShowing " << n + 1;

		if (m + 10 <= totalCount)
			cout << "~" << n + 10 << " out of " << totalCount << " results for \"" << searchVal << "\".\n" << endl;
		else
			cout << "~" << totalCount << " out of " << totalCount << " results for \"" << searchVal << "\".\n" << endl;

		page.color(7);
		cout << right << setw(5) << "No." << setw(5) << "ID" << setw(4) << " " << left << setw(19) << "Date" << setw(4) << " " << left << setw(56) << "Employee" << setw(4) << " ";
		cout << left << setw(10) << "Item Count" << setw(4) << " " << left << setw(10) << "Sales (RM)" << endl;

		for (int i = 0; i < SIZE; i++)
		{
			result->next();
			cout << right << setw(5) << n + 1 << setw(5) << result->getInt("transaction_id") << setw(4) << " " << left << setw(19) << result->getString("date");
			cout << setw(4) << " " << setw(3) << left << result->getInt("employee_id") << " - " << left << setw(50) << result->getString("name") << setw(4) << " ";
			cout << left << setw(10) << result->getInt("itemSold") << setw(4) << " " << right << setw(10) << fixed << showpoint << setprecision(2) << result->getDouble("totalSales") << endl;
			n++;
		}
	}
}
bool TransactionController::employeeExist(string search)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("select exists (select * from transactions inner JOIN transaction_details on(transaction_id = transactions.id) inner join employees on (employee_id = employees.id) where name like ? group by transactions.id)");

	ps->setString(1, "%" + search + "%");

	ResultSet* rs = ps->executeQuery();
	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}

int TransactionController::countSearchEmployeeDate(string searchName, string searchDate)
{
	string str = "select COUNT(DISTINCT transaction_id) AS count from transaction_details inner JOIN transactions on(transaction_id = transactions.id) "
		+ std::string("inner join employees on(transactions.employee_id = employees.id) inner join product_stocks on(transaction_details.`code`=product_stocks.`code`) ")
		+ "inner join products on (products.id = product_id) where employees.`name` like '%" + searchName + "%' AND transactions.created_at like '%" + searchDate + "%'";
	PreparedStatement* ps = Database::prepareStatement(str);
	ResultSet* result = ps->executeQuery();
	result->next();
	return (int)result->getInt64("count");
}
void TransactionController::viewTransListEmployeeDate(string searchName, string searchDate, const int m)
{
	int n = m;

	int totalCount = countSearchEmployeeDate(searchName,searchDate);

	if (totalCount == 0)
	{
		page.color(15);
		cout << "\nNumber of transactions: " << totalCount << endl;
	}
	else
	{
		string str = "select transaction_id, employee_id, employees.`name`,transactions.created_at AS date, sum(transaction_details.quantity * (unit_price - (unit_price * discount / 100))) "
			+ std::string("AS totalSales, sum(transaction_details.quantity) AS itemSold from transaction_details inner JOIN transactions on(transaction_id = transactions.id)")
			+ "inner join employees on(transactions.employee_id = employees.id) inner join product_stocks on(transaction_details.`code`=product_stocks.`code`) "
			+ "inner join products on(products.id = product_id) where employees.`name` LIKE ? and transactions.created_at like ? group by transaction_id order by transaction_id LIMIT 10 offset "
			+ to_string(m);

		PreparedStatement* ps = Database::prepareStatement(str);
		ps->setString(1, "%" + searchName + "%");
		ps->setString(2, "%" + searchDate + "%");
		ResultSet* result = ps->executeQuery();

		int SIZE = ((int)result->rowsCount() < totalCount) ? (int)result->rowsCount() : totalCount;

		page.color(15);
		cout << "\nNumber of transactions: " << totalCount << endl;

		cout << "\nShowing " << n + 1;

		if (m + 10 <= totalCount)
			cout << "~" << n + 10 << " out of " << totalCount << " results for \"" << searchName << "\" and \"" << searchDate << "\".\n" << endl;
		else
			cout << "~" << totalCount << " out of " << totalCount << " results for \"" << searchName << "\" and \"" << searchDate << "\".\n" << endl;

		page.color(7);
		cout << right << setw(5) << "No." << setw(5) << "ID" << setw(4) << " " << left << setw(19) << "Date" << setw(4) << " " << left << setw(56) << "Employee" << setw(4) << " ";
		cout << left << setw(10) << "Item Count" << setw(4) << " " << left << setw(10) << "Sales (RM)" << endl;

		for (int i = 0; i < SIZE; i++)
		{
			result->next();
			cout << right << setw(5) << n + 1 << setw(5) << result->getInt("transaction_id") << setw(4) << " " << left << setw(19) << result->getString("date");
			cout << setw(4) << " " << setw(3) << left << result->getInt("employee_id") << " - " << left << setw(50) << result->getString("name") << setw(4) << " ";
			cout << left << setw(10) << result->getInt("itemSold") << setw(4) << " " << right << setw(10) << fixed << showpoint << setprecision(2) << result->getDouble("totalSales") << endl;
			n++;
		}
	}
}
bool TransactionController::employeeDateExist(string searchName, string searchDate)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("select exists (select * from transactions inner JOIN transaction_details on(transaction_id = transactions.id) inner join employees on (employee_id = employees.id) where name like ? and transactions.created_at like ? group by transactions.id)");

	ps->setString(1, "%" + searchName + "%");
	ps->setString(2, "%" + searchDate + "%");

	ResultSet* rs = ps->executeQuery();
	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}

void TransactionController::transManagement()
{
	bool isBack = false;
	while (!isBack)
	{
		bool isStopLoop = false;

		int m = 0, a = 0, b = 0, c = 0;
		int id = 0, number = 0;
		string mngChoice;
		string searchVal = "", searchName = "", searchDate = "";
		bool isDateFound = false;
		bool isEmpFound = false;
		bool isEmpDateFound = false;
		bool isSuppFound = false;

		while (!isStopLoop)
		{
			bool isValidMngChoice = false;

			if (isDateFound)
			{
				system("cls");
				page.systemHeaderOrange();
				page.transMngmtHeader();
				viewTransListDate(searchVal, a);
			}
			else if (isEmpFound)
			{
				system("cls");
				page.systemHeaderOrange();
				page.transMngmtHeader();
				viewTransListEmployee(searchVal, b);
			}
			else if (isEmpDateFound)
			{
				system("cls");
				page.systemHeaderOrange();
				page.transMngmtHeader();
				viewTransListEmployeeDate(searchName, searchDate, c);
			}
			else
			{
				system("cls");
				page.systemHeaderOrange();
				page.transMngmtHeader();
				viewTransList(m);
			}

			cout << endl;
			page.transMngmtPanel();

			while (!isValidMngChoice)
			{
				page.color(14);
				cout << "\nEnter choice: ";
				page.color(7);
				mngChoice.clear();
				cin >> mngChoice;

				//Previous Page
				if (mngChoice == "1")
				{
					isValidMngChoice = true;

					if (isDateFound)
					{
						if (a == 0)
						{
							page.color(12);
							cout << "\nThis is the first page.\n" << endl;
							page.color(7);
							system("pause");
						}
						else
							a = a - 10;
					}
					else if (isEmpFound)
					{
						if (b == 0)
						{
							page.color(12);
							cout << "\nThis is the first page.\n" << endl;
							page.color(7);
							system("pause");
						}
						else
							b = b - 10;
					}
					else if (isEmpDateFound)
					{
						if (c == 0)
						{
							page.color(12);
							cout << "\nThis is the first page.\n" << endl;
							page.color(7);
							system("pause");
						}
						else
							c = c - 10;
					}
					else
					{
						if (m == 0)
						{
							page.color(12);
							cout << "\nThis is the first page.\n" << endl;
							page.color(7);
							system("pause");
						}
						else
							m = m - 10;
					}
				}

				//Next Page
				else if (mngChoice == "2")
				{
					isValidMngChoice = true;
					if (isDateFound)
					{
						if (a + 10 >= countSearchDate(searchVal))
						{
							page.color(12);
							cout << "\nThis is the last page.\n" << endl;
							page.color(7);
							system("pause");
						}
						else
							a = a + 10;
					}
					else if (isEmpFound)
					{
						if (b + 10 >= countSearchEmployee(searchVal))
						{
							page.color(12);
							cout << "\nThis is the last page.\n" << endl;
							page.color(7);
							system("pause");
						}
						else
							b = b + 10;
					}
					else if (isEmpDateFound)
					{
						if (c + 10 >= countSearchEmployeeDate(searchName, searchDate))
						{
							page.color(12);
							cout << "\nThis is the last page.\n" << endl;
							page.color(7);
							system("pause");
						}
						else
							c = c + 10;
					}
					else
					{
						if (m + 10 >= Transaction::count())
						{
							page.color(12);
							cout << "\nThis is the last page.\n" << endl;
							page.color(7);
							system("pause");
						}
						else
							m = m + 10;
					}
				}
				
				//Search Transaction
				else if (mngChoice == "3")
				{
					isValidMngChoice = true;
					a = 0, b = 0, c = 0;
					isDateFound = false;
					isEmpDateFound = false;
					isSuppFound = false;
					int i = 0;
					string searchChoice;
					bool isValidSearchChoice = false;
					page.searchTransPanelAdmin();

					while (!isValidSearchChoice)
					{
						page.color(14);
						cout << "\nEnter choice: ";
						page.color(7);
						searchChoice.clear();
						cin >> searchChoice;

						if (searchChoice == "1") //Date
						{
							isValidSearchChoice = true;

							page.showSearchDateFormat();
							bool isValidDate = false;
							while (!isValidDate)
							{
								page.color(14);
								cout << "\nEnter date to search: ";
								page.color(7);

								searchVal.clear();
								if (i == 0)
								{
									cin.ignore(1, '\n');
									i++;
								}
								getline(cin, searchVal);

								if (searchVal != "")
								{
									isValidDate = true;
									bool isDateExist = dateExist(searchVal);
									if (isDateExist)
										isDateFound = true;
									else
									{
										page.color(12);
										cout << "\nNo result found.\n" << endl;
										page.color(7);
										system("pause");
									}
								}
								else
								{
									page.color(12);
									cout << "\nPlease fill out this field. " << endl;
								}		
							}				
						}
						else if (searchChoice == "2") //EmployeeName
						{
							isValidSearchChoice = true;
							bool isValidName = false;
							while (!isValidName)
							{
								page.color(14);
								cout << "\nEnter employee name to search: ";
								page.color(7);

								searchVal.clear();
								if (i == 0)
								{
									cin.ignore(1, '\n');
									i++;
								}
								getline(cin, searchVal);

								if (searchVal != "")
								{
									isValidName = true;
									bool isEmpExist = employeeExist(searchVal);
									if (isEmpExist)
										isEmpFound = true;
									else
									{
										page.color(12);
										cout << "\nNo result found.\n" << endl;
										page.color(7);
										system("pause");
									}
								}
								else
								{
									page.color(12);
									cout << "\nPlease fill out this field. " << endl;
								}
							}						
						}
						else if (searchChoice == "3")//EmployeeName & Date
						{
							isValidSearchChoice = true;
							bool isValidName = false;
							while (!isValidName)
							{
								page.color(14);
								cout << left << setw(22) << "\nEnter Employee Name" << ": ";
								page.color(7);

								searchName.clear();
								if (i == 0)
								{
									cin.ignore(1, '\n');
									i++;
								}
								getline(cin, searchName);

								if (searchName == "")
								{
									page.color(12);
									cout << "\nPlease fill out this field. " << endl;
								}
								else
									isValidName = true;
							}
							
							bool isValidDate = false;
							while (!isValidDate)
							{
								page.color(14);
								cout << left << setw(22) << "\nEnter Date" << ": ";
								page.color(7);

								searchDate.clear();
								getline(cin, searchDate);

								if (searchDate == "")
								{
									page.color(12);
									cout << "\nPlease fill out this field. " << endl;
								}
								else
									isValidDate = true;
							}
							
							bool isEmpDateExist = employeeDateExist(searchName, searchDate);
							if (isEmpDateExist)
								isEmpDateFound = true;
							else
							{
								page.color(12);
								cout << "\nNo result found.\n" << endl;
								page.color(7);
								system("pause");
							}
						}
						else
							page.showInvalidChoiceMsg();
					}
				}

				//View Transaction
				else if (mngChoice == "4")
				{
					isValidMngChoice = true;
					string strId;
					bool isDigit = false;
					bool isValidStrId = false;

					while (!isValidStrId)
					{
						page.color(14);
						cout << "\nEnter Transaction ID: ";
						strId.clear();
						page.color(7);
						cin >> strId;

						isDigit = input.isDigit(strId);
						if (isDigit)
						{
							isValidStrId = true;
							int transId = stoi(strId);
							bool isIdExist = idExistTrans(transId);
							if (isIdExist)
							{
								system("cls");
								page.systemHeaderOrange();
								page.transMngmtHeader();
								viewDetails(transId);
								cout <<"\n"<< endl;
								page.color(7);
								system("pause");
							}

							//ID not found
							else
							{
								page.color(12);
								cout << "\nNo result found.\n" << endl;
								page.color(7);
								system("pause");
							}
						}
						//Invalid ID input (not digit)
						else
							page.showInvalidChoiceMsg();
					}
				}

				//Manage Refund Item
				else if (mngChoice == "5")
				{
					isValidMngChoice = true;
					string strId;
					bool isDigit = false;
					bool isValidStrId = false;

					while (!isValidStrId)
					{
						page.color(14);
						cout << "\nEnter Transaction ID: ";
						strId.clear();
						page.color(7);
						cin >> strId;

						isDigit = input.isDigit(strId);
						if (isDigit)
						{
							isValidStrId = true;
							int transId = stoi(strId);
							bool isIdExist = idExistTrans(transId);
							if (isIdExist)												
								manageRefund(transId);

							//ID not found
							else
							{
								page.color(12);
								cout << "\nNo result found.\n" << endl;
								page.color(7);
								system("pause");
							}
						}
						//Invalid ID input (not digit)
						else
							page.showInvalidChoiceMsg();
					}				
				}
				
				//Back
				else if (mngChoice == "6")
				{
					if (isDateFound)
					{
						isValidMngChoice = true;
						isStopLoop = true;
					}
					else if (isDateFound)
					{
						isValidMngChoice = true;
						isStopLoop = true;
					}
					else if (isEmpDateFound)
					{
						isValidMngChoice = true;
						isStopLoop = true;
					}
					else
					{
						isValidMngChoice = true;
						isStopLoop = true;
						isBack = true;
					}
				}

				//Invalid Input
				else
					page.showInvalidChoiceMsg();
			}
		}
	}
}

//Add Transaction
void TransactionController::addTransaction(int employeeId)
{
	Transaction trans;
	TransactionDetail transDetails;
	bool isQuitAdd = false;

	setEmployeeLoginId(employeeId);

	while (!isQuitAdd)
	{
		if (Transaction::count() == 0)
			trans.resetAutoIncrement();
		if (TransactionDetail::count() == 0)
			transDetails.resetAutoIncrement();

		system("cls");
		page.systemHeaderBlue();
		page.addtransHeader();

		page.color(14);
		cout << "\n Add New Transaction" << endl;
		for (int i = 0; i < 20; i++) { cout << char(205); }cout << endl;

		Transaction* t = new Transaction();

		t->setEmployeeId(getEmployeeLoginId());
		t->save();

		setA(0);
		int transId = Database::getLastInsertId();
		if (getA() == 0)
			setTransId(transId);

		isQuitAdd = addTransItem();
	}
}
bool TransactionController::addTransItem()
{
	bool cancelLoopAddItem = false;

	string searchCode, searchVal;
	setItemNo(0);

	while (!cancelLoopAddItem)
	{
		system("cls");
		page.systemHeaderBlue();
		page.addtransHeader();
		showTransaction(getTransId());
		if (getA() != 0)
		{
			showTransactionDetails(getTransId()); 
			page.color(15);
			cout << "\nSubtotal (RM): " << readTotalPrice(getTransId());
			cout << endl;
		}

		bool isInvalidProductCode = false;
		int i = 0;
		while (!isInvalidProductCode)
		{
			page.color(14);
			cout << "\nEnter product code [To manually find product name and id, enter 0] " << endl;
			cout << ": ";

			page.color(7);
			searchCode.clear();
			cin >> searchCode;

			if (searchCode != "")
			{
				bool isDigit = input.isDigit(searchCode);
				if (isDigit)
				{
					//manually enter
					if (searchCode == "0")
					{
						int i = 0;
						bool cancelLoopManual = false;					
						while (!cancelLoopManual)
						{
							bool noResult = false;
							while (!noResult)
							{
								setResultFound(0);
								page.color(14);
								cout << "\nEnter product id OR Search product name" << endl;
								cout << ": ";

								page.color(7);
								searchVal.clear();
								if (i == 0)
								{
									cin.ignore(1, '\n');
									i++;
								}
								getline(cin, searchVal);

								if (searchVal != "")
								{
									bool isDigit = input.isDigit(searchVal);
									//searchProductId
									if (isDigit)
									{
										int productId = stoi(searchVal);
										bool isProdIdExist = prodDetails.idExist(productId);
										if (isProdIdExist)
										{
											bool isValidConfirmProd = false;
											string confirmProd;

											prodDetails.purOrderShowProdDetails(productId);
											page.color(15);
											cout << "\nConfirm product?" << endl;
											page.showYesNoChoice();

											while (!isValidConfirmProd)
											{
												page.color(14);
												cout << left << setw(21) << "\nEnter choice" << ": ";
												confirmProd.clear();
												page.color(7);
												cin >> confirmProd;

												if (confirmProd == "Y" || confirmProd == "y")
												{
													
													cancelLoopManual = true;	
													isValidConfirmProd = true;
													setProductId(productId);
													string productSize;

													int i = 0;
													bool isValidProductSize = false;
													while (!isValidProductSize)
													{
														page.color(14);
														cout << left << setw(21) << "\nEnter Product Size" << ": ";
														productSize.clear();
														if (i == 0)
														{
															cin.ignore(1, '\n');
															i++;
														}
														page.color(7);
														getline(cin, productSize);
														if (productSize != "")
														{
															bool isProdSizeExist = prodDetails.sizeExist(getProductId(), productSize);

															if (isProdSizeExist)
															{
																isValidProductSize = true;
																setCode(prodDetails.readProdCode(getProductId(), productSize));

																string strQuantity, strDiscount;

																bool isValidQuantity = false;
																while (!isValidQuantity)
																{
																	page.color(14);
																	cout << left << setw(21) << "\nEnter Quantity" << ": ";
																	page.color(7);
																	strQuantity.clear();
																	cin >> strQuantity;
																	if (strQuantity != "")
																	{
																		bool isDigit = input.isDigit(strQuantity);
																		if (isDigit)
																		{
																			isValidQuantity = true;
																			setQuantity(stoi(strQuantity));
																		}
																		else
																			page.showInvalidChoiceMsg();
																	}
																	else
																	{
																		page.color(12);
																		cout << "\nPlease fill out this field." << endl;
																	}
																}

																bool isValidDiscount = false;

																page.color(15);
																cout << "\nAny discount?" << endl;
																page.showYesNoChoice();

																bool isValidDisChoice = false;
																string disChoice;
																while (!isValidDisChoice)
																{
																	page.color(14);
																	cout << left << setw(21) << "\nEnter choice" << ": ";
																	disChoice.clear();
																	page.color(7);
																	cin >> disChoice;

																	if (disChoice == "Y" || disChoice == "y")
																	{
																		isValidDisChoice = true;

																		bool isValidDis = false;
																		while (!isValidDis)
																		{
																			page.color(14);
																			cout << left << setw(21) << "\nEnter Discount (%)" << ": ";
																			strDiscount.clear();
																			page.color(7);
																			cin >> strDiscount;

																			if (strDiscount != "")
																			{
																				bool isDigit = input.isDigit(strDiscount);
																				if (isDigit)
																				{
																					isValidDis = true;
																					setDiscount(stoi(strDiscount));
																				}
																				else
																					page.showInvalidChoiceMsg();
																			}
																			else
																			{
																				page.color(12);
																				cout << "\nPlease fill out this field." << endl;
																			}
																		}
																	}
																	else if (disChoice == "N" || disChoice == "n")
																	{
																		isValidDisChoice = true;
																		setDiscount(0);
																	}
																	else
																		page.showInvalidChoiceMsg();
																}

																setSize(prodDetails.readProdSizeCode(getCode()));
																page.color(15);
																cout << endl;
																for (int i = 0; i < 200; i++) { cout << char(205); }cout << endl;
																page.color(7);
																cout << left << setw(17) << "\nProduct Name" << ": " << prodDetails.readProdNameId(getProductId(), getSize());
																cout << left << setw(17) << "\nProduct Code" << ": " << getCode();
																cout << left << setw(17) << "\nProduct Size" << ": " << getSize();
																cout << left << setw(17) << "\nUnit Price (RM)" << ": " << prodDetails.readProdPrice(getProductId());
																cout << left << setw(17) << "\nDiscount (%)" << ": " << getDiscount();
																cout << left << setw(17) << "\nQuantity" << ": " << getQuantity();
																page.color(15);
																cout << "\n\nAre you confirm?" << endl;
																page.showYesNoChoice();

																bool isValidConfirmManual = false;
																string confirmManual;

																while (!isValidConfirmManual)
																{
																	page.color(14);
																	cout << left << setw(21) << "\nEnter choice" << ": ";
																	confirmManual.clear();
																	page.color(7);
																	cin >> confirmManual;

																	if (confirmManual == "Y" || confirmManual == "y")
																	{	
																		isValidConfirmManual = true;
																		setResultFound(1);
																		noResult = true;																	
																		addTransactionDetails(getItemNo());
																		isInvalidProductCode = true;
																	}
																	else if (confirmManual == "N" || confirmManual == "n")
																	{

																		isValidConfirmManual = true;
																		setResultFound(1);
																		noResult = true;
																		isInvalidProductCode = true;
																	}
																	else
																		page.showInvalidChoiceMsg();
																}
															}
															else
															{
																page.color(12);
																cout << "\nNo result found." << endl;
															}

														}
														//productSize==""
														else
														{
															page.color(12);
															cout << "\nPlease fill out this field." << endl;
														}

													}
												}

												else if (confirmProd == "N" || confirmProd == "n")
												{
													isValidConfirmProd = true;
													noResult = true;						
												}
												else
													page.showInvalidChoiceMsg();
											}
										}
										else
										{
											page.color(12);
											cout << "\nNo result found." << endl;
											noResult = true;
										}
									}
									//searchProductName
									else
									{
										bool isNameExist = prodDetails.prodNameExist(searchVal);
										if (isNameExist)
											prodDetails.searchProdName(searchVal);

										else
										{
											page.color(12);
											cout << "\nNo result found." << endl;
											noResult = true;
										}
									}
								}
								//searchValue==""
								else
								{
									page.color(12);
									cout << "\nPlease fill out this field." << endl;
								}
							}
							
							if (noResult)
							{
								if(getResultFound()==0)
								{
									page.color(15);
									cout << "\nSearch Another Product?" << endl;
									page.showYesNoChoice();
									string contSearch;
									bool isValidContSearch = false;
									while (!isValidContSearch)
									{
										page.color(14);
										cout << "\nEnter choice: ";
										contSearch.clear();
										page.color(7);
										cin >> contSearch;

										if (contSearch == "Y" || contSearch == "y")
										{
											i = 0;
											system("cls");
											page.systemHeaderBlue();
											page.addtransHeader();
											showTransaction(getTransId());
											if (getA() != 0)
											{
												showTransactionDetails(getTransId());
												page.color(15);
												cout << "\nSubtotal (RM): " << readTotalPrice(getTransId());
												cout << endl;
											}
											isValidContSearch = true;
										}
										else if (contSearch == "N" || contSearch == "n")
										{
											isValidContSearch = true;
											cancelLoopManual = true;
											isInvalidProductCode = true;
										}
										else
											page.showInvalidChoiceMsg();
									}
								}
								
							}
						}
						
					}
					//searchcode
					else
					{
						int code = stoi(searchCode);
						bool isCodeExist = prodDetails.codeExist(code);
						if (isCodeExist)
						{
							int stockCount = prodDetails.readProdQuantityCode(code);
							if (stockCount == 0)
							{
								setProductId(prodDetails.readProdIdCode(code));
								prodDetails.transShowProdCodeDetails(getProductId(), code);
								page.color(12);
								cout << "\nNo stock for this product size!!" << endl;
								isInvalidProductCode = true;
							}
							else
							{
								setProductId(prodDetails.readProdIdCode(code));
								prodDetails.transShowProdCodeDetails(getProductId(), code);
								bool isValidConfirmCode = false;
								string confirmCode;
								page.color(15);
								cout << "\nConfirm item?" << endl;
								page.showYesNoChoice();

								while (!isValidConfirmCode)
								{
									page.color(14);
									cout << left << setw(15) << "\nEnter choice" << ": ";
									confirmCode.clear();
									page.color(7);
									cin >> confirmCode;

									if (confirmCode == "Y" || confirmCode == "y")
									{
										isValidConfirmCode = true;
										setCode(code);

										string strQuantity, strDiscount;

										bool isValidQuantity = false;
										while (!isValidQuantity)
										{
											page.color(14);
											cout << left << setw(15) << "\nEnter Quantity" << ": ";
											page.color(7);
											strQuantity.clear();
											cin >> strQuantity;
											if (strQuantity != "")
											{
												bool isDigit = input.isDigit(strQuantity);
												if (isDigit)
												{
													isValidQuantity = true;
													setQuantity(stoi(strQuantity));
												}
												else
													page.showInvalidChoiceMsg();
											}
											else
											{
												page.color(12);
												cout << "\nPlease fill out this field." << endl;
											}
										}

										bool isValidDiscount = false;

										page.color(15);
										cout << "\nAny discount?" << endl;
										page.showYesNoChoice();

										bool isValidDisChoice = false;
										string disChoice;
										while (!isValidDisChoice)
										{
											page.color(14);
											cout << left << setw(15) << "\nEnter choice" << ": ";
											disChoice.clear();
											page.color(7);
											cin >> disChoice;

											if (disChoice == "Y" || disChoice == "y")
											{
												isValidDisChoice = true;

												bool isValidDis = false;
												while (!isValidDis)
												{
													page.color(14);
													cout << left << setw(25) << "\nEnter Discount (%)" << ": ";
													strDiscount.clear();
													page.color(7);
													cin >> strDiscount;

													if (strDiscount != "")
													{
														bool isDigit = input.isDigit(strDiscount);
														if (isDigit)
														{
															isValidDis = true;
															setDiscount(stoi(strDiscount));
														}
														else
															page.showInvalidChoiceMsg();
													}
													else
													{
														page.color(12);
														cout << "\nPlease fill out this field." << endl;
													}
												}
											}
											else if (disChoice == "N" || disChoice == "n")
											{
												isValidDisChoice = true;
												setDiscount(0);
											}
											else
												page.showInvalidChoiceMsg();
										}

										setSize(prodDetails.readProdSizeCode(getCode()));
										page.color(15);
										cout << endl;
										for (int i = 0; i < 200; i++) { cout << char(205); }cout << endl;
										page.color(7);
										cout << left << setw(17) << "\nProduct Name" << ": " << prodDetails.readProdNameId(getProductId(), getSize());
										cout << left << setw(17) << "\nProduct Code" << ": " << getCode();
										cout << left << setw(17) << "\nProduct Size" << ": " << getSize();
										cout << left << setw(17) << "\nUnit Price (RM)" << ": " << prodDetails.readProdPrice(getProductId());
										cout << left << setw(17) << "\nDiscount (%)" << ": " << getDiscount();
										cout << left << setw(17) << "\nQuantity" << ": " << getQuantity();


										page.color(15);
										cout << "\n\nAre you confirm?" << endl;
										page.showYesNoChoice();

										bool isValidConfirmManual = false;
										string confirmManual;

										while (!isValidConfirmManual)
										{
											page.color(14);
											cout << "\nEnter choice: ";
											confirmManual.clear();
											page.color(7);
											cin >> confirmManual;

											if (confirmManual == "Y" || confirmManual == "y")
											{
												isValidConfirmManual = true;
												addTransactionDetails(getItemNo());
												isInvalidProductCode = true;
											}
											else if (confirmManual == "N" || confirmManual == "n")
											{
												isValidConfirmManual = true;
												isInvalidProductCode = true;
											}
											else
												page.showInvalidChoiceMsg();
										}
									}
									else if (confirmCode == "N" || confirmCode == "n")
									{
										isValidConfirmCode = true;
										isInvalidProductCode = true;
									}
									else
										page.showInvalidChoiceMsg();
								}
							}							
						}
						else
						{
							page.color(12);
							cout << "\nNo result found." << endl;
							page.color(7);
							cout << endl;
							system("pause");
							isInvalidProductCode = true;
						}
					}
				}
				else
					page.showInvalidChoiceMsg();
			}
			//searchCode==""
			else
			{
				page.color(12);
				cout << "\nPlease fill out this field." << endl;
			}
		}

		if (isInvalidProductCode)
		{
			bool cancelLoopPanel = false;
			while (!cancelLoopPanel)
			{
				system("cls");
				page.systemHeaderBlue();
				page.addtransHeader();
				showTransaction(getTransId());
				if (getA() != 0)
				{
					showTransactionDetails(getTransId());
					page.color(15);
					cout << "\nSubtotal (RM): " << readTotalPrice(getTransId());
					cout << endl;
				}

				cout << endl;
				page.color(15);
				cout << "\nPlease select a choice." << endl;
				page.color(11);
				cout << setw(5) << right << char(201);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(187) << endl;
				cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
				cout << setw(5) << right << char(186) << setw(51) << left << "  1 - Add Another Item" << char(186) << endl;
				cout << setw(5) << right << char(186) << setw(34) << left << "  2 - Delete item in Transaction #" << setw(17) << getTransId() << char(186) << endl;
				cout << setw(5) << right << char(186) << setw(22) << left << "  3 - End Transaction #" << setw(28) << getTransId() << char(186) << endl;
				cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
				cout << setw(5) << right << char(200);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(188) << endl;
				bool isValidAddItem = false;
				string addItem;
				while (!isValidAddItem)
				{
					page.color(14);
					cout << "\nEnter choice: ";
					addItem.clear();
					page.color(7);
					cin >> addItem;

					//Add another item
					if (addItem == "1")
					{
						isValidAddItem = true;
						cancelLoopPanel = true;
					}
					//Delete item
					else if (addItem == "2")
					{
						isValidAddItem = true;
						if (getA() != 0)
						{
							system("cls");
							page.systemHeaderBlue();
							page.addtransHeader();

							showTransaction(getTransId());
							showTransactionDetails(getTransId()); //Final PurchaseOrder
							page.color(15);
							cout << "\nSubtotal (RM): " << readTotalPrice(getTransId());
							cout << endl;

							string strItemNo;

							page.color(14);
							cout << "\nEnter item number to delete: ";
							page.color(7);
							strItemNo.clear();
							cin >> strItemNo;

							bool isDigit = input.isDigit(strItemNo);

							if (isDigit)
							{
								int itemNo = stoi(strItemNo);
								bool isItemNoExist = itemNoExistInTrans(getTransId(), itemNo);
								if (isItemNoExist)
								{
									TransactionDetail* td = TransactionDetail::gett(getTransId(), itemNo);
									td->displayDetails();

									bool isValidConfirmDelete = false;
									string confirmDelete;
									page.color(15);
									cout << "\nAre you confirm?" << endl;
									page.showYesNoChoice();

									while (!isValidConfirmDelete)
									{
										page.color(14);
										cout << "\nEnter choice: ";
										confirmDelete.clear();
										page.color(7);
										cin >> confirmDelete;

										if (confirmDelete == "Y" || confirmDelete == "y")
										{
											isValidConfirmDelete = true;
											setDeleteQuantity(readDeleteQuantity(getTransId(), itemNo));
											setCode(readDeleteCode(getTransId(), itemNo));
											td->erase();
											addProdQuantityInProductStock(getCode(), getDeleteQuantity());

											page.color(15);
											cout << "\nItem No." << itemNo << " successfully deleted from Transaction #" << getTransId() << endl;
											cout << endl;
											page.color(7);
											system("pause");
										}
										else if (confirmDelete == "N" || confirmDelete == "n")
										{
											isValidConfirmDelete = true;

										}
										else
											page.showInvalidChoiceMsg();
									}
								}
								else
								{
									page.color(12);
									cout << "\nNo result found." << endl;
								}
							}
							else
								page.showInvalidChoiceMsg();

						}
						else
						{
							page.color(12);
							cout << "\nNo item in Transaction #" << getTransId() << endl;
							cout << endl;
							page.color(7);
							system("pause");
						}
					}
					//End transaction
					else if (addItem == "3")
					{
						isValidAddItem = true;
						cancelLoopPanel = true;
						cancelLoopAddItem = true;
						system("cls");
						page.systemHeaderBlue();
						page.addtransHeader();

						showTransaction(getTransId());
						if (getA() != 0)
						{
							showTransactionDetails(getTransId());
							page.color(15);
							setSubTotal(readTotalPrice(getTransId()));
							cout << endl;
							cout << left << setw(22) << "\nTotal Payment (RM)" << ": " << getTotal() << endl;

							bool isValidCash = false;
							string strCash;
							while (!isValidCash)
							{
								page.color(14);
								cout << left << setw(22) << "\nCash Tendered" << ": ";
								page.color(7);
								cin >> strCash;

								bool isDigit = input.isDigit(strCash);
								if (isDigit)
								{
									double cashTendered = stod(strCash);

									if (getTotal() <= cashTendered)
									{
										isValidCash = true;
										isValidAddItem = true;
										cancelLoopAddItem = true;
										system("cls");
										page.systemHeaderBlue();
										page.addtransHeader();
										showTransaction(getTransId());
										showTransactionDetails(getTransId());
										page.color(15);
										setSubTotal(readTotalPrice(getTransId()));
										cout << endl;
										cout << left << setw(22) << "\nTotal Payment (RM)" << ": " << getTotal();
										cout << left << setw(22) << "\nCash Tendered (RM)" << ": " << fixed << showpoint << setprecision(2) << cashTendered;
										cout << left << setw(22) << "\nChange Due (RM)" << ": " << fixed << showpoint << setprecision(2) << cashTendered - getTotal() << endl;
									}
									else
									{
										page.color(12);
										cout << "\nInvalid amount!" << endl;
									}
								}
								else
									page.showInvalidChoiceMsg();
							}
							cout << endl;
						}
						else
						{
							cout << "\n\"Add Transaction\" action cancelled." << endl;
							deleteTransOrderWithoutDet(getTransId());
						}

						cout << endl;
						page.color(7);
						system("pause");
						return true;
					}
					else
						page.showInvalidChoiceMsg();
				}
			}		
		}
	}
}
void TransactionController::addTransactionDetails(int number)
{
	number++;
	setItemNo(number);
	TransactionDetail* td = new TransactionDetail();
	td->setItemNumber(number);
	td->setCode(getCode());
	td->setQuantity(getQuantity());
	td->setDiscount(getDiscount());
	td->setTransactionId(getTransId());
	td->save();

	reduceQuantityInProdStock(getCode(), getQuantity());
	setA(1);
}
void TransactionController::reduceQuantityInProdStock(int productcode, int quantity)
{
	ProductStock* ps = ProductStock::gett(productcode);
	if (ps != nullptr)
	{
		ps->setQuantity(ps->getQuantity() - quantity);
		ps->save();
	}
}

bool TransactionController::itemNoExistInTrans(int transId, int itemno)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("select EXISTS (select * from transaction_details where transaction_id = ? and item_number = ?)");

	ps->setInt(1, transId);
	ps->setInt(2, itemno);

	ResultSet* rs = ps->executeQuery();
	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}
int TransactionController::readDeleteQuantity(int transId, int itemNo)
{
	string str = "select quantity from transaction_details where transaction_id = ? and item_number = ?";

	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, transId);
	ps->setInt(2, itemNo);
	ResultSet* result = ps->executeQuery();

	result->next();
	return (result->getInt("quantity"));
}
int TransactionController::readDeleteCode(int transId, int itemNo)
{
	string str = "select `code` from transaction_details where transaction_id = ? and item_number = ?";

	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, transId);
	ps->setInt(2, itemNo);
	ResultSet* result = ps->executeQuery();

	result->next();
	return (result->getInt("code"));
}
void TransactionController::addProdQuantityInProductStock(int productcode, int quantity)
{
	ProductStock* ps = ProductStock::gett(productcode);
	if (ps != nullptr)
	{
		ps->setQuantity(ps->getQuantity() + quantity);
		ps->save();
	}
}

void TransactionController::showTransaction(int transId)
{
	string str = "select transactions.id, transactions.employee_id, employees.`name`, transactions.created_at from transactions inner join employees"
		+ std::string(" on (employee_id = employees.id) where transactions.id = ?;");

	page.color(15);
	cout << "\nShowing details of Transaction #" << transId << endl;

	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, transId);
	ResultSet* result = ps->executeQuery();

	for (int i = 0; i < result->rowsCount(); i++)
	{
		result->next();
		cout << left << setw(15) << "\nDate" << ": " << result->getString("created_at");
		cout << left << setw(15) << "\nCashier" << ": " << result->getInt("employee_id") << " - " << result->getString("name");
	}
	cout << endl;
}
void TransactionController::showTransactionDetails(int transId)
{
	string str = "select item_number, transaction_details.`code`,`name`,colour, product_stocks.size, transaction_details.quantity, unit_price, discount,  "
		+ std::string("(transaction_details.quantity * (unit_price - (unit_price * discount / 100))) AS subtotal from transaction_details inner join transactions on(transaction_id = transactions.id)")
		+ "inner join product_stocks on(transaction_details.`code`=product_stocks.`code`) inner join products on(product_stocks.product_id = products.id) "
		+ "where transaction_id = ? order by item_number;";

	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, transId);
	ResultSet* result = ps->executeQuery();

	cout << endl;
	cout << left << setw(8) << "Item No." << setw(4) << "" << left << setw(7) << "Code" << setw(4) << "" << left << setw(70) << "Product";
	cout << setw(4) << " " << left << setw(12) << "Colour" << setw(4) << " " << left << setw(10) << "Size" << setw(4) << " ";
	cout << left << setw(10) << "Unit Price" << setw(4) << " " << left << setw(13) << "Discount (%)" << setw(4) << " ";
	cout << left << setw(8) << "Quantity" << setw(4) << " " << "SubTotal (RM)" << endl;

	page.color(7);
	for (int i = 0; i < result->rowsCount(); i++)
	{
		result->next();
		cout << left << setw(8) << result->getInt("item_number") << setw(4) << "" << left << setw(7) << result->getInt("code") << setw(4) << "" << left << setw(70) << result->getString("name");
		cout << setw(4) << " " << left << setw(12) << result->getString("colour") << setw(4) << " " << left << setw(10) << result->getString("size") << setw(4) << " ";
		cout << left << setw(10) << fixed << showpoint << setprecision(2) << result->getDouble("unit_price") << setw(4) << " " << left << setw(13) << result->getInt("discount") << setw(4);
		cout << " " << left << setw(8) << result->getInt("quantity") << setw(4) << " " << fixed << showpoint << setprecision(2) << result->getDouble("subtotal") << endl;
	}
}
double TransactionController::readTotalPrice(int transId)
{
	string str = "select sum(transaction_details.quantity*(unit_price -(unit_price*discount/100))) AS total "
		+ std::string("from transaction_details inner join transactions on(transaction_id = transactions.id)")
		+ "inner join product_stocks on(transaction_details.`code`=product_stocks.`code`) inner join products on(product_stocks.product_id = products.id) "
		+ "where transaction_id = ? order by item_number;";

	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, transId);
	ResultSet* result = ps->executeQuery();

	result->next();
	return (result->getDouble("total"));
}

void TransactionController::setSubTotal(double n) {
	subtotal = n;
}
double TransactionController::getTotal() {
	return subtotal;
}

void TransactionController::deleteTransOrderWithoutDet(int transId)
{
	bool isIdDetExist = idExistTransDet(transId);
	if (!isIdDetExist)
	{
		Transaction* t = Transaction::get(transId);
		t->erase();
	}
}
bool TransactionController::idExistTransDet(int id)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("SELECT EXISTS (SELECT * FROM transaction_details WHERE transaction_id = ?)");

	ps->setInt(1, id);

	ResultSet* rs = ps->executeQuery();
	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}

void TransactionController::setA(int n) {
	a = n;
}
int TransactionController::getA() {
	return a;
}
void TransactionController::setResultFound(int n) {
	resultNotFound = n;
}
int TransactionController::getResultFound() {
	return resultNotFound;
}
void TransactionController::setItemNo(int n) {
	itemNo = n;
}
int TransactionController::getItemNo() {
	return itemNo;
}

void TransactionController::setDeleteQuantity(int n) {
	deleteQuantity = n;
}
int TransactionController::getDeleteQuantity() {
	return deleteQuantity;
}

void TransactionController::setEmployeeLoginId(int n) {
	employeeLoginId = n;
}
int TransactionController::getEmployeeLoginId() {
	return employeeLoginId;
}
void TransactionController::setTransId(int n) {
	transactionId = n;
}
int TransactionController::getTransId() {
	return transactionId;
}

void TransactionController::setProductId(int n) {
	productId = n;
}
int TransactionController::getProductId() {
	return productId;
}
void TransactionController::setCode(int n) {
	code = n;
}
int TransactionController::getCode() {
	return code;
}
void TransactionController::setSize(string n) {
	size = n;
}
string TransactionController::getSize() {
	return size;
}
void TransactionController::setQuantity(int n) {
	quantity = n;
}
int TransactionController::getQuantity() {
	return quantity;
}
void TransactionController::setDiscount(int n) {
	discount = n;
}
int TransactionController::getDiscount() {
	return discount;
}

//view Transaction
bool TransactionController::idExistTrans(int id)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("SELECT EXISTS (SELECT * FROM transactions WHERE id = ?)");

	ps->setInt(1, id);

	ResultSet* rs = ps->executeQuery();
	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}
void TransactionController::viewDetails(int transId)
{
	showTransaction(transId);
	showTransactionDetails(transId); 
	page.color(15);
	cout << "\n\nTotal Payment (RM): " << fixed << showpoint << setprecision(2) << readTotalPrice(transId);
	cout << endl;
}

void TransactionController::manageRefund(int transId)
{
	bool quitRefund = false;
	while (!quitRefund)
	{
		bool isInvalidRefund = false;
		system("cls");
		page.systemHeaderOrange();
		page.transMngmtHeader();
		viewDetails(transId);

		while (!isInvalidRefund)
		{
			string strItemNo;
			page.color(14);
			cout << "\nEnter refund item's item number in Transaction #" << transId << ": ";
			page.color(7);
			strItemNo.clear();
			cin >> strItemNo;

			bool isDigit = input.isDigit(strItemNo);
			if (isDigit)
			{
				int itemNo = stoi(strItemNo);
				bool isItemNoExist = itemNoExistInTrans(transId, itemNo);
				if (isItemNoExist)
				{
					TransactionDetail* td = TransactionDetail::gett(transId, itemNo);
					td->displayDetails();

					bool isValidConfirmItemNo = false;
					string confirmItemNo;
					page.color(15);
					cout << "\nAre you confirm?" << endl;
					page.showYesNoChoice();

					while (!isValidConfirmItemNo)
					{
						page.color(14);
						cout << "\nEnter choice: ";
						confirmItemNo.clear();
						page.color(7);
						cin >> confirmItemNo;

						if (confirmItemNo == "Y" || confirmItemNo == "y")
						{
							isValidConfirmItemNo = true;
							int oriQuantity = readDeleteQuantity(transId, itemNo);

							string strRefundQuantity;
							bool validRQuantity = false;
							while (!validRQuantity)
							{
								page.color(14);
								cout << "\nEnter refund item's quantity: ";
								page.color(7);
								strRefundQuantity.clear();
								cin >> strRefundQuantity;

								if (stoi(strRefundQuantity) > oriQuantity)
								{
									page.color(12);
									cout << "\nInvalid quantity!!" << endl;
								}
								else
								{
									validRQuantity = true;
								}
							}
													
							bool isValidConfirmQuantity = false;
							string confirmQuantity;
							page.color(15);
							cout << "\nAre you confirm?" << endl;
							page.showYesNoChoice();

							while (!isValidConfirmQuantity)
							{
								page.color(14);
								cout << "\nEnter choice: ";
								confirmQuantity.clear();
								page.color(7);
								cin >> confirmQuantity;

								if (confirmQuantity == "Y" || confirmQuantity == "y")
								{
									isValidConfirmQuantity = true;
									int refundQuantity = stoi(strRefundQuantity);
									setDeleteQuantity(refundQuantity);
									setCode(readDeleteCode(transId, itemNo));
									
									if (refundQuantity == oriQuantity)
									{
										td->erase();
										page.color(15);
										cout << "\nItem No." << itemNo << " successfully deleted from Transaction #" << transId << endl;
									}
									else
									{
										td->setQuantity(td->getQuantity() - refundQuantity);
										td->save();
										page.color(15);
										cout << "\nItem No." << itemNo << " 's quantity successfully reduced." << endl;
									}

									addProdQuantityInProductStock(getCode(), getDeleteQuantity());
									isInvalidRefund = true;
									cout << endl;
									page.color(7);
									system("pause");
									system("cls");
									page.systemHeaderOrange();
									page.transMngmtHeader();
									viewDetails(transId);

								}
								else if (confirmQuantity == "N" || confirmQuantity == "n")
								{
									isValidConfirmQuantity = true;
									isInvalidRefund = true;
								}
								else
									page.showInvalidChoiceMsg();
							}

						}
						else if (confirmItemNo == "N" || confirmItemNo == "n")
						{
							isValidConfirmItemNo = true;
							isInvalidRefund = true;
						}
						else
							page.showInvalidChoiceMsg();
					}
				}
				else
				{
					page.color(12);
					cout << "\nNo result found." << endl;
				}
			}
			else
				page.showInvalidChoiceMsg();
		}
		
		if (isInvalidRefund)
		{
			cout << endl;
			page.color(15);
			cout << "\nContinue refund action?" << endl;
			page.showYesNoChoice();

			bool isValidCont = false;
			string contAction;
			while (!isValidCont)
			{
				page.color(14);
				cout << "\nEnter choice: ";
				contAction.clear();
				page.color(7);
				cin >> contAction;

				if (contAction == "Y" || contAction == "y")
				{
					isValidCont = true;
				}
				else if (contAction == "N" || contAction == "n")
				{
					isValidCont = true;
					quitRefund = true;				
				}
			}
		}
		
	}	
}

//User
void TransactionController::viewSearchTransUser(int empId)
{
	bool isBack = false;
	while (!isBack)
	{
		bool isStopLoop = false;

		int m = 0, a = 0;
		int id = 0;
		string searchChoice;
		string searchVal = "";
		bool isDateFound = false;

		while (!isStopLoop)
		{
			bool isValidSearchChoice = false;

			if (isDateFound)
			{
				system("cls");
				page.systemHeaderBlue();
				page.transSearchViewHeaderUser();
				viewTransListDateUser(empId, searchVal, a);
			}
			else
			{
				system("cls");
				page.systemHeaderBlue();
				page.transSearchViewHeaderUser();
				viewTransListUser(empId, m);
			}

			cout << endl;
			page.transSearchViewPanelUser();

			while (!isValidSearchChoice)
			{
				page.color(14);
				cout << "\nEnter choice: ";
				page.color(7);
				searchChoice.clear();
				cin >> searchChoice;

				//Previous Page
				if (searchChoice == "1")
				{
					isValidSearchChoice = true;

					if (isDateFound)
					{
						if (a == 0)
						{
							page.color(12);
							cout << "\nThis is the first page.\n" << endl;
							page.color(7);
							system("pause");
						}
						else
							a = a - 10;
					}
					else
					{
						if (m == 0)
						{
							page.color(12);
							cout << "\nThis is the first page.\n" << endl;
							page.color(7);
							system("pause");
						}
						else
							m = m - 10;
					}
				}

				//Next Page
				else if (searchChoice == "2")
				{
					isValidSearchChoice = true;
					if (isDateFound)
					{
						if (a + 10 >= countSearchDateUser(empId, searchVal))
						{
							page.color(12);
							cout << "\nThis is the last page.\n" << endl;
							page.color(7);
							system("pause");
						}
						else
							a = a + 10;
					}
					else
					{
						if (m + 10 >= countTransListUser(empId))
						{
							page.color(12);
							cout << "\nThis is the last page.\n" << endl;
							page.color(7);
							system("pause");
						}
						else
							m = m + 10;
					}
				}

				//Search Previous Transaction Made
				else if (searchChoice == "3")
				{
					isValidSearchChoice = true;
					a = 0;
					isDateFound = false;

					int i = 0;
					
					page.showSearchDateFormat();

					bool isValidDate = false;
					while (!isValidDate)
					{
						page.color(14);
						cout << "\nEnter date to search: ";
						page.color(7);

						searchVal.clear();
						if (i == 0)
						{
							cin.ignore(1, '\n');
							i++;
						}
						getline(cin, searchVal);

						if (searchVal != "")
						{
							isValidDate = true;
							bool isDateExist = dateExistUser(empId, searchVal);
							if (isDateExist)
								isDateFound = true;
							else
							{
								page.color(12);
								cout << "\nNo result found.\n" << endl;
								page.color(7);
								system("pause");
							}
						}
						else
						{
							page.color(12);
							cout << "\nPlease fill out this field." << endl;
						}						
					}					
				}

				//View Transaction Details
				else if (searchChoice == "4")
				{
					isValidSearchChoice = true;
					string strId;
					bool isDigit = false;
					bool isValidStrId = false;

					while (!isValidStrId)
					{
						page.color(14);
						cout << "\nEnter Transaction ID: ";
						strId.clear();
						page.color(7);
						cin >> strId;

						isDigit = input.isDigit(strId);
						if (isDigit)
						{
							isValidStrId = true;
							int id = stoi(strId);
							bool isIdExist = idExistTrans(id);
							if (isIdExist)
							{
								system("cls");
								page.systemHeaderBlue();
								page.transSearchViewHeaderUser();

								viewDetails(id);
								
								cout << endl;
								page.color(7);
								system("pause");
							}
							//ID not found
							else
							{
								page.color(12);
								cout << "\nNo result found.\n" << endl;
								page.color(7);
								system("pause");
							}
						}
						//Invalid ID input (not digit)
						else
							page.showInvalidChoiceMsg();
					}
				}

				//Back
				else if (searchChoice == "5")
				{
					if (isDateFound)
					{
						isValidSearchChoice = true;
						isStopLoop = true;
					}
					else
					{
						isValidSearchChoice = true;
						isStopLoop = true;
						isBack = true;
					}
				}

				//Invalid Input
				else
					page.showInvalidChoiceMsg();
			}
		}
	}
}

int TransactionController::countTransListUser(int empId)
{
	string str = "select COUNT(DISTINCT transaction_id) AS count from transaction_details inner JOIN transactions on(transaction_id = transactions.id) "
		+ std::string("inner join employees on(transactions.employee_id = employees.id) inner join product_stocks on(transaction_details.`code`=product_stocks.`code`) ")
		+ "inner join products on (products.id = product_id)where employees.id = ? AND transactions.created_at LIKE ? ";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, empId);
	ps->setString(2, "%" + Database::getDateNow() + "%");
	ResultSet* result = ps->executeQuery();
	result->next();
	return (int)result->getInt64("count");
}
void TransactionController::viewTransListUser(int empId, const int m)
{
	int n = m;

	int totalCount = countTransListUser(empId);

	if (totalCount == 0)
	{
		page.color(15);
		cout << "\nNumber of transactions made today: " << totalCount << endl;
	}
	else
	{
		string str = "select transaction_id, employee_id, employees.`name`,transactions.created_at AS date, sum(transaction_details.quantity * (unit_price - (unit_price * discount / 100))) "
			+ std::string("AS totalSales, sum(transaction_details.quantity) AS itemSold from transaction_details inner JOIN transactions on(transaction_id = transactions.id)")
			+ "inner join employees on(transactions.employee_id = employees.id) inner join product_stocks on(transaction_details.`code`=product_stocks.`code`) "
			+ "inner join products on(products.id = product_id) where employees.id = ? AND transactions.created_at LIKE  ? group by transaction_id order by transaction_id LIMIT 10 offset "
			+ to_string(m);

		PreparedStatement* ps = Database::prepareStatement(str);
		ps->setInt(1, empId);
		ps->setString(2, "%" + Database::getDateNow() + "%");
		ResultSet* result = ps->executeQuery();

		int SIZE = ((int)result->rowsCount() < totalCount) ? (int)result->rowsCount() : totalCount;

		page.color(15);
		cout << "\nNumber of transactions made today: " << totalCount << endl;

		cout << "\nShowing " << n + 1;

		if (m + 10 <= totalCount)
			cout << "~" << n + 10 << " out of " << totalCount << " results.\n" << endl;
		else
			cout << "~" << totalCount << " out of " << totalCount << " results.\n" << endl;

		page.color(7);
		cout << right << setw(5) << "No." << setw(5) << "ID" << setw(4) << " " << left << setw(19) << "Date" << setw(4) << " ";
		cout << left << setw(10) << "Item Count" << setw(4) << " " << left << setw(10) << "Sales (RM)" << endl;

		for (int i = 0; i < SIZE; i++)
		{
			result->next();
			cout << right << setw(5) << n + 1 << setw(5) << result->getInt("transaction_id") << setw(4) << " " << left << setw(19) << result->getString("date") << setw(4) << " ";
			cout << left << setw(10) << result->getInt("itemSold") << setw(4) << " " << right << setw(10) << fixed << showpoint << setprecision(2) << result->getDouble("totalSales") << endl;
			n++;
		}
	}
}

int TransactionController::countSearchDateUser(int empId, string search)
{
	string str = "select COUNT(DISTINCT transaction_id) AS count from transaction_details inner JOIN transactions on(transaction_id = transactions.id) "
		+ std::string("inner join employees on(transactions.employee_id = employees.id) inner join product_stocks on(transaction_details.`code`=product_stocks.`code`) ")
		+ "inner join products on (products.id = product_id) where employees.id = ? AND transactions.created_at LIKE ? ";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, empId);
	ps->setString(2, "%" + search + "%");
	ResultSet* result = ps->executeQuery();
	result->next();
	return (int)result->getInt64("count");
}
void TransactionController::viewTransListDateUser(int empId, string searchVal, const int m)
{
	int n = m;

	int totalCount = countSearchDateUser(empId, searchVal);

	if (totalCount == 0)
	{
		page.color(15);
		cout << "\nNumber of transactions made: " << totalCount << endl;
	}
	else
	{
		string str = "select transaction_id, employee_id, employees.`name`,transactions.created_at AS date, sum(transaction_details.quantity * (unit_price - (unit_price * discount / 100))) "
			+ std::string("AS totalSales, sum(transaction_details.quantity) AS itemSold from transaction_details inner JOIN transactions on(transaction_id = transactions.id)")
			+ "inner join employees on(transactions.employee_id = employees.id) inner join product_stocks on(transaction_details.`code`=product_stocks.`code`) "
			+ "inner join products on(products.id = product_id) where employees.id = ? AND transactions.created_at LIKE  ? group by transaction_id order by transaction_id LIMIT 10 offset "
			+ to_string(m);

		PreparedStatement* ps = Database::prepareStatement(str);
		ps->setInt(1, empId);
		ps->setString(2, "%" + searchVal + "%");
		ResultSet* result = ps->executeQuery();

		int SIZE = ((int)result->rowsCount() < totalCount) ? (int)result->rowsCount() : totalCount;

		page.color(15);
		cout << "\nNumber of transactions made: " << totalCount << endl;

		cout << "\nShowing " << n + 1;

		if (m + 10 <= totalCount)
			cout << "~" << n + 10 << " out of " << totalCount << " results for \"" << searchVal << "\".\n" << endl;
		else
			cout << "~" << totalCount << " out of " << totalCount << " results for \"" << searchVal << "\".\n" << endl;

		page.color(7);
		cout << right << setw(5) << "No." << setw(5) << "ID" << setw(4) << " " << left << setw(19) << "Date" << setw(4) << " ";
		cout << left << setw(10) << "Item Count" << setw(4) << " " << left << setw(10) << "Sales (RM)" << endl;

		for (int i = 0; i < SIZE; i++)
		{
			result->next();
			cout << right << setw(5) << n + 1 << setw(5) << result->getInt("transaction_id") << setw(4) << " " << left << setw(19) << result->getString("date") << setw(4) << " ";
			cout << left << setw(10) << result->getInt("itemSold") << setw(4) << " " << right << setw(10) << fixed << showpoint << setprecision(2) << result->getDouble("totalSales") << endl;
			n++;
		}
	}
}
bool TransactionController::dateExistUser(int empId, string date)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("select exists (select * from transactions inner JOIN transaction_details on(transaction_id = transactions.id) inner join employees on (employee_id = employees.id) where employees.id = ? and transactions.created_at like ? group by transactions.id)");
	
	ps->setInt(1, empId);
	ps->setString(2, "%" + date + "%");

	ResultSet* rs = ps->executeQuery();
	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}

