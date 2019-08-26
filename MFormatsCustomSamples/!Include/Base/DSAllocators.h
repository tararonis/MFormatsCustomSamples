//---------------------------------------------------------------------------
// This file is part of the MediaLooks DirectShow Template Library (DSTL)
//
// DSAllocators.h : sample and memory allocator classes
//
//---------------------------------------------------------------------------
// Copyright (c) 2002-2006, MediaLooks Ltd.
// www.medialooks.com (dev@medialooks.com)
//
// Author:  Dmitry Nikitin (DKN)
// Version: 1.3 (2006.12)
//
//---------------------------------------------------------------------------
// CONFIDENTIAL INFORMATION
//
// This file is Intellectual Property (IP) of MediaLooks Ltd. and is
// strictly confidential. You can gain access to this file only if you
// sign a License Agreement and a Non-Disclosure Agreement (NDA) with
// MediaLooks Ltd. If you had not signed any of these documents, please
// contact <dev@medialooks.com> immediately.
//
//---------------------------------------------------------------------------
// $Id: DSAllocators.h,v 1.74 2011/12/26 07:09:09 dkn Exp $
//---------------------------------------------------------------------------

#ifndef __DSALLOCATORS_H_
#define __DSALLOCATORS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DSTypes.h"

/////////////////////////////////////////////////////////////////////////////
// If defined: allocator memory and samples will be freed on IMemAllocator::Decommit
// call or after release last sample
//#define FREE_ON_DECOMMIT

// Fix for Mainconcept bug in IMediaSample2::SetProperties(): media type is set
// in AM_SAMPLE2_PROPERTIES, but AM_SAMPLE_TYPECHANGED flag not set
// If defined: don't check AM_SAMPLE_TYPECHANGED flag and put media type to sample
// if media type pointer != NULL
//#define FIX_Mainconcept_MediaSample2_MediaType

/////////////////////////////////////////////////////////////////////////////
// Forward declaration
class IMediaSample2Wrapper;
class IMediaSample2Redirect;

template <class T> class IMemAllocatorImpl;
class IMemAllocatorRedirect;

class CBufferOnVirtualAlloc;
class CArrayOnVirtualAlloc;
template <class TBuffer = CBufferOnVirtualAlloc> class CAllocatorBuffers;
template <class TSampleObject> class CSampleArray;

// Special class (instead CComObjectRootEx<>) for remome asserts at InternalRelease()
// calls from ffdshow video encoder
#ifdef	NO_FFDSHOW_ASSERT
template <class ThreadModel> class CComObjectRootExNoAss;
#endif

class CMediaSample;
class CMediaSampleCashe;
class CPreviewMediaSample;
template <class TAllocator, class TBase> class CComSampleObject;

template <class T, class TBuffers = CAllocatorBuffers<CBufferOnVirtualAlloc>, class TSample = CMediaSample>
class CMemAllocBase;
template <class T, class TBuffers = CAllocatorBuffers<CBufferOnVirtualAlloc>, class TSample = CMediaSampleCashe>
class CMemAllocTrans;

template <class TMemory> class CMemoryAllocatorT;
typedef CMemoryAllocatorT<CBufferOnVirtualAlloc>	CMemoryAllocator;

// CMemoryAllocator used as default for work. If you want to change it, then:
// input pin class should be derived from
//		public CMemInputPin<CCoMjpegEncoderIn, CSplittedMemoryAllocator>
// (don't forget to rebuild full project!)
// in output pin class CAllocatorMatch::AllocatorCreate() method should be overriden:
//		HRESULT AllocatorCreate(IMemAllocator** ppMemAlloc)
//		{
//			return CSplittedMemoryAllocator::CreateAllocatorInstance(ppMemAlloc);
//		}

typedef CMemoryAllocatorT<CArrayOnVirtualAlloc>		CSplittedMemoryAllocator;

class CTransMemoryAllocator;
class CMemAllocRedirector;
template <class TSample> class CMemAllocSampleRedirector;

class CSampleCopier;
class COverlayMixerCopier;
template <class T> class CSampleT;

/////////////////////////////////////////////////////////////////////////////
#ifndef SMP_PROPS_DEFINED
#define SMP_PROPS_DEFINED

typedef struct tagSmpProps
{
	// Sample flags: AM_SAMPLE_TIMEVALID, AM_SAMPLE_STOPVALID, AM_SAMPLE_MEDIATIMEVALID,
	// AM_SAMPLE_SPLICEPOINT, AM_SAMPLE_PREROLL, AM_SAMPLE_DATADISCONTINUITY
	DWORD			dwFlags;

	// IMediaSample::GetTime(&tS, &tE);
	REFERENCE_TIME	tS;
	REFERENCE_TIME	tE;

	// IMediaSample::GetMediaTime(&llMS, &llME);
	LONGLONG		llMS;
	LONGLONG		llME;
} SMP_PROPS;

#endif	// SMP_PROPS_DEFINED

HRESULT __stdcall GetSampleProps(IMediaSample* pSample, SMP_PROPS* pSmpProps,
								 BYTE** ppBuffer = NULL, LONG* pcbActual = NULL);
HRESULT __stdcall SetSampleProps(IMediaSample* pSample, SMP_PROPS* pSmpProps);

/////////////////////////////////////////////////////////////////////////////
// For redirector classes
#ifndef HOST_CALL_HRESULT

#define HOST_CALL_HRESULT(hrHostDontSet, level, spHost, fun)			\
	HRESULT hr = hrHostDontSet;											\
	if(!spHost)															\
	{																	\
		ATLTRACE2(atlTraceCOM, level, _T("Host don't set\n"));			\
	}																	\
	else																\
	{																	\
		hr = spHost->fun;												\
		ATLTRACE2(atlTraceCOM, level, _T("Host : hr = 0x%X\n"), hr);	\
	}																	\
	return hr

#define HOST_CALL(level, spHost, fun)	HOST_CALL_HRESULT(E_FAIL, level, spHost, fun)

#endif	//HOST_CALL_HRESULT

/////////////////////////////////////////////////////////////////////////////
// Special interface for redirector classes

MIDL_INTERFACE("9039B6A0-2744-48D0-A7ED-7A3344EF9F9A")
IDSXRedirector : public IUnknown
{
public:
	// Exposes original (host) object
	STDMETHOD(Expose)(OUT IUnknown** ppHost) PURE;

	// Wraps up external object
	STDMETHOD(Wrap)(IN REFIID iid, IN IUnknown* pObject, OUT IUnknown** ppWrapper) PURE;
};

/////////////////////////////////////////////////////////////////////////////
// Extention interface for memory allocators
MIDL_INTERFACE("9039B6A1-2744-48D0-A7ED-7A3344EF9F9A")
IMemAllocatorEx : public IUnknown
{
public:
	// Attach new media type for next sample
	STDMETHOD(SetMediaType)(IN const AM_MEDIA_TYPE* pmt) PURE;
};

/////////////////////////////////////////////////////////////////////////////
// Extention interface for memory allocators for transformer filters
//
// Sequence of calls:
//
//	// Upstream filter gets new sample 
//	Upstream filter:  IMemAllocator::GetBuffer(&pSample)
//	{
//		// Our allocator creates new sample and sends notification to host object with it
//		IMemAllocHostCallback::OnGetSample(pSample)
//		{
//			// Commands in this block executed by host object
//
//			// Get new sample from downstream filter
//			IMemAllocator::GetBuffer(&pExternSample);
//
//			// Get media type from downstream sample
//			pExternSample->GetMediaType(&pmt);
//			if(pmt != NULL)
//			{
//				// Conversion of media type
//				pmtNew = ConvertMT(pmt);
//
//				// Set new media type into our sample
//				pSample->SetMediaType(pmtNew);
//			}
//
//			// Put sample from downstream filter to our sample for use in IMemInputPin::Receive()
//			pSample->QueryInterface( IID_IMediaSampleCache )
//			IMediaSampleCache::PutExternSample(pExternSample);
//		}
//		return
//	}

MIDL_INTERFACE("9039B6A2-2744-48D0-A7ED-7A3344EF9F9A")
IMemAllocHostCallback : public IUnknown
{
public:
	// Notification from allocator: IMemAllocator::GetBuffer() was called
	// pSample: new sample object
	// TODO. AM_GBF_NOWAIT
	STDMETHOD(OnGetSample)(IMediaSample* pSample) PURE;
};

//---------------------------------
MIDL_INTERFACE("9039B6A3-2744-48D0-A7ED-7A3344EF9F9A")
IMediaSampleCache : public IUnknown
{
public:
	// Put extern sample
	STDMETHOD(PutExternSample)(IN IMediaSample* pExternSample) PURE;

	// Get extern sample
	STDMETHOD(GetExternSample)(OUT IMediaSample** ppExternSample) PURE;

	// Check extern sample present
	STDMETHOD(HasExternSample)() PURE;
};

//---------------------------------
MIDL_INTERFACE("9039B6A4-2744-48D0-A7ED-7A3344EF9F9A")
IMemAllocTrans : public IUnknown
{
public:
	// Break link with host object
	STDMETHOD(BreakLink)(IN IMemAllocHostCallback* pHost) PURE;
};

/////////////////////////////////////////////////////////////////////////////
// IMediaSample2Wrapper

// This flag internal only (don't defined in tagAM_SAMPLE_PROPERTY_FLAGS)
#ifndef AM_SAMPLE_MEDIATIMEVALID
	#define AM_SAMPLE_MEDIATIMEVALID	0x20
#endif	// AM_SAMPLE_MEDIATIMEVALID

#define AM_SAMPLE_VALIDFLAGMASK		0x1FF

#ifndef TRACELEVEL_IMediaSample2
	#define TRACELEVEL_IMediaSample2	10
#endif

#ifndef TRACELEVEL_IMediaSample_MediaTimeErr
	#define TRACELEVEL_IMediaSample_MediaTimeErr	2
#endif

#ifndef TRACELEVEL_IMediaSample2_High
	#define TRACELEVEL_IMediaSample2_High	(TRACELEVEL_IMediaSample2 - 2)
#endif

#ifndef TRACELEVEL_IMediaSample2_Low
	#define TRACELEVEL_IMediaSample2_Low	(TRACELEVEL_IMediaSample2 + 2)
#endif

//---------------------------------
class ATL_NO_VTABLE IMediaSample2Wrapper : public IMediaSample2
{
protected:

	BYTE*			m_pBuffer;				// Pointer to the complete buffer
	ULONG			m_cbBuffer;				// Size of the buffer
	ULONG			m_cbActual;				// Length of data in this sample
	DWORD			m_dwFlags;				// Sample state flags
	DWORD			m_dwTypeSpecificFlags;	// Media type specific flags
	AM_MEDIA_TYPE*	m_pMediaType;			// Media type, attached to sample
	REFERENCE_TIME	m_tStart;				// Start sample time
	REFERENCE_TIME	m_tEnd;					// End sample time
	LONGLONG		m_llMediaStart;			// Real media start position
	LONGLONG		m_llMediaEnd;			// Real media end position
	DWORD			m_dwStreamId;			// Stream Id

	//---------------------------------

public:
	virtual LPCTSTR GetClassName() const { return _T("IMediaSample2Wrapper"); }

	IMediaSample2Wrapper() : m_pMediaType(NULL) { Clear(); }
	~IMediaSample2Wrapper() { ::MTFree(m_pMediaType); }
	
	void Clear()
	{
 		m_pBuffer = NULL;
 		m_cbBuffer = 0;
 		m_cbActual = 0;
		m_dwFlags = 0;
		m_dwTypeSpecificFlags = 0;

		m_tStart = m_tEnd = (REFERENCE_TIME)0;
		m_llMediaStart = m_llMediaEnd = (LONGLONG)0;
		m_dwStreamId = AM_STREAM_MEDIA;
	}

	void ClearMediaType()
	{
		::MTFree(m_pMediaType);
		m_pMediaType = NULL;
	}

	void SetBuffer(BYTE* pBuffer, ULONG cbBuffer)
	{
		ATLASSERT((m_pBuffer != NULL && m_cbBuffer != 0) || (m_pBuffer == NULL && m_cbBuffer == 0));
		m_pBuffer = pBuffer;
		m_cbBuffer = cbBuffer;
	}

	//---------------------------------
	// IMediaSample

	// Retrieves a read/write pointer to this buffer's memory
	STDMETHOD(GetPointer)(OUT BYTE** ppBuffer)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2_Low,
			_T("%s: IMediaSample::GetPointer(): pBuffer=0x%X\n"), GetClassName(), m_pBuffer);
		ATLASSERT(ppBuffer != NULL);
		if(ppBuffer == NULL) return E_POINTER;

		ATLASSERT(m_pBuffer != NULL);
		*ppBuffer = m_pBuffer;
		return (m_pBuffer != NULL)? S_OK : E_FAIL;
	}

	// Retrieves the size, in bytes, of the buffer data area
	STDMETHOD_(long, GetSize)()
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2_Low,
			_T("%s: IMediaSample::GetSize(): cbBuffer=%u (0x%X)\n"),
			GetClassName(), m_cbBuffer, m_cbBuffer);
		return m_cbBuffer;
	}

	// Retrieves the stream times at which this sample should begin and finish
	STDMETHOD(GetTime)(OUT REFERENCE_TIME* pTimeStart, OUT REFERENCE_TIME* pTimeEnd)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2_Low,
			_T("%s: IMediaSample::GetTime(), tS=%s  tE=%s\n"), GetClassName(),
			(m_dwFlags & AM_SAMPLE_TIMEVALID)? RT2STR(m_tStart) : _T("???"),
			(m_dwFlags & AM_SAMPLE_STOPVALID)? RT2STR(m_tEnd) :
				(m_dwFlags & AM_SAMPLE_TIMEVALID)? RT2STR(m_tStart + (REFERENCE_TIME)1) : _T("???"));
		
		ATLASSERT(pTimeStart != NULL);
		if(pTimeStart == NULL) return E_INVALIDARG;
		ATLASSERT(pTimeEnd != NULL);
		if(pTimeEnd == NULL) return E_INVALIDARG;

		if((m_dwFlags & AM_SAMPLE_STOPVALID) == 0)
		{
			if((m_dwFlags & AM_SAMPLE_TIMEVALID) == 0)
				return VFW_E_SAMPLE_TIME_NOT_SET;
			else
			{
				*pTimeStart = m_tStart;

				// Direct Show 8.0 "BaseClasses\amfilter.cpp" : Make sure old stuff works
				*pTimeEnd = m_tStart + (REFERENCE_TIME)1;
				return VFW_S_NO_STOP_TIME;
			}
		}

		*pTimeStart = m_tStart;
		*pTimeEnd = m_tEnd;
		return S_OK;
	}

	// Sets the stream time when this sample should begin and finish
	//
	// Remarks (DirectX SDK):
	//	"To invalidate the stream times, set pTimeStart and pTimeEnd to NULL.
	//	This will cause the IMediaSample::GetTime method to return VFW_E_SAMPLE_TIME_NOT_SET."
	STDMETHOD(SetTime)(IN REFERENCE_TIME* pTimeStart, IN REFERENCE_TIME* pTimeEnd)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2_Low,
			_T("%s: IMediaSample::SetTime( tS=%s  tE=%s )\n"), GetClassName(),
			(pTimeStart != NULL)? RT2STR(*pTimeStart) : _T("???"),
			(pTimeEnd != NULL)? RT2STR(*pTimeEnd) : _T("???"));

		if(pTimeStart == NULL)
		{
			ATLASSERT(pTimeEnd == NULL);
			m_dwFlags &= ~(AM_SAMPLE_TIMEVALID | AM_SAMPLE_STOPVALID);
			m_tStart = m_tEnd = (REFERENCE_TIME)0;
		}
		else
		{
			if(pTimeEnd == NULL)
			{
				m_dwFlags &= ~AM_SAMPLE_STOPVALID;
				m_tEnd = (REFERENCE_TIME)0;
			}
			else
			{
				// *pTimeEnd can be equal *pTimeStart
				ATLASSERT(*pTimeEnd >= *pTimeStart);
				if(*pTimeEnd < *pTimeStart) return E_INVALIDARG;

				m_tEnd = *pTimeEnd;
				m_dwFlags |= AM_SAMPLE_STOPVALID;
			}

			m_tStart = *pTimeStart;
			m_dwFlags |= AM_SAMPLE_TIMEVALID;
        }
	    return S_OK;
	}
	
	// Determines if the beginning of a sample is a synchronization point. 
	STDMETHOD(IsSyncPoint)()
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2_Low,
			_T("%s: IMediaSample::IsSyncPoint(): %s\n"), GetClassName(),
			(m_dwFlags & AM_SAMPLE_SPLICEPOINT)? _T("TRUE") : _T("FALSE"));

		return (m_dwFlags & AM_SAMPLE_SPLICEPOINT)? S_OK : S_FALSE;
	}

	// Sets or unsets the beginning of this sample is a synchronization point
	STDMETHOD(SetSyncPoint)(IN BOOL bIsSyncPoint)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2,
			_T("%s: IMediaSample::SetSyncPoint( %s )\n"), GetClassName(),
			(bIsSyncPoint)? _T("TRUE") : _T("FALSE"));

		if(bIsSyncPoint)
			m_dwFlags |= AM_SAMPLE_SPLICEPOINT;
		else
			m_dwFlags &= ~AM_SAMPLE_SPLICEPOINT;
		return S_OK;
	}

	// Preroll property. If TRUE, this sample is for preroll only and should not be displayed. 
	STDMETHOD(IsPreroll)()
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2_Low,
			_T("%s: IMediaSample::IsPreroll(): %s\n"), GetClassName(),
			(m_dwFlags & AM_SAMPLE_PREROLL)? _T("TRUE") : _T("FALSE"));

		return (m_dwFlags & AM_SAMPLE_PREROLL)? S_OK : S_FALSE;
	}

	// Sets the preroll property. If TRUE, this sample is for preroll only and should not be displayed
	STDMETHOD(SetPreroll)(IN BOOL bIsPreroll)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2,
			_T("%s: IMediaSample::SetPreroll( %s )\n"), GetClassName(),
			(bIsPreroll)? _T("TRUE") : _T("FALSE"));

		if(bIsPreroll)
			m_dwFlags |= AM_SAMPLE_PREROLL;
		else
			m_dwFlags &= ~AM_SAMPLE_PREROLL;
		return S_OK;
	}

	// Retrieves the data length of the sample
	STDMETHOD_(long, GetActualDataLength)()
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2_Low,
			_T("%s: IMediaSample::GetActualDataLength(): cbActual=%u\n"), GetClassName(), m_cbActual);
		return m_cbActual;
	}

	// Sets the sample's data length
	STDMETHOD(SetActualDataLength)(IN long lLen)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2,
			_T("%s: IMediaSample::SetActualDataLength( %u )\n"), GetClassName(), lLen);

		if(lLen > (long)m_cbBuffer)
		{
			ATLASSERT(!_T("Actual sample's data length greater then buffer size"));
			return VFW_E_BUFFER_OVERFLOW;
		}

		m_cbActual = lLen;
		return S_OK;
	}

	// Retrieves the media type of the IMediaSample object
	// This method allows for limited in-band format changes.
	STDMETHOD(GetMediaType)(OUT AM_MEDIA_TYPE** ppMediaType)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2_Low,
			_T("%s: IMediaSample::GetMediaType()\n"), GetClassName());
		ATLASSERT(ppMediaType != NULL);
		if(ppMediaType == NULL) return E_POINTER;

		// If SetMediaType() or SetProperties() not called yet - return S_FALSE
		if((m_dwFlags & AM_SAMPLE_TYPECHANGED) == 0)
		{
			// See comments to IMediaSample2::SetProperties()
			ATLASSERT(m_pMediaType == NULL);

			*ppMediaType = NULL;
			return S_FALSE;
		}

		ATLASSERT(m_pMediaType != NULL);

		*ppMediaType = ::MTDup(m_pMediaType);
		return (*ppMediaType != NULL)? S_OK : E_OUTOFMEMORY;
	}

	// Sets the media type for the IMediaSample object
	// This method allows for limited in-band format changes.
	STDMETHOD(SetMediaType)(IN AM_MEDIA_TYPE* pMediaType)
	{
		if(pMediaType != NULL)
		{
			ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2_High,
				_T("%s: IMediaSample::SetMediaType()\n"), GetClassName());
			Debug_MediaType(atlTraceGeneral, TRACELEVEL_IMediaSample2_High, pMediaType);
		}
		else if(m_pMediaType != NULL)
		{
			ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2_High,
				_T("%s: IMediaSample::SetMediaType( NULL )\n"), GetClassName());
		}

		// Delete the current media type
		ClearMediaType();
		m_dwFlags &= ~AM_SAMPLE_TYPECHANGED;

		// Mechanism for resetting the format type
		if(pMediaType == NULL)
			return S_OK;

		// Take a copy of the media type
		m_pMediaType = ::MTDup(pMediaType);
		if(m_pMediaType == NULL)
			return E_OUTOFMEMORY;

		m_dwFlags |= AM_SAMPLE_TYPECHANGED;
		return S_OK;
	}

	// Determines if there is discontinuity in the data stream.
	STDMETHOD(IsDiscontinuity)()
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2_Low,
			_T("%s: IMediaSample::IsDiscontinuity(): %s\n"), GetClassName(),
			(m_dwFlags & AM_SAMPLE_DATADISCONTINUITY)? _T("TRUE") : _T("FALSE"));

		return (m_dwFlags & AM_SAMPLE_DATADISCONTINUITY)? S_OK : S_FALSE;
	}
	
	// Sets the discontinuity property
	STDMETHOD(SetDiscontinuity)(IN BOOL bDiscontinuity)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2,
			_T("%s: IMediaSample::SetDiscontinuity( %s )\n"), GetClassName(),
			(bDiscontinuity)? _T("TRUE") : _T("FALSE"));

		if(bDiscontinuity)
			m_dwFlags |= AM_SAMPLE_DATADISCONTINUITY;
		else
			m_dwFlags &= ~AM_SAMPLE_DATADISCONTINUITY;
		return S_OK;
	}

	// Retrieves the media time stamps for this sample
	STDMETHOD(GetMediaTime)(OUT LONGLONG* pTimeStart, OUT LONGLONG* pTimeEnd)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2_Low,
			_T("%s: IMediaSample::GetMediaTime(): mS=%I64i  mE=%I64i\n"), GetClassName(),
			(m_dwFlags & AM_SAMPLE_MEDIATIMEVALID)? m_llMediaStart : (LONGLONG)0,
			(m_dwFlags & AM_SAMPLE_MEDIATIMEVALID)? m_llMediaEnd : (LONGLONG)0);

		ATLASSERT(pTimeStart != NULL);
		if(pTimeStart == NULL) return E_INVALIDARG;
		ATLASSERT(pTimeEnd != NULL);
		if(pTimeEnd == NULL) return E_INVALIDARG;
	
		if((m_dwFlags & AM_SAMPLE_MEDIATIMEVALID) == 0)
		{
			*pTimeStart = (LONGLONG)0;
			*pTimeEnd = (LONGLONG)0;
			return VFW_E_MEDIA_TIME_NOT_SET;
		}

		*pTimeStart = m_llMediaStart;
		*pTimeEnd = m_llMediaEnd;
		return S_OK;
	}
	
	// Sets the media time stamps for this sample
	//
	// Remarks (DirectX SDK):
	//	"To invalidate the media time, set pTimeStart and pTimeEnd to NULL.
	//	This will cause the IMediaSample::GetMediaTime method to return VFW_E_MEDIA_TIME_NOT_SET."
	STDMETHOD(SetMediaTime)(IN LONGLONG* pTimeStart, IN LONGLONG* pTimeEnd)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2_Low,
			_T("%s: IMediaSample::SetMediaTime( mS=%I64i  mE=%I64i )\n"), GetClassName(),
			(pTimeStart != NULL)? *pTimeStart : (LONGLONG)0,
			(pTimeStart != NULL && pTimeEnd != NULL)? *pTimeEnd : (LONGLONG)0);

		if(pTimeStart == NULL)
		{
			ATLASSERT(pTimeEnd == NULL);
			m_dwFlags &= ~AM_SAMPLE_MEDIATIMEVALID;
			m_llMediaStart = m_llMediaEnd = (LONGLONG)0;
		}
		else
		{
			ATLASSERT(pTimeEnd != NULL);
			if(pTimeEnd == NULL)
				return E_INVALIDARG;

			// This assert is arise in case of VBI pin of "AVerMedia, AVerTV WDM Video Capture (7134)":
			// Also MainConcept MPEG-2 Video Decoder mcm2vd.ax (1.0.241.61005) set this bad media times
			// *pTimeEnd == *pTimeStart
//			ATLASSERT(*pTimeEnd > *pTimeStart);
			if(*pTimeEnd == *pTimeStart)
			{
				ATLTRACE2(atlTraceGeneral, TRACELEVEL_IMediaSample_MediaTimeErr, _T("###Err### %s: SetMediaTime(): ")
					_T("*pTimeEnd == *pTimeStart = %s\n"), GetClassName(), N64STR(*pTimeStart));
			}
			else if(*pTimeEnd < *pTimeStart)
			{
				ATLASSERT(!_T("IMediaSample2Wrapper::SetMediaTime(): *pTimeEnd < *pTimeStart"));
				return E_INVALIDARG;
			}

			m_llMediaStart = *pTimeStart;
			m_llMediaEnd = *pTimeEnd;
			m_dwFlags |= AM_SAMPLE_MEDIATIMEVALID;
        }
	    return S_OK;
	}

	//---------------------------------
	// IMediaSample2

	// Retrieves the properties of a media sample.
	//
	// Remarks (DirectX SDK):
	//	"The retrieved data conforms to the format of the AM_SAMPLE2_PROPERTIES structure.
	//	You can retrieve a subset of the sample properties by setting cbProperties to a value
	//	less than the size of the AM_SAMPLE2_PROPERTIES structure.
	//	For efficiency, the pMediaType member returned in AM_SAMPLE2_PROPERTIES is a pointer
	//	to the data stored in the media sample, not a copy of that data. The pointer may
	//	become invalid after the sample is passed to another filter, or after the input pin's
	//	IMemInputPin::Receive method has completed. Also, do not free the pointer or delete
	//	the media type."
	STDMETHOD(GetProperties)(IN DWORD cbProperties, OUT BYTE *pbProperties)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2_Low,
			_T("%s: IMediaSample2::GetProperties( cbProps=%u )\n"), GetClassName(), cbProperties);

		if(cbProperties != 0)
		{
			ATLASSERT(pbProperties != NULL);
			if(pbProperties == NULL) return E_POINTER;

			AM_SAMPLE2_PROPERTIES rgProps;
			rgProps.cbData				= min(cbProperties, sizeof(AM_SAMPLE2_PROPERTIES));
			rgProps.dwTypeSpecificFlags = m_dwTypeSpecificFlags;
			rgProps.dwSampleFlags		= m_dwFlags & ~AM_SAMPLE_MEDIATIMEVALID;
			rgProps.lActual				= m_cbActual;
			rgProps.tStart				= m_tStart;
			rgProps.tStop				= m_tEnd;
			rgProps.dwStreamId			= m_dwStreamId;
			rgProps.pMediaType			= (m_dwFlags & AM_SAMPLE_TYPECHANGED)? m_pMediaType : NULL;
			rgProps.pbBuffer			= m_pBuffer;
			rgProps.cbBuffer			= m_cbBuffer;

			memcpy(pbProperties, &rgProps, rgProps.cbData);
		}
		return S_OK;
	}

	// Sets the properties of a media sample
	//
	// Remarks (DirectX SDK):
	//	"The data contained in pbProperties must conform to the format of the AM_SAMPLE2_PROPERTIES
	//	structure. You can specify a subset of the sample properties by setting cbProperties
	//	to a value less than the size of the AM_SAMPLE2_PROPERTIES structure."
	//
	// ATTENTION. Very bad method! What we should do with current parameters of sample?
	// If pbProperties includes all parameters of sample: trivial case (update all data).
	// If pbProperties includes not all parameters: should we clear rest of parameters or not?
	// For sample, possible next situation:
	//		(AM_SAMPLE2_PROPERTIES::dwSampleFlags & AM_SAMPLE_TYPECHANGED) != 0 and
	//		AM_SAMPLE2_PROPERTIES::pMediaType == NULL and
	//		m_pMediaType isn't empty
	// Should we clear bit AM_SAMPLE_TYPECHANGED in AM_SAMPLE2_PROPERTIES::dwSampleFlags?
	// Should we also to clear old m_pMediaType in this case?
	// Or should we set AM_SAMPLE_TYPECHANGED and use our old m_pMediaType?
	//
	// In DSTL 1.2 we to clear bit AM_SAMPLE_TYPECHANGED in AM_SAMPLE2_PROPERTIES::dwSampleFlags
	// but don't clear old m_pMediaType. This is reason of ASSERT in GetMediaType() method.
	//
	// From DSTL 1.3 we don't clear this bit if sample has m_pMediaType != NULL and
	// we use our old media type.

