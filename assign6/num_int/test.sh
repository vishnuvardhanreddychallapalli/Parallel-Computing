#!/bin/bash

# does not account for neg numbers
sci_notation_regex='^[0-9]+([.][0-9]+)?(e[0-9]+|e-[0-9]+)?$'

function test_time {
    # compare 
    if [[ ! $1 =~ $sci_notation_regex ]] ; 
    then
        echo ERROR: time is not on stderr or not formatted properly
        echo
        rm .time
        exit 1
    fi
    # delete tmp file 
    rm .time
}

SUCCESS_FILE=.passed_mpi_numint


# remove success file
if [ -e ${SUCCESS_FILE} ] ; 
then
    rm ${SUCCESS_FILE} 
fi


while read test; 
do
    t=($test)
    ANSW=$(mpirun -np ${t[1]} ./mpi_num_int 1 0 10 ${t[0]} ${t[2]} 2> .time < /dev/null)

    #correctness
    if ./../approx ${ANSW} ${t[3]};
    then # success
        test_time $(cat .time)
    else # fail
        echo FAIL: "mpirun -np ${t[1]} ./mpi_num_int 1 0 10 ${t[1]} ${t[2]}" should give roughly "${t[3]}" not ${ANSW}
        exit 1
    fi
    
    # "progess bar"
    echo -n "|"

done < cases.txt


touch ${SUCCESS_FILE} 

# all tests passed
echo
echo ================================
echo
echo Success! All NUMINT tests passed! ":)"
