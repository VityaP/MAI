
#define SUCCESS 0
#define FAILURE 1
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

typedef int32_t ElemType;

struct array {
    double *pointe;
};

extern void create(int size,struct array* DATA);

extern void insert(double value,int number,struct array DATA);

extern void erase(struct array* DATA);

extern void print(struct array DATA,int size);