#define SMP2_SIZE(field)	\
	(FIELD_OFFSET(AM_SAMPLE2_PROPERTIES, field) + sizeof(((AM_SAMPLE2_PROPERTIES*)0)->field))

	STDMETHOD(SetProperties)(IN DWORD cbProperties, IN const BYTE* pbProperties)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2_Low,
			_T("%s IMediaSample2::SetProperties( cbProps=%u, pbProps=0x%X )\n"),
			GetClassName(), cbProperties, pbProperties);

		// If pbProperties don't include dwTypeSpecificFlags : do nothing
		if(cbProperties < SMP2_SIZE(dwTypeSpecificFlags))
			return S_OK;

		//---------------------------------
		ATLASSERT(pbProperties != NULL);
		if(pbProperties == NULL)
			return E_POINTER;

		AM_SAMPLE2_PROPERTIES *pProps = (AM_SAMPLE2_PROPERTIES *)pbProperties;

		//  Don't use more data than is actually there
		if(cbProperties > pProps->cbData)
			cbProperties = pProps->cbData;

		// We only handle IMediaSample2
		if(cbProperties > sizeof(AM_SAMPLE2_PROPERTIES))
			return E_INVALIDARG;

		//---------------------------------
		// Testing input data
		DWORD dwSampleFlags = pProps->dwSampleFlags;

		if(cbProperties >= SMP2_SIZE(dwSampleFlags))
		{
			if(dwSampleFlags & (~AM_SAMPLE_VALIDFLAGMASK | AM_SAMPLE_MEDIATIMEVALID))
				return E_INVALIDARG;
		}

		// Check a flag isn't being set for a property not being provided
		if(cbProperties < SMP2_SIZE(tStop))
		{
			if((dwSampleFlags & AM_SAMPLE_TIMEVALID) != 0 && (m_dwFlags & AM_SAMPLE_STOPVALID) == 0)
				return E_INVALIDARG;
		}

		// Can't set the pointer or sizes
		if(cbProperties >= SMP2_SIZE(pbBuffer))
		{
			if(pProps->pbBuffer != NULL && pProps->pbBuffer != m_pBuffer)
				return E_INVALIDARG;
		}

		if(cbProperties >= SMP2_SIZE(cbBuffer))
		{
			if(pProps->cbBuffer != 0 && pProps->cbBuffer != (long)m_cbBuffer)
				return E_INVALIDARG;
			if(pProps->cbBuffer < pProps->lActual)
				return E_INVALIDARG;
		}

		AM_MEDIA_TYPE* pMediaType = NULL;
		if(cbProperties >= SMP2_SIZE(pMediaType))
		{
#ifdef FIX_Mainconcept_MediaSample2_MediaType
			if(pProps->pMediaType != NULL)
			{
				ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2_High,
					_T("%s IMediaSample2::SetProperties - change media type (corrected for Mainconcept)\n"), GetClassName());

				__try
				{
					pMediaType = ::MTDup(pProps->pMediaType);
					if(pMediaType == NULL)
						return E_OUTOFMEMORY;

					dwSampleFlags |= AM_SAMPLE_TYPECHANGED;
				}
				__except(DS_SEH_HANDLER) \
				{
					ATLTRACE2(atlTraceGeneral, 0, _T("###Err### %s IMediaSample2::SetProperties():")
						_T("MTDup(): Exception 0x%X\n"), GetClassName(), GetExceptionCode());
	
					pMediaType = NULL;
					dwSampleFlags &= ~AM_SAMPLE_TYPECHANGED;
				}
			}
			else
			{
				dwSampleFlags &= ~AM_SAMPLE_TYPECHANGED;

				// Clear our old media type
				::MTFree(m_pMediaType);
				m_pMediaType = NULL;
			}
#else
			if(dwSampleFlags & AM_SAMPLE_TYPECHANGED)
			{
				ATLASSERT(pProps->pMediaType != NULL);
				if(pProps->pMediaType == NULL) return E_POINTER;

				ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2_High,
					_T("%s IMediaSample2::SetProperties - change media type\n"), GetClassName());

				pMediaType = ::MTDup(pProps->pMediaType);
				if(pMediaType == NULL)
					return E_OUTOFMEMORY;
			}
			else
			{
				// Don't use new media type if AM_SAMPLE_TYPECHANGED don't set
				ATLASSERT(pProps->pMediaType == NULL);

				// Clear our old media type
				::MTFree(m_pMediaType);
				m_pMediaType = NULL;
			}
#endif	// FIX_Mainconcept_MediaSample2_MediaType
		}
		else if(dwSampleFlags & AM_SAMPLE_TYPECHANGED)
		{
			// Clear invalid flag if we haven't media type
			if(m_pMediaType == NULL)
				dwSampleFlags &= ~AM_SAMPLE_TYPECHANGED;
		}

		//-------------------------
		// Do assignments
		if(cbProperties >= SMP2_SIZE(dwTypeSpecificFlags))
			m_dwTypeSpecificFlags = pProps->dwTypeSpecificFlags;

		if(cbProperties >= SMP2_SIZE(dwSampleFlags))
			m_dwFlags = dwSampleFlags | (m_dwFlags & AM_SAMPLE_MEDIATIMEVALID);

		if(cbProperties >= SMP2_SIZE(lActual))
			m_cbActual = pProps->lActual;

		if(cbProperties >= SMP2_SIZE(tStart))
			m_tStart = pProps->tStart;

		if(cbProperties >= SMP2_SIZE(tStop))
			m_tEnd = pProps->tStop;

		if(cbProperties >= SMP2_SIZE(dwStreamId))
			m_dwStreamId = pProps->dwStreamId;

		if(pMediaType != NULL)
		{
			// AM_SAMPLE_TYPECHANGED already put to m_dwFlags
			::MTFree(m_pMediaType);
			m_pMediaType = pMediaType;
		}

		DebugMediaSample_SetProperties(cbProperties);
		return S_OK;
	}

	//---------------------------------

#ifndef _DEBUG
	void DebugMediaSample_SetProperties(IN DWORD /*cbProperties*/) {}

#else	//  _DEBUG

	void DebugMediaSample_SetProperties(IN DWORD cbProperties)
	{
		if(cbProperties < SMP2_SIZE(dwTypeSpecificFlags)) return;

		//---------------------------------
		TCHAR szBuf[256] = _T("");
		int cSym = 0;

		if(cbProperties >= SMP2_SIZE(dwTypeSpecificFlags))
			cSym = wsprintf(szBuf, _T("TypeFlags=0x%X"), m_dwTypeSpecificFlags);

		if(cbProperties >= SMP2_SIZE(dwSampleFlags))
			cSym += wsprintf(szBuf + cSym, _T(", SampleFlags=0x%X"), m_dwFlags);

		if(cbProperties >= SMP2_SIZE(lActual))
			cSym += wsprintf(szBuf + cSym, _T(", cbActual=0x%X"), m_cbActual);

		if(cbProperties >= SMP2_SIZE(tStart))
			cSym += wsprintf(szBuf + cSym, _T(", tS=%s"), RT2STR(m_tStart));

		if(cbProperties >= SMP2_SIZE(tStop))
			cSym += wsprintf(szBuf + cSym, _T(", tE=%s"), RT2STR(m_tEnd));

		if(cbProperties >= SMP2_SIZE(dwStreamId))
			cSym += wsprintf(szBuf + cSym, _T(", StreamId=0x%X"), m_dwStreamId);

		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2_Low, _T("Properties: %s\n"), szBuf);

		if(cbProperties >= SMP2_SIZE(pMediaType))
			Debug_MediaType(atlTraceCOM, TRACELEVEL_IMediaSample2_Low, m_pMediaType);
	}
#endif //_DEBUG
};

/////////////////////////////////////////////////////////////////////////////
// IMediaSample2Redirect

#ifndef TRACELEVEL_IMediaSample2Redir
	#define TRACELEVEL_IMediaSample2Redir	10
#endif

class ATL_NO_VTABLE IMediaSample2Redirect : public IMediaSample2
{
//	COM_INTERFACE_ENTRY(IMediaSample)
//	COM_INTERFACE_ENTRY(IMediaSample2)
public:

	virtual LPCTSTR GetClassName() const { return _T("IMediaSample2Redirect"); }

	HRESULT SetMediaSample2Redirect(IUnknown* pHost)
	{
		m_spMediaSampleHost.Release();
		m_spMediaSample2Host.Release();
		if(pHost == NULL)
			return S_OK;

		HRESULT hr = pHost->QueryInterface(__uuidof(IMediaSample), (void**)&m_spMediaSampleHost);
		if(hr == S_OK)
			pHost->QueryInterface(__uuidof(IMediaSample2), (void**)&m_spMediaSample2Host);
		return hr;
	}

	//---------------------------------
	// IMediaSample
	STDMETHOD(GetPointer)(OUT BYTE **ppBuffer)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2Redir,
			_T("%s: IMediaSample::GetPointer\n"), GetClassName());
		HOST_CALL(TRACELEVEL_IMediaSample2Redir, m_spMediaSampleHost, GetPointer(ppBuffer));
	}
	STDMETHOD_(long, GetSize)()
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2Redir,
			_T("%s: IMediaSample::GetSize\n"), GetClassName());
		HOST_CALL_HRESULT(0, TRACELEVEL_IMediaSample2Redir, m_spMediaSampleHost, GetSize());
	}
	STDMETHOD(GetTime)(OUT REFERENCE_TIME *pTimeStart, OUT REFERENCE_TIME *pTimeEnd)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2Redir,
			_T("%s: IMediaSample::GetTime\n"), GetClassName());
		HOST_CALL(TRACELEVEL_IMediaSample2Redir, m_spMediaSampleHost, GetTime(pTimeStart, pTimeEnd));
	}
	STDMETHOD(SetTime)(IN REFERENCE_TIME *pTimeStart, IN REFERENCE_TIME *pTimeEnd)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2Redir,
			_T("%s: IMediaSample::SetTime\n"), GetClassName());
		HOST_CALL(TRACELEVEL_IMediaSample2Redir, m_spMediaSampleHost, SetTime(pTimeStart,pTimeEnd));
	}
	STDMETHOD(IsSyncPoint)()
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2Redir,
			_T("%s: IMediaSample::IsSyncPoint\n"), GetClassName());
		HOST_CALL(TRACELEVEL_IMediaSample2Redir, m_spMediaSampleHost, IsSyncPoint());
	}
	STDMETHOD(SetSyncPoint)(IN BOOL bIsSyncPoint)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2Redir,
			_T("%s: IMediaSample::SetSyncPoint\n"), GetClassName());
		HOST_CALL(TRACELEVEL_IMediaSample2Redir, m_spMediaSampleHost, SetSyncPoint(bIsSyncPoint));
	}
	STDMETHOD(IsPreroll)()
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2Redir,
			_T("%s: IMediaSample::IsPreroll\n"), GetClassName());
		HOST_CALL(TRACELEVEL_IMediaSample2Redir, m_spMediaSampleHost, IsPreroll());
	}
	STDMETHOD(SetPreroll)(IN BOOL bIsPreroll)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2Redir,
			_T("%s: IMediaSample::SetPreroll\n"), GetClassName());
		HOST_CALL(TRACELEVEL_IMediaSample2Redir, m_spMediaSampleHost, SetPreroll(bIsPreroll));
	}
	STDMETHOD_(long, GetActualDataLength)()
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2Redir,
			_T("%s: IMediaSample::GetActualDataLength\n"), GetClassName());
		HOST_CALL_HRESULT(0, TRACELEVEL_IMediaSample2Redir, m_spMediaSampleHost, GetActualDataLength());
	}
	STDMETHOD(SetActualDataLength)(IN long lLen)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2Redir,
			_T("%s: IMediaSample::SetActualDataLength\n"), GetClassName());
		HOST_CALL(TRACELEVEL_IMediaSample2Redir, m_spMediaSampleHost, SetActualDataLength(lLen));
	}
	STDMETHOD(GetMediaType)(OUT AM_MEDIA_TYPE **ppMediaType)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2Redir,
			_T("%s: IMediaSample::GetMediaType\n"), GetClassName());
		HOST_CALL(TRACELEVEL_IMediaSample2Redir, m_spMediaSampleHost, GetMediaType(ppMediaType));
	}
	STDMETHOD(SetMediaType)(IN AM_MEDIA_TYPE *pMediaType)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2Redir,
			_T("%s: IMediaSample::SetMediaType\n"), GetClassName());
		HOST_CALL(TRACELEVEL_IMediaSample2Redir, m_spMediaSampleHost, SetMediaType(pMediaType));
	}
	STDMETHOD(IsDiscontinuity)()
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2Redir,
			_T("%s: IMediaSample::IsDiscontinuity\n"), GetClassName());
		HOST_CALL(TRACELEVEL_IMediaSample2Redir, m_spMediaSampleHost, IsDiscontinuity());
	}
	STDMETHOD(SetDiscontinuity)(IN BOOL bDiscontinuity)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2Redir,
			_T("%s: IMediaSample::SetDiscontinuity\n"), GetClassName());
		HOST_CALL(TRACELEVEL_IMediaSample2Redir, m_spMediaSampleHost, SetDiscontinuity(bDiscontinuity));
	}
	STDMETHOD(GetMediaTime)(OUT LONGLONG *pTimeStart, OUT LONGLONG *pTimeEnd)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2Redir,
			_T("%s: IMediaSample::GetMediaTime\n"), GetClassName());
		HOST_CALL(TRACELEVEL_IMediaSample2Redir, m_spMediaSampleHost, GetMediaTime(pTimeStart,pTimeEnd));
	}
	STDMETHOD(SetMediaTime)(IN LONGLONG *pTimeStart, IN LONGLONG *pTimeEnd)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2Redir,
			_T("%s: IMediaSample::SetMediaTime\n"), GetClassName());
		HOST_CALL(TRACELEVEL_IMediaSample2Redir, m_spMediaSampleHost, SetMediaTime(pTimeStart,pTimeEnd));
	}
        
	//---------------------------------
	// IMediaSample2
	STDMETHOD(GetProperties)(IN DWORD cbProperties, OUT BYTE *pbProperties)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2Redir,
			_T("%s: IMediaSample2::GetProperties\n"), GetClassName());
		HOST_CALL(TRACELEVEL_IMediaSample2Redir, m_spMediaSample2Host,
			GetProperties(cbProperties, pbProperties));
	}
	STDMETHOD(SetProperties)(IN DWORD cbProperties, IN const BYTE *pbProperties)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2Redir,
			_T("%s: IMediaSample2::SetProperties\n"), GetClassName());
		HOST_CALL(TRACELEVEL_IMediaSample2Redir, m_spMediaSample2Host,
			SetProperties(cbProperties, pbProperties));
	}

	//---------------------------------
	CComPtr<IMediaSample>	m_spMediaSampleHost;
	CComPtr<IMediaSample2>	m_spMediaSample2Host;
};

