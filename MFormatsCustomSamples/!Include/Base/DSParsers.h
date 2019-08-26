//---------------------------------------------------------------------------
// This file is part of the MediaLooks DirectShow Template Library (DSTL)
//
// DSParsers.h : some functions/classes for parsing DirectShow filter chains, etc. 
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

#ifndef __DSPARSERS_H_
#define __DSPARSERS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ctype.h>
#include "..\WinAPI\Macros.h"
#include "..\WinAPI\Functions.h"
#include "..\WinAPI\Containers.h"
#include "..\Base\DSDefines.h"

#ifdef UNICODE
#define DblToStr DblToStrW
#else
#define DblToStr DblToStrA
#endif // !UNICODE

/////////////////////////////////////////////////////////////////////////////
LPCTSTR __stdcall MakePrefix(LPCTSTR pszName, LPTSTR pszPrefix, DWORD nMaxLength);

class CGuidMap;

LPCTSTR __stdcall Guid2Name(REFGUID rguid, LPTSTR pszName, DWORD nMaxSym);
LPCTSTR __stdcall Fourcc2Str(DWORD dwFCC, LPTSTR pszFCC);
LPCTSTR __stdcall Fourcc2Name(DWORD dwFCC, LPTSTR pszFCC);
void __stdcall ParseMediaType(const AM_MEDIA_TYPE* pmt, LPTSTR pszMediaType, DWORD iMaxLength,
							  bool bShowParams = false);
LPCTSTR __stdcall FilterStateName(FILTER_STATE eState);
LPCTSTR __stdcall DVDDomainName(DVD_DOMAIN eDomain);
LPCTSTR __stdcall DSEventName(long lDSEvent);

// *ppszList must be freed with CoTaskMemFree(*ppszList)
HRESULT __stdcall GetFiltersList(REFCATID rguidDevClass, LPTSTR* ppszList);

HRESULT __stdcall SampleToFile(LPCTSTR pszFile, IMediaSample* pSample);
HRESULT __stdcall BufferToFile(LPCTSTR pszFile, BYTE* pbBuffer, DWORD cbBuffer);

class CFilterChainParser;

/////////////////////////////////////////////////////////////////////////////


// nMaxLength include ending zero
inline LPCTSTR __stdcall MakePrefix(LPCTSTR pszName, LPTSTR pszPrefix, DWORD nMaxLength)
{
	ATLASSERT(pszName != NULL);
	ATLASSERT(pszPrefix != NULL);
	ATLASSERT(nMaxLength > 0);

	if(pszPrefix == NULL || nMaxLength == 0)
		return NULL;
	if(nMaxLength < 6 || pszName == NULL)
	{
		pszPrefix[0] = (TCHAR)0;
		return pszPrefix;
	}

	DWORD n = lstrlen(pszName);
	bool bCut = false;
	if(n > nMaxLength - 3)
	{
		n = nMaxLength - 5;
		bCut = true;
	}
	memcpy(pszPrefix, pszName, n * sizeof(TCHAR));
	if(bCut)
		memcpy(pszPrefix + n, _T("..: "), 5 * sizeof(TCHAR));
	else
		memcpy(pszPrefix + n, _T(": "), 3 * sizeof(TCHAR));
	return pszPrefix;
}


#define MAKE_PREFIX(str)	( MakePrefix( (str), (LPTSTR)_alloca( 128*sizeof(TCHAR) ), 128) )

/////////////////////////////////////////////////////////////////////////////
// CGuidMap

class CGuidMap
{
public:
	CGuidMap();
	LPCTSTR Convert(REFGUID rguid) const;

	//---------------------------------
private:

	static bool s_bSet;

	static int __cdecl CompareGuidNames(const void* pArg1, const void* pArg2)
	{
		const GUID* pGuid1 = ((const GUID_NAME*)pArg1)->pguid;
		const GUID* pGuid2 = ((const GUID_NAME*)pArg2)->pguid;
		return memcmp(pGuid1, pGuid2, sizeof(GUID));
	}
};

__declspec(selectany) bool CGuidMap::s_bSet = false;

inline CGuidMap::CGuidMap()
{
	GuidNameNode* pNode = PREV_GUID_NAME_NODE;
	if(!s_bSet && pNode != NULL)
	{
		while(pNode != NULL)
		{
			ATLASSERT(pNode->pGuidName != NULL && pNode->cElems > 0);

			if(pNode->pGuidName != NULL && pNode->cElems > 0)
				qsort((void*)pNode->pGuidName, pNode->cElems, sizeof(GUID_NAME), CompareGuidNames);
			pNode = pNode->pPrevNode;
		}
		s_bSet = true;
	}
}

inline LPCTSTR CGuidMap::Convert(REFGUID rguid) const
{
	GuidNameNode* pNode = PREV_GUID_NAME_NODE;
	if(!s_bSet || pNode == NULL) return _T("");

	GUID_NAME rgIn = { &rguid, _T("") };
	GUID_NAME* pResult = NULL;
	while(pNode != NULL && pResult == NULL)
	{
		pResult = (GUID_NAME*)bsearch(&rgIn, pNode->pGuidName, pNode->cElems,
									  sizeof(GUID_NAME), CompareGuidNames);
		pNode = pNode->pPrevNode;
	}

	return (pResult == NULL)? _T("") : pResult->pszName;
}

