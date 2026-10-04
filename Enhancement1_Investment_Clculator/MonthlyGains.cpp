
#include "MonthlyGains.h"

void MonthlyGains::setInitial(double t_userInitial) { 
	//used to set initial depost
	m_InitialInvestment = t_userInitial;
}

void MonthlyGains::setMonthly(double t_userMonthlyDeposit) { 
	//used to set monthly depost
	m_MonthlyDeposit = t_userMonthlyDeposit;
}

void MonthlyGains::setInterest(double t_userInterest) { 
	//used to set Interest rate
	m_AnnualInterest = t_userInterest;
}

void MonthlyGains::setNumYears(int t_userYears) { 
	// used to set total number of years
	m_NumYears = t_userYears;
}

void MonthlyGains::setEndBalanceNoMonthly() {
	
	//set all my variables in the function
	double startingAmount = m_InitialInvestment;
	int totalMonths = m_NumYears * 12;
	int i;
	double monthlyInterest;
	double total = 0.0;
	double yearInterest = 0.0;
	int yearCount = 12;
	int year = 0;

	
	//loops for each month
	for (i = 0; i != totalMonths; i++) {
		yearCount--;

		//Calculate interest each month
		monthlyInterest = startingAmount * ((m_AnnualInterest / 100) / 12);
		//Calculae new starting amount
		startingAmount = startingAmount + monthlyInterest;
		//adds monthly interest to to the total year intrest
		yearInterest = monthlyInterest + yearInterest;
		total = startingAmount;

		//loops every 12 months, stores yearly result
		if (yearCount == 0) {
			year = year++;
			m_NoMonthlyBalance = total;
			m_NoMonthlyInterest = yearInterest;
			yearInterest = 0.0;
			yearCount = 12;
		}
	}

	m_NoMonthlyBalance = total;
}

//used to set the end of year balance
void MonthlyGains::setYearEndBalanceMonthly() {

	//set all my variables in the function
	double startingAmount = m_InitialInvestment;
	int totalMonths = m_NumYears * 12;
	int i;
	double monthlyInterest;
	double total = 0.0;
	double yearInterest = 0.0;
	int yearCount = 12;
	int year = 0;



	//loops for each month
	for (i = 0; i != totalMonths; i++) {
		yearCount--;
		
		//Calculate interest earned each month
		monthlyInterest = (startingAmount + m_MonthlyDeposit) * ((m_AnnualInterest / 100) / 12);
		//Calculate new starting amount
		startingAmount = startingAmount + m_MonthlyDeposit + monthlyInterest;
		//adds monthly interest to the total yeat interest
		yearInterest = monthlyInterest + yearInterest;
		total = startingAmount;
		
		//loops every 12 months, stores yearly result
		if (yearCount == 0) {
			year++;
			m_MonthlyBalance = total;
			m_MonthlyInterest = yearInterest;
			yearInterest = 0.0;
			yearCount = 12;
		}
	}
	m_MonthlyBalance = total;
}

//Get Year balance with monthly deposit
double MonthlyGains::getNoMonthlyBalance() const {
	return m_NoMonthlyBalance;
}

//Get Interest with monthly deposit
double MonthlyGains::getNoMonthlyInterest() const {
	return m_NoMonthlyInterest;
}

//Get year balance without monthly deposit
double MonthlyGains::getMonthlyBalance() const {
	return m_MonthlyBalance;
}

//get Interest without monthly deposit
double MonthlyGains::getMonthlyInterest() const {
	return m_MonthlyInterest;
}