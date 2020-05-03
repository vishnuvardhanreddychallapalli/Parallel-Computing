CXX=mpicxx
CC=mpicc
LD=mpicxx
CXXFLAGS=-std=c++11
LDLIBS=libfunctions.a

mpi_num_int:


bench: mpi_num_int
	./queue.sh

plot:
	./plot.sh

test: mpi_num_int ../approx
	./test.sh

clean:
	-rm *.o
	-rm mpi_num_int

distclean:
	-rm *.sh.*

.PHONY: test plot
