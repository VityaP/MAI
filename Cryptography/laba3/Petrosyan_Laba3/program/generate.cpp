#include <bits/stdc++.h>
using namespace std;
 
#define _DEBUG
 
int main() {
	
	std::ios_base::sync_with_stdio(false);
	// std::cin.tie(NULL);

    const int LengthOfString = 128;
    cout << "How much strings?\n";
    int n;
    cin >> n;
    cout << "Should it be determined? Press 0 or 1\n";
    bool determine = true;
    cin >> determine;
    if( determine == true ){
        srand(20);
    }
    else{
        srand(time(NULL));
    }

    #ifdef _DEBUG
		freopen("TestData.txt", "w", stdout);
	#endif

    for(int i = 0; i < n; ++i){
        vector<int> s;
        for(int j = 0; j < LengthOfString; ++j){
            s.push_back( (rand() % 2) );
        }
        for( int k = 0; k < s.size(); ++k){
            cout << s[k];
        }
        cout << "\n";
        int randPos = rand() % 128;
        // for(int pos = 0; pos < LengthOfString; ++pos){
        //     if( s[pos] == 0){
        //         s[pos] = 1;
        //     }
        //     else{
        //         s[pos] = 0;
        //     }
        //     for( int k = 0; k < s.size(); ++k){
        //         cout << s[k];
        //     }
        //     cout << "\n";
        //     if( s[pos] == 0){
        //         s[pos] = 1;
        //     }
        //     else{
        //         s[pos] = 0;
        //     }
        // }
        if( s[randPos] == 0){
            s[randPos] = 1;
        }
        else{
            s[randPos] = 0;
        }
        for( int k = 0; k < s.size(); ++k){
            cout << s[k];
        }
        cout << "\n";
    }

	return 0;  
}