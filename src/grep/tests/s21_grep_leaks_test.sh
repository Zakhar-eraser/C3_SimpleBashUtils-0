#!/bin/bash

test_leaks() {
    echo "START LEAKS TEST with args: $1"
    echo $1 >> tests/leaks_output.txt
    leaks -atExit -- ./s21_grep $1 >> tests/leaks_output.txt
    if [[ $? -eq 0 ]]; then
        OK=1
    fi    
}

echo 'TESTING s21_grep FOR LEAKS'

test_leaks '-o the tests/some_text.txt'
test_leaks '-o -e gehehehehe -e hehehehe -e hehehe -e hehe -e he tests/specific_test.txt'
test_leaks 'a tests/some_text.txt'
test_leaks 'not_1_1_exits tests/some_text.txt'
test_leaks 'a tests/some_text.txt tests/other_text.txt'
test_leaks '-n a tests/some_text.txt'
test_leaks '-n a tests/some_text.txt tests/other_text.txt'
test_leaks '-n -e Ipsum -e Lorem tests/some_text.txt'
test_leaks '-h a tests/some_text.txt tests/other_text.txt'
test_leaks '-i The tests/some_text.txt tests/other_text.txt'
test_leaks '-v the tests/some_text.txt tests/other_text.txt'
test_leaks '-c the tests/some_text.txt'
test_leaks '-c the tests/some_text.txt tests/other_text.txt'
test_leaks '-l The tests/some_text.txt tests/other_text.txt'
test_leaks '-s The not_exists tests/other_text.txt'
test_leaks '-f tests/regex.txt tests/some_text.txt tests/other_text.txt'
test_leaks '-e the -e The tests/some_text.txt tests/other_text.txt'
test_leaks "-e ^$ -e the tests/some_text.txt"
echo
echo 'paired options tests'
echo
test_leaks '-se the not_1_1_exits tests/other_text.txt'
test_leaks '-sn -e the not_1_1_exits tests/other_text.txt'
test_leaks '-sh -e the not_1_1_exits tests/some_text.txt tests/other_text.txt'
test_leaks '-si -e the not_1_1_exits tests/some_text.txt tests/other_text.txt'
test_leaks '-sv -e the not_1_1_exits tests/some_text.txt tests/other_text.txt'
test_leaks '-sc -e the not_1_1_exits tests/some_text.txt tests/other_text.txt'
test_leaks '-sl -e the not_1_1_exits tests/some_text.txt tests/other_text.txt'
test_leaks '-sl -e the -f tests/regex.txt not_1_1_exits tests/some_text.txt tests/other_text.txt'
test_leaks '-nh the tests/some_text.txt tests/other_text.txt'
test_leaks '-ni the tests/some_text.txt tests/other_text.txt'
test_leaks '-nv the tests/some_text.txt tests/other_text.txt'
test_leaks '-nl the tests/some_text.txt tests/other_text.txt'
test_leaks '-n -e the -f tests/regex.txt tests/some_text.txt tests/other_text.txt'
test_leaks '-on the tests/some_text.txt tests/some_text.txt'
test_leaks "-no -e [the] tests/other_text.txt"
test_leaks '-oi the tests/other_text.txt tests/some_text.txt'
test_leaks '-ov the tests/other_text.txt tests/some_text.txt'
test_leaks '-ol the tests/other_text.txt tests/some_text.txt'
test_leaks '-oc the tests/other_text.txt tests/some_text.txt'
test_leaks '-oh the tests/other_text.txt tests/some_text.txt'
test_leaks '-os -e the -f tests/regex.txt tests/nit.txt -n tests/some_text.txt'
test_leaks '-e hehehehe -e eheh -e he tests/specific_test.txt -o'
test_leaks '-iv the tests/other_text.txt tests/some_text.txt'
test_leaks '-ic the tests/other_text.txt tests/some_text.txt'
test_leaks '-il the tests/other_text.txt tests/some_text.txt'
test_leaks '-in the tests/other_text.txt tests/some_text.txt'
test_leaks '-ih the tests/other_text.txt tests/some_text.txt'
test_leaks '-vc the tests/other_text.txt tests/some_text.txt'
test_leaks '-vl the tests/other_text.txt tests/some_text.txt'
test_leaks '-vn the tests/other_text.txt tests/some_text.txt'
test_leaks '-vh the tests/other_text.txt tests/some_text.txt'
test_leaks '-cv the tests/other_text.txt tests/some_text.txt'
test_leaks '-cl the tests/other_text.txt tests/some_text.txt'
test_leaks '-cn the tests/other_text.txt tests/some_text.txt'
test_leaks '-ch the tests/other_text.txt tests/some_text.txt'
test_leaks '-lh the tests/other_text.txt tests/some_text.txt'

grep LEAK: tests/leaks_output.txt
echo 'Press enter to remove tests/leaks_output.txt file'
read
rm tests/leaks_output.txt
exit $OK