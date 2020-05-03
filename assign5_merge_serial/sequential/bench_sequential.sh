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


# reduce sequential
for n in $REDUCE_NS ;
do
    ./reduce_seq ${n}   >/dev/null 2> ${RESULTDIR}/reduction_seq_${n}
done

# longest common subsequence sequential
for m in ${LCS_NS};
do
    for n in ${LCS_NS};
    do 
        if [ "${m} -le "${n} ] ; then
            ./lcs_seq ${m} ${n} >/dev/null 2> ${RESULTDIR}/lcs_seq_${m}_${n}
        fi
    done
done

# mergesort sequential
for n in ${MERGESORT_NS} ; 
do
  ./mergesort_seq ${n}  >/dev/null 2> ${RESULTDIR}mergesort_seq_${n}
done

# bubblesort sequential
for n in ${BUBBLESORT_NS} ; 
do
  ./bubblesort_seq ${n}  >/dev/null 2> ${RESULTDIR}/bubblesort_seq_${n}
done

