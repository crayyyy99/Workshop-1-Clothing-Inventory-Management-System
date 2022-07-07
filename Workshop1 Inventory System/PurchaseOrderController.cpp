#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <iomanip>
#include <conio.h>
#include <cstdlib> 
#include <ctime> 

using namespace std;

#include <mysql/jdbc.h>
using namespace sql;

#include "PurchaseOrderController.h"
#include "SupplierController.h"
#include "ProductController.h"
#include "DatabaseConnection.h"
#include "PurchaseOrder.h"
#include "PurchaseOrderDetail.h"
#include "IndexPage.h"
#include "CheckInput.h"

void PurchaseOrderController::viewPurOrderList(const int m)
{
	int n = m;

	int totalCount = PurchaseOrder::count();

	if (totalCount == 0)
	{
		page.color(15);
		cout << "\nNumber of purchase orders: " << totalCount << endl;
	}
	else
	{
		string str = "select purchase_orders.id, supplier_id, suppliers.company_name, employee_id, employees.`name`,purchase_orders.created_at AS date, "
			+ std::string("sum(purchasing_price * quantity) AS total from purchase_orders inner JOIN purchase_order_details on(purchase_order_id = purchase_orders.id) ")
			+ "inner join suppliers on(supplier_id = suppliers.id) inner join employees on(employee_id = employees.id) group by purchase_order_id LIMIT 10 offset "
			+ to_string(m);

		PreparedStatement* ps = Database::prepareStatement(str);
		ResultSet* result = ps->executeQuery();

		int SIZE = ((int)result->rowsCount() < totalCount) ? (int)result->rowsCount() : totalCount;

		page.color(15);
		cout << "\nNumber of purchase orders: " << totalCount << endl;

		cout << "\nShowing " << n + 1;

		if (m + 10 <= totalCount)
			cout << "~" << n + 10 << " out of " << totalCount << " results.\n" << endl;
		else
			cout << "~" << totalCount << " out of " << totalCount << " results.\n" << endl;

		page.color(7);
		cout << right << setw(5) << "No." << setw(5) << "ID" << setw(4) << " " << left << setw(19) << "Date" << setw(4) << " " << left << setw(56) << "Supplier" << setw(4) << " ";
		cout << left << setw(56) << "Employee" << setw(4) << " " << "Total Payment (RM)" << endl;

		for (int i = 0; i < SIZE; i++)
		{
			result->next();
			cout << right << setw(5) << n + 1 << setw(5) << result->getInt("id") << setw(4) << " " << left << setw(19) << result->getString("date");
			cout << setw(4) << " " << setw(3) << left << result->getInt("supplier_id") <<" - "<< left << setw(50) << result->getString("company_name");
			cout << setw(4) << " " << setw(3) << left << result->getInt("employee_id") << " - " << left << setw(50) << result->getString("name") << setw(4) << " ";
			cout << right << setw(18) << fixed << showpoint << setprecision(2) << result->getDouble("total") << endl;
			n++;
		}
	}
}

int PurchaseOrderController::countSearchDate(string search)
{
	string str = "select COUNT(DISTINCT purchase_orders.id) AS count from purchase_orders where purchase_orders.created_at like '%" + search + "%'";
	PreparedStatement* ps = Database::prepareStatement(str);
	ResultSet* result = ps->executeQuery();
	result->next();
	return (int)result->getInt64("count");
}
void PurchaseOrderController::viewPurOrderListDate(string searchVal, const int m)
{
	int n = m;

	int totalCount = countSearchDate(searchVal);

	if (totalCount == 0)
	{
		page.color(15);
		cout << "\nNumber of purchase orders: " << totalCount << endl;
	}
	else
	{
		string str = "select purchase_orders.id, supplier_id, suppliers.company_name, employee_id, employees.`name`,purchase_orders.created_at AS date, "
			+ std::string("sum(purchasing_price * quantity) AS total from purchase_orders inner JOIN purchase_order_details on(purchase_order_id = purchase_orders.id) ")
			+ "inner join suppliers on(supplier_id = suppliers.id) inner join employees on(employee_id = employees.id) where purchase_orders.created_at LIKE ? "
			+ "group by purchase_order_id LIMIT 10 offset "
			+ to_string(m);

		PreparedStatement* ps = Database::prepareStatement(str);
		ps->setString(1, "%" + searchVal + "%");
		ResultSet* result = ps->executeQuery();

		int SIZE = ((int)result->rowsCount() < totalCount) ? (int)result->rowsCount() : totalCount;

		page.color(15);
		cout << "\nNumber of purchase orders: " << totalCount << endl;

		cout << "\nShowing " << n + 1;

		if (m + 10 <= totalCount)
			cout << "~" << n + 10 << " out of " << totalCount << " results for \"" << searchVal << "\".\n" << endl;
		else
			cout << "~" << totalCount << " out of " << totalCount << " results for \"" << searchVal << "\".\n" << endl;

		page.color(7);
		cout << right << setw(5) << "No." << setw(5) << "ID" << setw(4) << " " << left << setw(19) << "Date" << setw(4) << " " << left << setw(56) << "Supplier" << setw(4) << " ";
		cout << left << setw(56) << "Employee" << setw(4) << " " << "Total Payment (RM)" << endl;

		for (int i = 0; i < SIZE; i++)
		{
			result->next();
			cout << right << setw(5) << n + 1 << setw(5) << result->getInt("id") << setw(4) << " " << left << setw(19) << result->getString("date");
			cout << setw(4) << " " << setw(3) << left << result->getInt("supplier_id") << " - " << left << setw(50) << result->getString("company_name");
			cout << setw(4) << " " << setw(3) << left << result->getInt("employee_id") << " - " << left << setw(50) << result->getString("name") << setw(4) << " ";
			cout << right << setw(18) << fixed << showpoint << setprecision(2) << result->getDouble("total") << endl;
			n++;
		}
	}
}
bool PurchaseOrderController::dateExist(string date)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("SELECT EXISTS (SELECT * FROM purchase_orders WHERE created_at LIKE ?)");

	ps->setString(1, "%" + date + "%");

	ResultSet* rs = ps->executeQuery();
	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}

