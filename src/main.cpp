#include <iostream>
#include "shape.h"
#include "state.h"
#include "cfgParse.h"


int main(int argc, char *argv[]) {

	config cfg = getConfig(argv[1]);
		
	state test(cfg.inputFile);
	test.randomize(cfg.seed);
	test.printSolution(cfg.solutionFile);
	test.printLayout("./layout.txt");	

	return 0;
}