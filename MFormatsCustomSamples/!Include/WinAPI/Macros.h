//---------------------------------------------------------------------------
// This file is part of the MediaLooks DirectShow Template Library (DSTL)
//
// Macros.h : some macro definitions
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

#ifndef __MACROS_H_
#define __MACROS_H_

#include <Wincodec.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Some predefined string values 
#define MF_SIDEDATA_SAVE			L"_save_to_stream_"
#define MF_SIDEDATA_LOAD			L"_load_from_stream_"
#define MF_SIDEDATA_SIZE			L"_data_size_"
#define MF_SIDEDATA_REMOVE_STR		L"_str_remove_"

// New properties, TODO define all other string constants
#define MFSD_CREATETIME_DATA_SETGET		L"__crtm_"
#define MFSD_PARENT_OBJ_GET				L"_parent_"
#define MFSD_ALLOCATOR_OBJ_GET			L"_allocator_"
#define MFSD_BUFFER_OBJ_SETGET			L"_buffer_"
#define MFSD_BUFFER2_OBJ_SETGET			L"_buffer2_"

#define MF_PROCESS_PID_STR			L"_process_pid_"
#define MF_ANALYTICS_STR_ID			L"__ML_ANALYTICS_EVENT__"


// Query and call specified interface method
// Usage:
// HRESULT hr = S_OK; // Note: Before using HRESULT hr = S_OK; should be defined
// INTERFACE_CALL(IMFormat, m_objMPlaylist, FormatVideoSet(eMFT_Convert, &vidProps));
#ifndef INTERFACE_CALL
#define INTERFACE_CALL( _Interface, _pObject, _call ) { CComQIPtrM<_Interface> qpObject( _pObject ); if(!qpObject) hr = E_NOINTERFACE; else hr = qpObject->_call; }
#endif

// Define and create object 
// Usage
// HRESULT hr = S_OK;
// OBJECT_CREATE(IMDevice, objMLive, MLive);
#ifndef OBJECT_CREATE
#define OBJECT_CREATE( _Interface, _pObjVar, _ObjName ) CComPtr<_Interface> _pObjVar; hr = _pObjVar.CoCreateInstance(__uuidof(_ObjName));
#endif

/////////////////////////////////////////////////////////////////////////////
#ifndef __INTERFACE
#define  __INTERFACE	MIDL_INTERFACE
#endif	//__INTERFACE

#ifndef SIZEOF_ARRAY
#define SIZEOF_ARRAY(arr)	(sizeof(arr)/sizeof((arr)[0]))
#endif // SIZEOF_ARRAY

#ifndef ERROR_FROM_WIN32
#define ERROR_FROM_WIN32(x, err)   ((x)? ((HRESULT) (((x) & 0x0000FFFF) | (FACILITY_WIN32 << 16) | 0x80000000)) : (err))
#endif	// ERROR_FROM_WIN32


#define DS_MSEC			(REFERENCE_TIME)10000L		// 1 ms in 100 ns DirectShow units
#define DS_ONESEC		(REFERENCE_TIME)10000000L	// 1 second in 100 ns DirectShow units
#define DS_ONEMINUTE	(DS_ONESEC * 60)	
#define DS_ONEHOUR		(DS_ONESEC * 3600)	
#define DS_24HOUR		(DS_ONESEC * 3600 * 24)	

#define DS_NOPTS		(MINLONGLONG)

// uTime: time in 100 ns units
// ATTENTION !!! if uTime > MAX_LONG * DS_MSEC - result isn't correct
#define U2MS(uTime)		(LONG)((uTime)/DS_MSEC)
#define MSEC(uTime)		U2MS(uTime)

#define MS2U(msTime)	( (REFERENCE_TIME)(msTime) * DS_MSEC )

//////////////////////////////////////////////////////////////////////////
// Safe macros
// 

#define _SF_DEF_SIZE					(512)
#define USES_SF_MACROS_SZ(_wch_count)		WCHAR __szStrBuf[16][_wch_count] = {}; int __nStrBufIdx = 0;
#define USES_SF_MACROS						USES_SF_MACROS_SZ(_SF_DEF_SIZE)
#define __STR_BUF						(__szStrBuf[__nStrBufIdx = (__nStrBufIdx++) % SIZEOF_ARRAY(__szStrBuf)])
#define __STR_BUF_LEN						(SIZEOF_ARRAY(__szStrBuf[0]))

