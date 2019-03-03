#include "stdafx.h"
#include "UVCCameraLibrary.h"


UVCCameraLibrary::UVCCameraLibrary()
{
	// initialize COM
	CoInitialize(0);
}

UVCCameraLibrary::~UVCCameraLibrary()
{
	// release
	if(pEnumMoniker != NULL)
		pEnumMoniker->Release();
	if(pCreateDevEnum != NULL)
		pCreateDevEnum->Release();

	// finalize COM
	CoUninitialize();
}

void UVCCameraLibrary::listDevices(char **cameraNames , int &nDevices)
{
	nDevices = 0;
	IBaseFilter *pDeviceFilter = NULL;
	// to select a video input device
	ICreateDevEnum *pCreateDevEnum = NULL;
	IEnumMoniker *pEnumMoniker = NULL;
	IMoniker *pMoniker = NULL;
	ULONG nFetched = 0;
	// initialize COM
	CoInitialize(0);
	// Create CreateDevEnum to list device
	CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum, (PVOID *)&pCreateDevEnum);

	// Create EnumMoniker to list VideoInputDevice 
	pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,
		&pEnumMoniker, 0);
	if (pEnumMoniker == NULL) {
		// this will be shown if there is no capture device
		printf("no device\n");
		return;
	}

	// reset EnumMoniker
	pEnumMoniker->Reset();
	
	while (pEnumMoniker->Next(1, &pMoniker, &nFetched) == S_OK) {

		char cameraRealNames[256][256];
		
		IPropertyBag *pPropertyBag;
		TCHAR devname[256];

		// bind to IPropertyBag
		pMoniker->BindToStorage(0, 0, IID_IPropertyBag,
			(void **)&pPropertyBag);

		VARIANT var;

		// get FriendlyName
		var.vt = VT_BSTR;
		pPropertyBag->Read(L"FriendlyName", &var, 0);//DevicePath
		WideCharToMultiByte(CP_ACP, 0,
			var.bstrVal, -1, devname, sizeof(devname), 0, 0);
		VariantClear(&var);

		int nSameNamedDevices = 0;
		for (int j = 0; j < nDevices; j++)
		{
			if (strcmp(cameraRealNames[j], devname) == 0)
				nSameNamedDevices++;
		}
		strcpy_s(cameraRealNames[nDevices], devname);
		if(nSameNamedDevices > 0)
			sprintf_s(devname, "%s #%d", devname, nSameNamedDevices);
		strcpy_s(cameraNames[nDevices] , sizeof(devname) , (TCHAR*)devname);

		// release
		pMoniker->Release();
		pPropertyBag->Release();

		nDevices++;
	}	
	// release

	pEnumMoniker->Release();
	pCreateDevEnum->Release();

	// finalize COM
	CoUninitialize();
}

void UVCCameraLibrary::getEnumMoniker()
{
	// Create CreateDevEnum to list device
	CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum, (PVOID *)&pCreateDevEnum);

	// Create EnumMoniker to list VideoInputDevice 
	pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,
		&pEnumMoniker, 0);
	if (pEnumMoniker == NULL) {
		// this will be shown if there is no capture device
		printf("no device\n");
		return;
	}

	// reset EnumMoniker
	pEnumMoniker->Reset();
}

bool UVCCameraLibrary::connectDevice(char *deviceName)
{
	getEnumMoniker();

	if (pEnumMoniker == NULL)
		return false;

	int nDevices = 0;
	while (pEnumMoniker->Next(1, &pMoniker, &nFetched) == S_OK) {
		

		char cameraRealNames[256][256];

		IPropertyBag *pPropertyBag;
		TCHAR devname[256];

		// bind to IPropertyBag
		pMoniker->BindToStorage(0, 0, IID_IPropertyBag,
			(void **)&pPropertyBag);

		VARIANT var;

		// get FriendlyName
		var.vt = VT_BSTR;
		pPropertyBag->Read(L"FriendlyName", &var, 0);
		WideCharToMultiByte(CP_ACP, 0,
			var.bstrVal, -1, devname, sizeof(devname), 0, 0);
		VariantClear(&var);

		int nSameNamedDevices = 0;
		for (int j = 0; j < nDevices; j++)
		{
			if (strcmp(cameraRealNames[j], devname) == 0)
				nSameNamedDevices++;
		}
		strcpy_s(cameraRealNames[nDevices], devname);

		if(nSameNamedDevices > 0)
			sprintf_s(devname, "%s #%d", devname, nSameNamedDevices);

		//printf("devname %s , devicename %s\n", devname, deviceName);

		if (strcmp(devname, deviceName) == 0)
		{
			pMoniker->BindToObject(0, 0, IID_IBaseFilter,
				(void**)&pDeviceFilter);
			if (pDeviceFilter != NULL)
				return true;
		}

		// release
		pMoniker->Release();
		pPropertyBag->Release();

		nDevices++;
	}
	return false;
}

