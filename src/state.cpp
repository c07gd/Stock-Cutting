#include "state.h"
#include <iostream>
#include <fstream>


state::state(uint8_t width, uint8_t numShapes) {
	//m_width = width;
	//m_layout = new shape_layout[numShapes];

}

state::state(std::string filename) {
	int x;
	std::string s;
	std::ifstream in(filename, std::ios_base::in);
	if (!in.is_open()) {
		std::cout << "Error: Unable to open file";
		return;
	}
	in >> x;
	m_width = (uint8_t)x;
	in >> x;
	m_numShapes = (uint8_t)x;
	m_shapes = new shape_layout[m_numShapes];
	in.ignore(64, '\n');
	m_length = 0;
	int i = 0;
	while (getline(in, s)) {
		m_shapes[i].shape = new shape(s.c_str());
		m_shapes[i].x = 0;
		m_shapes[i].y = 0;
		m_shapes[i].rot = 0;
		m_length += m_shapes[i].shape->getWidth();
		i++;
	}
	m_layout = new bool*[m_width];
	for (i = 0; i < m_width; i++) {
		m_layout[i] = new bool[m_length];
		for (int j = 0; j < m_length; j++) {
			m_layout[i][j] = false;
		}
	}
	in.close();		
}

state::~state() {
	for (int i = 0; i < m_width; i++)
		free(m_layout[i]);
	free(m_layout);
	free(m_shapes);
}