#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SizeV 81
#define SizeArray 20

typedef struct _players{
    int id;
    int id_opponent;
    int status;
    int type;
    struct _players *next;
}players;

extern int Find(players **head,int id);

extern void Printdatabase(players **head);

extern int Delete_players(players **head,int id);

extern void Disconnect_player(players **head,int id);

extern int Connect_player(players **head,int id,int type);

extern int OpponentID(players **head,int id);

extern int Getstatus(players **head,int id);

extern int EnteringTOserver(players **head,int id,int type,int status);

extern void PrintInfo( int * Array);

extern void PrintGame( char * Array);

extern int Position( char * Array , int index);

extern int PositionforCell( char * Array , int index);

extern int Add( char * Array , int lastindex , int userindex , char what , int * taken,int escape);

extern int Check( char * Array , int lastindex , int userindex);

extern int CheckWinlocal( char * Array , int lastindex );

extern void AddglobalWin( char * Array , int index , char what);

extern int CheckGlobal( char * Array , int index );

extern int CheckWinglobal( char * Array );

#endif 
/*FUNCTIONS_H*/