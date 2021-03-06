#!/bin/bash

# https://unix.stackexchange.com/questions/369181/printing-every-nth-line-out-of-a-large-file-into-a-new-file
# https://www.networkworld.com/article/3606570/random-identity-generation-in-linux.html

make pessoas

rm pessoas.bin

END=1000
for ((i=1;i<=END;i++)); do
    FirstLastName=$(rig | sed -n '1~5p')
    Age=$(shuf  -i 20-120 -n 1)
    ./pessoas -i "$FirstLastName" "$Age"
done

# time ./pessoas -u_old "FirstName LastName" 45
# time ./pessoas -u_new 1000 45