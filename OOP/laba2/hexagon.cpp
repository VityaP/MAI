#include <iostream>
#include <cmath>
#include "hexagon.h"

Hexagon::Hexagon() : Hexagon(0)
{
}

Hexagon::Hexagon(int32_t i) : side(i)
{
	std::cout << "Hexagon created: " << side << std::endl;
}

Hexagon::Hexagon(std::istream &is)
{
	is >> side;
}

Hexagon::Hexagon(const Hexagon& orig)
{
	//std::cout << "Hexagon copy created" << std::endl;
	side = orig.side;
}

int32_t Hexagon::Side()
{
    return side;
}

double Hexagon::Square()
{
	return (3.0 * sqrt(3.0) * double(side) * double(side)) / 2.0;
}

void Hexagon::Print() 
{
	std::cout << "Side = " << side << std::endl;
}

Hexagon::~Hexagon()
{
	std::cout << "Hexagon deleted" << std::endl;
}

Hexagon& Hexagon::operator=(const Hexagon& right)
{
    if (this == &right) {
        return *this;
    }

    //std::cout << "Hexagon copied" << std::endl;

    side = right.side;

    return *this;
}

Hexagon& Hexagon::operator++()
{
    side++;
    return *this;
}

Hexagon operator+(const Hexagon& left,const Hexagon& right) {
    return Hexagon(left.side + right.side);
}


std::ostream& operator<<(std::ostream& os, const Hexagon& obj) {
    os << "Side = " << obj.side << std::endl;
    return os;
}

std::istream& operator>>(std::istream& is, Hexagon& obj)
{
    is >> obj.side;
    return is;
}

bool operator==(const Hexagon& left, const Hexagon& right) {
    return left.side == right.side;
}