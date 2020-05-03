#!/bin/sh

qsub -q mamba -l nodes=1:ppn=7:gpus=1 -d $(pwd) test_gpu.sh