/////////////////////////////////////////////////////////////////////////////
inline LPCTSTR __stdcall Guid2Name(REFGUID rguid, LPTSTR pszName, DWORD nMaxSym)
{
	ATLASSERT(pszName != NULL && _T("Guid2Name()"));
	if(pszName == NULL) return _T("");

	ATLASSERT(nMaxSym > 0 && _T("Guid2Name()"));
	if(nMaxSym == 0) return _T("");

	//---------------------------------
	LPCTSTR psz = CGuidMap().Convert(rguid);
	if(*psz != (TCHAR)0)
		lstrcpyn(pszName, psz, nMaxSym);
	else
	{
		TCHAR szBuf[128] = _T("");		// Size if enough : max size of string from Guid2Str() <= 40 symbols

		if(memcmp( (const BYTE*)&FOURCC_Base + sizeof(DWORD), (const BYTE*)&rguid + sizeof(DWORD),
				   sizeof(GUID) - sizeof(DWORD) ) == 0)
		{
			DWORD dwFCC = GUID2FOURCC(rguid);
			if(dwFCC < (DWORD)MAXWORD)
			{
				// Audio types
				wsprintf(szBuf, _T("TAG_0x%X"), dwFCC);
				if(dwFCC == WAVE_FORMAT_MPEGLAYER3)
					lstrcat(szBuf, _T("(MP3)"));
			}
			else
			{
				TCHAR szFCC[5];
				if(*Fourcc2Str(dwFCC, szFCC) != (TCHAR)0)
				{
					lstrcpy(szBuf, _T("FCC_"));
					lstrcat(szBuf, szFCC);
				}
				else
					Guid2Str(rguid, szBuf);
			}
		}
		else
			Guid2Str(rguid, szBuf);

		lstrcpyn(pszName, szBuf, nMaxSym);
	}

	return pszName;
}

#define GUID2NAME(rguid)	(Guid2Name( (rguid), (LPTSTR)_alloca(128*sizeof(TCHAR)), 128 ))

//---------------------------------
inline LPCTSTR __stdcall Fourcc2Str(DWORD dwFCC, LPTSTR pszFCC)
{
	ATLASSERT(pszFCC != NULL && _T("Fourcc2Str()"));
	if(pszFCC == NULL) return NULL;

	char sz[5];
	*(DWORD*)sz = dwFCC;
	sz[4] = (char)0;

	int i = 0;
	for(; i < 4; ++i)
	{
		int c = 0xFF & sz[i];
		if(!isprint(c)) break;
	}
	if(i == 4)
	{
		USES_CONVERSION;
		lstrcpy(pszFCC, A2T(sz));
	}
	else
		*pszFCC = (TCHAR)0;

	return pszFCC;
}

//---------------------------------
// Length of pszFCC must be at least 16 symbols with ending zero
inline LPCTSTR __stdcall Fourcc2Name(DWORD dwFCC, LPTSTR pszFCC)
{
	ATLASSERT(pszFCC != NULL && _T("Fourcc2Name()"));
	if(pszFCC == NULL) return NULL;

	if(*Fourcc2Str(dwFCC, pszFCC) != (TCHAR)0)
		return pszFCC;

	//---------------------------------
	struct FCC_NAME
	{
		DWORD dwFCC;
		LPCTSTR pszName;
	};
	static FCC_NAME arrFccName[] =
	{
		// From BITMAPINFOHEADER::biCompression
		{ BI_RGB,		_T("BI_RGB") },
		{ BI_RLE8,		_T("BI_RLE8") },
		{ BI_RLE4,		_T("BI_RLE4") },
		{ BI_BITFIELDS,	_T("BI_BITFIELDS") },

		// From MEDIASUBTYPE
		{ 0xe436eb78,	_T("RGB1") },
		{ 0xe436eb79,	_T("RGB4") },
		{ 0xe436eb7a,	_T("RGB8") },
		{ 0xe436eb7b,	_T("RGB565") },
		{ 0xe436eb7c,	_T("RGB555") },
		{ 0xe436eb7d,	_T("RGB24") },
		{ 0xe436eb7e,	_T("RGB32") },
		{ 0x297c55af,	_T("ARGB1555") },
		{ 0x6e6415e6,	_T("ARGB4444") },
		{ 0x773c9ac0,	_T("ARGB32") },
	};

	for(int i = 0; i < SIZEOF_ARRAY(arrFccName); ++i)
	{
		if(arrFccName[i].dwFCC == dwFCC)
		{
			lstrcpy(pszFCC, arrFccName[i].pszName);
			return pszFCC;
		}
	}

	//---------------------------------
	wsprintf(pszFCC, _T("0x%X"), dwFCC);
	return pszFCC;
}

#define FCC2NAME(dwFCC)	(Fourcc2Name( (dwFCC), (LPTSTR)_alloca(16*sizeof(TCHAR)) ))

inline LPCWSTR __stdcall Fourcc2StrW(DWORD dwFCC, LPWSTR pszFCC)
{
	ATLASSERT(pszFCC != NULL && L"Fourcc2Str()");
	if(pszFCC == NULL) return NULL;

	char sz[5] = {0};
	*(DWORD*)sz = dwFCC;

	int i = 0;
	for(; i < 4; ++i)
	{
		int c = 0xFF & sz[i];
		if(!isprint(c)) break;
	}
	if(i == 4)
	{
		USES_CONVERSION;
		lstrcpyW(pszFCC, A2W(sz));
	}
	else
		*pszFCC = (TCHAR)0;

	return pszFCC;
}

inline DWORD __stdcall Str2FourccW(LPCWSTR pszFCC)
{
	if(pszFCC == NULL) return 0;

	DWORD dwRes = 0;
	BYTE* pRes = (BYTE*)&dwRes;
	int nChar = 0;
	while( pszFCC[nChar] && nChar < 4 )
	{
		pRes[nChar] = (BYTE)pszFCC[nChar];
		nChar++;
	}

	return dwRes;
}

