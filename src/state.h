#ifndef STATE_H
#define STATE_H


#include "shape.h"
#include <cstdint>
#include <string>


class state {
private:
	uint8_t			m_width;
	uint8_t			m_length;
	bool**			m_layout;
	uint8_t			m_numShapes;
	shape_layout*	m_shapes;
public:
	state::state(std::string filename);
	~state();
	void randomize(unsigned int seed);
	bool placementIsValid(shape* s, int x, int y, int rot);
	void placeShape(int i, int x, int y, int rot);
	void printSolution(std::string filename);
	void printLayout(std::string filename);
};


#endif