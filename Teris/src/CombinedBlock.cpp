#include "CombinedBlock.h"
#include "header.h"
#include "MyProjectMain.h"

CombinedBlock::CombinedBlock(MyProjectMain* pEngine)
: DisplayableObject(pEngine)
, m_pMainEngine( pEngine )
, currentTime(0)
, mainBlock( pEngine )
, block1( pEngine )
, block2( pEngine )
, block3( pEngine )
{
	// set rand() seed
	srand((unsigned)(m_pMainEngine->GetTime()));

	// set initial position
	m_iPreviousScreenX = m_iCurrentScreenX = 212;
	m_iPreviousScreenY = m_iCurrentScreenY = 25;

	// set current shape and select next shape
	setCurrentAndSelectNext();
	// based on shape ,set coulour index
	setColour(shapeCode);

	m_iDrawWidth = 24;						
	m_iDrawHeight = 24;	
	
	// show object
	SetVisible(true);
}

CombinedBlock::~CombinedBlock(void)
{
}

void CombinedBlock::Draw()
{
	if (!IsVisible()) {
		return;
	}
	// draw blocks
	mainBlock.Draw();
	block1.Draw();
	block2.Draw();
	block3.Draw();
}

void CombinedBlock::DoUpdate( int iCurrentTime )
{
	// this piece of code will execute once per (850-m_pMainEngine->getLevel()*100) ticks
	// it will make blocks drop
	if ((iCurrentTime-currentTime) >= (850-m_pMainEngine->getLevel()*100)) {
		currentTime=iCurrentTime;
		m_iCurrentScreenY += 24;
		setCurrentPosition(m_iCurrentScreenX,m_iCurrentScreenY);
		m_pMainEngine->KeyDown(SDLK_DOWN);
	}
								
	// rotate , if it doesn't , for exampe , touch the left side of touch the bottom
	if ( GetEngine()->IsKeyPressed( SDLK_UP ) ) {
		if (!isBottom()) {
			switch (shapeCode)
			{
			case 0:
				if (mainBlock.GetYCentre() <= 541) {
					setShape(1);
				}
				break;
			case 1:
				if (mainBlock.GetXCentre() >= 80 && mainBlock.GetXCentre() <=392) {
					setShape(0);
				}
				break;
			case 2:
				if (mainBlock.GetYCentre() <= 541 ) {
					setShape(3);
				}
				break;
			case 3:
				if (mainBlock.GetXCentre() >=  56) {
					setShape(4);
				}
				break;
			case 4:
				setShape(5);
				break;
			case 5:
				if (mainBlock.GetXCentre() <=392 ) {
					setShape(2);
				}
				break;
			case 6:
				setShape(7);
				break;
			case 7:
				if (mainBlock.GetXCentre() <=392 ) {
					setShape(8);
				}
				break;
			case 8:
				if (mainBlock.GetYCentre() <= 541 ) {
					setShape(9);
				}
				break;
			case 9:
				if (mainBlock.GetXCentre() >=  56) {
					setShape(6);
				}
				break;
			case 10:
				break;
			case 11:
				if (mainBlock.GetYCentre() <= 541 ) {
					setShape(12);
				}
				break;
			case 12:
				if (mainBlock.GetXCentre() <=392 ) {
					setShape(11);
				}
				break;
			case 13:
				if (mainBlock.GetYCentre() <= 541 ) {
					setShape(14);
				}
				break;
			case 14:
				if (mainBlock.GetXCentre() <=392 ) {
					setShape(13);
				}
				break;
			case 15:
				if (mainBlock.GetYCentre() <= 541 ) {
					setShape(16);
				}
				break;
			case 16:
				if (mainBlock.GetXCentre() >=  56) {
					setShape(17);
				}
				break;
			case 17:
				setShape(18);
				break;
			case 18:
				if (mainBlock.GetXCentre() <=392 ) {
					setShape(15);
				}
				break;
			}
			m_pMainEngine->KeyDown(SDLK_UP);	
		}
	}
	if ( GetEngine()->IsKeyPressed( SDLK_DOWN ) ) {	
		if (!isBottom()) {
			m_iCurrentScreenY += 24;
			setCurrentPosition(m_iCurrentScreenX,m_iCurrentScreenY);
			m_pMainEngine->KeyDown(SDLK_DOWN);
		}
	}
	if ( GetEngine()->IsKeyPressed( SDLK_LEFT ) ) {																	
		if(!isLeftOutOfScope() && !isBottom()) {
			m_iCurrentScreenX -= 24;
			setCurrentPosition(m_iCurrentScreenX,m_iCurrentScreenY);
			m_pMainEngine->KeyDown(SDLK_LEFT);
		}
	}
	if ( GetEngine()->IsKeyPressed( SDLK_RIGHT ) ) {																
		if(!isRightOutOfScope() && !isBottom()) {
			m_iCurrentScreenX += 24;
			setCurrentPosition(m_iCurrentScreenX,m_iCurrentScreenY);
			m_pMainEngine->KeyDown(SDLK_RIGHT);
		}
	}
	// refresh
	mainBlock.RedrawObjects();
	block1.RedrawObjects();
	block2.RedrawObjects();
	block3.RedrawObjects();
	
	if (isBottom()) {
		int iMapX1 = m_pMainEngine->GetTileManager().GetTileXForPositionOnScreen(mainBlock.GetXCentre());
		int iMapY1 = m_pMainEngine->GetTileManager().GetTileYForPositionOnScreen(mainBlock.GetYCentre());
		int iMapX2 = m_pMainEngine->GetTileManager().GetTileXForPositionOnScreen(block1.GetXCentre());
		int iMapY2 = m_pMainEngine->GetTileManager().GetTileYForPositionOnScreen(block1.GetYCentre());
		int iMapX3 = m_pMainEngine->GetTileManager().GetTileXForPositionOnScreen(block2.GetXCentre());
		int iMapY3 = m_pMainEngine->GetTileManager().GetTileYForPositionOnScreen(block2.GetYCentre());
		int iMapX4 = m_pMainEngine->GetTileManager().GetTileXForPositionOnScreen(block3.GetXCentre());
		int iMapY4 = m_pMainEngine->GetTileManager().GetTileYForPositionOnScreen(block3.GetYCentre());

		// draw tiles as the same shape as the object on the background
		if (m_pMainEngine->setTileData(iMapX1,iMapY1,iMapX2,iMapY2,iMapX3,iMapY3,iMapX4,iMapY4,colourCode)) {
			
			// initial the position
			m_iCurrentScreenX = 212;
			m_iCurrentScreenY = 25;
			// set current blocks and select next blocks
			setCurrentAndSelectNext();
			setColour(shapeCode);
		}

		// if there is at lease one line linked , undraw them , and adjust tile data 
		bool allNeedUpdate = false;
		for (int i = 1; i < 23; i++) {
			// whether there is one line linked
			bool needUpdate = true;
			for (int j = 0; j < 17; j++) {
				if (m_pMainEngine->GetTileManager().GetValue(j,i)==0) {
					needUpdate &= false;
				}
			}
			// if so, undraw this line
			if (needUpdate) {
				for (int x = i; x >= 1; x--) {															
					for (int y = 0; y < 17; y++) {
						m_pMainEngine->GetTileManager().SetValue(y,x,m_pMainEngine->GetTileManager().GetValue(y,x-1));					
					}
				}
				m_pMainEngine->addScore();
				m_pMainEngine->addLevel();
			}
			allNeedUpdate = allNeedUpdate || needUpdate;
		}
		// undraw linked line and adjust new tile data
		if (allNeedUpdate) {
			m_pMainEngine->GetTileManager().DrawAllTiles(m_pMainEngine, m_pMainEngine->GetBackground(), 0, 0, 16, 22 );
		}
	}
	// refresh
	mainBlock.RedrawObjects();
	block1.RedrawObjects();
	block2.RedrawObjects();
	block3.RedrawObjects();
}

