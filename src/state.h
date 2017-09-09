/******************************************************************************
*	state.h
*
*	Declarations for the state class for the Stock Cutting Problem.
*
*	Stuart Miller
*	Missouri S&T CS 5401
*	Fall 2017
******************************************************************************/
#ifndef STATE_H
#define STATE_H


/**********************************************************
*	Headers
**********************************************************/
#include "shape.h"
#include <string>


/**********************************************************
*	State Class
**********************************************************/
class state {
	
private:

	// Meta Variables
	int			m_width;
	int			m_length;
	bool**		m_layout;
	int			m_fitness;
	int			m_numShapes;
	shape*		m_shapes;

	// Layout Parameters
	int*		m_x;
	int*		m_y;
	int*		m_rot;

	// Private Funtions
	void constructArrays();


public:

	// Constructors, Destructors, Etc.
	state();
	state(shape* shapes, int width, int numShapes);
	state(const state& rhs);
	state& operator=(const state &rhs);
	~state();

	// Member Functions
	void randomize(unsigned int seed);
	bool placementIsValid(int i, int x, int y, int rot);
	void placeShape(int i, int x, int y, int rot);
	void printSolution(std::string filename);
	void printLayout(std::string filename);

	// Accessors, Mutators
	int getFitness() { return m_fitness; };

};


#endif
