#!/bin/bash
	
select CMD in "nano" "vi" "links" "exit"; do
    if [[ $CMD == "exit" ]]; then
        exit
    fi
    
    if [[ $CMD != "" ]]; then
        eval $CMD
        break
    fi
done