CFLAGS=-O3 -std=c11 -fPIC -g -fopenmp
CXXFLAGS=-O3 -std=c++11 -fPIC -g -fopenmp
LDFLAGS=-fopenmp
ARCHIVES=../libgen.a 
LD=g++

all: lcs

lcs: lcs.o
	$(LD) $(LDFLAGS) lcs.o $(ARCHIVES) -o lcs

test: lcs
	./test.sh

bench: lcs
	./queue.sh

plot:  
	./plot.sh

clean:
	-rm *.o
	-rm lcs

distclean:
	-rm *.sh.*


.PHONY: lcs
