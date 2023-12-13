#!/bin/bash

while true; do
    read LINE
    case $LINE in
        "+")
            kill -USR1 $(cat f.pid)
        ;;
        "-")
            kill -SIGTRAP $(cat f.pid)
        ;;
        "**")
            kill -SIGSYS $(cat f.pid)
        ;;
        "*")
            kill -USR2 $(cat f.pid)
        ;;
        *TERM*)
            kill -SIGTERM $(cat f.pid)
            exit
        ;;
        *)
	    :
	;;
    esac
done
