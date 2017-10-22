# Stock Cutting Problem

Evolutionary Algorithms for the Offline Cutting Stock Problem aka Offline 2D Bin Packing Problem.

### Requirements

This program makes use of C++11 in order to access the high_resolution_clock in \<chrono\> for microsecond timer seeding.

* C++11

### Running

Use the included ```run.sh``` to compile and run the program. You will need to supply the configuration file and shape pattern file as arguments. Shape files are taken directly from Dr. T's website [here](http://web.mst.edu/~tauritzd/courses/ec/cs5401fs2017/).

```bash
./run.sh cfg/cfg1_control_set1.cfg patterns/50Shapes.txt
```

A batch run script ```run_batch.sh``` is also included which will re-run all test sets used in this assignment. It included to aid the developer and is not reccommended for the TA to use.

### Bonus Information
###### Bonus 3

* Edge calculating fucntion can be found in ```state::calcEdges()```, in [state.cpp](https://github.com/stewythe1st/Stock-Cutting/blob/master/src/state.cpp), [line 529](https://github.com/stewythe1st/Stock-Cutting/blob/master/src/state.cpp#L529)
* Documentation, graphs, and and explanation of findings for Bonus 3 can be found in assn1d.pdf in this repo's root directory.

### Input

The program expects the following files as inputs.

* Config File - Contains configurable parameters. See instance1.cfg for an example.
* Shape File - List of shapes to be processed. A series of tokens tracing the area of the shape. Each token is in the form of \<direction\>\<distance\>. Each problem instance is automatically configured to pull its corresponding shape file.


### Output

The program will output the following. The filepath of each in configurable in the passed .cfg file.

* Log File - Prints a header for each run and a series of tab-delimited rows in the form of \<fitness evaluations\>\<tab\>\<average fitness\>\<tab\>\<best fitness\>
* Standard Out - Mirrors the logfile.
* Solution File - Prints the layout of the best fitness for all runs. Each line is in the form of \<x\>,\<y\>,\<rotation\>
* Layout File (optional) - For debugging only. Prints the layout of shapes on the stock.

### Author

Stuart Miller - sm67c@mst.edu - [Github](https://github.com/stewythe1st)