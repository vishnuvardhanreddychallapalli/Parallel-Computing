CFLAGS=-O3 -std=c11 -fPIC -g -fopenmp
CXXFLAGS=-O3 -std=c++11 -fPIC -g -fopenmp
LDFLAGS=-fopenmp
ARCHIVES=../libgen.a 
LD=g++


all: bubblesort 


bubblesort: bubblesort.o
	$(LD) $(LDFLAGS) bubblesort.o $(ARCHIVES) -o bubblesort

test: bubblesort
	./test.sh

bench: bubblesort
	./queue.sh

plot:  
	./plot.sh

clean:
	-rm *.o
	-rm bubblesort

distclean:
	-rm *.sh.*


.PHONY:  bubblesort
