#!/bin/bash

PIPE=$1
MODE=+
RES=1

clean()
{
	killall tail
	rm $PIPE
}

(tail -f $PIPE) | while true; do
	read LINE
	case $LINE in
		"QUIT")
			echo "Quitting."
			clean
			kill $(cat e.pid)
			exit
		;;
		"+"|"*")
			MODE=$LINE
		;;
		[[:digit:]]*)
			let RES=$RES$MODE$LINE
			echo $RES
		;;
		*)
			echo "Error: Incorrect input."
			clean
			kill -USR1 $(cat e.pid)
			exit 1
		;;
	esac
done
