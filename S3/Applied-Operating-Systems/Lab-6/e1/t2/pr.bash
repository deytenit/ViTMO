#!/bin/bash

for (( I=1; I <= $1; ++I )); do
	../shared/complexfn $I &
done
wait
