//---------------------------------------------------------------------------
// This file is part of the MediaLooks DirectShow Template Library (DSTL)
//
// DSGuard.h : functions and classes for guarded calls of some unsafe methods
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
//
// Sample :
//	hr = SEH_IPin::Connect(spPin, spReceivePin, NULL);
//
// Output to log if raised excepion:
//	###Err### SEH_IPin::Connect(pReceivePin, pmt): Exception 0xC0000005
//
//---------------------------------------------------------------------------

#ifndef __DSGUARD_H_
#define __DSGUARD_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\WinAPI\Functions.h"	// Guid2Str()
#include "..\Base\DSDefines.h"				// DS_SEH_HANDLER, TRACELEVEL_SEH

/////////////////////////////////////////////////////////////////////////////
// Functions in this file

//bool __stdcall IsMMXState();
void __stdcall ClearMMXState();

WORD __stdcall FPUPrecision();

HRESULT __stdcall SEH_CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter,
									   DWORD dwClsContext, REFIID riid, LPVOID FAR* ppv);
HRESULT __stdcall SEH_QueryInterface(IUnknown* pThis, REFIID riid, void** ppvObject);

//---------------------------------
// Classes in this file
class CFPUPrecision;
class CFPUAutoPrec;

class SEH_IPin;
class SEH_IMemInputPin;
class SEH_IBaseFilter;
class SEH_IMemAllocator;
class SEH_IGraphBuilder;
class SEH_IPersistStream;
class SEH_IPropertyBag;
class SEH_IEnumPins;
class SEH_IEnumMediaTypes;
class SEH_IEnumFilters;
class SEH_IKsPropertySet;
class SEH_IDvdControl2;
class SEH_IDvdInfo2;
class SEH_ICreateDevEnum;
class SEH_IEnumMoniker;
class SEH_IMoniker;

/////////////////////////////////////////////////////////////////////////////
#ifdef _WIN64
	#include <mmintrin.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// Checks MMX state : some codecs not call emms instruction after MMX
// operations and some floating point operations failed (or result is incorrect).
#ifndef _WIN64
inline bool __stdcall IsMMXState()
{
	WORD arrFPUEnv[14];
	__asm fnstenv arrFPUEnv
	return (arrFPUEnv[4] != 0xffff);
}
#else 
inline bool __stdcall IsMMXState()
{
	return false;
}
#endif	// !_WIN64

#ifndef _WIN64
// ATTENTION : emms clear FPU registers, you should use this function only for fixing bugs
inline void __stdcall ClearMMXState()
{
	if(IsMMXState())
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### MMX state don't cleared - clear FPU tag word !!!\n"));
		__asm	emms
	}
}
#endif	// !_WIN64

#ifndef	TRACE_MMX_STATE
#ifndef _WIN64
	#define	TRACE_MMX_STATE \
		if(IsMMXState()) \
		{ \
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### MMX state don't cleared !!!\n")); \
		}
#else
	#define	TRACE_MMX_STATE
#endif	// !_WIN64
#endif	// TRACE_MMX_STATE

/////////////////////////////////////////////////////////////////////////////
#ifndef _WIN64
#define GUARDED_CALL(level, prefix, fun) \
	HRESULT hr = E_INVALIDARG; \
	if(pThis != NULL) \
	{ \
		__try \
		{ \
			hr = pThis->fun; \
		} \
		__except(DS_SEH_HANDLER) \
		{ \
			ATLTRACE2(atlTraceGeneral, level, \
				_T("###Err### ") prefix _T(#fun) _T(": Exception 0x%X\n"), GetExceptionCode()); \
			if(IsMMXState()) \
			{ \
				ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### MMX state don't cleared !!!\n")); \
				__asm	emms \
			} \
			hr = E_UNEXPECTED; \
		} \
	}

#else
#define GUARDED_CALL(level, prefix, fun) \
	HRESULT hr = E_INVALIDARG; \
	if(pThis != NULL) \
	{ \
		__try \
		{ \
			hr = pThis->fun; \
		} \
		__except(DS_SEH_HANDLER) \
		{ \
			ATLTRACE2(atlTraceGeneral, level, \
				_T("###Err### ") prefix _T(#fun) _T(": Exception 0x%X\n"), GetExceptionCode()); \
			hr = E_UNEXPECTED; \
		} \
	}
#endif	// !_WIN64

/////////////////////////////////////////////////////////////////////////////

#ifndef _WIN64

// FPU control word
//
//                  15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0
//                 | - | - | - | X |  RC   |  PC   | - | - | P | U | O | Z | D | I |
//                 |   |   |   |   |       |       |   |   | M | M | M | M | M | M |
// Default: 0x027f   0   0   0   0   0   0   1   0   0   1   1   1   1   1   1   1
//
//	X - Infinity Control
//	RC - Rounding Control
//	PC - Precision Control
//
//	Exception Masks
//	PM - Precision
//	UM - Underflow
//	OM - Overflow
//	ZM - Zero Divide
//	DM - Denormal Operand
//	IM - Invalid Operation

// Real bits for FPU control word
#define	FPU_PC		0x300		// Precision control mask
#define	FPU_24		0			// Single precision (24 bits)
#define	FPU_53		0x200		// Double precision (53 bits)
#define	FPU_64		0x300		// Double extended precision (64 bits)

inline WORD __stdcall FPUPrecision()
{
	unsigned short wControl;
	__asm	fstcw	wControl
	return (WORD)(wControl & FPU_PC);
}

//---------------------------------
class CFPUPrecision
{
	WORD m_wControl;
public:
	CFPUPrecision(): m_wControl(FPU_53) {}

	void Set(WORD wPrecision = FPU_53)
	{
		WORD wControl;
		__asm	fstcw	wControl

		m_wControl = wControl;
		wControl = (WORD)((wControl & ~FPU_PC) | (wPrecision & FPU_PC));
		__asm	fldcw	wControl
	}
	void Reset()
	{
		WORD wControl;
		__asm	fstcw	wControl

		wControl = (WORD)((wControl & ~FPU_PC) | (m_wControl & FPU_PC));
		__asm	fldcw	wControl
	}
};

class CFPUAutoPrec : public CFPUPrecision
{
public:
	CFPUAutoPrec(WORD wPrecision = FPU_53)
	{
		Set(wPrecision);
	}
	~CFPUAutoPrec()
	{
		Reset();
	}
};

#else

class CFPUAutoPrec
{
public:
	CFPUAutoPrec()
	{
	}
	~CFPUAutoPrec()
	{
	}
};
#endif	// !_WIN64

//---------------------------------
#ifndef	TRACE_FPU_PRECISION
#ifndef _WIN64
	#define	TRACE_FPU_PRECISION(minPrecision) \
		if(FPUPrecision() < minPrecision) \
		{ \
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### FPU has low precision %s bits\n"), \
				(FPUPrecision() == FPU_24)? _T("24") : _T("53")); \
		}
#else
	#define	TRACE_FPU_PRECISION(minPrecision)
#endif	// !_WIN64
#endif	// TRACE_FPU_PRECISION

#ifndef	TRACE_FPU_PRECISION_STD
	#define	TRACE_FPU_PRECISION_STD		TRACE_FPU_PRECISION(FPU_53)
#endif	// TRACE_FPU_PRECISION_STD

/////////////////////////////////////////////////////////////////////////////
// Guarded call for CoCreateInstance()

#ifndef TRACELEVEL_SEH_CoCreateInstance
	#define TRACELEVEL_SEH_CoCreateInstance	(TRACELEVEL_SEH)
#endif

inline HRESULT __stdcall SEH_CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter,
											  DWORD dwClsContext, REFIID riid, LPVOID FAR* ppv)
{
	HRESULT hr = S_OK;
	__try
	{
		hr = ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv);
	}
	__except(DS_SEH_HANDLER)
	{
#ifndef NDEBUG
		DWORD dwExcept = GetExceptionCode();
		TCHAR szGuid[64] = _T("");
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_SEH_CoCreateInstance,
			_T("###Err### SEH_CoCreateInstance( %s ): Exception 0x%X\n"),
			Guid2Str(rclsid, szGuid), dwExcept);
#endif
#ifndef _WIN64
		if(IsMMXState())
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### MMX state don't cleared !!!\n"));

//			_mm_empty();

			__asm	emms
		}
#endif
		hr = E_UNEXPECTED;
	}
	return hr;
}

