
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

#include "InvestmentTester.h"

void InvestmentTester::runTests() {

	cout << endl;
	cout << "----------Automated Tests--------------" << endl;

	int passedTests = testInvestment();

	cout << endl;
	cout << passedTests << " out of 4 tests passed." << endl;

	cout << "All automated tests completed." << endl;

	cout << "---------------------------------------" << endl;
	cout << endl;
}

int InvestmentTester::testInvestment() {

	int passedTests = 0;

	MonthlyGains firstInvestment;

	//Set test values
	firstInvestment.setInitial(1000.00);
	firstInvestment.setMonthly(100.00);
	firstInvestment.setInterest(5.00);
	firstInvestment.setNumYears(1);

	//Calculates balance
	firstInvestment.setEndBalanceNoMonthly();
	firstInvestment.setYearEndBalanceMonthly();

	//Expected results with test values
	double expectedNoMonthly = 1051.16;
	double expectedMonthly = 2284.16;

	//Calculated results with test values
	double actualNoMonthly = firstInvestment.getNoMonthlyBalance();
	double actualMonthly = firstInvestment.getMonthlyBalance();

	//Display test results
	cout << fixed << setprecision(2);
	cout << endl;
	cout << "First Test Case" << endl;
	cout << "Test 1 without monthly deposits: ";

	if (abs(actualNoMonthly - expectedNoMonthly) < 0.01) {
		cout << "PASS" << endl;
		passedTests++;
	}
	else {
		cout << "FAIL" << endl;
	}

	cout << "Test 2 with monthly deposits: ";

	if (abs(actualMonthly - expectedMonthly) < 0.01) {
		cout << "PASS" << endl;
		passedTests++;
	}
	else {
		cout << "FAIL" << endl;
	}


	//Runs a second test
	cout << endl;
	cout << "Second test Case" << endl;

	MonthlyGains secondInvestment;

	//Set test values
	secondInvestment.setInitial(5000.00);
	secondInvestment.setMonthly(200.00);
	secondInvestment.setInterest(4.00);
	secondInvestment.setNumYears(2);

	//Calculates balance
	secondInvestment.setEndBalanceNoMonthly();
	secondInvestment.setYearEndBalanceMonthly();
	
	//Expected resultes with test values
	double expectedSecondNoMonthly = 5415.71;
	double expectedSecondMonthly = 10420.92;

	//Calculated results with test values
	double actualSecondNoMonthly = secondInvestment.getNoMonthlyBalance();
	double actualSecondMonthly = secondInvestment.getMonthlyBalance();

	cout << "Test 3 without monthly deposits: ";

	if (abs(actualSecondNoMonthly - expectedSecondNoMonthly) < 0.01) {
		cout << "PASS" << endl;
		passedTests++;
	}
	else {
		cout << "FAIL" << endl;
	}

	cout << "Test 4 with monthly deposits: ";

	if (abs(actualSecondMonthly - expectedSecondMonthly) < 0.01) {
		cout << "PASS" << endl;
		passedTests++;
	}
	else {
		cout << "FAIL" << endl;
	}

	return passedTests;

}

