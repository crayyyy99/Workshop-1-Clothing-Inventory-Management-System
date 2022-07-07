#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <iomanip>
#include <conio.h>

using namespace std;

#include <mysql/jdbc.h>
using namespace sql;

#include "ReportController.h"
#include "DatabaseConnection.h"
#include "Database.h"
#include "IndexPage.h"
#include "CheckInput.h"

void ReportController::reportManagement()
{
	bool isBack = false;
	while (!isBack)
	{
		bool isStopLoop = false;

		int m = 0, a = 0;
		int id = 0;
		string mngChoice;

		while (!isStopLoop)
		{
			bool isValidMngChoice = false;
			system("cls");
			page.systemHeaderOrange();
			page.viewReportHeader();
			page.viewReportTypePanel();

			while (!isValidMngChoice)
			{
				page.color(14);
				cout << "\nEnter choice: ";
				page.color(7);
				mngChoice.clear();
				cin >> mngChoice;

				//Sales Report
				if (mngChoice == "1")
				{
					isValidMngChoice = true;
					salesReport();
				}
		
				//Best Seller
				else if (mngChoice == "2")
				{
					isValidMngChoice = true;
					bestSellerReport();
				}

				//Back
				else if (mngChoice == "3")
				{
					isValidMngChoice = true;
					isStopLoop = true;
					isBack = true;
				}

				//Invalid Input
				else
					page.showInvalidChoiceMsg();
			}
		}
	}
}

//Sales
void ReportController::salesReport()
{
	bool isBack = false;
	while (!isBack)
	{
		system("cls");
		page.systemHeaderOrange();
		page.viewSalesReportHeader();
		page.viewReportTimePanel();

		bool isValidTimeChoice = false;
		string timeChoice;
		while (!isValidTimeChoice)
		{
			page.color(14);
			cout << "\nEnter choice: ";
			page.color(7);
			timeChoice.clear();
			cin >> timeChoice;

			//Daily
			if (timeChoice == "1")
			{
				isValidTimeChoice = true;
				dailySales();
				cout << "\n" << endl;
				page.color(7);
				system("pause");
			}
			//Monthly
			else if (timeChoice == "2")
			{
				isValidTimeChoice = true;
				monthlySales();
				cout << "\n" << endl;
				page.color(7);
				system("pause");
			}
			//Annual
			else if (timeChoice == "3")
			{
				isValidTimeChoice = true;
				annualSales();
				cout << "\n" << endl;
				page.color(7);
				system("pause");
			}
			//Total
			else if (timeChoice == "4")
			{
				isValidTimeChoice = true;
				totalSales();
				cout << "\n" << endl;
				page.color(7);
				system("pause");
			}
			//Back
			else if (timeChoice == "5")
			{
				isValidTimeChoice = true;
				isBack = true;
			}
			else
				page.showInvalidChoiceMsg();
		}
	}
}

void ReportController::totalSales()
{
	system("cls");
	page.systemHeaderOrange();
	page.viewSalesReportHeader();

	page.color(14);
	cout << "\n Total Sales Report" << endl;
	for (int i = 0; i < 20; i++) { cout << char(205); }cout << endl;

	string str = "select COUNT(DISTINCT transaction_id) AS count, count(transaction_details.quantity) as totalItem,"
		+ std::string(" round(sum(transaction_details.quantity * (unit_price - (unit_price * discount / 100))),1) AS totalSales from transactions ")
		+ "inner join transaction_details on (transactions.id = transaction_id) inner join product_stocks on (transaction_details.`code`=product_stocks.`code`) "
		+ "inner join products on(product_id = products.id)";
	PreparedStatement* ps = Database::prepareStatement(str);
	ResultSet* result = ps->executeQuery();
	result->next();
	page.color(15);
	cout << left << setw(29) << "\nTotal number of sales" << ": " << result->getInt("count");
	cout << left << setw(29) << "\nTotal number of item sold" << ": " << result->getInt("totalItem");
	cout << left << setw(29) << "\nTotal sales" << ": RM" << fixed << showpoint << setprecision(2) << result->getDouble("totalSales") << endl;
}

