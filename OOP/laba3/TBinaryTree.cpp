#include "TBinaryTree.h"
#include "TBinaryTreeItem.h"
#include <cstdlib>
#include <iostream>


TBinaryTree::TBinaryTree() {
    head = nullptr;
}


std::shared_ptr<TBinaryTreeItem> TBinaryTree::find(std::shared_ptr<Figure> &obj)
{
    std::shared_ptr<TBinaryTreeItem> item = head;
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


std::shared_ptr<TBinaryTreeItem> TBinaryTree::minValueNode(std::shared_ptr<TBinaryTreeItem> root)
{
     std::shared_ptr<TBinaryTreeItem> min = root;
 
    while (min->left != nullptr) {
        min = min->left;
    }
 
    return min;
}

std::shared_ptr<TBinaryTreeItem> TBinaryTree::deleteNode(std::shared_ptr<TBinaryTreeItem> root, int32_t side)
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
            std::shared_ptr<TBinaryTreeItem> temp = root->right;
            root->left = nullptr;
            root->right = nullptr;
            return temp;
        } else if (root->right == nullptr) {
            std::shared_ptr<TBinaryTreeItem> temp = root->left;
            root->left = nullptr;
            root->right = nullptr;
            return temp;
        }
    
        std::shared_ptr<TBinaryTreeItem> temp = minValueNode(root->right);
        root->item = temp->item;
        root->right = deleteNode(root->right, temp->item->Side());
    }
    return root;
}


void TBinaryTree::remove(int32_t side)
{
    head = TBinaryTree::deleteNode(head, side);

}

void TBinaryTree::insert(std::shared_ptr<Figure> &obj)
{
    if (head == nullptr) {
        head = std::make_shared<TBinaryTreeItem>(obj);
        return;
    }


    std::shared_ptr<TBinaryTreeItem> item = head;
    while (true) {
        if (obj->Side() <= item->item->Side()) {
            if (item->left == nullptr) {
                item->left = std::make_shared<TBinaryTreeItem>(obj);
                break;
            } else {
                item = item->left;
            }
        } else {
            if (item->right == nullptr) {
                item->right = std::make_shared<TBinaryTreeItem>(obj);
                break;
            } else {
                item = item->right;
            }
        }
    }
}

void TBinaryTree::print_tree(std::shared_ptr<TBinaryTreeItem> item, int32_t a, std::ostream& os)
{
    for (int32_t i = 0; i < a; i++) {
        os << "  ";
    }
    item->GetFigure()->Print();
    if (item->left != nullptr) {
        TBinaryTree::print_tree(item->left, a + 1, os);
    } else if (item->right != nullptr) {
        for (int32_t i = 0; i <= a; i++) {
            os << "  ";
        }
        os << "null" << std::endl;
    }
    if (item->right != nullptr) {
        TBinaryTree::print_tree(item->right, a + 1, os);
    } else if (item->left != nullptr) {
        for (int32_t i = 0; i <= a; i++) {
            os << "  ";
        }
        os << "null" << std::endl;
    }
}



void TBinaryTree::print()
{
    if (head != nullptr) {
        TBinaryTree::print_tree(head, 0, std::cout);
    }
}

void TBinaryTree::print(std::ostream& os)
{
    if (head != nullptr) {
        TBinaryTree::print_tree(head, 0, os);
    }
}


bool TBinaryTree::empty()
{
    return head == nullptr;
}

TBinaryTree::~TBinaryTree()
{
}


std::ostream& operator<<(std::ostream& os, TBinaryTree& tree) {
    tree.print(os);
    return os;
}