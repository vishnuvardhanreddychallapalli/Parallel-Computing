#!/bin/sh

RESULTDIR=result/
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
NODES="1 2 3 4 5 6" # since nodes are run from 6 -> 1 reverse for cosnsistent plot coloring


# error checking
#   file existence
#   file populate
for NP in ${NODES};
do
   for P in ${PS};
   do
      NPP=$(expr ${NP} \* ${P})
      if [ "${NPP}" -le "32" ] ;
      then
         for INTEN in ${INTENSITIES};
         do
            for N in ${NUM_INT_NS};
            do
               if [ ! -s ${RESULTDIR}/mpi_num_int_${N}_${INTEN}_${NP}_${P} ] ;
               then
                  echo ERROR: ${RESULTDIR}/mpi_num_int_${N}_${INTEN}_${NP}_${P} not found 
                  echo run \'make bench\'  and WAIT for it to complete
                  exit 1
               fi
            done
         done
      fi
   done
done



#exit 1
	     
# format output
for NP in ${NODES};
do
   for P in ${PS};
   do

      NPP=$(expr ${NP} \* ${P})
      if [ "${NPP}" -le "32" ] ;
      then
         # format 'N seq para'
         for INTEN in ${INTENSITIES};
         do
            for N in ${NUM_INT_NS};
            do
		echo ${N} \
		     $(cat ${RESULTDIR}/mpi_num_int_${N}_${INTEN}_1_1)\
		     $(cat ${RESULTDIR}/mpi_num_int_${N}_${INTEN}_${NP}_${P})
            done  > ${RESULTDIR}/speedupn_${NP}_${P}_${INTEN}
         done

         # format 'INTEN seq para'
         for N in ${NUM_INT_NS};
         do
            for INTEN in ${INTENSITIES};
            do
		echo ${INTEN} \
		     $(cat ${RESULTDIR}/mpi_num_int_${N}_${INTEN}_1_1)\
		     $(cat ${RESULTDIR}/mpi_num_int_${N}_${INTEN}_${NP}_${P})
            done  > ${RESULTDIR}/speedup_inten_${NP}_${P}_${N}
         done

      fi
   done
done

for N in ${NUM_INT_NS};
do
   for INTEN in ${INTENSITIES};
   do

         # format 'N seq para'
      for NP in ${NODES};
      do
         for P in ${PS};
         do
            NPP=$(expr ${NP} \* ${P})
            if [ "${NPP}" -le "32" ] ;
            then
		echo ${P} \
		     $(cat ${RESULTDIR}/mpi_num_int_${N}_${INTEN}_1_1)\
		     $(cat ${RESULTDIR}/mpi_num_int_${N}_${INTEN}_${NP}_${P})
            fi
         done  > ${RESULTDIR}/speedup_nodes_${N}_${INTEN}_${NP}

      done

      for P in ${PS};
      do
         for NP in ${NODES};
         do
            NPP=$(expr ${NP} \* ${P})
            if [ "${NPP}" -le "32" ] ;
            then
		echo ${NP} \
		     $(cat ${RESULTDIR}/mpi_num_int_${N}_${INTEN}_1_1)\
		     $(cat ${RESULTDIR}/mpi_num_int_${N}_${INTEN}_${NP}_${P})
            fi
         done  > ${RESULTDIR}/speedup_pernode_${N}_${INTEN}_${P}
      done

   done
done


# plot

for N in ${NUM_INT_NS} ;
do
   for INTEN in ${INTENSITIES};
   do
      GSPNP="${GSPNP} ; set title 'N=${N} INTEN=${INTEN}' ; plot "
      color=1
      for NP in ${NODES};
      do
         GSPNP="${GSPNP} '${RESULTDIR}/speedup_nodes_${N}_${INTEN}_${NP}' u 1:(\$2/\$3) t 'Num Nodes=${NP}' lc ${color} lw 3,  "
         color=`expr $color + 1`
      done

      GSPP="${GSPP} ; set title 'N=${N} INTEN=${INTEN}' ; plot "
      color=1
      for P in ${PS};
      do
         GSPP="${GSPP} '${RESULTDIR}/speedup_pernode_${N}_${INTEN}_${P}' u 1:(\$2/\$3) t 'Cores per node=${P}' lc ${color} lw 3,  "
         color=`expr $color + 1`
      done
 
   done
done

for NP in ${NODES};
do
   for P in ${PS};
   do
      NPP=$(expr ${NP} \* ${P})
      if [ "${NPP}" -le "32" ] ;
      then

         GSP="${GSP} ; set title 'Num Nodes=${NP} Cores per Node=${P}' ; plot "
         color=1
         for INTEN in ${INTENSITIES};
         do

                GSP="${GSP} '${RESULTDIR}/speedupn_${NP}_${P}_${INTEN}' u 1:(\$2/\$3) t 'inten=${INTEN}' lc ${color} lw 3,  "
                color=`expr $color + 1`

         done

         GSPINT="${GSPINT} ; set title 'Num Nodes=${NP} Cores per node=${P}' ; plot "
         color=1
         for N in ${NUM_INT_NS};
         do

                GSPINT="${GSPINT} '${RESULTDIR}/speedup_inten_${NP}_${P}_${N}' u 1:(\$2/\$3) t 'N=${N}' lc ${color} lw 3,  "
                color=`expr $color + 1`

         done
      fi
   done
done


gnuplot <<EOF

set terminal pdf
set output '${PLOTDIR}numint_N_plots.pdf'
set style data linespoints
set key top left;
set xlabel 'N'; 
set ylabel 'speedup';
set xrange [1000000:*];
set logscale x 10;
set yrange [0:36];
set ytics 2;

${GSP}

EOF

gnuplot <<EOF

set terminal pdf
set output '${PLOTDIR}numint_INTEN_plots.pdf'
set style data linespoints
set key top left;
set xlabel 'INTEN'; 
set ylabel 'speedup';
set xrange [*:*];
set logscale x 10;
set yrange [0:36];
set ytics 2;

${GSPINT}

EOF

gnuplot <<EOF

set terminal pdf
set output '${PLOTDIR}numint_NODES_plots.pdf'
set style data linespoints
set key top left;
set key samplen 2 spacing 1 font ",8"
set xlabel 'Cores per node'; 
set ylabel 'speedup';
set xrange [1:12];
set yrange [0:36];
set ytics 2;
set xtics 1;

${GSPNP}

EOF

gnuplot <<EOF

set terminal pdf
set output '${PLOTDIR}numint_PER_NODE_plots.pdf'
set style data linespoints
set key top right;
set key samplen 2 spacing 1 font ",8"
set xlabel 'Number of Nodes'; 
set ylabel 'speedup';
set xrange [1:7];
set yrange [0:36];
set ytics 2;
set xtics 1;

${GSPP}

EOF

