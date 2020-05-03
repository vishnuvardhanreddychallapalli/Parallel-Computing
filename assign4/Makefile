CFLAGS=-O3 -std=c11 -fPIC -g -fopenmp -Xassembler -mrelax-relocations=no
CXXFLAGS=-O3 -std=c++11 -fPIC -g -fopenmp-Xassembler -mrelax-relocations=no
LDFLAGS=-fopenmp -fPIE
LD=g++


libfunctions.a: functions.o
	ar rcs libfunctions.a functions.o

libintegrate.a: sequential_lib.o
	ar rcs libintegrate.a sequential_lib.o

libgen.a: gen_lib.o
	ar rcs libgen.a gen_lib.o

assignment-openmp-loop.pdf: assignment-openmp-loop.tex
	pdflatex assignment-openmp-loop.tex

assignment-openmp-loop.tgz: Makefile approx.cpp \
                            libfunctions.a libgen.a libintegrate.a \
                            sequential reduce numint prefixsum mergesort \
                            .gitignore params.sh \
                            assignment-openmp-loop.pdf
	tar zcvf assignment-openmp-loop.tgz \
                            Makefile approx.cpp \
                            libfunctions.a libgen.a libintegrate.a \
                            sequential reduce numint prefixsum mergesort \
                            .gitignore params.sh \
                            assignment-openmp-loop.pdf

