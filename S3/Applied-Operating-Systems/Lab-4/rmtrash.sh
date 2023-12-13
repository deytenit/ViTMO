#!/bin/bash

error () {
    echo "$1"
    echo "Usage: rmtrash.sh (FILE)"
    exit 1
}

if [[ $# -ne 1 ]]; then
    error "One operand was expected"
fi

FILE="$1"

if [[ ! -e $FILE ]]; then
    error "No such file"
fi

mkdir -p "$HOME/.trash"

IDX=0
while [[ -e "$HOME/.trash/${IDX}/$FILE" ]]; do
    let ++IDX
done

LINK="${IDX}/$FILE"

mkdir -p "$HOME/.trash/$IDX"

ln "$FILE" "$HOME/.trash/$LINK" && rm $FILE && echo "$(realpath $FILE):$LINK" >> ~/.trash.log