void UVCCameraLibrary::disconnectDevice()
{
	if(pDeviceFilter != NULL)
	    pDeviceFilter->Release();
	pDeviceFilter = NULL;
}

HRESULT UVCCameraLibrary::movePanOneLeft(int pan)
{
	return moveCamera(KSPROPERTY_CAMERACONTROL_PAN_RELATIVE , - pan);
}
HRESULT UVCCameraLibrary::movePanOneRight(int pan)
{
	return moveCamera(KSPROPERTY_CAMERACONTROL_PAN_RELATIVE, pan);
}
HRESULT UVCCameraLibrary::moveTiltOneTop(int tilt)
{
	return moveCamera(KSPROPERTY_CAMERACONTROL_TILT_RELATIVE , tilt);
}
HRESULT UVCCameraLibrary::moveTiltOneBottom(int tilt)
{
	return moveCamera(KSPROPERTY_CAMERACONTROL_TILT_RELATIVE, -tilt);
}
/*HRESULT angleUpLeft(int pan, int tilt)
{
	HRESULT hr;
	return hr;
}
HRESULT angleUpRight(int pan, int tilt)
{
	HRESULT hr;
	return hr;
}
HRESULT angleDownLeft(int pan, int tilt)
{
	HRESULT hr;
	return hr;
}
HRESULT anglueDownRight(int pan, int tilt)
{
	HRESULT hr;
	return hr;
}*/
HRESULT UVCCameraLibrary::zoomOneIn(int zoom)
{
	return moveCamera(KSPROPERTY_CAMERACONTROL_ZOOM_RELATIVE , zoom);
}
HRESULT UVCCameraLibrary::zoomOneOut(int zoom)
{
	return moveCamera(KSPROPERTY_CAMERACONTROL_ZOOM_RELATIVE, -zoom);
}
HRESULT UVCCameraLibrary::focusOneIn(int focus)
{
	return moveCamera(KSPROPERTY_CAMERACONTROL_FOCUS_RELATIVE , focus);
}
HRESULT UVCCameraLibrary::focusOneOut(int focus)
{
	return moveCamera(KSPROPERTY_CAMERACONTROL_FOCUS_RELATIVE, -focus);
}
HRESULT UVCCameraLibrary::setAutoFocus(bool af)
{
	stopFocusing();
	HRESULT hr;
	IAMCameraControl *pCameraControl = 0;
	hr = pDeviceFilter->QueryInterface(IID_IAMCameraControl, (void**)&pCameraControl);
	if (FAILED(hr))
	{
		// The device does not support IAMCameraControl
		printf("This device does not support IAMCameraControl\n");

	}
	else
	{
		long Min, Max, Step, Default, Flags, Val;

		// Get the range and default values 
		hr = pCameraControl->GetRange(CameraControl_Focus, &Min, &Max, &Step, &Default, &Flags);
		if (SUCCEEDED(hr))
		{
			hr = pCameraControl->Get(CameraControl_Focus, &Val, &Flags);
			if(af)
				hr = pCameraControl->Set(CameraControl_Focus, Val , CameraControl_Flags_Auto);
			else
				hr = pCameraControl->Set(CameraControl_Focus, Val, CameraControl_Flags_Manual);
		}
		else
		{
			printf("This device does not support PTZControl\n");
		}

	}
	if(pCameraControl != NULL)
	    pCameraControl->Release();
	return hr;
}

