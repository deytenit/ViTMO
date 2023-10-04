#!/bin/bash

IN="/var/log"

find $IN -name "*\.log" | xargs wc -l | awk "END { print $1 }"