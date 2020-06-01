#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include <stdio.h>
#include <stdbool.h>
#include "config.h"
#include "database.h"
int connect()
{
    int msgflg = IPC_CREAT | 0666;
    key_t key = (key_t)SBERBANK;// You should explicitly chose bank
    int msgid = msgget(key, msgflg);
    if(msgid != -1)
    {
        printf("successfull connection\n");
        return msgid;
    }
    else 
    {
        printf("Erorr while connection to cloud\n");
        return CONNECT_ERROR;
    }
}
void proccessing(message* msg, database* list)
{
    if(msg->type == AUTH_MSG)
    {
        if(!add_user(list, msg))
        {
            msg->value = AUTH_ERROR;
            return;
        }
    }
    else if(msg->type == 1)// Transfer $
    {
        field* sender = find_user(list, msg->sender_id);
        field* recipient = find_user(list, msg->recipient_id);
        if(recipient == NULL)// no such user
        {
            msg->value = UNREGISTERED_USER;
            return;
        }
        if(msg->value <= sender->user.account)
        {
            sender->user.account -= msg->value;
            recipient->user.account += msg->value;
            return;
        }
        else // not enough money for this operation
        {
            msg->value = NOT_ENOUGH_MONEY;
            return;
        }
    }
    else if(msg->type == 2)// Get bank account
    {
        field* tmp = find_user(list, msg->sender_id);
        msg->value = tmp->user.account;
        return;
    }
    else if(msg->type == 3)// withdraw $
    {
        field* tmp = find_user(list, msg->sender_id);
        if(msg->value <= tmp->user.account)
        {
            tmp->user.account -= msg->value;
            return;
        }
        else
        {
            msg->value = NOT_ENOUGH_MONEY;
            return;
        }
    }
    else // 4
    {
        field* tmp = find_user(list, msg->sender_id);
        tmp->user.account += msg->value;
        return;
    }
}
int main()
{
    int msgid_cloud = connect();
    if(msgid_cloud == CONNECT_ERROR) exit(-1);
    message* msg = (message*) malloc(sizeof(message));
    msg->mtype = 1;
    database* list = initialize();
    while(1)
    {
        //printf("waiting\n");
        if(msgrcv(msgid_cloud, msg, sizeof(message), 1, 0) < 0)
        {
            printf("Waiting for msgs from server\n");
            continue;
        }
        printf("Message recieved\n");
        proccessing(msg, list);
        int msgid_sender = msgget((key_t)msg->sender_id, IPC_CREAT | 0666);
        if(msgsnd(msgid_sender, msg, sizeof(message), IPC_NOWAIT) < 0)
        {
            perror("msgsnd 1\n");
        }
    }
    return 0;
}