#include <bits/stdc++.h>
using namespace std;
 
#define _DEBUG

vector<int> parse( const string &originStr ){
    vector<int> origin;
    for(int j = 0; j < originStr.size(); ++j){
        if( originStr[j] == '0'){
            origin.push_back(0);
            origin.push_back(0);
            origin.push_back(0);
            origin.push_back(0);
        }
        if( originStr[j] == '1'){
            origin.push_back(0);
            origin.push_back(0);
            origin.push_back(0);
            origin.push_back(1);
        }
        if( originStr[j] == '2'){
            origin.push_back(0);
            origin.push_back(0);
            origin.push_back(1);
            origin.push_back(0);
        }
        if( originStr[j] == '3'){
            origin.push_back(0);
            origin.push_back(0);
            origin.push_back(1);
            origin.push_back(1);
        }
        if( originStr[j] == '4'){
            origin.push_back(0);
            origin.push_back(1);
            origin.push_back(0);
            origin.push_back(0);
        }
        if( originStr[j] == '5'){
            origin.push_back(0);
            origin.push_back(1);
            origin.push_back(0);
            origin.push_back(1);
        }
        if( originStr[j] == '6'){
            origin.push_back(0);
            origin.push_back(1);
            origin.push_back(1);
            origin.push_back(0);
        }
        if( originStr[j] == '7'){
            origin.push_back(0);
            origin.push_back(1);
            origin.push_back(1);
            origin.push_back(1);
        }
        if( originStr[j] == '8'){
            origin.push_back(1);
            origin.push_back(0);
            origin.push_back(0);
            origin.push_back(0);
        }
        if( originStr[j] == '9'){
            origin.push_back(1);
            origin.push_back(0);
            origin.push_back(0);
            origin.push_back(1);
        }
        if( originStr[j] == 'a'){
            origin.push_back(1);
            origin.push_back(0);
            origin.push_back(1);
            origin.push_back(0);
        }
        if( originStr[j] == 'b'){
            origin.push_back(1);
            origin.push_back(0);
            origin.push_back(1);
            origin.push_back(1);
        }
        if( originStr[j] == 'c'){
            origin.push_back(1);
            origin.push_back(1);
            origin.push_back(0);
            origin.push_back(0);
        }
        if( originStr[j] == 'd'){
            origin.push_back(1);
            origin.push_back(1);
            origin.push_back(0);
            origin.push_back(1);
        }
        if( originStr[j] == 'e'){
            origin.push_back(1);
            origin.push_back(1);
            origin.push_back(1);
            origin.push_back(0);
        }
        if( originStr[j] == 'f'){
            origin.push_back(1);
            origin.push_back(1);
            origin.push_back(1);
            origin.push_back(1);
        }
    }
    return origin;
}

int main() {

    #ifdef _DEBUG
        freopen("Result.txt", "r", stdin);
		freopen("DataToPlot.txt", "w", stdout);
	#endif
	
	std::ios_base::sync_with_stdio(false);
	// std::cin.tie(NULL);

    const int LengthOfString = 1;
    long long AmountOfStrings;
    long long StopRound;
    cin >> AmountOfStrings >> StopRound;
    long long cnt = 0;
    long long bound = AmountOfStrings / (LengthOfString + 1);
    for(int amount = 0; amount < bound; ++amount){
        string originStr;
        cin >> originStr;
        vector<int> origin = parse(originStr);
        
        for(int i = 0; i < LengthOfString; ++i){
            string tmpStr;
            cin >> tmpStr;
            vector<int> tmp = parse(tmpStr);
            for(int j = 0; j < origin.size(); ++j){
                if( origin[j] != tmp[j] ){
                    ++cnt;
                }
            }
        }
    }
    double answer = (double) cnt / (LengthOfString * bound);
    // int answer = cnt / ( AmountOfStrings - bound) ;
    cout << "Stop an Round " << StopRound << "\n";
    cout << answer << " bits\n";
    
	return 0;  
}