/////////////////////////////////////////////////////////////////////////////
// IMemAllocatorImpl
//
//	State machine:
//
//	1. AS_Create : after creation of object
//		SetProperties() : E_ABORT
//		GetProperties() : E_ABORT
//		Commit()		: E_ABORT
//		Decommit()		: E_ABORT
//		GetBuffer()		: E_ABORT
//		ReleaseBuffer()	: E_ABORT
//
//	2. AS_Init : After Init()
//	=>3	SetProperties() : (properties is OK)? (S_OK, goto 3) : (E_POINTER, VFW_E_BADALIGN, VFW_E_SIZENOTSET)
//		GetProperties() : (E_POINTER or VFW_E_BUFFER_NOTSET)
//		Commit()		: E_FAIL
//		Decommit()		: E_FAIL
//		GetBuffer()		: (E_POINTER or VFW_E_BUFFER_NOTSET)
//		ReleaseBuffer()	: (E_POINTER or ???)
//
//	3. AS_Prop : After SetProperties()
//		SetProperties() : (properties is OK)? (S_OK) : (E_POINTER, VFW_E_BADALIGN, VFW_E_SIZENOTSET)
//		GetProperties() : (properties and S_OK) or (E_POINTER)
//	=>4	Commit()		: (***Alloc()*** is OK)? (***AddRef()***, S_OK, goto 4) : E_OUTOFMEMORY
//		Decommit()		: (do nothing, S_OK)
//		GetBuffer()		: (E_POINTER or VFW_E_NOT_COMMITTED)
//		ReleaseBuffer()	: (E_POINTER or ???)
//
//	4. AS_Commit : After Commit() : IsFree() == true
//		SetProperties() : (E_POINTER, VFW_E_ALREADY_COMMITTED)
//		GetProperties() : (properties and S_OK) or (E_POINTER)
//		Commit()		: (do nothing, S_OK)
//	=>3	Decommit()		: (***Release()***, S_OK, goto 3)
//		GetBuffer()		: (param is BAD)? E_POINTER :
//	=>6							(IsBusy())? (goto 6, repeat GetBuffer()) : 
//	=>5									(***CreateObject()*** is OK)? (S_OK, goto 5) : (CreateObject() errors)
//		ReleaseBuffer()	: (E_POINTER or ???)
//
//	5. AS_Run : IsFree() == false
//		SetProperties() : (E_POINTER, VFW_E_ALREADY_COMMITTED)
//		GetProperties() : (properties and S_OK) or (E_POINTER)
//		Commit()		: (do nothing, S_OK)
//	=>7	Decommit()		: (S_OK, goto 7)
//		GetBuffer()		: (param is BAD)? E_POINTER :
//	=>6							(IsBusy())? (goto 6, repeat GetBuffer()) : 
//										(***CreateObject()*** is OK)? S_OK : (CreateObject() errors) :
//		ReleaseBuffer()	: (param is BAD)? E_POINTER :
//								(return memory, notify waiting samples, S_OK),
//	=>4							if(IsFree()) goto 4
//
//	6. AS_Busy : IsBusy() == true, sample threads in wait state
//		SetProperties() : (E_POINTER, VFW_E_ALREADY_COMMITTED)
//		GetProperties() : (properties and S_OK) or (E_POINTER)
//		Commit()		: (do nothing, S_OK)
//	=>7	Decommit()		: (notify waiting samples, S_OK, goto 7)
//		GetBuffer()		: (param is BAD)? E_POINTER :
//								(dwFlags & AM_GBF_NOWAIT)? VFW_E_TIMEOUT :
//									put thread into wait state
//		ReleaseBuffer()	: (param is BAD)? E_POINTER :
//	=>5							(return memory, notify waiting samples, S_OK, goto 5)
//
//	7. AS_Decommit : After Decommit() from state 5 or 6, while(!IsFree())
//		SetProperties() : (E_POINTER, VFW_E_BUFFERS_OUTSTANDING)
//		GetProperties() : (E_POINTER, VFW_E_NOT_COMMITTED)
//	=>6	Commit()		: (S_OK), (IsBusy())? goto 6 :
//	=>5														  goto 5
//		Decommit()		: (notify waiting samples, S_OK)
//		GetBuffer()		: (param is BAD)? E_POINTER : VFW_E_NOT_COMMITTED
//		ReleaseBuffer()	: (param is BAD)? E_POINTER :
//									(return memory, notify waiting samples, S_OK),
//	=>3								if(IsFree()) (***Release()***, goto 3)
//
//---------------------------------------------------------------------------
// Alignment :
//
//	The IMediaSample::GetPointer() method returns a pointer to the beginning of the buffer,
//	not including the prefix bytes designated by cbPrefix.
//		pBuffer: pSample->GetPointer(&pBuffer),
//		pPrefix = pBuffer - cbPrefix
//
//	A buffer/prefix is aligned if the start address is a multiple of cbAlign.
//	If a nonzero prefix is used, then the alignment cbAlign is applied to the prefix
//	If a prefix is equal to 0, then the beginning of the buffer is aligned according to cbAlign
//
//	The alignment must be a power of two. Depending on the microprocessor, reads and writes
//	to an aligned buffer might be faster than to an unaligned buffer. Also, some microprocessors
//	do not support unaligned reads and writes.
//
//  Real buffer start    Buffer start for IMediaSample::GetPointer()
//        V                 V
//        |xx|____Prefix____|________________Buffer________________|
//           A
//  Prefix/buffer start (aligned according to cbAlign)
//
//           |<--- size should be aligned according to cbAlign --->|
//
// If prefix != 0:
//		The beginning of the prefix should be aligned according to cbAlign (pPrefix % cbAlign == 0).
//		The summary size cbPrefix + cbBuffer also should be aligned
//		ATTENTION. Buffer start in this case can be not aligned !
//
//		From AVI Mux tests: if start properties = { 16, 2304, 512, 8 },
//		then buffer size should be increased to 2552 and real properties = { 16, 2552, 512, 8 }
//		( 2552 + 8 = 2560, 2560/512 = 5, 2560%512 = 0 )
//		Buffer start is aligned to 8, not 512 !
// 
// If prefix == 0:
//		The beginning of the buffer should be aligned on according to cbAlign (pBuffer % cbAlign == 0).
//		The buffer size cbBuffer also should be aligned
//---------------------------------------------------------------------------

#ifndef TRACELEVEL_IMemAllocator
	#define TRACELEVEL_IMemAllocator	8
#endif

#ifndef TRACELEVEL_IMemAllocator_Low
	#define TRACELEVEL_IMemAllocator_Low	(TRACELEVEL_IMemAllocator + 2)
#endif

#ifndef TRACELEVEL_IMemAllocator_Busy
	#define TRACELEVEL_IMemAllocator_Busy	(TRACELEVEL_IMemAllocator)
#endif

template <class T>
class ATL_NO_VTABLE IMemAllocatorImpl : public IMemAllocatorCallbackTemp
{
public:

	typedef enum tagAllocState
	{
		AS_Create, AS_Init, AS_Prop, AS_Commit, AS_Run, AS_Busy, AS_Decommit
	} ALLOCSTATE;

	ALLOCSTATE			 m_eState;

	ALLOCATOR_PROPERTIES m_rgProps;
    HANDLE				 m_hSem;

	CComPtr<IMemAllocatorNotifyCallbackTemp> m_spNotify;

	//---------------------------------
	virtual LPCTSTR GetClassName() const { return _T("IMemAllocatorImpl"); }

	IMemAllocatorImpl()
	{
		memset(&m_rgProps, 0, sizeof(ALLOCATOR_PROPERTIES));
		m_rgProps.cbAlign = 1;

		m_hSem = NULL;
		m_eState = AS_Create;
	}

	~IMemAllocatorImpl()
	{
		ATLASSERT(m_eState == AS_Create);
		if(m_hSem != NULL)
			::CloseHandle(m_hSem);
	}

	HRESULT Init()
	{
		ATLASSERT(m_eState == AS_Create);
		ATLASSERT(m_hSem == NULL);

		m_hSem = ::CreateSemaphore(NULL, 0, 0x7FFFFFFF, NULL);
		if(m_hSem == NULL)
			return E_OUTOFMEMORY;

		m_eState = AS_Init;
		return S_OK;
	}
	void Reset()
	{
		ATLASSERT(m_eState == AS_Create || m_eState == AS_Init || m_eState == AS_Prop);

		if(m_hSem != NULL)
		{
			::CloseHandle(m_hSem);
			m_hSem = NULL;
		}
		m_eState = AS_Create;
	}

	//---------------------------------
	// IMemAllocator

	// Specifies the size, number, and alignment of blocks
	// This method assumes that all blocks are the same size. 
	//
	// Remarks (DirectX SDK):
	//	"This method specifies the buffer requirements, but does not allocate any buffers.
	//	Call the IMemAllocator::Commit method to allocate buffers.
	//	The caller allocates two ALLOCATOR_PROPERTIES structures. The pRequest parameter
	//	contains the caller's buffer requirements, including the number of buffers and
	//	the size of each buffer. When the method returns, the pActual parameter contains
	//	the actual buffer properties, as set by the allocator.
	//	When this method is called, the allocator must not be committed or have outstanding buffers."
	STDMETHOD(SetProperties)(IN ALLOCATOR_PROPERTIES *pRequest, OUT ALLOCATOR_PROPERTIES *pActual)
	{
		ATLASSERT(pRequest != NULL);
		if(pRequest == NULL) return E_POINTER;

		ATLASSERT(pActual != NULL);
		if(pActual == NULL) return E_POINTER;

		memset(pActual, 0, sizeof(ALLOCATOR_PROPERTIES));

		T* pT = static_cast<T*>(this);
		T::ObjectLock lock(pT);

		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocator,
			_T("%s: IMemAllocator::SetProperties( %i, %i, %i, %i ) State:%d [0x%08X]\n"),
			GetClassName(), pRequest->cBuffers, pRequest->cbBuffer, pRequest->cbAlign, pRequest->cbPrefix, 
			m_eState, pT);

		//---------------------------------
		if(m_eState == AS_Create)	return E_ABORT;
		if(m_eState == AS_Commit)	return VFW_E_ALREADY_COMMITTED;
		if(m_eState == AS_Run)		return VFW_E_ALREADY_COMMITTED;
		if(m_eState == AS_Busy)		return VFW_E_ALREADY_COMMITTED;
		if(m_eState == AS_Decommit)	return VFW_E_BUFFERS_OUTSTANDING;

		//---------------------------------
		ALLOCATOR_PROPERTIES rgProps = *pRequest;
		HRESULT hr = pT->AdjustProperties(rgProps);
		if(SUCCEEDED(hr))
		{
			m_rgProps = rgProps;
			*pActual = rgProps;

			if(m_eState == AS_Init)
				m_eState = AS_Prop;
		}
		return hr;
	}

	// Retrieves the properties being used on this allocator
	//
	// Remarks (DirectX SDK):
	//	"Calls to this method might not succeed until the IMemAllocator::Commit method is called."
	STDMETHOD(GetProperties)(OUT ALLOCATOR_PROPERTIES *pProps)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocator,
			_T("%s: IMemAllocator::GetProperties\n"), GetClassName());
		ATLASSERT(pProps != NULL);
		if(pProps == NULL) return E_POINTER;

		T* pT = static_cast<T*>(this);
		T::ObjectLock lock(pT);

		//---------------------------------
		if(m_eState == AS_Create)	return E_ABORT;
		if(m_eState == AS_Init)		return VFW_E_BUFFER_NOTSET;

		//---------------------------------
		// AS_Prop, AS_Commit, AS_Run, AS_Busy, AS_Decommit
		*pProps = m_rgProps;
		return S_OK;
	}

	// Commits the memory for the specified buffers
	STDMETHOD(Commit)()
	{
		T* pT = static_cast<T*>(this);
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocator,
			_T("%s: IMemAllocator::Commit [0x%08X]\n"), GetClassName(), pT);

		T::ObjectLock lock(pT);

		HRESULT hr = S_OK;
		switch(m_eState)
		{
			case AS_Create :	hr = E_ABORT; break;
			case AS_Init :		hr = E_FAIL; break;

			case AS_Prop :		hr = pT->Init();
								if(SUCCEEDED(hr))
								{
									pT->GetUnknown()->AddRef();
									m_eState = AS_Commit;
								}
								break;

			case AS_Commit :
			case AS_Run :
			case AS_Busy :		break;

			case AS_Decommit :	m_eState = (pT->NumFreeBuffers() == 0)? AS_Busy : AS_Run;
								break;
		}
		return hr;
	}

	// Releases the memory for the specified buffers
	//
	// Remarks (DirectX SDK):
	//	"Any threads waiting in the IMemAllocator::GetBuffer method return with an error.
	//	Further calls to GetBuffer fail, until the IMemAllocator::Commit method is called.
	//	The purpose of the Decommit method is to prevent filters from getting any more
	//	samples from the allocator. Filters that already hold a reference count on a sample
	//	are not affected. After a filter releases a sample and the reference count goes to zero,
	//	however, the sample is no longer available. 
	//	The allocator may free the memory belonging to any sample with a reference count of zero.
	//	Thus, the Decommit method "releases" the memory in the sense that filters stop having
	//	access to it. Whether the memory actually returns to the heap dependson the implementation
	//	of the allocator. Some allocators wait until their own destructor method. However,
	//	an allocator must not leave any allocated memory behind when it deletes itself.
	//	Therefore, an allocator's destructor must wait until all of its samples are released."
	STDMETHOD(Decommit)()
	{
		T* pT = static_cast<T*>(this);
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocator,
			_T("%s: IMemAllocator::Decommit [0x%08X]\n"), GetClassName(), pT);

		T::ObjectLock lock(pT);

		HRESULT hr = S_OK;
		switch(m_eState)
		{
			case AS_Create :	hr = E_ABORT; break;
			case AS_Init :		hr = E_FAIL; break;
			case AS_Prop :		break;

			case AS_Commit:		ATLASSERT(pT->IsAllReturn());

			case AS_Run :
			case AS_Busy :		m_eState = AS_Decommit;

			case AS_Decommit :	if(pT->IsAllReturn())
								{
									// ATTENTION. For protection of allocator from destruction
									// pointer to allocator addref'ed in this moment.
									// See chain of calls:
									// CComSampleObject::Release() => ~CComSampleObject() =>
									// CMemAllocBase::ClearSample()-> ReleaseBuffer()
									pT->GetUnknown()->Release();
									m_eState = AS_Prop;
#ifdef FREE_ON_DECOMMIT
									pT->FreeMemory();
#endif
								}
								pT->NotifySample();
								break;

		}
		return hr;
	}

	// Retrieves a container for a sample
	//
	// Remarks (DirectX SDK):
	//	"By default, this method blocks until a free sample is available or the allocator
	//	is decommitted. If the caller specifies the AM_GBF_NOWAIT flag and no sample is
	//	available, the allocator can return immediately with a return value of VFW_E_TIMEOUT.
	//	However, allocators are not required to support this flag.
	//	The sample returned in ppBuffer has a valid buffer pointer. The caller is responsible
	//	for setting any other properties on the sample, such as the time stamps, the media times,
	//	or the sync-point property.
	//	The pStartTime and pEndTime parameters are not applied to the sample. The allocator
	//	might use these values to determine which buffer it retrieves.
	//	You must call the IMemAllocator::Commit method before calling this method. This method
	//	fails after the IMemAllocator::Decommit method is called. If the method succeeds,
	//	the IMediaSample interface that it returns has an outstanding reference count.
	//	Be sure to release the interface when you are done using it."
	STDMETHOD(GetBuffer)(OUT IMediaSample **ppSample,
						 IN REFERENCE_TIME* /*pStartTime*/,
						 IN REFERENCE_TIME* /*pEndTime*/,
						 IN DWORD dwFlags)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocator_Low,
			_T("%s: IMemAllocator::GetBuffer\n"), GetClassName());
		ATLASSERT(ppSample != NULL);
		if(ppSample == NULL) return E_POINTER;

		T* pT = static_cast<T*>(this);

		*ppSample = NULL;
		HRESULT hr = S_OK;
		for(;;)
		{
			{
				T::ObjectLock lock(pT);

				switch(m_eState)
				{
					case AS_Create :	return E_ABORT;
					case AS_Init :		return VFW_E_BUFFER_NOTSET;
					case AS_Prop :		return VFW_E_NOT_COMMITTED;

					case AS_Commit:
					case AS_Run :		hr = pT->CreateSample(ppSample);
										if(FAILED(hr))
										{
											ATLASSERT(!_T("IMemAllocatorImpl::GetBuffer(): CreateSample() FAILED"));
											return hr;
										}
										if(hr == S_OK)
										{
											m_eState = AS_Run;
											return S_OK;
										}

										ATLASSERT(hr == S_FALSE);
										if(m_eState == AS_Commit)
											return E_FAIL;

										m_eState = AS_Busy;
										ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocator_Busy,
											_T("[Allocator] Allocator busy, waiting = %s\n"),
											(dwFlags & AM_GBF_NOWAIT)? _T("NO"): _T("YES"));

					case AS_Busy :		// AS_Busy state can be changed only in calls of
										// Commit/Decommit/ReleaseBuffer
										if(dwFlags & AM_GBF_NOWAIT)
											return VFW_E_TIMEOUT;
										
										pT->SetWaiting();
										break;

					case AS_Decommit :	return VFW_E_NOT_COMMITTED;
				}
			}

			::WaitForSingleObject(m_hSem, INFINITE);

			ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocator_Low, _T("[Allocator] Wait period ended\n"));
		}

		return hr;
	}

	// Releases CComSampleObject object
	// Calls in CComSampleObject destructor
	STDMETHOD(ReleaseBuffer)(IN IMediaSample *pSample)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocator_Low,
			_T("%s: IMemAllocator::ReleaseBuffer\n"), GetClassName());
		ATLASSERT(pSample != NULL);
		if(pSample == NULL) return E_POINTER;

		//---------------------------------
