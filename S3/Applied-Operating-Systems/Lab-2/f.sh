#!/bin/bash

MAX_PID=0
MAX_VM=0

for FILE in $(find /proc -maxdepth 1 -wholename '/proc/[0-9]*'); do
    if [[ ! -f "$FILE/status" ]]; then
        continue
    fi

    PROC_PID=$(grep "^Pid:" "$FILE/status" | awk '{ print $2 }')
    PROC_VM=$(grep "^VmSize:" "$FILE/status" | awk '{ print $2 }')

    if [[ $PROC_VM -gt $MAX_VM ]]; then
        MAX_VM=$PROC_VM
        MAX_PID=$PROC_PID
    fi
done

echo $MAX_PID
