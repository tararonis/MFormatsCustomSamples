//---------------------------------------------------------------------------
// This file is part of the MediaLooks DirectShow Template Library (DSTL)
//
// DSDefines.h : base definitions for DirectShow Template Library.
//				 Also included some definitions not defined in DirectX SDK
//				 or defined only in DDKs
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

#ifndef __DSDEFINES_H_
#define __DSDEFINES_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <basetsd.h>
#include <strmif.h>

#include "..\WinAPI\Macros.h"
#include "uuids.h"

/////////////////////////////////////////////////////////////////////////////
// Definitions for SEH_... calls

#ifndef DS_SEH_HANDLER
	#define DS_SEH_HANDLER	EXCEPTION_EXECUTE_HANDLER
#endif

#ifndef TRACELEVEL_SEH
	#define TRACELEVEL_SEH	1
#endif

//////////////////////////////////////////////////////////////////////////
// Alignment definitions

#define STD_ALIGN	8			// Standard alignment (from some Win DDK files for DS)

#define ALIGN_UP(x, align)		( ((DWORD)(x) + (DWORD)(align) - 1) & ~((DWORD)(align) - 1) )
#define ALIGN_UP_PNT(x, align)	( ((DWORD_PTR)(x) + (DWORD_PTR)(align) - 1) & ~((DWORD_PTR)(align) - 1) )
#define ALIGN_DOWN(x, align)	( (DWORD)(x) & ~((DWORD)(align) - 1) )

#define ALIGN_UP_N64(x, align)		( ((UINT64)(x) + (UINT64)(align) - 1) & ~((UINT64)(align) - 1) )
#define ALIGN_DOWN_N64(x, align)	( (UINT64)(x) & ~((UINT64)(align) - 1) )

// #define ALIGN_UP_SIGN( x, align ) ((x) >= 0 ? ALIGN_UP( (x), align ) : ALIGN_UP( (x) - 1, align ) ) 
// #define ALIGN_DOWN_SIGN( x, align ) ((x) >= 0 ? ALIGN_DOWN( (x), align ) : ALIGN_DOWN( (x) - 1, align ) ) 

// The ALIGN_DOWN_SIGN -49 -> -50 (used e.g. for align negative height)
#define ALIGN_UP_SIGN( x, align ) ((x) >= 0 ? (int)ALIGN_UP( (x), align ) : -1 * (int)ALIGN_UP( -1 *(x), align ) ) 
#define ALIGN_DOWN_SIGN( x, align ) ((x) >= 0 ? (int)ALIGN_DOWN( (x), align ) : -1 * (int)ALIGN_DOWN( -1 * (x), align ) ) 

// The ALIGN_DOWN_POS -49 -> -48 (used e.g. for align negative pos)
#define ALIGN_DOWN_POS( x, align ) ((x) >= 0 ? (int)ALIGN_DOWN( (x), align ) : -1 * (int)ALIGN_UP( -1 * (x), align ) ) 
#define ALIGN_UP_POS( x, align ) ((x) >= 0 ? (int)ALIGN_UP( (x), align ) : -1 * (int)ALIGN_DOWN( -1 * (x), align ) ) 

#define WCHAR_ALLOC( nChar ) (LPWSTR)alloca(((nChar)+1) * sizeof(WCHAR))

#ifdef _DEBUG
#define DEBUG_FALSE	(false)
#else
#define DEBUG_FALSE	(true)
#endif

#ifdef _DEBUG
#define DEBUG_TRUE	(true)
#else
#define DEBUG_TRUE	(false)
#endif


//---------------------------------
// REFERENCE_TIME definitions/conversions

const REFERENCE_TIME MAX_CLOCK_TIME = 0x7FFFFFFFFFFFFFFF;


/////////////////////////////////////////////////////////////////////////////
#ifndef PIC_PROPS_DEFINED
#define PIC_PROPS_DEFINED

typedef struct tagPicProps
{
	// First DWORD code from subtype GUID (also for RGB)
	int	fccType;
	// Width of picture in pixels
	int	nWidth;

	// Height of picture in pixels
	// For RGB:
	//	positive value: bottom-up orientation
	//	negative value: top-down orientation
	int	nHeight;

	// Alignment width of buffer's row in pixels.
	// If nRowWidth == 0 then nRowWidth = nWidth
	int	nRowWidth;
} PIC_PROPS;

#endif	// PIC_PROPS_DEFINED

/////////////////////////////////////////////////////////////////////////////
// Base FOURCC for mediatypes/subtypes/etc.

// 00000000-0000-0010-8000-00AA00389B71			FOURCC_Base
extern "C" const __declspec(selectany) GUID FOURCC_Base =
	{0x00000000,0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}}; 

//---------------------------------
// Macros for video 

