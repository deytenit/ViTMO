#!/bin/bash

LOG="result.log"
> "$LOG"

for I in {1..20}; do
	echo "$I:" >> "$LOG"

	for J in {1..10}; do
		\time -f "%e" ./sq.bash $I 2>> "$LOG"
	done

	>> "$LOG"
done
