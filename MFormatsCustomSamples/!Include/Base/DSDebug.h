//---------------------------------------------------------------------------
// This file is part of the MediaLooks DirectShow Template Library (DSTL)
//
// DSDebug.h : set of debug functions
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

#ifndef __DSDEBUG_H_
#define __DSDEBUG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DSParsers.h"

/////////////////////////////////////////////////////////////////////////////
#ifdef DS_LOG_ENABLED

void __stdcall Debug_MediaSample2(DWORD_PTR category, UINT level, IMediaSample* pSample,
								  LPCTSTR pszPrefix = NULL, bool bExtended = false);
void __stdcall Debug_PinMediaTypes(DWORD_PTR category, UINT level, IUnknown* _pPinUnk, LPCTSTR pszPrefix = NULL);
void __stdcall Debug_MediaType(DWORD_PTR category, UINT level, const AM_MEDIA_TYPE* pmt, LPCTSTR pszPrefix = NULL);
void __stdcall Debug_WaveFormatEx(DWORD_PTR category, UINT level, const WAVEFORMATEX* pWaveFormat,
								  LPCTSTR pszPrefix = NULL);
void __stdcall Debug_BitmapInfoHeader(DWORD_PTR category, UINT level, const BITMAPINFOHEADER* pbmiHeader,
									  LPCTSTR pszPrefix = NULL);
void __stdcall Debug_DvInfo(DWORD_PTR category, UINT level, const DVINFO* pdvInfo, LPCTSTR pszPrefix = NULL);
void __stdcall Debug_Time(DWORD_PTR category, UINT level, IReferenceClock* pRefClock, LPCTSTR pszPrefix = NULL);
void __stdcall Debug_TimeDiff(DWORD_PTR category, UINT level, IReferenceClock* pRefClock, REFERENCE_TIME t,
							  LPCTSTR pszPrefix = NULL);
void __stdcall Debug_SampleLag(DWORD_PTR category, UINT level, IMediaSample* pSample, REFERENCE_TIME tStart,
							   IReferenceClock* pRefClock, LPCTSTR pszPrefix = NULL);
void __stdcall Debug_Quality(DWORD_PTR category, UINT level, Quality& q, LPCTSTR pszPrefix = NULL);
void __stdcall Debug_Allocator(DWORD_PTR category, UINT level, IMemAllocator *pAllocator,
							   LPCTSTR pszPrefix = NULL);
void __stdcall Debug_AllocatorProps(DWORD_PTR category, UINT level, ALLOCATOR_PROPERTIES* pProps,
									LPCTSTR pszPrefix = NULL);
bool __stdcall Debug_Dump(DWORD_PTR category, UINT level, UINT cb, const void* pv, LPCTSTR pszPrefix = NULL);
bool __stdcall Debug_DumpSample(DWORD_PTR category, UINT level, IMediaSample* pSample,
								LPCTSTR pszPrefix = NULL);
bool __stdcall Debug_FileDump(DWORD_PTR category, UINT level, HANDLE hFile, LONGLONG llPosition, DWORD cbData);
bool __stdcall Debug_AsyncReaderDump(DWORD_PTR category, UINT level, IAsyncReader* pAsyncReader,
									 LONGLONG llPosition, DWORD cbData);
bool __stdcall Debug_Stream(DWORD_PTR category, UINT level, IStream* pStm,
							bool bFromCurrentPosition = false, LPCTSTR pszPrefix = NULL);
void __stdcall Debug_MultiString(DWORD_PTR category, UINT level, LPTSTR pszString);
void __stdcall Debug_FilterFromPin(DWORD_PTR category, UINT level, LPCTSTR pszPrefix, IPin* pPin);
void __stdcall Debug_Filter(DWORD_PTR category, UINT level, LPCTSTR pszComment, REFCLSID rclsid,
							HRESULT hrCreate);
void __stdcall Debug_FilterInfo(DWORD_PTR category, UINT level, IBaseFilter* pFilter);
void __stdcall Debug_MixerPinConfig(DWORD_PTR category, UINT level, IMixerPinConfig2* pMPC);
void __stdcall Debug_AMStreamConfig(DWORD_PTR category, UINT level, IUnknown* pUnk, LPCTSTR pszPrefix = NULL);
void __stdcall Debug_KsPropertySet(DWORD_PTR category, UINT level, REFGUID guidPropSet,
								   DWORD dwPropID, DWORD cbInstanceData, DWORD cbPropData,
								   LPCTSTR pszPrefix1 = NULL, LPCTSTR pszPrefix2 = NULL);
void __stdcall Debug_KsDataFormat(DWORD_PTR category, UINT level, KSDATAFORMAT* pKsDataFormat,
								  LPCTSTR pszPrefix = NULL);
void __stdcall Debug_KsVBIInfoHeader(DWORD_PTR category, UINT level, KS_VBIINFOHEADER* pKsVBIIH,
									 LPCTSTR pszPrefix = NULL);
void __stdcall Debug_ColorControl(DWORD_PTR category, UINT level, const DDCOLORCONTROL& rgCC);

//---------------------------------
struct DebugSample
{
	DebugSample(bool bUse = false) 
	{
		memset(this, 0, sizeof(*this));
		m_bUse = bUse;
		m_bSet = false;
	}

	void Set(IMediaSample* pSample)
	{
		if(!m_bUse || pSample == NULL || m_pBuffer == NULL) return;

		//---------------------------------
		m_bSet = true;
		m_lBufLen = pSample->GetSize();
		m_lActualDataLen = pSample->GetActualDataLength();
		m_cSyncPoint = (BYTE)((pSample->IsSyncPoint() == S_OK)? 1 : 0);
		m_cPreroll = (BYTE)((pSample->IsPreroll() == S_OK)? 1 : 0);
		m_cDiscontinuity = (BYTE)((pSample->IsDiscontinuity() == S_OK)? 1 : 0);

		m_hrGetTime = pSample->GetTime(&m_tStart, &m_tEnd);
		m_hrGetMediaTime = pSample->GetMediaTime(&m_llMediaStart, &m_llMediaEnd);

		CComPtr<IMediaSample2> spSample2;
		m_hrQIMediaSample2 = pSample->QueryInterface(__uuidof(IMediaSample2), (void**)&spSample2);
		if(m_hrQIMediaSample2 == S_OK)
		{
			AM_SAMPLE2_PROPERTIES rgProps = {0};
			m_hrGetProps = spSample2->GetProperties(12, (BYTE*)&rgProps);
			if(m_hrGetProps == S_OK)
			{
				m_dwTypeSpecificFlags = rgProps.dwTypeSpecificFlags;
				m_dwSampleFlags = rgProps.dwSampleFlags;
			}
		}
		else
			m_hrGetProps = S_FALSE;
	}

	//---------------------------------
	bool			m_bUse;
	bool			m_bSet;

	BYTE*			m_pBuffer;
	LONG			m_lBufLen;
	LONG			m_lActualDataLen;
	BYTE			m_cSyncPoint;
	BYTE			m_cPreroll;
	BYTE			m_cDiscontinuity;

	HRESULT			m_hrGetTime;
    REFERENCE_TIME	m_tStart;
    REFERENCE_TIME	m_tEnd;

	HRESULT			m_hrGetMediaTime;
    REFERENCE_TIME	m_llMediaStart;
    REFERENCE_TIME	m_llMediaEnd;

	HRESULT			m_hrQIMediaSample2;
	HRESULT			m_hrGetProps;
	DWORD			m_dwTypeSpecificFlags;
	DWORD			m_dwSampleFlags;
};

inline void __stdcall Debug_MediaSample(DWORD_PTR category, UINT level, LPCTSTR pszPrefix, IMediaSample* pSample,
										DebugSample* pPrev = NULL, LPCTSTR pszSuffix = NULL)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return;