/////////////////////////////////////////////////////////////////////////////
// Guarded call for QueryInterface()

#ifndef TRACELEVEL_SEH_QueryInterface
	#define TRACELEVEL_SEH_QueryInterface	(TRACELEVEL_SEH)
#endif

inline HRESULT __stdcall SEH_QueryInterface(IUnknown* pThis, REFIID riid, void** ppvObject)
{
	ATLASSERT(pThis != NULL);
	HRESULT hr = E_INVALIDARG;
	if(pThis != NULL)
	{
		TCHAR szIID[64] = _T("");
		szIID;

		__try
		{
			hr = pThis->QueryInterface(riid, ppvObject);
		}
		__except(DS_SEH_HANDLER)
		{
#ifndef NDEBUG
			DWORD dwExcept = GetExceptionCode();
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_SEH_QueryInterface,
				_T("###Err### SEH_QueryInterface( %s ): Exception 0x%X\n"),
				Guid2Str(riid, szIID), dwExcept);
#endif
#ifndef _WIN64
			if(IsMMXState())
			{
				ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### MMX state don't cleared !!!\n"));

//				_mm_empty();

				__asm	emms

			}
#endif
			hr = E_UNEXPECTED;
		}

		if(hr == S_OK && *ppvObject == NULL)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_SEH_QueryInterface,
				_T("###Err### SEH_QueryInterface( %s ): hr == S_OK && *ppvObject == NULL\n"),
				Guid2Str(riid, szIID));
			hr = E_NOINTERFACE;
		}
	}
	return hr;
}

/////////////////////////////////////////////////////////////////////////////
// Guarded calls for methods of IPin interface

#ifndef TRACELEVEL_SEH_IPin
	#define TRACELEVEL_SEH_IPin	(TRACELEVEL_SEH)
#endif

#define GUARDED_CALL_IPin(fun) \
	GUARDED_CALL(TRACELEVEL_SEH_IPin, _T("SEH_IPin::"), fun)

class SEH_IPin
{
public :
	static HRESULT Connect(IPin* pThis, IPin* pReceivePin, const AM_MEDIA_TYPE* pmt)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IPin(Connect(pReceivePin, pmt));
		return hr;
	}
	static HRESULT ReceiveConnection(IPin* pThis, IPin* pConnector, const AM_MEDIA_TYPE* pmt)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IPin(ReceiveConnection(pConnector, pmt));
		return hr;
	}
	static HRESULT Disconnect(IPin* pThis)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IPin(Disconnect());
		return hr;
	}
	static HRESULT ConnectedTo(IPin* pThis, IPin** pPin)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IPin(ConnectedTo(pPin));
		return hr;
	}
	static HRESULT ConnectionMediaType(IPin* pThis, AM_MEDIA_TYPE* pmt)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IPin(ConnectionMediaType(pmt));
		return hr;
	}
	static HRESULT QueryPinInfo(IPin* pThis, PIN_INFO* pInfo)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IPin(QueryPinInfo(pInfo));
		return hr;
	}
	static HRESULT QueryDirection(IPin* pThis, PIN_DIRECTION* pPinDir)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IPin(QueryDirection(pPinDir));
		return hr;
	}
	static HRESULT QueryId(IPin* pThis, LPWSTR* Id)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IPin(QueryId(Id));
		return hr;
	}
	static HRESULT QueryAccept(IPin* pThis, const AM_MEDIA_TYPE* pmt)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IPin(QueryAccept(pmt));
		return hr;
	}
	static HRESULT EnumMediaTypes(IPin* pThis, IEnumMediaTypes** ppEnum)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IPin(EnumMediaTypes(ppEnum));
		return hr;
	}
	static HRESULT QueryInternalConnections(IPin* pThis, IPin** ppPin, ULONG* nPin)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IPin(QueryInternalConnections(ppPin, nPin));
		return hr;
	}
	static HRESULT EndOfStream(IPin* pThis)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IPin(EndOfStream());
		return hr;
	}
	static HRESULT BeginFlush(IPin* pThis)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IPin(BeginFlush());
		return hr;
	}
	static HRESULT EndFlush(IPin* pThis)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IPin(EndFlush());
		return hr;
	}
	static HRESULT NewSegment(IPin* pThis, REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IPin(NewSegment(tStart, tStop, dRate));
		return hr;
	}
};

#undef GUARDED_CALL_IPin

/////////////////////////////////////////////////////////////////////////////
// Guarded calls for methods of IMemInputPin interface

#ifndef TRACELEVEL_SEH_IMemInputPin
	#define TRACELEVEL_SEH_IMemInputPin (TRACELEVEL_SEH)
#endif

#define GUARDED_CALL_IMemInputPin(fun) \
	GUARDED_CALL(TRACELEVEL_SEH_IMemInputPin, _T("SEH_IMemInputPin::"), fun)

class SEH_IMemInputPin
{
public:
	static HRESULT GetAllocator(IMemInputPin* pThis, IMemAllocator** ppAllocator)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMemInputPin(GetAllocator(ppAllocator));
		return hr;
	}
	static HRESULT NotifyAllocator(IMemInputPin* pThis, IMemAllocator* pAllocator, BOOL bReadOnly)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMemInputPin(NotifyAllocator(pAllocator, bReadOnly));
		return hr;
	}
	static HRESULT GetAllocatorRequirements(IMemInputPin* pThis, ALLOCATOR_PROPERTIES* pProps)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMemInputPin(GetAllocatorRequirements(pProps));
		return hr;
	}
	static HRESULT Receive(IMemInputPin* pThis, IMediaSample* pSample)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMemInputPin(Receive(pSample));
		return hr;
	}
	static HRESULT ReceiveMultiple(IMemInputPin* pThis,
		IMediaSample** pSamples, long nSamples, long* nSamplesProcessed)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMemInputPin(ReceiveMultiple(pSamples, nSamples, nSamplesProcessed));
		return hr;
	}
	static HRESULT ReceiveCanBlock(IMemInputPin* pThis)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMemInputPin(ReceiveCanBlock());
		return hr;
	}
};

