//---------------------------------------------------------------------------
// This file is part of the MediaLooks DirectShow Template Library (DSTL)
//
// Diagnostics.h : some classes for diagnostic purposes
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

#ifndef __DIAGNOSTICS_H_
#define __DIAGNOSTICS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Functions.h"
#include "Wrappers.h"

#include <winver.h>
#pragma comment(lib, "version.lib")

#ifdef NTDDI_WINBLUE
#if NTDDI_VERSION >= NTDDI_WINBLUE
#include <versionhelpers.h>
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
inline bool __stdcall IsWinNT()
{
#if NTDDI_VERSION < NTDDI_WIN2K
	// true : Windows NT/2000/XP
	static bool bWinNT = (GetVersion() < 0x80000000); 
	return bWinNT;
#else
	return true;
#endif

}

#ifndef NTDDI_WINBLUE
#define NTDDI_WINBLUE (0x06030000)
#endif

/////////////////////////////////////////////////////////////////////////////
typedef enum tagWinVersion
{
	WIN_UNK = 0,
	WIN_WIN32s,
	WIN_95,
	WIN_95_OSR2,			// Windows 95 OSR2
	WIN_98,
	WIN_98_SE,			// Windows 98 Second Edition
	WIN_ME,
	WIN_NT351,
	WIN_NT4,
	WIN_2000,
	WIN_XP,
	WIN_VISTA,
	WIN_7,
	WIN_8,
	WIN_81

} WIN_VERSION;

// Some parts of VS_FIXEDFILEINFO
//
// ATTENTION.
// File's propety page "Version" and resource editor of VC show version data
// as decimal words. Comments to VS_FIXEDFILEINFO version fields isn't valid -
// you should convert hex data to decimal (as below)!

typedef struct tagVerInfo
{
	DWORD		dwProdVerMS;		// e.g. 0x00030010 = "3.16"
	DWORD		dwProdVerLS;		// e.g. 0x00000031 = "0.49"
	DWORD		dwFileVerMS;		// e.g. 0x0019014e = "25.334"
	DWORD		dwFileVerLS;		// e.g. 0x0e9000ff = "3728.255"
} VERINFO;

//---------------------------------
class CDiag
{
public:
	static WIN_VERSION GetWinVer(DWORD* pdwServicePack);

	static bool IsWin9x(WIN_VERSION eWinVer)
	{
		return (eWinVer == WIN_95 || eWinVer == WIN_95_OSR2 || eWinVer == WIN_98 || 
				eWinVer == WIN_98_SE || eWinVer == WIN_ME);
	}

	//---------------------------------
	// *pFileTime is file last write time
	// At least one from pdwSize, pFileTime or pSysTime should be != NULL
	// If file not found : return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
	static HRESULT GetFileInfo(IN LPCTSTR pszFile, OUT DWORD* pdwSize,
							   OUT SYSTEMTIME* pSysTime, OUT FILETIME* pFileTime = NULL);

	static HRESULT GetFileInfoEx(IN LPCTSTR pszFile, OUT ULONGLONG* pullSize,
								 OUT SYSTEMTIME* pSysTime, OUT FILETIME* pFileTime = NULL);

	//---------------------------------
	// If version info not found : return HRESULT_FROM_WIN32(ERROR_NOT_FOUND)
	static HRESULT GetVerInfo(IN LPCTSTR pszModule, OUT VERINFO* pVerInfo);

	// Gets pszField field of version info data for some locales
	// nMaxLength includes a terminating null character
	// Return :
	//	S_OK : all is OK
	//	HRESULT_FROM_WIN32(ERROR_NOT_FOUND) : version info don't found
	//	HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY) : nMaxLength < size of version string
	//	another bad HRESULT
	static HRESULT GetLocaleVerInfo(IN LPCTSTR pszModule, IN LPCTSTR pszField,
									OUT LPTSTR pszInfo, IN DWORD nMaxLength);

	// Converts version number from string to DWORD
	static bool String2Dword(IN LPCTSTR psz, DWORD* pdw);

	//---------------------------------
	static HRESULT GetMaxFileSize(LPCTSTR pszFileName, ULONGLONG* pullSize);
};

