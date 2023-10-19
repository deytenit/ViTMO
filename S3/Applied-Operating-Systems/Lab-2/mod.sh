h#!/bin/bash

# Same as g.sh + print process's user and calculate memory consumption by each.
#
# Print: Top 10 processes by reads and user with the most reads by their processes.

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

    PROC_USER=$(grep "^Uid:" "/proc/$PROC_ID/status" | awk '{ print $2 }' | xargs getent passwd | awk -F ':' '{print $1}')
    PROC_READ=$(grep "^read_bytes:" "/proc/$PROC_ID/io" | awk '{ print $2 }')
    PROC_CMD=$(tr -d '\0' < "/proc/$PROC_ID/cmdline")

    echo "$PROC_ID $PROC_USER $PROC_CMD $(($PROC_READ - ${PROC_READS[IDX]}))" >> tmp
done

sort -nr -k4 tmp | head -n +10 | sed 's/ / : /g' | awk -F ' : ' '
BEGIN {
    max_user = "";
    max_read = 0;
}
{
    user_reads[$2] += $4;

    if (user_reads[$2] > max_read) {
        max_user = $2;
        max_read = user_reads[$2];
    }

    print($0);
}
END {
    print(max_user, "->", max_read);
}
'
rm tmp
