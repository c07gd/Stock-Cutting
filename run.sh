#!/bin/bash

# Check existance of arguments
if [ $# -eq 0 ]
  then
    echo "Error: Please supply 2 arguments. Valid form is ’./run.sh <configuration-filepath> <problem-instance>"
	exit 0
fi

# Determine which input file to use
shapefile=""
case $2 in
	"problem1" | "1" | "instance1")
		shapefile="patterns/50Shapes.txt"
		;;
    "pattern2" | "2" | "instance2")
        shapefile="patterns/100Shapes.txt"
		;;
	"pattern3" | "3" | "instance3")
		shapefile="patterns/100ShapesComplex.txt"
		;;
	*)
		echo "Error: Please supply a valid problem instance as argument 2."
		exit 0
		;;
esac

# Compile project
# Note: Compiling with C++11 to make use of high_resolution_clock for microsecond seeding
g++ src/*.cpp -std=c++11 -o stock_cutting.exe

# Run with given configuration
echo $shapefile
./stock_cutting.exe $1 $shapefile