#define RT2STR_SF(t)					( RefTime2Str( (t), __STR_BUF ) )
#define N64STR_SF(num64)				( N64ToStr((num64), (LPTSTR)__STR_BUF) )
#define ST2STR_SF(time)					( SysTime2Str((time), (LPTSTR)__STR_BUF, true) )
#define FT2STR_SF(time)					( FileTime2Str((time), (LPTSTR)__STR_BUF, true) )


#define USES_RT2STR_EX					TCHAR __szRt2StrEx[32][32] = {}; int __nRt2StrIdx = 0;
#define RT2STR_EXN(t)					( RefTime2Str( (t), __szRt2StrEx[__nRt2StrIdx = (__nRt2StrIdx++) % 32] ) )
#define RT2STR_EX(t,idx)				RT2STR_EXN(t) 

//////////////////////////////////////////////////////////////////////////
// Utility
// 
#define ZERO_OBJECT( _obj )  ZERO_STRUCT(_obj)
#define ZERO_STRUCT( _struct )  (::ZeroMemory( &_struct, sizeof(_struct) ))
#define IS_SAME_STRUCT( _struct1, _struct2 )  (sizeof(_struct1) == sizeof(_struct2) && memcmp( &_struct1, &_struct2, sizeof(_struct1)) == 0)
#define STR_IS_EMPTY( _psz ) ( !(_psz) || !(_psz)[0] )
#define STR_LEN( _psz ) ( (_psz) ? lstrlenW( _psz ) : 0 )
#define STR_IS_SAME_I( _psz, _cmp ) ( ((_psz) && (_cmp)) ? lstrcmpiW( _psz, _cmp ) == 0 : (bool)(_psz) == (bool)(_cmp) )
#define STR_IS_SAME( _psz, _cmp ) ( ((_psz) && (_cmp)) ? lstrcmpW( _psz, _cmp ) == 0 : (bool)(_psz) == (bool)(_cmp) )
#define STR_IS_SAME_IA( _psz, _cmp ) ( ((_psz) && (_cmp)) ? lstrcmpiA( _psz, _cmp ) == 0 : (bool)(_psz) == (bool)(_cmp) )
#define STR_IS_SAMEA( _psz, _cmp ) ( ((_psz) && (_cmp)) ? lstrcmpA( _psz, _cmp ) == 0 : (bool)(_psz) == (bool)(_cmp) )


#ifndef ALLOCA_ARRAY
#define ALLOCA_ARRAY( _type, _count ) (_type*)alloca( sizeof(_type) * (_count) )
#endif


/////////////////////////////////////////////////////////////////////////////
// Math
// 
#ifndef ABS
#define ABS(x)		( ((x) >= 0)? (x) : -(x) )
#endif	// ABS

#ifndef SIGN
#define SIGN(val)	( ((val) >= 0)? 1 : (-1) )
#endif


#ifndef MAX
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN 
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef ROUND
#define ROUND(x)			(static_cast<int>(x + 0.5))
#endif

//////////////////////////////////////////////////////////////////////////
// Resource
// 
#ifndef RESOURCE_STR
#define RESOURCE_STR( _id ) LoadResString( (_id), (LPTSTR)alloca(1024*sizeof(TCHAR)), 1024 )
#endif

#ifndef RESOURCE_STR2
#define RESOURCE_STR2( _id, _length ) LoadResString( (_id), (LPTSTR)alloca((_length)*sizeof(TCHAR)), (_length) )
#endif

//////////////////////////////////////////////////////////////////////////
// Some WinNT definition

#ifndef MINLONGLONG
#define MINLONGLONG			(LONGLONG)(0x8000000000000000)
#endif

#ifndef MAXLONGLONG
#define MAXLONGLONG			(LONGLONG)(0x7fffffffffffffff)
#endif

#ifndef LODWORD
#define LODWORD(l)			( (DWORD)((ULONGLONG)(l) & 0xffffffff) )
#endif

#ifndef HIDWORD
#define HIDWORD(l)			( (DWORD)((ULONGLONG)(l) >> 32) )
#endif

#ifndef MAKELONGLONG
#define MAKELONGLONG(a, b)	( (((ULONGLONG)(a) & 0xffffffff))|(((ULONGLONG)(b) & 0xffffffff) << 32) )
#endif

//////////////////////////////////////////////////////////////////////////
// Color-related macros
#ifndef RGBA
#define RGBA(r, g, b, alpha) ((COLORREF)( ((DWORD)r & 0xFF)|(((DWORD)g & 0xFF)<<8)|(((DWORD)b & 0xFF)<<16)|(((DWORD)alpha & 0xFF)<<24) ) )
#endif

