//
//  Eraser.h
//


#ifndef ERASER_H
#define ERASER_H

#include "Tool.h"

class Eraser : public Tool 
{
public:

	Eraser();
	virtual ~Eraser();
	

	float **getMask();
	void createMask(int r, int c);
	int getMaskRow();
	int getMaskCol();

private:
	float **m_mask;
	int m_row;
	int m_col;

};

#endif
