#!/bin/bash
#g++ TestGenerator.cpp -o genData
#time ./genData
g++ -std=c++17 main.cpp -g3 -o main
time ./main learn input.txt tree 20
time ./main classify tree answer inputOnly.txt