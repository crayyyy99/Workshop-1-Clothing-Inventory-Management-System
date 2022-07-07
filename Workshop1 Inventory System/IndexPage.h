#ifndef INDEXPAGE_H
#define INDEXPAGE_H

class IndexPage 
{
	public:
		void color(unsigned short color);
		void chgConTitleScrnSize();

		void sysWelcomeHeader();
		void systemHeaderYellow();
		void systemHeaderOrange();
		void systemHeaderBlue();
		void userTypePageHeader();

		//Employee
		void userLoginPageHeader();
		void adminLoginPageHeader();
		void registrationHeader();
		void userPanelHeader();
		void adminPanelHeader();
		void editAccDetailsHeader();
		void empMngmtHeader();
		void editUserDetailsHeader();
		
		void selectLoginRegisterPanel();
		void selectUserTypePanel();
		void exitConfirmPanel();
		void logoutConfirmPanel();
		void registrationConfirmPanel();
		void userPanel();
		void adminPanel();
		void editAccDetailsPanelUser();
		void empMngmtPanel();
		void modifyEmpPanel();

		//Supplier		
		void suppMngmtHeader();

		void suppMngmtPanel();
		void modifySuppPanel();

		//Product
		void prodMngmtHeader();

		void prodMngmtPanel();
		void addSizePanel();
		void modifyProdPanel();
		
		void prodSearchViewHeaderUser();
		void prodSearchViewPanelUser();

		//Purchase Order
		void purOrderMngmtHeader();

		void purOrderMngmtPanel();
		void searchPurOrderPanel();

		//Transaction
		void addtransHeader();
		void transMngmtHeader();
		void transMngmtPanel();
		void searchTransPanelAdmin();
		void transSearchViewPanelUser();
		void transSearchViewHeaderUser();

		//Report
		void viewReportHeader();
		void viewReportTypePanel();
		void viewReportTimePanel();
		void viewSalesReportHeader();
		void viewBestSellerReportHeader();

		//Message
		void showYesNoChoice();
		void showInvalidChoiceMsg();
		void showSearchDateFormat();
};


#endif // !INDEXPAGE_H