/////////////////////////////////////////////////////////////////////////////
inline WIN_VERSION CDiag::GetWinVer(DWORD* pdwServicePack)
{
	WIN_VERSION eWinVer = WIN_UNK;
	if(pdwServicePack != NULL) *pdwServicePack = 0;

	
#if NTDDI_VERSION < NTDDI_WINBLUE
	//---------------------------------
	OSVERSIONINFOEX rgOsVer = {0};
	rgOsVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if(!::GetVersionEx((OSVERSIONINFO*)&rgOsVer))
	{
		rgOsVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if(!::GetVersionEx((OSVERSIONINFO*)&rgOsVer))
		{
			ATLASSERT(!_T("CDiag : GetVersionEx() FAILED"));
			return WIN_UNK;
		}
	}
	//---------------------------------
	if(rgOsVer.dwPlatformId == VER_PLATFORM_WIN32s)
	{
		eWinVer = WIN_WIN32s;
	}
	else if(rgOsVer.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
	{
		if(rgOsVer.dwMajorVersion != 4)
		{
			ATLASSERT(!_T("CDiag : VER_PLATFORM_WIN32_WINDOWS, dwMajorVersion != 4"));
			return WIN_UNK;
		}

		if(rgOsVer.dwMinorVersion == 0)
		{
			if(rgOsVer.szCSDVersion[1] == 'C' || rgOsVer.szCSDVersion[1] == 'B')
				eWinVer = WIN_95_OSR2;
			else
				eWinVer = WIN_95;
		}
		else if(rgOsVer.dwMinorVersion == 10)
		{
			if(rgOsVer.szCSDVersion[1] == 'A')
				eWinVer = WIN_98_SE;
			else
				eWinVer = WIN_98;
		}
		else if(rgOsVer.dwMinorVersion == 90)
		{
			eWinVer = WIN_ME;
		}
		else
		{
			ATLASSERT(!_T("CDiag : VER_PLATFORM_WIN32_WINDOWS, unknown dwMinorVersion"));
			return WIN_UNK;
		}
	}
	else if(rgOsVer.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		if(rgOsVer.dwMajorVersion == 3)
		{
			if(rgOsVer.dwMinorVersion != 51)
				// Only showing assert, returning WIN_NT351 in any case
				ATLASSERT(!_T("CDiag : VER_PLATFORM_WIN32_NT, dwMajorVersion=3, dwMinorVersion!=51"));

			eWinVer = WIN_NT351;
		}
		else if(rgOsVer.dwMajorVersion == 4)
		{
			if(rgOsVer.dwMinorVersion != 0)
				// Only showing assert, returning WIN_NT4 in any case
				ATLASSERT(!_T("CDiag : VER_PLATFORM_WIN32_NT, dwMajorVersion=4, dwMinorVersion!=0"));

			eWinVer = WIN_NT4;
		}
		else if(rgOsVer.dwMajorVersion == 5)
		{
			if(rgOsVer.dwMinorVersion == 0)
			{
				eWinVer = WIN_2000;
			}
			else if(rgOsVer.dwMinorVersion == 1)
			{
				eWinVer = WIN_XP;
			}
			else
			{
				ATLASSERT(!_T("CDiag : VER_PLATFORM_WIN32_NT, dwMajorVersion=5, unknown dwMinorVersion"));
				return WIN_UNK;
			}
		}
		else
		{
			ATLASSERT(!_T("CDiag : VER_PLATFORM_WIN32_NT, unknown dwMajorVersion"));
			return WIN_UNK;
		}
	}
	else
	{
		ATLASSERT(!_T("CDiag : unknown dwPlatformId"));
		return WIN_UNK;
	}

	if(pdwServicePack != NULL)
		*pdwServicePack = ((DWORD)rgOsVer.wServicePackMajor << 16) + (DWORD)rgOsVer.wServicePackMinor;

#else
	if (IsWindows8Point1OrGreater())
		return WIN_81;

	if (IsWindows8OrGreater())
		return WIN_8;

	if (IsWindows7OrGreater())
		return WIN_7;

	if (IsWindowsVistaOrGreater())
		return WIN_VISTA;

	if (IsWindowsXPOrGreater())
		return WIN_XP;

#endif


	
	return eWinVer;
}

//---------------------------------
// If file not found: return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
inline HRESULT CDiag::GetFileInfo(IN LPCTSTR pszFile, OUT DWORD* pdwSize,
								OUT SYSTEMTIME* pSysTime, OUT FILETIME* pFileTime)
{
	ULONGLONG ullSize = (ULONGLONG)0;
	HRESULT hr = GetFileInfoEx(pszFile, (pdwSize != NULL)? &ullSize : NULL, pSysTime, pFileTime);
	if(hr == S_OK && pdwSize != NULL)
	{
		if(ullSize >= (ULONGLONG)MAXDWORD)
		{
			ATLASSERT(!_T("CDiag::GetFileInfo() : File size >= MAXDWORD"));
			hr = E_UNEXPECTED;
		}
		else
			*pdwSize = (DWORD)ullSize;
	}
	return hr;
}

// If file not found: return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
inline HRESULT CDiag::GetFileInfoEx(IN LPCTSTR pszFile, OUT ULONGLONG* pullSize,
									OUT SYSTEMTIME* pSysTime, OUT FILETIME* pFileTime)
{
	ATLASSERT(pszFile != NULL);
	if(pszFile == NULL) return ERROR_INVALID_PARAMETER;

	ATLASSERT(pullSize != NULL || pSysTime != NULL || pFileTime != NULL);

	//---------------------------------
	HRESULT hr = S_OK;
	::SetLastError(NO_ERROR);

	WIN32_FILE_ATTRIBUTE_DATA rgFileAttr = {0};
	if(!::GetFileAttributesEx(pszFile, GetFileExInfoStandard, &rgFileAttr))
	{
		DWORD dw = ::GetLastError();
		if(dw == ERROR_FILE_NOT_FOUND)
		{
			TCHAR szPath[_MAX_PATH] = _T("");
			if(::SearchPath(NULL, pszFile, NULL, _MAX_PATH, szPath, NULL) > 0)
			{
				if(!GetFileAttributesEx(szPath, GetFileExInfoStandard, &rgFileAttr))
					hr = ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
			}
			else
				hr = ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
		}
		else
			hr = ERROR_FROM_WIN32(dw, E_FAIL);
	}

	//---------------------------------
	if(hr == S_OK)
	{
		if(pullSize != NULL)
		{
			*(DWORD*)pullSize = rgFileAttr.nFileSizeLow;
			*((DWORD*)pullSize + 1) = rgFileAttr.nFileSizeHigh;
		}

		if(pFileTime != NULL)
			memcpy(pFileTime, &rgFileAttr.ftLastWriteTime, sizeof(FILETIME));
			
		if(pSysTime != NULL)
		{
			if(!::FileTimeToSystemTime(&rgFileAttr.ftLastWriteTime, pSysTime))
				hr = ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
		}
	}
	return hr;
}

// If version info not found: return HRESULT_FROM_WIN32(ERROR_NOT_FOUND)
inline HRESULT CDiag::GetVerInfo(IN LPCTSTR pszModule, OUT VERINFO* pVerInfo)
{
	ATLASSERT(pszModule != NULL);
	if(pszModule == NULL) return E_INVALIDARG;
	ATLASSERT(pVerInfo != NULL);
	if(pVerInfo == NULL) return E_POINTER;

	memset(pVerInfo, 0, sizeof(VERINFO));

	//---------------------------------
	::SetLastError(ERROR_SUCCESS);

	DWORD dwHandle = 0;
	DWORD dwSize = ::GetFileVersionInfoSize(const_cast<LPTSTR>(pszModule), &dwHandle);
	if(dwSize == 0)
		return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
	
	LPBYTE pbVerBlock = (LPBYTE)_alloca(dwSize);
	if(!::GetFileVersionInfo(const_cast<LPTSTR>(pszModule), dwHandle, dwSize, pbVerBlock))
		return ERROR_FROM_WIN32(::GetLastError(), E_FAIL);

	VS_FIXEDFILEINFO* pvsffi = NULL;
	UINT nLen = 0;
	if(!::VerQueryValue(pbVerBlock, _T("\\"), (void**)&pvsffi, &nLen))
		return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
	if(pvsffi == NULL || nLen < sizeof(VS_FIXEDFILEINFO))
		return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);

	//---------------------------------
	pVerInfo->dwProdVerMS = pvsffi->dwProductVersionMS;
	pVerInfo->dwProdVerLS = pvsffi->dwProductVersionLS;
	pVerInfo->dwFileVerMS = pvsffi->dwFileVersionMS;
	pVerInfo->dwFileVerLS = pvsffi->dwFileVersionLS;

	return S_OK;
}

