//---------------------------------------------------------------------------
// This file is part of the Medialooks Media Platform (MMP)
//
// MObjectBase.h 
//
//---------------------------------------------------------------------------
// Copyright (c) 2011, Medialooks Ltd.
// www.medialooks.com (dev@medialooks.com)
//
// Author:  Vsevolod Burkutsky (VVB)
// Version: 1.0 (2011.12)
//---------------------------------------------------------------------------
// $Id$
//---------------------------------------------------------------------------

#pragma once

#include "MDefines.h"
#include "../Base/MHelpers.h"
#include "../Base/MCreator.h"


template <class T, class TInterface = IMFormat>
class ATL_NO_VTABLE IMFormatImpl:
	public TInterface
{
	// Used for mfreader.convert_format=true
	friend class CCoMFile;
protected:
	CRWSynchroLite				m_rwFormat;
	CSimpleArray2<M_VID_PROPS>	m_arrVideoFormat[eMFT_Output+1];
	CSimpleArray2<M_AUD_PROPS>	m_arrAudioFormat[eMFT_Output+1];

private:

	M_AV_PROPS					m_avFormatIn;
	M_AV_PROPS					m_avFormatCnv;
	M_VID_PROPS					m_vidFormatOverride;
	

	// Only for return reference
	M_AV_PROPS					m_avFormatOut;

    // Format conversion support
	CComAutoCriticalSection		m_csConvert;
	CComPtr<IMFConverter>		m_cpMFConverter;	// Frame converter		

	// Renderer mode flag
	// If set, user can set the output format
	bool						m_bRenderer;
public:
	

	typedef IMFormatImpl<T,TInterface> _IMFormatImpl;
public:

	IMFormatImpl( bool _bUseTLS = true )
		: m_rwFormat( RWLITE_CS_SPIN, _bUseTLS ),
		  m_bRenderer( false )
	{
		::ZeroMemory( &m_avFormatIn, sizeof(m_avFormatIn) );
		::ZeroMemory( &m_avFormatCnv, sizeof(m_avFormatCnv) );
		::ZeroMemory( &m_avFormatOut, sizeof(m_avFormatOut) );
		::ZeroMemory( &m_vidFormatOverride, sizeof(m_vidFormatOverride) );
	}

	static LPCWSTR FormatType2Str(eMFormatType _eFormatType)
	{
		switch (_eFormatType)
		{
		case eMFT_Input:
			return L"eMFT_Input";
		case eMFT_Convert:
			return L"eMFT_Input";
		case eMFT_Override:
			return L"eMFT_Input";
		case eMFT_Output:
			return L"eMFT_Input";
		}

		return L"###eMFT_Wrong###";
	}

	HRESULT GetRedirector_Format( eMFormatType _eFormatType, IMFormat** _ppMFormat )
	{
		return S_FALSE;
	}

	HRESULT GetRedirector_Format( eMFormatType _eFormatType, IMFFormat** _ppMFormat )
	{
		return S_FALSE;
	}

	void Format_SetMode( bool _bRenderer )
	{
		m_bRenderer = _bRenderer;
	}

	bool Format_IsRenderer() const { return m_bRenderer; }

	bool Format_IsSupported( eMFormatType _eFormatType, M_VID_PROPS* _pVidProps )
	{
		// TODO: Supported formats
		return true;
	}

	bool Format_IsSupported( eMFormatType _eFormatType, M_AUD_PROPS* _pAudProps )
	{
		// TODO: Supported formats
		return true;
	}

	bool Format_OnSet( eMFormatType _eFormatType, M_AUD_PROPS* _pAudProps )
	{
		// TODO: Supported formats
		return true;
	}

	bool Format_OnSet( eMFormatType _eFormatType, M_VID_PROPS* _pVidProps )
	{
		// TODO: Supported formats
		return true;
	}

	void Format_ConvertReset()
	{
		CAutoCS crs(m_csConvert);

		T* pT = static_cast<T*>(this);

		ATLTRACE2( atlTraceGeneral, 2, _T("%s::Format_ConvertReset(Reset:%08X)\n"), pT->GetClassName(), m_cpMFConverter.p );

		m_cpMFConverter.Release();
	}

	template <class TFrameInterface>
	HRESULT Format_Convert( IUnknown* _pFrameSrc, M_AV_PROPS* _pAVPropsOut, CSimpleArray2<CComPtr<TFrameInterface> >* _pArrConverted, bool _bAsync, double _dblRate = 0.0, bool _bForceConversion = false)
	{
		deque<CComPtr<TFrameInterface> > deqConverted;
		HRESULT hr = Format_Convert( _pFrameSrc, _pAVPropsOut, deqConverted, _bAsync, _dblRate );
		if( FAILED( hr ) )
			return hr;

		while( !deqConverted.empty() )
		{
			_pArrConverted->Add( deqConverted.front() );
			deqConverted.pop_front();
		}

		return hr;
	}

	template <class TFrameInterface>
	HRESULT Format_Convert( IUnknown* _pFrameSrc, M_AV_PROPS* _pAVPropsOut, deque<CComPtr<TFrameInterface> >& _rDeqConverted, bool _bAsync, double _dblRate = 0.0, bool _bForceConversion = false)
	{
		CComQIPtr<IMFFrame> qpFrameSrc = _pFrameSrc;
		if (!qpFrameSrc )
			return E_NOINTERFACE;		

		CBasicPerfD perf(L"Format_Convert(Perf)");

		T* pT = static_cast<T*>(this); 

		CAutoCS crs(m_csConvert);

		M_AV_PROPS avPropsIn = MFFrame_AVProps( _pFrameSrc );
		M_AV_PROPS avPropsCnv = _pAVPropsOut ? *_pAVPropsOut : m_avFormatCnv;

		// For GPU we make conversion if color_matrix specified 
		// TODO: Add color_matrix support into MFConverter
		if (!MFFrame_IsGPU(_pFrameSrc) && GPU_PipelineMode() > 0 && pT->MObjectXML()->HaveAttribute(L"gpu.rgb_transform_matrix", true))
			_bForceConversion = true;

		if( !_bForceConversion && !IsPartiallySpecified( &avPropsCnv.vidProps ) && !IsSpecified( &avPropsCnv.audProps) )
		{
			MTRACE("NO CONVERSION (RELEASE CONVERTER)");

			m_cpMFConverter.Release();

			return S_FALSE;
		}

		// Check - same AVProps, empty AVProps, same vidProps (diff audProps), same vidProps
		M_AV_PROPS avPropsOut = {};
		if (IsSpecified(&avPropsCnv.vidProps) || IsPartiallySpecified(&avPropsCnv.vidProps))
		{
			avPropsOut.vidProps = avPropsCnv.vidProps;

			// Special fix for 'format.set_default_pixelformat'=false
			if (avPropsOut.vidProps.eVideoFormat != eMVF_Custom)
			{
				UpdateMVidProps(&avPropsOut.vidProps);
				if (avPropsCnv.vidProps.fccType == eMFCC_Default)
				{
					avPropsOut.vidProps.fccType = avPropsIn.vidProps.fccType;
					avPropsOut.vidProps.nRowBytes = 0;
					UpdateMVidProps(&avPropsOut.vidProps);
					if( IsRGB(avPropsOut.vidProps.fccType) && !IsBottomTop(&avPropsIn.vidProps))
						avPropsOut.vidProps.nHeight = -1 * ABS(avPropsOut.vidProps.nHeight);
					else 
						avPropsOut.vidProps.nHeight = ABS(avPropsOut.vidProps.nHeight);
				}
			}
			else
			{
				avPropsOut.vidProps = MVidPropsMerge(&avPropsCnv.vidProps, &avPropsIn.vidProps);
			}
		}
		else
		{
			avPropsOut.vidProps = avPropsIn.vidProps;
		}

		if( IsSpecified( &avPropsCnv.audProps) )
			avPropsOut.audProps = avPropsCnv.audProps;
		else if (IsPartiallySpecified(&avPropsCnv.vidProps))
			avPropsOut.audProps = MAudPropsMerge(&avPropsCnv.audProps, &avPropsIn.audProps);
		else
			avPropsOut.audProps = avPropsIn.audProps;

		if( !_bForceConversion && IsSameFormatAV( &avPropsOut, &avPropsIn) > 0 )
		{
			MTRACE( "SAME FORMAT[%ws %ws] (RELEASE CONVERTER):%08X", 
				MVID2STR_S(&avPropsIn.vidProps), MAUD2STR_S(&avPropsIn.audProps), (ULONG)m_cpMFConverter.p);

			m_cpMFConverter.Release();

			return S_FALSE;
		}
		
		if( !m_cpMFConverter )
		{
			HRESULT hr = m_cpMFConverter.CoCreateInstance( __uuidof( MFConverter ) );
			ATLASSERT( SUCCEEDED( hr ) );
			if ( FAILED(hr) )
				return hr;

			MTRACE("CREATE CONVERTER:%08X", (ULONG)m_cpMFConverter.p );
		}

		LPCWSTR pszScalingQ = pT->MPropsXML()->HaveAttribute( L"scaling_quality" ) ?
			pT->MPropsXML()->GetStringAttribute( L"scaling_quality" ) : pT->MPropsXML()->GetStringAttribute( L"object::scaling_quality", L"auto" );

		WCHAR szCnvProps[256] = {};
		_snwprintf(szCnvProps, SIZEOF_ARRAY(szCnvProps), L"async='%ws' scaling_quality='%ws' rate='%f' force_conversion='%ws'", 
			_bAsync ? L"true" : L"false", pszScalingQ, _dblRate != 0.0 ? _dblRate : 1.0, _bForceConversion ? L"true" : L"false" );
		
		MTRACE("[%ws %ws]->[%ws %ws] Props:%ws IN:%ws", 
			MVID2STR_S(&avPropsIn.vidProps), MAUD2STR_S(&avPropsIn.audProps),
			MVID2STR_S(&avPropsOut.vidProps), MAUD2STR_S(&avPropsOut.audProps), szCnvProps,
			MFRAME2STR( qpFrameSrc ) );

		int nFrameRest = 1;
		while ( nFrameRest > 0 )
		{
			CComPtr<IMFFrame> cpMFFrameOut;
			HRESULT hr = m_cpMFConverter->ConverterFrameConvert( CComBSTR(L"imformat_cnv"), qpFrameSrc, &avPropsOut, &cpMFFrameOut, &nFrameRest, CComBSTR( szCnvProps ) );
			if( FAILED( hr ) )
			{
				MTRACE("[%ws %ws]->[%ws %ws] Rest:%d - ConverterFrameConvert() FAILED hr=%08X",
					MVID2STR_S(&avPropsIn.vidProps), MAUD2STR_S(&avPropsIn.audProps),
					MVID2STR_S(&avPropsOut.vidProps), MAUD2STR_S(&avPropsOut.audProps), nFrameRest, hr );

				return hr;
			}

			MTRACE("[%ws %ws]->[%ws %ws] Rest:%d OUT:%ws", 
				MVID2STR_S(&avPropsIn.vidProps), MAUD2STR_S(&avPropsIn.audProps),
				MVID2STR_S(&avPropsOut.vidProps), MAUD2STR_S(&avPropsOut.audProps), nFrameRest, MFRAME2STR( cpMFFrameOut ) );
						
			if( cpMFFrameOut )				
			{
				CComQIPtr<TFrameInterface> qpFrameOut = cpMFFrameOut;
				ATLASSERT( qpFrameOut );
				_rDeqConverted.push_back( qpFrameOut );
			}	

			// For use already converted frame in next cycle
			qpFrameSrc.Release();
		} 
		
		return S_OK;
	}

	void Format_SetDefualt(eMFormatType _eFormatType)
	{
		ATLASSERT( (int)_eFormatType >= 0 && (int)_eFormatType < SIZEOF_ARRAY( m_arrVideoFormat ) );
		if( (int)_eFormatType < 0 || (int)_eFormatType >= SIZEOF_ARRAY( m_arrVideoFormat )) return;

		T* pT = static_cast<T*>(this); 

		pT->Format_FillVideo( _eFormatType );
		if( m_arrVideoFormat[_eFormatType].GetSize() )
			Format_GetVideo(_eFormatType) = m_arrVideoFormat[_eFormatType][0];

		pT->Format_FillAudio( _eFormatType );
		if( m_arrAudioFormat[_eFormatType].GetSize() )
			Format_GetAudio(_eFormatType) = m_arrAudioFormat[_eFormatType][0];
	}

	void Format_FillVideo(/*[in]*/ eMFormatType _eFormatType)
	{
		ATLASSERT( (int)_eFormatType >= 0 && (int)_eFormatType < SIZEOF_ARRAY( m_arrVideoFormat ) );
		if( (int)_eFormatType < 0 || (int)_eFormatType >= SIZEOF_ARRAY( m_arrVideoFormat )) return;

		CAutoRWL rw(m_rwFormat,true);
		m_arrVideoFormat[_eFormatType].RemoveAll();

		T* pT = static_cast<T*>(this); 
		
		for( int i = 0; i < eMVF_Disabled; i++ )
		{
			M_VID_PROPS vidProps = {};
			vidProps.eVideoFormat = (eMVideoFormat)i;
			if( !VF_IsCorrect( vidProps.eVideoFormat ) )
				continue;

			UpdateMVidProps( &vidProps );
			if( pT->Format_IsSupported(_eFormatType, &vidProps) )
				m_arrVideoFormat[_eFormatType].Add( vidProps );
		}

		M_VID_PROPS vidProps = {eMVF_Disabled};
		if( pT->Format_IsSupported(_eFormatType, &vidProps) )
			m_arrVideoFormat[_eFormatType].Add( vidProps );
	}

	int Format_FindVideo( eMFormatType _eFormatType, M_VID_PROPS* _pVidProps, bool _bAddToList, bool _bFindClosestFormat = false )
	{
		ATLASSERT( (int)_eFormatType >= 0 && (int)_eFormatType < SIZEOF_ARRAY( m_arrVideoFormat ) );
		if( (int)_eFormatType < 0 || (int)_eFormatType >= SIZEOF_ARRAY( m_arrVideoFormat )) return -1;

		ATLASSERT( _pVidProps );
		if( !_pVidProps ) return 0;

		T* pT = static_cast<T*>(this); 

		bool bAdd = _bAddToList;
		{
			CAutoRWL rw(m_rwFormat,true);

			if( !m_arrVideoFormat[_eFormatType].GetSize() )
				pT->Format_FillVideo(_eFormatType);

			for( int i = 0; i < m_arrVideoFormat[_eFormatType].GetSize(); i++ )
			{
				if( IsSameFormat( &m_arrVideoFormat[_eFormatType][i], _pVidProps ) )
				{
					return i;
				}
			}

			if (_bFindClosestFormat)
			{
				M_VID_PROPS vidProps = *_pVidProps;
				UpdateMVidProps(&vidProps);

				
				// Find format with same size 
				// Map (rate distance->video format)
				std::map<double, int> mapSameSize;
				for (int i = 0; i < m_arrVideoFormat[_eFormatType].GetSize(); ++i)
				{
					M_VID_PROPS* pVidPropsCheck = &m_arrVideoFormat[_eFormatType][i];

					if (IsSameFormat(pVidPropsCheck, &vidProps, 1))
					{
						// Basic check - the nearest rate
						double dblRateDiff = ABS(pVidPropsCheck->dblRate - vidProps.dblRate);

						// Take interlace into account, the fact what 50p->25p better than 50p->30p conversion
						// - the logic is below (not good, make better one)
						if (IS_RATE(pVidPropsCheck->dblRate, vidProps.dblRate)) // e.g. 25p -> 25i , 25p -> 25i
							dblRateDiff = (IsInterlaced(pVidPropsCheck) == IsInterlaced(&vidProps)) ? 0.0 : 0.7;
						else if (ABS(pVidPropsCheck->dblRate - vidProps.dblRate) < 0.06) // e.g. 30p -> 29.97p 
							dblRateDiff = (IsInterlaced(pVidPropsCheck) == IsInterlaced(&vidProps)) ? 0.5 : 1.5;
						else if (IS_RATE(pVidPropsCheck->dblRate, vidProps.dblRate / 2))
							dblRateDiff = (IsInterlaced(pVidPropsCheck) == IsInterlaced(&vidProps)) ? 2.0 : 1.3;
						else if (ABS(pVidPropsCheck->dblRate - vidProps.dblRate / 2) < 0.06)
							dblRateDiff = (IsInterlaced(pVidPropsCheck) == IsInterlaced(&vidProps)) ? 2.2 : 1.4;
						
						mapSameSize.insert(make_pair(dblRateDiff, i));
					}
				}

				if (mapSameSize.empty())
					return -1;

				return mapSameSize.begin()->second;
			}
		}

		if( bAdd )
		{
			CAutoRWL rw(m_rwFormat,true);

			m_arrVideoFormat[_eFormatType].Add( *_pVidProps );
			return m_arrVideoFormat[_eFormatType].GetSize() - 1;
		}

		return -1;
	}

	int Format_FindAudio( eMFormatType _eFormatType, M_AUD_PROPS* _pAudProps, bool _bAddToList )
	{
		ATLASSERT( (int)_eFormatType >= 0 && (int)_eFormatType < SIZEOF_ARRAY( m_arrVideoFormat ) );
		if( (int)_eFormatType < 0 || (int)_eFormatType >= SIZEOF_ARRAY( m_arrVideoFormat )) return -1;

		ATLASSERT( _pAudProps );
		if( !_pAudProps ) return 0;

		T* pT = static_cast<T*>(this); 

		bool bAdd = _bAddToList;
		{
			CAutoRWL rw(m_rwFormat,true);

			if( !m_arrAudioFormat[_eFormatType].GetSize() )
				pT->Format_FillAudio(_eFormatType);

			for( int i = 0; i < m_arrAudioFormat[_eFormatType].GetSize(); i++ )
			{
				if( IsSameFormat( &m_arrAudioFormat[_eFormatType][i], _pAudProps ) )
				{
					return i;
				}
			}
		}

		if( bAdd )
		{
			CAutoRWL rw(m_rwFormat,true);

			m_arrAudioFormat[_eFormatType].Add( *_pAudProps );
			return m_arrAudioFormat[_eFormatType].GetSize() - 1;
		}

		return -1;
	}

	void Format_FillAudio(/*[in]*/ eMFormatType _eFormatType)
	{
		ATLASSERT( (int)_eFormatType >= 0 && (int)_eFormatType < SIZEOF_ARRAY( m_arrAudioFormat ) );
		if( (int)_eFormatType < 0 || (int)_eFormatType >= SIZEOF_ARRAY( m_arrAudioFormat )) return;

		CAutoRWL rw(m_rwFormat,true);
		m_arrAudioFormat[_eFormatType].RemoveAll();

		T* pT = static_cast<T*>(this); 

		M_AUD_PROPS audProps ={};
		if( pT->Format_IsSupported(_eFormatType, &audProps) )
			m_arrAudioFormat[_eFormatType].Add( audProps );

		for( int bt = 0; bt < SIZEOF_ARRAY(eMAudio_Bits); bt++ )
		{
			for( int f = 0; f < SIZEOF_ARRAY(eMAudio_Freq); f++ )
			{
				for( int ch = 0; ch < SIZEOF_ARRAY(eMAudio_Channels); ch++ )
				{
					audProps.nChannels = eMAudio_Channels[ch];
					audProps.nSamplesPerSec = eMAudio_Freq[f];
					audProps.nBitsPerSample = eMAudio_Bits[bt];

					if( pT->Format_IsSupported(_eFormatType, &audProps) )
						m_arrAudioFormat[_eFormatType].Add( audProps );
				}
			}
		}

		audProps.nChannels = -1;
		audProps.nBitsPerSample = 0;
		audProps.nSamplesPerSec = 0;
		if( pT->Format_IsSupported(_eFormatType, &audProps) )
			m_arrAudioFormat[_eFormatType].Add( audProps );
	}

		
	void Format_Reset()
	{
		CAutoRWL rw(m_rwFormat,true);
		for( int i = 0; i < SIZEOF_ARRAY(m_arrVideoFormat); i++ )
		{
			m_arrVideoFormat[i].RemoveAll();
			m_arrAudioFormat[i].RemoveAll();
		}
	}

	M_VID_PROPS& Format_GetVideo( eMFormatType _eType )
	{
		T* pT = static_cast<T*>(this);

		switch( _eType )
		{
		case eMFT_Convert:
			return m_avFormatCnv.vidProps;
		case eMFT_Override:
			return m_vidFormatOverride;
		case eMFT_Output:
			{
				
				pT->Format_UpdateAVProps( NULL, &m_avFormatOut);
				return m_avFormatOut.vidProps;
			}
		}

		return m_avFormatIn.vidProps;
	}

	M_AV_PROPS& Format_Get( eMFormatType _eType )
	{
		T* pT = static_cast<T*>(this);

		switch( _eType )
		{
		case eMFT_Convert:
			return m_avFormatCnv;
		case eMFT_Output:
			{
				pT->Format_UpdateAVProps( NULL, &m_avFormatOut);
				return m_avFormatOut;
			}
		}

		return m_avFormatIn;
	}

	M_AUD_PROPS& Format_GetAudio( eMFormatType _eType )
	{
		T* pT = static_cast<T*>(this);

		switch( _eType )
		{
		case eMFT_Convert:
			return m_avFormatCnv.audProps;
		case eMFT_Output:
			{
				pT->Format_UpdateAVProps( NULL, &m_avFormatOut);
				return m_avFormatOut.audProps;
			}
		}

		return m_avFormatIn.audProps;
	}

	void Format_SetInput( M_VID_PROPS* _pVidProps, M_AUD_PROPS* _pAudProps )
	{
		CAutoRWL rw( m_rwFormat, true );
		if( _pVidProps )
			m_avFormatIn.vidProps = *_pVidProps;

		if( _pAudProps )
			m_avFormatIn.audProps = *_pAudProps;
	}

	// 1 = Convert Video, 2 - Convert Audio, 3 - Convert A/V
	int Format_UpdateAVProps( M_AV_PROPS* _pAVPropsIn, M_AV_PROPS* _pAVPropsOut )
	{
		if( _pAVPropsIn )
		{
			CAutoRWL rw( m_rwFormat, true );

			if( IsSpecified(&_pAVPropsIn->vidProps) )
				m_avFormatIn.vidProps = _pAVPropsIn->vidProps;
			
			if( IsSpecified(&_pAVPropsIn->audProps) )
				m_avFormatIn.audProps = _pAVPropsIn->audProps;
		}
		
		CAutoRWL rw( m_rwFormat, false );

		M_AV_PROPS avFormatIn = m_avFormatIn;
		if( !_pAVPropsIn )
			_pAVPropsIn = &avFormatIn;
				
		// Update override
		MVidPropsOverride( &_pAVPropsIn->vidProps, &m_vidFormatOverride );
		
		int nNeedConvert = 0;
		M_AV_PROPS avPropsOut  = *_pAVPropsIn;
		if( IsPartiallySpecified( &m_avFormatCnv.vidProps ) || m_avFormatCnv.vidProps.eVideoFormat == eMVF_Disabled )
		{
			nNeedConvert |= 1;
			avPropsOut.vidProps = MVidPropsMerge(&m_avFormatCnv.vidProps, &avPropsOut.vidProps);
		}
		else 
		{
			if( m_avFormatCnv.vidProps.fccType )
			{
				nNeedConvert |= 1;
				avPropsOut.vidProps.fccType = m_avFormatCnv.vidProps.fccType;
				avPropsOut.vidProps.nRowBytes = Img_RowSizeM( avPropsOut.vidProps.fccType, avPropsOut.vidProps.nWidth );
			}

			if( m_avFormatCnv.vidProps.nAspectX && m_avFormatCnv.vidProps.nAspectY )
			{
				nNeedConvert |= 1;
				avPropsOut.vidProps.nAspectX = m_avFormatCnv.vidProps.nAspectX;
				avPropsOut.vidProps.nAspectY = m_avFormatCnv.vidProps.nAspectY;
			}
		}

		if( m_vidFormatOverride.eInterlace == eMI_OneField )
		{
			avPropsOut.vidProps.eInterlace = eMI_Progressive; // For deinterlace during conversion
			nNeedConvert |= 1;
		}

		// Fix for do not make UYVY/HDYC or RGB32/ARGB32 conversion
		M_VID_PROPS vidCompareIn = _pAVPropsIn->vidProps;
		if( IsSimilarFCC( _pAVPropsIn->vidProps.fccType, avPropsOut.vidProps.fccType ) )
			vidCompareIn.fccType = avPropsOut.vidProps.fccType; 

		if (IsSameFormat(&avPropsOut.vidProps, &vidCompareIn))
		{
			// Fix for MP-192 
			avPropsOut.vidProps.nRowBytes = _pAVPropsIn->vidProps.nRowBytes;
			nNeedConvert &= ~1;
		}

		if (m_avFormatCnv.audProps.nTrackSplitBits > 0 || (m_avFormatCnv.audProps.nChannels && m_avFormatCnv.audProps.nChannels != avPropsOut.audProps.nChannels))
		{
			nNeedConvert |= 2;
			avPropsOut.audProps.nTrackSplitBits = m_avFormatCnv.audProps.nTrackSplitBits;
		}

		if( m_avFormatCnv.audProps.nChannels )
		{
			nNeedConvert |= 2;
			avPropsOut.audProps.nChannels = m_avFormatCnv.audProps.nChannels;
		}

		if( m_avFormatCnv.audProps.nBitsPerSample )
		{
			nNeedConvert |= 2;
			avPropsOut.audProps.nBitsPerSample = m_avFormatCnv.audProps.nBitsPerSample;
		}

		if( m_avFormatCnv.audProps.nSamplesPerSec > 0 )
		{
			nNeedConvert |= 2;
			avPropsOut.audProps.nSamplesPerSec = m_avFormatCnv.audProps.nSamplesPerSec;
		}

		

		if( IsSameFormat( &avPropsOut.audProps, &_pAVPropsIn->audProps ) )
			nNeedConvert &= ~2;

		UpdateMVidProps(&avPropsOut.vidProps);

		avPropsOut.bLocked = _pAVPropsIn->bLocked;

		if( _pAVPropsOut )
			*_pAVPropsOut = avPropsOut;

		return nNeedConvert;
	}

	// [helpstring)("Get video formats )(orignal for MLive) number")] 
	STDMETHOD(FormatVideoGetCount)( /*[in]*/ eMFormatType _eFormatType, /*[out]*/ int* _pnCount)
	{
		ATLASSERT( (int)_eFormatType >= 0 && (int)_eFormatType < SIZEOF_ARRAY( m_arrVideoFormat ) );
		if( (int)_eFormatType < 0 || (int)_eFormatType >= SIZEOF_ARRAY( m_arrVideoFormat )) return E_INVALIDARG;

		T* pT = static_cast<T*>(this);

		CComPtr<IMFormat> cpFormat;
		pT->GetRedirector_Format( _eFormatType, &cpFormat );
		if( cpFormat ) 
			return cpFormat->FormatVideoGetCount( _eFormatType, _pnCount );

#ifdef  MFORMATS_INTERFACES
		{
			CComPtr<IMFFormat> cpFormat;
			pT->GetRedirector_Format( _eFormatType, &cpFormat );
			if( cpFormat ) 
				return cpFormat->FormatVideoGetCount( _eFormatType, _pnCount );
		}
#endif
		
		pT->Format_FillVideo(_eFormatType);

		CAutoRWL rw( m_rwFormat, false );
		if( _pnCount )
			*_pnCount = m_arrVideoFormat[_eFormatType].GetSize();

		return S_OK;
	}

	// [helpstring)("Get video format of device by it's index, for MLive - we enum ONLY supported )(original) formats")] 
	STDMETHOD(FormatVideoGetByIndex)( eMFormatType _eFormatType, /*[in]*/ int _nIndex, /*[out]*/ M_VID_PROPS* _pVidProps, /*[out]*/ BSTR* _pbsName )
	{
		ATLASSERT( (int)_eFormatType >= 0 && (int)_eFormatType < SIZEOF_ARRAY( m_arrVideoFormat ) );
		if( (int)_eFormatType < 0 || (int)_eFormatType >= SIZEOF_ARRAY( m_arrVideoFormat )) return E_INVALIDARG;

		CComPtr<IMFormat> cpFormat;
		T* pT = static_cast<T*>(this);
		pT->GetRedirector_Format( _eFormatType, &cpFormat );
		if( cpFormat ) 
			return cpFormat->FormatVideoGetByIndex( _eFormatType, _nIndex, _pVidProps, _pbsName );

#ifdef  MFORMATS_INTERFACES
		{
			CComPtr<IMFFormat> cpFormat;
			pT->GetRedirector_Format( _eFormatType, &cpFormat );
			if( cpFormat ) 
				return cpFormat->FormatVideoGetByIndex( _eFormatType, _nIndex, _pVidProps, _pbsName );
		}
#endif

		if( !m_arrVideoFormat[_eFormatType].GetSize() )
			pT->Format_FillVideo(_eFormatType);

		CAutoRWL rw( m_rwFormat, false );

		if (_nIndex < 0 || _nIndex >= m_arrVideoFormat[_eFormatType].GetSize())
		{
			MTRACE_ERR("Index is incorrect (is device closed?)");
			return S_FALSE; // E_INVALIDARG;
		}

		if( _pVidProps )
			*_pVidProps = m_arrVideoFormat[_eFormatType][_nIndex];

		if( _pbsName )
			*_pbsName = CComBSTR( MVID2STR(&m_arrVideoFormat[_eFormatType][_nIndex]) ).Detach();

		return S_OK;
	}

	// [helpstring)("Set output video format for object. Note: The _bOriginal used ONLY for devices )(MLive), for other objects this parameter ignored")] 
	STDMETHOD(FormatVideoSet)( /*[in]*/ eMFormatType _eFormatType, /*[in]*/ M_VID_PROPS* _pVidProps )
	{
		MTRACE("(%ws, %ws)", FormatType2Str(_eFormatType), MVID2STR_S(_pVidProps));

		// TODO: Make doc & common logic:
		// DO NOT SET input/output format for renderer, file
		// DO NOT SET output format for live
		if( _eFormatType == eMFT_Output )
		{
			if( !Format_IsRenderer() )
				return E_INVALIDARG;

			_eFormatType = eMFT_Convert;
		}

		// Check for valid convert format
		if( _pVidProps && 
			_pVidProps->fccType != eMFCC_Default && 
			_eFormatType != eMFT_Input &&
			(!Img_IsSupportM( _pVidProps ) || (IsPlanar(_pVidProps->fccType) && !GPU_PipelineMode())) )
		{
			return VFW_E_TYPE_NOT_ACCEPTED;
		}
		
		CComPtr<IMFormat> cpFormat;
		T* pT = static_cast<T*>(this);
		pT->GetRedirector_Format( _eFormatType, &cpFormat );
		if( cpFormat ) 
			return cpFormat->FormatVideoSet( _eFormatType, _pVidProps );

#ifdef  MFORMATS_INTERFACES
		{
			CComPtr<IMFFormat> cpFormat;
			pT->GetRedirector_Format( _eFormatType, &cpFormat );
			if( cpFormat ) 
				return cpFormat->FormatVideoSet( _eFormatType, _pVidProps );
		}
#endif

		M_VID_PROPS vidPropsCheck = {};
		M_VID_PROPS vidPropsSet = {};
		if( _pVidProps ) 
		{
			vidPropsSet = *_pVidProps;
			vidPropsCheck = *_pVidProps;

			// Updated 2016/07/14 - IGNORE nRowBytes for FormatVideoSet()
			vidPropsSet.nRowBytes = 0;
			vidPropsCheck.nRowBytes = 0;

			if( _eFormatType != eMFT_Override )
			{
				if (!pT->MObjectXML()->GetBoolAttribute(L"mfconverter.enabled") ||
					pT->MObjectXML()->GetBoolAttribute(L"format.set_default_pixelformat", true))
				{
					if (IsSpecified(&vidPropsSet))
					{
						// This is for NOT change specified FCC is set only eVideoFormat
						if (_eFormatType == eMFT_Convert && vidPropsSet.fccType == eMFCC_Default)
						{
							vidPropsSet.fccType = m_avFormatCnv.vidProps.fccType;
							if (IsSD(&vidPropsSet) && vidPropsSet.fccType == eMFCC_HDYC)
								vidPropsSet.fccType = eMFCC_UYVY;
							else if (!IsSD(&vidPropsSet) && vidPropsSet.fccType == eMFCC_UYVY)
								vidPropsSet.fccType = eMFCC_HDYC;
						}

						UpdateMVidProps(&vidPropsSet);

						vidPropsCheck = vidPropsSet;
					}
					else if (IsPartiallySpecified(&vidPropsSet))
					{
						// e.g. set only fps etc.
						vidPropsCheck = MVidPropsMerge(&vidPropsSet, &m_avFormatCnv.vidProps);
					}
				}
			}

			if( !pT->Format_IsSupported(_eFormatType, &vidPropsCheck) )
				return VFW_E_TYPE_NOT_ACCEPTED;
		}

		// Special fix for bottom-top RGB  (always use top-bottom)
		if (IsRGB(vidPropsSet.fccType) && vidPropsSet.nHeight > 0)
			vidPropsSet.nHeight *= -1;
		
		M_VID_PROPS vidPropsRestore = {};
		M_VID_PROPS* pVidProps = NULL;
		{
			CAutoRWL rw(m_rwFormat, true);

			switch( _eFormatType )
			{
			case eMFT_Input:
				pVidProps = &m_avFormatIn.vidProps;
				break;
			case eMFT_Convert:
				pVidProps = &m_avFormatCnv.vidProps;
				break;
			case eMFT_Override:
				pVidProps = &m_vidFormatOverride;
				break;
			default:
				return E_INVALIDARG;
			}

			vidPropsRestore = *pVidProps;
			if( IsSpecified( &vidPropsCheck) && !IsDisabled( &vidPropsCheck) )
			{
				vidPropsCheck = MVidPropsMerge( &vidPropsCheck, pVidProps);
			}
			
			if( _eFormatType != eMFT_Override && 
				IsSameFormat( &vidPropsCheck, pVidProps) && 
				vidPropsCheck.eVideoFormat != eMVF_Disabled && 
				vidPropsCheck.eVideoFormat != eMVF_Custom )
			{
				pVidProps->eScaleType = vidPropsCheck.eScaleType;
				return S_FALSE;
			}
			
			*pVidProps = vidPropsSet;
		}

		if( !pT->Format_OnSet(_eFormatType, &vidPropsCheck ) )
		{
			CAutoRWL rw(m_rwFormat, true);
			*pVidProps = vidPropsRestore;

			return VFW_E_TYPE_NOT_ACCEPTED;
		}

		Format_FindVideo( _eFormatType, &vidPropsCheck, _pVidProps ? true : false );
		
		return S_OK;
	}

	// [helpstring)("Get currently set video format. Note: The _bOriginal used ONLY for files and devices )(MFile, MLive), for other objects this parameter ignored")]
	STDMETHOD(FormatVideoGet)( /*[in]*/ eMFormatType _eFormatType, /*[out]*/ M_VID_PROPS* _pVidProps, /*[out]*/ int* _pnIndex, /*[out]*/ BSTR* _pbsName )
	{
		CComPtr<IMFormat> cpFormat;
		T* pT = static_cast<T*>(this);
		pT->GetRedirector_Format( _eFormatType, &cpFormat );
		if (cpFormat)
		{
			// Fix for external process. Parameters should not to be NULL.
			int nIndexIPC = 0;
			CComBSTR cbsNameIPC;
			return cpFormat->FormatVideoGet(_eFormatType, _pVidProps, _pnIndex ? _pnIndex : &nIndexIPC, _pbsName ? _pbsName : &cbsNameIPC);
		}

#ifdef  MFORMATS_INTERFACES
		{
			CComPtr<IMFFormat> cpFormat;
			pT->GetRedirector_Format( _eFormatType, &cpFormat );
			if (cpFormat)
			{
				// Fix for external process. Parameters should not to be NULL.
				int nIndexIPC = 0;
				CComBSTR cbsNameIPC;
				return cpFormat->FormatVideoGet(_eFormatType, _pVidProps, _pnIndex ? _pnIndex : &nIndexIPC, _pbsName ? _pbsName : &cbsNameIPC);
			}				
		}
#endif

		CAutoRWL rw(m_rwFormat, true);

		M_AV_PROPS avPropsOut = {};
		M_VID_PROPS* pVidProps = NULL;
		switch( _eFormatType )
		{
		case eMFT_Input:
			pVidProps = &m_avFormatIn.vidProps;
			break;
		case eMFT_Convert:
			pVidProps = &m_avFormatCnv.vidProps;
			break;
		case eMFT_Override:
			pVidProps = &m_vidFormatOverride;
			break;
		case eMFT_Output:
			pT->Format_UpdateAVProps( NULL, &avPropsOut);
			pVidProps = &avPropsOut.vidProps;
			break;
		default:
			return E_INVALIDARG;
		}

		if( _pVidProps )
			*_pVidProps = *pVidProps;

		if( _pnIndex && _eFormatType != eMFT_Override )
			*_pnIndex = Format_FindVideo( _eFormatType, pVidProps, false );

		if( _pbsName )
			*_pbsName = CComBSTR( MVID2STR(pVidProps) ).Detach();

		return IsSpecified( pVidProps ) ? S_OK : S_FALSE;
	}

	// Audio
	STDMETHOD(FormatAudioGetCount)( /*[in]*/ eMFormatType _eFormatType, /*[out]*/ int* _pnCount)
	{
		ATLASSERT( (int)_eFormatType >= 0 && (int)_eFormatType < SIZEOF_ARRAY( m_arrAudioFormat ) );
		if( (int)_eFormatType < 0 || (int)_eFormatType >= SIZEOF_ARRAY( m_arrAudioFormat )) return E_INVALIDARG;

		CComPtr<IMFormat> cpFormat;
		T* pT = static_cast<T*>(this);
		pT->GetRedirector_Format( _eFormatType, &cpFormat );
		if( cpFormat ) 
			return cpFormat->FormatAudioGetCount( _eFormatType, _pnCount );

#ifdef  MFORMATS_INTERFACES
		{
			CComPtr<IMFFormat> cpFormat;
			pT->GetRedirector_Format( _eFormatType, &cpFormat );
			if( cpFormat ) 
				return cpFormat->FormatAudioGetCount( _eFormatType, _pnCount );
		}
#endif

		pT->Format_FillAudio(_eFormatType);

		CAutoRWL rw( m_rwFormat, false );
		if( _pnCount )
			*_pnCount = m_arrAudioFormat[_eFormatType].GetSize();

		return S_OK;
	}

	// [helpstring)("Get audeo format of device by it's index, for MLive - we enum ONLY supported )(original) formats")] 
	STDMETHOD(FormatAudioGetByIndex)( /*[in]*/ eMFormatType _eFormatType, /*[in]*/ int _nIndex, /*[out]*/ M_AUD_PROPS* _pAudProps, /*[out]*/ BSTR* _pbsName )
	{
		ATLASSERT( (int)_eFormatType >= 0 && (int)_eFormatType < SIZEOF_ARRAY( m_arrVideoFormat ) );
		if( (int)_eFormatType < 0 || (int)_eFormatType >= SIZEOF_ARRAY( m_arrVideoFormat )) return E_INVALIDARG;

		CComPtr<IMFormat> cpFormat;
		T* pT = static_cast<T*>(this);
		pT->GetRedirector_Format( _eFormatType, &cpFormat );
		if( cpFormat ) 
			return cpFormat->FormatAudioGetByIndex( _eFormatType, _nIndex, _pAudProps, _pbsName );

#ifdef  MFORMATS_INTERFACES
		{
			CComPtr<IMFFormat> cpFormat;
			pT->GetRedirector_Format( _eFormatType, &cpFormat );
			if( cpFormat ) 
				return cpFormat->FormatAudioGetByIndex( _eFormatType, _nIndex, _pAudProps, _pbsName );
		}
#endif

		if( !m_arrAudioFormat[_eFormatType].GetSize() )
			pT->Format_FillAudio(_eFormatType);

		CAutoRWL rw( m_rwFormat, false );

		if( _nIndex < 0 || _nIndex >= m_arrAudioFormat[_eFormatType].GetSize() )
			return E_INVALIDARG;

		if( _pAudProps )
			*_pAudProps = m_arrAudioFormat[_eFormatType][_nIndex];

		if( _pbsName )
			*_pbsName = CComBSTR( MAUD2STR(&m_arrAudioFormat[_eFormatType][_nIndex]) ).Detach();

		return S_OK;
	}

	// [helpstring)("Set output video format for object. Note: The _bOriginal used ONLY for devices )(MLive), for other objects this parameter ignored")] 
	STDMETHOD(FormatAudioSet)( /*[in]*/ eMFormatType _eFormatType, /*[in]*/ M_AUD_PROPS* _pAudProps )
	{
		MTRACE("(%ws, %ws)", FormatType2Str(_eFormatType), MAUD2STR_S(_pAudProps));

		if( _eFormatType == eMFT_Output )
		{
			if( !Format_IsRenderer() )
				return E_INVALIDARG;

			_eFormatType = eMFT_Convert;
		}

		CComPtr<IMFormat> cpFormat;
		T* pT = static_cast<T*>(this);
		pT->GetRedirector_Format( _eFormatType, &cpFormat );
		if( cpFormat ) 
			return cpFormat->FormatAudioSet( _eFormatType, _pAudProps );

#ifdef  MFORMATS_INTERFACES
		{
			CComPtr<IMFFormat> cpFormat;
			pT->GetRedirector_Format( _eFormatType, &cpFormat );
			if( cpFormat ) 
				return cpFormat->FormatAudioSet( _eFormatType, _pAudProps );
		}
#endif

		M_AUD_PROPS audPropsSet = {};
		if( _pAudProps ) 
		{
			audPropsSet = *_pAudProps;

			if( !pT->Format_IsSupported(_eFormatType, &audPropsSet) )
				return VFW_E_TYPE_NOT_ACCEPTED;
		}

		M_AUD_PROPS audPropsRestore = {};
		M_AUD_PROPS* pAudProps = NULL;
		{
			CAutoRWL rw(m_rwFormat, true);

			switch( _eFormatType )
			{
			case eMFT_Input:
				pAudProps = &m_avFormatIn.audProps;
				break;
			case eMFT_Convert:
				pAudProps = &m_avFormatCnv.audProps;
				break;
			default:
				return E_INVALIDARG;
			}

			audPropsRestore = *pAudProps;
			if( IsSpecified( &audPropsSet, false ) && !IsDisabled( &audPropsSet ) )
				audPropsSet = MAudPropsMerge( &audPropsSet, pAudProps );
		
			if( IsSameFormat(pAudProps, &audPropsSet) && !IsDisabled( &audPropsSet ) )
				return S_FALSE;

			*pAudProps = audPropsSet;
		}

		if( !pT->Format_OnSet(_eFormatType, &audPropsSet ) )
		{
			CAutoRWL rw(m_rwFormat, true);

			*pAudProps = audPropsRestore;

			return VFW_E_TYPE_NOT_ACCEPTED;
		}
		
		Format_FindAudio( _eFormatType, &audPropsSet, _pAudProps ? true : false );
		
		return S_OK;
	}

	// [helpstring)("Get currently set audeo format. Note: The _bOriginal used ONLY for files and devices )(MFile, MLive), for other objects this parameter ignored")]
	STDMETHOD(FormatAudioGet)( /*[in]*/ eMFormatType _eFormatType, /*[out]*/ M_AUD_PROPS* _pAudProps, /*[out]*/ int* _pnIndex, /*[out]*/ BSTR* _pbsName )
	{
		if( _eFormatType == eMFT_Override )
			return E_INVALIDARG;

		CComPtr<IMFormat> cpFormat;
		T* pT = static_cast<T*>(this);
		pT->GetRedirector_Format( _eFormatType, &cpFormat );
		if( cpFormat ) 
			return cpFormat->FormatAudioGet( _eFormatType, _pAudProps, _pnIndex, _pbsName );

#ifdef  MFORMATS_INTERFACES
		{
			CComPtr<IMFFormat> cpFormat;
			pT->GetRedirector_Format( _eFormatType, &cpFormat );
			if( cpFormat ) 
				return cpFormat->FormatAudioGet( _eFormatType, _pAudProps, _pnIndex, _pbsName );
		}
#endif

		CAutoRWL rw(m_rwFormat, true);

		M_AV_PROPS avPropsOut = {};
		M_AUD_PROPS* pAudProps = NULL;
		switch( _eFormatType )
		{
		case eMFT_Input:
			pAudProps = &m_avFormatIn.audProps;
			break;
		case eMFT_Convert:
			pAudProps = &m_avFormatCnv.audProps;
			break;
		case eMFT_Output:
			pT->Format_UpdateAVProps( NULL, &avPropsOut);
			pAudProps = &avPropsOut.audProps;
			break;
		default:
			return E_INVALIDARG;
		}

		if( _pAudProps )
			*_pAudProps = *pAudProps;

		if( _pnIndex && _eFormatType != eMFT_Override )
			*_pnIndex = Format_FindAudio( _eFormatType, pAudProps, false );

		if( _pbsName )
			*_pbsName = CComBSTR( MAUD2STR(pAudProps) ).Detach();

		return (IsSpecified( pAudProps ) || IsDisabled( pAudProps ) ) ? S_OK : S_FALSE;
	}
};

