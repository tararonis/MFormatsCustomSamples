//---------------------------------------------------------------------------
// MFunction.h : (extract basic func from MHelpers)
//
// Description: 
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

#ifndef _M_FUNCTIONS_H_INCLUDED_ZZ_
#define _M_FUNCTIONS_H_INCLUDED_ZZ_

#pragma once

#ifndef _LPCBYTE_DEFINED
#define _LPCBYTE_DEFINED
typedef const BYTE *LPCBYTE;
#endif
#ifndef _LPCVOID_DEFINED
#define _LPCVOID_DEFINED
typedef const VOID *LPCVOID;
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include <map>
using std::map;

#include <Strsafe.h>

typedef LONGLONG REFERENCE_TIME;

#include "DSDefines.h"
#include "../WinAPI/Macros.h"
#include "../WinAPI/Containers.h"
#include "../WinAPI/Functions.h"
#include "../WinAPI/wrappers.h"
#include "../WinAPI/Perf.h"

_declspec(selectany) extern const GUID LIBID_MPLATFORMLib_X = {0x92BEC1C8,0xEA18,0x45C7,0xB8,0xC1,0x5B,0x2C,0xC1,0xB5,0x31,0xDE};


// For same interface methods, but different names (e.g. MP & MF proxy) 
#define COM_INTERFACE_ENTRY_RENAME(new_interface,existing_interface)\
{&_ATL_IIDOF(new_interface), \
	offsetofclass(existing_interface, _ComMapClass), \
	_ATL_SIMPLEMAPENTRY},

#define DATA2STR( _pbData, _cbData ) (Data2StrW( (LPBYTE)_pbData, _cbData, true, alloca( sizeof(WCHAR) * (3 * _cbData + 2) ) ) )

// Unicode comversion macros
#define M_W2A( _str) (_str ? Wide2Char(_str, (LPSTR)alloca(lstrlenW(_str) * 4 + 4) ) : NULL )
#define M_A2W( _str) (_str ? Char2Wide(_str, (LPWSTR)alloca(lstrlenA(_str) * 4 + 4) ) : NULL )

#define M_W2A_CP( _str, _locale) (_str ? Wide2Char(_str, (LPSTR)alloca(lstrlenW(_str) * 4 + 4), -1, _locale ) : NULL )
#define M_A2W_CP( _str, _locale) (_str ? Char2Wide(_str, (LPWSTR)alloca(lstrlenA(_str) * 4 + 4), -1, _locale ) : NULL )
 
#define UNIQUE_ID( _prefix ) (UniqueID( _prefix, (LPWSTR)alloca(128) ))

#define FCC2KEY( fcc )  (Fcc2Key((fcc), (LPWSTR)alloca(32))) 

// e.g. audio.1
#define INDEXEDNAME( _name, _index ) ( IndexedName(_name, _index, (LPWSTR)alloca(MAX_PATH*2), L'.' ) )
#define INDEXEDNAME2( _name, _index, _postfix ) ( IndexedName(_name, _index, (LPWSTR)alloca(MAX_PATH*2), L'.', _postfix ) )
#define INDEXEDNAME_SF( _name, _index ) ( IndexedName(_name, _index, (LPWSTR)__STR_BUF, L'.' ) )
#define INDEXEDNAME2_SF( _name, _index, _postfix ) ( IndexedName(_name, _index, (LPWSTR)__STR_BUF, L'.', _postfix ) )
// e.g. Decklink SDI (1) 
#define INDEXEDDEVNAME( _name, _index ) ( IndexedName(_name, _index, (LPWSTR)alloca(MAX_PATH*2), L'(' ) )
// e.g. peer[1]
#define INDEXEDARRNAME( _name, _index ) ( IndexedName(_name, _index, (LPWSTR)alloca(MAX_PATH*2), L'[' ) )
#define INDEXEDARRNAME2( _name, _index, _postfix ) ( IndexedName(_name, _index, (LPWSTR)alloca(MAX_PATH*2), L'[', _postfix ) )
// e.g. track='1'
#define NAMEVALUE( _name, _str_value ) ( NameValueStr(_name, _str_value, (LPWSTR)alloca( ( 8 + (_name ? lstrlenW(_name) : 0) + (_str_value ? lstrlenW(_str_value) : 0) ) * sizeof(WCHAR) ) ) )
#define NAMEVALUEINT( _name, _int_value ) ( NameValueStr(_name, INT2STRW(_int_value), (LPWSTR)alloca( ( 32 + (_name ? lstrlenW(_name) : 0) ) * sizeof(WCHAR) ) ) )
// e.g. peer[idrVga1w2]
#define KEYEDNAME( _name, _key ) ( KeyedName(_name, _key, (LPWSTR)alloca(MAX_PATH*2), L'[' ) )
#define KEYEDNAME2( _name, _key, _postfix ) ( KeyedName(_name, _key, (LPWSTR)alloca(MAX_PATH*2), L'[', _postfix ) )

#define STR_APPEND( _pszFirst, _pszSecond ) ( StrAppendW( _pszFirst, _pszSecond, ALLOCA_ARRAY( WCHAR, STR_LEN(_pszFirst) + STR_LEN(_pszSecond) + 2 ), -1 ) )
#define STR_APPEND_SF( _pszFirst, _pszSecond ) ( StrAppendW( _pszFirst, _pszSecond, (LPWSTR)__STR_BUF, __STR_BUF_LEN ) )

#define STR_APPEND3( _pszFirst, _pszSecond, _pszThird ) ( StrAppendW( _pszFirst, _pszSecond, ALLOCA_ARRAY( WCHAR, STR_LEN(_pszFirst) + STR_LEN(_pszSecond) + STR_LEN(_pszThird) + 2 ), STR_LEN(_pszFirst) + STR_LEN(_pszSecond) + STR_LEN(_pszThird) + 2, _pszThird ) )
#define STR_APPEND3_SF( _pszFirst, _pszSecond, _pszThird ) ( StrAppendW( _pszFirst, _pszSecond, (LPWSTR)__STR_BUF, __STR_BUF_LEN, _pszThird ) )



#if defined(_DEBUG) && defined(_MSC_VER)
// For set thread name: https://msdn.microsoft.com/en-us/library/xcb2z8hs(VS.90).aspx

#pragma pack(push,8)
struct VC_THREADNAME_INFO
{
	DWORD dwType; // Must be 0x1000.
	LPCSTR szName; // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags; // Reserved for future use, must be zero.
};
#pragma pack(pop)

inline void MSetThreadName( DWORD dwThreadID, char* _threadName, LPCWSTR _prefix = NULL)
{
	return;
//	char* pszName = (char*)alloca( 1024 );
// 	if( _prefix && _prefix[0] )
// 	{
// 		lstrcpyA( pszName, W2A(_prefix) );
// 		lstrcatA( pszName, "::" );
// 		lstrcatA( pszName, _threadName );
// 	}
// 	else
// 	{
// 		lstrcpyA( pszName, _threadName );
// 	}

	VC_THREADNAME_INFO info = {};
	info.dwType = 0x1000;
	info.szName = _threadName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;

	const DWORD MS_VC_EXCEPTION= 0x406D1388;

	__try
	{
		RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
	}
}
#else
inline void MSetThreadName( DWORD dwThreadID, char* threadName, LPCWSTR _prefix = NULL)
{
	ATLASSERT( !"MSetThreadName" );
}
#endif


#ifdef _DEBUG

#include <Psapi.h>
#pragma comment(lib,"Psapi.lib")

inline void Debug_Memory( LONG lLevel, LPCWSTR _pszInfo = NULL )
{
	PROCESS_MEMORY_COUNTERS psMemCounters = {};
	psMemCounters.cb = sizeof(PROCESS_MEMORY_COUNTERS);
	::GetProcessMemoryInfo( GetCurrentProcess(), &psMemCounters, sizeof(PROCESS_MEMORY_COUNTERS) );


	ATLTRACE2( atlTraceGeneral, lLevel, _T("[MEM] PageFault:%d WorkingSet:%s/%s PagedPoolUsage:%s/%s NonPagedPoolUsage:%s/%s PagefileUsage:%s/%s [%ws]\n"), 
		psMemCounters.PageFaultCount, 
		BYTES2STR(psMemCounters.WorkingSetSize), BYTES2STR(psMemCounters.PeakWorkingSetSize), BYTES2STR(psMemCounters.QuotaPagedPoolUsage), BYTES2STR(psMemCounters.QuotaPeakPagedPoolUsage), 
		BYTES2STR(psMemCounters.QuotaNonPagedPoolUsage), BYTES2STR(psMemCounters.QuotaPeakNonPagedPoolUsage), BYTES2STR(psMemCounters.PagefileUsage), BYTES2STR(psMemCounters.PeakPagefileUsage),
		_pszInfo ? _pszInfo : L"Debug_Memory" );
}
#else
inline void Debug_Memory( LONG lLevel, LPCWSTR _pszInfo = NULL ) {}
#endif

struct MEM_COPY_CB
{
	LPVOID pvSource;
	LPVOID pvDest;
	DWORD cbBytes;
	HANDLE hDone;
};

#ifndef MEMCPY_OPTIMIZE
#define MEMCPY_OPTIMIZE  1
#endif

#if (MEMCPY_OPTIMIZE > 0)
#include "../asm/SIMD64.h"
#define CopyMemoryX SIMDCopyMem
#else
#define CopyMemoryX CopyMemory
#endif


inline DWORD WINAPI CopyMemoryMP_Callback( LPVOID _pvUser )
{
	MEM_COPY_CB* pCopy = (MEM_COPY_CB*)_pvUser;
	CopyMemoryX( pCopy->pvDest, pCopy->pvSource, pCopy->cbBytes );
	if( pCopy->hDone )
		::SetEvent( pCopy->hDone );

	return 0;
}

inline void CopyMemoryMP( void* pDst, const void* pSrc, size_t cb, int _nThreads = 2 )
{
	if( cb > 1024 * 1024 && _nThreads > 1 )
	{
#ifdef _DEBUG
		TCHAR szInfo[128]={};
		_stprintf( szInfo, _T("CopyMemoryMP(%I64X->%I64X, cb:%zd, Threads:%d)"),
			(ULONGLONG)pSrc, (ULONGLONG)pDst, cb, _nThreads );

		CBasicPerf perf(szInfo);
#endif

		DWORD cbCopy = (DWORD)cb / _nThreads;
		cbCopy = ALIGN_UP( cbCopy, 512 );

		LPBYTE pbSrc = (LPBYTE)pSrc;
		LPBYTE pbDst = (LPBYTE)pDst;
		MEM_COPY_CB* pCopy = (MEM_COPY_CB*)alloca( sizeof(MEM_COPY_CB) * (_nThreads - 1) );
		HANDLE* phWait = (HANDLE*)alloca( sizeof(HANDLE) * (_nThreads - 1) );
		for( int i = 0; i < _nThreads - 1; i++ )
		{
			pCopy[i].pvDest = pbDst;
			pCopy[i].pvSource = pbSrc;
			pCopy[i].cbBytes = cbCopy;
			pCopy[i].hDone = phWait[i] = ::CreateEvent( NULL, TRUE, FALSE, NULL );

			pbDst += cbCopy;
			pbSrc += cbCopy;
			cb -= cbCopy;

			::QueueUserWorkItem( CopyMemoryMP_Callback, pCopy + i, 0 );
		}

		CopyMemoryX( pbDst, pbSrc, (DWORD)cb );
		::WaitForMultipleObjects( _nThreads - 1, phWait, TRUE, 10000 );
		for( int i = 0; i < _nThreads - 1; i++ )
		{
			::CloseHandle( pCopy[i].hDone );
		}
	}
	else
	{
		::CopyMemory( pDst, pSrc, cb );
	}
}

inline static void CopyMemory_C4R_Shift2Bits(LPCBYTE pSrc, int srcStep, LPBYTE pDst, int dstStep, SIZE roiSize, int _bShiftRight)
{
	ATLASSERT(ABS(srcStep) >= roiSize.cx && ABS(dstStep) >= roiSize.cx);
	
	if (ABS(srcStep) >= roiSize.cx && ABS(dstStep) >= roiSize.cx)
	{
		if (_bShiftRight)
		{
			for (int i = 0; i < roiSize.cy; i++)
			{
				SIMDCopyDWORD_Shift2Right(pDst, pSrc, roiSize.cx);
				pSrc += srcStep;
				pDst += dstStep;
			}
		}
		else
		{
			for (int i = 0; i < roiSize.cy; i++)
			{
				SIMDCopyDWORD_Shift2Left(pDst, pSrc, roiSize.cx);
				pSrc += srcStep;
				pDst += dstStep;
			}
		}

	}
}

inline static void CopyMemory_C1R( LPCBYTE pSrc, int srcStep, LPBYTE pDst, int dstStep, SIZE roiSize )
{
	ATLASSERT( ABS(srcStep) >= roiSize.cx && ABS(dstStep) >= roiSize.cx );
	if( ABS(srcStep) >= roiSize.cx && ABS(dstStep) >= roiSize.cx )
	{
		for( int i = 0; i < roiSize.cy; i++ )
		{
			CopyMemoryX( pDst, pSrc, roiSize.cx );
			pSrc += srcStep;
			pDst += dstStep;
		}
	}
}

