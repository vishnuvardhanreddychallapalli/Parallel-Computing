CFLAGS=-O3 -std=c11 -fPIC -g -fopenmp -Xassembler -mrelax-relocations=no
CXXFLAGS=-O3 -std=c++11 -fPIC -g -fopenmp -Xassembler -mrelax-relocations=no
LDFLAGS=-fopenmp
ARCHIVES=libgen.a 
LD=g++


all: reduce mergesort bubblesort lcs approx



assignment-openmp-advanced.tgz: Makefile approx.cpp \
                            libgen.a \
                            sequential reduce mergesort bubblesort lcs \
                            params.sh \
                            assignment-openmp-advanced.pdf
	tar zcvf assignment-openmp-advanced.tgz \
                            libgen.a \
                            Makefile approx.cpp \
                            sequential reduce mergesort bubblesort lcs \
                            .gitignore params.sh \
                            assignment-openmp-advanced.pdf
