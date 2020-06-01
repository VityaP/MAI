
/*
This program is the private property of Viktor Petrosyan. 
Any use without the consent of the author is prohibited. 
The program began writing on January 4, 2019 
Last edit 26 January 2019 at 15: 49
*/
#include "functions.h"

int Find(players **head,int id){

    players *tmp=*head;    

    while(tmp!=NULL){

        if(tmp->id==id){

            return 1;

        }

        tmp = tmp->next;

    }
    
    return 0;

}

void Printdatabase(players **head){

    players *prev,*tmp=*head;    

    if(*head==NULL){

        return ;            

    }

    printf("|       ID      |    STATUS     |     TYPE      |   ID_OPPONENT |\n");
    
    while(tmp!=NULL){

        prev=tmp;
            
        printf("|\t%d\t|\t%d\t|\t%d\t|\t%d\t|\n",tmp->id,tmp->status,tmp->type,tmp->id_opponent);

        if(tmp!=NULL){

            tmp = tmp->next;

        }
    
    }

}

int Delete_players(players **head,int id){
    
	players *tmp=*head;
	
    while( tmp!=NULL ){
	
    	if(tmp->id == id){
	
    		tmp -> id = -1;
            
            tmp -> id_opponent = -1;
            
            tmp -> status = -1;
            
            tmp -> type = -1;

            return 1;

        }
	
    	tmp = tmp->next;	
	
    }

    return 0;

}

int Connect_player(players **head,int id,int type){

    int globalusl = 0;

    players *currentplayer = NULL;

    players *prev;
    
    players *tmp=*head;    

    while(tmp!=NULL){

        prev=tmp;

        if(tmp->id==id){

            if ( tmp->status == 2) {

                return 0 ;
            
            }
            else{

                currentplayer = tmp;

            }
            

        }

        tmp = tmp->next;

    }
    
    tmp = *head;    
    
    while(tmp!=NULL){
    
        prev=tmp;
        
        if( tmp->status == 1 &&  tmp->type != type ){

            currentplayer->status = 2;

            tmp->status = 2;

            currentplayer->id_opponent = tmp->id;

            tmp->id_opponent = currentplayer->id;

            globalusl ++;
    
        }
    
        if(tmp!=NULL)
    
        tmp = tmp->next;    
    
    }
    
    if (globalusl > 0) {

        return 1;
        
    }
    else{

        return 0;

    }
    
}

void Disconnect_player(players **head,int id){

    players *tmp=*head;    

    while(tmp!=NULL){

        if(tmp->id==id){

            tmp->status=3;

            break;

        }

        tmp = tmp->next;

    }

}

int OpponentID(players **head,int id){

    players *prev,*tmp=*head;    

    if(*head==NULL){

        return -1;            

    }

    while(tmp!=NULL){

        prev=tmp;

        if( tmp->id==id && tmp->status == 2){

            return tmp->id_opponent;

        }

        if(tmp!=NULL){

            tmp = tmp->next;

        }
    
    }
    
    return -1;


}

int Getstatus(players **head,int id){

    players *tmp=*head;    

    while(tmp!=NULL){

        if(tmp->id==id){

            return tmp->status;

        }

        tmp = tmp->next;

    }
    
    return -1;

}

int EnteringTOserver(players **head,int id,int type,int status){

    players *tmp=*head;

    players *prev;

    while(tmp!=NULL){

        if( tmp->id==id ){

            if( tmp->status != 3){

                return 0;

            }
            else{

                tmp->id_opponent = 0;

                tmp->type = type;

                if ( status == 11) {

                    tmp->status =2;

                }
                else{

                    tmp->status =1;

                }

                return 1;

            }

        }

        tmp = tmp->next;

    }

    tmp=*head;
    
    if(*head==NULL){
    
        (*head)=(players*)malloc(sizeof(players));
    
        (*head)->id=id;

        if ( status == 11) {

            (*head)->status=2;

        }
        else{

            (*head)->status=1;

        }
    
        (*head)->next=NULL;

        (*head)->type = type;
    
        return 1;                   
    
    }
    
    while(tmp!=NULL){
    
        if(tmp->id == -1){

            tmp->id=id;

            if ( status == 11) {

                tmp->status =2;

            }
            else{

                tmp->status =1;

            }

            tmp->type = type;

            return 1;

        }

        prev = tmp;
    
        tmp=tmp->next;    
    
    }
    
    if(tmp==NULL){

        tmp=(players*)malloc(sizeof(players));

        prev->next = tmp;
    
        tmp->id=id;

        if ( status == 11) {

            tmp->status =2;

        }
        else{

            tmp->status =1;

        }

        tmp->type = type;

        tmp->next=NULL;
    
    }
    
    return 1;

}

