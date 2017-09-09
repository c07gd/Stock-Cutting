#ifndef STATE_H
#define STATE_H


#include "shape.h"
#include <string>


class state {
private:
	int			m_width;
	int			m_length;
	bool**		m_layout;
	int			m_fitness;
	int			m_numShapes;
	shape*		m_shapes;
	int*		m_x;
	int*		m_y;
	int*		m_rot;
	void constructArrays();
public:
	state();
	state(shape* shapes, int width, int numShapes);
	state(const state& rhs);
	state& operator=(const state &rhs);
	~state();
	void randomize(unsigned int seed);
	bool placementIsValid(int i, int x, int y, int rot);
	void placeShape(int i, int x, int y, int rot);
	void printSolution(std::string filename);
	void printLayout(std::string filename);
	int getFitness() { return m_fitness; };
};


#endif