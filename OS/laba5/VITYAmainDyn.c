#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <inttypes.h>
#include <stdbool.h>
#define SUCCESS 0
#define FAILURE 1

typedef int32_t ElemType;

struct array {
    double *pointe;
};

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
    void (*create)(int size,struct array* DATA);

    void (*insert)(double value,int number,struct array DATA);

    void (*erase)(struct array* DATA);

    void (*print)(struct array DATA,int size);

    char *err;

    void *libHandle;

    libHandle = dlopen("libarray.so", RTLD_LAZY);

    if (!libHandle) {

        fprintf(stderr, "%s\n", dlerror());

        exit(1);

    }

    create = dlsym(libHandle, "create");

    insert = dlsym(libHandle, "insert");

    erase = dlsym(libHandle, "erase");

    print = dlsym(libHandle, "print");

    err = dlerror();

    if ( err ) {

        fprintf(stderr, "%s\n", err);

        exit(FAILURE);

    }

    int act = 0;

    ElemType key = 0;

    struct array p;

    double value;

    int size , number;

    printf("This is runtime linking\n\n");

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
    
    dlclose(libHandle);
    
    return SUCCESS;

}