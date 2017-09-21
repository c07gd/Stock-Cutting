/******************************************************************************
*	state.cpp
*
*	Definitions for the state class for the Stock Cutting Problem.
*
*	Stuart Miller
*	Missouri S&T CS 5401
*	Fall 2017
******************************************************************************/


/**********************************************************
*	Headers
**********************************************************/
#include "state.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdlib.h>


/**********************************************************
*	state()
*	Default constructor
**********************************************************/
state::state() {
	
	// Assign default values
	m_width = 0;
	m_length = 0;
	m_fitness = -1;
	m_numShapes = 0;
	m_layout = NULL;
	m_shapes = NULL;
	m_x = NULL;
	m_y = NULL;
	m_rot = NULL;

	return;
}


/**********************************************************
*	state(shape* shapes, int width, int numShapes)
*	Custom constructor
*	 @param shapes pointer to an array of shapes
*	 @param width width of the stock
*	 @param numShapes length of the shapes array
**********************************************************/
state::state(shape* shapes, int width, int numShapes) {
	
	// Copy data over to members
	m_shapes = shapes;
	m_width = width;
	m_numShapes = numShapes;

	// Calculate max length
	m_length = 0;
	for (int i = 0; i < m_width; i++)
		m_length += m_shapes[i].getLength();

	// Construct arrays
	constructArrays();

	return;
}


/**********************************************************
*	state(const state& rhs)
*	Copy constructor
*	 @param rhs state to be copied
**********************************************************/
state::state(const state& rhs) {

	// Copy data over
	m_length = rhs.m_length;
	m_width = rhs.m_width;
	m_numShapes = rhs.m_numShapes;
	m_shapes = rhs.m_shapes;
	m_fitness = rhs.m_fitness;

	// Construct arrays
	constructArrays();

	// Copy array data over
	for (int i = 0; i < m_numShapes; i++) {
		m_x[i] = rhs.m_x[i];
		m_y[i] = rhs.m_y[i];
		m_rot[i] = rhs.m_rot[i];
	}

	return;
}


/**********************************************************
*	operator=(const state &rhs)
*	Assignment operator
*	 @param rhs state to be copied
*	 @return state being copied into
**********************************************************/
state& state::operator=(const state &rhs) {
	
	// Test for self assignment
	if (this == &rhs)
		return *this;

	// Copy data over
	m_length = rhs.m_length;
	m_width = rhs.m_width;
	m_numShapes = rhs.m_numShapes;
	m_shapes = rhs.m_shapes;
	m_fitness = rhs.m_fitness;
	
	// Construct arrays
	constructArrays();

	// Copy array data over
	for (int i = 0; i < m_numShapes; i++) {
		m_x[i] = rhs.m_x[i];
		m_y[i] = rhs.m_y[i];
		m_rot[i] = rhs.m_rot[i];
	}

	return *this;
}


/**********************************************************
*	~state()
*	Destructor
**********************************************************/
state::~state() {
	for (int i = 0; i < m_width; i++)
		delete[] m_layout[i];
	delete[] m_layout;
	delete[] m_x;
	delete[] m_y;
	delete[] m_rot;
}


/**********************************************************
*	constructArrays()
*	Allocates memory for arrays. Only called statically.
**********************************************************/
void state::constructArrays() {

	// Construct layout array
	m_layout = new bool*[m_width];
	for (int i = 0; i < m_width; i++) {
		m_layout[i] = new bool[m_length];
		for (int j = 0; j < m_length; j++) {
			m_layout[i][j] = false;
		}
	}

	// Construct layout data arrays
	m_x = new int[m_numShapes];
	m_y = new int[m_numShapes];
	m_rot = new int[m_numShapes];

	// Initialize layout data
	for (int i = 0; i < m_numShapes; i++) {
		m_x[i] = 0;
		m_y[i] = 0;
		m_rot[i] = ROT_0_DEG;
	}
}


/**********************************************************
*	placementIsValid(int i, int x, int y, int rot)
*	Checks if given shape placement is valid
*	 @param i the index of the shape in member array
*	 @param x the given x coordinate
*	 @param y the given y coordinate
*	 @param rot the given rotation
*	 @return true if placement is valid
**********************************************************/
bool state::placementIsValid(int i, int x, int y, int rot) {
	
	// Variables
	std::vector<move>* moves;
	int traceX = x;
	int traceY = y;

	// Make sure start position is valid
	if (traceX < 0 || traceX >= m_width || traceY < 0 || traceY >= m_length || m_layout[traceX][traceY])
		return false;
	
	// Trace moves and verify it doesn't overlap or go off the layout
	moves = m_shapes[i].getMoves();
	for (std::vector<move>::iterator it = moves->begin(); it != moves->end(); ++it) {
		for (int j = 0; j < (*it).distance; j++) {
			switch (rotateShape((*it).direction, rot)) {
			case DIR_UP:
				traceX++;
				break;
			case DIR_DOWN:
				traceX--;
				break;
			case DIR_LEFT:
				traceY--;
				break;
			case DIR_RIGHT:
				traceY++;
				break;
			}
			if (traceX < 0 || traceX >= m_width || traceY < 0 || traceY >= m_length || m_layout[traceX][traceY])
				return false;
		}
	}
	return true;
}