#endif
	if(level > ATL_TRACE_LEVEL) return;

	LPCTSTR pszB = (pszPrefix == NULL)? _T("\0"): pszPrefix;
	LPCTSTR pszE = (pszSuffix == NULL)? _T("\0"): pszSuffix;

	DebugSample theCurrent(true);

	if(pSample == NULL)
	{
		if(pPrev != NULL && pPrev->m_bUse)
			*pPrev = theCurrent;

		ATLTRACE2(category, level, _T("###Err### Sample %s: Empty sample pointer%s\n"), pszB, pszE);
		return;
	}

	HRESULT hr = pSample->GetPointer(&theCurrent.m_pBuffer);
	if(FAILED(hr))
	{
		if(pPrev != NULL && pPrev->m_bUse)
			*pPrev = theCurrent;

		ATLTRACE2(category, level,
			_T("###Err### Sample %s: GetPointer() FAILED, hr = 0x%X%s\n"), pszB, hr, pszE);
		return;
	}

	//---------------------------------
	theCurrent.Set(pSample);

	if(	pPrev == NULL ||
		!pPrev->m_bUse || !pPrev->m_bSet ||
		(theCurrent.m_lBufLen != pPrev->m_lBufLen) ||
		(theCurrent.m_lActualDataLen != pPrev->m_lActualDataLen) ||
		(theCurrent.m_cPreroll != pPrev->m_cPreroll) ||
		(theCurrent.m_cSyncPoint != pPrev->m_cSyncPoint) ||
		(theCurrent.m_cDiscontinuity != pPrev->m_cDiscontinuity))
	{
		ATLTRACE2(category, level,
			_T("Sample %s:  pBuf=0x%p  size=%i  actual=%i  flags={%u%u%u}\n"),
			pszB, theCurrent.m_pBuffer, theCurrent.m_lBufLen, theCurrent.m_lActualDataLen,
			theCurrent.m_cDiscontinuity, theCurrent.m_cPreroll, theCurrent.m_cSyncPoint);

		LPCTSTR psz_tS = NULL;
		LPCTSTR psz_tE = NULL;
		if(SUCCEEDED(theCurrent.m_hrGetTime))
		{
			psz_tS = RT2STR(theCurrent.m_tStart);
			psz_tE = RT2STR(theCurrent.m_tEnd);
		}

		if(FAILED(theCurrent.m_hrGetMediaTime))
		{
			if(psz_tS == NULL)
				ATLTRACE2(category, level, _T("Sample %s: TIME_NOT_SET%s\n"), pszB, pszE);
			else
				ATLTRACE2(category, level, _T("Sample %s:  tS=%s  tE=%s%s\n"), pszB, psz_tS, psz_tE, pszE);
		}
		else
		{
			if(psz_tS == NULL)
				ATLTRACE2(category, level, _T("Sample %s: TIME_NOT_SET  mS=%I64i  mE=%I64i%s\n"),
					pszB, theCurrent.m_llMediaStart, theCurrent.m_llMediaEnd, pszE);
			else
				ATLTRACE2(category, level,
					_T("Sample %s:  tS=%s  tE=%s  mS=%I64i  mE=%I64i%s\n"),
					pszB, psz_tS, psz_tE, theCurrent.m_llMediaStart, theCurrent.m_llMediaEnd, pszE);
		}
	}
	
	// pPrev->m_bSet = true !!!

	else if(FAILED(theCurrent.m_hrGetTime))
	{
		if(pPrev == NULL || theCurrent.m_pBuffer != pPrev->m_pBuffer)
		{
			ATLTRACE2(category, level, _T("Sample %s: TIME_NOT_SET  pBuf=0x%p%s\n"),
				pszB, theCurrent.m_pBuffer, pszE);
		}
		else
			ATLTRACE2(category, level, _T("###Err### Sample %s: TIME_NOT_SET%s\n"), pszB, pszE);
	}
	else
	{
		TCHAR szBuf[64];
		if(pPrev != NULL && theCurrent.m_pBuffer == pPrev->m_pBuffer)
			szBuf[0] = (TCHAR)'\0';
		else
			wsprintf(szBuf, _T("  pBuf=0x%X"), theCurrent.m_pBuffer);

		if(FAILED(theCurrent.m_hrGetMediaTime))
		{
			if(pPrev != NULL && pPrev->m_hrGetTime == S_OK)
			{
				ATLTRACE2(category, level, _T("Sample %s:  tS=%s  tE=%s  dur=%s%s%s\n"),
					pszB, RT2STR(theCurrent.m_tStart), RT2STR(theCurrent.m_tEnd),
					RT2STR(theCurrent.m_tStart - pPrev->m_tStart), szBuf, pszE);
			}
			else
			{
				ATLTRACE2(category, level, _T("Sample %s:  tS=%s  tE=%s%s%s\n"),
					pszB, RT2STR(theCurrent.m_tStart), RT2STR(theCurrent.m_tEnd), szBuf, pszE);
			}
		}
		else
		{
			if(pPrev != NULL && pPrev->m_hrGetTime == S_OK)
			{
				ATLTRACE2(category, level,
					_T("Sample %s:  tS=%s  tE=%s  dur=%s  mS=%I64i  mE=%I64i  %s%s\n"),
					pszB, RT2STR(theCurrent.m_tStart), RT2STR(theCurrent.m_tEnd),
					RT2STR(theCurrent.m_tStart - pPrev->m_tStart),
					theCurrent.m_llMediaStart, theCurrent.m_llMediaEnd, szBuf, pszE);
			}
			else
			{
				ATLTRACE2(category, level,
					_T("Sample %s:  tS=%s  tE=%s  mS=%I64i  mE=%I64i  %s%s\n"),
					pszB, RT2STR(theCurrent.m_tStart), RT2STR(theCurrent.m_tEnd),
					theCurrent.m_llMediaStart, theCurrent.m_llMediaEnd, szBuf, pszE);
			}
		}
	}

	//---------------------------------
	if(theCurrent.m_hrQIMediaSample2 == S_OK)
	{
		if(theCurrent.m_hrGetProps == S_OK)
		{
			if( pPrev == NULL ||
				!pPrev->m_bSet || pPrev->m_hrGetProps != S_OK ||
				(theCurrent.m_dwTypeSpecificFlags != pPrev->m_dwTypeSpecificFlags) ||
				(theCurrent.m_dwSampleFlags != pPrev->m_dwSampleFlags) )
			{
				ATLTRACE2(category, level,
					_T("IMediaSample2 %s: typeFlags=0x%X  smpFlags=0x%X\n"),
					pszB, theCurrent.m_dwTypeSpecificFlags, theCurrent.m_dwSampleFlags);
			}
		}
		else if(pPrev == NULL || !pPrev->m_bSet || pPrev->m_hrGetProps == S_OK)
		{
			ATLTRACE2(category, level, _T("###Err### IMediaSample2 %s: GetProperties()=0x%X\n"),
				pszB, theCurrent.m_hrGetProps);
		}
	}
	else if(pPrev != NULL && pPrev->m_bSet && pPrev->m_hrQIMediaSample2 == S_OK)
	{
		ATLTRACE2(category, level, _T("###Err### IMediaSample2 %s: QI()=0x%X\n"),
			pszB, theCurrent.m_hrQIMediaSample2);
	}

	if(pPrev != NULL && pPrev->m_bUse)
		*pPrev = theCurrent;
}

/////////////////////////////////////////////////////////////////////////////
inline void __stdcall Debug_MediaSample2(DWORD_PTR category, UINT level, IMediaSample* pSample,
										 LPCTSTR pszPrefix /*= NULL*/, bool bExtended /*= false*/)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return;
#endif
	if(level > ATL_TRACE_LEVEL) return;

	LPTSTR psz = _T("");
	if(pszPrefix != NULL)
	{
		DWORD nSym = lstrlen(pszPrefix);
		psz = (LPTSTR)_alloca((nSym + 1 + 1) * sizeof(TCHAR));
		lstrcpy(psz, pszPrefix);
		psz[nSym] = (TCHAR)' ';
		psz[nSym + 1] = 0;
	}

	if(pSample == NULL)
	{
		ATLTRACE2(category, level, _T("###Err### %sEmpty sample pointer\n"), psz);
		return;
	}

	//---------------------------------
    REFERENCE_TIME tS = (REFERENCE_TIME)0;
    REFERENCE_TIME tE = (REFERENCE_TIME)0;
	HRESULT hr = pSample->GetTime(&tS, &tE);

	TCHAR szT[64];
	if(hr == S_OK)
	{
		wsprintf(szT, _T("tS=%s  tE=%s"), RT2STR(tS), RT2STR(tE));
	}
	else if(hr == VFW_S_NO_STOP_TIME)
	{
		wsprintf(szT, _T("tS=%s  tE=? (0x%X)"), RT2STR(tS), hr);
	}
	else
	{
		wsprintf(szT, _T("tS/tE=? (0x%X)"), hr);
	}

	//---------------------------------
	LONGLONG llMS = (LONGLONG)0;
	LONGLONG llME = (LONGLONG)0;
	hr = pSample->GetMediaTime(&llMS, &llME);

	TCHAR szMT[64];
	if(hr == S_OK)
	{
		wsprintf(szMT, _T("mS=%s  mE=%s"), N64STR(llMS), N64STR(llME));
	}
	else
	{
		wsprintf(szMT, _T("mS/mE=? (0x%X)"), hr);
	}

	//---------------------------------
	int cSynch = (pSample->IsSyncPoint() == S_OK)? 1 : 0;
	int cPreroll = (pSample->IsPreroll() == S_OK)? 1 : 0;
	int cDiscont = (pSample->IsDiscontinuity() == S_OK)? 1 : 0;

	//---------------------------------
	TCHAR szTypeFlags[64] = _T("");
	CComQIPtr<IMediaSample2> spMS2(pSample);
	if(spMS2 != NULL)
	{
		AM_SAMPLE2_PROPERTIES rgProps = {0};
		rgProps.cbData = 9 * sizeof(DWORD);
		hr = spMS2->GetProperties(rgProps.cbData, (BYTE*)&rgProps);
		if(hr == S_OK)
			wsprintf(szTypeFlags, _T(" TF=0x%X sId=0x%X"), rgProps.dwTypeSpecificFlags, rgProps.dwStreamId);
	}

	//---------------------------------
	TCHAR szExt[64];
	if(bExtended)
	{
		BYTE* pBuf = NULL;
		hr = pSample->GetPointer(&pBuf);
		long cbSize = pSample->GetSize();
		long cbActLen = pSample->GetActualDataLength();

		if(hr == S_OK)
			wsprintf(szExt, _T(" al=%i s=%i p=0x%p"), cbActLen, cbSize, pBuf);
		else
			wsprintf(szExt, _T(" al=%i s=%i p=? (0x%X)"), cbActLen, cbSize, hr);
	}
	else
		szExt[0] = (TCHAR)0;

	//---------------------------------
	AM_MEDIA_TYPE* pmt = NULL;
	hr = pSample->GetMediaType(&pmt);

	//---------------------------------
	ATLTRACE2(category, level, _T("%s%s  %s flags={%u%u%u}%s%s\n"),
		psz, szT, szMT, cSynch, cPreroll, cDiscont, szTypeFlags, szExt);

	if(hr == S_OK && pmt != NULL)
	{
		ATLTRACE2(category, level, _T("%sMedia type attached to sample:\n"), psz);
		Debug_MediaType(category, level, pmt);

		if(pmt->pbFormat != NULL)
			::CoTaskMemFree(pmt->pbFormat);
		if(pmt->pUnk != NULL)
			pmt->pUnk->Release();
		::CoTaskMemFree(pmt);
	}
}

/////////////////////////////////////////////////////////////////////////////
inline void __stdcall Debug_PinMediaTypes(DWORD_PTR category, UINT level, IUnknown* _pPinUnk, LPCTSTR pszPrefix /*= NULL*/)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return;
#endif
	if(level > ATL_TRACE_LEVEL) return;

	ATLTRACE2(category, level, _T("----------------------------------- Pin media types(%s) : BEGIN \n"), pszPrefix);

	CComQIPtr<IPin> pPin( _pPinUnk );

	if(pPin != NULL)
	{
		CComPtr<IEnumMediaTypes> spEnumMT;
		HRESULT hr = pPin->EnumMediaTypes(&spEnumMT);
		if(FAILED(hr))
		{
			ATLTRACE2(category, level,
				_T("###Err### Debug_PinMediaTypes(): pPin->EnumMediaTypes()=0x%X\n"), hr);
		}
		else
		{
			hr = spEnumMT->Reset();
			if(FAILED(hr))
			{
				ATLTRACE2(category, level,
					_T("###Err### Debug_PinMediaTypes(): spEnumMT->Reset()=0x%X\n"), hr);
			}
			else
			{
				AM_MEDIA_TYPE* pmt = NULL;
				ULONG cFetched = 0;
				for(;;)
				{
					hr = spEnumMT->Next(1, &pmt, &cFetched);
					if(hr == S_FALSE) break;
					if(hr != S_OK)
					{
						ATLTRACE2(category, level,
							_T("###Err### Debug_PinMediaTypes(): spEnumMT->Next()=0x%X\n"), hr);
						break;
					}

					if(cFetched != 1 || pmt == NULL)
					{
						ATLTRACE2(category, level, _T("###Err### Debug_PinMediaTypes(): ")
							_T("bad data from spEnumMT->Next(): cFetched=%i, pmt=0x%p\n"),
							cFetched, pmt);
					}
					else
					{
						Debug_MediaType(category, level, pmt, pszPrefix);

						if(pmt->pbFormat != NULL)
							::CoTaskMemFree(pmt->pbFormat);
						if(pmt->pUnk != NULL)
							pmt->pUnk->Release();
						::CoTaskMemFree(pmt);
						pmt = NULL;
					}
				}
			}
		}
	}
	else
		ATLTRACE2(category, level, _T("Debug_PinMediaTypes(): Empty pin pointer\n"));

	ATLTRACE2(category, level, _T("----------------------------------- Pin media types(%s) : END\n"), pszPrefix );
}

