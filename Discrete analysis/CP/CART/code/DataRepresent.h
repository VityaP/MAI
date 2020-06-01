#ifndef _DataRepresent_
#define _DataRepresent_

#include <vector>
#include <iostream>

static int n;
static int m;
static int hightBound = 100;
class DataRepresent{

	public:

    std::vector<double> space;
	bool value;
	DataRepresent(): value(true) , space(m){};

    DataRepresent &operator=(DataRepresent &ob){
        this->space = ob.space;
        this->value = ob.value;
        return * this;
    }
    friend std::ostream &operator<<(std::ostream &stream, DataRepresent &ob){
        for(int k = 0; k < m; ++k){
			stream << ob.space[k] << "\t";
		}
		stream << "\t\t" << ob.value << "\n";
        return stream;
    }
    DataRepresent& operator=(const DataRepresent& ob){
        for(int i = 0; i < m; ++i){
            this->space[i] = ob.space[i];
        }
        this->value = ob.value;
        return *this;
    }

};

#endif