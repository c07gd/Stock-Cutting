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
#include "cfgParse.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdlib.h>


/**********************************************************
*	Variables
**********************************************************/
extern int g_evals;
extern int g_targetEvals;


/**********************************************************
*	state()
*	Default constructor
**********************************************************/
state::state() {
	
	// Assign default values
	m_width = 0;
	m_length = 0;
	m_fitness = -1;
	m_penalty = 0;
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
	m_penalty = rhs.m_penalty;
	m_params = rhs.m_params;

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
*	Does not allocate memory for arrays. If allocation is 
*	needed, call the copy constructor first.
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
	m_penalty = rhs.m_penalty;
	m_params = rhs.m_params;

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
			m_layout[i][j] = 0;
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
	m_penalty = 0;
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

			// Check for penalty
			if (traceX < 0 || traceX >= m_width || traceY < 0 || traceY >= m_length)
				m_penalty++;
			else {
				if (m_layout[traceX][traceY])
					m_penalty++;
				m_layout[traceX][traceY] = true;
			}

			// Update fitness
			if (traceY > (m_length - m_fitness))
				m_fitness = m_length - traceY;
		}
	}

	// Discount any squares where the shape overlapped itself
	// Also bounds-check the penalty so it is never negative
	// This could happen if overlap occurs while off the layout
	m_penalty = std::max(0, m_penalty - m_shapes[i].getOverlap());

	return;
}


/**********************************************************
*	resolveConstraints(int idx, int type)
*	Handles inavlid placements. Calls the proper constraint 
*	resolution procedure based on the type passed.
*	 @param idx the index of the shape need resolution
*	 @param type the constraint resolution procedure to run
**********************************************************/
void state::resolveConstraints(int idx, int& x, int& y, int& rot, int type) {
	
	// Sanity check
	if(placementIsValid(idx, m_x[idx], m_y[idx], m_rot[idx]))
		return;
	
	// Run constraint resolution procedure based on passed type
	switch (type) {
		case CONSTRAINTSAT_REPAIR:
			repair(idx, m_x[idx], m_y[idx], m_rot[idx]);
			break;
		case CONSTRAINTSAT_RANDOM:
			do {
				m_x[idx] = rand() % m_width;
				m_y[idx] = rand() % m_length;
				m_rot[idx] = rand() % NUM_ROTS;
			} while (!placementIsValid(idx, m_x[idx], m_y[idx], m_rot[idx]));
			break;
		case CONSTRAINTSAT_PENALTY:
			break;
	}
	
	return;
}


/**********************************************************
*	repair(int& idx, int& x, int& y, int& rot)
*	Attempts a repair of the given shape placement. Spirals
*	outward from start location looking for a valid placement.
*	 @param idx the index of the shape in member array
*	 @param x the given x coordinate
*	 @param y the given y coordinate
*	 @param rot the given rotation
**********************************************************/
void state::repair(int idx, int& x, int& y, int& rot) {
	
	// Sanity check
	if(placementIsValid(idx, x, y, rot))
		return;
	
	// Variables
	bool found = false;
	
	// Spiral outward from starting point looking for a valid placement
	// Pattern is DRUULLDDDRRRUUUULLLL...
	for (int i = 0; !found; i++) {
		for (int j = 0; (j < i / 2) && !found; j++) {

			switch (i % NUM_DIRS) {
			case DIR_UP:
				x++;
				break;
			case DIR_DOWN:
				x--;
				break;
			case DIR_RIGHT:
				y++;
				break;
			case DIR_LEFT:
				y--;
				break;
			}

			// Try each rotation at these coords
			for (int k = ROT_0_DEG; k < NUM_ROTS && !found; k++) {
				if (placementIsValid(idx, x, y, rot))
					found = true;
			}

			// If nothing found, just accept the invalid state and move on
			// This should be extremely rare, I saw it get deadlocked here
			// about one in 30*10000 fitness evals
			// I can't be a very fit state if it can't find find a placement
			// (i.e. if it has a high fitness, there's plenty of room to the 
			// right to place stuff), so it won't survive long in the population
			if ((i*j*4) > (10 * RANDOM_MAX_TRIES))
				found = true;
		}
	}

	return;
}


/**********************************************************
*	calcFitness()
*	Updates the m_fitness member variable. Fitness is defined
*	as the max length minus the length used.
**********************************************************/
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

	// Subtract out weighted penalty value
	m_fitness -= (int)round((float)m_penalty * m_params.pw);

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


/**********************************************************
*	nPointCrossover(state* parent1, state* parent2, int n)
*	Assigns alleles to by choosing from each parents. N
*	crossover points are randomly selected. Also checks 
*	validity of the resulting placment and attempts a repair.
*	If n is set to -1, will use the adaptable parameter for
*	number of crossover points.
*	 @param parent1 pointer to first parent state
*	 @param parent2 pointer to second parent state
*	 @param n number of crossover points
*	 @param constraintSat method for resolving constraint violations
**********************************************************/
void state::nPointCrossover(state* parent1, state* parent2, int n, int constraintSat) {

	// Use adaptable parameter?
	if (m_params.enableCP)
		n = m_params.cp;

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
		m_x[i] = parent1->m_x[i];
		m_y[i] = parent1->m_y[i];
		m_rot[i] = parent1->m_rot[i];

		// Resolve constraints
		resolveConstraints(i, m_x[i], m_y[i], m_rot[i], constraintSat);

		// Place shape
		placeShape(i, m_x[i], m_y[i], m_rot[i]);
	}

	// Clean up
	delete[] crossoverPts;
	return;
}


