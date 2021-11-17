#pragma once
#include <vector>
#include "Stocks.hpp"
#include "Group.hpp"
#include <stdlib.h>
#include <vector>
#include <map>

class Bootstrap 
{

private:
	int N;														//N must be between 30 and 60
	vector<Group> ThreeGroups = vector<Group>(3);				//3 big groups
	vector<vector<vector<double>>> A_AAR_CAAR = vector<vector<vector<double>>>(3);				//average AAR and CAAR for 2N days for 3 groups
	vector<vector<vector<double>>> STD_AAR_CAAR = vector<vector<vector<double>>>(3);			//standard deviation of AAR and CAAR for 2N days for 3 groups
	
	
public:
	Bootstrap()
	{
		N = 30;													//N = 30 by default
		for (int g = 0; g < 3; ++g) 
		{
			A_AAR_CAAR[g].push_back(vector<double>(2 * N));		//adding a vector of 2N doubles to store average AAR of each group  
			A_AAR_CAAR[g].push_back(vector<double>(2 * N));		//adding a vector of 2N doubles to store average CAAR of each group
			
			STD_AAR_CAAR[g].push_back(vector<double>(2 * N));	//adding a vector of 2N doubles to store standard deviation of AAR of each group
			STD_AAR_CAAR[g].push_back(vector<double>(2 * N));	//adding a vector of 2N doubles to store standard deviation of CAAR of each group
		}

	};
	Bootstrap(int N_) 
	{	//constructor with setting N
		N = N_;				
		for (int g = 0; g < 3; ++g) 
		{
			A_AAR_CAAR[g].push_back(vector<double>(2 * N));
			A_AAR_CAAR[g].push_back(vector<double>(2 * N));
			STD_AAR_CAAR[g].push_back(vector<double>(2 * N));
			STD_AAR_CAAR[g].push_back(vector<double>(2 * N));
		}
	};
	~Bootstrap() {};
	void SetN(int N_);											//Set N
	const vector<vector<double>>& GetA_AAR_CAAR(int g) const;	//get A_AAR_CAAR of group g, 0 for beat, 1 for meeting, 2 for miss
	const vector<vector<double>>& GetSTD_AAR_CAAR(int g) const;	//get STD_AAR_CAAR of group g, 0 for beat, 1 for meeting, 2 for miss	
	const Group& GetBigGroup(int index) const;					//get group details
	void WriteGroups(const map<string, Stock>& allstocks_);		//separate all stocks into 3 groups
	Group Sampling(const Group& group);							//randomly select a sample of a group 50 stocks from the bigger group
	void Perform(const map<string, Stock>& stockMap);			//perform bootstrapping
	void Display() const;										//get instruction and display average AAR, CAAR and its standard deviation
};