/**********************************************************
*	placeShape(int i, int x, int y, int rot)
*	Marks a shape as placed on layout and data arrays
*	 @param i the index of the shape in member array
*	 @param x the given x coordinate
*	 @param y the given y coordinate
*	 @param rot the given rotation
**********************************************************/
void state::placeShape(int i, int x, int y, int rot) {
	
	// Variables
	std::vector<move>* moves;
	int traceX = x;
	int traceY = y;

	// Assign coordinates
	m_x[i] = x;
	m_y[i] = y;
	m_rot[i] = rot;

	// Trace moves and mark layout
	moves = m_shapes[i].getMoves();
	m_layout[traceX][traceY] = true;
	for (std::vector<move>::iterator it = moves->begin(); it != moves->end(); ++it) {
		for (int j = 0; j < (*it).distance; j++) {
			switch (rotateShape((*it).direction, rot)) {
			case DIR_UP:
				traceX++;
				break;
			case DIR_DOWN:
				traceX--;
				break;
			case DIR_LEFT:
				traceY--;
				break;
			case DIR_RIGHT:
				traceY++;
				break;
			}
			m_layout[traceX][traceY] = true;
		}
	}

	return;
}

void state::calcFitness() {

	// Variables
	bool exit;
	
	// Walk down the layout starting at the far end
	exit = false;
	for (int i = m_length - 1; i >= 0 && !exit; i--) {
		for (int j = 0; j < m_width && !exit; j++) {

			// When we hit a marked square, we're done
			// Calculate and assign fitness (total length - used length)
			if (m_layout[j][i]) {
				m_fitness = m_length - i - 1;
				exit = true;
			}
		}
	}

	return;
}


/**********************************************************
*	randomize(unsigned int seed)
*	Places all shapes in a random valid spot on the layout
*	 @param seed seed for the random number generator
**********************************************************/
void state::randomize() {
	
	// Variables
	int x;
	int y;
	int rot;
	int count;

	// Walk down shape array choosing and verifying random layouts
	for (int i = 0; i < m_numShapes; i++) {
		count = 0;
		do {
			x = rand() % m_width;
			y = rand() % m_length;
			rot = rand() % NUM_ROTS;
			count++;
		} while (!placementIsValid(i, x, y, rot) && count < RANDOM_MAX_TRIES);

		// If no valid placement found within max tries, restart layout
		if (count >= RANDOM_MAX_TRIES) {
			for (int i = 0; i < m_width; i++) {
				for (int j = 0; j < m_length; j++)
					m_layout[i][j] = false;
			}
			i = 0;
		}

		// Otherwise placement is valid and we can assign
		else
			placeShape(i, x, y, rot);
	}

	return;
}

void state::nPointCrossOver(state* parent1, state* parent2, int n) {

	// Variables
	int*	crossoverPts = new int[n];

	// Generate swap points
	if (n > m_numShapes)
		n = m_numShapes;
	for (int i = 0; i < n; i++)
		crossoverPts[i] = rand() % m_numShapes;
	std::sort(crossoverPts, crossoverPts + n);

	// Walk down chromosome and copy genes into offspring
	int j = 0;
	for (int i = 0; i < m_numShapes; i++) {

		// Every time we hit a crossover point, swap parents
		if (i >= crossoverPts[j] && j < n) {
			std::swap(parent1, parent2);
			j++;
		}

		// Check validity of new placement
		// If invalid, randomize until valid
		m_x[i] = parent1->m_x[i];
		m_y[i] = parent1->m_y[i];
		m_rot[i] = parent1->m_rot[i];
		while (!placementIsValid(i, m_x[i], m_y[i], m_rot[i])) {
			m_x[i] = rand() % m_width;
			m_y[i] = rand() % m_length;
			m_rot[i] = rand() % NUM_ROTS;
		}
		placeShape(i, m_x[i], m_y[i], m_rot[i]);
	}

	// Clean up
	delete[] crossoverPts;
	return;
}

void state::mutate() {

	// Variables
	int x;
	int y;
	int rot;
	int count;
	int idx;

	do {

		// Pick a random gene
		idx = rand() % m_numShapes;

		// Assign new random coordinates until valid
		count = 0;
		do {
			x = rand() % m_width;
			y = rand() % m_length;
			rot = rand() % NUM_ROTS;
			count++;
		} while (!placementIsValid(idx, x, y, rot) && count < RANDOM_MAX_TRIES);

	// If we couldn't find a valid assignment after max tries, pick a new gene to mutate
	} while (count >= RANDOM_MAX_TRIES);

	placeShape(idx, x, y, rot);

	return;
}


/**********************************************************
*	printSolution(std::string filename)
*	Prints a solution file with one line for each shape
*	Format is <x>,<y>,<rot>
*	 @param filename name of the solution file to write
**********************************************************/
void state::printSolution(std::string filename) {

	// Variables
	std::ofstream out;

	// Open output file
	out.open(filename);
	if (!out.is_open()) {
		std::cout << "Error: Unable to write solution file" << std::endl;
		exit(1);
	}

	// Walk down shapes array and output data
	for (int i = 0; i < m_numShapes; i++)
		out << m_x[i] << "," << m_y[i] << "," << m_rot[i] << std::endl;

	// Clean up
	out.close();

	return;
}


/**********************************************************
*	printLayout(std::string filename)
*	Prints the layout to a file
*	 @param filename name of the layout file to write
**********************************************************/
void state::printLayout(std::string filename) {

	// Variables
	std::ofstream out;

	// Mark layout array
	// (this isn't copied new new states automatically)
	for (int i = 0; i < m_numShapes; i++)
		placeShape(i, m_x[i], m_y[i], m_rot[i]);
	
	// Open output file
	out.open(filename);
	if (!out.is_open()) {
		std::cout << "Error: Unable to write layout file" << std::endl;;
		exit(1);
	}

	// Print out layout array
	for (int i = 0; i < m_width; i++) {
		for (int j = 0; j < m_length; j++) {
			if (m_layout[i][j])
				out << "#";
			else
				out << ".";
		}
		out << "\n";
	}

	// Clean up
	out.close();

	return;
}
