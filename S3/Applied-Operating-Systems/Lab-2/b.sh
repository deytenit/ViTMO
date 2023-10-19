#!/bin/bash

OUT="b.txt"
PROC="$(ps -e l --no-header | awk '{ print $3, $13 }' | head -n -5)"

printf "%s\n" "$PROC" | awk '$2 ~ /^\/sbin/ { print $1 }' > $OUT
