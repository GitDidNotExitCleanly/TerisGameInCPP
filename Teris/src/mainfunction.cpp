#include "header.h"
#include "templates.h"
#include "MyProjectMain.h"

#define BASE_SCREEN_WIDTH 800
#define BASE_SCREEN_HEIGHT 600

int main(int argc, char *argv[])
{
	int iResult;

	MyProjectMain oMain;
	
	char buf[1024];
	sprintf( buf, "My Demonstration Program : Size %d x %d", BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT );
	iResult = oMain.Initialise( buf, BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT, "Cornerstone Regular.ttf", 24 );
	iResult = oMain.MainLoop();
	oMain.Deinitialise();

	return iResult;
}
