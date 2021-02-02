#include <bits/stdc++.h>
#include <map>
#include <cmath>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <stack>
#include <queue>
#include <cstdio>
#include <chrono>
#include <complex>
//#pragma GCC optimize("Ofast")
//#pragma GCC target("avx,avx2,fma")
//#pragma GCC optimization ("unroll-loops")
using namespace std;
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()




int main() {
    freopen("primes.txt", "r", stdin);
    freopen("matr.txt", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int n = 10;
    int size;
    cin >> size;
    if( n > size ){
        cout << "You should Generate more Primes\n Your size is >= than amount of Primes you have in file primes.txt\n";
        abort();
    }
    cout << n << "\n";
    double *matrix = (double*) malloc( n * n * sizeof(double) );
    int value1, value2;
    for(int i = n - 1; i > 0; --i){
        cin >> value1 >> value2;
        int idx = i;
        for(int j = 0; idx < n; ++j){
            matrix[idx * n + j] = value1;
            matrix[j * n + idx] = value2;
            ++idx;
        }
    }

    cin >> value1;
    for(int i = 0; i < n; ++i){
        matrix[i * n + i] = value1;
    }

    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            cout << matrix[i * n + j] << " ";
        }
        cout << "\n";
    }


    return 0;
}