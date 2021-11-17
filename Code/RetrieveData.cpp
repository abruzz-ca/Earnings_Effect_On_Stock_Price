#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <locale>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <thread>

#include "curl/curl.h"
#include "RetrieveData.hpp"
#include "Stocks.hpp"

using namespace std;

int write_data(void* ptr, int size, int nmemb, FILE* stream) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return (int)written;
}

struct MemoryStruct {
    char* memory;
    size_t size;
};

void* myrealloc(void* ptr, size_t size) {
    if (ptr)
        return realloc(ptr, size);
    else
        return malloc(size);
}

int write_data2(void* ptr, size_t size, size_t nmemb, void* data) {
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)data;
    mem->memory = (char*)myrealloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory) {
        memcpy(&(mem->memory[mem->size]), ptr, realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;
    }
    return (int)realsize;
}

string getTimeinSeconds(string Time) {
    std::tm t = { 0 };
    std::istringstream ssTime(Time);
    char time[100];
    memset(time, 0, 100);
    if (ssTime >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S")) {
        //        cout << std::put_time(&t, "%c %Z") << "\n"
        //        << std::mktime(&t) << "\n";
        sprintf(time, "%ld", mktime(&t));
        return string(time);
    }
    else {
        cout << "Parse failed\n";
        return "";
    }
}
/*----------------------------RETRIEVE DATA PROTOTYPE---------------------------------*/
bool cmp(const pair<string, double>& p1, const pair<string, double>& p2) {
    return p1.second < p2.second;
}

// transfer the map of ticker and related data to vector data type, so we can using the built-in function of vector (sort) to compare value
map<int, string> getSortedEPS(map<string, Stock>& stockMap) {

    // Make vector of pair<Ticker, Surprise> to sort into groups
    vector<pair<string, double> > sortPct;
    for (map<string, Stock>::iterator it = stockMap.begin(); it != stockMap.end(); ++it) {
        sortPct.push_back(make_pair(it->first, it->second.GetSurprise()));
    }
    sort(sortPct.begin(), sortPct.end(), cmp);

    // split all the stocks into 3 groups with equivalent numbers of stock
    map<int, string> tickRank; // Map of stock's rank and ticker
    int count = 0;

    unsigned long group_size = stockMap.size() / 3;
    for (vector<pair<string, double> >::iterator iter = sortPct.begin(); iter != sortPct.end(); iter++) {
        tickRank[count] = iter->first;
        if (count <= group_size) {
            stockMap[iter->first].SetGroup(miss);
        }
        else if (count <= 2 * group_size) {
            stockMap[iter->first].SetGroup(meet);
        }
        else {
            stockMap[iter->first].SetGroup(beat);
        }
        count++;
    }
    return tickRank;
}


int ReadData(string filename, map<string, Stock>& stockMap, vector<string>& stocklist) {
    /* Read in the components file and create stockMap*/
    vector<string> row;
    string line, word;

    // Open file
    ifstream file(filename);

    // Check if file is open properly
    if (!file.is_open()) {
        std::cout << "ERROR: File Open Failed" << '\n';
        return -1;
    }

    //Read file
    while (getline(file, line)) {   //read a line
        std::istringstream s(line);
        row.clear();
        while (getline(s, word, ',')) row.push_back(word);  //read word by word

        //parse datetime
        tm Anctm = { 0 };
        stringstream ss(row[1]);
        ss >> get_time(&Anctm, "%Y-%m-%d");
        char date[20];
        sprintf(date, "%d-%.2d-%.2d", 1900 + Anctm.tm_year, 1 + Anctm.tm_mon, Anctm.tm_mday);
        Stock stockTemp(row[0], date, row[2], stod(row[3]), stod(row[4]));
        stockMap[row[0]] = stockTemp;
        stocklist.push_back(row[0]);
    }
    // Sorted into 3 groups of beat, meet, miss
    getSortedEPS(stockMap);
    file.close();
    cout << "Stock Map Created! StockList Created!\n";
    return 0;
}