inline LPCWSTR StrAppendW(LPCWSTR _pszFirst, LPCWSTR _pszSecond, LPWSTR _pszBuffer, int _nBufferSize, LPCWSTR _pszThird = NULL)
{
	ATLASSERT(_pszBuffer);
	if (!_pszBuffer) return NULL;

	if (!STR_IS_EMPTY(_pszFirst))
	{
		lstrcpynW(_pszBuffer, _pszFirst, _nBufferSize > 0 ? _nBufferSize : MAXSHORT);
	}
	else
	{
		_pszBuffer[0] = 0;
	}

	if (!STR_IS_EMPTY(_pszSecond))
		::StringCchCatW(_pszBuffer, _nBufferSize > 0 ? _nBufferSize : MAXSHORT, _pszSecond);

	if (!STR_IS_EMPTY(_pszThird))
		::StringCchCatW(_pszBuffer, _nBufferSize > 0 ? _nBufferSize : MAXSHORT, _pszThird);

	return _pszBuffer;
}

inline GUID StringAsGuid( LPCSTR _pszString )
{
	// Note: Have trailing 0 for CharLower()
	// {B8369C9B-2943-4BB1-AE0E-CEF5162F1900}
	GUID gRes = { 0xb8369c9b, 0x2943, 0x4bb1, { 0xae, 0xe, 0xce, 0xf5, 0x16, 0x2f, 0x19, 0x00 } };
	ATLASSERT(lstrlenA(_pszString) < sizeof(GUID) - 1);
	
	if (!STR_IS_EMPTY(_pszString))
		memcpy(&gRes, _pszString, MIN(sizeof(GUID) - 1, lstrlenA(_pszString)));
	else
		*(char*)&gRes = 0;

	// Note: Have trailing 0 for CharLower()
	::CharLowerA((char*)&gRes);
	return gRes;
}

inline char GetHexValue(char ch)
{
	if (ch >= '0' && ch <= '9')
		return (ch - '0');
	if (ch >= 'A' && ch <= 'F')
		return (ch - 'A' + 10);
	if (ch >= 'a' && ch <= 'f')
		return (ch - 'a' + 10);
	return -1;
}

inline char GetHexValueW(WCHAR ch)
{
	if (ch >= '0' && ch <= '9')
		return (char)(ch - L'0');
	if (ch >= 'A' && ch <= 'F')
		return (char)(ch - L'A' + 10);
	if (ch >= 'a' && ch <= 'f')
		return (char)(ch - L'a' + 10);
	return -1;
}

inline UINT MRand( UINT _nMax )
{
	UINT nVal = 0;
	rand_s( &nVal );

	return nVal % MAX( _nMax, 1 );
}

