//---------------------------------------------------------------------------
// MCalls.h : 
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
// 2017/04/27 (*)
//---------------------------------------------------------------------------

#ifndef _M_CALLS_H_
#define _M_CALLS_H_

#ifndef __MFormats_h__
#include "MFormats.h"
#endif

#include <deque>
// USed in MFProps_OptionsEnum
typedef std::deque<std::pair<CComBSTR, CComBSTR> > TDeqStrPairs;

#include "../Base/MFunctions.h"
#include "../Base/MCreator.h"
#include "../WinAPI/blob.h"
#include <dxgi.h>

#ifndef IMOBJECT_CALL
#define IMOBJECT_CALL( _pObject, _call ) INTERFACE_CALL( IMObject, _pObject, _call )
#endif
#ifndef IMFILE_CALL
#define IMFILE_CALL( _pObject, _call ) INTERFACE_CALL( IMFile, _pObject, _call )
#endif
#ifndef IMFORMAT_CALL
#define IMFORMAT_CALL( _pObject, _call ) INTERFACE_CALL( IMFormat, _pObject, _call )
#endif
#ifndef IMPLUGINS_CALL
#define IMPLUGINS_CALL( _pObject, _call ) INTERFACE_CALL( IMPlugins, _pObject, _call )
#endif

#define MI_INTERLACE( _eInterlace ) ((_eInterlace) == eMI_Field1First ? 1 : (_eInterlace) == eMI_Field2First ? -1 : 0)

#define IS_FLAG( _eVal, eFlag )  ( ((_eVal) & (eFlag)) == (eFlag) )
#define IS_FLAG_MASK( _eVal, eFlag, eMask )  ( ((_eVal) & (eMask)) == (eFlag) )
#define SET_FLAG( _peVal, eFlag, _bSet ) ( *(ULONG*)(_peVal) = (ULONG)((_bSet) ? ((*(_peVal))|(eFlag)) : ((*(_peVal)) & ~(eFlag))) )
// M_TIME macros
#define IS_BREAK( _pTime ) (((_pTime)->eFFlags & eMFF_Break) != 0)
#define IS_REAL_BREAK( _pTime, _bAlsoPause ) (((_pTime)->eFFlags & (eMFF_Dup|(_bAlsoPause ? 0 : eMFF_Pause)) ) == (int)eMFF_Break )

#define IS_LIVE( _pTime ) IS_FLAG( (_pTime)->eFFlags, eMFF_Live ) // (((_pTime)->eFFlags & eMFF_Live) == eMFF_Live )
#define IS_DUP( _pTime ) (((_pTime)->eFFlags & eMFF_Dup) == eMFF_Dup)
#define IS_PAUSE( _pTime ) (((_pTime)->eFFlags & eMFF_Pause) == eMFF_Pause)
#define IS_NETWORK( _pTime ) (((_pTime)->eFFlags & eMFF_Net) == eMFF_Net)
#define IS_LAST( _pTime ) (((_pTime)->eFFlags & eMFF_Last) == eMFF_Last)
#define IS_SKIPPED( _pTime ) (((_pTime)->eFFlags & eMFF_SkippedFrame) == eMFF_SkippedFrame )
#define IS_ADDED( _pTime ) (((_pTime)->eFFlags & eMFF_AddedFrame) == eMFF_AddedFrame )
#define IS_REVERSE( _pTime ) (((_pTime)->eFFlags & eMFF_Reverse) == eMFF_Reverse)
#define SET_BREAK( _pTime, _bSet ) ((_pTime)->eFFlags = (eMFrameFlags)((_bSet) ? (((_pTime)->eFFlags & ~(eMFF_Dup|eMFF_Pause))|eMFF_Break) : ((_pTime)->eFFlags & ~eMFF_Break)) )
#define SET_DUP( _pTime, _bSet ) ((_pTime)->eFFlags = (eMFrameFlags)((_bSet) ? ((_pTime)->eFFlags|eMFF_Dup) : IS_DUP(_pTime) ? ((_pTime)->eFFlags & ~eMFF_Dup) : (_pTime)->eFFlags) )
#define SET_PAUSE( _pTime, _bSet ) ((_pTime)->eFFlags = (eMFrameFlags)((_bSet) ? ((_pTime)->eFFlags|eMFF_Pause) : IS_PAUSE(_pTime) ? ((_pTime)->eFFlags & ~eMFF_Pause) : (_pTime)->eFFlags) )
#define SET_LIVE( _pTime, _bSet ) ((_pTime)->eFFlags = (eMFrameFlags)((_bSet) ? ((_pTime)->eFFlags|eMFF_Live) : ((_pTime)->eFFlags & ~eMFF_Live)) )
#define SET_LAST( _pTime, _bSet ) ((_pTime)->eFFlags = (eMFrameFlags)((_bSet) ? ((_pTime)->eFFlags|eMFF_Last) : ((_pTime)->eFFlags & ~eMFF_Last)) )
#define SET_SKIPPED( _pTime, _bSet ) ((_pTime)->eFFlags = (eMFrameFlags)((_bSet) ? ((_pTime)->eFFlags|eMFF_SkippedFrame) : ((_pTime)->eFFlags & ~eMFF_SkippedFrame)) )
#define SET_ADDED( _pTime, _bSet ) ((_pTime)->eFFlags = (eMFrameFlags)((_bSet) ? ((_pTime)->eFFlags|eMFF_AddedFrame) : ((_pTime)->eFFlags & ~eMFF_AddedFrame)) )
#define SET_NETWORK( _pTime, _bSet ) ((_pTime)->eFFlags = (eMFrameFlags)((_bSet) ? ((_pTime)->eFFlags|eMFF_Net) : ((_pTime)->eFFlags & ~eMFF_Net)) )
#define SET_REVERSE( _pTime, _bSet ) ((_pTime)->eFFlags = (eMFrameFlags)((_bSet) ? ((_pTime)->eFFlags|eMFF_Reverse) : ((_pTime)->eFFlags & ~eMFF_Reverse)) )

#define TIME_LEN( _pTime ) ( (_pTime)->rtEndTime - (_pTime)->rtStartTime )
#define TIME_IS_EMPTY( _pTime ) ( (_pTime)->rtStartTime == 0 && (_pTime)->rtEndTime == (_pTime)->rtStartTime )

// For buffer info
#define BUFFER_POS_HAVE( _eFlags )	( ( (_eFlags) & eMFF_BufferPos_Overflow) != 0 )
#define BUFFER_POS_GET( _eFlags )	(UINT)(_eFlags & eMFF_BufferPos_Overflow)

inline double BufferPosDbl(eMFrameFlags _eFlags)
{
	if (!BUFFER_POS_HAVE(_eFlags))
		return 0.0;

	int nPos = BUFFER_POS_GET(_eFlags);
	if (nPos >= eMFF_BufferPos_Min)
	{
		UINT nScale = (UINT)(eMFF_BufferPos_Max - eMFF_BufferPos_Min) >> 24;

		return (double)((nPos - eMFF_BufferPos_Min) >> 24) / nScale;
	}

	UINT nScale = (UINT)(eMFF_BufferPos_Min - eMFF_BufferPos_Zero) >> 24;

	return (double)((nPos - eMFF_BufferPos_Min) >> 24) / nScale;
}

#define MF_NO_TIME_LL	(MINLONGLONG)


//////////////////////////////////////////////////////////////////////////
// MFFrame helpers
// _nSetFlag < 0 -> Do nothing (return src flag)
// _nSetFlag == 0 -> Reset flag
// _nSetFlag == 1 -> Set flag
// _nSetFlag == 2 -> Set break via SET_BREAK() (reset Dup & Pause)
// 

inline bool MF_IsLocal( IUnknown* _pFrame )
{
	return MCreator::IsLocal(_pFrame);
}

// Return both (field 1 & field 2) textures
// Usage:
// CComPtr<ID3D11Texture2D> arrFrameTex[2];
// MFFrame_GetFieldsTextures(_pFrame, arrFrameTex);
template <class TTextureInterface>
inline HRESULT MFFrame_GetFieldsTextures(IUnknown* _pFrame, CComPtr<TTextureInterface>* _ppTextures, LPCWSTR _pszHints = NULL)
{
	CComQIPtr<IMFFrameGPU> qpMFFrameGPU(_pFrame);
	if (!qpMFFrameGPU)
		return E_NOINTERFACE;

	CComPtr<IUnknown> arrTexture[2];
	HRESULT hr = qpMFFrameGPU->MFVideoGetTexture(0, &arrTexture[0], CComBSTR(_pszHints));
	if (hr != S_OK)
		return HRESULT_FROM_WIN32(ERROR_EMPTY);

	qpMFFrameGPU->MFVideoGetTexture(1, &arrTexture[1], CComBSTR(_pszHints));

	ATLASSERT(arrTexture[0]);
	if (_ppTextures)
	{
		hr = arrTexture[0].QueryInterface(&_ppTextures[0]);
		if (FAILED(hr))
			return hr;

		if (arrTexture[1])
			arrTexture[1].QueryInterface(&_ppTextures[1]);
	}

	return S_OK;
}



template <class TTexture> 
inline HRESULT MFFrame_GetTexture(int _nField, IUnknown* _pFrame, TTexture** _ppTexture, LPCWSTR _pszHints = NULL )
{
	CComQIPtr<IMFFrameGPU> qpMFFrameGPU( _pFrame );
	if( !qpMFFrameGPU ) 
		return E_NOINTERFACE;

	CComPtr<IUnknown> cpTexture;
	HRESULT hr = qpMFFrameGPU->MFVideoGetTexture(_nField, &cpTexture, CComBSTR(_pszHints) );
	if( hr != S_OK )
		return HRESULT_FROM_WIN32( ERROR_EMPTY );

	ATLASSERT( cpTexture );
	if( _ppTexture )
	{
		hr = cpTexture.QueryInterface( _ppTexture );
		if( FAILED( hr ) )
			return hr;
	}

	return S_OK;
}

// template <>
// inline HRESULT MFFrame_GetTexture<IDX11Texture>(int _nField, IUnknown* _pFrame, IDX11Texture** _ppTexture, LPCWSTR _pszHints = NULL)
// {
// 	CComQIPtr<IMFFrameGPU> qpMFFrameGPU(_pFrame);
// 	if (!qpMFFrameGPU)
// 		return E_NOINTERFACE;
// 
// 	CComBSTR cbsHints = _pszHints;
// 	cbsHints.Append(L" dx11texture=true");
// 
// 	CComPtr<IUnknown> cpTexture;
// 	HRESULT hr = qpMFFrameGPU->MFVideoGetTexture(_nField, &cpTexture, cbsHints);
// 	if (hr != S_OK)
// 		return HRESULT_FROM_WIN32(ERROR_EMPTY);
// 
// 	ATLASSERT(cpTexture);
// 	if (_ppTexture)
// 	{
// 		hr = cpTexture.QueryInterface(_ppTexture);
// 		if (FAILED(hr))
// 			return hr;
// 	}
// 
// 	return S_OK;
// }



