#include "bank.h"

Client::Client(uint32_t _id, uint32_t bal = 0) : id(_id), balance(bal) {
}

uint32_t Client::get_balance() {
    return balance;
}

uint32_t Client::get_id() {
    return id;
}

void Client::add_money(uint32_t amount) {
    balance += amount;
}

void Client::get_money(uint32_t amount) {
    balance -= amount;
}

Bank::Bank() {
    id = std::rand() % MAX_BANKS_NUMBER + 1;
}

void Bank::add_new_client(uint32_t id) {
    data.push_back(std::make_shared<Client> (id));
}

void Bank::add_money(uint32_t id, uint32_t amount) {
    for (auto it : data) {
        if (id == it->get_id()) {
            it->add_money(amount);
            return;
        }
    }

    data.push_back(std::make_shared<Client> (id, amount));
}

bool Bank::get_money(uint32_t id, uint32_t amount) {
    for (auto it : data) {
        if (id == it->get_id()) {
            if (it->get_balance() >= amount) {
                it->get_money(amount);
                return true;
            }
        }
    }
    return false;
}

uint32_t Bank::balance(uint32_t id) {
    for (auto it : data) {
        if (id == it->get_id()) {
            return it->get_balance();
        }
    }
}