
#pragma once

#include "MonthlyGains.h"

class InvestmentManager {

public:

	//Starts investment calculator
	void run();

private:

	//Displays main menu
	void displayMenu();

	//Gets investment informaiton form the user
	bool getInvestmentInput(MonthlyGains& investment);

	//Display investment results
	void displayResults(const MonthlyGains& investment);

	//Compares two investment plans
	void compareInvestments();
};