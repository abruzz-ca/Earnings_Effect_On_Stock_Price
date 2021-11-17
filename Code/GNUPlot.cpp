#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <iostream>

using namespace std;

void plotResults(vector<double> xData, vector<double> yData, vector<double> yData2, vector<double> yData3, int dataSize) {
    
    FILE* gnuplotPipe, * tempDataFile;
    const char* tempDataFileName1 = "Beat";
    const char* tempDataFileName2 = "Meet";
    const char* tempDataFileName3 = "Miss";
    
    double x, y, x2, y2, x3, y3;
    int i;
    
    // gnuplotPipe = _popen("D:\\gnuplot\\bin\\gnuplot", "w");
    //gnuplotPipe = _popen("C:\\Program" "Files\\gnuplot\\bin\\gnuplot.exe", "w");
    gnuplotPipe = _popen("C:\\PROGRA~1\\gnuplot\\bin\\gnuplot.exe", "w");
    // gnuplotPipe = popen("/usr/local/bin/gnuplot","w");
    
    if (gnuplotPipe) {
        
        //fprintf(gnuplotPipe, "plot \"%s\" using 1:2 title 'Beat' with lines linetype 4 smooth csplines, \"%s\" using 1:2 title 'Meet' with lines linetype 6 smooth csplines, \"%s\" using 1:2 title 'Miss' with lines linetype 8 smooth csplines\n", tempDataFileName1, tempDataFileName2, tempDataFileName3);
        fprintf(gnuplotPipe, "set title \"Average CAAR\"\n set xlabel \"Days\"\n set ylabel \"Percentage\"\n plot \"%s\" using 1:2 title 'Beat' with lines linetype 4 smooth csplines, \"%s\" using 1:2 title 'Meet' with lines linetype 6 smooth csplines, \"%s\" using 1:2 title 'Miss' with lines linetype 8 smooth csplines\n", tempDataFileName1, tempDataFileName2, tempDataFileName3);
        fflush(gnuplotPipe);
        
        // Write 'beat' file
        tempDataFile = fopen(tempDataFileName1, "w");
        for (i = 0; i < dataSize; i++) {
            x = xData[i];
            y = yData[i];
            fprintf(tempDataFile, "%lf %lf\n", x, y);
        }
        fclose(tempDataFile);
        
    const char* tempDataFileName3 = "Miss";
        // Write 'meet' file
        tempDataFile = fopen(tempDataFileName2, "w");
        for (i = 0; i < dataSize; i++) {
            x2 = xData[i];
            y2 = yData2[i];
            fprintf(tempDataFile, "%lf %lf\n", x2, y2);
        }
        fclose(tempDataFile);
        
        // Write 'miss' file
        tempDataFile = fopen(tempDataFileName3, "w");
        for (i = 0; i < dataSize; i++) {
            x3 = xData[i];
            y3 = yData3[i];
            fprintf(tempDataFile, "%lf %lf\n", x3, y3);
        }
        fclose(tempDataFile);
        
       // printf("press enter to continue...");
        //getchar(); 
        //getchar();

       /* remove(tempDataFileName1);
        remove(tempDataFileName2);
        remove(tempDataFileName3);*/
       // fprintf(gnuplotPipe, "exit \n");
    }
    else {
        printf("gnuplot not found...");
    }
}
