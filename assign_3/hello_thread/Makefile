CFLAGS=-O3 -std=c11 -g -fPIC -pthread
CXXFLAGS=-O3 -std=c++11 -g -fPIC -pthread
LDFLAGS=-pthread
LD=g++

.PHONY: plot hello_thread

all: hello_thread

hello_thread: hello_thread.o
	$(LD) $(LDFLAGS) hello_thread.o $(LDLIBS) -o hello_thread

clean:
	./../clean.sh
