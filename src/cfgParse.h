/******************************************************************************
*	cfgParse.h
*
*	Values and logic for parsing a configuration file for the Stock Cutting
*	Problem.
*
*	Stuart Miller
*	Missouri S&T CS 5401
*	Fall 2017
******************************************************************************/
#ifndef CFGPARSE_H
#define CFGPARSE_H


/**********************************************************
*	Headers
**********************************************************/
#include "cfgDefault.h"
#include <string>
#include <fstream>
#include <iostream>


/**********************************************************
*	Compiler Constants
**********************************************************/
#define COMMENT_CHAR				('#')
#define LABEL_SEEDFROMTIME			("seedFromTime")
#define LABEL_SEED					("seed")
#define LABEL_SEEDSTATE				("seedState")
#define LABEL_ALGORITHM				("algorithm")
#define LABEL_RUNS					("runs")
#define LABEL_FITNESSEVALS			("fitnessEvals")
#define LABEL_LOGFILE				("logFile")
#define LABEL_SOLUTIONFILE			("solutionFile")
#define LABEL_INPUTFILE				("inputFile")
#define LABEL_MU					("mu")
#define LABEL_LAMBDA				("lambda")
#define LABEL_SURVIVALSTRATEGY		("survivalStrategy")
#define LABEL_RECOMBINATIONTYPE		("recombinationType")
#define LABEL_CROSSOVERS			("crossovers")
#define LABEL_CROSSOVERSSA			("crossoversSA")
#define LABEL_UNIFORMPROB			("uniformProb")
#define LABEL_MUTATIONTYPE			("mutationType")
#define LABEL_MUTATIONRATE			("mutationRate")
#define LABEL_MUTATIONRATESA		("mutationRateSA")
#define LABEL_CREEPDIST				("creepDist")
#define LABEL_PARENTSEL				("parentSel")
#define LABEL_PARENTSELTOURNSIZE	("parentSelTournSize")
#define LABEL_SURVIVORSEL			("survivorSel")
#define LABEL_SURVIVORSELTOURNSIZE	("survivorSelTournSize")
#define LABEL_CONSTRAINTSAT			("constraintSat")
#define LABEL_PENALTYWEIGHT			("penaltyWeight")
#define LABEL_PENALTYWEIGHTSA		("penaltyWeightSA")
#define LABEL_TERMTYPE				("termType")
#define LABEL_TERMGENSUNCHANGED		("termGensUnchanged")
#define LABEL_OBJECTIVE1			("objective1")
#define LABEL_OBJECTIVE2			("objective2")


/**********************************************************
*	Types, Etc.
**********************************************************/
struct config {
	int			seedFromTime;
	unsigned int	seed;
	std::vector<std::string>
					seedStates;
	char			algorithm;
	int				runs;
	int				fitnessEvals;
	std::string		logFile;
	std::string		solutionFile;
	std::string		inputFile;
	int				survivalStrategy;
	int				mu;
	int				lambda;
	int				recombinationType;
	int				crossovers;
	bool			crossoversSA;
	float			uniformProb;
	int				mutationType;
	float			mutationRate;
	bool			mutationRateSA;
	int				creepDist;
	int				parentSel;
	int				parentSelTournSize;
	int				survivorSel;
	int				survivorSelTournSize;
	int				constraintSat;
	float			penaltyWeight;
	int				penaltyWeightSA;
	int				termType;
	int				termGensUnchanged;
	int				objective1;
	int				objective2;
};

enum {
	SEED_STATIC,
	SEED_TIME	
};

enum {
	SURVIVALSTRAT_PLUS,
	SURVIVALSTRAT_COMMA
};

enum {
	RECOMBINATION_NPOINT,
	RECOMBINATION_UNIFORM
};

enum {
	MUTATION_RANDOM,
	MUTATION_CREEP
};

enum {
	PARENTSEL_FITNESSPROPOTIONAL,
	PARENTSEL_KTOURNAMENT,
	PARENTSEL_RANDOM
};

