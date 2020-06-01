#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <unistd.h>
#include "config.h"
#include "database.h"
int main()
{
    message* msg_a = (message*) malloc(sizeof(message));
    message* msg_b = (message*) malloc(sizeof(message));
    message* msg_c = (message*) malloc(sizeof(message));

    printf("a--------------------------------------------------\n");

     printf("Enter your name\n");
    scanf("%s", msg_a->name);
    printf("Enter your id\n");
    scanf("%li", &msg_a->sender_id);
    printf("Enter your amount of money in the bank\n");
    scanf("%lli", &msg_a->value);

     printf("b-------------------------------------------------\n");

     printf("Enter your name\n");
    scanf("%s", msg_b->name);
    printf("Enter your id\n");
    scanf("%li", &msg_b->sender_id);
    printf("Enter your amount of money in the bank\n");
    scanf("%lli", &msg_b->value);

    printf("c-------------------------------------------------\n");

     printf("Enter your name\n");
    scanf("%s", msg_c->name);
    printf("Enter your id\n");
    scanf("%li", &msg_c->sender_id);
    printf("Enter your amount of money in the bank\n");
    scanf("%lli", &msg_c->value);
    printf("----------------------------------------------------\n");

    database* list = initialize();
    add_user(list, msg_a);
    add_user(list, msg_b);
    add_user(list, msg_c);
    return 0;
}