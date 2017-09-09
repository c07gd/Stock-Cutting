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
#include <iostream>


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

	return;
}


/**********************************************************
*	getLength()
*	Finds length of the shape in its initial form. Does not
*	take into account shape the may be longer when rotated.
*	 @return length of the shape
**********************************************************/
int shape::getLength() {

	// Variables
	int		trace = 0;
	int		farRight = 0;
	int		farLeft = 0;

	// Iterate down moves vector, tracing shape
	// Mark furthest left and right positions
	for (std::vector<move>::iterator it = m_moves.begin(); it != m_moves.end(); ++it) {
		if ((*it).direction == DIR_LEFT) {
			trace -= (*it).distance;
			if (trace < farLeft)
				farLeft = trace;
		}
		else if ((*it).direction == DIR_RIGHT) {
			trace += (*it).distance;
			if (trace > farRight)
				farRight = trace;
		}
	}

	// Length is difference + 1 for start location
	return (farRight - farLeft + 1);
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
