#!/bin/bash

test() {
    echo "START TEST with args: $1"
    grep $1 > tests/1.txt
    grep/s21_grep $1 > tests/2.txt
    diff tests/1.txt tests/2.txt && echo SUCCESS
}

echo 'TESTING s21_grep'
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
test '-s -f tests/regex.txt tests/other_text.txt'