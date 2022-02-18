#!/bin/bash

head -c 100K </dev/urandom >input

buf_lens=()
for ((i = 1; i <= 8; i++))
do
    buf_lens+=("$((4**i))")
done

make ex1

for i in "${buf_lens[@]}"
do
   echo "Running mycp with buffer length: $i bytes."
   time ./ex1 input output "$i"
done

rm output