inline void __stdcall Debug_MediaType(DWORD_PTR category, UINT level, const AM_MEDIA_TYPE* pmt,
									  LPCTSTR pszPrefix /*= NULL*/)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return;
#endif
	if(level > ATL_TRACE_LEVEL) return;

	LPTSTR psz = _T("MT: ");
	if(pszPrefix != NULL)
	{
		DWORD nSym = lstrlen(pszPrefix);
		psz = (LPTSTR)_alloca((nSym + 1 + 1) * sizeof(TCHAR));
		lstrcpy(psz, pszPrefix);
		psz[nSym] = (TCHAR)' ';
		psz[nSym + 1] = 0;
	}

	if(pmt == NULL)
	{
		ATLTRACE2(category, level, _T("###Err### Debug_MediaType(): Empty media type pointer\n"));
		return;
	}

	//---------------------------------
	TCHAR szBuf[128];
	ParseMediaType(pmt, szBuf, 128);

	ATLTRACE2(category, level, _T("%s%s, size=%i, cbForm=%i, fix:%s, compr:%s\n"),
		psz, szBuf, pmt->lSampleSize, pmt->cbFormat,
		(pmt->bFixedSizeSamples)? _T("Y") : _T("N"),
		(pmt->bTemporalCompression)?  _T("Y") : _T("N"));

	if(pmt->pUnk != NULL)
		ATLTRACE2(category, level, _T("%s@@@@ pUnk = 0x%p\n"), psz, pmt->pUnk);

	//---------------------------------
	if(pmt->cbFormat == 0 || pmt->pbFormat == NULL)
	{
		if(pmt->cbFormat != 0 || pmt->pbFormat != NULL)
		{
			ATLTRACE2(category, level,
				_T("###Err### Debug_MediaType(): bad format parameters, cbFormat=%i, pbFormat=0x%X\n"),
				pmt->cbFormat, pmt->pbFormat);
		}
	}
	else
	{
		BITMAPINFOHEADER* pbmiHeader = NULL;
		if(IsEqualGUID(pmt->formattype, FORMAT_VideoInfo) || 
			IsEqualGUID(pmt->formattype, FORMAT_MPEGVideo))
		{
			VIDEOINFOHEADER* pHeader = (VIDEOINFOHEADER*)pmt->pbFormat;
			ATLTRACE2(category, level,
				_T("%ssrc=[%i,%i:%i,%i], targ=[%i,%i:%i,%i], rate=%u, errRate=%u, AvgT=%s\n"), psz,
				pHeader->rcSource.left, pHeader->rcSource.top, pHeader->rcSource.right, pHeader->rcSource.bottom,
				pHeader->rcTarget.left, pHeader->rcTarget.top, pHeader->rcTarget.right, pHeader->rcTarget.bottom,
				pHeader->dwBitRate, pHeader->dwBitErrorRate, RT2STR(pHeader->AvgTimePerFrame));
			pbmiHeader = &(pHeader->bmiHeader);
		}
		else if(IsEqualGUID(pmt->formattype, FORMAT_VideoInfo2) || 
			IsEqualGUID(pmt->formattype, FORMAT_MPEG2Video))
		{
			VIDEOINFOHEADER2* pHeader2 = (VIDEOINFOHEADER2*)pmt->pbFormat;

			ATLTRACE2(category, level,
				_T("%ssrc=[%i,%i:%i,%i], targ=[%i,%i:%i,%i], rate=%u, errRate=%u, AvgT=%s\n"), psz,
				pHeader2->rcSource.left, pHeader2->rcSource.top, pHeader2->rcSource.right, pHeader2->rcSource.bottom,
				pHeader2->rcTarget.left, pHeader2->rcTarget.top, pHeader2->rcTarget.right, pHeader2->rcTarget.bottom,
				pHeader2->dwBitRate, pHeader2->dwBitErrorRate, RT2STR(pHeader2->AvgTimePerFrame));

			ATLTRACE2(category, level,
				_T("%silace=0x%X%s, copProt=0x%X, aspect={%i,%i}, ctrl=0x%X\n"),
				psz, pHeader2->dwInterlaceFlags,
				((pHeader2->dwInterlaceFlags & AMINTERLACE_IsInterlaced) != 0)? _T(" (INTERLACED)") : _T(""),
				pHeader2->dwCopyProtectFlags,
				pHeader2->dwPictAspectRatioX, pHeader2->dwPictAspectRatioY, pHeader2->dwControlFlags);

			if(IsEqualGUID(pmt->formattype, FORMAT_MPEG2Video))
			{
				MPEG2VIDEOINFO* pMPEG2 = (MPEG2VIDEOINFO*)pmt->pbFormat;
				ATLTRACE2(category, level,
					_T("%sstartCode=0x%X, cbHeader=%u, profile=%u, level=%u, flags=0x%X, seqHeader[0]=0x%X\n"),
					psz, pMPEG2->dwStartTimeCode, pMPEG2->cbSequenceHeader, pMPEG2->dwProfile,
					pMPEG2->dwLevel, pMPEG2->dwFlags, pMPEG2->dwSequenceHeader[0]);

				if(pMPEG2->cbSequenceHeader > 1024)
				{
					Debug_Dump(category, level, 1024, (LPBYTE)pMPEG2->dwSequenceHeader,
						_T("dwSequenceHeader[] first 1024 bytes: "));
				}
				else
				{
					Debug_Dump(category, level, pMPEG2->cbSequenceHeader, (LPBYTE)pMPEG2->dwSequenceHeader,
						_T("dwSequenceHeader[] bytes: "));
				}
			}

			pbmiHeader = &(pHeader2->bmiHeader);
		}
		else if(IsEqualGUID(pmt->formattype, FORMAT_WaveFormatEx))
		{
			Debug_WaveFormatEx(category, level, (WAVEFORMATEX*)pmt->pbFormat, psz);
		}
		else if(IsEqualGUID(pmt->formattype, FORMAT_DvInfo))
		{
			Debug_DvInfo(category, level, (DVINFO*)pmt->pbFormat, psz);
		}
		
		if(pbmiHeader != NULL)
		{
			Debug_BitmapInfoHeader(category, level, pbmiHeader, psz);
		}
	}
}

inline void __stdcall Debug_WaveFormatEx(DWORD_PTR category, UINT level, const WAVEFORMATEX* pWaveFormat,
										 LPCTSTR pszPrefix /*= NULL*/)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return;
#endif
	if(level > ATL_TRACE_LEVEL) return;

	LPTSTR psz = _T("");
	if(pszPrefix != NULL)
	{
		DWORD nSym = lstrlen(pszPrefix);
		psz = (LPTSTR)_alloca((nSym + 1 + 1) * sizeof(TCHAR));
		lstrcpy(psz, pszPrefix);
		psz[nSym] = (TCHAR)' ';
		psz[nSym + 1] = 0;
	}

	if(pWaveFormat == NULL)
	{
		ATLTRACE2(category, level, _T("###Err### Debug_WaveFormatEx() : Empty WAVEFORMATEX pointer\n"));
		return;
	}

	ATLTRACE2(category, level,
		_T("%sWAVEFORMATEX: Tag=0x%X, chan=%i, nPerSec=%i, nAvg=%i, align=%i, bits=%i  size=%i\n"),
		psz,
		pWaveFormat->wFormatTag, pWaveFormat->nChannels, pWaveFormat->nSamplesPerSec,
		pWaveFormat->nAvgBytesPerSec, pWaveFormat->nBlockAlign,
		pWaveFormat->wBitsPerSample, pWaveFormat->cbSize);

	if(pWaveFormat->cbSize > 0)
	{
		if(pWaveFormat->cbSize > 1024)
		{
			Debug_Dump(category, level, 1024, (LPBYTE)pWaveFormat + sizeof(WAVEFORMATEX),
				_T("WAVEFORMATEX first 1024 extra bytes: "));
		}
		else
		{
			Debug_Dump(category, level, pWaveFormat->cbSize, (LPBYTE)pWaveFormat + sizeof(WAVEFORMATEX),
				_T("WAVEFORMATEX extra bytes: "));
		}
	}
}

inline void __stdcall Debug_BitmapInfoHeader(DWORD_PTR category, UINT level, const BITMAPINFOHEADER* pbmiHeader,
											 LPCTSTR pszPrefix /*= NULL*/)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return;
#endif
	if(level > ATL_TRACE_LEVEL) return;

	LPTSTR psz = _T("BITMAPINFOHEADER: ");
	if(pszPrefix != NULL)
	{
		DWORD nSym = lstrlen(pszPrefix);
		psz = (LPTSTR)_alloca((nSym + 1 + 1) * sizeof(TCHAR));
		lstrcpy(psz, pszPrefix);
		psz[nSym] = (TCHAR)' ';
		psz[nSym + 1] = 0;
	}

	if(pbmiHeader == NULL)
	{
		ATLTRACE2(category, level,
			_T("###Err### %sDebug_BitmapInfoHeader(): Empty header pointer\n"), psz);
	}
	else
	{
		ATLTRACE2(category, level,
			_T("%ssize=%i, w=%i, h=%i, planes=%i, bits=%i, compress=%s, sizeImg=%i\n"),
			psz,
			pbmiHeader->biSize, pbmiHeader->biWidth, pbmiHeader->biHeight, pbmiHeader->biPlanes,
			pbmiHeader->biBitCount, FCC2NAME(pbmiHeader->biCompression), pbmiHeader->biSizeImage);

		TCHAR szBuf[128] = _T("");
		if(pbmiHeader->biXPelsPerMeter != 0 || pbmiHeader->biYPelsPerMeter != 0)
		{
			wsprintf(szBuf, _T("XPels=%i, YPels=%i"), pbmiHeader->biXPelsPerMeter, pbmiHeader->biYPelsPerMeter);
		}
		if(pbmiHeader->biClrUsed != 0 || pbmiHeader->biClrImportant != 0)
		{
			if(szBuf[0] == (TCHAR)0)
				wsprintf(szBuf, _T("ClrUsed=%u, ClrImp=%u"),
					pbmiHeader->biClrUsed,pbmiHeader->biClrImportant);
			else
				wsprintf(szBuf + lstrlen(szBuf), _T(", ClrUsed=%u, ClrImp=%u"),
					pbmiHeader->biClrUsed,pbmiHeader->biClrImportant);
		}
		if(szBuf[0] != (TCHAR)0)
		{
			ATLTRACE2(category, level, _T("%s%s\n"), psz, szBuf);
		}
	}
}