//---------------------------------
// nMaxLength includes a terminating null character
//
// Return :
//	S_OK : all is OK
//	HRESULT_FROM_WIN32(ERROR_NOT_FOUND) : version info not found
//	HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY) : nMaxLength < size of version string
//	another bad HRESULT
//
// For sample: pszInfoField = _T("ProductVersion")

inline HRESULT CDiag::GetLocaleVerInfo( IN LPCTSTR pszModule, IN LPCTSTR pszField,
										OUT LPTSTR pszInfo, IN DWORD nMaxLength)
{
	ATLASSERT(pszModule != NULL);
	if(pszModule == NULL) return E_INVALIDARG;
	ATLASSERT(pszField != NULL);
	if(pszField == NULL) return E_INVALIDARG;
	ATLASSERT(pszInfo != NULL);
	if(pszInfo == NULL) return E_INVALIDARG;
	ATLASSERT(nMaxLength > 0);
	if(nMaxLength == 0) return E_INVALIDARG;

	pszInfo[0] = (TCHAR)0;

	//---------------------------------
	static WORD wLangs[] = { 0x409, 0x809, 0xc09, 0x1009, 0x1409, 0x1809 };	// english dialects 
	static WORD wCodes[] =
	{
		1200,		// Unicode
		1252,		// Multilingual
		1250,		// Latin-2 (Eastern European),
		1251		// Cyrillic
	};
	struct LANGANDCODEPAGE 
	{
		WORD wLanguage;
		WORD wCodePage;
	} *lpTranslate = NULL;

	//---------------------------------
	::SetLastError(ERROR_SUCCESS);

	DWORD dwHandle = 0;
	DWORD dwSize = ::GetFileVersionInfoSize(const_cast<LPTSTR>(pszModule), &dwHandle);
	if(dwSize == 0)
		return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
	
	LPBYTE pbVerBlock = (LPBYTE)_alloca(dwSize);
	if(!::GetFileVersionInfo(const_cast<LPTSTR>(pszModule), dwHandle, dwSize, pbVerBlock))
		return ERROR_FROM_WIN32(::GetLastError(), E_FAIL);

	//---------------------------------
	DWORD dwLocale = 0;
	UINT nLen = 0;
	if( ::VerQueryValue(pbVerBlock, _T("\\VarFileInfo\\Translation"), (LPVOID*)&lpTranslate, &nLen) &&
		lpTranslate != NULL && nLen >= sizeof(struct LANGANDCODEPAGE))
	{
		// Search English locale
		for(DWORD i = 0; i < (nLen/sizeof(struct LANGANDCODEPAGE)) && dwLocale == 0; i++)
		{
			for(int iLangs = 0; iLangs < SIZEOF_ARRAY(wLangs) && dwLocale == 0; ++iLangs)
			{
				if(lpTranslate->wLanguage != wLangs[iLangs]) continue;

				for(int iCodes = 0; iCodes < SIZEOF_ARRAY(wCodes); ++iCodes)
				{
					if(lpTranslate->wCodePage == wCodes[iCodes])
					{
						dwLocale = (wLangs[iLangs] << 16) + wCodes[iCodes];
						break;
					}
				}
			}
		}
	}
	if(dwLocale == 0)
		dwLocale = 0x040904b0; // English US, Unicode

	//---------------------------------
	TCHAR szStringFileInfo[129] = _T("\\StringFileInfo\\");
	if(_tcslen(szStringFileInfo) + _tcslen(pszField) + 9 >= SIZEOF_ARRAY(szStringFileInfo))
	{
		ATLASSERT(!_T("CDiag::GetLocaleVerInfo() - very long pszField string"));
		return E_INVALIDARG;
	}
	wsprintf(szStringFileInfo + _tcslen(szStringFileInfo), _T("%08x\\%s"), dwLocale, pszField);

	//---------------------------------
	LPTSTR pszVer = NULL;
	if(!::VerQueryValue(pbVerBlock, szStringFileInfo, (LPVOID*)&pszVer, &nLen))
		return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
	if(pszVer == NULL)
		return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);

	// nLen including a terminating null character
	if(nMaxLength > nLen)
		nMaxLength = nLen;

	memcpy(pszInfo, pszVer, (nMaxLength - 1) * sizeof(TCHAR));
	pszInfo[nMaxLength - 1] = (TCHAR)0;

	return (nMaxLength >= nLen)? S_OK : HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
}

