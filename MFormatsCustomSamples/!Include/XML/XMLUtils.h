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

#ifndef __XML_UTILS_H_
#define __XML_UTILS_H_

#include "../Base/DSDefines.h"
#include "../WinAPI/Macros.h"
#include "../WinAPI/blob.h"
#include "../WinAPI/Functions.h"

#include "DBLPrimitives.h"

#include "../Base/MFunctions.h"

#include <vector>
using std::vector;

#include <deque>
using std::deque;

#define DENIED_XML_CHARS	L"!\"#$%&'()*+,/;<=>?@[\]^`{|}~"

#define MAX_TABS	65
#define TABSTR( _nTab ) XMLUtils::PrintTab( MIN( MAX_TABS-1,_nTab), (LPWSTR)alloca( sizeof(WCHAR) * MAX_TABS ) )

#define BOOL2STRW(bVal)		( (bVal) ? L"true" : L"false" )

#define INT2STR(num)		( IntToStr((num), (LPTSTR)_alloca( 32*sizeof(TCHAR) )) )
#define INT2STRA(num)		( IntToStrA((num), (LPSTR)_alloca( 32*sizeof(CHAR) )) )
#define INT2STRW(num)		( IntToStrW((num), (LPWSTR)_alloca( 32*sizeof(WCHAR) )) )
#define INT64STRA(num)		( Int64ToStrA((num), (LPSTR)_alloca( 64*sizeof(CHAR) )) )
#define INT64STRW(num)		( Int64ToStrW((num), (LPWSTR)_alloca( 64*sizeof(WCHAR) )) )

#define INT2STR_SF(num)		( IntToStrW((num), (LPWSTR)__STR_BUF ) )
#define INT64STR_SF(num)	( Int64ToStrW((num), (LPWSTR)__STR_BUF ) )


#define MIN_INT( _val) ( (_val) > 0 ? (int)(_val) : (int)((_val)-1.0) )
#define MAX_INT( _val) ( (_val) > 0 ? (int)((_val)+1.0) : (int)((_val)) )

#define DBL2STR1W(num)				( DblToStrW((num), (LPWSTR)_alloca( 64*sizeof(WCHAR) ), 1) )
#define DBL2STR2W(num)				( DblToStrW((num), (LPWSTR)_alloca( 64*sizeof(WCHAR) ), 2) )
#define DBL2STR3W(num)				( DblToStrW((num), (LPWSTR)_alloca( 64*sizeof(WCHAR) ), 3) )
#define DBL2STRW(num)				( DblToStrW((num), (LPWSTR)_alloca( 64*sizeof(WCHAR) )) )
#define DBL2STRN(num, dec_places)	( DblToStrW((num), (LPWSTR)_alloca( 64*sizeof(CHAR) ), dec_places) )

#define DBL2STR_SF(num)				( DblToStrW((num), (LPWSTR)__STR_BUF ) )



#define DBL2STRN_SF(num, dec_places)			( DblToStrW((num), (LPWSTR)__STR_BUF, dec_places) )

#define DBL2STR1A(num)			( DblToStrA((num), (LPSTR)_alloca( 64*sizeof(CHAR) ), 1) )
#define DBL2STR2A(num)			( DblToStrA((num), (LPSTR)_alloca( 64*sizeof(CHAR) ), 2) )
#define DBL2STR3A(num)			( DblToStrA((num), (LPSTR)_alloca( 64*sizeof(CHAR) ), 3) )
#define DBL2STRA(num)			( DblToStrA((num), (LPSTR)_alloca( 64*sizeof(CHAR) )) )

#define DBL2STR1(num)			( DblToStr((num), (LPTSTR)_alloca( 64*sizeof(CHAR) ), 1) )
#define DBL2STR2(num)			( DblToStr((num), (LPTSTR)_alloca( 64*sizeof(CHAR) ), 2) )
#define DBL2STR3(num)			( DblToStr((num), (LPTSTR)_alloca( 64*sizeof(CHAR) ), 3) )
#define DBL2STR(num)			( DblToStr((num), (LPTSTR)_alloca( 64*sizeof(CHAR) )) )

#ifndef __DSPARSERS_H_


#define ST2STR(time)			( SysTime2StrW((time), (LPWSTR)_alloca( 64*sizeof(WCHAR) ), false) )
#define ST2STR_S(time)			( SysTime2StrW((time), (LPWSTR)_alloca( 64*sizeof(WCHAR) ), true) )

#define FCC2NAMEW(dwFCC)		(Fourcc2NameW( (dwFCC), (LPWSTR)_alloca(16*sizeof(WCHAR)) ))

inline LPCTSTR __stdcall SysTime2StrW(const SYSTEMTIME& _crSysTime, LPWSTR pszConv, bool _bShort /*= false*/)
{
	ATLASSERT(pszConv != NULL);
	if(pszConv == NULL) return NULL;

	if( _bShort || _crSysTime.wYear == 0 )
	{
		if( _crSysTime.wMilliseconds )
			wsprintfW( pszConv, L"%02d:%02d:%02d.%03d",
			_crSysTime.wHour, _crSysTime.wMinute, _crSysTime.wSecond, _crSysTime.wMilliseconds );
		else
			wsprintfW( pszConv, L"%02d:%02d:%02d",
			_crSysTime.wHour, _crSysTime.wMinute, _crSysTime.wSecond );
	}
	else
	{
		if( _crSysTime.wMilliseconds )
			wsprintfW( pszConv, L"%04d-%02d-%02d %02d:%02d:%02d.%03d",
			_crSysTime.wYear, _crSysTime.wMonth, _crSysTime.wDay, 
			_crSysTime.wHour, _crSysTime.wMinute, _crSysTime.wSecond, _crSysTime.wMilliseconds );
		else
			wsprintfW( pszConv, L"%04d-%02d-%02d %02d:%02d:%02d",
			_crSysTime.wYear, _crSysTime.wMonth, _crSysTime.wDay, 
			_crSysTime.wHour, _crSysTime.wMinute, _crSysTime.wSecond );
	}

	return pszConv;
}

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

#endif


#ifndef RGBA
#define RGBA(r,g,b, alpha) ((COLORREF)( ((DWORD)r & 0xFF)|(((DWORD)g & 0xFF)<<8)|(((DWORD)b & 0xFF)<<16)|(((DWORD)alpha & 0xFF)<<24) ) )
#endif

#ifdef UNICODE
#define IntToStr IntToStrW
#else
#define IntToStr IntToStrA
#endif // !UNICODE

#ifndef GetAlphaValue
#define GetAlphaValue(argb)      (LOBYTE(((DWORD)(argb))>>24))
#endif

inline LPCSTR __stdcall IntToStrA(int nNumber, LPSTR pszConv)
{
	wsprintfA( pszConv, "%d", nNumber );
	return pszConv;
}
inline LPCWSTR __stdcall IntToStrW(int nNumber, LPWSTR pszConv)
{
	wsprintfW( pszConv, L"%d", nNumber );
	return pszConv;
}

inline LPCSTR __stdcall Int64ToStrA(LONGLONG nNumber, LPSTR pszConv)
{
	wsprintfA( pszConv, "%I64d", nNumber );
	return pszConv;
}
inline LPCWSTR __stdcall Int64ToStrW(LONGLONG nNumber, LPWSTR pszConv)
{
	wsprintfW( pszConv, L"%I64d", nNumber );
	return pszConv;
}

#ifdef UNICODE
#define DblToStr DblToStrW
#else
#define DblToStr DblToStrA
#endif // !UNICODE

inline LPCSTR __stdcall DblToStrA(double dblNumber, LPSTR pszConv, int nPrecise = -1)
{
	int nChars = 0;
	if( nPrecise >= 0 )
	{
		CHAR szFormat[8]={0};
		sprintf( szFormat, "%%.0%df", nPrecise );
		nChars = sprintf( pszConv, szFormat, dblNumber );
	}
	else
	{
		nChars = sprintf( pszConv, "%f", dblNumber);
		//nChars--; // (the last digit usually wrong e.g. 0.2500000001) 
	}

	ATLASSERT( nChars >= 0 );
	if( nChars > 3 && (nPrecise < 0 || nPrecise > 2) )
	{
		// Cut off trailing zerous (the last digit usually wrong e.g. 0.2500000001)
		LPSTR pszTemp = pszConv + nChars; 
		while( pszTemp > pszConv + 2 && *(pszTemp - 2) != L'.' && *(pszTemp - 1) == L'0' )
			pszTemp--;
		*pszTemp = 0;
	}
	

	return pszConv;
}

inline LPCWSTR __stdcall DblToStrW(double dblNumber, LPWSTR pszConv, int nPrecise = -1)
{
	int nChars = 0;
	if( nPrecise >= 0 )
	{
		WCHAR szFormat[8]={0};
		swprintf( szFormat, L"%%.0%df", nPrecise );
		nChars = swprintf( pszConv, szFormat, dblNumber );
	}
	else
	{
		nChars = _swprintf( pszConv, L"%f", dblNumber);
		//nChars--; // (the last digit usually wrong e.g. 0.2500000001)
	}

	ATLASSERT( nChars >= 0 );
	if( nChars > 3 && (nPrecise < 0 || nPrecise > 2) )
	{
		// Cut off trailing zerous (the last digit usually wrong e.g. 0.2500000001)
		LPWSTR pszTemp = pszConv + nChars; 
		while( pszTemp > pszConv + 2 && *(pszTemp - 2) != L'.' && *(pszTemp - 1) == L'0' )
			pszTemp--;
		*pszTemp = 0;
	}

	return pszConv;
}

inline DWORD __stdcall BGR2RGB(DWORD _dwValue)
{
	DWORD dwRes = _dwValue;
	LPBYTE pbRes = (LPBYTE)&dwRes;
	LPBYTE pbSrc = (LPBYTE)&_dwValue;
	pbRes[0] = pbSrc[2];
	pbRes[2] = pbSrc[0];
	return dwRes;
}

// Convert the #rgb -> e.g. 0x909 -> 0x990099
inline DWORD __stdcall WEB2RGB(DWORD _dwValue)
{
	DWORD dwRes = 0;
	LPBYTE pbRes = (LPBYTE)&dwRes;
	LPBYTE pbSrc = (LPBYTE)&_dwValue;
	pbRes[0] = (pbSrc[0] & 0xF) * 0x11;
	pbRes[1] = (pbSrc[0] >> 4) * 0x11;
	pbRes[2] = (pbSrc[1] & 0xF) * 0x11;
	return dwRes;
}









#define HAVE_PREFIX(str,prefix) (StrCmpNIW( prefix, str, MIN( lstrlenW(prefix),lstrlenW(str) ) ) == 0)

#define HEX2STR(num)		( HexToStr((num), (LPTSTR)_alloca( 32*sizeof(TCHAR) )) )
#define HEX2STRA(num)		( HexToStrA((num), (LPSTR)_alloca( 32*sizeof(CHAR) )) )
#define HEX2STRW(num)		( HexToStrW((num), (LPWSTR)_alloca( 32*sizeof(WCHAR) )) )
#define HEX64STRA(num)		( Hex64ToStrA((num), (LPSTR)_alloca( 64*sizeof(CHAR) )) )
#define HEX64STRW(num)		( Hex64ToStrW((num), (LPWSTR)_alloca( 64*sizeof(WCHAR) )) )

#ifdef UNICODE
#define HexToStr HexToStrW
#else
#define HexToStr HexToStrA
#endif // !UNICODE

inline LPCSTR __stdcall HexToStrA(int nNumber, LPSTR pszConv)
{
	wsprintfA( pszConv, "0x%08X", nNumber );
	return pszConv;
}
inline LPCWSTR __stdcall HexToStrW(int nNumber, LPWSTR pszConv)
{
	wsprintfW( pszConv, L"0x%08X", nNumber );
	return pszConv;
}

inline LPCSTR __stdcall Hex64ToStrA(ULONGLONG nNumber, LPSTR pszConv)
{
	sprintf( pszConv, "0x%016I64X", nNumber );
	return pszConv;
}
inline LPCWSTR __stdcall Hex64ToStrW(ULONGLONG nNumber, LPWSTR pszConv)
{
	swprintf( pszConv, L"0x%016I64X", nNumber );
	return pszConv;
}

