//
//  Pen.h
//


#ifndef PEN_H
#define PEN_H

#include "Tool.h"

class Pen : public Tool 
{
public:

	Pen();
	virtual ~Pen();
	
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
