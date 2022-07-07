#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <iomanip>
#include <conio.h>

using namespace std;

#include <mysql/jdbc.h>
using namespace sql;

#include "SupplierController.h"
#include "DatabaseConnection.h"
#include "Supplier.h"
#include "IndexPage.h"
#include "CheckInput.h"

void SupplierController::viewSuppList(int m)
{
	system("cls");
	page.systemHeaderOrange();
	page.suppMngmtHeader();

	int n = m;
	if (Supplier::count() == 0)
	{
		page.color(15);
		cout << "\nNumber of suppliers: " << Supplier::count() << endl;
	}
	else if (m <= Supplier::count())
	{
		page.color(15);
		cout << "\nNumber of suppliers: " << Supplier::count() << endl;

		//Msg- Show m+1 ~ m+10 out of MAX results.

		cout << "\nShowing " << m + 1;

		if (m + 10 <= Supplier::count())
			cout << "~" << m + 10 << " out of " << Supplier::count() << " results.\n" << endl;
		else
			cout << "~" << Supplier::count() << " out of " << Supplier::count() << " results.\n" << endl;


		string str = "SELECT * FROM suppliers LIMIT 10 OFFSET " + to_string(m);
		PreparedStatement* ps = Database::prepareStatement(str);
		ResultSet* result = ps->executeQuery();
		list<Supplier*>list = Supplier::parseList(result);

		page.color(14);
		cout << right << setw(5) << "No." << setw(5) << "ID" << setw(4) << " " << left << setw(50) << "Company Name" << setw(4) << " ";
		cout << left << setw(40) << "Contact Name" << setw(4) << " " << "Status" << endl;
		page.color(7);

		for (auto const& item : list)
		{
			item->display(n);
			cout << endl;
			n++;
		}
	}
}

int SupplierController::countSearchName(string search)
{
	string str = "SELECT COUNT(id) AS count FROM Suppliers WHERE company_name LIKE '%" + search + "%' OR contact_name LIKE '%" + search + "%'";
	PreparedStatement* ps = Database::prepareStatement(str);
	ResultSet* result = ps->executeQuery();
	result->next();
	return (int)result->getInt64("count");
}
void SupplierController::viewSuppListName(string searchVal, int a)
{
	system("cls");
	page.systemHeaderOrange();
	page.suppMngmtHeader();

	int b = a;

	if (a <= countSearchName(searchVal))
	{
		page.color(15);
		cout << "\nShowing " << a + 1;

		if (a + 10 <= countSearchName(searchVal) && countSearchName(searchVal) > 10)
			cout << "~" << a + 10 << " out of " << countSearchName(searchVal) << " results for \"" << searchVal << "\".\n" << endl;
		else
			cout << "~" << countSearchName(searchVal) << " out of " << countSearchName(searchVal) << " results for \"" << searchVal << "\".\n" << endl;

		string str = "SELECT * FROM suppliers WHERE company_name LIKE '%" + searchVal + "%' OR contact_name LIKE '%" + searchVal + "%' LIMIT 10 OFFSET " + to_string(a);
		PreparedStatement* ps = Database::prepareStatement(str);
		ResultSet* result = ps->executeQuery();
		list<Supplier*>list = Supplier::parseList(result);

		page.color(14);
		cout << right << setw(5) << "No." << setw(5) << "ID" << setw(4) << " " << left << setw(50) << "Company Name" << setw(4) << " ";
		cout << left << setw(40) << "Contact Name" << setw(4) << " " << "Status" << endl;
		page.color(7);

		for (auto const& item : list)
		{
			item->display(b);
			cout << endl;
			b++;
		}
	}
}

