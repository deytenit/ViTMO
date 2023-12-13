#!/bin/bash

echo $$ > f.pid

MODE="+"
ADD=2
RES=1

mode_add()
{
    MODE="+"
}

mode_sub()
{
    MODE="-"
}

mode_pow()
{
    MODE="**"
}

mode_mul()
{
    MODE="*"
}

term()
{
    echo "Handler was terminated by 'SIGTERM' called by other proccess."
    rm f.pid
    exit
}

trap 'mode_add' USR1
trap 'mode_sub' SIGTRAP
trap 'mode_mul' USR2
trap 'mode_pow' SIGSYS
trap 'term' SIGTERM

while true; do
    let RES=$RES$MODE$ADD
    echo "$RES"
    sleep 1s
done
