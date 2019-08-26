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

#include "StdAfx.h"
#include "MFormats.h"

#include "../Base/MHelpers.h"
#include "../XML/XMLParseM.h"


typedef HRESULT (*PFNCAPTURECALLBACK)( IN void* pvInstance, IN IMFFrame*  _pFrame, IN LONGLONG _llExtraParam, IN LPCWSTR _pszExtraParam );

// For notification from capture object
// For error -> used negative _llExtraParam values
#define MFDEVICE_INPUTFORMAT_CHANGED			(1)



// This class represent one specified capture device (e.g. BlackMagic or 
template< class T, class TBase = AutoObjBase<T> >
class MCaptureBase: 
	public TBase
{
protected:
	// Critical section for multi threaded calls
	CComAutoCriticalSection		m_csDevice;

	// Device name (with channel)
	CComBSTR						m_cbsDeviceName;
	// AJA Device name (w/o channel - the same as devInfo.deviceIdentifier)
	CComBSTR						m_cbsDeviceIdentifier;

	// XML props (for config & statistics)
	// For statistics usually used "stat" node
	XMLParse::XMLNodePtr			m_pXMLProps;

	// Current state
	eMState						m_eState;
	// Capture AV Props
	M_AV_PROPS					m_avProps;
	// For callback
	PFNCAPTURECALLBACK			m_pfCallback;
	void*						m_pvCallback;
public:
	MCaptureBase() 
		: m_eState( eMS_Closed ),
		  m_pvCallback( NULL ),
		  m_pfCallback( NULL )
	{
		::ZeroMemory( &m_avProps, sizeof(m_avProps) );
	};

	virtual ~MCaptureBase() 
	{
	};

	//////////////////////////////////////////////////////////////////////////
	// For current_device <- live
	HRESULT Capture_SendEvent(/*[in] */LONGLONG llCallbackUserData, /*[in] */LPCWSTR bsName,
		/*[in] */LPCWSTR bsEventName, /*[in] */LPCWSTR bsEventParam, /*[in]*/IUnknown* pEventObject)
	{
		return E_NOTIMPL;
	}

	HRESULT Capture_SendFrame(/*[in]*/ LONGLONG llCallbackUserData, /*[in]*/ LPCWSTR bsName, /*[in]*/ IUnknown* pMFrame)
	{
		return E_NOTIMPL;
	}


	// Example for creation function
// 	static HRESULT CreateObject( IN int _nCardIdx, IN int _nChannelIdx, IN XMLParse::XMLNodePtr _pXMLProps, OUT MCaptureTest** _ppInstance )
// 	{
// 		MCaptureTest::TPtr pCaptureTest;
// 		MCaptureTest::CreateInstance( &pCaptureTest );
// 
// 		WCHAR szName[128]={};
// 		wsprintfW( szName, L"Demo Device Ch %d (%d)",  _nChannelIdx, _nCardIdx + 1 );
// 
// 		pCaptureTest->m_cbsDeviceName = szName;
// 		pCaptureTest->m_nDeviceIndex = _nCardIdx;
// 		pCaptureTest->m_nDeviceChannel = _nChannelIdx;
// 		pCaptureTest->m_pXMLProps = _pXMLProps;
// 
// 		return pCaptureTest.CopyTo( _ppInstance );
// 	}

	// Example for device enumeration - MUST BE DEFINED IN DERIVED CLASS
// 	static int EnumDevicesXML( IN XMLParse::XMLNodePtr _pXMLProps, OUT CSimpleMap2<CComBSTR, MCaptureTest::TPtr>* _pMapDevices )
// 	{
// 		ATLASSERT( _pMapDevices && _pXMLProps );
// 		if( !_pMapDevices || !_pXMLProps ) return E_POINTER;
// 
// 		_pMapDevices->RemoveAll();
// 
// 		for( int i = 0; i < _pXMLProps->GetIntAttribute( L"demo_devices_count", TEST_DEVICES_COUNT); i++ )
// 		{
// 			for( int j = 0; j < _pXMLProps->GetIntAttribute( L"demo_devices_channels", TEST_DEVICES_CHANNELS_COUNT); j++ )
// 			{
// 				MCaptureTest::TPtr pDevice;
// 				CreateObject( i, j, _pXMLProps, &pDevice );
// 
// 				_pMapDevices->Add( pDevice->Capture_Name(), pDevice );
// 			}
// 		}
// 
// 		return _pMapDevices->GetSize();
// 	}

public:

	//////////////////////////////////////////////////////////////////////////
	// Information methods
	
	// Return capture device name 
	virtual CComBSTR Capture_Name() { CAutoCS crs( m_csDevice); return m_cbsDeviceName; }

	// Return 0 -> not busy, != 0 is BUSY, return values (e.g. 1,2,3,-1 etc.) can be used for specified busy state.
	virtual int	Capture_IsBusy() = 0;

	//////////////////////////////////////////////////////////////////////////
	// Format enumeration / support methods
	
	// Return true if specified format supported
	virtual bool Capture_IsFormatSupported( M_VID_PROPS* _pVidProps ) 
	{
		return true;
	}

	// Return true if specified format supported
	virtual bool Capture_IsFormatSupported( M_AUD_PROPS* _pAudProps ) 
	{
		return true;
	}

	// Next methods used for CUSTOM video props (w/o eMVidFormat enumeration) and for specify supported memory formats

	// If return false -> for use default formats list and select desired format via Capture_IsFormatSupported() 
	// If return true -> used _pSupportedArray for fill formats list
	virtual bool Capture_EnumFormats( CSimpleArray2<M_VID_PROPS>* _pSupportedArray, CSimpleArray2<eMFCC>* _pPixelFormatsArray = NULL ) 
	{
		// By default - use default formats list
		return false;
	}

	// Next methods used for CUSTOM audio props (with non-standard sample-rate)
	// 
	// If return false -> for use default formats list and select desired format via Capture_IsFormatSupported() 
	// If return true -> used _pSupportedArray for fill formats list
	virtual bool Capture_EnumFormats( CSimpleArray2<M_AUD_PROPS>* _pSupportedArray ) 
	{
		// By default use default formats list
		return false;
	}

	
	//////////////////////////////////////////////////////////////////////////
	// Config methods
	
	// Set (change) capture props - the implementation should allow to change this props in ANY state (e.g. eMS_Running)
	// If some props required capture restart -> the implemenation sould stop, change props, start capture device
	virtual HRESULT Capture_OnPropsSet( IN LPCWSTR _pszPropName, IN LPCWSTR _pszPropValue ) = 0;

	// Return current capture props (used for get real props after e.g. Capture_OnPropsSet() failed)
	virtual LPCWSTR Capture_PropsGet( IN LPCWSTR _pszPropName ) = 0;

	//////////////////////////////////////////////////////////////////////////
	// Control methods

	// Open specified device for capture 
	// Used for fill device props (for later set via IMFProps interface) - could be skipped, bu just only Capture_Start() call.
	// Also can be used for 'reserve' device for capture - e.g. for some BMD devices - not allow to open the second instance or same device
	// State: eMS_Closed -> eMS_Stopped
	virtual HRESULT Capture_Open( IN XMLParse::XMLNodePtr _pNodeWithAttributesInfo ) = 0;

	// Start capture, the M_AV_PROPS could be NULL (for default or auto-detect), on return fill _pAVProps with current capture props
	// State: eMS_Closed, eMS_Stopped -> eMS_Running
	virtual HRESULT Capture_Start( IN OUT M_AV_PROPS* _pAVProps, IN void* _pvInstance, IN PFNCAPTURECALLBACK _pfOnFrame ) = 0;

	// Stop capture (optional, Capture_Close() could be used if device 'reserving' not used)
	// State: eMS_Running, eMS_Error -> eMS_Stopped, eMS_Closed -> eMS_Closed
	virtual HRESULT Capture_Stop() = 0;

	// Close capture 
	// State: eMS_Running, eMS_Stopped, eMS_Error -> eMS_Closed
	virtual HRESULT Capture_Close() = 0;

	// Return current capture state
	virtual eMState Capture_StateGet() { return m_eState; } 

	// Return current capture AV props
	virtual M_AV_PROPS Capture_AVPropsGet() { CAutoCS crs( m_csDevice); return m_avProps; }
};




