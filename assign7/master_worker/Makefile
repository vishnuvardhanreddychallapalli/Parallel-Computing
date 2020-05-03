CXX=mpicxx
CC=mpicxx
LD=mpicxx
CXXFLAGS=-std=c++11
LDLIBS=libfunctions.a 

mpi_master_worker:

#run: run_1x1 run_1x2 run_1x4 run_1x8 run_1x16 run_2x16 run_3x12 #1x1 does not make sense in master worker
#bench: run_1x2 run_1x4 run_1x8 run_1x16 run_2x16 run_3x12
#	echo
bench:
	./queue.sh

#1x1 does not make sense in master worker
# run_1x1: mpi_master_worker
# 	qsub -q mamba -l walltime=00:20:00 -d $(shell pwd) -l nodes=1:ppn=1 ./run.sh

run_1x2: mpi_master_worker
	qsub -q mamba -l walltime=00:20:00 -d $(shell pwd) -l nodes=1:ppn=2 ./run.sh

run_1x4: mpi_master_worker
	qsub -q mamba -l walltime=00:20:00 -d $(shell pwd) -l nodes=1:ppn=4 ./run.sh

run_1x8: mpi_master_worker
	qsub -q mamba -l walltime=00:20:00 -d $(shell pwd) -l nodes=1:ppn=8 ./run.sh

run_1x16: mpi_master_worker
	qsub -q mamba -l walltime=00:10:00 -d $(shell pwd) -l nodes=1:ppn=16 ./run.sh

run_2x16: mpi_master_worker
	qsub -q mamba -l walltime=00:10:00 -d $(shell pwd) -l nodes=2:ppn=16 ./run.sh

run_3x12: mpi_master_worker
	qsub -q mamba -l walltime=00:10:00 -d $(shell pwd) -l nodes=3:ppn=12 ./run.sh

table:
	./table.sh

plot:
	./plot.sh

test: mpi_master_worker ../approx
	./test.sh

clean:
	-rm *.o
	-rm mpi_master_worker

distclean:
	rm *.sh.*
