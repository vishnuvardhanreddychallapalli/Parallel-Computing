#!/bin/sh

RESULTFILE=result_cpu

rm ${RESULTFILE}

./polynomial 1000000000 1 | tee -a ${RESULTFILE}
./polynomial 1000000000 10 | tee -a ${RESULTFILE}
./polynomial 1000000000 100 | tee -a ${RESULTFILE}
./polynomial 1000000000 1000 | tee -a ${RESULTFILE}

