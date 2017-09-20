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
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "pool.h"
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
	state*			overallBest;
	std::ofstream	log;

	// Get configuration
	if (argc <= 1) {
		std::cout << "Error: Please supply config filename as argument" << std::endl;
		exit(1);
	}
	cfg = getConfig(argv[1]);
	cfg.inputFile = argv[2];

	// Get shapes
	readInputFile(argv[2], shapes, width, numShapes);

	srand(cfg.seed);

	// Construct initial state
	state initial(shapes, width, numShapes);
	initial.randomize();

	// Open log file
	log.open(cfg.logFile);
	if (!log.is_open()) {
		std::cout << "Error: Unable to write log file" << std::endl;
		exit(1);
	}
	log << "Result Log" << std::endl << std::endl;


	int mu = 100;
	int lambda = 50;
	int crossovers = 5;

	// Randomly generate a start population
	pool population;
	pool offspring;
	population.create(mu, &initial);
	population.randomizeAll();

	// Add offspring to population
	int bestFitness = 0;
	for (int i = 0; i < 1000; i++) {

		// Calculate fitness proportional probabilities for parent selection
		population.setFpProbability();

		// Create lambda offspring
		for (int j = 0; j < lambda; j++) {
			state* temp = new state(initial);
			temp->nPointCrossOver(population.chooseFpParent(), population.chooseFpParent(), crossovers);
			offspring.add(temp);
		}

		// Add offspring to population

		// If more offspring were generated...
		// ...we need to narrow the offspring pool to make the next generation
		if (mu <= lambda) {
			offspring.truncate(mu);
			population.empty();
			for (int j = 0; j < mu; j++)
				population.add(offspring.get(j));
		}

		// If fewer offspring were generated...
		// ...we need to choose which of the original population will survive
		else { // (mu > lambda)
			population.truncate(mu - offspring.getSize());
			for (int j = 0; j < lambda; j++)
				population.add(offspring.get(j));
		}

		// Empty the offspring pool
		offspring.empty();

		// Find the best fitness value to see if we're improving
		bestFitness = -1;
		for (int j = 0; j < mu; j++) {
			if (population.get(j)->getFitness() > bestFitness) {
				bestFitness = population.get(j)->getFitness();
			}
		}

		std::cout << "Iteration:\t" << i << "\t" << bestFitness << std::endl;
	}

	// Now that we're done, find the best member of our final population
	overallBest = &initial;
	for (int j = 0; j < mu; j++) {
		if (population.get(j)->getFitness() > overallBest->getFitness())
			overallBest = population.get(j);
	}

	// Print best solution
	overallBest->printSolution(cfg.solutionFile);
	overallBest->printLayout("solutions/layout.txt");

	// Clean up
	population.empty();
	offspring.empty();
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
