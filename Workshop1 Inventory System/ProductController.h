#include "Product.h"
#include "ProductStock.h"
#include "CheckInput.h"
#include "IndexPage.h"

#ifndef PRODUCTCONTROLLER_H
#define PRODUCTCONTROLLER_H

class ProductController
{
private:
	IndexPage page;
	CheckInput input;

	ProductStock prodStockDetails;
	int productID;

public:
	void viewProdList(const int m);

	int countSearchProductName(string search);
	void viewProdListProductName(string searchVal, const int a);
	bool prodNameExist(string productName);

	void addProduct();
	bool addProdStockDetails();
	void setProductId(int n);
	int getProductId();
	
	int generateCode();
	bool codeExist(int code);

	void showProductFullDetails(int productId);
	void showProductStockDetails(int productId);
	
	void prodManagementAdmin();

	bool idExist(int id);
	void modifyProdDetails(int id);
	bool prodActivated(int id);
	void deleteSize(int code);
	bool checkProductStatus(int id, string status);

	void deleteProd(int id);
	void deleteProdDetails(int productId);
	void eraseProdDetails(int* array, int SIZE);
	int countSize(int id);
	void changeDisabled(int id);

	int countLowStockProduct(int value);
	void viewLowStockProduct(int value, const int a);

	void prodSearchViewUser();
	
	void purOrderShowProdDetails(int productId);
	void searchProdName(string searchVal);
	bool sizeExist(int productId, string size);
	
	int readProdCode(int productId, string size);
	string readProdSizeId(int productId, string size);
	string readProdSizeCode(int productCode);
	string readProdNameId(int productId, string size);
	string readProdNameCode(int productCode);
	double readProdPrice(int productId);
	int readProdIdCode(int productCode);
	int readProdQuantityCode(int productCode);

	void transShowProdCodeDetails(int productId, int productcode);
};

#endif // !EMPLOYEECONTROLLER_H


