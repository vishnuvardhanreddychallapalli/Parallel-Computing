CFLAGS=-O3 -std=c11 -fPIC -g -fopenmp
CXXFLAGS=-O3 -std=c++11 -fPIC -g -fopenmp
LDFLAGS=-fopenmp
ARCHIVES=../libintegrate.a ../libfunctions.a ../libgen.a 
LD=g++


all: numint 

numint: numint.o
	$(LD) $(LDFLAGS) numint.o $(ARCHIVES) -o numint

test: numint ../approx
	./test.sh

bench: numint
	./queue.sh

plot:  
	./plot.sh

clean:
	-rm *.o
	-rm numint

distclean:
	-rm *.sh.*


.PHONY: numint
