#ifndef RetrieveData_hpp
#define RetrieveData_hpp
#include <map>
#include <stdio.h>
#include <string>
#include <vector>

#include "Stocks.hpp"

using namespace std;

int ReadData(string filename, map<string, Stock>& stockMap, vector<string>& stocklist);
//int RetrieveData(map<string, Stock> &stockMap, vector<string>::iterator begin, vector<string>::iterator end, int N, vector<string> &tickerWithoutData);
void multi_thread(map<string, Stock>& stockmap, vector<string>& stocklist, Stock& IWB, int N);
void stockMapExcessReturnCal(map<string, Stock>& stockMap, const Stock& Benchmark);

#endif /* RetrieveData_hpp */