//		{
//			REFERENCE_TIME tS = 0;
//			REFERENCE_TIME tE = 0;
//			pSample->GetTime(&tS, &tE);
//			ATLTRACE2(atlTraceCOM, 2, _T("%s: IMemAllocator::ReleaseBuffer( %s, %s )\n"),
//				GetClassName(), RT2STR(tS), RT2STR(tE));
//		}

		//---------------------------------
		T* pT = static_cast<T*>(this);

		HRESULT hr = S_OK;
		CComPtr<IMemAllocatorNotifyCallbackTemp> spNotify;

		{
			T::ObjectLock lock(pT);

			switch(m_eState)
			{
				case AS_Busy :		m_eState = AS_Run;
				case AS_Run :		if(pT->IsAllReturn())
										m_eState = AS_Commit;
									spNotify = m_spNotify;
									break;

				case AS_Decommit :	if(pT->IsAllReturn())
									{
										// ATTENTION. For protection of allocator from destruction
										// pointer to allocator addref'ed in this moment.
										// See chain of calls:
										// CComSampleObject::Release() => ~CComSampleObject() =>
										// CMemAllocBase::ClearSample()-> ReleaseBuffer()
										pT->GetUnknown()->Release();
										m_eState = AS_Prop;
									}

									spNotify = m_spNotify;
									break;

				case AS_Commit:		// If first call of GetBuffer() failed, then allocator
									// in AS_Commit state yet and all buffers must be returned
									// to buffer container
									ATLASSERT(pT->IsAllReturn());
									// Do nothing more
									break;

				default :			// AS_Create, AS_Init, AS_Prop
									hr = E_ABORT;
									break;
			}
		}

		// ATTENTION: Making call of m_spNotify->NotifyRelease() out of critical section.
		// If we'll using additional critical section for synchronize
		// usage/change m_spNotify, then possible dead lock. For sample:
		// pNotify->NotifyRelease() -> SetEvent() to another thread ->
		// Wait cycle end in another thread -> pAllocator->SetNotify()
		if(spNotify != NULL)
		{
			spNotify->NotifyRelease();
		}
		return hr;
	}

	//---------------------------------
	// IMemAllocatorCallbackTemp
    STDMETHOD(SetNotify)(IN IMemAllocatorNotifyCallbackTemp* pNotify)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocator,
			_T("%s: IMemAllocatorCallbackTemp::SetNotify( 0x%08X )\n"), GetClassName(), pNotify);

		T::ObjectLock lock(static_cast<T*>(this));
		m_spNotify = pNotify;
		return S_OK;
	}
	STDMETHOD(GetFreeCount)(OUT LONG* plBuffersFree)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocator,
			_T("%s: IMemAllocatorCallbackTemp::GetFreeCount\n"), GetClassName());

		ATLASSERT(plBuffersFree != NULL);
		if(plBuffersFree == NULL) return E_POINTER;

		//---------------------------------
		T* pT = static_cast<T*>(this);
		T::ObjectLock lock(pT);

		*plBuffersFree = 0;
		switch(m_eState)
		{
			case AS_Create :	return E_ABORT;
			case AS_Init :		return VFW_E_BUFFER_NOTSET;
			case AS_Prop :
			case AS_Decommit :	return VFW_E_NOT_COMMITTED;
		}

		// AS_Commit, AS_Run, AS_Busy
		*plBuffersFree = pT->NumFreeBuffers();
		return S_OK;
	}
};

/////////////////////////////////////////////////////////////////////////////
// IMemAllocatorRedirect

#ifndef TRACELEVEL_IMemAllocatorRedir
	#define TRACELEVEL_IMemAllocatorRedir	8
#endif

#ifndef TRACELEVEL_IMemAllocatorRedir_Low
	#define TRACELEVEL_IMemAllocatorRedir_Low	(TRACELEVEL_IMemAllocatorRedir + 2)
#endif

class ATL_NO_VTABLE IMemAllocatorRedirect : public IMemAllocator
{
//	COM_INTERFACE_ENTRY(IMemAllocator)
public:

	virtual LPCTSTR GetClassName() const { return _T("IMemAllocatorRedirect"); }

	HRESULT SetMemAllocRedirect(IUnknown* pHost)
	{
		m_spMemAllocHost.Release();
		return (pHost == NULL) ? S_OK : pHost->QueryInterface(__uuidof(IMemAllocator), (void**)&m_spMemAllocHost);
	}

	//---------------------------------
	// IMemAllocator
	STDMETHOD(SetProperties)(IN ALLOCATOR_PROPERTIES *pRequest, OUT ALLOCATOR_PROPERTIES *pActual)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocatorRedir,
			_T("%s: IMemAllocator::SetProperties : pRequest = { %i,  %i,  %i,  %i }\n"), GetClassName(),
			pRequest->cBuffers, pRequest->cbBuffer, pRequest->cbAlign, pRequest->cbPrefix);

		if(m_spMemAllocHost == NULL)
		{
			ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocatorRedir, _T("IMemAllocatorRedirect : Host don't set\n"));
			return E_FAIL;
		}

		//---------------------------------
		HRESULT hr = m_spMemAllocHost->SetProperties(pRequest, pActual);
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocatorRedir,
			_T("%s: IMemAllocator::SetProperties() = 0x%X\n"), GetClassName(), hr);

		if(SUCCEEDED(hr) && pActual != NULL)
		{
			ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocatorRedir,
				_T("%s: IMemAllocator::SetProperties : pActual = { %i,  %i,  %i,  %i }\n"), GetClassName(),
				pActual->cBuffers, pActual->cbBuffer, pActual->cbAlign, pActual->cbPrefix);
		}
		return hr;
	}
	STDMETHOD(GetProperties)(OUT ALLOCATOR_PROPERTIES *pProps)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocatorRedir,
			_T("%s: IMemAllocator::GetProperties\n"), GetClassName());

		if(m_spMemAllocHost == NULL)
		{
			ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocatorRedir, _T("IMemAllocatorRedirect : Host don't set\n"));
			return E_FAIL;
		}

		//---------------------------------
		HRESULT hr = m_spMemAllocHost->GetProperties(pProps);
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocatorRedir,
			_T("%s: IMemAllocator::GetProperties() = 0x%X\n"), GetClassName(), hr);

		if(SUCCEEDED(hr) && pProps != NULL)
		{
			ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocatorRedir,
				_T("%s: IMemAllocator::GetProperties : pProps = { %i,  %i,  %i,  %i }\n"), GetClassName(),
				pProps->cBuffers, pProps->cbBuffer, pProps->cbAlign, pProps->cbPrefix);
		}
		return hr;
	}
	STDMETHOD(Commit)()
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocatorRedir,
			_T("%s: IMemAllocator::Commit\n"), GetClassName());
		HOST_CALL(TRACELEVEL_IMemAllocatorRedir, m_spMemAllocHost, Commit());
	}
	STDMETHOD(Decommit)()
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocatorRedir,
			_T("%s: IMemAllocator::Decommit\n"), GetClassName());
		HOST_CALL(TRACELEVEL_IMemAllocatorRedir, m_spMemAllocHost, Decommit());
	}
	STDMETHOD(GetBuffer)(OUT IMediaSample **ppSample,
						 IN REFERENCE_TIME* pStartTime,
						 IN REFERENCE_TIME* pEndTime,
						 IN DWORD dwFlags)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocatorRedir_Low,
			_T("%s: IMemAllocator::GetBuffer(), dwFlags=%u\n"), GetClassName(), dwFlags);
		HOST_CALL(TRACELEVEL_IMemAllocatorRedir, m_spMemAllocHost,
			GetBuffer(ppSample, pStartTime, pEndTime, dwFlags));
	}
	STDMETHOD(ReleaseBuffer)(IN IMediaSample *pSample)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocatorRedir_Low,
			_T("%s: IMemAllocator::ReleaseBuffer\n"), GetClassName());
		HOST_CALL(TRACELEVEL_IMemAllocatorRedir, m_spMemAllocHost, ReleaseBuffer(pSample));
	}

	//---------------------------------
	CComPtr<IMemAllocator>	m_spMemAllocHost;
};

/////////////////////////////////////////////////////////////////////////////
// Memory buffers in one continuous chunk
//
// [DKN 2011-12]
// Change buffers alignment for cbAlign < 16: make real alignment = 16
// It can resolve some unvisible problems and increase performance
#define ALLOC_DEF_ALIGNMENT		16

class ATL_NO_VTABLE CBufferOnVirtualAlloc
{
	void*	m_pMemory;			// Original allocated buffer

	//---------------------------------
public:
	CBufferOnVirtualAlloc()
	{
		m_pMemory = NULL;
	}

	~CBufferOnVirtualAlloc()
	{
		if(m_pMemory != NULL)
			::VirtualFree(m_pMemory, 0, MEM_RELEASE);
	}

	bool CheckAlignment(DWORD /*cbAlignment*/)
	{
		return true;
	}

	//---------------------------------
	// Real buffer start   Buffers start for IMediaSample::GetPointer() (accordingly alignment)
	//     V               V                                 V 
	//     |xxxx|__Prefix__|_______Buffer_______|x|__Prefix__|_______Buffer_______|x|....
	//          A                                A                                 A
	// Prefix start (accordingly alignment)      Alignment for next blocks (size alignment)
	//
	// ATTENTION. cbAlign and internal_cbAlign must be a power of 2
	//
	// ppBuffers: allocated and cleared array of buffers for IMediaSample::GetPointer()
	// Size of ppBuffers array is cBuffers * sizeof(BYTE*)
	//
	// [DKN 2011-12]
	// Change buffers alignment for cbAlign < 16: make real alignment = 16
	// It can resolve some unvisible problems and increase performance

	bool Alloc(ALLOCATOR_PROPERTIES rgProps, BYTE** ppBuffers)
	{
		ATLASSERT(rgProps.cBuffers > 0 && rgProps.cbBuffer > 0 && rgProps.cbAlign >= 0);
		ATLASSERT(ppBuffers != NULL);

		ATLASSERT(m_pMemory == NULL);

		LONG cbAlign = rgProps.cbAlign;

		if(cbAlign == 0)
			cbAlign = 1;
		else
		{
			// cbAlign must be a power of 2
			ATLASSERT((-cbAlign & cbAlign) == cbAlign);
		}

		if(cbAlign < ALLOC_DEF_ALIGNMENT)
			cbAlign = ALLOC_DEF_ALIGNMENT;

		//---------------------------------
		// Memory size for all buffers
		DWORD cb = (DWORD)(rgProps.cbPrefix + rgProps.cbBuffer);
		DWORD cbAlignedSize = ALIGN_UP(cb, cbAlign);
		cb = cbAlignedSize * (DWORD)(rgProps.cBuffers - 1) + cb;

		SYSTEM_INFO rgSysInfo;
		GetSystemInfo(&rgSysInfo);
		if((DWORD)cbAlign > rgSysInfo.dwAllocationGranularity)
		{
			// Extending buffer size correspondingly to alignment request
			cb += (DWORD)cbAlign - rgSysInfo.dwAllocationGranularity;
		}
		
		m_pMemory = ::VirtualAlloc(NULL, cb, MEM_COMMIT, PAGE_READWRITE);
		if(m_pMemory == NULL)
			return false;

		//---------------------------------
		// Fill array of buffers for IMediaSample::GetPointer() (accordingly alignment)
		ppBuffers[0] = (BYTE*)ALIGN_UP_PNT(m_pMemory, cbAlign) + rgProps.cbPrefix;
		for(LONG i = 1; i < rgProps.cBuffers; ++i)
		{
			ppBuffers[i] = ppBuffers[i - 1] + cbAlignedSize;
		}
		return true;
	}

	// Don't use ppBuffers/nBuffers in this case, deleting all memory
	void Free(BYTE** /*ppBuffers*/ = NULL, DWORD /*nBuffers*/ = 0)
	{
		if(m_pMemory != NULL)
		{
			::VirtualFree(m_pMemory, 0, MEM_RELEASE);
			m_pMemory = NULL;
		}
	}
};

//---------------------------------
// Array of memory buffers (separate VirtualAlloc() for each buffer)

class ATL_NO_VTABLE CArrayOnVirtualAlloc
{
	void**	m_ppBuffers;			// Array with pointers to buffers
	DWORD	m_nBuffers;				// Number of buffers

	//---------------------------------
public:
	CArrayOnVirtualAlloc()
	{
		m_ppBuffers = NULL;
		m_nBuffers = 0;
	}

	~CArrayOnVirtualAlloc()
	{
		if(m_ppBuffers != NULL)
		{
			for(DWORD n = 0; n < m_nBuffers; ++n)
			{
				if(m_ppBuffers[n] != NULL)
					::VirtualFree(m_ppBuffers[n], 0, MEM_RELEASE);
			}
			free(m_ppBuffers);
		}
	}

	bool CheckAlignment(DWORD /*cbAlignment*/)
	{
		return true;
	}

	//---------------------------------
	// Real buffer start   Buffers start for IMediaSample::GetPointer() (accordingly alignment)
	//     V               V
	//     |xxxx|__Prefix__|_______Buffer_______|
	//          A
	// Prefix start (accordingly alignment)         
	//
	// ATTENTION. cbAlign and internal_cbAlign must be a power of 2
	//
	// ppBuffers: allocated and cleared array of buffers for IMediaSample::GetPointer()
	// Size of ppBuffers array is cBuffers * sizeof(BYTE*)
	//
	// [DKN 2011-12]
	// Don't change buffers alignment for cbAlign < 16: separate VirtualAlloc()
	// used for each buffer and we don't need in alignment correction

	bool Alloc(ALLOCATOR_PROPERTIES rgProps, BYTE** ppBuffers)
	{
		ATLASSERT(rgProps.cBuffers > 0 && rgProps.cbBuffer > 0 && rgProps.cbAlign >= 0);
		ATLASSERT(ppBuffers != NULL);

		ATLASSERT(m_ppBuffers == NULL);
		ATLASSERT(m_nBuffers == 0);

		if(rgProps.cbAlign == 0)
			rgProps.cbAlign = 1;
		else
		{
			// cbAlign must be a power of 2
			ATLASSERT((-rgProps.cbAlign & rgProps.cbAlign) == rgProps.cbAlign);
		}

		//---------------------------------
		// Memory size for all buffers
		DWORD cb = (DWORD)(rgProps.cbPrefix + rgProps.cbBuffer);

		SYSTEM_INFO rgSysInfo;
		GetSystemInfo(&rgSysInfo);
		if((DWORD)rgProps.cbAlign > rgSysInfo.dwAllocationGranularity)
		{
			// Extending buffer size correspondingly to alignment request
			cb += (DWORD)rgProps.cbAlign - rgSysInfo.dwAllocationGranularity;
		}

		//---------------------------------
		m_ppBuffers = (void**)malloc(rgProps.cBuffers * sizeof(void*));
		if(m_ppBuffers == NULL)
			return false;

		memset(m_ppBuffers, 0, rgProps.cBuffers * sizeof(void*));
		m_nBuffers = rgProps.cBuffers;

		DWORD n;
		for(n = 0; n < m_nBuffers; ++n)
		{
			m_ppBuffers[n] = ::VirtualAlloc(NULL, cb, MEM_COMMIT, PAGE_READWRITE);
			if(m_ppBuffers[n] == NULL)
			{
				Free();
				return false;
			}
		}

		//---------------------------------
		// Fill array of buffers for IMediaSample::GetPointer() (accordingly alignment)
		for(n = 0; n < m_nBuffers; ++n)
		{
			ppBuffers[n] = (BYTE*)ALIGN_UP_PNT(m_ppBuffers[n], rgProps.cbAlign) + rgProps.cbPrefix;
		}
		return true;
	}

	// Don't use ppBuffers/nBuffers in this case, deleting all memory
	void Free(BYTE** /*ppBuffers*/ = NULL, DWORD /*nBuffers*/ = 0)
	{
		if(m_ppBuffers != NULL)
		{
			for(DWORD n = 0; n < m_nBuffers; ++n)
			{
				if(m_ppBuffers[n] != NULL)
					::VirtualFree(m_ppBuffers[n], 0, MEM_RELEASE);
			}

			free(m_ppBuffers);
			m_ppBuffers = NULL;
			m_nBuffers = 0;
		}
	}
};

//---------------------------------
// CAllocatorBuffers - base container class for allocator buffers
template <class TBuffer /*= CBufferOnVirtualAlloc*/>
class ATL_NO_VTABLE CAllocatorBuffers : public TBuffer
{
public:
	CAllocatorBuffers() : m_nBuffers(0), m_stackFree(NULL), m_nFree(0) {}

	~CAllocatorBuffers()
	{
		Clear();
	}

	void Clear()
	{
		// Have all buffers returned ?
		ATLASSERT(IsAllReturn());

		if(m_stackFree != NULL)
		{
			TBuffer::Free(m_stackFree, m_nBuffers);
			free(m_stackFree);
			m_stackFree = NULL;
		}
		m_nBuffers = m_nFree = 0;
	}

	bool Init(ALLOCATOR_PROPERTIES rgProps)
	{
		ATLASSERT(rgProps.cBuffers > 0 && rgProps.cbBuffer > 0 && rgProps.cbAlign >= 0);

		if(rgProps.cbAlign <= 0)
			rgProps.cbAlign = 1;

		//---------------------------------
		Clear();

		DWORD nBuffers = rgProps.cBuffers;
		m_stackFree = (BYTE**)malloc(nBuffers * sizeof(BYTE*));
		if(m_stackFree == NULL)
			return false;

		memset(m_stackFree, 0, nBuffers * sizeof(BYTE*));
		m_nBuffers = m_nFree = nBuffers;

		if(!TBuffer::Alloc(rgProps, m_stackFree))
		{
			Clear();
			return false;
		}

		return true;
	}

	DWORD NumFreeBuffers() const
	{
		return m_nFree;
	}

	bool IsAllReturn() const
	{
		return m_nBuffers == m_nFree;
	}

	void PutBuffer(BYTE *pBuffer)
	{
		ATLASSERT(m_stackFree != NULL && !IsAllReturn());
		m_stackFree[m_nFree++] = pBuffer;
	}

	BYTE* GetBuffer()
	{
		ATLASSERT(m_stackFree != NULL);

		if(NumFreeBuffers() == 0)
			return NULL;

		return m_stackFree[--m_nFree];
	}

	//---------------------------------
	//  Real buffer start    Buffer start for IMediaSample::GetPointer()
	//        V                 V
	//        |xx|____Prefix____|________________Buffer________________|
	//           A
	//  Prefix/buffer start (aligned according to cbAlign)
	//
	//           |<--- size should be aligned according to cbAlign --->|
	//
	// pBuffer: pSample->GetPointer(&pBuffer),
	// pPrefix = pBuffer - cbPrefix
	//
	// If prefix != 0:
	//		The beginning of the prefix should be aligned according to cbAlign (pPrefix % cbAlign == 0).
	//		The summary size cbPrefix + cbBuffer also should be aligned
	//		ATTENTION. Buffer start in this case can be not aligned !
	//
	//		From AVI Mux tests: if start properties = { 16, 2304, 512, 8 },
	//		then buffer size should be increased to 2552 and real properties = { 16, 2552, 512, 8 }
	//		( 2552 + 8 = 2560, 2560/512 = 5, 2560%512 = 0 )
	//		Buffer start is aligned to 8, not 512 !
	// 
	// If prefix == 0:
	//		The beginning of the buffer should be aligned on according to cbAlign (pBuffer % cbAlign == 0).
	//		The buffer size cbBuffer also should be aligned

	HRESULT Correct(ALLOCATOR_PROPERTIES& rgProps)
	{
		//---------------------------------
		// Testing input parameters

		// Check alignment in base class
		if(!TBuffer::CheckAlignment(rgProps.cbAlign))
		{
			ATLASSERT(!_T("CAllocatorBuffers::Correct(): Alignment ñheck failed in base class"));
			return VFW_E_BADALIGN;
		}

		// Check the alignment request is a power of 2
		LONG cbAlign = rgProps.cbAlign;
		if((-cbAlign & cbAlign) != cbAlign)
		{
			LONG cbNewAlign = 1;
			while(cbAlign != 0)
			{
				cbAlign >>= 1;
				cbNewAlign <<= 1;
			}
			if(cbNewAlign == 0)
			{
				ATLASSERT(!_T("CCAllocatorBuffers::Correct(): Invalid alignment"));
				return VFW_E_BADALIGN;
			}

			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Warning, _T("WARNING. CAllocatorBuffers::Correct(): ")
				_T("Alignment not a power of 2 (rgProps.cbAlign = 0x%X), corrected alignment = 0x%X\n"),
				rgProps.cbAlign, cbNewAlign);

			rgProps.cbAlign = cbNewAlign;
		}

		if(rgProps.cBuffers <= 0 || rgProps.cbBuffer <= 0)
		{
			ATLASSERT(!_T("CAllocatorBuffers::Correct() : Invalid numbers of buffers or buffer size"));
			return VFW_E_SIZENOTSET;
		}

		//---------------------------------
		// Correcting parameters
		if(rgProps.cbAlign == 0)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Warning,
				_T("WARNING. CAllocatorBuffers::Correct(): rgProps.cbAlign == 0\n"));

			rgProps.cbAlign = 1;
		}

		// Assert from AVI Mux:
		// (pSample->GetSize() + m_cbPrefix) % m_cbAlign == 0

		LONG n = (rgProps.cbBuffer + rgProps.cbPrefix - 1)/rgProps.cbAlign + 1;
		n *= rgProps.cbAlign;
		n -= rgProps.cbPrefix;
		if(rgProps.cbBuffer != n)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Warning, _T("WARNING. CAllocatorBuffers::Correct(): ")
				_T("input props = { %i, %i, %i, %i }, corrected size of buffer = %i\n"),
				rgProps.cBuffers, rgProps.cbBuffer, rgProps.cbAlign, rgProps.cbPrefix, n);

			rgProps.cbBuffer = n;
		}
		return S_OK;
	}

	//---------------------------------

