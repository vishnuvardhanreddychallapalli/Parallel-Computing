#!/bin/sh

RESULTDIR=result/
SEQ_RESULTDIR=../sequential/result/
PLOTDIR=plots/

# create plots directory
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
         # sequential 
         if [ ! -s ${SEQ_RESULTDIR}/reduction_seq_${n} ] ;
         then
             echo ERROR: Must run the sequential bench before plotting
             echo Please GO TO the sequential directory
             echo and run \'make bench\' there
             exit 1
         fi

         # parallel
         if [ ! -s ${RESULTDIR}reduction_${n}_${t} ] ;
         then
             echo ERROR: ${file} not found or empty
             echo run \'make bench\'  and WAIT for it to complete
             exit 1
         fi
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
	     $(cat ${RESULTDIR}reduction_${n}_${t})
    done   > ${RESULTDIR}speedup_reduction_${n}

done


for t in ${THREADS};
do
    for n in ${REDUCE_NS};
    do
	#output in format "thread seq par"
        echo ${n} \
	     $(cat ${SEQ_RESULTDIR}reduction_seq_${n}) \
	     $(cat ${RESULTDIR}reduction_${n}_${t})
    done   > ${RESULTDIR}speedup_reduction_thread_${t}
done


# plot
GSP="${GSP} ; set title 'reduce '; plot  "
color=1
for n in ${REDUCE_NS};
do 
    GSP="${GSP}  '${RESULTDIR}/speedup_reduction_${n}' u 1:(\$2/\$3) t 'n=${n}' lc ${color} lw 3 , "
    color=`expr $color + 1`
done


GTSP="${GTSP} ; set title 'reduce '; plot  "
color=1
for t in ${THREADS};
do 
    GTSP="${GTSP}  '${RESULTDIR}/speedup_reduction_thread_${t}' u 1:(\$2/\$3) t 'nbt=${t}' lc ${color} lw 3 , "
    color=`expr $color + 1`
done

color=1
for t in ${THREADS}; 
do 
    GTSP="${GTSP} ; set title '${t} threads'; \
		    set ylabel 'speedup'; \
		    set xlabel 'N'; \
		    set xrange [*:*]; \
		    set logscale x 10; \
		    set yrange [0:20]; \
		    set ytics 2;" 

    GTSP="${GTSP} plot '${RESULTDIR}/speedup_reduction_thread_${t}' u 1:(\$2/\$3) t 'nbt=${t}' lc ${color} lw 3, "
    color=`expr $color + 1`
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
set key top left;
set xlabel 'N'; 
set ylabel 'speedup';
set xrange [*:*];
set logscale x 10;
set yrange [0:20];
set ytics 2;
${GTSP}
EOF
