#ifndef TBINARYTREE_H
#define TBINARYTREE_H

#include <cstdlib>
#include <iostream>
#include "TBinaryTreeItem.h"
#include "TIterator.h"
#include <future>
#include <thread>
#include <mutex>
#include <functional>

template <class T> class TBinaryTree
{
public:
    TBinaryTree();
    std::shared_ptr<TBinaryTreeItem<T>> find(std::shared_ptr<T> &obj);
    void remove(int32_t side);
    void insert(std::shared_ptr<T> &obj);
    void print();
    void print(std::ostream& os);
    template <class A> friend std::ostream& operator<<(std::ostream& os, TBinaryTree<A> &tree);
    bool empty();

    void gg();
    TIterator<TBinaryTreeItem<T>,T> begin();
    TIterator<TBinaryTreeItem<T>,T> end();

    virtual ~TBinaryTree();
private:
    std::shared_ptr<TBinaryTreeItem<T>> head;
    void  rm(std::shared_ptr<TBinaryTreeItem<T>> node);
    std::shared_ptr<TBinaryTreeItem<T>> Successor(std::shared_ptr<TBinaryTreeItem<T>> node);
    std::shared_ptr<TBinaryTreeItem<T>> minValueNode(std::shared_ptr<TBinaryTreeItem<T>> root);
    std::shared_ptr<TBinaryTreeItem<T>> deleteNode(std::shared_ptr<TBinaryTreeItem<T>> root, int32_t side);
    void print_tree(std::shared_ptr<TBinaryTreeItem<T>> item, int32_t a, std::ostream& os);
};

#endif