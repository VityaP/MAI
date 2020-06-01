da6 : main.o bigint.o
	g++ -Wall -pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -o da6 main.o bigint.o
main.o : main.cpp bigint.hpp
	g++ -std=c++11 -Wall -c main.cpp
bigint.o : bigint.cpp bigint.hpp
	g++ -std=c++11 -Wall -c bigint.cpp
clear :
	rm *.o da6