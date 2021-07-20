#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <ctime>
#include <map>
using namespace std;

vector<pair<int, int>> generateData(int a, int b, int size, double garbagePercent = 0.01){
    // generates array of pairs<x, y> : y = a * x + b
    vector<pair<int, int>> data(size + 1);
    srand(time(NULL));

    int maxY = 0;

    for(int i = 0; i < data.size(); ++i){
        int x = rand() % 1000;
        int y = a * x + b;
        maxY = max(maxY, y);
        data[i].first = x + (rand() % 10);
        data[i].second = y + (rand() % 10);
    }

    // generate garbage and shuffles it into the data
    for(int i = 0; i < size * garbagePercent; ++i){
        data[i].first = rand() % 1000;
        data[i].second = rand() % maxY;
        swap(data[i], data[rand() % size]);
    }

    return data;
}

int main(){

    int a, b, n; // n pair of point <x, y> where y = a * x + b
    cin >> a >> b >> n;
    int mat, disp, garbageProcent; // y' = y + N(mat, disp)
    cin >> mat >> disp >> garbageProcent;

    // Task:
    // 1) Generate data set    
    // 2) find a, b            MSE
    // 3) find garbage points  RANSAC algorithm c++ PSL library

    vector<pair<int, int>> points = generateData(a, b, n);

    for(int i = 0; i < n; ++i){
        double value = (double) points[i].second / points[i].first;
        cout << value << "\n";
    }

    return 0;
}