#!/bin/sh

RESULTDIR=result/
SEQ_RESULTDIR=../sequential/result/
PLOTDIR=plots/
h=`hostname`


# create result directory
if [ ! -d ${RESULTDIR} ];
then
    echo "must run make bench_bubblesort"
#    exit 1
fi

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
for n in ${BUBBLESORT_NS};
do
    for t in ${THREADS};
    do
         # sequential
         if [ ! -s ${SEQ_RESULTDIR}/bubblesort_seq_${n} ] ;
         then
             echo ERROR: Must run the sequential bench before plotting
             echo Please GO TO the sequential directory
             echo and run \'make bench\' there
             exit 1
         fi

         # parallel
         if [ ! -s ${RESULTDIR}/bubblesort_${n}_${t} ] ;
         then
             echo ERROR: ${RESULTDIR}/bubblesort_${n}_${t} not found 
             echo run \'make bench\'  and WAIT for it to complete
             exit 1
         fi
    done
done
	     

# format output
for n in ${BUBBLESORT_NS};
do
    for t in ${THREADS};
    do
	#output in format "thread seq par"
	echo ${t} \
	     $(cat ${SEQ_RESULTDIR}/bubblesort_seq_${n}) \
	     $(cat ${RESULTDIR}/bubblesort_${n}_${t})
    done   > ${RESULTDIR}/speedup_bubblesort_${n}
done

for t in ${THREADS};
do
    for n in ${BUBBLESORT_NS};
    do
      #output in format "n seq par"
      echo ${n} \
           $(cat ${SEQ_RESULTDIR}/bubblesort_seq_${n}) \
           $(cat ${RESULTDIR}/bubblesort_${n}_${t}) 
    done > ${RESULTDIR}/speedup_bubblesort_thread_${t}
done





# create plotting commands
GSP="${GSP} ; set title 'bubblesort'; plot  "
color=1
for n in ${BUBBLESORT_NS};
do 
    GSP="${GSP}  '${RESULTDIR}/speedup_bubblesort_${n}' u 1:(\$2/\$3) t 'n=${n}' lc ${color} lw 3 , "
    color=`expr $color + 1`
done

GTSP="${GTSP} ; set title 'bubblesort'; plot  "
color=1
for t in ${THREADS};
do 
    GTSP="${GTSP}  '${RESULTDIR}/speedup_bubblesort_thread_${t}' u 1:(\$2/\$3) t 'nbt=${t}' lc ${color} lw 3 , "
    color=`expr $color + 1`
done

color=1
for t in ${THREADS}; 
do 
    GTSP="${GTSP} ; set title '${t} threads'; \
                    set key top left; \
                    set xlabel 'N'; \
                    set ylabel 'speedup'; \
                    set xrange [*:*]; \
                    set yrange [0:20]; \
                    set logscale x 10; \
                    set ytics 2;"
    GTSP="${GTSP} plot '${RESULTDIR}/speedup_bubblesort_thread_${t}' u 1:(\$2/\$3) t 'nbt=${t}' lc ${color} lw 3 ; "
    color=`expr $color + 1`
done



# plot
gnuplot <<EOF
set terminal pdf
set output '${PLOTDIR}bubblesort_speedup_n.pdf'
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
set output '${PLOTDIR}bubblesort_speedup_thread.pdf'
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