// Syntax of string for conversion:
//		[Some spaces]D[D] divider1 D[D] divider2 [some symbols]
// where D is decimal digit, [divider1] = '.' or ',', [divider2] = '.' or ',' or '\0' 
inline bool CDiag::String2Dword(IN LPCTSTR psz, DWORD* pdw)
{
	ATLASSERT(psz != NULL);
	if(psz == NULL) return false;
	ATLASSERT(pdw != NULL);
	if(pdw == NULL) return false;

	*pdw = 0;

	//---------------------------------
	LPCTSTR p = psz;
	while(*p == (TCHAR)' ')
		++p;

	// Max WORD decimal value = 65535 : 5 symbols + '\0'
	TCHAR szNumber1[6] = _T("");
	TCHAR szNumber2[6] = _T("");

	bool bZero = (*p == (TCHAR)'0');
	int n = 0;
	while(*p == (TCHAR)'0')
		++p;
	while(n < SIZEOF_ARRAY(szNumber1) - 1 && (*p >= (TCHAR)'0' && *p <= (TCHAR)'9'))
		szNumber1[n++] = *p++;
	if(n == 0 && bZero)
		szNumber1[n++] = (TCHAR)'0';
	if(n == 0 || (*p != (TCHAR)'.' && *p != (TCHAR)','))
		return false;

	++p;
	szNumber1[n] = (TCHAR)0;

	bZero = (*p == (TCHAR)'0');
	n = 0;
	while(*p == (TCHAR)'0')
		++p;
	while(n < SIZEOF_ARRAY(szNumber2) - 1 && (*p >= (TCHAR)'0' && *p <= (TCHAR)'9'))
		szNumber2[n++] = *p++;
	if(n == 0 && bZero)
		szNumber2[n++] = (TCHAR)'0';
	if(n == 0 || (*p != (TCHAR)'.' && *p != (TCHAR)',' && *p != (TCHAR)0))
		return false;

	szNumber2[n] = (TCHAR)0;

	//---------------------------------
	int iVerH = _ttoi(szNumber1);
	if(iVerH > (int)MAXWORD)
		return false;

	int iVerL = _ttoi(szNumber2);
	if(iVerL > (int)MAXWORD)
		return false;

	*pdw = (DWORD)((iVerH << 16) + iVerL);
	return true;
}

