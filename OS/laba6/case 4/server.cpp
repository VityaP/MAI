#include "bank.h"
#include <iostream>
#include <cstring>
#include "zmq.h"
#include <unistd.h>
#include <string>

struct MessageData {
    int clientId;
    int action;
    int amount;
    int whom;
    int bank_id;
    bool is_from_bank_to_bank;
};

int main(int argc, char const *argv[]) {
    void* context = zmq_ctx_new();
    void* serverSocket = zmq_socket(context, ZMQ_REP);
    
    char address[32];
    int bank_id;
    
    std::cout << "Enter bank id: ";
    std::cin >> bank_id;
    sprintf(address, "%s%d", "tcp://*:", bank_id);
    zmq_bind(serverSocket, address);
    
    std::cout << bank_id << "starts working" << std::endl;
    
    Bank b;
    
    for (;;) {
        zmq_msg_t message;
        zmq_msg_init(&message);
        zmq_msg_recv(&message, serverSocket, 0);
        MessageData *m = (MessageData *)zmq_msg_data(&message);
        zmq_msg_close(&message);
        
        char answer[128];

        if (!b.does_exist(m->clientId)) {
            b.add_new_client(m->clientId);
        }

        if (m->is_from_bank_to_bank) {
            b.add_money(m->clientId, m->amount);
            
			sprintf(answer, "Ok");    		
        } else
        switch(m->action) {
            case 1:
                if (b.get_money(m->clientId, m->amount)) {
                    sprintf(answer, "Here is your %d dollars", m->amount);
                    std::cout << "I`ve just given " << m->amount << " dollars to " << m->clientId << std::endl;
                } else {
                    sprintf(answer, "You don`t have enough money");
                    std::cout << m->clientId << " doesn`t have " << m->amount << " dollars" << std::endl;
                }
                break;
            case 2:
                b.add_money(m->clientId, m->amount);
                sprintf(answer, "Thank you for %d dollars", m->amount);
                std::cout << m->clientId << " has just put " << m->amount << " dollars into me" << std::endl;
                break;
            case 3:
                sprintf(answer, "Your balance is %d", b.balance(m->clientId));
                std::cout << m->clientId << " has just requested his balance" << std::endl;
                break;
            case 4:
                if (b.balance(m->clientId) >= m->amount) {
                    b.get_money(m->clientId, m->amount);
                    b.add_money(m->whom, m->amount);
                    sprintf(answer, "Done");
                    std::cout << "Transfered " << m->amount << " dollars from " << m->clientId << " to " << m->whom << std::endl;
                } else {
                    sprintf(answer, "You don`t have enough money");
                    std::cout << m->clientId << " doesn`t have " << m->amount << " dollars to transfer them to " << m->whom << std::endl;
                }
                break;
            case 5:
                std::cout << b.balance(m->clientId) << std::endl;
                if (b.balance(m->clientId) >= m->amount) {
                    b.get_money(m->clientId, m->amount);
                    std::cout << "here";
                    
                    void *tmpSenderSocket = zmq_socket(context, ZMQ_REQ);
                    char server_address[32];
                    sprintf(server_address, "%s%d", "tcp://localhost:", m->bank_id);
                    m->is_from_bank_to_bank = true;
                    zmq_connect(tmpSenderSocket, server_address);
                    
                    zmq_msg_t message_to_server;
                    zmq_msg_init_size(&message_to_server, sizeof(MessageData));
                    memcpy(zmq_msg_data(&message_to_server), m, sizeof(MessageData));
                    int send = zmq_msg_send(&message_to_server, tmpSenderSocket, 0);
                    zmq_msg_close(&message_to_server);
                    
					zmq_msg_t reply;
					zmq_msg_init(&reply);
					zmq_msg_recv(&reply, tmpSenderSocket, 0);
					size_t repSize = zmq_msg_size(&reply);
					char *replyMessage = (char *)zmq_msg_data(&reply);
					std::cout << replyMessage << std::endl;
					zmq_msg_close(&reply);
					
                    zmq_close(tmpSenderSocket);
                    
                    std::cout << "Transfered " << m->amount << " dollars from " << m->clientId << " to " << m->bank_id << "bank" << std::endl;
                    sprintf(answer, "Done");
                } else {
                    sprintf(answer, "You don`t have enough money");
                    std::cout << m->clientId << " doesn`t have " << m->amount << " dollars to transfer them to " << m->bank_id << "bank" << std::endl;
                }
                break;
        }
        
        zmq_msg_t reply;
        zmq_msg_init_size(&reply, strlen(answer) + 1);
        memcpy(zmq_msg_data(&reply), answer, strlen(answer) + 1);
        zmq_msg_send(&reply, serverSocket, 0);
        zmq_msg_close(&reply);
    }
    zmq_close(serverSocket);
    zmq_ctx_destroy(context);

    return 0;
}