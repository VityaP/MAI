#include "TBinaryTreeItem.h"

template <class T>
TBinaryTreeItem<T>::TBinaryTreeItem()
{
    left = nullptr;
    right = nullptr;
    parent = nullptr;
}

template <class T>
TBinaryTreeItem<T>::TBinaryTreeItem(const std::shared_ptr<T> &obj)
{
    this->item = obj;
    left = nullptr;
    right = nullptr;
    parent = nullptr;
}

template <class T> TAllocationBlock
TBinaryTreeItem<T>::btree_allocator(sizeof(TBinaryTreeItem<T>), 100);

template <class T>
TBinaryTreeItem<T>::~TBinaryTreeItem()
{
}

template <class T>
std::shared_ptr<T> TBinaryTreeItem<T>::GetFigure()
{
    return this->item;
}

template <class T>
std::shared_ptr<TBinaryTreeItem<T>> TBinaryTreeItem<T>::GetNext()
{
    if (this->left) {
        return this->left;
    } else if (this->right) {
        return this->right;
    } else if (!this->parent) {
        return nullptr;
    } else {
        std::shared_ptr<TBinaryTreeItem<T>> tmp = this->parent;
        while (!tmp->right) {
            if (!tmp->parent) {
                return nullptr;
            }
            tmp = tmp->parent;
        }
        return tmp->right;
    }
}

template <class T>
void *TBinaryTreeItem<T>::operator new(size_t size)
{
    return btree_allocator.allocate();
}

template <class T>
void TBinaryTreeItem<T>::operator delete(void *ptr)
{
    btree_allocator.deallocate(ptr);
}

#include "figure.h"
template class TBinaryTreeItem<Figure>;