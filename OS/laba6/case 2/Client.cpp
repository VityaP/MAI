#include <iostream>
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <zmq.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>
#include <sys/types.h>

#define ANSI_COLOR_RED "\x1b[31m" // Escape-sequences for changing color of text in terminal
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_RESET "\x1b[0m"

termios orig_termios; // Original state of terminal

void* context;
void* pushSocket;
void* subSocket;

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

struct HistReq {
    char Names[160];
};

struct PassCheck {
    int result; // 0 - OK, 1 - wrong pass, 2 - not found;
};

char* Login() {
    std::cout << "0 - sign in, 1 - sign up: ";
    int act;
    std::cin >> act;
    char* Name = (char*) malloc(80);
    char Password[256];
    std::cout << "Please, enter your name: ";
    std::cin >> Name;
    std::cout << "Please, enter your pass: ";
    std::cin >> Password;
    void* ctx = zmq_ctx_new();
    void* reqSocket = zmq_socket(ctx, ZMQ_REQ);
    zmq_setsockopt(reqSocket, ZMQ_LINGER, &LINGER_VAL, sizeof(LINGER_VAL));
    zmq_connect(reqSocket, "tcp://localhost:4043");
    PassData pd;
    pd.action = act;
    strcpy(pd.Name, Name);
    strcpy(pd.Password, Password);
    zmq_msg_t passMsg;
    zmq_msg_init_size(&passMsg, sizeof(PassData));
    memcpy(zmq_msg_data(&passMsg), &pd, sizeof(PassData));
    zmq_msg_send(&passMsg, reqSocket, 0);
    zmq_msg_close(&passMsg);
    zmq_msg_t reply;
    zmq_msg_init(&reply);
    zmq_msg_recv(&reply, reqSocket, 0);
    PassCheck* pc = (PassCheck*) zmq_msg_data(&reply);
    zmq_msg_close(&reply);
    if(pc->result == 0) {
        if(act == 0) {
            zmq_close(reqSocket);
            zmq_ctx_destroy(ctx);
            return Name;
        } else {
            zmq_close(reqSocket); // Closing socket and context, because we will open them again
            zmq_ctx_destroy(ctx);
            return Login();
        }
    } else if(pc->result == 1) {
        if(act == 0) {
            std::cout << "Wrong Password" << std::endl;
            zmq_close(reqSocket);
            zmq_ctx_destroy(ctx);
            return Login();
        } else {
            std::cout << "Account already exists" << std::endl;
            zmq_close(reqSocket);
            zmq_ctx_destroy(ctx);
            return Login();
        }
    } else {
        std::cout << "Account doesn't exist" << std::endl;
        zmq_close(reqSocket);
        zmq_ctx_destroy(ctx);
        return Login();
    }
}

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); // Return to original terminal state
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios); // Remembering original terminal state
    atexit(disableRawMode); // Return to original terminal state at exit
    termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON); // Disable canonical mode
    raw.c_iflag &= ~(IXON); // Disable Ctrl-q Ctrl-s
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void destrCtx() {
    zmq_close(subSocket);
    zmq_close(pushSocket);
    zmq_ctx_destroy(context);
}

void inpAddr(char* addr) {
    char c = getchar();
    int idx = 0;
    do {
        addr[idx++] = c;
        c = getchar();
    } while(c != ' ');
    addr[idx] = 0;
    return;
}

void getHistory(char* Name, char* Other) {
    void* histCtx = zmq_ctx_new();
    void* pushSock = zmq_socket(histCtx, ZMQ_PUSH);
    zmq_connect(pushSock, "tcp://localhost:4044");
    char whisp[160];
    if(Other != NULL) {
        if(strcmp(Name, Other) < 0) {
            strcpy(whisp, Name);
            strcat(whisp, Other);
        } else {
            strcpy(whisp, Other);
            strcat(whisp, Name);
        }
    } else {
        strcpy(whisp, "gr");
    }
    HistReq hr;
    strcpy(hr.Names, whisp);
    zmq_msg_t messageReq;
    zmq_msg_init_size(&messageReq, sizeof(HistReq));
    memcpy(zmq_msg_data(&messageReq), &hr, sizeof(HistReq));
    zmq_msg_send(&messageReq, pushSock, 0);
    zmq_msg_close(&messageReq);
    zmq_close(pushSock);
    void* subSock = zmq_socket(histCtx, ZMQ_SUB);
    char lWhisp[160];
    strcpy(lWhisp, " lst");
    strcat(lWhisp, whisp);
    zmq_connect(subSock, "tcp://localhost:4045");
    zmq_setsockopt(subSock, ZMQ_SUBSCRIBE, whisp, strlen(whisp)); // Subscribing to history
    zmq_setsockopt(subSock, ZMQ_SUBSCRIBE, lWhisp, strlen(lWhisp)); // For checking for last message in history
    while(1) {
        zmq_msg_t message;
        char filter[32];
        zmq_recv(subSock, filter, 4, 0);
        zmq_msg_init(&message);
        zmq_msg_recv(&message, subSock, 0);
        MessageData* m = (MessageData*) zmq_msg_data(&message);
        std::cout << m->Name << ": " << m->Message << std::endl;
        zmq_msg_close(&message);
        if(filter[0] == ' ') {
            break;
        }
    }
    zmq_close(subSock);
    zmq_ctx_destroy(histCtx);
    printf(ANSI_COLOR_RED " === History ended === \n" ANSI_COLOR_RESET);
}

