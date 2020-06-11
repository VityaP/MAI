#ifndef TBINARYTREEITEM
#define TBINARYTREEITEM

#include <cstdlib>
#include <iostream>
#include <memory>
#include "hexagon.h"
#include "triangle.h"
#include "octagon.h"
#include "TAllocationBlock.h"

template <class T> class TBinaryTree;

template <class T> class TBinaryTreeItem
{
public:
    TBinaryTreeItem();
    TBinaryTreeItem(const std::shared_ptr<T> &obj);

    std::shared_ptr<T> GetFigure();
    ~TBinaryTreeItem();
    std::shared_ptr<TBinaryTreeItem<T>> GetNext();

    void *operator new(size_t size);
    void operator delete(void *ptr);
    
    friend class TBinaryTree<T>;
private:
    std::shared_ptr<T> item;
    std::shared_ptr<TBinaryTreeItem<T>> left;
    std::shared_ptr<TBinaryTreeItem<T>> right;
    std::shared_ptr<TBinaryTreeItem<T>> parent;

    static TAllocationBlock btree_allocator;
};

#endif