inline DWORD __stdcall Str2FourccA(LPCSTR pszFCC)
{
	if(pszFCC == NULL) return 0;

	DWORD dwRes = 0;
	BYTE* pRes = (BYTE*)&dwRes;
	int nChar = 0;
	while( pszFCC[nChar] && nChar < 4 )
	{
		pRes[nChar] = (BYTE)pszFCC[nChar];
	}

	return dwRes;
}

inline LPCWSTR __stdcall Fourcc2NameW(DWORD dwFCC, LPWSTR pszFCC)
{
	ATLASSERT(pszFCC != NULL && L"Fourcc2Name()");
	if(pszFCC == NULL) return NULL;

	if(*Fourcc2StrW(dwFCC, pszFCC) != (TCHAR)0)
		return pszFCC;

	//---------------------------------
	struct FCC_NAME
	{
		DWORD dwFCC;
		LPCWSTR pszName;
	};
	static FCC_NAME arrFccName[] =
	{
		// From BITMAPINFOHEADER::biCompression
		{ BI_RGB,		L"BI_RGB" },
		{ BI_RLE8,		L"BI_RLE8" },
		{ BI_RLE4,		L"BI_RLE4" },
		{ BI_BITFIELDS,	L"BI_BITFIELDS" },

		// From MEDIASUBTYPE
		{ 0xe436eb78,	L"RGB1" },
		{ 0xe436eb79,	L"RGB4" },
		{ 0xe436eb7a,	L"RGB8" },
		{ 0xe436eb7b,	L"RGB565" },
		{ 0xe436eb7c,	L"RGB555" },
		{ 0xe436eb7d,	L"RGB24" },
		{ 0xe436eb7e,	L"RGB32" },
		{ 0x297c55af,	L"ARGB1555" },
		{ 0x6e6415e6,	L"ARGB4444" },
		{ 0x773c9ac0,	L"ARGB32" },
	};

	for(int i = 0; i < SIZEOF_ARRAY(arrFccName); ++i)
	{
		if(arrFccName[i].dwFCC == dwFCC)
		{
			lstrcpyW(pszFCC, arrFccName[i].pszName);
			return pszFCC;
		}
	}

	//---------------------------------
	wsprintfW(pszFCC, L"0x%X", dwFCC);
	return pszFCC;
}

#define FCC2NAMEW(dwFCC)		(Fourcc2NameW( (dwFCC), (LPWSTR)_alloca(16*sizeof(WCHAR)) ))

//---------------------------------
inline void __stdcall ParseMediaType(const AM_MEDIA_TYPE* pmt, LPTSTR pszMediaType, DWORD iMaxLength,
									 bool bShowParams)
{
	ATLASSERT(pszMediaType != NULL && _T("ParseMediaType()"));
	if(pszMediaType == NULL) return;

	if(pmt == NULL)
	{
		lstrcpyn(pszMediaType, _T("Empty media type\n"), iMaxLength);
		return;
	}

	//---------------------------------
	LPCTSTR pszMajorType = GUID2NAME(pmt->majortype);
	LPCTSTR pszSubtype = GUID2NAME(pmt->subtype);
	LPCTSTR pszFormat = GUID2NAME(pmt->formattype);

	//---------------------------------
	BITMAPINFOHEADER* pbmiHeader = NULL;
	TCHAR szParams[32] = _T("");

	if(pmt->pbFormat != NULL)
	{
		if( IsEqualGUID(pmt->formattype, FORMAT_VideoInfo) ||
			IsEqualGUID(pmt->formattype, FORMAT_MPEGVideo))
				pbmiHeader = &((VIDEOINFOHEADER*)pmt->pbFormat)->bmiHeader;
		else
			if( IsEqualGUID(pmt->formattype, FORMAT_VideoInfo2) || 
				IsEqualGUID(pmt->formattype, FORMAT_MPEG2Video))
				pbmiHeader = &((VIDEOINFOHEADER2*)pmt->pbFormat)->bmiHeader;
	}

	if(bShowParams && pbmiHeader != NULL)
		wsprintf(szParams, _T(" %d*%d"), pbmiHeader->biWidth, pbmiHeader->biHeight);

	//---------------------------------
	TCHAR szBuf[512];
	if(bShowParams)
		wsprintf(szBuf, _T("( %s / %s / %s )%s\r\n"), pszMajorType, pszSubtype, pszFormat, szParams);
	else
		wsprintf(szBuf, _T("( %s / %s / %s )"), pszMajorType, pszSubtype, pszFormat);

	lstrcpyn(pszMediaType, szBuf, iMaxLength);
}

//---------------------------------
inline LPCTSTR __stdcall FilterStateName(FILTER_STATE eState)
{
	switch(eState)
	{
		case State_Stopped:		return _T("State_Stopped");
		case State_Paused:		return _T("State_Paused");
		case State_Running:		return _T("State_Running");
	}

	ATLASSERT(!_T("FilterStateName(): Unknown filter state"));
	return _T("STATE UNKNOWN");
}

inline LPCTSTR __stdcall DVDDomainName(DVD_DOMAIN eDomain)
{
	static LPCTSTR pszDomainNames[] = 
	{
		_T("DVD_DOMAIN_???"),
		_T("DVD_DOMAIN_FirstPlay"),
		_T("DVD_DOMAIN_VideoManagerMenu"),
		_T("DVD_DOMAIN_VideoTitleSetMenu"),
		_T("DVD_DOMAIN_Title"),
		_T("DVD_DOMAIN_Stop")
	};

	DWORD n = (DWORD)eDomain;
	if(n >= SIZEOF_ARRAY(pszDomainNames))
		n = 0;
	return pszDomainNames[n];
}

