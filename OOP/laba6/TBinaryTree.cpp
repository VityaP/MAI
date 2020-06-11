#include "TBinaryTree.h"

template <class T>
TBinaryTree<T>::TBinaryTree()
{
    head = nullptr;
}

template <class T>
std::shared_ptr<TBinaryTreeItem<T>> TBinaryTree<T>::find(std::shared_ptr<T> &obj)
{
    std::shared_ptr<TBinaryTreeItem<T>> item = head;
    while (item != nullptr) {
        if (item->item->Side() == obj->Side()) {
            return item;
        } else if (item->item->Side() > obj->Side()) {
            item = item->left;
        } else if (item->item->Side() < obj->Side()) {
            item = item->right;
        }
    }
    return nullptr;
}

template <class T>
std::shared_ptr<TBinaryTreeItem<T>> TBinaryTree<T>::minValueNode(std::shared_ptr<TBinaryTreeItem<T>> root)
{
     std::shared_ptr<TBinaryTreeItem<T>> min = root;
 
    while (min->left != nullptr) {
        min = min->left;
    }
 
    return min;
}

template <class T>
std::shared_ptr<TBinaryTreeItem<T>> TBinaryTree<T>::deleteNode(std::shared_ptr<TBinaryTreeItem<T>> root, int32_t side)
{
    if (root == nullptr) {
        return root;
    }
 
    if (side < root->item->Side()) {
        root->left = deleteNode(root->left, side);
    } else if (side > root->item->Side()) {
        root->right = deleteNode(root->right, side);
    } else {
        if (root->left == nullptr) {
            std::shared_ptr<TBinaryTreeItem<T>> temp = root->right;
            root->left = nullptr;
            root->right = nullptr;
            root->parent = nullptr;
            return temp;
        } else if (root->right == nullptr) {
            std::shared_ptr<TBinaryTreeItem<T>> temp = root->left;
            root->left = nullptr;
            root->right = nullptr;
            root->parent = nullptr;
            return temp;
        }
    
        std::shared_ptr<TBinaryTreeItem<T>> temp = minValueNode(root->right);
        root->item = temp->item;
        root->right = deleteNode(root->right, temp->item->Side());
    }
    return root;
}

template <class T>
void TBinaryTree<T>::remove(int32_t side)
{
    head = TBinaryTree<T>::deleteNode(head, side);

}

template <class T>
void TBinaryTree<T>::insert(std::shared_ptr<T> &obj)
{
    if (head == nullptr) {
        head = std::make_shared<TBinaryTreeItem<T>>(obj);
        return;
    }


    std::shared_ptr<TBinaryTreeItem<T>> item = head;
    while (true) {
        if (obj->Side() <= item->item->Side()) {
            if (item->left == nullptr) {
                item->left = std::make_shared<TBinaryTreeItem<T>>(obj);
                break;
            } else {
                item = item->left;
            }
        } else {
            if (item->right == nullptr) {
                item->right = std::make_shared<TBinaryTreeItem<T>>(obj);
                break;
            } else {
                item = item->right;
            }
        }
    }
}

template <class T>
void TBinaryTree<T>::print_tree(std::shared_ptr<TBinaryTreeItem<T>> item, int32_t a, std::ostream& os)
{
    for (int32_t i = 0; i < a; i++) {
        os << "  ";
    }
    item->GetFigure()->Print();
    if (item->left != nullptr) {
        TBinaryTree<T>::print_tree(item->left, a + 1, os);
    } else if (item->right != nullptr) {
        for (int32_t i = 0; i <= a; i++) {
            os << "  ";
        }
        os << "null" << std::endl;
    }
    if (item->right != nullptr) {
        TBinaryTree<T>::print_tree(item->right, a + 1, os);
    } else if (item->left != nullptr) {
        for (int32_t i = 0; i <= a; i++) {
            os << "  ";
        }
        os << "null" << std::endl;
    }
}


template <class T>
void TBinaryTree<T>::print()
{
    if (head != nullptr) {
        TBinaryTree<T>::print_tree(head, 0, std::cout);
    }
}

template <class T>
void TBinaryTree<T>::print(std::ostream& os)
{
    if (head != nullptr) {
        TBinaryTree<T>::print_tree(head, 0, os);
    }
}

template <class T>
bool TBinaryTree<T>::empty()
{
    return head == nullptr;
}

template <class T>
TBinaryTree<T>::~TBinaryTree()
{
}

template <class T>
std::ostream& operator<<(std::ostream& os, TBinaryTree<T> &tree)
{
    tree.print(os);
    return os;
}

template <class T>
TIterator<TBinaryTreeItem<T>,T> TBinaryTree<T>::begin()
{
    return TIterator<TBinaryTreeItem<T>,T>(head);
}

template <class T>
TIterator<TBinaryTreeItem<T>,T> TBinaryTree<T>::end()
{
    std::shared_ptr<TBinaryTreeItem<T>> tmp = head;
    while (tmp->right) {
        tmp = tmp->right;
    }
    return TIterator<TBinaryTreeItem<T>,T>(tmp);
}

#include "figure.h"
template class TBinaryTree<Figure>;
template std::ostream& operator<<(std::ostream& os, TBinaryTree<Figure> &tree);