HRESULT UVCCameraLibrary::moveCamera(KSPROPERTY_VIDCAP_CAMERACONTROL prop, int step)
{
	HRESULT hr;
	IAMCameraControl *pCameraControl = 0;
	hr = pDeviceFilter->QueryInterface(IID_IAMCameraControl, (void**)&pCameraControl);
	if (FAILED(hr))
	{
		// The device does not support IAMCameraControl
		printf("This device does not support IAMCameraControl\n");

	}
	else
	{
		long Min, Max, Step, Default, Flags, Val;

		// Get the range and default values 
		hr = pCameraControl->GetRange(prop, &Min, &Max, &Step, &Default, &Flags);
		printf("Min %d , Max %d , Step %d", Min, Max , Step);
		if (SUCCEEDED(hr))
		{
			hr = pCameraControl->Set(prop, step, CameraControl_Flags_Manual);
		}
		else
		{
			printf("This device does not support PTZControl\n");
		}
	}
	if (pCameraControl != NULL)
		pCameraControl->Release();
	return hr;
}
HRESULT UVCCameraLibrary::moveTo(int pan, int tilt, int zoom)
{
	HRESULT hr;
	hr = stopMoving();
	hr = stopZooming();
	hr = stopFocusing();
	IAMCameraControl *pCameraControl = 0;
	hr = pDeviceFilter->QueryInterface(IID_IAMCameraControl, (void**)&pCameraControl);
	if (FAILED(hr))
	{
		// The device does not support IAMCameraControl
		printf("This device does not support IAMCameraControl\n");

	}
	else
	{
		long Min, Max, Step, Default, Flags, Val;

		// Get the range and default values 
		hr = pCameraControl->GetRange(CameraControl_Pan, &Min, &Max, &Step, &Default, &Flags);
		long panFlag = Flags;
		pan += Min;
		if (pan < Min)
			pan = Min;
		if (pan > Max)
			pan = Max;
		hr = pCameraControl->GetRange(CameraControl_Tilt, &Min, &Max, &Step, &Default, &Flags);
		long tiltFlag = Flags;
		tilt += Min;
		if (tilt < Min)
			tilt = Min;
		if (tilt > Max)
			tilt = Max;
		hr = pCameraControl->GetRange(CameraControl_Zoom, &Min, &Max, &Step, &Default, &Flags);
		long zoomFlag = Flags;
		zoom += Min;
		if (zoom < Min)
			zoom = Min;
		if (zoom > Max)
			zoom = Max;

		if (SUCCEEDED(hr))
		{
			hr = pCameraControl->Set(CameraControl_Pan, pan, CameraControl_Flags_Manual);

			hr = pCameraControl->Set(CameraControl_Tilt, tilt, CameraControl_Flags_Manual);

			hr = pCameraControl->Set(CameraControl_Zoom, zoom, CameraControl_Flags_Manual);
		}
		else
		{
			printf("This device does not support PTZControl\n");
		}
	}
	if (pCameraControl != NULL)
		pCameraControl->Release();
	return hr;
}
HRESULT UVCCameraLibrary::moveHome()
{
	HRESULT hr;
	hr = stopMoving();
	hr = stopZooming();
	hr = stopFocusing();
	IAMCameraControl *pCameraControl = 0;
	hr = pDeviceFilter->QueryInterface(IID_IAMCameraControl, (void**)&pCameraControl);
	if (FAILED(hr))
	{
		// The device does not support IAMCameraControl
		printf("This device does not support IAMCameraControl\n");

	}
	else
	{
		long Min, Max, Step, Default, Flags, Val;

		// Get the range and default values 
		hr = pCameraControl->GetRange(CameraControl_Pan, &Min, &Max, &Step, &Default, &Flags);
		long panFlag = Flags;
		long panDefault = Default;
		hr = pCameraControl->GetRange(CameraControl_Tilt, &Min, &Max, &Step, &Default, &Flags);
		long tiltFlag = Flags;
		long tiltDefault = Default;
		hr = pCameraControl->GetRange(CameraControl_Zoom, &Min, &Max, &Step, &Default, &Flags);
		long zoomFlag = Flags;
		long zoomDefault = Default;
		hr = pCameraControl->Get(CameraControl_Focus, &Val, &Flags);
		long focusFlag = Flags;
		long focusVal = Val;
		if (SUCCEEDED(hr))
		{
			hr = pCameraControl->Set(CameraControl_Pan, panDefault, CameraControl_Flags_Manual);

			hr = pCameraControl->Set(CameraControl_Tilt, tiltDefault, CameraControl_Flags_Manual);

			hr = pCameraControl->Set(CameraControl_Zoom, zoomDefault, CameraControl_Flags_Manual);
			if (focusFlag != CameraControl_Flags_Auto)
				hr = pCameraControl->Set(CameraControl_Focus, Val, CameraControl_Flags_Auto);
		}
		else
		{
			printf("This device does not support PTZControl\n");
		}
	}
	if (pCameraControl != NULL)
		pCameraControl->Release();
	return hr;
}

