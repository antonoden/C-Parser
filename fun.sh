#!/bin/sh

for i in TestSuite/fun*.pas; do
    echo "testing $i"
    base=$(basename "$i" .pas)
    ./parser < "$i" >> "Output/fun.out.txt"
done
