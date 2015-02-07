#ifndef MYPROJECTMAIN_H
#define MYPROJECTMAIN_H

#include "BaseEngine.h"
#include "TileManager.h"
#include "MyProjectTileManager.h"
#include "JPGImage.h"

class MyProjectMain : public BaseEngine
{
public:

	/**
	Constructor
	*/
	MyProjectMain(void);

	// Do any setup of back buffer prior to locking the screen buffer
	// Basically do the drawing of the background in here and it'll be copied to the screen for you as needed
	virtual void SetupBackgroundBuffer();

	// Create any moving objects
	int InitialiseObjects();

	/** Draw any strings */
	void DrawStrings();

	/**
	The game logic - move things and change the state if necessary.
	Must call Redraw(true/false) if anything changes that shows on the screen.
	*/
	virtual void GameAction();

	// Handle pressing of a mouse button
	void MouseDown( int iButton, int iX, int iY );

	// Handle pressing of a key
	virtual void KeyDown(int iKeyCode);

	enum State {stateInit,stateMain,statePaused,gameOver,highScore,stop};

	MyProjectTileManager& GetTileManager() { return m_oTiles; }

	void DrawScreen();
	void DrawChanges();

	// set the tile data in order to draw relevant blocks at correct place
	bool setTileData(int iMapX1,int IMapY1,int iMapX2,int IMapY2,int iMapX3,int IMapY3,int iMapX4,int IMapY4,int colour);
	// draw next blocks on the window "Next Block"
	void drawNext();
	// increase score by 10 for each line linked
	void addScore() {
		score += 10;
	}
	// 50 points to enter level 2, 100 points to level 3, 150 points to level 4 , etc.
	void addLevel() {
		if (score%(50*level)==0) {
			if (level < 8) {								// maximal level 8.
				level += 1;
			}
		}
	}
	// get level number , for changing speed of object's dropping
	int getLevel() {
		return level;
	}
	// set a number representing next blocks in order to show on window "Next Block"
	void setNext(int nextBlock) {
		next = nextBlock;
	}
	// get next blocks 
	int getNext() {
		return next;
	}
	
private:
	MyProjectTileManager m_oTiles;
	State m_state;
	// store relevant information 
	int level;
	int score;
	int next;
	// store high scores , will read data from files and store to this
	int high_score[6];
};

#endif
