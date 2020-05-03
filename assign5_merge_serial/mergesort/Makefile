CFLAGS=-O3 -std=c11 -fPIC -g -fopenmp
CXXFLAGS=-O3 -std=c++11 -fPIC -g -fopenmp
LDFLAGS=-fopenmp
ARCHIVES=../libgen.a 
LD=g++


all: mergesort 


mergesort: mergesort.o
	$(LD) $(LDFLAGS) mergesort.o $(ARCHIVES) -o mergesort

test: mergesort
	./test.sh

bench: mergesort
	./queue.sh

plot:  
	./plot.sh

clean:
	-rm *.o
	-rm mergesort

distclean:
	-rm *.sh.*


.PHONY:  mergesort
