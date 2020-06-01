#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "config.h"
#include "database.h"
bool connect(int* msgid)
{
    printf("Choose bank, which you want to connect...\n");
    printf("1 - SBERBANK\n");
    printf("2 - VTB\n");
    printf("3 - RAIFAZEN\n");
    int a;
    scanf("%d", &a);
    if(a == 1)
    {
        *msgid = msgget((key_t)SBERBANK, IPC_CREAT | 0666);
        //*msgid_cloud = msgget((key_t)SBERBANK_CLOUD, IPC_CREAT | 0666);
    }
    else if(a == 2)
    {

        *msgid = msgget((key_t)VTB, IPC_CREAT | 0666);
        //*msgid_cloud = msgget((key_t)VTB_CLOUD, IPC_CREAT | 0666);
    }
    else
    {
        *msgid = msgget((key_t)RAIFAZEN, IPC_CREAT | 0666);
        //*msgid_cloud = msgget((key_t)RAIFAZEN_CLOUD, IPC_CREAT | 0666);
    }
    if(*msgid != -1) 
    {
        printf("successfull connection\n");
        return true;
    }
    else
    {
        printf("Erorr while connection\n");
        return false;
    }
    
}
void proccessing(message* msg, database* list)
{
    if(msg->type == AUTH_MSG)
    {
        if(!add_user(list, msg))
        {
            msg->debit_acc  = AUTH_ERROR;
            return;
        }
    }
    else if(msg->type == 1)// Transfer $
    {
        field* sender = find_user(list, msg->sender_id);
        field* recipient = find_user(list, msg->recipient_id);
        if(recipient == NULL)// no such user
        {
            msg->debit_acc  = UNREGISTERED_USER;
            return;
        }
        long int val = msg->debit_acc;// save the value
        if(msg->debit_acc <= sender->user.debit_account)
        {
            sender->user.debit_account -= val;
            recipient->user.debit_account += val;
            return;
        }
        if(val > sender->user.debit_account)
        {
            if(val + abs(sender->user.credit_account) > sender->user.debit_account + MAX_VAL)// too big for both acc
            {
                msg->debit_acc = NOT_ENOUGH_MONEY;
                return;
            }
            else// we use credit acc
            {
                msg->debit_acc -= sender->user.debit_account;
                sender->user.debit_account = 0;
                sender->user.credit_account -= msg->debit_acc;
                recipient->user.debit_account += val;
                return;
            }
        }
    }
    else if(msg->type == 2)// Get bank account
    {
        field* tmp = find_user(list, msg->sender_id);
        msg->debit_acc = tmp->user.debit_account;
        msg->credit_acc = tmp->user.credit_account;
        return;
    }
    else if(msg->type == 3)// withdraw $
    {
        field* sender = find_user(list, msg->sender_id);
        long int val = msg->debit_acc;// save the value

        if(msg->debit_acc <= sender->user.debit_account)
        {
            sender->user.debit_account -= msg->debit_acc;
            return;
        }
        if(msg->debit_acc > sender->user.debit_account)
        {
            if(msg->debit_acc + abs(sender->user.credit_account) > sender->user.debit_account + MAX_VAL)// too big for both acc
            {
                msg->debit_acc = NOT_ENOUGH_MONEY;
                return;
            }
            else// we use credit acc
            {
                msg->debit_acc -= sender->user.debit_account;
                sender->user.debit_account = 0;
                sender->user.credit_account -= msg->debit_acc;
                msg->debit_acc = val;
                return;
            }
        }
    }
    else // 4
    {
        field* tmp = find_user(list, msg->sender_id);
        if(msg->debit_acc > 0)
        {
            tmp->user.debit_account += msg->debit_acc;
        }
        else
        {
            tmp->user.credit_account += msg->credit_acc;
        }
        return;
    }
}
int main()
{
    printf("SERVER APPLICATION\n");
    //system("./bank_setup"); // gcc -o bank_setup bank_setup.c
    //system("./cloud"); // gcc -o cloud cloud.c
    int msgid_server = 0;// Initial value
    if(!connect(&msgid_server))
    {
        printf("Error while connecting\n");
        exit(-1);
    }
    message* msg = (message*) malloc(sizeof(message));// message buffer
    database* list = initialize();
    while(1)
    {
        if(msgrcv(msgid_server, msg, sizeof(message), 1, 0) < 0) 
        {
            printf("waiting for msgs from clients\n");
            continue;
        }
        long int client_id = msg->sender_id;
        proccessing(msg, list);
        msg->mtype = 1;
        int client = msgget(client_id, 0666 | IPC_CREAT);
        msgsnd(client, msg, sizeof(message), IPC_NOWAIT);
    }
    return 0;
}