#undef GUARDED_CALL_IMemInputPin

/////////////////////////////////////////////////////////////////////////////
// Guarded calls for methods of IBaseFilter/IMediaFilter interfaces

#ifndef TRACELEVEL_SEH_IBaseFilter
	#define TRACELEVEL_SEH_IBaseFilter	(TRACELEVEL_SEH)
#endif

#define GUARDED_CALL_IBaseFilter(fun) \
	GUARDED_CALL(TRACELEVEL_SEH_IBaseFilter, _T("SEH_IBaseFilter::"), fun)

class SEH_IBaseFilter
{
public :

	//---------------------------------
	// IMediaFilter
	static HRESULT Stop(IMediaFilter* pThis)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IBaseFilter(Stop());
		return hr;
	}
	static HRESULT Pause(IMediaFilter* pThis)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IBaseFilter(Pause());
		return hr;
	}
	static HRESULT Run(IMediaFilter* pThis, REFERENCE_TIME tStart)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IBaseFilter(Run(tStart));
		return hr;
	}
	static HRESULT GetState(IMediaFilter* pThis, DWORD dwMilliSecsTimeout, FILTER_STATE* State)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IBaseFilter(GetState(dwMilliSecsTimeout, State));
		return hr;
	}
	static HRESULT SetSyncSource(IMediaFilter* pThis, IReferenceClock* pClock)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IBaseFilter(SetSyncSource(pClock));
		return hr;
	}
	static HRESULT GetSyncSource(IMediaFilter* pThis, IReferenceClock** ppClock)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IBaseFilter(GetSyncSource(ppClock));
		return hr;
	}

	//---------------------------------
	// IBaseFilter
	static HRESULT EnumPins(IBaseFilter* pThis, IEnumPins** ppEnum)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IBaseFilter(EnumPins(ppEnum));
		return hr;
	}
	static HRESULT FindPin(IBaseFilter* pThis, LPCWSTR Id, IPin** ppPin)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IBaseFilter(FindPin(Id, ppPin));
		return hr;
	}
	static HRESULT QueryFilterInfo(IBaseFilter* pThis, FILTER_INFO* pInfo)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IBaseFilter(QueryFilterInfo(pInfo));
		return hr;
	}
	static HRESULT JoinFilterGraph(IBaseFilter* pThis, IFilterGraph* pGraph, LPCWSTR pName)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IBaseFilter(JoinFilterGraph(pGraph, pName));
		return hr;
	}
	static HRESULT QueryVendorInfo(IBaseFilter* pThis, LPWSTR* pVendorInfo)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IBaseFilter(QueryVendorInfo(pVendorInfo));
		return hr;
	}
};

#undef GUARDED_CALL_IBaseFilter

/////////////////////////////////////////////////////////////////////////////
// Guarded calls for methods of IMemAllocator interface

#ifndef TRACELEVEL_SEH_IMemAllocator
	#define TRACELEVEL_SEH_IMemAllocator (TRACELEVEL_SEH)
#endif

#define GUARDED_CALL_IMemAllocator(fun) \
	GUARDED_CALL(TRACELEVEL_SEH_IMemAllocator, _T("SEH_IMemAllocator::"), fun)

class SEH_IMemAllocator
{
public:
	static HRESULT SetProperties(IMemAllocator* pThis, ALLOCATOR_PROPERTIES* pRequest,
		ALLOCATOR_PROPERTIES* pActual)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMemAllocator(SetProperties(pRequest, pActual));
		return hr;
	}
	static HRESULT GetProperties(IMemAllocator* pThis, ALLOCATOR_PROPERTIES* pProps)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMemAllocator(GetProperties(pProps));
		return hr;
	}
	static HRESULT Commit(IMemAllocator* pThis)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMemAllocator(Commit());
		return hr;
	}
	static HRESULT Decommit(IMemAllocator* pThis)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMemAllocator(Decommit());
		return hr;
	}
	static HRESULT GetBuffer(IMemAllocator* pThis, IMediaSample** ppBuffer,
		REFERENCE_TIME* pStartTime, REFERENCE_TIME* pEndTime, DWORD dwFlags)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMemAllocator(GetBuffer(ppBuffer, pStartTime, pEndTime, dwFlags));
		return hr;
	}
	static HRESULT ReleaseBuffer(IMemAllocator* pThis, IMediaSample* pBuffer)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMemAllocator(ReleaseBuffer(pBuffer));
		return hr;
	}
};

#undef GUARDED_CALL_IMemAllocator

/////////////////////////////////////////////////////////////////////////////
// Guarded calls for methods of IGraphBuilder/IFilterGraph interfaces

#ifndef TRACELEVEL_SEH_IGraphBuilder
	#define TRACELEVEL_SEH_IGraphBuilder	(TRACELEVEL_SEH)
#endif

#define GUARDED_CALL_IGraphBuilder(fun)	\
	GUARDED_CALL(TRACELEVEL_SEH_IGraphBuilder, _T("SEH_IGraphBuilder::"), fun)

class SEH_IGraphBuilder
{
public :

	//---------------------------------
    // IFilterGraph
	static HRESULT AddFilter(IFilterGraph* pThis, IBaseFilter* pFilter, LPCWSTR pName)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IGraphBuilder(AddFilter(pFilter, pName));
		return hr;
	}
	static HRESULT RemoveFilter(IFilterGraph* pThis, IBaseFilter* pFilter)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IGraphBuilder(RemoveFilter(pFilter));
		return hr;
	}
	static HRESULT EnumFilters(IFilterGraph* pThis, IEnumFilters** ppEnum)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IGraphBuilder(EnumFilters(ppEnum));
		return hr;
	}
	static HRESULT FindFilterByName(IFilterGraph* pThis, LPCWSTR pName, IBaseFilter** ppFilter)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IGraphBuilder(FindFilterByName(pName, ppFilter));
		return hr;
	}
	static HRESULT ConnectDirect(IFilterGraph* pThis, IPin* pPinOut, IPin* pPinIn, const AM_MEDIA_TYPE* pmt)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IGraphBuilder(ConnectDirect(pPinOut, pPinIn, pmt));
		return hr;
	}
	static HRESULT Reconnect(IFilterGraph* pThis, IPin* pPin)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IGraphBuilder(Reconnect(pPin));
		return hr;
	}
	static HRESULT Disconnect(IFilterGraph* pThis, IPin* pPin)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IGraphBuilder(Disconnect(pPin));
		return hr;
	}
	static HRESULT SetDefaultSyncSource(IFilterGraph* pThis)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IGraphBuilder(SetDefaultSyncSource());
		return hr;
	}

	//---------------------------------
	// IGraphBuilder
	static HRESULT Connect(IGraphBuilder* pThis, IPin* pPinOut, IPin* pPinIn)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IGraphBuilder(Connect(pPinOut, pPinIn));
		return hr;
	}
	static HRESULT Render(IGraphBuilder* pThis, IPin* pPinOut)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IGraphBuilder(Render(pPinOut));
		return hr;
	}
	static HRESULT RenderFile(IGraphBuilder* pThis, LPCWSTR lpcwstrFile, LPCWSTR lpcwstrPlayList)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IGraphBuilder(RenderFile(lpcwstrFile, lpcwstrPlayList));
		return hr;
	}
	static HRESULT AddSourceFilter( IGraphBuilder* pThis, LPCWSTR lpcwstrFileName,
									LPCWSTR lpcwstrFilterName, IBaseFilter** ppFilter)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IGraphBuilder(AddSourceFilter(lpcwstrFileName,lpcwstrFilterName,ppFilter));
		return hr;
	}
	static HRESULT SetLogFile(IGraphBuilder* pThis, DWORD_PTR hFile)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IGraphBuilder(SetLogFile(hFile));
		return hr;
	}

	static HRESULT Abort(IGraphBuilder* pThis)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IGraphBuilder(Abort());
		return hr;
	}
	static HRESULT ShouldOperationContinue(IGraphBuilder* pThis)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IGraphBuilder(ShouldOperationContinue());
		return hr;
	}
};

