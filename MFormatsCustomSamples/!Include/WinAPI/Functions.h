//---------------------------------------------------------------------------
// This file is part of the MediaLooks DirectShow Template Library (DSTL)
//
// Functions.h : some extentions of WinAPI functions
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

#ifndef __FUNCTIONS_H_
#define __FUNCTIONS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _LPCBYTE_DEFINED
#define _LPCBYTE_DEFINED
typedef const BYTE *LPCBYTE;
#endif

#include <commdlg.h>
#include <Dlgs.h>
#define _USE_MATH_DEFINES
#include <math.h>


#include <Mmsystem.h>
#include "Macros.h"

typedef LONGLONG REFERENCE_TIME;

#pragma warning(push)
#pragma warning(disable:4302)

// For rect flip
#define FLIP_HORZ	(1)
#define FLIP_VERT	(2)
#define FLIP_BOTH	(3)

//---------------------------------
// ATTENTION !!!
// This macros are very dangerous (_alloca()). Don't call theirs in cycle !!!

#define RT2STR(t)			( RefTime2Str( (t), (LPTSTR)_alloca( 32*sizeof(TCHAR) )) )
#define DRT2STR(t)			( RefTime2Str( DBL2INT64(t), (LPTSTR)_alloca( 32*sizeof(TCHAR) )) )
#define N64STR(num64)		( N64ToStr((num64), (LPTSTR)_alloca( 32*sizeof(TCHAR) )) )
#define UN64STR(num64)		( UN64ToStr((num64), (LPTSTR)_alloca( 32*sizeof(TCHAR) )) )
#define BYTES2STR(num64)		( Bytes2Str((num64), (LPTSTR)_alloca( 32*sizeof(TCHAR) )) )
#define SEC2STR(t)			( Second2Str( (t), (LPTSTR)_alloca( 32*sizeof(TCHAR) )) )
#define ST2STR(time)			( SysTime2Str((time), (LPTSTR)_alloca( 64*sizeof(TCHAR) ), false) )
#define ST2STR_S(time)		( SysTime2Str((time), (LPTSTR)_alloca( 64*sizeof(TCHAR) ), true) )
#define FT2STR(time)			( FileTime2Str((time), (LPTSTR)_alloca( 64*sizeof(TCHAR) ), false) )
#define FT2STR_S(time)		( FileTime2Str((time), (LPTSTR)_alloca( 64*sizeof(TCHAR) ), true) )


/////////////////////////////////////////////////////////////////////////////
LONG WINAPI DSInterlockedCompareExchange(IN OUT PLONG Destination, IN LONG Exchange, IN LONG Comparand);

LPCTSTR __stdcall RefTime2Str(REFERENCE_TIME t, LPTSTR pszConv);
LPCTSTR __stdcall Second2Str(DWORD dwSeconds, LPTSTR pszConv);
LPCTSTR __stdcall Bytes2Str(LONGLONG llBytes, LPTSTR pszConv);
LPCTSTR __stdcall SysTime2Str(const SYSTEMTIME& _crSysTime, LPTSTR pszConv, bool _bShort = false);
LPCTSTR __stdcall FileTime2Str(const LONGLONG& _crFileTime, LPTSTR pszConv, bool _bShort = false);
LPCTSTR __stdcall N64ToStr(LONGLONG llNumber, LPTSTR pszConv);
LPCTSTR __stdcall UN64ToStr(ULONGLONG ullNumber, LPTSTR pszConv);


HRESULT __stdcall GetWin32Error(DWORD dwError = ::GetLastError());

LPCTSTR __stdcall Guid2Str(REFGUID rguid, LPTSTR pszName);
HRESULT __stdcall ClsidToModuleName(IN REFCLSID rclsid, OUT LPTSTR pszName, IN DWORD nMaxSym);

HRESULT __stdcall ExtractDiskName(IN LPCTSTR pszFileName, IN OUT LPTSTR pszDiskName);

const char* __stdcall FindFileNameA(IN const char* pszPath);
const WCHAR* __stdcall FindFileNameW(IN const WCHAR* pwszPath);
#ifdef UNICODE
#define FindFileName		FindFileNameW
#else
#define FindFileName		FindFileNameA
#endif // !UNICODE
#define	ExtractFileName		FindFileName

const char* __stdcall FindFileExtensionA(const char* pszPath);
const WCHAR* __stdcall FindFileExtensionW(const WCHAR* pwszPath);
#ifdef UNICODE
#define FindFileExtension	FindFileExtensionW
#else
#define FindFileExtension	FindFileExtensionA
#endif // !UNICODE

LPCTSTR __stdcall ChangeFileExtention(IN LPTSTR pszName, IN LPCTSTR pszExt);
bool	__stdcall GetModuleFolder(IN HINSTANCE hModule, IN OUT LPTSTR pszFolder);

BOOL	__stdcall ShowFolderDialog(  HWND _hWndParent, LPTSTR _szFolderPath, LPTSTR _szTitle = NULL );
// Check existence of file or folder 
bool	__stdcall IsFileExist( LPCTSTR _szFileName );
// Create nested folder 
HRESULT __stdcall CreateFolder(LPCTSTR pszFolder);

bool	__stdcall GetExePath(LPTSTR pszExePath);
bool	__stdcall ConstructPath(LPCTSTR pszDefPath, LPTSTR pszFile);
bool	__stdcall IsCorrectPath(LPCTSTR pszPath);
HRESULT __stdcall CreateFolder(LPCTSTR pszFolder);

DWORD __stdcall GetOneBit(const BYTE* pbSrc, DWORD nSrcBitOffset, BYTE* pnBit, DWORD cbSrc = MAXDWORD/8);
DWORD __stdcall GetOneBit(const BYTE* pbSrc, DWORD nSrcBitOffset, DWORD* pnBit, DWORD cbSrc = MAXDWORD/8);
DWORD __stdcall PutOneBit(BYTE bBit, BYTE* pbDst, DWORD nDstBitOffset, DWORD cbDst = MAXDWORD/8);
DWORD __stdcall PutOneBit(DWORD dwBit, BYTE* pbDst, DWORD nDstBitOffset, DWORD cbDst = MAXDWORD/8);
DWORD __stdcall GetBits(const BYTE* pbSrc, DWORD nSrcBitOffset, DWORD nGetBits, DWORD* pnValue, DWORD cbSrc = MAXDWORD/8);
DWORD __stdcall PutBits(DWORD dwSrc, DWORD nSrcBits, BYTE* pbDst, DWORD nDstBitOffset, DWORD cbDst = MAXDWORD/8);
DWORD __stdcall GetBitSeq(const BYTE* pbSrc, DWORD nSrcBitOffset, BYTE* pbDst, DWORD nGetBits, DWORD cbSrc = MAXDWORD/8);
DWORD __stdcall PutBitSeq(const BYTE* pbSrc, DWORD nPutBits, BYTE* pbDst, DWORD nDstBitOffset, DWORD cbDst = MAXDWORD/8);

WORD	__fastcall SwapW(WORD w);
DWORD	__fastcall Swap24(DWORD dw);
DWORD	__fastcall SwapDW(DWORD dw);
// Commented specially for check code which uses it once more
// ULONGLONG __stdcall SwapI64(ULONGLONG ull);
ULONGLONG __fastcall SwapI64Full(ULONGLONG ull);
void	__stdcall ReverseBytes(LPBYTE _pbData, DWORD _cbData);

// Return Greater Common Divider
LONGLONG __stdcall MLGCD( LONGLONG nFirst, LONGLONG nSecond );

bool	__stdcall GetProcFreq(DWORD* pdwFreq, bool bCalculateOnly = false);
//int		__stdcall IsVideoMemory(LPBYTE pbTest, DWORD cbTestMax, LPBYTE pbRestore = NULL);
bool	__stdcall IsSlowReadMemory(void* pMemory, DWORD cbSize = 0x47F);
bool	__stdcall IsZeroMemory(void* pMemory, DWORD cbSize);
template <class T>
bool	__stdcall IsZeroObject(T* pObject);

DWORD	__stdcall PutStr(LPTSTR psz, LPCTSTR pszPut);
DWORD	__stdcall AddStr(LPTSTR psz, LPCTSTR pszAdd);
LPWSTR	__stdcall CreateWchName(LPCTSTR pszName, size_t nMaxSym = 0);

LPCTSTR	__stdcall LoadResString(UINT uIdString, LPTSTR pszString, ULONG cSize);

HRESULT __stdcall GetModuleVerInfo(VS_FIXEDFILEINFO* pFI, HMODULE hModule = NULL);

LPWSTR	__stdcall StrReplaceW(LPWSTR _pszString, LPCWSTR _pszFind, LPCWSTR _pszReplace, int nSizeMax = -1 );
LPSTR	__stdcall StrReplaceA(LPSTR _pszString, LPCSTR _pszFind, LPCSTR _pszReplace, int nSizeMax = -1 );

LONGLONG __stdcall GetUTCTime();
LONGLONG __stdcall GetLocalTime();

#ifdef UNICODE
#define StrReplace StrReplaceW
#else
#define StrReplace StrReplaceW
#endif


inline LPCTSTR __stdcall RefTime2Str(REFERENCE_TIME t, LPTSTR pszConv)
{
	ATLASSERT(pszConv != NULL);
	if(pszConv == NULL) return NULL;

	if( t == MINLONGLONG || t == MAXLONGLONG )
	{
		wsprintf( pszConv, t == MINLONGLONG ? _T("-") : _T("*") );
		return pszConv;
	}

	REFERENCE_TIME tHigh = t/DS_MSEC;
	LONG lUnit = (LONG)(t - MS2U(tHigh));
	if(lUnit < 0)
		lUnit = -lUnit;

	REFERENCE_TIME tSec = tHigh/(REFERENCE_TIME)1000;
	LONG lMSec = (LONG)(tHigh - tSec * (REFERENCE_TIME)1000);
	if(lMSec < 0)
		lMSec = -lMSec;

	LONG lSecHigh = (LONG)(tSec/(REFERENCE_TIME)1000000000L);
	LONG lSecLow = (LONG)(tSec - (REFERENCE_TIME)lSecHigh * (REFERENCE_TIME)1000000000L);

	if(lSecHigh == 0)
	{
		bool bMinus = (t < (REFERENCE_TIME)0) && (lSecLow == 0);
		if(lUnit != 0)
			wsprintf(pszConv, (bMinus)? _T("-%u.%03u'%04u") : _T("%d.%03u'%04u"), lSecLow, lMSec, lUnit);
		else
			wsprintf(pszConv, (bMinus)? _T("-%u.%03u") : _T("%d.%03u"), lSecLow, lMSec);
	}
	else
	{
		if(lSecLow < 0)
			lSecLow = -lSecLow;
		if(lUnit != 0)
			wsprintf(pszConv, _T("%d%09u.%03u'%04u"), lSecHigh, lSecLow, lMSec, lUnit);
		else
			wsprintf(pszConv, _T("%d%09u.%03u"), lSecHigh, lSecLow, lMSec);
	}
	return pszConv;
}

inline LPCTSTR __stdcall Second2Str(DWORD dwSeconds, LPTSTR pszConv)
{
	ATLASSERT(pszConv != NULL);
	if(pszConv == NULL) return NULL;

	wsprintf(pszConv, _T("%02u:%02u:%02u"), dwSeconds / 3600, (dwSeconds % 3600) / 60, dwSeconds % 60);
	return pszConv;
}

inline LPCTSTR __stdcall SysTime2Str(const SYSTEMTIME& _crSysTime, LPTSTR pszConv, bool _bShort /*= false*/)
{
	ATLASSERT(pszConv != NULL);
	if(pszConv == NULL) return NULL;

	if( _bShort || _crSysTime.wYear == 0 )
	{
		if( _crSysTime.wMilliseconds )
			wsprintf( pszConv, _T("%02d:%02d:%02d.%03d"),
			_crSysTime.wHour, _crSysTime.wMinute, _crSysTime.wSecond, _crSysTime.wMilliseconds );
		else
			wsprintf( pszConv, _T("%02d:%02d:%02d"),
			_crSysTime.wHour, _crSysTime.wMinute, _crSysTime.wSecond );
	}
	else
	{
		if( _crSysTime.wMilliseconds )
			wsprintf( pszConv, _T("%04d-%02d-%02d %02d:%02d:%02d.%03d"),
			_crSysTime.wYear, _crSysTime.wMonth, _crSysTime.wDay, 
			_crSysTime.wHour, _crSysTime.wMinute, _crSysTime.wSecond, _crSysTime.wMilliseconds );
		else
			wsprintf( pszConv, _T("%04d-%02d-%02d %02d:%02d:%02d"),
			_crSysTime.wYear, _crSysTime.wMonth, _crSysTime.wDay, 
			_crSysTime.wHour, _crSysTime.wMinute, _crSysTime.wSecond );
	}

	return pszConv;
}


inline LPCTSTR __stdcall FileTime2Str(const LONGLONG& _crFileTime, LPTSTR pszConv, bool _bShort /*= false*/)
{
	SYSTEMTIME sysTime = {};
	FileTimeToSystemTime((FILETIME*)&_crFileTime, &sysTime);

	return SysTime2Str(sysTime, pszConv, _bShort);
}


inline LPCTSTR __stdcall N64ToStr(LONGLONG llNumber, LPTSTR pszConv)
{
	ATLASSERT(pszConv != NULL);
	if(pszConv == NULL) return NULL;

	LPTSTR psz = pszConv;
	if(llNumber < 0)
	{
		// Make positive number
		if( llNumber == MINLONGLONG )
		{
			return _T("MINLONGLONG");
		}

		llNumber = -llNumber;
		// Put minus sign to buffer
		*psz = (TCHAR)'-';
		++psz;
	}

	// Print digits (grouped by 3 digit)
	bool bFirst = true;

	// Max LONGLONG:	9 223 372 036 854 775 807
	// Maximum divider: 1 000 000 000 000 000 000
	LONGLONG llVeryBig = (LONGLONG)1000000000000000000;
	while(llVeryBig != (LONGLONG)0)
	{
		DWORD dwQuotient = (DWORD)( llNumber/llVeryBig );
		if(!bFirst)
		{
			psz += wsprintf(psz, _T(" %03u"), dwQuotient);
		}
		else if(dwQuotient != 0 || llVeryBig == (ULONGLONG)1)
		{
			psz += wsprintf(psz, _T("%u"), dwQuotient);
			bFirst = false;
		}

		llNumber %= llVeryBig;
		llVeryBig /= (LONGLONG)1000;
	}

	return pszConv;
}

