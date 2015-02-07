#include "header.h"

#include "BaseEngine.h"
#include "JPGImage.h"
#include "MyProjectTileManager.h"
#include "MyProjectMain.h"
#include "CombinedBlock.h"
#include <string>
#include <fstream>


MyProjectMain::MyProjectMain(void)
: BaseEngine(2)
, m_state(stateInit)
, score(0)
, level(1)
, next((rand()+13)%19)
{
	// initialize high score list
	for (int i = 0; i < 6; i++)
	{
		high_score[i]=1;
	}

	// read data from file and store data to high score list
	std::ifstream inputFile;
	inputFile.open("high_score.txt");
	if (inputFile) {
		int index=0;
		while (inputFile >> high_score[index]) {
			index++;
		}
	}
	inputFile.close();

	// set background tile size and base tile position
	m_oTiles.SetSize( 17, 23 );
	m_oTiles.SetBaseTilesPositionOnScreen( 20, 25 );	
}

void MyProjectMain::SetupBackgroundBuffer()
{
	// load relevant picture
	ImageData background,window,highScoreBoard,over;
	background.LoadImage("earth.jpg");
	window.LoadImage("window.png");
	highScoreBoard.LoadImage("high_score.jpg");
	over.LoadImage("game_over.jpg");

	if (m_state==stateInit) {
		// load index page of game
		ImageData initial;
		initial.LoadImage("initial.jpg");
		initial.RenderImage(this->GetBackground(), 0, 0, 0, 0, 800, 600);
	}
	if (m_state==stateMain) {
		// set moving object visible
		(this->GetDisplayableObject(0))->SetVisible(true);
		// show background and game window
		background.RenderImage(this->GetBackground(), 0, 0, 0, 0, 800, 600);
		window.RenderImageWithMask(GetBackground(), 0, 0, 0, 0, 800, 600);
		
		// draw tiles on the ground
		m_oTiles.DrawAllTiles( this, 
			this->GetBackground(), 
			0, 0, 16, 22 );
		// draw next blocks on window "Next Block"
		drawNext();
	}
	if (m_state==statePaused) {
		// hide moving object
		(this->GetDisplayableObject(0))->SetVisible(false);
		// show background and game window
		background.RenderImage(this->GetBackground(), 0, 0, 0, 0, 800, 600);
		window.RenderImageWithMask(GetBackground(), 0, 0, 0, 0, 800, 600);	
	}
	if (m_state==gameOver) {
		// hide moving object
		(this->GetDisplayableObject(0))->SetVisible(false);
		// show game over picture
		over.RenderImage(GetBackground(), 0, 0, 0, 0, 800, 600);	
		
	}
	if (m_state==stop) {
		// hide moving object
		(this->GetDisplayableObject(0))->SetVisible(false);
		// show high score board
		highScoreBoard.RenderImage(GetBackground(), 0, 0, 0, 0, 800, 600);	
	}
	
}

int MyProjectMain::InitialiseObjects()
{
	// Record the fact that we are about to change the array - so it doesn't get used elsewhere without reloading it
	DrawableObjectsChanged();

	// Destroy any existing objects
	DestroyOldObjects();

	// Create an array one element larger than the number of objects that you want.
	m_ppDisplayableObjects = new DisplayableObject*[2];

	//You MUST set the array entry after the last one that you create to NULL, so that the system knows when to stop.
	// i.e. The LAST entry has to be NULL. The fact that it is NULL is used in order to work out where the end of the array is.
	m_ppDisplayableObjects[0] = new CombinedBlock(this);
	m_ppDisplayableObjects[1] = NULL;

	return 0;
}

/* Draw text labels */
void MyProjectMain::DrawStrings()
{	
	switch (m_state)
	{
	case MyProjectMain::stateInit:
		break;
	case MyProjectMain::stateMain:
		// draw score on relevant window
		CopyBackgroundPixels(610, 252, GetScreenWidth(), 40);
		DrawScreenString( 610,252, std::to_string(score).c_str(), 0xffff00, NULL );		//score
		SetNextUpdateRect( 610, 252, GetScreenWidth(), 40 );

		// draw level on relevant window
		CopyBackgroundPixels( 620, 347, GetScreenWidth(), 40);
		DrawScreenString( 620, 347,std::to_string(level).c_str(), 0xffff00, NULL );		//level
		SetNextUpdateRect( 620, 347, GetScreenWidth(), 40 );

		// draw  high scores on relevant window
		CopyBackgroundPixels( 620, 448, GetScreenWidth(), 40);
		DrawScreenString( 620, 448 ,std::to_string(high_score[0]).c_str(), 0xffff00, NULL );		// high scores
		SetNextUpdateRect( 620, 448, GetScreenWidth(), 40 );
		CopyBackgroundPixels( 620, 488, GetScreenWidth(), 40);
		DrawScreenString( 620, 488,std::to_string(high_score[1]).c_str(), 0xffff00, NULL );
		SetNextUpdateRect(620, 488, GetScreenWidth(), 40 );
		CopyBackgroundPixels( 620, 525, GetScreenWidth(), 40);
		DrawScreenString( 620, 525,std::to_string(high_score[2]).c_str(), 0xffff00, NULL );
		SetNextUpdateRect( 620, 525, GetScreenWidth(), 40 );
		break;
	case MyProjectMain::statePaused:
		// paused
		CopyBackgroundPixels( 0, 200, GetScreenWidth(), 10 );
		DrawScreenString( 150, 200, "Paused. Press SPACE to continue", 0xffffff, NULL );
		SetNextUpdateRect( 0, 200, GetScreenWidth(), 10 );
		break;
	case MyProjectMain::stop :
		for (int i = 0; i < 6; i++)
		{
			// draw high scores on high score board
			CopyBackgroundPixels( 0, 222+i*40, GetScreenWidth(), 20 );
			DrawScreenString( 430, 222+i*40, std::to_string(high_score[i]).c_str(), 0x000000, NULL );
			SetNextUpdateRect( 0, 222+i*40, GetScreenWidth(), 20 );
		}
		break;
	}
}

