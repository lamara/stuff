// MoreMoreDrawingStuff.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MoreMoreDrawingStuff.h"
#include "App.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	App theApp;

	if (theApp.Init(hInstance, nCmdShow))
	{
		//allows main to return quit messages
		//from the message loop
		int QuitParam = theApp.RunMessageLoop();
		return QuitParam;
	}
	
	return 0;
}