static inline BYTE GetColorAlpha( LPCWSTR _wszColor, int nDefValue = 255 )
{
	if( !_wszColor ) return nDefValue;
	LPWSTR pAlpha = StrChrW( _wszColor, L'(' );
	LPWSTR pDot = StrChrW( _wszColor, L'.' );
	if( !pAlpha ) return nDefValue;
	double dblRes = _wtof( pAlpha + 1);
	return INT2BYTE( DBL2INT( dblRes < 1 || pDot ? 255 * dblRes : dblRes ) );
}

static inline LPCWSTR Color2Str_I( COLORREF _crValue )
{
	_crValue &= 0x00FFFFFF;
	switch( _crValue )
	{
	case 0: return L"Black";
	case 0x00FFFFFF: return L"White";
	case RGB( 0xFF, 0xA5, 0): return L"Orange";
	case RGB( 255, 0, 0): return L"Red";
	case RGB( 0, 255, 0): return L"Lime";
	case RGB( 0, 0, 255): return L"Blue";
	case RGB( 0, 255, 255): return L"Aqua";
	case RGB( 255, 0, 255): return L"Magenta";
	case RGB( 255, 255, 0): return L"Yellow";
	case RGB( 128, 0, 0): return L"Maroon";
	case RGB( 139, 69, 19): return L"Brown";
	case RGB( 0, 128, 0): return L"Green";
	case RGB( 0, 0, 128): return L"Navy";
	case RGB( 0, 128, 128): return L"Teal";
	case RGB( 128, 128, 128): return L"Grey";
	case RGB( 128, 0, 128): return L"Purple";
	case RGB( 128, 128, 0): return L"Olive";
	case RGB( 192, 192, 192): return L"Silver";
	case RGB( 6, 84, 138): return L"ML";
	case RGB( 0, 144, 178): return L"MLL";
	case RGB( 255, 128, 128): return L"Pink";
	case RGB( 255, 0x45, 0): return L"OrangeRed";
	}
	return NULL;
}
static inline LPCWSTR Color2StrW( COLORREF _crValue, LPWSTR _pszBuffer )
{
	int nColor = _crValue & 0x00FFFFFF;
	int nAlpha = GetAlphaValue( _crValue );
	LPCWSTR pszColor = Color2Str_I( nColor );
	if( pszColor && nAlpha == 255 ) 
		return pszColor;

	if( pszColor )
		wsprintfW( _pszBuffer, L"%s(%d)", pszColor, nAlpha );
	else if( nAlpha == 255 )
		wsprintfW( _pszBuffer, L"#%06X", BGR2RGB(nColor) );
	else
		wsprintfW( _pszBuffer, L"#%06X(%d)", BGR2RGB(nColor), nAlpha );

	return _pszBuffer;
}
static inline LPCSTR Color2StrA( COLORREF _crValue, LPSTR _pszBuffer )
{
	USES_CONVERSION;

	int nColor = _crValue & 0x00FFFFFF;
	int nAlpha = GetAlphaValue( _crValue );
	LPCWSTR pszColor = Color2Str_I( nColor );
	if( pszColor && nAlpha == 255 ) 
		wsprintfA( _pszBuffer, "%s", W2A(pszColor) );
	else if( pszColor )
		wsprintfA( _pszBuffer, "%s(%d)", W2A(pszColor), nAlpha );
	else if( nAlpha == 255 )
		wsprintfA( _pszBuffer, "#%06X", BGR2RGB(nColor) );
	else
		wsprintfA( _pszBuffer, "#%06X(%d)", BGR2RGB(nColor), nAlpha );

	return _pszBuffer;
}

#define XML_COLOR_SEPARATOR	L"~!@#$%^&*-+=|\\/.,;`"	

static inline COLORREF Str2Color( LPCWSTR _wszValue, COLORREF _crDefValue = 0 )
{
	LPCWSTR pszRes = _wszValue;
	if( !pszRes ) return _crDefValue;

	WCHAR szColor[64]={0};
	lstrcpynW( szColor, pszRes, SIZEOF_ARRAY(szColor) );

	// Next color (e.g. gradient)
	LPWSTR pszNext = szColor + 1 + StrCSpnW( szColor + 1, XML_COLOR_SEPARATOR );
	pszNext[0] = 0;

	BYTE btAlpha = 255;
	LPWSTR pszAlpha = StrChrW( szColor, L'(' );
	if( pszAlpha )
	{
		LPWSTR pDot = StrChrW( pszAlpha, L'.' );
		
		double dblRes = _wtof( pszAlpha + 1);
		btAlpha = INT2BYTE( DBL2INT( dblRes < 1 || pDot ? 255 * dblRes : dblRes ) );

		pszAlpha[0] = 0;
	}

	if( lstrcmpiW( szColor, L"Default" ) == NULL || pszAlpha == szColor ) 
		return RGBA( GetRValue(_crDefValue), GetGValue(_crDefValue), GetBValue(_crDefValue), pszAlpha ? btAlpha : GetAlphaValue(_crDefValue) );

	if( lstrcmpiW( szColor, L"Orange" ) == NULL ) return RGBA( 0xFF, 0xA5, 0, btAlpha );
	if( lstrcmpiW( szColor, L"OrangeRed" ) == NULL ) return RGBA( 0xFF, 0x45, 0, btAlpha );

	if( lstrcmpiW( szColor, L"Aqua" ) == NULL || lstrcmpiW( szColor, L"cyan" ) == NULL ) return RGBA( 0, 255, 255, btAlpha );
	if( lstrcmpiW( szColor, L"Black" ) == NULL  ) return RGBA( 0, 0, 0, btAlpha );
	if( lstrcmpiW( szColor, L"Blue" ) == NULL ) return RGBA( 0, 0, 255, btAlpha );
	if( lstrcmpiW( szColor, L"Magenta" ) == NULL ) return RGBA( 255, 0, 255, btAlpha );

	if( lstrcmpiW( szColor, L"Grey" ) == NULL ) return RGBA( 128, 128, 128, btAlpha );
	if( lstrcmpiW( szColor, L"Green" ) == NULL ) return RGBA( 0, 128, 0, btAlpha );
	if( lstrcmpiW( szColor, L"Lime" ) == NULL ) return RGBA( 0, 255, 0, btAlpha );
	if( lstrcmpiW( szColor, L"Maroon" ) == NULL ) return RGBA( 128, 0, 0, btAlpha );
	if( lstrcmpiW( szColor, L"Brown" ) == NULL ) return RGBA( 139, 69, 19, btAlpha );

	if( lstrcmpiW( szColor, L"Navy" ) == NULL ) return RGBA( 0, 0, 128, btAlpha );
	if( lstrcmpiW( szColor, L"Olive" ) == NULL ) return RGBA( 128, 128, 0, btAlpha );
	if( lstrcmpiW( szColor, L"Purple" ) == NULL ) return RGBA( 128, 0, 128, btAlpha );
	if( lstrcmpiW( szColor, L"Red" ) == NULL ) return RGBA( 255, 0, 0, btAlpha );

	if( lstrcmpiW( szColor, L"Silver" ) == NULL ) return RGBA( 192, 192, 192, btAlpha );
	if( lstrcmpiW( szColor, L"Teal" ) == NULL ) return RGBA( 0, 128, 128, btAlpha );
	if( lstrcmpiW( szColor, L"White" ) == NULL ) return RGBA( 255, 255, 255, btAlpha );
	if( lstrcmpiW( szColor, L"Yellow" ) == NULL ) return RGBA( 255, 255, 0, btAlpha );
	if( lstrcmpiW( szColor, L"Pink" ) == NULL ) return RGBA( 255, 128, 128, btAlpha );
	if( lstrcmpiW( szColor, L"Transparent" ) == NULL ) return RGBA( 0, 0, 0, 0 );

	if( lstrcmpiW( szColor, L"medialooks" ) == NULL || lstrcmpiW( szColor, L"ml" ) == NULL ) return RGBA( 6, 84, 138, btAlpha );
	if( lstrcmpiW( szColor, L"medialooks-light" ) == NULL || lstrcmpiW( szColor, L"mll" ) == NULL || lstrcmpiW( szColor, L"mllight" ) == NULL ) return RGBA( 0, 144, 178, btAlpha );

	LPWSTR pEnd = 0;
	WCHAR wszTemp[16] = {0};
	wszTemp[0] = L'0';
	wszTemp[1] = L'x';
	if( szColor[0] == L'#' )
		lstrcpynW( wszTemp + 2, szColor + 1, 14 );
	else if( ((szColor[0] >= L'0' && szColor[0] <= L'9') || (szColor[0] >= L'A' && szColor[0] <= L'F')) && 
		((szColor[1] >= L'0' && szColor[1] <= L'9') || (szColor[1] >= L'A' && szColor[1] <= L'F')) )
		lstrcpynW( wszTemp + 2, szColor, 14 );
	else
		lstrcpynW( wszTemp, szColor, 14 );
	UINT nRes = wcstoul( wszTemp, &pEnd, 0 );
	if( pEnd == wszTemp ) return _crDefValue; // No conversion
	if( pEnd - wszTemp <= 5 )
	{
		// #rgb color type
		nRes = WEB2RGB( nRes );
	}
	nRes = BGR2RGB( nRes );

	// Check for alpha 0xFF000000 or 0x000000
	if( lstrlenW(wszTemp) <= 8 ) nRes |= (UINT)btAlpha << 24;

	return nRes;
}


static inline FOURCC Str2Fcc( LPCWSTR _szFCC )
{
	if( !_szFCC ) return 0;

	if( lstrcmpiW( _szFCC, L"ARGB32" ) == 0 )
		return FOURCC_ARGB32;

	if( lstrcmpiW( _szFCC, L"RGB32" ) == 0 )
		return FOURCC_RGB32;

	if( lstrcmpiW( _szFCC, L"RGB24" ) == 0 )
		return FOURCC_RGB24;

	if( lstrcmpiW( _szFCC, L"YUY2" ) == 0 )
		return FOURCC_YUY2;

	if( lstrcmpiW( _szFCC, L"YVYU" ) == 0 )
		return FOURCC_YVYU;

	if( lstrcmpiW( _szFCC, L"UYVY" ) == 0 )
		return FOURCC_UYVY;

	if( lstrcmpiW( _szFCC, L"YV12" ) == 0 )
		return FOURCC_YV12;

	if( lstrcmpiW( _szFCC, L"HDYC" ) == 0 )
		return FOURCC_HDYC;

	if( lstrcmpiW( _szFCC, L"I420" ) == 0 )
		return FOURCC_I420;

	if( lstrcmpiW( _szFCC, L"Y422" ) == 0 )
		return FOURCC_Y422;

	if( lstrcmpiW( _szFCC, L"NV12" ) == 0 )
		return FOURCC_NV12;

	if( lstrcmpiW( _szFCC, L"Y444" ) == 0 )
		return FOURCC_Y444;

	if( lstrcmpiW( _szFCC, L"v210" ) == 0 )
		return FOURCC_v210;

	if( lstrcmpiW( _szFCC, L"r210" ) == 0 )
		return FOURCC_r210;

	if( lstrcmpiW( _szFCC, L"MJPG" ) == 0 )
		return FCC('MJPG');

	if( lstrcmpiW( _szFCC, L"RGB8" ) == 0 )
		return FOURCC_RGB8;

	if( lstrcmpiW( _szFCC, L"BI_RGB" ) == 0 )
		return BI_RGB;

	if( lstrcmpiW( _szFCC, L"BI_RLE8" ) == 0 )
		return BI_RLE8;

	if( lstrcmpiW( _szFCC, L"BI_RLE4" ) == 0 )
		return BI_RLE4;

	if( lstrcmpiW( _szFCC, L"BI_BITFIELDS" ) == 0 )
		return BI_BITFIELDS;

	if( lstrcmpiW( _szFCC, L"ARGB4444" ) == 0 )
		return 0x6e6415e6; 

	if( lstrlenW(_szFCC) == 4 )
	{
		USES_CONVERSION;

		DWORD dwFCC = *(DWORD*)W2CA(_szFCC );
		return dwFCC;
	}

	// Custom 
	return 0;
}

#define COLOR2STRA(num)		( Color2StrA((num), (LPSTR)_alloca( 32*sizeof(CHAR) )) )
#define COLOR2STRW(num)		( Color2StrW((num), (LPWSTR)_alloca( 32*sizeof(WCHAR) )) )

