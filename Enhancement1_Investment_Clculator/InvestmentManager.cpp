
#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>

using namespace std;

#include "InvestmentManager.h"
#include "InvestmentTester.h"

void InvestmentManager::run() {
	
	int choice = 0;

	while (choice != 4) {

		displayMenu();

		cout << "Enter your choice: ";
		cin >> choice;

		//Validates input
		if (cin.fail()) {

			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			cout << "Invalid input" << endl;
			cout << endl;

			continue;
		}

		//Calculate Investment
		if (choice == 1) {

			MonthlyGains userGains;
			if (getInvestmentInput(userGains)) {

				userGains.setEndBalanceNoMonthly();
				userGains.setYearEndBalanceMonthly();

				displayResults(userGains);
			}
		}

		//Compare Two investments
		else if (choice == 2) {

			compareInvestments();
		}

		//Automated test
		else if (choice == 3) {

			InvestmentTester tester;

			tester.runTests();
		}

		//Close program
		else if (choice == 4) {

			cout << "Exiting Calculator" << endl;
		}
		else {
			cout << "Invalid input" << endl;
			cout << endl;
		}
	}	
}

//Data Input Display
void InvestmentManager::displayMenu() {
	
	cout << "-----------------------------------------------------------" << endl;
	cout << "--------------------Investment Calculator------------------" << endl;
	cout << "-----------------------------------------------------------" << endl;

	cout << "1. Calculate Investment" << endl;
	cout << "2. Compare Two Investments" << endl;
	cout << "3. Run Automated Tests" << endl;
	cout << "4. Exit" << endl;

}

//Collect user input
bool InvestmentManager::getInvestmentInput(MonthlyGains& investment) {

	double userInvestment;
	double userMonthlyDeposit;
	double userAnnualInterest;
	int userNumYears;

	cout << "Inital Investment Amount: ";
	cin >> userInvestment;

	//Validates investment input
	if (cin.fail() || userInvestment < 0) {

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "Invalid investment." << endl;
		cout << endl;

		return false;
	}

	cout << "Monthly Deposit:";
	cin >> userMonthlyDeposit;

	//Validates deposit input
	if (cin.fail() || userMonthlyDeposit < 0) {

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "Invalid monthly deposit." << endl;
		cout << endl;

		return false;
	}

	cout << "Annual Interest:";
	cin >> userAnnualInterest;

	//Validates interest input
	if (cin.fail() || userAnnualInterest < 0) {

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "Invalid annual interest." << endl;
		cout << endl;

		return false;
	}

	cout << "Number of years:";
	cin >> userNumYears;

	//Validates years input
	if (cin.fail() || userNumYears <= 0) {

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "Invalid number of years." << endl;
		cout << endl;

		return false;
	}

	//Check if inputs are valid
	if (cin.fail()) {

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "There was an invalid input." << endl;
		cout << endl;

		return false;
	}

	investment.setInitial(userInvestment);
	investment.setMonthly(userMonthlyDeposit);
	investment.setInterest(userAnnualInterest);
	investment.setNumYears(userNumYears);

	return true;
}

//Display investment result
void InvestmentManager::displayResults(const MonthlyGains& investment) {

	cout << endl;

	cout << "Investment Results" << endl;
	cout << "-----------------------------------------------------------" << endl;

	//Display with no monthly Investment
	cout << "Without Monthly Deposit" << endl;
	cout << "Final Balance: $"
		<< fixed << setprecision(2)
		<< investment.getNoMonthlyBalance()
		<< endl;
	cout << "Interest Earned: $"
		<< fixed << setprecision(2)
		<< investment.getNoMonthlyInterest()
		<< endl;
	cout << endl;

	//Display with monthly investment
	cout << "With Monthly Deposit" << endl;
	cout << "Final Balance: $"
		<< fixed << setprecision(2)
		<< investment.getMonthlyBalance()
		<< endl;
	cout << "Interest Earned: $"
		<< fixed << setprecision(2)
		<< investment.getMonthlyInterest()
		<< endl;
	cout << "-----------------------------------------------------------" << endl;
	cout << endl;
}

void InvestmentManager::compareInvestments() {

	MonthlyGains investmentA;
	MonthlyGains investmentB;

	cout << endl;
	cout << "-----------------------Investment A-------------------------" << endl;


	if (!getInvestmentInput(investmentA)) {
		return;
	}

	cout << endl;
	cout << "-----------------------Investment B-------------------------" << endl;

	if (!getInvestmentInput(investmentB)) {
		return;
	}

	//Calculate Investment A
	investmentA.setEndBalanceNoMonthly();
	investmentA.setYearEndBalanceMonthly();

	//Calculate investment B
	investmentB.setEndBalanceNoMonthly();
	investmentB.setYearEndBalanceMonthly();

	//Comparison display
	cout << endl;
	cout << "-------------------------Comparison------------------------" << endl;

	cout << fixed << setprecision(2);

	cout << endl;

	cout << left << setw(22) << ""
		<< setw(18) << "Investment A"
		<< setw(18) << "Investment B"
		<< endl;

	cout << "-----------------------------------------------------------" << endl;

	cout << left << setw(22) << "Without Deposit"
		<< right << "$" << setw(10) << fixed << setprecision(2)
		<< investmentA.getNoMonthlyBalance()
		<< "       $" << setw(10)
		<< investmentB.getNoMonthlyBalance()
		<< endl;

	cout << left << setw(22) << "With Deposit"
		<< right << "$" << setw(10) << fixed << setprecision(2)
		<< investmentA.getMonthlyBalance()
		<< "       $" << setw(10)
		<< investmentB.getMonthlyBalance()
		<< endl;


	double balanceA = investmentA.getMonthlyBalance();
	double balanceB = investmentB.getMonthlyBalance();

	double difference = abs(balanceA - balanceB);

	cout << "-----------------------------------------------------------" << endl;

	//Shows the difference and what investment has the larger final balance
	cout << "Balanace Difference: $"
		<< difference
		<< endl;
	if (balanceA > balanceB) {
		cout << "Investment A has the larger final balance." << endl;
	}
	else if (balanceB > balanceA) {
		cout << "Investment B has the larger final balance." << endl;
	}
	else {
		cout << "Both investments have the same final balance" << endl;
	}

	cout << endl;

}
