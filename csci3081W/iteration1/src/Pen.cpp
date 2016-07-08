//
//  Pen.cpp
//

#include <iostream>
#include <cstring>
#include <cmath>
#include "Pen.h"

using std::cerr;
using std::endl;
using std::cout;

Pen::Pen() {
	createMask(3, 3);
}

Pen::~Pen() {
	for (int i = 0; i < m_row; i++) {
		delete [] m_mask;
	}
}

void Pen::createMask(int r, int c) {
	m_row = r;
	m_col = c;

	m_mask = makeCircleMask(m_row, m_col);
}

float **Pen::getMask() {
	return m_mask;
}

int Pen::getMaskRow() {
	return m_row;
}

int Pen::getMaskCol() {
	return m_col;
}