#define YUV(Y,U,V) ((DWORD)((BYTE)(Y)|(((WORD)(BYTE)(U))<<8)|(((DWORD)(BYTE)(V))<<16)))

#define GUID2FOURCC(guid)	((guid).Data1)

// FCC macro definition (from AVIRIFF.H file). Sample: FCC('div3')
#ifndef FCC
	#define FCC(ch4) ((((DWORD)(ch4) & 0xFF) << 24) |     \
		              (((DWORD)(ch4) & 0xFF00) << 8) |    \
			          (((DWORD)(ch4) & 0xFF0000) >> 8) |  \
				      (((DWORD)(ch4) & 0xFF000000) >> 24))
#endif

// Case insensitive DWORD compare e.g. 'Div3' == 'diV3' or 'DivX' == 'divx'
#define IsEqualFCC(fcc1, fcc2)	( ((fcc1) | 0x20202020) == ((fcc2) | 0x20202020) )

// Usage: if( IS_GUID_FCC(pmt->subtype, FCC('MP4A')) ) ...
#define IS_GUID_FCC(guid, fcc)	\
	( memcmp( (const BYTE*)&FOURCC_Base + sizeof(DWORD), (const BYTE*)&(guid) + sizeof(DWORD), \
	  sizeof(GUID) - sizeof(DWORD) ) == 0 && IsEqualFCC((guid).Data1, (fcc)) )

// FOURCC for supported video types
// First DWORD of subtype GUID (for sample: 'YUY2' == MEDIASUBTYPE_YUY2)
typedef DWORD	FOURCC;

const FOURCC FOURCC_UNK    = 0;
const FOURCC FOURCC_YV12   = FCC('YV12');
const FOURCC FOURCC_YVU9   = FCC('YVU9');
const FOURCC FOURCC_I420   = FCC('I420');
const FOURCC FOURCC_YUY2   = FCC('YUY2');
const FOURCC FOURCC_UYVY   = FCC('UYVY');
const FOURCC FOURCC_YVYU   = FCC('YVYU');
const FOURCC FOURCC_VYUY   = FCC('VYUY');
const FOURCC FOURCC_IYU2   = FCC('IYU2');
const FOURCC FOURCC_HDYC   = FCC('HDYC');
const FOURCC FOURCC_v210   = FCC('v210');
const FOURCC FOURCC_r210   = FCC('r210');
const FOURCC FOURCC_Y422   = FCC('Y422');
const FOURCC FOURCC_NV12   = FCC('NV12');
const FOURCC FOURCC_Y444   = FCC('Y444');
const FOURCC FOURCC_ARGB32 = 0x773c9ac0;
const FOURCC FOURCC_RGB32  = 0xe436eb7e;
const FOURCC FOURCC_RGB24  = 0xe436eb7d;
const FOURCC FOURCC_RGB565 = 0xe436eb7b;
const FOURCC FOURCC_RGB555 = 0xe436eb7c;
const FOURCC FOURCC_RGB8   = 0xe436eb7a;

/////////////////////////////////////////////////////////////////////////////
inline GUID __stdcall Fourcc2Guid(FOURCC fcc)
{
	// For wideo subtypes fcc > MAXWORD
	// For audio subtypes fcc <= MAXWORD
	// Conversions for RGB:
	//	FOURCC_RGB32 -> MEDIASUBTYPE_RGB32
	//	FOURCC_RGB24 -> MEDIASUBTYPE_RGB24
	//	FOURCC_RGB565 -> MEDIASUBTYPE_RGB565
	//	FOURCC_RGB555 -> MEDIASUBTYPE_RGB555
	//	FOURCC_RGB8 -> MEDIASUBTYPE_RGB8

	switch( fcc )
	{
	case FOURCC_ARGB32:
		return MEDIASUBTYPE_ARGB32;
	case FOURCC_RGB32:
		return MEDIASUBTYPE_RGB32;
	case FOURCC_RGB24:
		return MEDIASUBTYPE_RGB24;
	case FOURCC_RGB555:
		return MEDIASUBTYPE_RGB555;
	case FOURCC_RGB565:
		return MEDIASUBTYPE_RGB565;
	case FOURCC_RGB8:
		return MEDIASUBTYPE_RGB8;
	}

	GUID guid = FOURCC_Base;
	guid.Data1 = fcc;
	return guid;
}

inline FOURCC __stdcall Fcc2Lo(FOURCC fcc)
{
	BYTE* pb = (BYTE*)&fcc;
	if('A' <= pb[0] && pb[0] <= 'Z')
		pb[0] |= 0x20;
	if('A' <= pb[1] && pb[1] <= 'Z')
		pb[1] |= 0x20;
	if('A' <= pb[2] && pb[2] <= 'Z')
		pb[2] |= 0x20;
	if('A' <= pb[3] && pb[3] <= 'Z')
		pb[3] |= 0x20;
	return fcc;
}

