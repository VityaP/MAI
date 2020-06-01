CC = gcc

all: run

run: libarray.so VITYAmainStat.o VITYAmainDyn.o ;$(CC)  -o run-stat VITYAmainStat.o -L. -larray -Wl,-rpath,. ; $(CC)  -o run-dyn VITYAmainDyn.o -ldl -L. -larray -Wl,-rpath,.

VITYAmainStat.o: VITYAmainStat.c ; $(CC) -c  VITYAmainStat.c

VITYAmainDyn.o: VITYAmainDyn.c ; $(CC) -c  VITYAmainDyn.c

array.o: array.c ; $(CC) -c -fPIC  array.c

libarray.so: array.o ; $(CC)  -shared -o libarray.so array.o

clean: ;rm -f *.o run-stat run-dyn *.so