inline LPCTSTR __stdcall DSEventName(long lDSEvent)
{
	// Searches array of EVENT_NAME from next conditions:
	// arr[0].lEvent <= lDSEvent <= arr[SIZEOF_ARRAY(arr) - 1].lEvent
	struct EVENT_NAME
	{
		long lEvent;
		LPCTSTR	pszName;
	};

	#define __EVENT_NAME(event)	{ event, _T(#event) }

	static EVENT_NAME rgQuartzEventNames[] =
	{
		__EVENT_NAME( EC_SYSTEMBASE ),
		__EVENT_NAME( EC_COMPLETE ),
		__EVENT_NAME( EC_USERABORT ),
		__EVENT_NAME( EC_ERRORABORT ),
		__EVENT_NAME( EC_TIME ),
		__EVENT_NAME( EC_REPAINT ),
		__EVENT_NAME( EC_STREAM_ERROR_STOPPED ),
		__EVENT_NAME( EC_STREAM_ERROR_STILLPLAYING ),
		__EVENT_NAME( EC_ERROR_STILLPLAYING ),
		__EVENT_NAME( EC_PALETTE_CHANGED ),
		__EVENT_NAME( EC_VIDEO_SIZE_CHANGED ),
		__EVENT_NAME( EC_QUALITY_CHANGE ),
		__EVENT_NAME( EC_SHUTTING_DOWN ),
		__EVENT_NAME( EC_CLOCK_CHANGED ),
		__EVENT_NAME( EC_PAUSED ),
		__EVENT_NAME( EC_OPENING_FILE ),
		__EVENT_NAME( EC_BUFFERING_DATA ),
		__EVENT_NAME( EC_FULLSCREEN_LOST ),
		__EVENT_NAME( EC_ACTIVATE ),
		__EVENT_NAME( EC_NEED_RESTART ),
		__EVENT_NAME( EC_WINDOW_DESTROYED ),
		__EVENT_NAME( EC_DISPLAY_CHANGED ),
		__EVENT_NAME( EC_STARVATION ),
		__EVENT_NAME( EC_OLE_EVENT ),
		__EVENT_NAME( EC_NOTIFY_WINDOW ),
		__EVENT_NAME( EC_STREAM_CONTROL_STOPPED ),
		__EVENT_NAME( EC_STREAM_CONTROL_STARTED ),
		__EVENT_NAME( EC_END_OF_SEGMENT ),
		__EVENT_NAME( EC_SEGMENT_STARTED ),
		__EVENT_NAME( EC_LENGTH_CHANGED ),
		__EVENT_NAME( EC_DEVICE_LOST ),
		__EVENT_NAME( EC_STEP_COMPLETE ),
		__EVENT_NAME( EC_TIMECODE_AVAILABLE ),
		__EVENT_NAME( EC_EXTDEVICE_MODE_CHANGE ),
		__EVENT_NAME( EC_GRAPH_CHANGED ),
		__EVENT_NAME( EC_CLOCK_UNSET ),
		__EVENT_NAME( EC_VMR_RENDERDEVICE_SET ),
		__EVENT_NAME( EC_VMR_SURFACE_FLIPPED ),
		__EVENT_NAME( EC_VMR_RECONNECTION_FAILED )
	};

	static EVENT_NAME rgDvdEventNames[] =
	{
		__EVENT_NAME( EC_DVDBASE ),
		__EVENT_NAME( EC_DVD_DOMAIN_CHANGE ),
		__EVENT_NAME( EC_DVD_TITLE_CHANGE ),
		__EVENT_NAME( EC_DVD_CHAPTER_START ),
		__EVENT_NAME( EC_DVD_AUDIO_STREAM_CHANGE ),
		__EVENT_NAME( EC_DVD_SUBPICTURE_STREAM_CHANGE ),
		__EVENT_NAME( EC_DVD_ANGLE_CHANGE ),
		__EVENT_NAME( EC_DVD_BUTTON_CHANGE ),
		__EVENT_NAME( EC_DVD_VALID_UOPS_CHANGE ),
		__EVENT_NAME( EC_DVD_STILL_ON ),
		__EVENT_NAME( EC_DVD_STILL_OFF ),
		__EVENT_NAME( EC_DVD_CURRENT_TIME ),
		__EVENT_NAME( EC_DVD_ERROR ),
		__EVENT_NAME( EC_DVD_WARNING ),
		__EVENT_NAME( EC_DVD_CHAPTER_AUTOSTOP ),
		__EVENT_NAME( EC_DVD_NO_FP_PGC ),
		__EVENT_NAME( EC_DVD_PLAYBACK_RATE_CHANGE ),
		__EVENT_NAME( EC_DVD_PARENTAL_LEVEL_CHANGE ),
		__EVENT_NAME( EC_DVD_PLAYBACK_STOPPED ),
		__EVENT_NAME( EC_DVD_ANGLES_AVAILABLE ),
		__EVENT_NAME( EC_DVD_PLAYPERIOD_AUTOSTOP ),
		__EVENT_NAME( EC_DVD_BUTTON_AUTO_ACTIVATED ),
		__EVENT_NAME( EC_DVD_CMD_START ),
		__EVENT_NAME( EC_DVD_CMD_END ),
		__EVENT_NAME( EC_DVD_DISC_EJECTED ),
		__EVENT_NAME( EC_DVD_DISC_INSERTED ),
		__EVENT_NAME( EC_DVD_CURRENT_HMSF_TIME ),
		__EVENT_NAME( EC_DVD_KARAOKE_MODE ),
	};

	static EVENT_NAME rgAnotherEventNames[] =
	{
		__EVENT_NAME( EC_WMT_INDEX_EVENT ),
		__EVENT_NAME( EC_WMT_EVENT ),
		__EVENT_NAME( EC_BUILT ),
		__EVENT_NAME( EC_UNBUILT ),
	};

	#undef __EVENT_NAME

	//---------------------------------
	if( rgQuartzEventNames[0].lEvent <= lDSEvent &&
		lDSEvent <= rgQuartzEventNames[SIZEOF_ARRAY(rgQuartzEventNames) - 1].lEvent)
	{
		for(int i = 0; i < SIZEOF_ARRAY(rgQuartzEventNames); ++i) 
		{
			if(rgQuartzEventNames[i].lEvent == lDSEvent)
				return rgQuartzEventNames[i].pszName;
		}
		return NULL;
	}

	if( rgDvdEventNames[0].lEvent <= lDSEvent &&
		lDSEvent <= rgDvdEventNames[SIZEOF_ARRAY(rgDvdEventNames) - 1].lEvent)
	{
		for(int i = 0; i < SIZEOF_ARRAY(rgDvdEventNames); ++i) 
		{
			if(rgDvdEventNames[i].lEvent == lDSEvent)
				return rgDvdEventNames[i].pszName;
		}
		return NULL;
	}

	if( rgAnotherEventNames[0].lEvent <= lDSEvent &&
		lDSEvent <= rgAnotherEventNames[SIZEOF_ARRAY(rgAnotherEventNames) - 1].lEvent)
	{
		for(int i = 0; i < SIZEOF_ARRAY(rgAnotherEventNames); ++i) 
		{
			if(rgAnotherEventNames[i].lEvent == lDSEvent)
				return rgAnotherEventNames[i].pszName;
		}
		return NULL;
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// *ppszList must be freed with CoTaskMemFree(*ppszList)
inline HRESULT __stdcall GetFiltersList(REFCATID rguidDevClass, LPTSTR* ppszList)
{
	ATLASSERT(ppszList != NULL);
	if(ppszList == NULL) return E_POINTER;
	*ppszList = NULL;

	//---------------------------------
	CStringBuf str;
	if(!str.Assign())
		return E_OUTOFMEMORY;

	//---------------------------------
	CComPtr<ICreateDevEnum> spSysDev;
	HRESULT hr = SEH_CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
		__uuidof(ICreateDevEnum), (void**)&spSysDev.p);
	if(hr != S_OK)
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### GetFiltersList(): ")
			_T("SEH_CoCreateInstance()=0x%X\n"), hr);
		return FAILED(hr)? hr : E_FAIL;
	}
	ATLASSERT(spSysDev != NULL);

	//---------------------------------
	CComPtr<IEnumMoniker> spEnum;
	hr = SEH_ICreateDevEnum::CreateClassEnumerator(spSysDev, rguidDevClass, &spEnum, 0);
	if(hr != S_OK)
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### GetFiltersList(): ")
			_T("SEH_ICreateDevEnum::CreateClassEnumerator()=0x%X\n"), hr);
		return FAILED(hr)? hr : E_FAIL;
	}
	ATLASSERT(spEnum != NULL);

	hr = SEH_IEnumMoniker::Reset(spEnum);
	if(FAILED(hr))
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### GetFiltersList(): ")
			_T("SEH_IEnumMoniker::Reset()=0x%X\n"), hr);
		return FAILED(hr)? hr : E_FAIL;
	}

	//---------------------------------
	USES_CONVERSION;
	TCHAR szName[256];

	wsprintf(szName, _T("\r\n[ List of filters for category: %s ]"), GUID2NAME(rguidDevClass));
	int nSym = lstrlen(szName);
	for(; nSym < 138; ++nSym) szName[nSym] = (TCHAR)'-';
	szName[nSym] = (TCHAR)0;
	str.Append(szName);
	str.Append(_T("\r\n\r\n"));

	DWORD cFetched = 0;
	while(hr == S_OK)
	{
		CComPtr<IMoniker> spMoniker;
		hr = SEH_IEnumMoniker::Next(spEnum, 1, &spMoniker, &cFetched);
		if(hr == S_FALSE)
			break;
		if(hr != S_OK)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### GetFiltersList(): ")
				_T("SEH_IEnumMoniker::Next()=0x%X\n"), hr);
			return FAILED(hr)? hr : E_FAIL;
		}

		//---------------------------------
		ATLASSERT(cFetched == 1);

		CComPtr<IPropertyBag> spPropBag;
		hr = SEH_IMoniker::BindToStorage(spMoniker, NULL, NULL, __uuidof(IPropertyBag), (void**)&spPropBag);
		if(hr != S_OK)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### GetFiltersList(): ")
				_T("SEH_IMoniker::BindToStorage()=0x%X\n"), hr);
			return FAILED(hr)? hr : E_FAIL;
		}

		CComVariant varName;
		varName.vt = VT_BSTR;
		hr = SEH_IPropertyBag::Read(spPropBag, L"FriendlyName", &varName, 0);
		if(hr != S_OK)
		{
			// Don't clear variant in CComVariant destructor: ACCESS_VIOLATION is possible
			varName.vt = VT_EMPTY;

			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### GetFiltersList(): ")
				_T("SEH_IPropertyBag::Read()=0x%X\n"), hr);
			return FAILED(hr)? hr : E_FAIL;
		}

		//---------------------------------
		if(varName.vt != VT_BSTR || varName.bstrVal == NULL)
		{
			wsprintf(szName, _T("ERROR: bad VARIANT, vt=%u, bstrVal=0x%X"),
				varName.vt, varName.bstrVal);
		}
		else
		{
			lstrcpyn(szName, OLE2T(varName.bstrVal), SIZEOF_ARRAY(szName) - 1);
		}

		nSym = lstrlen(szName);
		int nMax = 48;
		if(nSym >= nMax)
		{
			if(nSym < SIZEOF_ARRAY(szName) - 4)
				nMax = nSym + 4;
			else
				nMax = SIZEOF_ARRAY(szName) - 1;
		}

		for(; nSym < nMax; ++nSym)
			szName[nSym] = (TCHAR)' ';
		szName[nSym] = (TCHAR)0;
		str.Append(szName);

		//---------------------------------
		LPOLESTR pszDisplayName = NULL;
		hr = SEH_IMoniker::GetDisplayName(spMoniker, NULL, NULL, &pszDisplayName);
		if(hr != S_OK)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### GetFiltersList(): ")
				_T("SEH_IMoniker::GetDisplayName()=0x%X\n"), hr);

			wsprintf(szName, _T("ERROR: GetDisplayName()=0x%X"), hr);
		}
		else
		{
			ATLASSERT(pszDisplayName != NULL);
			lstrcpyn(szName, OLE2T(pszDisplayName), SIZEOF_ARRAY(szName));
			::CoTaskMemFree(pszDisplayName);
		}

		str.Append(szName);
		str.Append(_T("\r\n"));
	}
	
	str.Append(_T("----------------------------------------------------------------------------------------------------------------------------------------\r\n"));

	//---------------------------------
	*ppszList = str.Detach();
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
inline HRESULT __stdcall SampleToFile(LPCTSTR pszFile, IMediaSample* pSample)
{
	ATLASSERT(pszFile != NULL);
	if(pszFile == NULL) return E_INVALIDARG;
	ATLASSERT(pSample != NULL);
	if(pSample == NULL) return E_INVALIDARG;

	//---------------------------------
	BYTE* pbBuffer = NULL;
	HRESULT hr = pSample->GetPointer(&pbBuffer);
	if(hr != S_OK)
		return hr;
	if(pbBuffer == NULL)
		return E_INVALIDARG;

	DWORD cbBuffer = (DWORD)pSample->GetActualDataLength();
	if(cbBuffer == 0)
		return E_INVALIDARG;

	hr = BufferToFile(pszFile, pbBuffer, cbBuffer);
	return hr;
}

