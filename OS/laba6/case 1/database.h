#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "config.h"
typedef struct
{
    char name[14];
    long int id;
    long int credit_account;
    long int debit_account;
}client;
typedef struct field field;
struct field
{
    client user;
    field* next;
    field* prev;
};
typedef struct 
{
    field* begin;
    field* end;
    size_t size;
}database;
database* initialize();
bool add_user(database* list, message* auth_msg);
bool remove_user(database* list, message* shutdown_msg);
field* find_user(database* list, long long int id);