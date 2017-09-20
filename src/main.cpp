/******************************************************************************
*	main.cpp
*
*	Main file for the Stock Cutting Problem.
*
*	Stuart Miller
*	Missouri S&T CS 5401
*	Fall 2017
******************************************************************************/


/**********************************************************
*	Headers
**********************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "shape.h"
#include "state.h"
#include "cfgParse.h"


/**********************************************************
*	Local Funtions
**********************************************************/
void readInputFile(std::string filename, shape*& shapes, int& width, int& numShapes);


/**********************************************************
*	Main
**********************************************************/
int main(int argc, char *argv[]) {

	// Variables
	config			cfg;
	shape*			shapes = NULL;
	int				width = 0;
	int				numShapes = 0;
	unsigned int	seed;
	state			overallBest;
	std::ofstream	log;

	// Get configuration
	if (argc <= 1) {
		std::cout << "Error: Please supply config filename as argument" << std::endl;
		exit(1);
	}
	cfg = getConfig(argv[1]);
	cfg.inputFile = argv[2];

	// Get shapes
	readInputFile(cfg.inputFile, shapes, width, numShapes);

	// Construct initial state
	state initial(shapes, width, numShapes);

	// Open log file
	log.open(cfg.logFile);
	if (!log.is_open()) {
		std::cout << "Error: Unable to write log file" << std::endl;
		exit(1);
	}
	log << "Result Log" << std::endl << std::endl;

	// Run specified algorithm
	switch (cfg.algorithm) {
	case RANDOM_SEARCH:

		// Loop for the specified number of runs
		for (int i = 1; i <= cfg.runs; i++) {
			state runBest;

			// Loop for the specified number of fitness evals
			for (int j = 1; j <= cfg.fitnessEvals; j++) {

				// Generate new random state and test its fitness
				state eval = initial;
				if (cfg.seedFromTime) {
					std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> time = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now()); // Good lord, <chrono> types are awful aren't they??
					seed = (unsigned int)time.time_since_epoch().count();
				}
				else {
					seed = cfg.seed * i * j;
				}
				eval.randomize(seed);
				if (eval.getFitness() > runBest.getFitness()) {

					// Write to log if best fitness for this run
					log << "Run " << i << ":\t" << j << "\t" << eval.getFitness() << std::endl;
					std::cout << "Run " << i << ":\t" << j << "\t" << eval.getFitness() << std::endl;
					runBest = eval;
				}
			}
			log << std::endl;

			// Check if best overall
			if (runBest.getFitness() > overallBest.getFitness())
				overallBest = runBest;
		}
	}

	// Print best solution
	overallBest.printSolution(cfg.solutionFile);
	overallBest.printLayout("solutions/layout.txt");
	
	// Clean up
	delete[] shapes;

	return 0;
}


/**********************************************************
*	readInputFile(std::string filename, shape*& shapes, int& width, int& numShapes)
*	Parses shape input file
*	 @param filename name of the shape input file
*	 @param shapes pointer to the shapes array
*	 @param width width value to be written to
*	 @param numShapes numShapes value to be written to
**********************************************************/
void readInputFile(std::string filename, shape*& shapes, int& width, int& numShapes) {
	
	// Variables
	std::ifstream	in;
	std::string		line;
	int				i;

	// Open input file
	in.open(filename);
	if (!in.is_open()) {
		std::cout << "Error: Unable to read shape input file" << std::endl;
		exit(1);
	}

	// Read first line (width & numShapes)
	in >> width;
	in >> numShapes;
	in.ignore(64, '\n');

	// Construct shapes array
	shapes = new shape[numShapes];
	
	// Read in file line-by-line and assign moves
	i = 0;
	while (getline(in, line)) {
		shapes[i] = shape(line.c_str());
		i++;
	}

	// Clean up
	in.close();

	return;
}
