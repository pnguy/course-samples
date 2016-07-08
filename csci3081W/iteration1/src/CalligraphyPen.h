//
//  CalligraphyPen.h
//


#ifndef CALLIGRAPHYPEN_H
#define CALLIGRAPHYPEN_H

#include "Tool.h"

class CalligraphyPen : public Tool 
{
public:

	CalligraphyPen();
	virtual ~CalligraphyPen();
	

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
