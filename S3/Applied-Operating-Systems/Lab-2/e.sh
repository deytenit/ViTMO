#!/bin/bash

FILE="d.txt"

awk -F ' : ' '
    BEGIN {
        block_time = 0;
        block_cnt = 0;
        last = 0;
    }
    {
        split($2, ppid_entry, /=/);
        split($3, art_entry, /=/);
        if (ppid_entry[2] == last) {
            block_time += art_entry[2];
            block_cnt++;
        }
        else {
            print "Average_Running_Children_of_ParentID="last, "is", (block_time/block_cnt);
            last = ppid_entry[2];
            block_time = art_entry[2];
            block_cnt = 1;
        }

        print $0;
    }
    END {
        print "Average_Running_Children_of_ParentID="last, "is", (block_time/block_cnt);
    }
' $FILE > tmp

mv tmp $FILE
