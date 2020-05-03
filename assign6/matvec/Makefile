CXX=mpicxx
CC=mpicc
LD=mpicxx
CXXFLAGS=-std=c++11

mpi_matmul:


test: mpi_matmul
	./test.sh

bench: mpi_matmul
	./queue.sh

plot:
	./plot.sh

clean:
	-rm *.o
	-rm mpi_matmul

distclean:
	rm *.sh.*

.PHONY: test plot
