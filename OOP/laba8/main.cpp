#include <iostream>
#include <cstdlib>
#include <cstring>
#include <memory>

#include "figure.h"
#include "hexagon.h"
#include "octagon.h"
#include "triangle.h"
#include "TBinaryTree.h"

int Sort(std::shared_ptr<Figure> *&arr, int l, int r)
{
    int x = l + (r - l) / 2;
    int i = l;
    int j = r;

    while(i <= j) {
        while(arr[i]->Side() < arr[x]->Side()) {
            i++;
        }
        while(arr[j]->Side() > arr[x]->Side()) {
            j--;
        }
        if(i <= j) {
            std::shared_ptr<Figure> tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
    }
    if (i < r) { 
        std::packaged_task<int(std::shared_ptr<Figure> *&, int,int)> task(Sort);
        auto result = task.get_future();
     
        std::thread task_td(std::move(task), std::ref(arr), i, r);
        task_td.join();
        result.get();
    } 
    if (l < j) { 
        std::packaged_task<int(std::shared_ptr<Figure> *&, int,int)> task(Sort);
        auto result = task.get_future();
     
        std::thread task_td(std::move(task), std::ref(arr), l, j);
        task_td.join();
        result.get();
    }
    return 0;
}

void menu()
{
    std::cout << "Список операций:" << std::endl;
    std::cout << "1) Добавление треугольника" << std::endl;
    std::cout << "2) Добавление восьмиугольник" << std::endl;
    std::cout << "3) Добавление шестиугольника" << std::endl;
    std::cout << "4) Удаление фигуры из дерева по максимальной стороне" << std::endl;
    std::cout << "5) Печать дерева"  << std::endl;
    std::cout << "6) Печать дерева c итератором"  << std::endl;
    std::cout << "7) Балансировка бинарного дерева"  << std::endl;
    std::cout << "8) Балансировка бинарного дерева c помощью параллельной сортировки"  << std::endl;
    std::cout << "0) Выход" << std::endl;
}

int main(void)
{
    int act = 0;
    TBinaryTree<Figure> tree;
    std::shared_ptr<Figure> ptr;
    std::shared_ptr<Figure> *arr;
    arr = new std::shared_ptr<Figure> [20];
    int n;
    std::cout << "Чтобы вызвать меню, нажмите 9" << std::endl;
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
                if (tree.empty()) {
                    std::cout << "Дерево пусто" << std::endl;
                } else {
                    for(auto i : tree) {
                        i->Print();
                    }
                    std::cout<<std::endl;
                }
                break;
            case 7:
                tree.Clear();
                std::cout << "Введите количество вершин" << std::endl;
                std::cin >> n;
                for (int i = 0; i < n; ++i) {
                    std::cout << "Осталось ввести " << n - i << " вершин(у)" << std::endl;
                    ptr = std::make_shared<Hexagon>(std::cin);
                    arr[i] = ptr;
                }
                tree.MySort(arr, 0, n - 1);
                tree.Balanced(arr, 0, n - 1);
                break;
            case 8:
                tree.Clear();
                std::cout << "Введите количество вершин" << std::endl;
                std::cin >> n;
                for (int i = 0; i < n; ++i) {
                    std::cout << "Осталось ввести " << n - i << " вершин(у)" << std::endl;
                    ptr = std::make_shared<Hexagon>(std::cin);
                    arr[i] = ptr;
                }
                Sort(arr, 0, n - 1);
                //tree.MySort(arr, 0, n - 1);
                tree.Balanced(arr, 0, n - 1);
                break;
            case 9:
                menu();
                break;
            case 0:
                tree.gg();
                break;
            default:
                std::cout << "Такой команды не существует" << std::endl;;
                break;
        }
    } while (act);
    delete[] arr;
    return 0;
}