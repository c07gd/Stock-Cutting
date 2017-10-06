/******************************************************************************
*	shape.cpp
*
*	Definitions for the shape class for the Stock Cutting Problem.
*
*	Stuart Miller
*	Missouri S&T CS 5401
*	Fall 2017
******************************************************************************/


/**********************************************************
*	Headers
**********************************************************/
#include "shape.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>


/**********************************************************
*	shape(const char* line)
*	Custom constructor
*	 @param line c-string containing a shape data
**********************************************************/
shape::shape(const char* line) {

	// Variables
	int		i = 0;
	move	m;

	// Parse each line and add to moves vector
	do {
		while (line[i] == ' ')
			i++;
		switch (line[i]) {
		case 'U':
		case 'u':
			m.direction = DIR_UP;
			break;
		case 'R':
		case 'r':
			m.direction = DIR_RIGHT;
			break;
		case 'D':
		case 'd':
			m.direction = DIR_DOWN;
			break;
		case 'L':
		case 'l':
			m.direction = DIR_LEFT;
			break;
		}
		i++;
		m.distance = line[i] - '0';	// TODO: handling for two-digit distances
		i++;
		m_moves.push_back(m);
	} while (line[i] != '\n' && line[i] != '\0' && line[i] != '\r');

	// Update attributes
	calcOverlap();
	calcLength();

	return;
}


/**********************************************************
*	calcLength()
*	Finds the maximum diameter of the shape (side-to-side,
*	or top-to-bottom) and assigns the value to m_length.
**********************************************************/
void shape::calcLength() {

	// Variables
	int		traceLR = 0;
	int		traceUD = 0;
	int		farRight = 0;
	int		farLeft = 0;
	int		farUp = 0;
	int		farDown = 0;

	// Iterate down moves vector, tracing shape
	// Mark furthest left and right positions
	for (std::vector<move>::iterator it = m_moves.begin(); it != m_moves.end(); ++it) {
		if ((*it).direction == DIR_LEFT) {
			traceLR -= (*it).distance;
			if (traceLR < farLeft)
				farLeft = traceLR;
		}
		else if ((*it).direction == DIR_RIGHT) {
			traceLR += (*it).distance;
			if (traceLR > farRight)
				farRight = traceLR;
		}
		else if ((*it).direction == DIR_UP) {
			traceUD += (*it).distance;
			if (traceUD > farUp)
				farUp = traceUD;
		}
		else if ((*it).direction == DIR_DOWN) {
			traceUD -= (*it).distance;
			if (traceUD > farDown)
				farDown = traceUD;
		}
	}

	// Length is difference + 1 for start location
	m_length = std::max((farRight - farLeft + 1), (farUp - farDown + 1));

	return;
}


/**********************************************************
*	calcOverlap()
*	Finds the number of squares for which the shape traces
*	over itself. This is useful for discounting trace overlaps
*	from the penalty function later.
**********************************************************/
void shape::calcOverlap() {
	
	// Variables
	std::vector<int>	xVals;
	std::vector<int>	yVals;
	int					traceX = 0;
	int					traceY = 0;

	// Mark start position
	m_overlap = 0;
	xVals.push_back(traceX);
	yVals.push_back(traceY);

	// Trace out shape
	for (std::vector<move>::iterator it = m_moves.begin(); it != m_moves.end(); ++it) {
		for (int i = 0; i < (*it).distance; i++) {
			switch ((*it).direction) {
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

			// Check if we've marked this location before
			for (int i = 0; i < (int)xVals.size(); i++) {
				if (xVals[i] == traceX && yVals[i] == traceY) {
					m_overlap++;
					break;
				}
			}

			// Mark our location
			xVals.push_back(traceX);
			yVals.push_back(traceY);
		}
	}

	return;
}


/**********************************************************
*	print()
*	Prints a shapes moves. Format is <direction> <distance>
*	For debugging only
**********************************************************/
void shape::print() {
	for (std::vector<move>::iterator it = m_moves.begin(); it != m_moves.end(); ++it)
		std::cout << (*it).direction << (int)(*it).distance << " ";
	return;
}

/**********************************************************
*	readInputFile(std::string filename, shape*& shapes, int& width, int& numShapes)
*	Parses shape input file
*	 @param filename name of the shape input file
*	 @param shapes pointer to the shapes array
*	 @param width width value to be written to
*	 @param numShapes numShapes value to be written to
**********************************************************/
void readInputFile(std::string filename, shape*& shapes, int& width, int& numShapes) {

	// Variables
	std::ifstream	in;
	std::string		line;
	int				i;

	// Open input file
	in.open(filename);
	if (!in.is_open()) {
		std::cout << "Error: Unable to read shape input file" << std::endl;
		exit(1);
	}

	// Read first line (width & numShapes)
	in >> width;
	in >> numShapes;
	in.ignore(64, '\n');

	// Construct shapes array
	shapes = new shape[numShapes];

	// Read in file line-by-line and assign moves
	i = 0;
	while (getline(in, line)) {
		shapes[i] = shape(line.c_str());
		i++;
	}

	// Clean up
	in.close();

	return;
}
