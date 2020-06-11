#ifndef TBINARYTREEITEM
#define TBINARYTREEITEM

#include <memory>
#include "hexagon.h"
#include "triangle.h"
#include "octagon.h"


class TBinaryTree;

class TBinaryTreeItem
{
public:
    TBinaryTreeItem();
    TBinaryTreeItem(const std::shared_ptr<Figure> &obj);

    std::shared_ptr<Figure> GetFigure();
    ~TBinaryTreeItem();
    friend TBinaryTree;
private:
    std::shared_ptr<Figure> item;
    std::shared_ptr<TBinaryTreeItem> left;
    std::shared_ptr<TBinaryTreeItem> right;

};

#endif