int main(int argc, char** argv) {
    atexit(destrCtx);
    char* Name = Login();
    printf(ANSI_COLOR_RED "Entered as: %s\n", Name);
    printf(ANSI_COLOR_RESET);
    context = zmq_ctx_new();
    if(argc == 2)
        getHistory(Name, argv[1]);
    else
        getHistory(Name, NULL);
    pid_t display;
    if((display = fork()) == -1) {
        std::perror("Fork failed");
        return 1;
    }
    if(display == 0) {
        subSocket = zmq_socket(context, ZMQ_SUB);
        zmq_setsockopt(subSocket, ZMQ_LINGER, &LINGER_VAL, sizeof(LINGER_VAL));
        zmq_connect(subSocket, "tcp://localhost:4042");
        if(argc == 1) {
            zmq_setsockopt(subSocket, ZMQ_SUBSCRIBE, "gr", 2); // Subscribing to group
        }
        zmq_setsockopt(subSocket, ZMQ_SUBSCRIBE, Name, strlen(Name)); // Subscribing to personal messages
        while(1) {
            char theme[80];
            zmq_msg_t message;
            zmq_msg_init(&message);
            int th = zmq_recv(subSocket, theme, 3, 0);
            zmq_msg_recv(&message, subSocket, 0);
            MessageData *m = (MessageData*) zmq_msg_data(&message);
            if(strcmp(m->Name, Name) != 0) {
                printf("\33[2K\r");
                if(th != 2 && argc == 1) {
                    printf(ANSI_COLOR_GREEN);
                }
                std::cout << m->Name << ": " << m->Message << std::endl;
                printf(ANSI_COLOR_RESET);
                std::cout << Name << ": ";
                std::cout.flush();
            }
            zmq_msg_close(&message);
        }
    } else {
        pushSocket = zmq_socket(context, ZMQ_PUSH);
        zmq_setsockopt(pushSocket, ZMQ_LINGER, &LINGER_VAL, sizeof(LINGER_VAL));
        zmq_connect(pushSocket, "tcp://localhost:4041");
        int c = 0;
        c = getchar();
        while(1) {
            enableRawMode();
            MessageData message;
            strcpy(message.Name, Name);
            char addr[] = "gr\0"; 
            if(argc == 1) {
                strcpy(message.Address, addr); // Be default we are writing to the group
            } else {
                strcpy(message.Address, argv[1]); // Or personal message
            }
            std::cout << Name << ": ";
            int counter = 0;
            do {
                c = getchar();
                if(c == '/' && counter == 0 && argc == 1) { 
                    int cNext = getchar();
                    if(cNext == 'w') { // If we read /w then read destination
                        cNext = getchar(); // Skipping space
                        inpAddr(message.Address);
                    } else {
                        message.Message[counter++] = '/';
                        message.Message[counter++] = cNext;
                    }
                } else {
                    message.Message[counter++] = c;
                }
                if(c == 17) {
                    std::cout << std::endl;
                    free(Name);
                    kill(display, SIGKILL);
                    exit(0);
                }
            } while(c != '\n');
            message.Message[counter-1] = 0;
            zmq_msg_t zmqMessage;
            zmq_msg_init_size(&zmqMessage, sizeof(message));
            memcpy(zmq_msg_data(&zmqMessage), &message, sizeof(message));
            int send = zmq_msg_send(&zmqMessage, pushSocket, 0);
            if(send == -1) {
                std::perror("Can't push message");
                return 2;
            }
            zmq_msg_close(&zmqMessage);
        }
    }
}