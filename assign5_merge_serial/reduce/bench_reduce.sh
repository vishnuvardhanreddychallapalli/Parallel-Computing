#!/bin/sh

RESULTDIR=result/
h=`hostname`

if [ "$h" = "mba-i1.uncc.edu"  ];
then
    echo Do not run this on the headnode of the cluster, use qsub!
    exit 1
fi

if [ ! -d ${RESULTDIR} ];
then
    mkdir ${RESULTDIR}
fi


# import params
source ../params.sh

for n in ${REDUCE_NS};
do
    for t in ${THREADS};
    do
	./reduce ${n} ${t}  >/dev/null 2> ${RESULTDIR}reduction_${n}_${t}
    done
done
	     
