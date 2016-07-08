//
//  Highlighter.cpp
//


#include <iostream>
#include <cstring>
#include <cmath>
#include "Highlighter.h"

using std::cerr;
using std::endl;
using std::cout;

Highlighter::Highlighter() {
	createMask(5, 15);
}

Highlighter::~Highlighter() {
	for (int i = 0; i < m_row; i++) {
		delete [] m_mask;
	}
}

void Highlighter::createMask(int r, int c) {
	m_row = r;
	m_col = c;

	m_mask = new float *[r];
	for (int i = 0; i < r; i++) {
		m_mask[i] = new float[c];
		for (int j = 0; j < c; j++) {
			m_mask[i][j] = .4;	
		}
	}
}

float **Highlighter::getMask() {
	return m_mask;
}

int Highlighter::getMaskRow() {
	return m_row;
}

int Highlighter::getMaskCol() {
	return m_col;
}
