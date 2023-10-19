#!/bin/bash

OUT="a.txt"
PROC="$(ps -U $USER -o pid,command --no-header | awk '{ print $1":"$2 }' | head -n -5)"

printf "%s\n" "$PROC" | wc -l > $OUT
printf "%s\n" "$PROC" >> $OUT
