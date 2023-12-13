#!/bin/bash

mkdir files

LOG="result.log"
> "$LOG"

for I in {1..20}; do
	echo "$I:" >> "$LOG"
	../shared/filegen 15000000 $I

	for J in {1..10}; do
		\time -f "%e" ./pr.bash $I 15000000 2>> "$LOG"
	done

	>> "$LOG"
done
