CXX=mpicxx
CC=mpicc
LD=mpicxx

hello:

run_1x16: hello
	qsub -q mamba -l walltime=00:05:00 -d $(shell pwd) -l nodes=1:ppn=16 ./run.sh

run_2x8: hello
	qsub -q mamba -l walltime=00:05:00 -d $(shell pwd) -l nodes=2:ppn=8 ./run.sh

run_4x8: hello
	qsub -q mamba -l walltime=00:05:00 -d $(shell pwd) -l nodes=4:ppn=8 ./run.sh

run: run_1x16 run_2x8 run_4x8


clean:
	-rm *.o
	-rm hello

distclean:
	rm *.sh.*
