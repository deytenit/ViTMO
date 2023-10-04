#!/bin/bash

RES=""

while read STR; do
    if [[ $STR == "q" ]]; then
        break
    fi
    RES+=$STR
done

echo $RES