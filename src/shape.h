/******************************************************************************
*	shape.h
*
*	Declarations for the shape class for the Stock Cutting Problem.
*
*	Stuart Miller
*	Missouri S&T CS 5401
*	Fall 2017
******************************************************************************/
#ifndef SHAPE_H
#define SHAPE_H


/**********************************************************
*	Headers
**********************************************************/
#include <string>
#include <vector>


/**********************************************************
*	Compiler Constants
**********************************************************/
#define rotateShape(dir, rots) ((dir + rots) % NUM_DIRS)


/**********************************************************
*	Types, Etc.
**********************************************************/
enum directions{
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT,
	NUM_DIRS
};

enum rotations{
	ROT_0_DEG,
	ROT_90_DEG,
	ROT_180_DEG,
	ROT_270_DEG,
	NUM_ROTS
};

struct move {
	char	distance;
	char	direction;
};


/**********************************************************
*	Shape Class
**********************************************************/
class shape {

private:

	std::vector<move>	m_moves;
	int					m_length;
	int					m_overlap;

	// Private Functions
	void calcLength();
	void calcOverlap();

public:

	// Constructors, Destructors, Etc.
	shape() {};
	shape(const char* line);
	~shape() {};

	// Member Functions
	void print();

	// Accessors, Mutators
	inline std::vector<move>* getMoves() { return &m_moves; };
	inline int getLength() { return m_length; };
	inline int getOverlap() { return m_overlap; };

};


/**********************************************************
*	Non-Member Funtions
**********************************************************/
void readInputFile(std::string filename, shape*& shapes, int& width, int& numShapes);


#endif
