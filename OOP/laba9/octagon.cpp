#include <iostream>
#include <cmath>
#include "octagon.h"

Octagon::Octagon() : Octagon(0)
{
}

Octagon::Octagon(int32_t i) : side(i)
{
	std::cout << "Octagon created: " << side << std::endl;
}

Octagon::Octagon(std::istream &is)
{
	is >> side;
}

Octagon::Octagon(const Octagon& orig)
{
	std::cout << "Octagon copy created" << std::endl;
	side = orig.side;
}

double Octagon::Square()
{
	return (2.0 * double(side) * double(side)) * (1.0 + sqrt(2.0));
}

void Octagon::Print() 
{
	std::cout << "Side = " << side << std::endl;
}

Octagon::~Octagon()
{
	//std::cout << "Octagon deleted" << std::endl;
}

int32_t Octagon::Side()
{
    return side;
}

Octagon& Octagon::operator=(const Octagon& right)
{
    if (this == &right) {
        return *this;
    }

    side = right.side;

    return *this;
}

Octagon& Octagon::operator++()
{
    side++;
    return *this;
}

Octagon operator+(const Octagon& left,const Octagon& right)
{
    return Octagon(left.side + right.side);
}


std::ostream& operator<<(std::ostream& os, const Octagon& obj)
{
    os << "Side = " << obj.side << std::endl;
    return os;
}

std::istream& operator>>(std::istream& is, Octagon& obj)
{
    is >> obj.side;
    return is;
}

bool operator==(const Octagon& left, const Octagon& right)
{
    return left.side == right.side;
}

bool operator>(const Octagon& left, const Octagon& right)
{
    return left.side > right.side;
}

bool operator<(const Octagon& left, const Octagon& right)
{
    return left.side < right.side;
}