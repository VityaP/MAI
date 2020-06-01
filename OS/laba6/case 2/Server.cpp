#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <zmq.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <pthread.h>
#include <fcntl.h>

pid_t Print;

void* context;
void* pubSocket;
void* pullSocket;

void* resSocket;

void* histSocket;
void* histPubSock;

const int LINGER_VAL = 0;

struct MessageData {
    char Name[80];
    char Message[256];
    char Address[80];
};

struct PassData {
    int action; // 0 - login, 1 - create.
    char Name[80];
    char Password[256];
};

struct PassCheck {
    int result; // 0 - OK, 1 - wrong pass, 2 - not found;
};

struct HistReq {
    char Names[160];
};

void* printHist(void* ptr) {
    histSocket = zmq_socket(context, ZMQ_PULL); // new socket for accepting requests
    zmq_setsockopt(histSocket, ZMQ_LINGER, &LINGER_VAL, sizeof(LINGER_VAL)); // setting waiting period after closing socket to zero
    zmq_bind(histSocket, "tcp://*:4044"); 
    histPubSock = zmq_socket(context, ZMQ_PUB); // socket for answering requests
    zmq_setsockopt(histPubSock, ZMQ_LINGER, &LINGER_VAL, sizeof(LINGER_VAL));
    zmq_bind(histPubSock, "tcp://*:4045");
    while(1) {
        zmq_msg_t histReq;
        zmq_msg_init(&histReq);
        zmq_msg_recv(&histReq, histSocket, 0); // receiving requests
        std::cout << "Received a HIstReqmessage" << std::endl;
        HistReq* hr = (HistReq*) zmq_msg_data(&histReq);
        std::cout << hr->Names << std::endl;

        char whisp[160];
        strcpy(whisp, hr->Names);
        char filename[160];
        strcpy(filename, "./."); // Creating filename
        strcat(filename, hr->Names);
        strcat(filename, ".log");
        zmq_msg_close(&histReq);
        int histfile;
        histfile = open(filename, O_CREAT | O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); // opening file with history
        if(histfile == -1) {
            std::perror("Can't open hist file");
        }
        struct stat filestat;
        flock(histfile, LOCK_EX); // Locking file
        fstat(histfile, &filestat);
        int fsize = filestat.st_size;
        char Name[80];
        char Message[256];
        char Address[80];
        int readHist;
        int lastMsg = 0;
        if(fsize == 0) {
            MessageData md;
            strcpy(md.Name, "server");
            strcpy(md.Message, "no messages yet");
            strcpy(md.Address, "gr");
            zmq_msg_t histRes;
            zmq_msg_init_size(&histRes, sizeof(MessageData));
            memcpy(zmq_msg_data(&histRes), &md, sizeof(MessageData));
            char lWhisp[160];
            strcpy(lWhisp, " lst"); // Writing to the theme that the message is last one
            strcat(lWhisp, whisp);
            zmq_send(histPubSock, lWhisp, strlen(lWhisp), ZMQ_SNDMORE);
            zmq_msg_send(&histRes, histPubSock, 0); // sending answer
            zmq_msg_close(&histRes);
        }
        while((readHist = read(histfile, Name, 80)) != 0) { // Reading all messages
            read(histfile, Message, 256);
            read(histfile, Address, 80);
            char check;
            int r = read(histfile, &check, 1); // Checking if this message is last one
            if(r != 0) {
                lseek(histfile, -1, SEEK_CUR);
            } else {
                lastMsg = 1;
            }
            MessageData md;
            strcpy(md.Name, Name);
            strcpy(md.Message, Message);
            strcpy(md.Address, Address);
            zmq_msg_t histRes;
            zmq_msg_init_size(&histRes, sizeof(MessageData));
            memcpy(zmq_msg_data(&histRes), &md, sizeof(MessageData));
            if(lastMsg == 0) {
                zmq_send(histPubSock, whisp, strlen(whisp), ZMQ_SNDMORE);
            } else {
                char lWhisp[160];
                strcpy(lWhisp, " lst"); // Writing to the theme that the message is last one
                strcat(lWhisp, whisp);
                zmq_send(histPubSock, lWhisp, strlen(lWhisp), ZMQ_SNDMORE);
            }
            zmq_msg_send(&histRes, histPubSock, 0); // sending answer
            zmq_msg_close(&histRes);
        }
        flock(histfile, LOCK_UN); // unlocking file
        close(histfile);
    }
}

