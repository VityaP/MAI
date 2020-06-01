#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include <stdio.h>
#include "config.h"
int main()
{
    printf("Initialization of banks\n");
    int msgflg = IPC_CREAT | 0666;
    if(msgget((key_t)SBERBANK, msgflg) == -1) perror("SBERBANK\n");
    //if(msgget((key_t)VTB, msgflg) == -1) perror("VTB\n");
    //if(msgget((key_t)RAIFAZEN, msgflg) == -1) perror("RAIFAZEN\n");
    printf("All banks is opened\n");
    return 0;
} 