#ifndef BGRA
#define BGRA(b, g, r, alpha) RGBA(b, g, r, alpha)
#endif

#ifndef GetAlphaValue
#define GetAlphaValue(argb)      (LOBYTE(((DWORD)(argb))>>24))
#endif

#ifndef GetRGBValue
#define GetRGBValue(argb)      ((DWORD)(argb)&0x00FFFFFF)
#endif

//////////////////////////////////////////////////////////////////////////
// General macros
#define CONCAT2(X, Y) X##Y
#define CONCAT(X, Y) CONCAT2(X, Y)
#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)
#define STRWIDEN(X) CONCAT(L, STRINGIFY(X))

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)

//////////////////////////////////////////////////////////////////////////
// Performance macros

#ifdef _DEBUG

#ifndef MLOGTIME
#define MLOGTIME(_TIMER, _TMES) _TIMER.LogTime( L">>>Time>>>" WIDEN(__FUNCTION__) L" " WIDEN(_TMES) )
#endif 

#ifndef MBASICPERF
#define MBASICPERF CBasicPerfD perf( L">>>Perf>>>" WIDEN(__FILE__) L": " WIDEN(__FUNCTION__) )
#endif

#ifndef MBASICPERF2
#define MBASICPERF2(_MES) CBasicPerfD perf( L">>>Perf>>>" WIDEN(__FILE__) L": " WIDEN(__FUNCTION__) L": " WIDEN(_MES) )
#endif

#else
#define MLOGTIME(_TIMER, _TMES)
#define MBASICPERF
#define MBASICPERF2
#endif


//////////////////////////////////////////////////////////////////////////
// Rect macros

#ifndef IS_RECT_EMPTY
#define IS_RECT_EMPTY( _pRect ) ((_pRect) ? ((_pRect)->left >= (_pRect)->right || (_pRect)->top >= (_pRect)->bottom) : true)
#endif

#ifndef IS_RECT_AEMPTY
#define IS_RECT_AEMPTY( _pRect ) ((_pRect) ? ((_pRect)->left == (_pRect)->right || (_pRect)->top == (_pRect)->bottom) : true)
#endif

#ifndef RECT_W
#define RECT_W( _rect ) ((_rect).right - (_rect).left )
#endif

#ifndef RECT_H
#define RECT_H( _rect ) ((_rect).bottom - (_rect).top )
#endif

#ifndef CROP_W
#define CROP_W( _rect ) ((_rect).right + (_rect).left )
#endif

#ifndef CROP_H
#define CROP_H( _rect ) ((_rect).bottom + (_rect).top )
#endif

#ifndef PRECT_W
#define PRECT_W( _pRect ) ((_pRect) ? ((_pRect)->right - (_pRect)->left ) : 0)
#endif

#ifndef PRECT_H
#define PRECT_H( _pRect ) ((_pRect) ? ((_pRect)->bottom - (_pRect)->top ) : 0)
#endif

#ifndef PRECT_AW
#define PRECT_AW( _pRect ) ((_pRect) ? ABS((_pRect)->right - (_pRect)->left ) : 0)
#endif

#ifndef PRECT_AH
#define PRECT_AH( _pRect ) ((_pRect) ? ABS((_pRect)->bottom - (_pRect)->top ) : 0)
#endif

/////////////////////////////////////////////////////////////////////////////
// Some convertions
#define DBL2INT(dbl)		(INT)( ((dbl) >= 0.0)? (dbl) + 0.5 : (dbl) - 0.5 )
#define DBL2INT64(dbl)		(LONGLONG)( ((dbl) >= 0.0)? (dbl) + 0.5 : (dbl) - 0.5 )

#define INT2SHORT(val)		(SHORT)( ((INT)(val) < (SHORT)(WORD)MINSHORT)? MINSHORT : ((INT)(val) < (SHORT)MAXSHORT)? (val) : MAXSHORT )
#define INT2WORD(val)		(WORD)( ((INT)(val) < 0)? 0 : ((INT)(val) < MAXWORD)? (val) : MAXWORD )
#define INT2USHORT(val)		INT2WORD(val)

#define UINT2SHORT(val)		(SHORT)( ((UINT)(val) < MAXSHORT)? (val) : MAXSHORT )
#define UINT2WORD(val)		(WORD)( ((UINT)(val) < MAXWORD)? (val) : MAXWORD )
#define UINT2USHORT(val)	(USHORT)UINT2WORD(val)

