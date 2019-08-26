//---------------------------------------------------------------------------
// This file is part of the AGAVA DirectShow Template Library (DSTL)
//
// Parsers.h : some functions for data parsing
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

#ifndef __PARSERS_H_
#define __PARSERS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////
LPCTSTR __stdcall Variant2Str(const VARIANT* pVar, LPTSTR pszName, DWORD nMaxSym);
LPCTSTR __stdcall Vartype2Name(const VARTYPE _vt, LPTSTR pszName, DWORD nMaxSym);

/////////////////////////////////////////////////////////////////////////////
inline LPCTSTR __stdcall Variant2Str(const VARIANT* pVar, LPTSTR pszName, DWORD nMaxSym)
{
	ATLASSERT(pVar != NULL && _T("Variant2Str()"));
	if(pVar == NULL) return _T("###Err###");

	ATLASSERT(pszName != NULL && _T("Variant2Str()"));
	if(pszName == NULL) return _T("###Err###");

	ATLASSERT(nMaxSym > 0 && _T("Variant2Str()"));
	if(nMaxSym == 0) return _T("###Err###");

	//---------------------------------
	TCHAR szType[256] = _T("[");
	Vartype2Name(pVar->vt, szType + 1, SIZEOF_ARRAY(szType) - 3);
	lstrcat(szType, _T("] "));

	USES_CONVERSION;
	TCHAR szVal[256] = _T("???");
	switch(pVar->vt)
	{
		case VT_UI2:
			wsprintf(szVal, _T("%u"), pVar->uiVal);
			break;

		case VT_I4:
			wsprintf(szVal, _T("%i"), pVar->lVal);
			break;

		case VT_UI4:
			wsprintf(szVal, _T("%u"), pVar->ulVal);
			break;

		case VT_INT:
			wsprintf(szVal, _T("%i"), pVar->intVal);
			break;

		case VT_UINT:
			wsprintf(szVal, _T("%u"), pVar->uintVal);
			break;
 
		case VT_R8:
			_stprintf(szVal, _T("%g"), pVar->dblVal);
			break;

		case VT_BOOL:
			lstrcpy(szVal, (pVar->boolVal == VARIANT_TRUE)? _T("TRUE") : _T("FALSE"));
			break;

		case VT_BSTR:
			if(pVar->bstrVal != NULL)
			{
				szVal[0] = (TCHAR)'\"';
				lstrcpyn(szVal + 1, OLE2T(pVar->bstrVal), SIZEOF_ARRAY(szVal) - 2);
				int n = lstrlen(szVal);
				szVal[n] = (TCHAR)'\"';
				szVal[n + 1] = (TCHAR)0;
				if(n + 2 == SIZEOF_ARRAY(szVal))
				{
					szVal[n - 3] = (TCHAR)'.';
					szVal[n - 2] = (TCHAR)'.';
					szVal[n - 1] = (TCHAR)'.';
				}
			}
			else
				lstrcpy(szVal, _T("(null)"));
			break;
	}

	lstrcpyn(pszName, szType, nMaxSym);
	int n = lstrlen(pszName);
	lstrcpyn(pszName + n, szVal, nMaxSym - n);

	n = lstrlen(pszName);
	if((DWORD)n == nMaxSym - 1 && nMaxSym > 3)
	{
		pszName[n - 3] = (TCHAR)'.';
		pszName[n - 2] = (TCHAR)'.';
		pszName[n - 1] = (TCHAR)'.';
	}
	return pszName;
}