int PurchaseOrderController::countSearchEmployee(string search)
{
	string str = "select COUNT(DISTINCT purchase_orders.id) AS count from purchase_orders inner JOIN purchase_order_details on(purchase_order_id = purchase_orders.id)"
		+ std::string("inner join suppliers on(supplier_id = suppliers.id) inner join employees on(employee_id = employees.id) where name like '%" + search + "%' ");
	PreparedStatement* ps = Database::prepareStatement(str);
	ResultSet* result = ps->executeQuery();
	result->next();
	return (int)result->getInt64("count");
}
void PurchaseOrderController::viewPurOrderListEmployee(string searchVal, const int m)
{
	int n = m;

	int totalCount = countSearchEmployee(searchVal);

	if (totalCount == 0)
	{
		page.color(15);
		cout << "\nNumber of purchase orders: " << totalCount << endl;
	}
	else
	{
		string str = "select purchase_orders.id, supplier_id, suppliers.company_name, employee_id, employees.`name`,purchase_orders.created_at AS date, "
			+ std::string("sum(purchasing_price * quantity) AS total from purchase_orders inner JOIN purchase_order_details on(purchase_order_id = purchase_orders.id) ")
			+ "inner join suppliers on(supplier_id = suppliers.id) inner join employees on(employee_id = employees.id) where name LIKE ? "
			+ "group by purchase_order_id LIMIT 10 offset "
			+ to_string(m);

		PreparedStatement* ps = Database::prepareStatement(str);
		ps->setString(1, "%" + searchVal + "%");
		ResultSet* result = ps->executeQuery();

		int SIZE = ((int)result->rowsCount() < totalCount) ? (int)result->rowsCount() : totalCount;

		page.color(15);
		cout << "\nNumber of purchase orders: " << totalCount << endl;

		cout << "\nShowing " << n + 1;

		if (m + 10 <= totalCount)
			cout << "~" << n + 10 << " out of " << totalCount << " results for \"" << searchVal << "\".\n" << endl;
		else
			cout << "~" << totalCount << " out of " << totalCount << " results for \"" << searchVal << "\".\n" << endl;

		page.color(7);
		cout << right << setw(5) << "No." << setw(5) << "ID" << setw(4) << " " << left << setw(19) << "Date" << setw(4) << " " << left << setw(56) << "Supplier" << setw(4) << " ";
		cout << left << setw(56) << "Employee" << setw(4) << " " << "Total Payment (RM)" << endl;

		for (int i = 0; i < SIZE; i++)
		{
			result->next();
			cout << right << setw(5) << n + 1 << setw(5) << result->getInt("id") << setw(4) << " " << left << setw(19) << result->getString("date");
			cout << setw(4) << " " << setw(3) << left << result->getInt("supplier_id") << " - " << left << setw(50) << result->getString("company_name");
			cout << setw(4) << " " << setw(3) << left << result->getInt("employee_id") << " - " << left << setw(50) << result->getString("name") << setw(4) << " ";
			cout << right << setw(18) << fixed << showpoint << setprecision(2) << result->getDouble("total") << endl;
			n++;
		}
	}
}
bool PurchaseOrderController::employeeExist(string search)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("select exists (select * from purchase_orders inner JOIN purchase_order_details on (purchase_order_id = purchase_orders.id) inner join suppliers on (supplier_id = suppliers.id) inner join employees on (employee_id = employees.id) where name like ? group by purchase_orders.id)");

	ps->setString(1, "%" + search + "%");

	ResultSet* rs = ps->executeQuery();
	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}