#define PAIR2STR(first,second)	( XMLUtils::Pair2Str( pair<int,int>(first,second), (LPWSTR)_alloca( 64*sizeof(WCHAR) ) ) )
#define PAIR2STR2(first,second,delimer)	( XMLUtils::Pair2Str( pair<int,int>(first,second), (LPWSTR)_alloca( 64*sizeof(WCHAR) ), delimer ) )

#ifdef UNICODE
#define COLOR2STR COLOR2STRW
#else
#define COLOR2STR COLOR2STRA
#endif // !UNICODE

#ifndef TRACELEVEL_XMLParse
#define TRACELEVEL_XMLParse 10
#endif

#include <utility>
using std::pair;

/*
#ifndef M_DEFINES_INCLUDED
#define M_DEFINES_INCLUDED
typedef 
	enum eMState
{	eMS_Closed	= 0,
eMS_Stopped	= ( eMS_Closed + 1 ) ,
eMS_Paused	= ( eMS_Stopped + 1 ) ,
eMS_Running	= ( eMS_Paused + 1 ) 
} 	eMState;

typedef struct M_TIMECODE
{
	int nTrack;
	int nHours;
	int nMinutes;
	int nSeconds;
	int nFrames;
} 	M_TIMECODE;

typedef 
	enum eMFCC
{	eMFCC_Default	= 0,
eMFCC_r210	= 0x30313272,
eMFCC_v210	= 0x30313276,
eMFCC_I420	= 0x30323449,
eMFCC_YV12	= 0x32315659,
eMFCC_YUY2	= 0x32595559,
eMFCC_HDYC	= 0x43594448,
eMFCC_YVYU	= 0x55595659,
eMFCC_UYVY	= 0x59565955,
eMFCC_RGB24	= 0xe436eb7d,
eMFCC_RGB32	= 0xe436eb7e,
eMFCC_ARGB32	= 0x773c9ac0
} 	eMFCC;

typedef 
	enum eMKeepAspect
{	eMKA_Default	= 0,
eMKA_None	= ( eMKA_Default + 1 ) ,
eMKA_LetterBox	= ( eMKA_None + 1 ) ,
eMKA_Crop	= ( eMKA_LetterBox + 1 ) 
} 	eMKeepAspect;

typedef 
	enum eMInterlace
{	eMI_Default	= 0,
eMI_Field1First	= 0x1,
eMI_Field2First	= 0x2,
eMI_Progressive	= 0x3
} 	eMInterlace;

typedef 
	enum eM3DFormat
{	eM3D_None	= 0,
eM3D_Auto	= 0x1,
eM3D_TopAndBottom	= 0x2,
eM3D_SideBySide	= 0x3
} 	eM3DFormat;

typedef 
	enum eMVideoFormat
{	eMVF_Custom	= 0,
eMVF_PAL	= ( eMVF_Custom + 1 ) ,
eMVF_NTSC	= ( eMVF_PAL + 1 ) ,
eMVF_HD720_50p	= ( eMVF_NTSC + 1 ) ,
eMVF_HD720_5994p	= ( eMVF_HD720_50p + 1 ) ,
eMVF_HD720_60p	= ( eMVF_HD720_5994p + 1 ) ,
eMVF_HD1080_2398p	= ( eMVF_HD720_60p + 1 ) ,
eMVF_HD1080_24p	= ( eMVF_HD1080_2398p + 1 ) ,
eMVF_HD1080_25p	= ( eMVF_HD1080_24p + 1 ) ,
eMVF_HD1080_2997p	= ( eMVF_HD1080_25p + 1 ) ,
eMVF_HD1080_30p	= ( eMVF_HD1080_2997p + 1 ) ,
eMVF_HD1080_50i	= ( eMVF_HD1080_30p + 1 ) ,
eMVF_HD1080_5994i	= ( eMVF_HD1080_50i + 1 ) ,
eMVF_HD1080_60i	= ( eMVF_HD1080_5994i + 1 ) ,
eMVF_HD1080_50p	= ( eMVF_HD1080_60i + 1 ) ,
eMVF_HD1080_5994p	= ( eMVF_HD1080_50p + 1 ) ,
eMVF_HD1080_60p	= ( eMVF_HD1080_5994p + 1 ) ,
eMVF_NTSC_2398	= ( eMVF_HD1080_60p + 1 ) ,
eMVF_NTSC_p	= ( eMVF_NTSC_2398 + 1 ) ,
eMVF_PAL_p	= ( eMVF_NTSC_p + 1 ) ,
eMVF_2K_2398p	= ( eMVF_PAL_p + 1 ) ,
eMVF_2K_24p	= ( eMVF_2K_2398p + 1 ) ,
eMVF_2K_25p	= ( eMVF_2K_24p + 1 ) ,
eMVF_Disabled	= ( eMVF_2K_25p + 1 )
} 	eMVideoFormat;

typedef struct M_VID_PROPS
{
	eMVideoFormat eVideoFormat;
	eMFCC fccType;
	int nWidth;
	int nHeight;
	int nRowBytes;
	eMInterlace eInterlace;
	short nAspectX;
	short nAspectY;
	double dblRate;
	eMKeepAspect eKeepAspect;
	eM3DFormat e3DFormat;
} 	M_VID_PROPS;


typedef struct M_AUD_PROPS
{
	int nChannels;
	int nSamplesPerSec;
	int nBitsPerSample;
} 	M_AUD_PROPS;

typedef struct M_TIME
{
	M_TIMECODE tcFrame;
	BOOL bStreamBreak;
	REFERENCE_TIME rtStartTime;
	REFERENCE_TIME rtEndTime;
} 	M_TIME;

typedef struct M_AV_PROPS
{
	M_VID_PROPS vidProps;
	M_AUD_PROPS audProps;
} 	M_AV_PROPS;

#endif // M_DEFINES_INCLUDED
*/
/*
#ifndef MG_AV_PROPS_DEFINED
#define MG_AV_PROPS_DEFINED
typedef 
	enum eMGFCC
{	eMGFCC_Default	= 0,
eMGFCC_YV12	= 0x32315659,
eMGFCC_I420	= 0x30323449,	
eMGFCC_YUY2	= 0x32595559,
eMGFCC_UYVY	= 0x59565955,
eMGFCC_YVYU	= 0x55595659,
eMGFCC_HDYC	= 0x43594448,
eMGFCC_RGB24	= 0xe436eb7d,
eMGFCC_RGB32	= 0xe436eb7e,
eMGFCC_ARGB32	= 0x773c9ac0,
eMGFCC_v210	= 0x30313276,
eMGFCC_r210	= 0x30313272,
eMGFCC_NoVideo	= 0xffffffff
} 	eMGFCC;

typedef 
	enum eMGKeepAspect
{	eMGKA_Default	= 0,
eMGKA_None	= ( eMGKA_Default + 1 ) ,
eMGKA_LetterBox	= ( eMGKA_None + 1 ) ,
eMGKA_Crop	= ( eMGKA_LetterBox + 1 ) 
} 	eMGKeepAspect;

typedef 
	enum eMGInterlace
{	eMGI_Default	= 0,
eMGI_Field1First	= 0x1,
eMGI_Field2First	= 0x2,
eMGI_Progressive	= 0x3,
eMGI_FieldsOrderMask	= 0xf,
eMGI_3D_SideBySide	= 0x10,
eMGI_3D_TopBottom	= 0x20,
eMGI_3D_Interlaced	= 0x40,
eMGI_3DMask	= 0xff0,
eMGI_Field1First_3DSide	= 0x11,
eMGI_Field2First_3DSide	= 0x12,
eMGI_Progressive_3DSide	= 0x13,
eMGI_Field1First_3DTopBottom	= 0x21,
eMGI_Field2First_3DTopBottom	= 0x22,
eMGI_Progressive_3DTopBottom	= 0x23,
eMGI_Progressive_3DInterlaced	= 0x43
} 	eMGInterlace;

typedef 
	enum eMGVidQuality
{	eMGVQ_Default	= 0,
eMGVQ_Fast	= ( eMGVQ_Default + 1 ) ,
eMGVQ_Precise	= ( eMGVQ_Fast + 1 ) 
} 	eMGVidQuality;

typedef struct MG_VID_PROPS
{
	int nWidth;
	int nHeight;
	eMGFCC fccType;
	eMGInterlace eInterlace;
	short nAspectX;
	short nAspectY;
	eMGKeepAspect eKeepAspect;
	double dblRate;
} 	MG_VID_PROPS;

typedef struct MG_AUD_PROPS
{
	int nChannels;
	int nSamplesPerSec;
	int nBitsPerSample;
} 	MG_AUD_PROPS;

#endif // MG_AV_PROPS_DEFINED

static LPCWSTR eMG_KeepAspectStr[] = { L"default", L"none", L"letter-box", L"crop", L"no-scale", NULL };
static LPCWSTR eMG_InterlaceStr[] = { L"auto", L"top", L"bottom", L"progressive", NULL };

*/





struct XMLNamePart
{
	LPCWSTR pszPart;
	int     nLen;
	int		nOrder;

	inline CComBSTR Name() { return CComBSTR( nLen, pszPart); }


	// For access by 'id'
	LPCWSTR pszID;
	int		nIDLen;

	inline CComBSTR Id() { return CComBSTR(nIDLen, pszID); }
	inline bool HaveId() { return pszID != NULL && nIDLen > 0; }
};

struct XMLUnit
{
	double dblUnits;	
	int	   nType;		// Exact, Percent (%), Pixels (px), Points (pt)
};


// Here collected methods what not used XMLNode struct
struct XMLUtils
{
	friend class XMLParse;

	static inline LPCWSTR SkipPrefix( LPCWSTR _pszName, int _nSkip )
	{
		LPCWSTR pszRes = _pszName;
		while( _nSkip > 0 )
		{
			LPCWSTR pszFind = StrStrW( pszRes, L"::" );
			if( pszFind )
				pszRes = pszFind + 2;
			else
				break;

			_nSkip--;
		}

		return pszRes;
	}

	static inline XMLUnit MakeUnit( double _dblValue, int _nType )
	{
		XMLUnit unit = { _dblValue, _nType};
		return unit;
	}

	static inline XMLUnit MakePercent( double _dblValue, double _dblBase )
	{
		XMLUnit unit = { _dblValue, 0};

		if( _dblBase != 0 )
		{
			unit.dblUnits = unit.dblUnits * 100 / _dblBase;
			unit.nType = 1;
		}

		return unit;
	}

	static inline double Unit2Value( const XMLUnit& _crUnit, double _dblBase )
	{
		if( _crUnit.nType == 1 )
			return _dblBase != 0 ? (_crUnit.dblUnits * _dblBase) / 100 : _crUnit.dblUnits;
		else
			return _crUnit.dblUnits;
	}

	static XMLUnit Str2Unit( LPCWSTR pszRes )
	{
		XMLUnit unit = {0}; 
		if( !pszRes ) return unit;

		LPWSTR pEnd = 0;
		unit.dblUnits = wcstod( pszRes, &pEnd );
		if( pEnd && *pEnd == L'%' )
			unit.nType = 1;
		else if( pEnd && *pEnd == L'p' && *(pEnd + 1) == L'x' )
			unit.nType = 2;
		else if( pEnd && *pEnd == L'p' && *(pEnd + 1) == L't' )
			unit.nType = 3;

		return unit;
	}

	static LPCWSTR Unit2Str( const XMLUnit& _tValue, LPWSTR _pszPrint)
	{
		if( _tValue.nType == 3 )
			swprintf(_pszPrint, L"%dpt", DBL2INT(_tValue.dblUnits) );
		else if( _tValue.nType == 2 )
			swprintf(_pszPrint, L"%dpx", DBL2INT(_tValue.dblUnits) );
		else if( _tValue.nType == 1 )
			swprintf(_pszPrint, L"%.03f%%", _tValue.dblUnits );
		else
			swprintf(_pszPrint, L"%.02f", _tValue.dblUnits );

		return _pszPrint;
	}

	

// 	static XMLUnit Str2DblPerc( LPCWSTR pszRes, int nBaseSize = -100 )
// 	{
// 		if( !pszRes ) return 0;
// 
// 		LPWSTR pEnd = 0;
// 		XMLUnit unit = { wcstod( pszRes, &pEnd ), 0 };
// 		if( pEnd && *pEnd == L'%' )
// 			nRes = nBaseSize != 0 ? DBL2INT( nRes * nBaseSize / 100 ) : nRes; // For percent
// 
// 		return nRes;
// 	}
// 
// 	static LPCWSTR Dbl2StrPerc( const double& _tValue, const double& _dblBaseSize, LPWSTR _pszPrint)
// 	{
// 		if( _dblBaseSize != 0)
// 		{
// 			double dblValue = _tValue * 100 / _dblBaseSize;
// 			swprintf(_pszPrint, L"%.03f%%", dblValue );
// 
// 			return _pszPrint;
// 		}
// 		return DblToStrW( _tValue, 5, _pszPrint );
// 	}

	