void PrintInfo( int * Array){

    for(size_t n = 0; n <= 54; n = n + 27){

        for(size_t s = 0; s <= 6; s = s + 3){

            for(size_t k = 0; k <= 18; k = k + 9){

                for(size_t i = 1; i <= 3; i++){

                    printf(" %d ", Array[i+k+s+n] );
            
                }

                printf("\t");
            
            }

            printf("\n");
            
        }

        printf("\n");
    
    }

}

void PrintGame( char * Array){

    for(size_t n = 0; n <= 54; n = n + 27){

        for(size_t s = 0; s <= 6; s = s + 3){

            for(size_t k = 0; k <= 18; k = k + 9){

                for(size_t i = 1; i <= 3; i++){

                    printf(" %c ", Array[i+k+s+n] );
            
                }

                printf("\t");
            
            }

            printf("\n");
            
        }

        printf("\n");
    
    }

}

int Position( char * Array , int index){

    for(size_t n = 0; n <= 54; n = n + 27){

        for(size_t s = 0; s <= 6; s = s + 3){

            for(size_t k = 0; k <= 18; k = k + 9){

                for(size_t i = 1; i <= 3; i++){

                    if ( i+k+s+n == index ){
                        
                        return ( i + s );

                    }
            
                }
            
            }
            
        }
    
    }

    return -2;

}

int PositionforCell( char * Array , int index){

    for(size_t n = 0; n <= 54; n = n + 27){

        for(size_t s = 0; s <= 6; s = s + 3){

            for(size_t k = 0; k <= 18; k = k + 9){

                for(size_t i = 1; i <= 3; i++){

                    if ( i+k+s+n == index ){
                        
                        if( k == 0 && n == 0){
                            return 1;
                        }

                        if( k == 9 && n == 0){
                            return 2;
                        }

                        if( k == 18 && n == 0){
                            return 3;
                        }

                        if( k == 0 && n == 27){
                            return 4;
                        }

                        if( k == 9 && n == 27){
                            return 5;
                        }

                        if( k == 18 && n == 27){
                            return 6;
                        }

                        if( k == 0 && n == 54){
                            return 7;
                        }

                        if( k == 9 && n == 54){
                            return 8;
                        }

                        if( k == 18 && n == 54){
                            return 9;
                        }

                    }
            
                }
            
            }
            
        }
    
    }

    return -2;

}

int Add( char * Array , int lastindex , int userindex , char what , int * taken,int escape){

    int k , n;

    switch (lastindex){

        case 1:
            k = 0;
            n = 0;
            break;

        case 2:
            k = 9;
            n = 0;
            break;

        case 3:
            k = 18;
            n = 0;
            break;

        case 4:
            k = 0;
            n = 27;
            break;
        
        case 5:
            k = 9;
            n = 27;
            break;

        case 6:
            k = 18;
            n = 27;
            break;

        case 7:
            k = 0;
            n = 54;
            break;

        case 8:
            k = 9;
            n = 54;
            break;

        case 9:
            k = 18;
            n = 54;
            break;
    
        default:
            break;
    }

    if(Array[userindex+k+n] == '.'){

        Array[userindex+k+n] = what;

        return 0;
    
    }
    else{

        if ( taken[lastindex] == 9 ){

            printf("There are no free cells in the block. \nSelect any other index [1 ... 81] which is free\n");

            return 2;

        }

        if (escape){

            printf("This cell is taken. Choose another cell \n");

            printf("Enter index [1 ... 9] \n");

            return 1;
        }
        else{

            return 0;

        }

    }

}

int Check( char * Array , int lastindex , int userindex){

    int i , s;

    int k , n;

    switch (userindex){

        case 1:
            
            i = 1;
            s = 0;
            break;

        case 2:
            i = 2;
            s = 0;
            break;

        case 3:
            i = 3;
            s = 0;
            break;

        case 4:
            i = 1;
            s = 3;
            break;
        
        case 5:
            i = 2;
            s = 3;
            break;

        case 6:
            i = 3;
            s = 3;
            break;

        case 7:
            i = 1;
            s = 6;
            break;

        case 8:
            i = 2;
            s = 6;
            break;

        case 9:
            i = 3;
            s = 6;
            break;
    
        default:
            break;
    }

    switch (lastindex){

        case 1:
            k = 0;
            n = 0;
            break;

        case 2:
            k = 9;
            n = 0;
            break;

        case 3:
            k = 18;
            n = 0;
            break;

        case 4:
            k = 0;
            n = 27;
            break;
        
        case 5:
            k = 9;
            n = 27;
            break;

        case 6:
            k = 18;
            n = 27;
            break;

        case 7:
            k = 0;
            n = 54;
            break;

        case 8:
            k = 9;
            n = 54;
            break;

        case 9:
            k = 18;
            n = 54;
            break;
    
        default:
            break;
    }

    return Array[i+k+s+n] ;

}

