#!/bin/bash

OUT="d.txt"

for FILE in $(find /proc -maxdepth 1 -wholename '/proc/[0-9]*'); do
    if [[ ! -f "$FILE/status" ]]; then
        continue
    fi

    PROC_PID=$(awk '{ print $1 }' "$FILE/stat")
    PROC_PPID=$(awk '{ print $5 }' "$FILE/stat")
    PROC_RT=$(grep "^se.sum_exec_runtime" "$FILE/sched" | awk -F ':' '{ print $2 }')
    PROC_SW=$(grep "^nr_switches" "$FILE/sched" | awk -F ':' '{ print $2 }')

    echo "$PROC_PID $PROC_PPID $(echo "$PROC_RT / $PROC_SW" | bc -l)" >> tmp
done

sort -n -k2 tmp | awk '{ print "ProcessID="$1, ":", "Parent_ProcessID="$2, ":", "Average_Running_Time="$3 }' > $OUT
rm tmp
