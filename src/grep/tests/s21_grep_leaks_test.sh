#!/bin/bash

test_leaks() {
    echo "START TEST with args: $1"
    echo $1 >> tests/leaks_output.txt
    leaks -atExit -- ./s21_grep $1 >> tests/leaks_output.txt
}

echo 'TESTING s21_grep FOR LEAKS'

test_leaks '-o the tests/some_text.txt'
test_leaks 'a tests/some_text.txt'
test_leaks 'not_1_1_exits tests/some_text.txt'
test_leaks 'a tests/some_text.txt tests/other_text.txt'
test_leaks '-n a tests/some_text.txt'
test_leaks '-n a tests/some_text.txt tests/other_text.txt'
test_leaks '-h a tests/some_text.txt tests/other_text.txt'
test_leaks '-i The tests/some_text.txt tests/other_text.txt'
test_leaks '-v the tests/some_text.txt tests/other_text.txt'
test_leaks '-c the tests/some_text.txt'
test_leaks '-c the tests/some_text.txt tests/other_text.txt'
test_leaks '-l The tests/some_text.txt tests/other_text.txt'
test_leaks '-s The not_exists tests/other_text.txt'
test_leaks '-f tests/regex.txt tests/some_text.txt tests/other_text.txt'
test_leaks '-e the -e The tests/some_text.txt tests/other_text.txt'
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
test_leaks '-ov i tests/other_text.txt tests/some_text.txt'
test_leaks '-ol the tests/other_text.txt tests/some_text.txt'
test_leaks '-oc the tests/other_text.txt tests/some_text.txt'
test_leaks '-oh the tests/other_text.txt tests/some_text.txt'
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
test_leaks '-cn the tests/other_text.txt tests/some_text.txt'
test_leaks '-ch the tests/other_text.txt tests/some_text.txt'
test_leaks '-lh the tests/other_text.txt tests/some_text.txt'

grep LEAK: tests/leaks_output.txt
echo 'Press enter to remove tests/leaks_output.txt file'
read
rm tests/leaks_output.txt