inline CComBSTR UniqueString(int _nLen, LPCWSTR _pszChars = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" )
{
	LPCWSTR pszChars = _pszChars;
	if( STR_IS_EMPTY(pszChars) )
		pszChars = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

	int nMaxChars = lstrlenW(pszChars);

	LPWSTR pszBuffer = ALLOCA_ARRAY(WCHAR, _nLen + 1);
	for (int i = 0; i < _nLen; i++)
	{
		UINT nVal = 0;
		rand_s(&nVal);
		pszBuffer[i] = pszChars[nVal % nMaxChars];
	}
	pszBuffer[_nLen] = 0;
	return pszBuffer;
}

inline UINT64 _mix64_inl(UINT64& a, UINT64& b, UINT64& c)
{
	a = a - b;  a = a - c;  a = a ^ (c >> 43);
	b = b - c;  b = b - a;  b = b ^ (a << 9);
	c = c - a;  c = c - b;  c = c ^ (b >> 8);
	a = a - b;  a = a - c;  a = a ^ (c >> 38);
	b = b - c;  b = b - a;  b = b ^ (a << 23);
	c = c - a;  c = c - b;  c = c ^ (b >> 5);
	a = a - b;  a = a - c;  a = a ^ (c >> 35);
	b = b - c;  b = b - a;  b = b ^ (a << 49);
	c = c - a;  c = c - b;  c = c ^ (b >> 11);
	a = a - b;  a = a - c;  a = a ^ (c >> 12);
	b = b - c;  b = b - a;  b = b ^ (a << 18);
	c = c - a;  c = c - b;  c = c ^ (b >> 22);
	return c;
};


inline UINT64 Str2Hash64_I(LPCWSTR _pszStr, bool _bIgnoreCase, int _nLen = -1)
{
	if (!_pszStr)
		return 0;

	LPCWSTR pszStr = _pszStr;

	int n64Pos = 0;
	UINT64 arr64Values[3] = { 0xA518C95D1B2E3917,0x9e3779b9A6C517EB,0x6A81C43A71D2E082 };

	char chLowCase = _bIgnoreCase ? 0x20 : 0x00;

	UINT64 n64Convert = 0;
	char* pConvert = (char*)&n64Convert;
	int nPos = 0;
	while (*pszStr && (_nLen < 0 || pszStr < _pszStr + _nLen))
	{
		pConvert[nPos] = (char)(*pszStr) | chLowCase;
		pszStr++;
		nPos++;
		if (nPos >= 8)
		{
			arr64Values[n64Pos] += n64Convert;
			nPos = 0;
			n64Convert = 0;

			n64Pos++;
			if (n64Pos >= 3)
			{
				_mix64_inl(arr64Values[0], arr64Values[1], arr64Values[2]);
				n64Pos = 0;
			}
		}
	}

	arr64Values[n64Pos] += n64Convert;

	return _mix64_inl(arr64Values[0], arr64Values[1], arr64Values[2]);
};

inline UINT64 Data2Hash64_I(void* _pvData, DWORD _cbData)
{
	if (!_pvData || _cbData == 0)
		return 0;

	int n64Pos = 0;
	UINT64 arr64Values[3] = { 0xA518C95D1B2E3917,0x9e3779b9A6C517EB,0x6A81C43A71D2E082 };

	LPBYTE pbData = (LPBYTE)_pvData;

	UINT64 n64Convert = 0;
	LPBYTE pConvert = (LPBYTE)&n64Convert;
	int nPos = 0;
	for( UINT i = 0; i < _cbData; i++)
	{
		pConvert[nPos] = pbData[i];
		nPos++;
		if (nPos >= 8)
		{
			arr64Values[n64Pos] += n64Convert;
			nPos = 0;
			n64Convert = 0;

			n64Pos++;
			if (n64Pos >= 3)
			{
				_mix64_inl(arr64Values[0], arr64Values[1], arr64Values[2]);
				n64Pos = 0;
			}
		}
	}

	arr64Values[n64Pos] += n64Convert;

	return _mix64_inl(arr64Values[0], arr64Values[1], arr64Values[2]);
};



inline GUID GUIDFromStringW( LPCWSTR _szGuid )
{
	GUID gRes = {0};
	if( !_szGuid ) return gRes;

	BYTE* pGuid = (LPBYTE)&gRes;
	int nPos = 3; // First DWORD
	const WCHAR* pszGuid = _szGuid;
	while( pszGuid[0] && pszGuid[1] && nPos < 16 )
	{
		char ch1 = GetHexValueW(pszGuid[0]);
		char ch2 = GetHexValueW(pszGuid[1]);
		if( ch1 >= 0 && ch2 >= 0 )
		{
			pGuid[nPos] = (BYTE)(16*ch1+ch2);

			// DWORD, WORD, WORD, BYTES
			if( nPos < 8 )
				nPos--;
			else 
				nPos++;

			if( nPos == -1 ) // First DWORD -> First WORD
				nPos = 5;	
			else if( nPos == 3 ) // First WORD -> Second WORD
				nPos = 7;	
			else if( nPos == 5 ) // Second WORD -> BYTES
				nPos = 8;	


			pszGuid += 2;
		}
		else
		{
			pszGuid++;
		}
	}

	return nPos == 16 ? gRes : GUID_NULL;
}

inline GUID GUIDFromStringA( const char* _szGuid )
{
	GUID gRes = {0};
	if( !_szGuid ) return gRes;

	BYTE* pGuid = (LPBYTE)&gRes;
	int nPos = 3; // First DWORD
	const char* pszGuid = _szGuid;
	while( pszGuid[0] && pszGuid[1] && nPos < 16 )
	{
		char ch1 = GetHexValue(pszGuid[0]);
		char ch2 = GetHexValue(pszGuid[1]);
		if( ch1 >= 0 && ch2 >= 0 )
		{
			pGuid[nPos] = (BYTE)(16*ch1+ch2);

			// DWORD, WORD, WORD, BYTES
			if( nPos < 8 )
				nPos--;
			else 
				nPos++;

			if( nPos == -1 ) // First DWORD -> First WORD
				nPos = 5;	
			else if( nPos == 3 ) // First WORD -> Second WORD
				nPos = 7;	
			else if( nPos == 5 ) // Second WORD -> BYTES
				nPos = 8;	


			pszGuid += 2;
		}
		else
		{
			pszGuid++;
		}
	}

	return nPos == 16 ? gRes : GUID_NULL;
}

template <class T>
inline int ArrayIndex( const T& _value, const T* _checkArray, T _stop_val = NULL )
{
	for( int i = 0; _checkArray[i] != _stop_val; i++ )
	{
		if( _checkArray[i] == _value )
			return i;
	}

	return -1;
}

inline LPCWSTR Fcc2Key( DWORD _dwFCC, LPWSTR _pszBuff )
{
	LPBYTE pchFCC = (LPBYTE)&_dwFCC;
	if( isprint( pchFCC[0] ) && isprint( pchFCC[1] ) && isprint( pchFCC[2] ) && isprint( pchFCC[3] ) )
	{
		if( ::MultiByteToWideChar( CP_ACP, 0, (LPCSTR)pchFCC, 4, _pszBuff, 8 ) <= 0 )
			wsprintfW( _pszBuff, L"!ERR" );

		_pszBuff[4] = 0;
	}
	else
	{
		wsprintfW( _pszBuff, L"0x%08X", _dwFCC );
	}
	
	return _pszBuff;
}

inline DWORD Key2Fcc( LPCWSTR _pszFCC )
{
	DWORD dwRes = 0;
	if( _pszFCC )
	{
		int nLen = lstrlenW( _pszFCC );
		if( nLen > 4 && memcmp( _pszFCC, L"0x", 4 ) == 0  )
		{
			dwRes = wcstol( _pszFCC, NULL, 16);
		}
		else if( nLen > 4 && (_pszFCC[0] == L'#' || _pszFCC[0] == L'{') )
		{
			dwRes = wcstol( _pszFCC + 1, NULL, 16); // The { for GUID -> use only first DWORD
		}
		else if( nLen > 0 ) // Assume zero postfix > 4 or 5 valid
		{
			::WideCharToMultiByte( CP_ACP, 0, _pszFCC, nLen, (LPSTR)&dwRes, 4, NULL, NULL );
//			dwRes = SwapDW( *(DWORD*)_pszFCC );
		}
// 		else if( nLen > 0 ) // Assume zero postfix > 2 or 3 valid
// 		{
// 			dwRes = SwapW( *(short*)_pszFCC );
// 		}
	}
	return dwRes;
}


struct MRational
{
	int nNum;  // e.g. 30000
	int nDen;  // e.g. 1001
};


template <class TEnum>
inline TEnum PutFlag( TEnum* _peFlags, TEnum _eFlag )
{
	ATLASSERT( _peFlags );
	*_peFlags = (TEnum)( (int)(*_peFlags) | _eFlag );
	return *_peFlags;
}

template <class TEnum>
inline TEnum RemoveFlag( TEnum* _peFlags, TEnum _eFlag )
{
	ATLASSERT( _peFlags );
	*_peFlags = (TEnum)( (int)(*_peFlags) & ~_eFlag );
	return *_peFlags;
}

inline LPCWSTR UniqueID( LPCWSTR _pszPrexix, LPWSTR _pszBuff )
{
	wsprintfW( _pszBuff, L"%ws-%I64X", __rdtsc() );
	return _pszBuff;
}

static MRational Dbl2Rational( double _dblRate )
{
	MRational res = {};
	if( ABS(FRACT(_dblRate)) < 0.001)
	{
		res.nNum = (int)(_dblRate);
		res.nDen = 1;
	}
	else if( ABS(FRACT(_dblRate * 1001/1000)) < 0.001 )
	{
		// drop frame
		res.nNum = (int)(_dblRate * 1001);
		res.nDen = 1001;
	}
	else 
	{
		res.nNum = (int)(_dblRate * 1000);
		res.nDen = 1000;
	}

	return res;
}

//!!! clone of FFMUtils::Rate2Dbl(...)
template <class TRational>
inline static double Rational2Dbl( const TRational* pRat, bool _bInverse = false )
{
	if( _bInverse )
	{
		ATLASSERT( pRat && pRat->nDen );
		if( pRat && pRat->nDen )
			return (double)pRat->nNum / pRat->nDen;
	}
	else
	{
		ATLASSERT( pRat && pRat->nNum );
		if( pRat && pRat->nNum )
			return (double)pRat->nDen / pRat->nNum;
	}

	return 0;
}

static LONGLONG TimestampRescale_I(LONGLONG a, LONGLONG b, LONGLONG c)
{	
 	LONGLONG r = c/2;
 	ATLASSERT(c > 0);
 	ATLASSERT(b >= 0);	
 
 	if(a<0 && a != LONGLONG_MIN) return -TimestampRescale_I(-a, b, c);
 
 	if(b <= INT_MAX && c<= INT_MAX){
 		if(a <= INT_MAX)
 			return (a * b + r)/c;
 		else
 			return a/c*b + (a%c*b + r)/c;
 	}
 	
 	ULONGLONG a0= a&0xFFFFFFFF;
 	ULONGLONG a1= a>>32;
 	ULONGLONG b0= b&0xFFFFFFFF;
 	ULONGLONG b1= b>>32;
 	ULONGLONG t1= a0*b1 + a1*b0;
 	ULONGLONG t1a= t1<<32;
 	int i;
 
 	a0 = a0*b0 + t1a;
 	a1 = a1*b1 + (t1>>32) + (a0<t1a);
 	a0 += r;
 	a1 += (LONGLONG)a0<r;
 
 	for(i=63; i>=0; i--)
	{ 		
 		a1+= a1 + ((a0>>i)&1);
 		t1+=t1;
 		if(c <= (LONGLONG)a1){
 			a1 -= c;
 			t1++;
 		}
 	}
 	return t1; 	
}

template <class TRational>
static LONGLONG TimestampRescale(LONGLONG _llTimestamp, TRational* _pSrcTimebase, TRational* _pDestTimebase)
{
 	LONGLONG llNum = (LONGLONG)_pSrcTimebase->nNum * _pDestTimebase->nDen;
 	LONGLONG llDen = (LONGLONG)_pDestTimebase->nNum * _pSrcTimebase->nDen;
 	return TimestampRescale_I(_llTimestamp, llNum, llDen);
}

inline UINT SummBits( UINT _nValue )
{
	UINT nRes = 0;
	while( _nValue > 0 )
	{
		nRes += (_nValue & 1);
		_nValue >>= 1;
	}

	return nRes;
}

inline double CorrectAverageTimeM( REFERENCE_TIME& _rtTime, bool* _pbCorrect = false )
{
	if( _pbCorrect )
		*_pbCorrect = true;

	if( _rtTime >= 166600 && _rtTime <= 166700 ) 
	{
		_rtTime = 166667; // 60 fps
		return (double)DS_ONESEC / 60;
	}
	if( _rtTime >= 166800 && _rtTime <= 166900 ) 
	{
		_rtTime = 166833; // 59.94 fps
		return (double)DS_ONESEC * 1001 / 60000;
	}
	if( _rtTime >= 199950 && _rtTime <= 200050 ) 
	{
		_rtTime = 200000; // 50.00 fps
		return (double)_rtTime;
	}
	if( _rtTime >= 333300 && _rtTime <= 333400 ) 
	{
		_rtTime = 333333; // 30 fps
		return (double)DS_ONESEC / 30;
	}
	if( _rtTime >= 333600 && _rtTime <= 333700 ) 
	{
		_rtTime = 333667; // 29.97 fps
		return (double)DS_ONESEC * 1001 / 30000;
	}
	if( _rtTime >= 399950 && _rtTime <= 400050 )
	{
		_rtTime = 400000; // 25.00 fps
		return (double)_rtTime;
	}
	if( _rtTime >= 416600 && _rtTime <= 416700 )
	{
		_rtTime = 416667; // 24.00 fps
		return (double)DS_ONESEC / 24;
	}
	if( _rtTime >= 417000 && _rtTime <= 417200 ) 
	{
		_rtTime = 417083; // 23.98 fps
		return (double)DS_ONESEC * 1001 / 24000;
	}

	if( _pbCorrect )
		*_pbCorrect = false;

	return (double)_rtTime;
}

inline double AvgTime2Rate( REFERENCE_TIME _rtAvgTimePerFrame, bool _bCorrectRate = true, double dblDefRate = 0 )
{
	double dblAvgTime = _bCorrectRate ? CorrectAverageTimeM( _rtAvgTimePerFrame) : _rtAvgTimePerFrame;

	return dblAvgTime != 0 ? DS_ONESEC / ABS(dblAvgTime) : dblDefRate;
}

inline REFERENCE_TIME Rate2AvgTime( double _dblRate, REFERENCE_TIME _rtDefAvgTimePerFrame = 0)
{
	REFERENCE_TIME rtAvgTime = _dblRate != 0 ? DBL2INT64( DS_ONESEC / ABS(_dblRate) ) : _rtDefAvgTimePerFrame;
	CorrectAverageTimeM(rtAvgTime);
	return rtAvgTime;
}

inline double Rate2AvgTimeDbl( double _dblRate, double _dblDefAvgTimePerFrame = 0)
{
	double dblAvgTime = _dblRate > 0 ? DS_ONESEC / ABS(_dblRate) : _dblDefAvgTimePerFrame;
	REFERENCE_TIME rtAvgTime = DBL2INT64(dblAvgTime);
	bool bCorrected = false;
	double dblAvgTimeC = CorrectAverageTimeM(rtAvgTime, &bCorrected );

	return bCorrected ? dblAvgTimeC : dblAvgTime;
}

template <class TRational>
inline static REFERENCE_TIME Time2REF( LONGLONG _llTime, const TRational* pTimeBase )
{
	if( _llTime == MINLONGLONG )
		return MINLONGLONG;

	if( !pTimeBase || !pTimeBase->nDen || !pTimeBase->nNum )
	{
		// If time_base not specified -> assume reference time 100 nsec units 
		return _llTime;
	}

	return DBL2INT64((double)_llTime * DS_ONESEC * pTimeBase->nNum / pTimeBase->nDen);
}

template <class TRational>
inline static LONGLONG REF2Time( REFERENCE_TIME _rtTime, const TRational* pTimeBase )
{
	if( _rtTime == MINLONGLONG )
		return MINLONGLONG;

	if( !pTimeBase || !pTimeBase->nDen || !pTimeBase->nNum )
	{
		// If time_base not specified -> assume reference time 100 nsec units 
		return _rtTime;
	}

	return DBL2INT64((double)_rtTime * pTimeBase->nDen / (DS_ONESEC * pTimeBase->nNum));
}

// From FFmpeg
enum MRounding {
    M_ROUND_ZERO     = 0, ///< Round toward zero.
    M_ROUND_INF      = 1, ///< Round away from zero.
    M_ROUND_DOWN     = 2, ///< Round toward -infinity.
    M_ROUND_UP       = 3, ///< Round toward +infinity.
    M_ROUND_NEAR_INF = 5, ///< Round to nearest and halfway cases away from zero.
    /**
     * Flag telling rescaling functions to pass `INT64_MIN`/`MAX` through
     * unchanged, avoiding special cases for #M_NOPTS_VALUE.
     *
     * Unlike other values of the enumeration MRounding, this value is a
     * bitmask that must be used in conjunction with another value of the
     * enumeration through a bitwise OR, in order to set behavior for normal
     * cases.
     *
     * @code{.c}
     * av_rescale__round(3, 1, 2, M_ROUND_UP | M_ROUND_PASS_MINMAX);
     * // Rescaling 3:
     * //     Calculating 3 * 1 / 2
     * //     3 / 2 is rounded up to 2
     * //     => 2
     *
     * av_rescale__round(M_NOPTS_VALUE, 1, 2, M_ROUND_UP | M_ROUND_PASS_MINMAX);
     * // Rescaling M_NOPTS_VALUE:
     * //     M_NOPTS_VALUE == INT64_MIN
     * //     M_NOPTS_VALUE is passed through
     * //     => M_NOPTS_VALUE
     * @endcode
     */
    M_ROUND_PASS_MINMAX = 8192,
};

// a * b / c
inline static LONGLONG MulDiv64(LONGLONG a, LONGLONG b, LONGLONG c, enum MRounding _round = M_ROUND_NEAR_INF)
{
	LONGLONG r = 0;
	ATLASSERT(c > 0);
	ATLASSERT(b >=0);
	ATLASSERT((unsigned)(_round&~M_ROUND_PASS_MINMAX)<=5 && (_round&~M_ROUND_PASS_MINMAX)!=4);

	if (c <= 0 || b < 0 || !((unsigned)(_round&~M_ROUND_PASS_MINMAX)<=5 && (_round&~M_ROUND_PASS_MINMAX)!=4))
		return INT64_MIN;

	if (_round & M_ROUND_PASS_MINMAX) {
		if (a == INT64_MIN || a == INT64_MAX)
			return a;
		_round = MRounding((int)_round - M_ROUND_PASS_MINMAX);
	}

	if (a < 0 && a != INT64_MIN)
		return -1 * MulDiv64(-a, b, c, MRounding(_round ^ ((_round >> 1) & 1)) );

	if (_round == M_ROUND_NEAR_INF)
		r = c / 2;
	else if (_round & 1)
		r = c - 1;

	if (b <= INT_MAX && c <= INT_MAX) {
		if (a <= INT_MAX)
			return (a * b + r) / c;
		else
			return a / c * b + (a % c * b + r) / c;
	} else {

		ULONGLONG a0  = a & 0xFFFFFFFF;
		ULONGLONG a1  = a >> 32;
		ULONGLONG b0  = b & 0xFFFFFFFF;
		ULONGLONG b1  = b >> 32;
		ULONGLONG t1  = a0 * b1 + a1 * b0;
		ULONGLONG t1a = t1 << 32;

		a0  = a0 * b0 + t1a;
		a1  = a1 * b1 + (t1 >> 32) + (a0 < t1a);
		a0 += r;
		a1 += (LONGLONG)a0 < r;

		for (int i = 63; i >= 0; i--) {
			a1 += a1 + ((a0 >> i) & 1);
			t1 += t1;
			if (c <= (LONGLONG)a1) {
				a1 -= c;
				t1++;
			}
		}
		return t1;
	}
}

template <class TRational>
static LONGLONG Rescale64(LONGLONG val, TRational raSrc, TRational raDst)
{
	if( val == MINLONGLONG )
		return MINLONGLONG;

	LONGLONG b = raSrc.nNum * (LONGLONG)raDst.nDen;
	LONGLONG c = raDst.nNum * (LONGLONG)raSrc.nDen;
	return MulDiv64(val, b, c,M_ROUND_NEAR_INF);
}



inline LPSTR Wide2Char( LPCWSTR _pszWide, LPSTR _pszChar, int _nLen = -1, UINT _cPage = CP_ACP )
{
	ATLASSERT( _pszWide && _pszChar );
	if( !_pszWide || !_pszChar ) return NULL;
	// Use x2 as for e.g. Chinese language 
	int nChars = ::WideCharToMultiByte( _cPage, WC_COMPOSITECHECK|WC_DISCARDNS, _pszWide, _nLen, _pszChar, _nLen > 0 ? _nLen : (lstrlenW(_pszWide) * 4 + 4), NULL, NULL );
	if( !nChars )
		nChars = ::WideCharToMultiByte( _cPage, 0, _pszWide, _nLen, _pszChar, _nLen > 0 ? _nLen : (lstrlenW(_pszWide) * 4 + 4), NULL, NULL );
	_pszChar[nChars] = 0;
	return _pszChar;
}

inline LPCWSTR Char2Wide( LPCSTR _pszChar, LPWSTR _pszWide, int _nLen = -1, UINT _cPage = CP_ACP )
{
	ATLASSERT( _pszWide && _pszChar );
	if( !_pszWide || !_pszChar ) return NULL;

	int nChars = ::MultiByteToWideChar( _cPage, MB_PRECOMPOSED, _pszChar, _nLen, _pszWide, _nLen > 0 ? _nLen : (lstrlenA(_pszChar) + 1) );
	if( !nChars )
		nChars = ::MultiByteToWideChar( _cPage, 0, _pszChar, _nLen, _pszWide, _nLen > 0 ? _nLen : (lstrlenA(_pszChar) + 1) );
	_pszWide[nChars] = 0;
	return _pszWide;
}

// Simple test for unicode file
static int TestUnicode( LPBYTE _pbData, DWORD _cbData, double _dblAnalize = 0.9 )
{
	if( _cbData > 1 && _pbData[0] == 0xFF && _pbData[1] == 0xFE )
		return 2; // 1200

	if( _cbData > 1 && _pbData[0] == 0xFE && _pbData[1] == 0xFF )
		return -2; // 1201 (Big-Endian)

	int nCheck = DBL2INT(_dblAnalize * _cbData) - 1;
	nCheck = MIN( nCheck, (int)_cbData - 1);
	for( int i = 0 ; i < nCheck / 2; i++ )
	{
		if( _pbData[i*2] == 0 ) 
			return -1; // 1201

		if( _pbData[i*2+1] == 0 ) 
			return 1; // 1200
	}

	return 0;
}

inline UINT GetEncodingType( char* _pszText )
{
	// Check for UTF-7, 8
	int nCheckSize = 4096;
	char* pszText = (char*)alloca( nCheckSize );
	lstrcpynA( pszText, (char*)_pszText, 4096 );
	
	char* pszEnd = StrStrA( pszText, "?>" );
	if( pszEnd ) 
	{
		*pszEnd = 0;

		if( StrStrIA( pszText, "utf-8") ) 
			return CP_UTF8;

		if( StrStrIA( pszText, "utf-7") ) 
			return CP_UTF8;
	}

	return (UINT)-1;
}

// Detect string encoding and convert to unicode
// The _pszWide allocated size should be at least 4 times + 2 bytes bigger than input len.
inline int Text2Wide( LPBYTE _pbText, DWORD _cbText, LPWSTR _pszWide )
{
	ATLASSERT( _pszWide && _pbText );
	if( !_pszWide || !_pbText ) return 0;

	// http://stackoverflow.com/questions/4520184/how-to-detect-the-character-encoding-of-a-text-file

	int cbText = _cbText > 0 ? _cbText : lstrlenA((LPCSTR)_pbText);
	int nChars = 0;

	
	// Check for UTF-8
	if( (cbText > 2 && _pbText[0] == 0xEF && _pbText[1] == 0xBB && _pbText[2] == 0xBF) || GetEncodingType((char*)_pbText) == CP_UTF8 )
	{
		nChars = ::MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, (LPCSTR)_pbText, _cbText, _pszWide, cbText * 4 + 2 );
	}
	else if( (cbText > 2 && _pbText[0] == 0x2B && _pbText[1] == 0x2F && _pbText[2] == 0x76 ) || GetEncodingType((char*)_pbText) == CP_UTF7 )
	{
		nChars = ::MultiByteToWideChar( CP_UTF7, MB_ERR_INVALID_CHARS, (LPCSTR)_pbText, _cbText, _pszWide, cbText * 4 + 2 );
	}
	else if( cbText > 2 )
	{
		int nUnicode = TestUnicode( _pbText, _cbText );
		if( ABS( nUnicode ) >= 2 )
		{
			// BOF
			_pbText += 2;
			if( _cbText > 2 )
				_cbText -= 2;
		}

		if( nUnicode > 0 )
		{
			// Unicode 1200
			if( _cbText > 1 )
				lstrcpynW( _pszWide, (LPCWSTR)_pbText, _cbText / sizeof(WCHAR) + 1 );	
			else 
				lstrcpyW( _pszWide, (LPCWSTR)_pbText );	

			nChars = lstrlenW( _pszWide );
		}
		else if( nUnicode < 0 )
		{
			if( _cbText > 1 )
			{
				SwapWords( (LPBYTE)_pszWide, _pbText, _cbText / sizeof(WORD) );	
				_pszWide[ _cbText / sizeof(WORD) ] = 0;
			}
			else 
			{
				SwapWords( (LPBYTE)_pszWide, _pbText, -1 );	
			}

			nChars = lstrlenW( _pszWide );
		}
	}
	
	if( nChars == 0 )
		nChars = ::MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, (LPCSTR)_pbText, _cbText, _pszWide, cbText * 4 + 2 );
	if( nChars == 0 )
		nChars = ::MultiByteToWideChar( CP_ACP, MB_ERR_INVALID_CHARS, (LPCSTR)_pbText, _cbText, _pszWide, cbText * 4 + 2 );
	if( nChars == 0 )
		nChars = ::MultiByteToWideChar( CP_UTF7, MB_ERR_INVALID_CHARS, (LPCSTR)_pbText, _cbText, _pszWide, cbText * 4 + 2 );
	
	
	_pszWide[nChars] = 0;
	return nChars;
}

