#ifndef TBINARYTREEITEM
#define TBINARYTREEITEM

#include <cstdlib>
#include <iostream>
#include <memory>
#include "hexagon.h"
#include "triangle.h"
#include "octagon.h"

template <class T> class TBinaryTree;

template <class T> class TBinaryTreeItem
{
public:
    TBinaryTreeItem();
    TBinaryTreeItem(const std::shared_ptr<T> &obj);

    std::shared_ptr<T> GetFigure();
    ~TBinaryTreeItem();
    friend class TBinaryTree<T>;
    //template <class A> friend std::ostream& operator<<(std::ostream &os, TBinaryTreeItem<A> &obj);
private:
    std::shared_ptr<T> item;
    std::shared_ptr<TBinaryTreeItem<T>> left;
    std::shared_ptr<TBinaryTreeItem<T>> right;

};

#endif