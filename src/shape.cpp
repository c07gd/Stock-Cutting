#include "shape.h"
#include <cstdlib>
#include <iostream>


shape::shape(char* line) {
	uint8_t i = 0;
	move	m;
	do {
		while (line[i] == ' ')
			i++;
		m.direction = line[i];
		i++;
		m.distance = line[i] - '0';	// TODO: handling for two-digit distances
		i++;
		moves.push_back(m);
	} while (line[i] != '\n' && line[i] != '\0');
}


shape::~shape() {
}


void shape::print() {
	for (std::vector<move>::iterator it = moves.begin(); it != moves.end(); ++it) {
		std::cout << (*it).direction << (int)(*it).distance << " ";
	}
}