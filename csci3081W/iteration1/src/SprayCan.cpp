//
//  SprayCan.cpp
//


#include <iostream>
#include <cstring>
#include <cmath>
#include "SprayCan.h"

using std::cerr;
using std::endl;
using std::cout;

SprayCan::SprayCan() {
	createMask(41, 41);
}

SprayCan::~SprayCan() {
	for (int i = 0; i < m_row; i++) {
		delete [] m_mask;
	}
}

void SprayCan::createMask(int r, int c) {
	m_row = r;
	m_col = c;
	m_mask = makeCircleMask(m_row,m_col);

	int radius = m_row/2;
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {

			// Distance between i and j
			int distance = sqrt((i-radius)*(i-radius) + (j-radius)*(j-radius));
			if (distance <= radius) {
				m_mask[i][j] = .2 - distance*(.2/radius);
			}
		}
	}
}

float **SprayCan::getMask() {
	return m_mask;
}

int SprayCan::getMaskRow() {
	return m_row;
}

int SprayCan::getMaskCol() {
	return m_col;
}