/* Overridden GameAction which can be modified */
void MyProjectMain::GameAction()
{
	// If too early to act then do nothing
	if ( !TimeToAct() )
		return;

	// Don't act for another 10 ticks
	SetTimeToAct( 50 );

	switch (m_state)
	{
	case MyProjectMain::stateInit:
		break;
	case MyProjectMain::stateMain:
		// Tell all objects to update themselves
		UpdateAllObjects( GetTime() );
		break;
	case MyProjectMain::statePaused:
		break;
	case MyProjectMain::gameOver:
		break;
	case MyProjectMain::stop:
		break;
	}
}

// Override to handle a mouse press
void MyProjectMain::MouseDown( int iButton, int iX, int iY )
{
}

/*
Handle any key presses here.
Note that the objects themselves (e.g. player) may also check whether a key is currently pressed
*/
void MyProjectMain::KeyDown(int iKeyCode)
{
	// protect process of writing data to file in highScore state will not be affected
	if (m_state != highScore) {
		switch ( iKeyCode )
		{
		case SDLK_ESCAPE: // End program when escape is pressed
			SetExitWithCode( 0 );
			break;
		case SDLK_SPACE: // SPACE Pauses
			switch (m_state)
			{
			case MyProjectMain::stateInit:
				// Go to state main
				m_state = stateMain;
				// Force redraw of background
				SetupBackgroundBuffer();
				// Redraw the whole screen now
				Redraw(true);
				break;
			case MyProjectMain::stateMain:
				// Go to state paused
				m_state = statePaused;
				// Force redraw of background
				SetupBackgroundBuffer();
				// Redraw the whole screen now
				Redraw(true);
				break;
			case MyProjectMain::statePaused:
				// Go to state main
				m_state = stateMain;
				// Force redraw of background
				SetupBackgroundBuffer();
				// Redraw the whole screen now
				Redraw(true);
				break;
			}
			break;
		case SDLK_LEFT:
			// refresh
			if (m_state==stateMain) {
				SetupBackgroundBuffer();
				Redraw(true);
			}
			break;
		case SDLK_RIGHT:
			// refresh
			if (m_state==stateMain) {
				SetupBackgroundBuffer();
				Redraw(true);
			}
			break;
		case SDLK_UP:
			// refresh
			if (m_state==stateMain) {
				SetupBackgroundBuffer();
				Redraw(true);
			}
			break;
		case SDLK_DOWN:
			// refresh
			if (m_state==stateMain) {
				SetupBackgroundBuffer();
				Redraw(true);
			}
			break;
		case SDLK_h:
			// go to high score board
			if (m_state==stateInit) {
				m_state=stop;
				SetupBackgroundBuffer();
				Redraw(true);
			}
			break;
		case SDLK_b:
			// back from high score board or exit when game has been finished
			if (m_state==stop) {
				if (score == 0) {
					m_state=stateInit;
					SetupBackgroundBuffer();
					Redraw(true);
				}
				else
				{
					SetExitWithCode( 0 );
				}
			}
			break;
		}
	}
}

/* Draw the changes to the screen.
Remove the changing objects, redraw the strings and draw the changing objects again.
 */
void MyProjectMain::DrawChanges()
{
	// NEW IF
	if ( m_state == stateInit )
		return; // Do not draw objects if initialising

	// Remove objects from their old positions
	UndrawChangingObjects();
	// Draw the text for the user
	DrawStrings();
	// Draw objects at their new positions
	DrawChangingObjects();
}

/* Draw the screen - copy the background buffer, then draw the text and objects. */
void MyProjectMain::DrawScreen()
{
	// First draw the background
	//this->CopyBackgroundPixels( 100, 100, 100, 100 );
	CopyAllBackgroundBuffer();
	// And finally, draw the text
	DrawStrings();

	// NEW IF
	if ( m_state == stateInit )
		return; // Do not draw objects if initialising

	// Then draw the changing objects
	DrawChangingObjects();
}

