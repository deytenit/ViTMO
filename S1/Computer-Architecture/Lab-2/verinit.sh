#!/usr/bin/env bash

mkdir -p build
iverilog -g2012 -o ./build/testbench testbench.sv && vvp ./build/testbench

