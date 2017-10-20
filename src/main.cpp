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
#include <iomanip>
#include "pool.h"
#include "shape.h"
#include "state.h"
#include "cfgParse.h"
#include "pareto.h"


/**********************************************************
*	Compiler Constants
**********************************************************/
#define IO_FORMAT_FLOAT(x)	std::fixed << std::setprecision(x) << std::setfill('0')


/**********************************************************
*	Global Variables
**********************************************************/
int g_evals;
int g_targetEvals;


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
	std::ofstream	solution;
	pool			population;
	pool			offspring;
	bool			terminate = false;
	state*			parent1;
	state*			parent2;
	int				overallBestFitness = -1;
	std::vector<state*>
					bestPareto;

	// Get configuration
	if (argc <= 1) {
		std::cout << "Error: Please supply config filename as argument" << std::endl;
		exit(1);
	}
	cfg = getConfig(argv[1]);
	cfg.inputFile = argv[2];
	g_targetEvals = cfg.fitnessEvals;

	// Get shapes
	readInputFile(argv[2], shapes, width, numShapes);

	// Seed random number generator
	switch (cfg.seedFromTime) {
	case SEED_TIME: {
			std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> time = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now()); // Good lord, <chrono> types are awful aren't they??
			srand((unsigned int)time.time_since_epoch().count());
		}
		break;
	case SEED_STATIC:
		srand(cfg.seed);
		break;
	}	

	// Construct initial state
	state initial(shapes, width, numShapes);
	state overallBest(initial);
	initial.setParams(cfg);

	// Open log file
	log.open(cfg.logFile);
	if (!log.is_open()) {
		std::cout << "Error: Unable to write log file" << std::endl;
		exit(1);
	}
	log << "Configuration File: " << argv[1] << std::endl << std::endl;
	log << "Result Log" << std::endl;

	// Add offspring to population
	for (int run = 0; run < cfg.runs; run++) {

		log << std::endl << "Run " << run + 1 << std::endl;
		std::cout << std::endl << "Run " << run + 1 << std::endl;

		// Randomly generate a start population
		population.create(cfg.mu - cfg.seedStates.size(), &initial);
		population.randomizeAll(&cfg);

		// Add in seed states
		for (size_t i = 0; i < cfg.seedStates.size(); i++) {
			state* temp = new state(shapes, width, numShapes, cfg.seedStates[i]);
			population.add(temp);
		}
		g_evals = population.getSize();

		do {

			// Create lambda offsprsing
			for (int i = 0; i < cfg.lambda; i++) {
				state* temp = new state(initial);
				switch (cfg.parentSel) {
				case PARENTSEL_FITNESSPROPOTIONAL:
					population.setFpProbability();
					parent1 = population.chooseParentFP();
					parent2 = population.chooseParentFP();
				case PARENTSEL_KTOURNAMENT:
					parent1 = population.chooseParentKTourn(cfg.parentSelTournSize);
					parent2 = population.chooseParentKTourn(cfg.parentSelTournSize);
					break;
				case PARENTSEL_RANDOM:
					parent1 = population.chooseParentRandom();
					parent2 = population.chooseParentRandom();
					break;
				}
				temp->updateAdaptableParams(parent1, parent2);
				switch (cfg.recombinationType) {
				case RECOMBINATION_NPOINT:
					temp->nPointCrossover(parent1, parent2, cfg.crossovers, cfg.constraintSat);
					break;
				case RECOMBINATION_UNIFORM:
					temp->uniformCrossover(parent1, parent2, cfg.uniformProb, cfg.constraintSat);
					break;
				}				
				if (GEN_SCALED_PROB(4) <= cfg.mutationRate) {
					switch (cfg.mutationType) {
					case MUTATION_RANDOM:
						temp->randResetMutate(cfg.constraintSat);
						break;
					case MUTATION_CREEP:
						temp->creepMutate(cfg.creepDist, cfg.constraintSat);
						break;
					}
				}
				temp->setFitnessTypes(cfg.objective1, cfg.objective2);
				temp->calcFitness();
				offspring.add(temp);
				g_evals++;
			}

			// If comma survival, remove parents from population first
			if (cfg.survivalStrategy == SURVIVALSTRAT_COMMA) {
				population.destroy();
				population.empty();
			}

			// Add offspring to population
			for (int i = 0; i < offspring.getSize(); i++)
				population.add(offspring.get(i));
			offspring.empty();

			// Find Pareto optimal front
			population.calcPareto();

			// Reduce population size
			switch (cfg.survivorSel) {
			case SURVIVORSEL_TRUNCATION:
				population.reduceByTruncation(cfg.mu);
				break;
			case SURVIVORSEL_KTOURNAMENT:
				population.reduceByKTourn(cfg.mu, cfg.survivorSelTournSize);
				break;
			case SURVIVORSEL_RANDOM:
				population.reduceByRandom(cfg.mu);
				break;
			case SURVIVORSEL_FITNESSPROPORTIONAL:
				population.setFpProbability();
				population.reduceByFP(cfg.mu);
				break;
			}

			// Run termination test
			switch (cfg.termType) {
			case TERMTYPE_AVGFITNESS:
				terminate = population.termTestAvgFitness(cfg.termGensUnchanged, 2.0);
				break;
			case TERMTYPE_BESTFITNESS:
				terminate = population.termTestBestFitness(cfg.termGensUnchanged);
				break;
			case TERMTYPE_TOPDOMINATED:
				terminate = population.termTestTopDominated(cfg.termGensUnchanged);
				break;
			}

			// Log data
			log << g_evals << "\t";
			std::cout << g_evals << "\t";
			if (cfg.crossoversSA) {
				log << IO_FORMAT_FLOAT(3) << population.getAverage(CROSSOVER_POINTS) << "\t";
				std::cout << IO_FORMAT_FLOAT(3) << population.getAverage(CROSSOVER_POINTS) << "\t";
			}
			if (cfg.mutationRateSA) {
				log << IO_FORMAT_FLOAT(3) << population.getAverage(MUTATION_RATE) << "\t";
				std::cout << IO_FORMAT_FLOAT(3) << population.getAverage(MUTATION_RATE) << "\t";
			}
			if (cfg.constraintSat == CONSTRAINTSAT_PENALTY) {
				log << IO_FORMAT_FLOAT(3) << population.getAverage(PENALTY_WEIGHT) << "\t";
				std::cout << IO_FORMAT_FLOAT(3) << population.getAverage(PENALTY_WEIGHT) << "\t";
			}
			if (cfg.objective1 == OBJECTIVE_LENGTH || cfg.objective2 == OBJECTIVE_LENGTH) {
				log << IO_FORMAT_FLOAT(3) << population.getAverage(FITNESS_LENGTH) << "\t" << population.getBest(FITNESS_LENGTH, HIGHEST)->getFitness().length << "\t";
				std::cout << IO_FORMAT_FLOAT(3) << population.getAverage(FITNESS_LENGTH) << "\t" << population.getBest(FITNESS_LENGTH, HIGHEST)->getFitness().length << "\t";
			}
			if (cfg.objective1 == OBJECTIVE_WIDTH || cfg.objective2 == OBJECTIVE_WIDTH) {
				log << IO_FORMAT_FLOAT(3) << population.getAverage(FITNESS_WIDTH) << "\t" << population.getBest(FITNESS_WIDTH, HIGHEST)->getFitness().width << "\t";
				std::cout << IO_FORMAT_FLOAT(3) << population.getAverage(FITNESS_WIDTH) << "\t" << population.getBest(FITNESS_WIDTH, HIGHEST)->getFitness().width << "\t";
			}
			if (cfg.objective1 == OBJECTIVE_EDGES || cfg.objective2 == OBJECTIVE_EDGES) {
				log << IO_FORMAT_FLOAT(3) << population.getAverage(FITNESS_EDGES) << "\t" << population.getBest(FITNESS_EDGES, HIGHEST)->getFitness().edges << "\t";
				std::cout << IO_FORMAT_FLOAT(3) << population.getAverage(FITNESS_EDGES) << "\t" << population.getBest(FITNESS_EDGES, HIGHEST)->getFitness().edges << "\t";
			}
			log << std::endl;
			std::cout << std::endl;

			// End loop when termination test returns true of we hit our max number of evals
		} while (!terminate && (g_evals < cfg.fitnessEvals));

		// Check if this run has the best Pareto Front
		population.comparePareto(bestPareto);	
		
		// Clean up
		population.destroy();
		population.empty();
		offspring.destroy();
		offspring.empty();
	}

	// Print Pareto front of best solutions
	solution.open(cfg.solutionFile);
	if (!solution.is_open()) {
		std::cout << "Error: Unable to write solution file" << std::endl;
		exit(1);
	}
	solution << "Solutions in Pareto front: " << bestPareto.size() << std::endl << std::endl;
	for (size_t i = 0; i < bestPareto.size(); i++)
		solution << "Solution " << i + 1 << ":" << std::endl << bestPareto[i]->getSolutionString() << std::endl << std::endl;

	// Clean up
	delete[] shapes;

	return 0;
}
