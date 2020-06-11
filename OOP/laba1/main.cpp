#include <iostream>
#include <cstdlib>
//#include <cstring>
#include "triangle.h"
#include "hexagon.h"
#include "octagon.h"



int main(int argc, char** argv)
{
	const int max_size = 16;
    char s[max_size];
    std::cout << "Use 'help' or 'h' to get help." << std::endl;
    Triangle *ptr_tr = nullptr;
    Hexagon *ptr_hex = nullptr;
    Octagon *ptr_oct = nullptr;
    while (1) {
        std::cin.getline(s, max_size);
        std::cin.clear();
        std::cin.sync();
        if (!strcmp(s, "create_triangle") || !strcmp(s, "cr_tr")) {
            if (ptr_tr != nullptr)
            	delete ptr_tr;
            std::cout << "Enter two legs and hypotenuse" << std::endl;
            ptr_tr = new Triangle(std::cin);
        } else if (!strcmp(s, "create_hex") || !strcmp(s, "cr_hex")) {
            if (ptr_hex != nullptr)
            	delete ptr_hex;
            std::cout << "Enter side." << std::endl;
            ptr_hex = new Hexagon(std::cin);
        } else if (!strcmp(s, "create_octagon") || !strcmp(s, "cr_oct")) {
            if (ptr_oct != nullptr)
            	delete ptr_oct;
            std::cout << "Enter side." << std::endl;
            ptr_oct = new Octagon(std::cin);
        } else if (!strcmp(s, "print_triagle") || !strcmp(s, "pr_tr")) {
            if(ptr_tr == nullptr)
                std::cout << "Triangle doesn't exist." << std::endl;
            else
                ptr_tr->Print();
        } else if (!strcmp(s, "print_hexagon") || !strcmp(s, "pr_hex")) {
            if(ptr_hex == nullptr)
                std::cout << "Hexagon doesn't exist." << std::endl;
            else
                ptr_hex->Print();
        } else if (!strcmp(s, "print_octagon") || !strcmp(s, "pr_oct")) {
            if(ptr_oct == nullptr)
                std::cout << "Octagon doesn't exist." << std::endl;
            else
                ptr_oct->Print();
        } else if (!strcmp(s, "square_triagle") || !strcmp(s, "sq_tr")) {
            if(ptr_tr == nullptr)
                std::cout << "Triangle doesn't exist." << std::endl;
            else
                std::cout << "Square: " << ptr_tr->Square() << std::endl;
        } else if (!strcmp(s, "square_hexagon") || !strcmp(s, "sq_hex")) {
            if(ptr_hex == nullptr)
                std::cout << "Hexagon doesn't exist." << std::endl;
            else
                std::cout << "Square: " << ptr_hex->Square() << std::endl;
        } else if (!strcmp(s, "square_octagon") || !strcmp(s, "sq_oct")) {
            if(ptr_oct == nullptr)
                std::cout << "Octagon doesn't exist." << std::endl;
            else
                std::cout << "Square: " << ptr_oct->Square() << std::endl;
        } else if (!strcmp(s, "quit") || !strcmp(s, "exit") || !strcmp(s, "q")) {
            if (ptr_tr != nullptr) {
                delete ptr_tr;
            }
            if (ptr_hex != nullptr) {
                delete ptr_hex;
            }
            if (ptr_oct != nullptr) {
                delete ptr_oct;
            }
            break;
        } else if (!strcmp(s, "help") || !strcmp(s, "h")) {
            std::cout << "Commands 'create_triangle' and 'cr_tr' create new triagle with your parameters." << std::endl;
            std::cout << "Commands 'create_hexagon' and 'cr_hex' create new hexagon with your parameters." << std::endl;
            std::cout << "Commands 'create_octagon' and 'cr_oct' create new octagon with your parameters." << std::endl;
            std::cout << "Commands 'print_triangle' and 'pr_tr' output parameters of triagle." << std::endl;
            std::cout << "Commands 'print_hexagon' and 'pr_hex' output parameters of hexagon."  << std::endl;
            std::cout << "Commands 'print_octagon' and 'pr_oct' output parameters of octagon."  << std::endl;
            std::cout << "Commands 'square_triangle' and 'sq_tr' output square of triagle." << std::endl;
            std::cout << "Commands 'square_hexagon' and 'sq_hex' output square of hexagon." << std::endl;
            std::cout << "Commands 'square_octagon' and 'sq_oct' output square of octagon." << std::endl;
            std::cout << "Commands 'quit' and 'q' exit the program." << std::endl;
        }
    }
    return 0;
}