//---------------------------------------------------------------------------
// This file is part of the MediaLooks DirectShow Template Library (DSTL)
//
// DSStdAfx.h : base precompiled header for DirectShow Template Library
//
//---------------------------------------------------------------------------
// Copyright (c), Medialooks Soft OOO
// http://www.medialooks.com/support/
//
//---------------------------------------------------------------------------
//
// This file and the source code contained within is Intellectual Property
// of Medialooks Soft OOO. Your use of this source code is subject to
// the Medialooks End-User License Agreement that can be found
// here: http://www.medialooks.com/la/.
//
//---------------------------------------------------------------------------

#ifndef __DSSTDAFX_H
#define __DSSTDAFX_H

#define _USE_MATH_DEFINES

// warning C4244: 'argument' : conversion from 'double' to 'GLfloat', possible loss of data
// warning C4305: 'argument' : truncation from 'double' to 'Ipp32f'
// warning C4996: 'sprintf': This function or variable may be unsafe. Consider using sprintf_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
// warning C4800: 'LPCWSTR' : forcing value to bool 'true' or 'false' (performance warning)
// warning C4995: 'wsprintfW': name was marked as #pragma deprecated
// warning C4311: 'type cast': pointer truncation from 'LPBYTE' to 'ULONG' (during ATLTRACE)
// warning C4302: 'type cast': truncation from 'LPBYTE' to 'ULONG' (during ATLTRACE)
#ifdef _WIN64
#pragma warning( disable : 4995 4996 4305 4244 4311 4302 )
#else
#pragma warning( disable : 4995 4996 4305 4244 )
#endif
#define FIX_Mainconcept_MediaSample2_MediaType

/////////////////////////////////////////////////////////////////////////////
// If you want to change logging level dynamically, define appropriate
// global variable in stdafx.h before #include "DSStdafx.h". For example:
//
//	#define TRACELEVEL_IBaseFilter g_dwTRACELEVEL_IBaseFilter
//	__declspec(selectany) unsigned int g_dwTRACELEVEL_IBaseFilter = 10;
//	...<skipped>...
//	#include "DSStdafx.h"

//---------------------------------
// Standard class-specific trace levels:
// 10 : low (normal + 2)
// 8 :  normal
// 6 :  high (normal - 2)

//---------------------------------
// Base trace levels

// For FinalConstruct()/FinalRelease() and some important calls
#ifndef TRACELEVEL_High
	#define TRACELEVEL_High				2
#endif

// For warnings. Trace prefix in log files: "WARNING."
#ifndef TRACELEVEL_Warning
	#define TRACELEVEL_Warning			4
#endif

// For errors. Trace prefix in log files: "###Err###"
#ifndef TRACELEVEL_Error
	#define TRACELEVEL_Error			2
#endif

// For fatal errors.
#ifndef TRACELEVEL_Fatal
	#define TRACELEVEL_Fatal			1
#endif

// If DSTrace.h not included before
#ifndef DSTL_TRACE_MODULE
#define  DSTL_TRACE_MODULE(hInstance) 
#endif

/////////////////////////////////////////////////////////////////////////////
// Base definitions
#include "DSDefines.h"

/////////////////////////////////////////////////////////////////////////////
// DirectX SDK Part

#pragma message("-----------------------------------------------------------------------")
#pragma message("In order for the .idl files to compile correctly, include path to DirectX .idl files in:")
#pragma message("Tools->Options...->Directories->Include = <DirectX directory>\\INCLUDE\\DSHOWIDL")
#pragma message(" ")
#pragma message("To compile correctly DirectX SDK directories must be first in the list:")
#pragma message("Testing Tools->Options...->Directories->Include files and Library files")
#pragma message("-----------------------------------------------------------------------")