void SupplierController::suppManagement()
{
	bool isBack = false;
	while (!isBack)
	{
		bool isStopLoop = false;

		int m = 0, a = 0;
		int id = 0;
		string mngChoice;
		string searchVal = "";
		bool isNameFound = false;

		while (!isStopLoop)
		{
			bool isValidMngChoice = false;

			if (isNameFound)
				viewSuppListName(searchVal, a);
			else
				viewSuppList(m);

			cout << endl;
			page.suppMngmtPanel();
			
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

					if (isNameFound)
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
				else if (mngChoice == "2")
				{
					isValidMngChoice = true;
					if (isNameFound)
					{
						if (a + 10 >= countSearchName(searchVal))
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
						if (m + 10 >= Supplier::count())
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

				//Add New Supplier
				else if (mngChoice == "3")
				{
					isValidMngChoice = true;
					addSupplier();
				}

				//Search Supplier
				else if (mngChoice == "4")
				{
					isValidMngChoice = true;				
					isNameFound = false;

					bool isValidSearchChoice = false;
					bool isDigit = false;
					bool isValidStrId = false;
					bool isIdExist = false;

					isValidSearchChoice = true;
					a = 0;
					int i = 0;
					bool isNull = false;
					while (!isNull)
					{
						page.color(14);
						cout << "\nEnter supplier's company name or contact name to search: ";
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
							bool isNameExist = nameExist(searchVal);

							if (isNameExist)
								isNameFound = true;
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

				//View Supplier Details and Modify
				else if (mngChoice == "5")
				{
					isValidMngChoice = true;
					string strId;
					bool isDigit = false;
					bool isValidStrId = false;

					while (!isValidStrId)
					{
						page.color(14);
						cout << "\nEnter supplier id: ";
						strId.clear();
						page.color(7);
						cin >> strId;

						isDigit = input.isDigit(strId);
						if (isDigit)
						{
							isValidStrId = true;
							int id = stoi(strId);
							bool isIdExist = idExist(id);
							if (isIdExist)
								modifySuppDetails(id);
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

				//Delete Supplier
				else if (mngChoice == "6")
				{
					isValidMngChoice = true;
					string strId;
					bool isDigit = false;
					bool isValidStrId = false;
					
					page.color(15);
					cout << "\nFor supplier which is activated, delete supplier will change the status of this supplier to \"disabled\"." << endl;
					while (!isValidStrId)
					{
						page.color(14);
						cout << "\nEnter supplier id: ";
						strId.clear();
						page.color(7);
						cin >> strId;

						isDigit = input.isDigit(strId);
						if (isDigit)
						{
							isValidStrId = true;
							int id = stoi(strId);
							bool isIdExist = idExist(id);
							if (isIdExist)
								deleteSupp(id);
							else
							{
								page.color(12);
								cout << "\nNo result found.\n" << endl;
								page.color(7);
								system("pause");
							}
						}
						//Invalid ID input
						else
							page.showInvalidChoiceMsg();
					}
				}

				//Back
				else if (mngChoice == "7")
				{
					if (isNameFound)
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

void SupplierController::addSupplier()
{
	Supplier suppDetails;
	bool quitAdd = false;

	while (!quitAdd)
	{
		system("cls");
		page.systemHeaderOrange();
		page.suppMngmtHeader();

		
		string compName, contName, contTitle, compEmail, compAddress, contactNo1, contactNo2, otherDetails;
		int i = 0;
		bool isCancelLoop = false;
		bool isValidCompName = false;
		bool isValidContName = false;
		bool isValidCompEmail = false;
		bool isValidCompAddress = false;
		bool isValidContactNo1 = false;
		bool isValidContactNo2 = false;
		bool isNull = false;

		if (Supplier::count() == 0)
			suppDetails.resetAutoIncrement();

		system("cls");
		page.systemHeaderOrange();
		page.suppMngmtHeader();

		page.color(14);
		cout << "\n Add New Supplier" << endl;
		for (int i = 0; i < 18; i++) { cout << char(205); }cout << endl;

		while (!isValidCompName)
		{
			page.color(14);
			cout << setw(23) << "\nCompany Name" << ": ";
			page.color(7);
			compName.clear();
			if (i == 0)
			{
				cin.ignore(1, '\n');
				i++;
			}
			getline(cin, compName);

			isValidCompName = input.isNullOrWhiteSpace(compName);
			if (!isValidCompName)
			{
				page.color(12);
				cout << "\nPlease fill out this field. " << endl;
			}
			else
				suppDetails.setCompName(compName);
		}

		while (!isValidContName)
		{
			page.color(14);
			cout << setw(23) << "\nContact Name" << ": ";
			page.color(7);
			contName.clear();
			getline(cin, contName);

			isValidContName = input.isNullOrWhiteSpace(contName);
			if (!isValidContName)
			{
				page.color(12);
				cout << "\nPlease fill out this field. " << endl;
			}
			else
				suppDetails.setContactName(contName);
		}

		page.color(15);
		cout << "\nThis field may be blank." << endl;
		page.color(14);
		cout << setw(23) << "\nContact Title" << ": ";
		page.color(7);
		contTitle.clear();
		getline(cin, contTitle);
		suppDetails.setContactTitle(contTitle);

		page.color(15);
		cout << "\nThis field may be blank." << endl;
		while (!isValidCompEmail)
		{
			page.color(14);
			cout << setw(23) << "\nCompany E-mail Address" << ": ";
			page.color(7);
			compEmail.clear();
			getline(cin, compEmail);

			if (compEmail == "")
			{
				isValidCompEmail = true;
				suppDetails.setCompEmail(compEmail);
			}
			else
			{
				isValidCompEmail = input.isValidEmail(compEmail);

				if (!isValidCompEmail)
				{
					page.color(12);
					cout << "\nInvalid E-mail format. Please try again." << endl;
				}
				else
					suppDetails.setCompEmail(compEmail);
			}			
		}

		while (!isValidCompAddress)
		{
			page.color(14);
			cout << setw(23) << "\nCompany Address" << ": ";
			page.color(7);
			compAddress.clear();
			getline(cin, compAddress);

			isValidCompAddress = input.isNullOrWhiteSpace(compAddress);
			if (!isValidCompAddress)
			{
				page.color(12);
				cout << "\nPlease fill out this field. " << endl;
			}
			else
				suppDetails.setCompAddress(compAddress);
		}

		while (!isValidContactNo1)
		{
			page.color(14);
			cout << setw(23) << "\nContact Number 1" << ": ";
			page.color(7);
			contactNo1.clear();
			getline(cin, contactNo1);

			isValidContactNo1 = input.isValidContactNo(contactNo1);
			if (!isValidContactNo1)
			{
				page.color(12);
				cout << "\nInvalid format. The contact number must contains dash,'-'. Please try again." << endl;
			}
			else
				suppDetails.setContactNo1(contactNo1);
		}

		page.color(15);
		cout << "\nThis field may be blank." << endl;
		do
		{
			page.color(14);
			cout << setw(23) << "\nContact Number 2" << ": ";
			page.color(7);
			contactNo2.clear();
			getline(cin, contactNo2);

			if (contactNo2=="")
			{
				isValidContactNo2 = true;
				suppDetails.setContactNo2(contactNo2);
				break;
			}
			else
			{
				isValidContactNo2 = input.isValidContactNo(contactNo2);
				if (!isValidContactNo2)
				{
					page.color(12);
					cout << "\nInvalid format. The contact number must contains dash,'-'. Please try again." << endl;
				}
				else
					suppDetails.setContactNo2(contactNo2);
			}
		} while (!isValidContactNo2);

		page.color(15);
		cout << "\nThis field may be blank." << endl;
		page.color(14);
		cout << setw(22) << "\nOther Details" << ": ";
		page.color(7);
		otherDetails.clear();
		getline(cin, otherDetails);
		suppDetails.setOtherDetails(otherDetails);

		system("cls");
		page.systemHeaderOrange();
		page.suppMngmtHeader();

		page.color(15);
		cout << "\nPlease validate the supplier's details." << endl;
		page.color(7);
		cout << setw(23) << "\nCompany Name" << ": " << suppDetails.getCompName() << endl;
		cout << setw(23) << "\nContact Name" << ": " << suppDetails.getContactName() << endl;
		cout << setw(23) << "\nContact Title" << ": " << suppDetails.getContactTitle() << endl;
		cout << setw(23) << "\nCompany E-mail Address" << ": " << suppDetails.getCompEmail() << endl;
		cout << setw(23) << "\nCompany Address" << ": " << suppDetails.getCompAddress() << endl;
		cout << setw(23) << "\nContact Number" << ": " << suppDetails.getContactNo1() << endl;
		cout << setw(23) << "\nContact Number 2" << ": " << suppDetails.getContactNo2() << endl;
		cout << setw(23) << "\nOther Details" << ": " << suppDetails.getOtherDetails() << endl;
		cout << endl;

		bool isValidConfirmAdd = false;
		string confirmAdd;
		page.color(15);
		cout << "\nAre you confirm?" << endl;
		page.showYesNoChoice();
		
		while (!isValidConfirmAdd)
		{
			page.color(14);
			cout << "\nEnter choice: ";
			page.color(7);
			confirmAdd.clear();
			cin >> confirmAdd;

			if (confirmAdd == "Y" || confirmAdd == "y")
			{
				isValidConfirmAdd = true;

				Supplier* s = new Supplier();

				s->setCompName(suppDetails.getCompName());
				s->setContactName(suppDetails.getContactName());
				s->setContactTitle(suppDetails.getContactTitle());
				s->setCompEmail(suppDetails.getCompEmail());
				s->setCompAddress(suppDetails.getCompAddress());
				s->setContactNo1(suppDetails.getContactNo1());
				s->setContactNo2(suppDetails.getContactNo2());
				s->setOtherDetails(suppDetails.getOtherDetails());

				s->save();
				
				setSupId(Database::getLastInsertId());

				quitAdd = true;
				page.color(15);
				cout << "\nNew supplier successfully added." << endl;
				cout << endl;
				page.color(7);
				system("pause");
			}
			else if (confirmAdd == "N" || confirmAdd == "n")
			{
				isValidConfirmAdd = true;
				quitAdd = true;
			}	
			else
				page.showInvalidChoiceMsg();
		}	
	}
}

void SupplierController::setSupId(int id) {
	supId = id;
}
int SupplierController::getSupId() {
	return supId;
}

void SupplierController::modifySuppDetails(int id)
{
	bool isBack = false;
	string editChoice;

	while (!isBack)
	{
		system("cls");
		page.systemHeaderOrange();
		page.suppMngmtHeader();

		page.color(15);
		cout << "\nShowing details of Supplier #" << id << endl;
		page.color(7);
		Supplier* s = Supplier::get(id);
		cout << s->displayDetails() << endl;

		page.modifySuppPanel();
		bool isValidEditChoice = false;
		
		while (!isValidEditChoice)
		{
			page.color(14);
			cout << "\nEnter choice: ";
			page.color(7);
			editChoice.clear();
			cin >> editChoice;

			//Modify Company Name
			if (editChoice == "1")
			{
				isValidEditChoice = true;
				bool isValidCompName = false;
				string compName;
				int i = 0;
				while (!isValidCompName)
				{
					page.color(14);
					cout << setw(20) << "\nNew Company Name" << ": ";
					page.color(7);
					compName.clear();
					if (i == 0)
					{
						cin.ignore(1, '\n');
						i++;
					}
					getline(cin, compName);

					isValidCompName = input.isNullOrWhiteSpace(compName);
					if (!isValidCompName)
					{
						page.color(12);
						cout << "\nPlease fill out this field. " << endl;
					}
					else
					{
						s->setCompName(compName);
						s->save();

						page.color(15);
						cout << "\nCompany name successfully updated." << endl;
						cout << endl;
						page.color(7);
						system("pause");
					}
				}
			}
			//Modify Contact Name		
			else if (editChoice == "2")
			{
				isValidEditChoice = true;
				bool isValidContName = false;
				string contName;
				int i = 0;
				while (!isValidContName)
				{
					page.color(14);
					cout << setw(20) << "\nNew Contact Name" << ": ";
					page.color(7);
					contName.clear();
					if (i == 0)
					{
						cin.ignore(1, '\n');
						i++;
					}
					getline(cin, contName);

					isValidContName = input.isNullOrWhiteSpace(contName);
					if (!isValidContName)
					{
						page.color(12);
						cout << "\nPlease fill out this field. " << endl;
					}
					else
					{
						s->setContactName(contName);
						s->save();

						page.color(15);
						cout << "\nContact name successfully updated." << endl;
						cout << endl;
						page.color(7);
						system("pause");
					}
				}
			}
			//Modify Contact Title
			else if (editChoice == "3")
			{
				isValidEditChoice = true;
				string contTitle;
				int i = 0;

				page.color(14);
				cout << setw(20) << "\nContact Title" << ": ";
				page.color(7);
				contTitle.clear();
				if (i == 0)
				{
					cin.ignore(1, '\n');
					i++;
				}
				getline(cin, contTitle);

				s->setContactTitle(contTitle);
				s->save();

				page.color(15);
				cout << "\nContact title successfully updated." << endl;
				cout << endl;
				page.color(7);
				system("pause");
			}
			//Modify Company Email Address
			else if (editChoice == "4")
			{
				isValidEditChoice = true;
				bool isValidCompEmail = false;
				string compEmail;
				int i = 0;
				while (!isValidCompEmail)
				{
					page.color(14);
					cout << setw(20) << "\nNew Company E-mail Address" << ": ";
					page.color(7);
					compEmail.clear();
					if (i == 0)
					{
						cin.ignore(1, '\n');
						i++;
					}
					getline(cin, compEmail);

					isValidCompEmail = input.isValidEmail(compEmail);

					if (!isValidCompEmail)
					{
						page.color(12);
						cout << "\nInvalid E-mail format. Please try again." << endl;
					}
					else
					{
						s->setCompEmail(compEmail);
						s->save();

						page.color(15);
						cout << "\nCompany E-mail address successfully updated." << endl;
						cout << endl;
						page.color(7);
						system("pause");
					}
				}
			}			
			//Modify Company Address
			else if (editChoice == "5")
			{
				isValidEditChoice = true;
				bool isValidCompAddress = false;
				string compAddress;
				int i = 0;
				while (!isValidCompAddress)
				{
					page.color(14);
					cout << setw(20) << "\nCompany Address" << ": ";
					page.color(7);
					compAddress.clear();
					if (i == 0)
					{
						cin.ignore(1, '\n');
						i++;
					}
					getline(cin, compAddress);

					isValidCompAddress = input.isNullOrWhiteSpace(compAddress);
					if (!isValidCompAddress)
					{
						page.color(12);
						cout << "\nPlease fill out this field. " << endl;
					}
					else
					{
						s->setCompAddress(compAddress);
						s->save();

						page.color(15);
						cout << "\nCompany address successfully updated." << endl;
						cout << endl;
						page.color(7);
						system("pause");
					}
				}
			}
			//Modify Contact Number
			else if (editChoice == "6")
			{
				isValidEditChoice = true;
				bool isValidContactNo = false;
				string contactNo;
				int i = 0;

				while (!isValidContactNo)
				{
					page.color(14);
					cout << setw(20) << "\nContact Number" << ": ";
					page.color(7);
					contactNo.clear();
					if (i == 0)
					{
						cin.ignore(1, '\n');
						i++;
					}
					getline(cin, contactNo);

					isValidContactNo = input.isValidContactNo(contactNo);
					if (!isValidContactNo)
					{
						page.color(12);
						cout << "\nInvalid format. The contact number must contains dash,'-'. Please try again." << endl;
					}
					else
					{
						s->setContactNo1(contactNo);
						s->save();

						page.color(15);
						cout << "\nContact number successfully updated." << endl;
						cout << endl;
						page.color(7);
						system("pause");
					}
				}
			}
			//Modify Contact Number 2
			else if (editChoice == "7")
			{
				isValidEditChoice = true;
				bool isValidContactNo = false;
				bool isNull = false;
				string contactNo2;
				int i = 0;

				do
				{
					page.color(14);
					cout << setw(20) << "\nContact Number 2" << ": ";
					page.color(7);
					contactNo2.clear();
					if (i == 0)
					{
						cin.ignore(1, '\n');
						i++;
					}
					getline(cin, contactNo2);

					isNull = input.isNullOrWhiteSpace(contactNo2);
					if (isNull)
					{
						isValidContactNo = true;
						s->setContactNo2(contactNo2);
						s->save();

						page.color(15);
						cout << "\nContact number 2 successfully updated." << endl;
						cout << endl;
						page.color(7);
						system("pause");
					}
					else
					{
						isValidContactNo = input.isValidContactNo(contactNo2);
						if (!isValidContactNo)
						{
							page.color(12);
							cout << "\nInvalid format. The contact number must contains dash,'-'. Please try again." << endl;
						}
						else
						{
							s->setContactNo1(contactNo2);
							s->save();

							page.color(15);
							cout << "\nContact number 2 successfully updated." << endl;
							cout << endl;
							page.color(7);
							system("pause");
						}
					}
				} while (!isValidContactNo);
			}
			//Modify Other Details
			else if (editChoice == "8")
			{
				isValidEditChoice = true;
				string otherDetails;
				int i = 0;

				page.color(14);
				cout << setw(20) << "\nOther Details" << ": ";
				page.color(7);
				otherDetails.clear();
				if (i == 0)
				{
					cin.ignore(1, '\n');
					i++;
				}
				getline(cin, otherDetails);

				s->setOtherDetails(otherDetails);
				s->save();

				page.color(15);
				cout << "\nOther details successfully updated." << endl;
				cout << endl;
				page.color(7);
				system("pause");
			}
			//Back
			else if (editChoice == "9")
			{
				isValidEditChoice = true;
				isBack = true;
			}
			//Invalid Input
			else
				page.showInvalidChoiceMsg();
		}
	}
}
void SupplierController::deleteSupp(int id)
{
	system("cls");
	page.systemHeaderOrange();
	page.suppMngmtHeader();

	page.color(15);
	cout << "\nShowing details of Supplier #" << id << endl;
	Supplier* s = Supplier::get(id);
	cout << s->displayDetails() << endl;

	bool isValidDelConf = false;
	bool isSuppActivated = false;
	string delConf;
	int i = 0;
	page.color(15);
	cout << "\nAre you sure you want to delete Supplier #" << id << "?" << endl;
	page.showYesNoChoice();

	while (!isValidDelConf)
	{
		page.color(14);
		cout << "\nEnter choice: ";
		page.color(7);
		delConf.clear();
		cin >> delConf;

		//Confirm
		if (delConf == "Y" || delConf == "y")
		{
			isValidDelConf = true;
			
			isSuppActivated = suppActivated(id);
			if (isSuppActivated)
			{
				changeStatus(id);
				page.color(15);
				cout << "\nThis supplier is activated so the status of this supplier is changed to \"disabled\"." << endl;
				page.color(7);
				cout << endl;
				system("pause");
			}
			else
			{
				s->erase();
				page.color(15);
				cout << "\nSupplier #" << id << " deleted successfully.\n" << endl;
				page.color(7);
				system("pause");
			}		
		}
		//Cancel
		else if (delConf == "N" || delConf == "n")
		{
			isValidDelConf = true;
		}
		//Invalid Input
		else
			page.showInvalidChoiceMsg();
	}
}

void SupplierController::changeStatus(int id)
{
	Supplier* s = Supplier::get(id);
	if (s != nullptr)
	{
		string status = "disabled";
		s->setStatus(status);
		s->save();
	}
}
bool SupplierController::suppActivated(int id)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("SELECT EXISTS (SELECT * FROM Suppliers WHERE id = ? AND (status = ? OR status = ? ))");

	ps->setInt(1, id);
	ps->setString(2, "active");
	ps->setString(3, "disabled");

	ResultSet* rs = ps->executeQuery();
	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}

//validation
bool SupplierController::idExist(int id)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("SELECT EXISTS (SELECT * FROM Suppliers WHERE id = ?)");

	ps->setInt(1, id);

	ResultSet* rs = ps->executeQuery();
	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}
bool SupplierController::nameExist(string searchVal)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("SELECT EXISTS (SELECT * FROM Suppliers WHERE company_name LIKE ? OR contact_name LIKE ?)");

	ps->setString(1, "%" + searchVal + "%");
	ps->setString(2, "%" + searchVal + "%");
	ResultSet* rs = ps->executeQuery();
	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}

void SupplierController::searchSupName(string searchVal)
{
	int b = 0;
	int totalSearch = countSearchName(searchVal);
	
	string str = "SELECT * FROM suppliers WHERE company_name LIKE '%" + searchVal + "%'";
	PreparedStatement* ps = Database::prepareStatement(str);

	ResultSet* result = ps->executeQuery();

	int SIZE = ((int)result->rowsCount() < totalSearch) ? (int)result->rowsCount() : totalSearch;

	page.color(15);
	cout << "\nShowing " << b + 1;
	cout << "~" << countSearchName(searchVal) << " out of " << countSearchName(searchVal) << " results for \"" << searchVal << "\".\n" << endl;

	page.color(7);
	cout << right << setw(5) << "No." << setw(5) << "ID" << setw(4) << " " << left << setw(50) << "Company Name" << setw(4) << " ";
	cout << left << setw(40) << "Contact Name" << setw(4) << " " << "Status" << endl;

	for (int i = 0; i < SIZE; i++)
	{
		result->next();

		cout << right << setw(5) << b + 1 << setw(5) << result->getInt("id") << setw(4) << " " << left << setw(50) << result->getString("company_name");
		cout << setw(4) << " " << left << setw(40) << result->getString("contact_name") << setw(4) << " " << left << setw(7) << result->getString("status") << endl;
		b++;
	}
}
void SupplierController::showDetails(int id)
{
	page.color(15);
	cout << "\nShowing details of Supplier #" << id << endl;
	page.color(7);
	Supplier* s = Supplier::get(id);
	cout << s->displayDetails() << endl;
}