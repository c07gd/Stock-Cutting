# Stock Cutting Problem

Evolutionary Algorithms for the Offline Cutting Stock Problem aka Offline 2D Bin Packing Problem.

### Requirements

This program makes use of C++11 in order to access the high_resolution_clock in \<chrono\> for microsecond timer seeding.

* C++11

### Running

Use the included run.sh to compile and run the program. You will need to supply the configuration file as an argument.

```bash
./run.sh instance1.cfg
```

### Input

The program expects the following files as inputs.

* Config File - Contains configurable parameters. See instance1.cfg for an example.
* Shape File - List of shapes to be processed. A series of tokens tracing the area of the shape. Each token is in the form of \<direction\>\<distance\>.


### Output

The program will output the following. The filepath of each in configurable in the passed .cfg file.

* Log File - Prints each row that improves on the best fitness found for that run.
* Standard Out - Mirrors the logfile.
* Solution File - Prints the layout of the best fitness for all runs. Each line is in the form of <x><y><rotation>
* Layout File (optional) - For debugging only. Prints the layout of shapes on the stock.

### Author

Stuart Miller - sm67c@mst.edu - [Github](https://github.com/stewythe1st)