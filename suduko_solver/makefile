genericdfs.a: genericdfs.c
		ar rc genericdfs.a genericdfs.c
		
sudukusolver: sudukusolver.c sudukutree.c genericdfs.c
		gcc -Wextra -Wall -Wvla sudukusolver.c genericdfs.c sudukutree.c -lm -o sudukusolver
		
all: genericdfs.a sudukusolver

clean:
		rm -f genericdfs.a sudukusolver
		rm -f *.o
		
.PHONY: clean all 
