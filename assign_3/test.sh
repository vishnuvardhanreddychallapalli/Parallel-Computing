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


if [ -e .passed_"$1" ] ; 
then
    rm .passed_"$1"
fi


while read test; 
do

    t=($test)
    # correctness
#    if [ $1 = "static
    ANSW=$(./"$1"_sched ${t[0]} 0 10 ${t[1]} ${t[2]} ${t[3]} ${t[4]} ${t[5]}  2> .time)  

    if ./../approx ${ANSW} ${t[-1]};
    then
    # check time format
        test_time $(cat .time)

    else # display error and quit
        echo FAIL: "./"$1"_sched ${t[0]} 0 10 ${t[1]} ${t[2]} ${t[3]} ${t[4]}" should give roughly "${t[-1]}" not ${ANSW}
        exit 1
    fi

done < "$2" 


touch .passed_"$1"


# all tests passed
echo ================================
echo
echo Success! All tests passed! ":)"
