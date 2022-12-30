#!/bin/bash

test_leaks() {
    echo "START TEST with args: $1"
    echo $1 >> tests/leaks_output.txt
    leaks -atExit -- ./s21_cat $1 >> tests/leaks_output.txt
}

echo 'TESTING s21_cat FOR LEAKS'

test_leaks 'tests/some_text.txt'
test_leaks '-b tests/text_with_blanks.txt'
test_leaks '-e tests/unprintables.txt'
test_leaks '-n tests/text_with_blanks.txt'
test_leaks '-s tests/text_with_blanks.txt'
test_leaks '-t tests/unprintables.txt'

grep LEAK: tests/leaks_output.txt
echo 'Press enter to remove tests/leaks_output.txt file'
read
rm tests/leaks_output.txt