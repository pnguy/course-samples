//
//  CalligraphyPen.cpp
//


#include <iostream>
#include <cstring>
#include <cmath>
#include "CalligraphyPen.h"

using std::cerr;
using std::endl;
using std::cout;

CalligraphyPen::CalligraphyPen() {
	createMask(5, 15);
}

CalligraphyPen::~CalligraphyPen() {
	for (int i = 0; i < m_row; i++) {
		delete [] m_mask;
	}
}

void CalligraphyPen::createMask(int r, int c) {
	m_row = r;
	m_col = c;
	m_mask = new float *[m_row];
	for (int i = 0; i < m_row; i++) {
		m_mask[i] = new float[m_col];
		for (int j = 0; j < m_col; j++) {
			m_mask[i][j] = 1.0;
		}
	}
}

float **CalligraphyPen::getMask() {
	return m_mask;
}

int CalligraphyPen::getMaskRow() {
	return m_row;
}

int CalligraphyPen::getMaskCol() {
	return m_col;
}
