#include <iostream>
#include <cstdlib>
#include <cstring>
#include <memory>

#include "figure.h"
#include "hexagon.h"
#include "octagon.h"
#include "triangle.h"
#include "TBinaryTree.h"

void menu()
{
    std::cout << "Список операций:" << std::endl;
    std::cout << "1) Добавление треугольника" << std::endl;
    std::cout << "2) Добавление восьмиугольник" << std::endl;
    std::cout << "3) Добавление шестиугольника" << std::endl;
    std::cout << "4) Удаление фигуры из дерева по максимальной стороне" << std::endl;
    std::cout << "5) Печать дерева"  << std::endl;
    std::cout << "0) Выход" << std::endl;
}

int main(void)
{
    int act = 0;
    TBinaryTree<Figure> tree;
    std::shared_ptr<Figure> ptr;
    std::cout << "Чтобы вызвать меню, нажмите 6" << std::endl;
    do {
        std::cin >> act;
        switch(act) {
            case 1:
                ptr = std::make_shared<Triangle>(std::cin);
                tree.insert(ptr);
                break;
            case 2:
                ptr = std::make_shared<Octagon>(std::cin);
                tree.insert(ptr);
                break;
            case 3:
                ptr = std::make_shared<Hexagon>(std::cin);
                tree.insert(ptr);
                break;
            case 4:
                
                if (tree.empty()) {
                    std::cout << "Дерево пусто" << std::endl;
                } else {
                    int32_t numb;
                    std::cin >> numb;
                    tree.remove(numb);
                }
                break;
            case 5:
                if (tree.empty()) {
                    std::cout << "Дерево пусто" << std::endl;
                } else {
                    std::cout << tree << std::endl;
                }
                break;
            case 6:
                menu();
                break;
            case 0:
                break;
            default:
                std::cout << "Такой команды не существует" << std::endl;;
                break;
        }
    } while (act);
    return 0;
}