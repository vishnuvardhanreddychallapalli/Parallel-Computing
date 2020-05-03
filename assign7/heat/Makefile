CXX=mpicxx
CC=mpicxx
LD=mpicxx
CFLAGS=-O3 -std=c11 -fPIC -g
CXXFLAGS=-O3 -std=c++11 -fPIC -g
ARCHIVES=libgen.a
LDLIBS=libgen.a

mpi_heat:

heat_seq:

libgen.a: gen_lib.o
	ar rcs libgen.a gen_lib.o

bench: run_weak run_strong

plot:
	./plot.sh

test: mpi_heat
	./test.sh

run_strong: srun_1x1 srun_2x2 srun_3x3 srun_4x4 srun_5x5 
	echo

srun_1x1: mpi_heat
	qsub -q mamba -l walltime=04:00:00 -d $(shell pwd) -l nodes=1:ppn=1 -l mem=80GB ./run-strong.sh

srun_2x2: mpi_heat
	qsub -q mamba -l walltime=04:00:00 -d $(shell pwd) -l nodes=2:ppn=2 -l mem=80GB ./run-strong.sh

srun_3x3: mpi_heat
	qsub -q mamba -l walltime=04:00:00 -d $(shell pwd) -l nodes=3:ppn=3 -l mem=80GB ./run-strong.sh

srun_4x4: mpi_heat
	qsub -q mamba -l walltime=03:00:00 -d $(shell pwd) -l nodes=4:ppn=4 -l mem=80GB ./run-strong.sh

srun_5x5: mpi_heat
	qsub -q mamba -l walltime=03:00:00 -d $(shell pwd) -l nodes=5:ppn=5 -l mem=80GB ./run-strong.sh


run_weak: wrun_1x1 wrun_2x2 wrun_3x3 wrun_4x4 wrun_5x5 
	echo

wrun_1x1: mpi_heat
	qsub -q mamba -l walltime=04:00:00 -d $(shell pwd) -l nodes=1:ppn=1 -l mem=80GB ./run-weak.sh

wrun_2x2: mpi_heat
	qsub -q mamba -l walltime=04:00:00 -d $(shell pwd) -l nodes=2:ppn=2 -l mem=80GB ./run-weak.sh

wrun_3x3: mpi_heat
	qsub -q mamba -l walltime=04:00:00 -d $(shell pwd) -l nodes=3:ppn=3 -l mem=80GB ./run-weak.sh

wrun_4x4: mpi_heat
	qsub -q mamba -l walltime=03:00:00 -d $(shell pwd) -l nodes=4:ppn=4 -l mem=80GB ./run-weak.sh

wrun_5x5: mpi_heat
	qsub -q mamba -l walltime=03:00:00 -d $(shell pwd) -l nodes=5:ppn=5 -l mem=80GB ./run-weak.sh


run_big: brun_2x2 brun_3x3 brun_4x4 brun_5x5


brun_2x2: mpi_heat
	qsub -q mamba -l walltime=04:00:00 -d $(shell pwd) -l nodes=2:ppn=2 -l mem=250GB ./run-big.sh

brun_3x3: mpi_heat
	qsub -q mamba -l walltime=04:00:00 -d $(shell pwd) -l nodes=3:ppn=3 -l mem=250GB ./run-big.sh

brun_4x4: mpi_heat
	qsub -q mamba -l walltime=03:00:00 -d $(shell pwd) -l nodes=4:ppn=4 -l mem=250GB ./run-big.sh

brun_5x5: mpi_heat
	qsub -q mamba -l walltime=03:00:00 -d $(shell pwd) -l nodes=5:ppn=5 -l mem=250GB ./run-big.sh



table_strong:
	./table-strong.sh

table_weak:
	./table-weak.sh

clean:
	-rm *.o
	-rm mpi_heat

distclean:
	rm *.sh.*

