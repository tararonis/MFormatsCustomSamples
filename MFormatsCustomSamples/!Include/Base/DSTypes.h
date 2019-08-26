//---------------------------------------------------------------------------
// This file is part of the MediaLooks DirectShow Template Library (DSTL)
//
// DSTypes.h : functions and classes for media types (AM_MEDIA_TYPE).
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

#ifndef __DSTYPES_H_
#define __DSTYPES_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DSVideo.h"
#include "DSGuard.h"

/////////////////////////////////////////////////////////////////////////////
// MediaType functions

void __stdcall MTInit(IN OUT AM_MEDIA_TYPE* pmt, IN REFGUID major = GUID_NULL, IN REFGUID sub = GUID_NULL);
AM_MEDIA_TYPE* __stdcall MTCreate(IN REFGUID major, IN REFGUID sub = GUID_NULL,
					  IN REFGUID format = GUID_NULL, IN ULONG cb = 0, IN BYTE* pb = NULL);

AM_MEDIA_TYPE* __stdcall MTDup(IN const AM_MEDIA_TYPE* pSrc);
HRESULT __stdcall MTCopy(IN OUT AM_MEDIA_TYPE* pDst, IN const AM_MEDIA_TYPE* pSrc);
HRESULT __stdcall MTDupFormat(IN const AM_MEDIA_TYPE* pmt, OUT BYTE** ppbFormat);
BYTE*	__stdcall MTAllocFormat(IN const ULONG cb, IN const BYTE* pb = NULL);

void __stdcall MTClear(IN OUT AM_MEDIA_TYPE *pmt, IN REFGUID major = GUID_NULL, IN REFGUID sub = GUID_NULL);
void __stdcall MTClearTail(IN OUT AM_MEDIA_TYPE* pmt);
void __stdcall MTFree(IN OUT AM_MEDIA_TYPE *pmt);

bool __stdcall MTIsValid(IN const AM_MEDIA_TYPE* pmt);
bool __stdcall MTIsDefined(const AM_MEDIA_TYPE* pmt);
bool __stdcall MTIsMatched(const AM_MEDIA_TYPE* pmt, REFGUID major = GUID_NULL,
						   REFGUID sub = GUID_NULL, REFGUID format = GUID_NULL);
bool __stdcall MTIsSubset(IN const AM_MEDIA_TYPE* pSuper, IN const AM_MEDIA_TYPE* pSub);
bool __stdcall MTIsEqual(IN const AM_MEDIA_TYPE* pmt1, IN const AM_MEDIA_TYPE* pmt2);
bool __stdcall MTIsEqSubtypes(IN REFGUID sub1, IN REFGUID sub2);

BITMAPINFOHEADER* __stdcall MTBmpHeader(IN const AM_MEDIA_TYPE* pmt);
SIZE __stdcall MTResolution(IN const AM_MEDIA_TYPE* pmt);

REFERENCE_TIME __stdcall MTAverageTime(IN const AM_MEDIA_TYPE* pmt);
HRESULT __stdcall MTSetAverageTime(IN AM_MEDIA_TYPE* pmt, REFERENCE_TIME avgTime );

//---------------------------------------------------------------------------

inline void __stdcall MTInit(IN OUT AM_MEDIA_TYPE* pmt, IN REFGUID major /*= GUID_NULL*/,
							 IN REFGUID sub /*= GUID_NULL*/)
{
	ATLASSERT(pmt != NULL);
	ZeroMemory(pmt, sizeof(AM_MEDIA_TYPE));
	pmt->majortype = major;
	pmt->subtype = sub;
	pmt->lSampleSize = 1;
	pmt->bFixedSizeSamples = TRUE;
}

// If cb != 0 && pb == NULL : asign empty format block with size = cb
inline AM_MEDIA_TYPE* __stdcall MTCreate(IN REFGUID major, IN REFGUID sub /*= GUID_NULL*/,
										 IN REFGUID format /*= GUID_NULL*/,
										 IN ULONG cb /*= 0*/, IN BYTE* pb /*= NULL*/)
{
	ATLASSERT( !(cb == 0 && pb != NULL) );
	if(cb == 0 && pb != NULL) return NULL;

	AM_MEDIA_TYPE* pmt = (AM_MEDIA_TYPE*)::CoTaskMemAlloc(sizeof(AM_MEDIA_TYPE));
	if(pmt != NULL)
	{
		MTInit(pmt, major, sub);
		pmt->formattype = format;
		if(cb > 0)
		{
			pmt->pbFormat = MTAllocFormat(cb, pb);
			if(pmt->pbFormat == NULL)
			{
				MTFree(pmt);
				pmt = NULL;
			}
			else
				pmt->cbFormat = cb;
		}
	}
	return pmt;
}

inline AM_MEDIA_TYPE* __stdcall MTDup(IN const AM_MEDIA_TYPE* pSrc)
{
	if(pSrc == NULL) return NULL;

	AM_MEDIA_TYPE* pDst = (AM_MEDIA_TYPE*)::CoTaskMemAlloc(sizeof(AM_MEDIA_TYPE));
	if(pDst != NULL)
	{
		if(FAILED( MTCopy(pDst, pSrc) ))
		{
			::CoTaskMemFree(pDst);
			pDst = NULL;
		}
	}
	else
		ATLASSERT(!_T("MTDup() : CoTaskMemAlloc() == NULL"));
	return pDst;
}

// ATTENTION.
// pDst should be pointer to buffer of sizeof(AM_MEDIA_TYPE) bytes.
// Previos contents of pDst (pbFormat, pUnk) aren't cleared.
inline HRESULT __stdcall MTCopy(IN OUT AM_MEDIA_TYPE* pDst, IN const AM_MEDIA_TYPE* pSrc)
{
	ATLASSERT(pDst != NULL);
	if(pDst == NULL) return E_INVALIDARG;

	BYTE* pbFormat = NULL;
	HRESULT hr = MTDupFormat(pSrc, &pbFormat);
	if(FAILED(hr))
		return hr;

	CopyMemory(pDst, pSrc, sizeof(AM_MEDIA_TYPE));
	pDst->pbFormat = pbFormat;
	if(pSrc->pUnk != NULL) pSrc->pUnk->AddRef();
	return S_OK;
}

// ATTENTION. if pmt->pbFormat == NULL, then pmt->cbFormat should be == 0
inline HRESULT __stdcall MTDupFormat(IN const AM_MEDIA_TYPE* pmt, OUT BYTE** ppbFormat)
{
	ATLASSERT(pmt != NULL);
	if(pmt == NULL) return E_INVALIDARG;

	ATLASSERT(ppbFormat != NULL);
	if(ppbFormat == NULL) return E_POINTER;

	*ppbFormat = NULL;

	if(pmt->pbFormat == NULL)
	{
		ATLASSERT(pmt->cbFormat == 0);
		return (pmt->cbFormat == 0)? S_OK : E_UNEXPECTED;
	}

	ATLASSERT(pmt->cbFormat != 0);
	if(pmt->cbFormat == 0) return E_UNEXPECTED;

	*ppbFormat = MTAllocFormat(pmt->cbFormat, pmt->pbFormat);
	return (*ppbFormat != NULL)? S_OK : E_OUTOFMEMORY;
}

inline BYTE* __stdcall MTAllocFormat(IN const ULONG cb, IN const BYTE* pb /*= NULL*/)
{
	ATLASSERT(cb > 0);
	if(cb == 0) return NULL;

	BYTE* pbFormat = (BYTE*)::CoTaskMemAlloc(cb);
	if(pbFormat == NULL)
	{
		ATLASSERT(!_T("MTAllocFormat() : CoTaskMemAlloc() == NULL"));
	}
	else
	{
		if(pb != NULL)
			CopyMemory(pbFormat, pb, cb);
		else
			ZeroMemory(pbFormat, cb);
	}
	return pbFormat;
}

inline void __stdcall MTClear(IN OUT AM_MEDIA_TYPE *pmt, IN REFGUID major /*= GUID_NULL*/,
							  IN REFGUID sub /*= GUID_NULL*/)
{
	if(pmt != NULL)
	{
		::CoTaskMemFree(pmt->pbFormat);
		if(pmt->pUnk != NULL)
			pmt->pUnk->Release();

		MTInit(pmt, major, sub);
	}
}

inline void __stdcall MTClearTail(IN OUT AM_MEDIA_TYPE* pmt)
{
	if(pmt != NULL)
	{
		if(pmt->pbFormat)
		{
			::CoTaskMemFree(pmt->pbFormat);
			pmt->pbFormat = NULL;
			pmt->cbFormat = 0;
		}
		if(pmt->pUnk != NULL)
		{
			pmt->pUnk->Release();
			pmt->pUnk = NULL;
		}
	}
}

inline void __stdcall MTFree(IN OUT AM_MEDIA_TYPE *pmt)
{
	if(pmt != NULL)
	{
		::CoTaskMemFree(pmt->pbFormat);
		if(pmt->pUnk != NULL)
			pmt->pUnk->Release();
		::CoTaskMemFree(pmt);
	}
}

//---------------------------------
// Mediatype check/compare

inline bool __stdcall MTIsValid(IN const AM_MEDIA_TYPE* pmt)
{
	return (pmt != NULL && !IsEqualGUID(pmt->majortype, GUID_NULL));
}

inline bool __stdcall MTIsDefined(const AM_MEDIA_TYPE* pmt)
{
	if(!::MTIsValid(pmt))
		return false;

	//---------------------------------
	if( pmt->majortype == MEDIATYPE_Video ||
		pmt->majortype == MEDIATYPE_Audio ||
		pmt->majortype == MEDIATYPE_Interleaved ||
		pmt->majortype == MEDIATYPE_MPEG2_PES)
	{
		// Subtype, formattype and format block should be defined 
		if(	pmt->subtype == GUID_NULL ||
			pmt->formattype == GUID_NULL ||
			pmt->formattype == FORMAT_None ||
			pmt->cbFormat == 0 ||
			pmt->pbFormat == NULL)
			return false;

		return true;
	}
	else if(pmt->majortype == MEDIATYPE_DVD_ENCRYPTED_PACK ||
			pmt->majortype == MEDIATYPE_AUXLine21Data ||
			pmt->majortype == MEDIATYPE_VBI)
	{
		// Subtype should be defined
		if(pmt->subtype == GUID_NULL)
			return false;

		return true;
	}
	else if(pmt->majortype == MEDIATYPE_Text ||
			pmt->majortype == MEDIATYPE_Midi ||
			pmt->majortype == MEDIATYPE_ScriptCommand ||
			pmt->majortype == MEDIATYPE_Timecode ||
			pmt->majortype == MEDIATYPE_AnalogAudio)
	{
		// Subtype should be == GUID_NULL
		if(pmt->subtype == GUID_NULL)
			return true;

		return false;
	}

	// Another majortypes have unknown subtype/formattype
	//		MEDIATYPE_Stream
	//		MEDIATYPE_File
	//		MEDIATYPE_AnalogVideo
	//		MEDIATYPE_MPEG1SystemStream
	//		MEDIATYPE_MPEG2_PACK
	//		MEDIATYPE_DVD_NAVIGATION
	//		MEDIATYPE_URL_STREAM
	//		MEDIATYPE_LMRT
	//		???

	return true;
}