protected :
	DWORD	m_nBuffers;			// Numbers of reserved buffers
	BYTE**	m_stackFree;		// Stack of free aligned buffers
	DWORD	m_nFree;			// Numbers of free buffers
};

/////////////////////////////////////////////////////////////////////////////
// Helper container for store pointers to memory for CComSampleObject

template <class TSampleObject>
class ATL_NO_VTABLE CSampleArray
{
public:
	CSampleArray() : m_ppArr(NULL), m_nReserved(0), m_nAll(0), m_nBusy(0) {}

	~CSampleArray() { Clear(); }

	void Clear()
	{
		ATLASSERT(m_nBusy == m_nAll);
		if(m_ppArr != NULL)
		{
			for(DWORD n = 0; n < m_nBusy; ++n)
				free(m_ppArr[n]);
			free(m_ppArr);

			m_ppArr = NULL;
			m_nReserved = m_nAll = m_nBusy = 0;
		}
	}

	bool Init(DWORD nMax)
	{
		Clear();

		if(nMax > 0)
		{
			m_ppArr = (TSampleObject**)malloc(sizeof(TSampleObject*) * nMax);
			if(m_ppArr == NULL) return false;
	
			m_nReserved = nMax;

			for(; m_nAll < nMax; m_nBusy = ++m_nAll)
			{
				m_ppArr[m_nAll] = (TSampleObject*)malloc(sizeof(TSampleObject));
				if(m_ppArr[m_nAll] == NULL)
				{
					Clear();
					return false;
				}
			}
		}
		return true;
	}

	DWORD Capacity() const { return m_nAll; }

	//---------------------------------
	void Put(TSampleObject* pSample)
	{
		ATLASSERT(pSample != NULL);
		ATLASSERT(m_ppArr != NULL);
		ATLASSERT(m_nBusy < m_nAll);

		m_ppArr[m_nBusy++] = pSample;
	}

	TSampleObject* Get()
	{
		if(m_nBusy == 0 && !Extend()) return NULL;

		TSampleObject* p = m_ppArr[--m_nBusy];
		ATLASSERT(p != NULL && _T("CSampleArray::Get()"));
		if(p != NULL)
			memset(p, 0, sizeof(TSampleObject));
		return p;
	}

private :
	bool Extend()
	{
		// Container can be extended only if it's empty !
		ATLASSERT(m_nBusy == 0);

		if(m_nAll == m_nReserved)
		{
			DWORD nMax = (m_nReserved == 0)? 1 : 2 * m_nReserved;
			TSampleObject** ppArr = (TSampleObject**)malloc(sizeof(TSampleObject*) * nMax);
			if(ppArr == NULL) return false;

			free(m_ppArr);
			m_ppArr = ppArr;
			m_nReserved = nMax;
		}

		m_ppArr[0] = (TSampleObject*)malloc(sizeof(TSampleObject));
		if(m_ppArr[0] == NULL) return false;

		++m_nAll;
		m_nBusy = 1;
		return true;
	}

	//---------------------------------
	TSampleObject**	m_ppArr;
	DWORD			m_nReserved;
	DWORD			m_nAll;
	DWORD			m_nBusy;
};


/////////////////////////////////////////////////////////////////////////////
// Use next class for remome asserts at InternalRelease() calls from ffdshow
// video encoder
#ifdef	NO_FFDSHOW_ASSERT

template <class ThreadModel>
class CComObjectRootExNoAss : public CComObjectRootBase
{
public:
	typedef ThreadModel _ThreadModel;
	typedef typename _ThreadModel::AutoCriticalSection _CritSec;
	typedef CComObjectLockT<_ThreadModel> ObjectLock;

	ULONG InternalAddRef()
	{
//		ATLASSERT(m_dwRef != -1L);
		if(m_dwRef == -1L)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### CComObjectRootExNoAss::InternalAddRef(): ")
				_T("m_dwRef == -1\n"));
		}
		return _ThreadModel::Increment(&m_dwRef);
	}
	ULONG InternalRelease()
	{
//		ATLASSERT(m_dwRef > 0);
		if(m_dwRef == 0)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### CComObjectRootExNoAss::InternalRelease(): ")
				_T("m_dwRef == 0\n"));
		}
		return _ThreadModel::Decrement(&m_dwRef);
	}

	void Lock() {m_critsec.Lock();}
	void Unlock() {m_critsec.Unlock();}

private:
	_CritSec m_critsec;
};

#endif	// NO_FFDSHOW_ASSERT

/////////////////////////////////////////////////////////////////////////////
// CMediaSample
class ATL_NO_VTABLE CMediaSample :
#ifdef	NO_FFDSHOW_ASSERT
	public CComObjectRootExNoAss<CComMultiThreadModel>,
#else
	public CComObjectRootEx<CComMultiThreadModel>,
#endif
	public CComCoClass<CMediaSample>,
	public IMediaSample2Wrapper
{
public:
	virtual LPCTSTR GetClassName() const { return _T("CMediaSample"); }

	CMediaSample() {}
	~CMediaSample() {}
	
	HRESULT FinalConstruct()
	{
		ATLTRACE2(atlTraceGeneral, 10, _T("CMediaSample::FinalConstruct\n"));
		return S_OK;
	}
	void FinalRelease()
	{
		ATLTRACE2(atlTraceGeneral, 10, _T("CMediaSample::FinalRelease\n"));
	}

	//---------------------------------
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CMediaSample)
		COM_INTERFACE_ENTRY(IMediaSample)
		COM_INTERFACE_ENTRY(IMediaSample2)
	END_COM_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMediaSampleCashe

#ifndef TRACELEVEL_CMediaSampleCache
	#define TRACELEVEL_CMediaSampleCache	10
#endif

class ATL_NO_VTABLE CMediaSampleCashe :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMediaSampleCashe>,
	public IMediaSample2Wrapper,
	public IMediaSampleCache
{
	CComPtr<IMediaSample>	m_spCachedSample;

	//---------------------------------

public:
	virtual LPCTSTR GetClassName() const { return _T("CMediaSampleCashe"); }

	HRESULT FinalConstruct()
	{
		ATLTRACE2(atlTraceGeneral, 10, _T("CMediaSampleCashe::FinalConstruct\n"));
		return S_OK;
	}
	void FinalRelease()
	{
		ATLTRACE2(atlTraceGeneral, 10, _T("CMediaSampleCashe::FinalRelease\n"));
		m_spCachedSample.Release();
	}

	//---------------------------------
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CMediaSampleCashe)
		COM_INTERFACE_ENTRY(IMediaSample)
		COM_INTERFACE_ENTRY(IMediaSample2)
		COM_INTERFACE_ENTRY(IMediaSampleCache)
	END_COM_MAP()

	//---------------------------------
	// IMediaSampleCache

	// Put extern sample
	STDMETHOD(PutExternSample)(IN IMediaSample* pExternSample)
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_CMediaSampleCache,
			_T("%s: IMediaSampleCache::PutExternSample( 0x%X )\n"), GetClassName(), pExternSample);

		m_spCachedSample = pExternSample;
		return S_OK;
	}

	// Get extern sample
	STDMETHOD(GetExternSample)(OUT IMediaSample** ppExternSample)
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_CMediaSampleCache,
			_T("%s: IMediaSampleCache::GetExternSample(), m_spCachedSample=0x%X\n"),
			GetClassName(), m_spCachedSample);

		ATLASSERT(ppExternSample != NULL);
		if(ppExternSample == NULL) return E_POINTER;

		m_spCachedSample.CopyTo(ppExternSample);
		return (*ppExternSample != NULL)? S_OK : S_FALSE;
	}

	// Check extern sample present
	STDMETHOD(HasExternSample)()
	{
		return (m_spCachedSample != NULL)? S_OK : S_FALSE;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CPreviewMediaSample

class ATL_NO_VTABLE CPreviewMediaSample :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CPreviewMediaSample>,
	public IMediaSample2Redirect
{
public:
	virtual LPCTSTR GetClassName() const { return _T("CPreviewMediaSample"); }

	CPreviewMediaSample() {}
	~CPreviewMediaSample() {}
	
	HRESULT FinalConstruct()
	{
		ATLTRACE2(atlTraceGeneral, 10, _T("CPreviewMediaSample::FinalConstruct\n"));
		return S_OK;
	}
	void FinalRelease()
	{
		ATLTRACE2(atlTraceGeneral, 10, _T("CPreviewMediaSample::FinalRelease\n"));
	}

	//---------------------------------
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CPreviewMediaSample)
		COM_INTERFACE_ENTRY(IMediaSample)
		COM_INTERFACE_ENTRY(IMediaSample2)
	END_COM_MAP()

	//---------------------------------
	// Overriding of IMediaSample::GetTime()/GetMediaTime() for preview samples
	STDMETHOD(GetTime)(OUT REFERENCE_TIME* /*pTimeStart*/, OUT REFERENCE_TIME* /*pTimeEnd*/)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2Redir,
			_T("%s: IMediaSample::GetTime\n"), GetClassName());
		return VFW_E_SAMPLE_TIME_NOT_SET;
	}

	STDMETHOD(GetMediaTime)(OUT LONGLONG* /*pTimeStart*/, OUT LONGLONG* /*pTimeEnd*/)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2Redir,
			_T("%s: IMediaSample::GetMediaTime\n"), GetClassName());
		return VFW_E_MEDIA_TIME_NOT_SET;
	}

	//---------------------------------
	// Overriding of IMediaSample2::GetProperties() for preview samples
	STDMETHOD(GetProperties)(IN DWORD cbProperties, OUT BYTE *pbProperties)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2Redir,
			_T("%s: IMediaSample2::GetProperties\n"), GetClassName());

		if(!m_spMediaSample2Host)
		{
			ATLTRACE2(atlTraceCOM, TRACELEVEL_IMediaSample2Redir, _T("Host don't set\n"));
			return E_FAIL;
		}

		if(cbProperties == 0)
			return S_OK;

		ATLASSERT(pbProperties != NULL);
		if(pbProperties == NULL) return E_POINTER;

		//---------------------------------
		AM_SAMPLE2_PROPERTIES rgProps = {0};
		HRESULT hr = m_spMediaSample2Host->GetProperties(cbProperties, (BYTE*)&rgProps);
		if(SUCCEEDED(hr))
		{
			rgProps.dwSampleFlags &= ~(AM_SAMPLE_TIMEVALID | AM_SAMPLE_STOPVALID | AM_SAMPLE_MEDIATIMEVALID);
			rgProps.tStart = (REFERENCE_TIME)0;
			rgProps.tStop = (REFERENCE_TIME)0;
			memcpy(pbProperties, &rgProps, rgProps.cbData);
		}
		return hr;
	}
};

/////////////////////////////////////////////////////////////////////////////
template <class TAllocator, class TBase>
class CComSampleObject : public TBase
{
public:
	typedef TBase _BaseClass;
	typedef CComSampleObject<TAllocator, TBase>	_Smp;

	CComSampleObject(void* = NULL) : m_pAllocator(NULL) {}

	~CComSampleObject()
	{
		// Set refcount to 1 to protect destruction
		m_dwRef = 1;
		FinalRelease();

		if(m_pAllocator != NULL)
		{
			m_pAllocator->ClearSample(this);
			m_pAllocator = NULL;
		}

#ifdef _ATL_DEBUG_INTERFACES
		_Module.DeleteNonAddRefThunk(_GetRawUnknown());
#endif
		// Set refcount to 0 to protect second (and more) calls of destructor
		// We can comment this line: m_pAllocator = NULL is enough for fix problems
		// (with exception second and more calls of FinalRelease()).
		// But it's need for debugging.
		m_dwRef = 0;
	}

	STDMETHOD_(ULONG, AddRef)()
	{
		return InternalAddRef();
	}
	STDMETHOD_(ULONG, Release)()
	{
		ULONG l = InternalRelease();
		if(l == 0)
		{
			TAllocator* p = m_pAllocator;
			if(p != NULL)
			{
				// We have 2 calls to allocator: from ~CComSampleObject() -> m_pAllocator->ClearSample()
				// and p->ReturnSample() and we should lock allocator state
				p->Lock();
				p->GetUnknown()->AddRef();
			}

			this->~CComSampleObject();

			if(p != NULL)
			{
				p->ReturnSample(this);
				p->Unlock();
				p->GetUnknown()->Release();
			}
		}
		return l;
	}
	STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject)
	{
		return _InternalQueryInterface(iid, ppvObject);
	}
	template <class Q>
	HRESULT STDMETHODCALLTYPE QueryInterface(Q** pp)
	{
		return QueryInterface(__uuidof(Q), (void**)pp);
	}

	//---------------------------------
	void SetHost(TAllocator* pAllocator)
	{
		m_pAllocator = pAllocator;
	}
	void* operator new(size_t, _Smp* p)
	{
		return p;
	}
	void operator delete(void* /*pObj*/)	// For VC6
	{
		ATLASSERT(!_T("CComSampleObject::operator delete 1"));
	}
	void operator delete(void* /*pObj*/, _Smp* /*pDummy: for remove warning C4291 of VC7*/)
	{
		ATLASSERT(!_T("CComSampleObject::operator delete 2"));
	}

	//---------------------------------
	static HRESULT WINAPI InitInstance(_Smp* pSample, TAllocator* pAlloc, BYTE* pBuffer, ULONG cbBuffer,
		IMediaSample** ppMediaSample)
	{
		ATLASSERT(pSample != NULL && ppMediaSample != NULL);
		if(pSample == NULL || ppMediaSample == NULL) return E_POINTER;
		ATLASSERT(pAlloc != NULL && pBuffer != NULL && cbBuffer > 0);
		if(pAlloc == NULL || pBuffer == NULL || cbBuffer == 0) return E_INVALIDARG;

		//---------------------------------
		new(pSample) _Smp();

		pSample->SetHost(pAlloc);
		pSample->SetBuffer(pBuffer, cbBuffer);

		pSample->SetVoid(NULL);
		pSample->InternalFinalConstructAddRef();
		HRESULT hr = pSample->FinalConstruct();
		ATLASSERT(hr == S_OK && _T("CComSampleObject::InitInstance() : pSample->FinalConstruct() FAILED"));
		pSample->InternalFinalConstructRelease();

		pSample->GetUnknown()->AddRef();
		if(hr == S_OK)
		{
			hr = pSample->GetUnknown()->QueryInterface(ppMediaSample);
			ATLASSERT(hr == S_OK && _T("CComSampleObject::InitInstance() : pSample->QueryInterface() FAILED"));
		}
		pSample->GetUnknown()->Release();
		return hr;
	}

	static HRESULT WINAPI InitRedirector(_Smp* pSample, TAllocator* pAlloc, IUnknown* pHostSample,
		IMediaSample** ppMediaSample)
	{
		ATLASSERT(pSample != NULL && ppMediaSample != NULL);
		if(pSample == NULL || ppMediaSample == NULL) return E_POINTER;
		ATLASSERT(pAlloc != NULL && pHostSample != NULL);
		if(pAlloc == NULL || pHostSample == NULL) return E_INVALIDARG;

		//---------------------------------
		new(pSample) _Smp();

		pSample->SetHost(pAlloc);

		pSample->SetVoid(NULL);
		pSample->InternalFinalConstructAddRef();
		HRESULT hr = pSample->FinalConstruct();
		pSample->InternalFinalConstructRelease();

		pSample->GetUnknown()->AddRef();
		if(hr == S_OK)
			hr = pSample->SetMediaSample2Redirect(pHostSample);
		if(hr == S_OK)
			hr = pSample->GetUnknown()->QueryInterface(ppMediaSample);
		pSample->GetUnknown()->Release();
		return hr;
	}

protected:
	TAllocator*	m_pAllocator;
};

/////////////////////////////////////////////////////////////////////////////
// CMemAllocBase - class for linkage samples and buffers

template <class T, class TBuffers /*= CAllocatorBuffers<CBufferOnVirtualAlloc>*/, class TSample /*= CMediaSample*/>
class ATL_NO_VTABLE CMemAllocBase :
	public IMemAllocatorImpl<T>,
	public IMemAllocatorEx
{
//	COM_INTERFACE_ENTRY(IMemAllocator)
//	COM_INTERFACE_ENTRY(IMemAllocatorCallbackTemp)
//	COM_INTERFACE_ENTRY(IMemAllocatorEx)

	//---------------------------------
public:
	typedef CComSampleObject<T, TSample>		_SampleObject;
	typedef CMemAllocBase<T, TBuffers, TSample>	_AllocBase;

	virtual LPCTSTR GetClassName() const { return _T("CMemAllocBase"); }

	CMemAllocBase() : m_lWaiting(0) {}

	HRESULT InitAllocator()
	{
		return IMemAllocatorImpl<T>::Init();
	}
	void ReleaseAllocator()
	{
		IMemAllocatorImpl<T>::Reset();
		m_arrSamples.Clear();
		m_theBuffers.Clear();

		if(m_spNotify != NULL)
			m_spNotify.Release();
	}

	//---------------------------------
	// ATTENTION: this function is called in sample's destructor
	void ClearSample(_SampleObject* pSample)
	{
		T* pT = static_cast<T*>(this);
		T::ObjectLock lock(pT);

		BYTE* pBuffer = NULL;
		HRESULT hr = pSample->GetPointer(&pBuffer);
		if(hr == S_OK)
			m_theBuffers.PutBuffer(pBuffer);
		else
			ATLASSERT(!_T("pSample->GetPointer() != S_OK"));

		hr = pT->ReleaseBuffer(static_cast<IMediaSample*>(pSample));
		if(hr != S_OK)
			ATLASSERT(!_T("ReleaseBuffer() != S_OK"));
	}

	// ATTENTION: this function is called from CComSampleObject::Release()
	// after a call of object's destructor
	void ReturnSample(_SampleObject* pSample)
	{
		T* pT = static_cast<T*>(this);
		T::ObjectLock lock(pT);

		m_arrSamples.Put(pSample);

		if(m_eState == AS_Decommit && IsAllReturn())
		{
			// Decrease reference counter for compensate AddRef in Commit()
			// Reference counter AddRef'ed in CComSampleObject::Release()
			pT->GetUnknown()->Release();
			m_eState = AS_Prop;
#ifdef FREE_ON_DECOMMIT
			FreeMemory();
#endif
		}
		NotifySample();
	}

	HRESULT CreateSample(IMediaSample** ppSample)
	{
		ATLASSERT(ppSample != NULL);
		if(ppSample == NULL) return E_POINTER;

		*ppSample = NULL;

		T* pT = static_cast<T*>(this);
		T::ObjectLock lock(pT);

		BYTE* pBuffer = m_theBuffers.GetBuffer();
		// All samples are busy : this is normal situation
		if(pBuffer == NULL) return S_FALSE;

		_SampleObject* pSample = m_arrSamples.Get();
		if(pSample == NULL)
		{
			m_theBuffers.PutBuffer(pBuffer);
			return E_OUTOFMEMORY;
		}

		HRESULT hr = _SampleObject::InitInstance(pSample, pT, pBuffer, m_rgProps.cbBuffer, ppSample);
		if(hr == S_OK && m_theMediaType.IsValid())
		{
			HRESULT hrT = pSample->SetMediaType(&m_theMediaType);
			hrT;
			ATLASSERT(hrT == S_OK);
			m_theMediaType.Reset();
		}

		return hr;
	}

	HRESULT AdjustProperties(ALLOCATOR_PROPERTIES& rgProps)
	{
		return m_theBuffers.Correct(rgProps);
	}

	HRESULT Init()
	{
		ATLASSERT(m_eState == AS_Prop);
		if(m_eState != AS_Prop) return E_ABORT;

		return (m_arrSamples.Init(m_rgProps.cBuffers) && m_theBuffers.Init(m_rgProps))? S_OK : E_OUTOFMEMORY;
	}

	DWORD NumFreeBuffers() const
	{
		return m_theBuffers.NumFreeBuffers();
	}
	bool IsAllReturn() const
	{
		return m_theBuffers.IsAllReturn();
	}

    // Notifies that we're waiting for a sample
    void SetWaiting()
	{
		++m_lWaiting;
	}

	void NotifySample()
	{
		if(m_lWaiting != 0)
		{
			ATLASSERT(m_hSem != NULL);
			::ReleaseSemaphore(m_hSem, m_lWaiting, 0);
			m_lWaiting = 0;
		}
	}

	void FreeMemory()
	{
		ATLASSERT(m_eState == AS_Prop);
		m_arrSamples.Clear();
		m_theBuffers.Clear();
	}

	//---------------------------------
	// IMemAllocatorEx: extention interface for memory allocators

	// Attach new media type for next sample
	STDMETHOD(SetMediaType)(IN const AM_MEDIA_TYPE* pmt)
	{
		T* pT = static_cast<T*>(this);

		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocator,
			_T("%s: IMemAllocatorEx::SetMediaType() [0x%08X]\n"), GetClassName(), pT);
		Debug_MediaType(atlTraceGeneral, TRACELEVEL_IMemAllocator, pmt);

		T::ObjectLock lock(pT);
		HRESULT hr = m_theMediaType.Set(pmt);
		ATLASSERT(hr == S_OK);
		return hr;
	}

	//---------------------------------