#include <mpconfig.h>	// AM_ASPECT_RATIO_MODE, IMixerPinConfig, IMixerPinConfig2
#include <mmreg.h>		// Multimedia Registration: audio formats, etc.
#include <amvideo.h>	// Video interfaces and structures
#include <dvdmedia.h>	// DVD structures, etc.
#include <vfwmsgs.h>	// Error codes
#include <mediaerr.h>	// DMO error codes
#include <evcode.h>		// List of standard Quartz event codes and the expected params
#include <dvdevcod.h>   // DVD error event codes
#include <uuids.h>		// Declaration of type GUIDs and well-known clsids

#include <IL21Dec.h>	// IAMLine21Decoder
#include <dv.h>			// DV typedefs and defines

// Windows Driver Model/Connection and Streaming Architecture (WDM-CSA)
#include <ks.h>			// Core definitions
#include <ksmedia.h>	// Multimedia Definitions

//---------------------------------
#define __STREAMS__
#include <ksproxy.h>	// Interface definitions

// Added 2015-05-25 For compatibility with 8.1 SDK
#define __IKsControl_INTERFACE_DEFINED__

#ifdef __VIDEOPORT__
	// Videoport headers
	#include <Vptype.h>
	#include <Vpnotify.h>
	#include <Vpconfig.h>
#endif	// __VIDEOPORT__

//---------------------------------
// IMediaControl, IMediaPosition, etc.
#include <control.h>
#pragma comment(lib, "strmiids.lib")

#ifdef _DSTL_DIRECTSHOW_

// Time functions
#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")

// AMGetErrorText(...)
#include <errors.h>
#pragma comment(lib, "quartz.lib")

#endif

#ifndef OATRUE
	#define OATRUE (-1)
#endif // OATRUE

#ifndef OAFALSE
	#define OAFALSE (0)
#endif // OAFALSE

// Some definitions for old DirectX (6.0)
#ifdef __OLD_DS__

#define DWORD_PTR	DWORD
#define LONG_PTR	long

#endif	// __OLD_DS__

/////////////////////////////////////////////////////////////////////////////
// DirectShow Template Library part

// If DS_LOG_ENABLED is defined
//		if TRACE_MMX_STATE not defined: defining TRACE_MMX_STATE as empty
//		if TRACE_MMX_STATE defined: do nothing, TRACE_MMX_STATE defined
//									in "DirectShowX\DSDefines.h"
#if defined( DS_LOG_ENABLED )
	#ifndef TRACE_MMX_STATE
		#define	TRACE_MMX_STATE
	#endif // !TRACE_MMX_STATE
#endif	// !_DEBUG

//---------------------------------
// Guids and guid => name mapping
//#include "..\Filters\FilterUuids.h"
#include "DSGuids.h"

//#include "..\ippMerge.h"

//---------------------------------
#include "..\WinAPI\Macros.h"
#include "..\WinAPI\Guards.h"
#include "..\WinAPI\Functions.h"
#include "..\WinAPI\Wrappers.h"
#include "..\WinAPI\Containers.h"

//#include "..\Idl\DSTL.h"

#include "DSGuard.h"
#include "DSParsers.h"

// All methods are implemented only if defined DS_LOG_ENABLED
#include "DSDebug.h"

#include "DSVideo.h"
#include "DSTypes.h"

#ifdef _DSTL_DIRECTSHOW_
#include "../DSTL/DShowX/DSAllocators.h"
#include "../DSTL/DShowX/DSEnums.h"
#include "../DSTL/DShowX/DSWrappers.h"
#include "../DSTL/DShowX/DSFilters.h"
#endif

// For WASAPIBase.h -> as have definitions what conflict if included into several *.cpp file
// e.g. error LNK2005: _PKEY_DeviceInterface_FriendlyName already defined in CoMMixer.obj
// etc.
#include <Mmdeviceapi.h>
#include <Audioclient.h>
#include <Functiondiscoverykeys_devpkey.h> 


// Medialooks CLSIDs
#include "Medialooks_CLSID.h"

/////////////////////////////////////////////////////////////////////////////
#endif // __DSSTDAFX_H
