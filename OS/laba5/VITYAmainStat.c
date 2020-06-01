#include <stdio.h>
#include <stdlib.h>
#include "array.h"

void menu(){

    printf("\nChoose an operation:\n");
    
    printf("1 : Create\n");
    
    printf("    2 : Erase\n");
    
    printf("3 : Insert\n");
    
    printf("    4 : Print array\n");
    
    printf("0 : Exit\n");
   
}

int main(void)

{

    int act = 0;

    ElemType key = 0;

    struct array p;

    double value;

    int size , number;

    printf("This is compile-time linking\n\n");

    menu();

    while (scanf("%d", &act) && act) {

        switch(act) {

        case 1:

            printf("Enter size of array ");

            scanf("%d", &size);

            create(size , &p);
            
            break;

        case 2:

            erase( &p );
            
            break;
        
        case 3:

            printf("Index ");
            
            scanf("%d", &number);

            printf("Value ");

            scanf("%lf", &value);
            
            insert(value , number , p);

            break;

        case 4:

            print(p , size);

            break;

        default:

            printf("Incorrect command\n");

            break;

        }

    menu();

    }

    return SUCCESS;
}