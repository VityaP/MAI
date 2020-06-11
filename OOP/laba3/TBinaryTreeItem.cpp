#include "TBinaryTreeItem.h"


TBinaryTreeItem::TBinaryTreeItem()
{
    left = nullptr;
    right = nullptr;
}
TBinaryTreeItem::TBinaryTreeItem(const std::shared_ptr<Figure> &obj)
{
    this->item = obj;
    left = nullptr;
    right = nullptr;
}

TBinaryTreeItem::~TBinaryTreeItem()
{
}

std::shared_ptr<Figure> TBinaryTreeItem::GetFigure()
{
    return this->item;
}