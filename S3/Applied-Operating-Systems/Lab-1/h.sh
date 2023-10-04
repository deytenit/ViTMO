#!/bin/bash

IN="/etc/passwd"

awk -F ":" '{ print $1, $3 }' $IN | sort -n -k2