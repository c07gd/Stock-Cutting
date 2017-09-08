#ifndef SHAPE_H
#define SHAPE_H


#include <cstdint>
#include <vector>


#define UP			('U')
#define DOWN		('D')
#define LEFT		('L')
#define RIGHT		('R')

enum {
	ROT_0_DEG,
	ROT_90_DEG,
	ROT_180_DEG,
	ROT_270_DEG,
	NUM_ROTS
};


struct move {
	uint8_t distance;
	char	direction;
};


class shape {
private:
	std::vector<move> moves;
public:
	shape::shape() {};
	shape::shape(const char* line);
	~shape() {};
	int getLength();
	void print();
	inline std::vector<move>* getMoves() { return &moves; };
};

struct shape_layout {
	shape*	shape;
	uint8_t	x;
	uint8_t	y;
	uint8_t	rot;
};


#endif