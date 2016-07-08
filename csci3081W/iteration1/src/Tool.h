//
//  Tool.h
//


#ifndef TOOL_H
#define TOOL_H

class Tool 
{
public:

	Tool();
	virtual ~Tool();
	
	virtual float** getMask();
	virtual void createMask(int r, int c);
	virtual int getMaskRow();
	virtual int getMaskCol();
	virtual float** makeCircleMask(int r, int c);

private:

};

#endif
