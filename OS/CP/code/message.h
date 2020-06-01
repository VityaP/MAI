#ifndef MESSAGE_H
#define MESSAGE_H

#define Sizetext 127

typedef struct _msg{    
    int id;
    int movement;
    int receiverPlayer;
    int decision;
    int status;//0 manual conection//1 waiting//2 in game//3 in DB but mo in game
    int resign;//0 manual //1 resign game
    int lose;//0 manual //1 if lose
    int win;
    int winstat;
    int losestat;
    int playertype;
    int action;
    void *requester;
    char text[Sizetext];
} message;

#endif