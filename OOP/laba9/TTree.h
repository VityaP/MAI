#ifndef TTREE_H
#define TTREE_H

#include "figure.h"
#include <memory>
#include <iostream>
#include <future>
#include <thread>
#include <mutex>
#include <functional>
#include <exception>


template <typename Q> class TTree {
private:
    class Node {
        public:
            std::shared_ptr<Q> data;
            std::shared_ptr<Node> son;
            std::shared_ptr<Node> sibling;
            Node();
            Node(std::shared_ptr<Q> item);
    };

    std::shared_ptr<Node> root;
    unsigned int N;

    void recInsert(std::shared_ptr<Node>& node, std::shared_ptr<Q> item);

    void recInorder(std::shared_ptr<Node>& node);
public:
    TTree(const unsigned int& n);

    void insert(std::shared_ptr<Q> item);
    void inorder();
};

#endif