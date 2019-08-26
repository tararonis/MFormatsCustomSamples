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

#include "MFormats.h"

#include "../WinAPI/wrappers.h"
#include "../XML/XMLParseM.h"
#include "MFormatImpl.h"
#include "MSourceImpl.h"
#include "MPropsImpl.h"

static LPCWSTR ppszMFDev_AudioNames[] = { L"<From Video>", L"<No Audio>", NULL };

template <class T, class TCapture>
class IMFDeviceImpl_Capture : 
	public IMFDevice,
	public IMCallback
{
	typedef typename TCapture::TPtr	TCapturePtr;

	typedef IMFDeviceImpl_Capture<T,TCapture> _IMFDeviceImpl;
	

protected:
	CComAutoCriticalSection		m_csDevices;

	// Current device
	TCapturePtr	m_apCurrentDevice;
	
	// Map of devices
	CSimpleMap2<CComBSTR,TCapturePtr> m_mapDevices;

	// Audio enabled flag
	bool		m_bAudioEnabled;

public:

	// Override this method for custom frame handling
	HRESULT OnFrameReady( IN IMFFrame* _pFrame, IN LONGLONG _llExtraParam, IN LPCWSTR _pszExtraParam )
	{
		METHOD_MUST_BE_OVERRIDEN(OnFrameReady);
	}

	//////////////////////////////////////////////////////////////////////////
	// IMCallback implementation
	// For current_device <- live
	STDMETHOD(OnEvent)(/*[in] */LONGLONG llCallbackUserData, /*[in] */BSTR bsName,
		/*[in] */BSTR bsEventName, /*[in] */BSTR bsEventParam, /*[in]*/IUnknown* pEventObject)
	{
		TCapturePtr	apCurrentDevice = m_apCurrentDevice;
		if (apCurrentDevice)
			return apCurrentDevice->Capture_SendEvent(llCallbackUserData, bsName, bsEventName, bsEventParam, pEventObject);

		return S_FALSE;
	}

	STDMETHOD(OnFrame)(/*[in]*/ LONGLONG llCallbackUserData, /*[in]*/ BSTR bsName, /*[in]*/ IUnknown* pMFrame)
	{
		TCapturePtr	apCurrentDevice = m_apCurrentDevice;
		if (apCurrentDevice)
			return apCurrentDevice->Capture_SendFrame(llCallbackUserData, bsName, pMFrame);

		return S_FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// IMFDevice impl

	STDMETHOD(DeviceGetCount)( /*[in]*/ eMFDeviceType _eDevType, /*[out]*/ int* _pnCount )
	{
		T* pT = static_cast<T*>(this);

		CAutoCS crs(m_csDevices);

		if( !_pnCount )
			return E_POINTER;

		*_pnCount = 0;

		if( _eDevType == eMFDT_Video )
			*_pnCount = m_mapDevices.GetSize();
		else if( _eDevType == eMFDT_Audio )
			*_pnCount = 2;

		MTRACE( "Type:%d Count:%d", _eDevType, *_pnCount );

		return S_OK;
	}

	STDMETHOD(DeviceGetByIndex)( /*[in]*/ eMFDeviceType _eDevType, /*[in]*/ int _nIndex, /*[out]*/ BSTR* _pbsName, /*[out]*/ BOOL* _pbBusy )
	{
		T* pT = static_cast<T*>(this);

		CAutoCS crs(m_csDevices);

		if( _eDevType == eMFDT_Video )
		{
			if( _nIndex < 0 || _nIndex >= m_mapDevices.GetSize() )
				return HRESULT_FROM_WIN32( ERROR_INVALID_INDEX );

			if( _pbBusy )
				*_pbBusy = m_mapDevices[_nIndex].IsEqualObject( m_apCurrentDevice ) ? -1 : m_mapDevices[_nIndex]->Capture_IsBusy();

			if( _pbsName )
				m_mapDevices.GetKeyAt(_nIndex).CopyTo( _pbsName );

			MTRACE( "VIDEO Idx:%d Name:%ws", _nIndex, (LPCWSTR)m_mapDevices.GetKeyAt(_nIndex) );

			return S_OK;
		}

		if( _eDevType == eMFDT_Audio )
		{
			if( _nIndex < 0 || _nIndex >= 2 )
				return HRESULT_FROM_WIN32( ERROR_INVALID_INDEX );

			if( _pbsName && _nIndex == 0 )
				*_pbsName = CComBSTR( ppszMFDev_AudioNames[0] ).Detach();

			if( _pbsName && _nIndex == 1 )
				*_pbsName = CComBSTR( ppszMFDev_AudioNames[1] ).Detach();

			MTRACE( "AUDIO Idx:%d Name:%ws", _nIndex, ppszMFDev_AudioNames[_nIndex] );

			return S_OK;
		}

		return E_INVALIDARG;
	}

	STDMETHOD(DeviceSet)( /*[in]*/ eMFDeviceType _eDevType, /*[in]*/ int  _nIndex, /*[in]*/ BSTR _bsParam )
	{
		T* pT = static_cast<T*>(this);

		CAutoCS crs(m_csDevices);

		MTRACE( "(Type:%d, Idx:%d, Param:%ws)", _eDevType, _nIndex, _bsParam );

		if( _eDevType == eMFDT_Audio )
		{
			m_bAudioEnabled = _nIndex == 0 ? true : false;

			if( m_apCurrentDevice )
				m_apCurrentDevice->Capture_OnPropsSet( L"_audio_enabled_", m_bAudioEnabled ? L"true" : L"false" );

			return _nIndex > 1 ? HRESULT_FROM_WIN32( ERROR_INVALID_INDEX ) : S_OK ;
		}

		if( _eDevType != eMFDT_Video )
			return E_INVALIDARG;

		if( _nIndex < 0  || _nIndex >= m_mapDevices.GetSize( ) )
			return HRESULT_FROM_WIN32( ERROR_INVALID_INDEX );

		// Update properties
		pT->MPropsXML()->ParseAttributes( _bsParam, NULL );

		// Check the same device
		if( m_apCurrentDevice.IsEqualObject( m_mapDevices[ _nIndex ] ) )
		{
			// Same device
			ATLTRACE2(atlTraceGeneral, 8, _T("%s: DeviceSet(%ws) - SAME DEVICE\n"), 
				pT->GetClassName(), m_mapDevices.GetKeyAt( _nIndex ) );

			return S_FALSE;
		}

		// Check if device busy
		if( m_mapDevices[ _nIndex ]->Capture_IsBusy() )
		{
			// Device busy
			ATLTRACE2(atlTraceGeneral, 8, _T("WARNING !!! %s: DeviceSet(%ws) - DEVICE BUSY\n"), 
				pT->GetClassName(), m_mapDevices.GetKeyAt( _nIndex ) );

			return HRESULT_FROM_WIN32( ERROR_BUSY );
		}

		// Check current state
		eMState eState = m_apCurrentDevice ? m_apCurrentDevice->Capture_StateGet() : eMS_Closed;
		if( eState > eMS_Closed )
		{
			DeviceClose();
		}

		// Update current XML props and open device
		HRESULT hr = m_mapDevices[ _nIndex ]->Capture_Open( pT->MPropsXML() );
		if( FAILED( hr ) )
		{
			ATLTRACE2(atlTraceGeneral, 8, _T("###Err### %s: DeviceSet(%ws) - Capture_Open() FAILED hr=%08X\n"), 
				pT->GetClassName(), m_mapDevices.GetKeyAt( _nIndex ) );

			return hr;
		}

		// Reset audio props
		m_bAudioEnabled = true;
		m_apCurrentDevice = m_mapDevices[ _nIndex ];
		ATLASSERT( m_apCurrentDevice );

		ATLTRACE2(atlTraceGeneral, 8, _T("%s: DeviceSet(%ws) - Capture_Open() = %08X\n"), 
			pT->GetClassName(), m_mapDevices.GetKeyAt( _nIndex ) );

		// Start device
		if( eState > eMS_Stopped )
		{
			hr = DeviceOpen( NULL, NULL );
			if( FAILED( hr ) )
			{
				ATLTRACE2(atlTraceGeneral, 8, _T("###Err### %s: DeviceSet(%ws) - DeviceOpen() FAILED\n"), 
					pT->GetClassName(), m_mapDevices.GetKeyAt( _nIndex ) );

				return hr;
			}
		}

		return S_OK;
	}

	STDMETHOD(DeviceOpen)( /*[in]*/ BSTR _bsDeviceConfig, /*[in]*/ BSTR _bsPropsList )
	{
		T* pT = static_cast<T*>(this);

		CAutoCS crs(m_csDevices);

		MTRACE( "(%ws, %ws)", _bsDeviceConfig, _bsPropsList );

		XMLParse::XMLNodePtr xmlConfig( L"config", NULL );
		xmlConfig->ParseAttributes( _bsDeviceConfig, NULL, false, 1, true );

		CComBSTR cbsVideoName = xmlConfig->GetStringAttribute( L"video" );
		if( cbsVideoName.Length() )
		{
			int nDeviceIdx = m_mapDevices.FindKey( cbsVideoName );
			if( nDeviceIdx <= 0 )
			{
				ATLTRACE2(atlTraceGeneral, 8, _T("%s: DeviceOpen(%ws) - DEVICE '%ws' NOT FOUND\n"), 
					pT->GetClassName(), _bsDeviceConfig, cbsVideoName );

				return E_INVALIDARG;
			}

			HRESULT hr = DeviceSet( eMFDT_Video, nDeviceIdx, _bsPropsList );
			if( FAILED( hr ) )
			{
				ATLTRACE2(atlTraceGeneral, 8, _T("%s: DeviceOpen(%ws) - DeviceSet() FAILED hr=%08X\n"), 
					pT->GetClassName(), _bsDeviceConfig, cbsVideoName, hr );

				return hr;
			}
		}
		else
		{
			pT->MPropsXML()->ParseAttributes( _bsPropsList, NULL );
		}

		ATLASSERT( m_apCurrentDevice );
		if( !m_apCurrentDevice )
		{
			ATLTRACE2(atlTraceGeneral, 8, _T("###Err### %s: DeviceOpen(%ws) - m_apCurrentDevice = NULL\n"), 
				pT->GetClassName(), _bsDeviceConfig );

			return HRESULT_FROM_WIN32( ERROR_EMPTY );
		}

		// Audio
		m_bAudioEnabled = xmlConfig->GetBoolAttribute( L"audio_enabled", true );

		M_AV_PROPS avProps = pT->Format_Get(eMFT_Input);
		if( !m_bAudioEnabled )
			avProps.audProps.nChannels = -1; // Disable audio
		HRESULT hr = m_apCurrentDevice->Capture_Start( &avProps, pT, OnFrame_StaticCallback );
		if( FAILED( hr ) )
		{
			ATLTRACE2(atlTraceGeneral, 8, _T("###Err### %s: DeviceOpen(%ws) - m_apCurrentDevice(%ws)->Capture_Start() FAILED hr=%08X\n"), 
				pT->GetClassName(), _bsDeviceConfig, m_apCurrentDevice->Capture_Name(), hr );

			return HRESULT_FROM_WIN32( ERROR_EMPTY );
		}

		ATLTRACE2(atlTraceGeneral, 8, _T("%s: DeviceOpen(%ws) - m_apCurrentDevice(%ws)->Capture_Start()=%08X\n"), 
			pT->GetClassName(), _bsDeviceConfig, m_apCurrentDevice->Capture_Name(), hr );

		return S_OK; 
	}

	STDMETHOD(DeviceGet)( /*[in]*/ eMFDeviceType _eDevType, /*[out]*/ int* _pnIndex, /*[out]*/ BSTR* _pbsName )
	{
		T* pT = static_cast<T*>(this);

		// Find m_apCurrentDevice
		if( !m_apCurrentDevice )
		{
			if( _pnIndex )
				*_pnIndex = -1;
		}
		else 
		{
			if( _eDevType == eMFDT_All )
			{
				if( _pbsName )
				{
					// Return all config
					XMLParse::XMLNodePtr pXMLConfig( L"device", NULL );

					CComBSTR cbsVideoName;
					DeviceGet( eMFDT_Video, NULL, &cbsVideoName );
					pXMLConfig->SetAttribute( L"video", cbsVideoName );
					pXMLConfig->SetAttribute( L"audio_enabled", m_bAudioEnabled );

					pXMLConfig->SaveXML( _pbsName );
				}
			}
			else if( _eDevType == eMFDT_Audio )
			{
				*_pnIndex = m_bAudioEnabled ? 0 : 1;
				if( _pbsName )
					*_pbsName = CComBSTR( ppszMFDev_AudioNames[m_bAudioEnabled ? 0 : 1] ).Detach();
			}
			else
			{
				int nIdx = m_mapDevices.FindVal( m_apCurrentDevice );
				ATLASSERT( nIdx >= 0 );

				if( _pnIndex )
					*_pnIndex = nIdx;

				if( _pbsName )
					m_mapDevices.GetKeyAt(nIdx).CopyTo( _pbsName );
			}
		}

		return S_OK;
	}

	STDMETHOD(DeviceClose)( )
	{
		T* pT = static_cast<T*>(this);

		CAutoCS crs(m_csDevices);

		HRESULT hr = S_FALSE;
		if( m_apCurrentDevice )
		{
			hr = m_apCurrentDevice->Capture_Close();

			ATLTRACE2(atlTraceGeneral, 8, _T("%s: DeviceClose() - m_apCurrentDevice(%ws)->Capture_Close()=%08X\n"), 
				pT->GetClassName(), m_apCurrentDevice->Capture_Name(), hr );
		}

		return hr;
	}

	STDMETHOD(DeviceShowProps)( /*[in]*/ eMFDeviceType _eDevType, /*[in]*/ BSTR _bsPropsType, /*[in]*/ LONGLONG _lWndHandle )
	{
		return E_NOTIMPL;
	}

	STDMETHOD(DeviceGetInternal)( /*[in]*/ eMFDeviceType _eDevType, /*[out]*/ IUnknown** _ppObject )
	{
#ifdef MF_DEVICE_CEF_I
		if (m_apCurrentDevice)
			return m_apCurrentDevice->GetInternalInterface(NULL, _ppObject);
#endif // MF_DEVICE_CEF_I

		return E_NOTIMPL;
	}


private:
	//////////////////////////////////////////////////////////////////////////
	// Static callback

	static HRESULT OnFrame_StaticCallback( IN void* _pvInstance, IN IMFFrame* _pFrame, IN LONGLONG _llExtraParam, IN LPCWSTR _pszExtraParam )
	{
		T* pT = static_cast<T*>( _pvInstance );
		ATLASSERT( pT );

		return pT->OnFrame_Callback( _pFrame, _llExtraParam, _pszExtraParam );
	}

	HRESULT OnFrame_Callback( IN IMFFrame* _pFrame, IN LONGLONG _llExtraParam, IN LPCWSTR _pszExtraParam )
	{
		T* pT = static_cast<T*>( this );
		ATLASSERT( pT );

		if( !m_bAudioEnabled && _pFrame )
		{
			// Reset audio
			_pFrame->MFAudioSet( NULL, 0, 0 );
		}

		return pT->OnFrameReady( _pFrame, _llExtraParam, _pszExtraParam );
	}
};


template <class T, class TCapture>
class MFDeviceBase_Capture : 
	public IMFDeviceImpl_Capture<T, TCapture>,
	public IMSourceImpl<T>,
	public IMPropsImpl<T, IMFProps>,
	public IMFormatImpl<T, IMFFormat>
{
	friend class IMFormatImpl<T, IMFFormat>;
	friend class IMSourceImpl<T>;
	friend class IMPropsImpl<T, IMFProps>;

protected:

	MFDeviceBase_Capture()
	{
		// Disable notification if set the same attribute
		m_bIgnoreSameProps = true;
	}

	HRESULT MFDeviceBase_OnFinalConstuct( bool _bSetFirst = true )
	{
		int iCountDevices = TCapture::EnumDevicesXML( MPropsXML(), &m_mapDevices );
		if( iCountDevices <= 0 )
			return VFW_E_NO_CAPTURE_HARDWARE;

		T* pT = static_cast<T*>(this);

		// Set first 'non-busy' device
		if( _bSetFirst )
		{
			for( int i = 0; i < m_mapDevices.GetSize(); i++ )
			{
				HRESULT hr = pT->DeviceSet( eMFDT_Video, i, NULL );
				if( SUCCEEDED( hr ) )
					return hr;
			}
		}

		return S_FALSE;
	}

	void MFDeviceBase_OnFinalRelease()
	{
		T* pT = static_cast<T*>(this);
		pT->DeviceClose();
	}

public:

	// Override this method for error handling
	HRESULT OnError( IN HRESULT _hrErrorCode, IN LPCWSTR _pszDescritpion )
	{
		T* pT = static_cast<T*>(this);

		// TODO: Error handling
		ATLTRACE2( atlTraceGeneral, 2, _T("###Err### %s: OnFrameReady() - ERROR OCCURS hr=%08X, Info:%ws\n"),
			pT->GetClassName(), _hrErrorCode, _pszDescritpion );

		return S_OK;
	}

	// Override this method for custom frame handling
	HRESULT OnFrameReady( IN IMFFrame* _pFrame, IN LONGLONG _llExtraParam, IN LPCWSTR _pszExtraParam )
	{
		T* pT = static_cast<T*>(this);

		if( _pFrame )
		{
			// Update format 
			// Next line cause problem with wrong input format 
			// https://medialooks.myjetbrains.com/youtrack/issue/MP-276
			// NDI start with black frames (no video) with 59i and after to change into 
			// correct one, format not update
			// WARNING !!! Have to check other devices (e.g. AJA, etc.)

			// Reverted this fix #mp-731
			if( !IsSpecified( &pT->Format_Get(eMFT_Input).vidProps ) || STR_IS_SAME_I(_pszExtraParam, L"__Force_Update_FrameFormat_(NDI)__") )
			{
				M_AV_PROPS avPropsIn = {};
				_pFrame->MFAVPropsGet( &avPropsIn, NULL );
				pT->Format_UpdateAVProps( &avPropsIn, NULL );
			}

			// Put frame to source
			pT->Source_PutFrame( _pFrame );

			return S_OK;
		}

		return pT->OnError( (HRESULT)_llExtraParam, _pszExtraParam );
	}

	
protected:

	//////////////////////////////////////////////////////////////////////////
	// IMFormatImpl

	void Format_FillVideo(/*[in]*/ eMFormatType _eFormatType)
	{
		CAutoCS crs(m_csDevices);

		if( _eFormatType != eMFT_Input || 
			!m_apCurrentDevice || 
			!m_apCurrentDevice->Capture_EnumFormats( &m_arrVideoFormat[eMFT_Input] ) )
		{
			_IMFormatImpl::Format_FillVideo( _eFormatType );
		}
	}

	void Format_FillAudio(/*[in]*/ eMFormatType _eFormatType)
	{
		CAutoCS crs(m_csDevices);

		if( _eFormatType != eMFT_Input || 
			!m_apCurrentDevice || 
			!m_apCurrentDevice->Capture_EnumFormats( &m_arrAudioFormat[eMFT_Input] ) )
		{
			_IMFormatImpl::Format_FillAudio( _eFormatType );
		}
	}

	bool Format_IsSupported( eMFormatType _eType, M_VID_PROPS* _pVidProps )
	{
		if( _eType != eMFT_Input )
			return true;

		CAutoCS crs(m_csDevices);

		if( !m_apCurrentDevice )
			return true;

		return m_apCurrentDevice->Capture_IsFormatSupported( _pVidProps );
	}

	bool Format_IsSupported( eMFormatType _eType, M_AUD_PROPS* _pAudProps )
	{
		if( _eType != eMFT_Input )
			return true;

		CAutoCS crs(m_csDevices);

		if( !m_apCurrentDevice )
			return true;

		return m_apCurrentDevice->Capture_IsFormatSupported( _pAudProps );
	}

	bool Format_OnSet( eMFormatType _eFormatType, M_AUD_PROPS* _pAudProps )
	{
		if( _eFormatType != eMFT_Input )
			return true;

		T* pT = static_cast<T*>( this );

		CAutoCS crs(m_csDevices);

		if( m_apCurrentDevice && !m_apCurrentDevice->Capture_IsFormatSupported( _pAudProps ) )
		{
			return false;
		}

		return pT->OnChangeFormat();
	}

	bool Format_OnSet( eMFormatType _eFormatType, M_VID_PROPS* _pVidProps )
	{
		if( _eFormatType != eMFT_Input )
			return true;

		T* pT = static_cast<T*>( this );

		CAutoCS crs(m_csDevices);

		if( m_apCurrentDevice && !m_apCurrentDevice->Capture_IsFormatSupported( _pVidProps ) )
		{
			return false;
		}

		return pT->OnChangeFormat();
	}

	bool OnChangeFormat()
	{
		CAutoCS crs(m_csDevices);

		T* pT = static_cast<T*>(this);

		if( m_apCurrentDevice )
		{
			// Compare current & input (user-set) format
			M_AV_PROPS avPropsChange = pT->Format_Get(eMFT_Input);
			M_AV_PROPS avPropsCurrent = m_apCurrentDevice->Capture_AVPropsGet();
			if( IsSpecified( &avPropsChange ) && IsSameFormatAV( &avPropsChange, &avPropsCurrent ) < 0)
			{
				ATLTRACE2(atlTraceGeneral, 8, _T("%s: OnChangeFormat() - FORMAT CHANGED\n"), pT->GetClassName() );

				// Restart capture if format changed
				eMState eState = m_apCurrentDevice->Capture_StateGet();
				if( eState >= eMS_Stopped )
				{
					pT->DeviceClose();

					HRESULT hr = pT->DeviceOpen( NULL, NULL );
					if( FAILED( hr ) )
					{
						ATLTRACE2(atlTraceGeneral, 8, _T("###Err### %s: OnChangeFormat() - DeviceOpen() FAILED\n"), 
							pT->GetClassName() );

						return false;
					}
				}
			}
		}

		return true;
	}

	// IMPropsImpl
	HRESULT OnSetMProps( IN LPCWSTR _bsPropName, IN LPCWSTR _bsPropValue )
	{
		CAutoCS crs(m_csDevices);

		T* pT = static_cast<T*>( this );

		if( m_apCurrentDevice )
		{
			HRESULT hr = m_apCurrentDevice->Capture_OnPropsSet( _bsPropName, _bsPropValue );
			if( FAILED( hr ) )
			{
				ATLTRACE2(atlTraceGeneral, 8, _T("###Err### %s: OnSetMProps() - Capture_OnPropsSet(%ws,%ws) FAILED\n"), 
					pT->GetClassName(), _bsPropName, _bsPropValue );

				return hr;
			}
		}

		return S_OK;
	}


	// IMSourceImpl
	HRESULT Source_OnInit()
	{
		T* pT = static_cast<T*>( this );

		return pT->Source_OnFrameGet(NULL);
	};

	HRESULT Source_OnClose( int _nReceivers )
	{
		T* pT = static_cast<T*>( this );

		CAutoCS crs(m_csDevices);

		HRESULT hr = S_OK;
		if( _nReceivers == 0 )
		{
			return pT->DeviceClose();
		}

		return S_FALSE;
	};

	HRESULT Source_OnFrameGet( BSTR _bsPropsList )
	{
		T* pT = static_cast<T*>( this );

		CAutoCS crs(m_csDevices);

		if( !m_apCurrentDevice )
			return HRESULT_FROM_WIN32(ERROR_EMPTY);

		if( m_apCurrentDevice->Capture_StateGet() < eMS_Running )
		{
			HRESULT hr = DeviceOpen(NULL, _bsPropsList);
			if( FAILED( hr ) )
			{
				ATLTRACE2(atlTraceGeneral, 8, _T("###Err### %s: Source_OnFrameGet() - DeviceOpen() FAILED\n"), 
					pT->GetClassName() );

				return hr;
			}
		}

		return S_OK;
	}

private:
	//////////////////////////////////////////////////////////////////////////
	// Static callback

	static HRESULT OnFrame_StaticCallback( IN void* _pvInstance, IN IMFFrame* _pFrame, IN LONGLONG _llExtraParam, IN LPCWSTR _pszExtraParam )
	{
		T* pT = static_cast<T*>( _pvInstance );
		ATLASSERT( pT );
		return pT->OnFrameReady( _pFrame, _llExtraParam, _pszExtraParam );
	}
};




template <class T, class TRender>
class IMFDeviceImpl_Render: 
	public IMFDevice,
	public IMCallback
{
protected:

	typedef typename TRender::TPtr	TRenderPtr;
	typedef IMFDeviceImpl_Render<T,TRender> _IMFDeviceImpl;

protected:
	CComAutoCriticalSection		m_csDevices;

	// Current device
	TRenderPtr	m_apCurrentDevice;

	// Map of devices
	CSimpleMap2<CComBSTR,TRenderPtr> m_mapDevices;

public:
	//////////////////////////////////////////////////////////////////////////
	// IMCallback implementation
	// For renderer <- current_device
	STDMETHOD(OnEvent)(/*[in] */LONGLONG llCallbackUserData, /*[in] */BSTR bsName,
		/*[in] */BSTR bsEventName, /*[in] */BSTR bsEventParam, /*[in]*/IUnknown* pEventObject)
	{
		// Note: Use Sync events - for do not use threads/windows and make Events_Start()/Events_Stop()
		T* pT = static_cast<T*>(this);
		return pT->Events_OnEvent(bsName, bsEventName, bsEventParam, pT->GetUnknown(), true);
	}

	STDMETHOD(OnFrame)(/*[in]*/ LONGLONG llCallbackUserData, /*[in]*/ BSTR bsName, /*[in]*/ IUnknown* pMFrame)
	{
		// Note: Use Sync events - for do not use threads/windows and make Events_Start()/Events_Stop()
		T* pT = static_cast<T*>(this);
		return pT->Events_OnFrame(bsName, pMFrame, true, true);
	}

	//////////////////////////////////////////////////////////////////////////
	// IMFDevice impl

	STDMETHOD(DeviceGetCount)( /*[in]*/ eMFDeviceType _eDevType, /*[out]*/ int* _pnCount )
	{
		T* pT = static_cast<T*>(this);

		CAutoCS crs(m_csDevices);

		if( !_pnCount )
			return E_POINTER;

		*_pnCount = m_mapDevices.GetSize();

		return S_OK;
	}

	STDMETHOD(DeviceGetByIndex)( /*[in]*/ eMFDeviceType _eDevType, /*[in]*/ int _nIndex, /*[out]*/ BSTR* _pbsName, /*[out]*/ BOOL* _pbBusy )
	{
		T* pT = static_cast<T*>(this);

		CAutoCS crs(m_csDevices);

		if( _nIndex < 0 || _nIndex >= m_mapDevices.GetSize() )
			return HRESULT_FROM_WIN32( ERROR_INVALID_INDEX );

		if( _pbBusy )
			*_pbBusy = m_mapDevices[_nIndex].IsEqualObject( m_apCurrentDevice ) ? -1 : m_mapDevices[_nIndex]->Render_IsBusy();

		if( _pbsName )
			m_mapDevices.GetKeyAt(_nIndex).CopyTo( _pbsName );

		return S_OK;


		return E_INVALIDARG;
	}

	STDMETHOD(DeviceSet)( /*[in]*/ eMFDeviceType _eDevType, /*[in]*/ int  _nIndex, /*[in]*/ BSTR _bsParam )
	{
		T* pT = static_cast<T*>(this);

		CAutoCS crs(m_csDevices);

		if( _nIndex < 0  || _nIndex >= m_mapDevices.GetSize( ) )
			return HRESULT_FROM_WIN32( ERROR_INVALID_INDEX );

		// Update properties
		pT->MPropsXML()->ParseAttributes( _bsParam, NULL );

		// Check the same device
		if( m_apCurrentDevice.IsEqualObject( m_mapDevices[ _nIndex ] ) )
		{
			// Same device
			ATLTRACE2(atlTraceGeneral, 8, _T("%s: DeviceSet(%ws) - SAME DEVICE\n"), 
				pT->GetClassName(), m_mapDevices.GetKeyAt( _nIndex ) );

			return S_FALSE;
		}

		// Check if device busy
		if( m_mapDevices[ _nIndex ]->Render_IsBusy() )
		{
			// Device busy
			ATLTRACE2(atlTraceGeneral, 8, _T("WARNING !!! %s: DeviceSet(%ws) - DEVICE BUSY\n"), 
				pT->GetClassName(), m_mapDevices.GetKeyAt( _nIndex ) );
		}

		// Check current state
		eMState eState = m_apCurrentDevice ? m_apCurrentDevice->Render_StateGet() : eMS_Closed;
		if( eState > eMS_Closed )
		{
			DeviceClose();
		}

		// Update current XML props and open device
		HRESULT hr = m_mapDevices[ _nIndex ]->Render_Open( pT->MPropsXML() );
		if( FAILED( hr ) )
		{
			ATLTRACE2(atlTraceGeneral, 8, _T("###Err### %s: DeviceSet(%ws) - Render_Open() FAILED hr=%08X\n"), 
				pT->GetClassName(), m_mapDevices.GetKeyAt( _nIndex ) );

			return hr;
		}

		// Set current device
		m_apCurrentDevice = m_mapDevices[ _nIndex ];
		ATLASSERT( m_apCurrentDevice );

		// Set callback
		m_apCurrentDevice->MRenderBase_SetCallback(static_cast<IMCallback*>(this));
		ATLTRACE2(atlTraceGeneral, 8, _T("%s: DeviceSet(%ws) - Render_Open() = %08X\n"), 
			pT->GetClassName(), m_mapDevices.GetKeyAt( _nIndex ) );

		// Start device
		if( eState > eMS_Stopped )
		{
			hr = DeviceOpen( NULL, NULL );
			if( FAILED( hr ) )
			{
				ATLTRACE2(atlTraceGeneral, 8, _T("###Err### %s: DeviceSet(%ws) - DeviceOpen() FAILED\n"), 
					pT->GetClassName(), m_mapDevices.GetKeyAt( _nIndex ) );

				return hr;
			}
		}

		return S_OK;
	}

	STDMETHOD(DeviceOpen)( /*[in]*/ BSTR _bsDeviceConfig, /*[in]*/ BSTR _bsPropsList )
	{
		T* pT = static_cast<T*>(this);

		CAutoCS crs(m_csDevices);

		XMLParse::XMLNodePtr xmlConfig( L"config", NULL );
		xmlConfig->ParseAttributes( _bsDeviceConfig, NULL, false, 1, true );

		CComBSTR cbsVideoName = xmlConfig->GetStringAttribute( L"name" );
		if( cbsVideoName.Length() )
		{
			int nDeviceIdx = m_mapDevices.FindKey( cbsVideoName );
			if( nDeviceIdx <= 0 )
			{
				ATLTRACE2(atlTraceGeneral, 8, _T("%s: DeviceOpen(%ws) - DEVICE '%ws' NOT FOUND\n"), 
					pT->GetClassName(), _bsDeviceConfig, cbsVideoName );

				return E_INVALIDARG;
			}

			HRESULT hr = DeviceSet( eMFDT_Video, nDeviceIdx, _bsPropsList );
			if( FAILED( hr ) )
			{
				ATLTRACE2(atlTraceGeneral, 8, _T("%s: DeviceOpen(%ws) - DeviceSet() FAILED hr=%08X\n"), 
					pT->GetClassName(), _bsDeviceConfig, cbsVideoName, hr );

				return hr;
			}
		}
		else
		{
			pT->MPropsXML()->ParseAttributes( _bsPropsList, NULL );
		}

		ATLASSERT( m_apCurrentDevice );
		if( !m_apCurrentDevice )
		{
			ATLTRACE2(atlTraceGeneral, 8, _T("###Err### %s: DeviceOpen(%ws) - m_apCurrentDevice = NULL\n"), 
				pT->GetClassName(), _bsDeviceConfig );

			return HRESULT_FROM_WIN32( ERROR_EMPTY );
		}

		M_AV_PROPS avProps = pT->Format_Get(eMFT_Output);
		HRESULT hr = m_apCurrentDevice->Render_Start( &avProps );
		if( FAILED( hr ) )
		{
			ATLTRACE2(atlTraceGeneral, 8, _T("###Err### %s: DeviceOpen(%ws) - m_apCurrentDevice(%ws)->Render_Start() FAILED hr=%08X\n"), 
				pT->GetClassName(), _bsDeviceConfig, m_apCurrentDevice->Render_Name(), hr );

			return HRESULT_FROM_WIN32( ERROR_EMPTY );
		}

		ATLTRACE2(atlTraceGeneral, 8, _T("%s: DeviceOpen(%ws) - m_apCurrentDevice(%ws)->Render_Start()=%08X\n"), 
			pT->GetClassName(), _bsDeviceConfig, m_apCurrentDevice->Render_Name(), hr );

		return S_OK; 
	}

	STDMETHOD(DeviceGet)( /*[in]*/ eMFDeviceType _eDevType, /*[out]*/ int* _pnIndex, /*[out]*/ BSTR* _pbsName )
	{
		T* pT = static_cast<T*>(this);

		// Find m_apCurrentDevice
		if( !m_apCurrentDevice )
		{
			if( _pnIndex )
				*_pnIndex = -1;
		}
		else
		{
			int nIdx = m_mapDevices.FindVal( m_apCurrentDevice );
			ATLASSERT( nIdx >= 0 );

			if( _pnIndex )
				*_pnIndex = nIdx;

			if( _pbsName )
				m_mapDevices.GetKeyAt(nIdx).CopyTo( _pbsName );
		}

		return S_OK;
	}

	STDMETHOD(DeviceClose)( )
	{
		T* pT = static_cast<T*>(this);

		CAutoCS crs(m_csDevices);

		HRESULT hr = S_FALSE;
		if( m_apCurrentDevice )
		{
			hr = m_apCurrentDevice->Render_Close();

			ATLTRACE2(atlTraceGeneral, 8, _T("%s: DeviceClose() - m_apCurrentDevice(%ws)->Render_Close()=%08X\n"), 
				pT->GetClassName(), m_apCurrentDevice->Render_Name(), hr );
		}

		return hr;
	}

	STDMETHOD(DeviceShowProps)( /*[in]*/ eMFDeviceType _eDevType, /*[in]*/ BSTR _bsPropsType, /*[in]*/ LONGLONG _lWndHandle )
	{
		return E_NOTIMPL;
	}

	STDMETHOD(DeviceGetInternal)( /*[in]*/ eMFDeviceType _eDevType, /*[out]*/ IUnknown** _ppObject )
	{
		return E_NOTIMPL;
	}
};



template <class T, class TRender>
class MFDeviceBase_Render : 
	public IMFDeviceImpl_Render<T, TRender>,
	public IMPropsImpl<T, IMFProps>,
	public IMFormatImpl<T, IMFFormat>,
	public IMFReceiver,	
	public CDeqActive<T, pair<CComPtr<IMFFrame>,CComBSTR > >
{
	friend class IMFormatImpl<T, IMFFormat>;
	friend class IMPropsImpl<T, IMFProps>;

	// MFConverter
	CComPtr<IMFConverter>	m_qpMFConverter;
	
protected:

	typedef MFDeviceBase_Render<T, TRender> _MFDeviceBase;

	HRESULT MFDeviceBase_OnFinalConstuct()
	{
		int iCountDevices = TRender::EnumDevicesXML( MPropsXML(), &m_mapDevices );
		if( iCountDevices <= 0 )
			return VFW_E_NO_CAPTURE_HARDWARE;

		T* pT = static_cast<T*>(this);

		// Set first 'non-busy' device
		for( int i = 0; i < m_mapDevices.GetSize(); i++ )
		{
			HRESULT hr = pT->DeviceSet( eMFDT_Video, 0, NULL );
			if( SUCCEEDED( hr ) )
				return hr;
		}

		return S_FALSE;
	}

	void MFDeviceBase_OnFinalRelease()
	{
		T* pT = static_cast<T*>(this);
		pT->DeviceClose();
	}

public:

	MFDeviceBase_Render() 
	{
		MPropsXML()->SetAttribute( L"input.buffers", 1 );
		
		// Disable notification if set the same attribute
		m_bIgnoreSameProps = true;
	}


	// IMFReceiever
	STDMETHOD(ReceiverFramePut)( /*[in]*/ IMFFrame* _pFrame, /*[in]*/ REFERENCE_TIME _rtMaxWait, /*[in]*/ BSTR _bsProps )
	{
		if( !_pFrame )
			return E_INVALIDARG;

		T* pT = static_cast<T*>( this );

		M_TIME mTime = MFFrame_Time(_pFrame);
		M_AV_PROPS avPropsIn = MFFrame_AVProps(_pFrame);
		M_AV_PROPS avPropsOut = pT->Format_Get(eMFT_Convert);

		// Check for supported formats
		TRenderPtr pDevice = m_apCurrentDevice;
		if( pDevice )
		{
			if( !IsSpecified( &avPropsOut.vidProps ) )
				pDevice->Render_IsFormatSupported( &avPropsIn.vidProps, &avPropsOut.vidProps);

			if( !IsSpecified( &avPropsOut.audProps ) )
				pDevice->Render_IsFormatSupported( &avPropsIn.audProps, &avPropsOut.audProps);
		}

		CSimpleArray2<CComPtr<IMFFrame> > arrConverted;

		// Convert format
		HRESULT hr = Format_Convert( _pFrame, &avPropsOut, &arrConverted, true );
		if ( hr != S_OK )
		{	
			ATLASSERT( arrConverted.GetSize() == 0 );
			arrConverted.Add( _pFrame );

			hr = S_OK;
		}

		DeqSetMax( MPropsXML()->GetIntAttribute( L"input.buffers" ), false );

		for(int i = 0; i < arrConverted.GetSize(); i++)
		{
			if( DeqMaxFrames() > 0 )
			{
				int nRes = DeqPutRT( make_pair( arrConverted[i], _bsProps), IS_LIVE( &mTime ) ? (arrConverted.GetSize() > 1 ? 10 * DS_MSEC : 0) : _rtMaxWait );
				if( nRes < 0 )
				{
					ATLTRACE2(atlTraceGeneral, 8, _T("%s::MediaReceive() - BUFFER OVERLOW (Size:%d)\n"), pT->GetClassName(), DeqSize() );

					MPropsXML(L"stat")->IncrementAttribute( L"buffer_overlow", 1, true );

					return S_FALSE;
				}
			}
			else
			{
				hr = _ReceiverFramePut( arrConverted[i], _rtMaxWait, _bsProps );
				if( FAILED( hr ) )
				{
					ATLTRACE2(atlTraceGeneral, 8, _T("###Err### %s::MediaReceive() - _ReceiverFramePut(%ws) FAILED hr=%08X\n"), 
						pT->GetClassName(), MFRAME2STR( arrConverted[i] ) );

					return hr;
				}
			}
		}					

		return hr;
	}

	STDMETHOD(DeviceClose)()
	{
		DeqFree(true);

		return _IMFDeviceImpl::DeviceClose();
	}

	HRESULT OnActiveDeq( const pair<CComPtr<IMFFrame>,CComBSTR >& _object, int _nDeqSize )
	{
		return _ReceiverFramePut( _object.first, -1, _object.second );
	}

	//////////////////////////////////////////////////////////////////////////
	// IMFormatImpl
	
	void Format_FillVideo(/*[in]*/ eMFormatType _eFormatType)
	{
		CAutoCS crs(m_csDevices);

		if( _eFormatType == eMFT_Input )
			return;

		if( !m_apCurrentDevice || 
			!m_apCurrentDevice->Render_EnumFormats( &m_arrVideoFormat[_eFormatType] ) )
		{
			_IMFormatImpl::Format_FillVideo( _eFormatType );
		}
	}

	void Format_FillAudio(/*[in]*/ eMFormatType _eFormatType)
	{
		CAutoCS crs(m_csDevices);

		if( _eFormatType == eMFT_Input )
			return;

		if( !m_apCurrentDevice || 
			!m_apCurrentDevice->Render_EnumFormats( &m_arrAudioFormat[_eFormatType] ) )
		{
			_IMFormatImpl::Format_FillAudio( _eFormatType );
		}
	}

	bool Format_IsSupported( eMFormatType _eType, M_VID_PROPS* _pVidProps )
	{
		if( _eType == eMFT_Input )
			return false;

		CAutoCS crs(m_csDevices);

		if( !m_apCurrentDevice )
			return true;

		return m_apCurrentDevice->Render_IsFormatSupported( _pVidProps );
	}

	bool Format_IsSupported( eMFormatType _eType, M_AUD_PROPS* _pAudProps )
	{
		if( _eType == eMFT_Input )
			return false;

		CAutoCS crs(m_csDevices);

		if( !m_apCurrentDevice )
			return true;

		return m_apCurrentDevice->Render_IsFormatSupported( _pAudProps );
	}

	bool Format_OnSet( eMFormatType _eFormatType, M_AUD_PROPS* _pAudProps )
	{
		if( _eFormatType == eMFT_Input )
			return false;

		T* pT = static_cast<T*>( this );

		CAutoCS crs(m_csDevices);

		if( m_apCurrentDevice && !m_apCurrentDevice->Render_IsFormatSupported( _pAudProps ) )
		{
			return false;
		}

		return pT->OnChangeFormat();
	}

	bool Format_OnSet( eMFormatType _eFormatType, M_VID_PROPS* _pVidProps )
	{
		if( _eFormatType == eMFT_Input )
			return false;

		T* pT = static_cast<T*>( this );

		CAutoCS crs(m_csDevices);

		if( m_apCurrentDevice && !m_apCurrentDevice->Render_IsFormatSupported( _pVidProps ) )
		{
			return false;
		}

		return pT->OnChangeFormat();
	}

protected:

	bool OnChangeFormat()
	{
		CAutoCS crs(m_csDevices);

		T* pT = static_cast<T*>(this);

		if( m_apCurrentDevice )
		{
			// Compare current & input (user-set) format
			M_AV_PROPS avPropsChange = pT->Format_Get(eMFT_Convert);
			M_AV_PROPS avPropsCurrent = m_apCurrentDevice->Render_AVPropsGet();
			if( IsSpecified( &avPropsChange ) && IsSameFormatAV( &avPropsChange, &avPropsCurrent ) <= 0 )
			{
				// Restart capture if format changed
				eMState eState = m_apCurrentDevice->Render_StateGet();
				if( eState >= eMS_Stopped )
				{
					pT->DeviceClose();

					HRESULT hr = pT->DeviceOpen( NULL, NULL );
					if( FAILED( hr ) )
					{
						ATLTRACE2(atlTraceGeneral, 8, _T("###Err### %s: OnChangeFormat() - DeviceOpen() FAILED\n"), 
							pT->GetClassName() );

						return false;
					}
				}
			}
		}

		return true;
	}

	// IMPropsImpl
	HRESULT OnSetMProps( IN LPCWSTR _bsPropName, IN LPCWSTR _bsPropValue )
	{
		CAutoCS crs(m_csDevices);

		T* pT = static_cast<T*>( this );

		if( m_apCurrentDevice )
		{
			CComBSTR cbsTest = MPropsXML()->GetStringAttribute( _bsPropName );

			HRESULT hr = m_apCurrentDevice->Render_OnPropsSet( _bsPropName, _bsPropValue );
			if( FAILED( hr ) )
			{
				ATLTRACE2(atlTraceGeneral, 8, _T("###Err### %s: OnSetMProps() - Render_OnPropsSet(%ws,%ws) FAILED\n"), 
					pT->GetClassName(), _bsPropName, _bsPropValue );

				return hr;
			}
		}

		return S_OK;
	}



	

	HRESULT _ReceiverFramePut( /*[in]*/ IMFFrame* _pFrame, /*[in]*/ REFERENCE_TIME _rtMaxWait, /*[in]*/ BSTR _bsProps )
	{
		// TODO: ActiveDeq

		TRenderPtr pRender;
		{
			CAutoCS crs(m_csDevices );

			pRender = m_apCurrentDevice;

			ATLASSERT( pRender );
			if( !pRender )
				return E_UNEXPECTED;
		}

		MFFrame_WaitAsync( _pFrame );

		XMLParse::XMLNodePtr xmlProps( L"props", NULL );
		xmlProps->ParseAttributes( _bsProps, NULL );

		return pRender->Render_Frame( _pFrame, xmlProps );
	}
};

