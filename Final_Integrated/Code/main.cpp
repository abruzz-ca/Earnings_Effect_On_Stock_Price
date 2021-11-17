//
//  main.cpp
//
#include <map>
#include "Stocks.hpp"
#include "RetrieveData.hpp"
#include "Bootstrap.hpp"
#include <math.h>
#include "GNUPlot.hpp"
#include "ShowMenu.h"

using namespace std;

int main() {

    cout << "Please input N!" << endl;
    cout << "N needs to be between 30 and 60." << endl;
    int N = 0;
    cin >> N;

    while ((N < 30) || (N > 60))
    {
        cout << "Please make sure your input is between 30 and 60!" << endl;
        cin >> N;
    }     
    
	time_t start = time(NULL);

    cout << "Retrieving " << 2 * N + 1 << " days of historical price data for all stocks." << endl;

	map<string, Stock> stockMap;
    vector<string> stockList;
    vector<string> tickerWithoutData;
    Stock IWB("IWB", "2020-10-30", "", 0.0, 0.0);

    ReadData("Surprise.csv", stockMap, stockList);
       
	multi_thread(stockMap, stockList, IWB, N);
   
    stockMapExcessReturnCal(stockMap, IWB);

    time_t end = time(NULL);
    cout << "Time elapsed: " << (end - start) << " secs" << endl << endl;


    //Print Menu

    MainMenu(N, stockMap);
        

    return 0;
}
