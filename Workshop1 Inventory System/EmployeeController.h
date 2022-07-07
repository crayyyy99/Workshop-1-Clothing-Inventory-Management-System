#include "Employee.h"
#include "CheckInput.h"
#include "IndexPage.h"

#ifndef EMPLOYEECONTROLLER_H
#define EMPLOYEECONTROLLER_H

class EmployeeController
{
private:
	IndexPage page;
	CheckInput input;
	int loginId;
	string loginUsername, loginPassword;

public:
	void registerNewUser();
	bool loginUser();
	bool loginAdmin();

	void setLoginUsername(string n);
	string getLoginUsername();
	void readLoginId(string username);
	void setLoginId(int n);
	int getLoginId();

	void editAccountDetails();

	void viewEmpList(int m);
	void empManagement();
	
	int countSearchResult(string searchName);
	void viewEmpListSName(string searchVal, int a);
	
	void modifyEmpDetails(int id);
	void deleteEmp(int id);

	//validation
	bool validateUserLogin(string username, string password);
	bool validateAccountActive(string username, string password);
	bool validateAdminLogin(string adminUsername, string adminPassword);
	bool usernameExist(string username);
	bool idExist(int id);
	bool nameExist(string name);
};


#endif // !EMPLOYEECONTROLLER_H


