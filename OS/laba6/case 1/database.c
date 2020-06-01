#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "database.h"
#include <string.h>
#include <stdbool.h>
database* initialize()
{
    database* list = (database*) malloc(sizeof(database));
    list->begin = NULL;
    list->end = NULL;
    list->size = 0;
    return list;
}
bool add_user(database* list, message* auth_msg)
{
    field* tmp = (field*)malloc(sizeof(field));
    if(tmp == NULL) return false;
    strcpy(tmp->user.name, auth_msg->name);
    tmp->user.id = auth_msg->sender_id;
    tmp->user.debit_account = auth_msg->debit_acc; // Money 
    tmp->user.credit_account = 0;
    if(list->size == 0)
    {
        list->begin = tmp;
        list->end = tmp;
        tmp->next = NULL;
        tmp->prev = NULL;
        ++list->size;
        return true;
    }
    else
    {
        tmp->next = list->begin;
        list->begin->prev = tmp;
        tmp->prev = NULL;
        list->begin = tmp;
        ++list->size;
        return true;
    }

}
field* find_user(database* list, long long int id)// try to find rec
{
    field* tmp = list->begin;
    while(tmp != NULL)
    {
        if(tmp->user.id == id) return tmp;
        tmp = tmp->next;
    }
    return NULL;
}
bool remove_user(database* list, message* shutdown_msg)
{
    return true;// TODO
}