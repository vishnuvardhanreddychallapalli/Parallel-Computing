CFLAGS=-O3 -std=c11 -g -fPIC -pthread -Xassembler -mrelax-relocations=no
CXXFLAGS=-O3 -std=c++11 -g -fPIC -pthread -Xassembler -mrelax-relocations=no
LDFLAGS=-pthread -fPIE
LDLIBS=libintegrate.a libfunctions.a
LD=g++

approx:
	$(LD) $(CXXFLAGS) $(LDFLAGS) approx.cpp $(LDLIBS) -o approx

libfunctions.a: functions.o
	ar rcs libfunctions.a functions.o

libintegrate.a: sequential_lib.o
	ar rcs libintegrate.a sequential_lib.o

assignment-pthreads.pdf: assignment-pthreads.tex
	pdflatex assignment-pthreads.tex

assignment-pthreads.tgz:  assignment-pthreads.pdf \
                         Makefile approx.cpp libfunctions.a libintegrate.a .gitignore \
                         params.sh test.sh clean.sh \
                         sequential static dynamic hello_thread
	tar zcvf assignment-pthreads.tgz \
                         assignment-pthreads.pdf \
                         Makefile approx.cpp libfunctions.a libintegrate.a .gitignore \
                         params.sh test.sh clean.sh \
                         sequential static dynamic hello_thread
