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

SUCCESS_FILE=.passed_reduce

# remove success file 
if [ -e ${SUCCESS_FILE} ] ; 
then
    rm ${SUCCESS_FILE} 
fi



#./reduce <n> <nbthreads>

for n in 10 1000 11112;
do 
  for nbthreads in 1 2 3 4;
  do 
    TEST=$(./reduce $n $nbthreads 2> .time)
    if ./../approx 0 "$TEST";
      then
        test_time $(cat .time)
      else 
        echo FAIL: "./reduce $n $nbthreads" should give 0 not ${TEST}
        exit 1
     fi
  done
done

touch ${SUCCESS_FILE}

echo "================================"
echo
echo "Success! All REDUCE tests passed! :)"
