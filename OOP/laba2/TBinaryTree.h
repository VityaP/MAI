#ifndef TBINARYTREE
#define TBINARYTREE

#include "TBinaryTreeItem.h"

class TBinaryTree
{
public:
    friend std::ostream& operator<<(std::ostream& os, TBinaryTree& tree);
    TBinaryTree();
    TBinaryTreeItem* find(int32_t a);
    void remove(int32_t a);
    void insert(Hexagon &&hexagon);
    void print();
    void print(std::ostream& os);
    bool empty();
    virtual ~TBinaryTree();

private:
    TBinaryTreeItem* head;
    TBinaryTreeItem* minValueNode(TBinaryTreeItem* root);
    TBinaryTreeItem* deleteNode(TBinaryTreeItem* root, int32_t a);
    void print_tree(TBinaryTreeItem* item, int32_t a, std::ostream& os);
};

#endif