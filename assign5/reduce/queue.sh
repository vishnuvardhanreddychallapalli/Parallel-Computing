#!/bin/bash

if [ ! -e .passed_reduce ] ;
then
    echo "Must pass \`make test\` before queueing jobs"
    exit 1
fi


running=$(qstat -u ${USER} | egrep "bench_reduce.*[R|Q]")
if [ -n "${running}" ] ;
then
    echo "------------------------------------"
    echo ${USER} has this job in the queue or running 
    qstat | grep ${USER}
    echo
    echo please delete this job or wait for it to complete
    exit 1
fi


qsub -q mamba -d $(pwd) -l walltime=0:10:00,nodes=1:ppn=16 bench_reduce.sh

echo "----------------------"
echo
echo "Once the job COMPLETES, plot with \`make plot\`"