//---------------------------------------------------------------------------
// If *pszFileName == 0, then pullSize is max file size for current directory

inline HRESULT CDiag::GetMaxFileSize(LPCTSTR pszFileName, ULONGLONG* pullSize)
{
	ATLASSERT(pullSize != NULL);
	if(pullSize == NULL) return E_POINTER;
	*pullSize = (ULONGLONG)0;

	ATLASSERT(pszFileName != NULL);
	if(pszFileName == NULL) return E_INVALIDARG;

	//---------------------------------
	TCHAR szRoot[_MAX_PATH] = _T("");
	HRESULT hr = ::ExtractDiskName(pszFileName, szRoot);
	if(FAILED(hr))
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error,
			_T("###Err### CDiag::GetMaxFileSize - ExtractDiskName( '%s' )=0x%X\n"),
			pszFileName, hr);

		ATLASSERT(!_T("CDiag::GetMaxFileSize - ExtractDiskName() FAILED"));
		return hr;
	}

	ULARGE_INTEGER uliAvaliable = {0};
	if(!::GetDiskFreeSpaceEx(szRoot, &uliAvaliable, NULL, NULL))
	{
		hr = ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### CDiag::GetMaxFileSize - ")
			_T("GetDiskFreeSpaceEx( '%s' )=0x%X\n"), szRoot, hr);
		return hr;
	}

	*pullSize = uliAvaliable.QuadPart;
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Versions of some base modules: (8.0, 8.1, 9.0)
//	{ _T("ddraw.dll"),   { 0x50001, 0x22580400 }, { 0x50001, 0x26000881 }, { 0x50003, 0x00000900 } },
//	{ _T("d3d8.dll"),    { 0x50001, 0x22580400 }, { 0x50001, 0x26000881 }, { 0x50003, 0x00000900 } },
//	{ _T("dsound.dll"),  { 0x50001, 0x22580400 }, { 0x50001, 0x26000881 }, { 0x50003, 0x00000900 } },
//	{ _T("devenum.dll"), { 0x60003, 0x00010400 }, { 0x60003, 0x00010881 }, { 0x60005, 0x00010900 } },
//	{ _T("qcap.dll"),    { 0x60003, 0x00010400 }, { 0x60003, 0x00010881 }, { 0x60005, 0x00010900 } },
//	{ _T("qdv.dll"),     { 0x60003, 0x00010400 }, { 0x60003, 0x00010881 }, { 0x60005, 0x00010900 } },
//	{ _T("qdvd.dll"),    { 0x60003, 0x00010400 }, { 0x60003, 0x00010881 }, { 0x60005, 0x00010900 } },
//	{ _T("quartz.dll"),  { 0x60003, 0x00010400 }, { 0x60003, 0x00010881 }, { 0x60005, 0x00010900 } }

