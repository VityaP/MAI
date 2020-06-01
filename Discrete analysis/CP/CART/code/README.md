# CART
Classification and Regression Tree

*n и m находятся в файле TestGenerator.cpp*

**Пример использования в автоматическом режиме**

chmod +x ./work.sh

./work.sh

**Пример использования в ручном режиме**

g++ TestGenerator.cpp

./a.out

make

./main learn dataWithValue tree 10

./main classify tree answer data

**Benchmark:**

| n 	| m 	| time 	  | memory    |
|-------|-------|---------|-----------|
| 10^2  | 10^5  |         | 2 x 80 mb |
| 10^3  | 10^4  |         | 2 x 80 mb |
| 10^4  | 10^3  |         | 2 x 80 mb |
| 10^5  | 10^2  |         | 2 x 80 mb |

