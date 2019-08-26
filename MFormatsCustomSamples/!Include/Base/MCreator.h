//---------------------------------------------------------------------------
// MCreator.h : 
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

#pragma once

#ifndef __MFormats_h__
#include "MFormats.h"
#endif

#ifdef MFRAMES_STATIC_FACTORY
#include <Classes/MFramesImpl.h>
#endif

#include "MFunctions.h"

class MCreator
{
	// For frames store
	static CComAutoCriticalSection	s_csFactory;

	static IMFFactory*				s_pMFFactory;
	static IMFrames_I*				s_pFramesStore;

public:

	static inline IMFrames_I* GetFramesStore() 
	{
		if( s_pFramesStore )
			return s_pFramesStore;

		CAutoCS crs( s_csFactory );

		if( !s_pFramesStore )
		{
			IMFFactory* pFactory = GetMFFactory();
			if( pFactory )
			{
				pFactory->QueryInterface( __uuidof(IMFrames_I), (void**)&s_pFramesStore );
				ATLASSERT( s_pFramesStore );
			}
		}

		return s_pFramesStore;
	}

	static inline IMFFactory* GetMFFactory() 
	{
		if( s_pMFFactory )
			return s_pMFFactory;

		CAutoCS crs( s_csFactory );

		if( !s_pMFFactory )
		{
			HRESULT hr = S_OK;
#ifdef MFRAMES_STATIC_FACTORY
			hr = CCoMFFactory_I::CreateInternal( &s_pMFFactory );
			ATLASSERT( SUCCEEDED( hr ) && "CreateInternal(MFFactory)" );
#else
			CActiveContext actContext;
			actContext.Init();

			hr = ::CoCreateInstance( __uuidof(MFFactory), NULL, CLSCTX_INPROC, __uuidof(IMFFactory), (void**)&s_pMFFactory );
			if( FAILED( hr) )
				hr = CreateObjectDirect(L"Medialooks.Runtime.dll", __uuidof(MFFactory), &s_pMFFactory);
			ATLASSERT( SUCCEEDED( hr ) && "CoCreateInstance(MFFactory)");
#endif
		}
		
		return s_pMFFactory;
	}

	//////////////////////////////////////////////////////////////////////////
	// Analytics
	static HRESULT Analytics_OnEventSync(LPCWSTR _pszAction, LPCWSTR _pszActionPostfix = NULL, LPCWSTR _pszCategory = L"Feature", LPCWSTR _pszLabel = NULL, LPCWSTR _pszValue = NULL)
	{
		return Analytics_OnEvent(_pszAction, _pszActionPostfix, _pszCategory, _pszLabel, _pszValue, true);
	}

	static HRESULT Analytics_OnEvent( LPCWSTR _pszAction, LPCWSTR _pszActionPostfix = NULL, LPCWSTR _pszCategory = L"Feature", LPCWSTR _pszLabel = NULL, LPCWSTR _pszValue = NULL, bool _bSync = false )
	{
		CComQIPtr<IMFSideData> qpMFSideData( GetMFFactory() );
		if( !qpMFSideData )
			return E_NOINTERFACE;

		WCHAR szAction[1024]={};
		if( _pszAction )
		{
			_snwprintf(szAction, SIZEOF_ARRAY(szAction) - 2, _pszAction );
			if( _pszActionPostfix )
			{
				LPWSTR pActionEnd = szAction + lstrlenW(szAction);

				if( _pszActionPostfix[0] == L'.')
					_pszActionPostfix++;

				*pActionEnd = '_';
				pActionEnd++;
				
				while( *_pszActionPostfix && pActionEnd < szAction + SIZEOF_ARRAY(szAction) )
				{
					if( *_pszActionPostfix != '_' && *_pszActionPostfix != ' ' )
					{
						*pActionEnd = *_pszActionPostfix;
						pActionEnd++;
					}

					_pszActionPostfix++;
				}

				*pActionEnd = 0;
			}
		}

		int nBuffer = 4096;
		WCHAR* pszParamList = ALLOCA_ARRAY( WCHAR, nBuffer );
		LPWSTR pParam = pszParamList;
		pParam += _snwprintf( pParam, nBuffer - (pParam - pszParamList), L"action='%ws' ", szAction );
		if( _pszLabel )
			pParam += _snwprintf( pParam, nBuffer - (pParam - pszParamList), L"label='%ws' ", _pszLabel );
		if( _pszValue )
			pParam += _snwprintf( pParam, nBuffer - (pParam - pszParamList), L"value='%ws' ", _pszValue );
		if( _pszCategory )
			pParam += _snwprintf( pParam, nBuffer - (pParam - pszParamList), L"category='%ws' ", _pszCategory );
		if (_bSync)
			pParam += _snwprintf(pParam, nBuffer - (pParam - pszParamList), L"sync='true' ");

		return  qpMFSideData->MFStrSet( CComBSTR(MF_ANALYTICS_STR_ID), CComBSTR(pszParamList) );
	}

