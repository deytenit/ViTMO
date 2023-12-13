#!/bin/bash

N=$1
DATA=()

while true; do
	DATA+=(1 2 3 4 5 6 7 8 9 10)

	if [[ ${#DATA[@]} -gt $N ]]; then
		exit
	fi
done