void ReportController::annualSales()
{
	system("cls");
	page.systemHeaderOrange();
	page.viewSalesReportHeader();

	page.color(14);
	cout << "\n Annual Sales Report" << endl;
	for (int i = 0; i < 21; i++) { cout << char(205); }cout << endl;
	int k = 0;
	string year;
	bool isValidStr = false;
	while (!isValidStr)
	{
		page.color(14);
		cout << "\nEnter Year [Format YYYY]: ";
		page.color(7);
		year.clear();
		cin >> year;

		bool isDigit = input.isDigit(year);
		if (!isDigit)
			page.showInvalidChoiceMsg();
		else if(isDigit && year.length() != 4)
			page.showInvalidChoiceMsg();
		else
		{
			isValidStr = true;
			system("cls");
			page.systemHeaderOrange();
			page.viewSalesReportHeader();
			page.color(14);
			cout << "\n Sales Report for Year " << year << endl;
			for (int i = 0; i < 28; i++) { cout << char(205); }cout << endl;
			
			page.color(15);
			string month[12] = { "JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER" };
			
			if (countTotalAnnualSales(year) == 0)
			{
				page.color(12);
				cout << "\nNo record." << endl;
			}
			else
			{
				cout << endl;
				cout << left << setw(15) << "Month" << setw(4) << " " << left << setw(18) << "Number of Sales" << setw(4) << " " << left << setw(11) << "Item Count" << setw(4) << " ";
				cout << left << setw(10) << "Total (RM)" << setw(4) << " " << "\n" << endl;
				page.color(7);
				for (int i = 0; i < 12; i++)
				{
					if (i < 9)
					{
						string str = "select COUNT(DISTINCT transaction_id) AS count, count(transaction_details.quantity) as totalItem,"
							+ std::string(" round(sum(transaction_details.quantity * (unit_price - (unit_price * discount / 100))),1) AS totalSales from transactions ")
							+ "inner join transaction_details on (transactions.id = transaction_id) inner join product_stocks on (transaction_details.`code`=product_stocks.`code`) "
							+ "inner join products on(product_id = products.id) where transactions.created_at like ? ";
						PreparedStatement* ps = Database::prepareStatement(str);
						ps->setString(1, "%" + year + "-0" + to_string(i + 1) + "%");
						ResultSet* result = ps->executeQuery();
						result->next();
						if (result->getInt("count") != 0)
						{
							k++;
							cout << left << setw(15) << month[i] << setw(4) << " " << left << setw(18) << result->getInt("count") << setw(4) << " " << left << setw(11) << result->getInt("totalItem");
							cout << setw(4) << " " << right << setw(10) << fixed << showpoint << setprecision(2) << result->getDouble("totalSales") << "\n" << endl;
						}
					}
					else
					{
						string str = "select COUNT(DISTINCT transaction_id) AS count, count(transaction_details.quantity) as totalItem,"
							+ std::string(" round(sum(transaction_details.quantity * (unit_price - (unit_price * discount / 100))),1) AS totalSales from transactions ")
							+ "inner join transaction_details on (transactions.id = transaction_id) inner join product_stocks on (transaction_details.`code`=product_stocks.`code`) "
							+ "inner join products on(product_id = products.id) where transactions.created_at like ? ";
						PreparedStatement* ps = Database::prepareStatement(str);
						ps->setString(1, "%" + year + "-" + to_string(i + 1) + "%");
						ResultSet* result = ps->executeQuery();
						result->next();
						if (result->getInt("count") != 0)
						{
							k++;
							cout << left << setw(15) << month[i] << setw(4) << " " << left << setw(18) << result->getInt("count") << setw(4) << " " << left << setw(11) << result->getInt("totalItem");
							cout << setw(4) << " " << right << setw(10) << fixed << showpoint << setprecision(2) << result->getDouble("totalSales") << "\n" << endl;
						}
					}
				}

				string str = "select COUNT(DISTINCT transaction_id) AS count, count(transaction_details.quantity) as totalItem,"
					+ std::string(" round(sum(transaction_details.quantity * (unit_price - (unit_price * discount / 100))),1) AS totalSales from transactions ")
					+ "inner join transaction_details on (transactions.id = transaction_id) inner join product_stocks on (transaction_details.`code`=product_stocks.`code`) "
					+ "inner join products on(product_id = products.id) where transactions.created_at like ? ";

				PreparedStatement* ps = Database::prepareStatement(str);
				ps->setString(1, "%" + year + "%");
				ResultSet* result = ps->executeQuery();
				result->next();
				page.color(15);
				for (int i = 0; i < 70; i++) { cout << char(205); } cout << endl;
				cout << left << setw(35) << "\nTotal Number of Sales " << ": " << result->getInt("count");
				cout << left << setw(35) << "\nTotal Item Count " << ": " << result->getInt("totalItem");
				cout << left << setw(35) << "\nTotal Sales " << ": RM" << fixed << showpoint << setprecision(2) << result->getDouble("totalSales") << endl;

				cout << left << setw(35) << "\nAverage Number of Sales Per Month" << ": ";

				if (result->getInt("count") / k <= 0)
					cout << "1";
				else
					cout << result->getInt("count") / k;

				cout << left << setw(35) << "\nAverage Item Count Per Month" << ": ";

				if (result->getInt("totalItem") / k <= 0)
					cout << "1";
				else
					cout << result->getInt("totalItem") / k;
				cout << left << setw(35) << "\nAverage Sales Per Month" << ": RM" << fixed << showpoint << setprecision(2) << result->getDouble("totalSales") / k;
			}
		}
	}
}
int  ReportController::countTotalAnnualSales(string year)
{
	string str = "select COUNT(DISTINCT transaction_id) AS count from transactions "
		+ std::string("inner join transaction_details on (transactions.id = transaction_id) inner join product_stocks on (transaction_details.`code`=product_stocks.`code`) ")
		+ "inner join products on(product_id = products.id) where transactions.created_at like ? ";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setString(1, "%" + year + "%");
	ResultSet* result = ps->executeQuery();
	result->next();
	return (int)result->getInt64("count");
}

