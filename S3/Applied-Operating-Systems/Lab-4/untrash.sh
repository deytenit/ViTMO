#!/bin/bash

error () {
    echo "$1"
    echo "Usage: untrash.sh (FILE)"
    exit 1
}

TRASH="$HOME/.trash"
LOG="$HOME/.trash.log"
FILE="$1"

if [[ ! -d $TRASH ]]; then
    error "Trash directory does not exist"
fi

if [[ ! -f $LOG ]]; then
    error "Log file does not exist"
fi

if [[ $# -ne 1 ]]; then
    error "One operand was expected"
fi

while read LINE; do
    TARGET=$(echo "$LINE" | awk -F ":" '{ print $NF }' | awk -F "/" '{ print $NF }')
    DIR=$(dirname "$(echo "$LINE" | awk -F ":" '{ print $1 }')")
    IDX=$(echo "$LINE" | awk -F ":" '{ print $NF }' | awk -F "/" '{ print $1 }')

    if [[ "$FILE" == "$TARGET" ]]; then
        echo "Untrash $DIR/$FILE ? (y/n)"
        read RES < /dev/tty
        if [[ "$RES" == "y" ]]; then
            if [[ ! -d $DIR ]]; then
                echo "Target directory does not exist, saving to HOME!"
                DIR="$HOME"
            fi

            while [[ -f "$DIR/$TARGET" ]]; do
                echo "File already exists. Type new file name:"
                read TARGET < /dev/tty
            done

            ln "$TRASH/$IDX/$FILE" "$DIR/$TARGET"
            rm "$TRASH/$IDX/$FILE"

            if [[ -z $(ls -A "$TRASH/$IDX") ]]; then
                rm -r "$TRASH/$IDX"
            fi

            sed -i "\;$LINE;d" "$LOG"

            break
        fi
    fi
done < "$LOG"
