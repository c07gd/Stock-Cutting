#include "state.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>


state::state(std::string filename) {
	int x;
	std::string s;
	std::ifstream in(filename, std::ios_base::in);
	if (!in.is_open()) {
		std::cout << "Error: Unable to open file";
		return;
	}
	in >> x;
	m_width = (uint8_t)x;
	in >> x;
	m_numShapes = (uint8_t)x;
	m_shapes = new shape_layout[m_numShapes];
	in.ignore(64, '\n');
	m_length = 0;
	int i = 0;
	while (getline(in, s)) {
		m_shapes[i].shape = new shape(s.c_str());
		m_shapes[i].x = 0;
		m_shapes[i].y = 0;
		m_shapes[i].rot = 0;
		m_length += m_shapes[i].shape->getWidth();
		i++;
	}
	m_layout = new bool*[m_width];
	for (i = 0; i < m_width; i++) {
		m_layout[i] = new bool[m_length];
		for (int j = 0; j < m_length; j++) {
			m_layout[i][j] = false;
		}
	}
	in.close();		
}

state::~state() {
	for (int i = 0; i < m_width; i++)
		free(m_layout[i]);
	free(m_layout);
	free(m_shapes);
}

bool state::placementIsValid(shape* s, int x, int y, int rot) {
	
	// Variables
	std::vector<move>* moves = s->getMoves();
	int traceX = x;
	int traceY = y;

	// If we're starting in an already taken square, just return
	if (m_layout[traceX][traceY])
		return false;
	
	// Trace shape and verify it doesn't overlap or go off the layout
	for (std::vector<move>::iterator it = moves->begin(); it != moves->end(); ++it) {
		switch ((*it).direction) {
		case UP:
			traceX += (*it).distance;
			break;
		case DOWN:
			traceX -= (*it).distance;
			break;
		case LEFT:
			traceY -= (*it).distance;
			break;
		case RIGHT:
			traceY += (*it).distance;
			break;
		}
		if (traceX < 0 || traceX >= m_width || traceY < 0 || traceY >= m_length || m_layout[traceX][traceY])
			return false;
	}
	return true;
}

void state::placeShape(int i, int x, int y, int rot) {
	
	// Variables
	std::vector<move>* moves = m_shapes[i].shape->getMoves();
	int traceX = x;
	int traceY = y;

	// Assign coordinates
	m_shapes[i].x = x;
	m_shapes[i].y = y;
	m_shapes[i].rot = rot;

	// Mark layout
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
	
	// Declarations
	int x, y, rot;

	// Seed randomizer
	srand(seed);

	// Walk down shape array choosing and verifying random layouts
	for (int i = 0; i < m_numShapes; i++) {
		do {
			x = rand() % m_width;
			y = rand() % m_length;
			rot = 0;// rand() % NUM_ROTS;
		} while (!placementIsValid(m_shapes[i].shape, x, y, rot));
		placeShape(i, x, y, rot);
	}
}

void state::printSolution(std::string filename) {
	std::ofstream out(filename, std::ios_base::out);
	if (!out.is_open()) {
		std::cout << "Error: Unable to write output file";
		return;
	}
	for (int i = 0; i < m_numShapes; i++) {
		out << ((int)m_shapes[i].y) << ", " << ((int)m_shapes[i].x) << ", " << ((int)m_shapes[i].rot) << std::endl;
	}
	out.close();
	return;
}

void state::printLayout(std::string filename) {
	std::ofstream out(filename, std::ios_base::out);
	if (!out.is_open()) {
		std::cout << "Error: Unable to write output file";
		return;
	}
	for (int i = 0; i < m_width; i++) {
		for (int j = 0; j < m_length; j++) {
			if (m_layout[i][j])
				out << "#";
			else
				out << ".";
		}
		out << "\n";
	}
	out.close();
	return;
}