	static double Str2AR( LPCWSTR _szString, double dblDefault = 0.0 )
	{
		if( !_szString )
			return dblDefault;

		if( StrChrW( _szString, L':' ) ) 
		{
			std::pair<int,int> ar = Str2Pair( _szString );
			if( ar.second )
			{
				return (double)ar.first / ar.second;
			}
		}	

		return _wtof( _szString );
	}

	static std::pair<int,int> Str2Pair( LPCWSTR _szPair, int* _pnFirst, int* _pnSecond, std::pair<int,int> _prDefault = std::pair<int,int>(0,0) )
	{
		std::pair<int,int> res = _prDefault;

		if( _szPair && StrCSpnW( _szPair, L"0123456789" ) < lstrlenW(_szPair) )
		{
			LPWSTR pEnd = 0;
			res.first = wcstol( _szPair, &pEnd, 0 );
			if( pEnd )
			{ 
				res.second = wcstol( pEnd + 1, &pEnd, 0 );
			}
		}

		if( _pnFirst ) *_pnFirst = res.first;
		if( _pnSecond ) *_pnSecond = res.second;

		return res;
	}


	static std::pair<int,int> Str2Pair( LPCWSTR _szString, std::pair<int,int> _prDefault = std::pair<int,int>(0,0) )
	{
		return Str2Pair( _szString, NULL, NULL, _prDefault );
	}

	static LPCWSTR Pair2Str( const std::pair<int,int>& _value, LPWSTR _szBuffer, char chSep = L':')
	{
		wsprintfW( _szBuffer, L"%d%c%d", _value.first, chSep, _value.second );
		return _szBuffer;
	}

	static LPCWSTR Enum2Str(const LPCWSTR _checkArray[], int _nValue, LPCWSTR _wsDefValue = NULL )
	{
		if( _nValue < 0 ) return _wsDefValue;

		for( int i = 0; i < _nValue; i++ )
		{
			if( !_checkArray[i] ) return _wsDefValue;
		}

		return _checkArray[_nValue] ? _checkArray[_nValue] : _wsDefValue;
	}

	template <class TEnum>
	static TEnum Str2Enum( const LPCWSTR _checkArray[], LPCWSTR _wszValue, TEnum nDefValue )
	{
		if( !_wszValue ) return nDefValue;

		int nRet = 0;
		while( _checkArray[nRet] )
		{
			if( lstrcmpiW( _checkArray[nRet], _wszValue ) == 0  ) return (TEnum)nRet;

			nRet++;
		} 

		return (TEnum)nDefValue;
	}

	static int Str2Enum( const LPCWSTR _checkArray[], LPCWSTR _wszValue )
	{
		if( !_wszValue ) return -1;

		int nRet = 0;
		while( _checkArray[nRet] )
		{
			if( lstrcmpiW( _checkArray[nRet], _wszValue ) == 0  ) return nRet;

			nRet++;
		} 

		return -1;
	}

	template <class TEnum>
	static bool StrGetEnum(const LPCWSTR _checkArray[], LPCWSTR _wszValue, TEnum& _rValue )
	{
		if( !_wszValue ) return false;

		int nRet = 0;
		while( _checkArray[nRet] )
		{
			if( lstrcmpiW( _checkArray[nRet], _wszValue ) == 0  )
			{
				_rValue = (TEnum)nRet;
				return true;
			}

			nRet++;
		} 

		return false;
	}

	static inline bool _ParamStr_Value(LPCWSTR pStr, BSTR* _pbsValue, LPCWSTR pEnd = NULL, LPCWSTR* _ppszNext = NULL )
	{
		if (STR_IS_EMPTY(pStr))
			return false;

		// Get value
		int nSkip = 0;
		const WCHAR* pVarEnd = NULL;
		if (*pStr == L'\'')
		{
			pStr++;
			// Find end 
			pVarEnd = StrChrW(pStr, L'\'');
			if( pVarEnd && (!pEnd || pVarEnd < pEnd ) )
				nSkip = 1;
		}
		else if (*pStr == L'\"')
		{
			pStr++;
			// Find end 
			pVarEnd = StrChrW(pStr, L'\"');
			if (pVarEnd && (!pEnd || pVarEnd < pEnd))
				nSkip = 1;
		}
		else
		{
			// Find blank
			pVarEnd = pStr;
			while ( (pVarEnd < pEnd || pEnd == NULL) && *pVarEnd && !XMLUtils::IsBlank(*pVarEnd)) pVarEnd++;
		}

		if (!pVarEnd)
			pVarEnd = pEnd ? pEnd : (pStr + lstrlenW(pStr));
		else if( pEnd )
			pVarEnd = MIN(pVarEnd, pEnd);

		if (_ppszNext)
			*_ppszNext = pVarEnd + nSkip;

		if( _pbsValue )
			*_pbsValue = ::SysAllocStringLen(pStr, pVarEnd - pStr);

		return true;
	}

	static CComBSTR ParamStr_GetString(LPCWSTR _pszParamStr, LPCWSTR _pszParamName)
	{
		if (STR_IS_EMPTY(_pszParamStr) || STR_IS_EMPTY(_pszParamName))
			return NULL;

		LPCWSTR pszValue = StrStrIW(_pszParamStr, _pszParamName);
		while (pszValue != NULL)
		{
			pszValue += lstrlenW(_pszParamName);
			pszValue = SkipBlank(pszValue, false);
			if (*pszValue != L'=')
			{
				pszValue = StrStrIW(pszValue, _pszParamName);
				continue;
			}
			
			ATLASSERT(*pszValue == L'=');
			++pszValue; // Skip '='

			pszValue = SkipBlank(pszValue, false);
			CComBSTR cbsValue;
			_ParamStr_Value(pszValue, &cbsValue);
			return cbsValue;
		}

		return NULL;
	}

	static LONGLONG ParamStr_GetLong(LPCWSTR _pszParamStr, LPCWSTR _pszParamName, LONGLONG _llDefault = -1 )
	{
		CComBSTR cbsValue = ParamStr_GetString(_pszParamStr, _pszParamName);
		if (cbsValue.m_str && IsDigit(*cbsValue.m_str) )
			return _wtoi64(cbsValue.m_str);

		return _llDefault;
	}


	static HRESULT Array2String( const CSimpleArray2<CComBSTR>& _rArray, BSTR* _pbsRes, LPCWSTR _pszDelimer = L", " )
	{
		if( !_pbsRes ) return E_POINTER;

		if( !_rArray.GetSize() ) return E_INVALIDARG;

		CComBSTR cbsRes;
		for( int i = 0; i < _rArray.GetSize() - 1 ; i++ )
		{
			cbsRes.Append( _rArray[i] );
			cbsRes.Append( _pszDelimer );
		}

		cbsRes.Append( _rArray[_rArray.GetSize() - 1] );

		*_pbsRes = cbsRes.Detach();
		return S_OK;
	}

	static int StringSplit( LPCWSTR _wsString, LPCWSTR _wsDelimer, CSimpleArray2<CComBSTR>& _rArray, BOOL _bIgoreCase = FALSE)
	{
		if( !_wsString || !_wsDelimer )
			return -1;

		int nCount = 1;
		int nDelLen = lstrlenW(_wsDelimer);
		LPCWSTR pszStart = _wsString;
		while( pszStart )
		{
			LPCWSTR pszNext = _bIgoreCase ? StrStrIW( pszStart, _wsDelimer ) : StrStrW( pszStart, _wsDelimer );
			if( !pszNext )
			{
				CComBSTR cbsNext( pszStart );
				_rArray.Add( cbsNext );

				break;
			}

			CComBSTR cbsNext( pszNext - pszStart, pszStart );
			_rArray.Add( cbsNext );

			pszStart = pszNext + nDelLen;

			nCount++;
		}

		return nCount;
	}

	template<class TStringArray>
	static HRESULT String2Array( LPCWSTR _wsString, TStringArray& _rArray, BOOL _bLowerCase = FALSE )
	{
		if( !_wsString || !lstrlenW(_wsString) ) return E_INVALIDARG;

		LPCWSTR pTemp = _wsString;
		while( *pTemp )
		{
			// Get value end
			LPCWSTR pEnd = pTemp;
			while( *pEnd && !IsSeparator( *pEnd ) ) pEnd++;

			CComBSTR cbsValue;
			cbsValue.Attach( ::SysAllocStringLen( pTemp, pEnd - pTemp ) );
			_rArray.Add( cbsValue );

			// Skip separator (,)
			pTemp = pEnd;
			while( *pTemp && IsSeparator( *pTemp ) ) pTemp++;
		}

		return S_OK;
	}

	template<class TStringArray>
	static HRESULT StringSplit( LPCWSTR _wsString, TStringArray& _rArray, WCHAR _pszSeparator )
	{
		if( !_wsString || !lstrlenW(_wsString) ) return E_INVALIDARG;

		LPCWSTR pTemp = _wsString;
		while( *pTemp )
		{
			// Get value end
			LPCWSTR pEnd = pTemp;
			while( *pEnd && *pEnd != _pszSeparator ) pEnd++;

			CComBSTR cbsValue;
			cbsValue.Attach( ::SysAllocStringLen( pTemp, pEnd - pTemp ) );
			_rArray.Add( cbsValue );

			// Skip separator (,)
			pTemp = pEnd;
			while( *pTemp && *pTemp == _pszSeparator ) pTemp++;
		}

		return S_OK;
	}

	template <class T>
	static HRESULT ArrayInt2String(const T* _pArray, int _nSize, BSTR* _pbsRes, LPCWSTR _pszDelimer = L", " )
	{
		if (!_pbsRes) return E_POINTER;

		if (!_pArray || _nSize == 0 ) return S_FALSE;

		CComBSTR cbsRes;
		for (int i = 0; i < _nSize - 1; i++)
		{
			cbsRes.Append(INT2STR(_pArray[i]));
			cbsRes.Append(_pszDelimer);
		}

		cbsRes.Append(INT2STR(_pArray[_nSize - 1]));

		*_pbsRes = cbsRes.Detach();
		return S_OK;
	}

	template <class T> 
	static HRESULT ArrayInt2String( const CSimpleArray2<T>& _rArray, BSTR* _pbsRes )
	{
		return ArrayInt2String( _rArray.m_aT, _rArray.GetSize(), _pbsRes );
	}


	template <class T>
	static CComBSTR ArrayInt2String(const T* _pArray, int _nSize)
	{
		CComBSTR cbsArray;
		ArrayInt2String(_pArray, _nSize, &cbsArray);
		return cbsArray;
	}
	// Note: Split array used for audio tracks split (e.g. MFAVPropsSet() )
	template <class T> 
	static HRESULT String2ArrayInt( LPCWSTR _wsString, CSimpleArray2<T>& _rArray, CSimpleArray2<CComBSTR>* _pArraySplit = NULL, bool _bUniqueIndex = false )
	{
		if( STR_IS_EMPTY( _wsString ) ) return E_INVALIDARG;

		LPCWSTR pTemp = _wsString;
		while( *pTemp )
		{
			while (*pTemp && !IsDigit(*pTemp) || *pTemp == L'.') pTemp++;

			if( !*pTemp )
				break;

			LPWSTR pEnd = NULL;
			long lValue = wcstol( pTemp, &pEnd, 0 );
			pTemp = pEnd;
			if (*pTemp == L'.')
			{
				// Skip fractal part (for do not parse it's like next int)
				while (*pTemp && IsDigit(*pTemp) ) pTemp++;
			}

			while( *pTemp && !IsDigit( *pTemp ) || *pTemp == L'.' ) pTemp++;

			if( !_bUniqueIndex || _rArray.Find(lValue) < 0 )
			{
				_rArray.Add(lValue);

				if( _pArraySplit && pTemp > pEnd )
				{
					CComBSTR cbsDelimer( pTemp - pEnd, pEnd );
					LPWSTR pszDel = cbsDelimer.m_str;
					StrTrimW( pszDel, L" \n\r\t" );
					_pArraySplit->Add( pszDel );
				}
			}
		}

		return S_OK;
	}

