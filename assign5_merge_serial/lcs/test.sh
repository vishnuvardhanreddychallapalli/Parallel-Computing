#!/bin/bash

# does not account for neg numbers
sci_notation_regex='^[0-9]+([.][0-9]+)?(e[0-9]+|e-[0-9]+)?$'

function test_time {
    # compare 
    if [[ $1 =~ $sci_notation_regex ]] ; 
    then
#        echo time properly formatted 
	echo -n ''
    else
        echo ERROR: time is not on stderr or not formatted properly
        echo
        rm .time
        exit 1
    fi
    # delete tmp file 
    rm .time
}


SUCCESS_FILE=.passed_lcs

if [ -e ${SUCCESS_FILE} ] ;
then
    rm ${SUCCESS_FILE} 
fi

for m in 17 100 1000;
do
for n in 17 1000;
do 
    for nbt in 1 2 3 4;
    do
        TEST=$(./lcs $m $n $nbt 2> .time)
        if [ "$TEST" = "checked" ] ; 
        then
            test_time $(cat .time)
        else
            echo FAIL: "./lcs $m $n $nbt is not correct"
            exit 1
        fi
    done
done
done

touch ${SUCCESS_FILE} 

echo "================================"
echo
echo "Success! All LCS tests passed! :)"