int PurchaseOrderController::countSearchSupp(string search)
{
	string str = "select COUNT(DISTINCT purchase_orders.id) AS count from purchase_orders inner JOIN purchase_order_details on(purchase_order_id = purchase_orders.id)"
		+ std::string("inner join suppliers on(supplier_id = suppliers.id) inner join employees on(employee_id = employees.id) where company_name like '%" + search + "%' ");
	PreparedStatement* ps = Database::prepareStatement(str);
	ResultSet* result = ps->executeQuery();
	result->next();
	return (int)result->getInt64("count");
}
void PurchaseOrderController::viewPurOrderListSupp(string searchVal, const int m)
{
	int n = m;

	int totalCount = countSearchSupp(searchVal);

	if (totalCount == 0)
	{
		page.color(15);
		cout << "\nNumber of purchase orders: " << totalCount << endl;
	}
	else
	{
		string str = "select purchase_orders.id, supplier_id, suppliers.company_name, employee_id, employees.`name`,purchase_orders.created_at AS date, "
			+ std::string("sum(purchasing_price * quantity) AS total from purchase_orders inner JOIN purchase_order_details on(purchase_order_id = purchase_orders.id) ")
			+ "inner join suppliers on(supplier_id = suppliers.id) inner join employees on(employee_id = employees.id) where company_name LIKE ? "
			+ "group by purchase_order_id LIMIT 10 offset "
			+ to_string(m);

		PreparedStatement* ps = Database::prepareStatement(str);
		ps->setString(1, "%" + searchVal + "%");
		ResultSet* result = ps->executeQuery();

		int SIZE = ((int)result->rowsCount() < totalCount) ? (int)result->rowsCount() : totalCount;

		page.color(15);
		cout << "\nNumber of purchase orders: " << totalCount << endl;

		cout << "\nShowing " << n + 1;

		if (m + 10 <= totalCount)
			cout << "~" << n + 10 << " out of " << totalCount << " results for \"" << searchVal << "\".\n" << endl;
		else
			cout << "~" << totalCount << " out of " << totalCount << " results for \"" << searchVal << "\".\n" << endl;

		page.color(7);
		cout << right << setw(5) << "No." << setw(5) << "ID" << setw(4) << " " << left << setw(19) << "Date" << setw(4) << " " << left << setw(56) << "Supplier" << setw(4) << " ";
		cout << left << setw(56) << "Employee" << setw(4) << " " << "Total Payment (RM)" << endl;

		for (int i = 0; i < SIZE; i++)
		{
			result->next();
			cout << right << setw(5) << n + 1 << setw(5) << result->getInt("id") << setw(4) << " " << left << setw(19) << result->getString("date");
			cout << setw(4) << " " << setw(3) << left << result->getInt("supplier_id") << " - " << left << setw(50) << result->getString("company_name");
			cout << setw(4) << " " << setw(3) << left << result->getInt("employee_id") << " - " << left << setw(50) << result->getString("name") << setw(4) << " ";
			cout << right << setw(18) << fixed << showpoint << setprecision(2) << result->getDouble("total") << endl;
			n++;
		}
	}
}
bool PurchaseOrderController::suppExist(string search)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("select exists(select * from purchase_orders inner JOIN purchase_order_details on (purchase_order_id = purchase_orders.id) inner join suppliers on (supplier_id = suppliers.id) inner join employees on (employee_id = employees.id) where company_name like ? group by purchase_orders.id)");

	ps->setString(1, "%" + search + "%");

	ResultSet* rs = ps->executeQuery();
	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}

