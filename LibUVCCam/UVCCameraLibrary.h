#ifndef __UVCCAMERALIBRARY_H__
#define __UVCCAMERALIBRARY_H__

#include <windows.h>
#include <iostream>

#include <stdio.h>
#include <dshow.h>
#include <ks.h>
#include <Ksmedia.h>


#ifdef UVCCAMERALIBRARY_EXPORTS
#define UVCCAMERAAPI __declspec(dllexport)
#else
#define UVCCAMERAAPI __declspec(dllimport)
#endif
#pragma once

class UVCCAMERAAPI UVCCameraLibrary
{
public:
	UVCCameraLibrary();
	~UVCCameraLibrary();

	//static method to list video connected capture devices
	static void listDevices(char **devices , int &nDevices);

	//connect to device with device name
	bool connectDevice(char *deviceName);

	//disconnect device
	void disconnectDevice();

	//camera control functions
	//pan,tilt
	HRESULT movePanOneLeft(int pan);
	HRESULT movePanOneRight(int pan);
	HRESULT moveTiltOneTop(int tilt);
	HRESULT moveTiltOneBottom(int tilt);
	/*HRESULT angleUpLeft(int pan, int tilt);
	HRESULT angleUpRight(int pan, int tilt);
	HRESULT angleDownLeft(int pan, int tilt);
	HRESULT anglueDownRight(int pan, int tilt);*/
	//zoom
	HRESULT zoomOneIn(int zoom);
	HRESULT zoomOneOut(int zoom);
	//focus
	HRESULT focusOneIn(int focus);
	HRESULT focusOneOut(int focus);
	//home
	HRESULT moveHome();
	//move to absolute position
	HRESULT moveTo(int pan, int tilt, int zoom);
	//set auto/manual of focus
	HRESULT setAutoFocus(bool af);
	//stop moving, zooming, focusing
	HRESULT stopMoving();
	HRESULT stopZooming();
	HRESULT stopFocusing();
	
	//get focus status(Auto/Manual)
	bool getAutoFocus();

	//get camera properties
	long getPan();
	long getTilt();
	long getZoom();
	long getFocus();
private:

	//base directshow filter
	IBaseFilter *pDeviceFilter = NULL;

	// to select a video input device
	ICreateDevEnum *pCreateDevEnum = NULL;
	IEnumMoniker *pEnumMoniker = NULL;
	IMoniker *pMoniker = NULL;
	ULONG nFetched = 0;
	void getEnumMoniker();

	//change the property of camera according to @prop
	HRESULT moveCamera(KSPROPERTY_VIDCAP_CAMERACONTROL prop , int step);
	//stop change of the property
	HRESULT stopControling(KSPROPERTY_VIDCAP_CAMERACONTROL prop);

	//get Auto/Manual status of property
	bool getAuto(CameraControlProperty prop);
	//get value of the property
	long getVal(CameraControlProperty prop);

};

#endif