inline CComBSTR Text2WideStr( LPBYTE _pbText, DWORD _cbText )
{
	CComBSTR cbsRes;

	int cbText = _cbText > 0 ? _cbText : lstrlenA((LPCSTR)_pbText);
	LPBYTE pbData = new BYTE[cbText*4+2];
	int nChars = Text2Wide( _pbText, _cbText, (LPWSTR)pbData );
	if( nChars > 0 )
		cbsRes = (LPWSTR)pbData;

	delete [] pbData;
	return cbsRes;
}

inline LPCWSTR IndexedName( LPCWSTR _pszBaseName, int _nIndex, LPWSTR _pszBuffer, WCHAR _chSeparator = L'.', LPCWSTR _pszPostfix = NULL )
{
	if( _nIndex <= 0 && (STR_IS_EMPTY(_pszPostfix) || _nIndex  < 0 ) )
		return _pszBaseName;

	WCHAR _chSeparator2 = _chSeparator == L'(' ? L')' : 
		_chSeparator == L'[' ? L']' : 
		_chSeparator == L'{' ? L'}' : 
		_chSeparator == L'<' ? L'>' :
		_chSeparator;

	if (_pszPostfix == NULL)
		_pszPostfix = L"";

	if( _chSeparator == L'(' && STR_IS_EMPTY( _pszPostfix ) )
		wsprintfW( _pszBuffer, L"%ws (%d)", _pszBaseName, _nIndex );
	else if( STR_IS_EMPTY( _pszPostfix ) && _chSeparator == _chSeparator2 )
		wsprintfW( _pszBuffer, L"%ws%wc%d", _pszBaseName, _chSeparator, _nIndex );
	else
		wsprintfW( _pszBuffer, L"%ws%wc%d%wc%ws", _pszBaseName, _chSeparator, _nIndex, _chSeparator2, _pszPostfix );

	return _pszBuffer;
}

inline LPCWSTR KeyedName(LPCWSTR _pszBaseName, LPCWSTR _pszKey, LPWSTR _pszBuffer, WCHAR _chSeparator = L'[', LPCWSTR _pszPostfix = NULL)
{
	if (STR_IS_EMPTY(_pszKey) && STR_IS_EMPTY(_pszPostfix) )
		return _pszBaseName;

	if( STR_IS_EMPTY(_pszKey) )
		wsprintfW(_pszBuffer, L"%ws%ws", _pszBaseName, STR_IS_EMPTY(_pszPostfix) ? L"" : _pszPostfix );
	else if (_chSeparator == L'(')
		wsprintfW(_pszBuffer, L"%ws(%ws)%ws", _pszBaseName, _pszKey, STR_IS_EMPTY(_pszPostfix) ? L"" : _pszPostfix);
	else if (_chSeparator == L'[')
		wsprintfW(_pszBuffer, L"%ws[%ws]%ws", _pszBaseName, _pszKey, STR_IS_EMPTY(_pszPostfix) ? L"" : _pszPostfix);
	else if (_chSeparator == L'{')
		wsprintfW(_pszBuffer, L"%ws{%ws}%ws", _pszBaseName, _pszKey, STR_IS_EMPTY(_pszPostfix) ? L"" : _pszPostfix);

	return _pszBuffer;
}

inline LPCWSTR NameValueStr( LPCWSTR _pszName, LPCWSTR _pszValue, LPWSTR _pszBuffer )
{
	if( !STR_IS_EMPTY(_pszName) && _pszValue )
		wsprintfW( _pszBuffer, L"%ws='%ws'", _pszName, _pszValue );
	else
		_pszBuffer[0] = 0;

	return _pszBuffer;
}


inline double Accel( double dblPos, double _dblAccel = 1.0 )
{
	if( _dblAccel <= 0.0 )
		return dblPos;

	// 0->1 lin->cos
	// 1->xxx cos -> cos( exp)
	if( _dblAccel < 0 )
	{
		if( dblPos >= 0.5 )
		{
			dblPos = pow( dblPos * 2, -1 * _dblAccel + 1.0 ) / 2.0; 
		}
		else
		{
			dblPos = 1.0 - pow( (1 - dblPos) * 2, -1 * _dblAccel + 1.0 ) / 2.0; 
		}
	}
	else 
	{
		if( dblPos <= 0.5 )
		{
			dblPos = pow( dblPos * 2, _dblAccel + 1.0 ) / 2.0; 
		}
		else
		{
			dblPos = 1.0 - pow( (1 - dblPos) * 2, _dblAccel + 1.0) / 2.0; 
		}

		double dblSin = (1.0 - cos(M_PI * dblPos) ) / 2.0;
		if( _dblAccel < 1.0 )
		{
			dblPos = dblPos * (1.0 - _dblAccel) + dblSin * _dblAccel;
		}
		else
		{
			dblPos = dblSin;
		}
	}
	
	
	return dblPos;
}

//-1..1|1..-1
inline double Hounting( double dblPos, int _nAccel = 1 )
{

	// 	if( dblPos <= 0.5 )
	// 	{
	// 		dblPos = pow( dblPos * 2, _nAccel + 1) / 2.0; 
	// 		//dblPos = exp( dblPos * 2) / 2.0; 
	// 	}
	// 	else
	// 	{
	// 		//dblPos = 1.0 - exp( (1 - dblPos) * 2 ) / 2.0; 
	// 		dblPos = 1.0 - pow( (1 - dblPos) * 2, _nAccel + 1) / 2.0; 
	// 	}

	dblPos = cos(2 * M_PI * dblPos);

	return dblPos;
}

static bool MStrInList( LPCWSTR _pszName, const LPCWSTR* _ppNames )
{
	if( !_pszName ) return false;

	if( !_ppNames ) return true;

	const LPCWSTR* ppNames = _ppNames;
	while( *ppNames )
	{
		if( lstrcmpiW( _pszName, *ppNames ) == 0 )
			return true;

		ppNames++;
	}

	return false;
}

template <class T>
HRESULT MUnmarshallInterface( IStream* _pStream, T** _ppInterface)
{
	ATLTRACE2( atlTraceGeneral, 10, _T("::MUnmarshallInterface(%ws) Thread:%04X (%d) Stm:%08X\n"), CComBSTR( __uuidof(T) ), GetCurrentThreadId(), GetCurrentThreadId(), _pStream );

	if( !_pStream ) return E_INVALIDARG;
	if( !_ppInterface ) return E_POINTER;

	LARGE_INTEGER liZero = {0};
	_pStream->Seek( liZero, STREAM_SEEK_SET, NULL );
	
	return ::CoUnmarshalInterface( _pStream, __uuidof(T), (void**)_ppInterface );
}

template <class T>
HRESULT MMarshallInterface( T* _pInterface, IStream** _ppStream )
{
	ATLTRACE2( atlTraceGeneral, 10, _T("::MMarshallInterface(%ws) Thread:%04X (%d) Itf:%08X\n"), CComBSTR( __uuidof(T) ), 
		GetCurrentThreadId(), GetCurrentThreadId(), _pInterface );

	if( !_pInterface ) return E_INVALIDARG;

	if( !_ppStream ) return E_POINTER;

	// Create stream
	return ::CoMarshalInterThreadInterfaceInStream( __uuidof(T), _pInterface, _ppStream );
}

// VVB: WARNING !!! bWaitAll is very DANGEROUS as WAIT handles and events 
// - The function returns when all objects in the pHandles array are signaled and an input event has been received, all at the same time.
// https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-msgwaitformultipleobjectsex
inline DWORD WaitForMultipleObjects_MessageLoop( int nCount, HANDLE* lpHandles, BOOL bWaitAll, DWORD dwMilliseconds )
{
	ATLASSERT(!bWaitAll && "WaitForMultipleObjects_MessageLoop - WAIT ALL is DANGEROUS - see comments in code");
	bool bWait = true;
	while(bWait)
	{
		DWORD dwStart = GetTickCount();
		DWORD dwRet = ::MsgWaitForMultipleObjectsEx(nCount, lpHandles, dwMilliseconds, QS_ALLINPUT, (bWaitAll ? MWMO_WAITALL : 0)|MWMO_INPUTAVAILABLE);
		if (dwRet != WAIT_OBJECT_0 + nCount)
			return dwRet;

		DWORD dwWait = GetTickCount() - dwStart;
		if( dwMilliseconds != INFINITE && dwWait >= dwMilliseconds )
			bWait = false;
		else if( dwMilliseconds != INFINITE )
			dwMilliseconds -= dwMilliseconds;

		MSG msg = {};
		while(PeekMessage(&msg,0,0,0,PM_NOREMOVE))
		{
			// check for unicode window so we call the appropriate functions
			BOOL bRet = ::GetMessage(&msg, NULL, 0, 0);
			if (bRet > 0)
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}

			DWORD dwRes = ::WaitForMultipleObjects(nCount, lpHandles, bWaitAll, 0);
			if( dwRes != WAIT_TIMEOUT )
				return dwRes;
		}
	}

	return WAIT_TIMEOUT;
}