inline void __stdcall Debug_DvInfo(DWORD_PTR category, UINT level, const DVINFO* pdvInfo,
											 LPCTSTR pszPrefix /*= NULL*/)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return;
#endif
	if(level > ATL_TRACE_LEVEL) return;

	LPTSTR psz = _T("DVINFO: ");
	if(pszPrefix != NULL)
	{
		DWORD nSym = lstrlen(pszPrefix);
		psz = (LPTSTR)_alloca((nSym + 1 + 1) * sizeof(TCHAR));
		lstrcpy(psz, pszPrefix);
		psz[nSym] = (TCHAR)' ';
		psz[nSym + 1] = 0;
	}

	if(pdvInfo == NULL)
	{
		ATLTRACE2(category, level,
			_T("###Err### %sDebug_DvInfo(): Empty DVINFO pointer\n"), psz);
	}
	else
	{
		// Debug only dvsd structure
		ATLTRACE2(category, level, 
			_T("%s\n\t")
			_T("dwDVAAuxSrc =%08X, dwDVAAuxCtl =%08X (1st 5/6 DIF seq)\n\t")
			_T("dwDVAAuxSrc1=%08X, dwDVAAuxCtl1=%08X (2nd 5/6 DIF seq)\n\t")
			_T("dwDVVAuxSrc =%08X, dwDVVAuxCtl =%08X (Video)\n\t")
			_T("dwDVReserved[0]=%08X, dwDVReserved[1]=%08X\n"),
			psz,
			pdvInfo->dwDVAAuxSrc, pdvInfo->dwDVAAuxCtl,
			pdvInfo->dwDVAAuxSrc1, pdvInfo->dwDVAAuxCtl1,
			pdvInfo->dwDVVAuxSrc, pdvInfo->dwDVVAuxCtl,
			pdvInfo->dwDVReserved[0], pdvInfo->dwDVReserved[1]);
	}
}

inline void __stdcall Debug_Time(DWORD_PTR category, UINT level, IReferenceClock* pRefClock, LPCTSTR pszPrefix /*= NULL*/)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return;
#endif
	if(level > ATL_TRACE_LEVEL) return;

	LPTSTR psz = _T("");
	if(pszPrefix != NULL)
	{
		DWORD nSym = lstrlen(pszPrefix);
		psz = (LPTSTR)_alloca((nSym + 1 + 1) * sizeof(TCHAR));
		lstrcpy(psz, pszPrefix);
		psz[nSym] = (TCHAR)' ';
		psz[nSym + 1] = 0;
	}

	if(pRefClock == NULL)
	{
		ATLTRACE2(category, level, _T("###Err### %sDebug_Time(): Empty pointer to IReferenceClock\n"), psz);
	}
	else
	{
		REFERENCE_TIME tClock = (REFERENCE_TIME)0;
		HRESULT hr = pRefClock->GetTime(&tClock);
		if(hr == S_OK || hr == S_FALSE)
		{
			LPCTSTR pszHR = (hr == S_OK)? _T("") :  _T(" (S_FALSE)");
			ATLTRACE2(category, level, _T("%s%s%s\n"), psz, RT2STR(tClock), pszHR);
		}
		else
		{
			ATLTRACE2(category, level, _T("###Err### %spRefClock->GetTime()=0x%X\n"), psz, hr);
		}
	}
}

inline void __stdcall Debug_TimeDiff(DWORD_PTR category, UINT level, IReferenceClock* pRefClock, REFERENCE_TIME t,
									 LPCTSTR pszPrefix /*= NULL*/)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return;
#endif
	if(level > ATL_TRACE_LEVEL) return;

	LPTSTR psz = _T("");
	if(pszPrefix != NULL)
	{
		DWORD nSym = lstrlen(pszPrefix);
		psz = (LPTSTR)_alloca((nSym + 1 + 1) * sizeof(TCHAR));
		lstrcpy(psz, pszPrefix);
		psz[nSym] = (TCHAR)' ';
		psz[nSym + 1] = 0;
	}

	if(pRefClock == NULL)
	{
		ATLTRACE2(category, level, _T("###Err### %sDebug_TimeDiff(): Empty pointer to IReferenceClock\n"), psz);
		return;
	}

	REFERENCE_TIME tClock = (REFERENCE_TIME)0;
	HRESULT hr = pRefClock->GetTime(&tClock);
	if(hr == S_OK || hr == S_FALSE)
	{
		REFERENCE_TIME tDiff = t - tClock;

		LPCTSTR pszHR = (hr == S_OK)? _T("") :  _T(" (S_FALSE)");
		ATLTRACE2(category, level, _T("%sDiff=%s, t=%s, clock=%s%s\n"),
			psz, RT2STR(tDiff), RT2STR(t), RT2STR(tClock), pszHR);
	}
	else
	{
		ATLTRACE2(category, level, _T("###Err### %sDebug_TimeDiff(): pRefClock->GetTime()=0x%X, t=%s\n"),
			psz, hr, RT2STR(t));
	}
}

inline void __stdcall Debug_SampleLag(DWORD_PTR category, UINT level, IMediaSample* pSample, REFERENCE_TIME tStart,
									  IReferenceClock* pRefClock, LPCTSTR pszPrefix /*= NULL*/)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return;
#endif
	if(level > ATL_TRACE_LEVEL) return;

	LPTSTR psz = _T("");
	if(pszPrefix != NULL)
	{
		DWORD nSym = lstrlen(pszPrefix);
		psz = (LPTSTR)_alloca((nSym + 1 + 1) * sizeof(TCHAR));
		lstrcpy(psz, pszPrefix);
		psz[nSym] = (TCHAR)' ';
		psz[nSym + 1] = 0;
	}

	if(pSample == NULL)
	{
		ATLTRACE2(category, level, _T("###Err### %sDebug_Time(): Empty pointer to IMediaSample\n"), psz);
		return;
	}
	if(pRefClock == NULL)
	{
		ATLTRACE2(category, level, _T("###Err### %sDebug_Time(): Empty pointer to IReferenceClock\n"), psz);
		return;
	}

	REFERENCE_TIME tS = (REFERENCE_TIME)0;
	REFERENCE_TIME tE = (REFERENCE_TIME)0;
	HRESULT hrSmp = pSample->GetTime(&tS, &tE);

	REFERENCE_TIME tClock = (REFERENCE_TIME)0;
	HRESULT hrT = pRefClock->GetTime(&tClock);

	if(FAILED(hrSmp))
	{
		if(hrT == S_OK || hrT == S_FALSE)
		{
			tS = tClock - tStart;

			ATLTRACE2(category, level, _T("%spSample->GetTime()=0x%X, shift=%s, clock=%s%s, tStart=%s\n"),
				psz, hrSmp, RT2STR(tS), RT2STR(tClock), (hrT == S_OK)? _T("") : _T(" (S_FALSE)"),
				RT2STR(tStart));
		}
		else
		{
			ATLTRACE2(category, level, _T("###Err### %spSample->GetTime()=0x%X, pRefClock->GetTime()=0x%X\n"),
				psz, hrSmp, hrT);
		}
	}
	else
	{
		if(hrT == S_OK || hrT == S_FALSE)
		{
			REFERENCE_TIME tDiff = tS + tStart - tClock;

			ATLTRACE2(category, level, _T("%sDiff=%s, tS=%s, tStart=%s, clock=%s%s\n"),
				psz, RT2STR(tDiff), RT2STR(tS), RT2STR(tStart),
				RT2STR(tClock), (hrT == S_OK)? _T("") : _T(" (S_FALSE)"));
		}
		else
		{
			ATLTRACE2(category, level, _T("###Err### %spRefClock->GetTime()=0x%X, tS=%s, tStart=%s\n"),
				psz, hrT, RT2STR(tS), RT2STR(tStart));
		}
	}
}

inline void __stdcall Debug_Quality(DWORD_PTR category, UINT level, Quality& q, LPCTSTR pszPrefix /*= NULL*/)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return;
#endif
	if(level > ATL_TRACE_LEVEL) return;

	ATLTRACE2(category, level,
		_T("%sQuality : Type=%s, Proportion=%i, Late=%s, TimeStamp=%s\n"),
		(pszPrefix != NULL)? pszPrefix : _T(""),
		(q.Type == Famine)? _T("Famine") : _T("Flood"),
		q.Proportion, RT2STR(q.Late), RT2STR(q.TimeStamp));
}

inline void __stdcall Debug_Allocator(DWORD_PTR category, UINT level, IMemAllocator *pAllocator,
									  LPCTSTR pszPrefix /*= NULL*/)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return;
#endif
	if(level > ATL_TRACE_LEVEL) return;

	LPTSTR psz = _T("");
	if(pszPrefix != NULL)
	{
		DWORD nSym = lstrlen(pszPrefix);
		psz = (LPTSTR)_alloca((nSym + 1 + 1) * sizeof(TCHAR));
		lstrcpy(psz, pszPrefix);
		psz[nSym] = (TCHAR)' ';
		psz[nSym + 1] = 0;
	}

	if(pAllocator == NULL)
	{
		ATLTRACE2(category, level, _T("###Err### %sDebug_Allocator(): Empty allocator pointer\n"), psz);
		return;
	}

	ALLOCATOR_PROPERTIES rgProp = {0};
	HRESULT hr = pAllocator->GetProperties(&rgProp);
	if(FAILED(hr))
	{
		ATLTRACE2(category, level,
			_T("###Err### %sDebug_Allocator(): pAllocator->GetProperties()=0x%X\n"), psz, hr);
	}
	else
	{
		ATLTRACE2(category, level, _T("%sIMemAllocator=0x%p, props = { %i, %i, %i, %i }\n"),
				psz, pAllocator, rgProp.cBuffers, rgProp.cbBuffer, rgProp.cbAlign, rgProp.cbPrefix);
	}
}

inline void __stdcall Debug_AllocatorProps(DWORD_PTR category, UINT level, ALLOCATOR_PROPERTIES* pProps,
										   LPCTSTR pszPrefix /*= NULL*/)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return;
