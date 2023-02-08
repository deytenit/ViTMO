#!/usr/bin/env bash

mkdir -p build
clang++ -g mockup.cpp -o ./build/mockup
./build/mockup

