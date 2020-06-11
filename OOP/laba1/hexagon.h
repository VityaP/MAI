#ifndef HEXAGON_H
#define HEXAGON_H
#include <cstdlib>
#include <iostream>
#include "figure.h"

class Hexagon : public Figure{
public:
	Hexagon();
	Hexagon(std::istream &is);
	Hexagon(int32_t i);
	Hexagon(const Hexagon& orig);
	double Square() override;
	void Print() override;
	virtual ~Hexagon();	
private:
	int32_t side;
};

#endif /* HEXAGON_H */