inline bool __stdcall MTIsMatched(const AM_MEDIA_TYPE* pmt, REFGUID major /*= GUID_NULL*/,
								  REFGUID sub /*= GUID_NULL*/, REFGUID format /*= GUID_NULL*/)
{
	if(!::MTIsValid(pmt))
		return false;
	
	if(major != GUID_NULL && pmt->majortype != major)
		return false;
	if(sub != GUID_NULL && !::MTIsEqSubtypes(pmt->subtype, sub))
		return false;
	if(format == GUID_NULL)
	{
		// Don't check pmt->formattype, pmt->cbFormat and pmt->pbFormat
		return true;
	}
	if(pmt->formattype != format)
		return false;

	//---------------------------------
	DWORD cb = 0;
	if(format == FORMAT_VideoInfo)
		cb = sizeof(VIDEOINFOHEADER);
	else if(format == FORMAT_VideoInfo2)
		cb = sizeof(VIDEOINFOHEADER2);
	else if(format == FORMAT_MPEGVideo)
		cb = sizeof(MPEG1VIDEOINFO);
	else if(format == FORMAT_MPEG2Video)
		cb = sizeof(MPEG2VIDEOINFO);
	else if(format == FORMAT_DvInfo)
		cb = sizeof(DVINFO);
	else if(format == FORMAT_WaveFormatEx)
		cb = sizeof(WAVEFORMATEX);

	if(cb > 0)
		return (pmt->cbFormat >= cb && pmt->pbFormat != NULL);

	// Check for unknown format structures
	return ( (pmt->cbFormat == 0 && pmt->pbFormat == NULL) ||
			 (pmt->cbFormat > 0 && pmt->pbFormat != NULL) );
}

inline bool __stdcall MTIsSubset(IN const AM_MEDIA_TYPE* pSuper, IN const AM_MEDIA_TYPE* pSub)
{
	if(!MTIsValid(pSuper) || !MTIsValid(pSub))
		return false;
	if(!IsEqualGUID(pSuper->majortype, pSub->majortype))
		return false;

	if(IsEqualGUID(pSuper->subtype, GUID_NULL))
	{
		// Don't check subtype, format and cbFormat/pbFormat
		return true;
	}
	if(!MTIsEqSubtypes(pSuper->subtype, pSub->subtype))
		return false;

	if(IsEqualGUID(pSuper->formattype, GUID_NULL))
	{
		// Don't check format and cbFormat/pbFormat
		return true;
	}
	if(!IsEqualGUID(pSuper->formattype, pSub->formattype))
		return false;

	if(pSuper->cbFormat == 0)
	{
		// Don't check cbFormat/pbFormat
		return true;
	}
	if(pSuper->cbFormat != pSub->cbFormat)
		return false;

	if(pSuper->pbFormat == NULL || pSub->pbFormat == NULL)
		return false;

	return (memcmp(pSuper->pbFormat, pSub->pbFormat, pSuper->cbFormat) == 0);
}

// Comment from MS Direct Show base classes :
//		"I don't believe we need to check sample size or
//		temporal compression flags, since I think these must
//		be represented in the type, subtype and format somehow. They
//		are pulled out as separate flags so that people who don't understand
//		the particular format representation can still see them, but
//		they should duplicate information in the format block."
//
// ATTENTION
// For AVI Mux and Video renderers temporal compression flag is very important and
// should be checked separately.
// lSampleSize frequently has bad value (or don't set - WinDVD Audio, for sample).
// We don't check this values. They should be checked separately.

inline bool __stdcall MTIsEqual(IN const AM_MEDIA_TYPE* pmt1, IN const AM_MEDIA_TYPE* pmt2)
{
	if(pmt1 == NULL || pmt2 == NULL)
		return false;
	if(pmt1 == pmt2)
		return true;

	if(!IsEqualGUID(pmt1->majortype, pmt2->majortype))
		return false;
	if(!MTIsEqSubtypes(pmt1->subtype, pmt2->subtype))
		return false;
	if(!IsEqualGUID(pmt1->formattype, pmt2->formattype))
		return false;
	if(pmt1->cbFormat != pmt2->cbFormat)
		return false;

	return (pmt1->cbFormat == 0 || memcmp(pmt1->pbFormat, pmt2->pbFormat, pmt1->cbFormat) == 0);
}

// The comparison for first DWORD of video subtypes, created on base FOURCC_Base
// should be case insensitive.
// For sample: 
//	sub1 = { FCC('abcd'), 0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}}
// and
//	sub2 = { FCC('AbCd'), 0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}}
// is equal
//
// ATTENTION. In some cases (for some codecs) FOURCC is case sensitive

inline bool __stdcall MTIsEqSubtypes(IN REFGUID sub1, IN REFGUID sub2)
{
    if(memcmp(&sub1.Data2, &sub2.Data2, sizeof(GUID) - sizeof(DWORD)) != 0)
		return false;
	if(sub1.Data1 == sub2.Data1)
		return true;
    if(memcmp(&sub1.Data2, &FOURCC_Base.Data2, sizeof(GUID) - sizeof(DWORD)) != 0)
		return false;

	// Subtypes is equal, if:
	// 1. All bytes of first DWORD are printable (in range 0x20 -:- 0x7E)
	// 2. If byte is alpha, then (byte1 | 0x20) == (byte2 | 0x20), else byte1 == byte2
	BYTE* pb1 = (BYTE*)&sub1.Data1;
	BYTE* pb2 = (BYTE*)&sub2.Data1;
	for(int i = 0; i < sizeof(DWORD); ++i, ++pb1, ++pb2)
	{
		if(*pb1 < 0x20 || 0x7E < *pb1 || *pb2 < 0x20 || 0x7E < *pb2)
			return false;

		BYTE c = (BYTE)(*pb1 | 0x20);
		if('a' <= c && c <= 'z')
		{
			if(c != (*pb2 | 0x20))
				return false;
		}
		else if(*pb1 != *pb2)
			return false;
	}
	return true;
}

inline BITMAPINFOHEADER* __stdcall MTBmpHeader(IN const AM_MEDIA_TYPE* pmt)
{
	if(pmt == NULL)
		return NULL;
	if(pmt->majortype != MEDIATYPE_Video)
		return NULL;
	if(pmt->pbFormat == NULL)
		return NULL;

	if(pmt->formattype == FORMAT_VideoInfo)
	{
		ATLASSERT(pmt->cbFormat >= sizeof(VIDEOINFOHEADER));
		if(pmt->cbFormat < sizeof(VIDEOINFOHEADER)) return NULL;

		return &((VIDEOINFOHEADER*)(pmt->pbFormat))->bmiHeader;
	}
	if(pmt->formattype == FORMAT_VideoInfo2)
	{
		ATLASSERT(pmt->cbFormat >= sizeof(VIDEOINFOHEADER2));
		if(pmt->cbFormat < sizeof(VIDEOINFOHEADER2)) return NULL;

		return &((VIDEOINFOHEADER2*)(pmt->pbFormat))->bmiHeader;
	}
	return NULL;
}

inline SIZE __stdcall MTResolution(IN const AM_MEDIA_TYPE* pmt)
{
	SIZE s = {0,0};
	const BITMAPINFOHEADER* pbmi = MTBmpHeader(pmt);
	if(pbmi != NULL)
	{
		s.cx = pbmi->biWidth;
		s.cy = pbmi->biHeight;
	}
	return s;
}

inline REFERENCE_TIME __stdcall MTAverageTime(IN const AM_MEDIA_TYPE* pmt)
{
	ATLASSERT(pmt != NULL);
	if(pmt == NULL) return (REFERENCE_TIME)0;

	ATLASSERT(pmt->majortype == MEDIATYPE_Video);
	if(pmt->majortype != MEDIATYPE_Video) return (REFERENCE_TIME)0;

	ATLASSERT(pmt->formattype == FORMAT_VideoInfo || pmt->formattype == FORMAT_VideoInfo2 ||
		pmt->formattype == FORMAT_MPEGVideo || pmt->formattype == FORMAT_MPEG2Video );
	if(pmt->formattype != FORMAT_VideoInfo && pmt->formattype != FORMAT_VideoInfo2 &&
		pmt->formattype == FORMAT_MPEGVideo && pmt->formattype == FORMAT_MPEG2Video ) return (REFERENCE_TIME)0;

	ATLASSERT(pmt->cbFormat >= sizeof(VIDEOINFOHEADER) && pmt->pbFormat != NULL);
	if(pmt->cbFormat < sizeof(VIDEOINFOHEADER) || pmt->pbFormat == NULL) return (REFERENCE_TIME)0;

	//---------------------------------
	// First 5 members of VIDEOINFOHEADER, VIDEOINFOHEADER2, MPEG1VIDEOINFO and MPEG2VIDEOINFO are the same
	VIDEOINFOHEADER* pvih = (VIDEOINFOHEADER*)pmt->pbFormat;
	return pvih->AvgTimePerFrame;
}

