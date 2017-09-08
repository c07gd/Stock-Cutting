#include "shape.h"
#include <cstdlib>
#include <iostream>


shape::shape(const char* line) {
	uint8_t i = 0;
	move	m;
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
		moves.push_back(m);
	} while (line[i] != '\n' && line[i] != '\0');
}


int shape::getLength() {
	int trace = 0;
	int farRight = 0;
	int farLeft = 0;
	for (std::vector<move>::iterator it = moves.begin(); it != moves.end(); ++it) {
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
	return (farRight - farLeft + 1);
}

void shape::print() {
	for (std::vector<move>::iterator it = moves.begin(); it != moves.end(); ++it) {
		std::cout << (*it).direction << (int)(*it).distance << " ";
	}
}