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
#define LABEL_ALGORITHM				("algorithm")
#define LABEL_RUNS					("runs")
#define LABEL_FITNESSEVALS			("fitnessEvals")
#define LABEL_LOGFILE				("logFile")
#define LABEL_SOLUTIONFILE			("solutionFile")
#define LABEL_INPUTFILE				("inputFile")
#define LABEL_MU					("mu")
#define LABEL_LAMBDA				("lambda")
#define LABEL_PARENTSEL				("parentSel")
#define LABEL_PARENTSELTOURNSIZE	("parentSelTournSize")
#define LABEL_SURVIVORSEL			("survivorSel")
#define LABEL_SURVIVORSELTOURNSIZE	("survivorSelTournSize")
#define LABEL_MUTATIONRATE			("mutationRate")
#define LABEL_CROSSOVERS			("crossovers")
#define LABEL_TERMEVALS				("termEvals")
#define LABEL_TERMTYPE				("termType")
#define LABEL_TERMGENSUNCHANGED		("termGensUnchanged")


/**********************************************************
*	Types, Etc.
**********************************************************/
struct config {
	bool			seedFromTime;
	unsigned int	seed;
	char			algorithm;
	int				runs;
	int				fitnessEvals;
	std::string		logFile;
	std::string		solutionFile;
	std::string		inputFile;
	int				mu;
	int				lambda;
	int				parentSel;
	int				parentSelTournSize;
	int				survivorSel;
	int				survivorSelTournSize;
	float			mutationRate;
	int				crossovers;
	int				termEvals;
	int				termType;
	int				termGensUnchanged;
};

enum {
	PARENTSEL_FITNESSPROPOTIONAL,
	PARENTSEL_KTOURNAMENT
} parentSelection;

enum {
	SURVIVORSEL_TRUNCATION,
	SURVIVORSEL_KTOURNAMENT
} survivorSelection;

enum {
	TERMTYPE_NUMEVALS,
	TERMTYPE_AVGFITNESS,
	TERMTYPE_BESTFITNESS
} termType;


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
	cfg.mu					= DEFAULT_MU;
	cfg.lambda				= DEFAULT_LAMBDA;
	cfg.parentSel			= DEFAULT_PARENTSEL;
	cfg.parentSelTournSize	= DEFAULT_PARENTSELTOURNSIZE;
	cfg.survivorSel			= DEFAULT_SURVIVORSEL;
	cfg.survivorSelTournSize= DEFAULT_SURVIVORSELTOURNSIZE;
	cfg.mutationRate		= DEFAULT_MUTATIONRATE;
	cfg.crossovers			= DEFAULT_CROSSOVERS;
	cfg.termType			= DEFAULT_TERMTYPE;
	cfg.termGensUnchanged	= DEFAULT_TERMGENSUNCHANGED;
	
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
				cfg.seedFromTime = (rhs == "true" || rhs == "True");
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
			else if (lhs == LABEL_PARENTSEL)
				cfg.parentSel = atoi(rhs.c_str());
			else if (lhs == LABEL_PARENTSELTOURNSIZE)
				cfg.parentSelTournSize = atoi(rhs.c_str());
			else if (lhs == LABEL_SURVIVORSEL)
				cfg.survivorSel = atoi(rhs.c_str());
			else if (lhs == LABEL_SURVIVORSELTOURNSIZE)
				cfg.survivorSelTournSize = atoi(rhs.c_str());
			else if (lhs == LABEL_CROSSOVERS)
				cfg.crossovers = atoi(rhs.c_str());
			else if (lhs == LABEL_MUTATIONRATE)
				cfg.mutationRate = (float)atof(rhs.c_str());
			else if (lhs == LABEL_TERMTYPE)
				cfg.termType = atoi(rhs.c_str());
			else if (lhs == LABEL_TERMGENSUNCHANGED)
				cfg.termGensUnchanged = atoi(rhs.c_str());
		}
	}

	return cfg;
}


#endif