inline HRESULT __stdcall BufferToFile(LPCTSTR pszFile, BYTE* pbBuffer, DWORD cbBuffer)
{
	ATLASSERT(pszFile != NULL);
	if(pszFile == NULL) return E_INVALIDARG;
	ATLASSERT(pbBuffer != NULL);
	if(pbBuffer == NULL) return E_INVALIDARG;
	ATLASSERT(cbBuffer > 0);
	if(cbBuffer == 0) return E_INVALIDARG;

	//---------------------------------
	HRESULT hr = S_OK;
	::SetLastError(ERROR_SUCCESS);

	HANDLE hFile = ::CreateFile(pszFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return ERROR_FROM_WIN32(::GetLastError(), E_FAIL);

	DWORD cbWritten = 0;
	BOOL bRet = ::WriteFile(hFile, pbBuffer, cbBuffer, &cbWritten, NULL);
	if(!bRet || cbWritten != cbBuffer)
		hr = (bRet)? E_FAIL : ERROR_FROM_WIN32(::GetLastError(), E_FAIL);

	::CloseHandle(hFile);
	return hr;
}

/////////////////////////////////////////////////////////////////////////////
// Class for save filter chain data into file or text string 

class CFilterChainParser
{
public :
	CFilterChainParser(const CGuidMap& theGuidMap = CGuidMap()) : m_theGuidMap(theGuidMap)
	{
		m_szGUID[0] = (TCHAR)0;
	}

	HRESULT SaveFiltersChain(IFilterGraph *pGraph, LPCTSTR pszFileName);
	HRESULT GetFiltersChain(IFilterGraph *pGraph, LPTSTR* ppszChain);
	bool	GetFilterData(CStringBuf& str, IBaseFilter* pFilter);
	bool	GetFileName(CStringBuf& str, IBaseFilter* pFilter);
	bool	GetPinData(CStringBuf& str, IPin* pPin);
	bool	GetPinMediaType(CStringBuf& str, IPin* pPin);

	//---------------------------------
	LPCTSTR Translate(REFGUID rGuid)
	{
		LPCTSTR psz = m_theGuidMap.Convert(rGuid);
		if(*psz == (TCHAR)0)
			psz = Guid2Name(rGuid, m_szGUID, 256);
		return psz;
	}

	//---------------------------------
	const CGuidMap	m_theGuidMap;
	TCHAR			m_szGUID[256];
};

inline HRESULT CFilterChainParser::SaveFiltersChain(IFilterGraph *pGraph, LPCTSTR pszFileName)
{
	ATLASSERT(pGraph != NULL && pszFileName != NULL);
	if(pGraph == NULL || pszFileName == NULL) return E_INVALIDARG;

	::SetLastError(0);

	HANDLE hFile = ::CreateFile(pszFileName, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return ERROR_FROM_WIN32(::GetLastError(), E_FAIL);

	//---------------------------------
	LPTSTR pszChain = NULL;
	HRESULT hr = GetFiltersChain(pGraph, &pszChain);
	if(FAILED(hr))
	{
		::CloseHandle(hFile);
		return hr;
	}

	//---------------------------------
	DWORD cbSize = lstrlen(pszChain);
	ATLASSERT(cbSize > 0);
	cbSize = (cbSize + 1) * sizeof(TCHAR);

	DWORD cbWritten = 0;
	BOOL bRet = ::WriteFile(hFile, pszChain, cbSize, &cbWritten, NULL);
	if(!bRet || cbWritten != cbSize)
		hr = ERROR_FROM_WIN32(::GetLastError(), E_FAIL);

	bRet = ::FlushFileBuffers(hFile);
	if(hr == S_OK && !bRet)
		hr = ERROR_FROM_WIN32(::GetLastError(), E_FAIL);

	bRet = ::CloseHandle(hFile);
	if(hr == S_OK && !bRet)
		hr = ERROR_FROM_WIN32(::GetLastError(), E_FAIL);

	::CoTaskMemFree(pszChain);
	return hr;
}

inline HRESULT CFilterChainParser::GetFiltersChain(IFilterGraph *pGraph, LPTSTR* ppszChain)
{
	ATLASSERT(pGraph != NULL);
	if(pGraph == NULL) return E_INVALIDARG;
	ATLASSERT(ppszChain != NULL);
	if(ppszChain == NULL) return E_POINTER;

	//---------------------------------
	*ppszChain = NULL;

	CStringBuf str;
	if(!str.Assign())
		return E_OUTOFMEMORY;

	if(!str.Append(_T("\r\n[Filter Graph]------------------------------------------------------\r\n")))
		return E_OUTOFMEMORY;

	//---------------------------------
	TCHAR szBuf[256] = _T("");

	CComPtr<IEnumFilters> spEnum;
	HRESULT hr = pGraph->EnumFilters(&spEnum);
	if(hr != S_OK)
	{
		wsprintf(szBuf, _T("###Err### CFilterChainParser::GetFiltersChain(): ")
			_T("pGraph->EnumFilters()=0x%X\r\n"), hr);
		if(!str.Append(szBuf))
			return E_OUTOFMEMORY;

		*ppszChain = str.Detach();
		return S_OK;
	}

	spEnum->Reset();

	//---------------------------------
	ULONG cFetched = 0;
	for(;;)
	{
		CComPtr<IBaseFilter> spFilter;
		hr = spEnum->Next(1, &spFilter, &cFetched);
		if(hr == S_FALSE)
			break;

		if(hr != S_OK)
		{
			wsprintf(szBuf, _T("###Err### CFilterChainParser::GetFiltersChain(): ")
				_T("spEnum->Next()=0x%X\r\n"), hr);
			if(!str.Append(szBuf))
				return E_OUTOFMEMORY;

			break;
		}

		ATLASSERT(cFetched == 1);
		if(!GetFilterData(str, spFilter)) return E_OUTOFMEMORY;
	}

	if(!str.Append(_T("\r\n--------------------------------------------------------------------\r\n\r\n")))
		return E_OUTOFMEMORY;

	*ppszChain = str.Detach();
	return S_OK;
}

inline bool CFilterChainParser::GetFilterData(CStringBuf& str, IBaseFilter* pFilter)
{
	ATLASSERT(pFilter != NULL);

	TCHAR szBuf[512] = _T("");

	FILTER_INFO rgInfo;
	rgInfo.achName[0] = L'\0';
	rgInfo.pGraph = NULL;
	HRESULT hr = pFilter->QueryFilterInfo(&rgInfo);
	if(FAILED(hr))
	{
		wsprintf(szBuf, _T("###Err### CFilterChainParser::GetFilterData(): ")
			_T("pFilter->QueryFilterInfo()=0x%X\r\n\r\n"), hr);
		return str.Append(szBuf);
	}

	CLSID clsid = { 0 };
	hr = pFilter->GetClassID(&clsid);
	if(FAILED(hr))
	{
		wsprintf(szBuf, _T("###Err### CFilterChainParser::GetFilterData(): ")
			_T("pFilter->GetClassID()=0x%X\r\n\r\n"), hr);
		return str.Append(szBuf);
	}

	if(rgInfo.pGraph != NULL)
		rgInfo.pGraph->Release();

	//---------------------------------
	USES_CONVERSION;
	LPCTSTR pszName = W2T(rgInfo.achName);

	wsprintf(szBuf, _T("\r\nFilter %s (%s)\r\n"), Translate(clsid), pszName);
	if(!str.Append(szBuf))
		return false;

	//---------------------------------
	// Get file name from filter
	if(!GetFileName(str, pFilter))
		return false;

	//---------------------------------
	CComPtr<IEnumPins> spEnum;
	hr = pFilter->EnumPins(&spEnum);
	if(hr != S_OK)
	{
		wsprintf(szBuf, _T("###Err### CFilterChainParser::GetFilterData(): ")
			_T("pFilter->EnumPins()=0x%X\r\n\r\n"), hr);
		return str.Append(szBuf);
	}

	spEnum->Reset();
	ULONG cFetched = 0;
	for(;;)
	{
		CComPtr<IPin> spPin;
		hr = spEnum->Next(1, &spPin, &cFetched);
		if(hr == S_FALSE)
			break;

		if(hr != S_OK)
		{
			wsprintf(szBuf, _T("###Err### CFilterChainParser::GetFilterData(): ")
				_T("spEnum->Next()=0x%X\r\n\r\n"), hr);
			return str.Append(szBuf);
		}

		ATLASSERT(cFetched == 1);
		if(!GetPinData(str, spPin))
			return false;
	}

	return true;
}

inline bool CFilterChainParser::GetFileName(CStringBuf& str, IBaseFilter* pFilter)
{
	ATLASSERT(pFilter != NULL);

	//---------------------------------
	HRESULT hr = S_OK;
	TCHAR szBuf[512] = _T("");
	USES_CONVERSION;

	CComQIPtr<IFileSourceFilter> spFileSourceFilter(pFilter);
	if(spFileSourceFilter != NULL)
	{
		LPOLESTR pwszFileName = NULL;
		AM_MEDIA_TYPE mt = {0};
		hr = spFileSourceFilter->GetCurFile(&pwszFileName, &mt);
		if(hr != S_OK)
		{
			wsprintf(szBuf, _T("###Err### CFilterChainParser::GetFileName(): ")
				_T("spFileSourceFilter->GetCurFile()=0x%X\r\n\r\n"), hr);
		}
		else
		{
			wsprintf(szBuf, _T("\tINPUT FILE: \"%s\"\r\n"), W2T(pwszFileName));

			::CoTaskMemFree(pwszFileName);

			if(mt.pbFormat != NULL)
				CoTaskMemFree(mt.pbFormat);
			if(mt.pUnk != NULL)
				mt.pUnk->Release();
		}

		if(!str.Append(szBuf))
			return false;
	}

	CComQIPtr<IFileSinkFilter> spFileSinkFilter(pFilter);
	if(spFileSinkFilter != NULL)
	{
		LPOLESTR pwszFileName = NULL;
		AM_MEDIA_TYPE mt = {0};
		hr = spFileSinkFilter->GetCurFile(&pwszFileName, &mt);
		if(hr != S_OK)
		{
			wsprintf(szBuf, _T("###Err### CFilterChainParser::GetFileName(): ")
				_T("spFileSinkFilter->GetCurFile()=0x%X\r\n\r\n"), hr);
		}
		else
		{
			wsprintf(szBuf, _T("\tOUTPUT FILE: \"%s\"\r\n"), W2T(pwszFileName));

			::CoTaskMemFree(pwszFileName);

			if(mt.pbFormat != NULL)
				CoTaskMemFree(mt.pbFormat);
			if(mt.pUnk != NULL)
				mt.pUnk->Release();
		}

		if(!str.Append(szBuf))
			return false;
	}

	return true;
}

inline bool CFilterChainParser::GetPinData(CStringBuf& str, IPin* pPin)
{
	ATLASSERT(pPin != NULL);

	TCHAR szBuf[512] = _T("");

	PIN_INFO rgInfo = {0};
	HRESULT hr = pPin->QueryPinInfo(&rgInfo);
	if(FAILED(hr))
	{
		wsprintf(szBuf, _T("###Err### CFilterChainParser::GetPinData(): ")
			_T("pPin->QueryPinInfo()=0x%X\r\n\r\n"), hr);
		return str.Append(szBuf);
	}

	ATLASSERT(rgInfo.pFilter != NULL);
	rgInfo.pFilter->Release();

	//---------------------------------
	LPCTSTR pszDir = (rgInfo.dir == PINDIR_INPUT)? _T("INPUT") :
		(rgInfo.dir == PINDIR_OUTPUT)? _T("OUTPUT") : _T("UNKNOWN");

	USES_CONVERSION;
	LPCTSTR pszName = W2T(rgInfo.achName);
	wsprintf(szBuf, _T("\t%s: [%s]"), pszDir, pszName);
	if(!str.Append(szBuf))
		return false;

	//---------------------------------
	CComPtr<IPin> spPinConnected;
	hr = pPin->ConnectedTo(&spPinConnected);
	if(hr == VFW_E_NOT_CONNECTED)
	{
		return str.Append(_T("    NOT CONNECTED\r\n"));
	}
	if(FAILED(hr))
	{
		wsprintf(szBuf, _T("\r\n###Err### CFilterChainParser::GetPinData(): ")
			_T("pPin->ConnectedTo()=0x%X\r\n"), hr);
		return str.Append(szBuf);
	}

	//---------------------------------
	rgInfo.achName[0] = L'\0';
	rgInfo.dir = PINDIR_INPUT;
	rgInfo.pFilter = NULL;
	hr = spPinConnected->QueryPinInfo(&rgInfo);
	if(FAILED(hr))
	{
		wsprintf(szBuf, _T("\r\n###Err### CFilterChainParser::GetPinData(): ")
			_T("spPinConnected->QueryPinInfo()=0x%X\r\n"), hr);
		return str.Append(szBuf);
	}

	ATLASSERT(rgInfo.pFilter != NULL);
	CComPtr<IBaseFilter> spConnectedFilter;
	spConnectedFilter.Attach(rgInfo.pFilter);

	FILTER_INFO rgFilterInfo;
	rgFilterInfo.achName[0] = L'\0';
	rgFilterInfo.pGraph = NULL;
	hr = spConnectedFilter->QueryFilterInfo(&rgFilterInfo);
	if(FAILED(hr))
	{
		wsprintf(szBuf, _T("\r\n###Err### CFilterChainParser::GetPinData(): ")
			_T("spConnectedFilter->QueryFilterInfo()=0x%X\r\n"), hr);
		return str.Append(szBuf);
	}

	ATLASSERT(rgFilterInfo.pGraph != NULL);
	rgFilterInfo.pGraph->Release();

	pszName = W2T(rgFilterInfo.achName);
	wsprintf(szBuf, _T(" -> [%s] "), pszName);
	if(!str.Append(szBuf))
		return false;

	return GetPinMediaType(str, pPin);
}

inline bool CFilterChainParser::GetPinMediaType(CStringBuf& str, IPin* pPin)
{
	ATLASSERT(pPin != NULL);

	TCHAR szBuf[256] = _T("");

	CComPtr<IPin> spConnectedPin;
	HRESULT hr = pPin->ConnectedTo(&spConnectedPin);
	if(hr != S_OK) return true;

	AM_MEDIA_TYPE rgMT = {0};
	hr = pPin->ConnectionMediaType(&rgMT);
	if(hr != S_OK)
	{
		wsprintf(szBuf, _T("\r\n###Err### CFilterChainParser::GetPinMediaType(): ")
			_T("pPin->ConnectionMediaType()=0x%X\r\n"), hr);
		return str.Append(szBuf);
	}

	ParseMediaType(&rgMT, szBuf, 256, true);

	if(rgMT.pbFormat != NULL)
		CoTaskMemFree(rgMT.pbFormat);
	if(rgMT.pUnk != NULL)
		rgMT.pUnk->Release();

	return str.Append(szBuf);
}


/////////////////////////////////////////////////////////////////////////////
#endif	// __DSPARSERS_H_
