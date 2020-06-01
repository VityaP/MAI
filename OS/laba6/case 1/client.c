#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "config.h"
int connect()
{
    printf("Choose bank, which you want to connect...\n");
    printf("1 - SBERBANK\n");
    printf("2 - VTB\n");
    printf("3 - RAIFAZEN\n");
    int a;
    scanf("%d", &a);
    int msgid;
    if(a == 1)
    {
        int msgflg = IPC_CREAT | 0666;
        key_t key = (key_t)SBERBANK;
        msgid = msgget(key, msgflg);
    }
    else if(a == 2)
    {
        int msgflg = IPC_CREAT | 0666;
        key_t key = (key_t)VTB;
        msgid = msgget(key, msgflg);
    }
    else
    {
        int msgflg = IPC_CREAT | 0666;
        key_t key = (key_t)RAIFAZEN;
        msgid = msgget(key, msgflg);
    }
    if(msgid != -1)
    {
        printf("Successful connection\n");
        return msgid;
    }
    else 
    {
        printf("Error while connection to the server\n");
        exit(-1);
    }
}
void reply(int msgid_client)
{
    message* tmp_msg = (message*) malloc(sizeof(message));
    while(1)
    {
        if(msgrcv(msgid_client, tmp_msg, sizeof(message), 1, 0) < 1) continue;
        if(tmp_msg->type == 1)
        {
            if(tmp_msg->debit_acc == NOT_ENOUGH_MONEY) printf("Not enough money for this operation.\n");
            else if(tmp_msg->debit_acc  == UNREGISTERED_USER) printf("No such user in system\n");
            else printf("Successful transfer!\n");
        }
        else if(tmp_msg->type == 2)
        {
            printf("You have: %lli$ on debit account\n", tmp_msg->debit_acc);
            printf("You have: %lli$ on credit account\n", tmp_msg->credit_acc);
        }
        else if(tmp_msg->type == 3)
        {
            if(tmp_msg->debit_acc  == NOT_ENOUGH_MONEY) printf("Not enough money for this operation\n");
            else printf("Operation complete. Here your %llu$\n", tmp_msg->debit_acc );
        }
        else if(tmp_msg->type == 4)
        {
            printf("Operation complete.\n");
        }    
        else printf("Undefined behaviour\n");
        free(tmp_msg);
        return;                    
    }
}
int auth(int msgid_serv, message* msg)
{
    printf("Enter your name\n");
    scanf("%s", msg->name);
    printf("Enter your id\n");
    scanf("%li", &msg->sender_id);
    printf("Enter your amount of money in the bank\n");
    scanf("%lli", &msg->debit_acc );
    int msgid_client = msgget((key_t)msg->sender_id, 0666 | IPC_CREAT);
    msg->type = AUTH_MSG;
    msgsnd(msgid_serv, msg, sizeof(message), IPC_NOWAIT);// sending auth message to server

    message* tmp_msg = (message*) malloc(sizeof(message));
    while(1)// waiting fo result
    {
        if(msgrcv(msgid_client, tmp_msg, sizeof(message), 1, 0) < 0)
        {
            sleep(1);
            printf("waiting for authorization..\n");
        }
        else
        {
            if(tmp_msg->type == AUTH_ERROR)
            {
                printf("Error while authorization\n");
                free(tmp_msg);
                exit(-1);
            }
            else 
            {
                free(tmp_msg);
                printf("Successfull authorization\n");
                return msgid_client;
            }
        }
    }
}
int main()
{
    printf("CLIENT APPLICATION\n");
    message* msg = (message*) malloc(sizeof(message));
    msg->mtype = 1;// default value

    int msgid_serv = connect(); // connection to preferable bank

    int msgid_client = auth(msgid_serv, msg);// auth on the server

    char command;
    while(1)
    {
        printf("1 - transfer $ to someone\n");
        printf("2 - get your bank account\n");
        printf("3 - withdraw some $\n");
        printf("4 - replenish a bank account\n");
        printf("q - for exit\n"); 
        scanf("%s", &command);
        if(command == '1')
        {
            //sender_id is already filled
            msg->type = 1;
            printf("Enter id of recipient\n");
            scanf("%li", &msg->recipient_id);
            printf("Enter amount of $\n");
            scanf("%lli", &msg->debit_acc);
            if(msgsnd(msgid_serv, msg, sizeof(message), IPC_NOWAIT) == -1)
            {
                perror("msgsnd 1\n");
                break;
            }
            else reply(msgid_client);
        }     
        else if(command == '2')
        {
            //sender_id is already filled            
            msg->type = 2;
            if(msgsnd(msgid_serv, msg, sizeof(message), IPC_NOWAIT) == -1)
            {
                perror("msgsnd 2\n");
                break;
            }
            else reply(msgid_client);
        }  
        else if(command == '3')
        {
            //sender_id is already filled                        
            msg->type = 3;
            printf("how much money do you want to withdraw?\n");
            scanf("%lli", &(msg->debit_acc));
            if(msgsnd(msgid_serv, msg, sizeof(message), IPC_NOWAIT) == -1)
            {
                perror("msgsnd 3\n");
                break;
            }
            else reply(msgid_client);
        }  
        else if(command == '4')
        {
            //sender_id is already filled                                    
            msg->type = 4;
            printf("which account you want to replish? [D/C]\n");
            fflush(stdin);
            fflush(stdout);
            char c;
            scanf("%s", &c);
            if(c == 'D')
            {
                printf("how much money you want to add to your debit account?\n");
                scanf("%lli", &(msg->debit_acc)); 
                msg->credit_acc = 0;
            }
            else
            {
                printf("how much money you want to add to your credit account?\n");
                scanf("%lli", &(msg->credit_acc)); 
                msg->debit_acc = 0;
            }
            if(msgsnd(msgid_serv, msg, sizeof(message), IPC_NOWAIT) == -1)
            {
                perror("msgsnd 4\n");
                break;
            }
            else reply(msgid_client);
        }  
        else if(command == 'q')
        {
            printf("exit from client\n");
            msgctl(msgid_client,IPC_RMID,NULL);
            free(msg);
            exit(0);
        }  
        else printf("Wrong command. Try again\n");
    }
    return 0;
}