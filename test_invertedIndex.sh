#!/bin/sh
# Navid Bhuiyan
# Test files simply using one shell script

# Move files to testing folder
cp *.c testFiles/
cp *.h testFiles/
cd testFiles
gcc -Wall -Werror -lm -std=c11 *.c -o invertedIndex
# Simple Tests (Part 1)
./invertedIndex
# Compare files searching for single word and their indices in one file
files=(network computer database)
for name in "${files[@]}"
do
    if diff -s "${name}.txt" "${name}_exp.txt" > /dev/null
    then
        echo "> Test Passed: ${name}.txt"
    else
        echo "> Test Failed: ${name}.txt"
    fi
done
# Compare files searching for multiple words and their indices in one file
files=(network_computer software_security)
for name in "${files[@]}"
do
    if diff -s "${name}.txt" "${name}_exp.txt" > /dev/null
    then
        echo "> Test Passed: ${name}.txt"
    else
        echo "> Test Failed: ${name}.txt"
    fi
done
# Clean-up
rm *.c *.h