inline LPCTSTR __stdcall Vartype2Name(const VARTYPE _vt, LPTSTR pszName, DWORD nMaxSym)
{
	struct VT_NAME
	{
		VARTYPE vt;
		LPCTSTR	pszName;
	};

	#define __VT_NAME(vt)	{ vt, _T(#vt) }

	static VT_NAME rgVTNames[] =
	{
		__VT_NAME(VT_EMPTY),
		__VT_NAME(VT_NULL),
		__VT_NAME(VT_I2),
		__VT_NAME(VT_I4),
		__VT_NAME(VT_R4),
		__VT_NAME(VT_R8),
		__VT_NAME(VT_CY),
		__VT_NAME(VT_DATE),
		__VT_NAME(VT_BSTR),
		__VT_NAME(VT_DISPATCH),
		__VT_NAME(VT_ERROR),
		__VT_NAME(VT_BOOL),
		__VT_NAME(VT_VARIANT),
		__VT_NAME(VT_UNKNOWN),
		__VT_NAME(VT_DECIMAL),
		__VT_NAME(VT_I1),
		__VT_NAME(VT_UI1),
		__VT_NAME(VT_UI2),
		__VT_NAME(VT_UI4),
		__VT_NAME(VT_I8),
		__VT_NAME(VT_UI8),
		__VT_NAME(VT_INT),
		__VT_NAME(VT_UINT),
		__VT_NAME(VT_VOID),
		__VT_NAME(VT_HRESULT),
		__VT_NAME(VT_PTR),
		__VT_NAME(VT_SAFEARRAY),
		__VT_NAME(VT_CARRAY),
		__VT_NAME(VT_USERDEFINED),
		__VT_NAME(VT_LPSTR),
		__VT_NAME(VT_LPWSTR),
		__VT_NAME(VT_RECORD),
		__VT_NAME(VT_FILETIME),
		__VT_NAME(VT_BLOB),
		__VT_NAME(VT_STREAM),
		__VT_NAME(VT_STORAGE),
		__VT_NAME(VT_STREAMED_OBJECT),
		__VT_NAME(VT_STORED_OBJECT),
		__VT_NAME(VT_BLOB_OBJECT),
		__VT_NAME(VT_CF),
		__VT_NAME(VT_CLSID),
		__VT_NAME(VT_BSTR_BLOB)
	};

	static VT_NAME rgVTExts[] =
	{
		__VT_NAME(VT_VECTOR),
		__VT_NAME(VT_ARRAY),
		__VT_NAME(VT_BYREF),
		__VT_NAME(VT_RESERVED)
	};

	#undef __VT_NAME
	
	//---------------------------------
	ATLASSERT(pszName != NULL && _T("Vartype2Name()"));
	if(pszName == NULL) return _T("###Err###");

	ATLASSERT(nMaxSym > 0 && _T("Vartype2Name()"));
	if(nMaxSym == 0) return _T("###Err###");

	//---------------------------------
	VARTYPE vt = (_vt & VT_TYPEMASK);

	LPCTSTR pszVT = NULL;
	int i = 0;
	for( i = 0; i < SIZEOF_ARRAY(rgVTNames); ++i) 
	{
		if(rgVTNames[i].vt == vt)
		{
			pszVT = rgVTNames[i].pszName;
			break;
		}
	}

	TCHAR sz[128] = _T("");
	VARTYPE vtExt = (_vt & ~VT_TYPEMASK);
	if(vtExt != 0)
	{
		VARTYPE vtE = vtExt;
		for(i = 0; i < SIZEOF_ARRAY(rgVTExts); ++i) 
		{
			if((rgVTExts[i].vt & vtE) != 0)
			{
				lstrcat(sz, rgVTExts[i].pszName);
				lstrcat(sz, _T(" | "));
				
				vtE &= ~rgVTExts[i].vt;
			}
		}
		if(vtE != 0)
			wsprintf(sz + lstrlen(sz), _T("0x%X | "), vtE);
	}

	if(pszVT != NULL)
		lstrcat(sz, pszVT);
	else
		wsprintf(sz + lstrlen(sz), _T("0x%X"), vt);

	lstrcpyn(pszName, sz, nMaxSym);
	return pszName;
}

/////////////////////////////////////////////////////////////////////////////
#endif	// __PARSERS_H_
