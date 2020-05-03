#!/bin/bash

running=$(qstat -u ${USER} | egrep "bench_sequential.*[R|Q]")
if [ -n "${running}" ] ;
then
    echo "------------------------------------"
    echo ${USER} has this job in the queue or running 
    qstat | grep ${USER}
    echo
    echo please delete this job or wait for it to complete
    exit 1
fi

qsub -q mamba -d $(pwd) -l walltime=0:20:00,nodes=1:ppn=16 bench_sequential.sh

