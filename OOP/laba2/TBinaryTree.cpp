#include "TBinaryTree.h"
#include "TBinaryTreeItem.h"
#include <cstdlib>
#include <iostream>


TBinaryTree::TBinaryTree() {
    head = nullptr;
}


TBinaryTreeItem* TBinaryTree::find(int32_t a)
{
    TBinaryTreeItem* item = head;
    while (item != nullptr) {
        if (item->Side() == a) {
            return item;
        } else if (item->Side() > a) {
            item = item->left;
        } else if (item->Side() < a) {
            item = item->right;
        }
    }
    return nullptr;
}


TBinaryTreeItem* TBinaryTree::minValueNode(TBinaryTreeItem* root)
{
    TBinaryTreeItem* min = root;
 
    while (min->left != nullptr) {
        min = min->left;
    }
 
    return min;
}

TBinaryTreeItem* TBinaryTree::deleteNode(TBinaryTreeItem* root, int32_t a)
{
    if (root == nullptr) {
        return root;
    }
 
    if (a < root->Side()) {
        root->left = deleteNode(root->left, a);
    } else if (a > root->Side()) {
        root->right = deleteNode(root->right, a);
    } else {
        if (root->left == nullptr) {
            TBinaryTreeItem *temp = root->right;
            root->left = nullptr;
            root->right = nullptr;
            delete root;
            return temp;
        } else if (root->right == nullptr) {
            TBinaryTreeItem *temp = root->left;
            root->left = nullptr;
            root->right = nullptr;
            delete root;
            return temp;
        }
 
        TBinaryTreeItem* temp = minValueNode(root->right);
        root->hexagon = temp->hexagon;
        root->right = deleteNode(root->right, temp->Side());
    }
    return root;
}


void TBinaryTree::remove(int32_t a)
{
    head = TBinaryTree::deleteNode(head, a);

}

void TBinaryTree::insert(Hexagon &&hexagon)
{
    if (head == nullptr) {
        head = new TBinaryTreeItem(hexagon);
        return;
    }


    TBinaryTreeItem* item = head;
    while (true) {
        if (hexagon.Side() <= item->Side()) {
            if (item->left == nullptr) {
                item->left = new TBinaryTreeItem(hexagon);
                break;
            } else {
                item = item->left;
            }
        } else {
            if (item->right == nullptr) {
                item->right = new TBinaryTreeItem(hexagon);
                break;
            } else {
                item = item->right;
            }
        }
    }
}

void TBinaryTree::print_tree(TBinaryTreeItem* item, int32_t a, std::ostream& os)
{
    for (int32_t i = 0; i < a; i++) {
        os << "  ";
    }
    os << item->Side() << std::endl;
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
    delete head;
}


std::ostream& operator<<(std::ostream& os, TBinaryTree& tree) {
    tree.print(os);
    return os;
}