void CombinedBlock::setCurrentPosition(int currentX,int currentY) {
	// set mainBlock as the same position as this class
	mainBlock.setCurrentPosition(currentX,currentY);
	 
	// based on shape ,set each block different position
	switch (shapeCode)
	{
	case 0:
		block1.setCurrentPosition(currentX-2*m_iDrawWidth,currentY);
		block2.setCurrentPosition(currentX-m_iDrawWidth,currentY);
		block3.setCurrentPosition(currentX+m_iDrawWidth,currentY);
		break;
	case 1:
		block1.setCurrentPosition(currentX,currentY-2*m_iDrawHeight);
		block2.setCurrentPosition(currentX,currentY-m_iDrawHeight);
		block3.setCurrentPosition(currentX,currentY+m_iDrawHeight);
		break;
	case 2:
		block1.setCurrentPosition(currentX-m_iDrawWidth,currentY);
		block2.setCurrentPosition(currentX+m_iDrawWidth,currentY-m_iDrawHeight);
		block3.setCurrentPosition(currentX+m_iDrawWidth,currentY);
		break;
	case 3:
		block1.setCurrentPosition(currentX,currentY-m_iDrawHeight);
		block2.setCurrentPosition(currentX,currentY+m_iDrawHeight);
		block3.setCurrentPosition(currentX+m_iDrawWidth,currentY+m_iDrawHeight);
		break;
	case 4:
		block1.setCurrentPosition(currentX-m_iDrawWidth,currentY);
		block2.setCurrentPosition(currentX+m_iDrawWidth,currentY);
		block3.setCurrentPosition(currentX-m_iDrawWidth,currentY+m_iDrawHeight);
		break;
	case 5:
		block1.setCurrentPosition(currentX-m_iDrawWidth,currentY-m_iDrawHeight);
		block2.setCurrentPosition(currentX,currentY-m_iDrawHeight);
		block3.setCurrentPosition(currentX,currentY+m_iDrawHeight);
		break;
	case 6:
		block1.setCurrentPosition(currentX-m_iDrawWidth,currentY);
		block2.setCurrentPosition(currentX+m_iDrawWidth,currentY);
		block3.setCurrentPosition(currentX+m_iDrawWidth,currentY+m_iDrawHeight);
		break;
	case 7:
		block1.setCurrentPosition(currentX-m_iDrawWidth,currentY+m_iDrawHeight);
		block2.setCurrentPosition(currentX,currentY-m_iDrawHeight);
		block3.setCurrentPosition(currentX,currentY+m_iDrawHeight);
		break;
	case 8:
		block1.setCurrentPosition(currentX-m_iDrawWidth,currentY-m_iDrawHeight);
		block2.setCurrentPosition(currentX-m_iDrawWidth,currentY);
		block3.setCurrentPosition(currentX+m_iDrawWidth,currentY);
		break;
	case 9:
		block1.setCurrentPosition(currentX,currentY-m_iDrawHeight);
		block2.setCurrentPosition(currentX+m_iDrawWidth,currentY-m_iDrawHeight);
		block3.setCurrentPosition(currentX,currentY+m_iDrawHeight);
		break;
	case 10:
		block1.setCurrentPosition(currentX+m_iDrawWidth,currentY);
		block2.setCurrentPosition(currentX,currentY+m_iDrawHeight);
		block3.setCurrentPosition(currentX+m_iDrawWidth,currentY+m_iDrawHeight);
		break;
	case 11:
		block1.setCurrentPosition(currentX-m_iDrawWidth,currentY);
		block2.setCurrentPosition(currentX,currentY-m_iDrawHeight);
		block3.setCurrentPosition(currentX+m_iDrawWidth,currentY-m_iDrawHeight);
		break;
	case 12:
		block1.setCurrentPosition(currentX-m_iDrawWidth,currentY-m_iDrawHeight);
		block2.setCurrentPosition(currentX-m_iDrawWidth,currentY);
		block3.setCurrentPosition(currentX,currentY+m_iDrawHeight);
		break;
	case 13:
		block1.setCurrentPosition(currentX-m_iDrawWidth,currentY-m_iDrawHeight);
		block2.setCurrentPosition(currentX,currentY-m_iDrawHeight);
		block3.setCurrentPosition(currentX+m_iDrawWidth,currentY);
		break;
	case 14:
		block1.setCurrentPosition(currentX-m_iDrawWidth,currentY);
		block2.setCurrentPosition(currentX-m_iDrawWidth,currentY+m_iDrawHeight);
		block3.setCurrentPosition(currentX,currentY-m_iDrawHeight);
		break;
	case 15:
		block1.setCurrentPosition(currentX-m_iDrawWidth,currentY);
		block2.setCurrentPosition(currentX,currentY-m_iDrawHeight);
		block3.setCurrentPosition(currentX+m_iDrawWidth,currentY);
		break;
	case 16:
		block1.setCurrentPosition(currentX,currentY-m_iDrawHeight);
		block2.setCurrentPosition(currentX,currentY+m_iDrawHeight);
		block3.setCurrentPosition(currentX+m_iDrawWidth,currentY);
		break;
	case 17:
		block1.setCurrentPosition(currentX-m_iDrawWidth,currentY);
		block2.setCurrentPosition(currentX+m_iDrawWidth,currentY);
		block3.setCurrentPosition(currentX,currentY+m_iDrawHeight);
		break;
	case 18:
		block1.setCurrentPosition(currentX-m_iDrawWidth,currentY);
		block2.setCurrentPosition(currentX,currentY-m_iDrawHeight);
		block3.setCurrentPosition(currentX,currentY+m_iDrawHeight);
		break;
	}
}

