#!/bin/bash
g++ generate.cpp -o genData
./genData
python3 ./MD5.py
g++ Analyze.cpp -o countData
./countData