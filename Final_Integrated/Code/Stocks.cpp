//
//  Stocks.hpp
//  final_project
//
//  Created by TungDang on 5/6/21.
//

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <map>

#include "Stocks.hpp"

using namespace std;

// Default constructor
Stock::Stock() : ticker(""), AncDate(""), EstEps(0.0), ActEps(0.0) {}
// Constructor with params
Stock::Stock(string ticker_, string AncDate_, string EndPer_, double EstEps_, double ActEps_) : ticker(ticker_), AncDate(AncDate_), EndPer(EndPer_), EstEps(EstEps_), ActEps(ActEps_) {}
// Copy constructor
Stock::Stock(const Stock& stock_) : ticker(stock_.ticker), AncDate(stock_.AncDate), EndPer(stock_.EndPer), AdjClose(stock_.AdjClose), Return(stock_.Return), ExcessReturn(stock_.ExcessReturn), CumReturn(stock_.CumReturn), Dates(stock_.Dates), group(stock_.group), EstEps(stock_.EstEps), ActEps(stock_.ActEps) {}
// Destructor
Stock::~Stock() {}

// Access functions
string Stock::GetTicker() const { return ticker; }
string Stock::GetAncDate() const { return AncDate; }
string Stock::GetEndPer() const { return EndPer; }
vector<double> Stock::GetAdjClose() const { return AdjClose; }
vector<double> Stock::GetReturn() const { return Return; }
vector<double> Stock::GetExcessReturn() const { return ExcessReturn; }
vector<double> Stock::GetCumReturn() const { return CumReturn; }
vector<string> Stock::GetDates() const { return Dates; }
GR Stock::GetGroup() const { return group; }
double Stock::GetEstEps() const { return EstEps; }
double Stock::GetActEps() const { return ActEps; }
double Stock::GetSurprise() const {
    if (EstEps == 0) return NULL;
    return (ActEps - EstEps) / abs(EstEps);
}

// Modify functions
void Stock::SetAdjClose(const vector<double>& AdjClose_) {
    /* Retrieve Adjusted Close prices & Calculate return*/
    AdjClose = AdjClose_;
    Return.clear(); Return.push_back(NULL);

    // Calculate stock return
    for (int i = 1; i < AdjClose_.size(); ++i) {
        if (AdjClose_[i - 1] == 0) Return.push_back(NULL);
        else Return.push_back(AdjClose_[i] / AdjClose_[i - 1] - 1);
    }

    // Calculate stock cumulative return
    CumReturn.clear(); CumReturn.push_back(0);
    for (int i = 1; i < AdjClose_.size(); ++i) {
        CumReturn.push_back((1 + CumReturn[i - 1]) * (1 + Return[i]) - 1);
    }
}

void Stock::SetDates(const vector<string>& Dates_) {
    /* Retrieve Dates Data*/
    Dates = Dates_;
}

void Stock::SetGroup(const GR group_) {
    /* Retrieve group*/
    group = group_;
}

void Stock::CalcExcessReturn(const Stock& Benchmark) {
    ExcessReturn.clear();
    ExcessReturn.push_back(NULL);

    // Copy data from benchmark
    vector<string> B_Dates = Benchmark.GetDates();
    vector<double> B_Return = Benchmark.GetReturn();

    // Find the the start of stock in the benchmark dates array
    auto stockStartDatePtr = find(B_Dates.begin(), B_Dates.end(), Dates[0]);
    size_t stockStartDateIndex = stockStartDatePtr - B_Dates.begin();

    // Loop to calculate excess return daily
    if (stockStartDatePtr != B_Dates.end()) { // found the date index
        for (int i = 1; i < Dates.size() && stockStartDateIndex + i < B_Return.size(); ++i) {
            ExcessReturn.push_back(Return[i] - B_Return[stockStartDateIndex + i]);
        }
    }
    else { // NOT found the date index
        cout << ticker << " not have relative benchmark return.\n";
    }
}

// Overload outstream operator
ostream& operator << (ostream& out, const Stock& Stock_) {
    out << "Ticker:" << Stock_.ticker << endl;

    out << "Announcent Date: " << Stock_.AncDate << endl;
    out << "Period End: " << Stock_.EndPer << endl;

    out << "Estimated Earning: " << Stock_.EstEps << endl;
    out << "Actual Earning: " << Stock_.ActEps << endl;
    out << "Surprise: " << Stock_.ActEps - Stock_.EstEps << endl;
    out << "Surprise (in percentage):" << fixed << setprecision(2) << Stock_.GetSurprise() * 100 << "\%" << endl;
    vector<string> temp{ "beat","meet","miss" };
    out << "Group:" << temp[Stock_.GetGroup()] << endl;

    out << "Adjusted Daily Price and Cumulative Return:" << Stock_.GetAdjClose().size() << endl;
    for (int i = 0; i < Stock_.AdjClose.size(); ++i) {
        if (i == 0) {
            out << Stock_.Dates[i] << "\t" << fixed << setprecision(2) << Stock_.AdjClose[i] << "\t" << fixed << setprecision(2) << "--" << endl;
        }
        else {
            out << Stock_.Dates[i] << "\t" << fixed << setprecision(2) << Stock_.AdjClose[i] << "\t" << fixed << setprecision(4) << Stock_.CumReturn[i] << endl;
        }
    }
    return out;
}

