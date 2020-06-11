#include "TTree.h"
#include <functional>

template <typename Q>
TTree<Q>::TTree(const unsigned int& n)
{
    root = std::make_shared<Node>(Node());
    root->son = std::make_shared<Node>(Node());
    N = n;
}

template <typename Q>
TTree<Q>::Node::Node()
{
    son = sibling = nullptr;
}

template <typename Q>
TTree<Q>::Node::Node(std::shared_ptr<Q> item)
{
    data = item;
}

template <typename Q>
void TTree<Q>::recInsert(std::shared_ptr<Node>& node, std::shared_ptr<Q> item)
{
    if (!node->data) {
        node->data = item;
    } else {
        auto sibl = node;
    
        for (int i = 0; i < N; i++) {
            if (!sibl->sibling) {
                sibl->sibling = std::make_shared<Node>(Node(item));
                return;
            }
            sibl = sibl->sibling;
        }
        if (node->son) {
            recInsert(node->son, item);
        } else {
            node->son = std::make_shared<Node>(Node(item));
        }
  }
}

template <typename Q>
void TTree<Q>::insert(std::shared_ptr<Q> item)
{
    recInsert(root->son, item);
}

template <typename Q>
void TTree<Q>::recInorder(std::shared_ptr<Node>& node)
{
    if (node) {
        auto future = std::async(std::launch::async, *(node->data));
        future.get();
    
        if (node->sibling) {
            recInorder(node->sibling);
        }
        if (node->son) {
            recInorder(node->son);
        }
    }
}

template <typename Q>
void TTree<Q>::inorder()
{
    recInorder(root->son);
}

#include <functional>
template class TTree<std::function<void(void)>>;