inline DWORD WaitForMultipleObjects_WithCancel( int _nWaitAllCount, HANDLE* _phWaitAllEvents, int _nWaitCancelCount, HANDLE* _phWaitCancelEvents, DWORD _dwMilliseconds )
{
	ATLASSERT( _nWaitAllCount );
	if( _nWaitAllCount <= 0 )
		return (DWORD)-1;

	ATLASSERT(_nWaitCancelCount > 0 ? _phWaitCancelEvents != NULL : _nWaitCancelCount == 0 );

	if( _nWaitCancelCount <= 0 || !_phWaitCancelEvents )
		return ::WaitForMultipleObjects( _nWaitAllCount, _phWaitAllEvents, TRUE, _dwMilliseconds );

	DWORD dwStart = ::GetTickCount();
	
	CSimpleArray2<HANDLE> arrWait;
	for( int i = 0; i < _nWaitCancelCount ; i++ )
		arrWait.Add( _phWaitCancelEvents[i] );

	for( int i = 0; i < _nWaitAllCount; i++ )
		arrWait.Add( _phWaitAllEvents[i] );
	
	DWORD dwRes = WAIT_OBJECT_0;
	while( arrWait.GetSize() > _nWaitCancelCount )
	{
		dwRes = ::WaitForMultipleObjects( arrWait.GetSize(), arrWait.m_aT, FALSE, _dwMilliseconds );
		if( dwRes == WAIT_TIMEOUT || 
			(int)(dwRes - WAIT_OBJECT_0) < _nWaitCancelCount ||
			arrWait.GetSize() <= _nWaitCancelCount + 1 )
		{
			break;
		}

		arrWait.RemoveAt( dwRes - WAIT_OBJECT_0 );

		DWORD dwElapsed = GetTickCount() - dwStart;
		if( _dwMilliseconds != INFINITE )
		{
			if( dwElapsed >= _dwMilliseconds )
				return WAIT_TIMEOUT;

			_dwMilliseconds -= dwElapsed;
		}
	}
	
	return dwRes;
}

inline LONGLONG GetFileModificationTime( LPCWSTR _szFileName )
{
	WIN32_FIND_DATAW findData={0};
	HANDLE hFind = FindFirstFileW( _szFileName, &findData );
	if( hFind != INVALID_HANDLE_VALUE )
	{
		::FindClose( hFind );
		return *(LONGLONG*)&findData.ftLastWriteTime;
	}

	return (LONGLONG)-1;
}

inline LONGLONG GetFileSize( LPCWSTR _szFileName )
{
	WIN32_FIND_DATAW findData={0};
	HANDLE hFind = FindFirstFileW( _szFileName, &findData );
	if( hFind != INVALID_HANDLE_VALUE )
	{
		::FindClose( hFind );
		LONGLONG llSize = (((ULONGLONG)findData.nFileSizeHigh) << 32) + (ULONGLONG)findData.nFileSizeLow;
		return llSize;
	}

	return (LONGLONG)-1;
}


