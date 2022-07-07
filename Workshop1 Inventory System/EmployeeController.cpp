#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <iomanip>
#include <conio.h>

using namespace std;

#include <mysql/jdbc.h>
using namespace sql;

#include "EmployeeController.h"
#include "DatabaseConnection.h"
#include "Employee.h"
#include "IndexPage.h"
#include "CheckInput.h"
#include "sha256.h"

bool EmployeeController::loginUser()
{
	bool quitLogin = false;
	bool loginResult = false;
	string username, password;

	while (!quitLogin)
	{
		system("cls");
		page.systemHeaderBlue();
		page.userLoginPageHeader();

		page.color(14);
		cout << setw(20) << "\nUsername" << ": ";
		page.color(7);
		username.clear();
		cin.ignore(1, '\n');
		getline(cin, username);

		page.color(14);
		cout << setw(20) << "\nPassword" << ": ";
		page.color(7);
		password.clear();
		char pwd;
		while ((pwd = _getch()) != 13) //ASCII 13 carriage return
		{
			password += pwd;
			cout << '*';
		}

		password = sha256(password);

		loginResult = validateUserLogin(username, password);

		if (loginResult)
		{
			bool accountActive = false;
			accountActive = validateAccountActive(username, password);
			if (accountActive)
			{
				setLoginUsername(username);
				readLoginId(username);
				page.color(15);
				cout << endl;
				cout << "\nLogin Successful." << endl;
				cout << endl;
				page.color(7);
				system("pause");
				return true;
			}
			else
			{
				page.color(12);
				cout << "\n\nYour account has been disabled!\n" << endl;
				page.color(7);
				system("pause");
				quitLogin = true;
				return false;
			}
		}
		else
		{
			bool isValidTryAgain = false;
			string tryAgain;

			page.color(12);
			cout << "\n\nLogin Error: Incorrect Username or Password";
			cout << "\nPlease check your username and password!" << endl;

			page.color(15);
			cout << "\nDo you want to try again?" << endl;
			page.showYesNoChoice();
			
			while (!isValidTryAgain)
			{
				page.color(14);
				cout << "\nEnter choice: ";
				page.color(7);
				tryAgain.clear();
				cin >> tryAgain;

				//Try Again
				if (tryAgain == "Y" || tryAgain == "y")
				{
					isValidTryAgain = true;
					quitLogin = false;
				}

				//Quit Login
				else if (tryAgain == "N" || tryAgain == "n")
				{
					isValidTryAgain = true;
					quitLogin = true;
					return false;
				}

				//Invalid Input
				else
					page.showInvalidChoiceMsg();
			}
		}
	}
}
bool EmployeeController::loginAdmin()
{
	bool quitLogin = false;
	bool loginResult = false;
	string username, password;

	while (!quitLogin)
	{
		system("cls");
		page.systemHeaderOrange();
		page.adminLoginPageHeader();

		page.color(14);
		cout << setw(20) << "\nUsername" << ": ";
		page.color(7);
		username.clear();
		cin.ignore(1, '\n');
		getline(cin, username);

		page.color(14);
		cout << setw(20) << "\nPassword" << ": ";
		page.color(7);

		password.clear();
		char pwd;
		while ((pwd = _getch()) != 13) //ASCII 13 carriage return
		{
			password += pwd;
			cout << '*';
		}

		password = sha256(password);

		loginResult = validateUserLogin(username, password);

		if (loginResult)
		{
			bool accountActive = false;
			accountActive = validateAccountActive(username, password);
			if (accountActive)
			{
				bool isAdminLogin = false;
				isAdminLogin = validateAdminLogin(username, password);

				if (isAdminLogin)
				{
					setLoginUsername(username);
					readLoginId(username);
					page.color(15);
					cout << endl;
					cout << "\nLogin Successful." << endl;
					cout << endl;
					page.color(7);
					system("pause");
					return true;
				}
				else
				{
					page.color(15);
					cout << endl;
					cout << "\nYour account does not have permission to access Admin Panel.\nTo access Admin Panel, please inform Admin to change your account permission." << endl;
					cout << endl;
					page.color(7);
					system("pause");
					return false;
				}
			}
			else
			{
				page.color(12);
				cout << "\n\nYour account has been disabled!\n" << endl;
				page.color(7);
				system("pause");
				quitLogin = true;
				return false;
			}			
		}
		else
		{
			bool isValidTryAgain = false;
			string tryAgain;

			page.color(12);
			cout << "\n\nLogin Error: Incorrect Username or Password";
			cout << "\nPlease check your username and password!" << endl;
			page.color(15);
			cout << "\nDo you want to try again?" << endl;
			page.showYesNoChoice();

			while (!isValidTryAgain)
			{
				page.color(14);
				cout << "\nEnter choice: ";
				page.color(7);
				tryAgain.clear();
				cin >> tryAgain;

				//Try Again
				if (tryAgain == "Y" || tryAgain == "y")
				{
					isValidTryAgain = true;
					quitLogin = false;
				}

				//Quit Login
				else if (tryAgain == "N" || tryAgain == "n")
				{
					isValidTryAgain = true;
					quitLogin = true;
					return false;
				}

				//Invalid Input
				else
					page.showInvalidChoiceMsg();
			}
		}
	}
}