#undef GUARDED_CALL_IGraphBuilder

/////////////////////////////////////////////////////////////////////////////
// Guarded calls for methods of IPersistStream interface

#ifndef TRACELEVEL_SEH_IPersistStream
	#define TRACELEVEL_SEH_IPersistStream	(TRACELEVEL_SEH)
#endif

#define GUARDED_CALL_IPersistStream(fun) \
	GUARDED_CALL(TRACELEVEL_SEH_IPersistStream, _T("SEH_IPersistStream::"), fun)

class SEH_IPersistStream
{
public :
	static HRESULT IsDirty(IPersistStream* pThis)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IPersistStream(IsDirty());
		return hr;
	}
	static HRESULT Load(IPersistStream* pThis, IStream* pStm)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IPersistStream(Load(pStm));
		return hr;
	}
	static HRESULT Save(IPersistStream* pThis, IStream* pStm, BOOL fClearDirty)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IPersistStream(Save(pStm, fClearDirty));
		return hr;
	}
	static HRESULT GetSizeMax(IPersistStream* pThis, ULARGE_INTEGER* pcbSize)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IPersistStream(GetSizeMax(pcbSize));
		return hr;
	}
};

#undef GUARDED_CALL_IPersistStream

/////////////////////////////////////////////////////////////////////////////
// Guarded calls for methods of IPropertyBag interface

#ifndef TRACELEVEL_SEH_IPropertyBag
	#define TRACELEVEL_SEH_IPropertyBag	(TRACELEVEL_SEH)
#endif

#define GUARDED_CALL_IPropertyBag(fun) \
	GUARDED_CALL(TRACELEVEL_SEH_IPropertyBag, _T("SEH_IPropertyBag::"), fun)

class SEH_IPropertyBag
{
public :
	static HRESULT Read(IPropertyBag* pThis, LPCOLESTR pszPropName, VARIANT* pVar, IErrorLog* pErrorLog)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IPropertyBag(Read(pszPropName, pVar, pErrorLog));
		return hr;
	}
	static HRESULT Write(IPropertyBag* pThis, LPCOLESTR pszPropName, VARIANT* pVar)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IPropertyBag(Write(pszPropName, pVar));
		return hr;
	}
};

#undef GUARDED_CALL_IPropertyBag

/////////////////////////////////////////////////////////////////////////////
// Guarded calls for methods of IEnumPins interface

#ifndef TRACELEVEL_SEH_IEnumPins
	#define TRACELEVEL_SEH_IEnumPins	(TRACELEVEL_SEH)
#endif

#define GUARDED_CALL_IEnumPins(fun) \
	GUARDED_CALL(TRACELEVEL_SEH_IEnumPins, _T("SEH_IEnumPins::"), fun)

class SEH_IEnumPins
{
public :
	static HRESULT Next(IEnumPins* pThis, ULONG cPins, IPin** ppPins, ULONG* pcFetched)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IEnumPins(Next(cPins, ppPins, pcFetched));
		return hr;
	}
	static HRESULT Skip(IEnumPins* pThis, ULONG cPins)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IEnumPins(Skip(cPins));
		return hr;
	}
	static HRESULT Reset(IEnumPins* pThis)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IEnumPins(Reset());
		return hr;
	}
	static HRESULT Clone(IEnumPins* pThis, IEnumPins** ppEnum)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IEnumPins(Clone(ppEnum));
		return hr;
	}
};

#undef GUARDED_CALL_IEnumPins

/////////////////////////////////////////////////////////////////////////////
// Guarded calls for methods of IEnumMediaTypes interface

#ifndef TRACELEVEL_SEH_IEnumMediaTypes
	#define TRACELEVEL_SEH_IEnumMediaTypes	(TRACELEVEL_SEH)
#endif

#define GUARDED_CALL_IEnumMediaTypes(fun) \
	GUARDED_CALL(TRACELEVEL_SEH_IEnumMediaTypes, _T("SEH_IEnumMediaTypes::"), fun)

class SEH_IEnumMediaTypes
{
public :
	static HRESULT Next(IEnumMediaTypes* pThis, ULONG cMediaTypes, AM_MEDIA_TYPE** ppMediaTypes,
		ULONG* pcFetched)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IEnumMediaTypes(Next(cMediaTypes, ppMediaTypes, pcFetched));
		return hr;
	}
	static HRESULT Skip(IEnumMediaTypes* pThis, ULONG cMediaTypes)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IEnumMediaTypes(Skip(cMediaTypes));
		return hr;
	}
	static HRESULT Reset(IEnumMediaTypes* pThis)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IEnumMediaTypes(Reset());
		return hr;
	}
	static HRESULT Clone(IEnumMediaTypes* pThis, IEnumMediaTypes** ppEnum)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IEnumMediaTypes(Clone(ppEnum));
		return hr;
	}
};

#undef GUARDED_CALL_IEnumMediaTypes

/////////////////////////////////////////////////////////////////////////////
// Guarded calls for methods of IEnumFilters interface

#ifndef TRACELEVEL_SEH_IEnumFilters
	#define TRACELEVEL_SEH_IEnumFilters	(TRACELEVEL_SEH)
#endif

#define GUARDED_CALL_IEnumFilters(fun) \
	GUARDED_CALL(TRACELEVEL_SEH_IEnumFilters, _T("SEH_IEnumFilters::"), fun)

class SEH_IEnumFilters
{
public :
	static HRESULT Next(IEnumFilters* pThis, ULONG cFilters, IBaseFilter** ppFilter, ULONG* pcFetched)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IEnumFilters(Next(cFilters, ppFilter, pcFetched));
		return hr;
	}
	static HRESULT Skip(IEnumFilters* pThis, ULONG cFilters)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IEnumFilters(Skip(cFilters));
		return hr;
	}
	static HRESULT Reset(IEnumFilters* pThis)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IEnumFilters(Reset());
		return hr;
	}
	static HRESULT Clone(IEnumFilters* pThis, IEnumFilters** ppEnum)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IEnumFilters(Clone(ppEnum));
		return hr;
	}
};