void PurchaseOrderController::purOrderManagement(int loginId)
{
	setEmployeeLoginId(loginId);

	bool isBack = false;
	while (!isBack)
	{
		bool isStopLoop = false;

		int m = 0, a = 0, b = 0, c = 0;
		int id = 0, number = 0;
		string mngChoice;
		string searchVal = "";
		bool isDateFound = false;
		bool isEmpFound = false;
		bool isSuppFound = false;

		while (!isStopLoop)
		{
			bool isValidMngChoice = false;

			if (isDateFound)
			{
				system("cls");
				page.systemHeaderOrange();
				page.purOrderMngmtHeader();
				viewPurOrderListDate(searchVal, a);
			}
			else if (isEmpFound)
			{
				system("cls");
				page.systemHeaderOrange();
				page.purOrderMngmtHeader();
				viewPurOrderListEmployee(searchVal, b);
			}
			else if (isSuppFound)
			{
				system("cls");
				page.systemHeaderOrange();
				page.purOrderMngmtHeader();
				viewPurOrderListSupp(searchVal, c);
			}
			else
			{
				system("cls");
				page.systemHeaderOrange();
				page.purOrderMngmtHeader();
				viewPurOrderList(m);
			}

			cout << endl;
			page.purOrderMngmtPanel();

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
					else if (isSuppFound)
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
					else if (isSuppFound)
					{
						if (c + 10 >= countSearchSupp(searchVal))
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
						if (m + 10 >= PurchaseOrder::count())
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

				//Add New Purchase Order
				else if (mngChoice == "3")
				{
					isValidMngChoice = true;
					addPurOrder();
				}

				//Search Purchase Order
				else if (mngChoice == "4")
				{
					isValidMngChoice = true;
					a = 0, b = 0, c = 0;
					isDateFound = false;
					isEmpFound = false;
					isSuppFound = false;
					int i = 0;
					string searchChoice;
					bool isValidSearchChoice = false;
					page.searchPurOrderPanel();

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
							bool isNull = false;
							while (!isNull)
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
									isNull = true;
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
							bool isNull = false;
							while (!isNull)
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
									isNull = true;
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
						else if (searchChoice == "3")//SuppCompany
						{
							isValidSearchChoice = true;
							bool isNull = false;
							while (!isNull)
							{
								page.color(14);
								cout << "\nEnter supplier company to search: ";
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
									isNull = true;
									bool isSuppExist = suppExist(searchVal);
									if (isSuppExist)
										isSuppFound = true;
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
						else
							page.showInvalidChoiceMsg();
					}				
				}

				//View Purchase Order
				else if (mngChoice == "5")
				{
					isValidMngChoice = true;
					string strId;
					bool isDigit = false;
					bool isValidStrId = false;

					while (!isValidStrId)
					{
						page.color(14);
						cout << "\nEnter Purchase Order ID: ";
						strId.clear();
						page.color(7);
						cin >> strId;

						isDigit = input.isDigit(strId);
						if (isDigit)
						{
							isValidStrId = true;
							int purOrderid = stoi(strId);
							bool isIdExist = idExistPurOrder(purOrderid);
							if (isIdExist)
							{
								system("cls");
								page.systemHeaderOrange();
								page.purOrderMngmtHeader();
								viewDetails(purOrderid);
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
				else if (mngChoice == "6")
				{
					if (isDateFound)
					{
						isValidMngChoice = true;
						isStopLoop = true;
					}
					else if (isEmpFound)
					{
						isValidMngChoice = true;
						isStopLoop = true;
					}
					else if (isSuppFound)
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

void PurchaseOrderController::addPurOrder()
{
	PurchaseOrder purOrder;
	PurchaseOrderDetail purOrderDetails;
	SupplierController supDetails;	
	bool isQuitAdd = false;

	while (!isQuitAdd)
	{

		if (PurchaseOrder::count() == 0)
			purOrder.resetAutoIncrement();
		if (PurchaseOrderDetail::count() == 0)
			purOrderDetails.resetAutoIncrement();

		system("cls");
		page.systemHeaderOrange();
		page.purOrderMngmtHeader();

		page.color(14);
		cout << "\n Add New Purchase Order" << endl;
		for (int i = 0; i < 24; i++) { cout << char(205); }cout << endl;

		int i = 0;
		string searchVal;
		bool cancelLoopSupplier = false;

		while (!cancelLoopSupplier)
		{
			bool noSupResult = false;
			page.color(14);
			cout << "\nEnter supplier id OR Search supplier's company name" << endl;
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
				if (isDigit)
				{
					int id = stoi(searchVal);
					bool isSupIdExist = supDetails.idExist(id);
					if (isSupIdExist)
					{
						cancelLoopSupplier = true;

						bool isValidConfirmSup = false;
						string confirmSup;
						supDetails.showDetails(id);
						page.color(15);
						cout << "\nConfirm supplier?" << endl;
						page.showYesNoChoice();

						while (!isValidConfirmSup)
						{													
							page.color(14);
							cout << "\nEnter choice: ";
							confirmSup.clear();
							page.color(7);
							cin >> confirmSup;

							if (confirmSup == "Y" || confirmSup == "y")
							{
								isValidConfirmSup = true;
								cancelLoopSupplier = true;
								setSupplierId(id);

								PurchaseOrder* po = new PurchaseOrder();

								po->setSupplierId(getSupplierId());
								po->setEmployeeId(getEmployeeLoginId());
								po->save();

								setA(0);
								int purOrderId = Database::getLastInsertId();
								if (getA() == 0)
									setPurOrderId(purOrderId);
								
								isQuitAdd = addPurOrderItem();
							}
							else if (confirmSup == "N" || confirmSup == "n")
							{
								isValidConfirmSup = true;
							}
							else
								page.showInvalidChoiceMsg();
						}
					}
					else
					{
						page.color(12);
						cout << "\nNo result found." << endl;
						noSupResult = true;									
					}
				}
				else
				{
					bool isNameExist = supDetails.nameExist(searchVal);
					if (isNameExist)
						supDetails.searchSupName(searchVal);
					
					else
					{
						page.color(12);
						cout << "\nNo result found." << endl;
						noSupResult = true;
					}
				}
			}
			else
			{
				page.color(12);
				cout << "\nPlease fill out this field." << endl;
			}

			if (noSupResult)
			{
				bool isValidQuitAdd = false;
				string quitAddPurOrder;
				page.color(15);
				cout << "\nContinue Add Purchase Order?" << endl;
				page.showYesNoChoice();

				while (!isValidQuitAdd)
				{		
					page.color(14);
					cout << "\nEnter choice: ";
					quitAddPurOrder.clear();
					page.color(7);
					cin >> quitAddPurOrder;

					if (quitAddPurOrder == "Y" || quitAddPurOrder == "y")
					{
						isValidQuitAdd = true;
						cancelLoopSupplier = true;
					}
					else if (quitAddPurOrder == "N" || quitAddPurOrder == "n")
					{
						isValidQuitAdd = true;
						cancelLoopSupplier = true;
						isQuitAdd = true;
					}
					else
						page.showInvalidChoiceMsg();
				}
			}
		}		
	}
}
bool PurchaseOrderController::addPurOrderItem()
{
	bool cancelLoopAddProductId = false;
	
	string searchVal;
	int number = 0;
	
	while (!cancelLoopAddProductId)
	{
		system("cls");
		page.systemHeaderOrange();
		page.purOrderMngmtHeader();

		showPurchaseOrder(getPurOrderId());
		if (getA() != 0)
		{
			showPurchaseOrderDetails(getPurOrderId()); //Final PurchaseOrder
			page.color(15);
			cout << "\nTotal Payment (RM): " << readTotalPrice(getPurOrderId());
			cout << endl;
		}

		bool isInvalidProduct = false;
		int i = 0;
		while (!isInvalidProduct)
		{
			setResultNotFound(0);
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
							cout << "\nEnter choice: ";
							confirmProd.clear();
							page.color(7);
							cin >> confirmProd;

							if (confirmProd == "Y" || confirmProd == "y")
							{
								isValidConfirmProd = true;
								setProductId(productId);
								string productSize;
								
								bool quitAddProductSize = false;
								while (!quitAddProductSize)
								{
									system("cls");
									page.systemHeaderOrange();
									page.purOrderMngmtHeader();

									showPurchaseOrder(getPurOrderId());
									if (getA() != 0)
									{
										showPurchaseOrderDetails(getPurOrderId()); //Final PurchaseOrder
										page.color(15);
										cout << "\nTotal Payment (RM): " << readTotalPrice(getPurOrderId());
										cout << endl;
									}
								
									page.color(15);
									cout << endl;
									for (int i = 0; i < 200; i++) { cout << char(205); }cout << endl;
									prodDetails.purOrderShowProdDetails(getProductId());

									int i = 0;
									bool isValidProductSize = false;
									while (!isValidProductSize)
									{
										page.color(14);
										cout << left << setw(33) << "\nEnter product size to purchase" << ": ";
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
												setSize(prodDetails.readProdSizeId(getProductId(), productSize));
												setCode(prodDetails.readProdCode(getProductId(), productSize));

												string strPurPrice, strQuantity;
												bool isValidPrice = false;
												while (!isValidPrice)
												{
													page.color(14);
													cout << left << setw(33) << "\nEnter Purchasing Price (RM)" << ": ";
													page.color(7);
													strPurPrice.clear();
													cin >> strPurPrice;
													if (strPurPrice != "")
													{
														bool isDigit = input.isDigit(strPurPrice);
														if (isDigit)
														{
															isValidPrice = true;
															setPrice(stod(strPurPrice));
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

												bool isValidQuantity = false;
												while (!isValidQuantity)
												{
													page.color(14);
													cout << left << setw(33) << "\nEnter Quantity" << ": ";
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

												page.color(15);
												cout << endl;
												for (int i = 0; i < 200; i++) { cout << char(205); }cout << endl;
												page.color(7);
												cout << left << setw(25) << "\nProduct Code" << ": " << getCode();
												cout << left << setw(25) << "\nProduct Size" << ": " << getSize();
												cout << left << setw(25) << "\nPurchasing Price (RM)" << ": " << fixed << showpoint << setprecision(2) << getPrice();
												cout << left << setw(25) << "\nQuantity" << ": " << getQuantity();;
												page.color(15);
												cout << "\n\nAre you confirm?" << endl;
												page.showYesNoChoice();

												bool isValidConfirmSize = false;
												string confirmSize;
												while (!isValidConfirmSize)
												{
													page.color(14);
													cout << "\nEnter choice: ";
													confirmSize.clear();
													page.color(7);
													cin >> confirmSize;

													if (confirmSize == "Y" || confirmSize == "y")
													{
														isValidConfirmSize = true;
														number++;
														PurchaseOrderDetail* pod = new PurchaseOrderDetail();
														pod->setItemNumber(number);
														pod->setCode(getCode());
														pod->setPurPrice(getPrice());
														pod->setQuantity(getQuantity());
														pod->setPurOrderId(getPurOrderId());
														pod->save();

														addProdQuantity(getCode(), getQuantity());
														changeProdStatus(getProductId());
														changeSupStatus(getSupplierId());
														setA(1);

														system("cls");
														page.systemHeaderOrange();
														page.purOrderMngmtHeader();

														showPurchaseOrder(getPurOrderId());
														if (getA() != 0)
														{
															showPurchaseOrderDetails(getPurOrderId()); //Final PurchaseOrder
															page.color(15);
															cout << "\nTotal Payment (RM): " << readTotalPrice(getPurOrderId());
															cout << endl;
														}

														bool cancelLoopChoice = false;
														while (!cancelLoopChoice)
														{
															system("cls");
															page.systemHeaderOrange();
															page.purOrderMngmtHeader();

															showPurchaseOrder(getPurOrderId());
															if (getA() != 0)
															{
																showPurchaseOrderDetails(getPurOrderId()); //Final PurchaseOrder
																page.color(15);
																cout << "\nTotal Payment (RM): " << readTotalPrice(getPurOrderId());
																cout << endl;
															}

															page.color(15);
															cout << endl;
															for (int i = 0; i < 200; i++) { cout << char(205); }cout << endl;
															page.color(7);
															prodDetails.purOrderShowProdDetails(getProductId());

															page.color(15);
															cout << "\nPlease select a choice." << endl;
															page.color(11);
															cout << setw(5) << right << char(201);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(187) << endl;
															cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
															cout << setw(5) << right << char(186) << setw(51) << left << "  1 - Add Another Size" << char(186) << endl;
															cout << setw(5) << right << char(186) << setw(51) << left << "  2 - Add Another Product" << char(186) << endl;
															cout << setw(5) << right << char(186) << setw(37) << left << "  3 - Delete item in Purchase Order #" << setw(14) << getPurOrderId() << char(186) << endl;
															cout << setw(5) << right << char(186) << setw(26) << left << "  4 - End Purchase Order #" << setw(25) << getPurOrderId() << char(186) << endl;
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

																//Add another size
																if (addItem == "1")
																{
																	isValidAddItem = true;
																	cancelLoopChoice = true;
																	i = 0;
																	system("cls");
																	page.systemHeaderOrange();
																	page.purOrderMngmtHeader();

																	showPurchaseOrder(getPurOrderId());
																	if (getA() != 0)
																	{
																		showPurchaseOrderDetails(getPurOrderId()); //Final PurchaseOrder
																		page.color(15);
																		cout << "\nTotal Payment (RM): " << readTotalPrice(getPurOrderId());
																		cout << endl;
																	}

																	page.color(15);
																	cout << endl;
																	for (int i = 0; i < 200; i++) { cout << char(205); }cout << endl;
																	prodDetails.purOrderShowProdDetails(getProductId());
																}
																//Add another product
																else if (addItem == "2")
																{
																	isValidAddItem = true;
																	cancelLoopChoice = true;
																	isValidProductSize = true;
																	quitAddProductSize = true;
																	isValidConfirmProd = true;
																	isInvalidProduct = true;
																	i = 0;
																}
																//Delete item
																else if (addItem == "3")
																{
																	isValidAddItem = true;
																	if (getA() != 0)
																	{
																		system("cls");
																		page.systemHeaderOrange();
																		page.purOrderMngmtHeader();

																		showPurchaseOrder(getPurOrderId());
																		showPurchaseOrderDetails(getPurOrderId()); //Final PurchaseOrder
																		page.color(15);
																		cout << "\nTotal Payment (RM): " << readTotalPrice(getPurOrderId());
																		cout << endl;

																		string strItemNo;
																		bool isValidItemNo = false;

																		page.color(14);
																		cout << "\nEnter item number to delete: ";
																		page.color(7);
																		strItemNo.clear();
																		cin >> strItemNo;

																		if (strItemNo != "")
																		{
																			bool isDigit = input.isDigit(strItemNo);

																			if (isDigit)
																			{
																				int itemNo = stoi(strItemNo);
																				bool isItemNoExist = itemNoExistInPurOrder(getPurOrderId(), itemNo);
																				if (isItemNoExist)
																				{
																					PurchaseOrderDetail* pod = PurchaseOrderDetail::gett(getPurOrderId(), itemNo);
																					pod->displayDetails();

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
																							setDeleteQuantity(readDeleteQuantity(getPurOrderId(), itemNo));
																							setCode(readDeleteCode(getPurOrderId(), itemNo));
																							pod->erase();
																							deleteQuantityInProdStock(getCode(), getDeleteQuantity());

																							page.color(15);
																							cout << "\nItem No." << itemNo << " successfully deleted from Purchase Order #" << getPurOrderId() << endl;
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
																			cout << "\nPlease fill out this field." << endl;
																		}
																	}
																	else
																	{
																		page.color(12);
																		cout << "\nNo item in Purchase Order #" << getPurOrderId() << endl;
																		cout << endl;
																		page.color(7);
																		system("pause");
																	}
																}
																//End purchase order
																else if (addItem == "4")
																{
																	isValidAddItem = true;
																	cancelLoopChoice = true;
																	system("cls");
																	page.systemHeaderOrange();
																	page.purOrderMngmtHeader();
																	showPurchaseOrder(getPurOrderId());
																	if (getA() != 0)
																	{
																		showPurchaseOrderDetails(getPurOrderId()); //Final PurchaseOrder
																		page.color(15);
																		cout << "\nTotal Payment (RM): " << readTotalPrice(getPurOrderId());
																		cout << endl;
																	}
																	else
																	{
																		cout << "\n\"Add Purchase Order\" action cancelled." << endl;
																		deletePurOrderWithoutDet(getPurOrderId());
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
													else if (confirmSize == "N" || confirmSize == "n")
													{
														isValidConfirmSize = true;
														bool cancelLoopChoice = false;
														while (!cancelLoopChoice)
														{
															system("cls");
															page.systemHeaderOrange();
															page.purOrderMngmtHeader();

															showPurchaseOrder(getPurOrderId());
															if (getA() != 0)
															{
																showPurchaseOrderDetails(getPurOrderId()); //Final PurchaseOrder
																page.color(15);
																cout << "\nTotal Payment (RM): " << readTotalPrice(getPurOrderId());
																cout << endl;
															}

															page.color(15);
															cout << endl;
															for (int i = 0; i < 200; i++) { cout << char(205); }cout << endl;
															page.color(7);
															prodDetails.purOrderShowProdDetails(getProductId());

															cout << endl;
															page.color(15);
															cout << "\nPlease select a choice." << endl;
															page.color(11);
															cout << setw(5) << right << char(201);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(187) << endl;
															cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
															cout << setw(5) << right << char(186) << setw(51) << left << "  1 - Add Another Size" << char(186) << endl;
															cout << setw(5) << right << char(186) << setw(51) << left << "  2 - Add Another Product" << char(186) << endl;
															cout << setw(5) << right << char(186) << setw(37) << left << "  3 - Delete item in Purchase Order #" << setw(14) << getPurOrderId() << char(186) << endl;
															cout << setw(5) << right << char(186) << setw(26) << left << "  4 - End Purchase Order #" << setw(25) << getPurOrderId() << char(186) << endl;
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

																//Add another size
																if (addItem == "1")
																{
																	isValidAddItem = true;
																	cancelLoopChoice = true;
																	i = 0;
																	system("cls");
																	page.systemHeaderOrange();
																	page.purOrderMngmtHeader();

																	showPurchaseOrder(getPurOrderId());
																	if (getA() != 0)
																	{
																		showPurchaseOrderDetails(getPurOrderId()); //Final PurchaseOrder
																		page.color(15);
																		cout << "\nTotal Payment (RM): " << readTotalPrice(getPurOrderId());
																		cout << endl;
																	}

																	page.color(15);
																	cout << endl;
																	for (int i = 0; i < 200; i++) { cout << char(205); }cout << endl;
																	prodDetails.purOrderShowProdDetails(getProductId());
																}
																//Add another product
																else if (addItem == "2")
																{
																	isValidAddItem = true;
																	cancelLoopChoice = true;
																	isValidProductSize = true;
																	quitAddProductSize = true;
																	isValidConfirmProd = true;
																	isInvalidProduct = true;
																	i = 0;
																}
																//Delete item
																else if (addItem == "3")
																{
																	isValidAddItem = true;
																	if (getA() != 0)
																	{
																		system("cls");
																		page.systemHeaderOrange();
																		page.purOrderMngmtHeader();

																		showPurchaseOrder(getPurOrderId());
																		showPurchaseOrderDetails(getPurOrderId()); //Final PurchaseOrder
																		page.color(15);
																		cout << "\nTotal Payment (RM): " << readTotalPrice(getPurOrderId());
																		cout << endl;

																		string strItemNo;
																		bool isValidItemNo = false;
																		
																		page.color(14);
																		cout << "\nEnter item number to delete: ";
																		page.color(7);
																		strItemNo.clear();
																		cin >> strItemNo;

																		if (strItemNo != "")
																		{
																			bool isDigit = input.isDigit(strItemNo);

																			if (isDigit)
																			{
																				int itemNo = stoi(strItemNo);
																				bool isItemNoExist = itemNoExistInPurOrder(getPurOrderId(), itemNo);
																				if (isItemNoExist)
																				{
																					PurchaseOrderDetail* pod = PurchaseOrderDetail::gett(getPurOrderId(), itemNo);
																					pod->displayDetails();

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
																							setDeleteQuantity(readDeleteQuantity(getPurOrderId(), itemNo));
																							setCode(readDeleteCode(getPurOrderId(), itemNo));
																							pod->erase();
																							deleteQuantityInProdStock(getCode(), getDeleteQuantity());
																							
																							page.color(15);
																							cout << "\nItem No." << itemNo << " successfully deleted from Purchase Order #" << getPurOrderId() << endl;
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
																			cout << "\nPlease fill out this field." << endl;
																		}
																	}
																	else
																	{
																		page.color(12);
																		cout << "\nNo item in Purchase Order #" << getPurOrderId() << endl;
																		cout << endl;
																		page.color(7);
																		system("pause");
																	}
																}
																//End purchase order
																else if (addItem == "4")
																{
																	isValidAddItem = true;
																	cancelLoopChoice = true;
																	system("cls");
																	page.systemHeaderOrange();
																	page.purOrderMngmtHeader();
																	showPurchaseOrder(getPurOrderId());
																	if (getA() != 0)
																	{
																		showPurchaseOrderDetails(getPurOrderId()); //Final PurchaseOrder
																		page.color(15);
																		cout << "\nTotal Payment (RM): " << readTotalPrice(getPurOrderId());
																		cout << endl;
																	}
																	else
																	{
																		cout << "\n\"Add Purchase Order\" action cancelled." << endl;
																		deletePurOrderWithoutDet(getPurOrderId());
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
							}
								
							else if (confirmProd == "N" || confirmProd == "n")
							{
								isValidConfirmProd = true;
								isInvalidProduct = true;
								setResultNotFound(1);
							}
							else
								page.showInvalidChoiceMsg();
						}
					}
					else
					{
						page.color(12);
						cout << "\nNo result found." << endl;
						isInvalidProduct = true;
						setResultNotFound(1);
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
						isInvalidProduct = true;
						setResultNotFound(1);
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

		if (isInvalidProduct)
		{
			if (getResultNotFound() == 1)
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
						isValidContSearch = true;
					}
					else if (contSearch == "N" || contSearch == "n")
					{
						isValidContSearch = true;
						cancelLoopAddProductId = true;
						system("cls");
						page.systemHeaderOrange();
						page.purOrderMngmtHeader();
						showPurchaseOrder(getPurOrderId());
						if (getA() != 0)
						{
							showPurchaseOrderDetails(getPurOrderId()); //Final PurchaseOrder
							page.color(15);
							cout << "\nTotal Payment (RM): " << readTotalPrice(getPurOrderId());
							cout << endl;
						}
						else
						{
							cout << "\n\"Add Purchase Order\" action cancelled." << endl;
							deletePurOrderWithoutDet(getPurOrderId());
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

bool PurchaseOrderController::itemNoExistInPurOrder(int purOrderid, int itemno)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("select EXISTS (select * from purchase_order_details where purchase_order_id = ? and item_number = ?)");

	ps->setInt(1, purOrderid);
	ps->setInt(2, itemno);

	ResultSet* rs = ps->executeQuery();
	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}
int PurchaseOrderController::readDeleteQuantity(int purOrderid, int itemNo)
{
	string str = "select quantity from purchase_order_details where purchase_order_id = ? and item_number = ?";

	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, purOrderid);
	ps->setInt(2, itemNo);
	ResultSet* result = ps->executeQuery();

	result->next();
	return (result->getInt("quantity"));
}

int PurchaseOrderController::readDeleteCode(int purOrderid, int itemNo)
{
	string str = "select `code` from purchase_order_details where purchase_order_id = ? and item_number = ?";

	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, purOrderid);
	ps->setInt(2, itemNo);
	ResultSet* result = ps->executeQuery();

	result->next();
	return (result->getInt("code"));
}

void PurchaseOrderController::deleteQuantityInProdStock(int productcode, int quantity)
{
	ProductStock* ps = ProductStock::gett(productcode);
	if (ps != nullptr)
	{
		ps->setQuantity(ps->getQuantity() - quantity);
		ps->save();
	}
}

void PurchaseOrderController::changeProdStatus(int productid)
{
	Product* p = Product::get(productid);
	if (p != nullptr)
	{
		p->setStatus("active");
		p->save();
	}
}
void PurchaseOrderController::addProdQuantity(int productcode, int quantity)
{
	ProductStock* ps = ProductStock::gett(productcode); 
	if (ps != nullptr)
	{
		ps->setQuantity(ps->getQuantity() + quantity);
		ps->save();
	}
}
void PurchaseOrderController::changeSupStatus(int supid)
{
	Supplier* s = Supplier::get(supid); 
	if (s != nullptr)
	{
		s->setStatus("active");
		s->save();
	}
}

void PurchaseOrderController::showPurchaseOrder(int purOrderId)
{
	string str = "select purchase_orders.id, purchase_orders.employee_id,employees.`name`, purchase_orders.supplier_id, suppliers.company_name"
		+ std::string(", purchase_orders.created_at from purchase_orders inner JOIN suppliers on(purchase_orders.supplier_id = suppliers.id)")
		+ "inner JOIN employees on(employees.id = purchase_orders.employee_id) where purchase_orders.id = ?;";

	page.color(15);
	cout << "\nShowing details of Purchase Order #" << purOrderId << endl;

	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, purOrderId);
	ResultSet* result = ps->executeQuery();

	for (int i = 0; i < result->rowsCount(); i++)
	{
		result->next();
		cout << left << setw(15) << "\nDate" << ": " << result->getString("created_at");
		cout << left << setw(15) << "\nEmployee" << ": " << result->getInt("employee_id") << " - " << result->getString("name");
		cout << left << setw(15) << "\nSupplier" << ": " << result->getInt("supplier_id") << " - " << result->getString("company_name") << endl;
	}
}
void PurchaseOrderController::showPurchaseOrderDetails(int purOrderId)
{
	string str = "select purchase_order_details.item_number, purchase_order_details.`code`,purchasing_price, purchase_order_details.quantity,(purchasing_price*purchase_order_details.quantity) AS subtotal, "
		+ std::string("size, product_id, `name`,colour, gender, type from purchase_order_details inner JOIN product_stocks on(purchase_order_details.`code`=product_stocks.`code`)")
		+ "inner JOIN products on(product_stocks.product_id = products.id) where purchase_order_id = ? order by item_number;";

	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, purOrderId);
	ResultSet* result = ps->executeQuery();

	cout << endl;
	cout << left << setw(8) << "Item No." << setw(4) << "" << setw(70) << "Product" << setw(8) << "" << setw(4) << " " << left << setw(12) << "Colour" << setw(4) << " ";
	cout << left << setw(7) << "Gender" << setw(4) << " " << left << setw(12) << "Type" << setw(4) << " ";
	cout << left << setw(7) << "Code" << setw(4) << " " << left << setw(10) << "Size" << setw(4) << " ";
	cout << left << setw(16) << "Purchasing Price" << setw(4) << " " << left << setw(8) << "Quantity" << setw(4) << " " << "SubTotal (RM)" << endl;

	page.color(7);
	for (int i = 0; i < result->rowsCount(); i++)
	{
		result->next();
		cout << left << setw(8) << result->getInt("item_number")  << setw(4) << "" << left << setw(5) << result->getInt("product_id") << " - " << left << setw(70) << result->getString("name") << setw(4) << " ";
		cout << left << setw(12) << result->getString("colour") << setw(4) << " " << left << setw(7) << result->getString("gender") << setw(4) << " ";
		cout << left << setw(12) << result->getString("type") << setw(4) << " ";
		cout << left << setw(7) << result->getInt("code") << setw(4) << " " << left << setw(10) << result->getString("size") << setw(4) << " ";
		cout << left << setw(16) << result->getDouble("purchasing_price") << setw(4) << " " << left << setw(8) << result->getInt("quantity") << setw(4) << " " << result->getDouble("subtotal") << endl;
	}
}
double PurchaseOrderController::readTotalPrice(int purOrderid)
{
	string str = "select sum(purchasing_price * purchase_order_details.quantity) AS total from purchase_order_details "
		+ std::string("inner JOIN product_stocks on(purchase_order_details.`code`=product_stocks.`code`) inner JOIN products on(product_stocks.product_id = products.id)")
		+ " where purchase_order_id = ?";

	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, purOrderid);
	ResultSet* result = ps->executeQuery();

	result->next();
	return (result->getDouble("total"));
}

void PurchaseOrderController::viewDetails(int purOrderid)
{
	showPurchaseOrder(purOrderid);	
	showPurchaseOrderDetails(purOrderid); //Final PurchaseOrder
	page.color(15);
	cout << "\n\nTotal Payment (RM): " << readTotalPrice(purOrderid);
	cout << endl;
}

void PurchaseOrderController::deletePurOrderWithoutDet(int purOrderid)
{
	bool isIdDetExist = idExistPurOrderDet(purOrderid);
	if (!isIdDetExist)
	{
		PurchaseOrder* po = PurchaseOrder::get(purOrderid);
		po->erase();
	}
}

bool PurchaseOrderController::idExistPurOrder(int id)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("SELECT EXISTS (SELECT * FROM purchase_orders WHERE id = ?)");

	ps->setInt(1, id);

	ResultSet* rs = ps->executeQuery();
	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}
bool PurchaseOrderController::idExistPurOrderDet(int id)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("SELECT EXISTS (SELECT * FROM purchase_order_details WHERE purchase_order_id = ?)");

	ps->setInt(1, id);

	ResultSet* rs = ps->executeQuery();
	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}

void PurchaseOrderController::setA(int n) {
	a = n;
}
int PurchaseOrderController::getA() {
	return a;
}

void PurchaseOrderController::setResultNotFound(int n) {
	resultNotFound = n;
}
int PurchaseOrderController::getResultNotFound() {
	return resultNotFound;
}

void PurchaseOrderController::setItemNo(int n) {
	itemNo = n;
}
int PurchaseOrderController::getItemNo() {
	return itemNo;
}

void PurchaseOrderController::setProductId(int n) {
	productId = n;
}
int PurchaseOrderController::getProductId() {
	return productId;
}

void PurchaseOrderController::setSize(string n) {
	size = n;
}
string PurchaseOrderController::getSize() {
	return size;
}
void PurchaseOrderController::setPrice(double n) {
	price = n;
}
double PurchaseOrderController::getPrice() {
	return price;
}
void PurchaseOrderController::setQuantity(int n) {
	quantity = n;
}
int PurchaseOrderController::getQuantity() {
	return quantity;
}

void PurchaseOrderController::setCode(int n) {
	code = n;
}
int PurchaseOrderController::getCode() {
	return code;
}
void PurchaseOrderController::setDeleteQuantity(int n) {
	deleteQuantity = n;
}
int PurchaseOrderController::getDeleteQuantity() {
	return deleteQuantity;
}


void PurchaseOrderController::setSupplierId(int n) {
	supplierId= n;
}
int PurchaseOrderController::getSupplierId() {
	return supplierId;
}
void PurchaseOrderController::setEmployeeLoginId(int n) {
	employeeLoginId = n;
}
int PurchaseOrderController::getEmployeeLoginId() {
	return employeeLoginId;
}
void PurchaseOrderController::setPurOrderId(int n) {
	purchaseOrderId = n;
}
int PurchaseOrderController::getPurOrderId() {
	return purchaseOrderId;
}
