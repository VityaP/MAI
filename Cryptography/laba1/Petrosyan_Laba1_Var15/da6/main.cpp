#include "bigint.hpp"
using namespace std;

BigInt gcd(BigInt a,BigInt b){
    while ( !( a == 0 ) ){
        if( a < b ){
            BigInt tmp = a;
            a = b;
            b = tmp;
        }
        BigInt k = a / b;
        a = a - b*k;
        // cout << a << "\n";
    }
    return b; 
}

int StingToInt(const string &str) {
    int tmp = 0;
    for(int i = 0; i < str.size(); ++i) {
        tmp = tmp * 10 + (str[i] - '0');
    }
    return tmp;
}

int main() {

    freopen("input.txt", "r", stdin);
    string strNum1;
    cin >> strNum1;
    BigInt num1(strNum1);

    for(int i = 0; i < 19; ++i){
        string strNum2;
        cin >> strNum2;
        BigInt num2(strNum2);
        BigInt res = gcd(num1,num2);
        if( i >= 15){
            cout << "\n\n" << i + 1;
        }
        else{
            cout << "\n\n" << i;
        }
        cout << " RESULT " << res << "\n\n";
    }
    BigInt sixteen("139476625665968066568224614983908130916411561642255796653097148734670476110858310346843945046884204821224835911446227920807676218254025383087995289367050711834371298932142600299173740378514261264695250897646270832307124860459187526487820547916176260627474993626505280101101880100712184854128131522995503631439");
    BigInt res = num1 / sixteen;
    cout << "\n\nFirst\n" << sixteen << "\n";
    cout << "\n\nSecond\n" << res << "\n";
    cout << "\n\n";
    BigInt check = sixteen * res;
    cout << check << "\n";
    cout << num1 << "\n";
    if( check == num1 ){
        cout << "ok\n";
    }
    return 0;

}