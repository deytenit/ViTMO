#!/bin/bash

TEST="./d.test.sh"

$TEST & PID1=$!
$TEST & PID2=$!
$TEST & PID3=$!

echo "$PID1" "$PID2" "$PID3"

cpulimit -p "$PID1" -l 10 &
top -p "$PID1","$PID2","$PID3"

kill "$PID3"
top -p "$PID1","$PID2","$PID3"

kill "$PID1"
kill "$PID2"