	//////////////////////////////////////////////////////////////////////////
	// Settings save/load
	// 

	static HRESULT SaveSettings( LPCWSTR _pszKeyName, LPCWSTR _pszAttributesList )
	{
		CComQIPtr<IMFConfig> qpMFConfig( GetMFFactory() );
		if( !qpMFConfig )
			return E_NOINTERFACE;

		return qpMFConfig->SaveSettings( CComBSTR(_pszKeyName), CComBSTR( _pszAttributesList ) );
	}

	static HRESULT LoadSettings( LPCWSTR _pszKeyName, BSTR* _pbsAttributesList )
	{
		CComQIPtr<IMFConfig> qpMFConfig( GetMFFactory() );
		if( !qpMFConfig )
			return E_NOINTERFACE;

		return qpMFConfig->LoadSettings( CComBSTR(_pszKeyName), _pbsAttributesList );
	}

	inline static bool IsLocal( IUnknown* _pFrame )
	{
		DWORD dwProcessID = 0;
		CComQIPtr<IMFFrame> qpMFFrame( _pFrame );
		if( qpMFFrame )
		{
			qpMFFrame->MFGetProcessID( &dwProcessID );
			return dwProcessID == GetCurrentProcessId();
		}

		CComQIPtr<IMFBuffer> qpMFBuffer( _pFrame );
		if( qpMFBuffer )
		{
			LONGLONG llSharedID = 0;
			eMFBufferFlags eFlags = eMFBF_Empty;
			qpMFBuffer->BufferSharedIdGet( &llSharedID, &dwProcessID, &eFlags );
			return dwProcessID == GetCurrentProcessId();
		}

		CComQIPtr<IMFSideData> qpMFSideData( _pFrame );
		if( qpMFSideData )
		{
			LONG lFake = 0;
			LONGLONG llProcessID = 0;
			qpMFSideData->MFDataGet( CComBSTR(L"_process_pid_"), &lFake, &llProcessID );
			ATLASSERT( llProcessID );
			return (DWORD)llProcessID == GetCurrentProcessId();
		}

		// Unknown object type
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// Allocator
		 

// Priority for IMFFrame
template<class TFrame>
	static HRESULT MakeLocal( IUnknown* _pFrame, TFrame** _ppFrameLocal, bool _bMakeRef = false )
	{
		if (!_pFrame)
			return E_INVALIDARG;

		CComPtr<IMFFrame> cpMFFrameLocal;
		HRESULT hr = MakeLocalMF( _pFrame, &cpMFFrameLocal, _bMakeRef );
		if( SUCCEEDED( hr) )
			return cpMFFrameLocal.QueryInterface( _ppFrameLocal );

		CComPtr<IMFBuffer> cpMFBuffer;
		hr = MakeLocalBF( _pFrame, &cpMFBuffer, _bMakeRef );
		if( SUCCEEDED( hr) )
			return cpMFBuffer.QueryInterface( _ppFrameLocal );

// 		CComPtr<IMFSideData> cpMFSideData;
// 		hr = MakeLocalSD( _pFrame, &cpMFSideData, _bMakeRef );
// 		if( SUCCEEDED( hr) )
// 			return cpMFSideData.QueryInterface( _ppFrameLocal );

		return hr;
	}

	static HRESULT MakeLocalMF( IUnknown* _pFrame, IMFFrame** _ppFrameLocal, bool _bMakeRef = false )
	{
		if (!_pFrame)
			return E_INVALIDARG;

		bool bLocal = IsLocal(_pFrame);
		if( !_bMakeRef && bLocal )
			return _pFrame->QueryInterface( _ppFrameLocal );

// 		if( MFFrame_IsGPU( _pFrame) )
// 		{
// 			CComQIPtr<IMFFrameGPU> qpMFFrameGPU(_pFrame);
// 			if (qpMFFrameGPU)
// 				qpMFFrameGPU->MFVideoWaitExecution(NULL);
// 		}

		IMFFactory* pFactory = GetMFFactory();
		if( !pFactory ) return E_UNEXPECTED;

		return pFactory->MFFrameMakeLocal( _pFrame, _ppFrameLocal, _bMakeRef );
	}

	static HRESULT MakeLocalBF( IUnknown* _pBuffer, IMFBuffer** _ppBufferLocal, bool _bMakeRef = false )
	{
		if (!_pBuffer)
			return E_INVALIDARG;

		if( !_bMakeRef && IsLocal(_pBuffer) )
			return _pBuffer->QueryInterface( _ppBufferLocal );

		CComQIPtr<IMFBuffer> qpMFBuffer( _pBuffer );
		if( !qpMFBuffer ) return E_NOINTERFACE;

		CComQIPtr<IMFBuffersAllocator> qpBuffersAlloc( GetMFFactory() );
		if( !qpBuffersAlloc ) return E_UNEXPECTED;

		return qpBuffersAlloc->MFBufferTransferIPC( qpMFBuffer, _ppBufferLocal, (eMFTransferFlags)((_bMakeRef ? eMFTF_CopyLocal : eMFTF_Default) | eMFTF_SideDataCopy) );
	}

// 	static HRESULT MakeLocalSD( IUnknown* _pSideData, IMFSideData** _ppSideDataLocal, bool _bMakeRef = false )
// 	{
// 		if( !_bMakeRef && IsLocal(_pSideData) )
// 			return _pSideData->QueryInterface( _ppSideDataLocal );
// 
// 		CComQIPtr<IMFSideData> qpMFSideDataRemote( _pSideData );
// 		if( !qpMFSideDataRemote ) return E_NOINTERFACE;
// 
// 		// TODO: Use MFSideDataLocal class
// 		CComPtr<IMFSideData> cpMFSideDataLocal;
// 		HRESULT hr = cpMFSideDataLocal.CoCreateInstance( __uuidof(MFSideData) );
// 		if( FAILED( hr ) )
// 			return hr;
// 
// 	}

// 	static HRESULT MakeLocalPKT( IUnknown* _pFrame, IMFPacket** _ppFrameLocal, bool _bMakeRef = false )
// 	{
// 		if( !_bMakeRef && IsLocal(_pFrame) )
// 			return _pFrame->QueryInterface( _ppFrameLocal );
// 
// 		CComQIPtr<IMFBuffer> qpMFFrame( _pFrame );
// 		if( !qpMFFrame ) return E_NOINTERFACE;
// 
// 		CComQIPtr<IMFBuffersAllocator> qpBuffersAlloc( GetMFFactory() );
// 		if( !qpBuffersAlloc ) return E_UNEXPECTED;
// 
// 		return qpBuffersAlloc->MFBufferTransferIPC( qpMFFrame, _ppFrameLocal, _bMakeRef ? eMFTF_CopyLocal : eMFTF_Default );
// 	}

	template<class TFrame> 
	static HRESULT MFFrameCreate( /*[in]*/ int _cbSizeMax, /*[out]*/ TFrame** _ppFrame )
	{
		IMFFactory* pFactory = GetMFFactory();
		if( !pFactory )
			return CO_E_CLASS_CREATE_FAILED;

		CComPtr<IMFFrame> cpMFFrame;
		HRESULT hr = pFactory->MFFrameCreatePlain( _cbSizeMax, &cpMFFrame );
		if( FAILED( hr ) )
			return hr;

		ATLASSERT( cpMFFrame );
		return cpMFFrame.QueryInterface( _ppFrame );
	}


	template<class TFrame>
	static HRESULT MFFrameClone( /*[in]*/ IUnknown* _pFrame, /*[out]*/ TFrame** _ppCloneFrame, /*[in]*/ eMFrameClone _eCloneType, /*[in]*/ eMFCC _fccPixelFormat = eMFCC_Default )
	{
		IMFFactory* pFactory = GetMFFactory();
		if( !pFactory )
			return CO_E_CLASS_CREATE_FAILED;
		
		CComPtr<IMFFrame> cpMFFrame;
		HRESULT hr = pFactory->MFFrameClone( _pFrame, &cpMFFrame, _eCloneType, _fccPixelFormat );
		if( FAILED( hr ) )
			return hr;

		ATLASSERT( cpMFFrame );
		return cpMFFrame.QueryInterface( _ppCloneFrame );
	}

	static DXGI_FORMAT DX_Fcc2Format_I(eMFCC _eFCC)
	{
		//return DXGI_FORMAT_B8G8R8A8_UNORM;

		switch (_eFCC)
		{
		case eMFCC_RGB24:
		case eMFCC_RGB32:
			//return DXGI_FORMAT_B8G8R8X8_UNORM;
		case eMFCC_ARGB32:
			return DXGI_FORMAT_B8G8R8A8_UNORM;
		case eMFCC_RGB8:
			return DXGI_FORMAT_R8_UNORM;
		case eMFCC_YUY2:
		case eMFCC_HDYC:
		case eMFCC_YVYU:
		case eMFCC_UYVY:
			return DXGI_FORMAT_YUY2;
		case eMFCC_I420:
		case eMFCC_YV12:
		case eMFCC_NV12:
			return DXGI_FORMAT_NV12;
		case eMFCC_r210:
			return DXGI_FORMAT_R10G10B10A2_UNORM;
		case eMFCC_v210:
			return DXGI_FORMAT_Y210;
		default:
			break;
		}

		return DXGI_FORMAT_B8G8R8A8_UNORM;
	}

	// If _bLazyClone is true -> DO NOT CLONE if GPU/CPU is same and FCC is same
	template<class TFrame>
	static HRESULT MFFrameCloneI(bool _bLazyClone, /*[in,out]*/ TFrame& _rCloneFrame, /*[in]*/ eMFrameClone _eCloneType, /*[in]*/ eMFCC _fccPixelFormat = eMFCC_Default)
	{
		if (_bLazyClone)
		{
			bool bSkipClone = IsLocal(_rCloneFrame);

			// Clone if pixel format changed -> reset _bLazyClone flag
			if (_fccPixelFormat != eMFCC_Default)
			{
				M_AV_PROPS avProps = MFFrame_AVProps(_rCloneFrame);
				
				// TODO: Think how to optimize this method
				if(avProps.vidProps.eGPUFlags != eGTF_CPU_Frame && DX_Fcc2Format_I(_fccPixelFormat) != (avProps.vidProps.eGPUFlags & eGTF_GPU_FormatMask))
					bSkipClone = false; // For GPU check texture format
				else if (avProps.vidProps.eGPUFlags == eGTF_CPU_Frame && !IsSimilarFCC(avProps.vidProps.fccType, _fccPixelFormat))
					bSkipClone = false; // For CPU check frame format
			}

			// Clone if cpu/gpu type changed -> reset _bLazyClone flag
			if (_eCloneType & (eMFC_ForceCPU|eMFC_ForceGPU) )
			{
				bool bGPUFrame = MFFrame_IsGPU(_rCloneFrame);
				if ((bGPUFrame && (_eCloneType & eMFC_ForceCPU)) ||
					(!bGPUFrame && (_eCloneType & eMFC_ForceGPU)))
				{
					bSkipClone = false;
				}
				
				if ((_eCloneType & (eMFC_Video|eMFC_VideoRef)) == 0)
					_eCloneType = (eMFrameClone)(_eCloneType | eMFC_Reference);
			}

			if (bSkipClone) // Fo nothing -> cpu/gpu type is same, fcc is same
				return S_FALSE;
		}

		CComPtr<IMFFrame> cpMFFrame;
		HRESULT hr = MFFrameClone(_rCloneFrame, &cpMFFrame, _eCloneType, _fccPixelFormat);
		if (FAILED(hr))
			return hr;

		ATLASSERT(cpMFFrame);
		_rCloneFrame = cpMFFrame;

		return S_OK;
	}

	template <class TFrame>
	static HRESULT MFFrameCreateFromMem( /*[in]*/ M_AV_PROPS* _pAVProps, /*[in]*/ LONGLONG _pbVideo, /*[in]*/ long _lAudioSamples, /*[in]*/ LONGLONG _lpAudio, /*[out]*/ TFrame** _ppFrame, /*[in]*/ LPCWSTR _pszPropsList = NULL )
	{
		IMFFactory* pFactory = GetMFFactory();
		if( !pFactory ) return E_UNEXPECTED;

		CComPtr<IMFFrame> cpMFFrame;
		HRESULT hr = pFactory->MFFrameCreateFromMem( _pAVProps, _pbVideo, _lAudioSamples, _lpAudio, &cpMFFrame, _pszPropsList ? CComBSTR(_pszPropsList) : NULL );
		if( FAILED( hr ) )
			return hr;

		ATLASSERT( cpMFFrame );
		if( !cpMFFrame )
			return E_UNEXPECTED;

		return cpMFFrame.QueryInterface( _ppFrame );
	}

	template <class TFrame>
	static HRESULT MFFrameCreateFromFile( /*[in]*/ LPCWSTR _pszFileName, /*[out]*/ TFrame** _ppFrame, /*[in]*/ LPCWSTR _pszPropsList )
	{
		IMFFactory* pFactory = GetMFFactory();
		if( !pFactory ) return E_UNEXPECTED;

		CComPtr<IMFFrame> cpMFFrame;
		HRESULT hr = pFactory->MFFrameCreateFromFile( CComBSTR(_pszFileName), &cpMFFrame, CComBSTR(_pszPropsList) );
		if (FAILED(hr))
			return hr;

		ATLASSERT(cpMFFrame);
		if (!cpMFFrame)
			return E_UNEXPECTED;

		return cpMFFrame.QueryInterface(_ppFrame);
	}

	static HRESULT MFFrameCreateFromHBITMAP( /*[in]*/ LONGLONG _hBitmap, /*[out]*/ IMFFrame** _ppFrame, /*[in]*/ BSTR _bsPropsList )
	{
		IMFFactory* pFactory = GetMFFactory();
		if( !pFactory ) return E_UNEXPECTED;

		return pFactory->MFFrameCreateFromHBITMAP( _hBitmap, _ppFrame, _bsPropsList );
	}

#ifdef __DSVIDEO_H_
	static HRESULT MFFrameCreateFromResource( /*[in]*/ DWORD _dwResourceID, /*[out]*/ IMFFrame** _ppFrame, /*[in]*/ BSTR _bsPropsList, /*[in]*/ LPCWSTR _pszResourceType = L"CUSTOM" )
	{
		
#if (_ATL_VER >= 0x0700)
		HINSTANCE hInstance = ATL::_AtlBaseModule.GetResourceInstance();
#else	// !(_ATL_VER >= 0x0700)
		HINSTANCE hInstance = _Module.GetResourceInstance();
#endif	// !(_ATL_VER >= 0x0700)

		USES_CONVERSION;

		LPBYTE pbImageTask = NULL;
		BITMAPINFOHEADER bmiHeader ={0};
		HRESULT hr = CPicturePersist::LoadFromResource( hInstance, _dwResourceID, W2CT(_pszResourceType), &bmiHeader, &pbImageTask );
		ATLASSERT( SUCCEEDED( hr ) );
		if( FAILED( hr ) ) return hr;

		M_AV_PROPS avProps = {};
		avProps.vidProps.fccType = bmiHeader.biBitCount == 32 ? eMFCC_RGB32 : eMFCC_RGB24;
		avProps.vidProps.nHeight = bmiHeader.biHeight;
		avProps.vidProps.nWidth = bmiHeader.biWidth;
		avProps.vidProps.nRowBytes = ALIGN_UP( avProps.vidProps.nWidth * (avProps.vidProps.fccType == eMFCC_RGB32 ? 4 : 3 ), 4 );

		hr = MCreator::MFFrameCreateFromMem( &avProps, (LONGLONG)pbImageTask, 0, NULL, _ppFrame, NULL );
		::CoTaskMemFree(pbImageTask);

		return hr;
	}
#endif

	static HRESULT MFFrameCreateWrapper( /*[in]*/ M_VID_PROPS* _pVidProps, /*[in] */DWORD _cbVideo, /*[in]*/ LONGLONG _lpVideo, /*[in]*/ IUnknown* _pObject, /*[out]*/ IMFFrame** _ppFrame )
	{
		IMFFactory* pFactory = GetMFFactory();
		if( !pFactory )
			return CO_E_CLASS_CREATE_FAILED;
		
		return pFactory->MFFrameCreateWrapper( _pVidProps, _cbVideo, _lpVideo, _pObject, _ppFrame );
	}

template <class TFrame>
	static HRESULT MFFrameCreatePlain( /*[in]*/ LONG _cbVideoSize, /*[out]*/ TFrame** _ppFrame )
	{
		IMFFactory* pFactory = GetMFFactory();
		if( !pFactory )
			return CO_E_CLASS_CREATE_FAILED;

		CComPtr<IMFFrame> cpMFFrame;
		HRESULT hr = pFactory->MFFrameCreatePlain( _cbVideoSize, &cpMFFrame );
		if( FAILED( hr ) )
			return hr;

		ATLASSERT( cpMFFrame );
		if( !cpMFFrame )
			return E_UNEXPECTED;

		return cpMFFrame.QueryInterface( _ppFrame );
	}

template <class TFrame>
	static HRESULT MFFrameCreateFromMemEx( /*[in]*/ M_AV_PROPS* _pAVProps, /*[in]*/ MF_VID_PTR* _pVideoPtr, /*[in]*/ long _lAudioSamples, /*[in]*/ LONGLONG _lpAudio, /*[out]*/ TFrame** _ppFrame, /*[in]*/ BSTR _bsPropsList)
	{
		IMFFactory* pFactory = GetMFFactory();
		if (!pFactory)
			return CO_E_CLASS_CREATE_FAILED;

		CComPtr<IMFFrame> cpMFFrame;
		HRESULT hr = pFactory->MFFrameCreateFromMemEx(_pAVProps, _pVideoPtr, _lAudioSamples, _lpAudio, &cpMFFrame, _bsPropsList);
		if (FAILED(hr))
			return hr;

		ATLASSERT(cpMFFrame);
		if (!cpMFFrame)
			return E_UNEXPECTED;

		return cpMFFrame.QueryInterface(_ppFrame);
	}

template <class TFrame>
	static HRESULT MFFrameCreateFromTexture( /*[in]*/ M_AV_PROPS* _pAVProps, /*[in]*/ IUnknown* _pD3DTexture2D, /*[in]*/ long _lAudioSamples, /*[in]*/ LONGLONG _lpAudio, /*[out]*/ TFrame** _ppFrame, /*[in]*/ LPCWSTR _pszPropsList )
	{
		IMFFactory* pFactory = GetMFFactory();
		if( !pFactory ) return E_UNEXPECTED;

		CComPtr<IMFFrame> cpMFFrame;
		HRESULT hr = pFactory->MFFrameCreateFromTexture( _pAVProps, _pD3DTexture2D, _lAudioSamples, _lpAudio, &cpMFFrame, _pszPropsList ? CComBSTR(_pszPropsList) : NULL );
		if( FAILED( hr ) )
			return hr;

		ATLASSERT( cpMFFrame );
		if( !cpMFFrame )
			return E_UNEXPECTED;

		return cpMFFrame.QueryInterface( _ppFrame );
	}

	template <class TFrame>
	static HRESULT MFFrameCreateFromFields( /*[in]*/ M_AV_PROPS* _pAVProps, /*[in]*/ IUnknown* _pD3DTexture2D_F1, /*[in]*/ IUnknown* _pD3DTexture2D_F2, /*[in]*/ long _lAudioSamples, /*[in]*/ LONGLONG _lpAudio, /*[out]*/ TFrame** _ppFrame, /*[in]*/ LPCWSTR _pszPropsList)
	{
		IMFFactory* pFactory = GetMFFactory();
		if (!pFactory) return E_UNEXPECTED;

		CComPtr<IMFFrame> cpMFFrame;
		HRESULT hr = pFactory->MFFrameCreateFromFields(_pAVProps, _pD3DTexture2D_F1, _pD3DTexture2D_F2, _lAudioSamples, _lpAudio, &cpMFFrame, _pszPropsList ? CComBSTR(_pszPropsList) : NULL);
		if (FAILED(hr))
			return hr;

		ATLASSERT(cpMFFrame);
		if (!cpMFFrame)
			return E_UNEXPECTED;

		return cpMFFrame.QueryInterface(_ppFrame);
	}


	template<class TInterface>
	static HRESULT MakeLocalI( CComPtr<TInterface>& _rcpFrame,  bool _bMakeRef = false )
	{
		CComPtr<TInterface> cpMFFrameRes;
		HRESULT hr = MakeLocal( _rcpFrame, &cpMFFrameRes, _bMakeRef );
		if( FAILED( hr ) )
			return hr;

		if( !cpMFFrameRes )
			return S_FALSE;

		_rcpFrame = cpMFFrameRes;
		return S_OK;
	}

	template<class TInterface>
	static HRESULT MakeLocalI( CComPtrM<TInterface>& _rcpFrame,  bool _bMakeRef = false )
	{
		CComPtr<TInterface> cpMFFrameRes;
		HRESULT hr = MakeLocal( _rcpFrame, &cpMFFrameRes, _bMakeRef );
		if( FAILED( hr ) )
			return hr;

		if( !cpMFFrameRes )
			return S_FALSE;

		_rcpFrame = cpMFFrameRes;
		return S_OK;
	}


	template<class TOutput>
	static HRESULT FramesClone( /*[in]*/ IUnknown* _pFrameIn, /*[out]*/ TOutput** _ppFrameClone, /*[in]*/ eMFrameClone _eCloneType, /*[in]*/ eMFCC _fccPixelFormat )
	{
		if( !_pFrameIn )
			return E_INVALIDARG;

		IMFFactory* pFactory = GetMFFactory();
		if( !pFactory )
			return CO_E_CLASS_CREATE_FAILED;
		
		CComPtr<IMFFrame> cpFrameClone;
		HRESULT hr = pFactory->MFFrameClone( _pFrameIn, &cpFrameClone, _eCloneType, _fccPixelFormat );
		if( FAILED( hr ) )
			return hr;
		
		hr = cpFrameClone.QueryInterface( _ppFrameClone );
		ATLASSERT( SUCCEEDED( hr ) && "FramesClone(IMFFrame.QueryInterface)" );
		if( SUCCEEDED( hr ) )
			return hr;

		return hr;
	}
	

	static HRESULT FramesCreate( /*[in]*/ int _cbSizeMax, /*[in]*/ BSTR  _bsProps, /*[out]*/ IMFrame** _ppFrame )
	{
		IMFrames_I* pFrames = GetFramesStore();
		if( !pFrames )
			return E_UNEXPECTED;

		return pFrames->FramesCreate( _cbSizeMax, _bsProps, _ppFrame );
	}

	static HRESULT FramesCreate_I( /*[in]*/ int _cbVideoSize, /*[in]*/ int _cbAudioSize, /*[in]*/ BSTR  _bsProps, /*[out]*/ IMFrame_I** _ppFrame )
	{
		IMFrames_I* pFrames = GetFramesStore();
		if( !pFrames )
			return E_UNEXPECTED;

		return pFrames->FramesCreate_I( _cbVideoSize, _cbAudioSize, _bsProps, _ppFrame );
	}

	static HRESULT FramesMakeLocal( /*[in]*/ IMFrame* _pFrame, /*[out]*/ IMFrame** _ppFrameLocal, BOOL _bMakeRef )
	{
		IMFrames_I* pFrames = GetFramesStore();
		if( !pFrames )
			return E_UNEXPECTED;

		return pFrames->FramesMakeLocal( _pFrame, _ppFrameLocal, _bMakeRef );
	}

	static HRESULT FramesCreateFromMem( /*[in]*/ M_VID_PROPS* _pVidProps, /*[in]*/ LONGLONG _pbVideo, 
		/*[out]*/ IMFrame** _ppFrame, /*[in]*/ BSTR _bsParam )
	{
		IMFrames_I* pFrames = GetFramesStore();
		if( !pFrames )
			return E_UNEXPECTED;

		return pFrames->FramesCreateFromMem( _pVidProps, _pbVideo, _ppFrame, _bsParam );
	}

	static HRESULT FramesCreateFromFile( /*[in]*/ BSTR _bsFileName, /*[out]*/ IMFrame** _ppFrame, /*[in]*/ BSTR _bsParam )
	{
		IMFrames_I* pFrames = GetFramesStore();
		if( !pFrames )
			return E_UNEXPECTED;

		return pFrames->FramesCreateFromFile( _bsFileName, _ppFrame, _bsParam );
	}

	static HRESULT FramesCreateFromHBITMAP( /*[in]*/ LONGLONG _hBitmap, /*[out]*/ IMFrame** _ppFrame, /*[in]*/ BSTR _bsParam )
	{
		IMFrames_I* pFrames = GetFramesStore();
		if( !pFrames )
			return E_UNEXPECTED;

		return pFrames->FramesCreateFromHBITMAP( _hBitmap, _ppFrame, _bsParam );
	}

	
	static HRESULT FramesConvert_I( /*[in]*/ IMFrame* _pFrameSrcL_F1, /*[in]*/ IMFrame* _pFrameSrcR_F2, /*[in]*/ M_VID_PROPS* _pVidPropsDest, /*[out]*/ IMFrame** _ppFrameDest, /*[in]*/ BSTR _bsParam )
	{
		IMFrames_I* pFrames = GetFramesStore();
		if( !pFrames )
			return E_UNEXPECTED;

		return pFrames->FramesConvert_I( _pFrameSrcL_F1, _pFrameSrcR_F2, _pVidPropsDest, _ppFrameDest, _bsParam );
	}

	// IMFrames_I
// 	static HRESULT AllocBuffer( DWORD _cbSize, LONGLONG* _ppBuffer )
// 	{
// 		IMFrames_I* pFrames = GetFramesStore();
// 		if( !pFrames )
// 			return E_UNEXPECTED;
// 
// 		return pFrames->AllocBuffer( _cbSize, _ppBuffer );
// 	}
// 
// 	static HRESULT ReleaseBuffer( LONGLONG _pBuffer )
// 	{
// 		IMFrames_I* pFrames = GetFramesStore();
// 		if( !pFrames )
// 			return E_UNEXPECTED;
// 
// 		return pFrames->ReleaseBuffer( _pBuffer );
// 	}

	// IMFBufferAllocator
	template<class TInterface>
	static HRESULT MFBufferAlloc( /*[in]*/ DWORD _cbSize, /*[in]*/ LONGLONG _lpBuffer, /*[out]*/ TInterface** _ppBuffer, /*[in]*/ eMFBufferFlags _eFlags)
	{
		CComQIPtr<IMFBuffersAllocator> qpMFBuffers( GetMFFactory() );
		if( !qpMFBuffers )
			return E_NOINTERFACE;

		CComPtr<IMFBuffer> cpMFBuffer;
		HRESULT hr = qpMFBuffers->MFBufferAlloc( _cbSize, _lpBuffer, &cpMFBuffer, _eFlags );
		if( FAILED( hr ) )
			return hr;

		return cpMFBuffer.QueryInterface( _ppBuffer );
	}

	// IMFBufferAllocator
	template<class TInterface>
	static HRESULT MFBufferAllocPacket( /*[in]*/ M_STREAM_INFO* _pStreamInfo, /*[in]*/ M_PACKET_INFO* _pPacketInfo, /*[out]*/ TInterface** _ppPacket )
	{
		CComQIPtr<IMFBuffersAllocator> qpMFBuffers( GetMFFactory() );
		if( !qpMFBuffers )
			return E_NOINTERFACE;

		CComPtr<IMFPacket> cpMFPacket;
		HRESULT hr = qpMFBuffers->MFBufferAllocPacket( _pStreamInfo, _pPacketInfo, &cpMFPacket );
		if( FAILED( hr ) )
			return hr;

		return cpMFPacket.QueryInterface( _ppPacket );
	}

	template<class TInterface>
	static HRESULT MFBufferTransferIPC( /*[in]*/ IUnknown* _pRemoteBuffer, /*[out]*/ TInterface** _ppLocalBuffer, /*[in]*/ eMFTransferFlags _eFlags )
	{
		CComQIPtr<IMFBuffer> qpBFBufferIn( _pRemoteBuffer );
		if( !qpBFBufferIn )
			return E_NOINTERFACE;

		CComQIPtr<IMFBuffersAllocator> qpMFBuffersAllocator( GetMFFactory() );
		if( !qpMFBuffersAllocator )
			return E_NOINTERFACE;

		CComPtr<IMFBuffer> cpMFBuffer;
		HRESULT hr = qpMFBuffersAllocator->MFBufferTransferIPC( qpBFBufferIn, &cpMFBuffer, _eFlags );
		if( FAILED( hr ) )
			return hr;

		return cpMFBuffer.QueryInterface( _ppLocalBuffer );
	}
};

_declspec(selectany) CComAutoCriticalSection	 MCreator::s_csFactory;
_declspec(selectany) IMFFactory*		MCreator::s_pMFFactory;
_declspec(selectany) IMFrames_I*		MCreator::s_pFramesStore;
