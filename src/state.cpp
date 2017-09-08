#include "state.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>


state::state(shape* shapes, int width, int numShapes) {
	
	// Copy data over to members
	m_shapes = shapes;
	m_width = width;
	m_numShapes = numShapes;

	// Calculate max length
	m_length = 0;
	for (int i = 0; i < m_width; i++) {
		m_length += m_shapes[i].getLength();
	}

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
	for (int i = 0; i < m_width; i++) {
		m_x[i] = 0;
		m_y[i] = 0;
		m_rot[i] = ROT_0_DEG;
	}
}

state::~state() {
	for (int i = 0; i < m_width; i++)
		delete[] m_layout[i];
	delete[] m_layout;
	delete[] m_x;
	delete[] m_y;
	delete[] m_rot;
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
			switch ((*it).direction) {
			case UP:
				traceX++;
				break;
			case DOWN:
				traceX--;
				break;
			case LEFT:
				traceY--;
				break;
			case RIGHT:
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
			switch ((*it).direction) {
			case UP:
				traceX++;
				break;
			case DOWN:
				traceX--;
				break;
			case LEFT:
				traceY--;
				break;
			case RIGHT:
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

	// Seed randomizer
	srand(seed);

	// Walk down shape array choosing and verifying random layouts
	for (int i = 0; i < m_numShapes; i++) {
		do {
			x = rand() % m_width;
			y = rand() % m_length;
			rot = 0;// rand() % NUM_ROTS;
		} while (!placementIsValid(i, x, y, rot));
		placeShape(i, x, y, rot);
	}
}

void state::printSolution(std::string filename) {

	// Variables
	std::ofstream out;

	// Open output file
	out.open(filename, std::ifstream::out);
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
	
	// Open output file
	out.open(filename, std::ifstream::out);
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