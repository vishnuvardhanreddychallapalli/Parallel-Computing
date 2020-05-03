#!/bin/sh

RESULTDIR=result/

#for i in 1 2 4 8 16 32 36;
for i in 2 4 8 16 32 36; #P=1 does not make sense for master worker.
do
    echo $i $(cat ${RESULTDIR}/${i})
done

