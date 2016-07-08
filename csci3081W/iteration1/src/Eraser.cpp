//
//  CalligraphyPen.cpp
//


#include <iostream>
#include <cstring>
#include <cmath>
#include "Eraser.h"

using std::cerr;
using std::endl;
using std::cout;

Eraser::Eraser() {
	createMask(21, 21);
}

Eraser::~Eraser() {
	for (int i = 0; i < m_row; i++) {
		delete [] m_mask;
	}
}

void Eraser::createMask(int r, int c) {
	m_row = r;
	m_col = c;
	m_mask = makeCircleMask(m_row,m_col);	
}

float **Eraser::getMask() {
	return m_mask;
}

int Eraser::getMaskRow() {
	return m_row;
}

int Eraser::getMaskCol() {
	return m_col;
}
