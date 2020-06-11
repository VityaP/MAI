#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <cstdlib>
#include <iostream>
#include "figure.h"

class Triangle : public Figure{
public:
	Triangle();
	Triangle(std::istream &is);
	Triangle(size_t i,size_t j,size_t k);
	Triangle(const Triangle& orig);

	Triangle& operator++();
	Triangle& operator=(const Triangle& right);

	friend Triangle operator+(const Triangle& left,const Triangle& right);
	friend std::ostream& operator<<(std::ostream& os, const Triangle& obj);
	friend std::istream& operator>>(std::istream& is, Triangle& obj);
    friend bool operator>(const Triangle& left, const Triangle& right);
    friend bool operator<(const Triangle& left, const Triangle& right);
	friend bool operator==(const Triangle& left, const Triangle& right);
    
	int32_t Side() override;
	double Square() override;
	void Print() override;
	virtual ~Triangle();	
private:
	size_t side_a;
	size_t side_b;
	size_t side_c;
};

#endif /* TRIANGLE_H */