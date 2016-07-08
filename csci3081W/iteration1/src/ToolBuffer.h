//
//  ToolBuffer.h
//

#ifndef TOOLBUFFER_H
#define TOOLBUFFER_H

#include <vector>
using std::vector;

class PixelBuffer;
class ColorData;
class Tool;

class ToolBuffer 
{
public:

	ToolBuffer(int tool, int w, int h, float r, float g, float b);
	virtual ~ToolBuffer();

	// Sets active tool based on GLUI tool array
	void setActiveTool(int tool);
	int getActiveTool();

	void applyTool(int x, int y, int curTool, PixelBuffer *displayBuffer);
	void setColor(float r, float g, float b);
	ColorData createColorData(float &intensity, int x, int y, PixelBuffer * displayBuffer);	

	int getMaskRow();
	int getMaskCol();

	void setRed(float r);
	void setGreen(float g);
	void setBlue(float b);
	
	float getRed();
	float getGreen();
	float getBlue();

private:
	
	vector<Tool*> m_availTools;
	int m_activeTool;
	
	int m_width;
	int m_height;

	float m_red;
	float m_green;
	float m_blue;

};

#endif