protected:

	TBuffers					m_theBuffers;		// Container for memory buffers
	CSampleArray<_SampleObject> m_arrSamples;		// Container for free samples
    long						m_lWaiting;			// Waiting for a free element
	CMType						m_theMediaType;		// Mediatype for next requested sample
};

/////////////////////////////////////////////////////////////////////////////
// CMemAllocTrans

template <class T, class TBuffers /*= CAllocatorBuffers<CBufferOnVirtualAlloc>*/, class TSample /*= CMediaSampleCashe*/>
class ATL_NO_VTABLE CMemAllocTrans :
	public CMemAllocBase<T, TBuffers, TSample>,
	public IMemAllocTrans
{
//	COM_INTERFACE_ENTRY(IMemAllocator)
//	COM_INTERFACE_ENTRY(IMemAllocatorCallbackTemp)
//	COM_INTERFACE_ENTRY(IMemAllocatorEx)
//	COM_INTERFACE_ENTRY(IMemAllocTrans)

	CComPtr<IMemAllocHostCallback>	m_spMemAllocHostCallback;

protected:
	void SetHost(IMemAllocHostCallback* pHost)
	{
		m_spMemAllocHostCallback = pHost;
	}

public:
	virtual LPCTSTR GetClassName() const { return _T("CMemAllocTrans"); }

	void ReleaseAllocator()
	{
		_AllocBase::ReleaseAllocator();
		m_spMemAllocHostCallback.Release();
	}

	//---------------------------------
	// IMemAllocator overriding
	STDMETHOD(GetBuffer)(OUT IMediaSample **ppSample,
		IN REFERENCE_TIME* pStartTime, IN REFERENCE_TIME* pEndTime, IN DWORD dwFlags)
	{
		HRESULT hr = IMemAllocatorImpl<T>::GetBuffer(ppSample, pStartTime, pEndTime, dwFlags);
		if(hr == S_OK)
		{
			T::ObjectLock lock(static_cast<T*>(this));
			if(m_spMemAllocHostCallback != NULL)
				m_spMemAllocHostCallback->OnGetSample(*ppSample);
		}
		return hr;
	}

	//---------------------------------
	// IMemAllocTrans
	// Break link with host object
	STDMETHOD(BreakLink)(IN IMemAllocHostCallback* pHost)
	{
		T* pT = static_cast<T*>(this);

		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocator,
			_T("%s: IMemAllocTrans::BreakLink( 0x%X ) [0x%08X]\n"), GetClassName(), pHost, pT);

		//---------------------------------
		T::ObjectLock lock(pT);

		if(pHost == NULL)
			return E_INVALIDARG;
		if(!m_spMemAllocHostCallback)
			return S_OK;
		if(!m_spMemAllocHostCallback.IsEqualObject(pHost))
			return E_INVALIDARG;

		m_spMemAllocHostCallback.Release();
		return S_OK;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CMemoryAllocator

template <class TMemory>
class ATL_NO_VTABLE CMemoryAllocatorT :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMemoryAllocatorT<TMemory> >,
	public CMemAllocBase<CMemoryAllocatorT<TMemory>, CAllocatorBuffers<TMemory> >
{
public:
	virtual LPCTSTR GetClassName() const { return _T("CMemoryAllocatorT"); }

	HRESULT FinalConstruct()
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_High,
			_T("CMemoryAllocatorT::FinalConstruct [0x%08X]\n"), this);
		return _AllocBase::InitAllocator();
	}
	void FinalRelease()
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_High,
			_T("CMemoryAllocatorT::FinalRelease [0x%08X]\n"), this);
		_AllocBase::ReleaseAllocator();
	}

	ULONG InternalRelease()
	{
		ULONG n = CComObjectRootEx<CComMultiThreadModel>::InternalRelease();
		if(n == 1 && m_eState == AS_Commit)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### CMemoryAllocatorT::InternalRelease(): ")
				_T("IMemAllocator::Decommit() isn't called, memory isn't freed\n"));
		}
		return n;
	}

	//---------------------------------
	DECLARE_NOT_AGGREGATABLE(CMemoryAllocatorT<TMemory>)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CMemoryAllocatorT<TMemory>)
		COM_INTERFACE_ENTRY(IMemAllocator)
		COM_INTERFACE_ENTRY(IMemAllocatorCallbackTemp)
		COM_INTERFACE_ENTRY(IMemAllocatorEx)
	END_COM_MAP()

	//---------------------------------
	static HRESULT CreateAllocatorInstance(IMemAllocator** ppAllocator)
	{
		ATLASSERT(ppAllocator != NULL);
		if(ppAllocator == NULL) return E_POINTER;

		*ppAllocator = NULL;

		CMemoryAllocatorT* pObject = NULL;
		HRESULT hr = DVCreator<CMemoryAllocatorT<TMemory> >::Create(&pObject);
		if(hr == S_OK)
		{
			hr = pObject->GetUnknown()->QueryInterface(__uuidof(IMemAllocator), (void**)ppAllocator);
			pObject->GetUnknown()->Release();
		}
		return hr;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CTransMemoryAllocator

class ATL_NO_VTABLE CTransMemoryAllocator :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CTransMemoryAllocator>,
	public CMemAllocTrans<CTransMemoryAllocator>
{
public:
	virtual LPCTSTR GetClassName() const { return _T("CTransMemoryAllocator"); }

	HRESULT FinalConstruct()
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_High,
			_T("CTransMemoryAllocator::FinalConstruct [0x%08X]\n"), this);
		return CMemAllocTrans<CTransMemoryAllocator>::InitAllocator();
	}
	void FinalRelease()
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_High,
			_T("CTransMemoryAllocator::FinalRelease [0x%08X]\n"), this);
		CMemAllocTrans<CTransMemoryAllocator>::ReleaseAllocator();
	}

	ULONG InternalRelease()
	{
		ULONG n = CComObjectRootEx<CComMultiThreadModel>::InternalRelease();
		if(n == 1 && m_eState == AS_Commit)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### CTransMemoryAllocator::InternalRelease(): ")
				_T("IMemAllocator::Decommit() isn't called, memory isn't freed\n"));
		}
		return n;
	}

	//---------------------------------
	DECLARE_NOT_AGGREGATABLE(CTransMemoryAllocator)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CTransMemoryAllocator)
		COM_INTERFACE_ENTRY(IMemAllocator)
		COM_INTERFACE_ENTRY(IMemAllocatorCallbackTemp)
		COM_INTERFACE_ENTRY(IMemAllocatorEx)
		COM_INTERFACE_ENTRY(IMemAllocTrans)
	END_COM_MAP()

	//---------------------------------
	static HRESULT CreateAllocatorInstance(IMemAllocator** ppAllocator, IMemAllocHostCallback* pHost = NULL)
	{
		ATLASSERT(ppAllocator != NULL);
		if(ppAllocator == NULL) return E_POINTER;

		*ppAllocator = NULL;

		CTransMemoryAllocator* pObject = NULL;
		HRESULT hr = DVCreator<CTransMemoryAllocator>::Create(&pObject);
		if(hr == S_OK)
		{
			hr = pObject->GetUnknown()->QueryInterface(__uuidof(IMemAllocator), (void**)ppAllocator);
			pObject->GetUnknown()->Release();
		}
		if(hr == S_OK)
			pObject->SetHost(pHost);
		return hr;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CMemAllocRedirector

class ATL_NO_VTABLE CMemAllocRedirector :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMemAllocRedirector>,
	public IMemAllocatorRedirect,
	public IDSXRedirector
{
public:
	virtual LPCTSTR GetClassName() const { return _T("CMemAllocRedirector"); }

	HRESULT FinalConstruct()
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_High,
			_T("CMemAllocRedirector::FinalConstruct\n"));
		return S_OK;
	}
	void FinalRelease()
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_High,
			_T("CMemAllocRedirector::FinalRelease\n"));
		SetMemAllocRedirect(NULL);
	}

	//---------------------------------
	DECLARE_NOT_AGGREGATABLE(CMemAllocRedirector)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CMemAllocRedirector)
		COM_INTERFACE_ENTRY(IMemAllocator)
		COM_INTERFACE_ENTRY(IDSXRedirector)
	END_COM_MAP()

	//---------------------------------
	// IDSXRedirector
	// Exposes host allocator
	STDMETHOD(Expose)(OUT IUnknown** ppHost)
	{
		ATLTRACE2(atlTraceCOM, 2, _T("%s: IDSXRedirector::ExposeE\n"), GetClassName());

		ATLASSERT(ppHost != NULL);
		if(ppHost == NULL) return E_POINTER;

		if(!m_spMemAllocHost) return E_UNEXPECTED;

		*ppHost = m_spMemAllocHost.p;
		(*ppHost)->AddRef();
		return S_OK;
	}

	STDMETHOD(Wrap)(IN REFIID /*iid*/, IN IUnknown* /*pObject*/, OUT IUnknown** /*ppWrapper*/)
	{
		ATLTRACENOTIMPL(_T("CMemAllocRedirector::Wrap"));
	}

	//---------------------------------
	static HRESULT CreateRedirector(IUnknown* pHost, IMemAllocator** ppAllocator)
	{
		ATLASSERT(pHost != NULL && ppAllocator != NULL);
		if(ppAllocator == NULL) return E_POINTER;

		*ppAllocator = NULL;

		//---------------------------------
		CComObject<CMemAllocRedirector>* pComObject = NULL;
		HRESULT hr = CComObject<CMemAllocRedirector>::CreateInstance(&pComObject);
		if(SUCCEEDED(hr))
		{
			pComObject->GetUnknown()->AddRef();
			hr = pComObject->SetMemAllocRedirect(pHost);
			if(hr == S_OK)
				hr = pComObject->QueryInterface(__uuidof(IMemAllocator), (void**)ppAllocator);
			pComObject->GetUnknown()->Release();
		}
		return hr;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CMemAllocSampleRedirector - redirector with duplication and change of samples
// TSample: see CPreviewMediaSample

template <class TSample>
class ATL_NO_VTABLE CMemAllocSampleRedirector :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMemAllocSampleRedirector<TSample> >,
	public IMemAllocatorRedirect,
	public IDSXRedirector
{
public:
	typedef CComSampleObject<CMemAllocSampleRedirector<TSample>, TSample>	_SampleObject;

	virtual LPCTSTR GetClassName() const { return _T("CMemAllocSampleRedirector"); }

	CMemAllocSampleRedirector() : m_bInited(false) {}

	HRESULT FinalConstruct()
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_High, _T("CMemAllocSampleRedirector::FinalConstruct\n"));
		return S_OK;
	}
	void FinalRelease()
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_High, _T("CMemAllocSampleRedirector::FinalRelease\n"));
		SetMemAllocRedirect(NULL);
		m_arrSamples.Clear();
	}

	//---------------------------------
	DECLARE_NOT_AGGREGATABLE(CMemAllocSampleRedirector)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CMemAllocSampleRedirector)
		COM_INTERFACE_ENTRY(IMemAllocator)
		COM_INTERFACE_ENTRY(IDSXRedirector)
	END_COM_MAP()

	//---------------------------------
	// IDSXRedirector
	// Exposes host allocator
	STDMETHOD(Expose)(OUT IUnknown** ppHost)
	{
		ATLTRACE2(atlTraceCOM, 2, _T("%s: IDSXRedirector::Expose\n"), GetClassName());

		ATLASSERT(ppHost != NULL);
		if(ppHost == NULL) return E_POINTER;

		if(!m_spMemAllocHost) return E_UNEXPECTED;

		*ppHost = m_spMemAllocHost.p;
		(*ppHost)->AddRef();
		return S_OK;
	}

	// Wraps up external sample
	STDMETHOD(Wrap)(IN REFIID iid, IN IUnknown* pObject, OUT IUnknown** ppWrapper)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocatorRedir_Low,
			_T("%s: IDSXRedirector::Wrap\n"), GetClassName());
		
		ATLASSERT(IsEqualGUID(iid, __uuidof(IMediaSample)) || IsEqualGUID(iid, __uuidof(IMediaSample2)));
		if(!IsEqualGUID(iid, __uuidof(IMediaSample)) && !IsEqualGUID(iid, __uuidof(IMediaSample2)))
			return E_INVALIDARG;

		ATLASSERT(pObject != NULL);
		if(pObject == NULL) return E_INVALIDARG;

		ATLASSERT(ppWrapper != NULL);
		if(ppWrapper == NULL) return E_POINTER;

		if(IsEqualGUID(iid, __uuidof(IMediaSample)))
		{
			CComQIPtr<IMediaSample> sp(pObject);
			if(!sp) return E_NOINTERFACE;
		}
		else
		{
			CComQIPtr<IMediaSample2> sp2(pObject);
			if(!sp2) return E_NOINTERFACE;
		}

		//---------------------------------
		*ppWrapper = NULL;
		CComPtr<IMediaSample> spSample;
		HRESULT hr = CreateSample(pObject, &spSample);
		if(hr == S_OK)
		{
			if(IsEqualGUID(iid, __uuidof(IMediaSample2)))
			{
				CComQIPtr<IMediaSample2> spSample2(spSample);
				ATLASSERT(spSample2 != NULL);
				if(!spSample2) return E_NOINTERFACE;

				*ppWrapper = spSample2.Detach();
			}
			else
				*ppWrapper = spSample.Detach();
		}
		return hr;
	}

	//---------------------------------
	// IMemAllocator

	// Calls of Commit()/Decommit() methods only redirected -
	// we don't know, how much AddRef()/Release() was called for extern allocator

	STDMETHOD(SetProperties)(IN ALLOCATOR_PROPERTIES *pRequest, OUT ALLOCATOR_PROPERTIES *pActual)
	{
		ATLASSERT(pRequest != NULL);
		if(pRequest == NULL) return E_POINTER;
		ATLASSERT(pActual != NULL);
		if(pActual == NULL) return E_POINTER;

		memset(pActual, 0, sizeof(ALLOCATOR_PROPERTIES));

		//---------------------------------
		ObjectLock lock(this);

		ALLOCATOR_PROPERTIES rgHost = {0};
		HRESULT hr = IMemAllocatorRedirect::GetProperties(&rgHost);
		if(hr == S_OK)
			MixProps(rgHost, *pRequest);

		hr = IMemAllocatorRedirect::SetProperties((hr == S_OK)? &rgHost : pRequest, pActual);
		if(hr == S_OK)
		{
			m_bInited = false;
			Init();
		}
		return hr;
	}

	STDMETHOD(GetBuffer)(OUT IMediaSample **ppSample,
						 IN REFERENCE_TIME* pStartTime, IN REFERENCE_TIME* pEndTime,
						 IN DWORD dwFlags)
	{
		ATLASSERT(ppSample != NULL);
		if(ppSample == NULL) return E_POINTER;

		*ppSample = NULL;

		//---------------------------------
		ObjectLock lock(this);

		CComPtr<IMediaSample> spHostSample;
		HRESULT hr = IMemAllocatorRedirect::GetBuffer(&spHostSample, pStartTime, pEndTime, dwFlags);
		if(hr == S_OK)
		{
			if(!m_bInited)
				hr = Init();
			if(hr == S_OK)
				hr = CreateSample(spHostSample, ppSample);
		}
		return hr;
	}

	//---------------------------------
	HRESULT CreateSample(IUnknown* pHostSample, IMediaSample** ppSample)
	{
		ATLASSERT(pHostSample != NULL);
		if(pHostSample == NULL) return E_INVALIDARG;
		ATLASSERT(ppSample != NULL);
		if(ppSample == NULL) return E_POINTER;

		*ppSample = NULL;

		ObjectLock lock(this);
		_SampleObject* pSample = m_arrSamples.Get();
		if(pSample == NULL)	return E_OUTOFMEMORY;
		
		HRESULT hr = _SampleObject::InitRedirector(pSample, this, pHostSample, ppSample);

		// ATTENTION : increasing reference count of allocator redirector object
		// Allocator redirector Release call on last Release() of our media sample :
		// CComSampleObject<>::Release() -> ReturnSample() -> allocator Release()
		if(hr == S_OK)
			GetUnknown()->AddRef();
		return hr;
	}

	// We don't initialization, only we wait for SetProperties() or first GetBuffer() calls
	static HRESULT CreateRedirector(IUnknown* pHost, IMemAllocator** ppAllocator)
	{
		ATLASSERT(pHost != NULL && ppAllocator != NULL);
		if(ppAllocator == NULL) return E_POINTER;

		*ppAllocator = NULL;

		//---------------------------------
		CComObject<CMemAllocSampleRedirector<TSample> >* pComObject = NULL;
		HRESULT hr = CComObject<CMemAllocSampleRedirector<TSample> >::CreateInstance(&pComObject);
		if(SUCCEEDED(hr))
		{
			pComObject->GetUnknown()->AddRef();
			hr = pComObject->SetMemAllocRedirect(pHost);
			if(hr == S_OK)
				hr = pComObject->GetUnknown()->QueryInterface(__uuidof(IMemAllocator), (void**)ppAllocator);
			pComObject->GetUnknown()->Release();
		}
		return hr;
	}

	//---------------------------------
	// ATTENTION: this function is called in sample's destructor
	void ClearSample(_SampleObject* /*pSample*/)
	{
		// Do nothing - our sample call Release() on allocator redirector object in destructor
	}
	// ATTENTION: this function is called after a call of object's destructor
	void ReturnSample(_SampleObject* pSample)
	{
		ObjectLock lock(this);
		m_arrSamples.Put(pSample);

		// ATTENTION : decreasing reference count of allocator redirector object
		GetUnknown()->Release();
	}

	//---------------------------------
private:

	HRESULT Init()
	{
		ATLASSERT(m_spMemAllocHost != NULL);
		if(!m_spMemAllocHost) return E_FAIL;

		//---------------------------------
		ObjectLock lock(this);

		ALLOCATOR_PROPERTIES rgProps = {0};
		HRESULT hr = m_spMemAllocHost->GetProperties(&rgProps);
		if(FAILED(hr))
			return hr;
		if(rgProps.cBuffers == 0)
			return VFW_E_SIZENOTSET;

		ATLTRACE2(atlTraceCOM, TRACELEVEL_IMemAllocatorRedir,
			_T("%s: Host allocator: rgProps = { %i,  %i,  %i,  %i }\n"), GetClassName(),
			rgProps.cBuffers, rgProps.cbBuffer, rgProps.cbAlign, rgProps.cbPrefix);

		if(m_arrSamples.Capacity() < (DWORD)rgProps.cBuffers)
		{
			m_bInited = false;
			if(!m_arrSamples.Init(rgProps.cBuffers))
				return E_OUTOFMEMORY;
		}
		m_bInited = true;
		return S_OK;
	}

	void MixProps(ALLOCATOR_PROPERTIES& rgMix, const ALLOCATOR_PROPERTIES& rgWanted) const
	{
		if(rgMix.cBuffers < rgWanted.cBuffers)
			rgMix.cBuffers = rgWanted.cBuffers;
		if(rgMix.cbBuffer < rgWanted.cbBuffer)
			rgMix.cbBuffer = rgWanted.cbBuffer;
		if(rgMix.cbAlign < rgWanted.cbAlign)
			rgMix.cbAlign = rgWanted.cbAlign;
		if(rgMix.cbPrefix < rgWanted.cbPrefix)
			rgMix.cbPrefix = rgWanted.cbPrefix;
	}

	//---------------------------------
	bool m_bInited;
	CSampleArray<_SampleObject> m_arrSamples;		// Container for freed samples
};

/////////////////////////////////////////////////////////////////////////////
typedef enum PROP_MASK
{
	PM_START	= 0x1,		// m_tS is valid
	PM_END		= 0x2,		// m_tE is valid
	PM_POSITION	= 0x4,		// m_llMS & m_llME are valid
	PM_SYNCH	= 0x10,		// m_fSyncPoint is valid
	PM_PREROLL	= 0x20,		// m_fPreroll is valid
	PM_DISCONT	= 0x40,		// m_fDiscontinuity is valid
	PM_TYPEFL	= 0x80,		// m_dwTypeFlags (from AM_SAMPLE2_PROPERTIES) is valid
	PM_TYPE		= 0x100,	// m_pMT is valid
	PM_BUFF		= 0x1000,	// m_pbBuf & m_cbBuf is valid
	PM_ACTLEN	= 0x2000,	// m_lActual is valid
	PM_STREAMID	= 0x8000	// IMediaSample2, AM_SAMPLE2_PROPERTIES::dwStreamId is valid
};

#define PM_FLAGS	(PM_SYNCH | PM_PREROLL | PM_DISCONT | PM_TYPEFL)
#define PM_STANDARD	(PM_START | PM_END | PM_FLAGS | PM_ACTLEN)
#define PM_EXTENDED	(PM_STANDARD | PM_POSITION | PM_BUFF)
#define PM_ALL		0xFFFF

