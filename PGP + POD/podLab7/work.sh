#!/bin/bash
mpic++ --std=c++11 -fopenmp -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -o src parallel.cpp
mpirun -np 1 --map-by node -mca btl ^openib ./src < input1.txt
mpirun -np 1 --map-by node -mca btl ^openib ./src < input2_1proc.txt
mpirun -np 2 --map-by node -mca btl ^openib ./src < input2.txt
rm src