inline bool __stdcall FccEqu(FOURCC fcc1, FOURCC fcc2)
{
	if(!IsEqualFCC(fcc1, fcc2))
		return false;

	BYTE* pb1 = (BYTE*)&fcc1;
	BYTE* pb2 = (BYTE*)&fcc2;

	BYTE bLo = (BYTE)(pb1[0] | 0x20);
	if(pb1[0] != pb2[0] && (bLo < 'a' || 'z' < bLo))
		return false;
	bLo = (BYTE)(pb1[1] | 0x20);
	if(pb1[1] != pb2[1] && (bLo < 'a' || 'z' < bLo))
		return false;
	bLo = (BYTE)(pb1[2] | 0x20);
	if(pb1[2] != pb2[2] && (bLo < 'a' || 'z' < bLo))
		return false;
	bLo = (BYTE)(pb1[3] | 0x20);
	if(pb1[3] != pb2[3] && (bLo < 'a' || 'z' < bLo))
		return false;

	return true;
}

inline bool __stdcall IsGuidFcc(REFGUID guid, FOURCC fcc)
{
	return (memcmp((const BYTE*)&FOURCC_Base + sizeof(DWORD), (const BYTE*)&guid + sizeof(DWORD), sizeof(GUID) - sizeof(DWORD)) == 0 &&
		FccEqu(guid.Data1, fcc));
}

inline WORD __stdcall Fourcc2Bits(FOURCC fcc)
{
	switch(fcc)
	{
		case FOURCC_YUY2 :
		case FOURCC_UYVY :
		case FOURCC_HDYC :
		case FOURCC_YVYU :
		case FOURCC_Y422 :
		case FOURCC_NV12 :
		case FOURCC_RGB565 :
		case FOURCC_RGB555 :	return 16;

		case FOURCC_YV12 :
		case FOURCC_I420 :		return 12;

		case FOURCC_IYU2 :
		case FOURCC_Y444 :		return 24;

		case FOURCC_ARGB32 :	
		case FOURCC_RGB32 :		return 32;

		case FOURCC_RGB24 :		return 24;

		case FOURCC_YVU9 :		return 9;

		case FOURCC_RGB8 :		return 8;

		case FOURCC_v210 :		return 20;

		case FOURCC_r210 :		return 32;
	}

	//ATLASSERT(!_T("Fcc2Bits(): unknown FOURCC code"));
	return 0;
}

inline bool __stdcall IsPlanar(FOURCC fcc)
{
	switch(fcc)
	{
		case FOURCC_YUY2 :
		case FOURCC_UYVY :
		case FOURCC_YVYU :
		case FOURCC_IYU2 :
		case FOURCC_HDYC :
		case FOURCC_r210 :
		case FOURCC_v210 :
		case FOURCC_RGB565 :
		case FOURCC_RGB8 :
		case FOURCC_RGB555 :
		case FOURCC_RGB24 :
		case FOURCC_RGB32 :	
		case FOURCC_ARGB32 :	return false;

		case FOURCC_YV12 :
		case FOURCC_I420 :
		case FOURCC_Y422 :
		case FOURCC_NV12 :
		case FOURCC_Y444 :		return true;
	}

	//ATLASSERT(!_T("IsPlanar(): unknown FOURCC code"));
	return false;
}

inline bool __stdcall IsRGB(FOURCC fcc, bool _b8BitsOnly = false)
{
	switch(fcc)
	{
		case FOURCC_v210 :
		case FOURCC_YUY2 :
		case FOURCC_UYVY :
		case FOURCC_HDYC :
		case FOURCC_YVYU :
		case FOURCC_YV12 :
		case FOURCC_IYU2 :
		case FOURCC_I420 :
		case FOURCC_Y422 :
		case FOURCC_NV12 :
		case FOURCC_Y444 :	return false;

		case FOURCC_r210 :
		case FOURCC_RGB555 :
		case FOURCC_RGB565 :
			if (_b8BitsOnly)
				return false;

		case FOURCC_RGB8:
		case FOURCC_RGB24 :
		case FOURCC_RGB32 :	
		case FOURCC_ARGB32 : return true;
	}

	//ATLASSERT(!_T("IsRGB(): unknown FOURCC code"));
	return false;
}

// This function is not correct: color space and number of bits
// are independent variables (see H.264)
inline bool __stdcall Is10Bit(FOURCC fcc)
{
	switch(fcc)
	{
		case FOURCC_v210 :
		case FOURCC_r210 :	return true;

		case FOURCC_YUY2 :
		case FOURCC_UYVY :
		case FOURCC_YVYU :
		case FOURCC_IYU2 :
		case FOURCC_HDYC :
		case FOURCC_RGB565 :
		case FOURCC_RGB8 :
		case FOURCC_RGB555 :
		case FOURCC_RGB24 :
		case FOURCC_RGB32 :
		case FOURCC_ARGB32 :
		case FOURCC_YV12 :
		case FOURCC_I420 :
		case FOURCC_Y422 :
		case FOURCC_NV12 :
		case FOURCC_Y444 :	return false;
	}

	//ATLASSERT(!_T("Is10Bit(): unknown FOURCC code"));
	return false;
}

