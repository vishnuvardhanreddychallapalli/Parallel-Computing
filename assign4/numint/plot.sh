#!/bin/sh

RESULTDIR=result/
SEQ_RESULTDIR=../sequential/result/
PLOTDIR=plots/
h=`hostname`


# create result directory
if [ ! -d ${RESULTDIR} ];
then
    mkdir ${RESULTDIR}
fi

# create plot directory
if [ ! -d ${PLOTDIR} ];
then
    mkdir ${PLOTDIR}
fi

# import params
source ../params.sh


# error checking
#   file existence
#   file populate
for n in ${NUMINT_NS};
do

    for inten in ${INTENSITIES};
    do

    if [ ! -e ${SEQ_RESULTDIR}/numint_seq_${n}_${inten} ] ;
    then
         echo ERROR: Must run the sequential bench before plotting
         echo Please GO TO the sequential directory
         echo and run \'make bench\' there
         exit 1
     fi


    for gran in ${GRANS};
    do

        if ! [[ "$n" -gt "100000" && "$gran" -le "10" ]] ;
        then
            for t in ${THREADS};
            do

                 if [ ! -e ${RESULTDIR}/numint_${n}_${t}_${gran}_${inten} ] ;
                 then
                     echo ERROR: ${RESULTDIR}/numint_${n}_${t}_${gran}_${inten} not found 
                     echo run \'make bench\'  and WAIT for it to complete
                     exit 1
                 fi

                 file_content=($(wc -w ${RESULTDIR}/numint_${n}_${t}_${gran}_${inten}))
                 if [ ${file_content[0]} == "0" ];
                 then
                     echo ERROR: ${RESULTDIR}/numint_${n}_${t}_${gran}_${inten} is empty 
                     echo run \'make bench\' and WAIT for it to complete
                     exit 1
                 fi
            done
        fi

    done
    done
done
	
	     
# format output
for n in ${NUMINT_NS};
do
    for inten in ${INTENSITIES}
    do
	for gran in ${GRANS}
	do

    if ! [[ "$n" -gt "100000" && "$gran" -le "10" ]] ;
    then
	    for t in ${THREADS};
	    do
		#output in format "thread seq par"
		echo ${t} \
		     $(cat ${SEQ_RESULTDIR}/numint_seq_${n}_${inten}) \
		     $(cat ${RESULTDIR}/numint_${n}_${t}_${gran}_${inten})
	    done   > ${RESULTDIR}/speedup_numint_${n}_${gran}_${inten}
    fi

	done
    done
done

for t in ${THREADS};
do 
    for inten in ${INTENSITIES}
    do
        for gran in ${GRANS};
        do
            for n  in ${NUMINT_NS};
            do

    if ! [[ "$n" -gt "100000" && "$gran" -le "10" ]] ;
    then
                echo ${n} \
		     $(cat ${SEQ_RESULTDIR}/numint_seq_${n}_${inten}) \
		     $(cat ${RESULTDIR}/numint_${n}_${t}_${gran}_${inten})
    fi

            done >   ${RESULTDIR}/speedup_numint_thread_${t}_${gran}_${inten}
        done
    done
done




# plot
for n in ${NUMINT_NS} ; 
do 
    for gran in ${GRANS} ; 
    do 

        if ! [[ "$n" -gt "100000" && "$gran" -le "10" ]] ;
        then

    GSP="${GSP} ; set title 'n=$n gran=$gran' ; plot "
    color=1
            for inten in ${INTENSITIES} ;
            do

                GSP="${GSP} '${RESULTDIR}/speedup_numint_${n}_${gran}_${inten}' u 1:(\$2/\$3) t 'inten=${inten}' lc ${color} lw 3,  "
                color=`expr $color + 1`
            done
        fi
    done
done



for t in ${THREADS};
do 
    for inten in ${INTENSITIES}
    do
        for gran in ${GRANS};
        do
            GTSP="${GTSP} ; set title 'threads=${t}, gran=${gran}, inten=${inten}' ; \
                            set key top left; \
                            set xlabel 'N'; \
                            set ylabel 'speedup'; \
                            set xrange [*:1000000000]; \
                            set logscale x 10; \
                            set yrange [0:20]; \
                            set ytics 2; "
            GTSP="${GTSP} plot '${RESULTDIR}/speedup_numint_thread_${t}_${gran}_${inten}' u 1:(\$2/\$3) t 'thread' lc 1 lw 3;"

        done
    done
done


#set title 'n=1000' ;
#plot '${RESULTDIR}/speedup_numint_1000_1_10' u 1:(\$2/\$3) t 'gran=1 inten=10' lc 1 lw 3, \
#     '${RESULTDIR}/speedup_numint_1000_1_1000' u 1:(\$2/\$3) t 'gran=1 inten=1000' lc 3 lw 3, \
#     '${RESULTDIR}/speedup_numint_1000_1000_10' u 1:(\$2/\$3) t 'gran=1000 inten=10' lc 4 lw 3, \
#     '${RESULTDIR}/speedup_numint_1000_1000_1000' u 1:(\$2/\$3) t 'gran=1000 inten=1000' lc 5 lw 3
#
#set title 'n=100000000' ;
#plot '${RESULTDIR}/speedup_numint_100000000_1_10' u 1:(\$2/\$3) t 'gran=1 inten=10' lc 1 lw 3, \
#     '${RESULTDIR}/speedup_numint_100000000_1_1000' u 1:(\$2/\$3) t 'gran=1 inten=1000' lc 3 lw 3, \
#     '${RESULTDIR}/speedup_numint_100000000_1000_10' u 1:(\$2/\$3) t 'gran=1000 inten=10' lc 4 lw 3, \
#     '${RESULTDIR}/speedup_numint_100000000_1000_1000' u 1:(\$2/\$3) t 'gran=1000 inten=1000' lc 5 lw 3


gnuplot <<EOF

set terminal pdf
set output '${PLOTDIR}numint_plots.pdf'
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
set output '${PLOTDIR}numint_plots_thread.pdf'
set style data linespoints

${GTSP}

EOF