#undef GUARDED_CALL_IEnumFilters

/////////////////////////////////////////////////////////////////////////////
// Guarded calls for methods of IKsPropertySet interface

#ifndef TRACELEVEL_SEH_IKsPropertySet
	#define TRACELEVEL_SEH_IKsPropertySet	(TRACELEVEL_SEH)
#endif

#define GUARDED_CALL_IKsPropertySet(fun) \
	GUARDED_CALL(TRACELEVEL_SEH_IKsPropertySet, _T("SEH_IKsPropertySet::"), fun)

class SEH_IKsPropertySet
{
public :
	static HRESULT Set(IKsPropertySet* pThis, REFGUID guidPropSet, DWORD dwPropID,
					   LPVOID pInstData, DWORD cbInstData, LPVOID pPropData, DWORD cbPropData)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IKsPropertySet(Set(guidPropSet,dwPropID,pInstData,cbInstData,pPropData,cbPropData));
		return hr;
	}
	static HRESULT Get(IKsPropertySet* pThis, REFGUID guidPropSet, DWORD dwPropID,
					   LPVOID pInstData, DWORD cbInstData, LPVOID pPropData, DWORD cbPropData,
					   DWORD* pcbReturned)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IKsPropertySet(Get(guidPropSet,dwPropID,pInstData,cbInstData,pPropData,cbPropData,pcbReturned));
		return hr;
	}
	static HRESULT QuerySupported(IKsPropertySet* pThis, REFGUID guidPropSet, DWORD dwPropID,
								  DWORD* pTypeSupport)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IKsPropertySet(QuerySupported(guidPropSet,dwPropID,pTypeSupport));
		return hr;
	}
};

#undef GUARDED_CALL_IKsPropertySet

/////////////////////////////////////////////////////////////////////////////
// Guarded calls for methods of IDvdControl2 interface

#ifndef TRACELEVEL_SEH_IDvdControl2
	#define TRACELEVEL_SEH_IDvdControl2 (TRACELEVEL_SEH)
#endif

#define GUARDED_CALL_IDvdControl2(fun) \
	GUARDED_CALL(TRACELEVEL_SEH_IDvdControl2, _T("SEH_IDvdControl2::"), fun)

class SEH_IDvdControl2
{
public:
	static HRESULT PlayTitle(IDvdControl2* pThis, ULONG ulTitle, DWORD dwFlags, IDvdCmd **ppCmd)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( PlayTitle(ulTitle, dwFlags, ppCmd) );
		return hr;
	}
	static HRESULT PlayChapterInTitle(IDvdControl2* pThis, ULONG ulTitle, ULONG ulChapter,
		DWORD dwFlags, IDvdCmd** ppCmd)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( PlayChapterInTitle(ulTitle, ulChapter, dwFlags, ppCmd) );
		return hr;
	}
	static HRESULT PlayAtTimeInTitle(IDvdControl2* pThis, ULONG ulTitle, DVD_HMSF_TIMECODE* pStartTime,
		DWORD dwFlags, IDvdCmd** ppCmd)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( PlayAtTimeInTitle(ulTitle, pStartTime, dwFlags, ppCmd) );
		return hr;
	}
	static HRESULT Stop(IDvdControl2* pThis)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( Stop() );
		return hr;
	}
	static HRESULT ReturnFromSubmenu(IDvdControl2* pThis, DWORD dwFlags, IDvdCmd** ppCmd)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( ReturnFromSubmenu(dwFlags, ppCmd) );
		return hr;
	}
	static HRESULT PlayAtTime(IDvdControl2* pThis, DVD_HMSF_TIMECODE* pTime, DWORD dwFlags, IDvdCmd** ppCmd)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( PlayAtTime(pTime, dwFlags, ppCmd) );
		return hr;
	}
	static HRESULT PlayChapter(IDvdControl2* pThis, ULONG ulChapter, DWORD dwFlags, IDvdCmd** ppCmd)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( PlayChapter(ulChapter, dwFlags, ppCmd) );
		return hr;
	}
	static HRESULT PlayPrevChapter(IDvdControl2* pThis, DWORD dwFlags, IDvdCmd** ppCmd)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( PlayPrevChapter(dwFlags, ppCmd) );
		return hr;
	}
	static HRESULT ReplayChapter(IDvdControl2* pThis, DWORD dwFlags, IDvdCmd** ppCmd)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( ReplayChapter(dwFlags, ppCmd) );
		return hr;
	}
	static HRESULT PlayNextChapter(IDvdControl2* pThis, DWORD dwFlags, IDvdCmd** ppCmd)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( PlayNextChapter(dwFlags, ppCmd) );
		return hr;
	}
	static HRESULT PlayForwards(IDvdControl2* pThis, double dSpeed, DWORD dwFlags, IDvdCmd** ppCmd)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( PlayForwards(dSpeed, dwFlags, ppCmd) );
		return hr;
	}
	static HRESULT PlayBackwards(IDvdControl2* pThis, double dSpeed, DWORD dwFlags, IDvdCmd** ppCmd)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( PlayBackwards(dSpeed, dwFlags, ppCmd) );
		return hr;
	}
	static HRESULT ShowMenu(IDvdControl2* pThis, DVD_MENU_ID MenuID, DWORD dwFlags, IDvdCmd** ppCmd)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( ShowMenu(MenuID, dwFlags, ppCmd) );
		return hr;
	}
	static HRESULT Resume(IDvdControl2* pThis, DWORD dwFlags, IDvdCmd** ppCmd)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( Resume(dwFlags, ppCmd) );
		return hr;
	}
	static HRESULT SelectRelativeButton(IDvdControl2* pThis, DVD_RELATIVE_BUTTON buttonDir)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( SelectRelativeButton(buttonDir) );
		return hr;
	}
	static HRESULT ActivateButton(IDvdControl2* pThis)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( ActivateButton() );
		return hr;
	}
	static HRESULT SelectButton(IDvdControl2* pThis, ULONG ulButton)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( SelectButton(ulButton) );
		return hr;
	}
	static HRESULT SelectAndActivateButton(IDvdControl2* pThis, ULONG ulButton)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( SelectAndActivateButton(ulButton) );
		return hr;
	}
	static HRESULT StillOff(IDvdControl2* pThis)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( StillOff() );
		return hr;
	}
	static HRESULT Pause(IDvdControl2* pThis, BOOL bState)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( Pause(bState) );
		return hr;
	}
	static HRESULT SelectAudioStream(IDvdControl2* pThis, ULONG ulAudio, DWORD dwFlags,
		IDvdCmd** ppCmd)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( SelectAudioStream(ulAudio, dwFlags, ppCmd) );
		return hr;
	}
	static HRESULT SelectSubpictureStream(IDvdControl2* pThis, ULONG ulSubPicture, DWORD dwFlags,
		IDvdCmd** ppCmd)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( SelectSubpictureStream(ulSubPicture, dwFlags, ppCmd) );
		return hr;
	}
	static HRESULT SetSubpictureState(IDvdControl2* pThis, BOOL bState, DWORD dwFlags, IDvdCmd** ppCmd)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( SetSubpictureState(bState, dwFlags, ppCmd) );
		return hr;
	}
	static HRESULT SelectAngle(IDvdControl2* pThis, ULONG ulAngle, DWORD dwFlags, IDvdCmd** ppCmd)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( SelectAngle(ulAngle, dwFlags, ppCmd) );
		return hr;
	}
	static HRESULT SelectParentalLevel(IDvdControl2* pThis, ULONG ulParentalLevel)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( SelectParentalLevel(ulParentalLevel) );
		return hr;
	}
	static HRESULT SelectParentalCountry(IDvdControl2* pThis, BYTE bCountry[2])
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( SelectParentalCountry(bCountry ) );
		return hr;
	}
	static HRESULT SelectKaraokeAudioPresentationMode(IDvdControl2* pThis, ULONG ulMode)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( SelectKaraokeAudioPresentationMode(ulMode) );
		return hr;
	}
	static HRESULT SelectVideoModePreference(IDvdControl2* pThis, ULONG ulPreferredDisplayMode)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( SelectVideoModePreference(ulPreferredDisplayMode) );
		return hr;
	}
	static HRESULT SetDVDDirectory(IDvdControl2* pThis, LPCWSTR pszwPath)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( SetDVDDirectory(pszwPath) );
		return hr;
	}
	static HRESULT ActivateAtPosition(IDvdControl2* pThis, POINT point)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( ActivateAtPosition(point) );
		return hr;
	}
	static HRESULT SelectAtPosition(IDvdControl2* pThis, POINT point)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( SelectAtPosition(point) );
		return hr;
	}
	static HRESULT PlayChaptersAutoStop(IDvdControl2* pThis, ULONG ulTitle, ULONG ulChapter,
		ULONG ulChaptersToPlay, DWORD dwFlags, IDvdCmd** ppCmd)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( PlayChaptersAutoStop(ulTitle, ulChapter, ulChaptersToPlay, dwFlags, ppCmd) );
		return hr;
	}
	static HRESULT AcceptParentalLevelChange(IDvdControl2* pThis, BOOL bAccept)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( AcceptParentalLevelChange(bAccept) );
		return hr;
	}
	static HRESULT SetOption(IDvdControl2* pThis, DVD_OPTION_FLAG flag, BOOL fState)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( SetOption(flag, fState) );
		return hr;
	}
	static HRESULT SetState(IDvdControl2* pThis, IDvdState* pState, DWORD dwFlags, IDvdCmd** ppCmd)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( SetState(pState, dwFlags, ppCmd) );
		return hr;
	}
	static HRESULT PlayPeriodInTitleAutoStop(IDvdControl2* pThis, ULONG ulTitle,
		DVD_HMSF_TIMECODE* pStartTime, DVD_HMSF_TIMECODE* pEndTime, DWORD dwFlags, IDvdCmd** ppCmd)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( PlayPeriodInTitleAutoStop(ulTitle, pStartTime, pEndTime, dwFlags, ppCmd) );
		return hr;
	}
	static HRESULT SetGPRM(IDvdControl2* pThis, ULONG ulIndex, WORD wValue, DWORD dwFlags,
		IDvdCmd** ppCmd)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( SetGPRM(ulIndex, wValue, dwFlags, ppCmd) );
		return hr;
	}
	static HRESULT SelectDefaultMenuLanguage(IDvdControl2* pThis, LCID Language)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( SelectDefaultMenuLanguage(Language) );
		return hr;
	}
	static HRESULT SelectDefaultAudioLanguage(IDvdControl2* pThis, LCID Language,
		DVD_AUDIO_LANG_EXT audioExtension)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( SelectDefaultAudioLanguage(Language, audioExtension) );
		return hr;
	}
	static HRESULT SelectDefaultSubpictureLanguage(IDvdControl2* pThis, LCID Language,
		DVD_SUBPICTURE_LANG_EXT subpictureExtension) 
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdControl2( SelectDefaultSubpictureLanguage(Language, subpictureExtension) );
		return hr;
	}
};