// For compare ARGB & RGB32, UYVY & HDYC
inline bool __stdcall IsSimilarFCC(FOURCC fcc1, FOURCC fcc2, bool _bIgnoreYUV_Matrix = true)
{
	if( fcc1 == FOURCC_HDYC && _bIgnoreYUV_Matrix)
		fcc1 = FOURCC_UYVY;
	if( fcc1 == FOURCC_RGB32 )
		fcc1 = FOURCC_ARGB32;

	if( fcc2 == FOURCC_HDYC && _bIgnoreYUV_Matrix)
		fcc2 = FOURCC_UYVY;
	if( fcc2 == FOURCC_RGB32 )
		fcc2 = FOURCC_ARGB32;

	return fcc1 == fcc2;
}

/////////////////////////////////////////////////////////////////////////////
// Audio formats
#ifndef WAVE_FORMAT_MP3
	#define WAVE_FORMAT_MP3		0x0055
#endif	// WAVE_FORMAT_MP3

/////////////////////////////////////////////////////////////////////////////
// Structs and definitions for mapping GUIDs to readable strings
typedef	struct tagGUID_NAME
{
	const GUID* pguid;
	LPCTSTR	pszName;
} GUID_NAME;

typedef	struct tagGuidNameNode
{
	GUID_NAME*			pGuidName;
	DWORD				cElems;
	tagGuidNameNode*	pPrevNode;
} GuidNameNode;

#define PREV_GUID_NAME_NODE	NULL

#define GUID_NAME_NODE(node, arrGuidName)	\
	__declspec(selectany) GuidNameNode node =	\
	{ arrGuidName, sizeof(arrGuidName)/sizeof(arrGuidName[0]), PREV_GUID_NAME_NODE };

#define __GUIDSTR(guid)	{ &guid, _T(#guid) }
#define __UUIDSTR(uiid) { &__uuidof(uiid), _T(#uiid) }

///////////////////////////////////////////////////////////////////////////
// Min/default/max values for fields of DDCOLORCONTROL structure

// DDCOLORCONTROL::lBrightness
// Specifies the luminance intensity (Black Level), in IRE units*100.
// This member can range from 0 to 10,000, with a default value of 750 (7.5 IRE). 
#define DDCOLOR_BRIGHTNESS_MIN		0
#define DDCOLOR_BRIGHTNESS_DEF		750
#define DDCOLOR_BRIGHTNESS_MAX		10000

// DDCOLORCONTROL::lContrast
// Specifies the relative difference between higher and lower intensity luminance values, in IRE units*100.
// This member can range from 0 to 20,000, with a default value of 10,000 (100 IRE)
#define DDCOLOR_CONTRAST_MIN		0
#define DDCOLOR_CONTRAST_DEF		10000
#define DDCOLOR_CONTRAST_MAX		20000

// DDCOLORCONTROL::lHue
// Specifies the phase relationship of the chrominance components.
// Hue is specified in degrees, with a valid range of -180 to 180. The default is 0 degrees.
#define DDCOLOR_HUE_MIN				-180
#define DDCOLOR_HUE_DEF				0
#define DDCOLOR_HUE_MAX				180

// DDCOLORCONTROL::lSaturation
// Specifies the color intensity, in IRE units*100.
// This member can range from 0 to 20,000, with a default value of 10,000 (100 IRE). 
#define DDCOLOR_SATURATION_MIN		0
#define DDCOLOR_SATURATION_DEF		10000
#define DDCOLOR_SATURATION_MAX		20000

// DDCOLORCONTROL::lSharpness
// Specifies the sharpness in arbitrary units. The valid range is 0 to 10, with a default value of 5.
#define DDCOLOR_SHARPNESS_MIN		0
#define DDCOLOR_SHARPNESS_DEF		5
#define DDCOLOR_SHARPNESS_MAX		10

// DDCOLORCONTROL::lGamma
// Controls the amount of gamma correction applied to the luminance values.
// The valid range is 1 to 500 gamma units, with a default of 1.
#define DDCOLOR_GAMMA_MIN			1
#define DDCOLOR_GAMMA_DEF			1
#define DDCOLOR_GAMMA_MAX			500

// DDCOLORCONTROL::lColorEnable
// Specifies a flag indicating whether color is used.
// If this member is zero, color is not used; if it is 1, then color is used. The default value is 1. 
#define DDCOLOR_COLORENABLE_DEF		1

/////////////////////////////////////////////////////////////////////////////
#endif	// __DSDEFINES_H_
