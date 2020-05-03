qsub -q mamba -d $(pwd) -l nodes=1:ppn=16 bench_sequential.sh
