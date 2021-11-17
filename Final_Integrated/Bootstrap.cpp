#include "Bootstrap.hpp"
#include <algorithm>
#include <random>
#include <chrono>
#include <math.h>
#include <iomanip>			//to set precision in cout


void Bootstrap::SetN(int N_)
{
	N = N_;
}

const vector<vector<double>>& Bootstrap::GetA_AAR_CAAR(int g) const 
{
	return A_AAR_CAAR[g];
}

const vector<vector<double>>& Bootstrap::GetSTD_AAR_CAAR(int g) const 
{
	return STD_AAR_CAAR[g];
}

const Group& Bootstrap::GetBigGroup(int index) const 
{
	return ThreeGroups[index];
}


void Bootstrap::WriteGroups(const map<string, Stock>& allstocks_) 
{
	for (auto i = allstocks_.begin(); i != allstocks_.end(); i++) 
	{
		GR g = i->second.GetGroup();

		switch (g)
		{
		case beat:
			ThreeGroups[0].addnew(i->second);
			break;
		case meet:
			ThreeGroups[1].addnew(i->second);
			break;
		case miss:
			ThreeGroups[2].addnew(i->second);
			break;
		}
	}
}

Group Bootstrap::Sampling(const Group& group) 
{
	Group sampled(N);
	vector<string> keys = group.Gettickers();											//get all the tickers from big group

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();		//producing seed. time since epoch Jan 1 00:00:00 1970
	std::shuffle(keys.begin(), keys.end(), std::default_random_engine(seed));			//randomly rearrange elements - taking uniform random number generator as parameter

	for (int i = 0; i < 50; ++i) 
	{
		sampled.addnew(group.GetStock(keys[i]));										//add 50 stocks into the group
	}

	return sampled;
}


void  Bootstrap::Perform(const map<string, Stock>& stockMap) 
{
	vector< map<int, vector<vector<double>>>> ALL_AAR_CAAR(3);	//to store all AAR and CAAR for the 3 groups to compute STD_AAR_CAAR

	//cout << "Start Bootstrapping" << endl;
		
	for (int i = 0; i < 40; ++i)								//40 samplings
	{								
		for (int j = 0; j < 3; ++j) 
		{
			Group sampled = Sampling(ThreeGroups[j]);			//randomly select 50 stocks from each group
		
			sampled.Computing(stockMap);						//compute AAR and CAAR for each group

			std::vector<double> AAR = sampled.GetAAR();			//retrieve AAR from each group
			std::vector<double> CAAR = sampled.GetCAAR();		//retrieve CAAR from each group

			ALL_AAR_CAAR[j][0].push_back(AAR);					//store AAR to ALL_AAR_CAAR
			ALL_AAR_CAAR[j][1].push_back(CAAR);					//store AAR to ALL_AAR_CAAR
	
			for (int k = 0; k < 2 * N; k++) 
			{
				A_AAR_CAAR[j][0][k] = (A_AAR_CAAR[j][0][k] * i + AAR[k]) / (i + 1);		//compute average AAR for each group
				A_AAR_CAAR[j][1][k] = (A_AAR_CAAR[j][1][k] * i + CAAR[k]) / (i + 1);	//compute average CAAR for each group
			}
		}

		//std::cout << "Bootstrap Number " << i + 1 << " Done." << std::endl;
	}

	//compute standard deviation
	for (int g = 0; g < 3; g++)
	{
		for (int a = 0 ;a < 2; a++)
		{
			for (int n = 0; n < 2*N; n++)
			{
				double temp = 0;

				for (int t = 0; t < 40;t++)
				{
					temp += pow((A_AAR_CAAR[g][a][n] - ALL_AAR_CAAR[g].find(a)->second[t][n]), 2);
				}
				
				STD_AAR_CAAR[g][a][n] = sqrt(temp / 39);

			}
		}
	}
}

void Bootstrap::Display() const
{//get instruction and display average AAR, CAAR and its standard deviation

	int gr;
	cout << "Which group do you want to check?" << endl;
	cout << "Type 0 for Beat" << endl;
	cout << "Type 1 for Meet" << endl;
	cout << "Type 2 for Miss" << endl;
	cin >> gr;
	while ((gr != 0) && (gr != 1) && (gr != 2))
	{
		cout << "Please make sure you enter 0 or 1 or 2!" << endl;
		cout << "Thank you!" << endl;
		cin >> gr;
	}

	cout << '\t' << '\t' << "Average AAR" << '\t' << "Standard Deviation" << endl;
	for (int i = 0; i < 2 * N; i++)
	{
		cout << fixed << "Day " << '\t' << i - N + 1 << '\t' << setprecision(7) << A_AAR_CAAR[gr][0][i] << '\t' << STD_AAR_CAAR[gr][0][i] << endl;
	}
	cout << endl << endl;

	cout << '\t' << '\t' << "Average CAAR" << '\t' << "Standard Deviation" << endl;
	for (int i = 0; i < 2 * N; i++)
	{
		cout << fixed << "Day " << '\t' << i - N + 1 << '\t' << setprecision(7) << A_AAR_CAAR[gr][1][i] << '\t' << STD_AAR_CAAR[gr][1][i] << endl;
	}	
}