#!/bin/sh

RESULTDIR=result/
SEQ_RESULTDIR=../sequential/result/
PLOTDIR=plots/

if [ ! -d ${PLOTDIR} ];
then
    mkdir ${PLOTDIR}
fi

# import params
source ../params.sh


# error checking
#   file existence
#   file populate
for n in ${REDUCE_NS};
do
    for t in ${THREADS};
    do
         if [ ! -e ${SEQ_RESULTDIR}/reduction_seq_${n} ] ;
         then
             echo ERROR: Must run the sequential bench before plotting
             echo Please GO TO the sequential directory
             echo and run \'make bench\' there
             exit 1
         fi

         FILES="${RESULTDIR}reduction_${n}_${t}_static_-1 ${RESULTDIR}reduction_${n}_${t}_dynamic_1 ${RESULTDIR}reduction_${n}_${t}_dynamic_1000 ${RESULTDIR}reduction_${n}_${t}_dynamic_100000"

         for file in ${FILES};
         do


         if [ ! -e ${file} ] ;
         then
             echo ERROR: ${file} not found 
             echo run \'make bench\'  and WAIT for it to complete
             exit 1
         fi

         file_content=($(wc -w ${file}))
         if [ ${file_content[0]} == "0" ];
         then
             echo ERROR: ${file} is empty
             echo run \'make bench\' and WAIT for it to complete
             exit 1
         fi

         done 	
    done
done
	     

# format output
for n in ${REDUCE_NS};
do
    for t in ${THREADS};
    do
	#output in format "thread seq par"
	echo ${t} \
	     $(cat ${SEQ_RESULTDIR}reduction_seq_${n}) \
	     $(cat ${RESULTDIR}reduction_${n}_${t}_static_-1)
    done   > ${RESULTDIR}speedup_reduction_${n}_static_-1

    for t in ${THREADS};
    do
	#output in format "thread seq par"
	echo ${t} \
	     $(cat ${SEQ_RESULTDIR}reduction_seq_${n}) \
	     $(cat ${RESULTDIR}reduction_${n}_${t}_dynamic_1)
    done   > ${RESULTDIR}speedup_reduction_${n}_dynamic_1

    for t in ${THREADS};
    do
	#output in format "thread seq par"
	echo ${t} \
	     $(cat ${SEQ_RESULTDIR}reduction_seq_${n}) \
	     $(cat ${RESULTDIR}reduction_${n}_${t}_dynamic_1000)
    done   > ${RESULTDIR}speedup_reduction_${n}_dynamic_1000

    for t in ${THREADS};
    do
	#output in format "thread seq par"
	echo ${t} \
	     $(cat ${SEQ_RESULTDIR}reduction_seq_${n}) \
	     $(cat ${RESULTDIR}reduction_${n}_${t}_dynamic_100000)
    done   > ${RESULTDIR}speedup_reduction_${n}_dynamic_100000
done


for t in ${THREADS};
do
    for n in ${REDUCE_NS};
    do
	#output in format "thread seq par"
        echo ${n} \
	     $(cat ${SEQ_RESULTDIR}reduction_seq_${n}) \
	     $(cat ${RESULTDIR}reduction_${n}_${t}_static_-1)
    done   > ${RESULTDIR}speedup_reduction_thread_${t}_static_-1

    for n in ${REDUCE_NS}$NS;
    do
	#output in format "thread seq par"
        echo ${n} \
	     $(cat ${SEQ_RESULTDIR}reduction_seq_${n}) \
	     $(cat ${RESULTDIR}reduction_${n}_${t}_dynamic_1)
    done   > ${RESULTDIR}speedup_reduction_thread_${t}_dynamic_1

    for n in ${REDUCE_NS};
    do
	#output in format "thread seq par"
        echo ${n} \
	     $(cat ${SEQ_RESULTDIR}reduction_seq_${n}) \
	     $(cat ${RESULTDIR}reduction_${n}_${t}_dynamic_1000)
    done   > ${RESULTDIR}speedup_reduction_thread_${t}_dynamic_1000

    for n in ${REDUCE_NS};
    do
	#output in format "thread seq par"
        echo ${n} \
	     $(cat ${SEQ_RESULTDIR}reduction_seq_${n}) \
	     $(cat ${RESULTDIR}reduction_${n}_${t}_dynamic_100000)
    done   > ${RESULTDIR}speedup_reduction_thread_${t}_dynamic_100000