// whether object touches the bottom of game window or touches the bottom of tiles in background
bool CombinedBlock::isBottom() {																							
	if ( mainBlock.GetYCentre() > 541 || block1.GetYCentre() > 541 || block2.GetYCentre() > 541 || block3.GetYCentre() > 541) {	
		return true;
	}
	if (m_pMainEngine->GetTileManager().GetTileYForPositionOnScreen(mainBlock.GetYCentre())<22 &&
		m_pMainEngine->GetTileManager().GetTileYForPositionOnScreen(block1.GetYCentre())<22 &&
		m_pMainEngine->GetTileManager().GetTileYForPositionOnScreen(block2.GetYCentre())<22 &&
		m_pMainEngine->GetTileManager().GetTileYForPositionOnScreen(block3.GetYCentre())<22 ) {
		if (m_pMainEngine->GetTileManager().GetValue( m_pMainEngine->GetTileManager().GetTileXForPositionOnScreen(mainBlock.GetXCentre()) , m_pMainEngine->GetTileManager().GetTileYForPositionOnScreen(mainBlock.GetYCentre())+1  ) > 0 ||
			m_pMainEngine->GetTileManager().GetValue( m_pMainEngine->GetTileManager().GetTileXForPositionOnScreen(block1.GetXCentre()) , m_pMainEngine->GetTileManager().GetTileYForPositionOnScreen(block1.GetYCentre())+1  ) > 0 ||
			m_pMainEngine->GetTileManager().GetValue( m_pMainEngine->GetTileManager().GetTileXForPositionOnScreen(block2.GetXCentre()) , m_pMainEngine->GetTileManager().GetTileYForPositionOnScreen(block2.GetYCentre())+1  ) > 0 ||
			m_pMainEngine->GetTileManager().GetValue( m_pMainEngine->GetTileManager().GetTileXForPositionOnScreen(block3.GetXCentre()) , m_pMainEngine->GetTileManager().GetTileYForPositionOnScreen(block3.GetYCentre())+1  ) > 0 ) 
		{
		return true;	
		}
	}
	return false;
}

