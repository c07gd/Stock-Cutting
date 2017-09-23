/******************************************************************************
*	pool.cpp
*
*	Definitions for the pool class for the Stock Cutting Problem.
*
*	Stuart Miller
*	Missouri S&T CS 5401
*	Fall 2017
******************************************************************************/


/**********************************************************
*	Headers
**********************************************************/
#include "pool.h"
#include <algorithm>


/**********************************************************
*	Types, Etc.
**********************************************************/
enum {
	HIGHEST,
	LOWEST
};


/**********************************************************
*	create(int size, state* initial)
*	Creates new state copies of initial and adds them to
*	the pool
*	 @param size number of states to add to the pool
*	 @param initial pointer to state for each new state to copy 
**********************************************************/
void pool::create(int size, state* initial) {
	for (int i = 0; i < size; i++) {
		state* temp = new state(*initial);
		m_states.push_back(temp);
	}

	return;
}


/**********************************************************
*	empty()
*	Empties the pool. Does not destory the states in the pool.
*	Also resets all meta variables.
**********************************************************/
void pool::empty(){
	m_states.clear();
	m_lastAvgFitness = 0.0f;
	m_lastBestFitness = 0;
	m_numGensUnchanged = 0;

	return;
}


/**********************************************************
*	destroy()
*	Destroys all states in the pool. Does not empty the 
*	vector or have any other effect on the pool.
**********************************************************/
void pool::destroy() {
	for (std::vector<state*>::iterator it = m_states.begin(); it != m_states.end(); ++it)
		delete (*it);

	return;
}


/**********************************************************
*	randomizeAll()
*	Assigns random placements to all states in the pool.
*	Also calculates the new fitness for each.
**********************************************************/
void pool::randomizeAll() {
	for (std::vector<state*>::iterator it = m_states.begin(); it != m_states.end(); ++it) {
		(*it)->randomize();
		(*it)->calcFitness();
	}

	return;
}


/**********************************************************
*	setFpProbability()
*	Updates the fitness proportional vector. Each element
*	holds a ordered slice of (0.0-1.0) with width eqaul to
*	elementFitness/totalFitness
**********************************************************/
void pool::setFpProbability() {

	// Variables
	int		totalFitness;
	float	lastProbability;

	// Get the sum of fitness values
	totalFitness = 0;
	for (std::vector<state*>::iterator it = m_states.begin(); it != m_states.end(); ++it) {
		totalFitness += (*it)->getFitness();
	}

	lastProbability = 0.0f;
	m_fpProbability.clear();
	for (std::vector<state*>::iterator it = m_states.begin(); it != m_states.end(); ++it) {
		m_fpProbability.push_back((float)(*it)->getFitness() / (float)totalFitness + lastProbability);
		lastProbability = m_fpProbability.back();
	}

	return;
}


/**********************************************************
*	chooseParentFP()
*	Returns a pointer to a chonse state. Choice is based on
*	the probabilities assigns in setFpProbability(). As such,
*	setFpProbability() must have been called before this.
**********************************************************/
state* pool::chooseParentFP() {

	// Choose a random value [0.0 - 1.0]
	float value = (float)(rand() % 100000)  / 100000.0f;

	// Walk down the FP probability array and find the corresponding parent
	size_t i = 0;
	while (m_fpProbability[i] < value && i < m_states.size())
		i++;

	return m_states[i];
}

state* pool::chooseParentKTourn(int k) {
	return m_states[kTournament(k, HIGHEST)];
}


/**********************************************************
*	reduceByTruncation()
*	Reduces the pool to the passed size by removing elements
*	with the lowest fitness. Elements removed are not destroyed.
*	 @param size desired size of the pool after removal
**********************************************************/
void pool::reduceByTruncation(int size) {

	// Sort pool
	std::sort(m_states.begin(), m_states.end(), compareState);

	// Remove from bottom until shrunk to desired size
	m_states.resize(size);

	return;
}


