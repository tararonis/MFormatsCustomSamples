//---------------------------------------------------------------------------
// This file is part of the MediaLooks DirectShow Template Library (DSTL)
//
// Guards.h : guarded calls of some WinAPI functions
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

#ifndef __GUARDS_H_
#define __GUARDS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Base/MFunctions.h"

/////////////////////////////////////////////////////////////////////////////
#ifndef DS_SEH_HANDLER
	#define DS_SEH_HANDLER	EXCEPTION_EXECUTE_HANDLER
#endif

/////////////////////////////////////////////////////////////////////////////
bool __stdcall SEH_CopyMemory(void* pDst, const void* pSrc, size_t cb);
bool __stdcall SEH_MoveMemory(void* pDst, const void* pSrc, size_t cb);
bool __stdcall SEH_FillMemory(void* pDst, int nValue, size_t cb);
bool __stdcall SEH_ZeroMemory(void* pDst, size_t cb);

//////////////////////////////////////////////////////////////////////////
// Some functions with exceptions handling
inline bool __stdcall SEH_CopyMemory(void* pDst, const void* pSrc, size_t cb)
{
	bool bRet = true;
	__try
	{
		CopyMemoryX(pDst, pSrc, (DWORD)cb);
	}
	__except(DS_SEH_HANDLER)
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Fatal,
			_T("###Err### SEH_CopyMemory( pDst=0x%X, pSrc=0x%X, cb=0x%X ): Exception 0x%X\n"),
			pDst, pSrc, cb, GetExceptionCode());
		bRet = false;
	}
	return bRet;
}

inline bool __stdcall SEH_MoveMemory(void* pDst, const void* pSrc, size_t cb)
{
	bool bRet = true;
	__try
	{
		::MoveMemory(pDst, pSrc, cb);
	}
	__except(DS_SEH_HANDLER)
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Fatal,
			_T("###Err### SEH_MoveMemory( pDst=0x%X, pSrc=0x%X, cb=0x%X ): Exception 0x%X\n"),
			pDst, pSrc, cb, GetExceptionCode());
		bRet = false;
	}
	return bRet;
}

// ATTENTION. We use signature of this function similar to memset():
// pointer to buffer (pDst), value for fill (nValue), size of buffer (cb)
// In ::FillMemory() last two parameters have opposite order
inline bool __stdcall SEH_FillMemory(void* pDst, int nValue, size_t cb)
{
	bool bRet = true;
	__try
	{
		::FillMemory(pDst, cb, nValue);
	}
	__except(DS_SEH_HANDLER)
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Fatal,
			_T("###Err### SEH_FillMemory( pDst=0x%X, nValue=0x%X, cb=0x%X ): Exception 0x%X\n"),
			pDst, nValue, cb, GetExceptionCode());
		bRet = false;
	}
	return bRet;
}

inline bool __stdcall SEH_ZeroMemory(void* pDst, size_t cb)
{
	bool bRet = true;
	__try
	{
		::FillMemory(pDst, cb, 0 );
	}
	__except(DS_SEH_HANDLER)
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Fatal,
			_T("###Err### SEH_ZeroMemory( pDst=0x%X, cb=0x%X ): Exception 0x%X\n"),
			pDst, cb, GetExceptionCode());
		bRet = false;
	}
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
#endif	 // __GUARDS_H_
