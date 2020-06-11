#include "TBinaryTreeItem.h"

template <class T>
TBinaryTreeItem<T>::TBinaryTreeItem()
{
    item = nullptr;
    left = nullptr;
    right = nullptr;
    parent = nullptr;
}

template <class T>
TBinaryTreeItem<T>::TBinaryTreeItem(const std::shared_ptr<T> obj, std::shared_ptr<TBinaryTreeItem<T>> l, std::shared_ptr<TBinaryTreeItem<T>> r, std::shared_ptr<TBinaryTreeItem<T>> p)
{
    item = obj;
    left = l;
    right = r;
    parent = p;
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

template <class T>
std::shared_ptr<TBinaryTreeItem<T>> TBinaryTreeItem<T>::GetNext()
{
    if (this->right) {
        std::shared_ptr<TBinaryTreeItem<T>> temp = this->right;
        while (temp->left) {
            temp = temp->left;
        }
        return temp;
    }

    std::shared_ptr<TBinaryTreeItem<T>> succ = nullptr;
    std::shared_ptr<TBinaryTreeItem<T>> n = std::make_shared<TBinaryTreeItem<T>>(*this);
    std::shared_ptr<TBinaryTreeItem<T>> root = std::make_shared<TBinaryTreeItem<T>>(*this);
    while (root->parent) {
        root = root->parent;
    }
    

    while (root) {
        if (n->item->Side() < root->item->Side()) {
            succ = root;
            root = root->left;
        } else if (n->item->Side() > root->item->Side()) {
            root = root->right;
        } else {
            break;
        }
    }
    return succ;
}

#include "figure.h"
template class TBinaryTreeItem<Figure>;
//template std::ostream& operator<<(std::ostream &out, TBinaryTreeItem<Figure> &obj);