bool MyProjectMain::setTileData(int iMapX1,int iMapY1,int iMapX2,int iMapY2,int iMapX3,int iMapY3,int iMapX4,int iMapY4 ,int colour) {
	// game over
	if (iMapY1<=0 || iMapY2<=0 || iMapY3<=0 || iMapY4<=0) {
		// get new record
		if (score >= high_score[5]) {
			m_state=highScore;
			// if state is highScore , change data in high score list and store new data to file
	// at end of this if block , it will change state to stop ,so this code will not change data repeatedly 
		for (int i = 0; i < 6; i++)
		{
			if (score >= high_score[i]) {
				for (int j = 4; j >=i; j--)
				{
					high_score[j+1]=high_score[j];
				}
				high_score[i]=score;
				break;
			}
			
		}

		std::ofstream outputFile;
		outputFile.open("high_score.txt");
		if (outputFile) {
			for (int i = 0; i < 6; i++)
			{
				outputFile << high_score[i]<<"\n";
			}
		}
		outputFile.close();
		m_state=stop;
		}
		else { // game over
			m_state=gameOver;
		}
		// refresh
		SetupBackgroundBuffer();
		Redraw(true);
		return false;
	}

	// set tile data for each single block
	m_oTiles.SetValue(iMapX1,iMapY1,colour);
	m_oTiles.SetValue(iMapX2,iMapY2,colour);
	m_oTiles.SetValue(iMapX3,iMapY3,colour);
	m_oTiles.SetValue(iMapX4,iMapY4,colour);
	// draw tiles
	m_oTiles.DrawAllTiles( this, 
			this->GetBackground(), 
			0, 0, 16, 22 );
	return true;
}

void MyProjectMain::drawNext() {
	// load picture of blocks of different shape and show them on the window "Next Block"
	ImageData blockToShow;	
	switch (next)
	{
	case 0:
		blockToShow.LoadImage("0.png");
		blockToShow.RenderImage(this->GetBackground(), 0, 0, 585, 100, 96, 24);
		break;
	case 1:
		blockToShow.LoadImage("1.png");
		blockToShow.RenderImage(this->GetBackground(), 0, 0, 610, 60, 24, 96);
		break;
	case 2:
		blockToShow.LoadImage("2.png");
		blockToShow.RenderImage(this->GetBackground(), 0, 0, 590, 90, 72, 48);
		break;
	case 3:
		blockToShow.LoadImage("3.png");
		blockToShow.RenderImage(this->GetBackground(), 0, 0, 600, 80, 48, 72);
		break;
	case 4:
		blockToShow.LoadImage("4.png");
		blockToShow.RenderImage(this->GetBackground(), 0, 0, 590, 90, 72, 48);
		break;
	case 5:
		blockToShow.LoadImage("5.png");
		blockToShow.RenderImage(this->GetBackground(), 0, 0, 600, 80, 48, 72);
		break;
	case 6:
		blockToShow.LoadImage("6.png");
		blockToShow.RenderImage(this->GetBackground(), 0, 0, 590, 90, 72, 48);
		break;
	case 7:
		blockToShow.LoadImage("7.png");
		blockToShow.RenderImage(this->GetBackground(), 0, 0, 600, 80, 48, 72);
		break;
	case 8:
		blockToShow.LoadImage("8.png");
		blockToShow.RenderImage(this->GetBackground(), 0, 0, 590, 90, 72, 48);
		break;
	case 9:
		blockToShow.LoadImage("9.png");
		blockToShow.RenderImage(this->GetBackground(), 0, 0, 600, 80, 48, 72);
		break;
	case 10:
		blockToShow.LoadImage("10.png");
		blockToShow.RenderImage(this->GetBackground(), 0, 0, 600, 90, 48, 48);
		break;
	case 11:
		blockToShow.LoadImage("11.png");
		blockToShow.RenderImage(this->GetBackground(), 0, 0, 590, 90, 72, 48);
		break;
	case 12:
		blockToShow.LoadImage("12.png");
		blockToShow.RenderImage(this->GetBackground(), 0, 0, 600, 80, 48, 72);
		break;
	case 13:
		blockToShow.LoadImage("13.png");
		blockToShow.RenderImage(this->GetBackground(), 0, 0, 590, 90, 72, 48);
		break;
	case 14:
		blockToShow.LoadImage("14.png");
		blockToShow.RenderImage(this->GetBackground(), 0, 0, 600, 80, 48, 72);
		break;
	case 15:
		blockToShow.LoadImage("15.png");
		blockToShow.RenderImage(this->GetBackground(), 0, 0, 590, 90, 72, 48);
		break;
	case 16:
		blockToShow.LoadImage("16.png");
		blockToShow.RenderImage(this->GetBackground(), 0, 0, 600, 80, 48, 72);
		break;
	case 17:
		blockToShow.LoadImage("17.png");
		blockToShow.RenderImage(this->GetBackground(), 0, 0, 590, 90, 72, 48);
		break;
	case 18:
		blockToShow.LoadImage("18.png");
		blockToShow.RenderImage(this->GetBackground(), 0, 0, 600, 80, 48, 72);
		break;
	}
}
