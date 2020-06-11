#include "TTree.h"
#include "TBinaryTree.h"

template <class Q, class O>
TTree<Q, O>::TTree() {
    root = std::make_shared<Node>(Node());
    root->son = std::make_shared<Node>(Node());
}

template <class Q, class O>
TTree<Q, O>::Node::Node()
{
    son = sibling = nullptr;
    itemsInNode = 0;
}

template <class Q, class O>
TTree<Q, O>::Node::Node(O& item)
{
    data.insert(item);
    itemsInNode = 1;
}

template <class Q, class O>
void TTree<Q, O>::recRemByType(std::shared_ptr<Node>& node, int& type)
{
    if (node->itemsInNode) {
        for (auto i : node->data) {
            if (i->type() == type) {
                node->data.remove(i->Side());
                node->itemsInNode--;
            }
        }
        if (node->sibling) {
            recRemByType(node->sibling, type);
        }
        if (node->son) {
            recRemByType(node->son, type);
        }
    }
}

template <class Q, class O>
void TTree<Q, O>::removeByType(int& type)
{
    recRemByType(root->son, type);
}

template <class Q, class O>
void TTree<Q, O>::recInsert(std::shared_ptr<Node>& node, O& item)
{
    if (node->itemsInNode < 5) {
        node->data.insert(item);
        node->itemsInNode++;
    } else {
        auto sibl = node;
    
        for (int i = 0; i < 3; i++) {
            if (!sibl->sibling) {
                sibl->sibling = std::make_shared<Node>(Node(item));
                return;
            }
            if (sibl->sibling->itemsInNode < 5) {
                recInsert(sibl->sibling, item);
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

template <class Q, class O>
void TTree<Q, O>::insert(O& item)
{
    recInsert(root->son, item);
}

template <class Q, class O>
void TTree<Q, O>::recInorder(std::shared_ptr<Node>& node)
{
    if (node->itemsInNode) {
        for (auto i: node->data) {
            i->Print();
        }
        std::cout << "\n";
        if (node->sibling) {
            recInorder(node->sibling);
        }
        if (node->son) {
            recInorder(node->son);
        }
    }
}

template <class Q, class O>
void TTree<Q, O>::inorder()
{
    if (root->son->son || root->son->sibling) { 
        clear(root->son, root);
    }
    recInorder(root->son);
}

template <class Q, class O>
void TTree<Q, O>::recRemLesser(std::shared_ptr<Node>& node, int32_t& side)
{
    if (node->itemsInNode) {
        for (auto i : node->data) {
            if (i->Side() == side) {
                node->data.remove(i->Side());
                node->itemsInNode--;
            }
        }
        if (node->sibling) {
            recRemLesser(node->sibling, side);
        }
        if (node->son) {
            recRemLesser(node->son, side);
        }
    }
}

template <class Q, class O>
void TTree<Q, O>::removeLesser(int32_t& side)
{
    recRemLesser(root->son, side);
}

template <class Q, class O>
void TTree<Q, O>::clear(std::shared_ptr<Node>& node, std::shared_ptr<Node>& parent)
{
    if (node) {
        if (!node->itemsInNode) {
            auto orphan = node;
            auto orphanPar = parent;
            if (node->sibling) {
                orphan = node->sibling;
                orphanPar = node;
            } else if (node->son) {
                orphan = node->sibling;
                orphanPar = node;
            }
            while (orphan->sibling || orphan->son) {
                orphanPar = orphan;
                if (orphan->sibling) {
                    orphan = orphan->sibling;
                } else if (orphan->son) {
                    orphan = orphan->son;
                }
            }
            if (orphanPar->sibling == orphan) {
                std::swap(node->data, orphan->data);
                node->itemsInNode = orphan->itemsInNode;
                orphanPar->sibling = nullptr;
            } else if (orphanPar->son == orphan) {
                std::swap(node->data, orphan->data);
                node->itemsInNode = orphan->itemsInNode;
                orphanPar->son = nullptr;
            }
        }
    }
    if (node) {
        if (node->son) {
            clear(node->son, node);
        }
        if (node->sibling) {
            clear(node->sibling, node);
        }
    }
}

template <class Q, class O>
void TTree<Q, O>::GG(std::shared_ptr<Node>& node)
{
    if (node->itemsInNode) {
        node->data.gg();
        if (node->sibling) {
            GG(node->sibling);
        }
        if (node->son) {
            GG(node->son);
        }
    }
}

template <class Q, class O>
void TTree<Q, O>::RM()
{
    GG(root->son);
}

#include "TBinaryTree.h"
template class TTree<TBinaryTree<Figure>, std::shared_ptr<Figure>>;