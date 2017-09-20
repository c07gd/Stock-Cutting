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


void pool::create(int size, state* initial) {
	for (int i = 0; i < size; i++) {
		state* temp = new state(*initial);
		m_states.push_back(temp);
	}

	return;
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

state* pool::chooseFpParent() {

	// Choose a random value [0.0 - 1.0]
	float value = (float)(rand() % 100000)  / 100000;

	// Walk down the FP probability array and find the corresponding parent
	size_t i = 0;
	while (m_fpProbability[i] < value && i < m_states.size()) {
		i++;
	}

	return m_states[i];
}


void pool::truncate(int size) {

	// Sort pool
	std::sort(m_states.begin(), m_states.end(), compareState);

	// Remove from bottom until shrunk to desired size
	m_states.resize(size);

	return;
}