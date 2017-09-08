#ifndef SHAPE_H
#define SHAPE_H


#include <cstdint>
#include <vector>

#define rotate(dir, rots) ((dir + rots) % NUM_DIRS)

enum {
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT,
	NUM_DIRS
};

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

int mutateDirection(int direction);

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