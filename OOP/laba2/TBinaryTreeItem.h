#ifndef TBINARYTREEITEM
#define TBINARYTREEITEM

#include "hexagon.h"


class TBinaryTree;

class TBinaryTreeItem
{
public:
    TBinaryTreeItem();
    TBinaryTreeItem(Hexagon& hexagon);

    int32_t Side();
    Hexagon GetHexagon();
    ~TBinaryTreeItem();
    friend TBinaryTree;
private:
    Hexagon hexagon;
    TBinaryTreeItem* left;
    TBinaryTreeItem* right;
};

#endif