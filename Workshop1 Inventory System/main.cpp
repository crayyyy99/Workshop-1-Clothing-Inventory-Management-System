#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

#include "IndexPage.h"
#include "EmployeeController.h"
#include "SupplierController.h"
#include "ProductController.h"
#include "PurchaseOrderController.h"
#include "TransactionController.h"
#include "ReportController.h"
#include "Database.h"

int main()
{
	Database::connect(); //connect database
	
	IndexPage page;
	EmployeeController employee;
	SupplierController supplier;
	ProductController product;
	PurchaseOrderController purchaseOrder;
	TransactionController transaction;
	ReportController report;

	string logOrReg, userLoginOrReg, userType, userChoice, adminChoice;
	
	page.chgConTitleScrnSize();
	
	bool exit = false;
	bool isValidLoginOrReg = false;
	bool isBackToMainPage = true;

	while (!exit || !isValidLoginOrReg|| isBackToMainPage)
	{
		system("cls");
		page.sysWelcomeHeader();
		page.selectLoginRegisterPanel();

		page.color(14);
		cout << "\nEnter choice: ";
		page.color(7);
		logOrReg.clear();
		cin >> logOrReg;

		//Login
		if (logOrReg == "1")
		{
			isValidLoginOrReg = true;
			bool isValidUserType = false;

			system("cls");
			page.systemHeaderYellow();
			page.userTypePageHeader();
			page.selectUserTypePanel();

			while (!isValidUserType)
			{
				page.color(14);
				cout << "\nEnter choice: ";
				page.color(7);
				userType.clear();
				cin >> userType;

				//Salesperson
				if (userType == "1")
				{
					isValidUserType = true;
					bool login = false;
					login = employee.loginUser();

					if (login)
					{
						bool logout = false;
						string logoutConf;

						while (!logout)
						{
							bool isValidUserChoice = false;

							system("cls");
							page.systemHeaderBlue();
							page.userPanelHeader();

							page.color(14);
							cout << "\nWelcome " << employee.getLoginUsername() << "!";
							page.color(7);
							page.userPanel();

							while (!isValidUserChoice)
							{
								page.color(14);
								cout << "\nEnter choice: ";
								page.color(7);
								userChoice.clear();
								cin >> userChoice;

								//Add Transactions
								if (userChoice == "1")
								{
									isValidUserChoice = true;
									transaction.addTransaction(employee.getLoginId());
								}

								//View or Search Transactions
								else if (userChoice == "2")
								{
									isValidUserChoice = true;
									transaction.viewSearchTransUser(employee.getLoginId());
								}

								//View or Search Products
								else if (userChoice == "3")
								{
									isValidUserChoice = true;
									product.prodSearchViewUser();
								}

								//Edit Account Details
								else if (userChoice == "4")
								{
									isValidUserChoice = true;
									employee.editAccountDetails();
								}

								//Log Out
								else if (userChoice == "5")
								{
									isValidUserChoice = true;
									bool isValidLogoutConfirm = false;

									page.logoutConfirmPanel();

									while (!isValidLogoutConfirm)
									{
										page.color(14);
										cout << "\nEnter choice: ";
										page.color(7);
										logoutConf.clear();
										cin >> logoutConf;

										//Confirm
										if (logoutConf == "Y" || logoutConf == "y")
										{
											isValidLogoutConfirm = true;
											logout = true;
											isBackToMainPage = true;
											page.color(15);
											cout << "\nLogout Successful." << endl;
											cout << endl;
											page.color(7);
											system("pause");
										}
										//Cancel
										else if (logoutConf == "N" || logoutConf == "n")
										{
											isValidLogoutConfirm = true;
										}
										//Invalid Input
										else
											page.showInvalidChoiceMsg();
									}
								}

								//Invalid Input
								else
									page.showInvalidChoiceMsg();
							}
						}
					}
					else
					{
						isBackToMainPage = true;
					}
				}

				//Admin
				else if (userType == "2")
				{
					isValidUserType = true;
					bool login = false;
					login = employee.loginAdmin();

					if (login)
					{
						bool logout = false;
						string logoutConf;

						while (!logout)
						{
							bool isValidAdminChoice = false;

							system("cls");
							page.systemHeaderOrange();
							page.adminPanelHeader();

							page.color(14);
							cout << "\nWelcome " << employee.getLoginUsername() << "!";
							page.adminPanel();

							while (!isValidAdminChoice)
							{
								page.color(14);
								cout << "\nEnter choice: ";
								page.color(7);
								adminChoice.clear();
								cin >> adminChoice;

								//Manage Users
								if (adminChoice == "1")
								{
									isValidAdminChoice = true;
									employee.empManagement();
								}

								//Manage Products
								else if (adminChoice == "2")
								{
									isValidAdminChoice = true;
									product.prodManagementAdmin();
								}

								//Manage Suppliers
								else if (adminChoice == "3")
								{
									isValidAdminChoice = true;
									supplier.suppManagement();
								}

								//Manage Transactions
								else if (adminChoice == "5")
								{
									isValidAdminChoice = true;
									transaction.transManagement();
								}

								//Manage Purchase Orders
								else if (adminChoice == "4")
								{
									isValidAdminChoice = true;
									purchaseOrder.purOrderManagement(employee.getLoginId());
								}

								//View Reports 
								else if (adminChoice == "6")
								{
									isValidAdminChoice = true;
									report.reportManagement();
								}

								//Log Out
								else if (adminChoice == "7")
								{
									isValidAdminChoice = true;
									bool isValidLogoutConfirm = false;
									page.logoutConfirmPanel();

									while (!isValidLogoutConfirm)
									{
										page.color(14);
										cout << "\nEnter choice: ";
										page.color(7);
										logoutConf.clear();
										cin >> logoutConf;

										//Confirm
										if (logoutConf == "Y" || logoutConf == "y")
										{
											isValidLogoutConfirm = true;
											logout = true;
											isBackToMainPage = true;
											page.color(15);
											cout << "\nLogout Successful." << endl;
											cout << endl;
											page.color(7);
											system("pause");
										}
										//Cancel
										else if (logoutConf == "N" || logoutConf == "n")
										{
											isValidLogoutConfirm = true;
										}
										//Invalid Input
										else
											page.showInvalidChoiceMsg();
									}
								}

								//Invalid Input
								else
									page.showInvalidChoiceMsg();
							}
						}
					}
					else
						isBackToMainPage = true;
				}

				//Back
				else if (userType == "3")
				{
					isValidUserType = true;
					isBackToMainPage = true;
				}

				//Invalid Input
				else
					page.showInvalidChoiceMsg();
			}
		}

		//Register
		else if (logOrReg == "2")
		{
			isValidLoginOrReg = true;
			employee.registerNewUser();

			isBackToMainPage = true;
		}

		//Exit
		else if (logOrReg == "3")
		{
			isValidLoginOrReg = true;
			bool isValidExitConfirm = false;
			string exitConf;

			page.exitConfirmPanel();

			while (!isValidExitConfirm)
			{
				page.color(14);
				cout << "\nEnter choice: ";
				page.color(7);
				exitConf.clear();
				cin >> exitConf;

				//Confirm
				if (exitConf == "Y" || exitConf == "y")
				{
					isValidExitConfirm = true;
					exit = true;
					isBackToMainPage = false;
				}
				//Cancel
				else if (exitConf == "N" || exitConf == "n")
				{
					isValidExitConfirm = true;
					isBackToMainPage = true;
				}
				//Invalid Input
				else
					page.showInvalidChoiceMsg();
			}
		}

		//Invalid Input
		else
			page.showInvalidChoiceMsg();			
	}
		//Exit Program
		if (exit == true)
		{
			Database::disconnect(); //disconnect database
			system("cls");
			return 0;
		}				
}
