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
*	Compiler Constants
**********************************************************/
#define RANDOM_MAX_TRIES	(10000)
#define ELEMENT_IS_EQUAL(e1,idx1,e2,idx2) \
							(e1->m_x[idx1] == e2->m_x[idx2] &&  \
							 e1->m_y[idx1] == e2->m_y[idx2] &&  \
							 e1->m_rot[idx1] == e2->m_rot[idx2])

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
	void randomize();
	bool placementIsValid(int i, int x, int y, int rot);
	void placeShape(int i, int x, int y, int rot);
	void repair(int& idx, int& x, int& y, int& rot);
	void calcFitness();
	void nPointCrossover(state* parent1, state* parent2, int n);
	void uniformCrossover(state* parent1, state* parent2, float p);
	void randResetMutate();
	void creepMutate(int creepDist);
	void printSolution(std::string filename);
	void printLayout(std::string filename);
	
	// Accessors, Mutators
	int getFitness() const { return m_fitness; };

};

/**********************************************************
*	State Compare Function
*	Returns true if s1 has a higher fitness value
**********************************************************/
inline bool compareState(state* s1, state* s2) { return(s1->getFitness() > s2->getFitness()); }

#endif
