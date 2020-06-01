#include "array.h"


void create(int size,struct array* DATA){

    DATA->pointe = (double *)malloc(size*sizeof(double));

    for(int i = 0; i < size; i++){
        
    DATA->pointe[i] = 0.0;
    
    }
}

void insert(double value,int number,struct array DATA){

    DATA.pointe[ number ] = value;
}

void erase(struct array* DATA){

    free(DATA->pointe);

    DATA->pointe = NULL;

}

void print(struct array DATA,int size){

    if (DATA.pointe == NULL){

        printf("empty array");

    }
    else{
        for(int i = 0; i < size; i++){

            printf("%lf\n",DATA.pointe[ i ]);
        
        }
    }
}