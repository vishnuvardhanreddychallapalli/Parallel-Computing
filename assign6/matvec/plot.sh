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


# error checking
#   file existence
#   file populate
for NP in ${MAT_MUL_NP};
do
   for N in ${MAT_MUL_NS};
   do

      # skip NS to large to fit on 1 node
      if [[ "${N}" -gt "100000" && ${NP} -eq "1" ]]; 
      then
         continue
      fi

      for ITER in ${ITERS};
      do
         if [ ! -s ${RESULTDIR}/mpi_matmul_${N}_${ITER}_${NP} ] ;
         then
            echo ERROR: ${RESULTDIR}/mpi_matmul_${N}_${ITER}_${NP} not found 
            echo run \'make bench\'  and WAIT for it to complete
            exit 1
         fi
      done
   done
done


	     
# format output
for NP in ${MAT__MUL_NP};
do
   for P in ${PS};
   do

      NPP=$(expr ${NP} \* ${P})
      if [ "${NPP}" -le "32" ] ;
      then
         # format 'N seq para'
         for ITER in ${ITERS};
         do
            for N in ${MAT_MUL_NS};
            do
		echo ${N} \
		     $(cat ${RESULTDIR}/mpi_matmul_${N}_${ITER}_1_1)\
		     $(cat ${RESULTDIR}/mpi_matmul_${N}_${ITER}_${NP}_${P})
            done  > ${RESULTDIR}/speedupn_${NP}_${P}_${ITER}
         done

         # format 'ITER seq para'
         for N in ${MAT_MUL_NS};
         do
            for ITER in ${ITERS};
            do
		echo ${ITER} \
		     $(cat ${RESULTDIR}/mpi_matmul_${N}_${ITER}_1_1)\
		     $(cat ${RESULTDIR}/mpi_matmul_${N}_${ITER}_${NP}_${P})
            done  > ${RESULTDIR}/speedup_inten_${NP}_${P}_${N}
         done

      fi
   done
done

for NP in ${MAT_MUL_NP};
do

   for ITER in ${ITERS};
   do
      # format 'N seq para'
      for N in ${MAT_MUL_NS};
      do
         if [ "${N}" -lt "100000" ] ; 
         then
            echo ${N} \
	         $(cat ${RESULTDIR}/mpi_matmul_${N}_${ITER}_1)\
		 $(cat ${RESULTDIR}/mpi_matmul_${N}_${ITER}_${NP})
         fi
      done  > ${RESULTDIR}/speedup_n_${NP}_${ITER}
   done

   for N in ${MAT_MUL_NS};
   do
      if [ "${N}" -lt "100000" ] ; 
      then
      # format 'N seq para'
         for ITER in ${ITERS};
         do
            echo ${ITER} \
	         $(cat ${RESULTDIR}/mpi_matmul_${N}_${ITER}_1)\
		 $(cat ${RESULTDIR}/mpi_matmul_${N}_${ITER}_${NP})
         done  > ${RESULTDIR}/speedup_iter_${NP}_${N}
      fi
   done
done


# plot

for ITER in ${ITERS};
do
   GSPNP="${GSPNP} ; set title 'ITER=${ITER}' ; plot "
   color=1
   for NP in ${MAT_MUL_NP};
   do
      GSPNP="${GSPNP} '${RESULTDIR}/speedup_n_${NP}_${ITER}' u 1:(\$2/\$3) t 'nodes x core per: ${NP}x${NP}' lc ${color} lw 3,  "
      color=`expr $color + 1`
   done
done

for NP in ${MAT_MUL_NP} ;
do
   GSPIT="${GSPIT} ; set title 'Number of Nodes=${NP}' ; plot "
   color=1
   for N in ${MAT_MUL_NS};
   do
      if [ "${N}" -lt "100000" ] ; 
      then
         GSPIT="${GSPIT} '${RESULTDIR}/speedup_iter_${NP}_${N}' u 1:(\$2/\$3) t 'N=${N}' lc ${color} lw 3,  "
         color=`expr $color + 1`
      fi
   done
done

# creat time tables
# pretty sure most of this is unnessary
echo "\documentclass{article}" > plots/time_table.tex
echo "\usepackage{listings}" >> plots/time_table.tex
echo "\usepackage{color}" >> plots/time_table.tex
echo "\usepackage{hyperref}" >> plots/time_table.tex
echo "\usepackage{graphics}" >> plots/time_table.tex
echo "\usepackage{graphicx}" >> plots/time_table.tex
echo "\begin{document}" >> plots/time_table.tex

for ITER in ${ITERS};
do

   # build column headers
   COL_ARGS="{ | l |"
   HEADER=" N "
   for NP in ${MAT_MUL_NP};
   do
      COL_ARGS="${COL_ARGS} l |"
      HEADER="${HEADER} & ${NP}x${NP} "
   done 
   COL_ARGS="${COL_ARGS} }"
   HEADER="${HEADER} \\\\"
   echo "\section{Time Table for ${ITER} iterations}" >> plots/time_table.tex
   echo "\begin{tabular} ${COL_ARGS} " >> plots/time_table.tex
   echo "\hline" >> plots/time_table.tex
   echo " & \multicolumn{5}{ c| }{Nodes x Cores per Node} \\\\ \cline{2-6}" >> plots/time_table.tex
   echo  ${HEADER} >> plots/time_table.tex
   echo "\hline" >> plots/time_table.tex

   for N in ${MAT_MUL_NS};
   do
      ROW="${N} "
      for NP in ${MAT_MUL_NP};
      do

         if [[ "${N}" -gt "100000" && ${NP} -eq "1" ]]; 
         then
           ROW="${ROW} & N/A "
           continue
         fi
         ROW="${ROW} & $(cat ${RESULTDIR}/mpi_matmul_${N}_${ITER}_${NP})"
#     echo ${N} ${NP} $(cat ${RESULTDIR}/mpi_matmul_${N}_${ITER}_${NP})
      done
      echo ${ROW} "\\\\ \hline" >> plots/time_table.tex
   done

   echo "\hline" >> plots/time_table.tex
      echo "\end{tabular}" >> plots/time_table.tex
done

echo "\end{document}" >> plots/time_table.tex

cd plots
pdflatex time_table.tex > /dev/null

# clean up from pdflatex
EXTS="aux log out tex"
for EXT in ${EXTS} ;
do
  rm time_table.${EXT}
done

cd ..


#gnuplot <<EOF

#set terminal pdf
#set output '${PLOTDIR}matmul_N_plots.pdf'
#set style data linespoints
#set key top left;
#set xlabel 'N'; 
#set ylabel 'speedup';
#set xrange [*:*];
#set logscale x 10;
#set yrange [0:36];
#set ytics 2;

#${GSPNP}

#EOF


gnuplot <<EOF

set terminal pdf
set output '${PLOTDIR}matmul_ITER_plots.pdf'
set style data linespoints
set key top left;
set xlabel 'ITER'; 
set ylabel 'speedup';
set xrange [1:5];
set yrange [0:36];
set ytics 2;

${GSPIT}

EOF
