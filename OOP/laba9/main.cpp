#include <iostream>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <random>
#include <functional>

#include "TBinaryTree.h"
#include "figure.h"
#include "hexagon.h"
#include "octagon.h"
#include "triangle.h"
#include "TTree.h"

using namespace std;

int main(void) {
  TBinaryTree<Figure> ttree;
  typedef std::function<void(void)> command;
  TTree<command> tree(4);
  command cmdInsert = [&]() {
    std::cout << "Command: Insert" << std::endl;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1, 10);
    for (int i = 0; i < 10; i++) {
      int side = distribution(generator);
      if ((side % 2) == 0) {
        std::shared_ptr<Figure> ptr = std::make_shared<Triangle>(Triangle(side, side, side));
        if (ttree.find(ptr) == nullptr) {
            ttree.insert(ptr);
        }
      } else if((side % 3) == 0) {
        std::shared_ptr<Figure> ptr = std::make_shared<Octagon>(Octagon(side));
        if (ttree.find(ptr) == nullptr) {
            ttree.insert(ptr);
        }
      } else {
        std::shared_ptr<Figure> ptr = std::make_shared<Hexagon>(Hexagon(side));
        if (ttree.find(ptr) == nullptr) {
            ttree.insert(ptr);
        }
      }
    }
  };
  command cmdPrint = [&]() {
    std::cout << "Command: Print" << std::endl;
    for (auto i : ttree) {
      i->Print();
    }
  };
  command cmdRemove = [&]() {
    std::cout << "Command: Remove" << std::endl;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1, 10);
    int side = distribution(generator);
    std::cout << "Lesser than " << side << std::endl;
    for (int i = 0; i < 10; i++) {
      
      for (auto iter: ttree) {
        if (iter->Side() < side) {
          ttree.remove(iter->Side());
          break;
        }
      }
    }
  };
  
  tree.insert(std::shared_ptr<command>(&cmdInsert, [](command*){}));
  tree.insert(std::shared_ptr<command>(&cmdPrint, [](command*){}));
  tree.insert(std::shared_ptr<command>(&cmdRemove, [](command*){}));
  tree.insert(std::shared_ptr<command>(&cmdPrint, [](command*){}));              
  tree.inorder();
  
  return 0;
}