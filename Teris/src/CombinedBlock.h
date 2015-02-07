#pragma once
#include "displayableobject.h"
#include "MovementPosition.h"
#include "MyProjectMain.h"
#include "Block.h"

class MyProjectMain;

class CombinedBlock :
	public DisplayableObject
{
public:
	CombinedBlock(MyProjectMain* m_pEngine);
	~CombinedBlock(void);

	void Draw();
	void DoUpdate( int iCurrentTime );
	// set iCurrentScreenX and iCurrentScreenY of all single blocks
	void setCurrentPosition(int m_iCurrentScreenX,int m_iCurrentScreenY);
	// set colour index of all single blocks
	void setColour(int shapeCode);
	//set shape index of combined blocks , there are 19 shapes in total
	void setShape(int shape);

private :
	MyProjectMain* m_pMainEngine;
	// store shape index
	int shapeCode;
	/*
	{I,L,J,X,S,T,Z} -> {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18}
	*/
	// store colour index
	int colourCode;
	// store current time, so program can wait for a period of time before executing some codes
	int currentTime;

	// four single blocks
	Block mainBlock;
	Block block1;
	Block block2;
	Block block3;

	// decide whether blocks touch the bottom of game window or other blocks already dropped
	bool isBottom();
	// decide whether blocks touch the left side of game window or other blocks already dropped
	bool isLeftOutOfScope();
	// decide whether blocks touch the right side of game window or other blocks already dropped
	bool isRightOutOfScope();
	// set shape index and select next shape index ,store it to m_ppMainEngine->next
	void setCurrentAndSelectNext();
};

