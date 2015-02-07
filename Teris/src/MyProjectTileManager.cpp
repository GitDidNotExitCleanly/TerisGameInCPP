#include "header.h"
#include "MyProjectTileManager.h"

MyProjectTileManager :: MyProjectTileManager(void) 
{
}

MyProjectTileManager :: ~MyProjectTileManager(void) 
{
}

// Get the size of a tile - width
int MyProjectTileManager::GetTileWidth()
{
	return 24;
}

// Get the size of a tile - height
int MyProjectTileManager::GetTileHeight()
{
	return 24;
}

// Draw a tile
void MyProjectTileManager::DrawTileAt( 
	BaseEngine* pEngine, 
	SDL_Surface* pSurface, 
	int iMapX, int iMapY,
	int iStartPositionScreenX, int iStartPositionScreenY )
{

	// base on different shape of blocks , decide relevant colour , 0 means no need to draw
	switch( GetValue(iMapX,iMapY) )
	{
	case 0:
		break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 22:
		pEngine->DrawRectangle( 
			iStartPositionScreenX+1,
			iStartPositionScreenY+1, 
			iStartPositionScreenX + GetTileWidth() - 2,
			iStartPositionScreenY + GetTileHeight() - 2,
			pEngine->GetColour( GetValue(iMapX,iMapY) ),
			pSurface );
		break;
	}
}

