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

    Octagon& operator=(const Octagon& right);
    Octagon& operator++();

    friend Octagon operator+(const Octagon& left,const Octagon& right);
    friend std::ostream& operator<<(std::ostream& os, const Octagon& obj);
    friend std::istream& operator>>(std::istream& is, Octagon& obj);
    friend bool operator==(const Octagon& left, const Octagon& right);
    friend bool operator>(const Octagon& left, const Octagon& right);
    friend bool operator<(const Octagon& left, const Octagon& right);

    int32_t Side() override;
	double Square() override;
	void Print() override;

	virtual ~Octagon();
    int type() {return 2;}
private:
	int32_t side;
};

#endif /* OCTAGON_H */