class CDXDiag
{
public:
	static DWORD DXVersion(OUT LPTSTR pszVersion = NULL, IN DWORD nMaxSym = 0);
};

//---------------------------------
// Return DWORD with DirectX version from HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\DirectX\\Version
// String samples:
//	DirectX 9.0 = "4.09.0000.0900", DirectX 8.1 = "4.08.01.0881", DirectX 8.0 = "4.08.00.0400"
// Takes from string 2nd and 3rd fields and converts theirs to DWORD
//
// nMaxSym: size of pszVersion in symbols.
// nMaxSym includes a terminating null character.

inline DWORD CDXDiag::DXVersion(OUT LPTSTR pszVersion, IN DWORD nMaxSym)
{
	ATLASSERT((pszVersion != NULL && nMaxSym > 0) || (pszVersion == NULL && nMaxSym == 0));
	if((pszVersion != NULL && nMaxSym == 0) || (pszVersion == NULL && nMaxSym > 0)) return false;

	if(pszVersion != NULL)
		pszVersion[0] = (TCHAR)0;

	//---------------------------------
	CRegKey key;
	LONG lRet = key.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\DirectX"), KEY_READ);
	if(lRet != ERROR_SUCCESS)
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### CDXDiag::DXVersion(): ")
			_T("key.Open()=0x%X\n"), HRESULT_FROM_WIN32(lRet));
		return 0;
	}

	TCHAR szBuf[64] = _T("");
#if (_ATL_VER >= 0x0700)
	DWORD cbCount = sizeof(szBuf);
	lRet = key.QueryStringValue(_T("Version"), szBuf, &cbCount);
#else
	DWORD cbCount = SIZEOF_ARRAY(szBuf);
	lRet = key.QueryValue(szBuf, _T("Version"), &cbCount);
#endif
	if(lRet != ERROR_SUCCESS)
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### CDXDiag::DXVersion(): ")
			_T("key.QueryValue( Version )=0x%X\n"), HRESULT_FROM_WIN32(lRet));
		return 0;
	}

	szBuf[SIZEOF_ARRAY(szBuf) - 1] = (TCHAR)0;
	if(pszVersion != NULL)
	{
		lstrcpyn(pszVersion, szBuf, nMaxSym);
	}

	//---------------------------------
	// Parsing

	// Skip to '.' or '\0' (I don't know value of first field for old versions)
	int i = 0;
	while(szBuf[i] != (TCHAR)'.' && szBuf[i] != (TCHAR)0)
		++i;

	DWORD dwVersion = 0;
	if(szBuf[i] == (TCHAR)'.')
	{
		CDiag::String2Dword(&szBuf[i + 1], &dwVersion);
	}

	if(dwVersion == 0)
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### CDXDiag::DXVersion(): ")
			_T("can't parse version string '%s'\n"), szBuf);
	}

	return dwVersion;
}

/////////////////////////////////////////////////////////////////////////////
#endif	 // __DIAGNOSTICS_H_
