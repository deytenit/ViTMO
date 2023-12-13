#!/bin/bash

MEM_FILE="mem.log"
SWAP_FILE="swap.log"

> "$MEM_FILE"
> "$SWAP_FILE"


while true; do
	sleep 1s
	FREE="$(free --kibi)"
	echo "$FREE" | awk '{ if ($1 == "Mem:") print $3,$4;}' >> "$MEM_FILE"
	echo "$FREE" | awk '{ if ($1 == "Swap:") print $3,$4;}' >> "$SWAP_FILE"
done
