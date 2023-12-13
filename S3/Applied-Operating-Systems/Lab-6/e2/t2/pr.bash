#!/bin/bash

for (( I=1; I <= $1; ++I )); do
	../shared/accumulator $I $2 &
done
wait
