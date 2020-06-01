#ifndef BANK_H
#define BANK_H
#include <list>
#include <cinttypes>
#include <memory>
#include <random>

class Client {
private:
    uint32_t id;
    uint32_t balance;
public:
    Client(uint32_t _id, uint32_t bal);

    uint32_t get_balance();
    uint32_t get_id();
    void add_money(uint32_t amount);
    void get_money(uint32_t amount);
};

class Bank {
private:
    static const int MAX_BANKS_NUMBER = 10;
    uint32_t id;
    std::list<std::shared_ptr<Client>> data;
public:
    Bank();

    void add_new_client(uint32_t id);
    void add_money(uint32_t id, uint32_t amount);
    bool get_money(uint32_t id, uint32_t amount);
    uint32_t balance(uint32_t id);
};

#endif // BANK_H