inline int __stdcall EnumFilesWildChar( LPCWSTR _szFileName, CSimpleArray2<CComBSTR>& _rArrFiles, int _nFilesMax = 0 )
{
	WCHAR szPath[MAX_PATH*2] = {0};
	lstrcpynW( szPath, _szFileName, MAX_PATH );
	DWORD dwRes = GetFileAttributesW( szPath );
	if( dwRes == INVALID_FILE_ATTRIBUTES )
	{
		// Cut-off last parameters
		LPWSTR pszEnd = szPath + lstrlenW( szPath );
		while( pszEnd > szPath )
		{
			if( pszEnd[0] == L'/' || pszEnd[0] == L'\\' )
			{
				pszEnd[1] = 0;
				break;
			}
			pszEnd--;
		}

		dwRes = GetFileAttributesW( szPath );
	}

	if( dwRes != INVALID_FILE_ATTRIBUTES && (dwRes & FILE_ATTRIBUTE_DIRECTORY) )
	{
		WCHAR szFileName[MAX_PATH*2] = {0};
		lstrcpynW( szFileName, _szFileName, MAX_PATH );
		WIN32_FIND_DATAW fd = {0};
		HANDLE hFindFile = ::FindFirstFileW(_szFileName, &fd);
		if( hFindFile == INVALID_HANDLE_VALUE || (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
		{
			// Seems to be the path
			LPWSTR pszEnd = szFileName + lstrlenW( szFileName ) - 1;
			if( pszEnd[0] == L'/' || pszEnd[0] == L'\\' )
			{
				lstrcatW( szFileName, L"*.*" );
			}
			else if( pszEnd[0] != L'*' )
			{
				lstrcatW( szFileName, L"\\*.*" );
			}

			::FindClose(hFindFile);
			hFindFile = ::FindFirstFileW(szFileName, &fd);
		}
		if(hFindFile != INVALID_HANDLE_VALUE)
		{
			// Make canonic path
			LPWSTR pszEnd = szPath + lstrlenW( szPath ) - 1;
			if( pszEnd[0] != L'/' && pszEnd[0] != L'\\' ) lstrcatW(szPath, L"\\" );

			do
			{
				if( !(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
				{
					lstrcpyW( szFileName, szPath );
					lstrcatW( szFileName, fd.cFileName );
					_rArrFiles.Add( szFileName );
				}
			}
			while(::FindNextFileW(hFindFile, &fd) == TRUE && (_nFilesMax <= 0 || _rArrFiles.GetSize() < _nFilesMax) );

			::FindClose(hFindFile);
		}
	}

	return _rArrFiles.GetSize();
}

inline int __stdcall FindFileWildChar( LPCWSTR _szFileName, BSTR* _pbsFileName )
{
	CSimpleArray2<CComBSTR> arrFiles;
	int nFound = EnumFilesWildChar(_szFileName, arrFiles, 1 );
	if( nFound <= 0 )
		return nFound;

	if( _pbsFileName )
		*_pbsFileName = arrFiles[0].Detach();

	return 1;
}


//////////////////////////////////////////////////////////////////////////
// Files helpers

static bool IsHTTP( LPCWSTR _pszText )
{
	CComBSTR cbsFilePath = _pszText;
	CharLowerW(cbsFilePath.m_str);
	if( cbsFilePath.Length() > 7 && ( memcmp( cbsFilePath.m_str, L"http://", 7 * 2 ) == 0 || memcmp( cbsFilePath.m_str, L"https://", 8 * 2 ) == 0) )
	{
		return true;
	}

	return false;
}

inline bool IsFullPathW( LPCWSTR _szPath )
{
	if( IsProtocolW(_szPath, NULL) ) 
		return true;

	if( !_szPath || lstrlenW( _szPath ) < 4 ) return false;

	// Check for C:\path
	if( (_szPath[0] < L'A' || _szPath[0] > L'Z') && (_szPath[0] < L'a' || _szPath[0] > L'z') ||
		_szPath[1] != L':' ||
		(_szPath[2] != L'/' && _szPath[2] != L'\\') )
	{
		// Check for \\server\path
		if(  _szPath[0] != L'\\' || _szPath[1] != L'\\' || 
			( (_szPath[2] < L'A' || _szPath[2] > L'Z') && (_szPath[2] < L'a' || _szPath[2] > L'z') && (_szPath[2] < L'0' || _szPath[2] > L'9') && _szPath[2] != L'.' ) )
		{
			return false;
		}
	}

	return  true;
}


inline LPCWSTR ConstructPathW(LPCWSTR pszDefPath, LPWSTR pszFile, int _nMaxLen = MAX_PATH)
{
	//#pragma vcout("TODO. ConstructPath(): bad code, should be corrected")

	ATLASSERT(pszDefPath != NULL);
	if(pszDefPath == NULL) return NULL;
	ATLASSERT(pszFile != NULL);
	if(pszFile == NULL) return NULL;

	// If path to file is absolute: ignore def path and return original file name
	if( IsFullPathW(pszFile) ) return pszFile;

	// If def path not set: return original file name
	if(*pszDefPath == (WCHAR)0)
		return pszFile;

	// If device not specified in def path: return false
	if( !IsFullPathW(pszDefPath) ) return NULL;

	LPWSTR psz = pszFile;
	while(*psz == (WCHAR)'/' || *psz == (WCHAR)'\\')
		++psz;

	if(*psz == (WCHAR)0)
		return NULL;

	//---------------------------------
	// Calculate number of "..\" in file name
	DWORD nUpDir = 0;
	while(lstrlenW(psz) >= 3)		// "..\", "../"
	{
		if( psz[0] == (WCHAR)'.' && psz[1] == (WCHAR)'.' &&
			(psz[2] == (WCHAR)'/' || psz[2] == (WCHAR)'\\'))
		{
			psz += 3;
			++nUpDir;
		}
		else
			break;
	}

	int nSym = lstrlenW(psz);
	if(nSym == 0 || nSym >= _nMaxLen - 3)
		return NULL;

	//---------------------------------
	// Analyze path, skip directories on tail
	LPCWSTR pszStart = pszDefPath + 2;
	while( *pszStart != L'/' && *pszStart != L'\\' && *pszStart != 0 )
	{
		pszStart++;
	}
	if( *pszStart != 0 ) pszStart++;

	WCHAR* szName = (WCHAR*)alloca( _nMaxLen * sizeof(WCHAR) + 1 );
	lstrcpynW( szName, pszDefPath, (int)(pszStart - pszDefPath + 1) );

	if(*pszStart == (WCHAR)0)
	{
		if(nUpDir != 0)
			return NULL;

		lstrcatW(szName + 3, psz);
	}
	else
	{
		LPCWSTR pszEnd = pszStart + lstrlenW(pszStart) - 1;
		if(*pszEnd != (WCHAR)'/' && *pszEnd != (WCHAR)'\\')
			++pszEnd;

		// *pszEnd == 0 or '/' or '\' 
		while(nUpDir > 0 && pszEnd > pszStart)
		{
			while(pszEnd > pszStart)
			{
				--pszEnd;
				if(*pszEnd == (WCHAR)'/' || *pszEnd == (WCHAR)'\\')
					break;
			}

			--nUpDir;
		}

		if(nUpDir != 0)
			return NULL;

		if(pszEnd == pszStart)
		{
			lstrcatW(szName, psz);
		}
		else
		{
			int nOffset = lstrlenW(szName);

			int nPath = (int)(pszEnd - pszStart);
			if(nOffset + nPath + 1 + nSym >= _nMaxLen)
				return NULL;

			memcpy(szName + nOffset, pszStart, nPath * sizeof(WCHAR));
			szName[nPath + nOffset] = (WCHAR)'\\';
			lstrcpyW(szName + nOffset + nPath + 1, psz);
		}
	}

	lstrcpyW(pszFile, szName);
	return pszFile;
}

inline LPCWSTR ConstructPathW(LPCWSTR pszDefPath, LPCWSTR pszFile, CComBSTR& cbsFullPath )
{
	//#pragma vcout("TODO. ConstructPath(): bad code, should be corrected")

	ATLASSERT(pszDefPath != NULL);
	if(pszDefPath == NULL) return NULL;
	ATLASSERT(pszFile != NULL);
	if(pszFile == NULL) return NULL;

	// If device not specified in def path: return false
	// ATLASSERT( IsFullPathW(pszDefPath) );
	if( !IsFullPathW(pszDefPath) ) return NULL;

	// If path to file is absolute: ignore def path and return original file name
	if( IsFullPathW(pszFile) || *pszDefPath == (WCHAR)0 )
	{
		cbsFullPath = pszFile;
		return cbsFullPath.m_str;
	}

	LPCWSTR psz = pszFile;
	while(*psz == (WCHAR)'/' || *psz == (WCHAR)'\\')
		++psz;

	if(*psz == (WCHAR)0)
		return NULL;

	//---------------------------------
	// Calculate number of "..\" in file name
	DWORD nUpDir = 0;
	while(lstrlenW(psz) >= 3)		// "..\", "../"
	{
		if( psz[0] == (WCHAR)'.' && psz[1] == (WCHAR)'.' &&
			(psz[2] == (WCHAR)'/' || psz[2] == (WCHAR)'\\'))
		{
			psz += 3;
			++nUpDir;
		}
		else
			break;
	}

	int nSym = lstrlenW(psz);
	if(nSym == 0) return NULL;

	//---------------------------------
	// Analyze path, skip directories on tail
	LPCWSTR pszEnd = pszDefPath + lstrlenW(pszDefPath);
	while( pszEnd > pszDefPath )
	{
		if( *pszEnd == L'\\' || *pszEnd == L'/' )
		{
			if( nUpDir == 0 ) break;
			nUpDir--;
		}

		pszEnd--;
	}

	if( nUpDir > 0 )
	{
		// Wrong paths
		cbsFullPath = pszFile;
		return cbsFullPath.m_str;
	}

	cbsFullPath.Attach( ::SysAllocStringLen( pszDefPath, (int)(pszEnd - pszDefPath + 1) ) );
	cbsFullPath.Append( psz );

	return cbsFullPath.m_str;
}

// e.g.
// F:\DS_Test\DV\Root\root.exe
// F:\DS_Test\DV\Files\MyFile\file.bin
// ->
// ..\Files\MyFile\file.bin
//
// F:\DS_Test\DV\Root\root.exe
// F:\DS_Test\DV\Root\MyFile\file.bin

inline LPCWSTR GetRelativePathW(LPCWSTR pszDefPath, LPCWSTR pszFile, LPWSTR pszRelPath )
{
	//#pragma vcout("TODO. ConstructPath(): bad code, should be corrected")

	ATLASSERT(pszDefPath != NULL);
	if(pszDefPath == NULL) return false;
	ATLASSERT(pszFile != NULL);
	if(pszFile == NULL) return false;

	// If path to file is absolute: ignore def path and return original file name
	int nFirstDiff = 0;
	while( pszFile[nFirstDiff] && pszDefPath[nFirstDiff] == pszFile[nFirstDiff] ) nFirstDiff++;

	nFirstDiff--;

	while( nFirstDiff > 0 && pszDefPath[nFirstDiff] != L'/' && pszDefPath[nFirstDiff] != L'\\' )
		nFirstDiff--;

	// Seems different drives
	if( nFirstDiff <= 0 )
	{
		lstrcpyW( pszRelPath, pszFile );
		return pszRelPath;
	}

	// For up dirs
	pszRelPath[0] = 0;
	LPCWSTR pszTemp = pszDefPath + lstrlenW(pszDefPath);
	while( pszTemp > pszDefPath + nFirstDiff )
	{
		if( *pszTemp == L'/' || *pszTemp == L'\\' ) 
		{
			lstrcatW( pszRelPath, L"..\\" );
		}

		pszTemp--;
	}

	lstrcatW( pszRelPath, pszFile + nFirstDiff + 1);

	return pszRelPath;
}

inline bool MakeRelPath( CComBSTR& cbsPath, LPCWSTR _pszDefPath )
{
	if( _pszDefPath && IsFullPathW( _pszDefPath ) )
	{
		int nMaxLen = (lstrlenW(_pszDefPath) + cbsPath.Length() ) * 2 + 1;
		nMaxLen = MAX( nMaxLen, MAX_PATH );
		WCHAR* pszPath = (WCHAR*)alloca( sizeof(WCHAR) * nMaxLen );
		GetRelativePathW( _pszDefPath, cbsPath, pszPath );

		cbsPath = pszPath;
		return true;
	}

	return false;
}

inline bool MakeFullPath( CComBSTR& cbsPath, LPCWSTR _pszDefPath )
{
	if( !IsFullPathW(cbsPath) && _pszDefPath )
	{
		CComBSTR cbsFullPath = cbsPath;
		ConstructPathW( _pszDefPath, cbsPath, cbsFullPath );
		cbsPath = cbsFullPath;
		return true;
	}

	return false;
}

inline LPCWSTR GetFullPath_Module( CComBSTR& cbsPath, BOOL _bEXEPath = false )
{
	if( !IsFullPathW(cbsPath) )
	{
		WCHAR szMaxPath[1024]={0};

		__if_exists(_Module)
		{
			::GetModuleFileNameW( _bEXEPath ? NULL : _Module.GetModuleInstance(), szMaxPath, SIZEOF_ARRAY(szMaxPath) );
		}
		__if_not_exists(_Module)
		{
			::GetModuleFileNameW( _bEXEPath ? NULL : _AtlBaseModule.GetModuleInstance(), szMaxPath, SIZEOF_ARRAY(szMaxPath) );
		}


		CComBSTR cbsFullPath = cbsPath;
		if( ConstructPathW( szMaxPath, cbsPath, cbsFullPath ) )
		{
			cbsPath = cbsFullPath;
			return cbsPath;
		}
		else
		{
			return NULL;
		}
	}

	return cbsPath;
}


typedef HRESULT (STDAPICALLTYPE *PFGetClassObject)(REFCLSID rclsid, REFIID riid, LPVOID* ppv);

inline bool IsMServerProcess(bool _bCheckID = false)
{
	static int nIsMServer = -1;
	if (nIsMServer < 0)
	{
		WCHAR szServer[4096] = {};
		::GetModuleFileNameW(NULL, szServer, SIZEOF_ARRAY(szServer));
		LPCWSTR pszFileName = ::FindFileNameW(szServer);
		nIsMServer = STR_IS_SAME_I(pszFileName, L"MServer.exe");
	}
	if( nIsMServer == 0 )
		return false;

	if (_bCheckID)
	{
		LONGLONG llCurrentPID = GetCurrentProcessId();
		llCurrentPID = (llCurrentPID * 7236718232489) % 3112648232429;
		WCHAR szEnvValue[512] = {};
		::GetEnvironmentVariableW(L"_mserver_id_", szEnvValue, SIZEOF_ARRAY(szEnvValue));
		if (_wcstoi64(szEnvValue, NULL, 16) != llCurrentPID)
			return false;
	}
	
	return true;
}

inline HINSTANCE ATLModule_Instance()
{
	__if_exists(_Module)
	{
		return _Module.GetModuleInstance();
	}
	__if_not_exists(_Module)
	{
		return _AtlBaseModule.GetModuleInstance();
	}

	return NULL;
}

inline LPCWSTR DLLModuleName( LPWSTR _pszModuleName, int _nLen, HMODULE* _phModule = NULL )
{
	__if_exists(_Module)
	{
		if( _pszModuleName && _nLen > 0 )
			::GetModuleFileNameW( _Module.GetModuleInstance(), _pszModuleName, _nLen );
		if( _phModule )
			*_phModule = _Module.GetModuleInstance();
	}
	__if_not_exists(_Module)
	{
		if( _pszModuleName && _nLen > 0 )
			::GetModuleFileName( _AtlBaseModule.GetModuleInstance(), _pszModuleName, _nLen );

		if( _phModule )
			*_phModule = _AtlBaseModule.GetModuleInstance();
	}

	return _pszModuleName;
}

#define DLLMODULENAME()  (DLLModuleName( (LPWSTR)alloca(MAX_PATH*8), MAX_PATH * 4 ))


inline CComBSTR LoadTextFromResource( UINT resID, LPCWSTR _pszType = L"TEXT", int _nUnicode = -1 )
{
#if (_ATL_VER >= 0x0700)
	HINSTANCE hInstance = ATL::_AtlBaseModule.GetResourceInstance();
#else	// !(_ATL_VER >= 0x0700)
	HINSTANCE hInstance = _Module.GetResourceInstance();
#endif	// !(_ATL_VER >= 0x0700)

	HRSRC hResource = ::FindResourceW(hInstance, MAKEINTRESOURCEW(resID), _pszType);
	if (!hResource)
		return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);

	HGLOBAL hLoadedResource = LoadResource(hInstance, hResource);
	if (!hLoadedResource)
		return E_FAIL;

	LPBYTE pbLockedResource = (LPBYTE)LockResource(hLoadedResource);
	if (!pbLockedResource)
		return HRESULT_FROM_WIN32( ERROR_FILE_INVALID );

	DWORD dwResourceSize = SizeofResource(hInstance, hResource);
	if (!dwResourceSize)
		return HRESULT_FROM_WIN32( ERROR_FILE_INVALID );

	LPBYTE pbConvert = NULL;
	LPWSTR pszContent = NULL;
	LPCWSTR pszContentEnd = NULL;

	if( _nUnicode > 0 )
	{
		int nPos = 0;
		if( (pbLockedResource[0] & 0xFE) == 0xFE && (pbLockedResource[1] & 0xFE) == 0xFE)
			nPos = 2;

		pszContent = (LPWSTR)pbLockedResource;
		pszContentEnd = (LPWSTR)(pbLockedResource + dwResourceSize);

		// TODO: XMLUtils::PrepareForParse(pszContent);
	}
	else
	{
		// TODO: correct correction 
		pbConvert = new BYTE[dwResourceSize * 4 + 2];
		pszContent = (LPWSTR)pbConvert;

		if( Text2Wide( pbLockedResource, dwResourceSize, pszContent ) <= 0 )
			return E_INVALIDARG;
	}

	CComBSTR cbsRes = pszContent;

	if( pbConvert )
	{
		delete [] pbConvert;
		pbConvert = NULL;
	}

	return cbsRes;
}

template< class TInterface> 
static HRESULT CreateObjectDirect( LPCWSTR _pszFileName, CLSID _gClassID, TInterface** _ppObject, LPCWSTR _pszExtraPath = NULL, BSTR* _pbsFullPath = NULL )
{
	static CComAutoCriticalSection csMap;
	static std::map<CComBSTR,std::pair<HMODULE,CComBSTR> > mapLibs;
	
	CComBSTR cbsFullPath;
	CComBSTR cbsFileName = _pszFileName;
	::CharLowerW( cbsFileName.m_str );

	// Load lib
	HMODULE hLib = NULL;
	{
		CAutoCS crs(csMap);

		std::map<CComBSTR,std::pair<HMODULE,CComBSTR> >::iterator Iter = mapLibs.find( cbsFileName );
		if( Iter != mapLibs.end() )
		{
			hLib = Iter->second.first;

			if( _pbsFullPath )
				Iter->second.second.CopyTo( _pbsFullPath );
		}
		else
		{
			HMODULE hCurrentLib = NULL;
			WCHAR szCurrentPath[MAX_PATH*4] = {};
			DLLModuleName( szCurrentPath, SIZEOF_ARRAY(szCurrentPath), &hCurrentLib );

			MakeFullPath( cbsFileName, szCurrentPath );

			UINT ulErrorMode = ::GetErrorMode();
			::SetErrorMode( SEM_NOOPENFILEERRORBOX | SEM_NOGPFAULTERRORBOX | SEM_FAILCRITICALERRORS);

			if( STR_IS_SAME_I( szCurrentPath, cbsFileName ) )
			{
				ATLTRACE2( atlTraceGeneral, 2, _T("CreateObjectDirect(%ws,%ws) - SAME DLL:%ws\n"),
					GUID2STR( _gClassID ), _pszFileName, cbsFileName.m_str  );

				cbsFullPath = cbsFileName;
				hLib = hCurrentLib;
			}

			while( !hLib )
			{
				hLib = ::LoadLibraryExW( cbsFileName, NULL, 0x00001100); // LOAD_LIBRARY_SEARCH_DEFAULT_DIRS|LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR
				if( !hLib && ::GetLastError() == ERROR_INVALID_PARAMETER )
					hLib = ::LoadLibraryW( cbsFileName ); // Not all OS ver supports flags: https://msdn.microsoft.com/en-us/library/windows/desktop/ms684179(v=vs.85).aspx
				if( !hLib )
				{
					DWORD dwErr = ::GetLastError();

					ATLTRACE2( atlTraceGeneral, 2, _T("###Err### CreateObjectDirect(%ws,%ws) - LoadLibraryExW(%ws) FAILED dwRes=%d\n"),
						GUID2STR( _gClassID ), _pszFileName, cbsFileName.m_str, dwErr );
					// Try wild chars
					InsertBeforeExtension( cbsFileName, L"*" );

					CSimpleArray2<CComBSTR> arrNames;
					EnumFilesWildChar( cbsFileName, arrNames );
					if (!arrNames.GetSize())
					{
						// Try with "Medialooks." prefix;
						cbsFileName = STR_APPEND( L"Medialooks.", cbsFileName);
						EnumFilesWildChar(cbsFileName, arrNames);
					}
					for( int i = 0; i < arrNames.GetSize(); i++ )
					{
						hLib = ::LoadLibraryExW(arrNames[i], NULL, 0x00001100); // LOAD_LIBRARY_SEARCH_DEFAULT_DIRS|LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR
						if( !hLib && ::GetLastError() == ERROR_INVALID_PARAMETER )
							hLib = ::LoadLibraryW( arrNames[i] ); // Not all OS ver supports flags: https://msdn.microsoft.com/en-us/library/windows/desktop/ms684179(v=vs.85).aspx
						if( hLib )
						{
							cbsFullPath = arrNames[i];

							ATLTRACE2( atlTraceGeneral, 8, _T("CreateObjectDirect(%ws,%ws) - LoadLibraryExW(%ws) OK dwRes=%d\n"),
								GUID2STR( _gClassID ), _pszFileName, arrNames[i].m_str, dwErr );

							break;
						}

						dwErr = ::GetLastError();

						ATLTRACE2( atlTraceGeneral, 2, _T("###Err### CreateObjectDirect(%ws,%ws) - LoadLibraryExW(%ws) FAILED dwRes=%d\n"),
							GUID2STR( _gClassID ), _pszFileName, arrNames[i].m_str, dwErr );
					}
				}
				else
				{
					cbsFullPath = cbsFileName;
					break;
				}

				if( !_pszExtraPath )
					break; // Not found

				cbsFileName = _pszExtraPath;
				cbsFileName.Append( L"\\");
				cbsFileName.Append( _pszFileName );

				MakeFullPath( cbsFileName, szCurrentPath );
				_pszExtraPath = NULL; // For one cicle
			}

			::SetErrorMode( ulErrorMode );

			if( !hLib )
				return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );

			cbsFileName = _pszFileName;
			::CharLowerW( cbsFileName.m_str );
			mapLibs.insert( std::make_pair(cbsFileName, std::make_pair(hLib, cbsFullPath) ) );

			if( _pbsFullPath )
				cbsFullPath.CopyTo( _pbsFullPath );
		}
	}

	PFGetClassObject pfGetClassObject = (PFGetClassObject)::GetProcAddress( hLib, "DllGetClassObject" );
	ATLASSERT( pfGetClassObject );
	if( !pfGetClassObject )
		return HRESULT_FROM_WIN32( ERROR_PROC_NOT_FOUND );

	CComPtr<IClassFactory> cpClassFactory;
	HRESULT hr = pfGetClassObject( _gClassID, IID_IClassFactory, (void**)&cpClassFactory );
	if( FAILED( hr ) )
	{
		ATLTRACE2( atlTraceGeneral, 2, _T("###Err### CreateObjectDirect(%ws,%ws) - pfGetClassObject() FAILED hr=%08X\n"),
			GUID2STR( _gClassID ), _pszFileName, hr );

		return hr;
	}

	hr = cpClassFactory->CreateInstance( NULL, __uuidof(TInterface), (void**)_ppObject );
	if( FAILED( hr ) )
	{
		ATLTRACE2( atlTraceGeneral, 2, _T("###Err### CreateObjectDirect(%ws,%ws) - CreateInstance() FAILED hr=%08X\n"),
			GUID2STR( _gClassID ), _pszFileName, hr );

		return hr;
	}

	return hr;
}


// Register proxy/stub from specified DLL name (also search used postfixes - e.g. MPlatfromD.dll, MPlatfromPX.dll et)
// The FIRST CLSID is NULL terminated CLSID list should be proxy/stub CLSID 
static HRESULT RegisterProxyDirect( LPCWSTR _pszFileName, CLSID* _pInterfacesClsids, LPCWSTR _pszExtraPath = NULL )
{
	static CComAutoCriticalSection csMap;
	static std::map<CComBSTR,HMODULE> mapLibs;

	ATLASSERT( _pInterfacesClsids );

	CComBSTR cbsFileName = _pszFileName;
	::CharLowerW( cbsFileName.m_str );

	// Load lib
	HMODULE hLib = NULL;
	{
		CAutoCS crs(csMap);

		std::map<CComBSTR,HMODULE>::iterator Iter = mapLibs.find( cbsFileName );
		if( Iter != mapLibs.end() )
		{
			hLib = Iter->second;
		}
		else
		{
			WCHAR szCurrentPath[MAX_PATH*4] = {};
			DLLModuleName( szCurrentPath, SIZEOF_ARRAY(szCurrentPath) );
			MakeFullPath( cbsFileName, szCurrentPath );

			while( !hLib )
			{
				hLib = ::LoadLibraryW( cbsFileName );
				if( !hLib )
				{
					// Try wild chars
					InsertBeforeExtension( cbsFileName, L"*" );

					CSimpleArray2<CComBSTR> arrNames;
					EnumFilesWildChar( cbsFileName, arrNames );
					for( int i = 0; i < arrNames.GetSize(); i++ )
					{
						hLib = ::LoadLibraryW(arrNames[i]);
						if( hLib )
							break;
					}
				}

				if( !_pszExtraPath || hLib )
					break;

				cbsFileName = _pszExtraPath;
				cbsFileName.Append( L"\\");
				cbsFileName.Append( _pszFileName );

				MakeFullPath( cbsFileName, szCurrentPath );
				_pszExtraPath = NULL; // For one cicle
			}

			ATLASSERT( hLib && "RegisterProxyDirect(LoadLibraryW)" );
			if( !hLib )
				return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );

			cbsFileName = _pszFileName;
			::CharLowerW( cbsFileName.m_str );
			mapLibs.insert( std::make_pair(cbsFileName, hLib) );
		}
	}

	PFGetClassObject pfGetClassObject = (PFGetClassObject)::GetProcAddress( hLib, "DllGetClassObject" );
	ATLASSERT( pfGetClassObject && "RegisterProxyDirect(GetProcAddress)" );
	if( !pfGetClassObject )
		return HRESULT_FROM_WIN32( ERROR_PROC_NOT_FOUND );

	CComPtr<IPSFactoryBuffer> cpPSFactoryBuffer;
	HRESULT hr = pfGetClassObject( _pInterfacesClsids[0], IID_IPSFactoryBuffer, (void**)&cpPSFactoryBuffer );
	ATLASSERT( SUCCEEDED( hr ) && "RegisterProxyDirect(GetClassObject)" );
	if( FAILED( hr ) )
		return hr;

	DWORD dwUnused = 0;
	hr = ::CoRegisterClassObject( _pInterfacesClsids[0], cpPSFactoryBuffer, CLSCTX_INPROC_SERVER, REGCLS_MULTIPLEUSE, &dwUnused );
	ATLASSERT( SUCCEEDED( hr ) && "RegisterProxyDirect(CoRegisterClassObject)" );
	if( FAILED( hr ) )
		return hr;
// 
// 	dwUnused = 0;
// 	hr = ::CoRegisterClassObject( _pInterfacesClsids[0], cpProxyStub, CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE, &dwUnused );
// 	ATLASSERT( SUCCEEDED( hr ) && "RegisterProxyDirect(CoRegisterClassObject)" );
// 	if( FAILED( hr ) )
// 		return hr;

	for( int i = 0; _pInterfacesClsids[i] != GUID_NULL; i++ )
	{
		hr = ::CoRegisterPSClsid(_pInterfacesClsids[i], _pInterfacesClsids[0]);
		ATLASSERT( SUCCEEDED( hr ) && "RegisterProxyDirect(CoRegisterPSClsid)" );
	}

	return S_OK;
}


class MStringList
	: public CSimpleArray2<LPCWSTR>
{
private:
	BOOL Add(const LPCWSTR& t); 
	BOOL InsertAt(int nIndex, const LPCWSTR& t);
	BOOL SetAt(int nIndex, const LPCWSTR& t);
public:

	MStringList()
	{
		CSimpleArray2<LPCWSTR>::Add( NULL );
	}

	~MStringList()
	{
		RemoveAll();
	}

	int GetSize() const
	{
		ATLASSERT( m_nSize > 0 );
		return m_nSize - 1;
	}

	int AddList( const LPCWSTR* _ppszStrings )
	{
		int nAdded = 0;
		while( *_ppszStrings )
		{
			AddString( *_ppszStrings );
			_ppszStrings++;
			nAdded++;
		}

		return nAdded;
	}

	// e.g. "value\0value2\0value3\0\0"
	int AddZeroSplittedList( LPCWSTR _pszZeroSplittedList )
	{
		int nAdded = 0;
		LPCWSTR pszModule = _pszZeroSplittedList;
		while( pszModule[0] )
		{
			AddString( pszModule );
			pszModule += lstrlenW(pszModule) + 1;
			nAdded++;
		}

		return nAdded;
	}

	template <class TString>
	int AddArray( const CSimpleArray2<TString>& _rArrStrings )
	{
		int nAdded = 0;
		for( int i = 0; i < _rArrStrings.GetSize(); i++ )
		{
			if( !STR_IS_EMPTY( _rArrStrings[i] ) )
			{
				AddString( _rArrStrings[i] );
			
				nAdded++;
			}
		}

		return nAdded;
	}

	bool Intersection(const LPCWSTR* _ppszStrings, bool _bCaseSens, MStringList* pIntersectionList = NULL )
	{
		int nAdded = 0;
		while( *_ppszStrings )
		{
			if( Find( *_ppszStrings, _bCaseSens ) >= 0 )
			{
				if( pIntersectionList )
					pIntersectionList->AddString( *_ppszStrings );
				else
					return true;

				nAdded++;
			}

			_ppszStrings++;
		}

		return nAdded > 0;
	}

	int AddString( LPCWSTR _pszString, int _nLen = -1 )
	{
		if( !_pszString )
			return -1;

		if( _nLen < 0 )
			_nLen = lstrlenW( _pszString );
		ATLASSERT( m_nSize > 0 );
	
		LPWSTR pszStr = new WCHAR[ _nLen + 1 ];
		memcpy( pszStr, _pszString, _nLen * sizeof(WCHAR) );
		pszStr[ _nLen ] = 0;

		CSimpleArray2<LPCWSTR>::InsertAt( m_nSize - 1, pszStr );
		
		return m_nSize - 2;
	}

	int Find(const LPCWSTR& _pszRemove, bool _bCaseSens) const
	{
		ATLASSERT( _pszRemove );
		ATLASSERT( m_nSize > 0 );
		for( int i = 0; i < m_nSize - 1; i++ )
		{
			if( (_bCaseSens ? lstrcmpW( m_aT[i], _pszRemove ) : lstrcmpiW( m_aT[i], _pszRemove ) ) == 0 )
			{
				return i;
			}
		}

		return -1;
	}

	BOOL Remove( LPCWSTR _pszRemove, bool _bCaseSens = true )
	{
		int nIdx = Find( _pszRemove, _bCaseSens );
		if( nIdx >= 0 )
			return RemoveAt(nIdx);
			
		return FALSE;
	}

	BOOL RemoveAt(int nIndex)
	{
		ATLASSERT( nIndex >= 0 && nIndex < m_nSize - 1 );
		if(nIndex < 0 || nIndex >= m_nSize - 1 )
			return FALSE;

		delete [] m_aT[nIndex];
		m_aT[nIndex] = NULL;
		return RemoveAt( nIndex );
	}

	operator LPCWSTR* () { return GetData(); }

	void RemoveAll()
	{
		for( int i = 0; i < m_nSize - 1; i++ )
		{
			delete [] m_aT[i];
			m_aT[i] = NULL;
		}

		CSimpleArray2<LPCWSTR>::RemoveAll();
		CSimpleArray2<LPCWSTR>::Add( NULL );
	}

	LPCWSTR PrintList( CComBSTR& cbsList, LPCWSTR pszDelimer = L", " )
	{
		for( int i = 0; i < m_nSize - 1; i++ )
		{
			cbsList.Append( m_aT[i] );
			if( i < m_nSize - 2 )
				cbsList.Append( pszDelimer );
		}	

		return cbsList;
	}
};



inline bool InStringList( LPCWSTR _pszValue, const LPCWSTR* ppszTags, int* _pnIndex = NULL )
{
	if( ppszTags && _pszValue )
	{
		int nIndex = 0;
		while( *ppszTags )
		{
			if( lstrcmpW( _pszValue, *ppszTags ) == 0 )
			{
				if( _pnIndex )
					*_pnIndex = nIndex;

				return true;
			}

			ppszTags++;
			nIndex++;
		}
	}

	return false;
}

inline bool InStringListI( LPCWSTR _pszValue, const LPCWSTR* ppszTags, int* _pnIndex = NULL)
{
	if( ppszTags && _pszValue )
	{
		int nIndex = 0;
		while( *ppszTags )
		{
			if( lstrcmpiW( _pszValue, *ppszTags ) == 0 )
			{
				if( _pnIndex )
					*_pnIndex = nIndex;

				return true;
			}

			ppszTags++;
			nIndex++;
		}
	}

	return false;
}

inline bool InStringListA( LPCSTR _pszValue, const LPCSTR* ppszTags, int* _pnIndex = NULL )
{
	if( ppszTags && _pszValue )
	{
		int nIndex = 0;
		while( *ppszTags )
		{
			if( lstrcmpA( _pszValue, *ppszTags ) == 0 )
			{
				if( _pnIndex )
					*_pnIndex = nIndex;

				return true;
			}

			ppszTags++;
			nIndex++;
		}
	}

	return false;
}

inline bool InStringListIA( LPCSTR _pszValue, const LPCSTR* ppszTags, int* _pnIndex = NULL)
{
	if( ppszTags && _pszValue )
	{
		int nIndex = 0;
		while( *ppszTags )
		{
			if( lstrcmpiA( _pszValue, *ppszTags ) == 0 )
			{
				if( _pnIndex )
					*_pnIndex = nIndex;

				return true;
			}

			ppszTags++;
			nIndex++;
		}
	}

	return false;
}

static LPCWSTR GetPrefix( LPCWSTR _pszString, BSTR* _pbsPrefix )
{
	if( !STR_IS_EMPTY(_pszString) )
	{
		LPCWSTR pszRest = StrStrW( _pszString, L"::" );
		if( pszRest )
		{
			if( _pbsPrefix )
				*_pbsPrefix = ::SysAllocStringLen( _pszString, pszRest - _pszString );

			return pszRest + 2; // Skip '::'
		}
	}

	return NULL;
}

static bool IsPrefix( LPCWSTR _pszString, LPCWSTR _pszPrefix, LPCWSTR* _ppRestString = NULL )
{
	if( !_pszPrefix || !_pszString ) return false;

	int nLen = lstrlenW( _pszPrefix );
	if( StrCmpNIW( _pszString, _pszPrefix, nLen ) == 0 )
	{
		if( _ppRestString )
			*_ppRestString = _pszString + nLen;
		return true;
	}

	return false;
}

// WARNING !!! Return index + 1 - if not found return 0
static int IsPrefix( LPCWSTR _pszString, const LPCWSTR* _pszPrefixes, LPCWSTR* _ppRestString = NULL )
{
	if( !_pszString || !_pszPrefixes ) 
		return 0;

	for( int i = 0; _pszPrefixes[i] != NULL; i++ )
	{
		if( IsPrefix( _pszString, _pszPrefixes[i], _ppRestString ) )
			return i + 1;
	}

	return 0;
}

static bool IsPrefixA( LPCSTR _pszString, LPCSTR _pszPrefix, LPCSTR* _ppRestString = NULL)
{
	ATLASSERT( _pszString && _pszPrefix );
	if( !_pszPrefix || !_pszString ) return false;

	int nLen = lstrlenA( _pszPrefix );
	if( StrCmpNIA( _pszString, _pszPrefix, nLen ) == 0 )
	{
		if( _ppRestString )
			*_ppRestString = _pszString + nLen;
		return true;
	}

	return false;
}

static int IsPrefixA( LPCSTR _pszString, LPCSTR* _pszPrefixes, LPCSTR* _ppRestString = NULL )
{
	if( !_pszString || !_pszPrefixes ) 
		return 0;

	for( int i = 0; _pszPrefixes[i] != NULL; i++ )
	{
		if( IsPrefixA( _pszString, _pszPrefixes[i], _ppRestString ) )
			return i + 1;
	}

	return 0;
}

// Method for easy check redirection (e.g for preview redirection)
inline  bool CheckRedirector(CComBSTR& _rbsPropsName, LPCWSTR _pszPrefix, bool _bCutoffPrefix, int* _pnIndex = NULL)
{
	LPCWSTR pszName = NULL;
	if (!STR_IS_EMPTY(_pszPrefix) && (STR_IS_SAME_I(_rbsPropsName, _pszPrefix) || IsPrefix(_rbsPropsName, STR_APPEND(_pszPrefix, L"::"), &pszName)))
	{
		if (_bCutoffPrefix)
			_rbsPropsName.Attach(::SysAllocString(pszName));

		return true;
	}

	if (!STR_IS_EMPTY(_pszPrefix) && _pnIndex && IsPrefix(_rbsPropsName, STR_APPEND(_pszPrefix, L"["), &pszName))
	{
		if (pszName)
			*_pnIndex = _wtoi(pszName);

		pszName = StrStrW(pszName, L"::");
		if (pszName)
			pszName += 2;

		if (_bCutoffPrefix)
			_rbsPropsName.Attach(::SysAllocString(pszName));

		return true;
	}

	return false;
}

inline bool IsEqualInterfaces(IUnknown* _pFirst, IUnknown* _pSecond)
{
	CComPtr<IUnknown> cpFirst(_pFirst);
	return cpFirst.IsEqualObject(_pSecond);
}

inline LPCWSTR FindAttribute( LPCWSTR _pszString, LPCWSTR _pszAttrName, LPCWSTR* _ppszEnd )
{
	if( STR_IS_EMPTY(_pszString) || STR_IS_EMPTY( _pszAttrName ) )
		return NULL;

	LPCWSTR pszBuffer = StrStrIW( _pszString, _pszAttrName );
	if( !pszBuffer )
		return NULL;

	pszBuffer += lstrlenW(_pszAttrName);

	while( *pszBuffer == L' ' )
		pszBuffer++;

	if( *pszBuffer != L'=' )
		return NULL;

	pszBuffer++;

	while( *pszBuffer == L' ' )
		pszBuffer++;

	// Check quotes
	if( *pszBuffer == L'\'' || *pszBuffer == L'"' )
	{
		if( _ppszEnd )
			*_ppszEnd = StrChrW( pszBuffer + 1, *pszBuffer );

		pszBuffer++;
	}
	else
	{
		if( _ppszEnd )
			*_ppszEnd = StrChrW( pszBuffer, L' ' );
	}

	return pszBuffer;
}

inline LPSTR Data2Str( LPBYTE _pbData, DWORD _cbData, bool _bAddSpaces, void* _pBuffer )
{
	LPSTR pszData = (LPSTR)_pBuffer;
	for( int i = 0; i < (int)_cbData; i++ )
	{
		if( i > 0 && _bAddSpaces ) *pszData++ = ' ';

		*pszData++ = (_pbData[i] >> 4) + ((_pbData[i] >> 4) >= 10 ? ('A' - 10) : '0');
		*pszData++ = (_pbData[i] & 0xF) + ((_pbData[i] & 0xF) >= 10 ? ('A' - 10) : '0');
	}
	*pszData = 0;

	return (LPSTR)_pBuffer;
}

inline LPWSTR Data2StrW( LPBYTE _pbData, DWORD _cbData, bool _bAddSpaces, void* _pBuffer )
{
	LPWSTR pszData = (LPWSTR)_pBuffer;
	for( int i = 0; i < (int)_cbData; i++ )
	{
		if( i > 0 && _bAddSpaces ) *pszData++ = ' ';

		*pszData++ = (_pbData[i] >> 4) + ((_pbData[i] >> 4) >= 10 ? ('A' - 10) : '0');
		*pszData++ = (_pbData[i] & 0xF) + ((_pbData[i] & 0xF) >= 10 ? ('A' - 10) : '0');
	}
	*pszData = 0;

	return (LPWSTR)_pBuffer;
}

inline CComBSTR Data2Str( LPBYTE _pbData, DWORD _cbData, bool _bAddSpaces = false )
{
	void* pbBuffer = alloca( _cbData * 3 + 2);
	return Data2Str( _pbData, _cbData, _bAddSpaces, (LPSTR)pbBuffer );
}

inline int Str2Data( LPCWSTR _pszStr, LPBYTE _pbBuffer, int _cbBufferMax )
{
	int nDigitCount = 0;
	LPBYTE pbBuffer = _pbBuffer;
	::ZeroMemory( _pbBuffer, _cbBufferMax );
	LPCWSTR pszData = (LPCWSTR)_pszStr;
	while( *pszData && pbBuffer < _pbBuffer + _cbBufferMax)
	{
		if( *pszData >= L'0' && *pszData <= L'9' )
		{
			*pbBuffer += ((*pszData) - L'0');
		}
		else if( *pszData >= L'A' && *pszData <= L'F' )
		{
			*pbBuffer += ((*pszData) - L'A') + 10;
		}
		else if( *pszData >= L'a' && *pszData <= L'f' )
		{
			*pbBuffer += ((*pszData) - L'a') + 10;
		}
		else
		{
			pszData++;
			continue;
		}

		nDigitCount++;
		if( nDigitCount % 2 )
			*pbBuffer *= 16;
		else
			pbBuffer++;

		pszData++;
	}

	return pbBuffer - _pbBuffer;
}



class CActiveContext
{
	HANDLE		m_pActiveCtx;
	ULONG_PTR	m_lpCookie;

public:

	CActiveContext()
		: m_pActiveCtx( NULL ),
		  m_lpCookie( 0 )
	{
	}

	~CActiveContext()
	{
		Close();
	}

	HRESULT Init( LPCWSTR _pszManifest = NULL )
	{
		if( m_pActiveCtx )
		{
			ATLASSERT( m_lpCookie );
			return S_FALSE;
		}

		WCHAR szName[MAX_PATH * 8 + 16] = {};
		if( !_pszManifest )
		{
			DWORD nSize = SIZEOF_ARRAY(szName) - 16;
			::GetModuleFileNameW( NULL, szName, nSize );

			lstrcatW( szName, L".manifest" );
			_pszManifest = szName;
		}

		// Check file name
		if( ::GetFileAttributesW( _pszManifest) == INVALID_FILE_ATTRIBUTES )
		{
			return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );
		}

		ACTCTXW actctx = {};
		actctx.cbSize = sizeof(actctx); 
		actctx.lpSource = _pszManifest; // Path to the Side-by-Side Manifest File 

		m_pActiveCtx = CreateActCtxW(&actctx);
		ATLASSERT( m_pActiveCtx != INVALID_HANDLE_VALUE && "CreateActCtx" );
		if(m_pActiveCtx == INVALID_HANDLE_VALUE) return E_INVALIDARG;

		ATLASSERT( m_lpCookie == NULL );
		BOOL bOK = ActivateActCtx(m_pActiveCtx, &m_lpCookie);
		ATLASSERT( bOK && "ActivateActCtx" );
		if( !bOK )
		{
			ReleaseActCtx(m_pActiveCtx);
			m_pActiveCtx = NULL;
		}

		return S_OK;
	}

	void Close()
	{
		if( m_lpCookie )
		{
			DeactivateActCtx(0, m_lpCookie);
			m_lpCookie = NULL;
		}

		if( m_pActiveCtx )
		{
			ReleaseActCtx(m_pActiveCtx);
			m_pActiveCtx = NULL;
		}
	}
};

