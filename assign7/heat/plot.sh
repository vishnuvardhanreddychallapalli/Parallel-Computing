#!/bin/bash

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

PS="1 4 9 16 25"

# error checking
#   file existence
#   file populate

for P in ${PS}; 
do

   SIZE_WEAK=SIZE_WEAK_${P}
   for sz in ${!SIZE_WEAK} ;
   do
      if [ ! -s ${RESULTDIR}/weak_${sz}_${P} ] ;
      then
         echo ERROR: ${RESULTDIR}/weak_${sz}_${P} not found 
         echo run \'make run_weak\'  and WAIT for it to complete
         exit 1
      fi
   done


   for sz in ${SIZE_STRONG};
   do
      if [ ! -s ${RESULTDIR}/strong_${sz}_${P} ] ;
      then
         echo ERROR: ${RESULTDIR}/strong_${sz}_${P} not found 
         echo run \'make run_strong\'  and WAIT for it to complete
         exit 1
      fi
   done

done

FILES="time_weak_s time_weak_m time_weak_l"

for f in ${FILES};
do
   if [ -e ${RESULTDIR}/${f} ];
   then
      rm ${RESULTDIR}/${f}
   fi
done

#SIZE_WEAK_1="7900 11400 15600"
#SIZE_WEAK_4="15800 22800 31200"
#SIZE_WEAK_9="23700 34200 46800"
#SIZE_WEAK_16="31600 45600 62400"
#SIZE_WEAK_25="39500 57000 78000"

SIZE_WEAK_S="7900 15800 23700 31600 39500" 
SIZE_WEAK_M="11400 22800 34200 45600 57000"
SIZE_WEAK_L="15600 31200 46800 62400 78000"

echo 1 $(cat ${RESULTDIR}/weak_7900_1) >> ${RESULTDIR}/time_weak_s
echo 4 $(cat ${RESULTDIR}/weak_15800_4) >> ${RESULTDIR}/time_weak_s
echo 9 $(cat ${RESULTDIR}/weak_23700_9) >> ${RESULTDIR}/time_weak_s
echo 16 $(cat ${RESULTDIR}/weak_31600_16) >> ${RESULTDIR}/time_weak_s
echo 25 $(cat ${RESULTDIR}/weak_39500_25) >> ${RESULTDIR}/time_weak_s

echo 1 $(cat ${RESULTDIR}/weak_11400_1) >> ${RESULTDIR}/time_weak_m
echo 4 $(cat ${RESULTDIR}/weak_22800_4) >> ${RESULTDIR}/time_weak_m
echo 9 $(cat ${RESULTDIR}/weak_34200_9) >> ${RESULTDIR}/time_weak_m
echo 16 $(cat ${RESULTDIR}/weak_45600_16) >> ${RESULTDIR}/time_weak_m
echo 25 $(cat ${RESULTDIR}/weak_57000_25) >> ${RESULTDIR}/time_weak_m

echo 1 $(cat ${RESULTDIR}/weak_15600_1) >> ${RESULTDIR}/time_weak_l
echo 4 $(cat ${RESULTDIR}/weak_31200_4) >> ${RESULTDIR}/time_weak_l
echo 9 $(cat ${RESULTDIR}/weak_46800_9) >> ${RESULTDIR}/time_weak_l
echo 16 $(cat ${RESULTDIR}/weak_62400_16) >> ${RESULTDIR}/time_weak_l
echo 25 $(cat ${RESULTDIR}/weak_78000_25) >> ${RESULTDIR}/time_weak_l



# format output

for P in ${PS}; 
do
   for sz in ${SIZE_STRONG};
   do
      echo ${sz} \
           $(cat ${RESULTDIR}/strong_${sz}_1) \
           $(cat ${RESULTDIR}/strong_${sz}_${P}) 
   done > ${RESULTDIR}/speedup_strong_${P}
done


# plot

GSPW="${GSPW} ; set title 'weak: ${SIZE_WEAK_S}' ; "
GSPW="${GSPW} plot  '${RESULTDIR}/time_weak_s' lw 3  "
GSPW="${GSPW} ; set title 'weak: ${SIZE_WEAK_M}' ; "
GSPW="${GSPW} plot  '${RESULTDIR}/time_weak_m' lw 3  "
GSPW="${GSPW} ; set title 'weak: ${SIZE_WEAK_L}' ; "
GSPW="${GSPW} plot  '${RESULTDIR}/time_weak_l' lw 3  "

GSPNP="${GSPNP} ; set title 'strong scaling' ;  plot"
for P in $PS; 
do
      GSPNP="${GSPNP}  '${RESULTDIR}/speedup_strong_${P}' u 1:(\$2/\$3) t '${P}' lw 3,  "
done


gnuplot <<EOF

set terminal pdf
set output '${PLOTDIR}strong.pdf'
set style data linespoints
set key top left;
set xlabel 'N'; 
set ylabel 'speedup';
set yrange [0:30];
set ytics 2;

${GSPNP}

EOF


gnuplot <<EOF

set terminal pdf
set output '${PLOTDIR}weak.pdf'
set style data linespoints
set key top left;
set xlabel 'P'; 
set ylabel 'time';
set xrange [1:28];
set yrange [*:*];
set xtics 2;

${GSPW}

EOF
