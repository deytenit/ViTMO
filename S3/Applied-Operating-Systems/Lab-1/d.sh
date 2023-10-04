#!/bin/bash

if [[ $PWD == $HOME ]]; then
	echo "$HOME"
else
	echo "Error: $PWD is not a home directory!"
	exit 1
fi