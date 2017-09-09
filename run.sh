#!/bin/bash

# Compile project
# Note: Compiling with C++11 to make use of high_resolution_clock for microsecond seeding
g++ src/*.cpp -std=c++11 -o stock_cutting.exe

# Run with given configuration
./stock_cutting.exe $1