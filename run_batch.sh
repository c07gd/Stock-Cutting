#!/bin/bash

g++ src/*.cpp -std=c++11 -o stock_cutting.out

# Comment lines below as needed
./stock_cutting.out cfg/cfg1_set1.cfg patterns/50Shapes.txt &
./stock_cutting.out cfg/cfg1_set2.cfg patterns/100Shapes.txt &
./stock_cutting.out cfg/cfg1_set3.cfg patterns/100ShapesComplex.txt &
./stock_cutting.out cfg/cfg1_control_set1.cfg patterns/50Shapes.txt &
./stock_cutting.out cfg/cfg1_control_set2.cfg patterns/100Shapes.txt &
./stock_cutting.out cfg/cfg1_control_set3.cfg patterns/100ShapesComplex.txt &
./stock_cutting.out cfg/cfg2_no_mutation_set1.cfg patterns/50Shapes.txt &
./stock_cutting.out cfg/cfg2_no_mutation_set2.cfg patterns/100Shapes.txt &
./stock_cutting.out cfg/cfg2_no_mutation_set3.cfg patterns/100ShapesComplex.txt &
./stock_cutting.out cfg/cfg2_half_mutation_set1.cfg patterns/50Shapes.txt &
./stock_cutting.out cfg/cfg2_half_mutation_set2.cfg patterns/100Shapes.txt &
./stock_cutting.out cfg/cfg2_half_mutation_set3.cfg patterns/100ShapesComplex.txt &
./stock_cutting.out cfg/cfg2_all_mutation_set1.cfg patterns/50Shapes.txt &
./stock_cutting.out cfg/cfg2_all_mutation_set2.cfg patterns/100Shapes.txt &
./stock_cutting.out cfg/cfg2_all_mutation_set3.cfg patterns/100ShapesComplex.txt &
./stock_cutting.out cfg/cfg3_repair_set1.cfg patterns/50Shapes.txt &
./stock_cutting.out cfg/cfg3_repair_set2.cfg patterns/100Shapes.txt &
./stock_cutting.out cfg/cfg3_repair_set3.cfg patterns/100ShapesComplex.txt &
./stock_cutting.out cfg/cfg3_randreset_set1.cfg patterns/50Shapes.txt &
./stock_cutting.out cfg/cfg3_randreset_set2.cfg patterns/100Shapes.txt &
./stock_cutting.out cfg/cfg3_randreset_set3.cfg patterns/100ShapesComplex.txt &
./stock_cutting.out cfg/bonus3_set1.cfg patterns/50Shapes.txt &
./stock_cutting.out cfg/bonus3_set2.cfg patterns/100Shapes.txt &
./stock_cutting.out cfg/bonus3_set3.cfg patterns/100ShapesComplex.txt &
