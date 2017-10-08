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
#include "cfgParse.h"
#include <string>


/**********************************************************
*	Compiler Constants
**********************************************************/
#define RANDOM_MAX_TRIES	(10000)
#define PENALTY_WEIGHT		(3.0f)
#define GEN_SCALED_PROB(k)	((float)(rand() % (int)std::pow(10.0,k)) / std::pow(10.0,k))
#define ELEMENT_IS_EQUAL(e1,idx1,e2,idx2) \
							(e1->m_x[idx1] == e2->m_x[idx2] &&  \
							 e1->m_y[idx1] == e2->m_y[idx2] &&  \
							 e1->m_rot[idx1] == e2->m_rot[idx2])


/**********************************************************
*	Types, Etc.
**********************************************************/
struct adaptablepParams {
	bool	enablePW;
	int		typePW;
	float	pw;
	bool	enableCP;
	int		cp;
	bool	enableMR;
	float	mr;
};


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
	int			m_penalty;
	int			m_numShapes;
	shape*		m_shapes;
	adaptablepParams
				m_params;

	// Layout Parameters
	int*		m_x;
	int*		m_y;
	int*		m_rot;

	// Private Funtions
	void constructArrays();
	void repair(int idx, int& x, int& y, int& rot);
	void resolveConstraints(int idx, int& x, int& y, int& rot, int type);

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
	void calcFitness();
	void nPointCrossover(state* parent1, state* parent2, int n, int constraintSat);
	void uniformCrossover(state* parent1, state* parent2, float p, int constraintSat);
	void randResetMutate(int constraintSat);
	void creepMutate(int creepDist, int constraintSat);
	void updateAdaptableParams(state* parent1, state* parent2);
	void printSolution(std::string filename);
	void printLayout(std::string filename);
	
	// Accessors, Mutators
	inline int getFitness() const { return m_fitness; };
	inline adaptablepParams getParams() const { return m_params; };
	inline void setParams(config cfg) {
		m_params.enablePW = (cfg.constraintSat == CONSTRAINTSAT_PENALTY);
		m_params.typePW = cfg.penaltyWeightSA;
		m_params.pw = cfg.penaltyWeight;
		m_params.enableCP = cfg.crossoversSA;
		m_params.cp = cfg.crossovers;
		m_params.enableMR = cfg.mutationRateSA;
		m_params.mr = cfg.mutationRate;
	}

};


/**********************************************************
*	State Compare Function
*	Returns true if s1 has a higher fitness value
**********************************************************/
inline bool compareState(state* s1, state* s2) { return(s1->getFitness() > s2->getFitness()); }


#endif
