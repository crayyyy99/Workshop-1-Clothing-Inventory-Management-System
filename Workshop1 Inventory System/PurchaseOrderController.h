#include "ProductController.h"
#include "CheckInput.h"
#include "IndexPage.h"

#ifndef PURCHASEORDERCONTROLLER_H
#define PURCHASEORDERCONTROLLER_H

class PurchaseOrderController
{
private:
	IndexPage page;
	CheckInput input;
	ProductController prodDetails;
	int supplierId, employeeLoginId, purchaseOrderId;
	int productId, code, quantity, a, resultNotFound, deleteQuantity, itemNo;
	double price;
	string size;

public:
	void viewPurOrderList(const int m);

	int countSearchDate(string search);
	void viewPurOrderListDate(string searchVal, const int a);
	bool dateExist(string productName);

	int countSearchEmployee(string search);
	void viewPurOrderListEmployee(string searchVal, const int a);
	bool employeeExist(string productName);

	int countSearchSupp(string search);
	void viewPurOrderListSupp(string searchVal, const int a);
	bool suppExist(string productName);

	void purOrderManagement(int employeeId);

	void addPurOrder();
	bool addPurOrderItem();
	
	bool itemNoExistInPurOrder(int purOrderid, int itemNo);
	int readDeleteQuantity(int purOrderid, int itemNo);
	int readDeleteCode(int purOrderid, int itemNo);
	void deleteQuantityInProdStock(int productcode, int quantity);

	void changeProdStatus(int productid);
	void addProdQuantity(int productcode, int quantity);
	void changeSupStatus(int supid);

	bool idExistPurOrder(int id);
	bool idExistPurOrderDet(int id);

	void deletePurOrderWithoutDet(int purOrderid);

	void setA(int n);
	int getA();
	void setResultNotFound(int n);
	int getResultNotFound();
	void setItemNo(int n);
	int getItemNo();
	void setProductId(int n);
	int getProductId();
	void setSize(string n);
	string getSize();
	void setPrice(double n);
	double getPrice();
	void setQuantity(int n);
	int getQuantity();

	void setCode(int n);
	int getCode();
	void setDeleteQuantity(int n);
	int getDeleteQuantity();

	void setSupplierId(int n);
	int getSupplierId();
	void setEmployeeLoginId(int n);
	int getEmployeeLoginId();
	void setPurOrderId(int n);
	int getPurOrderId();

	void showPurchaseOrder(int purOrderid);
	void showPurchaseOrderDetails(int purOrderid);
	double readTotalPrice(int purOrderid);
	void viewDetails(int purOrderid);
};

#endif // !EMPLOYEECONTROLLER_H


