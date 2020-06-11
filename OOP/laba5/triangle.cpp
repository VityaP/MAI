#include <iostream>
#include <cmath>
#include "triangle.h"

Triangle::Triangle() : Triangle(0, 0, 0)
{
}

Triangle::Triangle(size_t i, size_t j, size_t k) : side_a(i), side_b(j), side_c(k)
{
	std::cout << "Triangle created: " << side_a << ", " << side_b << ", " << side_c << std::endl;
}

Triangle::Triangle(std::istream &is)
{
	is >> side_a;
	is >> side_b;
	is >> side_c;
}

Triangle::Triangle(const Triangle& orig)
{
	std::cout << "Triangle copy created" << std::endl;
	side_a = orig.side_a;
	side_b = orig.side_b;
	side_c = orig.side_c;
}

int32_t Triangle::Side()
{
    if (side_a >= side_b && side_a >= side_c) {
        return side_a;
    } else if (side_b >= side_a && side_b >= side_c) {
        return side_b;
    } else {
        return side_c;
    }
}

double Triangle::Square()
{
	double p = double(side_a + side_b + side_c) / 2.0;
	return sqrt(p * (p - double(side_a))*(p - double(side_b))*(p - double(side_c)));
}

std::ostream& operator<<(std::ostream& os, const Triangle& obj)
{
	os << "a=" << obj.side_a << ", b=" << obj.side_b << ", c=" << obj.side_c << std::endl;
	return os;
}

void Triangle::Print() 
{
	std::cout << "Side_A = " << side_a << ", Side_B = " << side_b << ", Side_C = " << side_c << std::endl;
}

Triangle::~Triangle()
{
	//std::cout << "Triangle deleted" << std::endl;
}

Triangle& Triangle::operator=(const Triangle& right)
{
    if (this == &right) return *this;
    std::cout << "Triangle copied" << std::endl;
    side_a = right.side_a;
    side_b = right.side_b;
    side_c = right.side_c;
    return *this;
}

Triangle& Triangle::operator++()
{
    side_a++;
    side_b++;
    side_c++;
    return *this;
}

Triangle operator+(const Triangle& left,const Triangle& right)
{
    return Triangle(left.side_a+right.side_a,left.side_b+right.side_b,left.side_c+right.side_c);
}

std::istream& operator>>(std::istream& is, Triangle& obj)
{
    is >> obj.side_a;
    is >> obj.side_b;
    is >> obj.side_c;
    return is;
}

bool operator==(const Triangle& left, const Triangle& right)
{
    return (left.side_a == right.side_a) &&
           (left.side_b == right.side_b) &&
           (left.side_c == right.side_c);
}

bool operator>(const Triangle& left, const Triangle& right)
{

    return (left.side_a > right.side_a) ||
           (left.side_a == right.side_a && left.side_b > right.side_b) ||
           (left.side_a == right.side_a && left.side_b == right.side_b && left.side_c > right.side_c);
}

bool operator<(const Triangle& left, const Triangle& right)
{

    return (left.side_a < right.side_a) ||
           (left.side_a == right.side_a && left.side_b < right.side_b) ||
           (left.side_a == right.side_a && left.side_b == right.side_b && left.side_c < right.side_c);
}