#define INT2CHAR(val)		(CHAR)( ((INT)(val) < (CHAR)(BYTE)MINCHAR)? MINCHAR : ((INT)(val) < (CHAR)MAXCHAR)? (val) : MAXCHAR )
#define INT2BYTE(val)		(BYTE)( ((INT)(val) < 0)? 0 : ((INT)(val) < MAXBYTE)? (val) : MAXBYTE )
#define INT2UCHAR(val)		(UCHAR)INT2BYTE(val)

#define UINT2CHAR(val)		(CHAR)( ((UINT)(val) < MAXCHAR)? (val) : MAXCHAR )
#define UINT2BYTE(val)		(BYTE)( ((UINT)(val) < MAXBYTE)? (val) : MAXBYTE )
#define UINT2UCHAR(val)		(UCHAR)UINT2BYTE(val)

#define SHORT2CHAR(val)		(CHAR)( ((SHORT)(val) < (CHAR)(BYTE)MINCHAR)? MINCHAR : ((SHORT)(val) < (CHAR)MAXCHAR)? (val) : MAXCHAR )
#define SHORT2BYTE(val)		(BYTE)( ((SHORT)(val) < 0)? 0 : ((SHORT)(val) < MAXBYTE)? (val) : MAXBYTE )
#define SHORT2UCHAR(val)	(UCHAR)SHORT2BYTE(val)

#define USHORT2CHAR(val)	(CHAR)( ((USHORT)(val) < MAXCHAR)? (val) : MAXCHAR )
#define USHORT2BYTE(val)	(BYTE)( ((USHORT)(val) < MAXBYTE)? (val) : MAXBYTE )
#define USHORT2UCHAR(val)	(UCHAR)USHORT2BYTE(val)

/////////////////////////////////////////////////////////////////////////////
// Fix for old InterlockedCompareExchange().
// Old InterlockedCompareExchange() has signature different from new version of this method.
// See MSDN "PRB: New Definitions of InterlockedExchange and InterlockedCompareExchange
// Not Backward Compatible" (Q292752)
#if defined(InterlockedCompareExchangePointer) && !defined(_M_IA64)
	// InterlockedCompareExchangePointer defined in SDK with the same signature as old
	// InterlockedCompareExchange.
	#define InterlockedCompareExchange_Fix(dst, exch, comp)	InterlockedCompareExchange((dst), (exch), (comp))
#else
	// Use type casting for old InterlockedCompareExchange()
	#define InterlockedCompareExchange_Fix(dst, exch, comp)	(LONG)InterlockedCompareExchange((PVOID*)(dst), (PVOID)(exch), (PVOID)(comp))
#endif

/////////////////////////////////////////////////////////////////////////////
// Output of error position to "Output window" of Visual Studio.
// File/line will be opened after double click on this output line in "Output window"
#define	DSTLTRACE(hr)	\
	ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("%s(%u) : Error 0x%X\n"), __FILE__, __LINE__, hr)

/////////////////////////////////////////////////////////////////////////////
// Printing of message to VisualC++ output window.
// Usage:
//	#pragma vcout("TODO: Microsoft DirectDraw shows many bad messages here")
// Output:
//	G:\cvsroot\DV\Diagnose\diag.cpp(838) : TODO: Microsoft DirectDraw shows many bad messages here
//
// ATTENTION. Don't use this macros in template classes !!!
// Possible result is: "fatal error C1001: INTERNAL COMPILER ERROR"

#ifndef vcout
	#define vcout_dummy_1(x)	#x
	#define vcout_dummy_2(x)	vcout_dummy_1(x)
	#define vcout(desc) message(__FILE__ "(" vcout_dummy_2( __LINE__ )  ") : " desc)
#endif	// vcout

#ifndef ABS_NEG
#define ABS_NEG( _val ) ((_val) >= 0 ? (_val) : -1 * ((_val) + 1) )
#endif

#ifndef NEG
#define NEG( _val ) (((_val) + 1) * -1)
#endif

#ifndef FRACT
#define FRACT( val )  ((val) - (LONGLONG)(val))
#endif

#ifndef FRACT_ABS
#define FRACT_ABS( val )  (FRACT(val) > 0.5 ? (1.0 - FRACT(val)) : FRACT(val))
#endif

// Value in static array (with bounds check)
#ifndef ARRAY_VAL
#define ARRAY_VAL( arr, idx ) arr[ MAX( MIN( (int)(idx), SIZEOF_ARRAY(arr) - 1 ), 0 ) ]
#endif