// whether object touches the leftmost side of game window or touches the leftmost side of tiles in background
bool CombinedBlock::isLeftOutOfScope() {
	if ( mainBlock.GetXCentre() < 56 || block1.GetXCentre() < 56 || block2.GetXCentre() < 56 || block3.GetXCentre() < 56) {										
		return true;
	}
	if (m_pMainEngine->GetTileManager().GetTileXForPositionOnScreen(mainBlock.GetXCentre())>0 &&
		m_pMainEngine->GetTileManager().GetTileXForPositionOnScreen(block1.GetXCentre())>0 &&
		m_pMainEngine->GetTileManager().GetTileXForPositionOnScreen(block2.GetXCentre())>0 &&
		m_pMainEngine->GetTileManager().GetTileXForPositionOnScreen(block3.GetXCentre())>0 ) {
		if (m_pMainEngine->GetTileManager().GetValue( m_pMainEngine->GetTileManager().GetTileXForPositionOnScreen(mainBlock.GetXCentre())-1 , m_pMainEngine->GetTileManager().GetTileYForPositionOnScreen(mainBlock.GetYCentre())  ) > 0 ||
			m_pMainEngine->GetTileManager().GetValue( m_pMainEngine->GetTileManager().GetTileXForPositionOnScreen(block1.GetXCentre())-1 , m_pMainEngine->GetTileManager().GetTileYForPositionOnScreen(block1.GetYCentre())  ) > 0 ||
			m_pMainEngine->GetTileManager().GetValue( m_pMainEngine->GetTileManager().GetTileXForPositionOnScreen(block2.GetXCentre())-1 , m_pMainEngine->GetTileManager().GetTileYForPositionOnScreen(block2.GetYCentre())  ) > 0 ||
			m_pMainEngine->GetTileManager().GetValue( m_pMainEngine->GetTileManager().GetTileXForPositionOnScreen(block3.GetXCentre())-1 , m_pMainEngine->GetTileManager().GetTileYForPositionOnScreen(block3.GetYCentre())  ) > 0 ) 
		{
			return true;
		}
	}
	return false;
}