#endif
	if(level > ATL_TRACE_LEVEL) return;

	LPTSTR psz = _T("");
	if(pszPrefix != NULL)
	{
		DWORD nSym = lstrlen(pszPrefix);
		psz = (LPTSTR)_alloca((nSym + 1 + 1) * sizeof(TCHAR));
		lstrcpy(psz, pszPrefix);
		psz[nSym] = (TCHAR)' ';
		psz[nSym + 1] = 0;
	}

	if(pProps == NULL)
	{
		ATLTRACE2(category, level, _T("###Err### %sDebug_AllocatorProps(): Empty pointer\n"), psz);
	}
	else
	{
		ATLTRACE2(category, level, _T("%s{ %i, %i, %i, %i }\n"),
				psz, pProps->cBuffers, pProps->cbBuffer, pProps->cbAlign, pProps->cbPrefix);
	}
}

inline bool __stdcall Debug_Dump(DWORD_PTR category, UINT level, UINT cb, const void* pv, LPCTSTR pszPrefix /*= NULL*/)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return true;
#endif
	if(level > ATL_TRACE_LEVEL) return true;

	if(pszPrefix != NULL)
		ATLTRACE2(category, level, _T("%s Trace from address 0x%p (%i bytes)\n"), pszPrefix, pv, cb);
	else
		ATLTRACE2(category, level, _T("Trace from address 0x%p (%i bytes)\n"), pv, cb);

	if(pv == NULL || cb == 0) return false;

	//---------------------------------
	TCHAR szDump[16*3+4+2+16+1];	// 71 = 16 * "XX " + 3*' ' + '|' + 2*' ' + 16*'.' + \0
	szDump[16*3+4+2+16] = (TCHAR)0;

	BYTE b, *pbLine, *pb = (BYTE*)pv;
	UINT i, j;
	for(i = 0; i < cb;)
	{
		LPTSTR pszHex = szDump;
		LPTSTR pszSym = pszHex + 16*3 + 4 + 2;
		pbLine = pb;

		for(j = 0; j < 16; ++j)
		{
			if(i < cb)
			{
				b = *pb++;
				++i;

				*pszHex = (TCHAR)(b >> 4);
				if(*pszHex < 0xA)
					*pszHex += (TCHAR)'0';
				else
					*pszHex += (TCHAR)'A' - 0xA;
				++pszHex;

				*pszHex = (TCHAR)(b & 0xF);
				if(*pszHex < 0xA)
					*pszHex += (TCHAR)'0';
				else
					*pszHex += (TCHAR)'A' - 0xA;
				++pszHex;

				// #ifdef _UNICODE: OutputDebugStringW stop convertion if symbol > 0x7F and we can't see more symbols
				// Disable symbols > 0x7F for TCHAR = char also
				*pszSym++ = (b < 0x20 || 0x7F < b)? (TCHAR)'.' : (TCHAR)b;
			}
			else
			{
				*pszHex++ = (TCHAR)' ';
				*pszHex++ = (TCHAR)' ';
			}

			*pszHex++ = (TCHAR)' ';
			if(j == 7)
			{
				*pszHex++ = (TCHAR)'|';
				*pszHex++ = (TCHAR)' ';
			}
			else if(j == 3 || j == 11)
				*pszHex++ = (TCHAR)' ';
		}

		*pszHex++ = (TCHAR)' ';
		*pszHex++ = (TCHAR)' ';
		*pszSym = 0;

		ATLTRACE2(category, level, _T("0x%p: %s\n"), pbLine, szDump);
	}

	ATLTRACE2(category, level, _T("\n"));
	return true;
}

inline bool __stdcall Debug_DumpSample(DWORD_PTR category, UINT level, IMediaSample* pSample,
									   LPCTSTR pszPrefix /*= NULL*/)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return true;
#endif
	if(level > ATL_TRACE_LEVEL) return true;

	LPTSTR psz = _T("");
	if(pszPrefix != NULL)
	{
		DWORD nSym = lstrlen(pszPrefix);
		psz = (LPTSTR)_alloca((nSym + 1 + 1) * sizeof(TCHAR));
		lstrcpy(psz, pszPrefix);
		psz[nSym] = (TCHAR)' ';
		psz[nSym + 1] = 0;
	}

	if(pSample == NULL)
	{
		ATLTRACE2(category, level, _T("###Err### %sDebug_DumpSample(): Empty sample pointer\n"), psz);
		return false;
	}

	BYTE* pBuffer = NULL;
	HRESULT hr = pSample->GetPointer(&pBuffer);
	if(FAILED(hr) || pBuffer == NULL)
	{
		ATLTRACE2(category, level,
			_T("###Err### %sDebug_DumpSample(): pSample->GetPointer()=0x%X, pBuffer=0x%p\n"),
			psz, hr, pBuffer);
		return false;
	}

	long lActual = pSample->GetActualDataLength();
	if(lActual <= 0)
	{
		ATLTRACE2(category, level,
			_T("###Err### %sDebug_DumpSample(): GetActualDataLength()=%i\n"), psz, lActual);
		return false;
	}

	return Debug_Dump(category, level, (UINT)lActual, pBuffer, pszPrefix);
}

inline bool __stdcall Debug_FileDump(DWORD_PTR category, UINT level, HANDLE hFile,
									 LONGLONG llPosition, DWORD cbData)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return true;
#endif
	if(level > ATL_TRACE_LEVEL) return true;

	//---------------------------------
	LONGLONG llSize = 0;
	if(hFile != NULL && hFile != INVALID_HANDLE_VALUE)
	{
		::SetLastError(NO_ERROR);

		DWORD dwFileSizeHigh = 0;
		DWORD dwFileSizeLow = ::GetFileSize(hFile, &dwFileSizeHigh);
		if(dwFileSizeLow == INVALID_FILE_SIZE)
		{
			DWORD dwErr = ::GetLastError();
			if(dwErr != NO_ERROR)
			{
				ATLTRACE2(category, TRACELEVEL_Error, _T("###Err### Debug_FileDump( file 0x%08X ): ")
					_T("GetFileSize()=%u\n"), hFile, dwErr);
				return false;
			}
		}

		llSize = (LONGLONG)dwFileSizeLow;
		*((DWORD*)&llSize + 1) = dwFileSizeHigh;
	}

	if(llSize <= llPosition)
	{
		ATLTRACE2(category, TRACELEVEL_Error, _T("###Err### Debug_FileDump( file 0x%08X ): ")
			_T("size %s <= position %s\n"), hFile, N64STR(llSize), N64STR(llPosition));
		return false;
	}

	//---------------------------------
	DWORD cbRead = cbData;
	if(cbData > 0x1000000)		// 16 MBytes
	{
		cbRead = 0x1000000;
		ATLTRACE2(category, level, _T("Debug_FileDump( file 0x%08X ): ")
			_T("Very big data size %u, change to %u\n"), hFile, cbData, cbRead);
	}

	if(llSize < llPosition + cbRead)
	{
		ATLTRACE2(category, level, _T("Debug_FileDump( file 0x%08X ): ")
			_T("Out of file: size %s < position %s + dump size %u, change dump size to %u\n"),
			hFile, N64STR(llSize), N64STR(llPosition), cbRead, (DWORD)(llSize - llPosition));
		cbRead = (DWORD)(llSize - llPosition);
	}

	ATLTRACE2(category, level, _T("File 0x%08X, size %s, position %s, dump size %u\n"),
		hFile, N64STR(llSize), N64STR(llPosition), cbRead);

	//---------------------------------
	DWORD dwDistanceToMove = *(DWORD*)&llPosition;
	LONG lDistanceToMoveHigh = *((LONG*)&llPosition + 1);
	DWORD dwRet = ::SetFilePointer(hFile, dwDistanceToMove, &lDistanceToMoveHigh, FILE_BEGIN);
	if(dwRet == (DWORD)-1)
	{
		DWORD dwErr = ::GetLastError();
		if(dwErr != NO_ERROR)
		{
			ATLTRACE2(category, TRACELEVEL_Error, _T("###Err### Debug_FileDump( file 0x%08X ): ")
				_T("SetFilePointer()=%u\n"), hFile, dwErr);
			return false;
		}
	}

	//---------------------------------
	void* pBuffer = malloc(cbRead);
	if(pBuffer == NULL)
	{
		ATLTRACE2(category, TRACELEVEL_Error, _T("###Err### Debug_FileDump( file 0x%08X ): ")
			_T("Out of memory\n"), hFile);
		return false;
	}

	DWORD cb = 0;
	BOOL bRet = ::ReadFile(hFile, pBuffer, cbRead, &cb, NULL);
	if(bRet == FALSE || cbRead != cb)
	{
		DWORD dwErr = ::GetLastError();
		if(dwErr != NO_ERROR)
		{
			ATLTRACE2(category, TRACELEVEL_Error, _T("###Err### Debug_FileDump( file 0x%08X ): ")
				_T("ReadFile()=%u\n"), hFile, dwErr);
		}
		else
		{
			ATLTRACE2(category, TRACELEVEL_Error, _T("###Err### Debug_FileDump( file 0x%08X ): ")
				_T("ReadFile()=NO_ERROR, cbRead=%u, cb=%u\n"), hFile, cbRead, cb);
		}

		free(pBuffer);
		return false;
	}

	//---------------------------------
	Debug_Dump(category, level, cbRead, pBuffer);
	free(pBuffer);
	return true;
}

inline bool __stdcall Debug_AsyncReaderDump(DWORD_PTR category, UINT level, IAsyncReader* pAsyncReader,
											LONGLONG llPosition, DWORD cbData)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return true;
