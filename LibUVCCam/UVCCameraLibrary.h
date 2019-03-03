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
	static void listDevices(char **devices , int &nDevices);
	bool connectDevice(char *deviceName);
	void disconnectDevice();
	HRESULT movePanOneLeft(int pan);
	HRESULT movePanOneRight(int pan);
	HRESULT moveTiltOneTop(int tilt);
	HRESULT moveTiltOneBottom(int tilt);
	/*HRESULT angleUpLeft(int pan, int tilt);
	HRESULT angleUpRight(int pan, int tilt);
	HRESULT angleDownLeft(int pan, int tilt);
	HRESULT anglueDownRight(int pan, int tilt);*/
	HRESULT zoomOneIn(int zoom);
	HRESULT zoomOneOut(int zoom);
	HRESULT focusOneIn(int focus);
	HRESULT focusOneOut(int focus);
	HRESULT moveHome();
	HRESULT moveTo(int pan, int tilt, int zoom);
	HRESULT setAutoFocus(bool af);
	HRESULT stopMoving();
	HRESULT stopZooming();
	HRESULT stopFocusing();
	

	bool getAutoFocus();
	long getPan();
	long getTilt();
	long getZoom();
	long getFocus();
private:


	IBaseFilter *pDeviceFilter = NULL;
	// to select a video input device
	ICreateDevEnum *pCreateDevEnum = NULL;
	IEnumMoniker *pEnumMoniker = NULL;
	IMoniker *pMoniker = NULL;
	ULONG nFetched = 0;
	void getEnumMoniker();
	HRESULT moveCamera(KSPROPERTY_VIDCAP_CAMERACONTROL prop , int step);
	HRESULT stopControling(KSPROPERTY_VIDCAP_CAMERACONTROL prop);

	bool getAuto(CameraControlProperty prop);
	long getVal(CameraControlProperty prop);

};

#endif
