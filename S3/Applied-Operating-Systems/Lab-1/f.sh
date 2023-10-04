#!/bin/bash

IN="/var/log/anaconda/X.log"
OUT="full.log"

awk '$3 == "(II)"' $IN | sed 's/(II)/Information:/g' > $OUT
awk '$3 == "(WW)"' $IN | sed 's/(WW)/Warning:/g' >> $OUT

cat $OUT