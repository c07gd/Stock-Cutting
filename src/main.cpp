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
#include <cmath>
#include <string>
#include <chrono>
#include "pool.h"
#include "shape.h"
#include "state.h"
#include "cfgParse.h"


/**********************************************************
*	Compiler Constants
**********************************************************/
#define GEN_SCALED_PROB(k) ((float)(rand() % (int)std::pow(10.0,k)) / std::pow(10.0,k))


/**********************************************************
*	Main
**********************************************************/
int main(int argc, char *argv[]) {

	// Variables
	config			cfg;
	shape*			shapes = NULL;
	int				width = 0;
	int				numShapes = 0;
	std::ofstream	log;
	pool			population;
	pool			offspring;
	int				run;
	bool			terminate = false;
	state*			parent1;
	state*			parent2;

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

	// Open log file
	log.open(cfg.logFile);
	if (!log.is_open()) {
		std::cout << "Error: Unable to write log file" << std::endl;
		exit(1);
	}
	log << "Result Log" << std::endl << std::endl;

	// Randomly generate a start population
	population.create(cfg.mu, &initial);
	population.randomizeAll();

	// Add offspring to population
	run = 0;
	do {
		run++;

		// Create lambda offspring
		for (int i = 0; i < cfg.lambda; i++) {
			state* temp = new state(initial);
			switch (cfg.parentSel) {
			case PARENTSEL_FITNESSPROPOTIONAL:
				population.setFpProbability();
				parent1 = population.chooseParentFP();
				parent2 = population.chooseParentFP();
			case PARENTSEL_KTOURNAMENT:
			default:
				parent1 = population.chooseParentKTourn(cfg.parentSelTournSize);
				parent2 = population.chooseParentKTourn(cfg.parentSelTournSize);
				break;
			}
			temp->nPointCrossOver(parent1, parent2, cfg.crossovers);
			if (GEN_SCALED_PROB(4) <= cfg.mutationRate)
				temp->mutate();
			temp->calcFitness();
			offspring.add(temp);
		}

		// Add offspring to population
		for (int i = 0; i < offspring.getSize(); i++)
			population.add(offspring.get(i));
		offspring.empty();

		// Reduce population size
		switch(cfg.survivorSel) {
		case SURVIVORSEL_TRUNCATION:
				population.reduceByTruncation(cfg.mu - offspring.getSize());
				break;
		case SURVIVORSEL_KTOURNAMENT:
			default:
				population.reduceByKTourn(cfg.mu - offspring.getSize(), cfg.survivorSelTournSize);
				break;
		}

		std::cout << "Iteration:\t" << run << "\t" << population.getFittestState()->getFitness() << std::endl;

		// Run termination test
		switch (cfg.termType) {
		case TERMTYPE_AVGFITNESS:
			terminate = population.termTestAvgFitness(cfg.termGensUnchanged, 2.0);
			break;
		case TERMTYPE_BESTFITNESS:
			terminate = population.termTestBestFitness(cfg.termGensUnchanged);
			break;
		}

	// End loop when termination test returns true of we hit our max number of evals
	} while (!terminate && !population.termTestNumEvals(cfg.termEvals));

	// Print best solution
	state* best = population.getFittestState();
	best->printSolution(cfg.solutionFile);
	best->printLayout("solutions/layout.txt");

	// Clean up
	population.empty();
	offspring.empty();
	delete[] shapes;

	return 0;
}
