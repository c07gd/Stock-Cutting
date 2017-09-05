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
	state(uint8_t width, uint8_t numShapes);
	state::state(std::string filename);
	~state();
};


#endif