#!/bin/bash

echo $$ > e.pid

PIPE="e.pipe"

mkfifo $PIPE

term()
{
    echo "Error: Incorrect input."
    rm e.pid
    exit 1
}

trap 'term' USR1

while true; do
    read LINE
    echo "$LINE" > $PIPE
done
