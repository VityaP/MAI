
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "zmq.h"
#include <unistd.h>
#include <ctime>
#include <random>
#include <iostream>
#include <string>

typedef struct MD
{
    int clientId;
    int messageNumber;
    char message[128];
    int action;
    int amount;
    int whom;
} MessageData;

int main(int argc, char const *argv[])
{
    void* context = zmq_ctx_new();
    srand(time(0));
    int clientId = std::rand();
    printf("Client %d Starting...\n", clientId);
    std::cout << "1 - get money" << std::endl;
    std::cout << "2 - put money" << std::endl;
    std::cout << "3 - balance" << std::endl;
    std::cout << "4 - send to somebody" << std::endl;

	char address[32];
	int bank_id;
	std::cin >> bank_id;
	sprintf(address, "%s%d", "tcp://localhost:", bank_id);
    void* senderSocket = zmq_socket(context, ZMQ_REQ);
    zmq_connect(senderSocket, address);
    int count = 0;
    for (;;)
    {
        MessageData md;
        md.clientId = clientId;
        md.messageNumber = count;
        std::cin >> md.action;
        switch(md.action) {
            case 1:
                std::cin >> md.amount;
                sprintf(md.message, "get money");
                break;
            case 2:
                std::cin >> md.amount;
                sprintf(md.message, "put money");
                break;
            case 3:
                sprintf(md.message, "balance");
                break;
            case 4:
                std::cin >> md.whom >> md.amount;
                sprintf(md.message, "send to somebody");
                break;
        }

        zmq_msg_t zmqMessage;
        zmq_msg_init_size(&zmqMessage, sizeof(MessageData));
        memcpy(zmq_msg_data(&zmqMessage), &md, sizeof(MessageData));

        int send = zmq_msg_send(&zmqMessage, senderSocket, 0);
        zmq_msg_close(&zmqMessage);

        zmq_msg_t reply;
        zmq_msg_init(&reply);
        zmq_msg_recv(&reply, senderSocket, 0);
        size_t repSize = zmq_msg_size(&reply);
        char *kek = (char *)zmq_msg_data(&reply);
        std::cout << kek << std::endl;
        zmq_msg_close(&reply);

        count++;
    }
    // We never get here though.
    zmq_close(senderSocket);
    zmq_ctx_destroy(context);

    return 0;
}