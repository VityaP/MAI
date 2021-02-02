#include <bits/stdc++.h>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <queue>
#include <stack>
//#pragma GCC optimize("Ofast")
//#pragma GCC target("avx,avx2,fma")
//#pragma GCC optimization ("unroll-loops")

using namespace std;

#define endl "\n"
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()

int main() {

	//freopen("treemaster.in", "r", stdin);
  	//freopen("treemaster.out", "w", stdout);

	ios::sync_with_stdio(false);
	cin.tie(0);
	int n;
	cin >> n;
	vector<int> a(n, 1);
	vector<int> b(n, -1);
	vector<int> c(n, 0);
	
	clock_t tt = clock();
	
	for(int i = 0; i < n; ++i){
	    c[i] = a[i] - b[i];
    }
    
    tt = clock() - tt;
    cout << ( (double) tt) / CLOCKS_PER_SEC << endl;
	
	return 0;
}

