#!/bin/sh

RESULTDIR=result/

source ../params.sh

for sz in ${SIZE_STRONG} ;
do

    echo ${sz}
    for i in 1 4 9 16 25;
    do
        echo $i $(cat ${RESULTDIR}/strong_${sz}_${i})
#        echo $i $(cat ${RESULTDIR}/${sz}_${i})
    done

    echo "------------------------"
  
done