int CheckWinlocal( char * Array , int lastindex ){

    int OK = 0;

    int u1 = 1;
    int u2 = 2;
    int u3 = 3;
    int u4 = 4;
    int u5 = 5;
    int u6 = 6;
    int u7 = 7;
    int u8 = 8;
    int u9 = 9;

    if (Check ( Array ,lastindex , u1 ) != '.'){

        if  ( Check ( Array ,lastindex , u1 ) == Check ( Array ,lastindex , u2 ) ) {

            OK = 1;
        
        }

        if  ( Check ( Array ,lastindex , u2 ) == Check ( Array ,lastindex , u3 ) ) {

            if (OK == 1){

                return 1;
            
            }
            else{

                OK = 0;

            }
        
        }
        else{

            OK = 0;

        }
    }
    else{

        OK = 0;

    }


    if (Check ( Array ,lastindex , u4 ) != '.'){

        if  ( Check ( Array ,lastindex , u4 ) == Check ( Array ,lastindex , u5 ) ) {

            OK = 1;
        
        }

        if  ( Check ( Array ,lastindex , u5 ) == Check ( Array ,lastindex , u6 ) ) {

            if (OK == 1){

                return 1;
            
            }
            else{

                OK = 0;
                
            }
        
        }
        else{

            OK = 0;

        }
    }
    else{

        OK = 0;

    }


    if (Check ( Array ,lastindex , u7 ) != '.'){

        if  ( Check ( Array ,lastindex , u7 ) == Check ( Array ,lastindex , u8 ) ) {

            OK = 1;
        
        }

        if  ( Check ( Array ,lastindex , u8 ) == Check ( Array ,lastindex , u9 ) ) {

            if (OK == 1){

                return 1;
            
            }
            else{

                OK = 0;
                
            }
        
        }
        else{

            OK = 0;

        }

    }
    else{

        OK = 0;

    }


    if (Check ( Array ,lastindex , u1 ) != '.'){

        if  ( Check ( Array ,lastindex , u1 ) == Check ( Array ,lastindex , u4 ) ) {

            OK = 1;
        
        }

        if  ( Check ( Array ,lastindex , u4 ) == Check ( Array ,lastindex , u7 ) ) {

            if (OK == 1){

                return 1;
            
            }
            else{

                OK = 0;
                
            }
        
        }
        else{

            OK = 0;

        }

    }
    else{

        OK = 0;

    }


    if (Check ( Array ,lastindex , u2 ) != '.'){

        if  ( Check ( Array ,lastindex , u2 ) == Check ( Array ,lastindex , u5 ) ) {

            OK = 1;
        
        }

        if  ( Check ( Array ,lastindex , u5 ) == Check ( Array ,lastindex , u8 ) ) {

            if (OK == 1){

                return 1;
            
            }
            else{

                OK = 0;
                
            }
        
        }
        else{

            OK = 0;

        }

    }
    else{

        OK = 0;

    }


    if (Check ( Array ,lastindex , u3 ) != '.'){

        if  ( Check ( Array ,lastindex , u3 ) == Check ( Array ,lastindex , u6 ) ) {

            OK = 1;
        
        }

        if  ( Check ( Array ,lastindex , u6 ) == Check ( Array ,lastindex , u9 ) ) {

            if (OK == 1){

                return 1;
            
            }
            else{

                OK = 0;
                
            }
        
        }
        else{

            OK = 0;

        }

    }
    else{

        OK = 0;

    }


    if (Check ( Array ,lastindex , u1 ) != '.'){

        if  ( Check ( Array ,lastindex , u1 ) == Check ( Array ,lastindex , u5 ) ) {

            OK = 1;
        
        }

        if  ( Check ( Array ,lastindex , u5 ) == Check ( Array ,lastindex , u9 ) ) {

            if (OK == 1){

                return 1;
            
            }
            else{

                OK = 0;
                
            }
        
        }
        else{

            OK = 0;

        }

    }
    else{

        OK = 0;

    }


    if (Check ( Array ,lastindex , u3 ) != '.'){

        if  ( Check ( Array ,lastindex , u3 ) == Check ( Array ,lastindex , u5 ) ) {

            OK = 1;
        
        }

        if  ( Check ( Array ,lastindex , u5 ) == Check ( Array ,lastindex , u7 ) ) {

            if (OK == 1){

                return 1;
            
            }
            else{

                OK = 0;
                
            }
        
        }
        else{

            OK = 0;

        }

    }
    else{

        OK = 0;

    }

    return 0;

}

