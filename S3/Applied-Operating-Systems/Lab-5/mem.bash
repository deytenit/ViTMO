#!/bin/bash

REPORT="report.log"
> "$REPORT"

DATA=()

for (( CNT=1; ; CNT++ )); do
	DATA+=(1 2 3 4 5 6 7 8 9 10)

	if [[ $(($CNT%100000)) -eq 0 ]]; then
		echo "${#DATA[@]}" >> "$REPORT"
	fi
done
