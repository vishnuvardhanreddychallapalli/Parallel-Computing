CFLAGS=-O3 -std=c11 -fPIC -g -fopenmp
CXXFLAGS=-O3 -std=c++11 -fPIC -g -fopenmp
LDFLAGS=-fopenmp
ARCHIVES=../libgen.a 
LD=g++


all: reduce 

reduce: reduce.o 
	$(LD) $(LDFLAGS) reduce.o $(ARCHIVES) -o reduce

test: reduce ../approx
	./test.sh

bench: reduce
	./queue.sh

plot: 
	./plot.sh

clean:
	-rm *.o
	-rm reduce

distclean:
	-rm *.sh.*


.PHONY: reduce 
