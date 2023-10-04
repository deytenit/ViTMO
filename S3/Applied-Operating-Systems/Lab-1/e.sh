#!/bin/bash

IN="/var/log/syslog"
OUT="info.log"

awk '$2 == "INFO"' $IN > $OUT