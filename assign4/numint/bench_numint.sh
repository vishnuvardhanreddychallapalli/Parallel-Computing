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

for n in ${NUMINT_NS};
do
    for inten in ${INTENSITIES};
    do
	for gran in ${GRANS};
	do
	    for t in ${THREADS};
	    do

            if ! [[ "$n" -gt "100000" && "$gran" -le "10" ]] ;
            then
#                echo ${n} ${inten} ${gran} ${t}
		./numint 1 1 10 ${n} ${inten} ${t} dynamic ${gran} >/dev/null 2> ${RESULTDIR}/numint_${n}_${t}_${gran}_${inten}
            fi
	    done
	done
    done
done
	     
echo ending time is $(date)