#endif
	if(level > ATL_TRACE_LEVEL) return true;

	if(pAsyncReader == NULL)
	{
		ATLTRACE2(category, TRACELEVEL_Error, _T("###Err### Debug_AsyncReaderDump(): pAsyncReader = NULL\n"));
		return false;
	}

	//---------------------------------
	LONGLONG llSize = 0;
	LONGLONG llAvailable = 0;
	HRESULT hr = pAsyncReader->Length(&llSize, &llAvailable);
	if(hr != S_OK)
	{
		ATLTRACE2(category, TRACELEVEL_Error, _T("###Err### Debug_AsyncReaderDump( pAsyncReader=0x%08X ): ")
			_T("pAsyncReader->Length()=0x%X\n"), pAsyncReader, hr);
		return false;
	}

	if(llSize <= llPosition)
	{
		ATLTRACE2(category, TRACELEVEL_Error, _T("###Err### Debug_AsyncReaderDump( pAsyncReader=0x%08X ): ")
			_T("size %s <= position %s\n"), pAsyncReader, N64STR(llSize), N64STR(llPosition));
		return false;
	}

	//---------------------------------
	DWORD cbRead = cbData;
	if(cbData > 0x1000000)		// 16 MBytes
	{
		cbRead = 0x1000000;
		ATLTRACE2(category, level, _T("Debug_AsyncReaderDump( pAsyncReader=0x%08X ): ")
			_T("Very big data size %u, change to %u\n"), pAsyncReader, cbData, cbRead);
	}

	if(llSize < llPosition + cbRead)
	{
		ATLTRACE2(category, level, _T("Debug_AsyncReaderDump( pAsyncReader=0x%08X ): ")
			_T("Out of range: size %s < position %s + dump size %u, change dump size to %u\n"),
			pAsyncReader, N64STR(llSize), N64STR(llPosition), cbRead, (DWORD)(llSize - llPosition));
		cbRead = (DWORD)(llSize - llPosition);
	}

	ATLTRACE2(category, level, _T("AsyncReader=0x%08X, size %s, position %s, dump size %u\n"),
		pAsyncReader, N64STR(llSize), N64STR(llPosition), cbRead);

	//---------------------------------
	void* pBuffer = malloc(cbRead);
	if(pBuffer == NULL)
	{
		ATLTRACE2(category, level, _T("Debug_AsyncReaderDump( pAsyncReader=0x%08X ): ")
			_T("Out of memory\n"), pAsyncReader);
		return false;
	}

	hr = pAsyncReader->SyncRead(llPosition, cbRead, (BYTE*)pBuffer);
	if(hr != S_OK)
	{
		if(hr == S_FALSE)
			ATLTRACE2(category, TRACELEVEL_Error, _T("###Err### Debug_AsyncReaderDump( pAsyncReader=0x%08X ): ")
				_T("AsyncReader EOF\n"), pAsyncReader);
		else
			ATLTRACE2(category, TRACELEVEL_Error, _T("###Err### Debug_AsyncReaderDump( pAsyncReader=0x%08X ): ")
				_T("pAsyncReader->SyncRead()=0x%X\n"), pAsyncReader, hr);

		free(pBuffer);
		return false;
	}

	//---------------------------------
	Debug_Dump(category, level, cbRead, pBuffer);
	free(pBuffer);
	return true;
}

// If return true : position in stream don't changed or successfully restored
inline bool __stdcall Debug_Stream(DWORD_PTR category, UINT level, IStream* pStm,
								   bool bFromCurrentPosition /*= false*/, LPCTSTR pszPrefix /*= NULL*/)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return true;
#endif
	if(level > ATL_TRACE_LEVEL) return true;

	LPTSTR psz = _T("");
	if(pszPrefix != NULL)
	{
		DWORD nSym = lstrlen(pszPrefix);
		psz = (LPTSTR)_alloca((nSym + 1 + 1) * sizeof(TCHAR));
		lstrcpy(psz, pszPrefix);
		psz[nSym] = (TCHAR)' ';
		psz[nSym + 1] = 0;
	}

	if(pStm == NULL)
	{
		ATLTRACE2(category, level, _T("###Err### %sDebug_Stream(): Empty stream pointer\n"), psz);
		return true;
	}

	LARGE_INTEGER liPos = {0};
	ULARGE_INTEGER uliOriginal = {0};
	HRESULT hr = pStm->Seek(liPos, STREAM_SEEK_CUR, &uliOriginal);
	if(hr != S_OK)
	{
		ATLTRACE2(category, level,
			_T("###Err### %sDebug_Stream(): pStm->Seek( 0, STREAM_SEEK_CUR )=0x%X\n"), psz, hr);
		return true;
	}

	//---------------------------------
	STATSTG rgStatStg = {0};
	hr = pStm->Stat(&rgStatStg, STATFLAG_NONAME);
	if(hr != S_OK)
	{
		ATLTRACE2(category, level, _T("###Err### %sDebug_Stream(): pStm->Stat()=0x%X\n"), psz, hr);
		return true;
	}

	LONGLONG llSize = (LONGLONG)rgStatStg.cbSize.QuadPart;
	if(bFromCurrentPosition)
		llSize -= (LONGLONG)uliOriginal.QuadPart;
	if(llSize < 0 || llSize >= 0x8FFFF)		// >= 0.5 MB, see below : _alloca(llSize)
	{
		ATLTRACE2(category, level,
			_T("###Err### %sDebug_Stream(): bad or very big stream, size=%I64i, curr=%I64i\n"),
			psz, (LONGLONG)rgStatStg.cbSize.QuadPart, (LONGLONG)uliOriginal.QuadPart);
		return true;
	}

	if(!bFromCurrentPosition)
	{
		hr = pStm->Seek(liPos, STREAM_SEEK_SET, NULL);
		if(hr != S_OK)
		{
			ATLTRACE2(category, level,
				_T("###Err### %sDebug_Stream(): pStm->Seek( 0, STREAM_SEEK_SET )=0x%X\n"), psz, hr);
			return true;
		}
	}

	//---------------------------------
	void* pBuf = _alloca((DWORD)llSize);
	DWORD cbRead = 0;
	hr = pStm->Read(pBuf, (DWORD)llSize, &cbRead);
	if(hr == S_OK && cbRead == (DWORD)llSize)
	{
		ATLTRACE2(category, level, _T("%sDump for stream 0x%X (size %I64i) from position %I64i\n"),
			psz, pStm, (LONGLONG)rgStatStg.cbSize.QuadPart,
			(bFromCurrentPosition)? (LONGLONG)uliOriginal.QuadPart : (LONGLONG)0);

		Debug_Dump(category, level, cbRead, pBuf);
	}
	else
	{
		ATLTRACE2(category, level,
			_T("###Err### %sDebug_Stream(): pStm->Read( %u bytes )=0x%X, cbRead=%u\n"),
			psz, (DWORD)llSize, hr, cbRead);
	}

	//---------------------------------
	liPos.QuadPart = (LONGLONG)uliOriginal.QuadPart;
	hr = pStm->Seek(liPos, STREAM_SEEK_SET, NULL);
	if(hr != S_OK)
	{
		ATLTRACE2(category, level, _T("###Err### %sDebug_Stream(): ")
			_T("pStm->Seek()=0x%X, can't returning to original position\n"),
			psz, hr);
		return false;
	}
	return true;
}

inline void __stdcall Debug_MultiString(DWORD_PTR category, UINT level, LPTSTR pszString)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return;
#endif
	if(level > ATL_TRACE_LEVEL) return;

	if(pszString == NULL) return;

	//---------------------------------
	LPTSTR psz = pszString;
	while(*psz != (TCHAR)0)
	{
		LPTSTR p = psz;
		while(*p != (TCHAR)'\n' && *p != (TCHAR)0) ++p;

		TCHAR sym = 0;
		if(*p == (TCHAR)'\n')
		{
			sym = *(p+1);
			*(p+1) = (TCHAR)0;
		}

		ATLTRACE2(category, level, psz);

		psz = p;
		if(*psz == (TCHAR)'\n')
		{
			++psz;
			*psz = sym;
		}
	}
}

inline void __stdcall Debug_FilterFromPin(DWORD_PTR category, UINT level, LPCTSTR pszPrefix, IPin* pPin)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return;
#endif
	if(level > ATL_TRACE_LEVEL) return;

	LPCTSTR psz = (pszPrefix != NULL)? pszPrefix : _T("");

	if(pPin == NULL)
	{
		ATLTRACE2(category, level, _T("###Err### %sDebug_FilterFromPin(): Empty pin pointer\n"), psz);
		return;
	}

	PIN_INFO rgPinInfo = {0};
	HRESULT hr = pPin->QueryPinInfo(&rgPinInfo);
	if(FAILED(hr))
	{
		ATLTRACE2(category, level,
			_T("###Err### %sDebug_FilterFromPin(): pPin->QueryPinInfo()=0x%X\n"), psz, hr);
		return;
	}
	if(rgPinInfo.pFilter == NULL)
	{
		ATLTRACE2(category, level,
			_T("###Err### %sDebug_FilterFromPin(): rgPinInfo.pFilter == NULL\n"), psz);
		return;
	}

	CLSID clsid = GUID_NULL;
	hr = rgPinInfo.pFilter->GetClassID(&clsid);
	if(FAILED(hr))
	{
		ATLTRACE2(category, level,
			_T("###Err### %sDebug_FilterFromPin(): pFilter->GetClassID()=0x%X\n"), psz, hr);
	}
	else
	{
		ATLTRACE2(category, level, _T("%sfilter [%s]\n"), psz, GUID2NAME(clsid));
	}
	rgPinInfo.pFilter->Release();
}

inline void __stdcall Debug_Filter(DWORD_PTR category, UINT level, LPCTSTR pszComment, REFCLSID rclsid, HRESULT hrCreate)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return;
#endif
	if(level > ATL_TRACE_LEVEL) return;

	LPCTSTR psz = (pszComment != NULL)? pszComment : _T("");
	LPCTSTR pszFilterName = GUID2NAME(rclsid);

	if(FAILED(hrCreate))
	{
		ATLTRACE2(category, level, _T("###Err### Filter: %s [%s] FAILED, hr = 0x%X\n"),
			psz, pszFilterName, hrCreate);
	}
	else
	{
		ATLTRACE2(category, level, _T("Filter: %s [%s] SUCCEEDED, hr = 0x%X\n"),
			psz, pszFilterName, hrCreate);
	}
}

inline void __stdcall Debug_FilterInfo(DWORD_PTR category, UINT level, IBaseFilter* pFilter)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return;
#endif
	if(level > ATL_TRACE_LEVEL) return;

	if(pFilter == NULL)
	{
		ATLTRACE2(category, level, _T("###Err### Debug_FilterInfo(): Empty filter pointer\n"));
		return;
	}

	CLSID clsid = GUID_NULL;
	HRESULT hr = pFilter->GetClassID(&clsid);
	if(FAILED(hr))
	{
		ATLTRACE2(category, level, _T("###Err### Debug_FilterInfo(): pFilter->GetClassID()=0x%X\n"), hr);
		return;
	}

	LPCTSTR pszFilterName = GUID2NAME(clsid);
	
	//---------------------------------
	FILTER_INFO rgInfo;
	rgInfo.achName[0] = L'\0';
	rgInfo.pGraph = NULL;
	hr = pFilter->QueryFilterInfo(&rgInfo);
	if(FAILED(hr))
	{
		ATLTRACE2(category, level,
			_T("###Err### Debug_FilterInfo(): pFilter->QueryFilterInfo()=0x%X for filter [%s]\n"),
			hr, pszFilterName);
		return;
	}

	USES_CONVERSION;
	ATLTRACE2(category, level, _T("[%s]: Filter name = [%s], %s\n"),
		pszFilterName,
		(rgInfo.achName[0] == L'\0')? _T("") : OLE2T(rgInfo.achName),
		(rgInfo.pGraph != NULL)? _T("joined to graph") : _T("not in graph"));

	if(rgInfo.pGraph != NULL)
		rgInfo.pGraph->Release();

	//---------------------------------
	LPWSTR pVendorInfo = NULL;
	hr = pFilter->QueryVendorInfo(&pVendorInfo);
	ATLTRACE2(category, level, _T("[%s]: Vendor Info = [%s]\n"), pszFilterName,
			  (FAILED(hr) || pVendorInfo == NULL)? _T("UNKNOWN") : OLE2T(pVendorInfo));
	if(pVendorInfo != NULL)
		CoTaskMemFree(pVendorInfo);
}

