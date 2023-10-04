#!/bin/bash

# Surname Name Middle Group Points
#  
# Print: (passed) / (overall) out of MXX37 group.

IN="./mod-test.txt"

awk '
{ 
    if ( $4 == "M3137" ) { 
        amount++;
        if ( $5 >= 60 ) 
            passed++;
    }
}
END {
    print passed, "/", amount, "students of M3137 passed!"
}
' $IN