	// Note: Split array used for audio tracks split (e.g. MFAVPropsSet() )
	template <class T>
	static HRESULT String2ArrayUInt(LPCWSTR _wsString, CSimpleArray2<T>& _rArray, CSimpleArray2<CComBSTR>* _pArraySplit = NULL, bool _bUniqueIndex = false)
	{
		if (!_wsString || !lstrlenW(_wsString)) return E_INVALIDARG;

		LPCWSTR pTemp = _wsString;
		while (*pTemp)
		{
			while (*pTemp && !IsDigit(*pTemp) || *pTemp == L'.') pTemp++;

			if (!*pTemp)
				break;

			LPWSTR pEnd = NULL;
			ULONG ulValue = wcstoul(pTemp, &pEnd, 0);
			pTemp = pEnd;

			while (*pTemp && !IsDigit(*pTemp) || *pTemp == L'.') pTemp++;

			if (!_bUniqueIndex || _rArray.Find(ulValue) < 0 )
			{
				_rArray.Add(ulValue);

				if (_pArraySplit && pTemp > pEnd)
				{
					CComBSTR cbsDelimer(pTemp - pEnd, pEnd);
					LPWSTR pszDel = cbsDelimer.m_str;
					StrTrimW(pszDel, L" \n\r\t");
					_pArraySplit->Add(pszDel);
				}
			}
		}

		return S_OK;
	}

	 
	static HRESULT String2ArrayColor( LPCWSTR _wsString, CSimpleArray2<COLORREF>& _rArray )
	{
		if( !_wsString || !lstrlenW(_wsString) ) return E_INVALIDARG;

		CSimpleArray2<CComBSTR> arrString;
		String2Array( _wsString, arrString ); 

		_rArray.RemoveAll();
		for( int i = 0; i < arrString.GetSize(); i++ )
		{
			_rArray.Add( Str2Color(arrString[i]) );
		}

		return S_OK;
	}

	template <class T> 
	static HRESULT ArrayDbl2String( const CSimpleArray2<T>& _rArray, BSTR* _pbsRes )
	{
		if( !_pbsRes ) return E_POINTER;

		if( !_rArray.GetSize() ) return E_INVALIDARG;

		CComBSTR cbsRes;
		for( int i = 0; i < _rArray.GetSize() - 1 ; i++ )
		{
			cbsRes.Append( DBL2STR3( _rArray[i]) );
			cbsRes.Append( ", " );
		}

		cbsRes.Append( DBL2STR3( _rArray[_rArray.GetSize() - 1]) );

		*_pbsRes = cbsRes.Detach();
		return S_OK;
	}

	template <class T>
	static CComBSTR ArrayDbl2String(const CSimpleArray2<T>& _rArray)
	{
		CComBSTR cbsArray;
		ArrayDbl2String(_rArray, &cbsArray);
		return cbsArray;
	}

	// Note: Check and make Unit test
	template <class T> 
	static HRESULT String2ArrayDbl( LPCWSTR _wsString, CSimpleArray2<T>& _rArray )
	{
		if( !_wsString || !lstrlenW(_wsString) ) return E_INVALIDARG;

		LPCWSTR pTemp = _wsString;
		while( *pTemp )
		{
			while (*pTemp && !IsDigit(*pTemp)) pTemp++;

			if (*pTemp)
			{
				double dblValue = _wtof(pTemp);
				_rArray.Add((T)dblValue);

				// Skip number
				while (*pTemp && IsDigit(*pTemp)) pTemp++;

				// Skip separator (,)
				while (*pTemp && !IsDigit(*pTemp)) pTemp++;
			}
		}

		return _rArray.GetSize() ? S_OK : S_FALSE;
	}

	static WCHAR String2Colors( LPCWSTR pszColor, CSimpleArray2<COLORREF>& arrColor )
	{
		WCHAR wChRes = 0; 
		LPCWSTR pszNext = pszColor;
		while( *pszNext )
		{
			COLORREF color = Str2Color( pszNext, -1 );
			arrColor.Add( color );
			pszNext += StrCSpnW( pszNext, XML_COLOR_SEPARATOR );
			if( !*pszNext )
				break;

			if( !wChRes)
				wChRes = *pszNext;

			pszNext++;
		}

		return wChRes;
	}
		
	static HRESULT String2Borders( LPCWSTR _wsString, RECT& _rcBorders )
	{
		if( !_wsString || !lstrlenW(_wsString) ) return E_INVALIDARG;

		CSimpleArray2<int> arrIndent;
		String2ArrayInt( _wsString, arrIndent );
		if( arrIndent.GetSize() == 1 )
		{
			_rcBorders.left = _rcBorders.right = _rcBorders.top = _rcBorders.bottom = arrIndent[0];
		}
		else if( arrIndent.GetSize() == 2 )
		{
			_rcBorders.left = _rcBorders.right = arrIndent[0];
			_rcBorders.top = _rcBorders.bottom = arrIndent[1];
		}
		else if( arrIndent.GetSize() == 3 )
		{
			_rcBorders.left = _rcBorders.right = arrIndent[0];
			_rcBorders.top = arrIndent[1];
			_rcBorders.bottom = arrIndent[2];
		}
		else if( arrIndent.GetSize() >= 4 )
		{
			_rcBorders.left = arrIndent[0];
			_rcBorders.top = arrIndent[1];
			_rcBorders.right = arrIndent[2];
			_rcBorders.bottom = arrIndent[3];
		}
		else
			return E_INVALIDARG;

		return S_OK;
	}

	// 	static XMLUnit Str2DblPerc( LPCWSTR pszRes, int nBaseSize = -100 )
	// 	{
	// 		if( !pszRes ) return 0;
	// 
	// 		LPWSTR pEnd = 0;
	// 		XMLUnit unit = { wcstod( pszRes, &pEnd ), 0 };
	// 		if( pEnd && *pEnd == L'%' )
	// 			nRes = nBaseSize != 0 ? DBL2INT( nRes * nBaseSize / 100 ) : nRes; // For percent
	// 
	// 		return nRes;
	// 	}

	static HRESULT String2Borders( LPCWSTR _wsString, RECT_DBL& _rcBorders, SIZE_DBL* pSizeBase  )
	{
		if( !_wsString || !lstrlenW(_wsString) ) return E_INVALIDARG;

		CSimpleArray2<CComBSTR> arrIndent;
		String2Array( _wsString, arrIndent );

		if( arrIndent.GetSize() == 1 )
		{
			_rcBorders.left = _rcBorders.right = Unit2Value( Str2Unit( arrIndent[0] ), pSizeBase ? pSizeBase->cx : -100 );
			_rcBorders.top = _rcBorders.bottom = Unit2Value( Str2Unit( arrIndent[0] ), pSizeBase ? pSizeBase->cy : -100 );
		}
		else if( arrIndent.GetSize() == 2 )
		{
			_rcBorders.left = _rcBorders.right = Unit2Value( Str2Unit( arrIndent[0] ), pSizeBase ? pSizeBase->cx : -100 );
			_rcBorders.top = _rcBorders.bottom = Unit2Value( Str2Unit( arrIndent[1] ), pSizeBase ? pSizeBase->cy : -100 );
		}
		else if( arrIndent.GetSize() == 3 )
		{
			_rcBorders.left = _rcBorders.right = Unit2Value( Str2Unit( arrIndent[0] ), pSizeBase ? pSizeBase->cx : -100 );
			_rcBorders.top = Unit2Value( Str2Unit( arrIndent[1] ), pSizeBase ? pSizeBase->cy : -100 );
			_rcBorders.bottom = Unit2Value( Str2Unit( arrIndent[2] ), pSizeBase ? pSizeBase->cy : -100 );
		}
		else if( arrIndent.GetSize() >= 4 )
		{
			_rcBorders.left = Unit2Value( Str2Unit( arrIndent[0] ), pSizeBase ? pSizeBase->cx : -100 );
			_rcBorders.top = Unit2Value( Str2Unit( arrIndent[1] ), pSizeBase ? pSizeBase->cy : -100 );
			_rcBorders.right = Unit2Value( Str2Unit( arrIndent[2] ), pSizeBase ? pSizeBase->cx : -100 );
			_rcBorders.bottom = Unit2Value( Str2Unit( arrIndent[3] ), pSizeBase ? pSizeBase->cy : -100 );
		}
		else
			return E_INVALIDARG;

		return S_OK;
	}

	
	// {100,100,20,40} {x,y,w,h}
	// Flip flags: 1 flip horz, 2 flip vert
	static RECT String2Rect( LPCWSTR _wsString, int* _pnFlipFlags = NULL )
	{
		RECT rcDest = {};

		if( !_wsString || !lstrlenW(_wsString) ) 
			return rcDest;

		CSimpleArray2<int> arrRect;
		String2ArrayInt( _wsString, arrRect );

		if( arrRect.GetSize() < 4 )
			return rcDest;

		rcDest.left = arrRect[0];
		rcDest.top = arrRect[1];
		rcDest.right = rcDest.left + ABS(arrRect[2]);
		rcDest.bottom = rcDest.bottom + ABS(arrRect[3]);

		if( _pnFlipFlags )
			*_pnFlipFlags = (arrRect[2] < 0 ? 1 : 0) | (arrRect[2] < 0 ? 2 : 0);

		return rcDest;
	}
	

	static LPCWSTR Borders2String( RECT_DBL& _rcBorders, CComBSTR& cbsRes, SIZE_DBL* pSizeBase )
	{
		SIZE_DBL szBase = {0};
		if( pSizeBase )
			szBase = *pSizeBase;

		int nStore = 4;
		if( _rcBorders.left == _rcBorders.right )
		{
			if( _rcBorders.top == _rcBorders.bottom )
			{
				double dblH = _rcBorders.left  / ((pSizeBase && pSizeBase->cx) ? pSizeBase->cx : 1.0);
				double dblV = _rcBorders.left  / ((pSizeBase && pSizeBase->cx) ? pSizeBase->cx : 1.0);
				if( ABS( dblH - dblV ) < 0.00001  )
					nStore = 1;
			}
			else
			{
				nStore = 3;
			}
		}

		WCHAR szBuffer[32]={0};
		cbsRes = Unit2Str( MakePercent( _rcBorders.left, szBase.cx ), szBuffer );
		if( nStore == 1 )
		{
			// Do nothing
		}
		else if( nStore == 2 )
		{
			cbsRes.Append( L","); 
			cbsRes.Append( Unit2Str( MakePercent( _rcBorders.right, szBase.cx ), szBuffer ) );
		}
		else if( nStore == 3 )
		{
			cbsRes.Append( L","); 
			cbsRes.Append( Unit2Str( MakePercent(  _rcBorders.right, szBase.cx ), szBuffer ) );
			cbsRes.Append( L","); 
			cbsRes.Append( Unit2Str( MakePercent(  _rcBorders.top, szBase.cy ), szBuffer ) );
		}
		else // if( nStore == 4 )
		{
			cbsRes.Append( L","); 
			cbsRes.Append( Unit2Str( MakePercent(  _rcBorders.right, szBase.cx ), szBuffer ) );
			cbsRes.Append( L","); 
			cbsRes.Append( Unit2Str( MakePercent(  _rcBorders.top, szBase.cy ), szBuffer ) );
			cbsRes.Append( L","); 
			cbsRes.Append( Unit2Str( MakePercent( _rcBorders.bottom, szBase.cy ), szBuffer ) );
		}

		return cbsRes.m_str;
	}

	