int RetrieveData(map<string, Stock>& stockMap, vector<string>::iterator begin, vector<string>::iterator end, int N, vector<string>& tickerWithoutData) {

    // name of files
    const char outfilename[FILENAME_MAX] = "Output.txt";
 

    // Initialized symbol pointer
    vector<string>::iterator itr = begin;

    // file pointers to create file that store the data
    FILE* fp2;
   

    // declaration of an object CURL
    CURL* handle;

    CURLcode result;

    // curl_easy_init() returns a CURL easy handle
    handle = curl_easy_init();

    // if everything's all right with the easy handle...
    if (handle)
    {
        string sCookies, sCrumb;
        while (true)
        {
            string startTime = getTimeinSeconds("2020-01-01T16:00:00");
            string endTime = getTimeinSeconds("2021-12-31T16:00:00");

            struct MemoryStruct data;
            data.memory = NULL;
            data.size = 0;

            if (sCookies.length() == 0 || sCrumb.length() == 0)
            {
                fp2 = fopen(outfilename, "w");
                curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history");
                curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
                curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
                curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
                curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
                curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");

                curl_easy_setopt(handle, CURLOPT_ENCODING, "");
                curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
                curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp2);
                result = curl_easy_perform(handle);
                fclose(fp2);

                if (result != CURLE_OK)
                {
                    // if errors have occurred, tell us what is wrong with result
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                    return 1;
                }

                curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
                curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);

                // perform, then store the expected code in result
                result = curl_easy_perform(handle);

                if (result != CURLE_OK)
                {
                    // if errors have occured, tell us what is wrong with result
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                    return 1;
                }

                char cKey[] = "CrumbStore\":{\"crumb\":\"";
                char* ptr1 = strstr(data.memory, cKey);
                char* ptr2 = ptr1 + strlen(cKey);
                char* ptr3 = strstr(ptr2, "\"}");
                if (ptr3 != NULL)
                    *ptr3 = NULL;

                sCrumb = ptr2;

                fp2 = fopen("cookies.txt", "r");
                char cCookies[100];
                if (fp2) {
                    while (fscanf(fp2, "%s", cCookies) != EOF);
                    fclose(fp2);
                }
                else
                    cerr << "cookies.txt does not exists" << endl;

                sCookies = cCookies;
                free(data.memory);
                data.memory = NULL;
                data.size = 0;
            }

            if (itr == end)
                break;

            string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
            string symbol = *itr;
            if (symbol.back() == '\r')
                symbol.pop_back();
            string urlB = "?period1=";
            string urlC = "&period2=";
            string urlD = "&interval=1d&events=history&crumb=";
            string url = urlA + symbol + urlB + startTime + urlC + endTime + urlD + sCrumb;
            const char* cURL = url.c_str();
            const char* cookies = sCookies.c_str();
            curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);
            curl_easy_setopt(handle, CURLOPT_URL, cURL);

            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);
            result = curl_easy_perform(handle);

            if (result != CURLE_OK)
            {
                // if errors have occurred, tell us what is wrong with result
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                return 1;
            }

            vector<double> AdjClose;
            vector<string> Dates;

            stringstream sData;
            sData.str(data.memory);
            string sValue, sDate;
            double dValue = 0;
            string line;
            getline(sData, line);
            while (getline(sData, line)) {
                size_t line_check = line.find('-');
                if (line_check != string::npos) {
                    sDate = line.substr(0, line.find_first_of(','));
                    line.erase(line.find_last_of(','));
                    sValue = line.substr(line.find_last_of(',') + 1);
                    dValue = strtod(sValue.c_str(), NULL);
                    // cout << sDate << " " << std::fixed << ::setprecision(6) << dValue << endl;
                    Dates.push_back(sDate);
                    AdjClose.push_back(dValue);
                }
            }

            vector<string>::iterator itr_AncDate = find(Dates.begin(), Dates.end(), stockMap[symbol].GetAncDate());
            long nindex = distance(Dates.begin(), itr_AncDate);
            long start_index = nindex - N;
            long end_index = nindex + N;
            if (start_index < 0 || end_index > Dates.size()) {
                //Missing data
                tickerWithoutData.push_back(symbol);
                std::ofstream out("wrong.txt", ios::app);
                out << symbol << ": ";
                out << endl;
                out.close();
                //cout << "No data" << endl;
            }
            else {
                //Find data matches 2N+1 days with announcement day as day0

                //for adjusted close
                auto bgn_close = AdjClose.begin() + start_index;
                auto end_close = AdjClose.begin() + end_index + 1;
                if (end_close > AdjClose.end()) end_close = AdjClose.end();
                vector<double> vec(bgn_close, end_close);
                stockMap[symbol].SetAdjClose(vec);

                //for dates
                auto bgn_date = Dates.begin() + start_index;
                auto end_date = Dates.begin() + end_index + 1;
                if (end_date > Dates.end()) end_date = Dates.end();
                vector<string> vec_date(bgn_date, end_date);
                stockMap[symbol].SetDates(vec_date);
            }

            free(data.memory);
            data.size = 0;
            itr++;
        }
    }
    else
    {
        fprintf(stderr, "Curl init failed!\n");
        return 1;
    }

    // cleanup since you've used curl_easy_init
    curl_easy_cleanup(handle);
    return 0;
}

#define NUM_THREAD 64
void multi_thread(map<string, Stock>& stockmap, vector<string>& stocklist, Stock& IWB, int N) {
    // Create temporary data structure for IWB to invoke RetrieveData Function
    map<string, Stock> tempmap;
    tempmap["IWB"] = IWB;
    vector<string> templist{ "IWB" };

    cout << "Retrieving historical price data..." << endl;

    //Multithread
    thread Retrieve[NUM_THREAD];
    int size = int(stockmap.size() / NUM_THREAD);
    vector<string>::iterator st = stocklist.begin();
    vector<string>::iterator ed = stocklist.begin();
    vector<string> NoDataTicker(0);//Mark missing data to be deleted
    //Creating threads
    curl_global_init(CURL_GLOBAL_ALL);
    for (int i = 0; i < NUM_THREAD; i++) {
        if (i == NUM_THREAD - 1) ed = stocklist.end();
        else advance(ed, size);
        Retrieve[i] = thread(RetrieveData, ref(stockmap), st, ed, N, ref(NoDataTicker));
        st = ed;
    }
    //Wait for all threads to finish
    for (int i = 0; i < NUM_THREAD; i++) Retrieve[i].join();
    //Retrieve IWB data
    RetrieveData(tempmap, templist.begin(), templist.end(), N + 60, NoDataTicker);
    IWB = tempmap["IWB"];
    // release resources acquired by curl_global_init()
    curl_global_cleanup();
    //Remove missing data from stockmap
    for (auto ptr = NoDataTicker.begin(); ptr != NoDataTicker.end(); ptr++) stockmap.erase(*ptr);
    cout << "Retrieved historical price data." << endl;
}

void stockMapExcessReturnCal(map<string, Stock>& stockMap, const Stock& Benchmark) {
    for (auto it = stockMap.begin(); it != stockMap.end(); ++it) {
        it->second.CalcExcessReturn(Benchmark);
    }
}
