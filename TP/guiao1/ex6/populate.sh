#!/bin/bash

# https://unix.stackexchange.com/questions/369181/printing-every-nth-line-out-of-a-large-file-into-a-new-file
# https://www.networkworld.com/article/3606570/random-identity-generation-in-linux.html

make pessoas

rm pessoas.bin

END=10000
POWERS_OF_TEN=1
for ((i=1;i<=END;i++)); do
    FirstLastName=$(rig | sed -n '1~5p')
    Age=$(shuf  -i 20-120 -n 1)
    if [ "$i" = "$POWERS_OF_TEN" ]
    then
        time ./pessoas -i "\"$FirstLastName\"" "$Age"
        POWERS_OF_TEN=("$((10*POWERS_OF_TEN))")
    else
        ./pessoas -i "\"$FirstLastName\"" "$Age"
    fi
    
done