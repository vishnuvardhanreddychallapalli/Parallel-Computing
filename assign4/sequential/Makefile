CFLAGS=-O3 -std=c11 -fPIC -g -fopenmp
CXXFLAGS=-O3 -std=c++11 -fPIC -g -fopenmp
LDFLAGS=-fopenmp
ARCHIVES=../libintegrate.a ../libfunctions.a ../libgen.a 
LD=g++


all: numint_seq prefixsum_seq reduce_seq mergesort_seq


reduce_seq: reduce_seq.o
	$(LD) $(LDFLAGS) reduce_seq.o $(ARCHIVES) -o reduce_seq

numint_seq: numint_seq.o
	$(LD) $(LDFLAGS) numint_seq.o $(ARCHIVES) -o numint_seq

prefixsum_seq: prefixsum_seq.o
	$(LD) $(LDFLAGS) prefixsum_seq.o $(ARCHIVES) -o prefixsum_seq

mergesort_seq: mergesort_seq.o
	$(LD) $(LDFLAGS) mergesort_seq.o $(ARCHIVES) -o mergesort_seq


bench: reduce_seq numint_seq prefixsum_seq mergesort_seq
	./queue.sh

clean:
	-rm *.o
	-rm reduce_seq numint_seq prefixsum_seq mergesort_seq

distclean:
	-rm *.sh.*

.PHONY: reduce_seq numint_seq prefixsum_seq mergesort_seq
