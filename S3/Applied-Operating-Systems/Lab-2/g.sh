#!/bin/bash

PROC_IDS=()
PROC_READS=()

for FILE in $(find /proc -maxdepth 1 -wholename '/proc/[0-9]*'); do
    if [[ ! -f "$FILE/status" ]]; then
        continue
    fi

    PROC_IDS+=($(grep "^Pid:" "$FILE/status" | awk '{ print $2 }'))
    PROC_READS+=($(grep "^read_bytes:" "$FILE/io" | awk '{ print $2 }'))
done

sleep 1m

for IDX in ${!PROC_IDS[@]}; do
    PROC_ID=${PROC_IDS[IDX]}

    if [[ ! -f "/proc/$PROC_ID/status" ]]; then
        continue
    fi

    PROC_READ=$(grep "^read_bytes:" "/proc/$PROC_ID/io" | awk '{ print $2 }')
    PROC_CMD=$(tr -d '\0' < "/proc/$PROC_ID/cmdline")

    echo "$PROC_ID $PROC_CMD $(($PROC_READ - ${PROC_READS[IDX]}))" >> tmp
done

sort -nr -k3 tmp | head -n +3 | sed 's/ / : /g'
rm tmp
