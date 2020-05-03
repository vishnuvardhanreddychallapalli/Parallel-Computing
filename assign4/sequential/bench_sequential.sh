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


echo starting time is $(date) 

# reduce sequential
for n in ${REDUCE_NS};
do
    ./reduce_seq ${n}   >/dev/null 2> ${RESULTDIR}/reduction_seq_${n}
done


# numerical integration sequential
for n in ${NUMINT_NS};
do
    for inten in ${INTENSITIES} ;
    do
        ./numint_seq 1 1 10 ${n} ${inten}  >/dev/null 2> ${RESULTDIR}/numint_seq_${n}_${inten}
    done
done


# prefixsum sequential
for n in ${PREFIXSUM_NS};
do 
    ./prefixsum_seq ${n} >/dev/null 2> ${RESULTDIR}/prefixsum_seq_${n}
done


# mergesort sequential
for n in ${MERGESORT_NS}; 
do
    ./mergesort_seq ${n}  >/dev/null 2> ${RESULTDIR}/mergesort_seq_${n}
done

echo ending time is $(date) 