class MFileWriter
{
	CComAutoCriticalSection m_csFile;
	CStdHandle		m_shFile;
	CComBSTR			m_cbsFileName;
public:

	MFileWriter()
	{
	}

	~MFileWriter()
	{
		FileClose();
	}

	HRESULT FileOpen( LPCWSTR _pszFileName, bool _bAppend )
	{
		CAutoCS crs(m_csFile );

		FileClose();

		HANDLE hFile = ::CreateFileW( _pszFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, _bAppend ? OPEN_ALWAYS : CREATE_ALWAYS, 0, NULL );
		DWORD dwErr = GetLastError();
		if( !hFile || hFile == INVALID_HANDLE_VALUE )
		{
			ATLTRACE2( atlTraceGeneral, 2, _T("###Err### MFileWriter::FileOpen(%ws,%ws) - CreateFileW() FAILED dwRes = %d\n"),
				_pszFileName, _bAppend ? L"APPEND" : L"NEW", dwErr );

			return dwErr != 0 ? HRESULT_FROM_WIN32( dwErr ) : E_FAIL;
		}

		ATLTRACE2( atlTraceGeneral, 2, _T("MFileWriter::FileOpen(%ws,%ws) - CreateFileW() dwRes = %d\n"),
			_pszFileName, _bAppend ? L"APPEND" : L"NEW", dwErr );

		m_shFile.Attach( hFile );
		m_cbsFileName = _pszFileName;

		return S_OK;
	}

