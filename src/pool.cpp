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

enum {
	HIGHEST,
	LOWEST
};

void pool::create(int size, state* initial) {
	for (int i = 0; i < size; i++) {
		state* temp = new state(*initial);
		m_states.push_back(temp);
	}

	return;
}

void pool::empty(){
	m_states.clear();
	m_lastAvgFitness = 0.0f;
	m_lastBestFitness = 0;
	m_numGensUnchanged = 0;
}

void pool::randomizeAll() {
	for (std::vector<state*>::iterator it = m_states.begin(); it != m_states.end(); ++it) {
		(*it)->randomize();
		(*it)->calcFitness();
	}

	return;
}

void pool::setFpProbability() {

	// Variables
	int		totalFitness;
	float	lastProbability;

	// Get the sum of fitness values
	totalFitness = 0;
	for (std::vector<state*>::iterator it = m_states.begin(); it != m_states.end(); ++it) {
		totalFitness += (*it)->getFitness();
	}

	lastProbability = 0.0;
	m_fpProbability.clear();
	for (std::vector<state*>::iterator it = m_states.begin(); it != m_states.end(); ++it) {
		m_fpProbability.push_back((float)(*it)->getFitness() / (float)totalFitness + lastProbability);
		lastProbability = m_fpProbability.back();
	}

	return;
}

state* pool::chooseParentFP() {

	// Choose a random value [0.0 - 1.0]
	float value = (float)(rand() % 100000)  / 100000;

	// Walk down the FP probability array and find the corresponding parent
	size_t i = 0;
	while (m_fpProbability[i] < value && i < m_states.size()) {
		i++;
	}

	return m_states[i];
}

state* pool::chooseParentKTourn(int k) {
	return m_states[kTournament(k, HIGHEST)];
}


void pool::reduceByTruncation(int size) {

	// Sort pool
	std::sort(m_states.begin(), m_states.end(), compareState);

	// Remove from bottom until shrunk to desired size
	m_states.resize(size);

	return;
}

void pool::reduceByKTourn(int size, int k) {
	
	// Variables
	int idx;

	// Run tournaments until we've shrunk to desired size
	while (m_states.size() > (size_t)size) {

		// Randomly pick k members of the pool, keeping track of the one with the best fitness 
		idx = kTournament(k, LOWEST);
		m_states.erase(m_states.begin() + idx);

	}

	return;
}

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

float pool::getAverageFitness() {

	// Variables
	int		totalFitness = 0;

	// Calculate total fitness
	for (std::vector<state*>::iterator it = m_states.begin(); it != m_states.end(); ++it)
		totalFitness += (*it)->getFitness();

	// Return average
	return (float)totalFitness / (float)m_states.size();
}