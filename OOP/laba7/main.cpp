#include <iostream>
#include <cstdlib>
#include <cstring>
#include <memory>

#include "TBinaryTree.h"
#include "figure.h"
#include "hexagon.h"
#include "octagon.h"
#include "triangle.h"
#include "TTree.h"

void menu()
{
    std::cout << "Список операций:" << std::endl;
    std::cout << "1) Добавление треугольника" << std::endl;
    std::cout << "2) Добавление восьмиугольник" << std::endl;
    std::cout << "3) Добавление шестиугольника" << std::endl;
    std::cout << "4) Удаление всех фигур одного типа из дерева. 1)Треугольник 2)Восьмиугольник 3)Шестиугольник" << std::endl;
    std::cout << "5) Удаление фигуры из дерева по максимальной стороне" << std::endl;
    std::cout << "6) Печать дерева"  << std::endl;
    std::cout << "0) Выход" << std::endl;
}

int main(void)
{
    int act = 0, index;
    TTree<TBinaryTree<Figure>, std::shared_ptr<Figure>> tree;
    std::shared_ptr<Figure> ptr;
    std::cout << "Чтобы вызвать меню, нажмите 7" << std::endl;
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
                std::cin >> index;
                tree.removeByType(index);
                break;
            case 5:
                int32_t numb;
                std::cin >> numb;
                tree.removeLesser(numb);
                break;
            case 6:
                tree.inorder();
                break;
            case 7:
                menu();
                break;
            case 0:
                tree.RM();
                break;
            default:
                std::cout << "Такой команды не существует" << std::endl;;
                break;
        }
    } while (act);
    return 0;
}   