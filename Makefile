CC = g++
Option = -O3


main : main.o cell_int2d.o input.o
	$(CC) $(Option) main.o cell_int2d.o input.o -o int_cic

main.o : main.cpp 
	$(CC) $(Option) -c -o main.o main.cpp

cell_int2d.o : cell_int2d.cpp cell_int2d.h
	$(CC) $(Option) -c -o cell_int2d.o cell_int2d.cpp

input.o : input.cpp input.h
	$(CC) $(Option) -c -o input.o input.cpp