/**********************************************************
*	reduceByKTourn()
*	Reduces the pool to the passed size by running k-tournaments.
*	Elements removed are not destroyed.
*	 @param size desired size of the pool after removal
*	 @param k number of states chosen in each tournament
**********************************************************/
void pool::reduceByKTourn(int size, int k) {
	
	// Variables
	int idx;

	// Run tournaments until we've shrunk to desired size
	while (m_states.size() > (size_t)size) {

		// Randomly pick k members of the pool, keeping track of the one with the best fitness 
		idx = kTournament(k, LOWEST);
		delete m_states[idx];
		m_states.erase(m_states.begin() + idx);

	}

	return;
}


/**********************************************************
*	kTournament(int k, int type)
*	Randomly picks k states and returns the index of the 
*	states one with the highest or lowest fitness bassed
*	on which type was passed.
*	 @param k number of states chosen in in the tournament
*	 @param type either HIGHEST or LOWEST for which element to return
*	 @return index of the chosen state
**********************************************************/
int pool::kTournament(int k, int type) {

	// Variables
	int idx;
	int bestIdx = rand() % m_states.size();

	// Randomly pick k members of the pool, keeping track of the one with the best fitness 
	for (int i = 1; i < k; i++) {
		idx = rand() % m_states.size();
		if (type == HIGHEST && m_states[idx]->getFitness() > m_states[bestIdx]->getFitness())
			bestIdx = idx;
		else if (type == LOWEST && m_states[idx]->getFitness() < m_states[bestIdx]->getFitness())
			bestIdx = idx;
	}

	return bestIdx;
}


/**********************************************************
*	termTestAvgFitness(int targetGensUnchanged, float unchangedVariance)
*	Determines if the pool's average fitness has remained
*	unchanged for a target number of generations. Allows
*	for a variance in average before resetting the
*	generation count.
*	 @param targetGensUnchanged target number of gerenations
*	 @param unchangedVariance allowed variance for each average value
*	 @return false unless target generations hit
**********************************************************/
bool pool::termTestAvgFitness(int targetGensUnchanged, float unchangedVariance) {

	float avgFitness = getAverageFitness();

	// If unchanged, increment counter
	if (abs(avgFitness - m_lastAvgFitness) < unchangedVariance)
		m_numGensUnchanged++;
	else
		m_numGensUnchanged = 0;
	m_lastAvgFitness = avgFitness;

	// Return false unless we hit our target
	if (m_numGensUnchanged < targetGensUnchanged)
		return false;

	// Clean up
	m_numGensUnchanged = 0;
	m_lastAvgFitness = -1;
	return true;
}


/**********************************************************
*	termTestBestFitness(int targetGensUnchanged)
*	Determines if the pool's best fitness has remained
*	unchanged for a target number of generations.
*	 @param targetGensUnchanged target number of gerenations
*	 @return false unless target generations hit
**********************************************************/
bool pool::termTestBestFitness(int targetGensUnchanged) {

	int bestFitness = getFittestState()->getFitness();
	
	// If unchanged, increment counter
	if (bestFitness == m_lastBestFitness)
		m_numGensUnchanged++;
	else
		m_numGensUnchanged = 0;
	m_lastBestFitness = bestFitness;

	// Return false unless we hit our target
	if (m_numGensUnchanged < targetGensUnchanged)
		return false;
	
	// Clean up
	m_numGensUnchanged = 0;
	m_lastBestFitness = -1;
	return true;
}


/**********************************************************
*	getFittestState()
*	Finds the fittest state in the pool.
*	 @return pointer to the fittest state in the pool
**********************************************************/
state* pool::getFittestState() {
	
	// Variables
	int		bestFitness = -1;
	state*	bestFitnessPtr;

	// Find best fitness
	for (std::vector<state*>::iterator it = m_states.begin(); it != m_states.end(); ++it) {
		if ((*it)->getFitness() > bestFitness) {
			bestFitness = (*it)->getFitness();
			bestFitnessPtr = (*it);
		}
	}

	return bestFitnessPtr;
}


/**********************************************************
*	getFittestState()
*	Finds the average fitness of all states in the pool.
*	 @return average fitness of all states in the pool
**********************************************************/
float pool::getAverageFitness() {

	// Variables
	int		totalFitness = 0;

	// Calculate total fitness
	for (std::vector<state*>::iterator it = m_states.begin(); it != m_states.end(); ++it)
		totalFitness += (*it)->getFitness();

	// Return average
	return (float)totalFitness / (float)m_states.size();
}
