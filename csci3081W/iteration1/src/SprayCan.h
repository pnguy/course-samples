//
//  SprayCan.h
//


#ifndef SPRAYCAN_H
#define SPRAYCAN_H

#include "Tool.h"

class SprayCan : public Tool 
{
public:

	SprayCan();
	virtual ~SprayCan();
	

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
