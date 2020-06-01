#ifndef CONFIG_H
#define CONFIG_H
#define SBERBANK 100
#define VTB 101
#define RAIFAZEN 102
//--------------------------
//--------------------------
#define AUTH_MSG 0
#define NOT_ENOUGH_MONEY -1
#define UNREGISTERED_USER -2
#define AUTH_ERROR -3
#define CONNECT_ERROR -4
#define MAX_VAL 1000
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
typedef struct /*msgbuf*/ //main struct
{
    long mtype;// Default field in msgbuf struct
    int type;//0, 1, 2, 3, 4  - type of message
    char name[14];
    long int sender_id;
    long int recipient_id;    
    long long int debit_acc;
    long long int credit_acc;
}message;
#endif //CONFIG_H