	static inline int XMLParseName( LPCWSTR _pszText, vector<XMLNamePart>& _vecParts )
	{
		ATLASSERT( _pszText );

		bool bFind = false;
		XMLNamePart xmlPart = {_pszText, 0, 0 }; 
		LPCWSTR pNext = _pszText;
		while( pNext = XMLSeparator( pNext, &xmlPart.nLen, &xmlPart.nOrder, &xmlPart.pszID, &xmlPart.nIDLen ) )
		{
			_vecParts.push_back( xmlPart );
			pNext += 2;
			xmlPart.pszPart = pNext;
		}

		_vecParts.push_back( xmlPart );

		return (int)_vecParts.size();
	}

	// _nStart < 0 -> from end
	// _nLen < 0 -> end pos from len
	static inline CComBSTR XMLNameGetPart( LPCWSTR _pszText, int _nStart, int _nEnd )
	{
		ATLASSERT( _pszText );

		vector<XMLNamePart> arrParts;
		int nLen = XMLParseName( _pszText, arrParts );
		if( nLen <= 0 )
			return NULL;

		if( _nStart < 0 )
			_nStart = nLen + _nStart;

		_nStart = MAX( 0, _nStart );

		if( _nEnd <= 0 )
			_nEnd = nLen + _nEnd;

		_nEnd = MAX( 1, _nEnd );
		

		LPCWSTR pszStart = _nStart < nLen ? arrParts[_nStart].pszPart : NULL;
		LPCWSTR pszEnd = _nEnd <= nLen ? arrParts[_nEnd - 1].pszPart : NULL;

		CComBSTR cbsRes( pszEnd > pszStart + 2 ? (pszEnd - 2) - pszStart: lstrlenW(pszStart), pszStart );
		return cbsRes;
	}

	// From node-1:node-2:arr -> node-1::node-2::arr
	static inline LPCWSTR XMLNameFromRef( LPCWSTR _pszNameRef, LPWSTR _pszName )
	{
		ATLASSERT( _pszName && _pszNameRef );

		LPWSTR pszName = _pszName;
		while( *_pszNameRef )
		{
			if( *_pszNameRef == L':' )
				*pszName++ = L':';

			*pszName++ = *_pszNameRef++;
		}

		*pszName = 0;

		return _pszName;
	}

	static inline LPCWSTR XMLNameToRef( LPCWSTR _pszName, LPWSTR _pszNameRef )
	{
		ATLASSERT( _pszName && _pszNameRef );

		LPWSTR pszNameRef = _pszNameRef;
		while( *_pszName )
		{
			if( *_pszName == L':' && *(_pszName + 1) == L':')
			{
				*pszNameRef++ = L':';
				_pszName += 2;
			}
			else
			{
				*pszNameRef++ = *_pszName++;
			}
		}

		*pszNameRef = 0;

		return _pszNameRef;
	}

	static inline LPWSTR ReplaceChars(LPWSTR _pszString, LPCWSTR _pszFind, WCHAR _wchReplace, bool _bTrimEnd = false)
	{
		int nFindLen = lstrlenW(_pszFind);
		int nStrLen = lstrlenW(_pszString);
		for( int i = 0; i < nStrLen; i++ )
		{
			for( int j = 0; j < nFindLen; j++ )
			{
				if( _pszString[i] == _pszFind[j] ) 
				{
					_pszString[i] = _wchReplace;

					if( _bTrimEnd && _pszString[i+1] == 0 )
					{
						LPWSTR pTmp = _pszString + i;
						while( *pTmp == _wchReplace && pTmp > _pszString )
						{
							*pTmp-- = 0;
						}
					}

				}
			}
		}

		return _pszString;
	}

public:
	

	static inline LPCWSTR XMLSeparator( LPCWSTR _pszText, int* _pnLen = NULL, int* _pnOrder = NULL, LPCWSTR* _ppszID = NULL, int* _pnIDLen = NULL )
	{
		if( _pnOrder )
			*_pnOrder = 0;

		ATLASSERT( _pszText );
		LPCWSTR pText = _pszText;

		if (_pnLen)
			*_pnLen = 0;

		if (_pnOrder)
			*_pnOrder = 0;

		if (_ppszID)
			*_ppszID = NULL;

		if (_pnIDLen)
			*_pnIDLen = 0;

		while( *pText )
		{
// VVB 2016/02/19:		Commented as seems to work improper:
//	- e.g. option info for 'Device Name (3)'
//	- later searched Device Name (3) Node (what is wrong)

			// 2017/04/28 -> Add index support (e.g. for stat)
			if (*pText == L'[')
			{
				bool bNumbersInside = true; // The node_name[idx] 
				LPCWSTR pEnd = pText + 1;
				while (*pEnd && *pEnd != L':' && *pEnd != L']' )
				{
					// Max index is '99999' - 5 digits
					if (*pEnd < L'0' || *pEnd > L'9' || pEnd - pText > 6 )
						bNumbersInside = false; // node_name[id] 

					pEnd++;
				}

				if (_pnLen)
					*_pnLen = pText - _pszText;

				if (_pnOrder)
					*_pnOrder = bNumbersInside ? _wcstoi64(pText + 1, NULL, 0) : 0;

				if (_ppszID)
					*_ppszID = bNumbersInside ? NULL : pText + 1;

				if (_pnIDLen)
					*_pnIDLen = bNumbersInside ? NULL : pEnd - pText - 1;

				pText = pEnd;

				while (*pText && *pText != L':')
					pText++;
				
				return *pText ? pText : NULL;
			}

			if( *pText == L':' && *(pText + 1) == L':' )
			{
				if( _pnLen )
					*_pnLen = pText - _pszText;

				return pText;
			}

			pText++;
		}

		if( _pnLen )
			*_pnLen = pText - _pszText;

		return NULL;
	}

	

	static inline LPCWSTR WordEnd( LPCWSTR _pszText ) 
	{
		while( *_pszText && IsValidChar( *_pszText ) ) _pszText++;

		return _pszText;	
	}

	static inline LPCWSTR SkipBlank( LPCWSTR _pszText, bool _bValue  ) 
	{
		if( _bValue )
		{
			while( *_pszText && IsBlankVal( *_pszText ) ) _pszText++;
		}
		else
		{
			while( *_pszText && IsBlank( *_pszText ) ) _pszText++;	
		}

		return _pszText;	
	}

	static inline LPCWSTR SkipBlankRev( LPCWSTR _pszText, bool _bValue  ) 
	{
		if( _bValue )
		{
			while( *_pszText && IsBlankVal( *_pszText ) ) _pszText--;
		}
		else
		{
			while( *_pszText && IsBlank( *_pszText ) ) _pszText--;	
		}

		return _pszText;	
	}

// 	static bool IsPrefix( LPCWSTR _pszString, LPCWSTR _pszPrefix, LPCWSTR* _ppRestString )
// 	{
// 		ATLASSERT( _pszString && _pszPrefix );
// 		if( !_pszPrefix || !_pszString ) return false;
// 
// 		int nLen = lstrlenW( _pszPrefix );
// 		if( StrCmpNIW( _pszString, _pszPrefix, nLen ) == 0 )
// 		{
// 			if( _ppRestString )
// 				*_ppRestString = _pszString + nLen;
// 			return true;
// 		}
// 
// 		return false;
// 	}
// 
// 	static int IsPrefix( LPCWSTR _pszString, LPCWSTR* _pszPrefixes, LPCWSTR* _ppRestString )
// 	{
// 		if( !_pszString || !_pszPrefixes ) 
// 			return 0;
// 
// 		for( int i = 0; _pszPrefixes[i] != NULL; i++ )
// 		{
// 			if( IsPrefix( _pszString, _pszPrefixes[i], _ppRestString ) )
// 				return i + 1;
// 		}
// 
// 		return 0;
// 	}

	// Note: Assume empty string is NOT number
	static inline bool IsNumber( LPCWSTR _pszString )
	{
		if( !_pszString || !_pszString[0] )
			return false;

		while( _pszString && *_pszString )
		{
			if( !IsDigit( *_pszString ) )
				return false;

			_pszString++;
		}

		return true;
	}

	static bool IsFile( LPCWSTR _pszString )
	{
		if( !_pszString )
			return false;

		LPCWSTR pszDeniedFileChar = L"?*\"<>|";
		int nLen = lstrlenW( _pszString);
		if( nLen > MAX_PATH * 10 || StrCSpnW( _pszString, pszDeniedFileChar ) < nLen )
			return false;

		if( IsFullPathW( _pszString) )
			return true;

		LPCWSTR pExt = FindFileExtensionW(_pszString);
		if( !pExt ) return false;

		if( lstrlenW( pExt ) == 3 )
			return true;

		return false;
	}

	static inline bool IsBasicVal( LPCWSTR _pszVal, LPCWSTR _pszCheck = L" \n\r\t`~!@#$%^&*()_-+={}[]\\|;:'\",.<>/?!*" )
	{
		if( StrCSpnW( _pszVal, _pszCheck ) != lstrlenW( _pszVal ) ) 
			return false;

		return true;
	}

	static inline bool IsBlank( WCHAR _ch ) 
	{
		if( _ch == L' ' || _ch == L'\n' || _ch == L'\r' || _ch == L'\t' || (_ch >= 0xFDD0 && _ch >= 0xFDEF) || _ch == 0xFFEF || _ch == 0xFFEE  ) return true;

		return false;
	}

	static inline bool IsSeparator( WCHAR _ch ) 
	{
		if( _ch == L' ' || _ch == L'\n' || _ch == L'\r' || _ch == L'\t' || _ch == L';' || _ch == L',' ) return true;

		return false;
	}

	static inline bool IsValidChar( WCHAR _ch ) 
	{
		if( _ch != 0 && _ch != L' ' && _ch != L'\n' && _ch != L'\r' && _ch != L'\t' && _ch != L';' && _ch != L',' &&
			_ch != L'<' && _ch != L'>' && _ch != L'/' && _ch != L'\\' && _ch != L'=' &&
			!((_ch >= 0xFDD0 && _ch >= 0xFDEF) || _ch == 0xFFEF || _ch == 0xFFEE) ) return true;

		return false;
	}

	static inline bool IsBlankVal( WCHAR _ch ) 
	{
		if( _ch == L'\n' || _ch == L'\r' || _ch == L'\t' || _ch == L'\t' || (_ch >= 0xFDD0 && _ch >= 0xFDEF) || _ch == 0xFFEF || _ch == 0xFFEE ) return true;

		return false;
	}

	static inline bool IsDigit( WCHAR _ch ) 
	{
		if( (_ch < L'0' || _ch > L'9') && _ch != L'.' && _ch != L'-') return false;

		return true;
	}

	static bool IsBlank( LPCWSTR _szValue )
	{
		if( !_szValue ) return true;

		for( int i = 0; i < lstrlenW( _szValue ); i++ )
		{
			if( _szValue[i] != L' ' ) return false;
		}

		return true;
	}

	static bool IsBlank( LPCSTR _szValue )
	{
		if( !_szValue ) return true;

		for( int i = 0; i < lstrlenA( _szValue ); i++ )
		{
			if( _szValue[i] != ' ' ) return false;
		}

		return true;
	}

	static bool IsColor( LPCWSTR _szValue )
	{
		return Str2Color( _szValue, -1) == Str2Color(_szValue, 0 );
	}

	static bool IsInteger( LPCWSTR _szValue )
	{
		if( STR_IS_EMPTY(_szValue) ) return false;

		for( int i = 0; i < lstrlenW( _szValue ); i++ )
		{
			if( _szValue[i] < L'0' || _szValue[i] > L'9' ) 
				return false;
		}

		return true;
	}

	static bool IsHex( LPCWSTR _szValue )
	{
		if( STR_IS_EMPTY(_szValue) || _szValue[0] != L'0' || _szValue[1] != L'x' )
			return false;
		
		for (int i = 2; i < lstrlenW(_szValue); i++)
		{
			if ((_szValue[i] < L'0' || _szValue[i] > L'9') &&
				(_szValue[i] < L'A' || _szValue[i] > L'F') &&
				(_szValue[i] < L'a' || _szValue[i] > L'f') ) 
			{
				return false;
			}
		}

		return true;
	}

	static bool IsFloat( LPCWSTR _szValue )
	{
		if( STR_IS_EMPTY(_szValue) ) 
			return false;

		for( int i = 0; i < lstrlenW( _szValue ); i++ )
		{
			if( (_szValue[i] < L'0' || _szValue[i] > L'9') && _szValue[i] != L'.' ) 
				return false;
		}

		return true;
	}

