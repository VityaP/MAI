
/*

This program is the private property of Viktor Petrosyan. 
Any use without the consent of the author is prohibited. 
The program began writing on January 4, 2019 
Last edit 26 January 2019 at 15: 49
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <zmq.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <sys/ioctl.h>
#include <stropts.h>
#include "functions.h"
#include "message.h"

volatile sig_atomic_t flag = 0;

void block_func(){

    printf("You lost by resignation\n");
	
	printf("Quitting the game\n");

    flag = 3;

}

void quit_server_func(){

	printf("You lost by resignation\n");
	
	printf("Quitting the game\n");

	flag = 3;

}

void send_recv(message *mes,int *smove,int *sstatus,int *sreceiverPlayer,int *swin,int *slose,int *swinstat,int *slosestat,int *TYPE,char *stext){

    zmq_msg_t request;
    zmq_msg_init_size(&request, sizeof(message));
    memcpy(zmq_msg_data(&request), mes, sizeof(message));
    zmq_msg_send(&request, mes->requester, 0);
    zmq_msg_close(&request);

    zmq_msg_init(&request);
    zmq_msg_recv(&request, mes->requester, 0);
    mes = (message*)zmq_msg_data(&request);
    *smove = mes->movement;
    *sstatus = mes->status;
    *sreceiverPlayer = mes->receiverPlayer;
    *swin = mes->win;
    *slose = mes->lose;
    *swinstat = mes->winstat;
    *slosestat = mes->losestat;
    *TYPE = mes->playertype;
    for(size_t i = 0; i < Sizetext; i++){

        stext[i] = mes->text[i];
    
    }
    
    zmq_msg_close(&request);
}

void menu(){

    puts("---------------------------MENU---------------------------\n");
    puts("           1 - To play with a certain player              \n");
    puts("             2 - To play with a random player             \n");
    puts("                                                          \n");
    puts("           3 - To view personal statistics                \n");
    puts("             4 - To view general statistics               \n");
    puts("           5 - To view database                           \n");
    puts("             6 - To delete your profile from DB           \n");
    puts("----------------------------------------------------------\n");

}

int main(int argc, char *argv[]){

    message mes;

    int TYPE = -1;

    char stext[Sizetext];

    int slose = 0;

    int swin = 0;

    int smove;

    int swinstat;

    int slosestat;

    int sstatus = 0;

    int sreceiverPlayer;

    char stringplayer[20];

    char txt[30];

    int adress;

    int postusl;

	void* context = zmq_ctx_new();

	if(context == NULL){

		return 0;

    }

	void* request = zmq_socket(context, ZMQ_REQ);

	if(request == NULL){

		return 0;

    }

	printf("Starting….\n");

    if ( atoi(argv[1]) < 0 || atoi(argv[1]) > (SizeArray - 1) ){

        printf("ID should be from [0 ; %d]\n",SizeArray);
        
        exit(0);

    }

    printf("Enter the address of server : ");

    scanf("%d",&adress);

    sprintf(txt,"%s%d","tcp://localhost:",adress);

    zmq_connect(request, txt);//create socket here

    char stringdecision[20];

    int decision = 0;

    menu();

    printf("Enter your decision\n");

    scanf("%s",stringdecision);

    decision = atoi(stringdecision);

    postusl = 0;

    if ( decision > 0 && decision < 7 ){

        postusl = 1;

    }
    while( postusl != 1){

        printf("Inccorect entrance.Please type again\n");

        menu();

        scanf("%s",stringdecision);

        decision = atoi(stringdecision);

        if ( decision > 0 && decision < 7 ){

            postusl = 1;

        }
    
    }

    switch (decision){

        case 1:
            //Entering to server DB
            mes.action = 1;

            mes.id=atoi(argv[1]);

            mes.requester=request;

            mes.status = 10 + decision;

            send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);
            //Entering to server DB
            
        break;

        case 2:
            //Entering to server DB
            mes.action = 1;

            mes.id=atoi(argv[1]);

            mes.requester=request;

            mes.status = 10 + decision;

            send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);
            //Entering to server DB
            
        break;

        case 3:
            //To view personal statistics
            mes.action = 6;

            mes.id=atoi(argv[1]);

            mes.requester=request;

            send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

            printf("ID %d won %d times and lost %d times\n",atoi(argv[1]),swinstat,slosestat);

            exit(0);
            //To view personal statistics
            
        break;

        case 4:
            //To view general statistics
            mes.action = 7;

            mes.id=atoi(argv[1]);

            mes.requester=request;

            send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

            int j = 0;

            puts("------------------------Statistics------------------------\n");

			for(int i = 0; j < 5; i = i +6){

                printf("ID\t%d\t won %d times and lost %d times\n",stext[i+1],stext[i+3],stext[i+5]);
							
				j++;
						
			}

            puts("----------------------------------------------------------\n");

            exit(0);
            //To view general statistics
            
        break;

        case 5:
            //To view database
            mes.action = 8;

            mes.requester=request;

            send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

            exit(0);
            //To view database
            
        break;

        case 6:
            //To delete yourself from DB
            mes.action = -1;

            mes.id=atoi(argv[1]);

            mes.requester=request;

            send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

            exit(0);
            //To delete yourself from DB
            
        break;

    }
    
    if (sstatus == 2){

        printf("Sorry, this ID is in use now\n");

        printf("Try another one\n");
	
        printf("Quitting the game\n");

        exit(0);

    }

    if (TYPE == 1){

        switch (decision){

            //case To play with guy you want
            case 1:

                printf("Enter ID of another player from [0 ; %d]: ",SizeArray);
                
                scanf("%s",stringplayer);

                if ( !strncmp(stringplayer,"exit",4) ){

                    mes.action = 5;
                
                    mes.resign = 1;

                    mes.id=atoi(argv[1]);

                    mes.requester=request;
                    
                    send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                    while( slose != 1){

                        mes.action = 5;
                    
                        mes.resign = 1;

                        mes.id=atoi(argv[1]);

                        mes.requester=request;

                        send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);
                        
                    }

                    printf("You lost by resignation\n");
        
                    printf("Quitting the game\n");

                    exit(0);

                }

                mes.receiverPlayer = atoi(stringplayer);

                postusl = 0;

                if ( mes.receiverPlayer > 0 && mes.receiverPlayer < SizeArray ){

                    postusl = 1;

                }

                while( postusl != 1){

                    printf("Inccorect ID.Please type again\n");

                    printf("Enter ID of another player from [0 ; %d]: ",SizeArray);

                    scanf("%s",stringplayer);

                    if ( !strncmp(stringplayer,"exit",4) ){

                        mes.action = 5;
                    
                        mes.resign = 1;

                        mes.id=atoi(argv[1]);

                        mes.requester=request;
                        
                        send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                        while( slose != 1){

                            mes.action = 5;
                        
                            mes.resign = 1;

                            mes.id=atoi(argv[1]);

                            mes.requester=request;

                            send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);
                            
                        }

                        printf("You lost by resignation\n");
        
                        printf("Quitting the game\n");

                        exit(0);

                    }

                    mes.receiverPlayer = atoi(stringplayer);

                    if ( mes.receiverPlayer > 0 && mes.receiverPlayer < SizeArray ){

                        postusl = 1;

                    }
                
                }

                //need to check it?

                mes.status = 2;

            break;

            //case To connect random guy who is ready
            case 2:

                printf("Searching a player...\n");

                mes.action = 4;

                mes.receiverPlayer = -2;

                mes.id=atoi(argv[1]);

                mes.playertype = TYPE;
                
                mes.requester=request;

                send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                while( sstatus != 2){

                    mes.action = 4;

                    mes.receiverPlayer = -2;

                    mes.id=atoi(argv[1]);

                    mes.playertype = TYPE;
                    
                    mes.requester=request;

                    send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                }
                
                mes.receiverPlayer = sreceiverPlayer;

            break;
        
        }

        if ( decision == 1 ) {

            mes.receiverPlayer = atoi(stringplayer);
        
        }
        else{

            mes.receiverPlayer = sreceiverPlayer;

        }

        puts("----------------------------------------------------------\n");

        printf("Player is found\nYou are playing against %d player\n", mes.receiverPlayer);

        printf("3\n");
        sleep(1);
        printf("2\n");
        sleep(1);
        printf("1\n");
        sleep(1);

        printf("Game Starts\n");

        int * infodata;

        char * gamedata;

        char * globalwin;

        int * takenArray;

        int position;

        int x = -1;

        infodata = (int *)malloc( ( SizeV + 1 ) * sizeof(int));

        gamedata = (char *)malloc( ( SizeV + 1 ) * sizeof(char));

        globalwin = (char *)malloc( ( 10 ) * sizeof(char));

        takenArray = (int *)malloc( ( 10 ) * sizeof(int));

        for(size_t i = 0; i <= SizeV; i++){

            infodata[i] = i ;

            gamedata[i] = '.' ;
        
        }

        for(size_t i = 0; i < 10; i++){

            globalwin[i] = '.';

            takenArray[i] = 0;

        }

        PrintInfo(infodata);

        PrintGame(gamedata);

        int index = -1;

        char ndex[20];

        int turn = 1;

        int usl = 0;

        printf("Enter index [1 ... 81]\n");

        scanf("%s",ndex);

        if ( !strncmp(ndex,"exit",4) ){

            mes.action = 5;
        
            mes.resign = 1;

            mes.id=atoi(argv[1]);

            mes.requester=request;
            
            send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

            while( slose != 1){

                mes.action = 5;
            
                mes.resign = 1;

                mes.id=atoi(argv[1]);

                mes.requester=request;

                send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);
                
            }

            printf("You lost by resignation\n");
        
            printf("Quitting the game\n");

            exit(0);

        }

        index = atoi(ndex);

        if ( ( index > 0 && index < 82 )  ){
            
            usl = 1;
            
        }

        while( usl != 1){

            printf("Inccorect index.Please type again\n");

            printf("Enter index [1 ... 81] \n");

            scanf("%s",ndex);

            if ( !strncmp(ndex,"exit",4) ){

                mes.action = 5;
            
                mes.resign = 1;

                mes.id=atoi(argv[1]);

                mes.requester=request;
                
                send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                while( slose != 1){

                    mes.action = 5;
                
                    mes.resign = 1;

                    mes.id=atoi(argv[1]);

                    mes.requester=request;

                    send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);
                    
                }

                printf("You lost by resignation\n");
        
                printf("Quitting the game\n");

                exit(0);

            }

            index = atoi(ndex);

            if ( index > 0 && index < 82 ){
            
                usl = 1;
            
            }
        
        }

        gamedata[index] = 'x';

        takenArray[ PositionforCell(gamedata,index) ] ++;

        mes.action = 2;

        mes.movement = index;

        if ( decision == 1 ) {

            mes.receiverPlayer = atoi(stringplayer);
        
        }
        else{

            mes.receiverPlayer = sreceiverPlayer;

        }
        
        mes.id=atoi(argv[1]);
        
        mes.requester=request;

        send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

        if (swin == 1){

            printf("You won by resignation\n");
        
            printf("Quitting the game\n");

            exit(0);

        }

        if (slose == 1){

            printf("You lost\n");
                    
            printf("Quitting the game\n");

            exit(0);

        }

        PrintGame(gamedata);

        turn = 2;

        position = Position(gamedata , index);

        printf("Opponent turn\n");

        signal(SIGTSTP, block_func);//ctr + Z
        
        signal(SIGINT,quit_server_func);//ctr + c

        if ( flag == 3){

            mes.action = 5;
        
            mes.resign = 1;

            mes.id=atoi(argv[1]);

            mes.requester=request;
            
            send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

            while( slose != 1){

                mes.action = 5;
            
                mes.resign = 1;

                mes.id=atoi(argv[1]);

                mes.requester=request;

                send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);
                
            }

            printf("You lost by resignation\n");
        
            printf("Quitting the game\n");

            exit(0);

        }

        while(flag!=3){

                if(!flag){

                while ( index != 100 ){

                    if (swin == 1){

                        printf("You won by resignation\n");
        
                        printf("Quitting the game\n");

                        exit(0);

                    }

                    if (slose == 1){

                        printf("You lost\n");
                    
                        printf("Quitting the game\n");

                        exit(0);

                    }

                    if (turn == 1) {

                        scanf("%s",ndex);

                        if ( !strncmp(ndex,"exit",4) ){

                            mes.action = 5;
                        
                            mes.resign = 1;

                            mes.id=atoi(argv[1]);

                            mes.requester=request;
                            
                            send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                            while( slose != 1){

                                mes.action = 5;
                            
                                mes.resign = 1;

                                mes.id=atoi(argv[1]);

                                mes.requester=request;

                                send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);
                                
                            }

                            printf("You lost by resignation\n");
        
                            printf("Quitting the game\n");

                            exit(0);

                        }

                        index = atoi(ndex);

                        usl = 0;

                        if ( index > 0 && index < 10 ){
                                    
                            usl = 1;
                                    
                        }

                        while( usl != 1){

                            printf("Inccorect index.Please type again\n");

                            printf("Enter index [1 ... 9] \n");

                            scanf("%s",ndex);

                            if ( !strncmp(ndex,"exit",4) ){

                                mes.action = 5;
                            
                                mes.resign = 1;

                                mes.id=atoi(argv[1]);

                                mes.requester=request;
                                
                                send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                                while( slose != 1){

                                    mes.action = 5;
                                
                                    mes.resign = 1;

                                    mes.id=atoi(argv[1]);

                                    mes.requester=request;

                                    send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);
                                    
                                }

                                exit(0);

                                printf("You lost by resignation\n");
        
                                printf("Quitting the game\n");

                            }

                            index = atoi(ndex);

                            if ( index > 0 && index < 10 ){
                                    
                                usl = 1;
                                    
                            }
                                
                        }

                        int escape = 1;

                        while ( (x = Add( gamedata , position , index , 'x', takenArray , escape ) ) ){

                            if ( x == 1){

                                scanf("%s",ndex);

                                if ( !strncmp(ndex,"exit",4) ){

                                    mes.action = 5;
                                
                                    mes.resign = 1;

                                    mes.id=atoi(argv[1]);

                                    mes.requester=request;
                                    
                                    send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                                    while( slose != 1){

                                        mes.action = 5;
                                    
                                        mes.resign = 1;

                                        mes.id=atoi(argv[1]);

                                        mes.requester=request;

                                        send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);
                                        
                                    }

                                    printf("You lost by resignation\n");
        
                                    printf("Quitting the game\n");

                                    exit(0);

                                }

                                index = atoi(ndex);

                                usl = 0;

                                if ( index > 0 && index < 10 ){
                                    
                                    usl = 1;
                                    
                                }

                                while( usl != 1){

                                    printf("Inccorect index.Please type again\n");

                                    printf("Enter index [1 ... 9] \n");

                                    scanf("%s",ndex);

                                    if ( !strncmp(ndex,"exit",4) ){

                                        mes.action = 5;
                                    
                                        mes.resign = 1;

                                        mes.id=atoi(argv[1]);

                                        mes.requester=request;
                                        
                                        send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                                        while( slose != 1){

                                            mes.action = 5;
                                        
                                            mes.resign = 1;

                                            mes.id=atoi(argv[1]);

                                            mes.requester=request;

                                            send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);
                                            
                                        }

                                        printf("You lost by resignation\n");
        
                                        printf("Quitting the game\n");

                                        exit(0);

                                    }

                                    index = atoi(ndex);

                                    if ( index > 0 && index < 10 ){
                                    
                                        usl = 1;
                                    
                                    }
                                
                                }

                            }
                            if ( x == 2){

                                int USLOVIE = 1; 

                                while(USLOVIE){
                                
                                    printf("Enter index [1 ... 81] which is free\n");

                                    PrintInfo(infodata);

                                    scanf("%s",ndex);

                                    if ( !strncmp(ndex,"exit",4) ){

                                        mes.action = 5;
                                    
                                        mes.resign = 1;

                                        mes.id=atoi(argv[1]);

                                        mes.requester=request;
                                        
                                        send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                                        while( slose != 1){

                                            mes.action = 5;
                                        
                                            mes.resign = 1;

                                            mes.id=atoi(argv[1]);

                                            mes.requester=request;

                                            send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);
                                            
                                        }

                                        printf("You lost by resignation\n");
        
                                        printf("Quitting the game\n");

                                        exit(0);

                                    }

                                    index = atoi(ndex);

                                    usl = 0;

                                    if ( index > 0 && index < 82 ){
                                        
                                        usl = 1;
                                        
                                    }

                                    while( usl != 1){

                                        printf("Inccorect index.Please type again\n");

                                        printf("Enter index [1 ... 81] \n");

                                        scanf("%s",ndex);

                                        if ( !strncmp(ndex,"exit",4) ){

                                            mes.action = 5;
                                        
                                            mes.resign = 1;

                                            mes.id=atoi(argv[1]);

                                            mes.requester=request;
                                            
                                            send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                                            while( slose != 1){

                                                mes.action = 5;
                                            
                                                mes.resign = 1;

                                                mes.id=atoi(argv[1]);

                                                mes.requester=request;

                                                send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);
                                                
                                            }

                                            printf("You lost by resignation\n");
        
                                            printf("Quitting the game\n");

                                            exit(0);

                                        }

                                        index = atoi(ndex);

                                        if ( index > 0 && index < 82 ){
                                        
                                            usl = 1;
                                        
                                        }
                                    
                                    }

                                    if (gamedata[index] == '.'){
                                        
                                        gamedata[index] = 'x';

                                        position = PositionforCell(gamedata,index);

                                        index = Position(gamedata , index);

                                        USLOVIE = 0;

                                    }
                                    else{

                                        printf("This sell is taken too.\n");
                                    
                                    }

                                }


                            }

                        }

                        takenArray[position]++;

                        if ( CheckWinlocal( gamedata , position ) ){

                            AddglobalWin(globalwin,position, 'x');

                            if ( CheckWinglobal(globalwin) ){

                                index = 100;

                                PrintGame(gamedata);

                                printf("You won\n");

                                printf("Quitting the game\n");

                                mes.action = 9;

                                mes.id=atoi(argv[1]);
                                    
                                mes.requester=request;

                                send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                                exit(0);

                                break;

                            }

                        }

                        position = index;

                        PrintGame(gamedata);

                        mes.action = 2;

                        mes.movement = index;

                        if ( decision == 1 ) {

                            mes.receiverPlayer = atoi(stringplayer);
                        
                        }
                        else{

                            mes.receiverPlayer = sreceiverPlayer;

                        }

                        mes.id=atoi(argv[1]);
                        
                        mes.requester=request;

                        send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                        if (swin == 1){

                            printf("You won by resignation\n");
        
                            printf("Quitting the game\n");

                            exit(0);

                        }

                        if (slose == 1){

                            printf("You lost\n");
                        
                            printf("Quitting the game\n");

                            exit(0);

                        }

                        turn = 2;

                        printf("Opponent turn\n");
                    
                    }
                    else {

                        mes.action = 3;

                        mes.movement = -2;

                        smove = -7;

                        mes.id=atoi(argv[1]);
                        
                        mes.requester=request;

                        send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                        while( smove == -7){

                        }

                        while( smove == -2){

                            mes.action = 3;

                            mes.id=atoi(argv[1]);
                                
                            mes.requester=request;

                            send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                        }

                        if (swin == 1){

                            printf("You won by resignation\n");
        
                            printf("Quitting the game\n");

                            exit(0);

                        }

                        if (slose == 1){

                            printf("You lost\n");
                        
                            printf("Quitting the game\n");

                            exit(0);

                        }

                        printf("\n");

                        index = smove;

                        int escape = 1;

                        if ( index > 0 && index < 10 ){

                            Add( gamedata , position , index , '0',takenArray,escape);
                                
                        }
                        else{

                            gamedata[index] = '0';

                            position = PositionforCell(gamedata,index);

                            index = Position(gamedata , index);

                            escape = 0;


                        }

                        takenArray[position]++;

                        if ( CheckWinlocal( gamedata , position ) ){

                            AddglobalWin(globalwin,position, '0');

                            if ( CheckWinglobal(globalwin) ){

                                index = 100;

                                PrintGame(gamedata);

                                printf("You lost\n");
        
                                printf("Quitting the game\n");

                                //SOMETHING

                                exit(0);

                                break;

                            }

                        }

                        position = index;

                        turn = 1;

                        PrintGame(gamedata);

                        printf("Enter index [1 ... 9] \n");

                    }

                }

            }

        }
        
        return 0;

    }
    else{

        switch (decision){

            //case To play with guy you want
            case 1:

                printf("Enter ID of another player from [0 ; %d]: ",SizeArray);
                
                scanf("%s",stringplayer);

                if ( !strncmp(stringplayer,"exit",4) ){

                    mes.action = 5;
                
                    mes.resign = 1;

                    mes.id=atoi(argv[1]);

                    mes.requester=request;
                    
                    send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                    while( slose != 1){

                        mes.action = 5;
                    
                        mes.resign = 1;

                        mes.id=atoi(argv[1]);

                        mes.requester=request;

                        send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);
                        
                    }

                    printf("You lost by resignation\n");
        
                    printf("Quitting the game\n");

                    exit(0);

                }

                mes.receiverPlayer = atoi(stringplayer);

                postusl = 0;

                if ( mes.receiverPlayer > 0 && mes.receiverPlayer < SizeArray ){

                    postusl = 1;

                }

                while( postusl != 1){

                    printf("Inccorect ID.Please type again\n");

                    printf("Enter ID of another player from [0 ; %d]: ",SizeArray);

                    scanf("%s",stringplayer);

                    if ( !strncmp(stringplayer,"exit",4) ){

                        mes.action = 5;
                    
                        mes.resign = 1;

                        mes.id=atoi(argv[1]);

                        mes.requester=request;
                        
                        send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                        while( slose != 1){

                            mes.action = 5;
                        
                            mes.resign = 1;

                            mes.id=atoi(argv[1]);

                            mes.requester=request;

                            send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);
                            
                        }

                        printf("You lost by resignation\n");
        
                        printf("Quitting the game\n");

                        exit(0);

                    }

                    mes.receiverPlayer = atoi(stringplayer);

                    if ( mes.receiverPlayer > 0 && mes.receiverPlayer < SizeArray ){

                        postusl = 1;

                    }
                
                }

                //need to check it?

                mes.status = 2;

            break;

            //case To connect random guy who is ready
            case 2:

                printf("Searching a player...\n");

                mes.action = 4;

                mes.receiverPlayer = -2;

                mes.id=atoi(argv[1]);

                mes.playertype = 2;
                
                mes.requester=request;

                send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                while( sstatus != 2){

                    mes.action = 4;

                    mes.receiverPlayer = -2;

                    mes.id=atoi(argv[1]);

                    mes.playertype = 2;
                    
                    mes.requester=request;

                    send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                }
                
                mes.receiverPlayer = sreceiverPlayer;

            break;
        
        }

        if ( decision == 1 ) {

            mes.receiverPlayer = atoi(stringplayer);
        
        }
        else{

            mes.receiverPlayer = sreceiverPlayer;

        }

        puts("----------------------------------------------------------\n");

        printf("Player is found\nYou are playing against %d player\n",mes.receiverPlayer);
        
        printf("3\n");
        sleep(1);
        printf("2\n");
        sleep(1);
        printf("1\n");
        sleep(1);

        printf("Game Starts\n");
        
        int * infodata;

        char * gamedata;

        char * globalwin;

        int * takenArray;

        int position;

        int x = -1;

        infodata = (int *)malloc( ( SizeV + 1 ) * sizeof(int));

        gamedata = (char *)malloc( ( SizeV + 1 ) * sizeof(char));

        globalwin = (char *)malloc( ( 10 ) * sizeof(char));

        takenArray = (int *)malloc( ( 10 ) * sizeof(int));

        for(size_t i = 0; i <= SizeV; i++){

            infodata[i] = i ;

            gamedata[i] = '.' ;
        
        }

        for(size_t i = 0; i < 10; i++){

            globalwin[i] = '.';

            takenArray[i] = 0;

        }

        PrintInfo(infodata);

        PrintGame(gamedata);

        int index = -1;

        char ndex[20];

        int turn = 1;

        int usl = 0;

        printf("Opponent turn\n");

        mes.action = 3;

        mes.movement = -2;

        smove = -7;

        mes.id=atoi(argv[1]);
                
        mes.requester=request;

        send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

        while( smove == -7){

        }

        while( smove == -2){

            mes.action = 3;

            mes.id=atoi(argv[1]);
                        
            mes.requester=request;

            send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

        }

        if (swin == 1){

            printf("You won by resignation\n");
        
            printf("Quitting the game\n");

            exit(0);

        }

        if (slose == 1){

            printf("You lost\n");
        
            printf("Quitting the game\n");

            exit(0);

        }

        printf("\n");

        index = smove;

        gamedata[index] = 'x';

        takenArray[ PositionforCell(gamedata,index) ] ++;

        turn = 2;

        PrintGame(gamedata);

        position = Position(gamedata , index);

        printf("Enter index [1 ... 9] \n"); 

        signal(SIGTSTP, block_func);//ctr + Z
        
        signal(SIGINT,quit_server_func);//ctr + c

        if ( flag == 3){

            mes.action = 5;
        
            mes.resign = 1;

            mes.id=atoi(argv[1]);

            mes.requester=request;
            
            send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

            while( slose != 1){

                mes.action = 5;
            
                mes.resign = 1;

                mes.id=atoi(argv[1]);

                mes.requester=request;

                send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);
                
            }

            exit(0);

        }

        while(flag!=3){

            if(!flag){
        
                while ( index != 100 ){

                    if (swin == 1){

                        printf("You won by resignation\n");
        
                        printf("Quitting the game\n");

                        exit(0);

                    }
                    if (slose == 1){

                        printf("You lost\n");
                    
                        printf("Quitting the game\n");

                        exit(0);

                    }

                    if (turn == 1) {

                        mes.action = 3;

                        mes.movement = -2;

                        smove = -7;

                        mes.id=atoi(argv[1]);
                        
                        mes.requester=request;

                        send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                        while( smove == -7){

                        }

                        while( smove == -2){

                            mes.action = 3;

                            mes.id=atoi(argv[1]);
                            
                            mes.requester=request;

                            send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                        }

                        if (swin == 1){

                            printf("You won by resignation\n");
        
                            printf("Quitting the game\n");

                            exit(0);

                        }

                        if (slose == 1){

                            printf("You lost\n");
                        
                            printf("Quitting the game\n");

                            exit(0);

                        }

                        printf("\n");

                        index = smove;

                        int escape = 1;

                        if ( index > 0 && index < 10 ){

                            Add( gamedata , position , index , 'x',takenArray,escape);
                        
                        }
                        else{

                            gamedata[index] = 'x';

                            position = PositionforCell(gamedata,index);

                            index = Position(gamedata , index);

                            escape = 0;


                        }

                        takenArray[position]++;

                        if ( CheckWinlocal( gamedata , position ) ){

                            AddglobalWin(globalwin,position, 'x');

                            if ( CheckWinglobal(globalwin) ){

                                index = 100;

                                PrintGame(gamedata);

                                printf("You lost\n");
        
                                printf("Quitting the game\n");

                                exit(0);

                                break;

                            }

                        }

                        position = index;

                        turn = 2;

                        PrintGame(gamedata);

                        printf("Enter index [1 ... 9] \n");
                    
                    }
                    else {

                        scanf("%s",ndex);

                        if ( !strncmp(ndex,"exit",4) ){

                            mes.action = 5;
                        
                            mes.resign = 1;

                            mes.id=atoi(argv[1]);

                            mes.requester=request;
                            
                            send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                            while( slose != 1){

                                mes.action = 5;
                            
                                mes.resign = 1;

                                mes.id=atoi(argv[1]);

                                mes.requester=request;

                                send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);
                                
                            }

                            printf("You lost by resignation\n");
        
                            printf("Quitting the game\n");

                            exit(0);

                        }

                        index = atoi(ndex);

                        usl = 0;

                        if ( index > 0 && index < 10 ){
                                    
                            usl = 1;
                                    
                        }

                        while( usl != 1){

                            printf("Inccorect index.Please type again\n");

                            printf("Enter index [1 ... 9] \n");

                            scanf("%s",ndex);

                            if ( !strncmp(ndex,"exit",4) ){

                                mes.action = 5;
                            
                                mes.resign = 1;

                                mes.id=atoi(argv[1]);

                                mes.requester=request;
                                
                                send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                                while( slose != 1){

                                    mes.action = 5;
                                
                                    mes.resign = 1;

                                    mes.id=atoi(argv[1]);

                                    mes.requester=request;

                                    send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);
                                    
                                }

                                printf("You lost by resignation\n");
        
                                printf("Quitting the game\n");

                                exit(0);

                            }

                            index = atoi(ndex);

                            if ( index > 0 && index < 10 ){
                                    
                                usl = 1;
                                    
                            }
                                
                        }

                        int escape = 1;

                        while ( (x = Add( gamedata , position , index , '0',takenArray,escape) ) ){

                            if ( x == 1){

                                scanf("%s",ndex);

                                if ( !strncmp(ndex,"exit",4) ){

                                    mes.action = 5;
                                
                                    mes.resign = 1;

                                    mes.id=atoi(argv[1]);

                                    mes.requester=request;
                                    
                                    send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                                    while( slose != 1){

                                        mes.action = 5;
                                    
                                        mes.resign = 1;

                                        mes.id=atoi(argv[1]);

                                        mes.requester=request;

                                        send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);
                                        
                                    }

                                    printf("You lost by resignation\n");
        
                                    printf("Quitting the game\n");

                                    exit(0);

                                }

                                index = atoi(ndex);

                                usl = 0;

                                if ( index > 0 && index < 10 ){
                                    
                                    usl = 1;
                                    
                                }

                                while( usl != 1){

                                    printf("Inccorect index.Please type again\n");

                                    printf("Enter index [1 ... 9] \n");

                                    scanf("%s",ndex);

                                    if ( !strncmp(ndex,"exit",4) ){

                                        mes.action = 5;
                                    
                                        mes.resign = 1;

                                        mes.id=atoi(argv[1]);

                                        mes.requester=request;
                                        
                                        send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                                        while( slose != 1){

                                            mes.action = 5;
                                        
                                            mes.resign = 1;

                                            mes.id=atoi(argv[1]);

                                            mes.requester=request;

                                            send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);
                                            
                                        }

                                        printf("You lost by resignation\n");
        
                                        printf("Quitting the game\n");

                                        exit(0);

                                    }

                                    index = atoi(ndex);

                                    if ( index > 0 && index < 10 ){
                                    
                                        usl = 1;
                                    
                                    }
                                
                                }

                            }
                            if ( x == 2){

                                int USLOVIE = 1; 

                                while(USLOVIE){
                                
                                    printf("Enter index [1 ... 81] which is free \n");

                                    PrintInfo(infodata);

                                    scanf("%s",ndex);

                                    if ( !strncmp(ndex,"exit",4) ){

                                        mes.action = 5;
                                    
                                        mes.resign = 1;

                                        mes.id=atoi(argv[1]);

                                        mes.requester=request;
                                        
                                        send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                                        while( slose != 1){

                                            mes.action = 5;
                                        
                                            mes.resign = 1;

                                            mes.id=atoi(argv[1]);

                                            mes.requester=request;

                                            send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);
                                            
                                        }

                                        printf("You lost by resignation\n");
        
                                        printf("Quitting the game\n");

                                        exit(0);

                                    }

                                    index = atoi(ndex);

                                    usl = 0;

                                    if ( index > 0 && index < 82 ){
                                        
                                        usl = 1;
                                        
                                    }

                                    while( usl != 1){

                                        printf("Inccorect index.Please type again\n");

                                        printf("Enter index [1 ... 81] \n");

                                        scanf("%s",ndex);

                                        if ( !strncmp(ndex,"exit",4) ){

                                            mes.action = 5;
                                        
                                            mes.resign = 1;

                                            mes.id=atoi(argv[1]);

                                            mes.requester=request;
                                            
                                            send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                                            while( slose != 1){

                                                mes.action = 5;
                                            
                                                mes.resign = 1;

                                                mes.id=atoi(argv[1]);

                                                mes.requester=request;

                                                send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);
                                                
                                            }

                                            printf("You lost by resignation\n");
        
                                            printf("Quitting the game\n");

                                            exit(0);

                                        }

                                        index = atoi(ndex);

                                        if ( index > 0 && index < 82 ){
                                        
                                            usl = 1;
                                        
                                        }
                                    
                                    }

                                    if (gamedata[index] == '.'){
                                        
                                        gamedata[index] = '0';

                                        position = PositionforCell(gamedata,index);

                                        index = Position(gamedata , index);

                                        USLOVIE = 0;

                                        escape = 0;

                                    }
                                    else{

                                        printf("This sell is taken too.\n");
                                    
                                    }

                                }


                            }

                        }

                        takenArray[position]++;

                        if ( CheckWinlocal( gamedata , position ) ){

                            AddglobalWin(globalwin,position, '0');

                            if ( CheckWinglobal(globalwin) ){

                                index = 100;

                                PrintGame(gamedata);

                                printf("You won \n");

                                printf("Quitting the game\n");

                                mes.action = 9;

                                mes.id=atoi(argv[1]);
                                    
                                mes.requester=request;

                                send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                                exit(0);

                                break;

                            }

                        }

                        position = index;

                        PrintGame(gamedata);

                        mes.action = 2;

                        mes.movement = index;

                        if ( decision == 1 ) {

                            mes.receiverPlayer = atoi(stringplayer);
                        
                        }
                        else{

                            mes.receiverPlayer = sreceiverPlayer;

                        }

                        mes.id=atoi(argv[1]);
                        
                        mes.requester=request;

                        send_recv(&mes,&smove,&sstatus,&sreceiverPlayer,&swin,&slose,&swinstat,&slosestat,&TYPE,stext);

                        if (swin == 1){

                            printf("You won by resignation\n");
        
                            printf("Quitting the game\n");

                            exit(0);

                        }

                        if (slose == 1){

                            printf("You lost\n");
                        
                            printf("Quitting the game\n");

                            exit(0);

                        }

                        turn = 1;

                        printf("Opponent turn\n");

                    }

                }

            }

        }

        return 0;

    }

    return 0;

}