inline void __stdcall Debug_MixerPinConfig(DWORD_PTR category, UINT level, IMixerPinConfig2* pMPC)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return;
#endif
	if(level > ATL_TRACE_LEVEL) return;

	if(pMPC == NULL)
	{
		ATLTRACE2(category, level, _T("###Err### Debug_MixerPinConfig() : Empty interface pointer\n"));
		return;
	}

	DDCOLORCONTROL rgCC = {0};
	rgCC.dwSize = sizeof(DDCOLORCONTROL);
	HRESULT hr = pMPC->GetOverlaySurfaceColorControls(&rgCC);
	if(hr != S_OK)
	{
		ATLTRACE2(category, level, _T("###Err### Debug_MixerPinConfig(): ")
			_T("IMixerPinConfig2::GetOverlaySurfaceColorControls()=0x%X\n"), hr);
		return;
	}

	Debug_ColorControl(category, level, rgCC);
}

inline void __stdcall Debug_AMStreamConfig(DWORD_PTR category, UINT level, IUnknown* pUnk, LPCTSTR pszPrefix /*= NULL*/)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return;
#endif
	if(level > ATL_TRACE_LEVEL) return;

	if(pszPrefix == NULL)
		pszPrefix = _T("");

	if(pUnk == NULL)
	{
		ATLTRACE2(category, level, _T("###Err### %sDebug_AMStreamConfig(): Empty interface pointer\n"), pszPrefix);
		return;
	}

	CComQIPtr<IAMStreamConfig> spAMStreamConfig(pUnk);
	if(!spAMStreamConfig)
	{
		ATLTRACE2(category, level, _T("### %sDebug_AMStreamConfig(): IAMStreamConfig not supported\n"), pszPrefix);
		return;
	}

	//---------------------------------
	int iCount = 0;
	int iSize = 0;
	HRESULT hr = spAMStreamConfig->GetNumberOfCapabilities(&iCount, &iSize);
	if(hr != S_OK)
	{
		ATLTRACE2(category, level, _T("###Err### %sDebug_AMStreamConfig(): GetNumberOfCapabilities()=0x%X\n"),
			pszPrefix, hr);
		return;
	}

	//---------------------------------
	ATLTRACE2(category, level, _T("=================================== %sIAMStreamConfig: count %i, size %i\n"),
		pszPrefix, iCount, iSize);

	BYTE* pSCC = (BYTE*)_alloca(iSize);
	AM_MEDIA_TYPE* pmt = NULL;

	TCHAR szNum[16], szGuid[128], szVS[512], sz1[32], sz2[32];

	for(int i = 0; i < iCount; ++i)
	{
		memset(pSCC, 0, iSize);

		hr = spAMStreamConfig->GetStreamCaps(i, &pmt, pSCC);
		if(hr != S_OK)
		{
			ATLTRACE2(category, level, _T("###Err### %sDebug_AMStreamConfig(): GetStreamCaps( %i )=0x%X\n"),
				pszPrefix, i, hr);
			continue;
		}

		//---------------------------------
		wsprintf(szNum, _T("%s%2i MT: "), pszPrefix, i);
		Debug_MediaType(category, level, pmt, szNum);

		wsprintf(szNum, _T("%s%2i SC: "), pszPrefix, i);
		if(iSize == sizeof(VIDEO_STREAM_CONFIG_CAPS))
		{
			VIDEO_STREAM_CONFIG_CAPS* pVSCC = (VIDEO_STREAM_CONFIG_CAPS*)pSCC;

			szVS[0] = 0;
			for(DWORD dwMask = 1; dwMask != 0; dwMask <<= 1)
			{
				if(pVSCC->VideoStandard & dwMask)
				{
					LPCTSTR pszVS = _T("UNK");
					switch(dwMask)
					{
						case AnalogVideo_None:			pszVS = _T("None"); break;
						case AnalogVideo_NTSC_M:		pszVS = _T("NTSC_M"); break;
						case AnalogVideo_NTSC_M_J:		pszVS = _T("NTSC_M_J"); break;
						case AnalogVideo_NTSC_433:		pszVS = _T("NTSC_433"); break;
						case AnalogVideo_PAL_B:			pszVS = _T("PAL_B"); break;
						case AnalogVideo_PAL_D:			pszVS = _T("PAL_D"); break;
						case AnalogVideo_PAL_G:			pszVS = _T("PAL_G"); break;
						case AnalogVideo_PAL_H:			pszVS = _T("PAL_H"); break;
						case AnalogVideo_PAL_I:			pszVS = _T("PAL_I"); break;
						case AnalogVideo_PAL_M:			pszVS = _T("PAL_M"); break;
						case AnalogVideo_PAL_N:			pszVS = _T("PAL_N"); break;
						case AnalogVideo_PAL_60:		pszVS = _T("PAL_60"); break;
						case AnalogVideo_SECAM_B:		pszVS = _T("SECAM_B"); break;
						case AnalogVideo_SECAM_D:		pszVS = _T("SECAM_D"); break;
						case AnalogVideo_SECAM_G:		pszVS = _T("SECAM_G"); break;
						case AnalogVideo_SECAM_H:		pszVS = _T("SECAM_H"); break;
						case AnalogVideo_SECAM_K:		pszVS = _T("SECAM_K"); break;
						case AnalogVideo_SECAM_K1:		pszVS = _T("SECAM_K1"); break;
						case AnalogVideo_SECAM_L:		pszVS = _T("SECAM_L"); break;
						case AnalogVideo_SECAM_L1:		pszVS = _T("SECAM_L1"); break;
						case AnalogVideo_PAL_N_COMBO:	pszVS = _T("PAL_N_COMBO"); break;
					}

					if(szVS[0] != 0)
						lstrcat(szVS, _T("|"));
					lstrcat(szVS, pszVS);
				}
			}

			ATLTRACE2(category, level, _T("%sguid: %s, VideoStandard: %s (0x%X)\n"),
				szNum, Guid2Name(pVSCC->guid, szGuid, 128), szVS, pVSCC->VideoStandard);
			ATLTRACE2(category, level, _T("%sInputSize: [%i,%i], MinCroppingSize: [%i,%i], MaxCroppingSize: [%i,%i]\n"),
				szNum, pVSCC->InputSize.cx, pVSCC->InputSize.cy, pVSCC->MinCroppingSize.cx, pVSCC->MinCroppingSize.cy,
				pVSCC->MaxCroppingSize.cx, pVSCC->MaxCroppingSize.cy);
			ATLTRACE2(category, level, _T("%sCropGranularityX=%i, CropGranularityY=%i, CropAlignX=%i, CropAlignY=%i\n"),
				szNum, pVSCC->CropGranularityX, pVSCC->CropGranularityY, pVSCC->CropAlignX, pVSCC->CropAlignY);
			ATLTRACE2(category, level, _T("%sMinOutputSize: [%i,%i], MaxOutputSize: [%i,%i], ")
				_T("OutputGranularityX=%i, OutputGranularityY=%i\n"),
				szNum, pVSCC->MinOutputSize.cx, pVSCC->MinOutputSize.cy, pVSCC->MaxOutputSize.cx, pVSCC->MaxOutputSize.cy,
				pVSCC->MaxCroppingSize.cx, pVSCC->MaxCroppingSize.cy);
			ATLTRACE2(category, level, _T("%sStretchTapsX=%i, StretchTapsY=%i, ShrinkTapsX=%i, ShrinkTapsY=%i\n"),
				szNum, pVSCC->StretchTapsX, pVSCC->StretchTapsY, pVSCC->ShrinkTapsX, pVSCC->ShrinkTapsY);
			ATLTRACE2(category, level, _T("%sMinFrameInterval=%s, MaxFrameInterval=%s, MinBitsPerSecond=%i, ")
				_T("MaxBitsPerSecond=%i\n"),
				szNum, RefTime2Str(pVSCC->MinFrameInterval, sz1), RefTime2Str(pVSCC->MaxFrameInterval, sz2),
				pVSCC->MinBitsPerSecond, pVSCC->MaxBitsPerSecond);
		}
		else if(iSize == sizeof(AUDIO_STREAM_CONFIG_CAPS))
		{
			AUDIO_STREAM_CONFIG_CAPS* pASCC = (AUDIO_STREAM_CONFIG_CAPS*)pSCC;

			ATLTRACE2(category, level, _T("%sguid: %s, MinimumChannels=%u, MaximumChannels=%u, ChannelsGranularity=%u\n"),
				szNum, Guid2Name(pASCC->guid, szGuid, 128), pASCC->MinimumChannels, pASCC->MaximumChannels,
				pASCC->ChannelsGranularity);
			ATLTRACE2(category, level, _T("%sMinimumBitsPerSample=%u, MaximumBitsPerSample=%u, BitsPerSampleGranularity=%u\n"),
				szNum, pASCC->MinimumBitsPerSample, pASCC->MaximumBitsPerSample, pASCC->BitsPerSampleGranularity);
			ATLTRACE2(category, level, _T("%sMinimumSampleFrequency=%u, MaximumSampleFrequency=%u, SampleFrequencyGranularity=%u\n"),
				szNum, pASCC->MinimumSampleFrequency, pASCC->MaximumSampleFrequency, pASCC->SampleFrequencyGranularity);
		}
		else
		{
			ATLTRACE2(category, level, _T("###Err### %sDebug_AMStreamConfig(): GetStreamCaps( %i ): Unknown struct\n"),
				pszPrefix, i);
		}

		ATLTRACE2(category, level, _T("\n"));

		//---------------------------------
		if(pmt != NULL)
		{
			::CoTaskMemFree(pmt->pbFormat);
			if(pmt->pUnk != NULL)
				pmt->pUnk->Release();
			::CoTaskMemFree(pmt);
			pmt = NULL;
		}
	}

	ATLTRACE2(category, level, _T("=================================== IAMStreamConfig: END\n"));
}

