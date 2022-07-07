
#include "CheckInput.h"
#include "IndexPage.h"

#ifndef REPORTCONTROLLER_H
#define REPORTCONTROLLER_H

class ReportController
{
private:
	IndexPage page;
	CheckInput input;

public:

	void reportManagement();

	void salesReport();
	void totalSales();
	void annualSales();
	int countTotalAnnualSales(string year);
	void monthlySales();
	int countTotalMonthlySales(string year, string month);
	void dailySales();
	int countTotalDailySales(string date);

	void bestSellerReport();
	void totalBestSeller();
	void annualBestSeller();
	int countTotalAnnualBestSeller(string year);
	void monthlyBestSeller();
	int countTotalMonthlyBestSeller(string year, string month);
	void dailyBestSeller();
	int countTotalDailyBestSeller(string date);
};

#endif


