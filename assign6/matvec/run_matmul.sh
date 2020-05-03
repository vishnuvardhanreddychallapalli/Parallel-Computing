#!/bin/sh

RESULTDIR=result/

if [ ! -d ${RESULTDIR} ];
then
    mkdir ${RESULTDIR}
fi

# import params
source ../params.sh

#NP=${PBS_NP}
P=${PBS_NUM_PPN}
NP=$(expr ${PBS_NP} / ${PBS_NUM_PPN})

for N in ${MAT_MUL_NS} ;
do
   for ITER in ${ITERS} ;
   do

      # skip NS to large to fit on 1 node
      if [[ "${N}" -gt "100000" && ${NP} -eq "1" ]]; 
      then
        continue
      fi

      mpirun ./mpi_matmul ${N} ${ITER} 2> ${RESULTDIR}/mpi_matmul_${N}_${ITER}_${NP}

   done
done

