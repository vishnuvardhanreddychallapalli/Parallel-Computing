#!/bin/sh

RESULTFILE=result_gpu

rm ${RESULTFILE}

./polynomial_gpu 1000000000 1 | tee -a ${RESULTFILE}
./polynomial_gpu 1000000000 10 | tee -a ${RESULTFILE}
./polynomial_gpu 1000000000 100 | tee -a ${RESULTFILE}
./polynomial_gpu 1000000000 1000 | tee -a ${RESULTFILE}

