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
#ifndef PARSECONFIG_H
#define PARSECONFIG_H


/**********************************************************
*	Headers
**********************************************************/
#include <string>
#include <fstream>
#include <iostream>


/**********************************************************
*	Compiler Constants
**********************************************************/
#define COMMENT_CHAR			('#')
#define LABEL_SEEDFROMTIME		("seedFromTime")
#define LABEL_SEED				("seed")
#define LABEL_ALGORITHM			("algorithm")
#define LABEL_RUNS				("runs")
#define LABEL_FITNESSEVALS		("fitnessEvals")
#define LABEL_LOGFILE			("logFile")
#define LABEL_SOLUTIONFILE		("solutionFile")
#define LABEL_INPUTFILE			("inputFile")
#define DEFAULT_SEEDFROMTIME	(false)
#define DEFAULT_SEED			(123456789)
#define	DEFAULT_ALGORITHM		('R')
#define DEFAULT_RUNS			(1)
#define DEFAULT_FITNESSEVALS	(1)
#define DEFAULT_LOGFILE			("logs/default.txt")
#define DEFAULT_SOLUTIONFILE	("solutions/default.txt")
#define DEFAULT_INPUTFILE		("patterns/default.txt")


/**********************************************************
*	Types, Etc.
**********************************************************/
enum {
	RANDOM_SEARCH,
	// more search types to be added later...
	INVALID
};

struct config {
	bool			seedFromTime;
	unsigned int	seed;
	char			algorithm;
	int				runs;
	int				fitnessEvals;
	std::string		logFile;
	std::string		solutionFile;
	std::string		inputFile;
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
	cfg.seedFromTime	= DEFAULT_SEEDFROMTIME;
	cfg.seed			= DEFAULT_SEED;
	cfg.algorithm		= DEFAULT_ALGORITHM;
	cfg.runs			= DEFAULT_RUNS;
	cfg.fitnessEvals	= DEFAULT_FITNESSEVALS;
	cfg.logFile			= DEFAULT_LOGFILE;
	cfg.solutionFile	= DEFAULT_SOLUTIONFILE;
	cfg.inputFile		= DEFAULT_INPUTFILE;
	
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
			else if (lhs == LABEL_ALGORITHM) {
				switch (rhs[0]) {
				case 'R':
				case 'r':
					cfg.algorithm = RANDOM_SEARCH;
					break;
				default:
					cfg.algorithm = INVALID;
					break;
				}
			}
		}
	}

	return cfg;
}


#endif