#undef GUARDED_CALL_IDvdControl2

/////////////////////////////////////////////////////////////////////////////
// Guarded calls for methods of IDvdInfo2 interface

#ifndef TRACELEVEL_SEH_IDvdInfo2
	#define TRACELEVEL_SEH_IDvdInfo2 (TRACELEVEL_SEH)
#endif

#define GUARDED_CALL_IDvdInfo2(fun) \
	GUARDED_CALL(TRACELEVEL_SEH_IDvdInfo2, _T("SEH_IDvdInfo2::"), fun)

class SEH_IDvdInfo2
{
public:
	static HRESULT GetCurrentDomain(IDvdInfo2* pThis, DVD_DOMAIN* pDomain)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetCurrentDomain(pDomain) );
		return hr;
	}
	static HRESULT GetCurrentLocation(IDvdInfo2* pThis, DVD_PLAYBACK_LOCATION2* pLocation)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetCurrentLocation(pLocation) );
		return hr;
	}
	static HRESULT GetTotalTitleTime(IDvdInfo2* pThis, DVD_HMSF_TIMECODE* pTotalTime, ULONG* ulTimeCodeFlags)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetTotalTitleTime(pTotalTime, ulTimeCodeFlags) );
		return hr;
	}
	static HRESULT GetCurrentButton(IDvdInfo2* pThis, ULONG* pulButtonsAvailable, ULONG* pulCurrentButton)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetCurrentButton(pulButtonsAvailable, pulCurrentButton) );
		return hr;
	}
	static HRESULT GetCurrentAngle(IDvdInfo2* pThis, ULONG* pulAnglesAvailable, ULONG* pulCurrentAngle)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetCurrentAngle(pulAnglesAvailable, pulCurrentAngle) );
		return hr;
	}
	static HRESULT GetCurrentAudio(IDvdInfo2* pThis, ULONG* pulStreamsAvailable, ULONG* pulCurrentStream)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetCurrentAudio(pulStreamsAvailable, pulCurrentStream) );
		return hr;
	}
	static HRESULT GetCurrentSubpicture(IDvdInfo2* pThis, ULONG* pulStreamsAvailable,
		ULONG* pulCurrentStream, BOOL* pbIsDisabled)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetCurrentSubpicture(pulStreamsAvailable, pulCurrentStream, pbIsDisabled) );
		return hr;
	}
	static HRESULT GetCurrentUOPS(IDvdInfo2* pThis, ULONG* pulUOPs) 
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetCurrentUOPS(pulUOPs) );
		return hr;
	}
	static HRESULT GetAllSPRMs(IDvdInfo2* pThis, SPRMARRAY* pRegisterArray)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetAllSPRMs(pRegisterArray) );
		return hr;
	}
	static HRESULT GetAllGPRMs(IDvdInfo2* pThis, GPRMARRAY* pRegisterArray)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetAllGPRMs(pRegisterArray) );
		return hr;
	}
	static HRESULT GetAudioLanguage(IDvdInfo2* pThis, ULONG ulStream, LCID* pLanguage)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetAudioLanguage(ulStream, pLanguage) );
		return hr;
	}
	static HRESULT GetSubpictureLanguage(IDvdInfo2* pThis, ULONG ulStream, LCID* pLanguage)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetSubpictureLanguage(ulStream, pLanguage) );
		return hr;
	}
	static HRESULT GetTitleAttributes(IDvdInfo2* pThis, ULONG ulTitle, DVD_MenuAttributes* pMenu,
		DVD_TitleAttributes* pTitle)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetTitleAttributes(ulTitle, pMenu, pTitle) );
		return hr;
	}
	static HRESULT GetVMGAttributes(IDvdInfo2* pThis, DVD_MenuAttributes* pATR)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetVMGAttributes(pATR) );
		return hr;
	}
	static HRESULT GetCurrentVideoAttributes(IDvdInfo2* pThis, DVD_VideoAttributes* pATR) 
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetCurrentVideoAttributes(pATR) );
		return hr;
	}
	static HRESULT GetAudioAttributes(IDvdInfo2* pThis, ULONG ulStream, DVD_AudioAttributes* pATR)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetAudioAttributes(ulStream, pATR) );
		return hr;
	}
	static HRESULT GetKaraokeAttributes(IDvdInfo2* pThis, ULONG ulStream, DVD_KaraokeAttributes* pAttributes)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetKaraokeAttributes(ulStream, pAttributes) );
		return hr;
	}
	static HRESULT GetSubpictureAttributes(IDvdInfo2* pThis, ULONG ulStream, DVD_SubpictureAttributes* pATR)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetSubpictureAttributes(ulStream, pATR) );
		return hr;
	}
	static HRESULT GetDVDVolumeInfo(IDvdInfo2* pThis, ULONG* pulNumOfVolumes, ULONG* pulVolume,
		DVD_DISC_SIDE* pSide, ULONG* pulNumOfTitles)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetDVDVolumeInfo(pulNumOfVolumes, pulVolume, pSide, pulNumOfTitles) );
		return hr;
	}
	static HRESULT GetDVDTextNumberOfLanguages(IDvdInfo2* pThis, ULONG* pulNumOfLangs)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetDVDTextNumberOfLanguages(pulNumOfLangs) );
		return hr;
	}
	static HRESULT GetDVDTextLanguageInfo(IDvdInfo2* pThis, ULONG ulLangIndex, ULONG* pulNumOfStrings,
		LCID* pLangCode, enum DVD_TextCharSet* pbCharacterSet)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetDVDTextLanguageInfo(ulLangIndex, pulNumOfStrings, pLangCode, pbCharacterSet) );
		return hr;
	}
	static HRESULT GetDVDTextStringAsNative(IDvdInfo2* pThis, ULONG ulLangIndex, ULONG ulStringIndex,
		BYTE* pbBuffer, ULONG ulMaxBufferSize, ULONG* pulActualSize, enum DVD_TextStringType* pType)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetDVDTextStringAsNative(ulLangIndex, ulStringIndex, pbBuffer, ulMaxBufferSize, pulActualSize, pType) );
		return hr;
	}
	static HRESULT GetDVDTextStringAsUnicode(IDvdInfo2* pThis, ULONG ulLangIndex, ULONG ulStringIndex,
		WCHAR* pchwBuffer, ULONG ulMaxBufferSize, ULONG* pulActualSize, enum DVD_TextStringType* pType)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetDVDTextStringAsUnicode(ulLangIndex, ulStringIndex, pchwBuffer, ulMaxBufferSize, pulActualSize, pType) );
		return hr;
	}
	static HRESULT GetPlayerParentalLevel(IDvdInfo2* pThis, ULONG* pulParentalLevel, BYTE pbCountryCode[2])
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetPlayerParentalLevel(pulParentalLevel, pbCountryCode) );
		return hr;
	}
	static HRESULT GetNumberOfChapters(IDvdInfo2* pThis, ULONG ulTitle, ULONG* pulNumOfChapters)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetNumberOfChapters(ulTitle, pulNumOfChapters) );
		return hr;
	}
	static HRESULT GetTitleParentalLevels(IDvdInfo2* pThis, ULONG ulTitle, ULONG* pulParentalLevels)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetTitleParentalLevels(ulTitle, pulParentalLevels) );
		return hr;
	}
	static HRESULT GetDVDDirectory(IDvdInfo2* pThis, LPWSTR pszwPath, ULONG ulMaxSize, ULONG* pulActualSize)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetDVDDirectory(pszwPath, ulMaxSize, pulActualSize) );
		return hr;
	}
	static HRESULT IsAudioStreamEnabled(IDvdInfo2* pThis, ULONG ulStreamNum, BOOL* pbEnabled)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( IsAudioStreamEnabled(ulStreamNum, pbEnabled) );
		return hr;
	}
	static HRESULT GetDiscID(IDvdInfo2* pThis, LPCWSTR pszwPath, ULONGLONG* pullDiscID)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetDiscID(pszwPath, pullDiscID) );
		return hr;
	}
	static HRESULT GetState(IDvdInfo2* pThis, IDvdState** pStateData)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetState(pStateData) );
		return hr;
	}
	static HRESULT GetMenuLanguages(IDvdInfo2* pThis, LCID* pLanguages, ULONG ulMaxLanguages,
		ULONG* pulActualLanguages)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetMenuLanguages(pLanguages, ulMaxLanguages, pulActualLanguages) );
		return hr;
	}
	static HRESULT GetButtonAtPosition(IDvdInfo2* pThis, POINT point, ULONG* pulButtonIndex)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetButtonAtPosition(point, pulButtonIndex) );
		return hr;
	}
	static HRESULT GetCmdFromEvent(IDvdInfo2* pThis, LONG_PTR lParam1, IDvdCmd** pCmdObj)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetCmdFromEvent(lParam1, pCmdObj) );
		return hr;
	}
	static HRESULT GetDefaultMenuLanguage(IDvdInfo2* pThis, LCID* pLanguage)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetDefaultMenuLanguage(pLanguage) );
		return hr;
	}
	static HRESULT GetDefaultAudioLanguage(IDvdInfo2* pThis, LCID* pLanguage,
		DVD_AUDIO_LANG_EXT* pAudioExtension)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetDefaultAudioLanguage(pLanguage, pAudioExtension) );
		return hr;
	}
	static HRESULT GetDefaultSubpictureLanguage(IDvdInfo2* pThis, LCID* pLanguage,
		DVD_SUBPICTURE_LANG_EXT* pSubpictureExtension)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetDefaultSubpictureLanguage(pLanguage, pSubpictureExtension) );
		return hr;
	}
	static HRESULT GetDecoderCaps(IDvdInfo2* pThis, DVD_DECODER_CAPS* pCaps)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetDecoderCaps(pCaps) );
		return hr;
	}
	static HRESULT GetButtonRect(IDvdInfo2* pThis, ULONG ulButton, RECT* pRect)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( GetButtonRect(ulButton, pRect) );
		return hr;
	}
	static HRESULT IsSubpictureStreamEnabled(IDvdInfo2* pThis, ULONG ulStreamNum, BOOL* pbEnabled)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IDvdInfo2( IsSubpictureStreamEnabled(ulStreamNum, pbEnabled) );
		return hr;
	}
};

