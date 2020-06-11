#ifndef TTREE_H
#define TTREE_H

#include "figure.h"
#include <memory>
#include <iostream>

template <class Q, class O> class TTree {
private:
    class Node {
    public:
        Q data;
        std::shared_ptr<Node> son;
        std::shared_ptr<Node> sibling;
        Node();
        Node(O&);
        int itemsInNode;
    };

    std::shared_ptr<Node> root;

    void recRemByType(std::shared_ptr<Node>&, int&);
    void recInsert(std::shared_ptr<Node>&, O&);
    void recInorder(std::shared_ptr<Node>&);
    void recRemLesser(std::shared_ptr<Node>&, int32_t&);
    void clear(std::shared_ptr<Node>&, std::shared_ptr<Node>&);
    void GG(std::shared_ptr<Node>& node);
public:
    TTree();

    void insert(O&);
    void inorder(); 
    void removeByType(int&);
    void removeLesser(int32_t&);
    void RM();
};

#endif