void ReportController::monthlySales()
{
	system("cls");
	page.systemHeaderOrange();
	page.viewSalesReportHeader();

	page.color(14);
	cout << "\n Monthly Sales Report" << endl;
	for (int i = 0; i < 22; i++) { cout << char(205); }cout << endl;
	int k = 0;
	string year, strMonth;
	bool isValidStrYear = false;
	while (!isValidStrYear)
	{
		page.color(14);
		cout << left << setw(28) << "\nEnter Year [Format YYYY]"<<": ";
		page.color(7);
		year.clear();
		cin >> year;

		bool isDigit = input.isDigit(year);
		if (!isDigit)
			page.showInvalidChoiceMsg();
		else if (isDigit && year.length() != 4)
			page.showInvalidChoiceMsg();
		else
		{
			isValidStrYear = true;
			bool isValidStrMonth = false;
			while (!isValidStrMonth)
			{
				page.color(14);
				cout << left << setw(28) << "\nEnter Month [Format MM]"<<": ";
				page.color(7);
				strMonth.clear();
				cin >> strMonth;

				bool isDigit = input.isDigit(strMonth);
				if (!isDigit)
					page.showInvalidChoiceMsg();
				else if (isDigit && strMonth.length() != 2)
					page.showInvalidChoiceMsg();
				else if (isDigit && stoi(strMonth) >= 32)
					page.showInvalidChoiceMsg();
				else
				{
					isValidStrMonth = true;
					system("cls");
					page.systemHeaderOrange();
					page.viewSalesReportHeader();
					page.color(14);
					string month[12] = { "JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER" };
					int day[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
					cout << "\n Sales Report for " << month[stoi(strMonth) - 1] << " " << year << endl;
					for (int i = 0; i < 19; i++) { cout << char(205); } for (int i = 0; i < month[stoi(strMonth) - 1].length(); i++) { cout << char(205); }	
					for (int i = 0; i < 5; i++) { cout << char(205); } cout << endl;
					
					if (countTotalMonthlySales(year, strMonth) == 0)
					{
						page.color(12);
						cout << "\nNo record." << endl;
					}
					else
					{
						cout << endl;
						page.color(15);
						cout << left << setw(2+ month[stoi(strMonth) - 1].length()+5) << "Date" << setw(4) << " " << left << setw(18) << "Number of Sales" << setw(4) << " " << left << setw(11) << "Item Count" << setw(4) << " ";
						cout << left << setw(10) << "Total (RM)"  << endl;
						cout << endl;
						page.color(7);

						for (int i = 0; i < day[stoi(strMonth) - 1]; i++)
						{
							if (i < 10)
							{
								string str = "select COUNT(DISTINCT transaction_id) AS count, count(transaction_details.quantity) as totalItem,"
									+ std::string(" round(sum(transaction_details.quantity * (unit_price - (unit_price * discount / 100))),1) AS totalSales from transactions ")
									+ "inner join transaction_details on (transactions.id = transaction_id) inner join product_stocks on (transaction_details.`code`=product_stocks.`code`) "
									+ "inner join products on(product_id = products.id) where transactions.created_at like ? ";
								PreparedStatement* ps = Database::prepareStatement(str);
								ps->setString(1, "%" + year + "-" + strMonth + "-0" + to_string(i + 1) + "%");
								ResultSet* result = ps->executeQuery();
								result->next();
								
								if (result->getInt("count") != 0)
								{
									cout << left << setw(2) << i + 1 <<left<<setw(month[stoi(strMonth) - 1].length())<< month[stoi(strMonth) - 1] <<" "<<year<< setw(4) << " " << left << setw(18) << result->getInt("count") << setw(4) << " " << left << setw(11) << result->getInt("totalItem");
									cout << setw(4) << " " << right << setw(10) << fixed << showpoint << setprecision(2) << result->getDouble("totalSales") << endl;
									k++;
								}
									
							}
							else
							{
								string str = "select COUNT(DISTINCT transaction_id) AS count, count(transaction_details.quantity) as totalItem,"
									+ std::string(" round(sum(transaction_details.quantity * (unit_price - (unit_price * discount / 100))),1) AS totalSales from transactions ")
									+ "inner join transaction_details on (transactions.id = transaction_id) inner join product_stocks on (transaction_details.`code`=product_stocks.`code`) "
									+ "inner join products on(product_id = products.id) where transactions.created_at like ? ";
								PreparedStatement* ps = Database::prepareStatement(str);
								ps->setString(1, "%" + year + "-" + strMonth + "-" + to_string(i + 1) + "%");
								ResultSet* result = ps->executeQuery();
								result->next();
								
								if (result->getInt("count") != 0)
								{
									cout << left << setw(2) << i + 1 << left << setw(month[stoi(strMonth) - 1].length()) << month[stoi(strMonth) - 1] << " " << year << setw(4) << " " << left << setw(18) << result->getInt("count") << setw(4) << " " << left << setw(11) << result->getInt("totalItem");
									cout << setw(4) << " " << right << setw(10) << fixed << showpoint << setprecision(2) << result->getDouble("totalSales") << endl;
									k++;
								}
							}
						}


						string str = "select COUNT(DISTINCT transaction_id) AS count, count(transaction_details.quantity) as totalItem,"
							+ std::string(" round(sum(transaction_details.quantity * (unit_price - (unit_price * discount / 100))),1) AS totalSales from transactions ")
							+ "inner join transaction_details on (transactions.id = transaction_id) inner join product_stocks on (transaction_details.`code`=product_stocks.`code`) "
							+ "inner join products on(product_id = products.id) where transactions.created_at like ? ";

						PreparedStatement* ps = Database::prepareStatement(str);
						ps->setString(1, "%" + year + "-" + strMonth + "%");
						ResultSet* result = ps->executeQuery();
						result->next();
						page.color(15);
						cout << endl;
						for (int i = 0; i < 70; i++) { cout << char(205); } cout << endl;
						cout << left << setw(35) << "\nTotal Number of Sales " << ": " << result->getInt("count");
						cout << left << setw(35) << "\nTotal Item Count " << ": " << result->getInt("totalItem");
						cout << left << setw(35) << "\nTotal Sales " << ": RM" << fixed << showpoint << setprecision(2) << result->getDouble("totalSales") << endl;

						
						cout << left << setw(35) << "\nAverage Number of Sales Per Day" << ": ";

						if (result->getInt("count") / k <= 0)
							cout << "1";
						else
							cout << result->getInt("count") / k;

						cout << left << setw(35) << "\nAverage Item Count Per Day" << ": ";

						if (result->getInt("totalItem") / k <= 0)
							cout << "1";
						else
							cout << result->getInt("totalItem") / k;

						cout << left << setw(35) << "\nAverage Sales Per Day" << ": RM" << fixed << showpoint << setprecision(2) << result->getDouble("totalSales") / k;
					}									
				}
			}		
		}			
	}


}
int  ReportController::countTotalMonthlySales(string year,string month)
{
	string str = "select COUNT(DISTINCT transaction_id) AS count from transactions "
		+ std::string("inner join transaction_details on (transactions.id = transaction_id) inner join product_stocks on (transaction_details.`code`=product_stocks.`code`) ")
		+ "inner join products on(product_id = products.id) where transactions.created_at like ? ";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setString(1, "%" + year + "-" + month + "%");
	ResultSet* result = ps->executeQuery();
	result->next();
	return (int)result->getInt64("count");
}

void ReportController::dailySales()
{
	system("cls");
	page.systemHeaderOrange();
	page.viewSalesReportHeader();

	page.color(14);
	cout << "\n Daily Sales Report" << endl;
	for (int i = 0; i < 20; i++) { cout << char(205); } cout << endl;

	string date;
	bool isValidStrDate = false;
	while (!isValidStrDate)
	{
		page.color(14);
		cout << left << setw(28) << "\nEnter Date [Format YYYY-MM-DD]" << ": ";
		page.color(7);
		date.clear();
		cin >> date;

		bool isValidDate = input.isValidDate(date);
		if (!isValidDate)
			page.showInvalidChoiceMsg();
		else
		{
			isValidStrDate = true;
			
			system("cls");
			page.systemHeaderOrange();
			page.viewSalesReportHeader();
			page.color(14);
			cout << "\n Sales Report for " << date << endl;
			for (int i = 0; i < 30; i++) { cout << char(205); } cout << endl;			
			page.color(15);

			if (countTotalDailySales(date) == 0)
			{
				page.color(12);
				cout << "\nNo record." << endl;
			}
			else
			{
				string str = "select COUNT(DISTINCT transaction_id) AS count, count(transaction_details.quantity) as totalItem,"
					+ std::string(" round(sum(transaction_details.quantity * (unit_price - (unit_price * discount / 100))),1) AS totalSales from transactions ")
					+ "inner join transaction_details on (transactions.id = transaction_id) inner join product_stocks on (transaction_details.`code`=product_stocks.`code`) "
					+ "inner join products on(product_id = products.id) where transactions.created_at like ? ";
				PreparedStatement* ps = Database::prepareStatement(str);
				ps->setString(1, "%" + date + "%");
				ResultSet* result = ps->executeQuery();
				result->next();

				page.color(15);
				cout << left << setw(30) << "\nTotal Number of Sales " << ": " << result->getInt("count");
				cout << left << setw(30) << "\nTotal Item Count " << ": " << result->getInt("totalItem");
				cout << left << setw(30) << "\nTotal Sales " << ": RM" << fixed << showpoint << setprecision(2) << result->getDouble("totalSales") << endl;

				cout << left << setw(30) << "\nAverage Per Sale" << ": RM" << fixed << showpoint << setprecision(2) << result->getDouble("totalSales") / result->getInt("count") << endl;
			}				
		}
	}
}
int ReportController::countTotalDailySales(string date)
{
	string str = "select COUNT(DISTINCT transaction_id) AS count from transactions "
		+ std::string("inner join transaction_details on (transactions.id = transaction_id) inner join product_stocks on (transaction_details.`code`=product_stocks.`code`) ")
		+ "inner join products on(product_id = products.id) where transactions.created_at like ? ";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setString(1, "%" + date + "%");
	ResultSet* result = ps->executeQuery();
	result->next();
	return (int)result->getInt64("count");
}

//Best Seller
void ReportController::bestSellerReport()
{
	bool isBack = false;
	while (!isBack)
	{
		system("cls");
		page.systemHeaderOrange();
		page.viewBestSellerReportHeader();
		page.viewReportTimePanel();

		bool isValidTimeChoice = false;
		string timeChoice;
		while (!isValidTimeChoice)
		{
			page.color(14);
			cout << "\nEnter choice: ";
			page.color(7);
			timeChoice.clear();
			cin >> timeChoice;

			//Daily
			if (timeChoice == "1")
			{
				isValidTimeChoice = true;
				dailyBestSeller();
				cout << "\n" << endl;
				page.color(7);
				system("pause");
			}
			//Monthly
			else if (timeChoice == "2")
			{
				isValidTimeChoice = true;
				monthlyBestSeller();
				cout << "\n" << endl;
				page.color(7);
				system("pause");
			}
			//Annual
			else if (timeChoice == "3")
			{
				isValidTimeChoice = true;
				annualBestSeller();
				cout << "\n" << endl;
				page.color(7);
				system("pause");
			}
			//Total
			else if (timeChoice == "4")
			{
				isValidTimeChoice = true;
				totalBestSeller();
				cout << "\n" << endl;
				page.color(7);
				system("pause");
			}
			//Back
			else if (timeChoice == "5")
			{
				isValidTimeChoice = true;
				isBack = true;
			}
			else
				page.showInvalidChoiceMsg();
		}
	}
}

void ReportController::totalBestSeller()
{
	system("cls");
	page.systemHeaderOrange();
	page.viewBestSellerReportHeader();

	page.color(14);
	cout << "\n Total Best Seller Report" << " (TOP 10)" << endl;
	for (int i = 0; i < 36; i++) { cout << char(205); }cout << endl;
	int k = 0;
	page.color(15);
	cout << endl;
	cout << right << setw(5) << "No." << setw(3) << " " << setw(5) << left << setw(86) << "Product" << setw(4) << " ";
	cout << left << setw(15) << "Colour" << setw(4) << " " << left << setw(7) << "Gender" << setw(4) << " " << left << setw(20) << "Type";
	cout << setw(4) << " " << left << setw(14) << "Quantity Sold" << endl;

	string str = "select products.id, products.`name`,colour, gender, type, sum(transaction_details.quantity) as totalQuantity from transactions "
		+ std::string("inner join transaction_details on(transaction_id = transactions.id) inner join product_stocks on(product_stocks.`code`=transaction_details.`code`)")
		+ "inner join products on(product_id = products.id)  group by products.id order by totalQuantity desc limit 10 ";

	PreparedStatement* ps = Database::prepareStatement(str);
	ResultSet* result = ps->executeQuery();
	page.color(7);
	for (int i = 0; i < result->rowsCount(); i++)
	{
		result->next();
		cout << right << setw(5) << k + 1 << setw(3) << " " << right << setw(3) << result->getInt("id") << setw(3) << " - " << left << setw(80) << result->getString("name") << setw(4) << " ";
		cout << left << setw(15) << result->getString("colour") << setw(4) << " " << left << setw(7) << result->getString("gender") << setw(4) << " " << left << setw(20) << result->getString("type");
		cout << setw(4) << " " << left << setw(14) << result->getInt("totalQuantity") << endl;
		k++;
	}
}

void ReportController::annualBestSeller()
{
	system("cls");
	page.systemHeaderOrange();
	page.viewBestSellerReportHeader();

	page.color(14);
	cout << "\n Annual Best Seller Report" << endl;
	for (int i = 0; i < 27; i++) { cout << char(205); }cout << endl;
	int k = 0;
	string year;
	bool isValidStr = false;
	while (!isValidStr)
	{
		page.color(14);
		cout << "\nEnter Year [Format YYYY]: ";
		page.color(7);
		year.clear();
		cin >> year;

		bool isDigit = input.isDigit(year);
		if (!isDigit)
			page.showInvalidChoiceMsg();
		else if (isDigit && year.length() != 4)
			page.showInvalidChoiceMsg();
		else
		{
			isValidStr = true;
			system("cls");
			page.systemHeaderOrange();
			page.viewBestSellerReportHeader();
			page.color(14);
			cout << "\n Best Seller Report for Year " << year <<" (TOP 10)"<< endl;
			for (int i = 0; i < 44; i++) { cout << char(205); }cout << endl;
			int k = 0;
			
			if (countTotalAnnualBestSeller(year) == 0)
			{
				page.color(12);
				cout << "\nNo record." << endl;
			}
			else
			{
				cout << endl;
				cout << right << setw(5) << "No." << setw(3) << " " << setw(5) << left << setw(86) << "Product" << setw(4) << " ";
				cout << left << setw(15) << "Colour" << setw(4) << " " << left << setw(7) << "Gender" << setw(4) << " " << left << setw(20) << "Type";
				cout << setw(4) << " " << left << setw(14) << "Quantity Sold" << endl;

				string str = "select products.id, products.`name`,colour, gender, type, sum(transaction_details.quantity) as totalQuantity from transactions "
					+ std::string("inner join transaction_details on(transaction_id = transactions.id) inner join product_stocks on(product_stocks.`code`=transaction_details.`code`)")
					+ "inner join products on(product_id = products.id) where transactions.created_at like ? group by products.id order by totalQuantity desc limit 10 ";

				PreparedStatement* ps = Database::prepareStatement(str);
				ps->setString(1, "%" + year + "%");
				ResultSet* result = ps->executeQuery();
				page.color(7);
				for (int i = 0; i < result->rowsCount(); i++)
				{
					result->next();
					cout << right << setw(5) << k + 1 << setw(3) << " " << right << setw(3) << result->getInt("id") << setw(3) << " - " << left << setw(80) << result->getString("name") << setw(4) << " ";
					cout << left << setw(15) << result->getString("colour") << setw(4) << " " << left << setw(7) << result->getString("gender") << setw(4) << " " << left << setw(20) << result->getString("type");
					cout << setw(4) << " " << left << setw(14) << result->getInt("totalQuantity") << endl;
					k++;
				}
			}
		}
	}
}
int  ReportController::countTotalAnnualBestSeller(string year)
{
	string str = "select count(DISTINCT transactions.id) as count from transactions inner join transaction_details on (transaction_id = transactions.id) "
		+ std::string("inner join product_stocks on (product_stocks.`code`=transaction_details.`code`) inner join products on (product_id = products.id)")
		+ " where transactions.created_at like ? ";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setString(1, "%" + year + "%");
	ResultSet* result = ps->executeQuery();
	result->next();
	return (int)result->getInt64("count");
}

void ReportController::monthlyBestSeller()
{
	system("cls");
	page.systemHeaderOrange();
	page.viewBestSellerReportHeader();

	page.color(14);
	cout << "\n Monthly Sales Report" << endl;
	for (int i = 0; i < 22; i++) { cout << char(205); }cout << endl;
	int k = 0;
	string year, strMonth;
	bool isValidStrYear = false;
	while (!isValidStrYear)
	{
		page.color(14);
		cout << left << setw(28) << "\nEnter Year [Format YYYY]" << ": ";
		page.color(7);
		year.clear();
		cin >> year;

		bool isDigit = input.isDigit(year);
		if (!isDigit)
			page.showInvalidChoiceMsg();
		else if (isDigit && year.length() != 4)
			page.showInvalidChoiceMsg();
		else
		{
			isValidStrYear = true;
			bool isValidStrMonth = false;
			while (!isValidStrMonth)
			{
				page.color(14);
				cout << left << setw(28) << "\nEnter Month [Format MM]" << ": ";
				page.color(7);
				strMonth.clear();
				cin >> strMonth;

				bool isDigit = input.isDigit(strMonth);
				if (!isDigit)
					page.showInvalidChoiceMsg();
				else if (isDigit && strMonth.length() != 2)
					page.showInvalidChoiceMsg();
				else if (isDigit && stoi(strMonth) >= 32)
					page.showInvalidChoiceMsg();
				else
				{
					isValidStrMonth = true;
					system("cls");
					page.systemHeaderOrange();
					page.viewBestSellerReportHeader();
					page.color(14);
					int k = 0;
					string month[12] = { "JANUARY", "FEBRUARY", "MARCH", "APRIL", "MAY", "JUNE", "JULY", "AUGUST", "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER" };
					int day[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
					cout << "\n Best Seller Report for " << month[stoi(strMonth) - 1] << " " << year << " (TOP 10)" << endl;
					for (int i = 0; i < 35; i++) { cout << char(205); } for (int i = 0; i < month[stoi(strMonth) - 1].length(); i++) { cout << char(205); }
					for (int i = 0; i < 5; i++) { cout << char(205); } cout << endl;

					if (countTotalMonthlyBestSeller(year, strMonth) == 0)
					{
						page.color(12);
						cout << "\nNo record." << endl;
					}
					else
					{
						page.color(15);
						cout << endl;
						cout << right << setw(5) << "No." << setw(3) << " " << setw(5) << left << setw(86) << "Product" << setw(4) << " ";
						cout << left << setw(15) << "Colour" << setw(4) << " " << left << setw(7) << "Gender" << setw(4) << " " << left << setw(20) << "Type";
						cout << setw(4) << " " << left << setw(14) << "Quantity Sold" << endl;

						string str = "select products.id, products.`name`,colour, gender, type, sum(transaction_details.quantity) as totalQuantity from transactions "
							+ std::string("inner join transaction_details on(transaction_id = transactions.id) inner join product_stocks on(product_stocks.`code`=transaction_details.`code`)")
							+ "inner join products on(product_id = products.id) where transactions.created_at like ? group by products.id order by totalQuantity desc limit 10 ";

						PreparedStatement* ps = Database::prepareStatement(str);
						ps->setString(1, "%" + year + "-" + strMonth + "%");
						ResultSet* result = ps->executeQuery();
						page.color(7);
						for (int i = 0; i < result->rowsCount(); i++)
						{
							result->next();
							cout << right << setw(5) << k + 1 << setw(3) << " " << right << setw(3) << result->getInt("id") << setw(3) << " - " << left << setw(80) << result->getString("name") << setw(4) << " ";
							cout << left << setw(15) << result->getString("colour") << setw(4) << " " << left << setw(7) << result->getString("gender") << setw(4) << " " << left << setw(20) << result->getString("type");
							cout << setw(4) << " " << left << setw(14) << result->getInt("totalQuantity") << endl;
							k++;
						}
					}
				}
			}
		}
	}


}
int  ReportController::countTotalMonthlyBestSeller(string year, string month)
{
	string str = "select count(DISTINCT transactions.id) as count from transactions inner join transaction_details on (transaction_id = transactions.id) "
		+ std::string("inner join product_stocks on (product_stocks.`code`=transaction_details.`code`) inner join products on (product_id = products.id)")
		+ " where transactions.created_at like ? ";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setString(1, "%" + year + "-" + month + "%");
	ResultSet* result = ps->executeQuery();
	result->next();
	return (int)result->getInt64("count");
}

void ReportController::dailyBestSeller()
{
	system("cls");
	page.systemHeaderOrange();
	page.viewBestSellerReportHeader();

	page.color(14);
	cout << "\n Daily Best Seller Report" << endl;
	for (int i = 0; i < 26; i++) { cout << char(205); } cout << endl;
	int k = 0;
	string date;
	bool isValidStrDate = false;
	while (!isValidStrDate)
	{
		page.color(14);
		cout << left << setw(28) << "\nEnter Date [Format YYYY-MM-DD]" << ": ";
		page.color(7);
		date.clear();
		cin >> date;

		bool isValidDate = input.isValidDate(date);
		if (!isValidDate)
			page.showInvalidChoiceMsg();
		else
		{
			isValidStrDate = true;

			system("cls");
			page.systemHeaderOrange();
			page.viewBestSellerReportHeader();
			page.color(14);
			cout << "\n Best Seller Report for " << date << " (TOP 10)" << endl;
			for (int i = 0; i < 45; i++) { cout << char(205); } cout << endl;
			page.color(15);

			if (countTotalDailyBestSeller(date) == 0)
			{
				page.color(12);
				cout << "\nNo record." << endl;
			}
			else
			{	
				cout << endl;
				cout << right << setw(5) << "No." << setw(3) << " " << setw(5)  << left << setw(86) << "Product" << setw(4) << " ";
				cout << left << setw(15) << "Colour" << setw(4) << " " << left << setw(7) << "Gender" << setw(4) << " " << left << setw(20) << "Type";
				cout << setw(4) << " " << left << setw(14) << "Quantity Sold" << endl;

				string str = "select products.id, products.`name`,colour, gender, type, sum(transaction_details.quantity) as totalQuantity from transactions "
					+ std::string("inner join transaction_details on(transaction_id = transactions.id) inner join product_stocks on(product_stocks.`code`=transaction_details.`code`)")
					+ "inner join products on(product_id = products.id) where transactions.created_at like ? group by products.id order by totalQuantity desc limit 10 ";

				PreparedStatement* ps = Database::prepareStatement(str);
				ps->setString(1, "%" + date + "%");
				ResultSet* result = ps->executeQuery();
				page.color(7);
				for (int i = 0; i < result->rowsCount(); i++)
				{
					result->next();
					cout << right << setw(5) << k+1 << setw(3) << " " <<right << setw(3) << result->getInt("id") << setw(3) << " - " << left << setw(80) << result->getString("name") << setw(4) << " ";
					cout << left << setw(15) << result->getString("colour") << setw(4) << " " << left << setw(7) << result->getString("gender") << setw(4) << " " << left << setw(20) << result->getString("type");
					cout << setw(4) << " " << left << setw(14) << result->getInt("totalQuantity") << endl;
					k++;
				}				
			}
		}
	}
}
int ReportController::countTotalDailyBestSeller(string date)
{
	string str = "select count(DISTINCT transactions.id) as count from transactions inner join transaction_details on (transaction_id = transactions.id) "
		+ std::string("inner join product_stocks on (product_stocks.`code`=transaction_details.`code`) inner join products on (product_id = products.id)")
		+ " where transactions.created_at like ? ";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setString(1, "%" + date + "%");
	ResultSet* result = ps->executeQuery();
	result->next();
	return (int)result->getInt64("count");
}