#!/bin/bash

test() {
    echo "START TEST with args: $1"
    cat $1 > tests/1.txt
    ./s21_cat $1 > tests/2.txt
    diff tests/1.txt tests/2.txt && echo SUCCESS
}

echo 'TESTING s21_cat'

test 'tests/empty.txt tests/empty.txt'
test 'tests/text_with_blanks.txt tests/some_text.txt'
test '-b tests/text_with_blanks.txt tests/some_text.txt'
#test '--number-nonblank tests/text_with_blanks.txt'
#test '-E tests/text_with_blanks.txt'
test '-v tests/unprintables.txt tests/text_with_blanks.txt'
test '-e tests/unprintables.txt tests/text_with_blanks.txt'
test '-n tests/text_with_blanks.txt tests/some_text.txt'
#test '--number tests/text_with_blanks.txt'
test '-s tests/text_with_blanks.txt tests/some_text.txt'
#test '--squeeze-blank tests/text_with_blanks.txt'
test '-t tests/unprintables.txt tests/some_text.txt'
#test '-T tests/text_with_blanks.txt'

test 'tests/empty.txt'
test 'tests/some_text.txt'
test '-b tests/some_text.txt'
test '-v tests/unprintables.txt'
test '-e tests/unprintables.txt'
test '-n tests/some_text.txt'
test '-s tests/text_with_blanks.txt'
test '-t tests/unprintables.txt'