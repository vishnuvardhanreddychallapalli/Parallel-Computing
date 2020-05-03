#!/bin/bash


if [ ! -e .passed_mpi_numint ] ;
then
   echo Must pass \`make test\` before queuing
   exit 1 
fi



# yea, uhh we not letting you submit multiple bench
qselect -u ${USER} -N run_numint.sh | xargs qdel

# import params
source ../params.sh

echo starting time is $(date)

#count=0
for NP in ${NODES} ;
do 
   for P in ${PS} ;
   do
       NPP=$(expr ${NP} \* ${P})
       if [ "$NPP" -le 32 ] ;
       then
#       echo ${NP} ${P}
#echo -n
#       count=`expr $count + 1`
          qsub -q mamba -l walltime=00:20:00 -d $(pwd) -l nodes=${NP}:ppn=${P} run_numint.sh
       fi
   done
done 

echo ending time is $(date)
