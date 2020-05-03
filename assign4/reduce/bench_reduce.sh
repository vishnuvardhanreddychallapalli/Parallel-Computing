#!/bin/sh

RESULTDIR=result/
h=`hostname`

if [ "$h" = "mba-i1.uncc.edu"  ];
then
    echo Do not run this on the headnode of the cluster, use qsub!
    exit 1
fi

# create result directory
if [ ! -d ${RESULTDIR} ];
then
    mkdir ${RESULTDIR}
fi


# import params
source ../params.sh


echo before starting time is $(date)

# execute 
for n in $REDUCE_NS;
do
    for t in $THREADS;
    do
	./reduce $n $t static -1 >/dev/null 2> ${RESULTDIR}reduction_${n}_${t}_static_-1
	./reduce $n $t dynamic 1 >/dev/null 2> ${RESULTDIR}reduction_${n}_${t}_dynamic_1
	./reduce $n $t dynamic 1000 >/dev/null 2> ${RESULTDIR}reduction_${n}_${t}_dynamic_1000
	./reduce $n $t dynamic 100000 >/dev/null 2> ${RESULTDIR}reduction_${n}_${t}_dynamic_100000	
    done
done

echo after starting time is $(date)
