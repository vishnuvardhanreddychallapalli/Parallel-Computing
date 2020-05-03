CFLAGS=-O3 -std=c11 -fPIC -g -fopenmp
CXXFLAGS=-O3 -std=c++11 -fPIC -g -fopenmp
LDFLAGS=-fopenmp
ARCHIVES=../libintegrate.a ../libfunctions.a ../libgen.a 
LD=g++


all: prefixsum

prefixsum: prefixsum.o
	$(LD) $(LDFLAGS) prefixsum.o $(ARCHIVES) -o prefixsum

test: prefixsum
	./test.sh

bench: prefixsum
	./queue.sh

plot: 
	./plot.sh

clean:
	-rm *.o
	-rm prefixsum

distclean:
	-rm *.sh.*

.PHONY: prefixsum 
