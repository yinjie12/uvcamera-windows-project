// LibraryTester.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <UVCCameraLibrary.h>
#include <Windows.h>

int main()
{
	char **deviceNames;
	deviceNames = new char*[256];
	for (int i = 0; i < 256; i++)
	{
		deviceNames[i] = new char[256];
	}

	int devices = 0;
	
	UVCCameraLibrary::listDevices(deviceNames, devices);
	printf("the number of devices = %d\n", devices);
	for (int i = 0; i < devices; i++)
	{
		printf("%s\n", deviceNames[i]);
	}
	UVCCameraLibrary lib;
	char devname[256];
	strcpy_s(devname , "HD Camera");
	if (lib.connectDevice(devname))
	{
		
		HRESULT hr;
		hr = lib.movePanOneLeft(5);
		/*hr = lib.moveTiltOneTop(5);
		if (SUCCEEDED(hr))
			printf("Successed\n");
		hr = lib.zoomOneIn(5);
		if (SUCCEEDED(hr))
			printf("Successed\n");
		hr = lib.focusOneIn(5);
		if (SUCCEEDED(hr))
			printf("Successed\n");

		hr = lib.moveHome();
		if(SUCCEEDED(hr))
			printf("Successed\n");
		
		printf("pan %d, tilt %d, zoom %d, focus %d\n", lib.getPan(), lib.getTilt(), lib.getZoom(), lib.getFocus());
		lib.setAutoFocus(false);
		if (lib.getAutoFocus())
			printf("Focus Auto\n");
		lib.setAutoFocus(true);
		if (lib.getAutoFocus())
			printf("Focus Auto\n");
		hr = lib.zoomOneIn(1000);*/
		
	}

	char a = 0;
	scanf_s("$c", &a);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
