#!/bin/bash

at now+1minutes -f ./a.sh
tail -n 0 -f "$HOME/report"
