#!/bin/bash

man bash | grep -o "\w\{4,\}" | sort | uniq -ic | sort -nr | head -3	