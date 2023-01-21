#!/bin/bash

OK=0

test_valgrind() {
    echo "START VALGRIND TEST with args: $1"
    valgrind --leak-check=full --log-file=tests/valgrind.log ./s21_grep $1
    egrep --color=auto -e 'ERROR SUMMARY: [^0]|are definitely|uninitialised|Unhandled exception|Invalid read|Invalid write|Invalid free|Source and desti|Mismatched free|unaddressable byte|vex x86' tests/valgrind.log
    if [[ $? -eq 0 ]]; then
        OK=1
    fi
}

echo 'TESTING s21_grep FOR LEAKS'

test_valgrind 'the tests/empty.txt'

#test_valgrind 'the tests/some_text.txt'
#test_valgrind '-o the tests/some_text.txt'
#test_valgrind 'not_1_1_exits tests/some_text.txt'
#test_valgrind 'a tests/some_text.txt tests/other_text.txt'
#test_valgrind '-n a tests/some_text.txt'
#test_valgrind '-n a tests/some_text.txt tests/other_text.txt'
#test_valgrind '-h a tests/some_text.txt tests/other_text.txt'
#test_valgrind '-i The tests/some_text.txt tests/other_text.txt'
#test_valgrind '-v the tests/some_text.txt tests/other_text.txt'
#test_valgrind '-c the tests/some_text.txt'
#test_valgrind '-c the tests/some_text.txt tests/other_text.txt'
#test_valgrind '-l The tests/some_text.txt tests/other_text.txt'
#test_valgrind '-s The not_exists tests/other_text.txt'
#test_valgrind '-f tests/regex.txt tests/some_text.txt tests/other_text.txt'
#test_valgrind '-e the -e The tests/some_text.txt tests/other_text.txt'
#test_valgrind '-se the not_1_1_exits tests/other_text.txt'
#test_valgrind '-sn -e the not_1_1_exits tests/other_text.txt'
#test_valgrind '-sh -e the not_1_1_exits tests/some_text.txt tests/other_text.txt'
#test_valgrind '-si -e the not_1_1_exits tests/some_text.txt tests/other_text.txt'
#test_valgrind '-sv -e the not_1_1_exits tests/some_text.txt tests/other_text.txt'
#test_valgrind '-sc -e the not_1_1_exits tests/some_text.txt tests/other_text.txt'
#test_valgrind '-sl -e the not_1_1_exits tests/some_text.txt tests/other_text.txt'
#test_valgrind '-sl -e the -f tests/regex.txt not_1_1_exits tests/some_text.txt tests/other_text.txt'
#test_valgrind '-nh the tests/some_text.txt tests/other_text.txt'
#test_valgrind '-ni the tests/some_text.txt tests/other_text.txt'
#test_valgrind '-nv the tests/some_text.txt tests/other_text.txt'
#test_valgrind '-nl the tests/some_text.txt tests/other_text.txt'
#test_valgrind '-n -e the -f tests/regex.txt tests/some_text.txt tests/other_text.txt'
#test_valgrind '-ov i tests/other_text.txt tests/some_text.txt'
#test_valgrind '-ol the tests/other_text.txt tests/some_text.txt'
#test_valgrind '-oc the tests/other_text.txt tests/some_text.txt'
#test_valgrind '-oh the tests/other_text.txt tests/some_text.txt'
#test_valgrind '-iv the tests/other_text.txt tests/some_text.txt'
#test_valgrind '-ic the tests/other_text.txt tests/some_text.txt'
#test_valgrind '-il the tests/other_text.txt tests/some_text.txt'
#test_valgrind '-in the tests/other_text.txt tests/some_text.txt'
#test_valgrind '-ih the tests/other_text.txt tests/some_text.txt'
#test_valgrind '-vc the tests/other_text.txt tests/some_text.txt'
#test_valgrind '-vl the tests/other_text.txt tests/some_text.txt'
#test_valgrind '-vn the tests/other_text.txt tests/some_text.txt'
#test_valgrind '-vh the tests/other_text.txt tests/some_text.txt'
#test_valgrind '-cv the tests/other_text.txt tests/some_text.txt'
#test_valgrind '-cn the tests/other_text.txt tests/some_text.txt'
#test_valgrind '-ch the tests/other_text.txt tests/some_text.txt'
#test_valgrind '-lh the tests/other_text.txt tests/some_text.txt'
exit $OK