//////////////////////////////////////////////////////////////////////////
// SideData implementation

template <class TInterface>
inline HRESULT MFSideData_ObjSequenceGet(IUnknown* _pFrame, LPCWSTR _pszObjBase, std::deque<CComPtr<TInterface> >& _rDeqObjects, BOOL _bMakeLocal)
{
	CComQIPtr<IMFSideData> qpMFFrame(_pFrame ? _pFrame : MCreator::GetMFFactory());
	if (!qpMFFrame)
		return E_NOINTERFACE;

	int nIndex = 0;
	while (true)
	{
		CComPtr<IUnknown> cpFrameObj;
		HRESULT hr = qpMFFrame->MFObjGet(STR_IS_EMPTY(_pszObjBase) ? NULL : CComBSTR(INDEXEDNAME(_pszObjBase, nIndex)), &cpFrameObj, _bMakeLocal);
		if (FAILED(hr))
			return hr;

		CComQIPtr<TInterface> qpReqObj(cpFrameObj);
		if (!qpReqObj)
			return nIndex == 0 ? HRESULT_FROM_WIN32(ERROR_NOT_FOUND) : S_OK;

		_rDeqObjects.push_back(qpReqObj);
		nIndex++;
	}

	return S_OK;
}

template <class TInterface>
inline HRESULT MFSideData_ObjGetByIndex(IUnknown* _pFrame, int _nIndex, BSTR* _pbsDataID, TInterface** _ppObject, BOOL _bMakeLocal)
{
	CComQIPtr<IMFSideData> qpMFFrame(_pFrame ? _pFrame : MCreator::GetMFFactory());
	if (!qpMFFrame)
		return E_NOINTERFACE;

	CComPtr<IUnknown> cpFrameObj;
	HRESULT hr = qpMFFrame->MFObjGetByIndex(_nIndex, _pbsDataID, &cpFrameObj, _bMakeLocal);
	if (FAILED(hr))
		return hr;

	if (!cpFrameObj) return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);

	return cpFrameObj.QueryInterface(_ppObject);
}

template <class TInterface>
inline HRESULT MFSideData_ObjGet(IUnknown* _pFrame, LPCWSTR _pszObjName, TInterface** _ppObject, BOOL _bMakeLocal = FALSE)
{
	CComQIPtr<IMFSideData> qpMFFrame(_pFrame ? _pFrame : MCreator::GetMFFactory());
	if (!qpMFFrame)
		return E_NOINTERFACE;

	CComPtr<IUnknown> cpFrameObj;
	HRESULT hr = qpMFFrame->MFObjGet(_pszObjName ? CComBSTR(_pszObjName) : NULL, &cpFrameObj, _bMakeLocal);
	if (FAILED(hr))
		return hr;

	if (!cpFrameObj) return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);

	return cpFrameObj.QueryInterface(_ppObject);
}

inline CComPtr<IUnknown> MFSideData_ObjGet(IUnknown* _pFrame, LPCWSTR _pszObjName, BOOL _bMakeLocal = FALSE)
{
	CComPtr<IUnknown> cpFrameObj;
	MFSideData_ObjGet(_pFrame, _pszObjName, &cpFrameObj, _bMakeLocal);
	return cpFrameObj;
}

inline HRESULT MFSideData_ObjSet(IUnknown* _pFrame, LPCWSTR _pszObjName, IUnknown* _pObject)
{
	CComQIPtr<IMFSideData> qpMFFrame(_pFrame ? _pFrame : MCreator::GetMFFactory());
	if (!qpMFFrame)
		return E_NOINTERFACE;

	return qpMFFrame->MFObjSet(_pszObjName ? CComBSTR(_pszObjName) : NULL, _pObject);
}

inline HRESULT MFSideData_ObjRemove(IUnknown* _pFrame, LPCWSTR _pszDataName)
{
	CComQIPtr<IMFSideData> qpMFFrame(_pFrame ? _pFrame : MCreator::GetMFFactory());
	if (!qpMFFrame)
		return E_NOINTERFACE;

	return qpMFFrame->MFObjSet(CComBSTR(_pszDataName), NULL);
}

inline HRESULT MFSideData_DataSet(IUnknown* _pFrame, LPCWSTR _pszDataName, LONGLONG _lpData, LONG _cbData = 0)
{
	CComQIPtr<IMFSideData> qpMFFrame(_pFrame ? _pFrame : MCreator::GetMFFactory());
	if (!qpMFFrame)
		return E_NOINTERFACE;

	return qpMFFrame->MFDataSet(CComBSTR(_pszDataName), _cbData, _lpData);
}

inline HRESULT MFSideData_DataRemove(IUnknown* _pFrame, LPCWSTR _pszDataName)
{
	CComQIPtr<IMFSideData> qpMFFrame(_pFrame ? _pFrame : MCreator::GetMFFactory());
	if (!qpMFFrame)
		return E_NOINTERFACE;

	return qpMFFrame->MFDataSet(CComBSTR(_pszDataName), -1, NULL);
}

inline HRESULT MFSideData_DataRemoveList(IUnknown* _pFrame, LPCWSTR* _ppszDataList)
{
	ATLASSERT(_ppszDataList);

	CComQIPtr<IMFSideData> qpMFFrame(_pFrame ? _pFrame : MCreator::GetMFFactory());
	if (!qpMFFrame)
		return E_NOINTERFACE;

	while (_ppszDataList && *_ppszDataList)
	{
		qpMFFrame->MFDataSet(CComBSTR(*_ppszDataList), -1, NULL);

		++_ppszDataList;
	}

	return S_OK;
}

inline LONGLONG MFSideData_DataGet(IUnknown* _pFrame, LPCWSTR _pszDataName, LONG* _pcbData = NULL, LONGLONG _llDefValue = 0)
{
	CComQIPtr<IMFSideData> qpMFFrame(_pFrame ? _pFrame : MCreator::GetMFFactory());
	if (!qpMFFrame)
		return _llDefValue;

	LONG cbData = -1;
	LONGLONG lpData = _llDefValue;
	qpMFFrame->MFDataGet(_pszDataName ? CComBSTR(_pszDataName) : NULL, &cbData, &lpData);

	if (_pcbData)
		*_pcbData = cbData;

	return lpData;
}

inline M_TIME* MFSideData_MTimeGet(IUnknown* _pFrame, LPCWSTR _pszDataName)
{
	LONG cbData = 0;
	M_TIME* pTime = (M_TIME*)MFSideData_DataGet(_pFrame, _pszDataName, &cbData);
	if (cbData != sizeof(M_TIME))
		return NULL;

	return pTime;
}

inline LONGLONG MFSideData_DataGetByIndex(IUnknown* _pFrame, int _nIndex, BSTR* _pbsDataID = NULL, LONG* _pcbData = NULL)
{
	CComQIPtr<IMFSideData> qpMFFrame(_pFrame ? _pFrame : MCreator::GetMFFactory());
	if (!qpMFFrame)
		return E_NOINTERFACE;

	CComBSTR cbsDataID;
	LONG cbData = -1;
	LONGLONG lpData = 0;
	qpMFFrame->MFDataGetByIndex(_nIndex, &cbsDataID, &cbData, &lpData);

	if (_pbsDataID)
		*_pbsDataID = cbsDataID.Detach();

	if (_pcbData)
		*_pcbData = cbData;

	return lpData;
}

inline HRESULT MFSideData_StrSet(IUnknown* _pFrame, LPCWSTR _pszDataName, LPCWSTR _pszString)
{
	CComQIPtr<IMFSideData> qpMFFrame(_pFrame ? _pFrame : MCreator::GetMFFactory());
	if (!qpMFFrame)
		return E_NOINTERFACE;

	return qpMFFrame->MFStrSet(_pszDataName ? CComBSTR(_pszDataName) : NULL, _pszString ? CComBSTR(_pszString) : NULL);
}

inline CComBSTR MFSideData_StrGet(IUnknown* _pFrame, LPCWSTR _pszDataName)
{
	CComQIPtr<IMFSideData> qpMFFrame(_pFrame ? _pFrame : MCreator::GetMFFactory());
	if (!qpMFFrame)
		return E_NOINTERFACE;

	CComBSTR cbsString;
	qpMFFrame->MFStrGet(_pszDataName ? CComBSTR(_pszDataName) : _pszDataName, &cbsString);

	return cbsString;
}

inline HRESULT MFSideData_StrRemove(IUnknown* _pFrame, LPCWSTR _pszDataName)
{
	CComQIPtr<IMFSideData> qpMFFrame(_pFrame ? _pFrame : MCreator::GetMFFactory());
	if (!qpMFFrame)
		return E_NOINTERFACE;

	return qpMFFrame->MFStrSet(CComBSTR(_pszDataName), NULL);
}

