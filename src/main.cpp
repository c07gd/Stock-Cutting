#include <iostream>
#include "shape.h"


int main(int argc, char *argv[]) {

	std::cout << "Hello world!" << std::endl;
	shape test("R3 D3 U3 D1 U3");
	test.print();
	std::cout << std::endl;

	return 0;
}