/////////////////////////////////////////////////////////////////////////////

#ifndef TRACELEVEL_Sample_Err
	#define TRACELEVEL_Sample_Err	2
#endif

#ifndef TRACELEVEL_Sample_MediaTimeErr
	#define TRACELEVEL_Sample_MediaTimeErr	2
#endif

#ifndef TRACELEVEL_Sample
	#define TRACELEVEL_Sample	8
#endif

#ifndef TRACELEVEL_Sample_Low
	#define TRACELEVEL_Sample_Low	(TRACELEVEL_Sample + 2)
#endif

//---------------------------------
class CSampleCopier
{
public:
	HRESULT CopyBuffer(IMediaSample* pDst, BYTE* pbSrc, LONG lActual, WORD wMask)
	{
		wMask &= PM_ACTLEN | PM_BUFF;

		if(wMask & PM_BUFF)
		{
			ATLASSERT(pbSrc != NULL);
			if(pbSrc == NULL) return E_INVALIDARG;

			// We should copy data from source buffer to desination.
			// Size of buffer is lActual, then PM_ACTLEN mask should be set also
			ATLASSERT(wMask & PM_ACTLEN);
			if((wMask & PM_ACTLEN) == 0) return E_INVALIDARG;
		}
		if((wMask & PM_ACTLEN) != 0 && lActual == 0)
		{
			wMask &= ~PM_BUFF;
		}
		if(wMask == 0)
			return S_OK;

		//---------------------------------
		HRESULT hr = S_OK;
		BYTE* pbDst = NULL;
		if(wMask & PM_ACTLEN)
		{
			if(lActual != 0)
			{
				hr = pDst->GetPointer(&pbDst);
				if(hr != S_OK || pbDst == NULL)
				{
					if(hr == S_OK)
					{
						ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
							_T("###Err### CSampleCopier::CopyBuffer(): GetPointer()=S_OK, pbDst==NULL\n"));
						ATLASSERT(!_T("CSampleCopier::CopyBuffer(): GetPointer()=S_OK, pbDst==NULL"));
					}
					else
					{
						ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
							_T("###Err### CSampleCopier::CopyBuffer(): GetPointer()=0x%X\n"), hr);
						ATLASSERT(!_T("CSampleCopier::CopyBuffer(): GetPointer() FAILED"));
					}
					return FAILED(hr)? hr : E_FAIL;
				}

				LONG lSize = 0;
				lSize = pDst->GetSize();
				if(lSize < lActual)
				{
					ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
						_T("###Err### CSampleCopier::CopyBuffer(): GetSize()=%i, lActual=%i\n"),
						lSize, lActual);
					ATLASSERT(!_T("CSampleCopier::CopyBuffer(): GetSize() < actual data length"));
					return E_FAIL;
				}
			}

			hr = pDst->SetActualDataLength(lActual);
			if(hr != S_OK)
			{
				ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
					_T("###Err### CSampleCopier::CopyBuffer(): SetActualDataLength( %i )=0x%X\n"),
					lActual, hr);
				ATLASSERT(!_T("CSampleCopier::CopyBuffer(): SetActualDataLength() FAILED"));
				return FAILED(hr)? hr : E_FAIL;
			}
		}

		if(wMask & PM_BUFF)
		{
			memcpy(pbDst, pbSrc, lActual);
		}
		return S_OK;
	}
};

//---------------------------------
// COverlayMixerCopier
//
// NOTES.
//
// 1. We can skip method for setting of actual buffer length of samples -
//    Overlay Mixer don't needed in this data.
//    From tests: WinDVD decoder doesn't set actual sample size. Tested with
//    actual buffer length = 0, length doesn't vary.
//
// 2. Buffer size and actual buffer size can be invalid in some cases (bugs in Overlay Mixer).
//	  We calculate the correct sizes of src/dst buffers
//    For sample, size of YV12 surface is the same, as must be for YVU9.
//	  Also sample properties can be invalid in case of WinDVD decoder after
//    Overlay Mixer allocator->Decommit()/Commit() for PAL DVD (height = 576)
//    (sequence of calls: "pause"->"play"->"pause"->"stop"->"pause"->"play")
//    After command Stop() WinDVD decoder the state of Overlay Mixer doesn't correct
//    and doesn't change sample height from 480 to 576 (sample buffer size also is low).
//	  For sample : real sample size = 829440 (720*576*2), but after stop command
//	  pOverlaySample->GetSize() == pOverlaySample->GetActualDataLength() == 691200 (720*480*2).
//
// For this reasons we not check the size of the buffer and actual data length of dest samples !!!

class COverlayMixerCopier
{
public:
	COverlayMixerCopier()
	{
		ClearOverlay();
	}

	void ClearOverlay()
	{
		m_fccType = FOURCC_UNK;
		m_nFormat = -1;
		m_nHeight = m_nWidthSrc = m_nWidthDst = 0;
		m_cbSrc = m_cbDst = 0;
	}

	HRESULT SetSource(const AM_MEDIA_TYPE* pmt)
	{
		return SetProperties(pmt, true);
	}
	HRESULT SetDestination(const AM_MEDIA_TYPE* pmt)
	{
		return SetProperties(pmt, false);
	}
	HRESULT SetProperties(const AM_MEDIA_TYPE* pmt, bool bSource)
	{
		ATLASSERT(pmt != NULL);
		if(pmt == NULL) return E_INVALIDARG;

		//---------------------------------
		FOURCC fccType = FOURCC_UNK;
		DWORD nWidth = 0;
		DWORD nHeight = 0;
		HRESULT hr = CMTHelp::MTParseVideo(pmt, &fccType, &nWidth, &nHeight);
		if(hr != S_OK)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
				_T("###Err### COverlayMixerCopier::SetProperties( %s ): ")
				_T("CMTHelp::MTParseVideo()=0x%X\n"), (bSource)? _T("SRC") : _T("DST"), hr);
			Debug_MediaType(atlTraceGeneral, TRACELEVEL_Sample_Err, pmt);

			ATLASSERT(!_T("COverlayMixerCopier::SetProperties(): MTParseVideo() FAILED"));
			return hr;
		}
		if(nWidth == 0 || nHeight == 0)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
				_T("###Err### COverlayMixerCopier::SetProperties( %s ): bad media type\n"),
				(bSource)? _T("SRC") : _T("DST"));
			Debug_MediaType(atlTraceGeneral, TRACELEVEL_Sample_Err, pmt);

			ATLASSERT(!_T("COverlayMixerCopier::SetProperties(): bad media type"));
			return VFW_E_INVALIDMEDIATYPE ;
		}

		int nFormat = CPictureFormats::Find(fccType);
		if(nFormat == -1)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
				_T("###Err### COverlayMixerCopier::SetProperties( %s ): ")
				_T("unsupported subtype %s\n"), (bSource)? _T("SRC") : _T("DST"), GUID2NAME(pmt->subtype));

			ATLASSERT(!_T("COverlayMixerCopier::SetProperties() - unsupported subtype"));
			return VFW_E_INVALIDSUBTYPE;
		}

		if((LONG)nHeight < 0)
			nHeight = (DWORD)(-(LONG)nHeight);

		if( m_fccType != FOURCC_UNK &&
			((bSource && m_nWidthDst != 0) || (!bSource && m_nWidthSrc != 0)) &&
			(fccType != m_fccType || nHeight != m_nHeight))
		{
			// The properties of a destination or source were already set.
			// If new subtype or height differs from current, then we reject
			// new properties. In this case for loading of new properties
			// it is necessary to clear this class with ClearOverlay()
			if(bSource)
			{
				ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
					_T("###Err### COverlayMixerCopier::SetProperties( %s ): ")
					_T("properties are different, source %s, height=%u, destination %s, height=%u\n"),
					(bSource)? _T("SRC") : _T("DST"), FCC2NAME(fccType), nHeight,
					FCC2NAME(m_fccType), m_nHeight);
			}
			else
			{
				ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
					_T("###Err### COverlayMixerCopier::SetProperties( %s ): ")
					_T("properties are different, source %s, height=%u, destination %s, height=%u\n"),
					(bSource)? _T("SRC") : _T("DST"), FCC2NAME(m_fccType), m_nHeight,
					FCC2NAME(fccType), nHeight);
			}

			ATLASSERT(!_T("COverlayMixerCopier::SetProperties() - properties are different"));
			return E_FAIL;
		}

		// We not compare fields m_nWidthDst and m_nWidthSrc - they can change

		// We not analyze fields VIDEOINFOHEADER2::rcSource and rcTarget -
		// I didn't see, that they should be used at work with Overlay Mixer //DKN

		//---------------------------------
		// Buffer size can be invalid in some cases (bugs in Overlay Mixer and decoders).
		// We calculate the correct sizes of src/dst buffers
		DWORD cbSize = 0;
		CPictureFormats::CalcImageSize(fccType, nWidth, nHeight, &cbSize);

		if(m_fccType == FOURCC_UNK)
		{
			m_fccType = fccType;
			m_nFormat = nFormat;
			m_nHeight = nHeight;
		}
		if(bSource)
		{
			m_nWidthSrc = nWidth;
			m_cbSrc = cbSize;
		}
		else
		{
			m_nWidthDst = nWidth;
			m_cbDst = cbSize;
		}
		return hr;
	}

	HRESULT CopyBuffer(IMediaSample* pDst, BYTE* pbSrc, LONG /*lActual*/, WORD wMask)
	{
		ATLASSERT(pDst != NULL);
		if(pDst == NULL) return E_INVALIDARG;

		// ATTENTION: We don't set actual data length for Overlay Mixer samples (see notes above)
		if((wMask & PM_BUFF) == 0)
			return S_OK;

		ATLASSERT(pbSrc != NULL);
		if(pbSrc == NULL) return E_INVALIDARG;

		if(m_fccType == FOURCC_UNK || m_nHeight == 0 || m_nWidthSrc == 0 || m_nWidthDst == 0)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
				_T("###Err### COverlayMixerCopier::CopyBuffer(): ")
				_T("fccType=%s, nWidthSrc=%u, nWidthDst=%u, nHeight=%u\n"),
				FCC2NAME(m_fccType), m_nWidthSrc, m_nWidthDst, m_nHeight);
			ATLASSERT(!_T("COverlayMixerCopier::CopyBuffer(): bad parameters"));
			return E_FAIL;
		}

		//---------------------------------
		BYTE* pbDst = NULL;
		HRESULT hr = pDst->GetPointer(&pbDst);
		if(hr != S_OK || pbDst == NULL)
		{
			if(hr == S_OK)
			{
				ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
					_T("###Err### COverlayMixerCopier::CopyBuffer(): GetPointer()=S_OK, pbDst==NULL\n"));
				ATLASSERT(!_T("COverlayMixerCopier::CopyBuffer(): GetPointer()=S_OK, pbDst==NULL"));
			}
			else
			{
				ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
					_T("###Err### COverlayMixerCopier::CopyBuffer(): GetPointer()=0x%X\n"), hr);
				ATLASSERT(!_T("CSampleCopier::CopyBuffer(): GetPointer() FAILED"));
			}
			return FAILED(hr)? hr : E_FAIL;
		}

		//---------------------------------
		hr = CPictureFormats::CopyData(pbDst, pbSrc, m_nFormat, m_nWidthSrc, m_nHeight, m_nWidthDst);
		if(hr != S_OK)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
				_T("###Err### COverlayMixerCopier::CopyBuffer(): CPictureFormats::CopyData()=0x%X\n"), hr);
			ATLASSERT(!_T("COverlayMixerCopier::CopyBuffer(): CPictureFormats::CopyData() FAILED"));
		}

		return hr;
	}

	//---------------------------------
	int		m_nFormat;			// Format's index of both pictures
	FOURCC	m_fccType;			// FCC type of both pictures
	DWORD	m_nHeight;			// Height of both pictures in pixel
	DWORD	m_nWidthSrc;		// Alignment width of source picture in pixel
	DWORD	m_nWidthDst;		// Alignment width of destination picture in pixel
	DWORD	m_cbSrc;			// Buffer size for source
	DWORD	m_cbDst;			// Buffer size for destination
};

//---------------------------------
template <class T>
class CSampleT : public T
{
public:
	CSampleT()
	{
		m_pMT = NULL;
		Clear();
	}
	~CSampleT()
	{
		MTFree(m_pMT);
	}

	CSampleT& operator=(const CSampleT<T>& smp)
	{
		if(&smp != this)
		{
			::MTFree(m_pMT);

			memcpy(this, &smp, sizeof(*this));
			if(m_pMT != NULL)
				m_pMT = MTDup(m_pMT);
		}
		return *this;
	}

	void Clear()
	{
		m_wProps = 0;
		m_tS = m_tE = (REFERENCE_TIME)0;
		m_llMS = m_llME = (LONGLONG)0;
		m_fSync = m_fPreroll = m_fDiscont = false;
		m_dwTypeFlags = 0;
		::MTFree(m_pMT);
		m_pMT = NULL;
		m_lActual = m_cbBuf = 0;
		m_pbBuf = NULL;
		m_dwStreamID = AM_STREAM_MEDIA;
	}

	//---------------------------------
	// Get sample from allocator and put all valid properties of sample
	// to internal variables
//	HRESULT GetSample(IMemAllocator* pAllocator, IMediaSample** ppSample, DWORD dwAllocatorFlags = 0);

	//---------------------------------
	// Initializes an object from sample pSrc
	//
	// Return:
	//	S_OK: if(wMask == PM_ALL): at least one property was loaded;
	//		  else: all wanted properties was loaded
	//	S_FALSE (only if wMask != PM_ALL): at least one wanted property was loaded
	//	FAILED(hr): some errors
	HRESULT Load(IMediaSample* pSrc, WORD wMask = PM_ALL);
	
	// Saves an object into pDst sample accordingly wMask & m_wProps
	HRESULT Save(IMediaSample* pDst, WORD wMask = PM_ALL);

	// Copies sample pSrc to an object and to sample pDst
	HRESULT Copy(IMediaSample* pDst, IMediaSample* pSrc, WORD wMask = PM_ALL)
	{
		HRESULT hrL = Load(pSrc, wMask);
		if(SUCCEEDED(hrL))
		{
			HRESULT hrS = Save(pDst, (WORD)(wMask & m_wProps));
			if(FAILED(hrS) || (hrL == S_OK && hrS != S_OK))
				hrL = hrS;
		}
		return hrL;
	}

	//---------------------------------
	bool Has(WORD wProps) const
	{
		// [DKN, 2008-09-29]: Commented: if sample has media type - false returned
//		wProps &= PM_EXTENDED;
		return (wProps != 0 && (m_wProps & wProps) == wProps);
	}
	void Reset(WORD wProps)
	{
		m_wProps &= ~wProps;
	}

	//---------------------------------
	void SetTime(REFERENCE_TIME tS, REFERENCE_TIME tE = (REFERENCE_TIME)MINLONGLONG)
	{
		ATLASSERT(tE == (REFERENCE_TIME)MINLONGLONG || tE > tS);

		m_tS = tS;
		m_tE = tE;

		m_wProps |= PM_START;
		if(m_tE != (REFERENCE_TIME)MINLONGLONG)
			m_wProps |= PM_END;
		else
			m_wProps &= ~PM_END;
	}
	void SetMedia(LONGLONG llMS, LONGLONG llME)
	{
		ATLASSERT(llME > llMS);
		
		m_llMS = llMS;
		m_llME = llME;
		m_wProps |= PM_POSITION;
	}

	void SetSync(BOOL fSync)
	{
		m_fSync = fSync;
		m_wProps |= PM_SYNCH;
	}
	void SetPreroll(BOOL fPreroll)
	{
		m_fPreroll = fPreroll;
		m_wProps |= PM_PREROLL;
	}
	void SetDiscont(BOOL fDiscont)
	{
		m_fDiscont = fDiscont;
		m_wProps |= PM_DISCONT;
	}
	void SetFlags(BOOL fSync, BOOL fPreroll, BOOL fDiscont)
	{
		m_fSync = fSync;
		m_fPreroll = fPreroll;
		m_fDiscont = fDiscont;
		m_wProps |= PM_SYNCH | PM_PREROLL | PM_DISCONT;
	}
	void SetTypeFlags(DWORD dwTypeFlags)
	{
		m_dwTypeFlags = dwTypeFlags;
		m_wProps |= PM_TYPEFL;
	}
	void SetStreamID(DWORD dwStreamID)
	{
		m_dwStreamID = dwStreamID;
		m_wProps |= PM_STREAMID;
	}
	HRESULT SetMediaType(const AM_MEDIA_TYPE* pMT)
	{
		MTFree(m_pMT);
		m_pMT = NULL;
		m_wProps &= ~PM_TYPE;

		if(pMT != NULL)
		{
			m_pMT = MTDup(pMT);
			if(m_pMT == NULL)
				return E_OUTOFMEMORY;

			m_wProps |= PM_TYPE;
		}
		return S_OK;
	}

	// In some cases we must set only ActualDataLength = 0
	void SetEmptyBuffer()
	{
		SetBuffer(NULL, 0);
	}
	// Sets the external buffer for copying to dest sample
	void SetBuffer(BYTE* pbBuf, LONG cbBuf)
	{
		ATLASSERT((pbBuf == NULL && cbBuf == 0) || (pbBuf != NULL && cbBuf > 0));

		m_pbBuf = pbBuf;
		if(m_pbBuf == NULL)
		{
			m_lActual = m_cbBuf = 0;
			m_wProps &= ~PM_BUFF;
		}
		else
		{
			m_lActual = m_cbBuf = cbBuf;
			m_wProps |= PM_BUFF;
		}
		m_wProps |= PM_ACTLEN;
	}

	void SetActualLen(LONG lActual)
	{
		m_lActual = lActual;
		m_wProps |= PM_ACTLEN;
	}

	//---------------------------------
	WORD			m_wProps;		// Composition of PROP_MASK for the loaded properties

	REFERENCE_TIME	m_tS;			// PM_START
	REFERENCE_TIME	m_tE;			// PM_END
	LONGLONG		m_llMS;			// PM_POSITION
	LONGLONG		m_llME;			// PM_POSITION
	BOOL			m_fSync;		// PM_SYNCH
	BOOL			m_fPreroll;		// PM_PREROLL
	BOOL			m_fDiscont;		// PM_DISCONT
	DWORD			m_dwTypeFlags;	// PM_TYPEFL
	AM_MEDIA_TYPE*	m_pMT;			// PM_TYPE
	LONG			m_lActual;		// PM_ACTLEN
	BYTE*			m_pbBuf;		// PM_BUFF
	LONG			m_cbBuf;		// PM_BUFF
	DWORD			m_dwStreamID;	// PM_STREAMID
};

typedef CSampleT<CSampleCopier>			CSample;
typedef CSampleT<COverlayMixerCopier>	CSampleOverlay;

//---------------------------------
// Get sample from allocator and put all valid properties of sample
// to internal variables
//template <class T>
//HRESULT CSampleT<T>::GetSample(IMemAllocator* pAllocator, IMediaSample** ppSample,
//							   DWORD dwAllocatorFlags /*= 0*/)
/*{
	ATLASSERT(pAllocator != NULL);
	if(pAllocator == NULL) return E_INVALIDARG;
	ATLASSERT(ppSample != NULL);
	if(ppSample == NULL) return E_POINTER;

	//---------------------------------
	*ppSample = NULL;

	HRESULT hr = pAllocator->GetBuffer(ppSample, NULL, NULL, dwAllocatorFlags);
	if(hr != S_OK)
	{
		if((dwAllocatorFlags & AM_GBF_NOWAIT) != 0 && hr == VFW_E_TIMEOUT)
		{
			ATLTRACE2(atlTraceGeneral, 10, _T("###Err### CSampleT::GetSample(): ")
				_T("pAllocator->GetBuffer()=VFW_E_TIMEOUT\n"));
		}
		else
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
				_T("###Err### CSampleT::GetSample(): pAllocator->GetBuffer()=0x%X\n"), hr);
		}
		return FAILED(hr)? hr : E_FAIL;
	}

	// Can be valid only PM_TYPE, PM_TYPEFL or PM_BUFF data
	Load(*ppSample, PM_TYPE | PM_TYPEFL | PM_BUFF);

	return (m_wProps != 0)? S_OK : S_FALSE;
}
*/

//---------------------------------
// Initializes an object from sample pSrc
//
//	S_OK: if(wMask == PM_ALL): at least one property was loaded;
//		  else: all wanted properties was loaded
//	S_FALSE (only if wMask != PM_ALL): at least one wanted property was loaded
//	FAILED(hr): some errors

