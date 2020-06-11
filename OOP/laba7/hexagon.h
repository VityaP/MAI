#ifndef HEXAGON_H
#define HEXAGON_H
#include <cstdlib>
#include <iostream>
#include "figure.h"

class Hexagon : public Figure{
public:
	Hexagon();
    Hexagon(int32_t i);
    Hexagon(const Hexagon& orig);
    Hexagon& operator++();

    friend bool operator==(const Hexagon& left, const Hexagon& right);
    friend Hexagon operator+(const Hexagon& left,const Hexagon& right);
    friend std::ostream& operator<<(std::ostream& os, const Hexagon& obj);
    friend std::istream& operator>>(std::istream& is, Hexagon& obj);
    friend bool operator>(const Hexagon& left, const Hexagon& right);
    friend bool operator<(const Hexagon& left, const Hexagon& right);

    Hexagon(std::istream &is);
    double Square() override;
    int32_t Side() override;
    void Print() override;
    Hexagon& operator=(const Hexagon& right);
	virtual ~Hexagon();
    int type() {return 3;}
private:
	int32_t side;
};

#endif /* HEXAGON_H */