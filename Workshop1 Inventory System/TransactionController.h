#include "ProductController.h"
#include "CheckInput.h"
#include "IndexPage.h"

#ifndef TRANSACTIONCONTROLLER_H
#define TRANSACTIONCONTROLLER_H

class TransactionController
{
private:
	IndexPage page;
	CheckInput input;
	ProductController prodDetails;
	int employeeLoginId, transactionId;

	int a, resultNotFound, productId, quantity, discount;
	int code, deleteQuantity, itemNo;
	string size;
	double subtotal;

public:
	
	void viewTransList(const int m);

	int countSearchDate(string search);
	void viewTransListDate(string searchVal, const int a);
	bool dateExist(string searchVal);

	int countSearchEmployee(string search);
	void viewTransListEmployee(string searchVal, const int a);
	bool employeeExist(string searchVal);
	
	int countSearchEmployeeDate(string searchName, string searchDate);
	void viewTransListEmployeeDate(string searchName, string searchDate, const int a);
	bool employeeDateExist(string searchName, string searchDate);
	
	void transManagement();

	//Add Transaction
	void addTransaction(int employeeId);
	bool addTransItem();
	void addTransactionDetails(int number);
	void reduceQuantityInProdStock(int productcode, int quantity);

	bool itemNoExistInTrans(int transId, int itemNo);
	int readDeleteQuantity(int transId, int itemNo);
	int readDeleteCode(int transId, int itemNo);
	void setDeleteQuantity(int n);
	int getDeleteQuantity();
	void addProdQuantityInProductStock(int productcode, int quantity);

	void deleteTransOrderWithoutDet(int transId);
	bool idExistTransDet(int id);

	void setA(int n);
	int getA();
	void setResultFound(int n);
	int getResultFound();
	void setItemNo(int n);
	int getItemNo();

	void showTransaction(int transid);
	void showTransactionDetails(int transId);
	double readTotalPrice(int transId);
	
	void setSubTotal(double n);
	double getTotal();

	void setEmployeeLoginId(int n);
	int getEmployeeLoginId();
	void setTransId(int n);
	int getTransId();

	void setProductId(int n);
	int getProductId();
	void setCode(int n);
	int getCode();
	void setSize(string n);
	string getSize();
	void setQuantity(int n);
	int getQuantity();
	void setDiscount(int n);
	int getDiscount();

	//View Transaction	
	bool idExistTrans(int id);
	void viewDetails(int purOrderid);


	//Manage Refund
	void manageRefund(int transId);

	//User
	void viewSearchTransUser(int empId);
	int countTransListUser(int empId);
	void viewTransListUser(int empId, const int m);

	int countSearchDateUser(int empId, string search);
	void viewTransListDateUser(int empId, string searchVal, const int a);
	bool dateExistUser(int empId, string searchVal);
};

#endif // !EMPLOYEECONTROLLER_H


