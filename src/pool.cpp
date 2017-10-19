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
#include "pareto.h"
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
void pool::randomizeAll(config* cfg) {
	for (std::vector<state*>::iterator it = m_states.begin(); it != m_states.end(); ++it) {
		(*it)->randomize();
		(*it)->setFitnessTypes(cfg->objective1, cfg->objective2);
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
		totalFitness += m_paretoMin + 1 - (*it)->getParetoLevel();
	}

	lastProbability = 0.0f;
	m_fpProbability.clear();
	for (std::vector<state*>::iterator it = m_states.begin(); it != m_states.end(); ++it) {
		m_fpProbability.push_back(((float)(m_paretoMin + 1 - (*it)->getParetoLevel()) / (float)totalFitness) + lastProbability);
		lastProbability = m_fpProbability.back();
	}

	return;
}


/**********************************************************
*	calcPareto()
*	Calls the pareto() function to calculate a pareto structure
*	and stores it as a member variable.
**********************************************************/
void pool::calcPareto() {

	// Variables
	std::vector<std::vector<int>> paretoLevels;

	// Build a Pareto-leveled structure
	pareto(m_states, paretoLevels);

	// Walk down the structure and assign level to each state
	for (size_t i = 0; i < paretoLevels.size(); i++) {
		for (size_t j = 0; j < paretoLevels[i].size(); j++) {
			m_states[paretoLevels[i][j]]->setParetoLevel(i);
		}
	}

	m_paretoMin = (int)paretoLevels.size() - 1;

	return;
}