enum {
	SURVIVORSEL_TRUNCATION,
	SURVIVORSEL_KTOURNAMENT,
	SURVIVORSEL_RANDOM,
	SURVIVORSEL_FITNESSPROPORTIONAL
};

enum {
	CONSTRAINTSAT_RANDOM,
	CONSTRAINTSAT_REPAIR,
	CONSTRAINTSAT_PENALTY
};

enum {
	PENALTYWEIGHT_FIXED,
	PENALTYWEIGHT_TIME,
	PENALTYWEIGHT_SA
};

enum {
	TERMTYPE_NUMEVALS,
	TERMTYPE_AVGFITNESS,
	TERMTYPE_BESTFITNESS,
	TERMTYPE_TOPDOMINATED
};

enum {
	OBJECTIVE_LENGTH,
	OBJECTIVE_WIDTH,
	OBJECTIVE_EDGES
};


/**********************************************************
*	getConfig(std::string filename)
*	Parses and returns configuration data
*	 @param filename name of the configuration file
*	 @return filled config sturct if successful
**********************************************************/
inline config getConfig(std::string filename) {

	// Variables
	config			cfg;
	std::string		line;
	std::string		lhs;
	std::string		rhs;
	std::size_t		split;
	std::ifstream	in;

	// Assign default values
	cfg.seedFromTime		= DEFAULT_SEEDFROMTIME;
	cfg.seed				= DEFAULT_SEED;
	cfg.algorithm			= DEFAULT_ALGORITHM;
	cfg.runs				= DEFAULT_RUNS;
	cfg.fitnessEvals		= DEFAULT_FITNESSEVALS;
	cfg.logFile				= DEFAULT_LOGFILE;
	cfg.solutionFile		= DEFAULT_SOLUTIONFILE;
	cfg.inputFile			= DEFAULT_INPUTFILE;
	cfg.survivalStrategy	= DEFAULT_SURVIVALSTRATEGY;
	cfg.mu					= DEFAULT_MU;
	cfg.lambda				= DEFAULT_LAMBDA;
	cfg.recombinationType	= DEFAULT_RECOMBINATIONTYPE;
	cfg.crossovers			= DEFAULT_CROSSOVERS;
	cfg.uniformProb			= DEFAULT_UNIFORMPROB;
	cfg.mutationType		= DEFAULT_MUTATIONTYPE;
	cfg.mutationRate		= DEFAULT_MUTATIONRATE;	
	cfg.mutationRateSA		= DEFAULT_MUTATIONRATESA;
	cfg.crossovers			= DEFAULT_CREEPDIST;
	cfg.crossoversSA		= DEFAULT_CROSSOVERSSA;
	cfg.parentSel			= DEFAULT_PARENTSEL;
	cfg.parentSelTournSize	= DEFAULT_PARENTSELTOURNSIZE;
	cfg.survivorSel			= DEFAULT_SURVIVORSEL;
	cfg.survivorSelTournSize= DEFAULT_SURVIVORSELTOURNSIZE;
	cfg.constraintSat		= DEFAULT_CONSTRAINTSAT;
	cfg.penaltyWeight		= DEFAULT_PENALTYWEIGHT;
	cfg.penaltyWeightSA		= DEFAULT_PENALTYWEIGHTSA;
	cfg.termType			= DEFAULT_TERMTYPE;
	cfg.termGensUnchanged	= DEFAULT_TERMGENSUNCHANGED;
	cfg.objective1			= DEFAULT_OBJCTIVE1;
	cfg.objective2			= DEFAULT_OBJCTIVE2;
	cfg.seedStates.clear();
	
	// Open configuration file
	in.open(filename);
	if (!in.is_open()) {
		std::cout << "Error: Unable to open config file" << std::endl;
		exit(1);
	}

	// Read in line-by-line
	while (getline(in, line)) {

		// Skip comment lines
		if (line[0] == COMMENT_CHAR)
			continue;

		// Handling for Windows-style line endings
		if (line.length() > 1 && line[line.length() - 1] == '\r')
			line.pop_back();

		// Split line into token(lhs) and value(rhs)
		split = line.find("=");
		if (split != std::string::npos) {
			lhs = line.substr(0, split);
			rhs = line.substr(split + 1);

			// Parse each configuration value and fill config struct
			if (lhs == LABEL_SEEDFROMTIME)
				cfg.seedFromTime = atoi(rhs.c_str());
			else if (lhs == LABEL_SEED)
				cfg.seed = (unsigned int)atoll(rhs.c_str());
			else if (lhs == LABEL_RUNS)
				cfg.runs = atoi(rhs.c_str());
			else if (lhs == LABEL_FITNESSEVALS)
				cfg.fitnessEvals = atoi(rhs.c_str());
			else if (lhs == LABEL_LOGFILE)
				cfg.logFile = rhs;
			else if (lhs == LABEL_SOLUTIONFILE)
				cfg.solutionFile = rhs;
			else if (lhs == LABEL_INPUTFILE)
				cfg.inputFile = rhs;
			else if (lhs == LABEL_MU)
				cfg.mu = atoi(rhs.c_str());
			else if (lhs == LABEL_LAMBDA)
				cfg.lambda = atoi(rhs.c_str());
			else if (lhs == LABEL_SURVIVALSTRATEGY)
				cfg.survivalStrategy = atoi(rhs.c_str());
			else if (lhs == LABEL_RECOMBINATIONTYPE)
				cfg.recombinationType = atoi(rhs.c_str());
			else if (lhs == LABEL_CROSSOVERS)
				cfg.crossovers = atoi(rhs.c_str());
			else if (lhs == LABEL_CROSSOVERSSA)
				cfg.crossoversSA = (bool)atoi(rhs.c_str());
			else if (lhs == LABEL_UNIFORMPROB)
				cfg.uniformProb = (float)atof(rhs.c_str());
			else if (lhs == LABEL_MUTATIONTYPE)
				cfg.mutationType = atoi(rhs.c_str());
			else if (lhs == LABEL_MUTATIONRATE)
				cfg.mutationRate = (float)atof(rhs.c_str());
			else if (lhs == LABEL_MUTATIONRATESA)
				cfg.mutationRateSA = (bool)atoi(rhs.c_str());
			else if (lhs == LABEL_CREEPDIST)
				cfg.creepDist = atoi(rhs.c_str());
			else if (lhs == LABEL_PARENTSEL)
				cfg.parentSel = atoi(rhs.c_str());
			else if (lhs == LABEL_PARENTSELTOURNSIZE)
				cfg.parentSelTournSize = atoi(rhs.c_str());
			else if (lhs == LABEL_SURVIVORSEL)
				cfg.survivorSel = atoi(rhs.c_str());
			else if (lhs == LABEL_SURVIVORSELTOURNSIZE)
				cfg.survivorSelTournSize = atoi(rhs.c_str());
			else if (lhs == LABEL_CONSTRAINTSAT)
				cfg.constraintSat = atoi(rhs.c_str());
			else if (lhs == LABEL_PENALTYWEIGHT)
				cfg.penaltyWeight = (float)atof(rhs.c_str());
			else if (lhs == LABEL_PENALTYWEIGHTSA)
				cfg.penaltyWeightSA = atoi(rhs.c_str());
			else if (lhs == LABEL_TERMTYPE)
				cfg.termType = atoi(rhs.c_str());
			else if (lhs == LABEL_TERMGENSUNCHANGED)
				cfg.termGensUnchanged = atoi(rhs.c_str());
			else if (lhs == LABEL_SEEDSTATE)
				cfg.seedStates.push_back(rhs);
			else if (lhs == LABEL_OBJECTIVE1)
				cfg.objective1 = atoi(rhs.c_str());
			else if (lhs == LABEL_OBJECTIVE2)
				cfg.objective2 = atoi(rhs.c_str());
		}
	}

	return cfg;
}


#endif
