#include <iostream>
#include <fstream>
#include <string>
#include "shape.h"
#include "state.h"
#include "cfgParse.h"

void readInputFile(std::string filename, shape*& shapes, int& width, int& numShapes);

int main(int argc, char *argv[]) {

	// Variables
	config		cfg;
	shape*		shapes = NULL;
	int			width = 0;
	int			numShapes = 0;

	// Get configuration
	if (argc <= 1) {
		std::cout << "Error: please supply config filename as argument" << std::endl;
		return 0;
	}
	cfg = getConfig(argv[1]);

	// Get shapes
	readInputFile(cfg.inputFile, shapes, width, numShapes);

	// Construct initial state
	state test(shapes, width, numShapes);

	// Randomize state layout
	test.randomize(cfg.seed);

	// Print output
	test.printSolution(cfg.solutionFile);
	test.printLayout("./layout.txt");

	// Clean up
	delete[] shapes;

	return 0;
}


void readInputFile(std::string filename, shape*& shapes, int& width, int& numShapes) {
	
	// Variables
	std::ifstream	in;
	std::string		line;
	int				i;

	// Open input file
	in.open(filename, std::ifstream::in);
	if (!in.is_open()) {
		std::cout << "Error: Unable to open file";
		return;
	}

	// Read first line (width & numShapes)
	in >> width;
	in >> numShapes;
	in.ignore(64, '\n');

	// Construct shapes array
	shapes = new shape[numShapes];
	
	// Read in file line-by-line and assign moves
	i = 0;
	while (getline(in, line)) {
		shapes[i] = shape(line.c_str());
		i++;
	}

	// Clean up
	in.close();

	return;
}