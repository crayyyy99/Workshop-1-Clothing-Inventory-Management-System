#include <iostream>
#include <windows.h>
#include <iomanip>
using namespace std;

#include "IndexPage.h"

void IndexPage::color(unsigned short color)
{
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

void IndexPage::chgConTitleScrnSize()
{
	SetConsoleTitle(TEXT("Blossom Fashion's Inventory Management System"));
	//system("mode 650");
}

//Header
void IndexPage::sysWelcomeHeader()
{
	color(228);
	cout << char(201);      for (int i = 0; i < 58; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << char(186) << setw(58) << " " << char(186) << endl;
	cout << char(186) << " Welcome to Blossom Fashion's Inventory Management System " << char(186) << endl;
	cout << char(186) << setw(58) << " " << char(186) << endl;
	cout << char(200);      for (int i = 0; i < 58; i++) { cout << char(205); }      cout << char(188) << endl;
}

void IndexPage::systemHeaderYellow()
{
	color(228);
	cout << char(201);      for (int i = 0; i < 58; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << char(186) << left << setw(58) << "      Blossom Fashion's Inventory Management System " << char(186) << endl;
}

void IndexPage::systemHeaderOrange()
{
	color(207);
	cout << char(201);      for (int i = 0; i < 58; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << char(186) << left << setw(58) << "      Blossom Fashion's Inventory Management System " << char(186) << endl;
}

void IndexPage::systemHeaderBlue()
{
	color(31);
	cout << char(201);      for (int i = 0; i < 58; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << char(186) << left << setw(58) << "      Blossom Fashion's Inventory Management System " << char(186) << endl;
}

void IndexPage::userTypePageHeader()
{
	cout << char(186) << left << setw(58) << "                        User Type" << char(186) << endl;
	cout << char(200);      for (int i = 0; i < 58; i++) { cout << char(205); }      cout << char(188) << endl;
	color(7);
}

//EMPLOYEE
//Header
void IndexPage::userLoginPageHeader()
{
	cout << char(186) << left << setw(58) << "                       User Login" << char(186) << endl;
	cout << char(200);      for (int i = 0; i < 58; i++) { cout << char(205); }      cout << char(188) << endl;
	color(7);
}

void IndexPage::adminLoginPageHeader()
{
	cout << char(186) << left << setw(58) << "                       Admin Login" << char(186) << endl;
	cout << char(200);      for (int i = 0; i < 58; i++) { cout << char(205); }      cout << char(188)  << endl;
	color(7);
}

void IndexPage::registrationHeader()
{
	cout << char(186) << left << setw(58) << "                   New User Registration" << char(186) << endl;
	cout << char(200);      for (int i = 0; i < 58; i++) { cout << char(205); }      cout << char(188) << "\n" << endl;
	color(7);
}

void IndexPage::userPanelHeader()
{
	cout << char(186) << left << setw(58) << "                        User Panel" << char(186) << endl;
	cout << char(200);      for (int i = 0; i < 58; i++) { cout << char(205); }      cout << char(188) << endl;
	color(7);
}

void IndexPage::adminPanelHeader()
{
	cout << char(186) << left << setw(58) << "                       Admin Panel" << char(186) << endl;
	cout << char(200);      for (int i = 0; i < 58; i++) { cout << char(205); }      cout << char(188) << endl;
	color(7);
}

void IndexPage::editAccDetailsHeader()
{
	cout << char(186) << left << setw(58) << "                   Edit Account Details" << char(186) << endl;
	cout << char(200);      for (int i = 0; i < 58; i++) { cout << char(205); }      cout << char(188) << endl;
	color(7);
}

void IndexPage::empMngmtHeader()
{
	cout << char(186) << left << setw(58) << "                     Manage Employees" << char(186) << endl;
	cout << char(200);      for (int i = 0; i < 58; i++) { cout << char(205); }      cout << char(188) << endl;
	color(7);
}

void IndexPage::editUserDetailsHeader()
{
	cout << char(186) << left << setw(58) << "                     Edit User Details" << char(186) << endl;
	cout << char(200);      for (int i = 0; i < 58; i++) { cout << char(205); }      cout << char(188) << endl;
	color(7);
}

//Panel
void IndexPage::selectLoginRegisterPanel()
{
	color(15);
	cout << "\nFor first-time user, please register your account." << endl;

	color(11);
	cout << "\nPlease select a choice." << endl;
	cout << setw(5) << right << char(201);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  1 - Login " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  2 - Register " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  3 - Exit" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(188) << endl;

	
}

void IndexPage::selectUserTypePanel()
{
	color(15);
	cout << "\nPlease select user type." << endl;
	color(11);
	cout << setw(5) << right << char(201);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  1 - Salesperson" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  2 - Admin" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  3 - Back " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(188) << endl;
}

void IndexPage::exitConfirmPanel()
{
	color(15);
	cout << "\nAre you sure you want to exit?" << endl;

	color(11);
	cout << setw(5) << right << char(201);      for (int i = 0; i < 26; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(26) << left << "  Y/y - Yes     N/n - No" << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 26; i++) { cout << char(205); }      cout << char(188) << endl;
}

void IndexPage::logoutConfirmPanel()
{
	color(15);
	cout << "\nAre you sure you want to logout?" << endl;

	color(11);
	cout << setw(5) << right << char(201);      for (int i = 0; i < 26; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(26) << left << "  Y/y - Yes     N/n - No" << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 26; i++) { cout << char(205); }      cout << char(188) << endl;
}

void IndexPage::registrationConfirmPanel()
{
	color(15);
	cout << "Do you confirm?" << endl;

	color(11);
	cout << setw(5) << right << char(201);      for (int i = 0; i < 26; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(26) << left << "  Y/y - Yes     N/n - No" << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 26; i++) { cout << char(205); }      cout << char(188) << endl;
}

void IndexPage::userPanel()
{
	color(7);
	cout << endl;

	color(15);
	cout << "\nPlease select a choice." << endl;
	color(11);
	cout << setw(5) << right << char(201);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  1 - Add Transactions" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  2 - View or Search Transactions Made" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  3 - View or Search Products" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  4 - Edit Account Details" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  5 - Logout" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(188) << endl;

}

void IndexPage::adminPanel()
{
	color(7);
	cout << endl;

	color(15);
	cout << "\nPlease select a choice." << endl;
	color(11);
	cout << setw(5) << right << char(201);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  1 - Manage Employees" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  2 - Manage Products" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  3 - Manage Suppliers " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  4 - Manage Purchase Orders " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  5 - Manage Transactions " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  6 - View Reports   " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  7 - Logout" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(188) << endl;
}

void IndexPage::editAccDetailsPanelUser()
{
	color(7);
	cout << endl;

	color(15);
	cout << "\nPlease select a choice to edit." << endl;
	color(11);
	cout << setw(5) << right << char(201);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  1 - Name" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  2 - IC Number" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  3 - Gender" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  4 - Date of Birth" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  5 - Contact Number" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  6 - Email Address" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  7 - Home Address" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  8 - Change Password" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  9 - Back" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(188) << endl;

}

void IndexPage::empMngmtPanel()
{
	color(15);
	cout << "\nPlease select a choice." << endl;
	color(11);
	cout << setw(5) << right << char(201);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  1 - Previous Page" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  2 - Next Page" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  3 - Search Employee" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  4 - View Employee Details and Modify" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  5 - Delete Employee" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  6 - Back " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(188) << endl;


}

void IndexPage::modifyEmpPanel()
{
	color(7);
	cout << endl;

	color(15);
	cout << "\nPlease select a choice to modify." << endl;
	color(11);
	cout << setw(5) << right << char(201);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  1 - Position" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  2 - Permission" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  3 - Reset Password" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  4 - Back" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(188) << endl;

	
}

//SUPPLIER
void IndexPage::suppMngmtHeader()
{
	cout << char(186) << left << setw(58) << "                    Manage Suppliers" << char(186) << endl;
	cout << char(200);      for (int i = 0; i < 58; i++) { cout << char(205); }      cout << char(188) << endl;
	color(7);
}

void IndexPage::suppMngmtPanel()
{
	color(15);
	cout << "\nPlease select a choice." << endl;
	color(11);
	cout << setw(5) << right << char(201);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  1 - Previous Page" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  2 - Next Page" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  3 - Add New Supplier" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  4 - Search Supplier" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  5 - View Supplier Details and Modify" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  6 - Delete Supplier" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  7 - Back " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(188) << endl;

	
}

void IndexPage::modifySuppPanel()
{
	color(7);
	cout << endl;

	color(15);
	cout << "\nPlease select a choice to modify." << endl;
	color(11);
	cout << setw(5) << right << char(201);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  1 - Company Name" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  2 - Contact Name" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  3 - Contact Title" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  4 - Company Email Address" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  5 - Company Address" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  6 - Contact Number 1" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  7 - Contact Number 2" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  8 - Other Details" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  9 - Back" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(188) << endl;

}

//Product
void IndexPage::prodMngmtHeader()
{
	cout << char(186) << left << setw(58) << "                    Manage Products" << char(186) << endl;
	cout << char(200);      for (int i = 0; i < 58; i++) { cout << char(205); }      cout << char(188) << endl;
	color(7);
}

void IndexPage::prodMngmtPanel()
{
	color(15);
	cout << "\nPlease select a choice." << endl;
	color(11);
	cout << setw(5) << right << char(201);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  1 - Previous Page" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  2 - Next Page" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  3 - Add New Product" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  4 - Search Product" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  5 - View Product Details and Modify" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  6 - Delete Product" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  7 - View Low Stock Product List" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  8 - Back " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(188) << endl;

}

void IndexPage::addSizePanel()
{
	color(15);
	cout << "\nPlease select a choice." << endl;
	color(11);
	cout << setw(5) << right << char(201);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  1 - This product is free size." << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  2 - This product has different sizes." << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(188) << endl;

}

void IndexPage::modifyProdPanel()
{
	color(7);
	cout << endl;

	color(15);
	cout << "\nPlease select a choice to modify." << endl;
	color(11);
	cout << setw(5) << right << char(201);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  1 - Name" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  2 - Colour" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  3 - Gender" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  4 - Type" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  5 - Description" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  6 - Unit Price" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  7 - Add Size" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  8 - Delete Size" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  9 - Back" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(188) << endl;

}

void IndexPage::prodSearchViewHeaderUser()
{
	cout << char(186) << left << setw(58) << "                 Search and View Products" << char(186) << endl;
	cout << char(200);      for (int i = 0; i < 58; i++) { cout << char(205); }      cout << char(188) << endl;
	color(7);
}

void IndexPage::prodSearchViewPanelUser()
{
	color(15);
	cout << "\nPlease select a choice." << endl;
	color(11);
	cout << setw(5) << right << char(201);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  1 - Previous Page" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  2 - Next Page" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  3 - Search Product" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  4 - View Product Details" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  5 - Back " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(188) << endl;
}

//Purchase Order
void IndexPage::purOrderMngmtHeader()
{
	cout << char(186) << left << setw(58) << "                   Manage Purchase Order" << char(186) << endl;
	cout << char(200);      for (int i = 0; i < 58; i++) { cout << char(205); }      cout << char(188) << endl;
	color(7);
}

void IndexPage::purOrderMngmtPanel()
{
	color(15);
	cout << "\nPlease select a choice." << endl;
	color(11);
	cout << setw(5) << right << char(201);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  1 - Previous Page" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  2 - Next Page" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  3 - Add Purchase Order" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  4 - Search Purchase Order" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  5 - View Purchase Order Details" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  6 - Back " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(188) << endl;

}

void IndexPage::searchPurOrderPanel()
{
	color(15);
	cout << "\nPlease select a choice to search." << endl;
	color(11);
	cout << setw(5) << right << char(201);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  1 - Search By Date" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  2 - Search By Employee Name" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  3 - Search By Supplier Company" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(188) << endl;
}


//Transaction
void IndexPage::addtransHeader() 
{
	cout << char(186) << left << setw(58) << "                     Add Transaction" << char(186) << endl;
	cout << char(200);      for (int i = 0; i < 58; i++) { cout << char(205); }      cout << char(188) << endl;
	color(7);
}
void IndexPage::transMngmtHeader()
{
	cout << char(186) << left << setw(58) << "                    Manage Transaction" << char(186) << endl;
	cout << char(200);      for (int i = 0; i < 58; i++) { cout << char(205); }      cout << char(188) << endl;
	color(7);
}

void IndexPage::transMngmtPanel()
{
	color(15);
	cout << "\nPlease select a choice." << endl;
	color(11);
	cout << setw(5) << right << char(201);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  1 - Previous Page" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  2 - Next Page" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  3 - Search Transaction" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  4 - View Transaction Details" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  5 - Manage Refund Item" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  6 - Back " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(188) << endl;
}

void IndexPage::searchTransPanelAdmin()
{
	color(15);
	cout << "\nPlease select a choice to search." << endl;
	color(11);
	cout << setw(5) << right << char(201);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  1 - Search By Date" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  2 - Search By Employee Name" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  3 - Search By Employee Name and Date" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(188) << endl;
}

void IndexPage::transSearchViewPanelUser()
{
	color(15);
	cout << "\nPlease select a choice." << endl;
	color(11);
	cout << setw(5) << right << char(201);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  1 - Previous Page" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  2 - Next Page" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  3 - Search Previous Transaction Made" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  4 - View Transaction Details" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  5 - Back " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(188) << endl;
}

void IndexPage::transSearchViewHeaderUser()
{
	cout << char(186) << left << setw(58) << "             Search and View Transaction Made" << char(186) << endl;
	cout << char(200);      for (int i = 0; i < 58; i++) { cout << char(205); }      cout << char(188) << endl;
	color(7);
}

//Report
void IndexPage::viewReportHeader()
{
	cout << char(186) << left << setw(58) << "                      View Reports" << char(186) << endl;
	cout << char(200);      for (int i = 0; i < 58; i++) { cout << char(205); }      cout << char(188) << endl;
	color(7);
}
void IndexPage::viewReportTypePanel()
{
	color(15);
	cout << "\nPlease select a report type." << endl;
	color(11);
	cout << setw(5) << right << char(201);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  1 - Sales Report " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  2 - Best Sellers Report " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  3 - Back " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(188) << endl;
}

void IndexPage::viewReportTimePanel()
{
	color(15);

	cout << "\nPlease select a choice." << endl;
	color(11);
	cout << setw(5) << right << char(201);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  1 - Daily Report" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  2 - Monthly Report" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  3 - Annual Report" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  4 - Total" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  5 - Back " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(188) << endl;
}

void IndexPage::viewSalesReportHeader()
{
	cout << char(186) << left << setw(58) << "                       Sales Report" << char(186) << endl;
	cout << char(200);      for (int i = 0; i < 58; i++) { cout << char(205); }      cout << char(188) << endl;
	color(7);
}

void IndexPage::viewBestSellerReportHeader()
{
	cout << char(186) << left << setw(58) << "                    Best Seller Report" << char(186) << endl;
	cout << char(200);      for (int i = 0; i < 58; i++) { cout << char(205); }      cout << char(188) << endl;
	color(7);
}

//Message

void IndexPage::showYesNoChoice()
{
	color(11);
	cout << setw(5) << right << char(201);      for (int i = 0; i < 26; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(26) << left << "  Y/y - Yes     N/n - No" << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 26; i++) { cout << char(205); }      cout << char(188) << endl;
}

void IndexPage::showInvalidChoiceMsg()
{
	color(12);
	cout << "\nInvalid input!!" << endl;
}

void IndexPage::showSearchDateFormat()
{
	color(15);
	cout << endl;
	cout << setw(5) << right << char(201);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(187) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  SEARCH FORMAT" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  For Specific Date  : YYYY-MM-DD" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  For Specific Month : YYYY-MM" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << left << "  For Specific Year : YYYY" << char(186) << endl;
	cout << setw(5) << right << char(186) << setw(51) << " " << char(186) << endl;
	cout << setw(5) << right << char(200);      for (int i = 0; i < 51; i++) { cout << char(205); }      cout << char(188) << endl;
}





