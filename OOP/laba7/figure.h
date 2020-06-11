#ifndef FIGURE_H
#define FIGURE_H

#include <cstdint>

class Figure
{
public:
    virtual double Square() = 0;
    virtual int32_t Side() = 0;
    virtual void Print() = 0;
    virtual ~Figure() {};
    virtual int type() = 0;
};

#endif /* FIGURE_H */