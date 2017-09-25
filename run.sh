#!/bin/bash

# Check existance of arguments
if [ $# -eq 0 ]
  then
    echo "Error: Please supply 2 arguments. Valid form is ’./run.sh <configuration-filepath> <problem-instance>"
	exit 0
fi

# Compile project
# Note: Compiling with C++11 to make use of high_resolution_clock for microsecond seeding
g++ src/*.cpp -std=c++11 -o stock_cutting.out

# Run with given configuration
./stock_cutting.out $1 $2
