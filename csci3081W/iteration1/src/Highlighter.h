//
//  Highlighter.h
//


#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include "Tool.h"

class Highlighter : public Tool 
{
public:

	Highlighter();
	virtual ~Highlighter();
	

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
