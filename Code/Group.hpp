#pragma once
#include<map>
#include<string>
#include<vector>
#include "Stocks.hpp"

class Group 
{
private:
	int N;										//N must be between 30 and 60
	map<string, Stock> stocks;					//all stocks info in the group
	vector<string> tickers;						//tickers for all the stocks in the group => to use in shuffle/Bootstrap
	vector<double> AAR;							//AAR for each group
	vector<double> CAAR;						//CAAR for each group

public:
	Group() 
	{//default constructor
		N = 30;											//N = 30 by default
		vector<double> AAR = vector<double>(2 * N);		//AAR for 2N days for each group
		vector<double> CAAR = vector<double>(2 * N);	//CAAR for 2N days for each group

	};		
	Group(int N_)
	{//constructor with setting N

		N = N_;
		AAR = vector<double>(2 * N);			//AAR for 2N days for each group
		CAAR = vector<double>(2 * N);			//CAAR for 2N days for each group

	};
	Group(const Group& group_) :stocks(group_.stocks), tickers(group_.tickers), AAR(group_.AAR), CAAR(group_.CAAR), N(group_.N) {};		//constructor with parameters
	~Group() {};	//destructor
	const vector<double>& GetAAR() const;				//to retrieve AAR data member
	const vector<double>& GetCAAR() const;				//to retrieve CAAR data member
	const Stock& GetStock(string ticker) const;			//get details of a stock in the group with ticker name
	const vector<string>& Gettickers() const;			//to retrieve tickers data member
	void Computing(const map<string, Stock>& stockMap);	//computing AAR and CAAR data member
	void addnew(const Stock& s);						//add a new stock into the group
};