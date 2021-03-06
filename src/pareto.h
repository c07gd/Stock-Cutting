/******************************************************************************
*	pareto.hpp
*
*	Functions for the Pareto optimum finding for the Stock Cutting Problem.
*
*	Stuart Miller
*	Missouri S&T CS 5401
*	Fall 2017
******************************************************************************/
#ifndef PARETO_H
#define PARETO_H


/**********************************************************
*	Headers
**********************************************************/
#include <vector>


/**********************************************************
*	compiler Constants
**********************************************************/
#define PARETO_DEBUG	(0)
#define EQUAL(a, b)		(getObjective(a, 0) == getObjective(b, 0) && getObjective(a, 1) == getObjective(b, 1))
#define DOMINATES(a, b)	(getObjective(a, 0) >= getObjective(b, 0) && getObjective(a, 1) >= getObjective(b, 1) && !EQUAL(a, b))


/**********************************************************
*	int getObjective(state* s, bool obj)
*	Returns given state's value for the given objective.
*	 @param state* state to get
*	 @param obj objective to get (0 for first, 1 for second)
**********************************************************/
inline int getObjective(state* s, bool obj) {
	switch (obj ? s->getFitness().objective1 : s->getFitness().objective2) {
	case OBJECTIVE_LENGTH:
		return s->getFitness().length;
		break;
	case OBJECTIVE_WIDTH:
		return s->getFitness().width;
		break;
	case OBJECTIVE_EDGES:
		return s->getFitness().edges;
		break;
	}
	return 0;
};


template <typename T, typename A>
inline void logPareto(std::vector<T, A> pool, std::string filename) {

	// Variables	
	std::ofstream out;
	
	// Open output file
	out.open(filename);
	if (!out.is_open()) {
		std::cout << "Error: Unable to write Pareto output file" << std::endl;;
		exit(1);
	}
	
	// Log all in the Pareto front
	for (size_t i = 0; i < pool.size(); i++) {
		if (pool[i]->getParetoLevel() == 0)
			out << getObjective(pool[i], 0) << "," << getObjective(pool[i], 1) << std::endl;
	}

	// Clean up
	out.close();
	return;
}


/**********************************************************
*	void pareto(std::vector<T, A> pool, std::vector<std::vector<int>>& pareto)
*	Calculates and returns a level-ordered pareto structure
*	of reference indices. The first index of the vector is
*	the level, the second is the index of the state in the
*	pool
*	 @param pool input pool of states
*	 @param pareto Pareto output structure
**********************************************************/
template <typename T, typename A>
inline void pareto(std::vector<T, A> pool, std::vector<std::vector<int>>& pareto) {
	
	// Variables
	bool added, nextLevel;

	// Add first level and push first state in pool to it
	std::vector<int> temp;
	temp.push_back(0);
	pareto.push_back(temp);

	// Loop through everyone in the pool
	for (size_t i = 1; i < pool.size(); i++) {
		added = false;

		// Loop through all levels of our pareto structure
		for (size_t j = 0; j < pareto.size(); j++) {

			// Loop through everyone at thie level
			nextLevel = false;
			for (size_t k = 0; k < pareto[j].size() && !nextLevel; k++) {

				// Does the kth element at this level dominate i?
				if (DOMINATES(pool[pareto[j][k]], pool[i])) {
					nextLevel = true;
				}
			}
			if (!nextLevel) {

				// Check if this will bump anyone out of this level
				paretoBump(pareto, j, pool, i);

				// Add ID to the level
				pareto[j].push_back(i);
				added = true;

				break;
			}
		}

		// If we made it to the end of the bottom level without adding, create a new level and add there
		if (!added) {
			std::vector<int> temp;
			temp.push_back(i);
			pareto.push_back(temp);
		}

		#if PARETO_DEBUG
		std::cout << "Round " << i << std::endl;
		printPareto(pareto);
		std::cout << std::endl;
		#endif
	}
	
	return;
}


/**********************************************************
*	paretoBump(std::vector<std::vector<int>>& pareto, size_t level, std::vector<T>  pool, int i)
*	Shifts an index down one level on the Pareto structure.
*	Checks if this downward bump would cause any cascading
*	bumps on that level. Calls itself recursively.
*	 @param pareto Pareto output structure
*	 @param level level the index is cbeing bumped down to
*	 @param pool input pool of states
*	 @param i index of the state being bumped
**********************************************************/
template <typename T, typename A>
inline static void paretoBump(std::vector<std::vector<int>>& pareto, size_t level, std::vector<T, A> pool, int i) {

	for (size_t k = 0; k < pareto[level].size(); k++) {

		// Does our new member here dominate anybody already at this level?
		if (DOMINATES(pool[i], pool[pareto[level][k]])) {

			// If this is the lowest level, make another level
			if ((level + 1) >= pareto.size()) {
				std::vector<int> temp;
				pareto.push_back(temp);
			}

			// Remove dominated from this level, check if if would dominate
			// anyone at its new level, then add it to that level. Also move
			// k back one index since we just removed from the vector
			int temp = pareto[level][k];
			pareto[level].erase(pareto[level].begin() + k);
			paretoBump(pareto, level + 1, pool, temp);
			pareto[level + 1].push_back(temp);
			k--;
		}
	}

	return;
}


/**********************************************************
*	printPareto(std::vector<std::vector<int>> pareto)
*	Pretty prints the passed Pareto structure to the std out.
*	FOR DEBUGGING ONLY. 
*	 @param pareto the Pareto output structure
**********************************************************/
inline void printPareto(std::vector<std::vector<int>> pareto) {

	for (size_t i = 0; i < pareto.size(); i++) {
		std::cout << i + 1 << ":\t";
		for (size_t j = 0; j < pareto[i].size(); j++) {
			std::cout << pareto[i][j] + 1 << ", ";
		}
		std::cout << std::endl;
	}

	return;
}


#endif
