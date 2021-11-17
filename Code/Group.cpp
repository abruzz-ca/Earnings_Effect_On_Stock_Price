#include "Group.hpp"
#include <iostream>


//overload += operator for vectors
vector<double>& operator += (vector<double>& v1, const vector<double>& v2)
{
	if (v1.size() != v2.size())
	{
		cerr << "The two vectors are of different sizes!" << endl;		//print out error
	}

	for (int i = 0; i < v1.size(); i++)
	{
		v1[i] += v2[i];
	}

	return v1;
}


//overload /= operator for vectors
vector<double>& operator /= (vector<double>& v, double num)
{
	for (int i = 0; i < v.size(); i++)
	{
		v[i] = v[i] / num;
	}

	return v;
}


const vector<double>& Group::GetAAR() const 
{
	return AAR;
}

const vector<double>& Group::GetCAAR() const 
{
	return CAAR;
}

const Stock& Group::GetStock(string ticker) const 
{
	auto i = stocks.find(ticker);
	return i->second;
}

void Group::Computing(const map<string, Stock>& stockMap) 
{

	//cout << "Perform computing " << endl;

	for (auto i = tickers.begin(); i != tickers.end(); ++i) 
	{

		string tempi = *i;					//dereference tickers' iterator
		vector<double> temp = stockMap.find(tempi)->second.GetExcessReturn();
		temp.erase(temp.begin());			//remove first element which is NULL
		
		AAR += temp;
	}
	AAR /= tickers.size();

	CAAR[0] = AAR[0];

	for (int i = 1; i < 2 * N; i++) 
	{
		CAAR[i] = CAAR[i - 1] + AAR[i];
	}
}

const vector<string>& Group::Gettickers() const 
{
	return tickers;
}


void Group::addnew(const Stock& s) 
{
	stocks.insert(pair<string, Stock>(s.GetTicker(), s));
	tickers.push_back(s.GetTicker());
}