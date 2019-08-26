// MF_Carles_33236.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <atlbase.h>  // CComPtr
#include "MFormats_h.h"
#include <deque>

CComPtr<IMPreview> mixerPreview;
unsigned int _WorkerThread(void* threadParams);
std::deque<CComPtr<IMFFrame>> buffer;
CRITICAL_SECTION bufferCS;

int main(int argc, char* argv[])
{
	//Initialize COM
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
		return -1;

	/* license */	

	InitializeCriticalSection(&bufferCS);

	// standard
	M_AV_PROPS avProps = {};
	avProps.vidProps.eVideoFormat = eMVF_HD1080_50i;
	avProps.audProps = { 2, 44100, 32, 0 };

	/* preview */
	CComPtr<IMPreview> inputpreview;
	hr = inputpreview.CoCreateInstance(__uuidof(MFPreview));
	if (FAILED(hr))
		return -1;

	hr = mixerPreview.CoCreateInstance(__uuidof(MFPreview));
	if (FAILED(hr))
		return -1;

	CComBSTR channelID = CComBSTR(L"");
	BOOL enableVideo = FALSE;
	BOOL enableAudio = FALSE;
	hr = inputpreview->PreviewEnable(channelID, enableAudio, enableVideo);
	if (FAILED(hr))
		return false;

	hr = mixerPreview->PreviewEnable(channelID, enableAudio, enableVideo);
	if (FAILED(hr))
		return false;

	CComPtr<IMFDevice> live;
	hr = live.CoCreateInstance(__uuidof(MFLive));
	if (FAILED(hr))
		return -1;

	CComBSTR params = CComBSTR(L"");
	hr = live->DeviceSet(eMFDT_Video, 1, params); /* "NDI Receiver */
	if (FAILED(hr))
		return -1;

	CComQIPtr<IMFFormat> format(live);
	eMFormatType ft = eMFT_Input;
	M_VID_PROPS props = {};
	hr = format->FormatVideoSet(ft, &props);
	if (FAILED(hr))
		return -1;

	M_AUD_PROPS aProps = {};
	hr = format->FormatAudioSet(ft, &aProps);
	if (FAILED(hr))
		return -1;

#if 0
	/* looking for the first valid signal */
	while (1)
	{
		CComQIPtr<IMFProps> mfprops(live);
		int linesCount = 0;
		mfprops->PropsOptionGetCount(CComBSTR(L"line-in"), &linesCount);
		CComBSTR option, help;
		for (int i = 0; i < linesCount; i++)
		{
			mfprops->PropsOptionGetByIndex(CComBSTR(L"line-in"), i, &option, &help);
			mfprops->PropsOptionSetByIndex(CComBSTR(L"line-in"), i);
			break;
		}
		if (linesCount > 0) break;
		Sleep(2000);
	}
#endif

	CComPtr<IMFFactory> factory;
	factory.CoCreateInstance(__uuidof(MFFactory));

	/* black frame */
	CComPtr<IMFFrame> blackFrame;
	CComBSTR colorParameters = L"solid_color='Black(255)'";
	factory->MFFrameCreateFromMem(&avProps, 0, (long)0, (LONGLONG)0, &blackFrame, colorParameters);

	HANDLE workerThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)_WorkerThread, NULL, 0, NULL);

	printf("Press <Q> to exit\n");

	// loop
	bool running = true;
	while (running)
	{
		CComPtr<IMFFrame> outputFrame;
		REFERENCE_TIME maxWait = -1;

#if 0
		CComQIPtr<IMFSource> source = CComQIPtr<IMFSource>(live);

		double timeSec = -1.f;
		// REFERENCE_TIME maxWait = -1;
		CComBSTR hints = CComBSTR(L"");
		source->SourceFrameConvertedGetByTime(&avProps, timeSec, maxWait, &outputFrame, hints);
#else
		outputFrame = blackFrame;
#endif

		EnterCriticalSection(&bufferCS);
		buffer.push_back(outputFrame);
		LeaveCriticalSection(&bufferCS);

		CComQIPtr<IMFReceiver> receiverPreview(inputpreview);
		// REFERENCE_TIME maxWait = -1;
		CComBSTR props = CComBSTR(L"");
		receiverPreview->ReceiverFramePut(outputFrame, maxWait, props);
	}

	// 
	CoUninitialize();

	return 0;
}

unsigned int _WorkerThread(void* threadParams)
{
	Sleep(2000);

	while (1)
	{
		CComPtr<IMFFrame> outputFrame;

		EnterCriticalSection(&bufferCS);
		if (buffer.size() > 0)
		{
			outputFrame = buffer.back();
			buffer.pop_back();
		}
		LeaveCriticalSection(&bufferCS);

		if (outputFrame)
		{
			DWORD start = GetTickCount();

			CComQIPtr<IMFReceiver> receiverPreview(mixerPreview);
			REFERENCE_TIME maxWait = -1;
			CComBSTR props = CComBSTR(L"");
			receiverPreview->ReceiverFramePut(outputFrame, maxWait, props);

			printf("Elapsed: %d\n", GetTickCount() - start);
		}
		else
			Sleep(1);
	}
}

