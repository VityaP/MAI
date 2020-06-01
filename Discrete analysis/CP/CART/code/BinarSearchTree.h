#include <iostream>
#include <cstdlib>
#include <vector>

class TTree{
    
public:

    TTree *left ;
    TTree *right ;
    int coordinate;		//[0..m]
    double position;	// R
    int ifBiggerValue;  //[0..AMORTIZATION]
    int ifSmallerValue; //[0..AMORTIZATION]
    friend std::ostream &operator<<(std::ostream &stream, TTree &ob){
		stream << ob.coordinate << " " << ob.position << " " << ob.ifSmallerValue << " " << ob.ifBiggerValue ;
        return stream;
    }
    friend std::istream &operator>>(std::istream &stream, TTree &ob){
		stream >> ob.coordinate >> ob.position >> ob.ifSmallerValue >> ob.ifBiggerValue ;
        return stream;
    }
    ~TTree(){
        
    }
    
};