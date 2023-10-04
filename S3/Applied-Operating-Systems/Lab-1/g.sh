#!/bin/bash

IN="/etc"
OUT="emails.lst"

grep -ohrI "[a-zA-Z0-9_-]\+@[a-zA-Z0-9_-]\+\.[a-z]\{2,\}" $IN | sed -z "s/\n/, /g;s/, $/\n/" > $OUT