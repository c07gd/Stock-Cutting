# Stock Cutting Problem

Evolutionary Algorithms for the Offline Cutting Stock Problem aka Offline 2D Bin Packing Problem.

### Requirements

This program makes use of C++11 in order to access the high_resolution_clock in \<chrono\> for microsecond timer seeding.

* C++11

### Running

Use the included ```run.sh``` to compile and run the program. You will need to supply the configuration file and shape pattern file as arguments. Shape files are taken directly from Dr. T's website [here](http://web.mst.edu/~tauritzd/courses/ec/cs5401fs2017/).

```bash
./run.sh instance1.cfg patterns/50Shapes.cfg
```

### Bonus Information
###### Bonus 1
The repair function can be found on [line 274](https://github.com/stewythe1st/Stock-Cutting/blob/master/src/state.cpp#L274) of [src/state.cpp](https://github.com/stewythe1st/Stock-Cutting/blob/master/src/state.cpp).

###### Bonus 2
The following operators can be found in [src/state.cpp](https://github.com/stewythe1st/Stock-Cutting/blob/master/src/state.cpp) on the lines listed below
* N-Point Crossover - [line 389](https://github.com/stewythe1st/Stock-Cutting/blob/master/src/state.cpp#L389)
* Uniform Crossover - [line 436](https://github.com/stewythe1st/Stock-Cutting/blob/master/src/state.cpp#L436)
* Random Reset Mutation - [line 470](https://github.com/stewythe1st/Stock-Cutting/blob/master/src/state.cpp#L470)
* Creep Mutation - [line 498](https://github.com/stewythe1st/Stock-Cutting/blob/master/src/state.cpp#L498)


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