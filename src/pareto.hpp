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
#define GET_PARAM_1(x)	((x)->getFitness().length)
#define GET_PARAM_2(x)	((x)->getFitness().width)
#define EQUAL(a, b)		(GET_PARAM_1(a) == GET_PARAM_1(b) && GET_PARAM_2(a) == GET_PARAM_2(b))
#define DOMINATES(a, b)	(GET_PARAM_1(a) >= GET_PARAM_1(b) && GET_PARAM_2(a) >= GET_PARAM_2(b) && !EQUAL(a, b))


/**********************************************************
*	Function Declarations
**********************************************************/
template <typename T, typename A>
void pareto(std::vector<T, A> pool, std::vector<std::vector<int>>& pareto);
template <typename T, typename A>
static void paretoBump(std::vector<std::vector<int>>& pareto, size_t level, std::vector<T, A> pool, int i);
void printPareto(std::vector<std::vector<int>> pareto);


/**********************************************************
*	pareto(std::vector<T> pool)
*	Calculates and returns a level-ordered pareto structure
*	of reference indices. The first index of the vector is
*	the level, the second is the index of the state in the
*	pool
*	 @param pool input pool of states
*	 @param pareto Pareto output structure
**********************************************************/
template <typename T, typename A>
void pareto(std::vector<T, A> pool, std::vector<std::vector<int>>& pareto) {
	
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
static void paretoBump(std::vector<std::vector<int>>& pareto, size_t level, std::vector<T, A> pool, int i) {

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
void printPareto(std::vector<std::vector<int>> pareto) {

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