done


# plot
for n in ${REDUCE_NS};
do 
   GSP="${GSP} ; set title 'n=${n}' ;  
       plot '${RESULTDIR}/speedup_reduction_${n}_static_-1' u 1:(\$2/\$3) t 'static' lc 1 lw 3, \
            '${RESULTDIR}/speedup_reduction_${n}_dynamic_1' u 1:(\$2/\$3) t 'dynamic,1' lc 3 lw 3, \
            '${RESULTDIR}/speedup_reduction_${n}_dynamic_1000' u 1:(\$2/\$3) t 'dynamic,1000' lc 4 lw 3, \
            '${RESULTDIR}/speedup_reduction_${n}_dynamic_100000' u 1:(\$2/\$3) t 'dynamic,100000' lc 5 lw 3;"
done


# logscale speedup plots
for t in ${THREADS}; 
do 
    GTSP="${GTSP} ; set title 'static | threads=${t}'; \
                    set key top left; \
                    set ylabel 'speedup'; \
                    set xlabel 'N'; \
                    set xrange [*:*]; \
                    set logscale x 10; \
                    set yrange [0:20]; \
                    set ytics 2;"
   GTSP="${GTSP} plot '${RESULTDIR}/speedup_reduction_thread_${t}_static_-1' u 1:(\$2/\$3) t 'thread' lc 1 lw 3;"
done

for t in ${THREADS}; 
do 
    GTSP="${GTSP} ; set title 'dynamic | threads=${t}, gran=1'; \
                    set key top left; \
                    set ylabel 'speedup'; \
                    set xlabel 'N'; \
                    set xrange [*:*]; \
                    set logscale x 10; \
                    set yrange [0:20]; \
                    set ytics 2;"
   GTSP="${GTSP} plot '${RESULTDIR}/speedup_reduction_thread_${t}_dynamic_1' u 1:(\$2/\$3) t 'thread' lc 1 lw 3;"
done

for t in ${THREADS}; 
do 
    GTSP="${GTSP} ; set title 'dynamic | threads=${t}, gran=10000'; \
                    set key top left; \
                    set ylabel 'speedup'; \
                    set xlabel 'N'; \
                    set xrange [*:*]; \
                    set logscale x 10; \
                    set yrange [0:20]; \
                    set ytics 2;"
   GTSP="${GTSP} plot '${RESULTDIR}/speedup_reduction_thread_${t}_dynamic_1000' u 1:(\$2/\$3) t 'thread' lc 1 lw 3;"
done

for t in ${THREADS}; 
do 
    GTSP="${GTSP} ; set title 'dynamic | threads=${t}, gran=100000'; \
                    set key top left; \
                    set ylabel 'speedup'; \
                    set xlabel 'N'; \
                    set xrange [*:*]; \
                    set logscale x 10; \
                    set yrange [0:20]; \
                    set ytics 2;"
   GTSP="${GTSP} plot '${RESULTDIR}/speedup_reduction_thread_${t}_dynamic_100000' u 1:(\$2/\$3) t 'thread' lc 1 lw 3;"
done


gnuplot <<EOF

set terminal pdf
set output '${PLOTDIR}reduce_plots.pdf'
set style data linespoints
set key top left;
set xlabel 'threads'; 
set ylabel 'speedup';
set xrange [0:20];
set yrange [0:20];
set ytics 2;
set xtics 2;

${GSP}

EOF


gnuplot <<EOF

set terminal pdf
set output '${PLOTDIR}reduce_plots_thread.pdf'
set style data linespoints


${GTSP}
EOF
