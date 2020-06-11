#include "TBinaryTreeItem.h"


TBinaryTreeItem::TBinaryTreeItem()
{
    left = nullptr;
    right = nullptr;
}
TBinaryTreeItem::TBinaryTreeItem(Hexagon& hexagon)
{
    this->hexagon = hexagon;
    left = nullptr;
    right = nullptr;
}


int32_t TBinaryTreeItem::Side()
{
    return hexagon.Side();
}

TBinaryTreeItem::~TBinaryTreeItem()
{
    delete left;
    delete right;
}

Hexagon TBinaryTreeItem::GetHexagon() {
    return hexagon;
}