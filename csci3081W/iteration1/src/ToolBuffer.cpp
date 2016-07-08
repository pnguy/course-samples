//
//  ToolBuffer.h
//  Note: Pixel array starts from bottem left corner.

#include <iostream>
#include <cstring>
#include <cmath>
#include <typeinfo>

#include "ToolBuffer.h"
#include "PixelBuffer.h"
#include "ColorData.h"
#include "Tool.h"

#include "Pen.h"
#include "Eraser.h"
#include "SprayCan.h"
#include "CalligraphyPen.h"
#include "Highlighter.h"
#include "Special.h"

#include <vector>

using std::cout;
using std::cerr;
using std::endl;
using std::vector;

ToolBuffer::ToolBuffer(int curTool, int w, int h, float r, float g, float b) {
	// Set current color	
	m_red = r;
	m_green = g;
	m_blue = b;

	// Set canvas size
	m_width = w;
	m_height = h;
	
	// Initialize tools
	m_activeTool = curTool;

	m_availTools.push_back(new Pen);
	m_availTools.push_back(new Eraser);
	m_availTools.push_back(new SprayCan);
	m_availTools.push_back(new CalligraphyPen);
	m_availTools.push_back(new Highlighter);
	m_availTools.push_back(new Special);
}


ToolBuffer::~ToolBuffer() {
	m_availTools.clear();
}

void ToolBuffer::applyTool(int x, int y, int curTool, PixelBuffer* displayBuffer) {
	m_activeTool = curTool;
	if (m_availTools[curTool] != NULL) {
		float **m = m_availTools[curTool]->getMask();
		int row = m_availTools[curTool]->getMaskRow();
		int col = m_availTools[curTool]->getMaskCol();
		
		int xC = col/2;
		int yC = row/2;	

		for(int i = 0; i < row; i++) {
			for(int j = 0; j < col; j++) {
				// If within the canvas, update displayBuffer	
				if (!((x+(i-xC) < 0) || (x+(i-xC) >= m_width) || (y+(j-yC) < 0) || (y+(j-yC) >= m_height))) {
					ColorData color = createColorData(m[i][j], x+(i-xC), y+(j-yC), displayBuffer);						
					displayBuffer->setPixel(x+(i-xC), y+(j-yC), color.clampedColor());
				}
			}
		}
	} else {
		cout << "Tool not yet implemented" << endl;
	}
}


// Setter and Getter for active tool
void ToolBuffer::setActiveTool(int tool) {
	m_activeTool = tool;
}

int ToolBuffer::getActiveTool() {
	return m_activeTool;
}

// Setters and Getters for RGB
ColorData ToolBuffer::createColorData(float &intensity, int x, int y, PixelBuffer* displayBuffer) {
	ColorData canvasColor = displayBuffer->getPixel(x, y);
	ColorData toolColor;
	float tempIntensity = intensity;
	
	if (m_activeTool == 1) { // Tool is eraser		
		toolColor = displayBuffer->getBackgroundColor();
	} else {	
		toolColor = ColorData(m_red, m_green, m_blue);
		if (m_activeTool == 4) { // Tool is highlighter
			tempIntensity *= canvasColor.getLuminance();
		}
	}

	return toolColor*tempIntensity + canvasColor*(1.0-tempIntensity);
}
	
void ToolBuffer::setColor(float r, float g, float b) {
	m_red = r;
	m_green = g;
	m_blue = b;
}

void ToolBuffer::setRed(float r) {
	m_red = r;
}

void ToolBuffer::setGreen(float g) {
	m_green = g;
}

void ToolBuffer::setBlue(float b) {
	m_blue = b;
}

float ToolBuffer::getRed() {
	return m_red;
}

float ToolBuffer::getGreen() {
	return m_green;
}

float ToolBuffer::getBlue() {
	return m_blue;
}

int ToolBuffer::getMaskRow() {
	return m_availTools[m_activeTool]->getMaskRow();
}

int ToolBuffer::getMaskCol() {
	return m_availTools[m_activeTool]->getMaskCol();
}


