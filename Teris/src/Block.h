#pragma once
#include "displayableobject.h"
#include "MovementPosition.h"
#include "MyProjectMain.h"

class MyProjectMain;

/*
 This class is for single block of a combined blocks
*/

class Block :
	public DisplayableObject
{
public:
	Block(MyProjectMain* pEngine);
	~Block(void);

	//draw single block ,based on colour index and iCurrentX and iCurrentY
	void Draw();
	//set iCurrentX and iCurrentY ,  will be called by CombinedBlock class
	void setCurrentPosition(int currentX,int currentY);
	// set colour index , will be called by CombinedBlock class
	inline void setColour(int colour) {
		colourCode = colour;
	}

private:
	MyProjectMain* m_pMainEngine;
	// store colour index
	int colourCode;
};

