//
//  Special.cpp
//


#include <iostream>
#include <cstring>
#include <cmath>
#include "Special.h"

using std::cerr;
using std::endl;
using std::cout;

Special::Special() {
	createMask(25, 25);
}

Special::~Special() {
	for (int i = 0; i < m_row; i++) {
		delete [] m_mask;
	}
}

void Special::createMask(int r, int c) {
	m_row = r;
	m_col = c;
	m_mask = new float *[m_row];
	for (int i = 0; i < m_row; i++) {
		m_mask[i] = new float[m_col];
	}
	m_mask[6][0] = m_mask[7][0] = m_mask[8][0] = m_mask[18][0]=m_mask[19][0]=m_mask[20][0] = 1.0;
	m_mask[4][1] = m_mask[5][1] = m_mask[9][1] = m_mask[10][1]=m_mask[16][1] = m_mask[17][1] = m_mask[21][1] = m_mask[22][1] = 1.0;
	m_mask[3][2] = m_mask[4][2] = m_mask[11][2]= m_mask[12][2]=m_mask[14][2] = m_mask[15][2] = m_mask[22][2] = m_mask[23][2] = 1.0;
	m_mask[2][3] = m_mask[3][3] = m_mask[12][3]= m_mask[13][3]=m_mask[14][3] = m_mask[23][2] = m_mask[24][3] = 1.0;
	m_mask[2][4] = m_mask[3][4] = m_mask[23][4] = m_mask[24][4] = 1.0;
	m_mask[2][5] = m_mask[3][5] = m_mask[23][5] = m_mask[24][5] = 1.0;
	m_mask[2][6] = m_mask[3][6] = m_mask[23][6] = m_mask[24][6] = 1.0;
	m_mask[3][7] = m_mask[4][7] = m_mask[22][7] = m_mask[23][7] = 1.0;
	m_mask[4][8] = m_mask[5][8] = m_mask[21][8] = m_mask[22][8] = 1.0;
	m_mask[5][9] = m_mask[6][8] = m_mask[20][9] = m_mask[21][9] = 1.0;
	m_mask[6][10] = m_mask[7][10] = m_mask[19][10] = m_mask[20][10] = 1.0;
	m_mask[7][11] = m_mask[8][11] = m_mask[18][11] = m_mask[19][11] = 1.0;
	m_mask[8][12] = m_mask[9][12] = m_mask[17][12] = m_mask[18][12] = 1.0;
	m_mask[9][13] = m_mask[10][13] = m_mask[16][13] = m_mask[17][13] = 1.0;
	m_mask[10][14] = m_mask[11][14] = m_mask[15][14] = m_mask[16][14] = 1.0;
	m_mask[11][15] = m_mask[12][15] = m_mask[14][15] = m_mask[15][15] = 1.0;
	m_mask[12][16] = m_mask[13][16] = m_mask[14][16] = 1.0;
	for(int i=1; i < m_row-1; i++){
		int count=1;
		for (int j = 0; j < m_col ; j++) {
			if (m_mask[i][j]!=0 && m_mask[i][j+1]==0) {
				if(m_mask[i][j]==1.0) {
					if((count%2)!=0){
						m_mask[i][j+1] = 0.4;
					}	
					count++;
				}else{
						m_mask[i][j+1] = 0.4;
				}
			} 
		}
	}
}

float **Special::getMask() {
	return m_mask;
}

int Special::getMaskRow() {
	return m_row;
}

int Special::getMaskCol() {
	return m_col;
}
