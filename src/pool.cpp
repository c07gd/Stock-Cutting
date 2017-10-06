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
*	Returns a pointer to a chonsen state. Choice is based on
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


/**********************************************************
*	chooseParentKTourn(int k)
*	Returns a pointer to a chosen state. Choice is based on 
*	the best state found amount k states, chosen randomly
*	with replacement.
*	 @param k size of the k-tournament
**********************************************************/
state* pool::chooseParentKTourn(int k) {
	return m_states[kTournament(k, true, true)];
}


/**********************************************************
*	chooseParentRandom()
*	Returns a pointer to a randomly chosen state.
**********************************************************/
state* pool::chooseParentRandom() {
	return m_states[rand() % m_states.size()];
}


/**********************************************************
*	reduceByTruncation()
*	Reduces the pool to the passed size by removing elements
*	with the lowest fitness.
*	 @param size desired size of the pool after removal
**********************************************************/
void pool::reduceByTruncation(int size) {

	// Sanity check
	if (m_states.size() < (size_t)size)
		return;

	// Sort pool
	std::sort(m_states.begin(), m_states.end(), compareState);

	// Remove from bottom until shrunk to desired size
	m_states.resize(size);

	return;
}


/**********************************************************
*	reduceByKTourn()
*	Reduces the pool to the passed size by running k-tournaments.
*	 @param size desired size of the pool after removal
*	 @param k number of states chosen in each tournament
**********************************************************/
void pool::reduceByKTourn(int size, int k) {

	// Sanity check
	if (m_states.size() < (size_t)size)
		return;
	
	// Variables
	int idx;

	// Run tournaments until we've shrunk to desired size
	while (m_states.size() > (size_t)size) {

		// Since we're not allowing replacement, need to make sure k isn't bigger than size
		if (k > (int)m_states.size())
			k = (int)m_states.size();

		// Randomly pick k members of the pool, keeping track of the one with the worst fitness 
		idx = kTournament(k, false, false);
		delete m_states[idx];
		m_states.erase(m_states.begin() + idx);
	}

	return;
}


/**********************************************************
*	reduceByFP()
*	Reduces the problem to the passed size by choosing
*	survivors based on fitnes proportion. Removes and destroys
*	any states not chosen in FP.
*	 @param size desired size of the pool after FP reduction
**********************************************************/
void pool::reduceByFP(int size) {

	// Sanity check
	if (m_states.size() < (size_t)size)
		return;

	// Variables
	int i, j;
	bool* survivors = new bool[size];

	// Initialize all states to non-surviving
	for (i = 0; i < size; i++)
		survivors[i] = false;

	// Choose states to survive
	for (i = 0; i < size; i++) {

		// Choose a random value [0.0 - 1.0]
		float value = (float)(rand() % 100000) / 100000.0f;

		// Walk down the FP probability array and find the corresponding state
		j = 0;
		while (m_fpProbability[j] < value && j < (int)m_states.size())
			j++;
		survivors[j] = true;
	}

	// Kill off any states not chosen to survive
	for (i = size - 1; i >= 0; i--) {
		if (!survivors[i]) {
			delete m_states[i];
			m_states.erase(m_states.begin() + i);
		}
	}

	// Clean up
	delete[] survivors;
	return;
}


/**********************************************************
*	reduceByRandom(int size)
*	Reduces the pool to the passed size by randomly removing
*	elements.
*	 @param size desired size of the pool after removal
**********************************************************/
void pool::reduceByRandom(int size) {

	// Sanity check
	if (m_states.size() < (size_t)size)
		return;

	// Variables 
	int idx;

	// Randomly choose states and remove them until desired size is reached
	while ((int)m_states.size() > size) {
		idx = rand() % m_states.size();
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
int pool::kTournament(int k, bool best, bool replacement) {

	// Variables
	int*	tournament = new int[k];
	int		bestIdx;
	bool	inTournament;

	// Randomly pick members for the tournament, with or without replacement
	for (int i = 0; i < k; i++) {
		tournament[i] = rand() % m_states.size();
		if (!replacement) {
			inTournament = false;
			for (int j = 0; j < i && !inTournament; j++) {
				if (tournament[j] == tournament[i]) {
					i--;
					break;
				}
			}
		}
	}

	// Run tournament, looking for best or worst fitness	
	bestIdx = tournament[0];
	for (int i = 1; i < k; i++) {
		if (best && m_states[tournament[i]]->getFitness() > m_states[bestIdx]->getFitness())
			bestIdx = tournament[i];
		else if (!best && m_states[tournament[i]]->getFitness() < m_states[bestIdx]->getFitness())
			bestIdx = tournament[i];
	}

	// Clean up
	delete[] tournament;
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


float pool::getAveragePenaltyWeight() {

	// Variables
	float	totalPenaltyWeight = 0;

	// Calculate total fitness
	for (std::vector<state*>::iterator it = m_states.begin(); it != m_states.end(); ++it)
		totalPenaltyWeight += (*it)->getParams().penaltyWeight;

	// Return average
	return totalPenaltyWeight / (float)m_states.size();
}


float pool::getAverageMutationRate() {

	// Variables
	float	totalMutationRate = 0.0f;

	// Calculate total fitness
	for (std::vector<state*>::iterator it = m_states.begin(); it != m_states.end(); ++it)
		totalMutationRate += (*it)->getParams().mutationRate;

	// Return average
	return totalMutationRate / (float)m_states.size();
}


float pool::getAverageCrossoverPoints() {

	// Variables
	int	totalCrossoverPoints = 0;

	// Calculate total fitness
	for (std::vector<state*>::iterator it = m_states.begin(); it != m_states.end(); ++it)
		totalCrossoverPoints += (*it)->getParams().crossoverPoints;

	// Return average
	return (float)totalCrossoverPoints / (float)m_states.size();
}
