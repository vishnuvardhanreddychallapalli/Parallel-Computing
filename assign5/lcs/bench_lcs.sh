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


# bench
echo starting time is $(date)

for m in ${LCS_NS};
do
   for n in ${LCS_NS};
   do
      for t in $THREADS;
      do
         if [ "${m}" -le "${n}" ] ;
         then
	    ./lcs $m $n $t >/dev/null 2> ${RESULTDIR}/lcs_${m}_${n}_${t}
         fi
      done
   done
done

echo ending time is $(date)