#undef GUARDED_CALL_IDvdInfo2

/////////////////////////////////////////////////////////////////////////////
// Guarded calls for methods of ICreateDevEnum interface

#ifndef TRACELEVEL_SEH_ICreateDevEnum
	#define TRACELEVEL_SEH_ICreateDevEnum	(TRACELEVEL_SEH)
#endif

#define GUARDED_CALL_ICreateDevEnum(fun) \
	GUARDED_CALL(TRACELEVEL_SEH_ICreateDevEnum, _T("SEH_ICreateDevEnum::"), fun)

class SEH_ICreateDevEnum
{
public :

	static HRESULT CreateClassEnumerator(ICreateDevEnum* pThis,
		REFCLSID clsidDeviceClass, IEnumMoniker **ppEnumMoniker, DWORD dwFlags)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_ICreateDevEnum(CreateClassEnumerator(clsidDeviceClass, ppEnumMoniker, dwFlags));
		return hr;
	}
};

#undef GUARDED_CALL_ICreateDevEnum

/////////////////////////////////////////////////////////////////////////////
// Guarded calls for methods of IEnumMoniker interface

#ifndef TRACELEVEL_SEH_IEnumMoniker
	#define TRACELEVEL_SEH_IEnumMoniker		(TRACELEVEL_SEH)
