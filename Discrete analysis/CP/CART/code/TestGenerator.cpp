#include <fstream>
#include <iostream>
#include <cstdlib>
const int MOD = (int)1e4;

int main(){

	std::ofstream fout;
	std::ofstream fout2;
	std::ofstream fout3;
	fout.open("dataWithValue");
	fout2.open("data");
	fout3.open("value");
	srand(2);
	int n,m;
	double d;
	int ivalue;
	/////////////////////////////
	////////
			n = 10000000;
			m = 1;
	////////
	/////////////////////////////
	fout << n << " " << m << "\n";
	fout2 << n << " " << m << "\n";

	for(int i = 0; i < n; ++i){
		for(int k = 0; k < m; ++k){
			d = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/MOD));
			fout << d << " ";
			fout2 << d << " ";
		}
		ivalue = rand() % 2;
		fout << ivalue << "\n";
		fout2 << "\n";
		fout3 << ivalue << "\n";
	}

	return 0;
}