#pragma once
#include "tilemanager.h"

/*
	This class is for Tile Manager
*/

class MyProjectTileManager :
	public TileManager
{
public:
	MyProjectTileManager(void);
	~MyProjectTileManager(void);

	// Get the size of a tile - width
	virtual int GetTileWidth();

	// Get the size of a tile - height
	virtual int GetTileHeight();

	// Draw a tile
	virtual void DrawTileAt( 
		BaseEngine* pEngine, 
		SDL_Surface* pSurface, 
		int iMapX, int iMapY,
		int iStartPositionScreenX, int iStartPositionScreenY );
};