/**********************************************************
*	uniformCrossover(state* parent1, state* parent2, float p)
*	Assigns each gene by choosing from parents. Each gene has
*	a "p" change of being chosen from parent1. Also checks
*	validity of the resulting placment and attempts a repair.
*	 @param parent1 pointer to first parent state
*	 @param parent2 pointer to second parent state
*	 @param p probabilitiy (0.0-1.0) of choosing parent1 vs. parent2
*	 @param constraintSat method for resolving constraint violations
**********************************************************/
void state::uniformCrossover(state* parent1, state* parent2, float p, int constraintSat) {

	//Variables
	float*	pArray;
	state*	chosenParent;

	// Generate probability array
	pArray = new float[m_numShapes];
	for (int i = 0; i < m_numShapes; i++)
		pArray[i] = float(rand() % 1000) / 1000.0f;

	// Inherit each gene from chosen parent based on probability
	for (int i = 0; i < m_numShapes; i++) {
		if (pArray[i] < p)
			chosenParent = parent1;
		else
			chosenParent = parent2;
		m_x[i] = chosenParent->m_x[i];
		m_y[i] = chosenParent->m_y[i];
		m_rot[i] = chosenParent->m_rot[i];
		
		// Resolve constraints
		resolveConstraints(i, m_x[i], m_y[i], m_rot[i], constraintSat);

		// Place shape
		placeShape(i, m_x[i], m_y[i], m_rot[i]);
	}
	
	// Clean up
	delete[] pArray;
	return;
}


/**********************************************************
*	randResetMutate()
*	Picks a random gene and assigns it a new random placement.
*	Also checks	validity of the resulting placment and
*	attempts a repair.
*	 @param constraintSat method for resolving constraint violations
**********************************************************/
void state::randResetMutate(int constraintSat) {

	// Variables
	int idx;

	// Pick a random gene
	idx = rand() % m_numShapes;

	// Choose new random coordinates
	m_x[idx] = rand() % m_width;
	m_y[idx] = rand() % m_length;
	m_rot[idx] = rand() % NUM_ROTS;
	
	// Resolve constraints
	resolveConstraints(idx, m_x[idx], m_y[idx], m_rot[idx], constraintSat);
	
	// Place shape
	placeShape(idx, m_x[idx], m_y[idx], m_rot[idx]);

	return;
}


/**********************************************************
*	creepMutate(int creepDist)
*	Picks a random allele on a random gene and moves it by
*	a small amount. Also checks validity of the resulting
*	placment and attempts a repair.
*	 @param creepDist max distance the chosen allele can creep
*	 @param constraintSat method for resolving constraint violations
**********************************************************/
void state::creepMutate(int creepDist, int constraintSat) {

	// Pick random values
	int idx = rand() % m_numShapes;
	int attrib = rand() % 3;
	int creep = rand() % (2 * creepDist) - creepDist;
	
	// Apply creep
	switch (attrib) {
	case 0:
		m_x[idx] += creepDist;
		break;
	case 1:
		m_y[idx] += creepDist;
		break;
	case 2:
		m_rot[idx] += abs(creepDist % 4);
		break;
	}
	
	// Resolve constraints
	resolveConstraints(idx, m_x[idx], m_y[idx], m_rot[idx], constraintSat);

	// Place shape
	placeShape(idx, m_x[idx], m_y[idx], m_rot[idx]);

	return;
}


/**********************************************************
*	updateAdaptableParams(state* parent1, state* parent2)
*	Evolves adaptable parameters based on recombination
*	and mutation from passed parents.
*	 @param parent1 pointer to first parent state
*	 @param parent2 pointer to second parent state
**********************************************************/
void state::updateAdaptableParams(state* parent1, state* parent2) {
#define MUTATION_STEP_SIZE 0.02f
#define MAX_PENALTY 10.0f
#define PENALTY_STEP_SIZE 0.2f

	// Penalty weight
	if (m_params.enablePW) {
		switch (m_params.typePW) {
		case PENALTYWEIGHT_TIME:
			m_params.pw = MAX_PENALTY * (float)g_evals / (float)g_targetEvals;
			break;
		case PENALTYWEIGHT_SA:
			if (rand() % 2)
				m_params.pw = parent1->m_params.pw;
			else
				m_params.pw = parent2->m_params.pw;
			if (GEN_SCALED_PROB(4) <= m_params.mr) {
				if (rand() % 2)
					m_params.pw += PENALTY_STEP_SIZE;
				else
					m_params.pw -= PENALTY_STEP_SIZE;
				m_params.pw = std::max(0.0f, m_params.pw);
			}
			break;
		case PENALTYWEIGHT_FIXED:
			break;
		}
	}

	// Mutation rate
	if (m_params.enableMR) {
		// Choose a parent's mutation rate to inherit
		if (rand() % 2)
			m_params.mr = parent1->m_params.mr;
		else
			m_params.mr = parent2->m_params.mr;

		// Mutate our mutation rate (whoa... that's meta...)
		if (GEN_SCALED_PROB(4) <= m_params.mr) {
			if (rand() % 2)
				m_params.mr += MUTATION_STEP_SIZE;
			else
				m_params.mr -= MUTATION_STEP_SIZE;
		}
	}

	// Crossover points
	if (m_params.enableCP) {

		// Choose a parent's number of crossover points to inherit
		if (rand() % 2)
			m_params.cp = parent1->m_params.cp;
		else
			m_params.cp = parent2->m_params.cp;

		// Mutate our number of crossover points
		if (GEN_SCALED_PROB(4) <= m_params.mr) {
			if (rand() % 2)
				m_params.cp++;
			else
				m_params.cp--;
			m_params.cp = std::max(0, m_params.cp);
		}
	}

	return;
#undef MUTATION_STEP_SIZE
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
		out << m_y[i] << "," << m_x[i] << "," << m_rot[i] << std::endl;

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
	for (int i = m_width-1; i >= 0; i--) {
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
