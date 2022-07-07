#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <iomanip>
#include <conio.h>
#include <cstdlib> 
#include <ctime> 

using namespace std;

#include <mysql/jdbc.h>
using namespace sql;

#include "ProductController.h"
#include "Supplier.h"
#include "DatabaseConnection.h"
#include "Product.h"
#include "ProductStock.h"
#include "IndexPage.h"
#include "CheckInput.h"

const int MIN = 1000000;
const int MAX = 9999999;

void ProductController::viewProdList(const int m)
{
	int n = m;

	int totalCount = Product::count();

	if (totalCount == 0)
	{
		page.color(15);
		cout << "\nNumber of products: " << totalCount << endl;
	}
	else
	{
		string str = "Select products.id, products.`name`, products.colour, products.gender, products.type,products.unit_price, products.`status`, sum(quantity) AS totalQuantity from products inner JOIN product_stocks on(products.id=product_stocks.product_id) group by product_stocks.product_id order by product_stocks.product_id LIMIT 10 OFFSET "
			+ to_string(m);
		PreparedStatement* ps = Database::prepareStatement(str);
		ResultSet* result = ps->executeQuery();

		int SIZE = ((int)result->rowsCount() < totalCount) ? (int)result->rowsCount() : totalCount;

		page.color(15);
		cout << "\nNumber of products: " << totalCount << endl;

		cout << "\nShowing " << n + 1;

		if (m + 10 <= totalCount)
			cout << "~" << n + 10 << " out of " << totalCount << " results.\n" << endl;
		else
			cout << "~" << totalCount << " out of " << totalCount << " results.\n" << endl;

		page.color(7);
		cout << right << setw(5) << "No." << setw(5) << "ID" << setw(4) << " " << left << setw(80) << "Product Name" << setw(4) << " ";
		cout << left << setw(15) << "Colour" << setw(4) << " " << left << setw(7) << "Gender" << setw(4) << " " << left << setw(20) << "Type";
		cout << setw(4) << " " << left << setw(15) << "Unit Price (RM)" << setw(4) << " ";
		cout << left << setw(12) << "Status" << setw(4) << " " << "Total Quantity" << endl;

		for (int i = 0; i < SIZE; i++)
		{
			result->next();
			cout << right << setw(5) << n + 1 << setw(5) << result->getInt("id") << setw(4) << " " << left << setw(80) << result->getString("name");
			cout << setw(4) << " " << left << setw(15) << result->getString("colour") << setw(4) << " " << left << setw(7) << result->getString("gender") << setw(4) << " ";
			cout << left << setw(20) << result->getString("type") << setw(4) << " " << right << setw(15) << fixed << showpoint << setprecision(2) << result->getDouble("unit_price");
			cout << setw(4) << " " << left << setw(12) << result->getString("status") << setw(4) << " " << right << setw(13) << result->getInt("totalQuantity") << endl;
			n++;
		}
		
	}
}

int ProductController::countSearchProductName(string search)
{
	string str = "SELECT COUNT(id) AS count FROM Products WHERE name LIKE '%" + search + "%'";
	PreparedStatement* ps = Database::prepareStatement(str);
	ResultSet* result = ps->executeQuery();
	result->next();
	return (int)result->getInt64("count");
}
void ProductController::viewProdListProductName(string searchVal, const int a)
{
	int b = a;

	int totalSearch = countSearchProductName(searchVal);

	if (a <= totalSearch)
	{
		string str = "Select products.id,products.`name`, products.colour, products.gender, products.type, products.unit_price, products.`status`, sum(quantity) AS totalQuantity from products inner JOIN product_stocks on(products.id=product_stocks.product_id) WHERE products.`name` LIKE ? group by product_stocks.product_id order by product_stocks.product_id LIMIT 10 OFFSET "
			+ to_string(a);

		PreparedStatement* ps = Database::prepareStatement(str);
		ps->setString(1, "%" + searchVal + "%");
		ResultSet* result = ps->executeQuery();

		int SIZE = ((int)result->rowsCount() < totalSearch) ? (int)result->rowsCount() : totalSearch;

		page.color(15);
		cout << "\nShowing " << a + 1;

		if (a + 10 <= countSearchProductName(searchVal))
			cout << "~" << a + 10 << " out of " << countSearchProductName(searchVal) << " results for \"" << searchVal << "\".\n" << endl;
		else
			cout << "~" << countSearchProductName(searchVal) << " out of " << countSearchProductName(searchVal) << " results for \"" << searchVal << "\".\n" << endl;


		page.color(7);
		cout << right << setw(5) << "No." << setw(5) << "ID" << setw(4) << " " << left << setw(80) << "Product Name" << setw(4) << " ";
		cout << left << setw(15) << "Colour" << setw(4) << " " << left << setw(7) << "Gender" << setw(4) << " " << left << setw(20) << "Type";
		cout << setw(4) << " " << left << setw(16) << "Unit Price (RM)" << setw(4) << " ";
		cout << left << setw(15) << "Status" << setw(4) << " " << "Total Quantity" << endl;

		for (int i = 0; i < SIZE; i++)
		{
			result->next();
			cout << right << setw(5) << b + 1 << setw(5) << result->getInt("id") << setw(4) << " " << left << setw(80) << result->getString("name");
			cout << setw(4) << " " << left << setw(15) << result->getString("colour") << setw(4) << " " << left << setw(7) << result->getString("gender") << setw(4) << " ";
			cout << left << setw(20) << result->getString("type") << setw(4) << " " << right << setw(15) << fixed << showpoint << setprecision(2) << result->getDouble("unit_price");
			cout << setw(4) << " " << left << setw(12) << result->getString("status") << setw(4) << " " << right << setw(13) << result->getInt("totalQuantity") << endl;
			b++;
		}
	}
}
bool ProductController::prodNameExist(string productName)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("SELECT EXISTS (SELECT * FROM Products WHERE name LIKE ?)");

	ps->setString(1, "%" + productName + "%");

	ResultSet* rs = ps->executeQuery();
	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}

