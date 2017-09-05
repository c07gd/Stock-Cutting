#ifndef SHAPE_H
#define SHAPE_H


#include <cstdint>
#include <vector>


#define UP			('U')
#define DOWN		('D')
#define LEFT		('L')
#define RIGHT		('R')


struct move {
	uint8_t distance;
	char	direction;
};


class shape {
private:
	std::vector<move> moves;
public:
	shape::shape(const char* line);
	~shape();
	int getWidth();
	void print();
};

struct shape_layout {
	shape*	shape;
	uint8_t	x;
	uint8_t	y;
	uint8_t	rot;
};


#endif