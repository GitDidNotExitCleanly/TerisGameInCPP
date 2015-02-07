#include "header.h"
#include "Block.h"
#include "MyProjectMain.h"


Block::Block(MyProjectMain* pEngine)
: DisplayableObject(pEngine)
, m_pMainEngine( pEngine )
{
	// every block is 24 width ,24 height
	m_iDrawWidth = 24;						
	m_iDrawHeight = 24;	
}

Block::~Block(void)
{
}

void Block::Draw()														
{
	if (!IsVisible()) {
		return;
	}
	// draw block if it doesn't be hidden by top side of game window
	if (m_iCurrentScreenY >= 25) {
		m_pMainEngine->DrawRectangle(								
					m_iCurrentScreenX+1,
					m_iCurrentScreenY+1,
					m_iCurrentScreenX-2+m_iDrawWidth ,
					m_iCurrentScreenY-2+m_iDrawHeight,
					m_pMainEngine->GetColour(colourCode) );

		StoreLastScreenPositionAndUpdateRect();
	}
}

// set iCurrentScreenX and iCurrentScreenY of a single block
void Block::setCurrentPosition(int currentX,int currentY) 
{
		m_iCurrentScreenX=currentX;
		m_iCurrentScreenY=currentY;
}
