#include "Supplier.h"
#include "CheckInput.h"
#include "IndexPage.h"

#ifndef SUPPLIERCONTROLLER_H
#define SUPPLIERCONTROLLER_H

class SupplierController
{
private:
	IndexPage page;
	CheckInput input;
	int supId;

public:
	void viewSuppList(int m);
	void suppManagement();
	
	int countSearchName(string search);
	void viewSuppListName(string searchVal, int a);
	
	void addSupplier();

	void setSupId(int id);
	int getSupId();

	void modifySuppDetails(int id);
	void deleteSupp(int id);
	void changeStatus(int id);

	void searchSupName(string searchVal);
	void showDetails(int id);

	//validation
	bool idExist(int id);
	bool nameExist(string compName);
	bool suppActivated(int id);
};

#endif // !EMPLOYEECONTROLLER_H