bool UVCCameraLibrary::getAutoFocus()
{
	return getAuto(CameraControl_Focus);
}
long UVCCameraLibrary::getPan()
{
	return getVal(CameraControl_Pan);
}
long UVCCameraLibrary::getTilt()
{
	return getVal(CameraControl_Tilt);
}
long UVCCameraLibrary::getZoom()
{
	return getVal(CameraControl_Zoom);
}
long UVCCameraLibrary::getFocus()
{
	return getVal(CameraControl_Focus);
}

bool UVCCameraLibrary::getAuto(CameraControlProperty prop)
{
	HRESULT hr;
	IAMCameraControl *pCameraControl = 0;
	hr = pDeviceFilter->QueryInterface(IID_IAMCameraControl, (void**)&pCameraControl);
	if (FAILED(hr))
	{
		// The device does not support IAMCameraControl
		if (pCameraControl != NULL)
			pCameraControl->Release();
		printf("This device does not support IAMCameraControl\n");
		return false;

	}
	else
	{
		long Flags, Val;

		// Get the range and default values 
		hr = pCameraControl->Get(prop, &Val, &Flags);
		if (pCameraControl != NULL)
			pCameraControl->Release();
		if (SUCCEEDED(hr))
		{
			if (Flags == CameraControl_Flags_Auto)
				return true;
			else
				return false;
		}
		else
		{
			printf("This device does not support PTZControl\n");
			return false;
		}
	}
	
}
long UVCCameraLibrary::getVal(CameraControlProperty prop)
{
	HRESULT hr;
	IAMCameraControl *pCameraControl = 0;
	hr = pDeviceFilter->QueryInterface(IID_IAMCameraControl, (void**)&pCameraControl);
	if (FAILED(hr))
	{
		// The device does not support IAMCameraControl
		if (pCameraControl != NULL)
			pCameraControl->Release();
		printf("This device does not support IAMCameraControl\n");
		return 0;

	}
	else
	{
		long Min, Max, Step, Default, Flags, Val;

		// Get the range and default values 
		hr = pCameraControl->GetRange(prop, &Min, &Max, &Step, &Default, &Flags);
		hr = pCameraControl->Get(prop, &Val, &Flags);
		if (pCameraControl != NULL)
			pCameraControl->Release();
		if (SUCCEEDED(hr))
		{
			return Val - Min;
		}
		else
		{
			printf("This device does not support PTZControl\n");
			return 0;
		}
	}
}

HRESULT UVCCameraLibrary::stopMoving()
{
	stopControling(KSPROPERTY_CAMERACONTROL_PAN_RELATIVE);
	return stopControling(KSPROPERTY_CAMERACONTROL_TILT_RELATIVE);
}
HRESULT UVCCameraLibrary::stopZooming()
{
	return stopControling(KSPROPERTY_CAMERACONTROL_ZOOM_RELATIVE);
}
HRESULT UVCCameraLibrary::stopFocusing()
{
	return stopControling(KSPROPERTY_CAMERACONTROL_FOCUS_RELATIVE);
}

HRESULT UVCCameraLibrary::stopControling(KSPROPERTY_VIDCAP_CAMERACONTROL prop)
{
	HRESULT hr;
	IAMCameraControl *pCameraControl = 0;
	hr = pDeviceFilter->QueryInterface(IID_IAMCameraControl, (void**)&pCameraControl);
	if (FAILED(hr))
	{
		// The device does not support IAMCameraControl
		printf("This device does not support IAMCameraControl\n");

	}
	else
	{
		long Min, Max, Step, Default, Flags, Val;

		// Get the range and default values 
		hr = pCameraControl->GetRange(prop, &Min, &Max, &Step, &Default, &Flags);
		printf("Min %d , Max %d , Step %d", Min, Max, Step);
		if (SUCCEEDED(hr))
		{
			hr = pCameraControl->Set(prop, 0, KSPROPERTY_CAMERACONTROL_FLAGS_RELATIVE);
		}
		else
		{
			printf("This device does not support PTZControl\n");
		}
	}
	if (pCameraControl != NULL)
		pCameraControl->Release();
	return hr;
}