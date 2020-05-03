CFLAGS=-O3 -std=c11 -fPIC -g -fopenmp
CXXFLAGS=-O3 -std=c++11 -fPIC -g -fopenmp
LDFLAGS=-fopenmp
ARCHIVES=../libgen.a 
LD=g++


UTILDIR=util


all: reduce mergesort bubblesort lcs approx


# archives

libgen.a: gen_lib.o
	ar rcs libgen.a gen_lib.o



reduce_seq: reduce_seq.o
	$(LD) $(LDFLAGS) reduce_seq.o $(ARCHIVES) -o reduce_seq


mergesort_seq: mergesort_seq.o
	$(LD) $(LDFLAGS) mergesort_seq.o $(ARCHIVES) -o mergesort_seq


bubblesort_seq: bubblesort_seq.o
	$(LD) $(LDFLAGS) bubblesort_seq.o $(ARCHIVES) -o bubblesort_seq


lcs_seq: lcs_seq.o
	$(LD) $(LDFLAGS) lcs_seq.o $(ARCHIVES) -o lcs_seq


# run benchmarks
bench: reduce_seq mergesort_seq bubblesort_seq lcs_seq
	./queue.sh

clean:
	-rm *.o
	-rm reduce_seq mergesort_seq bubblesort_seq lcs_seq

distclean:
	-rm *.sh.*


.PHONY: reduce mergesort bubblesort lcs reduce_seq mergesort_seq bubblesort_seq lcs_seq 