// whether object touches the rightmost side of game window or touches the rightmost side of tiles in background
bool CombinedBlock::isRightOutOfScope() {
	if ( mainBlock.GetXCentre() > 392 || block1.GetXCentre() > 392 || block2.GetXCentre() > 392 || block3.GetXCentre() > 392 ) {
		return true;
	}
	
	if (m_pMainEngine->GetTileManager().GetTileXForPositionOnScreen(mainBlock.GetXCentre())<16 &&
		m_pMainEngine->GetTileManager().GetTileXForPositionOnScreen(block1.GetXCentre())<16 &&
		m_pMainEngine->GetTileManager().GetTileXForPositionOnScreen(block2.GetXCentre())<16 &&
		m_pMainEngine->GetTileManager().GetTileXForPositionOnScreen(block3.GetXCentre())<16 ) {
		if (m_pMainEngine->GetTileManager().GetValue( m_pMainEngine->GetTileManager().GetTileXForPositionOnScreen(mainBlock.GetXCentre())+1 , m_pMainEngine->GetTileManager().GetTileYForPositionOnScreen(mainBlock.GetYCentre())  ) > 0 ||
			m_pMainEngine->GetTileManager().GetValue( m_pMainEngine->GetTileManager().GetTileXForPositionOnScreen(block1.GetXCentre())+1 , m_pMainEngine->GetTileManager().GetTileYForPositionOnScreen(block1.GetYCentre())  ) > 0 ||
			m_pMainEngine->GetTileManager().GetValue( m_pMainEngine->GetTileManager().GetTileXForPositionOnScreen(block2.GetXCentre())+1 , m_pMainEngine->GetTileManager().GetTileYForPositionOnScreen(block2.GetYCentre())  ) > 0 ||
			m_pMainEngine->GetTileManager().GetValue( m_pMainEngine->GetTileManager().GetTileXForPositionOnScreen(block3.GetXCentre())+1 , m_pMainEngine->GetTileManager().GetTileYForPositionOnScreen(block3.GetYCentre())  ) > 0 ) 
		{
			return true;
		}
	}
	return false;
}

// set colour index based on shape
void CombinedBlock::setColour(int shapeCode) {
	switch (shapeCode)
	{
	case 0:
	case 1:
		colourCode = 1;
		break;
	case 2:
	case 3:
	case 4:
	case 5:
		colourCode = 22;
		break;
	case 6:
	case 7:
	case 8:	
	case 9:
		colourCode = 3;
		break;
	case 10:
		colourCode = 5;
		break;
	case 11:
	case 12:
		colourCode = 4;
		break;
	case 13:
	case 14:
		colourCode = 2;
		break;
	case 15:
	case 16:
	case 17:
	case 18:
		colourCode = 6;
		break;
	}

	// pass colour index to each single block
	mainBlock.setColour(colourCode);
	block1.setColour(colourCode);
	block2.setColour(colourCode);
	block3.setColour(colourCode);
}

// set shape
void CombinedBlock::setShape(int shape) {
		shapeCode=shape;
		setCurrentPosition(m_iCurrentScreenX,m_iCurrentScreenY);
	}

// set current shape will value of next stored in MyProjectMain class
// select next shape randomly
void CombinedBlock::setCurrentAndSelectNext() {
	setShape(m_pMainEngine->getNext());
	m_pMainEngine->setNext((rand()+10)%19);
	m_pMainEngine->drawNext();
}