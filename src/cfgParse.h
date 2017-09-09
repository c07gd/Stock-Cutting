#ifndef PARSECONFIG_H
#define PARSECONFIG_H

#include <string>
#include <fstream>
#include <iostream>

#define COMMENT_CHAR			('#')
#define LABEL_SEEDFROMTIME		("seedFromTime")
#define LABEL_SEED				("seed")
#define LABEL_ALGORITHM			("algorithm")
#define LABEL_RUNS				("runs")
#define LABEL_FITNESSEVALS		("fitnessEvals")
#define LABEL_LOGFILE			("logFile")
#define LABEL_SOLUTIONFILE		("solutionFile")
#define LABEL_INPUTFILE			("inputFile")

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

inline config getConfig(std::string filename) {
	config cfg;
	std::string line, lhs, rhs;
	std::size_t split;
	std::ifstream in;
	
	in.open(filename);
	if (!in.is_open()) {
		std::cout << "Error: Unable to open config file" << std::endl;
		exit(1);
	}
	while (getline(in, line)) {
		if (line[0] == COMMENT_CHAR)
			continue;
		if (line.length() > 1 && line[line.length() - 1] == '\r')
			line.pop_back();
		split = line.find("=");
		if (split != std::string::npos) {
			lhs = line.substr(0, split);
			rhs = line.substr(split + 1);
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
