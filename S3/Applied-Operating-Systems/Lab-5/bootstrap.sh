#!/bin/bash

N=11662500
K=50

for (( CNT=0; CNT < K; CNT++ )); do
	./newmem.bash "$N" &
	sleep 1s
done



sleep 15s