void* checkLogins(void* ptr) {
    int passfile;
    passfile = open("./logins.log", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); // opening file with passwords
    if(passfile == -1) {
        std::perror("Can't open file");
        return NULL;
    }
    resSocket = zmq_socket(context, ZMQ_REP);
    zmq_setsockopt(resSocket, ZMQ_LINGER, &LINGER_VAL, sizeof(LINGER_VAL));
    zmq_bind(resSocket, "tcp://*:4043");
    while(1) {
        zmq_msg_t passMsg;
        zmq_msg_init(&passMsg);
        zmq_msg_recv(&passMsg, resSocket, 0); 
        std::cout << "PassCheck recv" << std::endl;
        PassData* pd = (PassData*) zmq_msg_data(&passMsg);
        std::cout << pd->action << " " << pd->Name << " " << pd->Password << std::endl;
        zmq_msg_close(&passMsg);
        char* name = pd->Name;
        char* pass = pd->Password;
        if(pd->action == 0) { // trying to enter
            lseek(passfile, 0, SEEK_SET);
            char fname[80];
            char fpass[256];
            int found = 0;
            int corPass = 0;
            int r = 0;
            while((r = read(passfile, fname, 80)) != 0) {
                if(r == -1) {
                    std::perror("Cant read file");
                }
                read(passfile, fpass, 256);
                std::cout << "Name: " << fname << std::endl;
                std::cout << "Pass: " << fpass << std::endl;
                if(strcmp(name, fname) == 0) {
                    found = 1;
                    if(strcmp(pass, fpass) == 0) {
                        corPass = 1;
                    }
                    break;
                }
            }
            PassCheck check;
            if(corPass == 1) { // OK 
                check.result = 0;
            } else if(found == 1) { // Wrong pass
                check.result = 1;
            } else {
                check.result = 2; // No such account exists
            }
            zmq_msg_t reply;
            zmq_msg_init_size(&reply, sizeof(PassCheck));
            memcpy(zmq_msg_data(&reply), &check, sizeof(PassCheck));
            if(zmq_msg_send(&reply, resSocket, 0) == -1) { // Sending answer
                std::perror("Cant send passcheck");
                return NULL;
            } else {
                std::cout << "PasscheckIN sent" << std::endl;
            }
            zmq_msg_close(&reply);
        } else {
            lseek(passfile, 0, SEEK_SET);
            char fname[80];
            char fpass[256];
            int found = 0;
            int r = 0;
            while((r = read(passfile, fname, 80)) != 0) {
                if(r == -1) {
                    std::perror("Cant read file");
                }
                read(passfile, fpass, 256);
                if(strcmp(name, fname) == 0) {
                    found = 1;
                    break;
                }
            }
            PassCheck check;
            if(found == 1) {
                check.result = 1; // Found account
            } else {
                check.result = 0;
            }
            if(found == 0) {
                lseek(passfile, 0, SEEK_END);
                write(passfile, name, 80); // If acc is not found then we add it
                write(passfile, pass, 256);
            }
            zmq_msg_t reply;
            zmq_msg_init_size(&reply, sizeof(PassCheck));
            memcpy(zmq_msg_data(&reply), &check, sizeof(PassCheck));
            if(zmq_msg_send(&reply, resSocket, 0) == -1) {
                std::perror("Can't send passcheck");
                return NULL;
            } else {
                std::cout << "PasscheckUP sent" << std::endl;
            }
            zmq_msg_close(&reply);
        }
    }
    return NULL;
}

void* chexit(void* ptr) {
    char ch;
    while(1) {
        std::cin >> ch;
        if(ch == 'q') {
            std::cout << "PrintExit: " << Print << std::endl;
            kill(Print, SIGKILL); // Killing second process
            exit(0); // EXiting the program
        }
    }
    return NULL;
}

void destrCtx() {
    std::cout << "Closing main sockets" << std::endl;
    zmq_close(pullSocket);
    zmq_close(pubSocket);
    std::cout << "Closing password socket" << std::endl;
    zmq_close(resSocket);
    std::cout << "Closing history sockets" << std::endl;
    zmq_close(histSocket);
    zmq_close(histPubSock);
    //std::cout << "Closing main context" << std::endl;
    // Something wrong happens here. The same issue is on github.com/zeromq/libzmq/issues/2343, but no soultion. So i just don't close the context;
    //zmq_ctx_destroy(context);
    std::cout << "ALL CLOSED" << std::endl;
}