inline LPCTSTR __stdcall UN64ToStr(ULONGLONG ullNumber, LPTSTR pszConv)
{
	ATLASSERT(pszConv != NULL);
	if(pszConv == NULL) return NULL;

	LPTSTR psz = pszConv;

	// Print digits (grouped by 3 digit)
	bool bFirst = true;

	// Max UINT64:		18 446 744 073 709 551 615 
	// Maximum divider:  1 000 000 000 000 000 000
	ULONGLONG ullVeryBig = (ULONGLONG)1000000000000000000;
	while(ullVeryBig != (ULONGLONG)0)
	{
		DWORD dwQuotient = (DWORD)( ullNumber/ullVeryBig );
		if(!bFirst)
		{
			psz += wsprintf(psz, _T(" %03u"), dwQuotient);
		}
		else if(dwQuotient != 0 || ullVeryBig == (ULONGLONG)1)
		{
			psz += wsprintf(psz, _T("%u"), dwQuotient);
			bFirst = false;
		}

		ullNumber %= ullVeryBig;
		ullVeryBig /= (ULONGLONG)1000;
	}

	return pszConv;
}

// For prepare string with numbers of Bytes/KB/MB/GB/TB
inline LPCTSTR __stdcall Bytes2Str(LONGLONG llBytes, LPTSTR pszConv)
{
	ATLASSERT(pszConv != NULL);
	if(pszConv == NULL) return NULL;

	if(llBytes < (LONGLONG)1024)
	{
		wsprintf(pszConv, _T("%u Bytes"), (DWORD)llBytes);
		return pszConv;
	}

	//---------------------------------
	static LPCTSTR pszFormat[] = { _T(" KB"), _T(" MB"), _T(" GB"), _T(" TB") };

	int i = 0;
	for(; i < SIZEOF_ARRAY(pszFormat) && llBytes >= (LONGLONG)1024 * 1024; ++i)
	{
		llBytes = (llBytes + (LONGLONG)512)/(LONGLONG)1024;
	}

	LONGLONG llH = llBytes/(LONGLONG)1024;
	if(i == SIZEOF_ARRAY(pszFormat) - 1)
		N64ToStr(llH, pszConv);
	else
		wsprintf(pszConv, _T("%u"), (DWORD)llH);

	llBytes -= llH * (LONGLONG)1024;
	DWORD nRest = (DWORD)( (llBytes * (LONGLONG)1000 + (LONGLONG)512)/(LONGLONG)1024 );
	if(nRest > 0)
		wsprintf(pszConv + lstrlen(pszConv), _T(".%03u"), nRest);

	return lstrcat(pszConv, pszFormat[i]);
}


inline LONGLONG __stdcall GetUTCTime()
{
	LONGLONG llRes = 0;
	GetSystemTimeAsFileTime( (FILETIME*)&llRes );
	return llRes;
}

inline LONGLONG __stdcall GetLocalTime()
{
	FILETIME fTime = {0};
	GetSystemTimeAsFileTime( &fTime );
	
	LONGLONG llRes = 0;
	FileTimeToLocalFileTime( &fTime, (FILETIME*)&llRes );

	return llRes;
}

// Convert ms since 1/1/1970 to 100ns since 1/1/1601;
// https://stackoverflow.com/questions/42328549/c-set-windows-time-from-ms-since-1-1-1970
inline LONGLONG UnixTime2WindowsTime(LONGLONG msecSinceEpoch) 
{
	LONGLONG result = msecSinceEpoch * DS_MSEC; // ms => 100ns
	result += 116444736000000000LL;    // Unix epoch in FILETIME
	return result;
}

inline LONGLONG WindowsTime2UnixTime(LONGLONG _rtFileTime) 
{
	LONGLONG result = _rtFileTime - 116444736000000000LL; // Unix epoch from FILETIME
	// ms <= 100ns
	return result / DS_MSEC;
}

// DWORD WaitWithMessageLoop(_In_ HANDLE hEvent, DWORD _dwWait = INFINITE )
// {
// 	MSG msg = {};
// 	DWORD dwRet = WAIT_OBJECT_0;
// 	
// 	while(1)
// 	{
// 		int nTime = GetTickCount();
// 		dwRet = MsgWaitForMultipleObjectsEx(1, &hEvent, _dwWait, QS_ALLINPUT, MWMO_INPUTAVAILABLE);
// 		
// 		if (dwRet == WAIT_OBJECT_0)
// 			return TRUE;    // The event was signaled
// 
// 		if (dwRet != WAIT_OBJECT_0 + 1)
// 			break;          // Something else happened
// 
// 		// There is one or more window message available. Dispatch them
// 		while(PeekMessage(&msg,0,0,0,PM_NOREMOVE))
// 		{
// 			// check for unicode window so we call the appropriate functions
// 			BOOL bUnicode = ::IsWindowUnicode(msg.hwnd);
// 			BOOL bRet;
// 
// 			if (bUnicode)
// 				bRet = ::GetMessageW(&msg, NULL, 0, 0);
// 			else
// 				bRet = ::GetMessageA(&msg, NULL, 0, 0);
// 
// 			if (bRet > 0)
// 			{
// 				::TranslateMessage(&msg);
// 
// 				if (bUnicode)
// 					::DispatchMessageW(&msg);
// 				else
// 					::DispatchMessageA(&msg);
// 			}
// 
// 			if (WaitForSingleObject(hEvent, 0) == WAIT_OBJECT_0)
// 				return TRUE; // Event is now signaled.
// 		}
// 
// 		nTime = GetTickCount() - nTime;
// 
// 		if( _dwWait != INFINITE )
// 		{
// 			_dwWait -= MIN( nTime, _dwWait );
// 
// 			if( _dwWait == 0 )
// 			{
// 				dwRet = WAIT_TIMEOUT;
// 				break;
// 			}
// 		}
// 	}
// 	return dwRet;
// }