void EmployeeController::registerNewUser()
{
	Employee regDetails;
	bool quitRegistration = false;

	while (!quitRegistration)
	{
		system("cls");
		page.systemHeaderYellow();
		page.registrationHeader();

		string name, ic, gender, dob, position, contactNo, email, homeAddress, username, password, reentered, hashedpwd;
		bool isCancelLoop = false;
		bool isValidPassword = false;
		bool isValidName = false;
		bool isValidIc = false;
		bool isValidDOB = false;
		bool isValidGender = false;
		bool isValidContactNo = false;
		bool isValidPosition = false;
		bool isValidEmail = false;
		bool isValidHomeAd = false;
		
		if (Employee::count() == 0)
		{
			page.color(11);
			cout << "Your account is the first account registered in this system and will be set as admin account." << endl;
			cout << "\n" << endl;
			regDetails.resetAutoIncrement();
			
		}

		page.color(14);
		cout << "Account Information" << endl;
		for (int i = 0; i < 20; i++) { cout << char(205); }
		page.color(15);
		cout << "\nPlease enter account username and password. The username and passwords must have at least 8 characters." << endl;
		int i = 0;
		while(!isCancelLoop)
		{
			page.color(14);
			cout << setw(20) << "\nUsername " << ": ";
			page.color(7);
			username.clear();
			if (i == 0)
			{
				cin.ignore(1, '\n');
				i++;
			}
			getline(cin, username);

			bool isValidUsername = usernameExist(username);

			if (isValidUsername)
			{
				page.color(12);
				cout << "\nUsername already exists. Try another." << endl;
			}
			else if (!isValidUsername)
			{
				if (username.length() < 8)
				{
					page.color(12);
					cout << "\nUsername must have at least 8 characters. Please try another username." << endl;
				}
				else
				{
					isCancelLoop = true;
					regDetails.setUsername(username);
				}					
			}
		} 

		while (!isValidPassword)
		{
			page.color(14);
			cout << setw(20) << "\nPassword" << ": ";
			page.color(7);
			password.clear();
			char pwd;
			while ((pwd = _getch()) != 13) //ASCII 13 carriage return3
			{
				password += pwd;
				cout << '*';
			}

			if (password.length() < 8)
			{
				page.color(12);
				cout << "\n\nPasswords must have at least 8 characters. Please try another password." << endl;
				page.color(7);

				password.clear();
			}
			else if (password.length() >= 8)
			{
				page.color(14);
				cout << endl;
				cout << setw(20) << "\nRe-enter Password" << ": ";
				page.color(7);
				reentered.clear();
				char repwd;
				while ((repwd = _getch()) != 13) //ASCII 13 carriage return
				{
					reentered += repwd;
					cout << '*';
				}

				if (password != reentered)
				{
					page.color(12);
					cout << "\n\nPlease make sure your passwords match." << endl;
					page.color(7);

					password.clear();
					reentered.clear();
				}
				else
				{
					isValidPassword = true;
					hashedpwd = sha256(reentered);
					regDetails.setPassword(hashedpwd);
				}
			}
		}

		cout << "\n\n" << endl;
		page.color(7);
		system("pause");

		system("cls");
		page.systemHeaderYellow();
		page.registrationHeader();

		page.color(14);
		cout << "User Information" << endl;
		for (int i = 0; i < 17; i++) { cout << char(205); }

		while (!isValidName)
		{
			page.color(14);
			cout << setw(20) << "\nName" << ": ";
			page.color(7);
			name.clear();
			getline(cin, name);

			isValidName = input.isNullOrWhiteSpace(name);

			if (!isValidName)
			{
				page.color(12);
				cout << "\nPlease fill out this field. " << endl;
			}
			else
				regDetails.setName(name);
		}

		page.color(15);
		cout << "\n\nFor IC Number, please key in your 12-digit IC number without dash, \"-\"." << endl;
		while (!isValidIc)
		{
			page.color(14);
			cout << setw(20) << "\nIC Number" << ": ";
			page.color(7);
			ic.clear();
			getline(cin, ic);

			isValidIc = input.isValidIc(ic);

			if (!isValidIc)
			{
				page.color(12);
				cout << "\nInvalid format. IC number must in 12-digit format without dashes. Please try again." << endl;
			}
			else
				regDetails.setIc(ic);
		}

		page.color(15);
		cout << "\n\nFor Gender, please enter \"Male\" or \"Female\"." << endl;
		while (!isValidGender)
		{
			page.color(14);
			cout << setw(20) << "\nGender" << ": ";
			page.color(7);
			gender.clear();
			getline(cin, gender);

			isValidGender = input.isValidGender(gender);

			if (!isValidGender)
			{
				page.color(12);
				cout << "\nInvalid format. Please enter \"Male\" or \"Female\" as your input." << endl;
			}
			else
				regDetails.setGender(gender);
		}

		page.color(15);
		cout << "\n\nFor Date of Birth, please enter your date of birth in the format of YYYY-MM-DD. [Y-year, M-month, D-day]" << endl;
		while (!isValidDOB)
		{
			page.color(14);
			cout << setw(20) << "\nDate Of Birth" << ": ";
			page.color(7);
			dob.clear();
			getline(cin, dob);

			isValidDOB = input.isValidDate(dob);

			if (!isValidDOB)
			{
				page.color(12);
				cout << "\nInvalid format. The format must in YYYY-MM-DD. Please try again." << endl;
			}
			else
				regDetails.setDob(dob);
		}

		page.color(15);
		cout << "\n\nFor contact number, please enter your contact number with dash,\"-\"." << endl;
		while (!isValidContactNo)
		{
			page.color(14);
			cout << setw(20) << "\nContact Number" << ": ";
			page.color(7);
			contactNo.clear();
			getline(cin, contactNo);

			isValidContactNo = input.isValidContactNo(contactNo);

			if (!isValidContactNo)
			{
				page.color(12);
				cout << "\nInvalid format. The contact number must contains dash,'-'. Please try again." << endl;
			}
			else
				regDetails.setContactNo(contactNo);
		}

		while (!isValidEmail)
		{
			page.color(14);
			cout << endl;
			cout << setw(20) << "\nE-mail address" << ": ";
			page.color(7);
			email.clear();
			getline(cin, email);

			isValidEmail = input.isValidEmail(email);

			if (!isValidEmail)
			{
				page.color(12);
				cout << "\nInvalid E-mail format. Please try again.";
			}
			else
				regDetails.setEmail(email);
		}

		while (!isValidHomeAd)
		{
			page.color(14);
			cout << endl;

			cout << setw(20) << "\nHome Address" << ": ";
			page.color(7);
			homeAddress.clear();
			getline(cin, homeAddress);

			isValidHomeAd = input.isNullOrWhiteSpace(homeAddress);

			if (!isValidHomeAd)
			{
				page.color(12);
				cout << "\nPlease fill out this field. ";
			}
			else
				regDetails.setHomeAddress(homeAddress);
		}

		cout << "\n" << endl;
		page.color(7);
		system("pause");

		system("cls");
		page.systemHeaderYellow();
		page.registrationHeader();

		page.color(15);
		cout << "Please validate your account details." << endl;
		page.color(14);
		cout << "\nAccount Information" << endl;
		for (int i = 0; i < 20; i++) { cout << char(205); }
		page.color(7);
		cout << setw(20) << "\nUsername" << ": " << regDetails.getUsername() << endl;
		cout << setw(20) << "\nPassword" << ": ";
		cout << password[0] << password[1];
		for (int i = 2; i < password.length(); i++)
		{
			cout << "*";
		}
		page.color(14);
		cout << "\n\nUser Information" << endl;
		for (int i = 0; i < 17; i++) { cout << char(205); }
		page.color(7);
		cout << setw(20) << "\nName" << ": " << regDetails.getName() << endl;
		cout << setw(20) << "\nIC Number" << ": " << regDetails.getIc() << endl;
		cout << setw(20) << "\nGender" << ": " << regDetails.getGender() << endl;
		cout << setw(20) << "\nDate of Birth" << ": " << regDetails.getDob() << endl;
		cout << setw(20) << "\nContact Number" << ": " << regDetails.getContactNo() << endl;
		cout << setw(20) << "\nE-mail Address" << ": " << regDetails.getEmail() << endl;
		cout << setw(20) << "\nHome Address" << ": " << regDetails.getHomeAddress() << endl;
		cout << "\n" << endl;

		bool isValidConfirmReg = false;
		string confirmReg, tryAgain;
		page.registrationConfirmPanel();
		
		while (!isValidConfirmReg)
		{
			page.color(14);
			cout << "\nEnter choice: ";
			page.color(7);
			confirmReg.clear();
			cin >> confirmReg;

			//Confirm Details
			if (confirmReg == "Y"||confirmReg=="y")
			{
				
				isValidConfirmReg = true;

				int count = regDetails.count();

				if (count == 0)
				{
					Employee* e = new Employee();

					e->setName(regDetails.getName());
					e->setIc(regDetails.getIc());
					e->setGender(regDetails.getGender());
					e->setDob(regDetails.getDob());
					e->setPosition("Owner");
					e->setContactNo(regDetails.getContactNo());
					e->setEmail(regDetails.getEmail());
					e->setHomeAddress(regDetails.getHomeAddress());
					e->setPermission("admin");
					e->setUsername(regDetails.getUsername());
					e->setPassword(regDetails.getPassword());

					e->save();
				}
				else
				{
					Employee* e = new Employee();

					e->setName(regDetails.getName());
					e->setIc(regDetails.getIc());
					e->setGender(regDetails.getGender());
					e->setDob(regDetails.getDob());
					e->setContactNo(regDetails.getContactNo());
					e->setEmail(regDetails.getEmail());
					e->setHomeAddress(regDetails.getHomeAddress());
					e->setUsername(regDetails.getUsername());
					e->setPassword(regDetails.getPassword());

					e->save();
				}

				quitRegistration = true;
				page.color(15);
				cout << "\nRegistration Successful. Please Login." << endl;
				cout << endl;
				page.color(7);
				system("pause");
			}

			//Cancel Registration
			else if (confirmReg == "N" || confirmReg == "n")
			{
				isValidConfirmReg = true;
				bool isValidTryAgain = false;

				page.color(15);
				cout << "\nDo you want to try again?" << endl;
				page.showYesNoChoice();
				
				while (!isValidTryAgain)
				{
					page.color(14);
					cout << "\nEnter choice: ";
					page.color(7);
					tryAgain.clear();
					cin >> tryAgain;

					//Try Again
					if (tryAgain == "Y" || tryAgain == "y")
					{
						isValidTryAgain = true;
						quitRegistration = false;
					}

					//Quit Registration
					else if (tryAgain == "N" || tryAgain == "n")
					{
						isValidTryAgain = true;
						quitRegistration = true;

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

void EmployeeController:: setLoginUsername(string n){
	loginUsername = n;
}
string EmployeeController::getLoginUsername(){
	return loginUsername;
}

void EmployeeController::readLoginId(string username)
{
	string str = "SELECT * FROM employees WHERE username = ?";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setString(1, username);
	ResultSet* result = ps->executeQuery();
	for (int i = 0; i < result->rowsCount(); i++)
	{
		result->next();
		setLoginId(result->getInt("id"));
	}
}
void EmployeeController::setLoginId(int n) {
	loginId = n;
}
int EmployeeController::getLoginId() {
	return loginId;
}


void EmployeeController::editAccountDetails()
{
	bool isBack = false;
	string editChoice;

	while (!isBack)
	{
		system("cls");
		page.systemHeaderBlue();
		page.editAccDetailsHeader();

		page.color(14);
		cout << "\n My Account Details " << endl;
		for (int i = 0; i < 21; i++) { cout << char(205); }
		page.color(7);

		Employee* e = Employee::get(getLoginUsername()); //READ (SELECT * FROM employees WHERE username = ? )
		if (e != nullptr)
			cout << e->displayAccDetails() << endl;

		page.editAccDetailsPanelUser();

		bool isValidEditChoice = false;
		

		while (!isValidEditChoice)
		{
			page.color(14);
			cout << "\nEnter choice: ";
			page.color(7);
			editChoice.clear();
			cin >> editChoice;

			//Edit Name
			if (editChoice == "1")
			{
				isValidEditChoice = true;
				bool isValidName = false;
				string name;
				int i = 0;
				while (!isValidName)
				{
					page.color(14);
					cout << setw(20) << "\nNew Name" << ": ";
					page.color(7);
					name.clear();
					if (i == 0)
					{
						cin.ignore(1, '\n');
						i++;
					}
					getline(cin, name);

					isValidName = input.isNullOrWhiteSpace(name);

					if (!isValidName)
					{
						page.color(12);
						cout << "\nPlease fill out this field. " << endl;
					}
					else
					{
						e->setName(name);
						e->save();

						page.color(15);
						cout << "\nName successfully updated." << endl;
						cout << endl;
						page.color(7);
						system("pause");
					}
				}
			}
			//Edit IC Number			
			else if (editChoice == "2")
			{
				isValidEditChoice = true;
				bool isValidIc = false;
				page.color(15);
				cout << "\nPlease enter your 12-digit IC number without dash, \"-\"." << endl;
				string ic;
				int i = 0;

				while (!isValidIc)
				{
					page.color(14);
					cout << setw(20) << "\nNew IC Number" << ": ";
					page.color(7);
					ic.clear();
					if (i == 0)
					{
						cin.ignore(1, '\n');
						i++;
					}
					getline(cin, ic);

					isValidIc = input.isValidIc(ic);
					if (!isValidIc)
					{
						page.color(12);
						cout << "\nInvalid format. IC number must in 12-digit format without dashes. Please try again." << endl;
					}
					else
					{
						e->setIc(ic);
						e->save();

						page.color(15);
						cout << "\nIC number successfully updated." << endl;
						cout << endl;
						page.color(7);
						system("pause");
					}
				}
			}
			//Edit Gender
			else if (editChoice == "3")
			{
				isValidEditChoice = true;
				bool isValidGender = false;
				page.color(15);
				cout << "\nPlease enter \"Male\" or \"Female\"." << endl;
				string gender;
				int i = 0;
				while (!isValidGender)
				{
					page.color(14);
					cout << setw(20) << "\nGender" << ": ";
					page.color(7);
					gender.clear();
					if (i == 0)
					{
						cin.ignore(1, '\n');
						i++;
					}
					getline(cin, gender);

					isValidGender = input.isValidGender(gender);
					if (!isValidGender)
					{
						page.color(12);
						cout << "\nInvalid format. Please enter \"Male\" or \"Female\" as your input." << endl;
					}
					else
					{
						e->setGender(gender);
						e->save();

						page.color(15);
						cout << "\nGender successfully updated." << endl;
						cout << endl;
						page.color(7);
						system("pause");
					}
				}
			}
			//Edit DOB
			else if (editChoice == "4")
			{
				isValidEditChoice = true;
				bool isValidDOB = false;
				page.color(15);
				cout << "\nPlease enter your date of birth in the format of YYYY-MM-DD. [Y-year, M-month, D-day]" << endl;
				string dob;

				int i = 0;
				while (!isValidDOB)
				{
					page.color(14);
					cout << setw(20) << "\nNew Date Of Birth" << ": ";
					page.color(7);
					dob.clear();
					if (i == 0)
					{
						cin.ignore(1, '\n');
						i++;
					}
					getline(cin, dob);

					isValidDOB = input.isValidDate(dob);

					if (!isValidDOB)
					{
						page.color(12);
						cout << "\nInvalid format. The format must in YYYY-MM-DD. Please try again." << endl;
					}

					else
					{
						e->setDob(dob);
						e->save();

						page.color(15);
						cout << "\nDate of Birth successfully updated." << endl;
						cout << endl;
						page.color(7);
						system("pause");
					}
				}
			}
			//Edit ContactNo
			else if (editChoice == "5")
			{
				isValidEditChoice = true;
				bool isValidContactNo = false;
				string contactNo;
				int i = 0;
				page.color(15);
				cout << "\nFor contact number, please enter your contact number with dash,'-'" << endl;
				while (!isValidContactNo)
				{
					page.color(14);
					cout << setw(20) << "\nNew Contact Number" << ": ";
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
						e->setContactNo(contactNo);
						e->save();

						page.color(15);
						cout << "\nContact number successfully updated." << endl;
						cout << endl;
						page.color(7);
						system("pause");
					}
				}
			}
			//Edit EmailAddress
			else if (editChoice == "6")
			{
				isValidEditChoice = true;
				bool isValidEmail = false;
				string email;
				int i = 0;
				while (!isValidEmail)
				{
					page.color(14);
					cout << setw(20) << "\nNew E-mail Address" << ": ";
					page.color(7);
					email.clear();
					if (i == 0)
					{
						cin.ignore(1, '\n');
						i++;
					}
					getline(cin, email);

					isValidEmail = input.isValidEmail(email);

					if (!isValidEmail)
					{
						page.color(12);
						cout << "\nInvalid E-mail format. Please try again." << endl;
					}
					else
					{
						e->setEmail(email);
						e->save();

						page.color(15);
						cout << "\nE-mail address successfully updated." << endl;
						cout << endl;
						page.color(7);
						system("pause");
					}
				}
			}			
			//Edit HomeAddress
			else if (editChoice == "7")
			{
				isValidEditChoice = true;
				bool isValidHomeAd = false;
				string homeAddress;
				int i = 0;
				while (!isValidHomeAd)
				{
					page.color(14);
					cout << setw(20) << "\nNew Home Address" << ": ";
					page.color(7);
					homeAddress.clear();
					if (i == 0)
					{
						cin.ignore(1, '\n');
						i++;
					}
					getline(cin, homeAddress);

					isValidHomeAd = input.isNullOrWhiteSpace(homeAddress);

					if (!isValidHomeAd)
					{
						page.color(12);
						cout << "\nPlease fill out this field. " << endl;
					}
					else
					{
						e->setHomeAddress(homeAddress);
						e->save();

						page.color(15);
						cout << "\nHome address successfully updated." << endl;
						cout << endl;
						page.color(7);
						system("pause");
					}
				}
			}
			//Change Password
			else if (editChoice == "8")
			{
				bool isCancelChgPwd = false;

				while (!isCancelChgPwd)
				{
					bool isPasswordExist;
					string password, reentered, hashedpwd;
					isValidEditChoice = true;
					page.color(14);
					cout << "\nCurrent Password\t: ";
					page.color(7);
					password.clear();
					char pwd;
					while ((pwd = _getch()) != 13) //ASCII 13 carriage return
					{
						password += pwd;
						cout << '*';
					}

					cout << endl;
					password = sha256(password);

					isPasswordExist = validateUserLogin(getLoginUsername(), password);

					if (isPasswordExist)
					{
						bool isValidPassword = false;

						while (!isValidPassword)
						{
							page.color(14);
							cout << "\nNew Password\t\t: ";
							page.color(7);
							password.clear();
							char pwd;
							while ((pwd = _getch()) != 13) //ASCII 13 carriage return3
							{
								password += pwd;
								cout << '*';
							}

							if (password.length() < 8)
							{
								page.color(12);
								cout << "\n\nPasswords must have at least 8 characters. Please try another password." << endl;
								page.color(7);

								password.clear();
							}
							else if (password.length() >= 8)
							{
								page.color(14);
								cout << endl;
								cout << "\nVerify New Password\t: ";
								page.color(7);
								reentered.clear();
								char repwd;
								while ((repwd = _getch()) != 13) //ASCII 13 carriage return
								{
									reentered += repwd;
									cout << '*';
								}

								if (password != reentered)
								{
									page.color(12);
									cout << endl;
									cout << "\nPlease make sure your passwords match." << endl;
									page.color(7);

									password.clear();
									reentered.clear();
								}
								else
								{
									isValidPassword = true;
									isCancelChgPwd = true;
									hashedpwd = sha256(reentered);
									e->setPassword(hashedpwd);
									e->save();

									page.color(15);
									cout << endl;
									cout << "\nPassword successfully changed." << endl;
									cout << endl;
									page.color(7);
									system("pause");
								}
							}
						}
					}
					else
					{
						page.color(12);
						cout << "\nWrong Password!" << endl;
						bool isValidTryAgain = false;
						string tryAgain;

						page.color(15);
						cout << "\nDo you want to try again?" << endl;
						page.showYesNoChoice();
						
						while (!isValidTryAgain)
						{
							page.color(14);
							cout << "\nEnter choice: ";
							page.color(7);
							tryAgain.clear();
							cin >> tryAgain;

							//Try Again
							if (tryAgain == "Y" || tryAgain == "y")
							{
								isValidTryAgain = true;
							}

							else if (tryAgain == "N" || tryAgain == "n")
							{
								isValidTryAgain = true;
								isCancelChgPwd = true;
							}

							//Invalid Input
							else
								page.showInvalidChoiceMsg();
						}
					}
				}
				
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

void EmployeeController::viewEmpList(int m)
{
	system("cls");
	page.systemHeaderOrange();
	page.empMngmtHeader();

	int n = m;
	if (m <= Employee::count())
	{
		page.color(15);
		cout << "\nNumber of employees: " << Employee::count() << endl;

		//Msg- Show m+1 ~ m+10 out of MAX results.
		
			cout << "\nShowing " << m + 1;

			if (m + 10 <= Employee::count())
				cout << "~" << m + 10 << " out of " << Employee::count() << " results.\n" << endl;
			else
				cout << "~" << Employee::count() << " out of " << Employee::count() << " results.\n" << endl;
		

		string str = "SELECT * FROM employees LIMIT 10 OFFSET " + to_string(m);
		PreparedStatement* ps = Database::prepareStatement(str);
		ResultSet* result = ps->executeQuery();
		page.color(14);
		cout << right << setw(5) << "No." << setw(5) << "ID" << setw(4) << " " << left << setw(40) << "Name" << setw(4) << " ";
		cout << left << setw(12) << "IC No." << setw(4) << " " << left << setw(11) << "Position" << setw(4) << " ";
		cout << left << setw(15) << "Username" << setw(4) << " " << left << setw(10) << "Status" << setw(4) << " " << "Permission" << endl;
		page.color(7);
		for (int i = 0; i < result->rowsCount(); i++)
		{
			result->next();
			cout << right << setw(5) << n + 1 << setw(5) << result->getInt("id") << setw(4) << " " << left << setw(40) << result->getString("name");
			cout << setw(4) << " " << left << setw(12) << result->getString("ic") << setw(4) << " " << left << setw(11) << result->getString("position") << setw(4) << " ";
			cout << left << setw(15) << result->getString("username") << setw(4) << " " << left << setw(10) << result->getString("status") << setw(4) << " " << result->getString("permission") << endl;
			n++;
		}
	}
}

int EmployeeController::countSearchResult(string searchName)
{
	string str = "SELECT COUNT(id) AS count FROM employees WHERE name LIKE '%" + searchName + "%'";
	PreparedStatement* ps = Database::prepareStatement(str);
	ResultSet* result = ps->executeQuery();
	result->next();
	return (int)result->getInt64("count");
}
void EmployeeController::viewEmpListSName(string searchName, int a)
{
	system("cls");
	page.systemHeaderOrange();
	page.empMngmtHeader();
	
	int b = a;

	if (a <= countSearchResult(searchName))
	{
		page.color(15);
		cout << "\nShowing " << a + 1;

		if (a + 10 <= countSearchResult(searchName) && countSearchResult(searchName) > 10)
			cout << "~" << a + 10 << " out of " << countSearchResult(searchName) << " results for \"" << searchName << "\".\n" << endl;
		else
			cout << "~" << countSearchResult(searchName) << " out of " << countSearchResult(searchName) << " results for \"" << searchName << "\".\n" << endl;

		string str = "SELECT * FROM employees WHERE name LIKE '%" + searchName + "%' LIMIT 10 OFFSET " + to_string(a);
		PreparedStatement* ps = Database::prepareStatement(str);
		ResultSet* result = ps->executeQuery();
		list<Employee*>list = Employee::parseList(result);

		page.color(14);
		cout << right << setw(5) << "No." << setw(5) << "ID" << setw(4) << " " << left << setw(40) << "Name" << setw(4) << " ";
		cout << left << setw(12) << "IC No." << setw(4) << " " << left << setw(11) << "Position" << setw(4) << " ";
		cout << left << setw(15) << "Username" << setw(4) << " " << left << setw(10) << "Status" << setw(4) << " " << "Permission" << endl;
		page.color(7);

		for (auto const& item : list)
		{
			item->display(b);
			cout << endl;
			b++;
		}
	}
}

void EmployeeController::empManagement()
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
				viewEmpListSName(searchVal, a);
			else
				viewEmpList(m);

			cout << endl;
			page.empMngmtPanel();
			
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
						if (a + 10 >= countSearchResult(searchVal))
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
						if (m + 10 >= Employee::count())
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

				//Search Employee
				else if (mngChoice == "3")
				{
					isValidMngChoice = true;
					a = 0;
					
					isNameFound = false;

					int i = 0;
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

				//View Employee Details and Modify
				else if (mngChoice == "4")
				{
					isValidMngChoice = true;
					string strId;
					bool isDigit = false;
					bool isValidStrId = false;

					while (!isValidStrId)
					{
						page.color(14);
						cout << "\nEnter Employee ID: ";
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
								modifyEmpDetails(id);
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
						{
							page.showInvalidChoiceMsg();
						}
					}
				}
				
				//Delete User
				else if (mngChoice == "5")
				{
					isValidMngChoice = true;
					string strId; 
					bool isDigit = false;
					bool isValidStrId = false;
					
					while (!isValidStrId)
					{						
						page.color(14);
						cout << "\nEnter Employee ID: ";
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
								deleteEmp(id);
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
				else if (mngChoice == "6")
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

void EmployeeController::modifyEmpDetails(int id)
{
	bool isBack = false;
	string editChoice;

	while (!isBack)
	{
		system("cls");
		page.systemHeaderOrange();
		page.empMngmtHeader();

		page.color(15);
		cout << "\nShowing details of Employee #" << id << endl;
		page.color(7);
		Employee* e = Employee::get(id);
		cout << e->displayDetails() << endl;

		page.modifyEmpPanel();
		bool isValidEditChoice = false;
		
		while (!isValidEditChoice)
		{
			page.color(14);
			cout << "\nEnter choice: ";
			page.color(7);
			editChoice.clear();
			cin >> editChoice;

			//Modify Position
			if (editChoice == "1")
			{
				isValidEditChoice = true;
				bool isValidPosition = false;
				string position;
				int i = 0;
				while (!isValidPosition)
				{
					page.color(14);
					cout <<left<< setw(20) << "\nPosition" << ": ";
					page.color(7);
					position.clear();
					if (i == 0)
					{
						cin.ignore(1, '\n');
						i++;
					}
					getline(cin, position);

					isValidPosition = input.isNullOrWhiteSpace(position);
					if (!isValidPosition)
					{
						page.color(12);
						cout << "\nPlease fill out this field. " << endl;
						page.color(7);
					}
					else
					{
						e->setPosition(position);
						e->save();

						page.color(15);
						cout << "\nPosition successfully updated." << endl;
						cout << endl;
						page.color(7);
						system("pause");
					}
				}
			}			
			//Modify Permission
			else if (editChoice == "2")
			{
				isValidEditChoice = true;
				bool isValidPermission = false;
				page.color(15);
				cout << "\nPlease enter \"admin\" or \"normaluser\"." << endl;
				string permission;
				int i = 0;
				while (!isValidPermission)
				{
					page.color(14);
					cout << left << setw(20) << "\nPermission" << ": ";
					page.color(7);
					permission.clear();
					if (i == 0)
					{
						cin.ignore(1, '\n');
						i++;
					}
					getline(cin, permission);

					isValidPermission = input.isValidPermission(permission);
					if (!isValidPermission)
					{
						page.color(12);
						cout << "\nInvalid format. Please enter \"admin\" or \"normaluser\" as your input." << endl;
					}
					else
					{
						e->setPermission(permission);
						e->save();

						page.color(15);
						cout << "\nPermission successfully updated." << endl;
						cout << endl;
						page.color(7);
						system("pause");
					}
				}
			}
			//Reset Password
			else if (editChoice == "3")
			{
				isValidEditChoice = true;
				string password, hashedpwd;
				password = e->getUsername();

				hashedpwd = sha256(password);
				e->setPassword(hashedpwd);
				e->save();

				page.color(15);
				cout << endl;
				cout << "\nPassword successfully reset." << endl;
				cout << endl;
				page.color(7);
				system("pause");
			}
			//Back
			else if (editChoice == "4")
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
void EmployeeController::deleteEmp(int id)
{
	system("cls");
	page.systemHeaderOrange();
	page.empMngmtHeader();

	page.color(15);
	cout << "\nShowing details of Employee #" << id << endl;
	Employee* e = Employee::get(id);
	page.color(7);
	cout << e->displayDetails() << endl;

	bool isValidDelConf = false;
	string delConf;
	int i = 0;
	page.color(15);
	cout << "\nAre you sure you want to delete Employee #" << id << "?" << endl;
	page.showYesNoChoice();
	page.color(7);
	
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
			e->setStatus("disabled");
			e->save();
			page.color(15);
			cout << "\nEmployee #" << id << " status updated to \"disabled\".\n" << endl;
			system("pause");
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

//validation
bool EmployeeController::validateUserLogin(string username, string password)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("SELECT EXISTS (SELECT * FROM Employees WHERE username = ? AND password = ?)");

	ps->setString(1, username);
	ps->setString(2, password);

	ResultSet* rs = ps->executeQuery();

	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}

bool EmployeeController::validateAccountActive(string username, string password)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("SELECT EXISTS (SELECT * FROM Employees WHERE username = ? AND password = ? AND status = ?)");

	ps->setString(1, username);
	ps->setString(2, password);
	ps->setString(3, "active");

	ResultSet* rs = ps->executeQuery();

	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}

bool EmployeeController::validateAdminLogin(string adminUsername, string adminPassword)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("SELECT EXISTS (SELECT * FROM Employees WHERE username = ? AND password = ? AND permission = ?)");

	ps->setString(1, adminUsername);
	ps->setString(2, adminPassword);
	ps->setString(3, "admin");

	ResultSet* rs = ps->executeQuery();

	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}

bool EmployeeController::usernameExist(string username)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("SELECT EXISTS (SELECT * FROM Employees WHERE username = ?)");

	ps->setString(1, username);

	ResultSet* rs = ps->executeQuery();
	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}
bool EmployeeController::idExist(int id)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("SELECT EXISTS (SELECT * FROM Employees WHERE id = ?)");

	ps->setInt(1, id);

	ResultSet* rs = ps->executeQuery();
	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}
bool EmployeeController::nameExist(string name)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("SELECT EXISTS (SELECT * FROM Employees WHERE name LIKE ?)");

	ps->setString(1, "%" + name + "%");

	ResultSet* rs = ps->executeQuery();
	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}

