#!/bin/bash

DEST="$HOME/restore"
LATEST="$HOME/$(ls $HOME | grep -E "^backup-[0-9]{4}-[0-9]{2}-[0-9]{2}$" | sort -n | tail -1)"

if [[ -z $LATEST ]]; then
    echo "No backups were found"
    exit 1
fi

mkdir -p "$DEST"
for FILE in $(find "$LATEST" -type f | grep -v -E ".[0-9]{4}-[0-9]{2}-[0-9]{2}$"); do
    DIR=$(dirname $(echo "$FILE" | sed "s;$LATEST/;;"))
    mkdir -p "$DEST/$DIR"
    cp $FILE "$DEST/$DIR/."
done

echo "$LATEST was restored"