/**********************************************************
*	comparePareto(std::vector<state*> bestPareto)
*	Compares the current Pareto front to the passed vector
*	of a pareto front. If current is better, copies current
*	into best. "Better" is defined as the front in which a
*	larger proportion of states dominate the other.
*	and stores it as a member variable.
	 @param bestPareto vector of the current best front
**********************************************************/
void pool::comparePareto(std::vector<state*>& bestPareto) {

	// Variables
	int		cnt1 = 0;
	int		cnt2 = 0;
	float	ratio1;
	float	ratio2;

	// If best is currently empty, just copy into it
	if (bestPareto.size() == 0) {
		for (size_t i = 0; i < m_states.size(); i++) {
			if (m_states[i]->getParetoLevel() == 1) {
				state* temp = new state(*m_states[i]);
				bestPareto.push_back(temp);
			}
		}
	}
	else {

		// Count states that dominate best and vice-versa
		for (size_t i = 0; i < m_states.size(); i++) {
			if (m_states[i]->getParetoLevel() == 0) {
				for (size_t j = 0; j < bestPareto.size(); j++) {
					if (DOMINATES(m_states[i], bestPareto[j]))
						cnt1++;
					if (DOMINATES(bestPareto[j], m_states[i]))
						cnt2++;
				}
			}
		}

		// If a higher proportion of states dominate best, make this our new best
		ratio1 = (float)cnt1 / (float)m_states.size();
		ratio2 = (float)cnt2 / (float)bestPareto.size();
		if (ratio1 > ratio2) {
			for (std::vector<state*>::iterator it = m_states.begin(); it != m_states.end(); ++it)
				delete (*it);
			bestPareto.clear();
			for (size_t i = 0; i < m_states.size(); i++) {
				if (m_states[i]->getParetoLevel() == 0) {
					state* temp = new state(*m_states[i]);
					bestPareto.push_back(temp);
				}
			}
		}
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
	float value = (float)(rand() % 10000)  / 10000.0f;

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
	return m_states[kTournament(k, LOWEST, true)];
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
void pool::reduceByTruncation(size_t size) {

	// Sanity check
	if (m_states.size() < size)
		return;

	// Variables
	size_t i = 0;

	// Remove states at the lowest Pareto level until the desired size is reached
	while (m_states.size() > size) {
		if (i >= m_states.size()) {
			i = 0;
			m_paretoMin--;
		}		
		if (m_states[i]->getParetoLevel() == m_paretoMin) {
			delete m_states[i];
			m_states.erase(m_states.begin() + i);
			i--;
		}
		i++;
	}

	return;
}


/**********************************************************
*	reduceByKTourn()
*	Reduces the pool to the passed size by running k-tournaments.
*	 @param size desired size of the pool after removal
*	 @param k number of states chosen in each tournament
**********************************************************/
void pool::reduceByKTourn(size_t size, int k) {

	// Sanity check
	if (m_states.size() < size)
		return;
	
	// Variables
	int idx;

	// Run tournaments until we've shrunk to desired size
	while (m_states.size() > size) {

		// Since we're not allowing replacement, need to make sure k isn't bigger than size
		if (k > (int)m_states.size())
			k = (int)m_states.size();

		// Randomly pick k members of the pool, keeping track of the one with the worst fitness 
		idx = kTournament(k, LOWEST, false);
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
void pool::reduceByFP(size_t size) {

	// Sanity check
	if (m_states.size() < (size_t)size)
		return;

	// Variables
	int j;
	bool* survivors = new bool[m_states.size()];

	// Initialize all states to non-surviving
	for (size_t i = 0; i < m_states.size(); i++)
		survivors[i] = false;

	// Choose states to survive
	for (size_t i = 0; i < size; i++) {

		// Choose a random value [0.0 - 1.0]
		float value = (float)(rand() % 10000) / 10000.0f;

		// Walk down the FP probability array and find the corresponding state
		j = 0;
		while (m_fpProbability[j] < value && j < (int)m_states.size())
			j++;
		if (survivors[j])
			i--;
		survivors[j] = true;
	}

	// Kill off any states not chosen to survive
	for (int i = m_states.size() - 1; i >= 0; i--) {
		if (!survivors[i]) {
			delete m_states[i];
			m_states.erase(m_states.begin() + i);
		}
	}

	// Clean up
	delete[] survivors;
	int i = 0;
	return;
}


/**********************************************************
*	reduceByRandom(int size)
*	Reduces the pool to the passed size by randomly removing
*	elements.
*	 @param size desired size of the pool after removal
**********************************************************/
void pool::reduceByRandom(size_t size) {

	// Sanity check
	if (m_states.size() < size)
		return;

	// Variables 
	int idx;

	// Randomly choose states and remove them until desired size is reached
	while (m_states.size() > size) {
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
		if (best && m_states[tournament[i]]->getParetoLevel() > m_states[bestIdx]->getParetoLevel())
			bestIdx = tournament[i];
		else if (!best && m_states[tournament[i]]->getParetoLevel() < m_states[bestIdx]->getParetoLevel())
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

	float avgFitness = getAverage(FITNESS_LENGTH);

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

	int bestFitness = getBest(FITNESS_LENGTH, HIGHEST)->getFitness().length;
	
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
*	termTestTopDominated(int targetGensUnchanged)
*	Determines if the pool's number of top dominated fitness
*	states has remained	unchanged for a target number of
*	generations.
*	 @param targetGensUnchanged target number of gerenations
*	 @return false unless target generations hit
**********************************************************/
bool pool::termTestTopDominated(int targetGensUnchanged) {

	// Variables
	int cnt = 0;

	for (std::vector<state*>::iterator it = m_states.begin(); it != m_states.end(); ++it) {
		if ((*it)->getParetoLevel() == 0)
			cnt++;
	}

	// If unchanged, increment counter
	if (cnt == m_lastTopDominated)
		m_lastTopDominated++;
	else
		m_numGensUnchanged = 0;
	m_lastTopDominated = cnt;

	// Return false unless we hit our target
	if (m_numGensUnchanged < targetGensUnchanged)
		return false;

	// Clean up
	m_numGensUnchanged = 0;
	m_lastTopDominated = -1;
	return true;
}


/**********************************************************
*	getBest(int parameter, int type)
*	Returns the best paramter given. Here, "best" means 
*	either highest or lowest, depending on the type given.
*	 @param parameter state parameter we're looking for
*	 @param type HIGHEST or LOWEST
*	 @return state* pointer to the best state found
**********************************************************/
state* pool::getBest(int parameter, int type) {

	// Variables
	int		valBest;
	int		valCurrent;
	state*	ptr;
	
	// Initialize
	ptr = m_states.front();
	switch (parameter) {
	case FITNESS_LENGTH:
		valBest = ptr->getFitness().length;
		break;
	case FITNESS_WIDTH:
		valBest = ptr->getFitness().width;
		break;
	case FITNESS_EDGES:
		valBest = ptr->getFitness().edges;
		break;
	}

	// Find best fitness
	for (std::vector<state*>::iterator it = m_states.begin(); it != m_states.end(); ++it) {
		switch (parameter) {
		case FITNESS_LENGTH:
			valCurrent = (*it)->getFitness().length;
			break;
		case FITNESS_WIDTH:
			valCurrent = (*it)->getFitness().width;
			break;
		case FITNESS_EDGES:
			valCurrent = (*it)->getFitness().edges;
			break;
		}
		if ((type == HIGHEST && valCurrent > valBest) || (type == LOWEST && valCurrent < valBest)) {
			ptr = (*it);
			valBest = valCurrent;
		}
	}

	return ptr;
}


/**********************************************************
*	getAverage(int parameter)
*	Returns the average of the paramter given for all states
*	in the pool.
*	 @param parameter state parameter we're looking for
*	 @return float* average value of the parameter given
**********************************************************/
float pool::getAverage(int parameter) {
	
	// Variables
	float total = 0;

	// Calculate total fitness
	for (std::vector<state*>::iterator it = m_states.begin(); it != m_states.end(); ++it)
		switch (parameter) {
		case FITNESS_LENGTH:
			total += (float)(*it)->getFitness().length;
			break;
		case FITNESS_WIDTH:
			total += (float)(*it)->getFitness().width;
			break;
		case FITNESS_EDGES:
			total += (float)(*it)->getFitness().edges;
			break;
		case MUTATION_RATE:
			total += (*it)->getParams().mr;
			break;
		case CROSSOVER_POINTS:
			total += (float)(*it)->getParams().cp;
			break;
		case PENALTY_WEIGHT:
			total += (*it)->getParams().pw;
			break;
		}

	// Return average
	return(total / (float)m_states.size());
}