// Value in static NULL ended array (with bounds check)
#ifndef ARRAYN_VAL
#define ARRAYN_VAL( arr, idx ) arr[ MAX( MIN( (int)(idx), SIZEOF_ARRAY(arr) - 2 ), 0 ) ]
#endif

#define _MTRACE_FATAL(_TMES){ ATLTRACE2(atlTraceGeneral,1,_T("###Err###(Fatal) %hs %s\n"),__FUNCTION__,_TMES); ATLASSERT(!"Fatal error"); }
#define _MTRACE_ERR(_TMES)  { ATLTRACE2(atlTraceGeneral,2,_T("###Err### %hs %s\n"),__FUNCTION__,_TMES); }
#define _MTRACE_WARN(_TMES) { ATLTRACE2(atlTraceGeneral,4,_T("WARNING !!! %hs %s\n"),__FUNCTION__,_TMES); }
#define _MTRACE_INFO(_TMES) { ATLTRACE2(atlTraceGeneral,6,_T("INFO >> %hs %s\n"),__FUNCTION__,_TMES); }
#define _MTRACE_PROC(_TMES) { ATLTRACE2(atlTraceGeneral,8,_T("%hs %s\n"),__FUNCTION__,_TMES); }
#define _MTRACE_LEVEL(_LEVEL,_TMES) { ATLTRACE2(atlTraceGeneral,_LEVEL,_T("%hs %s\n"),__FUNCTION__,_TMES); }
#define _MTRACE_PREFIX(_TPREF,_TMES) { ATLTRACE2(atlTraceGeneral,8,_T("%s %hs %s\n"),_TPREF,__FUNCTION__,_TMES); }

#ifndef MTRACE_BUFFER_LEN
#define MTRACE_BUFFER_LEN (1024)
#endif

#if defined(_DEBUG) || defined(_LOG_DEBUG)
#define MTRACE_FATAL(_TMES,...){ TCHAR pchFmtMes[MTRACE_BUFFER_LEN]; _sntprintf(pchFmtMes,SIZEOF_ARRAY(pchFmtMes),_T(_TMES),__VA_ARGS__); _MTRACE_FATAL(pchFmtMes);  }
#define MTRACE_ERR(_TMES,...)  { TCHAR pchFmtMes[MTRACE_BUFFER_LEN]; _sntprintf(pchFmtMes,SIZEOF_ARRAY(pchFmtMes),_T(_TMES),__VA_ARGS__); _MTRACE_ERR(pchFmtMes);  }
#define MTRACE_WARN(_TMES,...) { TCHAR pchFmtMes[MTRACE_BUFFER_LEN]; _sntprintf(pchFmtMes,SIZEOF_ARRAY(pchFmtMes),_T(_TMES),__VA_ARGS__); _MTRACE_WARN(pchFmtMes);  }
#define MTRACE_INFO(_TMES,...) { TCHAR pchFmtMes[MTRACE_BUFFER_LEN]; _sntprintf(pchFmtMes,SIZEOF_ARRAY(pchFmtMes),_T(_TMES),__VA_ARGS__); _MTRACE_INFO(pchFmtMes);  }
#define MTRACE_PROC(_TMES,...) { TCHAR pchFmtMes[MTRACE_BUFFER_LEN]; _sntprintf(pchFmtMes,SIZEOF_ARRAY(pchFmtMes),_T(_TMES),__VA_ARGS__); _MTRACE_PROC(pchFmtMes);  }
#define MTRACE_L(_LEVEL,_TMES,...) { TCHAR pchFmtMes[MTRACE_BUFFER_LEN]; _sntprintf(pchFmtMes,SIZEOF_ARRAY(pchFmtMes),_T(_TMES),__VA_ARGS__); _MTRACE_LEVEL(_LEVEL,pchFmtMes);  }
#define MTRACE_PREFIX(_TPREF,_TMES,...) { TCHAR pchFmtMes[MTRACE_BUFFER_LEN]; _sntprintf(pchFmtMes,SIZEOF_ARRAY(pchFmtMes),_T(_TMES),__VA_ARGS__); _MTRACE_PREFIX(_TPREF,pchFmtMes);  }
#else
#define MTRACE_FATAL
#define MTRACE_ERR
#define MTRACE_WARN
#define MTRACE_INFO
#define MTRACE_PROC
#define MTRACE_L
#define MTRACE_PREFIX
#endif

#define MTRACE MTRACE_PROC

/////////////////////////////////////////////////////////////////////////////
#endif	 // __MACROS_H_