inline bool InQuotes( LPCWSTR _pszText, BSTR* _pbsText = NULL, WCHAR _wchQuotes = 0 )
{
	if( !_pszText || !_pszText[0] )
		return false;

	int nLen = lstrlenW( _pszText );
	
	if( _pszText[0] != _pszText[nLen-1] ||
		(_wchQuotes && _pszText[0] != _wchQuotes ) ||
		( !_wchQuotes && (_pszText[0] != L'\'' && _pszText[0] != L'"') ) )
	{
		if( _pbsText )
			*_pbsText = CComBSTR( _pszText ).Detach();

		return false;
	}
	
	if( _pbsText )
	{
		CComBSTR cbsText( nLen - 2, _pszText + 1 );
		*_pbsText = cbsText.Detach();
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
// StrReplace
inline bool __stdcall IsZeroMemory(void* pMemory, DWORD cbSize)
{
	for( int i = 0; i < (int)cbSize; i++ )
	{
		if( *((LPBYTE)pMemory + i) != 0 ) return false;
	}

	return true;
}
template <class T> inline bool __stdcall  IsZeroObject(T* pObject)
{
	return IsZeroMemory( pObject, sizeof(T) );
}


//////////////////////////////////////////////////////////////////////////
// StrReplace
// 

inline LPWSTR __stdcall StrReplaceW(LPWSTR _pszString, LPCWSTR _pszFind, LPCWSTR _pszReplace, int nSizeMax /*= -1*/)
{
	if( STR_IS_EMPTY(_pszString) || STR_IS_EMPTY(_pszFind) )
		return _pszString;

	int nFind = lstrlenW( _pszFind );
	int nReplace = _pszReplace ? lstrlenW( _pszReplace ) : 0;

	ATLASSERT( nFind > 0 );

	if( nSizeMax < 0 )
	{
		nSizeMax = lstrlenW( _pszString ) + 1;
		ATLASSERT( nReplace <= nFind );
		if( nReplace > nFind )
			return _pszString;
	}
#ifdef _DEBUG
	else
	{
		int nLen = lstrlenW( _pszString );
		ATLASSERT( nSizeMax > MAX( nLen, (nLen / nFind) * nReplace + nLen % nFind ) );
	}
#endif

	WCHAR* szTemp = (WCHAR*)alloca( sizeof(WCHAR) * nSizeMax );
	::ZeroMemory( szTemp, nSizeMax * sizeof(WCHAR) );
	
	int nPosDst = 0;
	int nPosSrc = 0;
	WCHAR* pFind = NULL;
	while( (pFind = StrStrIW(_pszString + nPosSrc, _pszFind)) != NULL )
	{
		int nCopy = (int)(pFind - (_pszString + nPosSrc));
		lstrcpynW( szTemp + nPosDst, _pszString + nPosSrc, nCopy + 1);
		if( _pszReplace )
			lstrcatW( szTemp, _pszReplace );
		nPosDst += nCopy + nReplace;
		nPosSrc += nCopy + nFind;
	}
	lstrcatW( szTemp, _pszString + nPosSrc );
	lstrcpyW( _pszString, szTemp );
	return _pszString;
}

inline LPSTR __stdcall StrReplaceA(LPSTR _pszString, LPCSTR _pszFind, LPCSTR _pszReplace, int nSizeMax /*= -1*/ )
{
	if( STR_IS_EMPTY(_pszString) || STR_IS_EMPTY(_pszFind) )
		return _pszString;

	int nFind = lstrlenA( _pszFind );
	int nReplace = _pszReplace ? lstrlenA( _pszReplace ) : 0;

	ATLASSERT( nFind > 0 );

	if( nSizeMax < 0 )
	{
		nSizeMax = lstrlenA( _pszString ) + 1;
		ATLASSERT( nReplace <= nFind );
		if( nReplace > nFind )
			return _pszString;
	}
#ifdef _DEBUG
	else
	{
		int nLen = lstrlenA( _pszString );
		ATLASSERT( nSizeMax > MAX( nLen, (nLen / nFind) * nReplace + nLen % nFind ) );
	}
#endif

	CHAR* szTemp = (CHAR*)alloca( sizeof(CHAR) * nSizeMax );
	::ZeroMemory( szTemp, nSizeMax * sizeof(CHAR) );
	
	int nPosDst = 0;
	int nPosSrc = 0;
	CHAR* pFind = NULL;
	while( (pFind = StrStrIA(_pszString + nPosSrc, _pszFind)) != NULL )
	{
		int nCopy = (int)(pFind - (_pszString + nPosSrc));
		lstrcpynA( szTemp + nPosDst, _pszString + nPosSrc, nCopy + 1);
		if( _pszReplace )
			lstrcatA( szTemp, _pszReplace );
		nPosDst += nCopy + nReplace;
		nPosSrc += nCopy + nFind;
	}
	lstrcatA( szTemp, _pszString + nPosSrc );
	lstrcpyA( _pszString, szTemp );
	return _pszString;
}

inline bool IsProtocolW( LPCWSTR _pszURL, LPCWSTR _pszProtocol, BSTR* _pbsProtocol = NULL, BSTR* _pbsPath = NULL )
{
	// rtsp://
	if( !_pszURL ) 
		return false;

	LPCWSTR pszSep = StrChrW( _pszURL, L':' );
	if( !pszSep || !pszSep[0] || pszSep[1] != L'/' || pszSep[2] != L'/' )
		return false;

	CComBSTR cbsProtocolName( (int)(pszSep - _pszURL), _pszURL );
	::CharLowerW( cbsProtocolName );

	// Compare with specified
	if( STR_IS_EMPTY(_pszProtocol ) || STR_IS_SAME_I( _pszProtocol, cbsProtocolName ) )
	{
		if (_pbsProtocol)
			*_pbsProtocol = cbsProtocolName.Detach();

		if (_pbsPath)
			*_pbsPath = ::SysAllocString(pszSep + 3);

		return true;
	}

	return false;
}

inline bool IsProtocolA( LPCSTR _pszURL, LPCSTR _pszProtocol, BSTR* _pbsProtocol = NULL, BSTR* _pbsPath = NULL )
{
	// rtsp://
	if( !_pszURL ) 
		return false;

	LPCSTR pszSep = StrChrA( _pszURL, ':' );
	if( !pszSep || !pszSep[0] || pszSep[1] != '/' || pszSep[2] != '/' )
		return false;

	USES_CONVERSION;

	CComBSTR cbsProtocolName((int)(pszSep - _pszURL), _pszURL);
	::CharLowerW(cbsProtocolName);

	// Compare with specified
	if (STR_IS_EMPTY(_pszProtocol) || STR_IS_SAME_I(A2CW(_pszProtocol), cbsProtocolName))
	{
		if (_pbsProtocol)
			*_pbsProtocol = cbsProtocolName.Detach();

		if (_pbsPath)
			*_pbsPath = CComBSTR(pszSep + 3).Detach();

		return true;
	}

	return false;
}

// WARNING !!! Return -1 if not found
inline int IsProtocolsA(LPCSTR _pszURL, LPCSTR* _ppszProtocols, BSTR* _pbsProtocol = NULL, BSTR* _pbsPath = NULL)
{
	int nIndex = 0;
	LPCSTR* ppszProtocol = _ppszProtocols;
	while (*ppszProtocol)
	{
		if (IsProtocolA(_pszURL, *ppszProtocol, _pbsProtocol, _pbsPath))
			return nIndex;

		nIndex++;
		ppszProtocol++;
	}

	return -1;
}

// WARNING !!! Return -1 if not found
inline int IsProtocolsW(LPCWSTR _pszURL, LPCWSTR* _ppszProtocols, BSTR* _pbsProtocol = NULL, BSTR* _pbsPath = NULL)
{
	int nIndex = 0;
	LPCWSTR* ppszProtocol = _ppszProtocols;
	while (*ppszProtocol)
	{
		if (IsProtocolW(_pszURL, *ppszProtocol, _pbsProtocol, _pbsPath))
			return nIndex;

		nIndex++;
		ppszProtocol++;
	}

	return -1;
}

inline static CComBSTR Quotes_Clear(LPCWSTR _pszStrWithQuotes, bool _bSymmetric = false)
{
	CComBSTR cbsRes = _pszStrWithQuotes;

	if (STR_IS_EMPTY(_pszStrWithQuotes))
		return cbsRes;

	LPWSTR pszRes = cbsRes.m_str;
	if (pszRes[0] == L'\'' || pszRes[0] == L'"')
	{
		++pszRes;

		LPWSTR pszEnd = pszRes + lstrlenW(pszRes);
		--pszEnd;
		if (_bSymmetric)
		{
			if (pszEnd[0] == _pszStrWithQuotes[0])
				pszEnd[0] = 0;
		}
		else
		{
			if (pszEnd[0] == L'\'' || pszEnd[0] == L'"')
				pszEnd[0] = 0;
		}
	}

	return CComBSTR(pszRes);
}

//////////////////////////////////////////////////////////////////////////
// InterlockedCompareExchange impl. ( only for work with old SDK )

// #pragma warning( push )
// #pragma warning( disable : 4035 )
// inline LONG WINAPI DSInterlockedCompareExchange(IN OUT PLONG  Destination,
// 												IN LONG Exchange, IN LONG Comparand)
// {
// 	_asm
// 	{
// 		mov         ecx, Destination
// 		mov         edx, Exchange
// 		mov         eax, Comparand
// 		lock cmpxchg dword ptr [ecx], edx
// 	}
// }
// #pragma warning( pop )

/////////////////////////////////////////////////////////////////////////////
// GetWin32Error - converts WIN32 error code to HRESULT

inline HRESULT __stdcall GetWin32Error(DWORD dwError /*= ::GetLastError()*/)
{
	HRESULT hr = HRESULT_FROM_WIN32(dwError);
	ATLASSERT(FAILED(hr));
	return (FAILED(hr))? hr : E_FAIL;
}

/////////////////////////////////////////////////////////////////////////////
// lstrcpynW doesn't work on Win95 and Win98
// Extends set of functions from <atlconv.h>

#if defined(_UNICODE)
	// in these cases the default (TCHAR) is the same as OLECHAR
	inline OLECHAR* ocscpyn(LPOLESTR dst, LPCOLESTR src, int nMax)
	{
		return lstrcpynW(dst, src, nMax);
	}
#elif defined(OLE2ANSI)
	// in these cases the default (TCHAR) is the same as OLECHAR
	inline OLECHAR* ocscpyn(LPOLESTR dst, LPCOLESTR src, int nMax)
	{
		return lstrcpyn(dst, src, nMax);
	}
#else
	inline OLECHAR* ocscpyn(LPOLESTR dst, LPCOLESTR src, int nMax)
	{
		if(nMax > 0)
		{
			int n =  lstrlenW(src);
			if(n > 0)
			{
				if(n >= nMax)
					n = nMax - 1;
				memcpy(dst, src, n * sizeof(WCHAR));
			}
			dst[n] = (WCHAR)0;
		}
		return dst;
	}
#endif

/////////////////////////////////////////////////////////////////////////////
// GUIDs conversions

inline LPCTSTR __stdcall Guid2Str(REFGUID rguid, LPTSTR pszName)
{
	ATLASSERT(pszName != NULL);
	if(pszName != NULL)
	{
		// ATTENTION : The string includes enclosing braces !
		OLECHAR szBuf[40];
		szBuf[0] = 0;
		StringFromGUID2(rguid, szBuf, 40);

		USES_CONVERSION;
		lstrcpyn(pszName, OLE2T(szBuf), 40);
	}
	return pszName;
}

#define GUID2STR(rguid) CComBSTR(rguid).m_str //L""	//(Guid2Str( (rguid), (LPTSTR)_alloca(128*sizeof(TCHAR)) ))

//---------------------------------
// nMaxSym: maximal numbers of symbols in pszName, includes a terminating null character
inline HRESULT __stdcall ClsidToModuleName(IN REFCLSID rclsid, OUT LPTSTR pszName, IN DWORD nMaxSym)
{
	ATLASSERT(pszName != NULL);
	if(pszName == NULL) return E_INVALIDARG;
	ATLASSERT(nMaxSym > 1);
	if(nMaxSym <= 1) return E_INVALIDARG;

	pszName[0] = (TCHAR)0;

	//---------------------------------
	TCHAR szRegKey[128] = _T("CLSID\\");
	Guid2Str(rclsid, szRegKey + lstrlen(szRegKey));
	lstrcat(szRegKey, _T("\\InprocServer32"));

	CRegKey key;
	LONG lErr = key.Open(HKEY_CLASSES_ROOT, szRegKey, KEY_READ);
	if(lErr == ERROR_SUCCESS)
	{
#if (_ATL_VER < 0x0700)
		DWORD dwCount = nMaxSym * sizeof(TCHAR);
		lErr = key.QueryValue(pszName, NULL, &dwCount);
#else
		DWORD dwCount = nMaxSym;
		lErr = key.QueryStringValue(NULL, pszName, &dwCount);
#endif //(_ATL_VER < 0x0700)
	}

	//---------------------------------
	HRESULT hr = S_OK;
	if(lErr != ERROR_SUCCESS)
	{
		pszName[0] = (TCHAR)0;

		if(lErr == ERROR_FILE_NOT_FOUND)
			hr = REGDB_E_CLASSNOTREG;
		else if(lErr == ERROR_MORE_DATA)
		{
			ATLASSERT(!_T("ClsidToModuleName() : buffer is not large enough"));
			hr = E_UNEXPECTED;
		}
		else
			hr = HRESULT_FROM_WIN32(lErr);
	}
	return hr;
}

// Size of pszDiskName should be at least _MAX_PATH symbols
// If *pszFileName == 0, then extracts disk name for current directory
inline HRESULT __stdcall ExtractDiskName(IN LPCTSTR pszFileName, IN OUT LPTSTR pszDiskName)
{
	ATLASSERT(pszFileName != NULL);
	if(pszFileName == NULL) return E_INVALIDARG;
	ATLASSERT(pszDiskName != NULL);
	if(pszDiskName == NULL) return E_POINTER;

	*pszDiskName = (TCHAR)0;

	//---------------------------------
	TCHAR szRoot[_MAX_PATH];
	lstrcpyn(szRoot, pszFileName, SIZEOF_ARRAY(szRoot));

	if(lstrlen(szRoot) >= 2)
	{
		if(szRoot[0] == (TCHAR)'\\' && szRoot[1] == (TCHAR)'\\')
		{
			// We have network path (UNC)
			// Skip computer name
			int n = 2;
			while(szRoot[n] != (TCHAR)0 && szRoot[n] != (TCHAR)'\\' && szRoot[n] != (TCHAR)'/') ++n;

			if(szRoot[n] == (TCHAR)0 || szRoot[n + 1] == (TCHAR)0)
				return HRESULT_FROM_WIN32(ERROR_PATH_NOT_FOUND);

			// Change if szRoot[n] == '/'
			szRoot[n++] = (TCHAR)'\\';

			// Skip name of shared disk
			while(szRoot[n] != (TCHAR)0 && szRoot[n] != (TCHAR)'\\' && szRoot[n] != (TCHAR)'/') ++n;

			if(n > SIZEOF_ARRAY(szRoot) - 2)
				return HRESULT_FROM_WIN32(ERROR_BAD_NETPATH);

			// Add "\" after name of shared disk
			szRoot[n] = (TCHAR)'\\';
			szRoot[n + 1] = (TCHAR)0;

			lstrcpy(pszDiskName, szRoot);
			return S_OK;
		}

		if(szRoot[1] == (TCHAR)':')
		{
			// We have local absolute path
			szRoot[2] = (TCHAR)'\\';
			szRoot[3] = (TCHAR)0;

			lstrcpy(pszDiskName, szRoot);
			return S_OK;
		}
	}

	//---------------------------------
	// Assume we have relative path
	if(::GetCurrentDirectory(SIZEOF_ARRAY(szRoot), szRoot) == 0)
		return ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
	if(szRoot[1] != (TCHAR)':')
		return E_UNEXPECTED;

	szRoot[2] = (TCHAR)'\\';
	szRoot[3] = (TCHAR)0;
	lstrcpy(pszDiskName, szRoot);
	return S_OK;
}

// Return position of file name in full path
inline const char* __stdcall FindFileNameA(IN const char* pszPath)
{
	LPCSTR pszReturn = pszPath;
	if(pszPath != NULL && pszPath[0] != 0)
	{
		const char* psz = pszPath + lstrlenA(pszPath) - 1;
		while(psz != pszPath)
		{
			if(*psz == '/' || *psz == '\\' || *psz == ':')
				return psz + 1;

			// For not return just port number (e.g "udp://127.0.0.1:5000"), but for files e.g. C:somefile.exe -> the somefile.exe would be returned
			if (*psz == ':')
				pszReturn = psz + 1;

			--psz;
		}
	}
	return pszReturn;
}
inline const WCHAR* __stdcall FindFileNameW(IN const WCHAR* pwszPath)
{
	LPCWSTR pszReturn = pwszPath;
	if(pwszPath != NULL && pwszPath[0] != 0)
	{
		const WCHAR* pwsz = pwszPath + lstrlenW(pwszPath) - 1;
		while(pwsz != pwszPath)
		{
			if(*pwsz == (WCHAR)'/' || *pwsz == (WCHAR)'\\' )
				return pwsz + 1;

			// For not return just port number (e.g "udp://127.0.0.1:5000"), but for files e.g. C:somefile.exe -> the somefile.exe would be returned
			if (*pwsz == (WCHAR)':')
				pszReturn = pwsz + 1;

			--pwsz;
		}
	}
	return pszReturn;
}

// Return position of '.' before file extention in full path or position of ending zero
inline const char* __stdcall FindFileExtensionA(const char* pszPath)
{
	if(pszPath == NULL || pszPath[0] == 0)
		return pszPath;

	DWORD nSym = lstrlenA(pszPath);
	const char* psz = pszPath + nSym - 1;
	while(psz != pszPath)
	{
		if(*psz == '.')
			return psz;

		if(*psz == '/' || *psz == '\\' || *psz == ':')
			break;

		--psz;
	}

	// Extension not found: return position of ending zero
	return pszPath + nSym;
}
inline const WCHAR* __stdcall FindFileExtensionW(const WCHAR* pwszPath)
{
	if(pwszPath == NULL || pwszPath[0] == 0)
		return pwszPath;

	DWORD nSym = lstrlenW(pwszPath);
	const WCHAR* pwsz = pwszPath + nSym - 1;
	while(pwsz != pwszPath)
	{
		if(*pwsz == (WCHAR)'.')
			return pwsz;

		if(*pwsz == (WCHAR)'/' || *pwsz == (WCHAR)'\\' || *pwsz == (WCHAR)':')
			break;

		--pwsz;
	}

	// Extension not found: return position of ending zero
	return pwszPath + nSym;
}

// Compare file extension e.g. with 'mov' (NOT '.mov' )
inline bool IsFileExtensionW( LPCWSTR _pszFilePath, LPCWSTR _pszExt )
{
	LPCWSTR pszExt = FindFileExtensionW( _pszFilePath );
	if( pszExt && pszExt[0] )
		return lstrcmpiW( pszExt + 1, _pszExt ) == 0;

	return false;
}

// Compare file extension e.g. with 'mov' (NOT '.mov' )
inline bool IsFileExtensionA( LPCSTR _pszFilePath, LPCSTR _pszExt )
{
	LPCSTR pszExt = FindFileExtensionA( _pszFilePath );
	if( pszExt && pszExt[0] )
		return lstrcmpiA( pszExt + 1, _pszExt ) == 0;

	return false;
}

// Change extension of file. Size of pszName buffer should be at least MAX_PATH symbols
inline LPCTSTR __stdcall ChangeFileExtention(IN LPTSTR pszName, IN LPCTSTR pszExt)
{
	ATLASSERT(pszName != NULL);

	if(pszExt == NULL || pszName == NULL)
		return pszName;

	if(pszName[0] == (TCHAR)0)
	{
		*pszName = (TCHAR)'.';
		lstrcpyn(pszName + 1, pszExt, MAX_PATH - 1);
		return pszName;
	}

	DWORD nSym = lstrlen(pszName);
	ATLASSERT(nSym < MAX_PATH);
	if(nSym >= MAX_PATH) return pszName;

	//---------------------------------
	LPTSTR psz = (LPTSTR)FindFileExtension(pszName);
	if(*psz != (TCHAR)'.')
	{
		ATLASSERT(nSym < MAX_PATH - 1);
		if(nSym == MAX_PATH - 1)
			return pszName;

		*psz = (TCHAR)'.';
	}

	++psz;
	nSym = MAX_PATH - (DWORD)((DWORD_PTR)psz - (DWORD_PTR)pszName)/sizeof(TCHAR);
	lstrcpyn(psz, pszExt, nSym);
	return pszName;
}

inline LPCWSTR __stdcall InsertBeforeExtension(CComBSTR& cbsPath, IN LPCWSTR pszInsert)
{
	if( !pszInsert || !pszInsert[0] || !cbsPath.Length() )
		return cbsPath;

	// Create header file
	LPWSTR pszUpdatedPath = (LPWSTR)alloca(lstrlenW(cbsPath) * sizeof(WCHAR) + lstrlenW(pszInsert) * sizeof(WCHAR) + 8 );
	lstrcpyW( pszUpdatedPath, cbsPath );

	LPCWSTR pszExtSrc = ::FindFileExtensionW( cbsPath );
	LPWSTR pszExtDst = (LPWSTR)::FindFileExtensionW( pszUpdatedPath );
	if( pszExtDst )
		pszExtDst[0] = 0;
	lstrcatW( pszUpdatedPath, pszInsert );

	if( pszExtSrc )
		lstrcatW( pszUpdatedPath, pszExtSrc );

	cbsPath = pszUpdatedPath;
	return cbsPath;
}

// Size of pszFolder must be at least _MAX_PATH symbols with ending zero
inline bool __stdcall GetModuleFolder(IN HINSTANCE hModule, IN OUT LPTSTR pszFolder)
{
	ATLASSERT(pszFolder != NULL);
	if(pszFolder == NULL) return false;

	DWORD dwLen = ::GetModuleFileName(hModule, pszFolder, _MAX_PATH);
	if(dwLen != 0)
	{
		TCHAR* pszFile = _tcsrchr(pszFolder, (TCHAR)'\\');
		if(pszFile != NULL)
		{
			*(pszFile + 1) = (TCHAR)0;
			return true;
		}
	}

	*pszFolder = (TCHAR)0;
	return false;
}

// Next 2 methods can't be compiled without errors in console projects (additional headers need)
// For disable macro definition should be put to stdafx.h before any .h file
#ifndef NOT_INCLUDE_OFN

inline UINT_PTR CALLBACK _SFD_OFNHookProc( HWND hdlg, UINT uiMsg, WPARAM /*wParam*/, LPARAM lParam )
{
	OFNOTIFYEX* pOF = (OFNOTIFYEX*)lParam;
	if( uiMsg == WM_NOTIFY )
	{
		HWND hOpenDlg = ::GetParent( hdlg );
		switch( pOF->hdr.code )
		{
		case CDN_INITDONE:
			{
				::ShowWindow( ::GetDlgItem( hOpenDlg, cmb1 ), SW_HIDE );
				::ShowWindow( ::GetDlgItem( hOpenDlg, stc2 ), SW_HIDE );
				::ShowWindow( ::GetDlgItem( hOpenDlg, edt1 ), SW_HIDE );
				::ShowWindow( ::GetDlgItem( hOpenDlg, cmb13 ), SW_HIDE );
				
				// Rearrange Ok and Cancel buttons
				RECT rectOk = {0};
				::GetWindowRect( ::GetDlgItem( hOpenDlg, IDOK ), &rectOk );
				::ScreenToClient( hOpenDlg, (LPPOINT)&rectOk );
				::ScreenToClient( hOpenDlg, ((LPPOINT)&rectOk) + 1);
				RECT rectCancel = {0};
				::GetWindowRect( ::GetDlgItem( hOpenDlg, IDCANCEL ), &rectCancel );
				::ScreenToClient( hOpenDlg, (LPPOINT)&rectCancel );
				::ScreenToClient( hOpenDlg, ((LPPOINT)&rectCancel) + 1);

				::SetWindowPos( ::GetDlgItem( hOpenDlg, IDCANCEL ), NULL, rectCancel.left, rectCancel.top - 5, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
				::SetWindowPos( ::GetDlgItem( hOpenDlg, IDOK ), NULL, rectCancel.left - (rectCancel.right - rectCancel.left) - 5, rectCancel.top - 5, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
				
				// Set size and position for current folder control
				RECT rectList = {0};
				::GetWindowRect( ::GetDlgItem( hOpenDlg, lst1 ), &rectList );
				int nWidth = rectList.right - rectList.left;
				::SetWindowPos( ::GetDlgItem( hOpenDlg, stc3 ), NULL, rectOk.right - nWidth, rectOk.top - 5, nWidth, 20, SWP_NOZORDER );
				
				// Update style (not work by some reason)
				long lStyle = ::GetWindowLong( ::GetDlgItem( hOpenDlg, stc3 ), GWL_STYLE );
				lStyle |= SS_CENTERIMAGE | SS_SUNKEN;
				::SetWindowLong( ::GetDlgItem( hOpenDlg, stc3 ), GWL_STYLE, lStyle);
				
				break;	
			}
		case CDN_FOLDERCHANGE:
			{
				// Update current folder
				TCHAR szFolder[MAX_PATH] = {0};
				::SendMessage( hOpenDlg, CDM_GETFOLDERPATH, SIZEOF_ARRAY(szFolder), (LPARAM)szFolder );
				::SetDlgItemText( hOpenDlg, stc3, szFolder );
				ATLTRACE2( atlTraceGeneral, 2, _T("_SFD_OFNHookProc - Msg: CDN_FOLDERCHANGE [%s]\n"), szFolder );
				break;
			}
		case CDN_FILEOK:
			ATLTRACE2( atlTraceGeneral, 2, _T("_SFD_OFNHookProc - Msg: CDN_FILEOK\n") );
			break;
		case CDN_SELCHANGE:
			ATLTRACE2( atlTraceGeneral, 2, _T("_SFD_OFNHookProc - Msg: CDN_SELCHANGE\n") );
			break;
//		case CDN_INCLUDEITEM:
//			ATLTRACE2( atlTraceGeneral, 2, _T("_SFD_OFNHookProc - Msg: CDN_INCLUDEITEM\n") );
//			break;
		default:
			ATLTRACE2( atlTraceGeneral, 2, _T("_SFD_OFNHookProc - Msg: %08X\n"), uiMsg );
		}
	}
	
	return 0;
}

// Show dialog for select folder
inline BOOL	__stdcall ShowFolderDialog( HWND _hWndParent, LPTSTR _szFolderPath, LPTSTR _szTitle /*= NULL*/ )
{
	TCHAR szSourceFile[MAX_PATH]={0};
	lstrcpy( szSourceFile, _T("Fake") );

	OPENFILENAME ofn = {0};
	ofn.lStructSize = sizeof(OPENFILENAME) ;
	ofn.hwndOwner = _hWndParent;
	ofn.lpstrFilter = TEXT("Folder filter\0 \0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = szSourceFile;
	ofn.nMaxFile = SIZEOF_ARRAY(szSourceFile) ;
	ofn.lpstrFileTitle = NULL ;
	ofn.lpstrTitle = _szTitle ? _szTitle : TEXT("Browse For Folder") ;
	ofn.nMaxFileTitle = 0 ;
	ofn.lpstrInitialDir = _szFolderPath;
	ofn.Flags = OFN_EXPLORER | OFN_ENABLEHOOK | OFN_HIDEREADONLY;
	ofn.lpfnHook = _SFD_OFNHookProc;
	
	BOOL bRes = GetOpenFileName( &ofn );
	if(  bRes )
	{
		ofn.lpstrFile[ofn.nFileOffset] = 0;
		lstrcpy( _szFolderPath, ofn.lpstrFile );
	}

	return bRes;
}
#endif	// NOT_INCLUDE_OFN

inline bool __stdcall IsFileExist( LPCTSTR _szFileName )
{
	HANDLE hFile = CreateFile( _szFileName, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if( hFile != INVALID_HANDLE_VALUE )
	{
		::CloseHandle( hFile );
		return true;
	}

	return false;
}

// Gets path to current executed .exe module
// Size of pszExePath must be at least _MAX_PATH symbols with ending zero
// Sample: "c:\dir1\dir2\dir3\"
inline bool __stdcall GetExePath(LPTSTR pszExePath)
{
	ATLASSERT(pszExePath != NULL);
	if(pszExePath == NULL) return false;

	*pszExePath = (TCHAR)0;

	LPTSTR psz = ::GetCommandLine();
	ATLASSERT(psz != NULL);
	if(psz == NULL) return false;

	//---------------------------------
	TCHAR szPath[_MAX_PATH];
	DWORD nDivider = 0;

	if(*psz == (TCHAR)'"')
	{
		++psz;
		lstrcpy(szPath, psz);

		DWORD n = 0;
		while(psz[n] != (TCHAR)'\"')
		{
			if(psz[n] == (TCHAR)0)
				return false;

			if(psz[n] == (TCHAR)':' || psz[n] == (TCHAR)'\\' || psz[n] == (TCHAR)'/')
				nDivider = n;
			++n;
		}
	}
	else
	{
		// Path in 8.3 format
		lstrcpy(szPath, psz);

		DWORD n = 0;
		while(psz[n] != (TCHAR)' ' && psz[n] != (TCHAR)0)
		{
			if(psz[n] == (TCHAR)':' || psz[n] == (TCHAR)'\\' || psz[n] == (TCHAR)'/')
				nDivider = n;
			++n;
		}
	}

	if(nDivider > 0)
		++nDivider;

	szPath[nDivider] = (TCHAR)0;
	lstrcpy(pszExePath, szPath);
	return true;
}

// Constructs path to file from default path pszDefPath and realative path to file pszFile
//
// Default path: <drive>:[ ['\' | '/'] [ dir1 ['\' | '/' [dir2 ['\' | '/' ... ] ] ] ] ]
// If "<drive>:" not cpecified: returns false
// Samples:
//	c:
//	D:\
//	e:dir1
//	e:/dir1
//	e:/dir1/
//	F:\dir1/dir2
//	F:/dir1\dir2\
//
// File name: [..'/'|'\'] ... [..'/'|'\'] [['/'|'\']dir1'/'|'\'[ dir2'/'|'\'[...]]] <name.ext>
// Samples:
// a.avi
// dir1/b.mov
// dir1\dir2/c.mpg
// ../d.wav
// ..\../e.mp3
// ../dir1\f.wme

inline bool __stdcall ConstructPath(LPCTSTR pszDefPath, LPTSTR pszFile)
{
//#pragma vcout("TODO. ConstructPath(): bad code, should be corrected")

	ATLASSERT(pszDefPath != NULL);
	if(pszDefPath == NULL) return false;
	ATLASSERT(pszFile != NULL);
	if(pszFile == NULL) return false;

	LPTSTR psz = pszFile;
	while(*psz == (TCHAR)'/' || *psz == (TCHAR)'\\')
		++psz;

	if(*psz == (TCHAR)0)
		return false;

	// If path to file is absolute: ignore def path and return original file name
	bool b = ((TCHAR)'A' <= *psz && *psz <= (TCHAR)'Z') ||
		((TCHAR)'a' <= *psz && *psz <= (TCHAR)'z');
	if(b && psz[1] == (TCHAR)':')
		return true;

	// If def path not set: return original file name
	if(*pszDefPath == (TCHAR)0)
		return true;

	// If device not specified in def path: return false
	DWORD nSym = lstrlen(pszDefPath);
	if(nSym < 2 || nSym >= _MAX_PATH)
		return false;

	b = ((TCHAR)'A' <= *pszDefPath && *pszDefPath <= (TCHAR)'Z') ||
		((TCHAR)'a' <= *pszDefPath && *pszDefPath <= (TCHAR)'z');
	if(!b || pszDefPath[1] != (TCHAR)':')
		return false;

	//---------------------------------
	// Calculate number of "..\" in file name
	DWORD nUpDir = 0;
	while(lstrlen(psz) >= 3)		// "..\", "../"
	{
		if( psz[0] == (TCHAR)'.' && psz[1] == (TCHAR)'.' &&
			(psz[2] == (TCHAR)'/' || psz[2] == (TCHAR)'\\'))
		{
			psz += 3;
			++nUpDir;
		}
		else
			break;
	}

	nSym = lstrlen(psz);
	if(nSym == 0 || nSym >= _MAX_PATH - 3)
		return false;

	//---------------------------------
	// Analyze path, skip directories on tail
	LPCTSTR pszStart = pszDefPath + 2;
	if(*pszStart == (TCHAR)'/' || *pszStart == (TCHAR)'\\')
	{
		++pszStart;
		if(*pszStart == (TCHAR)'/' || *pszStart == (TCHAR)'\\')
		{
			// Two or more '//' in path
			return false;
		}
	}

	TCHAR szName[_MAX_PATH];
	szName[0] = *pszDefPath;
	szName[1] = (TCHAR)':';
	szName[2] = (TCHAR)'\\';

	if(*pszStart == (TCHAR)0)
	{
		if(nUpDir != 0)
			return false;

		lstrcpy(szName + 3, psz);
	}
	else
	{
		LPCTSTR pszEnd = pszStart + lstrlen(pszStart) - 1;
		if(*pszEnd != (TCHAR)'/' && *pszEnd != (TCHAR)'\\')
			++pszEnd;

		// *pszEnd == 0 or '/' or '\' 
		while(nUpDir > 0 && pszEnd > pszStart)
		{
			while(pszEnd > pszStart)
			{
				--pszEnd;
				if(*pszEnd == (TCHAR)'/' || *pszEnd == (TCHAR)'\\')
					break;
			}

			--nUpDir;
		}
		
		if(nUpDir != 0)
			return false;

		if(pszEnd == pszStart)
		{
			lstrcpy(szName + 3, psz);
		}
		else
		{
			DWORD_PTR nPath = (DWORD_PTR)(pszEnd - pszStart);
			if(3 + nPath + 1 + nSym >= _MAX_PATH)
				return false;

			memcpy(szName + 3, pszStart, nPath * sizeof(TCHAR));
			szName[nPath + 3] = (TCHAR)'\\';
			lstrcpy(szName + 3 + nPath + 1, psz);
		}
	}

	lstrcpy(pszFile, szName);
	return true;
}

// Checks disabled symbols in path name
inline bool __stdcall IsCorrectPath(LPCTSTR pszPath)
{
//#pragma vcout("TODO. IsCorrectPath(): should be extended")

	ATLASSERT(pszPath != NULL);
	if(pszPath == NULL) return false;

	//---------------------------------
	int nSym = lstrlen(pszPath);
	if(*pszPath == (TCHAR)'"')
	{
		if(nSym <= 2 || pszPath[nSym - 1] != (TCHAR)'"')
			return false;

		++pszPath;
		nSym -= 2;
	}

	//---------------------------------
	static LPCTSTR pszDisabled = _T("\"*?<>|");

	for(int i = 0; i < nSym; ++i)
	{
		for(int j = 0; pszDisabled[j] != (TCHAR)0; ++j)
		{
			if(_istcntrl(pszPath[i]) || pszPath[i] == pszDisabled[j])
				return false;
		}
	}
	return true;
}

// Creates nested folder
// pszFolder: <drive>:['\' | '/']<catalog1>\<catalog2>\...\<catalogN>['\' | '/']
inline HRESULT __stdcall CreateFolder(LPCTSTR pszFolder)
{
	ATLASSERT(pszFolder != NULL);
	if(pszFolder == NULL) return E_INVALIDARG;

	DWORD nSym = lstrlen(pszFolder);
	// MSDN, CreateDirectory(): "There is a default string size limit for paths of 248 characters"
	if(nSym < 3 || nSym > 248)
		return E_INVALIDARG;

	TCHAR cDrive = (TCHAR)(pszFolder[0] | 0x20);
	if(cDrive < (TCHAR)'a' || (TCHAR)'z' < cDrive || pszFolder[1] != (TCHAR)':')
		return E_INVALIDARG;

	if((pszFolder[2] == (TCHAR)'/' || pszFolder[2] == (TCHAR)'\\') && nSym == 3)
		return E_INVALIDARG;

	//---------------------------------
	SetLastError(NOERROR);

	TCHAR szTempFolder[249];
	lstrcpy(szTempFolder, pszFolder);

	DWORD nPos = 2;
	if(pszFolder[2] == (TCHAR)'/' || pszFolder[2] == (TCHAR)'\\')
		++nPos;

	while(nPos <= nSym)
	{
		if( szTempFolder[nPos] == (TCHAR)'/' || szTempFolder[nPos] == (TCHAR)'\\' ||
			szTempFolder[nPos] == (TCHAR)0)
		{
			szTempFolder[nPos] = (TCHAR)0;

			BOOL bRet = ::CreateDirectory(szTempFolder, NULL);
			if(!bRet)
			{
				DWORD dwErr = GetLastError();
				if(dwErr != ERROR_ALREADY_EXISTS)
					return HRESULT_FROM_WIN32(dwErr);

				::SetLastError(ERROR_SUCCESS);
			}

			szTempFolder[nPos] = pszFolder[nPos];

			if(nPos == nSym - 1)
			{
				// Last symbol before ending zero is '/' or '\'
				break;
			}
		}

		++nPos;
	}

	//---------------------------------
//	HANDLE hFile = ::CreateFile(pszFolder, 0,
//		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING,
//		FILE_FLAG_BACKUP_SEMANTICS, NULL);
//	if(hFile == INVALID_HANDLE_VALUE)
//		return HRESULT_FROM_WIN32(GetLastError());
//
//	::CloseHandle(hFile);

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Bit stream functions
//
// Bitstream format:
// Bytes:         0                       1                       2        ...
// Bits:          [7  6  5  4  3  2  1  0][7  6  5  4  3  2  1  0][7  6  5 ...
// nSrcBitOffset:  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 ...

inline DWORD __stdcall GetOneBit(const BYTE* pbSrc, DWORD nSrcBitOffset, BYTE* pnBit, DWORD cbSrc /*= MAXDWORD/8*/)
{
	ATLASSERT(pbSrc != NULL);
	if(pbSrc == NULL) return 0;
	ATLASSERT(pnBit != NULL);
	if(pnBit == NULL) return 0;

	ATLASSERT(cbSrc <= MAXDWORD/8);
	if(cbSrc * 8 <= nSrcBitOffset)
		return 0;

	//---------------------------------
	//__asm
	//{
	//	mov		ebx, nSrcBitOffset
	//	mov		ecx, ebx
	//	shr		ebx, 3			// ebx: byte offset = (nSrcBitOffset >> 3)
	//	mov		edi, pnBit
	//	mov		esi, pbSrc
	//	movzx	eax, byte ptr [esi + ebx]
	//	not		ecx
	//	and		ecx, 7
	//	shr		eax, cl
	//	and		eax, 1
	//	mov		byte ptr [edi], al
	//}

	//---------------------------------
	*pnBit = (BYTE)( (pbSrc[nSrcBitOffset >> 3] >> ((~nSrcBitOffset) & 7)) & 1 );
	return 1;
}

inline DWORD __stdcall GetOneBit(const BYTE* pbSrc, DWORD nSrcBitOffset, DWORD* pnBit, DWORD cbSrc /*= MAXDWORD/8*/)
{
	ATLASSERT(pbSrc != NULL);
	if(pbSrc == NULL) return 0;
	ATLASSERT(pnBit != NULL);
	if(pnBit == NULL) return 0;

	ATLASSERT(cbSrc <= MAXDWORD/8);
	if(cbSrc * 8 <= nSrcBitOffset)
		return 0;

	//---------------------------------
	//__asm
	//{
	//	mov		ebx, nSrcBitOffset
	//	mov		ecx, ebx
	//	shr		ebx, 3			// ebx: byte offset = (nSrcBitOffset >> 3)
	//	mov		esi, pbSrc
	//	movzx	eax, byte ptr [esi + ebx]
	//	not		ecx
	//	and		ecx, 7
	//	shr		eax, cl
	//	and		eax, 1
	//	mov		edi, pnBit
	//	mov		dword ptr [edi], eax
	//}

	//---------------------------------
	*pnBit = (pbSrc[nSrcBitOffset >> 3] >> ((~nSrcBitOffset) & 7)) & 1;
	return 1;
}

inline DWORD __stdcall PutOneBit(BYTE bBit, BYTE* pbDst, DWORD nDstBitOffset, DWORD cbDst /*= MAXDWORD/8*/)
{
	ATLASSERT(pbDst != NULL);
	if(pbDst == NULL) return 0;

	ATLASSERT(cbDst <= MAXDWORD/8);
	if(cbDst * 8 <= nDstBitOffset)
		return 0;

	//---------------------------------
	//__asm
	//{
	//	mov		ebx, nDstBitOffset
	//	mov		ecx, ebx
	//	shr		ebx, 3
	//	mov		edi, pbDst
	//	movzx	eax, byte ptr [edi + ebx]
	//	not		ecx
	//	and		ecx, 7
	//	xor		edx, edx
	//	not		edx
	//	shl		edx, cl
	//	add		edx, edx		// Additional left shift
	//	and		eax, edx
	//	movzx	edx, bBit
	//	and		edx, 1
	//	shl		edx, cl
	//	or		eax, edx
	//	mov		byte ptr [edi + ebx], al
	//}

	//---------------------------------
	BYTE* pb = pbDst + (nDstBitOffset >> 3);
	DWORD nBitPos = (~nDstBitOffset) & 7;
	*pb = (BYTE)( (*pb & ~(1 << nBitPos)) | ((bBit & 1) << nBitPos) );

	return 1;
}

inline DWORD __stdcall PutOneBit(DWORD dwBit, BYTE* pbDst, DWORD nDstBitOffset, DWORD cbDst /*= MAXDWORD/8*/)
{
	ATLASSERT(pbDst != NULL);
	if(pbDst == NULL) return 0;

	ATLASSERT(cbDst <= MAXDWORD/8);
	if(cbDst * 8 <= nDstBitOffset)
		return 0;

	//---------------------------------
	//__asm
	//{
	//	mov		ebx, nDstBitOffset
	//	mov		ecx, ebx
	//	shr		ebx, 3
	//	mov		edi, pbDst
	//	movzx	eax, byte ptr [edi + ebx]
	//	not		ecx
	//	and		ecx, 7
	//	xor		edx, edx
	//	not		edx
	//	shl		edx, cl
	//	add		edx, edx		// Additional left shift
	//	and		eax, edx
	//	mov		edx, dwBit
	//	and		edx, 1
	//	shl		edx, cl
	//	or		eax, edx
	//	mov		byte ptr [edi + ebx], al
	//}

	//---------------------------------
	BYTE* pb = pbDst + (nDstBitOffset >> 3);
	DWORD nBitPos = (~nDstBitOffset) & 7;
	*pb = (BYTE)( (*pb & ~(1 << nBitPos)) | (BYTE)((dwBit & 1) << nBitPos) );
	return 1;
}

//---------------------------------
// Bitstream format:
// Bytes:         0                       1                       2        ...
// Bits:          [7  6  5  4  3  2  1  0][7  6  5  4  3  2  1  0][7  6  5 ...
// nSrcBitOffset:  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 ...
//
// pbSrc: pointer to start of buffer
// nSrcBitOffset: offset in bits from start of buffer to get position.
// nGetBits: number of bits for get
// *pnValue: returned value
// cbSrc: size of buffer in bytes

inline DWORD __stdcall GetBits(const BYTE* pbSrc, DWORD nSrcBitOffset, DWORD nGetBits, DWORD* pnValue,
	DWORD cbSrc /*= MAXDWORD/8*/)
{
	ATLASSERT(pbSrc != NULL);
	if(pbSrc == NULL) return 0;
	ATLASSERT(pnValue != NULL);
	if(pnValue == NULL) return 0;

	ATLASSERT(nGetBits <= 32);
	ATLASSERT(cbSrc <= MAXDWORD/8);

	if(nGetBits == 0)
		return 0;
	if(cbSrc * 8 < nSrcBitOffset + nGetBits)
		return 0;

	//---------------------------------
#ifndef _WIN64
	__asm
	{
		mov		eax, nSrcBitOffset
			mov		ebx, eax
			shr		eax, 3			// eax: byte offset = (nSrcBitOffset >> 3)
			mov		esi, eax
			add		esi, pbSrc		// esi: pointer to first mixed byte

			and		ebx, 7
			add		ebx, nGetBits	// ebx = (nDstBitOffset & 7) + nGetBits

			add		eax, 5
			cmp		eax, cbSrc
			ja		L2

			//---------------------------------
			// Enough place for put mixed byte and 4 new bytes
			mov		eax, [esi]
		// Convert to DWORD from byte sequence
		bswap	eax

			cmp		ebx, 32
			ja		L1

			// summ of bits <= 32
			mov		ecx, 32
			sub		ecx, ebx
			shr		eax, cl			// eax >>= 32 - ( (nDstBitOffset & 7) + nGetBits )
			jmp		End

			//---------------------------------
			// 32 < summ of bits < 40
L1:		mov		ecx, ebx
		sub		ecx, 32
		shl		eax, cl			// eax <<= ( (nDstBitOffset & 7) + nGetBits ) - 32;

		mov		dl, byte ptr[esi + 4]
		mov		ecx, 40
			sub		ecx, ebx
			shr		dl, cl			// dl >> 40 - ( (nDstBitOffset & 7) + nGetBits )
			or		al, dl
			jmp		End

			//---------------------------------
L2:		xor		eax, eax

Cycle:	cmp		ebx, 8
		jb		L3

		shl		eax, 8
		mov		al, byte ptr [esi]		// eax = (eax << 8) | *pbSrc;
		sub		ebx, 8
			inc		esi
			jmp		Cycle

L3:		mov		ecx, ebx
		shl		eax, cl

		mov		dl, byte ptr [esi]
		mov		ecx, 8
			sub		ecx, ebx
			shr		dl, cl
			or		al, dl

			//---------------------------------
End:	mov		ecx, 32
		sub		ecx, nGetBits
		shl		eax, cl
		shr		eax, cl
		mov		edi, pnValue
		mov		dword ptr [edi], eax
	}
#else

	//---------------------------------
	DWORD nGetBitsMinusOne = nGetBits - 1;
	const BYTE* pb = pbSrc + (nSrcBitOffset >> 3);
	DWORD shift = (~(nSrcBitOffset + nGetBitsMinusOne)) & 7;

	// if nGetBits == 0x20 => (1 << nGetBits) = 1, mask = (1 << nGetBits) - 1 = 0
	DWORD mask = (1 << nGetBitsMinusOne);
	mask |= mask - 1;
	switch((shift + nGetBitsMinusOne) >> 3)
	{
	case 0:
		*pnValue = (*pb >> shift) & mask;
		break;

	case 1:
		ATLASSERT(0 < nGetBitsMinusOne && nGetBitsMinusOne < 16);
		*pnValue = ((((DWORD)pb[0] << 8) | (DWORD)pb[1]) >> shift) & mask;
		break;

	case 2:
		ATLASSERT(8 < nGetBitsMinusOne && nGetBitsMinusOne < 24);
		*pnValue = ((((DWORD)pb[0] << 16) | ((DWORD)pb[1] << 8) | (DWORD)pb[2]) >> shift) & mask;
		break;

	case 3:
		ATLASSERT(16 < nGetBitsMinusOne);
		*pnValue = ((((DWORD)pb[0] << 24) | ((DWORD)pb[1] << 16) | ((DWORD)pb[2] << 8) | (DWORD)pb[3]) >> shift) & mask;
		break;

	case 4:
		ATLASSERT(24 < nGetBitsMinusOne);
		*pnValue = ( ((((DWORD)pb[0] << 24) | ((DWORD)pb[1] << 16) | ((DWORD)pb[2] << 8) | (DWORD)pb[3]) << (8 - shift))
			| ((DWORD)pb[4] >> shift) ) & mask;
		break;
	}
#endif // !_WIN64

	return nGetBits;
}

// Put nSrcBits lower bits from dw to pbDst bit stream from nDstBitOffset bit position
// ATTENTION. Function can't be used for insertion bits into middle of bitstream:
// unused lower bits in last changed byte cleared !
inline DWORD __stdcall PutBits(DWORD dwSrc, DWORD nSrcBits, BYTE* pbDst, DWORD nDstBitOffset,
	DWORD cbDst /*= MAXDWORD/8*/)
{
	ATLASSERT(pbDst != NULL);
	if(pbDst == NULL) return 0;

	ATLASSERT(nSrcBits <= 32);
	ATLASSERT(cbDst <= MAXDWORD/8);

	if(nSrcBits == 0)
		return 0;
	if(cbDst * 8 < nDstBitOffset + nSrcBits)
		return 0;

	//---------------------------------
#ifndef _WIN64
	__asm
	{
		mov		ebx, nDstBitOffset
			mov		edi, ebx
			shr		edi, 3
			add		edi, pbDst		// edi: pointer to first mixed byte
			and		ebx, 7			// ebx = (nDstBitOffset & 7)

			mov		eax, dwSrc

			// Clear upper bits of dwSrc
			mov		ecx, 32
			sub		ecx, nSrcBits
			shl		eax, cl			// eax = dwSrc << (32 - nSrcBits)

			// Shift of dwSrc for free place for old data from pbDst
			mov		ecx, ebx
			shr		eax, cl			// eax = eax >> (nDstBitOffset & 7)

			// Clear edx: if nDstBitOffset == 0, then (32 - (nDstBitOffset & 7)) = 32
			// and shl edx, 32 do nothing (don't clear unused edx bits)
			xor		edx, edx

			// Get rest of old data (first mixed byte)
			mov		dl, byte ptr [edi]

		// Clear lower bits of first mixed byte *(pbDst + nDstBitOffset/8)
		mov		ecx, 8
			sub		ecx, ebx
			shr		edx, cl			// edx >>= (8 - (nDstBitOffset & 7))

			// Shift old bits to high position in resulted DWORD
			mov		ecx, 32
			sub		ecx, ebx
			shl		edx, cl			// edx <<= (32 - (nDstBitOffset & 7))

			// Mix old and new data
			or		eax, edx

			// Convert to byte sequence
			bswap	eax

			// Number of bits in result
			add		ebx, nSrcBits	// ebx = (nDstBitOffset & 7) + nSrcBits
			cmp		ebx, 25
			jl		L1

			mov		[edi], eax
			cmp		ebx, 33
			jl		End

			mov		eax, dwSrc
			mov		ecx, 40
			sub		ecx, ebx
			shl		eax, cl			// eax <<= (40 - ((nDstBitOffset & 7) + nSrcBits) )
			add		edi, 4
			jmp		L2

			//---------------------------------
L1:		cmp		ebx, 9
		jl		L2

		mov		word ptr [edi], ax
		cmp		ebx, 17
		jl		End

		shr		eax, 16
		add		edi, 2

L2:		mov		byte ptr [edi], al

End:
	}

	//---------------------------------
#else

	BYTE* pb = pbDst + (nDstBitOffset >> 3);

	// Source shifted and masked (all unknown upper bits of dwSrc go out)
	DWORD dwPut = dwSrc << (32 - nSrcBits);

	// Mix first byte (mask old valid part, add new part)
	DWORD nBitOffset = nDstBitOffset & 7;
	*pb = (*pb & (BYTE)(0xFF00 >> nBitOffset)) | (BYTE)(dwPut >> (24 + nBitOffset));

	// High part of dwPut is rest of added bits
	dwPut <<= 8 - nBitOffset;
	switch((nBitOffset + nSrcBits - 1) >> 3)
	{
	case 0:
		break;

	case 1:
		pb[1] = (BYTE)(dwPut >> 24);
		break;

	case 2:
		pb[1] = (BYTE)(dwPut >> 24);
		pb[2] = (BYTE)(dwPut >> 16);
		break;

	case 3:
		pb[1] = (BYTE)(dwPut >> 24);
		pb[2] = (BYTE)(dwPut >> 16);
		pb[3] = (BYTE)(dwPut >> 8);
		break;

	case 4:
		pb[1] = (BYTE)(dwPut >> 24);
		pb[2] = (BYTE)(dwPut >> 16);
		pb[3] = (BYTE)(dwPut >> 8);
		pb[4] = (BYTE)dwPut;
		break;
	}

#endif	// !_WIN64

	return nSrcBits;
}

inline DWORD __stdcall GetBitSeq(const BYTE* pbSrc, DWORD nSrcBitOffset, BYTE* pbDst, DWORD nGetBits,
	DWORD cbSrc /*= MAXDWORD/8*/)
{
	ATLASSERT(pbSrc != NULL);
	if(pbSrc == NULL) return 0;
	ATLASSERT(pbDst != NULL);
	if(pbDst == NULL) return 0;

	ATLASSERT(cbSrc <= MAXDWORD/8);

	if(nGetBits == 0)
		return 0;
	if(cbSrc * 8 < nSrcBitOffset + nGetBits)
		return 0;

	//---------------------------------
	DWORD nBytes = (nSrcBitOffset >> 3);
	pbSrc += nBytes;
	cbSrc -= nBytes + (nGetBits - 1)/32;
	nSrcBitOffset &= 0x7;

	DWORD nGet = nGetBits;
	if(nGet > 32)
	{
		DWORD dwPrev = SwapDW(*(DWORD*)pbSrc);
		pbSrc += 4;
		dwPrev <<= nSrcBitOffset;

		nGet -= 32;
		while(nGet > 32)
		{
			DWORD dwNext = SwapDW(*(DWORD*)pbSrc);
			pbSrc += 4;

			if(nSrcBitOffset != 0)
				dwPrev |= dwNext >> (32 - nSrcBitOffset);

			*(DWORD*)pbDst = SwapDW(dwPrev);
			pbDst += 4;

			dwPrev = dwNext << nSrcBitOffset;
			nGet -= 32;
		}

		// Put part of next byte to dwPrev
		BYTE bPrev = *pbSrc;

		if(nSrcBitOffset != 0)
			dwPrev |= (DWORD)(bPrev >> (8 - nSrcBitOffset));

		*(DWORD*)pbDst = SwapDW(dwPrev);
		pbDst += 4;
	}

	//---------------------------------
	DWORD dw = 0;
	GetBits(pbSrc, nSrcBitOffset, nGet, &dw, cbSrc);

	dw <<= (32 - nGet);
	dw = SwapDW(dw);

	if(nGet > 24)
		*(DWORD*)pbDst = dw;
	else if(nGet > 8)
	{
		*(WORD*)pbDst = (WORD)dw;

		if(nGet > 16)
			pbDst[2] = (BYTE)(dw >> 16);
	}
	else
		*pbDst = (BYTE)dw;

	return nGetBits;
}

inline DWORD __stdcall PutBitSeq(const BYTE* pbSrc, DWORD nPutBits, BYTE* pbDst, DWORD nDstBitOffset,
	DWORD cbDst /*= MAXDWORD/8*/)
{
	ATLASSERT(pbSrc != NULL);
	if(pbSrc == NULL) return 0;
	ATLASSERT(pbDst != NULL);
	if(pbDst == NULL) return 0;

	ATLASSERT(cbDst <= MAXDWORD/8);

	if(nPutBits == 0)
		return 0;
	if(cbDst * 8 < nDstBitOffset + nPutBits)
		return 0;

	//---------------------------------
	DWORD nBytes = (nDstBitOffset >> 3);
	pbDst += nBytes;
	cbDst -= nBytes + (nPutBits - 1)/32;
	nDstBitOffset &= 0x7;

	DWORD nBits = nPutBits;
	if(nBits > 32)
	{
		DWORD dwPrev = (DWORD)*pbDst;
		if(nDstBitOffset != 0)
		{
			dwPrev >>= (8 - nDstBitOffset);		// Clear lower bits
			dwPrev <<= (32 - nDstBitOffset);
		}
		else
			dwPrev = 0;

		while(nBits > 32)
		{
			DWORD dwNext = SwapDW(*(DWORD*)pbSrc);
			pbSrc += 4;

			*(DWORD*)pbDst = SwapDW(dwPrev | (dwNext >> nDstBitOffset));
			pbDst += 4;

			if(nDstBitOffset != 0)
				dwPrev = dwNext << (32 - nDstBitOffset);
			else
				dwPrev = 0;

			nBits -= 32;
		}

		// Put rest of byte from dwPrev to pbDst
		*pbDst = *((BYTE*)&dwPrev + 3);
	}

	//---------------------------------
	DWORD dw = 0;
	BYTE* pb = (BYTE*)&dw + 3;
	for(DWORD n = 0; n < (nBits + 7)/8; ++n, --pb, ++pbSrc)
		*pb = *pbSrc;

	dw >>= 32 - nBits;
	return (PutBits(dw, nBits, pbDst, nDstBitOffset, cbDst) != 0)? nPutBits : 0;
}

/////////////////////////////////////////////////////////////////////////////
// Conversion of data from big-endian to little-endian and vice versa
//
// NOTE. Don't use ECX and EDX registers for __fastcall. From MSDN "__fastcall":
// "Argument-passing order
//	The first two DWORD or smaller arguments are passed in ECX and EDX registers;
//	all other arguments are passed right to left."
//
// Really if 2 seqential calls of functions will be executed and code optimized
// by compiler, then error possible (ECX and EDX have invalid values).
//---------------------------------------------------------------------------

// 10 -> 01
__forceinline WORD __fastcall SwapW(WORD w)
{
	return (WORD)( (w << 8) | (w >> 8) );
}

#pragma warning( disable : 4035 )

// x210 -> -012
__forceinline DWORD __fastcall Swap24(DWORD dw)
{
#ifndef _WIN64
	__asm
	{
		mov		eax, [dw]
		bswap	eax			// x210 -> 012x
			shr		eax, 8		// 012x -> -012
	}

	//---------------------------------
#else
	//	return ((DWORD)(*(BYTE*)&dw) << 16) | ((DWORD)(*((BYTE*)&dw + 1)) << 8) | (DWORD)(*((BYTE*)&dw + 2));
	return ((dw >> 16) & 0xFF) | (dw & 0xFF00) | ((dw & 0xFF) << 16);
#endif	// !_WIN64
}

// 3210 -> 0123
__forceinline DWORD __fastcall SwapDW(DWORD dw)
{
#ifndef _WIN64
	__asm
	{
		mov		eax, [dw]
		bswap	eax
	}

	//---------------------------------
#else
	return (dw >> 24) | ((dw >> 8) & 0xFF00) | ((dw & 0xFF00) << 8) | (dw << 24);
#endif	// !_WIN64
}

// 76543210 -> 01234567 and vice versa
__forceinline ULONGLONG __fastcall SwapI64Full(ULONGLONG ull)
{
#ifndef _WIN64
	__asm
	{
		mov		edx, DWORD ptr [ull]
		mov		eax, DWORD ptr [ull + 4]
		bswap	edx
			bswap	eax
	}

	//---------------------------------
#else
	return ((ULONGLONG)SwapDW(*(DWORD*)&ull) << 32) | (ULONGLONG)SwapDW(*((DWORD*)&ull + 1));
#endif	// !_WIN64
}

#pragma warning( default : 4035 )


template< class T >
inline static void SwapVal(T& _first, T& _second)
{
	T temp = _first;
	_first = _second;
	_second = temp;
}

// Note: If _nWords < 0 - assumed ZERO terminated sequence
inline static void SwapWords(LPBYTE _pbDest, LPCBYTE _pbSource, int _nWords)
{
	ATLASSERT(_pbDest && _pbSource);
	if (!_pbDest || !_pbSource) return;

	if (_nWords >= 0)
	{
		for (int i = 0; i < _nWords; i++)
		{
			_pbDest[i * 2 + 1] = _pbSource[i * 2];
			_pbDest[i * 2] = _pbSource[i * 2 + 1];
		}
	}
	else
	{
		// e.g. Convert UNICODE big endian zero terminated string (!?)
		while (*(short*)_pbSource != 0)
		{
			_pbDest[0] = _pbSource[1];
			_pbDest[1] = _pbSource[0];

			_pbDest += 2;
			_pbSource += 2;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Reverse bytes in byte array 

inline void __stdcall ReverseBytes(LPBYTE _pbData, DWORD _cbData)
{
	ATLASSERT(_pbData != NULL);
	BYTE btTemp = 0;
	for(DWORD i = 0 ; i < (_cbData >> 1); i++)
	{
		btTemp = *(_pbData + _cbData  - 1 - i);
		*(_pbData + _cbData  - 1 - i) = *(_pbData + i);
		*(_pbData + i) = btTemp;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Hex <-> BCD conversions for 4 bytes in DWORD
inline DWORD __stdcall Hex2BCD(DWORD dwHex)
{
	BYTE* pb = (BYTE*)&dwHex;
	*pb++ = (BYTE)((*pb/10) << 4 | (*pb%10));
	*pb++ = (BYTE)((*pb/10) << 4 | (*pb%10));
	*pb++ = (BYTE)((*pb/10) << 4 | (*pb%10));
	*pb   = (BYTE)((*pb/10) << 4 | (*pb%10));
	return dwHex;
}

inline DWORD __stdcall BCD2Hex(DWORD dwBCD)
{
	BYTE* pb = (BYTE*)&dwBCD;
	*pb++ = (BYTE)(10 * (*pb >> 4) | (*pb & 0xF));
	*pb++ = (BYTE)(10 * (*pb >> 4) | (*pb & 0xF));
	*pb++ = (BYTE)(10 * (*pb >> 4) | (*pb & 0xF));
	*pb   = (BYTE)(10 * (*pb >> 4) | (*pb & 0xF));
	return dwBCD;
}

/////////////////////////////////////////////////////////////////////////////
// Return Greater Common Divider

inline LONGLONG __stdcall MLGCD( LONGLONG nFirst, LONGLONG nSecond )
{
	if( ABS( nFirst ) > ABS( nSecond ) )
	{
		if( nSecond == 0 ) return nFirst ? nFirst : 1;
		return MLGCD( nFirst % nSecond , nSecond );
	}
	
	// nSecond >= nFirst
	if( nFirst == 0 ) return nSecond ? nSecond : 1;
	return MLGCD( nFirst, nSecond % nFirst );
}

/////////////////////////////////////////////////////////////////////////////
#pragma warning( push )
#pragma warning( disable : 4035 )

// For compatibility with IntelC++
//#define rdtsc __asm __emit 0fh __asm __emit 031h

// ATTENTION. The compiler can't inline this function in Debug mode!
// MSDN: "You cannot inline a function if its uses inline assembly
// and is not compiled with /Og, /Ox, /O1, or /O2".
#ifdef _WIN64
#pragma intrinsic(__rdtsc)
// For 64 bits used next sequence: rdtsc / shl rdx,20h / or rax,rdx : rdtsc put data to low DWORDs or rdx:rax
#define TimeStamp()		__rdtsc()
#else
__inline ULONGLONG TimeStamp(void)
{
	__asm { rdtsc }
}
#endif

#pragma warning( pop )

// *pdwFreq = CPU frequency in MHz
inline bool __stdcall GetProcFreq(DWORD* pdwFreq, bool bCalculateOnly /*= false*/)
{
	ATLASSERT(pdwFreq != NULL);
	if(pdwFreq == NULL) return false;

	
	//---------------------------------
	if(!bCalculateOnly)
	{
		// Attempts take frequency from registry

		CRegKey theKey;
		LONG lRes = theKey.Open(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"), KEY_READ);
		if(lRes == ERROR_SUCCESS)
		{
			DWORD dwFreq = 0;
#if (_ATL_VER < 0x0700)
			lRes = theKey.QueryValue(dwFreq, _T("~MHz"));
#else
			lRes = theKey.QueryDWORDValue(_T("~MHz"), dwFreq);
#endif //(_ATL_VER < 0x0700)
			if(lRes == ERROR_SUCCESS && dwFreq > 0)
			{
				*pdwFreq = dwFreq;
				return true;
			}
		}
	}

	//---------------------------------
	// Attempts calculate frequency from QueryPerformanceCounter() and RDTSC
	LARGE_INTEGER liFrequency = {0,0};
	LONGLONG llCounter = (LONGLONG)0;
	ULONGLONG ulCycleStart = (ULONGLONG)0;
	ULONGLONG ulCycleEnd = (ULONGLONG)0;

	BOOL bRes = QueryPerformanceFrequency(&liFrequency);
	if(bRes && liFrequency.QuadPart > (LONGLONG)0)
	{
		LARGE_INTEGER liPerformanceCountStart = {0,0};
		bRes = QueryPerformanceCounter(&liPerformanceCountStart);
		if(bRes)
		{
			ulCycleStart = TimeStamp();

			Sleep(10);

			LARGE_INTEGER liPerformanceCountEnd = {0,0};
			bRes = QueryPerformanceCounter(&liPerformanceCountEnd);
			if(bRes)
			{
				ulCycleEnd = TimeStamp();
				llCounter = liPerformanceCountEnd.QuadPart - liPerformanceCountStart.QuadPart;
			}
		}
	}

	//---------------------------------
	// Attempts calculate frequency from timeGetTime() and RDTSC
	if(llCounter == (LONGLONG)0)
	{
		DWORD dwStart = timeGetTime();
		ulCycleStart = TimeStamp();

		Sleep(40);

		DWORD dwEnd = timeGetTime();
		ulCycleEnd = TimeStamp();

		llCounter = (LONGLONG)dwEnd - (LONGLONG)dwStart;
		liFrequency.QuadPart = (LONGLONG)1000;
	}

	if(llCounter > (LONGLONG)0)
	{
		LONGLONG llCycles = (LONGLONG)ulCycleEnd - (LONGLONG)ulCycleStart;
		double dblFreq = (double)llCycles * (double)liFrequency.QuadPart/(double)llCounter;

		*pdwFreq = (DWORD)(dblFreq/(double)1000000);
		return true;
	}

	return false;
}

// IsVideoMemory() - detect video/system memory
//  0 - system
//  1 - video
// -1 - seems context switching occurs
// inline int __stdcall IsVideoMemory(LPBYTE pbTest, DWORD cbTestMax, LPBYTE pbRestore /*= NULL*/)
// {
// 	if( cbTestMax == 0 )
// 		return -1;
// 
// 	// For timing
// 	// TODO: Take into account thread context switching
// 	LARGE_INTEGER liFreq = {0};
// 	LARGE_INTEGER liStart = {0};
// 	LARGE_INTEGER liEnd = {0};
// 	::QueryPerformanceFrequency( &liFreq );
// 
// 	// Maximum test 16 Kb
// 	DWORD dwTest = MIN( 8 * 1024, cbTestMax );
// 
// 	int nRes = -1;
// 	LONGLONG rtRead = 0;
// 	LONGLONG rtWrite = 0;
// 	if( pbRestore != NULL )
// 	{
// 		::QueryPerformanceCounter( &liStart );
// 		_asm
// 		{
// 			mov esi, pbTest;
// 			mov edi, pbRestore;
// 			mov ecx, dwTest;
// 			shr ecx, 2;
// next_read_res:
// 			mov eax, [esi];
// 			mov [edi], eax;
// 			add esi, 4;
// 			add edi, 4;
// 			dec ecx;
// 			jnz next_read_res;
// 		}
// 		::QueryPerformanceCounter( &liEnd );
// 
// 		rtRead = ((liEnd.QuadPart - liStart.QuadPart) * (LONGLONG)10000000L) / liFreq.QuadPart;
// 
// 		::QueryPerformanceCounter( &liStart );
// 		_asm
// 		{
// 			mov esi, pbRestore;
// 			mov edi, pbTest;
// 			mov ecx, dwTest;
// 			shr ecx, 2;
// next_write_res:
// 			mov eax, [esi];
// 			mov [edi], eax;
// 			add esi, 4;
// 			add edi, 4;
// 			dec ecx;
// 			jnz next_write_res;
// 		}
// 		::QueryPerformanceCounter( &liEnd );
// 		rtWrite = ((liEnd.QuadPart - liStart.QuadPart) * (LONGLONG)10000000L) / liFreq.QuadPart;
// 	}
// 	else
// 	{
// 		::QueryPerformanceCounter( &liStart );
// 		_asm
// 		{
// 			mov esi, pbTest;
// 			mov ecx, dwTest;
// 			shr ecx, 2;
// next_read:
// 			mov eax, [esi];
// 			add esi, 4;
// 			dec ecx;
// 			jnz next_read;
// 		}
// 		::QueryPerformanceCounter( &liEnd );
// 
// 		rtRead = ((liEnd.QuadPart - liStart.QuadPart) * (LONGLONG)10000000L) / liFreq.QuadPart;
// 
// 		::QueryPerformanceCounter( &liStart );
// 		_asm
// 		{
// 			mov edi, pbTest;
// 			mov ecx, dwTest;
// 			shr ecx, 2;
// next_write:
// 			mov [edi], eax;
// 			add edi, 4;
// 			dec ecx;
// 			jnz next_write;
// 		}
// 		::QueryPerformanceCounter( &liEnd );
// 		rtWrite = ((liEnd.QuadPart - liStart.QuadPart) * (LONGLONG)10000000L) / liFreq.QuadPart;
// 	}
// 	
// 	if( rtRead > rtWrite * 100 )
// 	{
// 		// Seems what video memory
// 		// TODO: Check context switching for video memory
// 		nRes = 1;
// 	}
// 	else if( rtRead < 10 * (LONGLONG)10000L ) // Check context switch (more the 10 msec )
// 	{
// 		nRes = 0;
// 	}
// 
// #ifndef NDEBUG
// 	int nWH = (int)(rtWrite/(LONGLONG)10000L);
// 	int nWL = (int)(rtWrite - (LONGLONG)nWH * (LONGLONG)10000L);
// 	int nRH = (int)(rtRead/(LONGLONG)10000L);
// 	int nRL = (int)(rtRead - (LONGLONG)nRH * (LONGLONG)10000L);
// 	ATLTRACE2( atlTraceGeneral, 2, _T("[MEMORY DETECT] Write: %i.%04i ms  Read: %i.%04i ms : %s\n"), 
// 		nWH, nWL, nRH, nRL,
// 		nRes == 1 ? _T("VIDEO") : nRes == 0 ? _T("SYSTEM") : _T("SHOULD BE REPEATED") );
// #endif
// 
// 	return nRes;
// }
// 
// From tests: optimal value for cbSize = 1024 (7F added for pMemory alignment correction)
// inline bool __stdcall IsSlowReadMemory(void* pMemory, DWORD cbSize /*= 0x47F*/)
// {
// 	ATLASSERT(pMemory != NULL);
// 	ATLASSERT(cbSize >= 0x100);
// 
// 	// Convert tested memory to 128 bytes alignmented
// 	void* pTested = (void*)( ((DWORD_PTR)pMemory + (DWORD_PTR)0x7F) & (DWORD_PTR)~0x7F );
// 	cbSize -= (DWORD)((DWORD_PTR)pTested - (DWORD_PTR)pMemory);
// 	if(cbSize > 0x8000)
// 		cbSize = 0x8000;
// 	else
// 		cbSize = (cbSize + 0x7F) & ~0x7F;
// 
// 	void* pSys = _alloca(cbSize + 0x7F);
// 	pSys = (void*)( ((DWORD_PTR)pSys + (DWORD_PTR)0x7F) & (DWORD_PTR)~0x7F );
// 
// 	//---------------------------------
// 	cbSize >>= 2;
// 
// 	LONGLONG llSys[5];
// 	memset(llSys, 0, sizeof(llSys));
// 	LONGLONG llTested[5];
// 	memset(llTested, 0, sizeof(llSys));
// 
// 	for(int i = 0; i < 10; ++i)
// 	{
// 		for(int j = 0; j < 2; ++j)
// 		{
// 			void* p = (j == 0)? pSys : pTested;
// 
// 			LARGE_INTEGER li = {0};
// 			_asm
// 			{
// 				rdtsc
// 				mov		ebx, eax
// 
// 				mov		ecx, cbSize
// 				mov		esi, p
// 			rep	lods	eax
// 
// 				mov		ecx, edx
// 				rdtsc
// 				sub		eax, ebx
// 				sbb		edx, ecx
// 				mov		li.LowPart, eax
// 				mov		li.HighPart, edx
// 			}
// 
// 			LONGLONG* pllArr = (j == 0)? llSys : llTested;
// 			if(i < 5)
// 				pllArr[i] = li.QuadPart;
// 			else
// 			{
// 				int nMax = 0;
// 				for(int n = 1; n < 5; ++n)
// 				{
// 					if(pllArr[nMax] < pllArr[n])
// 						nMax = n;
// 				}
// 
// 				if(li.QuadPart < pllArr[nMax])
// 					pllArr[nMax] = li.QuadPart;
// 			}
// 		}
// 	}
// 
// 	LONGLONG llS = 0;
// 	LONGLONG llT = 0;
// 	for(int n = 0; n < 5; ++n)
// 	{
// 		llS += llSys[n];
// 		llT += llTested[n];
// 	}
// 
// 	// From tests:
// 	// if pMemory is video memory, then relation in range 25 -:- 500
// 	// llS and llT can vary in 2 times
// 	ATLTRACE2(atlTraceGeneral, 2, _T("[Read Memory] system=%u, tested=%u, relation=%f\n"),
// 		(DWORD)llS, (DWORD)llT, (double)(llT/llS));
// 
// 	return 5 * llS < llT;
// }

/////////////////////////////////////////////////////////////////////////////
// Some helpers

inline DWORD __stdcall PutStr(LPTSTR psz, LPCTSTR pszPut)
{
	ATLASSERT(psz != NULL && pszPut != NULL);
	lstrcpy(psz, pszPut);
	return lstrlen(psz);
}
inline DWORD __stdcall AddStr(LPTSTR psz, LPCTSTR pszAdd)
{
	ATLASSERT(psz != NULL && pszAdd != NULL);
	lstrcat(psz, pszAdd);
	return lstrlen(psz);
}

//---------------------------------
// Output string should be freed with free()

inline LPWSTR __stdcall CreateWchName(LPCTSTR pszName, DWORD nMaxSym)
{
	if(pszName == NULL || pszName[0] == (TCHAR)'\0') return NULL;

	DWORD n = lstrlen(pszName);
	if(nMaxSym != 0 && n > nMaxSym) n = nMaxSym;

	LPWSTR pwchAchName = (LPWSTR)malloc((n+1)*sizeof(WCHAR));
	if(pwchAchName == NULL) return NULL;

#ifdef _UNICODE
	memcpy(pwchAchName, pszName, n*sizeof(WCHAR));
#else
	::MultiByteToWideChar(CP_ACP, 0, pszName, n, pwchAchName, n);
#endif
	pwchAchName[n] = L'\0';
	return pwchAchName;
}

// cSize: size of pszString in characters, including a terminating null
inline LPCTSTR __stdcall LoadResString(UINT uIdString, LPTSTR pszString, ULONG cSize)
{
	ATLASSERT(pszString != NULL && cSize > 0);

	int n = 0;
#if _MSC_VER > 1200
	__if_exists(_Module)
	{
		n = ::LoadString(_Module.GetResourceInstance(), uIdString, pszString, cSize);
	}
	__if_not_exists(_Module)
	{
		n = ::LoadString(_AtlBaseModule.GetResourceInstance(), uIdString, pszString, cSize);
	}
#else
	n = ::LoadString(_Module.GetResourceInstance(), uIdString, pszString, cSize);
#endif

	if(n == 0)
	{
		*pszString = (TCHAR)0;

		TCHAR szErr[256];
		wsprintf(szErr, _T("LoadResString() : LoadString( %u )=0x%X"),
			uIdString, HRESULT_FROM_WIN32(::GetLastError()));
		_RPTF0(_CRT_ASSERT, szErr);
		return _T("");
	}
	return pszString;
}

/////////////////////////////////////////////////////////////////////////////
// Fill VS_FIXEDFILEINFO struct for current module
// A value of hModule == NULL specifies the module handle associated with the image
// file that the operating system used to create the current process
inline HRESULT __stdcall GetModuleVerInfo(VS_FIXEDFILEINFO* pFI, HMODULE hModule /*= NULL*/)
{
	ATLASSERT(pFI != NULL);
	if(pFI == NULL) return E_INVALIDARG;

	memset(pFI, 0, sizeof(VS_FIXEDFILEINFO));

	//---------------------------------
	::SetLastError(ERROR_SUCCESS);

	HRSRC hRes = ::FindResource(hModule, MAKEINTRESOURCE(VS_VERSION_INFO), MAKEINTRESOURCE(RT_VERSION));
	if(hRes == NULL)
		return ERROR_FROM_WIN32(::GetLastError(), E_FAIL);

	HGLOBAL hGlob = ::LoadResource(hModule, hRes);
	if(hGlob == NULL)
		return ERROR_FROM_WIN32(::GetLastError(), E_FAIL);

	BYTE* pbRes = (BYTE*)::LockResource(hGlob);
	if(pbRes == NULL)
		return ERROR_FROM_WIN32(::GetLastError(), E_FAIL);

	//---------------------------------
	WORD wLength = *(WORD*)pbRes;
	pbRes += 2;
	if(wLength < 4)
		return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);

	WORD wValueLength = *(WORD*)pbRes;
	pbRes += 2;
	if(wValueLength == 0)
		return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);

	pbRes += 2;						// Skip WORD wType: type of data in the version resource

	if(lstrcmpW((LPCWSTR)pbRes, L"VS_VERSION_INFO") != 0)
		return E_FAIL;

	pbRes += (lstrlenW((LPCWSTR)pbRes) + 1) * sizeof(WCHAR);

	if((DWORD_PTR)pbRes & 0x2)		// WORD  Padding1[]
		pbRes += 2;

	if(wValueLength > sizeof(VS_FIXEDFILEINFO))
		wValueLength = sizeof(VS_FIXEDFILEINFO);

	memcpy(pFI, pbRes, wValueLength);
	return (wValueLength < sizeof(VS_FIXEDFILEINFO))? S_FALSE : S_OK;
}

// Moved from XMKUtils
#pragma warning(push)
#pragma warning(disable:4996)

inline HRESULT Str2SysTimeW(LPCWSTR _pszTime, SYSTEMTIME* pSysTime, bool _bOnlyTime = false)
{
	ATLASSERT(pSysTime && _pszTime);
	if (!pSysTime || !_pszTime) return E_POINTER;


	// 2011-01-30 12:30:12.100 USA, Asia
	// 01/30/2011 12:30:12.100 Europe	 
	// 01.30.2001 12:30:12.100 Russia

	// Get time
	bool bHaveSeconds = false;
	double dblSeconds = 0;
	int nValue = 0;
	int nDigit = 1;
	int nOrder = 0;
	const WCHAR* pDuration = _pszTime + lstrlenW(_pszTime) - 1;
	while (pDuration >= _pszTime)
	{
		if (*pDuration >= L'0' && *pDuration <= L'9')
		{
			nValue += (*pDuration - L'0') * nDigit;
			nDigit *= 10;
		}

		// The '.' use for msec
		if (*pDuration == L':' || *pDuration == L'.' ||
			*pDuration == L'H' || *pDuration == L'h' ||
			*pDuration == L'M' || *pDuration == L'm' ||
			*pDuration == L'S' || *pDuration == L's' ||
			*pDuration == L' ' || *pDuration == L'\r' || *pDuration == L'\n' || *pDuration == L'\t' || // For date
			pDuration == _pszTime)
		{
			if (nOrder == 0)
			{
				if (*pDuration != L'.')
				{
					dblSeconds += nValue;
					nOrder = 1;
				}
				else
				{
					dblSeconds += (double)nValue / nDigit;
					bHaveSeconds = true; // If have msec -> have seconds
				}
			}
			else if (nOrder == 1)
			{
				dblSeconds += nValue;
			}
			else if (nOrder == 2)
			{
				dblSeconds += nValue * 60;
			}
			else if (nOrder == 3)
			{
				bHaveSeconds = true;
				dblSeconds += nValue * 3600;
			}

			if (*pDuration == L'H' || *pDuration == L'h')
				nOrder = 3;
			else if (*pDuration == L'M' || *pDuration == L'm')
				nOrder = 2;
			else if (*pDuration == L'S' || *pDuration == L's')
				nOrder = 1;
			else // ':' or '.' separator
				nOrder++;

			nValue = 0;
			nDigit = 1;
		}

		if (*pDuration == L' ' || *pDuration == L'\r' || *pDuration == L'\n' || *pDuration == L'\t')
			break;

		pDuration--;
	}

	if (!bHaveSeconds) dblSeconds *= 60;

	pSysTime->wMilliseconds = DBL2INT((dblSeconds - (int)dblSeconds) * 1000);
	pSysTime->wHour = (int)dblSeconds / 3600;
	pSysTime->wMinute = ((int)dblSeconds % 3600) / 60;
	pSysTime->wSecond = (int)dblSeconds % 60;

	pSysTime->wHour = pSysTime->wHour % 24;

	pSysTime->wYear = 0;
	pSysTime->wMonth = 0;
	pSysTime->wDay = 0;

	// Get Date
	if (pDuration > _pszTime && !_bOnlyTime)
	{
		int nYear = 0;
		int nMonth = 0;
		int nDay = 0;
		if (swscanf(_pszTime, L"%d-%d-%d", &nYear, &nMonth, &nDay) != 3 && // USA, Default CG
			swscanf(_pszTime, L"%d/%d/%d", &nDay, &nMonth, &nYear) != 3 && // Europe
			swscanf(_pszTime, L"%d.%d.%d", &nDay, &nMonth, &nYear) != 3)  // Russia
		{
			pSysTime->wYear = 0;
			pSysTime->wMonth = 0;
			pSysTime->wDay = 0;
		}
		else
		{
			pSysTime->wYear = nYear;
			pSysTime->wMonth = nMonth;
			pSysTime->wDay = nDay;
		}
	}

	return S_OK;
}

#pragma warning(pop)


inline bool Str2Bool(LPCWSTR pszRes)
{
	if (STR_IS_EMPTY(pszRes))
		return false;

	LPWSTR szTrimmed = ALLOCA_ARRAY(WCHAR, lstrlenW(pszRes) + 2);
	lstrcpyW(szTrimmed, pszRes);
	LPCWSTR pszTrimChars = L" \r\n\t";
	StrTrimW(szTrimmed, pszTrimChars);

	if (lstrcmpW(szTrimmed, L"0") == 0 ||
		lstrcmpiW(szTrimmed, L"false") == 0 ||
		lstrcmpiW(szTrimmed, L"off") == 0 ||
		lstrcmpiW(szTrimmed, L"no") == 0)
	{
		return 0;
	}

	if (lstrcmpW(szTrimmed, L"1") == 0 ||
		lstrcmpiW(szTrimmed, L"true") == 0 ||
		lstrcmpiW(szTrimmed, L"yes") == 0 ||
		lstrcmpiW(szTrimmed, L"on") == 0)
	{
		return 1;
	}

	if (_wtof(szTrimmed) > 0)
		return true;

	return false;
}

inline int Str2BoolInt(LPCWSTR pszRes, int _nWrong = -1 )
{
	if (STR_IS_EMPTY(pszRes))
		return _nWrong;

	LPWSTR szTrimmed = ALLOCA_ARRAY(WCHAR, lstrlenW(pszRes) + 2);
	lstrcpyW(szTrimmed, pszRes);
	LPCWSTR pszTrimChars = L" \r\n\t";
	StrTrimW(szTrimmed, pszTrimChars);

	if (lstrcmpW(szTrimmed, L"0") == 0 ||
		lstrcmpiW(szTrimmed, L"false") == 0 ||
		lstrcmpiW(szTrimmed, L"off") == 0 ||
		lstrcmpiW(szTrimmed, L"no") == 0)
	{
		return 0;
	}

	if (lstrcmpW(szTrimmed, L"1") == 0 ||
		lstrcmpiW(szTrimmed, L"true") == 0 ||
		lstrcmpiW(szTrimmed, L"yes") == 0 ||
		lstrcmpiW(szTrimmed, L"on") == 0)
	{
		return 1;
	}

	int nFromInt = _wtoi(szTrimmed);
	if (nFromInt != 0)
		return nFromInt;

	return _nWrong;
}

inline double Str2Double(LPCWSTR pszRes)
{
	if (!pszRes) return 0;

	LPWSTR pEnd = 0;
	double dblRes = wcstod(pszRes, &pEnd);
	if (pEnd && *pEnd == L'%')
		dblRes /= 100.0;

	return dblRes;
}

inline LONGLONG Str2Bytes(LPCWSTR _pszBytes)
{
	if (!_pszBytes) return 0;

	LPWSTR pEnd = NULL;
	double dblValue = wcstod(_pszBytes, &pEnd);
	LONGLONG llVal = DBL2INT64(dblValue);
	if (pEnd && (*pEnd == L'K' || *pEnd == L'k'))
	{
		llVal = DBL2INT64(dblValue * 1000);
	}
	else if (pEnd && (*pEnd == L'M' || *pEnd == L'm'))
	{
		llVal = DBL2INT64(dblValue * 1000 * 1000);
	}
	else if (pEnd && (*pEnd == L'G' || *pEnd == L'g'))
	{
		llVal = DBL2INT64(dblValue * 1000 * 1000 * 1000);
	}
	else if (pEnd && (*pEnd == L'T' || *pEnd == L't'))
	{
		llVal = DBL2INT64(dblValue * 1000 * 1000 * 1000 * 1000);
	}

	return llVal;
}

inline bool Rect_IsAbsoluteEmpty(LPCRECT _pRect)
{
	if (!_pRect || _pRect->left == _pRect->right || _pRect->top == _pRect->bottom)
		return true;

	return false;
}

inline bool Rect_IsFull(LPCRECT _pRect, int _nWidth, int _nHeight)
{
	return _pRect && _pRect->left == 0 && _pRect->top == 0
		&& (PRECT_W(_pRect) == _nWidth || PRECT_W(_pRect) == 0)
		&& (PRECT_H(_pRect) == _nHeight || PRECT_H(_pRect) == 0);
}

inline RECT Rect_Scale(LPCRECT _pRect, double _dblScaleX, double _dblScaleY )
{
	RECT rcRes = {};
	if (_pRect)
	{
		rcRes = *_pRect;
		rcRes.left = DBL2INT(rcRes.left * _dblScaleX);
		rcRes.right = DBL2INT(rcRes.right * _dblScaleX);
		rcRes.top = DBL2INT(rcRes.top * _dblScaleY);
		rcRes.bottom = DBL2INT(rcRes.bottom * _dblScaleY);
	}

	return rcRes;
}



// 1 - flipped horz
// 2 - flipped vert
// 1 ^ 2 -> both
inline int Rect_IsFlipped(LPRECT _pRect, bool _bCorrectFlip)
{
	int nRes = 0;
	if (_pRect && _pRect->left > _pRect->right)
	{
		if( _bCorrectFlip )
			SwapVal(_pRect->left, _pRect->right);
		nRes |= FLIP_HORZ;
	}
	if (_pRect && _pRect->top > _pRect->bottom)
	{
		if (_bCorrectFlip)
			SwapVal(_pRect->top, _pRect->bottom);
		nRes |= FLIP_VERT;
	}

	return nRes;
}

// Correct:
// * zero rects, absolute empty rects:
// - _prcSource is NULL -> for NULL / absolute empty rects returned full rect 
// - _prcSource is not NULL -> for NULL return full rect, for absolute empty rects make rect with source size at left, top pos 
// * flipped rect:
// - _bCorrectFlip is true - remove flip
// - return current 

inline RECT Rect_Correct(LPCRECT _pRect, bool _bCorrectFlip, int _nWidthDef = 0, int _nHeightDef = 0, RECT* _prcSource = NULL, int* _pnFlipped = 0, bool _bCorrectOffscreen = false )
{
	int nFlipped = 0;
	RECT rcRes = { 0, 0, _nWidthDef, ABS(_nHeightDef) };
	if (!Rect_IsAbsoluteEmpty(_pRect))
	{
		ATLASSERT(_pRect);
		rcRes = *_pRect;
		nFlipped = Rect_IsFlipped(&rcRes, _bCorrectFlip);
	}
	else if (_pRect && !IS_RECT_AEMPTY( _prcSource) )
	{
		// e.g. for overlay -> specify only top-left pos
		rcRes = *_pRect;
		rcRes.right += PRECT_AW(_prcSource);
		rcRes.bottom += PRECT_AH(_prcSource);
	}

	if (_pnFlipped)
		*_pnFlipped = nFlipped;

	if (_bCorrectOffscreen)
	{
		rcRes.left = MAX(0, rcRes.left);
		rcRes.top = MAX(0, rcRes.top);

		if( _nWidthDef > 0 )
			rcRes.right = MIN(_nWidthDef, rcRes.right);
		if (_nHeightDef > 0)
			rcRes.bottom = MIN(_nHeightDef, rcRes.bottom);
	}

	return rcRes;
}

// TODO: Correction of rect size
inline RECT Rect_MoveInScreen(LPCRECT _pRect, LPCRECT _pRectScr)
{
	RECT rcRes = {};
	if (!_pRect || IS_RECT_EMPTY(_pRectScr))
		return _pRect ? *_pRect : rcRes;

	rcRes = *_pRect;

	int nOffscreen = 0;

	// Correct rect in bounds
	nOffscreen = _pRect->right - _pRectScr->right;
	rcRes.left -= MAX(nOffscreen, 0);
	rcRes.right -= MAX(nOffscreen, 0);

	nOffscreen = _pRect->bottom - _pRectScr->bottom;
	rcRes.top -= MAX(nOffscreen, 0);
	rcRes.bottom -= MAX(nOffscreen, 0);

	nOffscreen = _pRectScr->left - _pRect->left;
	rcRes.left += MAX(nOffscreen, 0);
	rcRes.right += MAX(nOffscreen, 0);

	nOffscreen = _pRectScr->top - _pRect->top;
	rcRes.top += MAX(nOffscreen, 0);
	rcRes.bottom += MAX(nOffscreen, 0);

// 	nOffscreen = _pRect->right - _pRectScr->right;
// 	rcRes.left -= MAX(nOffscreen / 2, 0);
// 	rcRes.right -= MAX(nOffscreen / 2, 0);
// 
// 	nOffscreen = _pRect->bottom - _pRectScr->bottom;
// 	rcRes.top -= MAX(nOffscreen / 2, 0);
// 	rcRes.bottom -= MAX(nOffscreen / 2, 0);

	return rcRes;
}

// Correct rects offscreen 
inline bool Rect_CorrectOffscreen(LPRECT _pRectSrc, LPRECT _pRectDst, SIZE* _pSizeSrc, SIZE* _pSizeDst)
{
	if (IS_RECT_EMPTY(_pRectSrc) || IS_RECT_EMPTY(_pRectDst))
		return false;

	double dblScaleX = (double)PRECT_W(_pRectDst) / PRECT_W(_pRectSrc);
	double dblScaleY = (double)PRECT_H(_pRectDst) / PRECT_H(_pRectSrc);

	// Correct src rect offscreen
	RECT rcSrc = *_pRectSrc;
	rcSrc.left = MAX(0, rcSrc.left);
	rcSrc.top = MAX(0, rcSrc.top);
	if (_pSizeSrc)
	{
		rcSrc.right = MIN(rcSrc.right, _pSizeSrc->cx);
		rcSrc.bottom = MIN(rcSrc.bottom, _pSizeSrc->cy);
	}

	// Correct output rect according to source rect changes
	RECT rcDest = *_pRectDst;
	rcDest.left += DBL2INT( (rcSrc.left - _pRectSrc->left) * dblScaleX );
	rcDest.top += DBL2INT((rcSrc.top - _pRectSrc->top) * dblScaleY);
	rcDest.right -= DBL2INT((_pRectSrc->right - rcSrc.right) * dblScaleX);
	rcDest.bottom -= DBL2INT((_pRectSrc->bottom - rcSrc.bottom) * dblScaleY);
	
	// Correct dst rect offscreen
	RECT rcDestC = rcDest;
	rcDestC.left = MAX(0, rcDestC.left);
	rcDestC.top = MAX(0, rcDestC.top);
	if (_pSizeDst)
	{
		rcDestC.right = MIN(rcDestC.right, _pSizeDst->cx);
		rcDestC.bottom = MIN(rcDestC.bottom, _pSizeDst->cy);
	}

	// Correct source rect according to dst rect changes
	rcSrc.left += DBL2INT((rcDestC.left - rcDest.left) / dblScaleX);
	rcSrc.top += DBL2INT((rcDestC.top - rcDest.top) / dblScaleY);
	rcSrc.right -= DBL2INT((rcDest.right - rcDestC.right) / dblScaleX);
	rcSrc.bottom -= DBL2INT((rcDest.bottom - rcDestC.bottom) / dblScaleY);

	*_pRectSrc = rcSrc;
	*_pRectDst = rcDestC;

	return !IS_RECT_EMPTY(&rcSrc) && !IS_RECT_EMPTY(&rcDestC);
}

// Note: 
// If _bFromScratch = true -> return rect with flip as set in _nFlip (ignore current RECT flip)
//	  _bFromScratch = false -> change flip for current rect.

// 1 - Horz flip
// 2 - Vert flip
inline RECT Rect_Flip(const RECT& _rcOriginal, int _nFlip, bool _bFromScratch )
{
	RECT rcRes = _rcOriginal;

	if(_bFromScratch)
		_nFlip ^= Rect_IsFlipped((LPRECT)&_rcOriginal, false);

	if (_nFlip & FLIP_HORZ)
		SwapVal(rcRes.left, rcRes.right);

	if (_nFlip & FLIP_VERT)
		SwapVal(rcRes.top, rcRes.bottom);

	return rcRes;
}

inline RECT Rect_Rotate( const RECT& _rcOriginal, double _dblRotateAngle, const POINT _ptOrigin )
{
	POINTF ptfOrigin = { (FLOAT)_ptOrigin.x, (FLOAT)_ptOrigin.y };

	POINTF ptEdgeSrc[4] = { { (float)_rcOriginal.left, (float)_rcOriginal.top },
	{ (float)_rcOriginal.right, (float)_rcOriginal.top },
	{ (float)_rcOriginal.right, (float)_rcOriginal.bottom },
	{ (float)_rcOriginal.left, (float)_rcOriginal.bottom } };

	POINTF ptEdgeRotate[4] = {};
	double dblAngleRad = _dblRotateAngle * M_PI / 180.0;
	RECT rcDest = { (LONG)ptfOrigin.x, (LONG)ptfOrigin.y, (LONG)ptfOrigin.x, (LONG)ptfOrigin.y };
	
	for( int n = 0; n < SIZEOF_ARRAY( ptEdgeSrc ); ++n )
	{
		ptEdgeSrc[n].x -= ptfOrigin.x;
		ptEdgeSrc[n].y -= ptfOrigin.y;

		ptEdgeRotate[n].x = ptEdgeSrc[n].x * cos( dblAngleRad ) -
			ptEdgeSrc[n].y * sin( dblAngleRad ) + ptfOrigin.x;
		ptEdgeRotate[n].y = ptEdgeSrc[n].x * sin( dblAngleRad ) +
			ptEdgeSrc[n].y * cos( dblAngleRad ) + ptfOrigin.y;

		rcDest.left = MIN( rcDest.left, ROUND( ptEdgeRotate[n].x ) );
		rcDest.top = MIN( rcDest.top, ROUND( ptEdgeRotate[n].y ) );
		rcDest.right = MAX( rcDest.right, ROUND( ptEdgeRotate[n].x ) );
		rcDest.bottom = MAX( rcDest.bottom, ROUND( ptEdgeRotate[n].y ) );
	}

	return rcDest;
}

inline RECT Rect_RotateCenter( const RECT& _rcOriginal, double _dblRotateAngle )
{
	POINT ptOrigin = { RECT_W(_rcOriginal) / 2, RECT_H(_rcOriginal) / 2 };

	return Rect_Rotate( _rcOriginal, _dblRotateAngle, ptOrigin );
}

/*
// pszVersion: buffer for write string with version/creation time/version type.
//			   Size of buffer should be at least 64 TCHAR symbols
//
// Sample for release version
//		1.2.0.9 (15-Mar-2010 12:25:33)
// Sample for debug version
//		1.2.0.9 (15-Mar-2010 12:25:33) [Debug]

inline LPTSTR __stdcall GetModuleVerString(LPTSTR pszVersion)
{
	ATLASSERT(pszVersion != NULL);
	if(pszVersion == NULL) return pszVersion;

	//---------------------------------
	VS_FIXEDFILEINFO fi = {0};
	GetModuleVerInfo(&fi, _Module.GetResourceInstance());

		TCHAR pszVersion[128];
		if(pszCompilationDate[4] == (TCHAR)' ')
		{
			wsprintf(pszVersion, _T("%i.%i.%i.%i (%c-%c%c%c-%c%c%c%c %s)"),
				*((WORD*)&fi.dwProductVersionMS + 1), *(WORD*)&fi.dwProductVersionMS,
				*((WORD*)&fi.dwProductVersionLS + 1), *(WORD*)&fi.dwProductVersionLS,
				pszCompilationDate[5],
				pszCompilationDate[0], pszCompilationDate[1], pszCompilationDate[2],
				pszCompilationDate[7], pszCompilationDate[8], pszCompilationDate[9], pszCompilationDate[10],
				pszCompilationTime);
		}
		else
		{
			wsprintf(pszVersion, _T("%i.%i.%i.%i (%c%c-%c%c%c-%c%c%c%c %s)"),
				*((WORD*)&fi.dwProductVersionMS + 1), *(WORD*)&fi.dwProductVersionMS,
				*((WORD*)&fi.dwProductVersionLS + 1), *(WORD*)&fi.dwProductVersionLS,
				pszCompilationDate[4], pszCompilationDate[5],
				pszCompilationDate[0], pszCompilationDate[1], pszCompilationDate[2],
				pszCompilationDate[7], pszCompilationDate[8], pszCompilationDate[9], pszCompilationDate[10],
				pszCompilationTime);
		}
#ifdef _DEBUG
		lstrcat(pszVersion, _T(" [Debug]"));
#endif
}
*/

#pragma warning(pop)

/////////////////////////////////////////////////////////////////////////////
#endif	 // __FUNCTIONS_H_