inline HRESULT __stdcall MTSetAverageTime(IN AM_MEDIA_TYPE* pmt, REFERENCE_TIME avgTime )
{
	ATLASSERT(pmt != NULL);
	if(pmt == NULL) return E_POINTER;

	ATLASSERT(pmt->majortype == MEDIATYPE_Video);
	if(pmt->majortype != MEDIATYPE_Video) return VFW_E_INVALIDMEDIATYPE;

	ATLASSERT(pmt->formattype == FORMAT_VideoInfo || pmt->formattype == FORMAT_VideoInfo2 ||
		pmt->formattype == FORMAT_MPEGVideo || pmt->formattype == FORMAT_MPEG2Video );
	if(pmt->formattype != FORMAT_VideoInfo && pmt->formattype != FORMAT_VideoInfo2 &&
		pmt->formattype == FORMAT_MPEGVideo && pmt->formattype == FORMAT_MPEG2Video ) return VFW_E_INVALIDMEDIATYPE;

	ATLASSERT(pmt->cbFormat >= sizeof(VIDEOINFOHEADER) && pmt->pbFormat != NULL);
	if(pmt->cbFormat < sizeof(VIDEOINFOHEADER) || pmt->pbFormat == NULL) return VFW_E_INVALIDMEDIATYPE;

	//---------------------------------
	// First 5 members of VIDEOINFOHEADER, VIDEOINFOHEADER2, MPEG1VIDEOINFO and MPEG2VIDEOINFO are the same
	VIDEOINFOHEADER* pvih = (VIDEOINFOHEADER*)pmt->pbFormat;

	pvih->AvgTimePerFrame = avgTime;
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// CMType

class ATL_NO_VTABLE CMType : public AM_MEDIA_TYPE
{
public:
	explicit CMType(IN REFGUID major = GUID_NULL, IN REFGUID sub = GUID_NULL)
	{
		MTInit(this, major, sub);
	}
	// ATTENTION. pbData can be == NULL if cbData != 0.
	// In this case you should set valid data to allocated pbData buffer after call.
	explicit CMType(IN REFGUID major, IN REFGUID sub, IN REFGUID format,
					IN const ULONG cb = 0, IN const BYTE *pb = NULL)
	{
		MTInit(this, major, sub);
		if(AssignFormat(cb, pb) == S_OK)
			formattype = format;
	}

	// For wideo subtypes dwFCC > MAXWORD
	// For audio subtypes dwFCC <= MAXWORD
	explicit CMType(IN DWORD dwFCC)
	{
		if(dwFCC > MAXWORD)
			MTInit(this, MEDIATYPE_Video, Fourcc2Guid(dwFCC));
		else
			MTInit(this, MEDIATYPE_Audio, Fourcc2Guid(dwFCC));
	}

	CMType(IN const AM_MEDIA_TYPE* pmt)
	{
		MTInit(this);
		if(pmt != NULL)
		{
			HRESULT hr = Set(pmt);
			if(hr != S_OK)
				ATLASSERT(!_T("CMType(const AM_MEDIA_TYPE* pmt) FAILED"));
		}
	}
    CMType(IN const CMType& mt)
	{
		MTInit(this);
		HRESULT hr = Set(mt);
		if(hr != S_OK)
			ATLASSERT(!_T("CMType(const CMType& mt) FAILED"));
	}

    ~CMType()
	{
		::CoTaskMemFree(pbFormat);
		if(pUnk != NULL)
			pUnk->Release();
	}

	//---------------------------------
	// Operators =
    CMType& operator=(IN const AM_MEDIA_TYPE* pmt)
	{
		if(pmt == NULL)
			Reset();
		else
		{
			HRESULT hr = Set(pmt);
			if(hr != S_OK)
			{
				ATLASSERT(!_T("CMType& operator=(const AM_MEDIA_TYPE* pmt) FAILED"));
				Reset();
			}
		}
		return *this;
	}

    CMType& operator=(IN const CMType& mt) { return *this = &mt; }

	//---------------------------------
	// Functions for set/get/reset
    HRESULT Set(IN const CMType& mt)
	{
		return Set(&mt);
	}
 	HRESULT Set(IN const AM_MEDIA_TYPE* pmt);
    HRESULT Set(IN REFGUID rMajortype, IN REFGUID rSubtype = GUID_NULL,
		IN REFGUID rFormattype = GUID_NULL, IN const ULONG cbData = 0, IN const BYTE* pbData = NULL);

	// Video types
	// ATTENTION. For video types dwFCC should be more MAXWORD (if value is <= MAXWORD, then it's audio)
	void SetVideo(IN DWORD dwFCC)
	{
		MTClear(this, MEDIATYPE_Video, Fourcc2Guid(dwFCC));
	}
	// Audio types
	void SetAudio(IN WORD wFormatTag)
	{
		MTClear(this, MEDIATYPE_Audio, Fourcc2Guid((DWORD)wFormatTag));
	}

    HRESULT Get(IN OUT CMType& mt) const
	{
		MTClear(&mt);
		return MTCopy(&mt, this);
	}
	void Reset()
	{
		MTClear(this);
	}

	HRESULT AssignFormat(IN const ULONG cb = 0, IN const BYTE* pb = NULL);
	HRESULT ReallocFormat(IN const ULONG cb);

	//---------------------------------
    HRESULT CopyToStruct(IN OUT AM_MEDIA_TYPE* pmt) const
	{
		// ATTENTION. Old contents of pmt (pbFormat, pUnk) aren't cleared.
		return MTCopy(pmt, this);
	}
	HRESULT Duplicate(OUT AM_MEDIA_TYPE** ppmt) const
	{
		ATLASSERT(ppmt != NULL);
		if(ppmt == NULL) return E_POINTER;
		*ppmt = MTDup(this);
		return (*ppmt != NULL)? S_OK : E_FAIL;
	}

	//---------------------------------
	// Testing/comparing

	bool IsValid() const
	{
		return !IsEqualGUID(majortype, GUID_NULL);
	}
	bool IsDefined() const
	{
		return MTIsDefined(this);
	}
	bool IsMatched(REFGUID major = GUID_NULL, REFGUID sub = GUID_NULL, REFGUID format = GUID_NULL) const
	{
		return MTIsMatched(this, major, sub, format);
	}

	bool IsAudio() const	{ return IsEqualGUID(majortype, MEDIATYPE_Audio) == TRUE; }
	bool IsVideo() const	{ return IsEqualGUID(majortype, MEDIATYPE_Video) == TRUE; }

	bool operator != (const CMType& mt) const			{ return !MTIsEqual(this, &mt); }
	bool operator != (const AM_MEDIA_TYPE* pmt) const	{ return !MTIsEqual(this, pmt); }

	bool operator == (const CMType& mt) const			{ return MTIsEqual(this, &mt); }
	bool operator == (const AM_MEDIA_TYPE* pmt) const	{ return MTIsEqual(this, pmt); }

	//---------------------------------
	// Partial comparisions

	// Is this media type subset of mt/pmt ?
	bool IsSubset(const CMType& mt) const			{ return MTIsSubset(&mt, this); }
	bool IsSubset(const AM_MEDIA_TYPE* pmt) const	{ return MTIsSubset(pmt, this); }

	// Is this media type superset for mt/pmt ?
	bool IsSuperset(const CMType& mt) const			{ return MTIsSubset(this, &mt); }
	bool IsSuperset(const AM_MEDIA_TYPE* pmt) const { return MTIsSubset(this, pmt); }

	bool IsComparable(const CMType& mt) const
	{
		return IsComparable(&mt);
	}
	bool IsComparable(const AM_MEDIA_TYPE* pmt) const
	{
		if(pmt == NULL)
			return false;
		return ( majortype == pmt->majortype &&
				 MTIsEqSubtypes(subtype, pmt->subtype) &&
				 formattype == pmt->formattype);
	}

	//---------------------------------
	// Type conversions
	operator VIDEOINFOHEADER*()
	{
		if(!IsEqualGUID(formattype, FORMAT_VideoInfo) || pbFormat == NULL)
			return NULL;
		ATLASSERT(cbFormat >= sizeof(VIDEOINFOHEADER));
		return (VIDEOINFOHEADER*)pbFormat;
	}
	operator const VIDEOINFOHEADER*() const
	{
		if(!IsEqualGUID(formattype, FORMAT_VideoInfo) || pbFormat == NULL)
			return NULL;
		ATLASSERT(cbFormat >= sizeof(VIDEOINFOHEADER));
		return (VIDEOINFOHEADER*)pbFormat;
	}
	operator VIDEOINFOHEADER2*()
	{
		if(!IsEqualGUID(formattype, FORMAT_VideoInfo2) || pbFormat == NULL)
			return NULL;
		ATLASSERT(cbFormat >= sizeof(VIDEOINFOHEADER2));
		return (VIDEOINFOHEADER2*)pbFormat;
	}
	operator const VIDEOINFOHEADER2*() const
	{
		if(!IsEqualGUID(formattype, FORMAT_VideoInfo2) || pbFormat == NULL)
			return NULL;
		ATLASSERT(cbFormat >= sizeof(VIDEOINFOHEADER2));
		return (VIDEOINFOHEADER2*)pbFormat;
	}
	operator BITMAPINFOHEADER*()
	{
		if(pbFormat != NULL)
		{
			if(IsEqualGUID(formattype, FORMAT_VideoInfo) || IsEqualGUID(formattype, FORMAT_MPEGVideo))
				return &((VIDEOINFOHEADER*)pbFormat)->bmiHeader;
			if(IsEqualGUID(formattype, FORMAT_VideoInfo2) || IsEqualGUID(formattype, FORMAT_MPEG2Video))
				return &((VIDEOINFOHEADER2*)pbFormat)->bmiHeader;
		}
		return NULL;
	}
	operator const BITMAPINFOHEADER*() const
	{
		if(pbFormat != NULL)
		{
			if(IsEqualGUID(formattype, FORMAT_VideoInfo) || IsEqualGUID(formattype, FORMAT_MPEGVideo))
				return &((VIDEOINFOHEADER*)pbFormat)->bmiHeader;
			if(IsEqualGUID(formattype, FORMAT_VideoInfo2) || IsEqualGUID(formattype, FORMAT_MPEG2Video))
				return &((VIDEOINFOHEADER2*)pbFormat)->bmiHeader;
		}
		return NULL;
	}
	operator WAVEFORMATEX*()
	{
		if(!IsEqualGUID(formattype, FORMAT_WaveFormatEx) || pbFormat == NULL)
			return NULL;
		ATLASSERT(cbFormat >= sizeof(WAVEFORMATEX));
		return (WAVEFORMATEX*)pbFormat;
	}
	operator const WAVEFORMATEX*() const
	{
		if(!IsEqualGUID(formattype, FORMAT_WaveFormatEx) || pbFormat == NULL)
			return NULL;
		ATLASSERT(cbFormat >= sizeof(WAVEFORMATEX));
		return (WAVEFORMATEX*)pbFormat;
	}
};

inline HRESULT CMType::Set(IN const AM_MEDIA_TYPE* pmt)
{
	if(pmt == NULL)
	{
		Reset();
		return S_OK;
	}

	if(this == pmt)
		return S_OK;

	//---------------------------------
	BYTE* pb = NULL;
	HRESULT hr = MTDupFormat(pmt, &pb);
	if(SUCCEEDED(hr))
	{
		::CoTaskMemFree(pbFormat);
		if(pUnk != NULL)
			pUnk->Release();

		CopyMemory(this, pmt, sizeof(AM_MEDIA_TYPE));
		pbFormat = pb;
		if(pUnk != NULL)
			pUnk->AddRef();
	}
	return hr;
}

inline HRESULT CMType::Set(IN REFGUID major, IN REFGUID sub /*= GUID_NULL*/, IN REFGUID format /*= GUID_NULL*/,
						   IN const ULONG cb /*= 0*/, IN const BYTE* pb /*= NULL*/)
{
	BYTE* pbCopy = NULL;
	if(cb != 0)
	{
		pbCopy = MTAllocFormat(cb, pb);
		if(pbCopy == NULL)
			return E_OUTOFMEMORY;
	}

	MTClear(this, major, sub);
	formattype = format;
	cbFormat = cb;
	pbFormat = pbCopy;
	return S_OK;
}

inline HRESULT CMType::AssignFormat(IN const ULONG cb /*= 0*/, IN const BYTE* pb /*= NULL*/)
{
	ATLASSERT(cb != 0 || pb == NULL);
	if(cb == 0 && pb != NULL) return E_INVALIDARG;

	ATLASSERT((cbFormat == 0) == (pbFormat == NULL));

	BYTE* pbCopy = NULL;
	if(cb != 0)
	{
		pbCopy = MTAllocFormat(cb, pb);
		if(pbCopy == NULL)
			return E_OUTOFMEMORY;
	}

	::CoTaskMemFree(pbFormat);

	cbFormat = cb;
	pbFormat = pbCopy;
	return S_OK;
}

inline HRESULT CMType::ReallocFormat(IN const ULONG cb)
{
	ATLASSERT((cbFormat == 0) == (pbFormat == NULL));

	if(cb != cbFormat)
	{
		BYTE* pb = (BYTE*)::CoTaskMemRealloc((void*)pbFormat, cb);
		if(cb > 0 && pb == NULL)
			return E_OUTOFMEMORY;

		pbFormat = pb;
		cbFormat = cb;
	}
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// CMTArray

#ifndef MAX_ENUM_MEDIATYPES_OUT_OF_SYNC
	#define MAX_ENUM_MEDIATYPES_OUT_OF_SYNC		16
#endif

class CMTArray : public CSimpleArray2<CMType>
{
public:
	CMTArray() {}
	~CMTArray() {}

	//---------------------------------
	HRESULT GetFromPin(IPin* pPin)
	{
		RemoveAll();
		return AddFromPin(pPin);
	}

	HRESULT AddFromPin(IPin* pPin)
	{
		ATLASSERT(pPin != NULL);
		if(pPin == NULL) return E_INVALIDARG;

		CComPtr<IEnumMediaTypes> spEnumMT;
		HRESULT hr = SEH_IPin::EnumMediaTypes(pPin, &spEnumMT);
		if(hr != S_OK)
		{
			return (hr == S_FALSE || FAILED(hr))? hr : E_FAIL;
		}

		CMTArray arrMT;
		for(int i = 0; i < MAX_ENUM_MEDIATYPES_OUT_OF_SYNC; ++i)
		{
			hr = arrMT.Collect(spEnumMT);
			if(hr != VFW_E_ENUM_OUT_OF_SYNC)
				break;
		}
		if(hr != S_OK)
			return FAILED(hr)? hr : E_FAIL;

		if(arrMT.GetSize() != 0)
		{
			if(!Append(arrMT))
				hr = E_OUTOFMEMORY;
		}
		else
			hr = S_FALSE;
		return hr;
	}

	HRESULT Collect(IEnumMediaTypes* pEnumMT)
	{
		ATLASSERT(pEnumMT != NULL);
		if(pEnumMT == NULL) return E_INVALIDARG;

		RemoveAll();

		HRESULT hr = SEH_IEnumMediaTypes::Reset(pEnumMT);
		if(FAILED(hr))
			return hr;

		ULONG cFetched = 0;
		while(hr == S_OK)
		{
			AM_MEDIA_TYPE* pmt = NULL;
			hr = SEH_IEnumMediaTypes::Next(pEnumMT, 1, &pmt, &cFetched);
			if(hr == S_OK)
			{
				ATLASSERT(cFetched == 1);

				if(pmt == NULL)
					hr = E_FAIL;
				else
				{
					hr = Attach(pmt);
					if(hr != S_OK)
					{
						::MTFree(pmt);
					}
				}
			}
		}
		if(hr != S_FALSE)
			RemoveAll();
		else
			hr = S_OK;
		return hr;
	}

	int Find(const AM_MEDIA_TYPE* pmt)
	{
		for(int i = 0; i < m_nSize; ++i)
		{
			if(m_aT[i] == pmt) return i;
		}
		return -1;
	}

	//---------------------------------

protected:
	
	// Additional operations, specific only for this container

	// If return TRUE:
	//	Last element = *pmt (pmt->pUnk and pmt->pbFormat moved to this element),
	//	pmt freed (called CoTaskMemFree(pmt))
	HRESULT Attach(AM_MEDIA_TYPE* pmt)
	{
		ATLASSERT(pmt != NULL);
		if(pmt == NULL) return E_INVALIDARG;

		if(m_nSize == m_nAllocSize)
		{
			CMType* aT;
			int nNewAllocSize = (m_nAllocSize == 0) ? 1 : (m_nSize * 2);
			aT = (CMType*)realloc(m_aT, nNewAllocSize * sizeof(CMType));
			if(aT == NULL)
				return E_OUTOFMEMORY;

			m_nAllocSize = nNewAllocSize;
			m_aT = aT;
		}
		++m_nSize;

		// Move all data (with pmt->pUnk and pmt->pbFormat) to last element
		ATLASSERT(sizeof(AM_MEDIA_TYPE) == sizeof(CMType));
		memcpy(&m_aT[m_nSize - 1], pmt, sizeof(AM_MEDIA_TYPE));

		// Free only input pointer (don't free pmt->pUnk and pmt->pbFormat !)
		::CoTaskMemFree(pmt);
		return S_OK;
	}

	// Moves all contents of arrMT to current container and sets zero size for arrMT
	BOOL Append(CSimpleArray2<CMType>& arrMT)
	{
		if(arrMT.m_nSize > 0)
		{
			if(m_nSize + arrMT.m_nSize >= m_nAllocSize)
			{
				CMType* aT;
				int nNewAllocSize = (m_nAllocSize == 0) ? arrMT.m_nSize : 2 * (m_nSize + arrMT.m_nSize);
				aT = (CMType*)realloc(m_aT, nNewAllocSize * sizeof(CMType));
				if(aT == NULL)
					return FALSE;

				m_nAllocSize = nNewAllocSize;
				m_aT = aT;
			}

			memcpy(&m_aT[m_nSize], arrMT.m_aT, arrMT.m_nSize * sizeof(CMType));
			m_nSize += arrMT.m_nSize;
			arrMT.m_nSize = 0;
		}
		return TRUE;
	}
};

/////////////////////////////////////////////////////////////////////////////
// 48000, 44100, 32000, 24000, 22050, 16000, 11025, 8000
typedef enum tagWaveType
{
	WT_48000_S_16	= 0,
	WT_48000_M_16,
	WT_48000_S_8,
	WT_48000_M_8,

	WT_44100_S_16,
	WT_44100_M_16,
	WT_44100_S_8,
	WT_44100_M_8,

	WT_32000_S_16,
	WT_32000_M_16,
	WT_32000_S_8,
	WT_32000_M_8,

	WT_24000_S_16,
	WT_24000_M_16,
	WT_24000_S_8,
	WT_24000_M_8,

	WT_22050_S_16,
	WT_22050_M_16,
	WT_22050_S_8,
	WT_22050_M_8,

	WT_16000_S_16,
	WT_16000_M_16,
	WT_16000_S_8,
	WT_16000_M_8,

	WT_11025_S_16,
	WT_11025_M_16,
	WT_11025_S_8,
	WT_11025_M_8,

	WT_8000_S_16,
	WT_8000_M_16,
	WT_8000_S_8,
	WT_8000_M_8
} WAVETYPE;

//---------------------------------
// Flags of difference for compared video mediatypes
// (see CMTHelp::MTCompareVideo(... DWORD* pdwDifferenceFlags))
#define	VCF_FORMAT_TYPE		0x00000001		// AM_MEDIA_TYPE::formattype are different
											// (accepted only FORMAT_VideoInfo and FORMAT_VideoInfo2)
#define	VCF_FORMAT_SIZE		0x00000002		// AM_MEDIA_TYPE::cbFormat are different
											// (additional format blocks are different)
#define	VCF_SOURCE_RECT		0x00000010		// VIDEOINFOHEADER::rcSource are different
#define	VCF_TARGET_RECT		0x00000020		// VIDEOINFOHEADER::rcTarget are different
#define	VCF_AVG_TIME		0x00000100		// VIDEOINFOHEADER::AvgTimePerFrame are different
#define	VCF_ASPECT_X		0x00001000		// VIDEOINFOHEADER2::dwPictAspectRatioX are different
#define	VCF_ASPECT_Y		0x00002000		// VIDEOINFOHEADER2::dwPictAspectRatioY are different
#define	VCF_INTERLACE		0x00004000		// VIDEOINFOHEADER2::dwInterlaceFlags are different
#define	VCF_BMP_WIDTH		0x00010000		// VIDEOINFOHEADER::bmiHeader.biWidth are different
#define	VCF_BMP_HEIGHT		0x00020000		// VIDEOINFOHEADER::bmiHeader.biHeight are different
#define	VCF_BMP_SIZE		0x00040000		// VIDEOINFOHEADER::bmiHeader.biSizeImage are different
#define	VCF_BMP_MASKS		0x00080000		// Color masks are different (valid only for BI_BITFIELDS)

//---------------------------------
class CMTHelp
{
public:

	//=================================================================
	// Functions for parse and check of media type and its parts
	static HRESULT MTParseVideo(const AM_MEDIA_TYPE* pmt, FOURCC* pfccType = NULL,
								DWORD* pnWidth = NULL, DWORD* pnHeight = NULL, DWORD* pcbSize = NULL)
	{
		if(pfccType != NULL)
			*pfccType = FOURCC_UNK;
		if(pnWidth != NULL)
			*pnWidth = 0;
		if(pnHeight != NULL)
			*pnHeight = 0;
		if(pcbSize != NULL)
			*pcbSize = 0;

		HRESULT hr = MTTestVideo(pmt);
		if(hr != S_OK)
			return hr;

		//---------------------------------
		BITMAPINFOHEADER* pbmiH = NULL;
		if( IsEqualGUID(pmt->formattype, FORMAT_VideoInfo) ||
			IsEqualGUID(pmt->formattype, FORMAT_MPEGVideo))
			pbmiH = &(((VIDEOINFOHEADER*)pmt->pbFormat)->bmiHeader);
		else
			pbmiH = &(((VIDEOINFOHEADER2*)pmt->pbFormat)->bmiHeader);

		//---------------------------------
		if(pfccType != NULL)
			*pfccType = GUID2FOURCC(pmt->subtype);
		if(pnWidth != NULL)
			*pnWidth = (DWORD)pbmiH->biWidth;
		if(pnHeight != NULL)
			*pnHeight = (DWORD)pbmiH->biHeight;
		if(pcbSize != NULL)
			*pcbSize = pbmiH->biSizeImage;
		return S_OK;
	}

	static HRESULT MTParseVideo(const AM_MEDIA_TYPE* pmt, PIC_PROPS* pPicProps )
	{
		if( !pPicProps ) return E_POINTER;

		::ZeroMemory( pPicProps, sizeof(PIC_PROPS) );

		HRESULT hr = MTTestVideo(pmt);
		if(hr != S_OK)
			return hr;

		pPicProps->fccType = GUID2FOURCC(pmt->subtype);

		BITMAPINFOHEADER* pbmiH = NULL;
		if( IsEqualGUID(pmt->formattype, FORMAT_VideoInfo) ||
			IsEqualGUID(pmt->formattype, FORMAT_MPEGVideo))
		{
			VIDEOINFOHEADER* pHeader = (VIDEOINFOHEADER*)pmt->pbFormat;
			pPicProps->nWidth = pHeader->rcTarget.right - pHeader->rcTarget.left;
			pPicProps->nRowWidth = pHeader->bmiHeader.biWidth;
			pPicProps->nHeight = pHeader->bmiHeader.biHeight;
		}
		else
		{
			VIDEOINFOHEADER2* pHeader = (VIDEOINFOHEADER2*)pmt->pbFormat;
			pPicProps->nWidth = pHeader->rcTarget.right - pHeader->rcTarget.left;
			pPicProps->nRowWidth = pHeader->bmiHeader.biWidth;
			pPicProps->nHeight = pHeader->bmiHeader.biHeight;
		}

		if( pPicProps->nWidth == 0 ) pPicProps->nWidth = pPicProps->nRowWidth;

		return S_OK;
	}

	static HRESULT MTParseVideoRect(const AM_MEDIA_TYPE* pmt, RECT* pSrc, RECT* pDst)
	{
		ATLASSERT(pSrc != NULL);
		if(pSrc == NULL) return E_POINTER;
		ATLASSERT(pDst != NULL);
		if(pDst == NULL) return E_POINTER;

		HRESULT hr = MTTestVideo(pmt);
		if(hr != S_OK)
			return hr;

		//---------------------------------
		if( IsEqualGUID(pmt->formattype, FORMAT_VideoInfo) ||
			IsEqualGUID(pmt->formattype, FORMAT_MPEGVideo))
		{
			VIDEOINFOHEADER* pVIH = (VIDEOINFOHEADER*)pmt->pbFormat;
			memcpy(pSrc, &pVIH->rcSource, sizeof(RECT));
			memcpy(pDst, &pVIH->rcTarget, sizeof(RECT));
		}
		else
		{
			VIDEOINFOHEADER2* pVIH2 = (VIDEOINFOHEADER2*)pmt->pbFormat;
			memcpy(pSrc, &pVIH2->rcSource, sizeof(RECT));
			memcpy(pDst, &pVIH2->rcTarget, sizeof(RECT));
		}
		return S_OK;
	}

	static HRESULT MTAspectRatio(const AM_MEDIA_TYPE* pmt, DWORD* pdwAspectRatioX, DWORD* pdwAspectRatioY)
	{
		if(pdwAspectRatioX != NULL)
			*pdwAspectRatioX = 0;
		if(pdwAspectRatioY != NULL)
			*pdwAspectRatioY = 0;

		HRESULT hr = MTTestVideo(pmt, true);
		if(hr != S_OK)
			return hr;

		//---------------------------------
		VIDEOINFOHEADER2* pviH = (VIDEOINFOHEADER2*)pmt->pbFormat;
		if(pdwAspectRatioX != NULL)
			*pdwAspectRatioX = pviH->dwPictAspectRatioX;
		if(pdwAspectRatioY != NULL)
			*pdwAspectRatioY = pviH->dwPictAspectRatioY;
		return S_OK;
	}

	//=================================================================
	// Functions for testing media types

	// We don't check subtype GUID, with the exception of subtype != GUID_NULL
	static HRESULT MTTestVideo(const AM_MEDIA_TYPE* pmt, bool bVideoInfo2Only = false)
	{
		ATLASSERT(pmt != NULL);
		if(pmt == NULL) return E_POINTER;

		if(pmt->majortype != MEDIATYPE_Video)
			return VFW_E_INVALIDMEDIATYPE;

		if(pmt->subtype == GUID_NULL)
			return VFW_E_INVALIDSUBTYPE;

		if(pmt->cbFormat == 0 || pmt->pbFormat == NULL)
			return VFW_E_INVALIDMEDIATYPE;

		if(pmt->formattype == FORMAT_VideoInfo2)
		{
			return (pmt->cbFormat >= sizeof(VIDEOINFOHEADER2))? S_OK : VFW_E_INVALIDMEDIATYPE;
		}
		if(pmt->formattype == FORMAT_MPEG2Video)
		{
			return (pmt->cbFormat >= sizeof(MPEG2VIDEOINFO))? S_OK : VFW_E_INVALIDMEDIATYPE;
		}

		if(!bVideoInfo2Only)
		{
			if(pmt->formattype == FORMAT_VideoInfo)
			{
				return (pmt->cbFormat >= sizeof(VIDEOINFOHEADER))? S_OK : VFW_E_INVALIDMEDIATYPE;
			}
			if(pmt->formattype == FORMAT_MPEGVideo)
			{
				return (pmt->cbFormat >= sizeof(MPEG1VIDEOINFO))? S_OK : VFW_E_INVALIDMEDIATYPE;
			}
		}
		return VFW_E_INVALIDMEDIATYPE;
	}

	static HRESULT MTTestYUV(const AM_MEDIA_TYPE* pmt, bool bVideoInfo2Only = false)
	{
		HRESULT hr = MTTestVideo(pmt, bVideoInfo2Only);
		if(hr == S_OK)
		{
			// We can use only known YUV subtypes
			int nFCC = CPictureFormats::Find(pmt->subtype);
			if(nFCC == -1)
				hr = VFW_E_INVALIDSUBTYPE;
			else if(!CPictureFormats::s_arrSupportedFormats[nFCC].bYUVFormat)
				hr = S_FALSE;
		}
		return hr;
	}

	static HRESULT MTCheckVideoFormat(const AM_MEDIA_TYPE* pmt)
	{
		ATLASSERT(pmt != NULL);
		if(pmt == NULL) return E_POINTER;

		if(pmt->majortype != MEDIATYPE_Video)
			return VFW_E_INVALIDMEDIATYPE;

		if(pmt->subtype == GUID_NULL)
			return VFW_E_INVALIDSUBTYPE;

		if(pmt->pbFormat == NULL)
			return VFW_E_INVALIDMEDIATYPE;

		//---------------------------------
		DWORD cb = 0;
		if(pmt->formattype == FORMAT_VideoInfo)
			cb = sizeof(VIDEOINFOHEADER);
		else if(pmt->formattype == FORMAT_VideoInfo2)
			cb = sizeof(VIDEOINFOHEADER2);
		else
			return VFW_E_INVALIDMEDIATYPE;

		return (pmt->cbFormat >= cb)? S_OK : VFW_E_INVALIDMEDIATYPE;
	}

	// pdwDifferenceFlags: see VCF_... definitions.
	// Returns:
	//	S_OK: media types are equal, *pdwDifferenceFlags = 0
	//	S_FALSE: some minor properties is different.
	//			 In this case *pdwDifferenceFlags contains VCF_... flags
	// FAILED(hr): media types are different
	static HRESULT MTCompareVideo(const AM_MEDIA_TYPE* pmt1, const AM_MEDIA_TYPE* pmt2,
		DWORD* pdwDifferenceFlags = NULL)
	{
		if(pdwDifferenceFlags != NULL)
			*pdwDifferenceFlags = 0;

		HRESULT hr = MTCheckVideoFormat(pmt1);
		if(hr != S_OK)
			return hr;

		hr = MTCheckVideoFormat(pmt2);
		if(hr != S_OK)
			return hr;

		//---------------------------------
		// TODO: subtypes can be different
		if(pmt1->subtype != pmt2->subtype)
			return E_FAIL;

		//---------------------------------
		DWORD dwFlags = 0;

		if(pmt1->formattype != pmt2->formattype)
			dwFlags |= VCF_FORMAT_TYPE;

		if(pmt1->cbFormat != pmt2->cbFormat)
			dwFlags |= VCF_FORMAT_SIZE;

		// We don't compare next fields of AM_MEDIA_TYPE:
		//	BOOL bFixedSizeSamples;
		//	BOOL bTemporalCompression;
		//	ULONG lSampleSize;
		// Different manufacturers sets different values for this fields.

		//---------------------------------
		// First 5 memebers of VIDEOINFOHEADER and VIDEOINFOHEADER2 structs is the same
		const VIDEOINFOHEADER* pvih1 = (const VIDEOINFOHEADER*)pmt1->pbFormat;
		const VIDEOINFOHEADER* pvih2 = (const VIDEOINFOHEADER*)pmt2->pbFormat;

		const DWORD* pdwS1 = (const DWORD*)&pvih1->rcSource;
		const DWORD* pdwT1 = (const DWORD*)&pvih1->rcTarget;
		const DWORD* pdwS2 = (const DWORD*)&pvih2->rcSource;
		const DWORD* pdwT2 = (const DWORD*)&pvih2->rcTarget;
		for(int i = 0; i < 4; ++i, ++pdwS1, ++pdwS2, ++pdwT1, ++pdwT2)
		{
			//	if(rcS1.left != 0 && rcS2.left != 0 && rcS1.left != rcS2.left)
			//		return false;

			if(*pdwS1 != 0 && *pdwS2 != 0 && *pdwS1 != *pdwS2)
				dwFlags |= VCF_SOURCE_RECT;
			if(*pdwT1 != 0 && *pdwT2 != 0 && *pdwT1 != *pdwT2)
				dwFlags |= VCF_TARGET_RECT;
		}

		// We ignore next fields of VIDEOINFOHEADER/VIDEOINFOHEADER2 structs
		//	DWORD dwBitRate;
		//	DWORD dwBitErrorRate;

		if(pvih1->AvgTimePerFrame != pvih2->AvgTimePerFrame)
			dwFlags |= VCF_AVG_TIME;

		//---------------------------------
//#pragma vcout("TODO: Should be extended for different formattypes")
		if(pmt1->formattype == FORMAT_VideoInfo2 && pmt2->formattype == FORMAT_VideoInfo2)
		{
			const VIDEOINFOHEADER2* pvih1 = (VIDEOINFOHEADER2*)pmt1->pbFormat;
			const VIDEOINFOHEADER2* pvih2 = (VIDEOINFOHEADER2*)pmt2->pbFormat;

			if(pvih1->dwPictAspectRatioX != pvih2->dwPictAspectRatioX)
				dwFlags |= VCF_ASPECT_X;
			if(pvih1->dwPictAspectRatioY != pvih2->dwPictAspectRatioY)
				dwFlags |= VCF_ASPECT_Y;
			if(pvih1->dwInterlaceFlags != pvih2->dwInterlaceFlags)
				dwFlags |= VCF_INTERLACE;

			// We don't compare next fields of VIDEOINFOHEADER2:
			//	DWORD dwCopyProtectFlags;
			//	DWORD dwControlFlags
			// Different manufacturers sets different values for this fields.
		}

		//---------------------------------
		const BITMAPINFOHEADER* pbih1 = NULL;
		const BITMAPINFOHEADER* pbih2 = NULL;

		if(pmt1->formattype == FORMAT_VideoInfo)
			pbih1 = &((const VIDEOINFOHEADER*)pmt1->pbFormat)->bmiHeader;
		else
			pbih1 = &((const VIDEOINFOHEADER2*)pmt1->pbFormat)->bmiHeader;

		if(pmt2->formattype == FORMAT_VideoInfo)
			pbih2 = &((const VIDEOINFOHEADER*)pmt2->pbFormat)->bmiHeader;
		else
			pbih2 = &((const VIDEOINFOHEADER2*)pmt2->pbFormat)->bmiHeader;

		if(pbih1->biSize != pbih2->biSize)
			return E_FAIL;
		if(pbih1->biPlanes != pbih2->biPlanes)
			return E_FAIL;
		if(pbih1->biBitCount != pbih2->biBitCount)
			return E_FAIL;
		if(pbih1->biCompression != pbih2->biCompression)
			return E_FAIL;

		// We ignore next fields of VIDEOINFOHEADER/VIDEOINFOHEADER2 structs
		//	LONG       biXPelsPerMeter;
		//	LONG       biYPelsPerMeter;

		// NOTE. We compare only size of color tables. We don't check data in tables.
		if(pbih1->biClrUsed != pbih2->biClrUsed)
			return E_FAIL;
		if(pbih1->biClrImportant != pbih2->biClrImportant)
			return E_FAIL;

		if(pbih1->biHeight != pbih2->biHeight)
		{
			FOURCC fcc = pbih1->biCompression;
			if( (fcc == FOURCC_YUY2 || fcc == FOURCC_UYVY || fcc == FOURCC_YVYU ||
				 fcc == FOURCC_YV12 || fcc == FOURCC_I420 || fcc == FOURCC_YVU9 ||
				 fcc == FOURCC_IYU2) &&
				pbih1->biHeight == -pbih2->biHeight)
			{
				// Don't set flag: for YUV mediatypes sign isn't meaningful
			}
			else
				dwFlags |= VCF_BMP_HEIGHT;
		}

		if(pbih1->biWidth != pbih2->biWidth)
		{
			dwFlags |= VCF_BMP_WIDTH;
		}
		if(pbih1->biSizeImage != pbih2->biSizeImage)
		{
			dwFlags |= VCF_BMP_SIZE;
		}

		if(pbih1->biCompression == BI_BITFIELDS)
		{
			// MSDN: "BI_BITFIELDS: Specifies that the bitmap is not compressed and
			// that the color table consists of three DWORD color masks that specify
			// the red, green, and blue components, respectively, of each pixel.
			// This is valid when used with 16- and 32-bpp bitmaps"
			ATLASSERT(pbih1->biBitCount == 16 || pbih1->biBitCount == 32);

			DWORD cb = 0;
			if(pmt1->formattype == FORMAT_VideoInfo)
				cb = sizeof(VIDEOINFOHEADER);
			else
				cb = sizeof(VIDEOINFOHEADER2);

			cb += 3 * sizeof(DWORD);

			if(pmt1->cbFormat < cb || pmt2->cbFormat < cb)
				return E_FAIL;

			DWORD* pMask1 = (DWORD*)(pbih1 + 1);
			DWORD* pMask2 = (DWORD*)(pbih2 + 1);
			if(pMask1[0] != pMask2[0] || pMask1[1] != pMask2[1] || pMask1[2] != pMask2[2])
				dwFlags |= VCF_BMP_MASKS;
		}

		if(pdwDifferenceFlags != NULL)
			*pdwDifferenceFlags = dwFlags;

		return (dwFlags == 0)? S_OK : S_FALSE;
	}

	static HRESULT MTTestAudio(const AM_MEDIA_TYPE* pmt, WORD wFormatTag = 0xFFFF)
	{
		ATLASSERT(pmt != NULL);
		if(pmt == NULL) return E_POINTER;

		if(!IsEqualGUID(pmt->majortype, MEDIATYPE_Audio))
			return VFW_E_INVALIDMEDIATYPE;
		if(IsEqualGUID(pmt->subtype, GUID_NULL))
			return VFW_E_INVALIDSUBTYPE;

		if( !IsEqualGUID(pmt->formattype, FORMAT_WaveFormatEx) ||
			pmt->pbFormat == NULL || pmt->cbFormat == 0 )
			return VFW_E_INVALIDMEDIATYPE;

		//---------------------------------
		WAVEFORMATEX* pWF = (WAVEFORMATEX*)pmt->pbFormat;

		if(IsEqualGUID(pmt->subtype, MEDIASUBTYPE_PCM))
		{
			if(wFormatTag != 0xFFFF && wFormatTag != WAVE_FORMAT_PCM && wFormatTag != WAVE_FORMAT_EXTENSIBLE)
				return VFW_E_INVALIDMEDIATYPE;

			HRESULT hr = MTTestWaveFormatPCM(pWF);
			if(hr != S_OK)
				return hr;
		}

		// Added by VVB
		if( pmt->subtype == MEDIASUBTYPE_DOLBY_AC3_SPDIF ||
			pmt->subtype == MEDIASUBTYPE_RAW_SPORT ||
			pmt->subtype == MEDIASUBTYPE_SPDIF_TAG_241h ||
			pmt->subtype == MEDIASUBTYPE_IEEE_FLOAT )
		{
			return VFW_E_INVALIDSUBTYPE;
		}

		if(pWF->wFormatTag == wFormatTag)
		{
			// Don't check format data: this code can be extended
			return S_OK;
		}

		return (wFormatTag == 0xFFFF)? S_OK : VFW_E_INVALIDSUBTYPE;
	}

	static HRESULT MTTestWaveFormatPCM(WAVEFORMATEX* pWF)
	{
		ATLASSERT(pWF != NULL);
		if(pWF == NULL) return E_POINTER;

		if(pWF->wFormatTag != WAVE_FORMAT_PCM && pWF->wFormatTag != WAVE_FORMAT_EXTENSIBLE)
			return VFW_E_INVALIDMEDIATYPE;

		ATLASSERT(pWF->nChannels != 0);
		ATLASSERT(pWF->nSamplesPerSec != 0);
		ATLASSERT(pWF->nAvgBytesPerSec != 0);
		ATLASSERT(pWF->nBlockAlign != 0);
		ATLASSERT(pWF->wBitsPerSample != 0);
		if( pWF->nChannels == 0 ||
			pWF->nSamplesPerSec == 0 ||
			pWF->nAvgBytesPerSec == 0 ||
			pWF->nBlockAlign == 0 ||
			pWF->wBitsPerSample == 0)
			return VFW_E_INVALIDMEDIATYPE;

		ATLASSERT((pWF->nChannels * pWF->wBitsPerSample)/8 <= pWF->nBlockAlign);
		if((pWF->nChannels * pWF->wBitsPerSample)/8 > pWF->nBlockAlign)
			return VFW_E_INVALIDMEDIATYPE;

		ATLASSERT(pWF->nSamplesPerSec * pWF->nBlockAlign == pWF->nAvgBytesPerSec);
		if(pWF->nSamplesPerSec * pWF->nBlockAlign != pWF->nAvgBytesPerSec)
			return VFW_E_INVALIDMEDIATYPE;

		return S_OK;
	}

	static HRESULT MTTestDV(const AM_MEDIA_TYPE* pmt, bool bPureDVOnly = true)
	{
		ATLASSERT(pmt != NULL);
		if(pmt == NULL) return E_POINTER;

		if(pmt->majortype != MEDIATYPE_Interleaved && (bPureDVOnly || pmt->majortype != MEDIATYPE_Video))
			return VFW_E_INVALIDMEDIATYPE;

		// Use subtypeDVH for check MEDIASUBTYPE_dvh1, 'DVHQ' and any dvh? types
		GUID subtypeDVH = pmt->subtype;
		((BYTE*)&subtypeDVH.Data1)[3] = (char)'1';

		if( pmt->subtype != MEDIASUBTYPE_dvsd &&
			pmt->subtype != MEDIASUBTYPE_dvhd &&
			pmt->subtype != MEDIASUBTYPE_dvsl &&
			pmt->subtype != MEDIASUBTYPE_dv25 &&
			pmt->subtype != MEDIASUBTYPE_dv50 &&
			!IS_GUID_FCC(subtypeDVH, FCC('dvh1')) && 
			!IS_GUID_FCC(pmt->subtype, FCC('CDVC')))
			return VFW_E_INVALIDSUBTYPE;

		if(pmt->pbFormat == NULL)
			return VFW_E_INVALIDMEDIATYPE;

		if(pmt->majortype == MEDIATYPE_Interleaved)
		{
			if(pmt->formattype != FORMAT_DvInfo)
				return VFW_E_INVALIDMEDIATYPE;
			if(pmt->cbFormat < sizeof(DVINFO))
				return VFW_E_INVALIDMEDIATYPE;
		}
		else
		{
			ATLASSERT(!bPureDVOnly && pmt->majortype == MEDIATYPE_Video);

			if(pmt->formattype == FORMAT_VideoInfo)
			{
				if(pmt->cbFormat < sizeof(VIDEOINFOHEADER))
					return VFW_E_INVALIDMEDIATYPE;

				const VIDEOINFOHEADER* pvih = (const VIDEOINFOHEADER*)pmt->pbFormat;
				if(pvih->bmiHeader.biWidth == 0 || pvih->bmiHeader.biHeight == 0)
					return VFW_E_INVALIDMEDIATYPE;
				if(pvih->bmiHeader.biCompression != GUID2FOURCC(pmt->subtype))
					return VFW_E_INVALIDMEDIATYPE;
			}
			else if(pmt->formattype == FORMAT_VideoInfo2)
			{
				if(pmt->cbFormat < sizeof(VIDEOINFOHEADER2))
					return VFW_E_INVALIDMEDIATYPE;

				const VIDEOINFOHEADER2* pvih2 = (const VIDEOINFOHEADER2*)pmt->pbFormat;
				if(pvih2->bmiHeader.biWidth == 0 || pvih2->bmiHeader.biHeight == 0)
					return VFW_E_INVALIDMEDIATYPE;
				if(pvih2->bmiHeader.biCompression != GUID2FOURCC(pmt->subtype))
					return VFW_E_INVALIDMEDIATYPE;
			}
			else
				return VFW_E_INVALIDMEDIATYPE;
		}

		return S_OK;
	}

	//=================================================================
	// Functions for create media types

	// nWidth, nHeight and nAlignedWidth corrected 
	// If nAlignedWidth = 0, then use nAlignedWidth = nWidth
	static HRESULT MTCreateVideo(IN OUT CMType& mt, IN FOURCC fccType, IN REFGUID formattype,
								 IN DWORD nWidth, IN int nHeight, IN DWORD nAlignedWidth = 0, IN bool _bAcceptCompressed = FALSE )
	{
		if(!IsEqualGUID(formattype, FORMAT_VideoInfo) && !IsEqualGUID(formattype, FORMAT_VideoInfo2))
			return E_INVALIDARG;

		// Don't correct properties, if properies are bad - return E_INVALIDARG
		int nFormat = CPictureFormats::TestAlign(fccType, nWidth, nHeight, nAlignedWidth);
		if(nFormat == -1 && !_bAcceptCompressed )
			return E_INVALIDARG;

		if(nAlignedWidth == 0)
		{
			// If TestAlign() is OK, then nWidth is aligned
			nAlignedWidth = nWidth;
		}

		GUID subtype = {};
		if( nFormat >= 0 )
			subtype = *(CPictureFormats::s_arrSupportedFormats[nFormat].pSubtype);
		else
			subtype = Fourcc2Guid( fccType );

		//---------------------------------
		ULONG cbFormat = 0;
		BYTE* pbSource = NULL;
		VIDEOINFOHEADER rgVInfo = {0};
		VIDEOINFOHEADER2 rgVInfo2 = {0};
		BITMAPINFOHEADER* pBmpHeader = NULL;
		RECT* prcSource = NULL;
		RECT* prcTarget = NULL;

		if(IsEqualGUID(formattype, FORMAT_VideoInfo))
		{
			cbFormat = sizeof(VIDEOINFOHEADER);
			pbSource = (BYTE*)&rgVInfo;
			pBmpHeader = &rgVInfo.bmiHeader;
			prcSource = &rgVInfo.rcSource;
			prcTarget = &rgVInfo.rcTarget;
		}
		else
		{
			cbFormat = sizeof(VIDEOINFOHEADER2);
			pbSource = (BYTE*)&rgVInfo2;
			pBmpHeader = &rgVInfo2.bmiHeader;
			prcSource = &rgVInfo2.rcSource;
			prcTarget = &rgVInfo2.rcTarget;
		}

		//---------------------------------
		memset(pbSource, 0, cbFormat);

		if(nAlignedWidth != nWidth)
		{
			LONG nH = (LONG)nHeight;
			if(nH < 0)
				nH = -nH;

			prcSource->right = nWidth;
			prcSource->bottom = nH;
			prcTarget->right = nWidth;
			prcTarget->bottom = nH;
		}

		if(IsEqualGUID(formattype, FORMAT_VideoInfo2))
		{
			// ATTENTION. We should set this values with something != 0
			// else several asserts from video renders will be shown and graph fail.
			// You can correct this values in caller program
			rgVInfo2.dwPictAspectRatioX = 4;
			rgVInfo2.dwPictAspectRatioY = 3;
		}

		HRESULT hr = CPicturePersist::FillBmpInfoHeader(fccType, nAlignedWidth, nHeight, pBmpHeader);
		if( FAILED( hr ) )
		{

// MT: ( MEDIATYPE_Video / FCC_MJPG / FORMAT_VideoInfo ), size=921600, cbForm=88, fix:Y, compr:N
// MT: src=[0,0:0,0], targ=[0,0:0,0], rate=221184000, errRate=0, AvgT=0.033'3333
// MT:  size=40, w=640, h=480, planes=1, bits=24, compress=MJPG, sizeImg=921600
			memset(pBmpHeader, 0, sizeof(BITMAPINFOHEADER));
			pBmpHeader->biSize = sizeof(BITMAPINFOHEADER);
			pBmpHeader->biWidth = (LONG)nWidth;
			pBmpHeader->biHeight = nHeight;
			pBmpHeader->biPlanes = 1;
			pBmpHeader->biBitCount = 24;
			pBmpHeader->biCompression = fccType;
			pBmpHeader->biSizeImage = nWidth * nHeight * 3; // In log: 
		}
		
		hr = mt.Set(MEDIATYPE_Video, subtype, formattype, cbFormat, pbSource);
		if(SUCCEEDED(hr))
			mt.lSampleSize = pBmpHeader->biSizeImage;
		return hr;
	}

	// This function needed for old VFW codecs.
	// Mediatype is MEDIATYPE_Video, subtype created on base of pBMPInfo->bmiHeader.biCompression
	//
	// ATTENTION : sets only VIDEOINFOHEADER::bmiHeader, all another fields of VIDEOINFOHEADER == 0
	static HRESULT MTCreateVideo(IN OUT CMType& mt, IN REFGUID format, const IN BITMAPINFO* pBMPInfo)
	{
		ATLASSERT(pBMPInfo != NULL);
		if(pBMPInfo == NULL) return E_INVALIDARG;

		const BITMAPINFOHEADER& bmiHeader = pBMPInfo->bmiHeader;
		ATLASSERT(bmiHeader.biSize >= sizeof(BITMAPINFOHEADER));
		if(bmiHeader.biSize < sizeof(BITMAPINFOHEADER)) return E_INVALIDARG;

		if(format != FORMAT_VideoInfo && format != FORMAT_VideoInfo2)
			return E_INVALIDARG;

		//---------------------------------
		DWORD dwCompression = bmiHeader.biCompression;
		GUID subtype = MEDIASUBTYPE_YUY2;
		const GUID* pSubtype = &subtype;

		if(dwCompression == BI_RGB)
		{
			WORD nBitCount = bmiHeader.biBitCount;

			if(nBitCount == 32)
				pSubtype = &MEDIASUBTYPE_RGB32;
			else if(nBitCount == 24)
				pSubtype = &MEDIASUBTYPE_RGB24;
			else if(nBitCount == 16)
				pSubtype = &MEDIASUBTYPE_RGB565;
			else
			{
				ATLASSERT(!_T("MTHelp::MTCreateVideo() : Unsupported BMP::biBitCount"));
				return E_INVALIDARG;
			}
		}
		else if(dwCompression == BI_RLE8 || dwCompression == BI_RLE4 || dwCompression == BI_BITFIELDS)
		{
			ATLASSERT(!_T("MTHelp::MTCreateVideo() : Unsupported BMP::biCompression"));
			return E_INVALIDARG;
		}
		else
			subtype.Data1 = bmiHeader.biCompression;

		//---------------------------------
		ULONG cbFormat = (format == FORMAT_VideoInfo)? sizeof(VIDEOINFOHEADER) : sizeof(VIDEOINFOHEADER2);
		cbFormat += bmiHeader.biSize - sizeof(BITMAPINFOHEADER);

		HRESULT hr = mt.Set(MEDIATYPE_Video, *pSubtype, format, cbFormat);
		if(hr == S_OK)
		{
			BYTE* pbFormat = mt.pbFormat;
			ATLASSERT(pbFormat != NULL);

			if(format == FORMAT_VideoInfo)
			{
				VIDEOINFOHEADER* pvih = (VIDEOINFOHEADER*)pbFormat;
				memset(pvih, 0, offsetof(VIDEOINFOHEADER, bmiHeader));
				memcpy(&pvih->bmiHeader, pBMPInfo, bmiHeader.biSize);
			}
			else
			{
				VIDEOINFOHEADER2* pvih2 = (VIDEOINFOHEADER2*)pbFormat;
				memset(pvih2, 0, offsetof(VIDEOINFOHEADER2, bmiHeader));
				memcpy(&pvih2->bmiHeader, pBMPInfo, bmiHeader.biSize);
			}
	
			mt.lSampleSize = bmiHeader.biSizeImage;
		}
		return hr;
	}

	// For custom audio types pWF->wFormatTag = WAVE_FORMAT_UNKNOWN
	// NOTE. If subtype is equal to GUID_NULL, then subtype will be constructed
	// from WAVEFORMATEX::wFormatTag
	static HRESULT MTCreateAudio(IN OUT CMType& mt, REFGUID subtype, WAVEFORMATEX* pWF)
	{
		ATLASSERT(pWF != NULL);
		if(pWF == NULL) return E_INVALIDARG;

		GUID gSubtype = subtype;
		if(gSubtype == GUID_NULL)
		{
			gSubtype = Fourcc2Guid(pWF->wFormatTag);
		}

		ULONG cbFormat = sizeof(WAVEFORMATEX) + pWF->cbSize;
		HRESULT hr = mt.Set(MEDIATYPE_Audio, gSubtype, FORMAT_WaveFormatEx, cbFormat, (BYTE*)pWF);
		if(hr == S_OK)
			mt.lSampleSize = pWF->nBlockAlign;
		return S_OK;
	}

	static HRESULT MTCreateFormatPCM(WAVEFORMATEX* pWF, WAVETYPE eWT)
	{
		ATLASSERT(pWF != NULL);
		if(pWF == NULL) return E_POINTER;
		ATLASSERT(eWT <= WT_8000_M_8);
		if(eWT > WT_8000_M_8) return E_INVALIDARG;

		//---------------------------------
		static DWORD arrFreq[] = { 48000, 44100, 32000, 24000, 22050, 16000, 11025, 8000 };
		ULONG n = (ULONG)eWT%4;		// 4: stereo+16bps, stereo+8bps, mono+16bps, mono+8bps

		pWF->wFormatTag = WAVE_FORMAT_PCM;
		pWF->nChannels = (WORD)((n & 1)? 1 : 2);
		pWF->nSamplesPerSec = arrFreq[ (ULONG)eWT / 4 ];
		pWF->wBitsPerSample = (WORD)((n & 2)? 8 : 16);
		pWF->nBlockAlign = (WORD)( (pWF->nChannels * pWF->wBitsPerSample)/8 );
		pWF->nAvgBytesPerSec = pWF->nSamplesPerSec * pWF->nBlockAlign;
		pWF->cbSize = 0;

		return S_OK;
	}

	static HRESULT MTCreatePCM(IN OUT CMType& mt, WAVETYPE eWT)
	{
		WAVEFORMATEX rgWF = {0};
		HRESULT hr = MTCreateFormatPCM(&rgWF, eWT);
		if(hr == S_OK)
			hr = mt.Set(MEDIATYPE_Audio, MEDIASUBTYPE_PCM, FORMAT_WaveFormatEx, sizeof(WAVEFORMATEX), (BYTE*)&rgWF);
		if(hr == S_OK)
			mt.lSampleSize = rgWF.nBlockAlign;
		return hr;
	}

	static HRESULT MTCreatePCM(IN OUT CMType& mt, DWORD dwFreq, short wChannels, short wBits,
		DWORD dwChannelMask = 0xFFFFFFFF)
	{
		ATLASSERT(dwFreq != 0);
		if(dwFreq == 0) return E_INVALIDARG;

		ATLASSERT(0 < wChannels && wChannels <= 32);
		if(wChannels <= 0 || wChannels > 32) return E_INVALIDARG;

		//---------------------------------
		WAVEFORMATEX rgWF = {0};
		WAVEFORMATEXTENSIBLE rgWFE = {0};

		BYTE* pbFormat = NULL;
		ULONG cbFormat = 0;
		ULONG lSampleSize = 0;

		bool bPCM = (wChannels <= 2 && wBits > 0 && wBits <= 16);
		if(bPCM)
		{
			ATLASSERT(wBits == 8 || wBits == 16);
			if(wBits != 8 && wBits != 16) return E_INVALIDARG;

			rgWF.wFormatTag = WAVE_FORMAT_PCM;
			rgWF.nChannels = wChannels;
			rgWF.nSamplesPerSec = dwFreq;
			rgWF.wBitsPerSample = wBits;

			rgWF.nBlockAlign = (WORD)((rgWF.nChannels * rgWF.wBitsPerSample)/8);
			rgWF.nAvgBytesPerSec = rgWF.nSamplesPerSec * rgWF.nBlockAlign;

			rgWF.cbSize = 0;

			pbFormat = (BYTE*)&rgWF;
			cbFormat = sizeof(WAVEFORMATEX);
			lSampleSize = rgWF.nBlockAlign;
		}
		else
		{
			ATLASSERT(0 < wChannels && wChannels <= 32);
			if(wChannels == 0 || wChannels > 32) return E_INVALIDARG;

			ATLASSERT( ABS(wBits) >= 4);
			if( ABS(wBits) < 4) return E_INVALIDARG;

			WORD wBitsPerSample = (WORD)ALIGN_UP(ABS(wBits), 8);

			rgWFE.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
			rgWFE.Format.nChannels = wChannels;
			rgWFE.Format.nSamplesPerSec = dwFreq;
			rgWFE.Format.wBitsPerSample = wBitsPerSample;

			rgWFE.Format.nBlockAlign = (WORD)((rgWFE.Format.nChannels * rgWFE.Format.wBitsPerSample)/8);
			rgWFE.Format.nAvgBytesPerSec = rgWFE.Format.nSamplesPerSec * rgWFE.Format.nBlockAlign;

			rgWFE.Format.cbSize = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);

			rgWFE.Samples.wValidBitsPerSample = ABS(wBits);

			if(dwChannelMask == 0xFFFFFFFF)
				dwChannelMask = (1 << wChannels) - 1;

			rgWFE.dwChannelMask = dwChannelMask;
			rgWFE.SubFormat = wBits > 0 ? KSDATAFORMAT_SUBTYPE_PCM : KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;

			pbFormat = (BYTE*)&rgWFE;
			cbFormat = sizeof(WAVEFORMATEXTENSIBLE);
			lSampleSize = rgWFE.Format.nBlockAlign;
		}

		HRESULT hr = mt.Set(MEDIATYPE_Audio, MEDIASUBTYPE_PCM, FORMAT_WaveFormatEx, cbFormat, pbFormat);
		if(SUCCEEDED(hr))
			mt.lSampleSize = lSampleSize;
		return hr;
	}

/* For references only: code moved to MTCreatePCM()
	static HRESULT MTCreatePCM_Ex(IN OUT CMType& mt, WORD wFreq, WORD wChannels, WORD wBits, DWORD dwChannelMask = 0)
	{
		ATLASSERT(wFreq != 0);
		if(wFreq == 0) return E_INVALIDARG;

		ATLASSERT(wChannels == 1 || (wChannels % 2 == 0 && wChannels < 32) );
		if(wChannels != 1 && (wChannels % 2 != 0 || wChannels >= 32)) return E_INVALIDARG;

		ATLASSERT(wBits >= 8);
		if(wBits < 8) return E_INVALIDARG;
		WORD wUseBits = ALIGN_UP(wBits,8);

		WAVEFORMATEXTENSIBLE rgWFE = {0};
		rgWFE.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
		rgWFE.Format.nChannels = wChannels;
		rgWFE.Format.nSamplesPerSec = wFreq;
		rgWFE.Format.wBitsPerSample = wUseBits;
		rgWFE.Format.nBlockAlign = (WORD)( (wChannels * wUseBits)/8 );
		rgWFE.Format.nAvgBytesPerSec = (DWORD)wFreq * (DWORD)rgWFE.Format.nBlockAlign;
		rgWFE.Format.cbSize = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
		if( wBits != wUseBits )
		{
			rgWFE.Samples.wValidBitsPerSample = wBits;
		}
		if( dwChannelMask == 0 ) dwChannelMask = (1 << wChannels) - 1;
		if( wChannels == 32 ) dwChannelMask = 0xFFFFFFFF;
		rgWFE.dwChannelMask = dwChannelMask;
		rgWFE.SubFormat = MEDIASUBTYPE_PCM;

		HRESULT hr = mt.Set(MEDIATYPE_Audio, MEDIASUBTYPE_PCM, FORMAT_WaveFormatEx,
						   sizeof(WAVEFORMATEXTENSIBLE), (BYTE*)&rgWFE);
		if(SUCCEEDED(hr))
			mt.lSampleSize = rgWFE.Format.nBlockAlign;
		return hr;
	}
*/
	// For mediatype numerator in audio channel
	static HRESULT MTGeneratePCM(ULONG ulNumber, AM_MEDIA_TYPE** ppmt)
	{
		ATLASSERT(ppmt != NULL);
		if(ppmt == NULL) return E_POINTER;
		*ppmt = NULL;

		//---------------------------------
		static DWORD arrFreq[] = { 48000, 44100, 22050, 11025, 8000 };
		if(ulNumber >= SIZEOF_ARRAY(arrFreq) * 2 * 2) return S_FALSE;

		ULONG n = ulNumber/SIZEOF_ARRAY(arrFreq);

		WAVEFORMATEX rgWF = {0};
		rgWF.wFormatTag = WAVE_FORMAT_PCM;
		rgWF.nChannels = (WORD)((n & 1)? 1 : 2);
		rgWF.nSamplesPerSec = arrFreq[ ulNumber%SIZEOF_ARRAY(arrFreq) ];
		rgWF.wBitsPerSample = (WORD)((n & 2)? 8 : 16);
		rgWF.nBlockAlign = (WORD)( (rgWF.nChannels * rgWF.wBitsPerSample)/8 );
		rgWF.nAvgBytesPerSec = rgWF.nSamplesPerSec * rgWF.nBlockAlign;

		*ppmt = MTCreate(MEDIATYPE_Audio, MEDIASUBTYPE_PCM, FORMAT_WaveFormatEx,
						 sizeof(WAVEFORMATEX), (BYTE*)&rgWF);
		ATLASSERT(*ppmt != NULL);
		if(*ppmt == NULL)
			return E_OUTOFMEMORY;

		(*ppmt)->lSampleSize = rgWF.nBlockAlign;
		return S_OK;
	}

	// Functions for construct array of mediatypes from base mediatype and
	// AUDIO_STREAM_CONFIG_CAPS structure
	static HRESULT MTGetFromAudStreamCaps(const AM_MEDIA_TYPE* pmtBase, 
		const AUDIO_STREAM_CONFIG_CAPS* pAudStreamCaps, CSimpleArray<CMType>& arrTypes) 
	{
		ATLASSERT(pmtBase != NULL);
		if(pmtBase == NULL) return E_POINTER;
		ATLASSERT(pAudStreamCaps != NULL);
		if(pAudStreamCaps == NULL) return E_POINTER;

		// Create CMType and retrieve WAVEFORMATEX structure
		CMType mtBase(pmtBase);
		WAVEFORMATEX* pBaseFormat = mtBase;
		ATLASSERT(pBaseFormat != NULL);
		if(pBaseFormat == NULL) return VFW_E_INVALIDMEDIATYPE;

		ATLASSERT(IsEqualGUID(pAudStreamCaps->guid, MEDIATYPE_Audio));
		if(!IsEqualGUID(pAudStreamCaps->guid, MEDIATYPE_Audio)) return VFW_E_INVALIDMEDIATYPE;

		// Round all supported channels
		for(ULONG nChannels = pAudStreamCaps->MaximumChannels; 
			nChannels >= pAudStreamCaps->MinimumChannels; 
			nChannels -= pAudStreamCaps->ChannelsGranularity)
		{
			// Round all supported BitsPerSample
			for(ULONG nBits = pAudStreamCaps->MaximumBitsPerSample;
				nBits >= pAudStreamCaps->MinimumBitsPerSample;
				nBits -= pAudStreamCaps->BitsPerSampleGranularity)
			{
				// Round all supported Frequency
				for(ULONG nFreq = pAudStreamCaps->MaximumSampleFrequency;
					nFreq >= pAudStreamCaps->MinimumSampleFrequency;
					nFreq -= pAudStreamCaps->SampleFrequencyGranularity)
				{
					// Construct new WAVEFORMATEX structure

					// Note: for WAVE_FORMAT_PCM cbSize can be not initialized or cbFormat
					//		 can be less then size of WAVEFORMATEX structure.
					DWORD cbSize = (pBaseFormat->wFormatTag == WAVE_FORMAT_PCM)? 0 : pBaseFormat->cbSize;
					WAVEFORMATEX* pNewFormat = (WAVEFORMATEX*)_alloca( sizeof(WAVEFORMATEX) + cbSize );
					::CopyMemory( pNewFormat, pBaseFormat, MIN( sizeof(WAVEFORMATEX) + cbSize, pmtBase->cbFormat ) );

					// Update new WAVEFORMATEX
					pNewFormat->cbSize = (WORD)cbSize;
					pNewFormat->nChannels = (WORD)nChannels;
					pNewFormat->wBitsPerSample = (WORD)nBits;
					pNewFormat->nSamplesPerSec = nFreq;

					// For WAVE_FORMAT_PCM we correct nAvgBytesPerSec and nBlockAlign 
					if( pBaseFormat->wFormatTag == WAVE_FORMAT_PCM ||
						pBaseFormat->wFormatTag == WAVE_FORMAT_EXTENSIBLE )
					{
						pNewFormat->nBlockAlign = (WORD)( (pNewFormat->nChannels * pNewFormat->wBitsPerSample)/8 );
						if(pBaseFormat->wFormatTag != WAVE_FORMAT_EXTENSIBLE)
						{
							pNewFormat->nAvgBytesPerSec = pNewFormat->nBlockAlign * pNewFormat->nSamplesPerSec;
						}
						else
						{
							ATLTRACE2(atlTraceGeneral, TRACELEVEL_Warning,
								_T("WARNING. [WAVE_FORMAT_EXTENSIBLE] nAvgBytesPerSec ")
								_T("copy from base structure ( %d )\n"), pNewFormat->nAvgBytesPerSec);
						}
					}
					else
					{
						ATLTRACE2(atlTraceGeneral, TRACELEVEL_Warning,
							_T("WARNING. [Tag=%d] nBlockAlign and nAvgBytesPerSec ")
							_T("copy from base structure ( %d, %d )\n"), 
							pNewFormat->wFormatTag, pNewFormat->nBlockAlign, pNewFormat->nAvgBytesPerSec);
					}

					// Create specified mediatype and add it into array
					CMType mtNew;
					HRESULT hr = MTCreateAudio(mtNew, mtBase.subtype, pNewFormat);
					ATLASSERT(hr == S_OK);
					if(hr == S_OK)
					{
						arrTypes.Add(mtNew);
					}
				}
			}
		}

		return S_OK;
	}

	//=================================================================
	// Functions for save/load media types

	static HRESULT MTWriteToFile(const AM_MEDIA_TYPE* pmt, HANDLE hFile)
	{
		ATLASSERT(pmt != NULL);
		if(pmt == NULL) return E_INVALIDARG;
		ATLASSERT(hFile != NULL && hFile != INVALID_HANDLE_VALUE);
		if(hFile == NULL || hFile == INVALID_HANDLE_VALUE) return E_INVALIDARG;
		
		//---------------------------------
		AM_MEDIA_TYPE mt;
		CopyMemory(&mt, pmt, sizeof(AM_MEDIA_TYPE));

		if(pmt->pbFormat != NULL)
		{
			ATLASSERT(pmt->cbFormat != 0);
			if(pmt->cbFormat == 0) return E_ABORT;

			// Change address of format structure to shift from AM_MEDIA_TYPE start
			mt.pbFormat = (BYTE*)sizeof(AM_MEDIA_TYPE);
		}
		else
		{
			ATLASSERT(pmt->cbFormat == 0);
			if(pmt->cbFormat != 0) return E_ABORT;
		}

		// Don't write COM-pointer!
		mt.pUnk = NULL;

		DWORD nWritten = 0;
		BOOL bRet = ::WriteFile(hFile, &mt, sizeof(AM_MEDIA_TYPE), &nWritten, NULL);
		if(!bRet || nWritten != sizeof(AM_MEDIA_TYPE))
			return ERROR_FROM_WIN32(::GetLastError(), E_FAIL);

		if(mt.pbFormat != NULL)
		{
			bRet = ::WriteFile(hFile, pmt->pbFormat, mt.cbFormat, &nWritten, NULL);
			if(!bRet || nWritten != mt.cbFormat)
				return ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
		}

		return S_OK;
	}

	static HRESULT MTReadFromFile(AM_MEDIA_TYPE** ppmt, HANDLE hFile)
	{
		ATLASSERT(ppmt != NULL);
		if(ppmt == NULL) return E_POINTER;
		ATLASSERT(hFile != NULL && hFile != INVALID_HANDLE_VALUE);
		if(hFile == NULL || hFile == INVALID_HANDLE_VALUE) return E_INVALIDARG;
		
		*ppmt = NULL;

		//---------------------------------
		HRESULT hr = S_OK;

		AM_MEDIA_TYPE rgMT = {0};
		DWORD nRead = 0;
		BOOL bRet = ::ReadFile(hFile, &rgMT, sizeof(AM_MEDIA_TYPE), &nRead, NULL);
		if(!bRet || nRead != sizeof(AM_MEDIA_TYPE))
			return ERROR_FROM_WIN32(::GetLastError(), E_FAIL);

		ATLASSERT(rgMT.pUnk == NULL);
		if(rgMT.pUnk != NULL) return E_FAIL;

		//---------------------------------
		BYTE* pbFormat = NULL;
		if(rgMT.pbFormat != 0)
		{
			ATLASSERT((DWORD_PTR)rgMT.pbFormat == sizeof(AM_MEDIA_TYPE));
			if((DWORD_PTR)rgMT.pbFormat != sizeof(AM_MEDIA_TYPE)) return E_FAIL;

			ATLASSERT(rgMT.cbFormat != 0);
			if(rgMT.cbFormat == 0) return E_FAIL;

			pbFormat = (BYTE*)::CoTaskMemAlloc(rgMT.cbFormat);
			if(pbFormat == NULL)
			{
				ATLASSERT(!_T("CMTHelp::MTReadFromFile() : CoTaskMemAlloc(rgMT.cbFormat) == NULL"));
				return E_OUTOFMEMORY;
			}

			bRet = ::ReadFile(hFile, pbFormat, rgMT.cbFormat, &nRead, NULL);
			if(!bRet || nRead != rgMT.cbFormat)
			{
				hr = ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
				CoTaskMemFree(pbFormat);
				return hr;
			}
		}
		else
		{
			ATLASSERT(rgMT.cbFormat == 0);
			if(rgMT.cbFormat != 0) return E_UNEXPECTED;
		}

		//---------------------------------
		*ppmt = (AM_MEDIA_TYPE*)::CoTaskMemAlloc(sizeof(AM_MEDIA_TYPE));
		if(*ppmt == NULL)
		{
			ATLASSERT(!_T("CMTHelp::MTReadFromFile() : CoTaskMemAlloc(sizeof(AM_MEDIA_TYPE)) == NULL"));
			CoTaskMemFree(pbFormat);
			return E_OUTOFMEMORY;
		}

		CopyMemory(*ppmt, &rgMT, sizeof(AM_MEDIA_TYPE));
		(*ppmt)->pbFormat = pbFormat;
		return S_OK;
	}

	static HRESULT MTWriteToStream(const AM_MEDIA_TYPE* pmt, IStream* pStream)
	{
		ATLASSERT(pmt != NULL);
		if(pmt == NULL) return E_INVALIDARG;
		ATLASSERT(pStream != NULL);
		if(pStream == NULL) return E_POINTER;
		
		//---------------------------------
		AM_MEDIA_TYPE mt;
		CopyMemory(&mt, pmt, sizeof(AM_MEDIA_TYPE));

		if(pmt->pbFormat != NULL)
		{
			ATLASSERT(pmt->cbFormat != 0);
			if(pmt->cbFormat == 0) return E_ABORT;

			// Change address of format structure to shift from AM_MEDIA_TYPE start
			mt.pbFormat = (BYTE*)sizeof(AM_MEDIA_TYPE);
		}
		else
		{
			ATLASSERT(pmt->cbFormat == 0);
			if(pmt->cbFormat != 0) return E_ABORT;
		}

		// Don't write COM-pointer!
		mt.pUnk = NULL;

		HRESULT hr = pStream->Write(&mt, sizeof(AM_MEDIA_TYPE), NULL);
		if(FAILED(hr))
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### CMTHelp::MTWriteToStream(): ")
				_T("pStream->Write( MT )=0x%X\n"), hr);
			return hr;
		}

		if(pmt->pbFormat != NULL)
		{
			hr = pStream->Write(pmt->pbFormat, mt.cbFormat, NULL);
			if( FAILED( hr ) )
			{
				ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### CMTHelp::MTWriteToStream(): ")
					_T("pStream->Write( FORMAT )=0x%X\n"), hr);
				return hr;
			}
		}

		return S_OK;
	}

	static HRESULT MTReadFromStream(AM_MEDIA_TYPE** ppmt, IStream* pStream)
	{
		ATLASSERT(ppmt != NULL);
		if(ppmt == NULL) return E_POINTER;
		ATLASSERT(pStream != NULL);
		if(pStream == NULL) return E_POINTER;
		
		*ppmt = NULL;

		//---------------------------------
		AM_MEDIA_TYPE rgMT = {0};
		HRESULT hr = pStream->Read(&rgMT, sizeof(AM_MEDIA_TYPE), NULL);
		if(FAILED(hr))
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### CMTHelp::MTReadFromStream(): ")
				_T("pStream->Read( MT )=0x%X\n"), hr);
			return hr;
		}

		ATLASSERT(rgMT.pUnk == NULL);
		if(rgMT.pUnk != NULL) return E_FAIL;

		//---------------------------------
		BYTE* pbFormat = NULL;
		if(rgMT.pbFormat != 0)
		{
			ATLASSERT((DWORD_PTR)rgMT.pbFormat == sizeof(AM_MEDIA_TYPE));
			if((DWORD_PTR)rgMT.pbFormat != sizeof(AM_MEDIA_TYPE)) return E_FAIL;

			ATLASSERT(rgMT.cbFormat != 0);
			if(rgMT.cbFormat == 0) return E_FAIL;

			pbFormat = (BYTE*)::CoTaskMemAlloc(rgMT.cbFormat);
			if(pbFormat == NULL)
			{
				ATLASSERT(!_T("CMTHelp::MTReadFromFile(): CoTaskMemAlloc(rgMT.cbFormat) == NULL"));
				return E_OUTOFMEMORY;
			}

			hr = pStream->Read(pbFormat, rgMT.cbFormat, NULL);
			if(FAILED(hr))
			{
				ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### CMTHelp::MTReadFromStream(): ")
					_T("pStream->Read( FORMAT )=0x%X\n"), hr );
				return hr;
			}
		}
		else
		{
			ATLASSERT(rgMT.cbFormat == 0);
			if(rgMT.cbFormat != 0) return E_FAIL;
		}

		//---------------------------------
		*ppmt = (AM_MEDIA_TYPE*)::CoTaskMemAlloc(sizeof(AM_MEDIA_TYPE));
		if(*ppmt == NULL)
		{
			ATLASSERT(!_T("CMTHelp::MTReadFromStream() : CoTaskMemAlloc(sizeof(AM_MEDIA_TYPE)) == NULL"));
			CoTaskMemFree(pbFormat);
			return E_OUTOFMEMORY;
		}

		CopyMemory(*ppmt, &rgMT, sizeof(AM_MEDIA_TYPE));
		(*ppmt)->pbFormat = pbFormat;
		return S_OK;
	}
};

/////////////////////////////////////////////////////////////////////////////
#endif	// __DSTYPES_H_
