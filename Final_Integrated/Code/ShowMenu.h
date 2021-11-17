

#include <map>
#include "Stocks.hpp"
#include "RetrieveData.hpp"
#include "Bootstrap.hpp"
#include <math.h>
#include "GNUPlot.hpp"
#include <iostream>
using namespace std;


#ifndef Header_h
#define Header_h

void ShowMenu()
{
    cout << "Menu: " << endl;
    cout << "Please enter 1, 2, 3 or 4 to select your option:" << endl << endl;
    
    cout << "1.Pull information for one stock from a group." << endl;
    cout << "2.Show Average AAR, Average CAAR and its Standard Deviation for one group." << endl;
    cout << "3.Show the graph with Average CAAR for all 3 groups." << endl;
    cout << "4.Exit the program." << endl<<endl;
    
}

void MainMenu(int N, const map<string, Stock>& stockMap_)
{
    Bootstrap Boo(N);
    Boo.WriteGroups(stockMap_);
    Boo.Perform(stockMap_);

    ShowMenu();
    cout << "Please select your option:  "<<endl;
    int opt = 0;
    cin >> opt;
    
    while (opt != 4)
    {
        switch(opt)
        {
            case 1:
            {   // input: ticker
                // display funtion needs to display daily price, cum daily returns, group name, announcement date, period ending , estimated eps, actual eps, surprise and surprise %. This can be stored into a file
                cout << "Please enter the ticker: ";
                string input_ticker; cin >> input_ticker;
                if (stockMap_.find(input_ticker) == stockMap_.end()) {
                    cout << "The input ticker either does NOT exist or does not have enough data!\n";
                }
                else {
                    cout << endl << stockMap_.at(input_ticker);
                }
                break;
            }
        
            case 2:
            {   // input: group name
                // display AAR, AAR-SD, CAAR, and CAAR-SD
                
                Boo.Display();
                cout << "The information has been displayed." << endl << endl;
                break;
            }
                
            case 3:
            {    // display the graph
                int nIntervals = 2 * N;
                vector<double> xData(nIntervals);
                vector<double> yData1(nIntervals);
                vector<double> yData2(nIntervals);
                vector<double> yData3(nIntervals);

                xData[0] = -N + 1;
                for (int i = 0; i < nIntervals - 1; i++) {
                    xData[i + 1] = xData[i] + 1.0;
                }

                for (int i = 0; i < nIntervals; i++) {
                    yData1[i] = Boo.GetA_AAR_CAAR(0)[1][i] * 100;    // Need to pull CAAR Vector for beat
                    yData2[i] = Boo.GetA_AAR_CAAR(1)[1][i] * 100;   // for meet
                    yData3[i] = Boo.GetA_AAR_CAAR(2)[1][i] * 100;   // for miss
                   // cout << "Day " << i << ": " << yData1[i] << ", " << yData2[i] << ", " << yData3[i] << "\n";
                }

                cout << "Graph shown!" << endl;
                plotResults(xData, yData1, yData2, yData3, nIntervals);
                cout << endl;
                //cout << "The graph has been displayed." << endl << endl;
                break;
            }
            
            default:
                cout<<"Invalid option number, please try again."<<endl<<endl;
            
        }
        ShowMenu();
        cout << "Please select another option:  "<<endl;
        cin >> opt;
    }
    cout<<"You've exited the program. Thank you!"<<endl<<endl;
}

#endif /* Header_h */
