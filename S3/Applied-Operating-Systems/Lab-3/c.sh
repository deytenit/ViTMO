#!/bin/bash

(crontab -l 2> /dev/null && echo "*/5 * * * 2 $HOME/src/os-lite/lab-3/a.sh") | crontab -