void ProductController::prodManagementAdmin()
{
	bool isBack = false;
	while (!isBack)
	{
		bool isStopLoop = false;

		int m = 0, a = 0, b = 0;
		int id = 0, number = 0;
		string mngChoice;
		string searchVal = "";
		bool isProdNameFound = false;
		bool isShowLowStock = false;

		while (!isStopLoop)
		{
			bool isValidMngChoice = false;

			if (isProdNameFound)
			{
				system("cls");
				page.systemHeaderOrange();
				page.prodMngmtHeader();
				viewProdListProductName(searchVal, a);
			}			
			else if (isShowLowStock)
			{
				system("cls");
				page.systemHeaderOrange();
				page.prodMngmtHeader();
				viewLowStockProduct(number, b);
			}				
			else
			{
				system("cls");
				page.systemHeaderOrange();
				page.prodMngmtHeader();
				viewProdList(m);
			}
				
			cout << endl;
			page.prodMngmtPanel();
			

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

					if (isProdNameFound)
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
					else if (isShowLowStock)
					{
						if (b == 0)
						{
							page.color(12);
							cout << "\nThis is the first page.\n" << endl;
							page.color(7);
							system("pause");
						}
						else
							b = b - 10;
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
					if (isProdNameFound)
					{
						if (a + 10 >= countSearchProductName(searchVal))
						{
							page.color(12);
							cout << "\nThis is the last page.\n" << endl;
							page.color(7);
							system("pause");
						}
						else
							a = a + 10;
					}
					else if (isShowLowStock)
					{
						if (b + 10 >= countLowStockProduct(number))
						{
							page.color(12);
							cout << "\nThis is the last page.\n" << endl;
							page.color(7);
							system("pause");
						}
						else
							b = b + 10;
					}
					else
					{
						if (m + 10 >= Product::count())
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

				//Add New Product
				else if (mngChoice == "3")
				{
					isValidMngChoice = true;
					addProduct();
				}

				//Search Product
				else if (mngChoice == "4")
				{
					isValidMngChoice = true;
					a = 0;
					isProdNameFound = false;

					int i = 0;
					bool isNull = false;
					while (!isNull)
					{
						page.color(14);
						cout << "\nEnter product name to search: ";
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
							bool isNameExist = prodNameExist(searchVal);
							if (isNameExist)
								isProdNameFound = true;
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

				//View Product Details and Modify
				else if (mngChoice == "5")
				{
					isValidMngChoice = true;
					string strId;
					bool isDigit = false;
					bool isValidStrId = false;

					while (!isValidStrId)
					{
						page.color(14);
						cout << "\nEnter Product ID: ";
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
								modifyProdDetails(id);
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

				//Delete Product
				else if (mngChoice == "6")
				{
					isValidMngChoice = true;
					string strId;
					bool isDigit = false;
					bool isValidStrId = false;

					page.color(15);
					cout << "\nFor product which is activated, delete product will change the status of this product to \"disabled\"." << endl;
					while (!isValidStrId)
					{
						page.color(14);
						cout << "\nEnter Product ID: ";
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
								deleteProd(id);
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

				//Check Low Stock Products
				else if (mngChoice == "7")
				{
					isValidMngChoice = true;
					string strNumber;
					bool isDigit = false;
					bool isValidNumber = false;
					b = 0;
					isShowLowStock = false;

					while (!isValidNumber)
					{
						page.color(15);
						cout << "\nEnter a number to check the products which its quantity is lower than and equal to this number." << endl;
						page.color(14);
						cout << "\nEnter number: ";
						strNumber.clear();
						page.color(7);
						cin >> strNumber;

						isDigit = input.isDigit(strNumber);
						if (isDigit)
						{
							number = stoi(strNumber);
							isValidNumber = true;
							isShowLowStock = true;
						}
						//Invalid ID input (not digit)
						else
							page.showInvalidChoiceMsg();
					}
				}
			
				//Back
				else if (mngChoice == "8")
				{
					if (isProdNameFound)
					{
						isValidMngChoice = true;
						isStopLoop = true;
					}
					else if (isShowLowStock)
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

void ProductController::addProduct()
{
	Product prodDetails;
	bool isQuitAdd = false;

	while (!isQuitAdd)
	{
		double unitPrice;
		string name, colour, gender, type, description, strUnitPrice;
		
		int i = 0;
		bool isCancelLoop = false;
		bool isValidName = false;
		bool isValidColour = false;
		bool isValidGender = false;
		bool isValidType = false;
		bool isValidDescription = false;	
		bool isValidUnitPrice = false;

		if (Product::count() == 0)
			prodDetails.resetAutoIncrement();
		if (ProductStock::count() == 0)
			prodStockDetails.resetAutoIncrement();

		system("cls");
		page.systemHeaderOrange();
		page.prodMngmtHeader();

		page.color(14);
		cout << "\n Add New Product" << endl;
		for (int i = 0; i < 18; i++) { cout << char(205); }cout << endl;

		while (!isValidName)
		{
			page.color(14);
			cout << setw(17) << "\nName" << ": ";
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
				page.color(7);
			}
			else
				prodDetails.setName(name);
		}

		while (!isValidColour)
		{
			page.color(14);
			cout << setw(17) << "\nColour" << ": ";
			page.color(7);
			colour.clear();
			getline(cin, colour);

			isValidColour = input.isNullOrWhiteSpace(colour);
			if (!isValidColour)
			{
				page.color(12);
				cout << "\nPlease fill out this field. " << endl;
				page.color(7);
			}
			else
				prodDetails.setColour(colour);
		}

		while (!isValidGender)
		{
			page.color(14);
			cout << setw(17) << "\nGender" << ": ";
			page.color(7);
			gender.clear();
			getline(cin, gender);

			isValidGender = input.isNullOrWhiteSpace(gender);
			if (!isValidGender)
			{
				page.color(12);
				cout << "\nPlease fill out this field. " << endl;
				page.color(7);
			}
			else
				prodDetails.setGender(gender);
		}

		while (!isValidType)
		{
			page.color(14);
			cout << setw(17) << "\nType" << ": ";
			page.color(7);
			type.clear();
			getline(cin, type);

			isValidType = input.isNullOrWhiteSpace(type);
			if (!isValidType)
			{
				page.color(12);
				cout << "\nPlease fill out this field. " << endl;
				page.color(7);
			}
			else
				prodDetails.setType(type);
		}
		
		while (!isValidDescription)
		{
			page.color(14);
			cout << setw(17) << "\nDescription" << ": ";
			page.color(7);
			description.clear();
			getline(cin, description);

			isValidDescription = input.isNullOrWhiteSpace(description);
			if (!isValidDescription)
			{
				page.color(12);
				cout << "\nPlease fill out this field. " << endl;
				page.color(7);
			}
			else
				prodDetails.setDescription(description);
		}

		while (!isValidUnitPrice)
		{
			page.color(14);
			cout << setw(17) << "\nUnit Price (RM)" << ": ";
			page.color(7);
			strUnitPrice.clear();
			getline(cin, strUnitPrice);

			if (strUnitPrice != "")
			{
				isValidUnitPrice = input.isDigit(strUnitPrice);
				if (!isValidUnitPrice)
					page.showInvalidChoiceMsg();

				else
				{
					unitPrice = stod(strUnitPrice);
					prodDetails.setUnitPrice(unitPrice);
				}
			}
			else
			{
				page.color(12);
				cout << "\nPlease fill out this field" << endl;
			}				
		}
			
		system("cls");
		page.systemHeaderOrange();
		page.prodMngmtHeader();

		page.color(15);
		cout << "\nPlease validate the product's details.\n" << endl;
		page.color(7);
		cout << setw(17) << "\nName" << ": " << prodDetails.getName() << endl;
		cout << setw(17) << "\nColour" << ": " << prodDetails.getColour() << endl;
		cout << setw(17) << "\nGender" << ": " << prodDetails.getGender() << endl;
		cout << setw(17) << "\nType" << ": " << prodDetails.getType() << endl;
		cout << setw(17) << "\nDescription" << ": " << prodDetails.getDescription() << endl;
		cout << setw(17) << "\nUnit Price (RM)" << ": " << fixed << showpoint << setprecision(2) << prodDetails.getUnitPrice() << endl;
		cout << endl;

		bool isValidConfirmProd = false;
		string confirmProd;		
		page.color(15);
		cout << "\nAre you confirm?" << endl;
		page.showYesNoChoice();

		while (!isValidConfirmProd)
		{
			page.color(14);
			cout << "\nEnter choice: ";
			confirmProd.clear();
			page.color(7);
			cin >> confirmProd;

			if (confirmProd == "Y" || confirmProd == "y")
			{			
				isValidConfirmProd = true;
				Product* p = new Product();

				p->setName(prodDetails.getName());
				p->setColour(prodDetails.getColour());
				p->setGender(prodDetails.getGender());
				p->setType(prodDetails.getType());
				p->setDescription(prodDetails.getDescription());
				p->setUnitPrice(prodDetails.getUnitPrice());

				p->save();
				
				setProductId(Database::getLastInsertId());

				isQuitAdd = addProdStockDetails();
			}
			else if (confirmProd == "N" || confirmProd == "n")
			{
				isValidConfirmProd = true;
				isQuitAdd = true;
			}
			else
				page.showInvalidChoiceMsg();
		}
	}
}
bool ProductController::addProdStockDetails()
{
	bool isCancelLoopShowDetails = false;

	//while (!isCancelLoopShowDetails)
	//{
		system("cls");
		page.systemHeaderOrange();
		page.prodMngmtHeader();

		Product* pd = Product::get(getProductId());
		page.color(15);
		cout << "\nShowing details of Product #" << getProductId() << endl;
		page.color(7);
		cout << pd->display() << endl;

		bool isValidSizeChoice = false;
		string sizeChoice;
		page.addSizePanel();
		
		while (!isValidSizeChoice)
		{
			page.color(14);
			cout << "\nEnter choice: ";
			page.color(7);
			sizeChoice.clear();
			cin >> sizeChoice;

			if (sizeChoice == "1")
			{
				ProductStock* psd = new ProductStock();
				psd->setSize("Free Size");
				psd->setProductId(getProductId());
				psd->setCode(generateCode());
				psd->save();
				page.color(15);
				cout << "\nSize \"Free size\" for Product #" << getProductId() << " is added successfully." << endl;
				page.color(7);
				cout << endl;
				system("pause");
				showProductFullDetails(getProductId());
				cout << endl;
				system("pause");
				return true;
			}
			else if (sizeChoice == "2")
			{
				bool isCancelLoopAddSize = false;
				string addAnotherSize, size;

				system("cls");
				page.systemHeaderOrange();
				page.prodMngmtHeader();

				Product* pd = Product::get(getProductId());
				page.color(15);
				cout << "\nShowing details of Product #" << getProductId() << endl;
				page.color(7);
				cout << pd->display() << "\n" << endl;

				while (!isCancelLoopAddSize)
				{
					

					bool isValidSize = false;
					int i = 0;
					while (!isValidSize)
					{
						page.color(14);
						cout << left << setw(13) << "\nEnter Size" << ": ";
						page.color(7);
						size.clear();
						if (i == 0)
						{
							cin.ignore(1, '\n');
							i++;
						}
						getline(cin, size);

						isValidSize = input.isNullOrWhiteSpace(size);
						if (!isValidSize)
						{
							page.color(12);
							cout << "\nPlease fill out this field. " << endl;
							page.color(7);
						}
						else
						{
							isValidSize = true;
							bool isValidConfirmProd = false;
							string confirmProd;
							page.color(15);
							cout << "\nAre you confirm?" << endl;
							page.showYesNoChoice();

							while (!isValidConfirmProd)
							{
								page.color(14);
								cout << "\nEnter choice: ";
								confirmProd.clear();
								page.color(7);
								cin >> confirmProd;

								if (confirmProd == "Y" || confirmProd == "y")
								{
									isValidConfirmProd = true;									
									ProductStock* psd = new ProductStock();
									psd->setSize(size);
									psd->setProductId(getProductId());
									psd->setCode(generateCode());
									psd->save();
									page.color(15);
									cout << "\nSize \"" << size << "\" for Product #" << getProductId() << " is added successfully." << endl;
									page.color(7);
									cout << endl;
									system("pause");

									showProductFullDetails(getProductId());
									cout << "\n\nAdd another size?" << endl;
									page.showYesNoChoice();

									bool isValidAddAnotherSize = false;
									while (!isValidAddAnotherSize)
									{
										page.color(14);
										cout << "\nEnter choice: ";
										page.color(7);
										addAnotherSize.clear();
										cin >> addAnotherSize;

										if (addAnotherSize == "Y" || addAnotherSize == "y")
										{
											isValidAddAnotherSize = true;
										}
										else if (addAnotherSize == "N" || addAnotherSize == "n")
										{
											isValidAddAnotherSize = true;
											isCancelLoopAddSize = true;
											showProductFullDetails(getProductId());
											cout << endl;
											system("pause");
											return true;
										}
										else
											page.showInvalidChoiceMsg();
									}
								}
								else if (confirmProd == "N" || confirmProd == "n")
								{
									isValidConfirmProd = true;
									isValidSize = true;
								}
								else
									page.showInvalidChoiceMsg();
							}			
						}
					}
				} 
			}
			else
				page.showInvalidChoiceMsg();
		}
	//}
}

void ProductController::showProductStockDetails(int productId)
{
	string str = "SELECT * FROM product_stocks WHERE product_id = ?";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, productId);
	ResultSet* result = ps->executeQuery();
	cout << "\n" << endl;
	page.color(15);
	cout << setw(10) << "Size" << setw(4) << " " << left << setw(7) << "Code" << setw(4) << " ";
	cout << left << setw(10) << "Quantity" << setw(4) << " " << left << setw(19) << "Created At";
	cout << setw(4) << " " << left << setw(19) << "Updated At" << endl;
	page.color(7);
	for (int i = 0; i < result->rowsCount(); i++)
	{
		result->next();
		cout << setw(10) << result->getString("size") << setw(4) << " " << left << setw(7) << result->getInt("code") << setw(4) << " ";
		cout << left << setw(10) << result->getInt("quantity") << setw(4) << " " << left << setw(19) << result->getString("created_at");
		cout << setw(4) << " " << left << setw(19) << result->getString("updated_at") << endl;
	}
}
void ProductController::showProductFullDetails(int productId)
{
	system("cls");
	page.systemHeaderOrange();
	page.prodMngmtHeader(); 
	
	Product* pd = Product::get(productId);
	page.color(15);
	cout << "\nShowing details of Product #" << productId << endl;
	page.color(7);
	cout << pd->display() << endl;
	
	showProductStockDetails(productId);
}

int ProductController::generateCode()
{
	srand(time(NULL));
	bool isInvalidCode = true;
	while (isInvalidCode)
	{
		int Code = rand() % (MAX - MIN + 1) + MIN;
		isInvalidCode = codeExist(Code);
		if (!isInvalidCode)
			return Code;
	}	
}
bool ProductController::codeExist(int code)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("SELECT EXISTS (SELECT * FROM product_stocks WHERE code = ?)");

	ps->setInt(1, code);

	ResultSet* rs = ps->executeQuery();
	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}

void ProductController::setProductId(int n)
{
	productID = n;
}
int ProductController::getProductId()
{
	return productID;
}

bool ProductController::idExist(int id)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("SELECT EXISTS (SELECT * FROM Products WHERE id = ?)");

	ps->setInt(1, id);

	ResultSet* rs = ps->executeQuery();
	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}

void ProductController::modifyProdDetails(int id)
{
	bool isBack = false;
	string editChoice;

	while (!isBack)
	{
		system("cls");
		page.systemHeaderOrange();
		page.prodMngmtHeader();

		Product* pd = Product::get(id);
		page.color(15);
		cout << "\nShowing details of Product #" << id << endl;
		page.color(7);
		cout << pd->display() << endl;

		showProductStockDetails(id);

		page.modifyProdPanel();
		bool isValidEditChoice = false;


		while (!isValidEditChoice)
		{
			page.color(14);
			cout << "\nEnter choice: ";
			page.color(7);
			editChoice.clear();
			cin >> editChoice;

			//Modify Product Name
			if (editChoice == "1")
			{
				isValidEditChoice = true;
				bool isValidProdName = false;
				string prodName;
				int i = 0;
				while (!isValidProdName)
				{
					page.color(14);
					cout << setw(20) << "\nNew Product Name" << ": ";
					page.color(7);
					prodName.clear();
					if (i == 0)
					{
						cin.ignore(1, '\n');
						i++;
					}
					getline(cin, prodName);

					isValidProdName = input.isNullOrWhiteSpace(prodName);
					if (!isValidProdName)
					{
						page.color(12);
						cout << "\nPlease fill out this field. " << endl;
						page.color(7);
					}
					else
					{
						pd->setName(prodName);
						pd->save();

						page.color(15);
						cout << "\nProduct name successfully updated." << endl;
						cout << endl;
						page.color(7);
						system("pause");
					}
				}
			}
			//Modify Product Colour		
			else if (editChoice == "2")
			{
				isValidEditChoice = true;
				bool isValidColour = false;
				string colour;
				int i = 0;
				while (!isValidColour)
				{
					page.color(14);
					cout << setw(20) << "\nNew Colour" << ": ";
					page.color(7);
					colour.clear();
					if (i == 0)
					{
						cin.ignore(1, '\n');
						i++;
					}
					getline(cin, colour);

					isValidColour = input.isNullOrWhiteSpace(colour);
					if (!isValidColour)
					{
						page.color(12);
						cout << "\nPlease fill out this field. " << endl;
						page.color(7);
					}
					else
					{
						pd->setColour(colour);
						pd->save();

						page.color(15);
						cout << "\nColour successfully updated." << endl;
						cout << endl;
						page.color(7);
						system("pause");
					}
				}
			}
			//Modify Product Gender	
			else if (editChoice == "3")
			{
				isValidEditChoice = true;
				bool isValidGender = false;
				string gender;
				int i = 0;
				while (!isValidGender)
				{
					page.color(14);
					cout << setw(20) << "\nNew Gender" << ": ";
					page.color(7);
					gender.clear();
					if (i == 0)
					{
						cin.ignore(1, '\n');
						i++;
					}
					getline(cin, gender);

					isValidGender = input.isNullOrWhiteSpace(gender);
					if (!isValidGender)
					{
						page.color(12);
						cout << "\nPlease fill out this field. " << endl;
						page.color(7);
					}
					else
					{
						pd->setGender(gender);
						pd->save();

						page.color(15);
						cout << "\nGender successfully updated." << endl;
						cout << endl;
						page.color(7);
						system("pause");
					}
				}
			}
			//Modify Product Type		
			else if (editChoice == "4")
			{
				isValidEditChoice = true;
				bool isValidType = false;
				string type;
				int i = 0;
				while (!isValidType)
				{
					page.color(14);
					cout << setw(20) << "\nNew Type" << ": ";
					page.color(7);
					type.clear();
					if (i == 0)
					{
						cin.ignore(1, '\n');
						i++;
					}
					getline(cin, type);

					isValidType = input.isNullOrWhiteSpace(type);
					if (!isValidType)
					{
						page.color(12);
						cout << "\nPlease fill out this field. " << endl;
						page.color(7);
					}
					else
					{
						pd->setType(type);
						pd->save();

						page.color(15);
						cout << "\nType successfully updated." << endl;
						cout << endl;
						page.color(7);
						system("pause");
					}
				}
			}
			//Modify Product Description
			else if (editChoice == "5")
			{
				isValidEditChoice = true;
				bool isValidDescription = false;
				string description;
				int i = 0;
				while (!isValidDescription)
				{
					page.color(14);
					cout << setw(20) << "\nNew Description" << ": ";
					page.color(7);
					description.clear();
					if (i == 0)
					{
						cin.ignore(1, '\n');
						i++;
					}
					getline(cin, description);

					isValidDescription = input.isNullOrWhiteSpace(description);
					if (!isValidDescription)
					{
						page.color(12);
						cout << "\nPlease fill out this field. " << endl;
						page.color(7);
					}
					else
					{
						pd->setDescription(description);
						pd->save();

						page.color(15);
						cout << "\nDescription successfully updated." << endl;
						cout << endl;
						page.color(7);
						system("pause");
					}
				}
			}
			//Modify Product Unit Price
			else if (editChoice == "6")
			{
				isValidEditChoice = true;
				bool isValidUnitPrice = false;
				string strUnitPrice;
				int i = 0;
				while (!isValidUnitPrice)
				{
					page.color(14);
					cout << setw(20) << "\nNew Unit Price" << ": ";
					page.color(7);
					strUnitPrice.clear();
					if (i == 0)
					{
						cin.ignore(1, '\n');
						i++;
					}
					getline(cin, strUnitPrice);

					isValidUnitPrice = input.isDigit(strUnitPrice);

					if (!isValidUnitPrice || strUnitPrice == "")
					{
						page.showInvalidChoiceMsg();
					}
					else
					{
						pd->setUnitPrice(stod(strUnitPrice));
						pd->save();

						page.color(15);
						cout << "\nUnit price successfully updated." << endl;
						cout << endl;
						page.color(7);
						system("pause");
					}
				}
			}
			//Add Size
			else if (editChoice == "7")
			{
				isValidEditChoice = true;
				bool isValidSize = false;
				int i = 0;
				string size;
				while (!isValidSize)
				{
					cout << left << setw(17) << "\nSize" << ": ";
					size.clear();
					if (i == 0)
					{
						cin.ignore(1, '\n');
						i++;
					}
					getline(cin, size);

					isValidSize = input.isNullOrWhiteSpace(size);
					if (!isValidSize)
					{
						page.color(12);
						cout << "\nPlease fill out this field. " << endl;
						page.color(7);
					}
					else
					{
						isValidSize = true;
						ProductStock* psd = new ProductStock();
						psd->setSize(size);
						psd->setProductId(id);
						psd->setCode(generateCode());
						psd->save();
						page.color(15);
						cout << "\nSize \"" << size << "\" for Product #" << id << " is added successfully." << endl;
						page.color(7);
						cout << endl;
						system("pause");
					}
				}
			}
			//Delete Size
			else if (editChoice == "8")
			{
				isValidEditChoice = true;
				bool isValidSize = false;
				bool isCodeExist = false;
				bool isProdActivated = false;
				string size;
				int i = 0;
				while (!isValidSize)
				{
					page.color(14);
					cout << setw(20) << "\nEnter product size" << ": ";
					page.color(7);
					size.clear();
					if (i == 0)
					{
						cin.ignore(1, '\n');
						i++;
					}
					getline(cin, size);


					if (size == "")
					{
						page.color(12);
						cout << "\nPlease fill out this field." << endl;
					}						
					else
					{
						bool isSizeExist = sizeExist(id, size);
						if (isSizeExist)
						{
							isValidSize = true;
							int code = readProdCode(id, size);

							isProdActivated = prodActivated(id);
							if (isProdActivated)
							{
								page.color(12);
								cout << "\nThis product is activated. You can't delete the size of this product." << endl;
								page.color(7);
								cout << endl;
								system("pause");
							}
							else
								deleteSize(code);
						}		
						else
						{
							page.color(12);
							cout << "\nNo result found.\n" << endl;
							page.color(7);
							system("pause");
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

bool ProductController::prodActivated(int id)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("SELECT EXISTS (SELECT * FROM Products WHERE id = ? AND (status = ? OR status = ? ))");

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

bool ProductController::checkProductStatus(int id, string status)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("SELECT EXISTS (SELECT * FROM Products WHERE id = ? AND status = ?)");

	ps->setInt(1, id);
	ps->setString(2, status);

	ResultSet* rs = ps->executeQuery();
	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}

void ProductController::deleteSize(int code)
{
	ProductStock* pss = ProductStock::gett(code);
	pss->erase();
	page.color(15);
	cout << "\nProduct code #" << code << " deleted successfully.\n" << endl;
	page.color(7);
	system("pause");
}

void ProductController::deleteProd(int id)
{
	system("cls");
	page.systemHeaderOrange();
	page.prodMngmtHeader();

	Product* pd = Product::get(id);
	page.color(15);
	cout << "\nShowing details of Product #" << id << endl;
	page.color(7);
	cout << pd->display() << endl;

	showProductStockDetails(id);

	bool isValidDelConf = false;
	bool isProdActivated = false;
	string delConf;
	int i = 0;
	page.color(15);
	cout << "\nAre you sure you want to delete Product #" << id << "?" << endl;
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
			isProdActivated = prodActivated(id);
			if (isProdActivated)
			{				
				changeDisabled(id);
				page.color(15);
				cout << "\nThis product is activated so the status of this product is changed to \"disabled\"." << endl;
				page.color(7);
				cout << endl;
				system("pause");
			}
			else
			{				
				pd->erase();

				deleteProdDetails(id);
				page.color(15);
				cout << "\nProduct #" << id << " deleted successfully.\n" << endl;
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
void ProductController::changeDisabled(int id)
{
	Product* pd = Product::get(id); 
	if (pd != nullptr)
	{
		string status = "disabled";
		pd->setStatus(status);
		pd->save();
	}
}
void ProductController::deleteProdDetails(int productId)
{
	string str = "SELECT * FROM product_stocks WHERE product_id = ? ";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, productId);
	ResultSet* result = ps->executeQuery();
	int array[20];
	
	int SIZE = countSize(productId);
	for (int i = 0; i < SIZE; i++)
	{
		result->next();	
		array[i] = result->getInt("id");		
	}
	eraseProdDetails(array, SIZE);
}

void ProductController::eraseProdDetails(int *array,  int SIZE)
{
	for (int i = 0; i < SIZE; i++)
	{
		ProductStock* pss = ProductStock::get(array[i]);
		pss->erase();
	}
}
int ProductController::countSize(int productId)
{
	string str = "SELECT COUNT(size) AS count FROM product_stocks WHERE product_id = ? ";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, productId);
	ResultSet* result = ps->executeQuery();
	result->next();
	return (int)result->getInt64("count");
}

int ProductController::countLowStockProduct(int value)
{
	string str = "select count(*)as count from(Select count(product_id) from products inner JOIN product_stocks on(products.id = product_stocks.product_id) group by products.id having sum(quantity) <= ?) as groupss";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, value);
	ResultSet* result = ps->executeQuery();
	result->next();
	return (int)result->getInt64("count");
}

void ProductController::viewLowStockProduct(int value, const int a)
{
	int b = a;

	int totalLowStock = countLowStockProduct(value);

	if (b <= totalLowStock)
	{
		page.color(15);

		string str = "Select products.id,products.name, products.colour, products.gender, products.type, products.unit_price, products.status, sum(quantity) AS totalQuantity from products inner JOIN product_stocks on(products.id = product_stocks.product_id) group by product_stocks.product_id having sum(quantity) <= ? order by product_stocks.product_id LIMIT 10 OFFSET ?";

		PreparedStatement* ps = Database::prepareStatement(str);
		ps->setInt(1, value);
		ps->setInt(2, b);
		ResultSet* result = ps->executeQuery();

		int size = ((int)result->rowsCount() < totalLowStock) ? (int)result->rowsCount() : totalLowStock;

		//!!!!!!!!!!!

		cout << "\nShowing " << b + 1;
		if (b + 10 <= size)
			cout << "~" << b + 10 << " out of " << totalLowStock << " results for Total Quantity <= " << value << "\n" << endl;
		else
			cout << "~" << totalLowStock << " out of " << totalLowStock << " results for Total Quantity <= " << value << "\n" << endl;

		page.color(7);
		cout << right << setw(5) << "No." << setw(5) << "ID" << setw(4) << " " << left << setw(80) << "Product Name" << setw(4) << " ";
		cout << left << setw(15) << "Colour" << setw(4) << " " << left << setw(7) << "Gender" << setw(4) << " " << left << setw(20) << "Type";
		cout << setw(4) << " " << left << setw(16) << "Unit Price (RM)" << setw(4) << " ";
		cout << left << setw(15) << "Status" << setw(4) << " " << "Total Quantity" << endl;
		//try {
		for (int i = 0; i < size; i++)
		{
			result->next();
			cout << right << setw(5) << b + 1 << setw(5) << result->getInt("id") << setw(4) << " " << left << setw(80) << result->getString("name");
			cout << setw(4) << " " << left << setw(15) << result->getString("colour") << setw(4) << " " << left << setw(7) << result->getString("gender") << setw(4) << " ";
			cout << left << setw(20) << result->getString("type") << setw(4) << " " << right << setw(15) << fixed << showpoint << setprecision(2) << result->getDouble("unit_price");
			cout << setw(4) << " " << left << setw(12) << result->getString("status") << setw(4) << " " << right << setw(13) << result->getInt("totalQuantity") << endl;
			b++;
		}
		//}
		//catch (exception ex)
		//{
		//	cout << ex.what();
		//}
	}
}

void ProductController::prodSearchViewUser()
{
	bool isBack = false;
	while (!isBack)
	{
		bool isStopLoop = false;

		int m = 0, a = 0;
		int id = 0;
		string prodChoice;
		string searchVal = "";
		bool isProdNameFound = false;

		while (!isStopLoop)
		{
			bool isValidProdChoice = false;

			if (isProdNameFound)
			{
				system("cls");
				page.systemHeaderBlue();
				page.prodSearchViewHeaderUser();
				viewProdListProductName(searchVal, a);
			}				
			else
			{
				system("cls");
				page.systemHeaderBlue();
				page.prodSearchViewHeaderUser();
				viewProdList(m);
			}
				
			cout << endl;
			page.prodSearchViewPanelUser();
			

			while (!isValidProdChoice)
			{
				page.color(14);
				cout << "\nEnter choice: ";
				page.color(7);
				prodChoice.clear();
				cin >> prodChoice;

				//Previous Page
				if (prodChoice == "1")
				{
					isValidProdChoice = true;

					if (isProdNameFound)
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
				else if (prodChoice == "2")
				{
					isValidProdChoice = true;
					if (isProdNameFound)
					{
						if (a + 10 >= countSearchProductName(searchVal))
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
						if (m + 10 >= Product::count())
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
			
				//Search Product
				else if (prodChoice == "3")
				{
					isValidProdChoice = true;
					a = 0;
					isProdNameFound = false;

					int i = 0;
					bool isValidName = false;
					while (!isValidName)
					{
						page.color(14);
						cout << "\nEnter product name to search: ";
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
							isValidName = true;
							bool isNameExist = prodNameExist(searchVal);
							if (isNameExist)
								isProdNameFound = true;
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

				//View Product Details
				else if (prodChoice == "4")
				{
					isValidProdChoice = true;
					string strId;
					bool isDigit = false;
					bool isValidStrId = false;

					while (!isValidStrId)
					{
						page.color(14);
						cout << "\nEnter Product ID: ";
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
							{
								system("cls");
								page.systemHeaderBlue();
								page.prodSearchViewHeaderUser();

								Product* pd = Product::get(id);
								page.color(15);
								cout << "\nShowing details of Product #" << id << endl;
								page.color(7);
								cout << pd->display() << endl;

								showProductStockDetails(id);

								cout << endl;
								page.color(7);
								system("pause");
							}
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

				//Back
				else if (prodChoice == "5")
				{
					if (isProdNameFound)
					{
						isValidProdChoice = true;
						isStopLoop = true;
					}					
					else
					{
						isValidProdChoice = true;
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

void ProductController::purOrderShowProdDetails(int productId)
{
	Product* pd = Product::get(productId);
	page.color(15);
	cout << "\nShowing details of Product #" << productId << endl;
	page.color(7);
	pd->display();
	cout << endl;

	showProductStockDetails(productId);
	cout << endl;
}
void ProductController::searchProdName(string searchVal)
{
	int b = 0;
	int totalSearch = countSearchProductName(searchVal);

	string str = "Select products.id,products.`name`, products.colour, products.gender, products.type, products.unit_price, products.`status`, sum(quantity) AS totalQuantity "
		+ std::string("from products inner JOIN product_stocks on(products.id=product_stocks.product_id) WHERE products.`name` LIKE ? group by product_stocks.product_id ")
		+ "order by product_stocks.product_id ";	

	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setString(1, "%" + searchVal + "%");
	ResultSet* result = ps->executeQuery();

	int SIZE = ((int)result->rowsCount() < totalSearch) ? (int)result->rowsCount() : totalSearch;

	page.color(15);
	cout << "\nShowing " << b + 1;
	cout << "~" << countSearchProductName(searchVal) << " out of " << countSearchProductName(searchVal) << " results for \"" << searchVal << "\".\n" << endl;

	page.color(7);
	cout << right << setw(5) << "No." << setw(5) << "ID" << setw(4) << " " << left << setw(80) << "Product Name" << setw(4) << " ";
	cout << left << setw(15) << "Colour" << setw(4) << " " << left << setw(7) << "Gender" << setw(4) << " " << left << setw(20) << "Type";
	cout << setw(4) << " " << left << setw(16) << "Unit Price (RM)" << setw(4) << " ";
	cout << left << setw(15) << "Status" << setw(4) << " " << "Total Quantity" << endl;

	for (int i = 0; i < SIZE; i++)
	{
		result->next();
		cout << right << setw(5) << b + 1 << setw(5) << result->getInt("id") << setw(4) << " " << left << setw(80) << result->getString("name");
		cout << setw(4) << " " << left << setw(15) << result->getString("colour") << setw(4) << " " << left << setw(7) << result->getString("gender") << setw(4) << " ";
		cout << left << setw(20) << result->getString("type") << setw(4) << " " << right << setw(15) << fixed << showpoint << setprecision(2) << result->getDouble("unit_price");
		cout << setw(4) << " " << left << setw(12) << result->getString("status") << setw(4) << " " << right << setw(13) << result->getInt("totalQuantity") << endl;
		b++;
	}

}

bool ProductController::sizeExist(int productId, string size)
{
	bool found = false;
	DatabaseConnection dbConn;
	PreparedStatement* ps = dbConn.prepareStatement("SELECT EXISTS (SELECT * FROM product_stocks WHERE product_id = ? AND size = ?)");

	ps->setInt(1, productId);
	ps->setString(2, size);

	ResultSet* rs = ps->executeQuery();
	rs->next();
	if (rs->getInt(1) == 1)
		found = true;

	delete rs;
	delete ps;

	return found;
}

int ProductController::readProdCode(int productId, string size)
{
	string str = "select product_stocks.`code` from product_stocks where product_id = ? and size = ? ";
		
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, productId);
	ps->setString(2, size);
	ResultSet* result = ps->executeQuery();

	result->next();
	return (result->getInt("code"));
}

string ProductController::readProdSizeId(int productId, string size)
{
	string str = "select size from product_stocks where product_id = ? and size = ? ";

	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, productId);
	ps->setString(2, size);
	ResultSet* result = ps->executeQuery();

	result->next();
	return (result->getString("size"));
}

string ProductController::readProdSizeCode(int productCode)
{
	string str = "select size from product_stocks where code = ? ";

	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, productCode);
	ResultSet* result = ps->executeQuery();

	result->next();
	return (result->getString("size"));
}

string ProductController::readProdNameId(int productId, string size)
{
	string str = "select `name` from products inner join product_stocks on (products.id = product_id) where product_id = ? and size = ? ";

	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, productId);
	ps->setString(2, size);
	ResultSet* result = ps->executeQuery();

	result->next();
	return (result->getString("name"));
}

string ProductController::readProdNameCode(int productCode)
{
	string str = "select `name` from products inner join product_stocks on(products.id = product_id) where code = ?";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, productCode);
	ResultSet* result = ps->executeQuery();

	result->next();
	return (result->getString("name"));
}

double ProductController::readProdPrice(int productId)
{
	string str = "select unit_price from products where id = ?";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, productId);
	ResultSet* result = ps->executeQuery();

	result->next();
	return (result->getDouble("unit_price"));
}

int ProductController::readProdIdCode(int productCode)
{
	string str = "select product_id from products inner join product_stocks on(products.id = product_id) where code = ?";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, productCode);
	ResultSet* result = ps->executeQuery();

	result->next();
	return (result->getInt("product_id"));
}

int ProductController::readProdQuantityCode(int productCode)
{
	string str = "select quantity from products inner join product_stocks on(products.id = product_id) where code = ?";
	PreparedStatement* ps = Database::prepareStatement(str);
	ps->setInt(1, productCode);
	ResultSet* result = ps->executeQuery();

	result->next();
	return (result->getInt("quantity"));
}

void ProductController::transShowProdCodeDetails(int productId, int productcode)
{
	page.color(15);
	cout << "\nShowing details of Product Code #" << productcode << endl;
	Product* pd = Product::get(productId);
	
	page.color(7);
	pd->displayDetails();
	ProductStock* ps = ProductStock::gett(productcode);
	ps->displayDetails();
	cout << "\n" << endl;
}

