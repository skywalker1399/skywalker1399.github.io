
#pragma once

class MonthlyGains {
public:

	//Name investment variables (t_ to note they are temporary)
	void setInitial(double t_InitialInvestment);
	void setMonthly(double t_MonthlyDepost);
	void setInterest(double t_AnnualInterest);
	void setNumYears(int t_NumYears);

	//Calculate investment balance
	void setEndBalanceNoMonthly();
	void setYearEndBalanceMonthly();

	//Get calculation results
	double getNoMonthlyBalance() const;
	double getNoMonthlyInterest() const;

	double getMonthlyBalance() const;
	double getMonthlyInterest() const;

private:
	//Name variables 
	double m_InitialInvestment;
	double m_MonthlyDeposit;
	double m_AnnualInterest;
	int m_NumYears;

	//Calculation results no monthly deposit
	double m_NoMonthlyInterest;
	double m_NoMonthlyBalance;

	//Calculation results with monthly deposit
	double m_MonthlyInterest;
	double m_MonthlyBalance;
};