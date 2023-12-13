#!/bin/bash

NOW=$(date +%Y-%m-%d)
LATEST=$(ls $HOME | grep -E "^backup" | grep -o -E "[0-9]{4}-[0-9]{2}-[0-9]{2}$" | sort -n | tail -1)

LATEST_SEC="$(date -d "$LATEST" +%s)"
NOW_SEC="$(date -d "$NOW" +%s )"

let DIFF=($NOW_SEC-$LATEST_SEC)/60/60/24

REPORT="$HOME/backup-report"
SRC="$HOME/source"
DEST="$HOME/backup-$NOW"

if [[ -z $LATEST ]] || [[ $DIFF -gt 7 ]]; then
    echo "Generating backup..."

    mkdir "$DEST"
    cp -a "$SRC/." "$DEST/"

    echo "$DEST was created." >> $REPORT
    echo "$(ls -ARo "$SRC" | sed "/^total/d")" >> $REPORT

    echo "Successfuly generated $DEST"
else
    echo "Updating existing backup..."

    echo "$DEST was modified." >> $REPORT

    for SRC_FILE in $(find "$SRC" -type f); do
        DEST_FILE="$DEST$(echo "$SRC_FILE" | sed "s;^$SRC;;g")"

        if [[ -f $DEST_FILE ]]; then
            SRC_SIZE=$(stat --printf="%s" "$SRC_FILE")
            DEST_SIZE=$(stat --printf="%s" "$DEST_FILE")

            if [[ $SRC_SIZE -ne $DEST_SIZE ]]; then
				mv -f "$DEST_FILE" "$DEST_FILE.$NOW"
                mkdir -p $(dirname "$DEST_FILE")
				cp "$SRC_FILE" "$DEST_FILE"
				echo "$(ls -o "$DEST_FILE" | sed "/^total/d") + $DEST_FILE.$NOW" >> $REPORT
			fi
        else
            mkdir -p $(dirname "$DEST_FILE")
            cp "$SRC_FILE" "$DEST_FILE"
            echo "$(ls -o "$DEST_FILE" | sed "/^total/d")" >> $REPORT
        fi
    done

    echo "$DEST was updated"
fi