template <class T>
HRESULT CSampleT<T>::Load(IMediaSample* pSrc, WORD wMask /*= PM_ALL*/)
{
	ATLASSERT(pSrc != NULL);
	if(pSrc == NULL) return E_INVALIDARG;
	ATLASSERT(wMask != 0);
	if(wMask == 0) return E_INVALIDARG;

	Clear();

	//---------------------------------
	HRESULT hr = S_FALSE;

	if(wMask & (PM_START | PM_END))
	{
		hr = pSrc->GetTime(&m_tS, &m_tE);
		if(hr == S_OK)
		{
			m_wProps |= (wMask & (PM_START | PM_END));

			if(m_tE == m_tS)
			{
				ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample,
					_T("WARNING. CSampleT::Load(): GetTime(): m_tE==m_tS=%s\n"), RT2STR(m_tS));

				// ATTENTION.
				// In some cases source filter for Winnow Videum 4400 capture card
				// sends samples with m_tE == m_tS. Make correction
				m_tE = m_tS + 1;
			}
			else if(m_tS > m_tE)
			{
				ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err, _T("###Err### CSampleT::Load(): ")
					_T("GetTime(): m_tS > m_tE,  m_tS=%s  m_tE=%s\n"), RT2STR(m_tS), RT2STR(m_tE));
				ATLASSERT(!_T("CSampleT::Load(): GetTime(): start time > end time "));

				m_tE = m_tS + 1;
			}
		}
		else if(hr == VFW_S_NO_STOP_TIME)
		{
			m_wProps |= (wMask & PM_START);
		}
		else
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Low,
				_T("WARNING. CSampleT::Load(): GetTime()=0x%X\n"), hr);
		}
	}

	if(wMask & PM_POSITION)
	{
		hr = pSrc->GetMediaTime(&m_llMS, &m_llME);
		if(hr == S_OK)
		{
			m_wProps |= PM_POSITION;

			if(m_llME == m_llMS)
			{
				// We can't correct media times, only put message to log file
				// MainConcept MPEG-2 Video Decoder mcm2vd.ax (1.0.241.61005) set this bad media times
				ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_MediaTimeErr, _T("###Err### CSampleT::Load(): ")
					_T("GetMediaTime(): m_llME==m_llMS=%s\n"), N64STR(m_llMS));
			}
			else if(m_llMS > m_llME)
			{
				ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err, _T("###Err### CSampleT::Load(): ")
					_T("GetMediaTime(): m_llMS > m_llME, m_llMS=%s m_llME=%s\n"), N64STR(m_llMS), N64STR(m_llME));
				ATLASSERT(!_T("CSampleT::Load(): GetMediaTime(): start media time > end media time"));
			}
		}
		else
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Low,
				_T("WARNING. CSampleT::Load(): GetMediaTime()=0x%X\n"), hr);
		}
	}

	//---------------------------------
	if(wMask & PM_SYNCH)
	{
		hr = pSrc->IsSyncPoint();
		if(hr == S_OK || hr == S_FALSE)
		{
			m_fSync = (hr == S_OK)? TRUE : FALSE;
			m_wProps |= PM_SYNCH;
		}
		else
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
				_T("###Err### CSampleT::Load(): IsSyncPoint()=0x%X\n"), hr);
			ATLASSERT(!_T("CSampleT::Load(): IsSyncPoint() FAILED"));
			return FAILED(hr)? hr : E_FAIL;
		}
	}

	if(wMask & PM_PREROLL)
	{
		hr = pSrc->IsPreroll();
		if(hr == S_OK || hr == S_FALSE)
		{
			m_fPreroll = (hr == S_OK)? TRUE : FALSE;
			m_wProps |= PM_PREROLL;
		}
		else
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
				_T("###Err### CSampleT::Load(): IsPreroll()=0x%X\n"), hr);
			ATLASSERT(!_T("CSampleT::Load(): IsPreroll() FAILED"));
			return FAILED(hr)? hr : E_FAIL;
		}
	}

	if(wMask & PM_DISCONT)
	{
		hr = pSrc->IsDiscontinuity();
		if(hr == S_OK || hr == S_FALSE)
		{
			m_fDiscont = (hr == S_OK)? TRUE : FALSE;
			m_wProps |= PM_DISCONT;
		}
		else
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
				_T("###Err### CSampleT::Load(): IsDiscontinuity()=0x%X\n"), hr);
			ATLASSERT(!_T("CSampleT::Load(): IsDiscontinuity() FAILED"));
			return FAILED(hr)? hr : E_FAIL;
		}
	}

	//---------------------------------
	if((wMask & PM_TYPEFL) != 0 || (wMask & PM_STREAMID) != 0)
	{
		CComQIPtr<IMediaSample2> spMS2(pSrc);
		if(spMS2 != NULL)
		{
			DWORD cbData = ((wMask & PM_STREAMID) == 0)? 2 * sizeof(DWORD) : 9 * sizeof(DWORD);

			AM_SAMPLE2_PROPERTIES rgProps = {0};
			rgProps.cbData = cbData;
			hr = spMS2->GetProperties(rgProps.cbData, (BYTE*)&rgProps);
			if(hr != S_OK || rgProps.cbData < cbData)
			{
				ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err, _T("###Err### CSampleT::Load(): ")
					_T("spMS2->GetProperties()=0x%X, cbData=%u, rgProps.cbData=%u\n"),
					hr, cbData, rgProps.cbData);
				ATLASSERT(!_T("CSampleT::Load(): spMS2->GetProperties() FAILED"));
				return FAILED(hr)? hr : E_FAIL;
			}

			if(wMask & PM_TYPEFL)
			{
				m_dwTypeFlags = rgProps.dwTypeSpecificFlags;
				m_wProps |= PM_TYPEFL;
			}
			if(wMask & PM_STREAMID)
			{
				m_dwStreamID = rgProps.dwStreamId;
				m_wProps |= PM_STREAMID;
			}
		}
	}

	//---------------------------------
	if(wMask & PM_TYPE)
	{
		hr = pSrc->GetMediaType(&m_pMT);
		if(hr == S_OK && m_pMT != NULL)
		{
			m_wProps |= PM_TYPE;
		}
		else if(hr == S_OK)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
				_T("###Err### CSampleT::Load(): GetMediaType()=S_OK, pMT=NULL\n"));
			ATLASSERT(!_T("CSampleT::Load(): GetMediaType()=S_OK, pMT=NULL"));
			return E_FAIL;
		}
		else
			m_pMT = NULL;
	}

	//---------------------------------
	LONG lActual = 0;
	if(wMask & (PM_BUFF|PM_ACTLEN))
		lActual = pSrc->GetActualDataLength();

	if(wMask & PM_BUFF)
	{
		hr = pSrc->GetPointer(&m_pbBuf);
		if(hr == S_OK && m_pbBuf != NULL)
		{
			m_cbBuf = pSrc->GetSize();
			if(m_cbBuf != 0)
			{
				m_wProps |= PM_BUFF;
			}
			else
			{
				ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
					_T("###Err### CSampleT::Load(): GetPointer()/GetSize(), pbBuf=0x%X, cbBuf==0\n"), m_pbBuf);
				m_pbBuf = NULL;
			}
		}
		else if(hr == S_OK)
		{
			// This situation is possible: some samples from SubPicture pin of DVD Navigator
			// Parameters of several samples:
			//	tS/tE=? (0x80040249)  mS/mE=? (0x80040251) flags={100} TF=0x10000 al=0 s=0 p=0x00000000
			//	tS/tE=? (0x80040249)  mS/mE=? (0x80040251) flags={000} TF=0x2D0C0000 al=0 s=0 p=0x00000000
			//	tS/tE=? (0x80040249)  mS/mE=? (0x80040251) flags={000} TF=0x2D490000 al=0 s=0 p=0x00000000
			if(lActual != 0)
			{
				ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
					_T("###Err### CSampleT::Load(): GetPointer()=S_OK, pbBuf==NULL\n"));
			}
		}
		else
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
				_T("###Err### CSampleT::Load(): GetPointer()=0x%X\n"), hr);
			m_pbBuf = NULL;
		}
	}

	if(wMask & PM_ACTLEN)
	{
		if((m_wProps & PM_BUFF) == 0 || lActual <= m_cbBuf)
		{
			m_wProps |= PM_ACTLEN;
			m_lActual = lActual; 
		}
		else
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
				_T("###Err### CSampleT::Load(): GetActualDataLength()=%i, cbBuf=%i\n"), m_lActual, m_cbBuf);
			ATLASSERT(!_T("CSampleT::Load(): GetActualDataLength() > buffer size"));
			return E_FAIL;
		}
	}

	//---------------------------------
	if(m_wProps == 0)
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
			_T("###Err### CSampleT::Load(): Can't load wanted (0x%x) properties\n"), wMask);
		return E_FAIL;
	}

	if(wMask != PM_ALL && wMask != m_wProps)
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Low, _T("CSampleT::Load(): ")
			_T("Not all wanted properties was loaded: wanted 0x%X, loaded 0x%X\n"), wMask, m_wProps);
		return S_FALSE;
	}

	return S_OK;
}

// Saves an object into pDst sample accordingly wMask
template <class T>
HRESULT CSampleT<T>::Save(IMediaSample* pDst, WORD wMask /*= PM_ALL*/)
{
	ATLASSERT(pDst != NULL);
	if(pDst == NULL) return E_INVALIDARG;

	WORD wSave = (WORD)(wMask & m_wProps);
	if(wSave == 0)
	{
		if(m_wProps == 0)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
				_T("###Err### CSampleT::Save(): sample not loaded yet\n"));
			ATLASSERT(!_T("CSampleT::Save(): sample not loaded yet"));
			return E_FAIL;
		}

		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample, _T("CSampleT::Save(): ")
			_T("Can't save wanted properties: wanted 0x%X, exist 0x%X\n"), wMask, m_wProps);
		return S_FALSE;
	}

	//---------------------------------
	HRESULT hr = S_OK;

	// We should set this data before another fields !
	if((wSave & PM_TYPEFL) != 0 || (wSave & PM_STREAMID) != 0)
	{
		CComQIPtr<IMediaSample2> spMS2(pDst);
		if(!spMS2)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
				_T("###Err### CSampleT::Save(): QI( IMediaSample2 ) FAILED\n"));
			ATLASSERT(!_T("CSampleT::Save(): QI( IMediaSample2 ) FAILED"));
			return E_NOINTERFACE;
		}

		AM_SAMPLE2_PROPERTIES rgProps = {0};
		DWORD cbData = ((wSave & PM_STREAMID) == 0)? 2 * sizeof(DWORD) : 9 * sizeof(DWORD);

		// Get old data from sample
		rgProps.cbData = cbData;
		hr = spMS2->GetProperties(cbData, (BYTE*)&rgProps);
		if(hr != S_OK)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
				_T("###Err### CSampleT::Save(): spMS2->GetProperties()=0x%X\n"), hr);
			ATLASSERT(!_T("CSampleT::Save(): spMS2->GetProperties() FAILED"));
			return FAILED(hr)? hr : E_FAIL;
		}

		// Change only new fields
		rgProps.cbData = cbData;
		if(wSave & PM_TYPEFL)
			rgProps.dwTypeSpecificFlags = m_dwTypeFlags;
		if(wSave & PM_STREAMID)
			rgProps.dwStreamId = m_dwStreamID;

		hr = spMS2->SetProperties(cbData, (BYTE*)&rgProps);
		if(hr != S_OK)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
				_T("###Err### CSampleT::Save(): spMS2->SetProperties()=0x%X\n"), hr);
			ATLASSERT(!_T("CSampleT::Save(): spMS2->SetProperties() FAILED"));
			return FAILED(hr)? hr : E_FAIL;
		}
	}

	//---------------------------------
	if(wSave & PM_START)
	{
		if(wSave & PM_END)
			hr = pDst->SetTime(&m_tS, &m_tE);
		else
			hr = pDst->SetTime(&m_tS, NULL);
		if(hr != S_OK)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
				_T("###Err### CSampleT::Save(): SetTime()=0x%X\n"), hr);
			ATLASSERT(!_T("CSampleT::Save(): SetTime() FAILED"));
			return FAILED(hr)? hr : E_FAIL;
		}
	}

	if(wSave & PM_POSITION)
	{
		hr = pDst->SetMediaTime(&m_llMS, &m_llME);
		if(hr != S_OK)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
				_T("###Err### CSampleT::Save(): SetMediaTime()=0x%X\n"), hr);
			ATLASSERT(!_T("CSampleT::Save(): SetMediaTime() FAILED"));
			return FAILED(hr)? hr : E_FAIL;
		}
	}

	//---------------------------------
	if(wSave & PM_SYNCH)
	{
		hr = pDst->SetSyncPoint(m_fSync);
		if(hr != S_OK)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
				_T("###Err### CSampleT::Save(): SetSyncPoint()=0x%X\n"), hr);
			ATLASSERT(!_T("CSampleT::Save(): SetSyncPoint() FAILED"));
			return FAILED(hr)? hr : E_FAIL;
		}
	}

	if(wSave & PM_PREROLL)
	{
		hr = pDst->SetPreroll(m_fPreroll);
		if(hr != S_OK)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
				_T("###Err### CSampleT::Save(): SetPreroll()=0x%X\n"), hr);
			ATLASSERT(!_T("CSampleT::Save(): SetPreroll() FAILED"));
			return FAILED(hr)? hr : E_FAIL;
		}
	}

	if(wSave & PM_DISCONT)
	{
		hr = pDst->SetDiscontinuity(m_fDiscont);
		if(hr != S_OK)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
				_T("###Err### CSampleT::Save(): SetDiscontinuity()=0x%X\n"), hr);
			ATLASSERT(!_T("CSampleT::Save(): SetDiscontinuity() FAILED"));
			return FAILED(hr)? hr : E_FAIL;
		}
	}

	//---------------------------------
	if(wSave & PM_TYPE)
	{
		hr = pDst->SetMediaType(m_pMT);
		if(hr != S_OK)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Sample_Err,
				_T("###Err### CSampleT::Save(): SetMediaType()=0x%X\n"), hr);
			ATLASSERT(!_T("CSampleT::Save(): SetMediaType() FAILED"));
			return FAILED(hr)? hr : E_FAIL;
		}
	}

	//---------------------------------
	if(wSave & (PM_ACTLEN|PM_BUFF))
		hr = CopyBuffer(pDst, m_pbBuf, m_lActual, wSave);

	return hr;
}

/////////////////////////////////////////////////////////////////////////////
inline HRESULT __stdcall GetSampleProps(IMediaSample* pSample, SMP_PROPS* pSmpProps,
										BYTE** ppBuffer /*= NULL*/, LONG* pcbActual /*= NULL*/)
{
	ATLASSERT(pSample != NULL);
	if(pSample == NULL) return E_INVALIDARG;
	ATLASSERT(pSmpProps != NULL);
	if(pSmpProps == NULL) return E_INVALIDARG;

	memset(pSmpProps, 0, sizeof(SMP_PROPS));

	//---------------------------------
	HRESULT hr = S_OK;
	if(ppBuffer != NULL)
	{
		// NOTE. hr == S_OK and *ppBuffer = NULL is normal situation: sample is control sample
		hr = pSample->GetPointer(ppBuffer);
		if(hr != S_OK)
			*ppBuffer = NULL;
	}

	if(pcbActual != NULL)
		*pcbActual = pSample->GetActualDataLength();

	//---------------------------------
	// Time stamps
	REFERENCE_TIME tS = (REFERENCE_TIME)0;
	REFERENCE_TIME tE = (REFERENCE_TIME)0;
	hr = pSample->GetTime(&tS, &tE);
	if(hr == S_OK)
	{
		pSmpProps->dwFlags |= AM_SAMPLE_TIMEVALID | AM_SAMPLE_STOPVALID;
		pSmpProps->tS = tS;

		if(tE == tS)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Warning, _T("WARNING. ::GetSampleProps(): ")
				_T("pSample->GetTime(): tE==tS=%s\n"), RT2STR(tS));

			// ATTENTION.
			// In some cases source filter for Winnow Videum 4400 capture card
			// sends samples with m_tE == m_tS. Make correction
			tE = tS + 1;
		}
		else if(tS > tE)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### ::GetSampleProps(): ")
				_T("pSample->GetTime(): tS > tE, tS=%s tE=%s\n"), RT2STR(tS), RT2STR(tE));

			// MPEG Layer 3 decoder make it
//			ATLASSERT(!_T("GetSampleProps(): GetTime(): start time > end time"));

			tE = tS + 1;
		}

		pSmpProps->tE = tE;
	}
	else if(hr == VFW_S_NO_STOP_TIME)
	{
		pSmpProps->dwFlags |= AM_SAMPLE_TIMEVALID;
		pSmpProps->tS = tS;
	}

	//---------------------------------
	// Media times
	LONGLONG llMS = (LONGLONG)0;
	LONGLONG llME = (LONGLONG)0;
	hr = pSample->GetMediaTime(&llMS, &llME);
	if(hr == S_OK)
	{
		if(llMS < llME)
		{
			pSmpProps->dwFlags |= AM_SAMPLE_MEDIATIMEVALID;
			pSmpProps->llMS = llMS;
			pSmpProps->llME = llME;
		}
		if(llME == llMS)
		{
			// We can't correct media times, only put message to log file
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### ::GetSampleProps(): ")
				_T("pSample->GetMediaTime(): llME==llMS=%s\n"), N64STR(llMS));
		}
		else
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### ::GetSampleProps(): ")
				_T("pSample->GetMediaTime(): llMS > llME, llMS=%s llME=%s\n"), N64STR(llMS), N64STR(llME));
//			ATLASSERT(!_T("GetSampleProps(): GetMediaTime(): start media time > end media time"));
		}
	}

	//---------------------------------
	// Flags
	hr = pSample->IsSyncPoint();
	if(hr == S_OK)
		pSmpProps->dwFlags |= AM_SAMPLE_SPLICEPOINT;
	else if(hr != S_FALSE)
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error,
			_T("###Err### ::GetSampleProps(): pSample->IsSyncPoint()=0x%X\n"), hr);
	}

	hr = pSample->IsPreroll();
	if(hr == S_OK)
		pSmpProps->dwFlags |= AM_SAMPLE_PREROLL;
	else if(hr != S_FALSE)
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error,
			_T("###Err### ::GetSampleProps(): pSample->IsPreroll()=0x%X\n"), hr);
	}

	hr = pSample->IsDiscontinuity();
	if(hr == S_OK)
		pSmpProps->dwFlags |= AM_SAMPLE_DATADISCONTINUITY;
	else if(hr != S_FALSE)
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error,
			_T("###Err### ::GetSampleProps(): pSample->IsDiscontinuity()=0x%X\n"), hr);
	}

	return S_OK;
}

inline HRESULT __stdcall SetSampleProps(IMediaSample* pSample, SMP_PROPS* pSmpProps)
{
	ATLASSERT(pSample != NULL);
	if(pSample == NULL) return E_INVALIDARG;
	ATLASSERT(pSmpProps != NULL);
	if(pSmpProps == NULL) return E_INVALIDARG;

	//---------------------------------
	HRESULT hr = S_OK;
	if(pSmpProps->dwFlags & AM_SAMPLE_TIMEVALID)
	{
		REFERENCE_TIME* ptE = (pSmpProps->dwFlags & AM_SAMPLE_STOPVALID)? &pSmpProps->tE : NULL;
		hr = pSample->SetTime(&pSmpProps->tS, ptE);
		if(hr != S_OK)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error,
				_T("###Err### ::SetSampleProps(): pSample->SetTime()=0x%X\n"), hr);
			return FAILED(hr)? hr : E_FAIL;
		}
	}

	//---------------------------------
	if(pSmpProps->dwFlags & AM_SAMPLE_MEDIATIMEVALID)
	{
		hr = pSample->SetMediaTime(&pSmpProps->llMS, &pSmpProps->llME);
		if(hr != S_OK)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error,
				_T("###Err### ::SetSampleProps(): pSample->SetMediaTime()=0x%X\n"), hr);
			return FAILED(hr)? hr : E_FAIL;
		}
	}

	//---------------------------------
	hr = pSample->SetSyncPoint((pSmpProps->dwFlags & AM_SAMPLE_SPLICEPOINT)? TRUE : FALSE);
	if(hr != S_OK)
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error,
			_T("###Err### ::SetSampleProps(): pSample->SetSyncPoint()=0x%X\n"), hr);
		return FAILED(hr)? hr : E_FAIL;
	}

	hr = pSample->SetPreroll((pSmpProps->dwFlags & AM_SAMPLE_PREROLL)? TRUE : FALSE);
	if(hr != S_OK)
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error,
			_T("###Err### ::SetSampleProps(): pSample->SetPreroll()=0x%X\n"), hr);
		return FAILED(hr)? hr : E_FAIL;
	}

	hr = pSample->SetDiscontinuity((pSmpProps->dwFlags & AM_SAMPLE_DATADISCONTINUITY)? TRUE : FALSE);
	if(hr != S_OK)
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error,
			_T("###Err### ::SetSampleProps(): pSample->SetDiscontinuity()=0x%X\n"), hr);
		return FAILED(hr)? hr : E_FAIL;
	}
	
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
#endif	//__DSALLOCATORS_H_
