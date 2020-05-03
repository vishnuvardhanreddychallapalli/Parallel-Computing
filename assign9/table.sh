#!/bin/sh

echo degree n cputime gputime gpuspeedup

join -j 2 result_cpu result_gpu | cut -f 1,2,3,5 -d \  | awk '{print $1, $2, $3, $4, $3/$4}' | tee resulttable.tex
