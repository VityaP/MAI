#include <iostream>
#include <cstdlib>
#include <cstring>
#include "hexagon.h"
#include "TBinaryTree.h"

const int MAX_SIZE = 16;


int main(int argc, char** argv)
{
    TBinaryTree *tree = new TBinaryTree();
    char s[MAX_SIZE];

    std::cout << "Use 'help' or 'h' to get help." << std::endl;

    while (1) {
        std::cin.getline(s, MAX_SIZE);
        std::cin.clear();
        std::cin.sync();
        if (!strcmp(s, "insert") || !strcmp(s, "ins")) {
            int32_t side;
            if (!(std::cin >> side)) {
                std::cout << "Wrong value." << std::endl;
                continue;
            }
            tree->insert(Hexagon(side));
            //std::cout << "Vertex add." << std::endl;
        } else if (!strcmp(s, "remove") || !strcmp(s, "rm")) {
            int32_t side;
            if (!(std::cin >> side)) {
                std::cout << "Wrong value." << std::endl;
                continue;
            }
            tree->remove(side);
            //std::cout << "Vertex add." << std::endl;
        } else if (!strcmp(s, "find") || !strcmp(s, "f")) {
            if (tree->empty()) {
                std::cout << "Tree is empty." << std::endl;
                continue;
            }
            int32_t side;
            if (!(std::cin >> side)) {
                std::cout << "Wrong value." << std::endl;
                continue;
            }
            TBinaryTreeItem* rect = tree->find(side);
            if (rect != nullptr) {
                rect->GetHexagon().Print();
            } else {
                std::cout << "Hexagon not found." << std::endl;
            }
        } else if (!strcmp(s, "destroy") || !strcmp(s, "d")) {
            delete tree;
            tree = new TBinaryTree();
            std::cout << "Tree destroy." << std::endl;
        } else if (!strcmp(s, "print") || !strcmp(s, "pr")) {
            if (!tree->empty()) {
                std::cout << *tree << std::endl;
            } else {
                std::cout << "Tree is empty." << std::endl;
            }
        } else if (!strcmp(s, "quit") || !strcmp(s, "exit") || !strcmp(s, "q")) {
            delete tree;
            break;
        } else if (!strcmp(s, "help") || !strcmp(s, "h")) {
            std::cout << "Commands 'insert' and 'ins' create new hexagon in bintree." << std::endl;
            std::cout << "Commands 'remove' and 'rm' remove hexagon in bintree." << std::endl;
            std::cout << "Commands 'find' and 'f' find hexagon with your side." << std::endl;
            std::cout << "Commands 'destroy' and 'd' destroy bintree." << std::endl;
            std::cout << "Commands 'print' and 'pr' print bintree."  << std::endl;
            std::cout << "Commands 'quit' and 'q' exit the program." << std::endl;
        }
    }
    return 0;
}