	void FileClose()
	{
		CAutoCS crs(m_csFile );

		m_shFile.Close();
		m_cbsFileName.Empty();
	}

	HRESULT FileWrite( LPCWSTR _pszFileName, LPBYTE _pbData, DWORD _cbData, bool _bAppendToNew = false )
	{
		if( !_pbData || !_cbData )
			return _cbData ? E_INVALIDARG : S_FALSE;

		CAutoCS crs(m_csFile );

		if( !_pszFileName && !m_shFile )
			return HRESULT_FROM_WIN32(ERROR_EMPTY);

		if( _pszFileName && (!m_cbsFileName || lstrcmpiW( m_cbsFileName, _pszFileName ) != 0 ) )
		{
			HRESULT hr = FileOpen( _pszFileName, _bAppendToNew );
			if( FAILED( hr ) )
				return hr;
		}

		DWORD cbWrite = 0;
		BOOL bOK = ::WriteFile( m_shFile, _pbData, _cbData, &cbWrite, NULL );
		DWORD dwErr = GetLastError();
		if( !bOK )
		{
			//ATLTRACE2( atlTraceGeneral, 2, _T("###Err### MFileWriter::FileWrite(%ws, %08X, %d) - WriteFile() FAILED dwRes = %d\n"),
			//	m_cbsFileName, (LONG)_pbData, _cbData, dwErr );

			return dwErr != 0 ? HRESULT_FROM_WIN32( dwErr ) : E_FAIL;
		}

		//ATLTRACE2( atlTraceGeneral, 8, _T("MFileWriter::FileWrite(%ws, %08X, %d) - WriteFile() dwRes = %d\n"),
		//	m_cbsFileName, (LONG)_pbData, _cbData, dwErr ); 

		return S_OK;
	}
};

class MLogWriter
{
	CComBSTR				m_cbsFileName;
	CComAutoCriticalSection m_csFile;
	HANDLE m_hFile;

public:
	MLogWriter()
		: m_hFile( NULL )
	{
	}

	~MLogWriter()
	{
		CAutoCS crs( m_csFile );

		if( m_hFile )
			::CloseHandle( m_hFile );
	}

	void WriteLog( LPCWSTR _pszFile, LPCSTR _pszString, bool _bDLLPath = false )
	{
		CAutoCS crs( m_csFile );

		CComBSTR cbsFileName = _pszFile;
		GetFullPath_Module( cbsFileName, !_bDLLPath );

		if(m_cbsFileName != cbsFileName && m_hFile )
		{
			::CloseHandle( m_hFile );
			m_hFile = NULL;
		}

		if( !m_hFile )
		{
			m_hFile = ::CreateFileW( cbsFileName, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL );
			::SetFilePointer( m_hFile, 0, NULL, SEEK_END );
		}

		m_cbsFileName = cbsFileName;

		DWORD cbWrite = 0;
		::WriteFile( m_hFile, _pszString, (DWORD)strlen( _pszString ), &cbWrite, NULL );
		::WriteFile( m_hFile, "\r\n", 2, &cbWrite, NULL );
		::FlushFileBuffers( m_hFile );
	}

	static void Write( LPCWSTR _pszFile, LPCSTR _pszString, bool _bDLLPath = false ) 
	{
#ifdef USE_MLOG_WRITER
		static MLogWriter s_log;
		s_log.WriteLog( _pszFile, _pszString );
#endif
	}
};


//////////////////////////////////////////////////////////////////////////
// 

#endif // _M_FUNCTIONS_H_INCLUDED_