	inline static LPCWSTR GetEndQuotes(LPCWSTR _lpFirst, LPCWSTR _lpPos)
	{
		if( !_lpFirst || !_lpPos ) return NULL;

		const WCHAR* pStr = _lpFirst;
		while( pStr < _lpPos)
		{
			pStr += StrCSpnW( pStr, L"\"'" );
			//   ... pos  "(... -> false
			//   ... "( ... pos    -> unknown
			if( !pStr || pStr >= _lpPos ) return NULL;  

			pStr = StrChrW( pStr + 1, pStr[0] ); 
			//   ..."(  pos  )"  -> true
			//   ..."( ... )" ... pos  -> unknown
			if( pStr > _lpPos ) return pStr + 1;

			// The quotes opened but not closed 
			if( !pStr ) return _lpPos;

			pStr++;
		}

		// This case only if _lpSrch start with quotes
		ATLASSERT( pStr == _lpPos || _lpPos[0] == L'"' || _lpPos[0] == L'\'' );

		return NULL;
	}

	// Search the sequence except the quoted 
	inline static LPCWSTR StrStrIW_Quotes(LPCWSTR _lpFirst, LPCWSTR _lpSrch)
	{
		if( !_lpFirst ) return NULL;
		if( !_lpSrch ) return NULL; // ??

		const WCHAR* pRes = NULL;
		const WCHAR* pStr = _lpFirst;
		while( pStr )
		{
			pRes = StrStrIW( pStr, _lpSrch );
			pStr = GetEndQuotes( pStr, pRes );
			if( pStr == pRes ) return NULL; // The quotes opened but not closed or pRes == NULL (not found)
		}

		return pRes;
	}

	// Search the char except the quoted 
// 	inline static LPCWSTR StrChrW_Quotes(LPCWSTR _lpFirst, WCHAR _wMatch)
// 	{
// 		if( !_lpFirst ) return NULL;
// 
// 		const WCHAR* pRes = NULL;
// 		const WCHAR* pStr = _lpFirst;
// 		while( pStr )
// 		{
// 			pRes = StrChrW( pStr, _wMatch );
// 			pStr = GetEndQuotes( pStr, pRes );
// 			if( pStr == pRes ) return NULL; // The quotes opened but not closed or pRes == NULL (not found)
// 		}
// 
// 		return pRes;
// 	}

	// Some text "xx'x"x"
	inline static LPCWSTR StrChrW_Quotes(LPCWSTR _lpFirst, WCHAR _wMatch)
	{
		if( !_lpFirst ) return NULL;

		WCHAR wQuotes = 0;
		const WCHAR* pStr = _lpFirst;
		while( *pStr )
		{
			if( wQuotes == 0 )
			{
				if( *pStr == _wMatch )
					return pStr;

				if( *pStr == L'\'' || *pStr == L'"' )
					wQuotes = *pStr;
			}
			else if( *pStr == wQuotes )
			{
				// Close quotes
				wQuotes = 0;
			}

			pStr++;
		}

		return NULL;
	}

	static inline LPCWSTR Tag2XML( LPWSTR _pszName )
	{
		if( !_pszName ) return NULL;

		WCHAR* pStr = _pszName;
		WCHAR* pRes = _pszName;
		while( *pStr )
		{
			if( pStr[0] == L':' && pStr[1] == L':')
			{
				*pRes++ =  L'_';
				*pRes++ =  L'_';
				pStr += 2;
			}
			else if( IsBlank( *pStr ) )
			{
				*pRes++ =  L'_';
				pStr += 1;
			}
			else if( pStr[0] != L'<' && pStr[0] != L'>')
			{
				*pRes++ = *pStr++;
			}
		}

		*pRes = L'\0';

		return _pszName;
	}

	static inline BSTR Value2XML( CComBSTR& _cbsValue, int _nConvertQuotes = 0 )
	{
		if( !_cbsValue.m_str ) return NULL;

		// Calc max size
		int nInputChars = lstrlenW( _cbsValue.m_str );
		if( _nConvertQuotes < 0 )
		{
			// Convert only if " and ' present in string
			if( StrCSpnIW( _cbsValue.m_str, L"\"'" ) != nInputChars )
				_nConvertQuotes = 1; // Convert
			else
				_nConvertQuotes = 0; // Not convert

		}
		// Max - 6 times longer ( " -> &quot; ) or 4 times longer (if not convert)

		CBlobNew blobTemp;
		HRESULT hr = blobTemp.AllocMem( (nInputChars * (_nConvertQuotes ? 6 : 5) ) * sizeof(WCHAR) + 16, true );
		if( FAILED( hr ) ) return NULL;

		WCHAR* pStr =  _cbsValue.m_str;
		WCHAR* pTemp = (WCHAR*)blobTemp.Ptr();

		int nChar = 0;
		for( int i = 0; i < nInputChars; i++ )
		{
			if( pStr[i] == L'<' )
			{
				// &lt;
				pTemp[nChar++] = L'&'; 
				pTemp[nChar++] = L'l'; 
				pTemp[nChar++] = L't'; 
				pTemp[nChar++] = L';'; 
			}
			else if( pStr[i] == L'>' )
			{
				// &gt;
				pTemp[nChar++] = L'&'; 
				pTemp[nChar++] = L'g'; 
				pTemp[nChar++] = L't'; 
				pTemp[nChar++] = L';'; 
			}
			else if( pStr[i] == L'&' )
			{
				// &amp;
				pTemp[nChar++] = L'&'; 
				pTemp[nChar++] = L'a'; 
				pTemp[nChar++] = L'm'; 
				pTemp[nChar++] = L'p'; 
				pTemp[nChar++] = L';'; 
			}
			else if( _nConvertQuotes && pStr[i] == L'\'' )
			{
				// &apos;
				pTemp[nChar++] = L'&'; 
				pTemp[nChar++] = L'a'; 
				pTemp[nChar++] = L'p'; 
				pTemp[nChar++] = L'o'; 
				pTemp[nChar++] = L's'; 
				pTemp[nChar++] = L';'; 
			}
			else if( _nConvertQuotes && pStr[i] == L'\"' )
			{
				// &quot;
				pTemp[nChar++] = L'&'; 
				pTemp[nChar++] = L'q'; 
				pTemp[nChar++] = L'u'; 
				pTemp[nChar++] = L'o'; 
				pTemp[nChar++] = L't'; 
				pTemp[nChar++] = L';'; 
			}
			else
			{
				pTemp[nChar++] = pStr[i]; 
			}
		}

		pTemp[nChar] = L'\0';
		_cbsValue = pTemp;
		return _cbsValue;
	}

	static inline LPCWSTR XML2Value( LPWSTR _szValue, bool _bNoEmptyCheck = false, bool _bKeepSpecialChars = false )
	{
		if( !_szValue ) return NULL;

		if( !_bKeepSpecialChars )
			ReplaceChars( _szValue, L"\r\n\t", L' ' );

		int nInputLen = lstrlenW( _szValue );

		WCHAR* pStr =  _szValue;
		WCHAR* pRes = _szValue;

		bool bNBSP = false;
		while( pStr < _szValue + nInputLen )
		{
			WCHAR* pEsc = StrChrW( pStr, L'&' );
			if( !pEsc )
			{
				memmove( pRes, pStr, lstrlenW( pStr ) * sizeof(WCHAR) );
				pRes += lstrlenW( pStr );
				pStr += lstrlenW( pStr );
			}
			else
			{
				if( pEsc != pStr )
				{
					memmove( pRes, pStr, (pEsc - pStr) * sizeof(WCHAR) );
					pRes += (pEsc - pStr);
				}

				if( memcmp( pEsc, L"&lt;", 4 ) == 0 )
				{
					*pRes++ = L'<';
					pStr = pEsc + 4;
				}
				else if( memcmp( pEsc, L"&gt;", 4 ) == 0 )
				{
					*pRes++ = L'>';
					pStr = pEsc + 4;
				}
				else if( memcmp( pEsc, L"&amp;", 5 ) == 0 )
				{
					*pRes++ = L'&';
					pStr = pEsc + 5;
				}
				else if( memcmp( pEsc, L"&apos;", 6 ) == 0 )
				{
					*pRes++ = L'\'';
					pStr = pEsc + 6;
				}
				else if( memcmp( pEsc, L"&nbsp;", 6 ) == 0 )
				{
					*pRes++ = L' ';
					pStr = pEsc + 6;
					bNBSP = true;
				}
				else if( memcmp( pEsc, L"&copy;", 6 ) == 0 )
				{
					*pRes++ = L'©'; // TODO: codes instead of typed symbols
					pStr = pEsc + 6;
				}
				else if( memcmp( pEsc, L"&reg;", 5 ) == 0 )
				{
					*pRes++ = L'®'; // TODO: codes instead of typed symbols
					pStr = pEsc + 5;
				}
				else if( memcmp( pEsc, L"&quot;", 6 ) == 0 )
				{
					*pRes++ = L'"';
					pStr = pEsc + 6;
				}
				else if( pEsc[1] == L'#' )
				{
					if( pEsc[2] == L'x' ) 
					{
						// &#x4e2d; -> &0x4e2d;
						pEsc[1] = L'0'; // for wcstoul
						pEsc++;
					}
					else
					{
						// &#20013;
						pEsc += 2;
					}

					LPWSTR pEnd = 0;
					*pRes++ = (WORD)wcstoul( pEsc, &pEnd, 0 );
					pStr = pEnd + 1;
				}
				else
				{
					// Skip only &
					pStr = pEsc + 1;
				}
			}
		}
		pRes[0] = L'\0';

		// Check for empty string
		if( _bNoEmptyCheck || bNBSP)
			return _szValue;

		bool bEmpty = true;
		for( int i = 0; i < lstrlenW(_szValue); i++ )
		{
			if( _szValue[i] != L' ' ) 
			{
				bEmpty = false;
				break;
			}
		}

		if( bEmpty ) _szValue[0] = L'\0';

		return bEmpty ? NULL : _szValue;
	}


	static inline LPCWSTR PrintTab( int _nTab, LPWSTR _szStr )
	{
		for( int i = 0; i < MIN( _nTab, MAX_TABS - 1); i++ )
		{
			_szStr[i] = L'\t';
		}
		_szStr[_nTab] = 0;
		return _szStr;
	}


	static LPCWSTR SplitLines( LPCWSTR pszText, CComBSTR& cbsText, LPCWSTR pszLineTag = L"text" )
	{
		ATLASSERT( pszLineTag && pszText );
		if( !pszText || !pszLineTag ) return NULL;


		// \r\n
		int nLen = 0;
		deque<XMLNamePart> deqLines;
		LPCWSTR psz = pszText;
		XMLNamePart xmlName ={ pszText, 0, 0};
		while( *psz )
		{
			if( *psz == L'\n' || *psz == L'\r' && *(psz+1) == L'\n' )
			{
				deqLines.push_back( xmlName ); 

				if( *psz == L'\r' )
					psz += 2;
				else
					psz++;

				xmlName.pszPart = psz;
				xmlName.nLen = 0;
			}
			else
			{
				psz++;
				nLen++;
				xmlName.nLen++;
			}
		}

		// <text> </text>
		int nTagLen = lstrlenW(pszLineTag);
		LPWSTR pszOpenTag = (LPWSTR)alloca( (nTagLen + 3 )* sizeof(WCHAR) );
		wsprintfW( pszOpenTag, L"<%ws>", pszLineTag );
		LPWSTR pszCloseTag = (LPWSTR)alloca( (nTagLen + 4 )* sizeof(WCHAR) );
		wsprintfW( pszCloseTag, L"</%ws>", pszLineTag );

		int nResLen = nLen + (int)deqLines.size() * (nTagLen * 2 + 5 )  + 1;
		BSTR bsRes = ::SysAllocStringLen(NULL, nResLen);

		LPWSTR pszRes = bsRes;
		std::deque<XMLNamePart>::iterator Iter = deqLines.begin();
		while( Iter != deqLines.end() )
		{
			pszRes += wsprintfW( pszRes, L"%ws", pszOpenTag );
			::CopyMemory( pszRes, Iter->pszPart, Iter->nLen * sizeof(WCHAR) );
			pszRes += Iter->nLen;
			pszRes += wsprintfW( pszRes, L"%ws", pszCloseTag );

			Iter++;
		}
		*pszRes = 0;

		cbsText.Attach( bsRes );
		return cbsText.m_str;
	}

