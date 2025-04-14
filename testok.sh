#!/bin/sh

for i in TestSuite/testok*.pas; do
    echo "testing $i"
    base=$(basename "$i" .pas)
    ./parser < "$i" >> "Output/testok.out.txt"
done
