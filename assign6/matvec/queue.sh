#!/bin/bash

if [ ! -e .passed_mpi_matmul ] ;
then
   echo Must pass \`make test\` before queuing
   exit 1 
fi

# yea, uhh we not letting you submit multiple bench
qselect -u ${USER} -N run_matmul.sh | xargs qdel

# import params
source ../params.sh

echo starting time is $(date)

#count=0
for NP in ${MAT_MUL_NP} ;
do 
   # try to be "respectful" of mem allocation
   if [ "${NP}" -eq "1" ] ;
   then
      MEM="80GB"
   else
      MEM="250GB"
   fi

   qsub -q mamba -l walltime=02:00:00 -d $(pwd) -l nodes=${NP}:ppn=${NP} -l mem=${MEM} run_matmul.sh

done 


echo ending time is $(date)