void AddglobalWin( char * Array , int index , char what){

    if(Array[index] == '.'){

        Array[index] = what;

    }
    
}

int CheckGlobal( char * Array , int index ){

    return Array[index];

}

int CheckWinglobal( char * Array ){

    int OK = 0;

    int u1 = 1;
    int u2 = 2;
    int u3 = 3;
    int u4 = 4;
    int u5 = 5;
    int u6 = 6;
    int u7 = 7;
    int u8 = 8;
    int u9 = 9;

    if (CheckGlobal ( Array , u1 ) != '.'){

        if  ( CheckGlobal ( Array , u1 ) == CheckGlobal ( Array , u2 ) ) {

            OK = 1;
        
        }

        if  ( CheckGlobal ( Array , u2 ) == CheckGlobal ( Array , u3 ) ) {

            if (OK == 1){

                return 1;
            
            }
            else{

                OK = 0;

            }
        
        }
        else{

            OK = 0;

        }
    }
    else{

        OK = 0;

    }


    if (CheckGlobal ( Array , u4 ) != '.'){

        if  ( CheckGlobal ( Array , u4 ) == CheckGlobal ( Array , u5 ) ) {

            OK = 1;
        
        }

        if  ( CheckGlobal ( Array , u5 ) == CheckGlobal ( Array , u6 ) ) {

            if (OK == 1){

                return 1;
            
            }
            else{

                OK = 0;
                
            }
        
        }
        else{

            OK = 0;

        }
    }
    else{

        OK = 0;

    }


    if (CheckGlobal ( Array , u7 ) != '.'){

        if  ( CheckGlobal ( Array , u7 ) == CheckGlobal ( Array , u8 ) ) {

            OK = 1;
        
        }

        if  ( CheckGlobal ( Array , u8 ) == CheckGlobal ( Array , u9 ) ) {

            if (OK == 1){

                return 1;
            
            }
            else{

                OK = 0;
                
            }
        
        }
        else{

            OK = 0;

        }

    }
    else{

        OK = 0;

    }


    if (CheckGlobal ( Array , u1 ) != '.'){

        if  ( CheckGlobal ( Array , u1 ) == CheckGlobal ( Array , u4 ) ) {

            OK = 1;
        
        }

        if  ( CheckGlobal ( Array , u4 ) == CheckGlobal ( Array , u7 ) ) {

            if (OK == 1){

                return 1;
            
            }
            else{

                OK = 0;
                
            }
        
        }
        else{

            OK = 0;

        }

    }
    else{

        OK = 0;

    }


    if (CheckGlobal ( Array , u2 ) != '.'){

        if  ( CheckGlobal ( Array , u2 ) == CheckGlobal ( Array , u5 ) ) {

            OK = 1;
        
        }

        if  ( CheckGlobal ( Array , u5 ) == CheckGlobal ( Array , u8 ) ) {

            if (OK == 1){

                return 1;
            
            }
            else{

                OK = 0;
                
            }
        
        }
        else{

            OK = 0;

        }

    }
    else{

        OK = 0;

    }


    if (CheckGlobal ( Array , u3 ) != '.'){

        if  ( CheckGlobal ( Array , u3 ) == CheckGlobal ( Array , u6 ) ) {

            OK = 1;
        
        }

        if  ( CheckGlobal ( Array , u6 ) == CheckGlobal ( Array , u9 ) ) {

            if (OK == 1){

                return 1;
            
            }
            else{

                OK = 0;
                
            }
        
        }
        else{

            OK = 0;

        }

    }
    else{

        OK = 0;

    }


    if (CheckGlobal ( Array , u1 ) != '.'){

        if  ( CheckGlobal ( Array , u1 ) == CheckGlobal ( Array , u5 ) ) {

            OK = 1;
        
        }

        if  ( CheckGlobal ( Array , u5 ) == CheckGlobal ( Array , u9 ) ) {

            if (OK == 1){

                return 1;
            
            }
            else{

                OK = 0;
                
            }
        
        }
        else{

            OK = 0;

        }

    }
    else{

        OK = 0;

    }


    if (CheckGlobal ( Array , u3 ) != '.'){

        if  ( CheckGlobal ( Array , u3 ) == CheckGlobal ( Array , u5 ) ) {

            OK = 1;
        
        }

        if  ( CheckGlobal ( Array , u5 ) == CheckGlobal ( Array , u7 ) ) {

            if (OK == 1){

                return 1;
            
            }
            else{

                OK = 0;
                
            }
        
        }
        else{

            OK = 0;

        }

    }
    else{

        OK = 0;

    }

    return 0;

}