	static BSTR Text2BR( CComBSTR& _cbsValue )
	{
		if( !_cbsValue.m_str ) return NULL;

		// Calc max size
		int nInputChars = lstrlenW( _cbsValue.m_str );
		// Max - 6 times longer ( \t -> <tab/> )

		CBlobNew blobTemp;
		HRESULT hr = blobTemp.AllocMem( nInputChars * 6 * sizeof(WCHAR) + 16, true );
		if( FAILED( hr ) ) return NULL;

		WCHAR* pStr =  _cbsValue.m_str;
		WCHAR* pTemp = (WCHAR*)blobTemp.Ptr();
		
		_cbsValue = Text2BR( pStr, pTemp );

		return _cbsValue;
	}

	static LPCWSTR Text2BR( LPCWSTR pszSource, LPWSTR pszDest )
	{
		LPCWSTR pStr =  pszSource;
		WCHAR* pTemp = (WCHAR*)pszDest;

		int nChar = 0;
		while( *pStr )
		{
			if( *pStr == L'\t' )
			{
				// <tab/>
				pTemp[nChar++] = L'<'; 
				pTemp[nChar++] = L't'; 
				pTemp[nChar++] = L'a'; 
				pTemp[nChar++] = L'b'; 
				pTemp[nChar++] = L'/'; 
				pTemp[nChar++] = L'>'; 
			}
			else if( *pStr == L'\n' )
			{
				// <br/>
				pTemp[nChar++] = L'<'; 
				pTemp[nChar++] = L'b'; 
				pTemp[nChar++] = L'r'; 
				pTemp[nChar++] = L'/'; 
				pTemp[nChar++] = L'>'; 
			}
			else if( *pStr != L'\r' )
			{
				pTemp[nChar++] = *pStr; 
			}

			pStr++;
		}

		pTemp[nChar] = L'\0';
		
		return pTemp;
	}

	static BSTR TextClearBR( CComBSTR& _cbsValue, WCHAR wchNL = L' ' )
	{
		if( !_cbsValue.m_str ) return NULL;

		// Calc max size
		int nInputChars = lstrlenW( _cbsValue.m_str );
		// Max - 6 times longer ( \t -> <tab/> )

		CBlobNew blobTemp;
		HRESULT hr = blobTemp.AllocMem( nInputChars * sizeof(WCHAR) + 16, true );
		if( FAILED( hr ) ) return NULL;

		WCHAR* pStr =  _cbsValue.m_str;
		WCHAR* pTemp = (WCHAR*)blobTemp.Ptr();

		int nChar = 0;
		for( int i = 0; i < nInputChars; i++ )
		{
			if( pStr[i] == L'\t' )
			{
				// <tab/>
				if( wchNL > 0 )
					pTemp[nChar++] = L' '; 
			}
			else if( pStr[i] == L'\n' )
			{
				// <br/>
				if( wchNL > 0 )
					pTemp[nChar++] = L' '; 
			}
			else if( pStr[i] != L'\r' )
			{
				pTemp[nChar++] = pStr[i]; 
			}
		}

		pTemp[nChar] = L'\0';
		_cbsValue = pTemp;

		return _cbsValue;
	}

	static inline LPCWSTR BR2Text( LPWSTR _szValue, bool _bReplaceTabs = true )
	{
		if( !_szValue ) return NULL;

		int nInputLen = lstrlenW( _szValue );

		WCHAR* pStr =  _szValue;
		WCHAR* pRes = _szValue;

		while( pStr && pStr < _szValue + nInputLen )
		{
			WCHAR* pBR = (WCHAR*)StrStrIW_Quotes( pStr, L"<br" );
			WCHAR* pTab = _bReplaceTabs ? (WCHAR*)StrStrIW_Quotes( pStr, L"<tab" ) : NULL;

			WCHAR* pFirst = pBR && pTab ? MIN( pBR, pTab ) : pBR ? pBR : pTab ? pTab : NULL;
			if( !pFirst )
			{
				memmove( pRes, pStr, lstrlenW( pStr ) * sizeof(WCHAR) );
				pRes += lstrlenW( pStr );
				pStr += lstrlenW( pStr );
			}
			else
			{
				WCHAR* pEnd = StrChrW( pFirst, L'>' );
				if( pEnd ) pEnd++;

				if( pFirst != pStr )
				{
					memmove( pRes, pStr, (pFirst - pStr) * sizeof(WCHAR) );
					pRes += (pFirst - pStr);
				}

				if( pBR )
				{
					*pRes++ = L'\r';
					*pRes++ = L'\n';
					pStr = pEnd;
				}
				if( pTab )
				{
					*pRes++ = L'\t';
					pStr = pEnd;
				}
			}
		}
		pRes[0] = L'\0';

		return _szValue;
	}


	static BSTR RemoveComments( LPCWSTR pszText, BSTR* _pbsText = NULL )
	{
		CComBSTR cbsRes;
		// Remove comment
		LPCWSTR pszEnd = pszText + lstrlenW(pszText);
		while( pszText < pszEnd )
		{
			LPCWSTR pszComment = StrStrIW( pszText, L"<!--" );
			if( pszComment )
			{
				CComBSTR cbsPart; 
				cbsPart.Attach( ::SysAllocStringLen( pszText, pszComment - pszText ) );
				cbsRes.Append( cbsPart.m_str );

				LPCWSTR pszCommentEnd = StrStrIW( pszComment + 4, L"-->" );
				if( pszCommentEnd ) 
					pszText = pszCommentEnd + 3;
				else
					break;
			}
			else
			{
				cbsRes.Append( pszText );
				break;
			}
		}

		if( _pbsText ) *_pbsText = cbsRes.m_str;

		return cbsRes.Detach();
	}

	static LPWSTR RemoveComments( LPWSTR pszText )
	{
		return RemoveTagContent( pszText, L"<!--", L"-->" );
	}

	static LPWSTR PrepareForParse( LPWSTR pszText )
	{
		return pszText;//RemoveTagContent( pszText, L"<!--", L"-->" );
	}

	static LPWSTR RemoveTagContent( LPWSTR pszText, LPCWSTR pszStartTag, LPCWSTR pszEndTag )
	{
		// Remove comment
		LPWSTR pszEnd = pszText + lstrlenW(pszText);
		while( pszText < pszEnd )
		{
			LPWSTR pszComment = StrStrIW( pszText, pszStartTag );
			if( pszComment )
			{
				LPWSTR pszCommentEnd = StrStrIW( pszComment + 4, pszEndTag );
				if( pszCommentEnd ) 
				{
					pszCommentEnd += 3;

					int nCopy = pszEnd - pszCommentEnd; 
					int nSkip = pszCommentEnd - pszComment;
					memmove( pszComment, pszCommentEnd, nCopy * sizeof(WCHAR) );
					pszEnd -= nSkip;
					pszEnd[ 0 ] = 0;

					pszText = pszComment;
				}
				else
				{
					pszComment[0] = 0;
					break;
				}
			}
			else
			{
				break;
			}
		}

		return pszText;
	}

	inline static LPCWSTR NextWord( LPCWSTR pszStart, LPCWSTR* ppszEnd, CComBSTR& _cbsWord, bool _bSearchTags )
	{
		_cbsWord.Empty();

		pszStart = SkipBlank( pszStart, false );
		LPCWSTR pszWordEnd = WordEnd( pszStart );
		// Check for <![CDATA[Some text]]>
		if( _bSearchTags && pszStart + 2 <= pszWordEnd )
		{
			if( pszStart[0] == L'!' && pszStart[1] == L'[')
			{
				pszWordEnd++;

				while( pszWordEnd > pszStart + 1 && *(pszWordEnd - 1)!= L'[' )
					pszWordEnd--;
			}
		}
		if( ppszEnd )
		{

			pszWordEnd = MIN( pszWordEnd, *ppszEnd );
			*ppszEnd = pszWordEnd;
		}

		if( pszWordEnd <= pszStart )
			return NULL;

		_cbsWord.Attach( ::SysAllocStringLen( pszStart, pszWordEnd - pszStart ) );
		return pszStart;
	}

	inline static LPCWSTR StrXMLOpen( LPCWSTR pszStart )
	{
		// Open tag w.o quotes
		while( pszStart && (pszStart = StrChrW( pszStart, L'<' ) ) )
		{
			if( pszStart[1] == L'?' ) // Correct as line is zero ended
			{
				pszStart = StrStrIW_Quotes( pszStart, L">" );
			}
			else if( pszStart[1] == '!' && pszStart[2] == L'-' && pszStart[3] == L'-') // Correct as line is zero ended
			{
				pszStart = StrStrIW( pszStart, L"-->" );
			}
			else
			{
				break;
			}
		}

		return pszStart;
	}

	inline static LPCWSTR StrXMLClose( LPCWSTR pszStart )
	{
		LPCWSTR pszEnd = StrChrW_Quotes( pszStart, L'>' );
		while( pszEnd )
		{
			// Check for comment end: ... -->
			if( pszEnd - pszStart > 3 && *(pszEnd - 1) == '-' && *(pszEnd - 2) == '-') // Correct as line is zero ended
			{
				pszEnd = StrChrW_Quotes( pszStart, L'>' );
			}
			else
			{
				break;
			}
		}

		return pszEnd;
	}

	// Type: 1: open, -1: close, 0: one tag
	inline static LPCWSTR NextTag( LPCWSTR pszStart, LPCWSTR* ppszEnd, CComBSTR& _cbsTag, BSTR* _pbsParam, int* _pnType )
	{
		if( _pnType )
			*_pnType = 0;

		int nType = 0;
		_cbsTag.Empty();

		if( _pbsParam ) 
			*_pbsParam = NULL;

		if( !pszStart ) return NULL;

		LPCWSTR pEnd = ppszEnd && *ppszEnd ? *ppszEnd : pszStart + lstrlenW(pszStart);
		if( pszStart >= pEnd ) 
			return NULL;

		LPCWSTR pOpen = StrXMLOpen( pszStart );
		if( !pOpen || pOpen >= pEnd ) 
			return NULL;

		LPCWSTR pClose = StrXMLClose( pOpen );
		if( !pClose || pClose > pEnd ) 
			pClose = pEnd - 1;

		if( ppszEnd )
			*ppszEnd = pClose + 1;

		LPCWSTR pTag = SkipBlank( pOpen + 1, false );
		if( *pTag == L'/' )
		{
			// < / ...
			nType = -1;
			if( _pnType ) * _pnType = nType;
			pTag++;
		}

		LPCWSTR pParam = pClose;
		pTag = NextWord( pTag, &pParam, _cbsTag, true );
		if( !pTag )
			return NULL;

		// Special fix for <![CDATA[Some Text]]>
		if( _cbsTag[0] == L'!' && _cbsTag[1] == L'[' )
		{
			pClose = StrStrW( pParam, L"]]>" );
			if( !pClose || pClose > pEnd ) 
				pClose = pEnd - 3;

			if( _pbsParam )
				*_pbsParam = ::SysAllocStringLen( pParam, pClose - pParam );

			if( ppszEnd )
				*ppszEnd = pClose + 3;

			nType = 2;
			if( _pnType )
				* _pnType = nType;

			return pOpen;
		}

		CharLowerW( _cbsTag.m_str );

		if( _cbsTag == "br" || _cbsTag == "tab" /*|| _cbsTag == "p"*/ )
		{
			nType = 0;
			if( _pnType )
				* _pnType = 0;
			return pOpen;
		}

		if( nType == -1 )
			return pOpen;	// Parsing complete </tag....>

		pParam = SkipBlank( pParam, false );
		pClose = SkipBlankRev( pClose - 1, false );

		if( *pClose != L'/' )
		{
			nType =	1; // <tag .... >
			if( _pnType ) //&&  _cbsTag != "img")
				* _pnType = nType;
		}
		else
		{
			// Skip '/' for parameters
			pClose = SkipBlankRev( pClose - 1, false );
		}
		// No params <tag /> or <tag>
		if( pClose == pParam )
			return pOpen;

		if( _pbsParam )
			*_pbsParam = ::SysAllocStringLen( pParam, pClose - pParam + 1 );

		return pOpen;
	}
};

#endif //__XML_UTILS_H_