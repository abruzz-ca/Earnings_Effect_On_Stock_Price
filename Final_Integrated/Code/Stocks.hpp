//
//  Stocks.hpp
//  final_project
//
//  Created by TungDang on 5/6/21.
//

#ifndef Stocks_hpp
#define Stocks_hpp

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

enum GR { beat, meet, miss };

class Stock {
private:
    string ticker;                  // Stock ticker
    string AncDate;                 // Announcement Date
    string EndPer;                  // Ending Period
    vector<double> AdjClose;        // Adjusted Close Prices
    vector<double> Return;          // Adjusted Returns (calculated by AdjClose)
    vector<double> ExcessReturn;    // Excess Return in comparison with benchmark (Return - Relative Benchmark return)
    vector<double> CumReturn;       // Cumulative return of stock (calculated by Return)
    vector<string> Dates;           // Dates matching Close Prices
    GR group;                       // Group information
    double EstEps;                  // Estimated Earning
    double ActEps;                  // Actual Earning
    

public:
    // Default constructor
    Stock();
    // Constructor with params
    Stock(string ticker_, string AncDate_, string EndPer_, double EstEps_, double ActEps_);
    // Copy constructor
    Stock(const Stock& stock_);
    // Destructor
    virtual ~Stock();

    // Access functions
    string GetTicker() const;
    string GetAncDate() const;
    string GetEndPer() const;
    vector<double> GetAdjClose() const;
    vector<double> GetReturn() const;
    vector<double> GetExcessReturn() const;
    vector<double> GetCumReturn() const;
    vector<string> GetDates() const;
    GR GetGroup() const;
    double GetEstEps() const;
    double GetActEps() const;
    double GetSurprise() const;


    // Modify functions
    void SetAdjClose(const vector<double>& AdjClose_);
    void SetDates(const vector<string>& Dates_);
    void SetGroup(const GR group_);
    void CalcExcessReturn(const Stock& Benchmark);

    // Overload outstream operator
    friend ostream& operator << (ostream& out, const Stock& Stock_);
};

#endif /* Stocks_hpp */
