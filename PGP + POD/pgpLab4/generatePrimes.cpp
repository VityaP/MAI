#include <bits/stdc++.h>
#include <iostream>
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

// Решето Эратосфена — это алгоритм, позволяющий найти все простые числа в отрезке [1; n] 
// за O (n \log \log n) операций.

// Идея проста — запишем ряд чисел 1 ... n, и будем вычеркивать сначала все числа, делящиеся на 2, 
// кроме самого числа 2, затем деляющиеся на 3, кроме самого числа 3, 
// затем на 5, затем на 7, 11, и все остальные простые до n.


int main() {

    // freopen("primes.txt", "r", stdin);
    freopen("primes.txt", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int n;
    cin >> n;

    vector<bool> prime (n+1, true);
    prime[0] = prime[1] = false;
    for (long long i = 2; i <= n; ++i){
        if (prime[i] == true){
            if (i * 1ll * i <= n){
                for (long long j = i * 1ll * i; j <= n; j += i){
                    prime[j] = false;
                }
            }
        }
    }

    long long amount = 0;
    for(int i = 0; i <= n; ++i){
        if( prime[i] == true ){
            ++amount;
        }
    }

    cout << amount << "\n";
    
    for(int i = 0; i <= n; ++i){
        if( prime[i] == true ){
            cout << i << " ";
        }
    }

    cout << "\n";

    return 0;
}