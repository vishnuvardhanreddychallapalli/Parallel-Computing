#!/bin/sh

RESULTDIR=result/
SEQ_RESULTDIR=../sequential/result/
PLOTDIR=plots/
h=`hostname`


if [ ! -d ${RESULTDIR} ];
then
    echo "must run make bench_lcs"
    exit 1
fi

if [ ! -d ${PLOTDIR} ];
then
    mkdir ${PLOTDIR}
fi

# import params
source ../params.sh

# error checking
#   file existence
#   file populate
for m in ${LCS_NS};
do
    for n in ${LCS_NS};
    do
        for t in ${THREADS};
        do
            # sequential
            if [ ! -s ${SEQ_RESULTDIR}/lcs_seq_${m}_${n} ] ;
            then
               echo ERROR: Must run the sequential bench before plotting
               echo Please GO TO the sequential directory
               echo and run \'make bench\' there
               exit 1
            fi

            # parallel
            if [ "${m}" -le "${n}" ] ; then
                if [ ! -s ${RESULTDIR}/lcs_${m}_${n}_${t} ] ;
                then
                    echo ERROR: ${RESULTDIR}/lcs_${m}_${n}_${t} not found or is empty 
                    echo run \'make bench\'  and WAIT for it to complete
                    exit 1
                fi
            fi
        done
    done
done
	
     

# format output
for m in ${LCS_NS} ;
do
    for n in ${LCS_NS} ;
    do
        if [ "${m}" -le "${n}" ] ; then
        for t in ${THREADS} ;
        do
	    #output in format "thread seq par"
	    echo ${t} \
	         $(cat ${SEQ_RESULTDIR}/lcs_seq_${m}_${n}) \
	         $(cat ${RESULTDIR}/lcs_${m}_${n}_${t})
        done   > ${RESULTDIR}/speedup_lcs_${m}_${n}
        fi
    done
done

for m in ${LCS_NS} ;
do
    for t in ${THREADS} ;
    do
	for n in ${LCS_NS} ;
	do
         if [ "${m}" -le "${n}" ] ; then
	    #output in format "m seq par"
	    echo ${n} \
	         $(cat ${SEQ_RESULTDIR}/lcs_seq_${m}_${n}) \
	         $(cat ${RESULTDIR}/lcs_${m}_${n}_${t})
         fi
        done   > ${RESULTDIR}/speedup_lcs_thread_${m}_${t}
    done
done


# plot
GSP="${GSP} ; set title 'lcs'; plot "
color=1
for m in ${LCS_NS} ;
do
    for n in ${LCS_NS} ;
    do 
        if [ "${m}" -le "${n}" ] ; then
            GSP="${GSP}  '${RESULTDIR}/speedup_lcs_${m}_${n}' u 1:(\$2/\$3) t 'm=${m} n=${n}' lc ${color} lw 3 , "
            color=`expr ${color} + 1`
        fi
    done
done

color=1
for m in ${LCS_NS} ;
do
    for n in ${LCS_NS} ;
    do 
        if [ "${m}" -le "${n}" ] ; then
           GSP="${GSP} ; set title 'lcs-${m}x${n}'; \
                    set key top left; \
                    set xlabel 'threads'; \
                    set ylabel 'speedup'; \
                    set xrange [0:20]; \
                    set yrange [0:20]; \
                    set ytics 2; \
                    set xtics 2;"
           GSP="${GSP} plot '${RESULTDIR}/speedup_lcs_${m}_${n}' u 1:(\$2/\$3) t 'm=${m} n=${n}' lc ${color}  lw 3 ; "

           color=`expr ${color} + 1`
        fi
    done
done


for m in ${LCS_NS} ;
do
    for n in ${LCS_NS} ;
    do 
    color=1
    for t in ${THREADS} ; 
    do 
        if [ "${m}" -lt "${n}" ] ; then

           GTSP="${GTSP} ; set title '${t} threads - ${m} x N'; \
                    set key top left; \
                    set xlabel 'N'; \
                    set ylabel 'speedup'; \
                    set xrange [*:*]; \
                    set yrange [0:20]; \
                    set logscale x 10; \
                    set ytics 2;" 
           GTSP="${GTSP} plot '${RESULTDIR}/speedup_lcs_thread_${m}_${t}' u 1:(\$2/\$3) t 'nbt=${t}' lc ${color}  lw 3 ; "

           color=`expr ${color} + 1`
        fi
    done
    done
done


#for n in ${LCS_NS} ;
#do
#    for t in ${THREADS} ; 
#    do 
#        cat ${RESULTDIR}/speedup_lcs_thread_${n}_${t}
#        GTSP="${GTSP} ; set title 'lcs-${t} threads, n=${n}'; \
#                    set key top left; \
#                    set xlabel 'M'; \
#                    set ylabel 'speedup'; \
#                    set xrange [*:*]; \
#                    set yrange [0:20]; \
#                    set ytics 2;"
#        GTSP="${GTSP} plot '${RESULTDIR}/speedup_lcs_thread_${n}_${t}' u 1:(\$2/\$3) t 'threads=${t}: n=${n}' lc 1 lw 3 ; "
#    done
#done

#echo ${GTSP}


gnuplot <<EOF
set terminal pdf
set output '${PLOTDIR}lcs_speedup_n.pdf'
set style data linespoints
set key top left;
set xlabel 'threads'; 
set ylabel 'speedup';
set xrange [0:20];
set yrange [0:20];
set xtics 2;
set ytics 2;

${GSP}

EOF

gnuplot <<EOF
set terminal pdf
set output '${PLOTDIR}lcs_speedup_thread.pdf'
set style data linespoints

${GTSP}


EOF
