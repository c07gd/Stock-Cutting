#include <iostream>
#include "shape.h"
#include "state.h"


int main(int argc, char *argv[]) {

	state test("./pattern/100shapes.txt");
	test.randomize(2461032);
	test.printSolution("./output.txt");
	test.printLayout("./layout.txt");

	return 0;
}