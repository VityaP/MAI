#ifndef TBINARYTREE
#define TBINARYTREE

#include "TBinaryTreeItem.h"

class TBinaryTree
{
public:
    friend std::ostream& operator<<(std::ostream& os, TBinaryTree& tree);
    TBinaryTree();
    std::shared_ptr<TBinaryTreeItem> find(std::shared_ptr<Figure> &obj);
    void remove(int32_t side);
    void insert(std::shared_ptr<Figure> &obj);
    void print();
    void print(std::ostream& os);
    bool empty();
    virtual ~TBinaryTree();
private:
    std::shared_ptr<TBinaryTreeItem> head;
    std::shared_ptr<TBinaryTreeItem> minValueNode(std::shared_ptr<TBinaryTreeItem> root);
    std::shared_ptr<TBinaryTreeItem> deleteNode(std::shared_ptr<TBinaryTreeItem> root, int32_t side);
    void print_tree(std::shared_ptr<TBinaryTreeItem> item, int32_t a, std::ostream& os);
};

#endif