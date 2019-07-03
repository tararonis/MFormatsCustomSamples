// Sample File Playback Console.cpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "MIDL/MFormats.h"
#include "Include/ArgParse/get_arguments.h"
#include "Include/ArgParse/platform_os.h"
using namespace std;

int EnumAndSelectVideoFormat( int _nSelFormat )
{
	if (_nSelFormat > 0 && _nSelFormat <= 17)
		return _nSelFormat;

	_tprintf(_T("Video formats:\n"));
	_tprintf(_T("1: NTSC - SMPTE 259M - SD NTSC standard (720x486@59.94i)\n"));
	_tprintf(_T("2: PAL - SMPTE 259M - SD PAL standard  (720x576@50i) \n"));
	_tprintf(_T("3: HD720_50p - SMPTE 296M - HD 720p @ 50Hz standard \n"));
	_tprintf(_T("4: HD720_5994p - SMPTE 296M - HD 720p @ 60/1001Hz standard \n"));
	_tprintf(_T("5: HD720_60p - SMPTE 296M - HD 720p @ 60Hz standard\n"));
	_tprintf(_T("6: HD1080_50i - SMPTE 274M - HD 1080i @ 50Hz standard\n"));
	_tprintf(_T("7: HD1080_5994i - SMPTE 274M - HD 1080i @ 60/1001Hz standard \n"));
	_tprintf(_T("8: eMVF_HD1080_60i - SMPTE 274M - HD 1080i @ 60Hz standard \n"));
	_tprintf(_T("9: eMVF_HD1080_50p - SMPTE 274M - HD 1080p @ 50Hz standard \n"));
	_tprintf(_T("10: eMVF_HD1080_5994p - SMPTE 274M - HD 1080i @ 60/1001Hz standard\n"));
	_tprintf(_T("11: HD1080_60p - SMPTE 274M - HD 1080p @ 60Hz standard \n"));
	_tprintf(_T("12: 2K_2398p - 2K Resolution 2048x1556 @ 24/1001 Hz\n"));
	_tprintf(_T("13: 2K_24p - 2K Resolution 2048x1556 @ 24 Hz\n"));
	_tprintf(_T("14: 2K_25p - 2K Resolution 2048x1556 @ 25 Hz\n"));
	_tprintf(_T("15: 4K_UHD_25p\n"));
	_tprintf(_T("16: 4K_UHD_2997p\n"));
	_tprintf(_T("17: 4K_UHD_30p \n\n"));
	_tprintf(_T("Select the number of video format:\n"));
	int nFormatRes = 0;
	cin >> nFormatRes;
	if (nFormatRes > 0 && nFormatRes <= 17)
	{
		return nFormatRes;
	}
	else
	{
		_tprintf(_T("ERROR: Invalid format number selected, skipping"));
		return 0;
	}
}

int EnumAndSelectRenderer( int _nSelRenderer )
{
	CComPtr<IMFDevice> cpRenderer;  //IMFDevice pointer
	HRESULT hr = cpRenderer.CoCreateInstance(__uuidof(MFRenderer));
	if (FAILED(hr))
	{
		_tprintf(_T("ERROR: Can't create renderer object instance, continuing \n"));
		return 0;
	}
	//Probe renderer passed as argument
	CComBSTR cbsName;
	BOOL bBusy;
	if (_nSelRenderer > 0)
	{
		cpRenderer->DeviceGetByIndex(eMFDT_Video, _nSelRenderer - 1, &cbsName, &bBusy);
		if (cbsName.Length())
			return _nSelRenderer;
	}

	//Get rendering devices count
	int nDevices = 0;
	cpRenderer->DeviceGetCount(eMFDT_Video, &nDevices);
	if (nDevices == 0)
	{
		_tprintf(_T("No available rendering devices found, continuing \n"));
		return 0;
	}
	_tprintf(_T("Rendering devices list:\n"));
	//Enumerate devices
	for (int i = 0; i < nDevices; i++)
	{
		cpRenderer->DeviceGetByIndex(eMFDT_Video, i, &cbsName, &bBusy);
		_tprintf(_T("%i: %s\n"),i + 1, cbsName);
	}
	//Probe selected renderer
	_tprintf(_T("Select the number of the rendering device\n"));
	int nRenderer = 0;
	cin >> nRenderer;
	hr = cpRenderer->DeviceGetByIndex(eMFDT_Video, nRenderer - 1, &cbsName, &bBusy);
	if (FAILED(hr) || !cbsName.Length())
	{
		_tprintf(_T("ERROR: Can't use this device for rendering \n"));
		return 0;
	}
	return nRenderer;
}

