#include "TBinaryTreeItem.h"

template <class T>
TBinaryTreeItem<T>::TBinaryTreeItem()
{
    left = nullptr;
    right = nullptr;
}

template <class T>
TBinaryTreeItem<T>::TBinaryTreeItem(const std::shared_ptr<T> &obj)
{
    this->item = obj;
    left = nullptr;
    right = nullptr;
}

template <class T>
TBinaryTreeItem<T>::~TBinaryTreeItem()
{
}

template <class T>
std::shared_ptr<T> TBinaryTreeItem<T>::GetFigure()
{
    return this->item;
}

#include "figure.h"
template class TBinaryTreeItem<Figure>;