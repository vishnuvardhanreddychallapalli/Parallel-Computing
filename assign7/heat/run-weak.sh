#!/bin/sh

RESULTDIR=result/

if [ ! -d ${RESULTDIR} ];
then
    mkdir ${RESULTDIR}
fi

source ../params.sh
P=${PBS_NP}

#SIZE_1="7900 11400 15600"
#SIZE_4="15800 22800 31200"
#SIZE_9="23700 34200 46800"
#SIZE_16="31600 45600 62400"
#SIZE_25="39500 57000 78000"

SIZE=SIZE_WEAK_${P}

for sz in ${!SIZE} ;
do
#   echo ${sz}
   mpirun ./mpi_heat $sz 5 2> ${RESULTDIR}/weak_${sz}_${P}
done