int main(int argc, char** argv) {
    pthread_t passCheck;
    if(pthread_create(&passCheck, 0, checkLogins, NULL)) { // Thread for password processing
        std::perror("Can't create thread");
        return 4;
    }
    pthread_t sendHist;
    pthread_create(&sendHist, 0, printHist, NULL); // Thread for history request processing
    context = zmq_ctx_new();
    int fd[2];
    pipe(fd); // Pipe for interaction between processes
    if((Print = fork()) == -1) { // Creating process
        std::perror("Fork failed");
        return 1;
    }
    if(Print == 0) { // Process sending messages from server to clients
        pubSocket = zmq_socket(context, ZMQ_PUB);
        zmq_setsockopt(pubSocket, ZMQ_LINGER, &LINGER_VAL, sizeof(LINGER_VAL));
        zmq_bind(pubSocket, "tcp://*:4042");
        while(1) {
            size_t mesSize;
            MessageData* m = (MessageData*) malloc(sizeof(MessageData));
            read(fd[0], m->Name, 80);
            read(fd[0], m->Message, 256);
            read(fd[0], m->Address, 80);
            std::cout << "pub addr: " << m->Address << " "  << strlen(m->Address) << std::endl;
            char histName[160];
            int isGroupHist = 1;
            strcpy(histName, "./.");
            if(strcmp(m->Address, "gr") != 0) { // If needed we are writing history
                isGroupHist = 0;
                if(strcmp(m->Name, m->Address) < 0) {
                    strcat(histName, m->Name);
                    strcat(histName, m->Address);
                } else {
                    strcat(histName, m->Address);
                    strcat(histName, m->Name);
                }
            } else {
                strcat(histName, "gr");
            }
            strcat(histName, ".log");
            int histfile;
            histfile = open(histName, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
            flock(histfile, LOCK_EX);
            if(isGroupHist) {
                struct stat filestat;
                fstat(histfile, &filestat);
                int fsize = filestat.st_size;
                if(fsize >= 8320) {
                    char Nm[80];
                    char Msg[256];
                    char Adr[80];
                    char tmpFileName[160];
                    strcpy(tmpFileName, histName);
                    strcat(tmpFileName, "tmp");
                    int tmpfile;
                    tmpfile = open(tmpFileName, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH, S_IWOTH);
                    lseek(histfile, 0, SEEK_SET);
                    read(histfile, Nm, 80);
                    read(histfile, Msg, 256);
                    read(histfile, Adr, 80);
                    for(int i = 0; i < 19; i++) {
                        read(histfile, Nm, 80);
                        read(histfile, Msg, 256);
                        read(histfile, Adr, 80);
                        write(tmpfile, Nm, 80);
                        write(tmpfile, Msg, 256);
                        write(tmpfile, Adr, 80);
                    }
                    lseek(histfile, 0, SEEK_SET);
                    lseek(tmpfile, 0, SEEK_SET);
                    ftruncate(histfile, 0);
                    std::cout << "Truncated" << std::endl;
                    for(int i = 0; i < 19; i++) {
                        read(tmpfile, Nm, 80);
                        read(tmpfile, Msg, 256);
                        read(tmpfile, Adr, 80);
                        write(histfile, Nm, 80);
                        write(histfile, Msg, 256);
                        write(histfile, Adr, 80);
                    }
                    ftruncate(tmpfile, 0);
                }
            }
            write(histfile, m->Name, 80);
            write(histfile, m->Message, 256);
            write(histfile, m->Address, 80);
            flock(histfile, LOCK_UN);
            close(histfile);
            zmq_msg_t message;
            zmq_msg_init_size(&message, sizeof(MessageData));
            memcpy(zmq_msg_data(&message), m, sizeof(MessageData));
            zmq_send(pubSocket, m->Address, strlen(m->Address), ZMQ_SNDMORE); // Sending theme
            int send = zmq_msg_send(&message, pubSocket, 0); //  Sending message
            if(send == -1) {
                std::perror("Can't publish message");
                return 2;
            } else {
                std::cout << "Message sent" << std::endl;
            }
            zmq_msg_close(&message);
            free(m);
        }
    } else {
        atexit(destrCtx); // Closing sockets at exit
        std::cout << "Print: " << Print << std::endl;
        pthread_t checkExit;
        pthread_create(&checkExit, 0, chexit, NULL);
        pullSocket = zmq_socket(context, ZMQ_PULL);
        zmq_setsockopt(pullSocket, ZMQ_LINGER, &LINGER_VAL, sizeof(LINGER_VAL));
        zmq_bind(pullSocket, "tcp://*:4041");
        while(1) {
            zmq_msg_t message;
            zmq_msg_init(&message);
            zmq_msg_recv(&message, pullSocket, 0); // receiving message
            std::cout << "Message recieved" << std::endl;
            MessageData *m = (MessageData*) zmq_msg_data(&message);
            std::cout << m->Name << " " << m->Message << " " << m->Address << std::endl;
            write(fd[1], m->Name, 80); // Sending message to the other process
            write(fd[1], m->Message, 256);
            write(fd[1], m->Address, 80);
            zmq_msg_close(&message);
        }
    }
}