#!/bin/bash

buf_lens=()
for ((i = 1; i <= 8; i++))
do
    buf_lens+=("$((4**i))")
done

make mycp

for i in "${!buf_lens[@]}"
do
   echo "Running mycp with buffer length: 4^$((i+1)) = ${buf_lens[$i]} bytes."
   echo "${buf_lens[$i]}"
   time ./mycp input output "${buf_lens[$i]}"
done

rm output