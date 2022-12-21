#!/bin/bash

test() {
    echo "START TEST with args: $1"
    grep $1 > tests/1.txt
    grep/s21_grep $1 > tests/2.txt
    diff tests/1.txt tests/2.txt && echo SUCCESS
}

echo 'TESTING s21_grep'
test '-o the tests/some_text.txt'
#test 'a tests/some_text.txt'
#test 'not_1_1_exits tests/some_text.txt'
#test 'a tests/some_text.txt tests/other_text.txt'
#test '-n a tests/some_text.txt'
#test '-n a tests/some_text.txt tests/other_text.txt'
#test '-h a tests/some_text.txt tests/other_text.txt'
#test '-i The tests/some_text.txt tests/other_text.txt'
#test '-v the tests/some_text.txt tests/other_text.txt'
#test '-c the tests/some_text.txt'
#test '-c the tests/some_text.txt tests/other_text.txt'
#test '-l The tests/some_text.txt tests/other_text.txt'
#test '-s The not_exists tests/other_text.txt'
#test '-f tests/regex.txt tests/some_text.txt tests/other_text.txt'
#test '-e the -e The tests/some_text.txt tests/other_text.txt'
#echo
#echo 'paired options tests'
#echo
#test '-se the not_1_1_exits tests/other_text.txt'
#test '-sn -e the not_1_1_exits tests/other_text.txt'
#test '-sh -e the not_1_1_exits tests/some_text.txt tests/other_text.txt'
#test '-si -e the not_1_1_exits tests/some_text.txt tests/other_text.txt'
#test '-sv -e the not_1_1_exits tests/some_text.txt tests/other_text.txt'
#test '-sc -e the not_1_1_exits tests/some_text.txt tests/other_text.txt'
#test '-sl -e the not_1_1_exits tests/some_text.txt tests/other_text.txt'
#test '-sl -e the -f tests/regex.txt not_1_1_exits tests/some_text.txt tests/other_text.txt'
#test '-nh the tests/some_text.txt tests/other_text.txt'
#test '-ni the tests/some_text.txt tests/other_text.txt'
#test '-nv the tests/some_text.txt tests/other_text.txt'
#test '-nl the tests/some_text.txt tests/other_text.txt'
#test '-n -e the -f tests/regex.txt tests/some_text.txt tests/other_text.txt'
#test '-on the tests/some_text.txt tests/some_text.txt'
#test '-oi the tests/other_text.txt tests/some_text.txt'
#test '-ov i tests/other_text.txt tests/some_text.txt'
#test '-ocv the tests/other_text.txt tests/some_text.txt'
#test '-ol the tests/other_text.txt tests/some_text.txt'