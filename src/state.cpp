#include "state.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>


state::state() {
	m_width = 0;
	m_length = 0;
	m_fitness = -1;
	m_numShapes = 0;
	m_layout = NULL;
	m_shapes = NULL;
	m_x = NULL;
	m_y = NULL;
	m_rot = NULL;
}

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

state::~state() {
	for (int i = 0; i < m_width; i++)
		delete[] m_layout[i];
	delete[] m_layout;
	delete[] m_x;
	delete[] m_y;
	delete[] m_rot;
}

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
			switch (rotate((*it).direction, rot)) {
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
			switch (rotate((*it).direction, rot)) {
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
}

void state::randomize(unsigned int seed) {
	
	// Variables
	int x;
	int y;
	int rot;
	int layoutLength;

	// Seed randomizer
	srand(seed);

	// Walk down shape array choosing and verifying random layouts
	for (int i = 0; i < m_numShapes; i++) {
		do {
			x = rand() % m_width;
			y = rand() % m_length;
			rot = rand() % NUM_ROTS;
		} while (!placementIsValid(i, x, y, rot));
		placeShape(i, x, y, rot);
	}

	// Calculate length used
	bool exit = false;
	for (int i = m_length - 1; i >= 0 && !exit; i--) {
		for (int j = 0; j < m_width && !exit; j++) {
			if (m_layout[j][i]) {
				layoutLength = i + 1;
				exit = true;
			}
		}
	}

	// Calculate fitness
	m_fitness = m_length - layoutLength;

	return;
}


void state::printSolution(std::string filename) {

	// Variables
	std::ofstream out;

	// Open output file
	out.open(filename);
	if (!out.is_open()) {
		std::cout << "Error: Unable to write output file";
		return;
	}

	// Walk down shapes array and output data
	for (int i = 0; i < m_numShapes; i++)
		out << m_x[i] << "," << m_y[i] << "," << m_rot[i] << std::endl;

	// Clean up
	out.close();

	return;
}

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
		std::cout << "Error: Unable to write output file";
		return;
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