inline HRESULT MFSideData_Copy_CreateTime(IUnknown* _pDataFrom, IUnknown* _pDataTo)
{
	LONGLONG llCreateTime = MFSideData_DataGet(_pDataFrom, MFSD_CREATETIME_DATA_SETGET);
	if (llCreateTime > 0)
	{
		MFSideData_DataSet(_pDataTo, MFSD_CREATETIME_DATA_SETGET, llCreateTime);
		return S_OK;
	}
	return E_NOINTERFACE;
}
// Flags:
// 0 - Copy string / data (via MFSave/MFLoad), WARNING !!! Do not copy objects, for copy objects use MFSideData_Copy(..., -1)
// 1 - Copy string
// 2 - Copy data
// 4 - Copy object interfaces
inline HRESULT MFSideData_Copy(IUnknown* _pDataFrom, IUnknown* _pDataTo, int _nCopyFlags = 0)
{
	CComQIPtr<IMFSideData> qpSD_From(_pDataFrom);
	CComQIPtr<IMFSideData> qpSD_To(_pDataTo);
	if (!qpSD_From || !qpSD_To)
		return E_NOINTERFACE;

	if (_nCopyFlags == 0)
	{
		CComQIPtr<IMFFrame> qpMFrame_From(_pDataFrom);
		CComQIPtr<IMFFrame> qpFrame_To(_pDataTo);
		if (qpFrame_To && qpMFrame_From)
		{
			DWORD cbData = 0;
			LPBYTE pbData = NULL;
			qpMFrame_From->MFSave(&cbData, &pbData, eMFC_Data);
			if (cbData && pbData)
				qpFrame_To->MFLoad(cbData, pbData, eMFC_Data);
			::CoTaskMemFree(pbData);
		}
		else
		{
			CComPtr<IUnknown> cpMFBufferSD_Remote;
			qpSD_From->MFObjGet(NULL, &cpMFBufferSD_Remote, TRUE);
			if (cpMFBufferSD_Remote)
			{
				qpSD_To->MFObjSet(NULL, cpMFBufferSD_Remote);
			}
		}
	}
	else
	{
		// WARNING !!! Only for local data
		ATLASSERT(MF_IsLocal(_pDataFrom) && MF_IsLocal(_pDataTo));

		int nIndex = 0;
		while (_nCopyFlags & 1)
		{
			CComBSTR cbsStr;
			CComBSTR cbsDataID;
			HRESULT hr = qpSD_From->MFStrGetByIndex(nIndex++, &cbsDataID, &cbsStr);
			if (hr == S_OK)
			{
				qpSD_To->MFStrSet(cbsDataID, cbsStr);
				continue;
			}

			nIndex = 0;
			break;
		}

		while (_nCopyFlags & 2)
		{
			LONG cbData = 0;
			LONGLONG lpData = NULL;
			CComBSTR cbsDataID;
			HRESULT hr = qpSD_From->MFDataGetByIndex(nIndex++, &cbsDataID, &cbData, &lpData);
			if (hr == S_OK)
			{
				qpSD_To->MFDataSet(cbsDataID, cbData, lpData);
				continue;
			}

			nIndex = 0;
			break;
		}

		while (_nCopyFlags & 4)
		{
			CComPtr<IUnknown> cpObj;
			CComBSTR cbsDataID;
			HRESULT hr = qpSD_From->MFObjGetByIndex(nIndex++, &cbsDataID, &cpObj, FALSE);
			if (hr == S_OK)
			{
				qpSD_To->MFObjSet(cbsDataID, cpObj);
				continue;
			}

			nIndex = 0;
			break;
		}
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
// MFFrame helpers

inline double MFFrame_DelayMsec(IUnknown* _pFrame, LONGLONG* _pllCreateTime = NULL)
{
	LONGLONG llCreateTime = MFSideData_DataGet(_pFrame, MFSD_CREATETIME_DATA_SETGET);

	if (_pllCreateTime)
		*_pllCreateTime = llCreateTime;

	return llCreateTime == 0 ? 0.0 : (double)(GetLocalTime() - llCreateTime) / DS_MSEC;
}


template <class TFrame> 
inline HRESULT MFFrame_UpdateFakeAV( IUnknown* _pFrame, M_AV_PROPS* _pAVPropsOut, TFrame** _ppFrameUpdate )
{
	if( !_pAVPropsOut )
		return S_FALSE;

	MF_FRAME_INFO mfInfo = MFFrame_Info( _pFrame );

	CComQIPtr<IMFFrame> cpFrameOut = _pFrame;
	if( !cpFrameOut ) return E_UNEXPECTED;

	// Check for audio/video only inputs
	if( _pAVPropsOut && IsSpecified( &_pAVPropsOut->vidProps) && (!IsSpecified( &mfInfo.avProps.vidProps ) || (mfInfo.cbVideo < 4 && !MFFrame_IsGPU(_pFrame) ) ) )
	{
		mfInfo.avProps.vidProps = _pAVPropsOut->vidProps;

		CComPtr<IMFFrame> cpFrameVid;
		HRESULT hr = MCreator::MFFrameCreateFromMem( &mfInfo.avProps, NULL, mfInfo.lAudioSamples, mfInfo.lpAudio, &cpFrameVid, NULL );
		if( FAILED( hr ) )
			return hr;

		if( cpFrameVid )
		{
			cpFrameOut = cpFrameVid;
			cpFrameOut->MFTimeSet( &mfInfo.mTime );
		}
	}

	if( _pAVPropsOut && IsSpecified( &_pAVPropsOut->audProps ) && (!IsSpecified( &mfInfo.avProps.audProps ) || mfInfo.cbAudio < 4 ) )
	{
		LONG cbAudio = MAudTime2Bytes( &_pAVPropsOut->audProps, mfInfo.mTime.rtEndTime - mfInfo.mTime.rtStartTime );
		if( cbAudio > 0 )
		{
			cpFrameOut->MFAudioSet( &_pAVPropsOut->audProps, cbAudio, NULL );
		}
	}

	return cpFrameOut.QueryInterface( _ppFrameUpdate );
}

inline eMFrameFlags MFFrame_UpdateFlag( IUnknown* _pFrame, int _eFlag, int _nSetFlag )
{
	eMFrameFlags eFlagsSrc = eMFF_None;
	CComQIPtr<IMFFrame> qpMFFrame( _pFrame );
	if( qpMFFrame )
	{
		M_TIME mTime = {};
		qpMFFrame->MFTimeGet( &mTime );
		eFlagsSrc = mTime.eFFlags;

		if( _nSetFlag == 2 && _eFlag == eMFF_Break )
			SET_BREAK( &mTime, true );
		else if( _nSetFlag > 0 )
			mTime.eFFlags = (eMFrameFlags)(mTime.eFFlags | _eFlag);
		else if( _nSetFlag == 0 )
			mTime.eFFlags = (eMFrameFlags)(mTime.eFFlags & ~_eFlag);

		if( _nSetFlag >= 0 && eFlagsSrc != mTime.eFFlags )
			qpMFFrame->MFTimeSet( &mTime );
	}

	return eFlagsSrc;
}

inline void MFFrame_WaitAsync( IUnknown* _pFrame )
{
	CComQIPtr<IMFFrame> qpMFFrame( _pFrame );
	ATLASSERT( qpMFFrame || !_pFrame );
	if( qpMFFrame )
		qpMFFrame->MFWaitAsync();
}

inline HRESULT MFFrame_WaitExecution(IUnknown* _pFrame, LPCWSTR _pszHints = NULL )
{
	CComQIPtr<IMFFrameGPU> qpMFFrame(_pFrame);
	ATLASSERT(qpMFFrame || !_pFrame);
	if (qpMFFrame)
		return qpMFFrame->MFVideoWaitExecution(CComBSTR(_pszHints));

	return E_NOINTERFACE;
}

inline MF_FRAME_INFO MFFrame_Info( IUnknown* _pFrame )
{
	MF_FRAME_INFO mfInfo = {};
	CComQIPtr<IMFFrame> qpMFFrame( _pFrame );
	ATLASSERT( qpMFFrame || !_pFrame );
	if( qpMFFrame )
	{
		qpMFFrame->MFAllGet( &mfInfo );
	}

	return mfInfo;
}


inline bool MFInfo_HaveVideo(MF_FRAME_INFO* _pFrameInfo)
{
	return _pFrameInfo &&
		(_pFrameInfo->lpVideo && _pFrameInfo->cbVideo > 8 || _pFrameInfo->avProps.vidProps.eGPUFlags != eGTF_CPU_Frame) &&
		_pFrameInfo->avProps.vidProps.nWidth > 0 &&
		_pFrameInfo->avProps.vidProps.nHeight != 0 &&
		_pFrameInfo->avProps.vidProps.fccType != eMFCC_Default;

}

inline bool MFInfo_HaveAudio(MF_FRAME_INFO* _pFrameInfo)
{
	return _pFrameInfo &&
		_pFrameInfo->lpAudio &&
		_pFrameInfo->cbAudio > 8 &&
		_pFrameInfo->avProps.audProps.nChannels > 0 &&
		_pFrameInfo->avProps.audProps.nBitsPerSample != 0 &&
		_pFrameInfo->avProps.audProps.nSamplesPerSec > 0;

}

inline bool MFFrame_HaveVideo(IUnknown* _pFrame)
{
	MF_FRAME_INFO mfInfo = MFFrame_Info(_pFrame);
	return MFInfo_HaveVideo(&mfInfo);
}

inline bool MFFrame_HaveAudio(IUnknown* _pFrame)
{
	MF_FRAME_INFO mfInfo = MFFrame_Info(_pFrame);
	return MFInfo_HaveAudio(&mfInfo);
}

inline M_AV_PROPS MFFrame_AVPropsEx(IUnknown* _pFrame, LONG* _plAudioSamples, MF_VID_PTR* _pVidPtr, LPBYTE* _pbAudio = NULL, bool* _pbQI_Ok = NULL)
{
	M_AV_PROPS avProps = {};
	CComQIPtr<IMFFrame> qpMFFrame(_pFrame);
	if (qpMFFrame)
	{
		if (_pbQI_Ok)
			*_pbQI_Ok = true;

		long lAudioSamples = 0; // For RPC
		qpMFFrame->MFAVPropsGet(&avProps, &lAudioSamples);
		if (_plAudioSamples)
			*_plAudioSamples = lAudioSamples;

		if (_pVidPtr)
			qpMFFrame->MFVideoGetBytesEx(_pVidPtr);

		if (_pbAudio)
		{
			LONG cbAudio = 0;
			LONGLONG lpAudio = 0;
			qpMFFrame->MFAudioGetBytes(&cbAudio, &lpAudio);
			*_pbAudio = (LPBYTE)lpAudio;
		}
	}
	else if (_pbQI_Ok)
	{
		*_pbQI_Ok = false;
	}

	return avProps;
}

// TODO: Move all Img_XXX function from MHelpers to another one
// Return pointer to video plane start (usefull for RGB bottom-top images)
inline LPBYTE Img_VidPlane(MF_VID_PTR* _pVidPtr, int _nPlane, int* _pnRowBytes = NULL)
{
	if (!_pVidPtr)
		return NULL;

	_nPlane = MAX(0, MIN(_nPlane, 2));

	LPBYTE pbVideo = (LPBYTE)_pVidPtr->lpVideoPlanes[_nPlane];
	if (pbVideo && _pVidPtr->cbVideoRowBytes[_nPlane] < 0)
	{
		ATLASSERT(_pVidPtr->szVideoPlanes[_nPlane].cy > 0);
		pbVideo += _pVidPtr->cbVideoRowBytes[_nPlane] * (_pVidPtr->szVideoPlanes[_nPlane].cy - 1);
	}

	if (_pnRowBytes && _pVidPtr->cbVideoRowBytes[_nPlane] != 0)
		*_pnRowBytes = ABS(_pVidPtr->cbVideoRowBytes[_nPlane]);

	return pbVideo;
}

inline M_AV_PROPS MFFrame_AVProps( IUnknown* _pFrame, LONG* _plAudioSamples = NULL, LPBYTE* _pbVideo = NULL, LPBYTE* _pbAudio = NULL, bool* _pbQI_Ok = NULL )
{
	MF_VID_PTR vidPtr = {};
	M_AV_PROPS avProps = MFFrame_AVPropsEx(_pFrame, _plAudioSamples, _pbVideo ? &vidPtr : NULL, _pbAudio, _pbQI_Ok);
	if (_pbVideo)
		*_pbVideo = Img_VidPlane(&vidPtr, 0, &avProps.vidProps.nRowBytes );
	
	return avProps;
}

inline double MFFrame_AvgTimeRT(IUnknown* _pFrame, double* _pdblRate = NULL, double _dblDefRate = 25.0)
{
	CComQIPtr<IMFFrame> qpMFFrame(_pFrame);
	ATLASSERT(qpMFFrame || !_pFrame);
	if (!qpMFFrame)
	{
		// VVB: Do not set _dblDefRate to 25.0 - at least for have ability to detect wrong frames
		//_dblDefRate = _dblDefRate > 0 ? _dblDefRate : 25.0;
		if (_pdblRate)
			*_pdblRate = _dblDefRate;

		return Rate2AvgTimeDbl(_dblDefRate);
	}

	M_TIME mTime = {};
	qpMFFrame->MFTimeGet(&mTime);

	LONG lAudioSamples = 0;
	M_AV_PROPS avProps = MFFrame_AVProps( _pFrame, &lAudioSamples );
	
	double dblAvgTimeT = TIME_LEN(&mTime);
	if (dblAvgTimeT <= 0.0 && avProps.audProps.nSamplesPerSec > 0)
		dblAvgTimeT = (double)lAudioSamples * DS_ONESEC / avProps.audProps.nSamplesPerSec;

	double dblAvgTimeV = Rate2AvgTimeDbl(avProps.vidProps.dblRate, dblAvgTimeT);
	
	if (dblAvgTimeV <= 0 )
		dblAvgTimeV = DS_ONESEC / (_dblDefRate > 0 ? _dblDefRate : 25.0); 
	
	if (_pdblRate)
		*_pdblRate = DS_ONESEC / dblAvgTimeV;

	ATLASSERT(dblAvgTimeV > 0);
	return dblAvgTimeV;
}

inline M_TIME MFFrame_Time(IUnknown* _pFrame, REFERENCE_TIME* _prtFrameDuration = NULL, double _dblDefRate = 25.0)
{
	M_TIME mTime = {};
	CComQIPtr<IMFFrame> qpMFFrame(_pFrame);
	ATLASSERT(qpMFFrame || !_pFrame);
	if (qpMFFrame)
		qpMFFrame->MFTimeGet(&mTime);

	if (_prtFrameDuration)
	{
		if (mTime.rtEndTime > mTime.rtStartTime + DS_MSEC)
		{
			*_prtFrameDuration = mTime.rtEndTime - mTime.rtStartTime;
		}
		else
		{
			*_prtFrameDuration = (REFERENCE_TIME)(MFFrame_AvgTimeRT(_pFrame, NULL, _dblDefRate) + 0.5);
		}
	}

	return mTime;
}

inline HRESULT MFFrame_TimeSet(IUnknown* _pFrame, M_TIME* _pTime)
{
	CComQIPtr<IMFFrame> qpMFFrame(_pFrame);
	ATLASSERT(qpMFFrame || !_pFrame);
	if (!qpMFFrame) return E_NOINTERFACE;

	return qpMFFrame->MFTimeSet(_pTime);
}

inline bool MTimeRev(IN OUT M_TIME* _pTime, bool _bReverse)
{
	ATLASSERT(_pTime);
	if (!_pTime) return false;

	bool bIsReverse = IS_FLAG(_pTime->eFFlags, eMFF_ReverseTime);
	if (bIsReverse == _bReverse)
		return false;

	REFERENCE_TIME tS = _pTime->rtStartTime;
	_pTime->rtStartTime = _pTime->rtEndTime * -1;
	_pTime->rtEndTime = tS * -1;

	SET_FLAG(&_pTime->eFFlags, eMFF_ReverseTime, _bReverse);

	return true;
}

inline HRESULT MFFrame_TimeRev(IUnknown* _pFrame, bool _bReverse, M_TIME* _pTime = NULL )
{
	CComQIPtr<IMFFrame> qpMFFrame(_pFrame);
	ATLASSERT(qpMFFrame || !_pFrame);
	if (!qpMFFrame) return E_NOINTERFACE;

	M_TIME mTime = {};
	qpMFFrame->MFTimeGet(&mTime);

	if (!MTimeRev(&mTime, _bReverse))
		return S_FALSE;

	qpMFFrame->MFTimeSet(&mTime);

	if (_pTime)
		*_pTime = mTime;

	return S_OK;
}

inline bool MFFrame_IsLocal( IUnknown* _pFrame )
{
	CComQIPtr<IMFFrame> qpMFFrame( _pFrame );
	if( qpMFFrame )
	{
		DWORD dwProcessID = 0;
		qpMFFrame->MFGetProcessID( &dwProcessID );
		if( dwProcessID == GetCurrentProcessId() )
			return true;
	}

	return false;
}


inline bool MFFrame_IsSameVideo( IUnknown* _pFrameOne, IUnknown* _pFrameTwo, bool _bCheckCut = false )
{
	CComQIPtr<IMFFrame> qpMFFrame[2] = { _pFrameOne, _pFrameTwo };
	if( !qpMFFrame[0] || !qpMFFrame[1] )
		return qpMFFrame[1].p == qpMFFrame[0].p;

	MF_FRAME_INFO mfInfo[2] = {};
	qpMFFrame[0]->MFAllGet( &mfInfo[0] );
	qpMFFrame[1]->MFAllGet( &mfInfo[1] );

	if (mfInfo[0].lpVideo == 0 && mfInfo[1].lpVideo == 0 )
	{
		CComPtr<IUnknown> cpTex[2];

		// TODO: Check second field ?
		MFFrame_GetTexture(0, _pFrameOne, &cpTex[0]);
		MFFrame_GetTexture(0, _pFrameTwo, &cpTex[1]);

		return cpTex[0].IsEqualObject(cpTex[1]);
	}

	return _bCheckCut ? mfInfo[0].lpVideo == mfInfo[1].lpVideo : mfInfo[0].llVideoID == mfInfo[1].llVideoID;
}

inline HRESULT MFFrame_CopyTime(IUnknown* _pFrameFrom, IUnknown* _pFrameTo, M_TIME* _pTimeFrom = NULL)
{
	CComQIPtr<IMFFrame> qpFrameIn(_pFrameFrom);
	CComQIPtr<IMFFrame> qpFrameOut(_pFrameTo);
	if (!qpFrameIn || !qpFrameOut)
		return E_NOINTERFACE;

	M_TIME mTime = {};
	qpFrameIn->MFTimeGet(&mTime);
	qpFrameOut->MFTimeSet(&mTime);

	if (_pTimeFrom)
		*_pTimeFrom = mTime;

	return S_OK;
}

template<class TFrame>
inline HRESULT MFFrame_Convert(IUnknown* _pFrameFrom, M_AV_PROPS* _pAVProps, TFrame** _ppFrameConvert, int *_pnFrameRest = NULL, LPCWSTR _pszPropsList = NULL, LPCWSTR _pszConverterID = NULL)
{
	CComQIPtr<IMFFrame> qpFrameIn(_pFrameFrom);
	if (!qpFrameIn)
		return E_NOINTERFACE;

	CComPtr<IMFFrame> cpMFrameCnv;
	HRESULT hr = qpFrameIn->MFConvert(_pAVProps, &cpMFrameCnv, _pnFrameRest, CComBSTR(_pszPropsList), CComBSTR(_pszConverterID));
	if (FAILED(hr))
		return hr;

	ATLASSERT(cpMFrameCnv);
	if (!cpMFrameCnv)
		return S_FALSE;

	return cpMFrameCnv.QueryInterface(_ppFrameConvert);
}

inline HRESULT MFFrame_CopyData( IUnknown* _pFrameFrom, IUnknown* _pFrameTo )
{
	CComQIPtr<IMFFrame> qpFrameIn( _pFrameFrom );
	CComQIPtr<IMFFrame> qpFrameOut( _pFrameTo );
	if( !qpFrameIn || !qpFrameOut )
		return E_NOINTERFACE;

	MF_FRAME_INFO mfInfo = {};
	qpFrameIn->MFAllGet( &mfInfo );

	if( mfInfo.nDataCount || mfInfo.nStrCount || mfInfo.nObjCount )
	{
		CComPtr<IUnknown> cpMFBufferSD_Remote;
		qpFrameIn->MFObjGet( NULL, &cpMFBufferSD_Remote, FALSE );
		if( cpMFBufferSD_Remote )
		{
			qpFrameOut->MFObjSet( NULL, cpMFBufferSD_Remote );
		}

		// 		DWORD cbData = 0;
		// 		LPBYTE pbData = NULL;
		// 		qpFrameIn->MFSave( &cbData, &pbData, eMFC_Data );
		// 		
		// 		qpFrameOut->MFLoad( cbData, pbData, eMFC_Data );
		// 		::CoTaskMemFree(pbData);

		return S_OK;
	}

	return S_FALSE;
}

inline HRESULT MFFrame_SaveToFile(IUnknown* _pFrame, LPCWSTR _pszName, int _nIndex = -1, int _nMaxAuto = 100 )
{
	CComQIPtr<IMFFrame> qpMFFrame(_pFrame);
	if (!qpMFFrame || STR_IS_EMPTY(_pszName) )
		return HRESULT_FROM_WIN32(ERROR_EMPTY);

	CComBSTR cbsPath = _pszName;

	if (_nIndex == -2)
	{
		static int nTestIndex = 0;
		_nIndex = nTestIndex++;
		nTestIndex %= MAX(_nMaxAuto,1);
	}

	if (_nIndex >= 0)
	{
		WCHAR szIndex[16] = {};
		wsprintfW(szIndex, L".%03d", _nIndex);
		InsertBeforeExtension(cbsPath, szIndex);
	}

	MTRACE("(%ws)", cbsPath.m_str);

	return qpMFFrame->MFVideoSaveToFile(cbsPath);
}

inline int MFFrame_IsGPU(IUnknown* _pFrame, DXGI_FORMAT* _pDXGIFormat = NULL )
{
	M_AV_PROPS avProps = MFFrame_AVProps(_pFrame);

	if (_pDXGIFormat)
		*_pDXGIFormat = (DXGI_FORMAT)(avProps.vidProps.eGPUFlags & eGTF_GPU_FormatMask);

	return avProps.vidProps.eGPUFlags == 0 ? 0 : IS_FLAG(avProps.vidProps.eGPUFlags, eGTF_GPU_SeparateFields) ? 2 : 1;
}

inline HRESULT MFFrame_DXWaitExecution(IUnknown* _pFrame, BOOL _bFlush, BOOL _bRemoteOnly)
{
	CComQIPtr<IMFFrameGPU> qpMFFrameGPU(_pFrame);
	if (!qpMFFrameGPU)
		return E_NOINTERFACE;

	if (!MFFrame_IsGPU(_pFrame) || (_bRemoteOnly && MFFrame_IsLocal(_pFrame)))
	{
		return S_FALSE;
	}

	return S_FALSE;// qpMFFrameGPU->MFVideoWaitExecution(_bFlush ? CComBSTR(L"flush=true") : NULL);
}

// Get CRC32 from MFFrame(for GPU frames convert to CPU), optionally convert to desired FCC  
// _nFCC_ConversionType = 1 - FCC conversion performed via CPU
// _nFCC_ConversionType = -1 - FCC conversion performed via GPU 
// _nFCC_ConversionType = 0 - default FCC conversion performed (for GPU frames GPU conversion, for CPU frames CPU conversion) 
// Note: The CPU/GPU FCC conversion may have diffrent results
inline LONGLONG MFFrame_CRC32(IUnknown* _pFrame, eMFCC _eFCCConvert = eMFCC_Default, int _nFCC_ConversionType = 0)
{
	CComQIPtr<IMFFrameGPU> qpMFFrame = _pFrame;
	if (!qpMFFrame) return 0;

	// Perform GPU conversion (optionally with FCC conversion)
	if (_nFCC_ConversionType < 0 && _eFCCConvert != eMFCC_Default)
		MCreator::MFFrameCloneI(true, qpMFFrame, eMFC_ForceGPU, _eFCCConvert);
	
	// Perform GPU -> CPU conversion
	MCreator::MFFrameCloneI(true, qpMFFrame, eMFC_ForceCPU, _nFCC_ConversionType != 1 ? _eFCCConvert : eMFCC_Default);

	// Perform CPU FCC conversion (if required)
	if( _nFCC_ConversionType > 0 && _eFCCConvert != eMFCC_Default)
		MCreator::MFFrameCloneI(true, qpMFFrame, eMFC_ForceCPU, _eFCCConvert);

	return MFSideData_DataGet(qpMFFrame, L"__crc32_");
}



//////////////////////////////////////////////////////////////////////////
// Shared data helpers
// - used for blind connect of separate object via IMFSideData or MFPipe e.g. WebRTC,
//   the required object selected via minimum elapsed time from creation time
//


// Set shared data with creation time and unique ID
// Note: _pSharedObj have to support IMFSideData interface (used for blind connect of separate object via IMFSideData or MFPipe e.g. WebRTC)
inline HRESULT MFSideData_SharedSet(IMFSideData* _pMFShared, LPCWSTR _pszPrefix, IN IUnknown* _pSharedObj, BSTR* _pbsUnuqieID = NULL)
{
	if (!_pMFShared)
		return E_INVALIDARG;

	if (!_pSharedObj)
		return E_INVALIDARG;

	LONGLONG llCreateTime = GetUTCTime();
	WCHAR* szUniqSharedName = ALLOCA_ARRAY(WCHAR, STR_LEN(_pszPrefix) + 128);
	wsprintf(szUniqSharedName, L"%ws_%04X_%04X_%I64X",
		STR_IS_EMPTY(_pszPrefix) ? L"SharedData" : _pszPrefix,
		GetCurrentProcessId(), GetCurrentThreadId(), llCreateTime);

	HRESULT hr = MFSideData_DataSet(_pSharedObj, L"creation_time", llCreateTime);
	ATLASSERT(SUCCEEDED(hr));
	if (FAILED(hr))
		return hr;

	hr = MFSideData_ObjSet(_pMFShared, szUniqSharedName, _pSharedObj);
	ATLASSERT(SUCCEEDED(hr));
	if (FAILED(hr))
		return hr;

	if (_pbsUnuqieID)
		*_pbsUnuqieID = ::SysAllocString(szUniqSharedName);

	return S_OK;
}

// Add shared data with creation time and unique ID
// Note: _pSharedObj have to support IMFSideData interface (used for blind connect of separate object via IMFSideData or MFPipe - e.g. WebRTC)
inline HRESULT MFSideData_SharedSet(LPCWSTR _pszSharedName, LPCWSTR _pszPrefix, IN IUnknown* _pSharedObj, BSTR* _pbsUnuqieID = NULL)
{
	if (!_pSharedObj)
		return E_POINTER;

	CComPtr<IMFSideData> cpMFSharedStore;
	if (!STR_IS_EMPTY(_pszSharedName))
	{
		HRESULT hr = MFSideData_ObjGet(NULL, _pszSharedName, &cpMFSharedStore, false);
		if (FAILED(hr))
			return hr;
	}

	return MFSideData_SharedSet(cpMFSharedStore, _pszPrefix, _pSharedObj, _pbsUnuqieID);
}

// Add shared data with creation time and unique ID
inline HRESULT MFSideData_SharedAdd(IMFSideData* _pMFShared, LPCWSTR _pszPrefix, IMFSideData** _ppSideData, BSTR* _pbsUnuqieID = NULL )
{
	if (!_pMFShared)
		return E_INVALIDARG;

	if (!_ppSideData)
		return E_POINTER;

	LONGLONG llCreateTime = GetUTCTime();
	WCHAR* szUniqSharedName = ALLOCA_ARRAY(WCHAR, STR_LEN(_pszPrefix) + 128);
	wsprintf(szUniqSharedName, L"%ws_%04X_%04X_%I64X",
		STR_IS_EMPTY(_pszPrefix) ? L"SharedData" : _pszPrefix,
		GetCurrentProcessId(), GetCurrentThreadId(), llCreateTime);

	MFSideData_DataSet(_pMFShared, STR_APPEND3(szUniqSharedName, L"::", L"creation_time"), llCreateTime);

	HRESULT hr = MFSideData_ObjGet(_pMFShared, szUniqSharedName, _ppSideData, false);
	if (FAILED(hr))
		return hr;

	if (_pbsUnuqieID)
		*_pbsUnuqieID = ::SysAllocString(szUniqSharedName);

	return S_OK;
}

// Add shared data with creation time and unique ID
inline HRESULT MFSideData_SharedAdd(LPCWSTR _pszSharedName, LPCWSTR _pszPrefix, IMFSideData** _ppSideData, BSTR* _pbsUnuqieID = NULL)
{
	if (!_ppSideData)
		return E_POINTER;

	CComPtr<IMFSideData> cpMFSharedStore;
	if (!STR_IS_EMPTY(_pszSharedName))
	{
		HRESULT hr = MFSideData_ObjGet(NULL, _pszSharedName, &cpMFSharedStore, false);
		if (FAILED(hr))
			return hr;
	}

	return MFSideData_SharedAdd(cpMFSharedStore, _pszPrefix, _ppSideData, _pbsUnuqieID );
}

inline HRESULT MFSideData_SharedRemove(LPCWSTR _pszSharedName, LPCWSTR _pszUniqueID)
{
	CComPtr<IMFSideData> cpMFSharedStore;
	if (!STR_IS_EMPTY(_pszSharedName))
	{
		HRESULT hr = MFSideData_ObjGet(NULL, _pszSharedName, &cpMFSharedStore, false);
		if (FAILED(hr))
			return hr;
	}

	return MFSideData_ObjSet(cpMFSharedStore, _pszUniqueID, NULL);
}

// Get shared data with minimum elapsed time
template<class TInterface>
inline HRESULT MFSideData_SharedGet(IUnknown* _pMFSidaDataShared, LPCWSTR _pszPrefix, TInterface** _ppSideData, LONGLONG* _pllElapsed = NULL, BSTR* _pbsUniqueID = NULL)
{
	LONGLONG llNow = GetUTCTime();
	LONGLONG llMinTime = MAXLONGLONG;

	CComBSTR cbsUniqueID;
	int nIndex = 0;
	CComPtr<IMFSideData> cpMinData;
	while (true)
	{
		CComBSTR cbsDataId;
		CComPtr<IMFSideData> cpSharedData;
		MFSideData_ObjGetByIndex(_pMFSidaDataShared, nIndex, &cbsDataId, &cpSharedData, false);

		if (!cpSharedData)
		{
			break;
		}

		if (STR_IS_EMPTY(_pszPrefix) || IsPrefix(cbsDataId, _pszPrefix))
		{
			LONGLONG llElapsed = llNow - MFSideData_DataGet(cpSharedData, L"creation_time");
			if (llElapsed < llMinTime)
			{
				cbsUniqueID = cbsDataId;
				cpMinData = cpSharedData;
				llMinTime = llElapsed;
			}
		}

		nIndex++;
	}

	if (!cpMinData)
		return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);

	if (_pllElapsed)
		*_pllElapsed = llMinTime;

	if (_pbsUniqueID)
		*_pbsUniqueID = cbsUniqueID.Detach();

	return cpMinData.QueryInterface(_ppSideData);
}

// Get first object w/o _pszUsedFlag and set _pszUsedFlag for this object data
// Note: Object have to support IMFSideData interface
template<class TInterface>
inline HRESULT MFSideData_ObjGet_FirstNotUsed(IUnknown* _pMFSidaData, LPCWSTR _pszPrefix, TInterface** _ppSideData, LPCWSTR _pszUsedFlag, BSTR* _pbsUniqueID = NULL)
{
	int nIndex = 0;
	CComPtr<IMFSideData> cpMinData;
	while (true)
	{
		CComBSTR cbsDataId;
		CComPtr<IMFSideData> cpSharedData;
		MFSideData_ObjGetByIndex(_pMFSidaData, nIndex, &cbsDataId, &cpSharedData, false);
		if (!cpSharedData)
			break;
		
		if (STR_IS_EMPTY(_pszPrefix) || IsPrefix(cbsDataId, _pszPrefix))
		{
			if (MFSideData_DataSet(cpSharedData, _pszUsedFlag, -1) == S_OK)
			{
				if(_ppSideData)
					cpSharedData.QueryInterface(_ppSideData);

				if (_pbsUniqueID)
					*_pbsUniqueID = cbsDataId.Detach();
				
				return S_OK;
			}
		}

		++nIndex;
	}

	return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
}

inline HRESULT MFSideData_SharedGet(LPCWSTR _pszSharedName, LPCWSTR _pszPrefix, IMFSideData** _ppSideData, LONGLONG* _pllElapsed = NULL)
{
	CComPtr<IMFSideData> cpMFSharedStore;
	HRESULT hr = MFSideData_ObjGet(NULL, _pszSharedName, &cpMFSharedStore, false);
	if (FAILED(hr))
		return hr;

	return MFSideData_SharedGet(cpMFSharedStore, _pszPrefix, _ppSideData, _pllElapsed);
}



// For compatibility with old code
//#define MFFrame_CopyData MFSideData_Copy
#define MFFrame_StrGet MFSideData_StrGet
#define MFFrame_StrSet MFSideData_StrSet
#define MFFrame_DataGet MFSideData_DataGet
#define MFFrame_DataSet MFSideData_DataSet
#define MFFrame_ObjGet MFSideData_ObjGet
#define MFFrame_ObjGetByIndex MFSideData_ObjGetByIndex
#define MFFrame_ObjSet MFSideData_ObjSet



inline HRESULT MFProps_Set( IUnknown* _pProps, LPCWSTR _pszName, LPCWSTR _pszValue )
{
	CComQIPtr<IMFProps> qpMFProps( _pProps ? _pProps : MCreator::GetMFFactory() );
	if( qpMFProps )
		return qpMFProps->PropsSet( CComBSTR(_pszName), CComBSTR(_pszValue) );

	CComQIPtr<IMProps> qpMProps( _pProps );
	if( qpMProps )
		return qpMProps->PropsSet( CComBSTR(_pszName), CComBSTR(_pszValue) );

	return _pProps ? E_NOINTERFACE : E_POINTER;
}

inline HRESULT MFProps_Get( IUnknown* _pProps, LPCWSTR _pszName, BSTR* _pbsValue )
{
	CComQIPtr<IMFProps> qpMFProps( _pProps ? _pProps : MCreator::GetMFFactory() );
	if( qpMFProps )
		return qpMFProps->PropsGet( CComBSTR(_pszName), _pbsValue );

	CComQIPtr<IMProps> qpMProps( _pProps );
	if( qpMProps )
		return qpMProps->PropsGet( CComBSTR(_pszName), _pbsValue );
			
	return _pProps ? E_NOINTERFACE : E_POINTER;
}

inline CComBSTR MFProps_Get( IUnknown* _pProps, LPCWSTR _pszName )
{
	CComBSTR cbsRes;
	MFProps_Get( _pProps, _pszName, &cbsRes );
	return cbsRes;
}

inline double MFProps_GetDouble( IUnknown* _pProps, LPCWSTR _pszName, double _dblDef = 0.0 )
{
	CComBSTR cbsRes;
	MFProps_Get( _pProps, _pszName, &cbsRes );
	if( !STR_IS_EMPTY( cbsRes ) )
	{
		_dblDef = _wtof( cbsRes.m_str );
	}
	return _dblDef;
}

inline int MFProps_GetInt( IUnknown* _pProps, LPCWSTR _pszName, int _nDef = 0.0 )
{
	CComBSTR cbsRes;
	MFProps_Get( _pProps, _pszName, &cbsRes );
	if( !STR_IS_EMPTY( cbsRes ) )
	{
		_nDef = _wtol( cbsRes.m_str );
	}
	return _nDef;
}

// Return int -> for have option to check the present of parameter, 
// for this case
// 0 - false
// 1 - true
// empty, or invalid value -> _nWrong (default is zero)
inline int MFProps_GetBool(IUnknown* _pProps, LPCWSTR _pszName, int _nWrong = 0)
{
	CComBSTR cbsRes;
	MFProps_Get(_pProps, _pszName, &cbsRes);

	return Str2BoolInt(cbsRes.m_str, _nWrong);
}

inline int MFProps_OptionsEnum(IUnknown* _pProps, LPCWSTR _pszName, TDeqStrPairs* _pDeqOptions)
{
	CComQIPtr<IMFProps> qpMFProps(_pProps);
	ATLASSERT(!_pProps || qpMFProps);
	if (!qpMFProps)
		return -1;

	int nCount = 0;
	qpMFProps->PropsOptionGetCount(CComBSTR(_pszName), &nCount);

	if (_pDeqOptions)
	{
		for (int i = 0; i < nCount; i++)
		{
			CComBSTR cbsName;
			CComBSTR cbsDescrition;
			qpMFProps->PropsOptionGetByIndex(CComBSTR(_pszName), i, &cbsName, &cbsDescrition);

			if (_pDeqOptions)
				_pDeqOptions->push_back(make_pair(cbsName,cbsDescrition));
		}
	}

	return nCount;
}

inline int MFProps_OptionsFind(IUnknown* _pProps, LPCWSTR _pszName, LPCWSTR _pszValue, bool _bExact, BSTR* _pbsExactName = NULL)
{
	CComQIPtr<IMFProps> qpMFProps(_pProps);
	ATLASSERT(!_pProps || qpMFProps);
	if (!qpMFProps)
		return -1;

	int nCount = 0;
	qpMFProps->PropsOptionGetCount(CComBSTR(_pszName), &nCount);

	
	for (int i = 0; i < nCount; i++)
	{
		CComBSTR cbsName;
		CComBSTR cbsDescrition;
		qpMFProps->PropsOptionGetByIndex(CComBSTR(_pszName), i, &cbsName, &cbsDescrition);

		if ( (_bExact && STR_IS_SAME_I(cbsName, _pszValue)) ||
			 (!_bExact && StrStrIW(cbsName, _pszValue) != NULL ))
		{
			if (_pbsExactName)
				*_pbsExactName = cbsName.Detach();

			return i;
		}

	}

	return -1;
}

// Method for easy check redirection (e.g for preview redirection)
// Note: MFProps_CheckRedirector( cbsPropsName, L"audio", &cbsPropsName.m_str ) is VALID
inline bool MFProps_CheckRedirector(LPCWSTR _pszPropsName, LPCWSTR _pszPrefix, BSTR* _pbsCorrectedPropsName )
{
	LPCWSTR pszName = NULL;
	if (!STR_IS_EMPTY(_pszPrefix) && (STR_IS_SAME_I(_pszPropsName, _pszPrefix) || IsPrefix(_pszPropsName, STR_APPEND(_pszPrefix, L"::"), &pszName)))
	{
		if (_pbsCorrectedPropsName)
		{
			CComBSTR cbsName = pszName;

			if (*_pbsCorrectedPropsName)
				::SysFreeString(*_pbsCorrectedPropsName);

			*_pbsCorrectedPropsName = cbsName.Detach();
		}
		
		return true;
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
// Helper method for gpu_pipeline

inline int GPU_PipelineMode()
{
	bool bGPU_Supported = MFProps_GetBool(NULL, L"gpu_pipeline");
	if (bGPU_Supported)
	{
		int nYUV_Processing = MFProps_GetInt(NULL, L"dx11.yuv_processing");
		return nYUV_Processing > 0 ? 2 : 1;
	}
	return 0;
}

inline eMFBufferFlags MFBuffer_SharedInfo( IUnknown* _pSrcBuffer, LONGLONG* _pllSharedID = NULL, DWORD* _pdwProcessID = NULL, IUnknown** _ppParent = NULL)
{
	CComQIPtr<IMFBuffer> qpMFBuffer( _pSrcBuffer );
	if( !qpMFBuffer )
		return eMFBF_Empty;

	eMFBufferFlags eFlags = eMFBF_Empty;

	DWORD dwProcessID = 0;
	LONGLONG llSharedID = 0;
	qpMFBuffer->BufferSharedIdGet( &llSharedID, &dwProcessID, &eFlags );

	if( _pllSharedID )
		*_pllSharedID = llSharedID;

	if( _pdwProcessID )
		*_pdwProcessID = dwProcessID;

	if( _ppParent )
	{
		MFFrame_ObjGet( _pSrcBuffer, MFSD_PARENT_OBJ_GET, _ppParent, false );
	}

	return eFlags;
}

inline LONGLONG MFBuffer_Size(IUnknown* _pBuffer, LONG* _plMaxSize = NULL, LONGLONG* _plpBufferNoLock = NULL)
{
	CComQIPtr<IMFBuffer> qpMFBuffer(_pBuffer);
	if (!qpMFBuffer)
		return 0;

	LONG cbData = 0;
	LONG cbActual = 0;
	LONGLONG lpData = NULL;
	qpMFBuffer->BufferLock(eMFLT_NoLock, &cbData, &cbActual, &lpData);
	return cbActual;
}

inline HRESULT MFBuffer_CopyFrom( IUnknown* _pSrcBuffer, void* _pvDest, long _cbSize, bool _bLock = false )
{
	CComQIPtr<IMFBuffer> qpMFBuffer( _pSrcBuffer );
	if( !qpMFBuffer )
		return E_NOINTERFACE;

	LONG cbData = 0;
	LONG cbActual = 0;
	LONGLONG lpData = NULL;
	qpMFBuffer->BufferLock( _bLock ? eMFLT_Read : eMFLT_NoLock, &cbData, &cbActual, &lpData );
	ATLASSERT( lpData );
	if( !lpData ) return E_UNEXPECTED;

	::SIMDCopyMem( _pvDest, (LPBYTE)lpData, MIN(_cbSize, cbData ) );

	if( _bLock )
		qpMFBuffer->BufferUnlock( eMFLT_Read );
	return S_OK;
}

inline HRESULT MFBuffer_CopyTo( IUnknown* _pDstBuffer, void* _pvSrc, long _cbSize, bool _bLock = false )
{
	CComQIPtr<IMFBuffer> qpMFBuffer( _pDstBuffer );
	if( !qpMFBuffer )
		return E_NOINTERFACE;

	LONG cbData = 0;
	LONG cbActual = 0;
	LONGLONG lpData = NULL;
	qpMFBuffer->BufferLock( _bLock ? eMFLT_Write : eMFLT_NoLock, &cbData, &cbActual, &lpData );
	ATLASSERT( lpData );
	if( !lpData ) return E_UNEXPECTED;

	::SIMDCopyMem( (LPBYTE)lpData, _pvSrc, MIN(_cbSize, cbData ) );
	if( _bLock )
		qpMFBuffer->BufferUnlock( eMFLT_Write );
	return S_OK;
}

inline BYTEBLOB MFBuffer_Blob( IUnknown* _pDstBuffer, int _nOffset = 0, bool _bActualSize = false )
{
	BYTEBLOB blobRes = {};
	CComQIPtr<IMFBuffer> qpMFBuffer( _pDstBuffer );
	if( qpMFBuffer )
	{
		LONG cbData = 0;
		LONG cbActual = 0;
		LONGLONG lpData = NULL;
		qpMFBuffer->BufferLock( eMFLT_NoLock, &cbData, &cbActual, &lpData );
		ATLASSERT( lpData );
		if( !lpData ) return blobRes;

		blobRes.cbData = _bActualSize ? cbActual : cbData;

		if( _nOffset > 0 && _nOffset <= (LONG)blobRes.cbData )
		{
			blobRes.cbData -= _nOffset;
			blobRes.pbData = (LPBYTE)lpData + _nOffset;
		}
		else
		{
			blobRes.cbData = 0;
			blobRes.pbData = NULL;
		}
	}

	return blobRes;
}

inline HRESULT MFBuffer_SaveToStream( IUnknown* _pBuffer, IStream* _pStream, bool _bSaveSideData, LONG* _pcbSize )
{
	if( !_pStream )
		return E_INVALIDARG;

	CComQIPtr<IMFBuffer> qpMFBuffer( _pBuffer );
	if( !qpMFBuffer )
		return E_NOINTERFACE;

	LONG cbActual = 0;
	LONGLONG lpData = NULL;
	HRESULT hr = qpMFBuffer->BufferLock( eMFLT_NoLock, NULL, &cbActual, &lpData );
	if( FAILED( hr ) )
		return hr;

	CComQIPtr<IMFSideData> qpSideData( _bSaveSideData ? qpMFBuffer.p : NULL );

	if( _pStream )
	{
		ULONG cbWritten = 0;

		// Mark
		DWORD dwMark = qpSideData ? FCC('BFSD') : FCC('BFRW');
		hr = _pStream->Write( &dwMark, sizeof(dwMark), &cbWritten );
		ATLASSERT( SUCCEEDED( hr ) );
		if( FAILED( hr ) )
			return hr;


		// Buffer type
		eMFBufferFlags eBufferType = eMFBF_Empty;
		qpMFBuffer->BufferFlagsGet( &eBufferType );
		hr = _pStream->Write( &eBufferType, sizeof(eBufferType), &cbWritten );
		ATLASSERT( SUCCEEDED( hr ) );
		if( FAILED( hr ) )
			return hr;

		// Len
		hr = _pStream->Write( &cbActual, sizeof(cbActual), &cbWritten );
		ATLASSERT( SUCCEEDED( hr ) );
		if( FAILED( hr ) )
			return hr;

		// Data
		hr = _pStream->Write( (LPBYTE)lpData, cbActual, &cbWritten );
		ATLASSERT( SUCCEEDED( hr ) );
		if( FAILED( hr ) )
			return hr;

		// Side data
		if( qpSideData )
			MFSideData_ObjSet( qpSideData, MF_SIDEDATA_SAVE, _pStream );
	}

	if( _pcbSize )
	{
		*_pcbSize = sizeof(DWORD) + sizeof(eMFBufferFlags) + sizeof(LONG) + cbActual;
		if( qpSideData )
			*_pcbSize += MFSideData_DataGet( qpSideData, MF_SIDEDATA_SIZE );
	}


	return S_OK;
}

// _nMakeIPC =  0 -> Same type IPC/InProc
// _nMakeIPC =  1 -> Force IPC
// _nMakeIPC = -1 -> Force InProc
template <class TInterface>
inline HRESULT MFBuffer_Clone( IUnknown *_pBuffer, TInterface** _ppBuffer, int _nMakeIPC = 0 )
{
	DWORD dwProcessID = 0;
	CComQIPtr<IMFBuffer> qpMFBuffer;
	eMFBufferFlags eFlags = MFBuffer_SharedInfo( _pBuffer, NULL, &dwProcessID );
	if( dwProcessID && dwProcessID != GetCurrentProcessId() )
	{
		MCreator::MFBufferTransferIPC( _pBuffer, &qpMFBuffer, eMFTF_Default );
	}
	else
	{
		qpMFBuffer = _pBuffer;
	}

	if( !qpMFBuffer )
		return E_NOINTERFACE;

	LONG cbActual = 0;
	LONGLONG lpData = 0;
	HRESULT hr = qpMFBuffer->BufferLock(eMFLT_NoLock, NULL, &cbActual, &lpData );
	ATLASSERT( SUCCEEDED( hr ) );
	if( FAILED( hr ) )
		return hr;

	if( _nMakeIPC != 0 )
		SET_FLAG( &eFlags, eMFBF_LocalOnly, _nMakeIPC < 0 );

	return MCreator::MFBufferAlloc( cbActual, lpData, _ppBuffer, eFlags );
}

template <class TInterface>
inline HRESULT MFBuffer_CreateFromStream( IUnknown* _pStream, TInterface** _ppMFBuffer )
{
	if( _ppMFBuffer )
		return E_POINTER;

	// Check for stream
	CComQIPtr<IStream> qpStream( _pStream );
	if( !qpStream )
		return E_NOINTERFACE;

	// Mark
	DWORD dwMark = 0;;
	HRESULT hr = qpStream->Read( &dwMark, sizeof(dwMark), NULL );
	ATLASSERT( SUCCEEDED( hr ) );
	if( FAILED( hr ) )
		return hr;

	if( dwMark != FCC('BFSD') && dwMark != FCC('BFRW') )
		return E_INVALIDARG;

	// Stream type
	eMFBufferFlags eBufferType = eMFBF_Empty;
	hr = qpStream->Read( &eBufferType, sizeof(eBufferType), NULL );
	ATLASSERT( SUCCEEDED( hr ) );
	if( FAILED( hr ) )
		return hr;

	// Size 
	LONG cbBuffer = 0;
	hr = qpStream->Read( &cbBuffer, sizeof(cbBuffer), NULL );
	ATLASSERT( SUCCEEDED( hr ) );
	if( FAILED( hr ) )
		return hr;

	// TODO: Check packets allocations

	// Alloc buffer
	CComPtr<IMFBuffer> cpMFBuffer;
	hr = MCreator::MFBufferAlloc( cbBuffer, NULL, &cpMFBuffer, eBufferType );
	ATLASSERT( SUCCEEDED( hr ) );
	if( FAILED( hr ) )
		return hr;

	// Read buffer data
	LONGLONG lpBuffer = NULL;
	cpMFBuffer->BufferLock(eMFLT_NoLock, NULL, NULL, &lpBuffer );
	ATLASSERT( lpBuffer );

	hr = qpStream->Read( (LPBYTE)lpBuffer, cbBuffer, NULL );
	ATLASSERT( SUCCEEDED( hr ) );
	if( FAILED( hr ) )
		return hr;

	if( dwMark == FCC('BFSD') )
	{
		// Read sidedata
		hr = MFSideData_ObjSet( cpMFBuffer, MF_SIDEDATA_LOAD, _pStream );
		ATLASSERT( SUCCEEDED( hr ) );
		if( FAILED( hr ) )
			return hr;
	}

	return cpMFBuffer.QueryInterface( _ppMFBuffer );
}

inline HRESULT MFPipe_Create(LPCWSTR _pszName, LPCWSTR _pszParam, IMFPipe** _ppMFPipe)
{
	if (!_ppMFPipe)
		return E_POINTER;

	CComPtr<IMFPipe> cpMFPipe;
	HRESULT hr = cpMFPipe.CoCreateInstance(__uuidof(MFPipe));
	ATLASSERT(SUCCEEDED(hr));
	if (FAILED(hr))
		return hr;

	hr = cpMFPipe->PipeCreate(CComBSTR(_pszName), CComBSTR(_pszParam));
	if (FAILED(hr))
	{
		MTRACE_ERR("cpMFPipe->PipeCreate(%ws,%ws) FAILED hr=%08X", _pszName, _pszParam, hr);
		return hr;
	}

	return cpMFPipe.QueryInterface(_ppMFPipe);
}

inline MF_PIPE_INFO MFPipe_Info(IUnknown* _pMFPipe, LPCWSTR _pszChannel,  BSTR* _pbsName = NULL )
{
	MF_PIPE_INFO mfInfo = {};
	CComQIPtr<IMFPipe> qpMFPipe(_pMFPipe);
	if (qpMFPipe)
	{
		CComBSTR cbsNameForIPC;
		qpMFPipe->PipeInfoGet(&cbsNameForIPC, CComBSTR(_pszChannel), &mfInfo);

		if (_pbsName)
			*_pbsName = cbsNameForIPC.Detach();
	}

	return mfInfo;
}

//////////////////////////////////////////////////////////////////////////
// Some helper function for IMSource_I / IMFSource_I calls
// 

template <class TFrame>
inline HRESULT MF_SourceFrameGetEx( IUnknown* _pSource, 
	LONGLONG* _plReaderID, 
	TFrame** _ppFrame, 
	REFERENCE_TIME _rtMaxWaitTime, 
	eFrameGetFlags _eGetFlags )
{
	CBasicPerfD perf(L"MF_SourceFrameGetEx");
	CComQIPtr<IMSource> qpMSource( _pSource );
	if( qpMSource )
	{
		CComPtr<IMFrame> cpMFrame;
		HRESULT hr = qpMSource->SourceFrameGetEx( _plReaderID, _rtMaxWaitTime, &cpMFrame, _eGetFlags );
		if( FAILED( hr ) )
			return hr;

		if( _eGetFlags == eFGT_Remove )
			return S_OK;

		ATLASSERT( cpMFrame );
		if( !cpMFrame ) return E_UNEXPECTED;

		// Check for out of process
		hr = MCreator::MakeLocalI( cpMFrame );
		if( FAILED( hr ) )
			return hr;

		return cpMFrame.QueryInterface( _ppFrame );
	}

	CComQIPtr<IMFSource_I> qpMFSource_I( _pSource );
	if( qpMFSource_I )
	{
		CComBSTR cbsProps;
		if( _eGetFlags == eFGT_Remove )
			cbsProps = L"remove_reader";
		else
		{
			if( _eGetFlags & eFGT_Sync )
				cbsProps = "sync_reader ";

			if( _eGetFlags & eFGT_FlushBuffers )
				cbsProps.Append("flush_reader");
		}

		CComPtr<IMFFrame> cpMFrame;
		HRESULT hr = qpMFSource_I->MFSourceFrameGetEx( _plReaderID, &cpMFrame, _rtMaxWaitTime, cbsProps );
		if( FAILED( hr ) )
			return hr;

		if( _eGetFlags == eFGT_Remove )
			return S_OK;

		ATLASSERT( cpMFrame );
		if( !cpMFrame ) return E_UNEXPECTED;

		// Check for out of process
		hr = MCreator::MakeLocalI( cpMFrame );
		if( FAILED( hr ) )
			return hr;

		return cpMFrame.QueryInterface( _ppFrame );
	}



	ATLASSERT( !_pSource && "MF_SourceXXX" );

	return E_NOINTERFACE;
}

inline HRESULT MF_SourceInit( IUnknown* _pSource, LONGLONG* _plUserData )
{
	CComQIPtr<IMFSource_I> qpMFSource_I( _pSource );
	if( qpMFSource_I )
		return qpMFSource_I->MFSourceInit( _plUserData );

	CComQIPtr<IMSource_I> qpMSource_I( _pSource );
	if( qpMSource_I )
		return qpMSource_I->SourceInit( _plUserData );

	ATLASSERT( !_pSource && "MF_SourceXXX" );

	return E_NOINTERFACE;
}

inline HRESULT MF_SourceClose(IUnknown* _pSource, LONGLONG _lUserData )
{
	CComQIPtr<IMFSource_I> qpMFSource_I( _pSource );
	if( qpMFSource_I )
		return qpMFSource_I->MFSourceClose( _lUserData );

	CComQIPtr<IMSource_I> qpMSource_I( _pSource );
	if( qpMSource_I )
		return qpMSource_I->SourceClose( _lUserData );

	ATLASSERT( !_pSource && "MF_SourceXXX" );

	return E_NOINTERFACE;
}

inline HRESULT MF_SourceRefAdd( IUnknown* _pSource, DWORD _dwProcessID )
{
	CComQIPtr<IMFSource_I> qpMFSource_I( _pSource );
	if( qpMFSource_I )
		return qpMFSource_I->MFSourceRefAdd( _dwProcessID );

	CComQIPtr<IMSource_I> qpMSource_I( _pSource );
	if( qpMSource_I )
		return qpMSource_I->SourceRefAdd( _dwProcessID );

	ATLASSERT( !_pSource && "MF_SourceXXX" );

	return E_NOINTERFACE;
}

inline HRESULT MF_SourceRefRemove( IUnknown* _pSource, DWORD _dwProcessID )
{
	CComQIPtr<IMFSource_I> qpMFSource_I( _pSource );
	if( qpMFSource_I )
		return qpMFSource_I->MFSourceRefRemove( _dwProcessID );

	CComQIPtr<IMSource_I> qpMSource_I( _pSource );
	if( qpMSource_I )
		return qpMSource_I->SourceRefRemove( _dwProcessID );

	ATLASSERT( !_pSource && "MF_SourceXXX" );

	return E_NOINTERFACE;
}

inline HRESULT MF_SourceHaveRefs( IUnknown* _pSource, LONGLONG* _phWaitHandle )
{
	CComQIPtr<IMFSource_I> qpMFSource_I( _pSource );
	if( qpMFSource_I )
		return qpMFSource_I->MFSourceHaveRefs( _phWaitHandle );

	CComQIPtr<IMSource_I> qpMSource_I( _pSource );
	if( qpMSource_I )
		return qpMSource_I->SourceHaveRefs( _phWaitHandle );

	ATLASSERT( !_pSource && "MF_SourceXXX" );

	return E_NOINTERFACE;
}

inline CComBSTR MF_SourceName(IUnknown* _pSource)
{
	if (!_pSource)
		return NULL;

	HRESULT hr = S_OK;
	CComBSTR cbsSourceName;
	CComQIPtr<IMSender_I> qpMSender_I(_pSource);
	if (qpMSender_I)
	{
		hr = qpMSender_I->SenderGetName(&cbsSourceName);
		ATLASSERT(SUCCEEDED(hr));
		if (!STR_IS_EMPTY(cbsSourceName))
			return cbsSourceName;
	}

	CComQIPtr<IMObject> qpMObject(_pSource);
	if (qpMObject)
	{
		hr = qpMObject->ObjectNameGet(&cbsSourceName);
		ATLASSERT(SUCCEEDED(hr));
		if (!STR_IS_EMPTY(cbsSourceName))
			return cbsSourceName;

	}

	return MFProps_Get(_pSource, L"object_name");
}

template<class TInterface>
inline HRESULT MF_SourceOpen(LPCWSTR _pszSourceName, TInterface** _ppSource, bool _bUpdateEnum = true )
{
	CComQIPtr<IMSenders> qpMSenders(MCreator::GetMFFactory());
	ATLASSERT(qpMSenders);
	if (!qpMSenders) return E_UNEXPECTED;

	if (_bUpdateEnum)
		qpMSenders->SendersGetCount(NULL);

	CComPtr<IUnknown> cpSender;
	HRESULT hr = qpMSenders->SendersGet(CComBSTR(_pszSourceName), &cpSender);
	if (FAILED(hr))
		return hr;

	ATLASSERT(cpSender);
	if (!cpSender)
		return E_UNEXPECTED;

	return cpSender.QueryInterface(_ppSource);
}


//////////////////////////////////////////////////////////////////////////
// MFPackets

inline M_PACKET_INFO MFPacket_Info(IUnknown* _pPacket, M_TIME* _pTime = NULL, M_STREAM_INFO* _pStreamInfo = NULL)
{
	M_PACKET_INFO pktInfo = {};
	CComQIPtr<IMFPacket> qpMFPacket(_pPacket);
	if (qpMFPacket)
	{
		qpMFPacket->PacketInfoGet(&pktInfo);

		if (_pTime)
			qpMFPacket->PacketTimeGet(_pTime);
		if (_pStreamInfo)
			qpMFPacket->PacketStreamInfoGet(_pStreamInfo);
	}

	return pktInfo;
}

inline M_TIME MFPacket_Time(IUnknown* _pPacket)
{
	M_TIME mTime = {};
	CComQIPtr<IMFPacket> qpMFPacket(_pPacket);
	if (qpMFPacket)
		qpMFPacket->PacketTimeGet(&mTime);

	return mTime;
}

inline HRESULT MFPacket_Create( IMFPacket** _ppPacket, LPBYTE _pbBuffer, int _cbBuffer, LPCWSTR _pszTag, LONGLONG _llPts = -1 )
{
	CComBSTR cbsTag = _pszTag;
	M_STREAM_INFO stmInfo = {};
	stmInfo.eMediaType = eMFMT_Multiplexed;
	stmInfo.bsCodecNameOrTag = cbsTag;

	M_PACKET_INFO pktInfo = {};
	pktInfo.lpPacketData = (LONGLONG)_pbBuffer;
	pktInfo.cbPacketData = _cbBuffer;
	pktInfo.llDts = _llPts;

	CComPtr<IMFPacket> cpMFPacket;
	HRESULT hr = MCreator::MFBufferAllocPacket(&stmInfo, &pktInfo, &cpMFPacket);
	if (FAILED(hr))
		return hr;
	
	return hr;
}

inline HRESULT MFPacket_SaveToFile(IUnknown* _pPacket, LPCWSTR _pszFileName, bool _bAppendFile = false, int _nIndex = -1, int _nMaxAuto = 100 )
{
	M_PACKET_INFO pktInfo = MFPacket_Info(_pPacket);
	if (!pktInfo.cbPacketData || !pktInfo.lpPacketData) return HRESULT_FROM_WIN32(ERROR_EMPTY);

	CComBSTR cbsPath = _pszFileName;

	if (_nIndex == -2)
	{
		static int nTestIndex = 0;
		_nIndex = nTestIndex++;
		nTestIndex %= MAX(_nMaxAuto, 1);
	}

	if (_nIndex >= 0)
	{
		WCHAR szIndex[16] = {};
		wsprintfW(szIndex, L".%03d", _nIndex);
		InsertBeforeExtension(cbsPath, szIndex);
	}

	MTRACE("(%ws)", cbsPath.m_str);

	CBlobFake blobSave;
	blobSave.Attach((LPBYTE)pktInfo.lpPacketData, pktInfo.cbPacketData);

	HRESULT hr = blobSave.SaveToFile(cbsPath, false, _bAppendFile);
	if (FAILED(hr))
	{
		MTRACE_ERR("blobSave.SaveToFile() FAILED hr=%08X", hr);
	}

	return hr;
}

//////////////////////////////////////////////////////////////////////////
// MFDevice

// Helper functions
inline int MFDevice_Find(IUnknown* _pDevice, eMFDeviceType _eDevType, LPCWSTR _pszName, bool _bExact = false)
{
	int nCount = 0;
	CComQIPtr<IMFDevice> qpMFDevice(_pDevice);
	if (!qpMFDevice)
		return E_NOINTERFACE;

	HRESULT hr = qpMFDevice->DeviceGetCount(_eDevType, &nCount);
	if (FAILED(hr))
	{
		MTRACE_ERR("qpMFDevice->DeviceGetCount(Type:%d) FAILED hr=%08X", (int)_eDevType, hr);
		return hr;
	}

	for (int i = 0; i < nCount; i++)
	{
		BOOL bBusy = FALSE;
		CComBSTR cbsDevName;
		hr = qpMFDevice->DeviceGetByIndex(_eDevType, i, &cbsDevName, &bBusy);
		if (FAILED(hr))
		{
			MTRACE_ERR("qpMFDevice->DeviceGetByIndex(%d/%d Type:%d) FAILED hr=%08X", i, nCount, (int)_eDevType, hr);
		}

		if (_bExact && STR_IS_SAME_I(cbsDevName, _pszName))
		{
			MTRACE("Device '%ws' Type:%d EXACT FOUND Idx:%d", _pszName, _eDevType, i);
			return i;
		}

		if (!_bExact && ::StrStrIW(cbsDevName, _pszName) != NULL)
		{
			MTRACE("Device '%ws' Type:%d SIMILAR FOUND Idx:%d Name:%ws", _pszName, _eDevType, i, cbsDevName.m_str);
			return i;
		}
	}

	MTRACE_WARN("Device '%ws' Type:%d NPT FOUND", _pszName, _eDevType );

	return -1;
}

inline HRESULT MFDevice_SetByName(IUnknown* _pDevice, eMFDeviceType _eDevType, LPCWSTR _pszName, LPCWSTR _pszParams, bool _bExact = false)
{
	int nDevIdx = MFDevice_Find(_pDevice, _eDevType, _pszName, _bExact);
	if (nDevIdx < 0)
	{
		return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
	}

	CComQIPtr<IMFDevice> qpMFDevice(_pDevice);
	ATLASSERT(qpMFDevice);

	HRESULT hr = qpMFDevice->DeviceSet(_eDevType, nDevIdx, CComBSTR(_pszParams));
	if (FAILED(hr))
	{
		MTRACE_ERR("qpMFDevice->DeviceSet(%ws) FAILED hr=%08X", _pszParams, hr);
		return hr;
	}
	
	return hr;
}

// For XML methods (used in WebRTC async/sync callbacks)
inline  CComBSTR Attributes_StringGet(IUnknown* _pAttributes, LPCWSTR _pszName, LPCWSTR _pszDef = NULL)
{
	HRESULT hr = E_NOINTERFACE;
	CComBSTR cbsReturn;
	CComQIPtr<IMAttributes> qpMAttributes(_pAttributes);
	if (qpMAttributes)
		hr = qpMAttributes->AttributesStringGet(CComBSTR(_pszName), &cbsReturn);
	
	return SUCCEEDED(hr) ? cbsReturn : CComBSTR(_pszDef);
}


// GPU Helpers

class GPUVideoLock
{
	CComQIPtr<IMFFrameGPU> qpMFrameGPU;

public:
	M_VID_PROPS vidProps;
	LPBYTE		pbVideo;
	MF_VID_PTR	vidPtr;

public:
	GPUVideoLock()
		: pbVideo(NULL)
	{
		ZERO_OBJECT(vidProps);
		ZERO_OBJECT(vidPtr);
	}

	GPUVideoLock(IUnknown* _pFrame, eMFLockType _eLockType)
		: pbVideo(NULL)
	{
		ZERO_OBJECT(vidProps);
		ZERO_OBJECT(vidPtr);

		Lock(_pFrame, _eLockType);
	}

	~GPUVideoLock()
	{
		Unlock();
	}

	bool Lock(IUnknown* _pFrame, eMFLockType _eLockType, eMFCC _ePixelFormat = eMFCC_Default)
	{
		Unlock();

		qpMFrameGPU = _pFrame;
		if (!qpMFrameGPU)
			return false;

		HRESULT hr = qpMFrameGPU->MFVideoLock(_eLockType, _ePixelFormat, &vidProps, &vidPtr, NULL);
		if (FAILED(hr))
		{
			qpMFrameGPU.Release();
			return false;
		}

		pbVideo = (LPBYTE)Img_VidPlane(&vidPtr, 0);
		return true;
	}

	operator bool() { return qpMFrameGPU != NULL; }

	bool Unlock()
	{
		ZERO_OBJECT(vidPtr);
		ZERO_OBJECT(vidProps);
		pbVideo = NULL;

		if (qpMFrameGPU)
		{
			qpMFrameGPU->MFVideoUnlock();
			qpMFrameGPU.Release();
			return true;
		}

		return false;
	}
};


#endif // _M_CALLS_H_
