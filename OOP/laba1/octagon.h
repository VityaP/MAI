#ifndef OCTAGON_H
#define OCTAGON_H
#include <cstdlib>
#include <iostream>
#include "figure.h"

class Octagon : public Figure{
public:
	Octagon();
	Octagon(std::istream &is);
	Octagon(int32_t i);
	Octagon(const Octagon& orig);
	double Square() override;
	void Print() override;
	virtual ~Octagon();	
private:
	int32_t side;
};

#endif /* OCTAGON_H */