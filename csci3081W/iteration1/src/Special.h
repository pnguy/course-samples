//
//  Special.h
//


#ifndef SPECIAL_H
#define SPECIAL_H

#include "Tool.h"

class Special : public Tool 
{
public:

	Special();
	virtual ~Special();
	

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