#endif

#define GUARDED_CALL_IEnumMoniker(fun) \
	GUARDED_CALL(TRACELEVEL_SEH_IEnumMoniker, _T("SEH_IEnumMoniker::"), fun)

class SEH_IEnumMoniker
{
public :
	static HRESULT Next(IEnumMoniker* pThis, ULONG celt, IMoniker** rgelt, ULONG* pceltFetched)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IEnumMoniker(Next(celt, rgelt, pceltFetched));
		return hr;
	}
	static HRESULT Skip(IEnumMoniker* pThis, ULONG celt)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IEnumMoniker(Skip(celt));
		return hr;
	}
	static HRESULT Reset(IEnumMoniker* pThis)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IEnumMoniker(Reset());
		return hr;
	}
	static HRESULT Clone(IEnumMoniker* pThis, IEnumMoniker** ppEnum)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IEnumMoniker(Clone(ppEnum));
		return hr;
	}
};

#undef GUARDED_CALL_IEnumMoniker

/////////////////////////////////////////////////////////////////////////////
// Guarded calls for methods of IMoniker interface

#ifndef TRACELEVEL_SEH_IMoniker
	#define TRACELEVEL_SEH_IMoniker	(TRACELEVEL_SEH)
#endif

#define GUARDED_CALL_IMoniker(fun) \
	GUARDED_CALL(TRACELEVEL_SEH_IMoniker, _T("SEH_IMoniker::"), fun)

class SEH_IMoniker
{
public :

	static HRESULT BindToObject(IMoniker* pThis,
		IBindCtx* pbc, IMoniker* pmkToLeft, REFIID riidResult, void** ppvResult)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMoniker(BindToObject(pbc, pmkToLeft, riidResult, ppvResult));
		return hr;
	}
	static HRESULT BindToStorage(IMoniker* pThis,
		IBindCtx* pbc, IMoniker* pmkToLeft, REFIID riid, void** ppvObj)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMoniker(BindToStorage(pbc, pmkToLeft, riid, ppvObj));
		return hr;
	}
	static HRESULT Reduce(IMoniker* pThis,
		IBindCtx* pbc, DWORD dwReduceHowFar, IMoniker** ppmkToLeft, IMoniker** ppmkReduced)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMoniker(Reduce(pbc, dwReduceHowFar, ppmkToLeft, ppmkReduced));
		return hr;
	}
	static HRESULT ComposeWith(IMoniker* pThis,
		IMoniker* pmkRight, BOOL fOnlyIfNotGeneric, IMoniker** ppmkComposite)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMoniker(ComposeWith(pmkRight, fOnlyIfNotGeneric, ppmkComposite));
		return hr;
	}
	static HRESULT Enum(IMoniker* pThis, BOOL fForward, IEnumMoniker** ppenumMoniker)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMoniker(Enum(fForward, ppenumMoniker));
		return hr;
	}
	static HRESULT IsEqual(IMoniker* pThis, IMoniker* pmkOtherMoniker)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMoniker(IsEqual(pmkOtherMoniker));
		return hr;
	}
	static HRESULT Hash(IMoniker* pThis, DWORD* pdwHash)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMoniker(Hash(pdwHash));
		return hr;
	}
	static HRESULT IsRunning(IMoniker* pThis,
		IBindCtx* pbc, IMoniker* pmkToLeft, IMoniker* pmkNewlyRunning)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMoniker(IsRunning(pbc, pmkToLeft, pmkNewlyRunning));
		return hr;
	}
	static HRESULT GetTimeOfLastChange(IMoniker* pThis,
		IBindCtx* pbc, IMoniker* pmkToLeft, FILETIME* pFileTime)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMoniker(GetTimeOfLastChange(pbc, pmkToLeft, pFileTime));
		return hr;
	}
	static HRESULT Inverse(IMoniker* pThis, IMoniker** ppmk)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMoniker(Inverse(ppmk));
		return hr;
	}
	static HRESULT CommonPrefixWith(IMoniker* pThis, IMoniker* pmkOther, IMoniker** ppmkPrefix)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMoniker(CommonPrefixWith(pmkOther, ppmkPrefix));
		return hr;
	}
	static HRESULT RelativePathTo(IMoniker* pThis, IMoniker* pmkOther, IMoniker** ppmkRelPath)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMoniker(RelativePathTo(pmkOther, ppmkRelPath));
		return hr;
	}
	static HRESULT GetDisplayName(IMoniker* pThis,
		IBindCtx* pbc, IMoniker* pmkToLeft, LPOLESTR* ppszDisplayName)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMoniker(GetDisplayName(pbc, pmkToLeft, ppszDisplayName));
		return hr;
	}
	static HRESULT ParseDisplayName(IMoniker* pThis,
		IBindCtx* pbc, IMoniker* pmkToLeft, LPOLESTR pszDisplayName, ULONG* pchEaten, IMoniker** ppmkOut)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMoniker(ParseDisplayName(pbc, pmkToLeft, pszDisplayName, pchEaten, ppmkOut));
		return hr;
	}
	static HRESULT IsSystemMoniker(IMoniker* pThis, DWORD* pdwMksys)
	{
		ATLASSERT(pThis != NULL);
		GUARDED_CALL_IMoniker(IsSystemMoniker(pdwMksys));
		return hr;
	}
};

#undef GUARDED_CALL_IMoniker

/////////////////////////////////////////////////////////////////////////////
#endif	// __DSGUARD_H_
