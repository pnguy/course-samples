//
//  Tool.cpp
//


#include <iostream>
#include <cstring>
#include <cmath>
#include "Tool.h"

using std::cerr;
using std::endl;
using std::cout;

Tool::Tool() {
}

Tool::~Tool() {
}

float **Tool::getMask() {
	return 0;
}

void Tool::createMask(int r, int c) {

}

int Tool::getMaskRow() {
	return 0;
}

int Tool::getMaskCol() {
	return 0;
}

float **Tool::makeCircleMask(int r, int c) {

	float **mask = new float *[r];

	for (int i = 0; i < r; i++) {
		mask[i] = new float[c];
		for (int j = 0; j < c; j++) {

			// Get distance between i and j
			float distance = sqrt((i-(r/2))*(i-(r/2)) + (j-(c/2))*(j-(c/2)));

			// Check if within circle
			if (distance < (r/2)) {
				mask[i][j] = 1.0;
			} else {
				mask[i][j] = 0.0;
			}
		}
	}

	return mask;
}