inline void __stdcall Debug_KsPropertySet(DWORD_PTR category, UINT level, REFGUID guidPropSet,
										  DWORD dwPropID, DWORD cbInstanceData, DWORD cbPropData,
										  LPCTSTR pszPrefix1 /*= NULL*/, LPCTSTR pszPrefix2 /*= NULL*/)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return;
#endif
	if(level > ATL_TRACE_LEVEL) return;

	LPCTSTR psz1 = (pszPrefix1 != NULL)? pszPrefix1 : _T("");
	LPCTSTR psz2 = (pszPrefix2 != NULL)? pszPrefix2 : _T("");

	ATLTRACE2(category, level, _T("%s%sguidPropSet=%s, dwPropID=%u, cbInst=%u, cbProp=%u\n"),
		psz1, psz2, GUID2NAME(guidPropSet), dwPropID, cbInstanceData, cbPropData);
}

inline void __stdcall Debug_KsDataFormat(DWORD_PTR category, UINT level, KSDATAFORMAT* pKsDataFormat,
										 LPCTSTR pszPrefix /*= NULL*/)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return;
#endif
	if(level > ATL_TRACE_LEVEL) return;

	LPCTSTR psz = (pszPrefix != NULL)? pszPrefix : _T("");
	if(pKsDataFormat == NULL)
	{
		ATLTRACE2(category, level, _T("###Err### %sDebug_KsDataFormat(): Empty pointer to struct\n"), psz);
		return;
	}

	//---------------------------------
	ATLTRACE2(category, level, _T("%sKSDATAFORMAT: FormatSize=%u, Flags=0x%X, SampleSize=%u\n"),
		psz, pKsDataFormat->FormatSize, pKsDataFormat->Flags, pKsDataFormat->SampleSize);

	ATLTRACE2(category, level, _T("%sKSDATAFORMAT: major %s, sub %s, specifier %s\n"),
		psz, GUID2NAME(pKsDataFormat->MajorFormat), GUID2NAME(pKsDataFormat->SubFormat),
		GUID2NAME(pKsDataFormat->Specifier));
}

inline void __stdcall Debug_KsVBIInfoHeader(DWORD_PTR category, UINT level, KS_VBIINFOHEADER* pKsVBIIH,
											LPCTSTR pszPrefix /*= NULL*/)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return;
#endif
	if(level > ATL_TRACE_LEVEL) return;

	LPCTSTR psz = (pszPrefix != NULL)? pszPrefix : _T("");
	if(pKsVBIIH == NULL)
	{
		ATLTRACE2(category, level, _T("###Err### %sDebug_KsVBIInfoHeader(): Empty pointer to struct\n"), psz);
		return;
	}

	//---------------------------------
	ATLTRACE2(category, level, _T("%s: KS_VBIINFOHEADER: StartLine=%u, EndLine=%u, SamplingFrequency=%u\n"),
		psz, pKsVBIIH->StartLine, pKsVBIIH->EndLine, pKsVBIIH->SamplingFrequency);

	ATLTRACE2(category, level, _T("%s: KS_VBIINFOHEADER: Min/Max LineStartTime: %u:%u, ActualLine Start/End Time=%u:%u\n"),
		psz, pKsVBIIH->MinLineStartTime, pKsVBIIH->MaxLineStartTime,
		pKsVBIIH->ActualLineStartTime, pKsVBIIH->ActualLineEndTime);

	ATLTRACE2(category, level, _T("%s: KS_VBIINFOHEADER: VideoStandard=0x%X, SamplesPerLine=%u, StrideInBytes=%u, BufferSize=%u\n"),
		psz, pKsVBIIH->VideoStandard, pKsVBIIH->SamplesPerLine,
		pKsVBIIH->StrideInBytes, pKsVBIIH->BufferSize);
}

inline void __stdcall Debug_ColorControl(DWORD_PTR category, UINT level, const DDCOLORCONTROL& rgCC)
{
#if ((_MSC_VER < 1400) || defined(_DEBUG))
	if((category & ATL_TRACE_CATEGORY) == 0) return;
#endif
	if(level > ATL_TRACE_LEVEL) return;

	//---------------------------------
	TCHAR szBuf[256] = _T("");
	LPTSTR psz = szBuf;

	if(rgCC.dwFlags & DDCOLOR_BRIGHTNESS)
		psz += wsprintf(psz, _T("  Brightness=%i"), rgCC.lBrightness);

	if(rgCC.dwFlags & DDCOLOR_CONTRAST)
		psz += wsprintf(psz, _T("  Contrast=%i"), rgCC.lContrast);

	if(rgCC.dwFlags & DDCOLOR_HUE)
		psz += wsprintf(psz, _T("  Hue=%i"), rgCC.lHue);

	if(rgCC.dwFlags & DDCOLOR_SATURATION)
		psz += wsprintf(psz, _T("  Saturation=%i"), rgCC.lSaturation);

	if(rgCC.dwFlags & DDCOLOR_SHARPNESS)
		psz += wsprintf(psz, _T("  Sharpness=%i"), rgCC.lSharpness);

	if(rgCC.dwFlags & DDCOLOR_GAMMA)
		psz += wsprintf(psz, _T("  Gamma=%i"), rgCC.lGamma);

	if(rgCC.dwFlags & DDCOLOR_COLORENABLE)
		psz += wsprintf(psz, _T("  ColorEnable=%i"), rgCC.lColorEnable);
						
	ATLTRACE2(category, level,
		_T("*** Surface properties : dwFlags=0x%X%s\n"), rgCC.dwFlags, szBuf);
}

//------------------------------------------------------------------------------

#else // !defined(DS_LOG_ENABLED)

struct DebugSample
{
	DebugSample(bool bUse = false) : m_bUse(bUse) {}
	bool m_bUse;
};

inline void __stdcall Debug_MediaSample(DWORD_PTR, UINT, LPCTSTR, IMediaSample*,
										DebugSample* /*pPrev*/ = NULL, LPCTSTR /*pszSuffix*/ = NULL)
{}
inline void __stdcall Debug_MediaSample2(DWORD_PTR, UINT, IMediaSample*,
										 LPCTSTR /*pszPrefix*/ = NULL, bool /*bExtended*/ = false)
{}
inline void __stdcall Debug_PinMediaTypes(DWORD_PTR, UINT, IUnknown*, LPCTSTR /*pszPrefix*/= NULL)
{}
inline void __stdcall Debug_MediaType(DWORD_PTR, UINT, const AM_MEDIA_TYPE*, LPCTSTR /*pszPrefix*/= NULL)
{}
inline void __stdcall Debug_WaveFormatEx(DWORD_PTR, UINT, const WAVEFORMATEX*, LPCTSTR /*pszPrefix*/ = NULL)
{}
inline void __stdcall Debug_BitmapInfoHeader(DWORD_PTR, UINT, const BITMAPINFOHEADER*,
											 LPCTSTR /*pszPrefix*/ = NULL)
{}
inline void __stdcall Debug_DvInfo(DWORD_PTR, UINT, const DVINFO*, LPCTSTR /*pszPrefix*/ = NULL)
{}
inline void __stdcall Debug_Time(DWORD_PTR, UINT, IReferenceClock*, LPCTSTR /*pszPrefix*/ = NULL)
{}
inline void __stdcall Debug_TimeDiff(DWORD_PTR, UINT, IReferenceClock*, REFERENCE_TIME,
									 LPCTSTR /*pszPrefix*/ = NULL)
{}
inline void __stdcall Debug_SampleLag(DWORD_PTR, UINT, IMediaSample*, REFERENCE_TIME, IReferenceClock*,
									  LPCTSTR /*pszPrefix*/ = NULL)
{}
inline void __stdcall Debug_Quality(DWORD_PTR, UINT, Quality&, LPCTSTR /*pszPrefix*/ = NULL)
{}
inline void __stdcall Debug_Allocator(DWORD_PTR, UINT, IMemAllocator*, LPCTSTR /*pszPrefix*/ = NULL)
{}
inline void __stdcall Debug_AllocatorProps(DWORD_PTR, UINT, ALLOCATOR_PROPERTIES*, LPCTSTR /*pszPrefix*/ = NULL)
{}
inline bool __stdcall Debug_Dump(DWORD_PTR, UINT, UINT, const void*, LPCTSTR /*pszPrefix*/ = NULL)
{ return true; }
inline bool __stdcall Debug_DumpSample(DWORD_PTR, UINT, IMediaSample*, LPCTSTR /*pszPrefix*/ = NULL)
{ return true; }
inline bool __stdcall Debug_FileDump(DWORD_PTR, UINT, HANDLE, LONGLONG, DWORD)
{ return true; }
inline bool __stdcall Debug_AsyncReaderDump(DWORD_PTR, UINT, IAsyncReader*, LONGLONG, DWORD)
{ return true; }
inline bool __stdcall Debug_Stream(DWORD_PTR, UINT, IStream*, bool /*bFromCurrentPosition*/ = false,
								   LPCTSTR /*pszPrefix*/ = NULL)
{ return true; }
inline void __stdcall Debug_MultiString(DWORD_PTR, UINT, LPTSTR)
{}
inline void __stdcall Debug_FilterFromPin(DWORD_PTR, UINT, LPCTSTR, IPin*)
{}
inline void __stdcall Debug_Filter(DWORD_PTR, UINT, LPCTSTR, REFCLSID, HRESULT)
{}
inline void __stdcall Debug_FilterInfo(DWORD_PTR, UINT, IBaseFilter*)
{}
inline void __stdcall Debug_MixerPinConfig(DWORD_PTR, UINT, IMixerPinConfig2*)
{}
inline void __stdcall Debug_AMStreamConfig(DWORD_PTR, UINT, IUnknown*, LPCTSTR /*pszPrefix*/ = NULL)
{}
inline void __stdcall Debug_KsPropertySet(DWORD_PTR, UINT, REFGUID, DWORD, DWORD, DWORD,
										  LPCTSTR /*pszPrefix1*/ = NULL, LPCTSTR /*pszPrefix2*/ = NULL)
{}
inline void __stdcall Debug_KsDataFormat(DWORD_PTR, UINT, KSDATAFORMAT*, LPCTSTR /*pszPrefix*/ = NULL)
{}
inline void __stdcall Debug_KsVBIInfoHeader(DWORD_PTR, UINT, KS_VBIINFOHEADER*, LPCTSTR /*pszPrefix*/ = NULL)
{}
inline void __stdcall Debug_ColorControl(DWORD_PTR, UINT, const DDCOLORCONTROL&)
{}

#endif	// DS_LOG_ENABLED

///////////////////////////////////////////////////////////////////////////
#endif	//__DSDEBUG_H_