HRESULT FilePlay( CComBSTR cbsFileName, int nFormat, int nRenderer )
{
	CComPtr<IMFReader> cpReader;    //IMFReader pointer
	CComPtr<IMFDevice> cpRenderer;  //IMFDevice pointer
	CComPtr<IMPreview> cpPreview;   //IMPreview pointer
	M_AV_PROPS avProps = {};        //Video props structure

	// Create MFReader instance
	HRESULT hr = cpReader.CoCreateInstance(__uuidof(MFReader));
	if (FAILED(hr))
	{
		_tprintf(_T("ERROR: Can't create MFReader instance \n"));
		return E_FAIL;
	}
	//Open File
	hr = cpReader->ReaderOpen(cbsFileName, CComBSTR(L""));
	if (FAILED(hr))
	{
		_tprintf(_T("ERROR: Can't open this media file\n"));
		return E_FAIL;
	}

	//Initialize preview
	hr = cpPreview.CoCreateInstance(__uuidof(MFPreview));
	if (FAILED(hr))
	{
		_tprintf(_T("ERROR: Can't create MFPreview instance \n"));
		return E_FAIL;
	}
	//Enable video and audio preview
	hr = cpPreview->PreviewEnable(CComBSTR(L""), true, true);


	//Set video format, if selected
	if (nFormat > 0)
	{
		switch (nFormat)
		{
		case 1:
			avProps.vidProps.eVideoFormat = eMVF_NTSC;
			break;
		case 2:
			avProps.vidProps.eVideoFormat = eMVF_PAL;
			break;
		case 3:
			avProps.vidProps.eVideoFormat = eMVF_HD720_50p;
			break;
		case 4:
			avProps.vidProps.eVideoFormat = eMVF_HD720_5994p;
			break;
		case 5:
			avProps.vidProps.eVideoFormat = eMVF_HD720_60p;
			break;
		case 6:
			avProps.vidProps.eVideoFormat = eMVF_HD1080_50i;
			break;
		case 7:
			avProps.vidProps.eVideoFormat = eMVF_HD1080_5994i;
			break;
		case 8:
			avProps.vidProps.eVideoFormat = eMVF_HD1080_60i;
			break;
		case 9:
			avProps.vidProps.eVideoFormat = eMVF_HD1080_50p;
			break;
		case 10:
			avProps.vidProps.eVideoFormat = eMVF_HD1080_5994p;
			break;
		case 11:
			avProps.vidProps.eVideoFormat = eMVF_HD1080_60p;
			break;
		case 12:
			avProps.vidProps.eVideoFormat = eMVF_2K_2398p;
			break;
		case 13:
			avProps.vidProps.eVideoFormat = eMVF_2K_24p;
			break;
		case 14:
			avProps.vidProps.eVideoFormat = eMVF_2K_25p;
			break;
		case 15:
			avProps.vidProps.eVideoFormat = eMVF_4K_UHD_25p;
			break;
		case 16:
			avProps.vidProps.eVideoFormat = eMVF_4K_UHD_2997p;
			break;
		case 17:
			avProps.vidProps.eVideoFormat = eMVF_4K_UHD_30p;
			break;
		default:
			avProps.vidProps.eVideoFormat = eMVF_Custom;
			break;
		}
	}

	//Set renderer, if selected
	if (nRenderer != 0)
	{
		HRESULT hr = cpRenderer.CoCreateInstance(__uuidof(MFRenderer));
		if (FAILED(hr))
		{
			_tprintf(_T("ERROR: Can't create MFRenderer instance, continuing \n"));
		}
		else
		{
			hr = cpRenderer->DeviceSet(eMFDT_Video, nRenderer - 1, CComBSTR(L""));
			if (FAILED(hr))
			{
				_tprintf(_T("ERROR: Can't set this device for rendering \n"));
			}
		}
	}


	CComPtr<IMFFrame> cpFrame;
	CComQIPtr<IMFReceiver> cpReceiverPreview (cpPreview);
	CComQIPtr<IMFReceiver> cpReceiverRenderer (cpRenderer);
	
	M_TIME mTime = {};

	M_AV_PROPS vProps = {};
	long samples = 0;
	//Get frames one by one from the source file and send them to preview/renderer
	_tprintf(_T("Playing... Press ESC to stop playback\n"));
	while (GetAsyncKeyState(VK_ESCAPE)==0)
	{
		cpFrame = NULL;
		//Get frame by number. -1 as frame number means next frame
		hr = cpReader->SourceFrameConvertedGetByTime(&avProps, -1, -1, &cpFrame, CComBSTR(L""));
		if (cpFrame)
		{
			//Check for the last frame to stop the playback
			hr = cpFrame->MFTimeGet(&mTime);
			if ((mTime.eFFlags & eMFF_Last) != 0)
				break;
			//Send frame to preview
			if (cpReceiverPreview)
			cpReceiverPreview->ReceiverFramePut(cpFrame, -1, CComBSTR(L""));
			
			cpFrame->MFAVPropsGet(&vProps, &samples);
			cout << "Count of Channels: " + vProps.audProps.nChannels << endl;			

			for (size_t i = 0; i < vProps.audProps.nChannels; i++)
			{
				cout << vProps.ancData.audOutput.arrVUMeter[i] << endl;
			}


			//Send frame to renderer
			if (nRenderer > 0 && cpReceiverRenderer)
				cpReceiverRenderer->ReceiverFramePut(cpFrame, -1, CComBSTR(L""));
		}
	}

	if (cpRenderer)
		cpRenderer->DeviceClose();
	if (cpReader)
		cpReader->ReaderClose();

	cpFrame = NULL;
	cpRenderer = NULL;
	cpReader = NULL;

	return S_OK;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//Initialize COM
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		_tprintf(_T("ERROR: Can't initialize COM \n"));
		return 0;
	}

	//Check if there is no arguments and print usage
	if (argc == 1)
	{
		_tprintf(_T("Plays a single media file using MFReader object\n\n"));
		_tprintf(_T("USAGE:\n"));
		_tprintf(_T("SampleFilePlaybackConsole.exe filename [-f][FormatNumber] [-r][RendererNumber] \n\n"));
		_tprintf(_T("Where: \n"));
		_tprintf(_T("filename \t Full path to media file\n"));
		_tprintf(_T("-f\tChoose video format\n"));
		_tprintf(_T("FormatNumber\tvideo format number)\n"));
		_tprintf(_T("-r\tChoose renderer to output video stream\n"));
		_tprintf(_T("RendererNumber\trenderer number\n"));

		_tprintf(_T("/n Press ESC to stop playback\n"));
	}

	//Parse arguments
	static const ArgOption_t argument_option_array[] =
	{
		{ _T('f'), NULL, OPTION_NO_PARAMETER, },
		{ _T('r'), NULL, OPTION_NO_PARAMETER, },
		{ 0, NULL, OPTION_NO_PARAMETER }
	};

	CComBSTR cbsFileName;            //Path to media file
	bool bChooseFormat = false;      //Choose video format
	bool bChooseRenderer = false;    //Chose renderer
	int nSelFormat = 0;
	int nSelRenderer = 0;
	int arg_result = 0;
	int arg_index = 0;
	int positional_arg_index = 0;
	while (  (arg_result = get_arguments(argc,argv,&argument_option_array[0], &arg_index)) > ARG_NO_MORE_ARGUMENTS )
	{
		if (arg_result != ARG_POSITIONAL_ARGUMENT)
		{
			//Processing optional arguments
			switch (arg_result)
			{
			case 'f':
				bChooseFormat = true;
				if ((arg_index + 1) < argc)
					nSelFormat = _ttoi(argv[arg_index + 1]);
				break;
			case 'r':
				bChooseRenderer = true;
				if ((arg_index + 1) < argc)
					nSelRenderer = _ttoi(argv[arg_index + 1]);
				break;
			default:
				//Unknown arguments will be skipped
				break;
			}
		}
		else
		{
			//Parsing positional argument. We suppose filename to be at the first place
			if (arg_index == 1)
			{
				cbsFileName = CComBSTR(argv[1]);
			}
		}
	}

	//Check for filename
	if( cbsFileName.Length() )
	{
		int nFormat = 0;
		int nRenderer = 0;
		if (bChooseFormat)
			nFormat = EnumAndSelectVideoFormat(nSelFormat);
		if (bChooseRenderer)
			nRenderer = EnumAndSelectRenderer(nSelRenderer);

		FilePlay(cbsFileName, nFormat, nRenderer);
	}
	else
	{
		_tprintf(_T("No valid media file specified for the playback"));
	}

	::CoUninitialize();
	return 0;
}

