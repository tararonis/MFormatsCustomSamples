//---------------------------------------------------------------------------
// This file is part of the Medialooks Media Platform (MMP)
//
// MSourceImpl.h 
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

#include "MDefines.h"

#include "../Base/MHelpers.h"
#include "../WinAPI/threads.h"
#include "../Devices/MTimer.h"

#include <map>
using std::map;
#include <set>
using std::set;

#include "../Base/MCreator.h"
#include "../Classes/MFExternalImpl.h"

//#define MSOURCE_PERF

#ifdef MSOURCE_PERF
#define CBasicPerfMSRC		CBasicPerfD
#else 
#define CBasicPerfMSRC	CBasicPerfFake
#endif

#ifdef _DEBUG
#define MAX_FAILED_PUT_CALLS	(10000)
#else
#define MAX_FAILED_PUT_CALLS	(100)
#endif

template <class T>
class IMSourceImpl
	: public IMFSource_I,
	  public IMSource_I,
	  public IMFExternalImpl<T>
{
	typedef IMSourceImpl<T> _IMSourceImpl;

	// For usage SourceFrameGetEx() for MFile and MPlaylist
	MTimer					m_timerForFiles;

	class MReader
		: public AutoObjBase<MReader>
	{
		mutable CComAutoCriticalSection	m_csDeq;
		typedef deque<CComPtr<IMFFrame> > TDeqFrames;
		TDeqFrames				m_deqFrames;
		UINT					m_uMaxDeq;
		int						m_nFailedLast;
		bool					m_bSync;

	private:
		MReader( int _nMaxDeq, bool _bSync )
			: m_uMaxDeq( _nMaxDeq ),
			m_nFailedLast( -1 ), // For set break to first frame
			m_nFailedTotal( 0 ),
			m_bNotAutoClose( false ),
			m_bSync( _bSync )
		{
			m_uMaxDeq = MAX( 1, m_uMaxDeq );
			m_shHaveFrame.Attach( ::CreateEvent( NULL, TRUE, FALSE, NULL ) );
			m_shHaveSlot.Attach( ::CreateEvent( NULL, TRUE, TRUE, NULL ) );
		}

	public:

		//CComPtrM<IMFFrame>		m_cpReadyFrame;
		//CStdHandle				m_shTimerGet;
		//MClock					m_clockReader;

		CStdHandle				m_shHaveFrame;
		CStdHandle				m_shHaveSlot;
		
		MTimer					m_timer;

		int						m_nFailedTotal;  // Public for debug only
		bool					m_bNotAutoClose;

		static HRESULT CreateInstance( MReader** _ppObject, int _nMaxDeq, bool _bSync )
		{
			ATLASSERT( _ppObject );
			if( !_ppObject ) return E_POINTER;

			MReader* pT = new MReader(_nMaxDeq, _bSync);
			pT->AddRef();
			*_ppObject = pT;
			return S_OK;
		}
		
		
		inline bool IsDeqFull( int* _pnFailedLast ) const
		{
			CAutoCS crs( m_csDeq );

			if( _pnFailedLast )
				*_pnFailedLast = m_nFailedLast;

			return m_deqFrames.size() >= m_uMaxDeq; 
		}

		inline bool IsDeqEmpty() const 
		{ 
			CAutoCS crs( m_csDeq );

			return m_deqFrames.size() == 0; 
		}

		inline int DeqSize()
		{
			CAutoCS crs(m_csDeq);

			return (int)m_deqFrames.size();
		}

		inline double HeadDelay()
		{
			CAutoCS crs(m_csDeq);

			return m_deqFrames.size() ? MFFrame_DelayMsec(m_deqFrames.front()) : -1.0;
		}

		inline bool IsSync() const { return m_bSync; }


		// _nTakeType = 0 -> keep frame
		// _nTakeType = 1 -> return oldest one, remove it
		// _nTakeType = 2 -> return oldest one, remove all
		// _nTakeType = -1 -> return newest one, remove all (flash)
		template< class TFrame>
		inline bool TakeFrame( TFrame** _ppFrame, int _nTakeType, bool _bClone = true )
		{
			ATLASSERT( _ppFrame );

			CComPtr<IMFFrame> cpMFFrame;

			bool bSetBreak = false;
			{
				CAutoCS crs( m_csDeq );

				if( m_deqFrames.empty() ) 
				{
					MTRACE_WARN( "DEQ IS EMPTY" );
					return false;
				}

				if( m_nFailedLast != 0 )
				{
					bSetBreak = true;
					m_nFailedLast = 0;
				}
				
				if( _nTakeType < 0 )
				{
					cpMFFrame = m_deqFrames.back();
					if( m_deqFrames.size() > 1 )
						bSetBreak = true;

					m_deqFrames.clear();
				}
				else 
				{
					cpMFFrame = m_deqFrames.front();
					if( _nTakeType == 1 )
					{
						m_deqFrames.pop_front();
					}
					else if( _nTakeType > 1 )
					{
						if( m_deqFrames.size() > 1 )
							m_nFailedLast++; // Need set break for next frame

						m_deqFrames.clear();
					}
				}

				if( m_deqFrames.size() == 0 )
					::ResetEvent( m_shHaveFrame );

				::SetEvent( m_shHaveSlot );
			}

			HRESULT hr = S_OK;
			if( _bClone )
				hr = MCreator::FramesClone( cpMFFrame, _ppFrame, eMFC_Reference, eMFCC_Default );
			else
				hr = cpMFFrame.QueryInterface( _ppFrame );

			ATLASSERT( SUCCEEDED( hr ) );

			// Note: Do not reset PAUSE/DUP flags !!!
			if( bSetBreak )
				MFFrame_UpdateFlag( *_ppFrame, eMFF_Break|eMFF_SkippedFrame, 1 );
			
			MTRACE( "(%08X:%04X) FRAME GET Type:%d Deq:%d/%d Failed:%d/%d %ws=%08X %ws [OUT:%ws]",
				(LONG)this, GetCurrentThreadId(), _nTakeType, (int)m_deqFrames.size(), m_uMaxDeq, m_nFailedLast, 
				m_nFailedTotal, _bClone ? L"Clone" : L"QI", hr, bSetBreak ? L"FORCE_BREAK" : L"", MFRAME2STR( *_ppFrame ) );


			return true;
		}

		template< class TFrame>
		inline bool PutFrame( TFrame* _pFrame, int* _pnFailed, bool* _pbFrameRemoved = NULL )
		{
			CComQIPtr<IMFFrame> qpMFrame( _pFrame );
			ATLASSERT( qpMFrame );
			if( !qpMFrame )
				return false;

			CAutoCS crs( m_csDeq );

			m_deqFrames.push_back( qpMFrame );

			if( m_deqFrames.size() > m_uMaxDeq )
			{
				MTRACE_WARN( "(%08X:%04X) Size:%d/%d Failed:%d/%d [DEQ_REMOVE: %ws] [PUT: %ws]", 
					(LONG)this, GetCurrentThreadId(), (int)m_deqFrames.size(), m_uMaxDeq, m_nFailedLast, m_nFailedTotal, MFRAME2STR( m_deqFrames.front() ), MFRAME2STR(qpMFrame) );

				m_nFailedLast++;
				m_nFailedTotal++;
				
				m_deqFrames.pop_front();

				if( _pbFrameRemoved )
					*_pbFrameRemoved = true;
			}
			else 
			{
				MTRACE( "(%08X:%04X) Size:%d/%d Failed:%d/%d [PUT: %ws]", 
					(LONG)this, GetCurrentThreadId(), (int)m_deqFrames.size(), m_uMaxDeq, m_nFailedLast, m_nFailedTotal, MFRAME2STR(qpMFrame) );

				if( _pbFrameRemoved )
					*_pbFrameRemoved = false;
			}

			::SetEvent( m_shHaveFrame );

			if( m_deqFrames.size() >= m_uMaxDeq )
				::ResetEvent( m_shHaveSlot );

			return true;
		}

		~MReader()
		{
			m_shHaveFrame.Close();
			m_shHaveSlot.Close();
			//m_shTimerGet.Close();
		}
	};

	

	CStdHandle								m_shHaveReaders;
	CRWSynchroSRW							m_rwMap;
	typedef CAutoObj<MReader>				MReaderPtr;
	typedef map<LONGLONG, MReaderPtr>		TMapReaders;

	TMapReaders			m_mapReaders;
	LONGLONG			m_llBaseCookie;

	CStdHandle		m_shFlush;
	CStdHandle		m_shEOS;
	MClock			m_clockPut;

	CComPtrM<IMFFrame>	m_cpFrameCache;

	// For wait files
	MTimer			m_timerFilesWait;
public:

	IMSourceImpl()
		: m_llBaseCookie( 0 )
	{
		m_shFlush.Attach( ::CreateEvent( NULL, TRUE, FALSE, NULL ) );		// Flush
		m_shEOS.Attach( ::CreateEvent( NULL, TRUE, FALSE, NULL ) );		// EOS
		m_shHaveReaders.Attach( ::CreateEvent( NULL, TRUE, FALSE, NULL ) );		// EOS
	}

	~IMSourceImpl()
	{
		Source_CloseAll();
	}

	HRESULT Source_Flush( bool _bResetEOS = true )
	{
		ATLTRACE2( atlTraceGeneral, 2, _T("IMSourceImpl(%08X,%04X)::Source_Flush(ResetEOS:%d) Set FLUSH, Reset EOS\n"), 
			(LONG)this, GetCurrentThreadId(), _bResetEOS );

		::SetEvent( m_shFlush );
		if( _bResetEOS )
			::ResetEvent( m_shEOS );

		m_cpFrameCache.Release();

		return S_OK;
	}

	HRESULT Source_EOS()
	{
		ATLTRACE2( atlTraceGeneral, 2, _T("IMSourceImpl(%08X,%04X)::Source_Flush() Set EOS\n"), 
			(LONG)this, GetCurrentThreadId() );

		::SetEvent( m_shEOS );
		return S_OK;
	}

	HRESULT Source_OnStart()
	{
		ATLTRACE2( atlTraceGeneral, 2, _T("IMSourceImpl(%08X,%04X)::Source_OnStart() Reset flash/EOS\n"), 
			(LONG)this, GetCurrentThreadId() );

		::ResetEvent( m_shEOS );
		::ResetEvent( m_shFlush );

		return S_OK;
	}

	void Source_CloseAll()
	{
		ATLTRACE2( atlTraceGeneral, 2, _T("IMSourceImpl(%08X,%04X)::Source_CloseAll()\n"), 
			(LONG)this, GetCurrentThreadId() );

		CAutoRWS rw( m_rwMap, true );

		::SetEvent(m_shFlush); // For e.g. break receiver wait 

		m_mapReaders.clear();
	}

	HRESULT Source_PutFrame( IUnknown* _pFrame, int _nWaitType = 0)
	{
		CComQIPtr<IMFFrame> qpMFrame( _pFrame );

		return Source_PutFrame( qpMFrame, _nWaitType );
	}

	// IF pull_mode.put_wait (or _nWaitType) == 1 -> for file wait x4 frame time
	// IF pull_mode.put_wait (or _nWaitType) == 2 -> for live & file wait x4 frame time
	// IF pull_mode.put_wait (or _nWaitType) == 5 -> for file wait x4 frame time, but NOT wait for first readers
	// IF pull_mode.put_wait (or _nWaitType) == 6 -> for live & file wait x4 frame time, but NOT wait for first readers
	HRESULT Source_PutFrame( IMFFrame* _pFrame, int _nWaitType = 0, REFERENCE_TIME _rtMaxWait = -1, int* _pnReaders = NULL, int* _pnWaitReaders = NULL )
	{
		T* pT = static_cast<T*>(this);
		int nReceivers = -1; // For call On_Close() if reader removed (NOTIMPL -> Need thread)
		{
			WCHAR sz[128] = {};
			wsprintf( sz, L"Source_PutFrame(TID:%04X)", GetCurrentThreadId() );

			CBasicPerfMSRC perf( sz );

			M_TIME mTime = {};
			if( _pFrame )
			{
				_pFrame->MFTimeGet( &mTime );

				HANDLE phWait[2] = { m_shFlush, m_shEOS };
				if( IS_REAL_BREAK( &mTime, false ) )
				{
					ATLTRACE2( atlTraceGeneral, 2, _T("IMSourceImpl(%08X,%04X)::Source_PutFrame() %ws Reset FLUSH, EOS\n"), 
						(LONG)this, GetCurrentThreadId(), MTIME2STR( mTime ) );

					pT->MPropsXML()->IncrementAttribute(L"stat::pull.breaks_in", 1, true);

					::ResetEvent( m_shFlush );
					::ResetEvent( m_shEOS );
				}
				else if( ::WaitForMultipleObjects( 2, phWait, FALSE, 0 ) != WAIT_TIMEOUT )
				{
					ATLTRACE2( atlTraceGeneral, 2, _T("WARNING !!! IMSourceImpl(%08X,%04X)::Source_PutFrame() %ws FLUSH/EOS state -> frame SKIPPED\n"), 
						(LONG)this, GetCurrentThreadId(), MTIME2STR( mTime ) );

					pT->MPropsXML()->IncrementAttribute(L"stat::pull.skipped_flush_eos", 1, true);

					return VFW_E_WRONG_STATE;
				}
			}

			TMapReaders mapRemove;

			// Check for not taken frames (for files)
			bool bWaitReaders = false; 
			int nWaitCicles = 0;
			CSimpleArray2<MReader::TPtr> arrWaitReaders;
			int nSyncWaitType = pT->MPropsXML()->GetIntAttribute( L"pull_mode.put_wait", _nWaitType );

			// Special fix for live buffering
			// VVB: Disabled during MP-1166 fix, no reason for add additional wait for Live
			// (if we delay Live here, the buffers before may overload and skipping anyway occurs)
// 			if( IS_LIVE( &mTime ) && pT->MPropsXML()->GetIntAttribute( L"pull_mode.output_type" ) > 1 )
// 				nSyncWaitType |= 2;

			// By default wait for file (30 cicles) for frame time
			// IF pull_mode.put_wait (or _nWaitType) == 1 -> for file wait x4 frame time
			// IF pull_mode.put_wait (or _nWaitType) == 2 -> for live & file wait x4 frame time
			if( (!IS_LIVE( &mTime ) || nSyncWaitType > 1) && 
				((nWaitCicles = pT->MPropsXML()->GetIntAttribute( L"pull_mode.cycles_before_disconnect", 15 ) ) > 0 || nSyncWaitType > 0) )
			{
				CAutoRWS rw( m_rwMap, false ); 

				TMapReaders::iterator Iter = m_mapReaders.begin();
				while( Iter != m_mapReaders.end() )
				{
					MReader::TPtr pReader = Iter->second;

					int nFailedLast = 0;
					if( pReader->IsDeqFull( &nFailedLast ) && pReader->IsSync() && (nFailedLast < nWaitCicles || IS_PAUSE(&mTime)))
					{
						ATLTRACE2( atlTraceGeneral, 8, _T("IMSourceImpl(%08X,%04X)::Source_PutFrame(%08X) %ws WAIT PREV (Failed:%d/%d Max Cicles:%d) - FILE/Live WAIT(%d)\n"), 
							(LONG)this, GetCurrentThreadId(), (LONG)Iter->first, MTIME2STR( mTime ),
							nFailedLast, pReader->m_nFailedTotal, nWaitCicles, nSyncWaitType );

						arrWaitReaders.Add( pReader );
						break;
					}

					Iter++;
				}

				if( m_mapReaders.empty() && nSyncWaitType > 0 && !IS_LIVE( &mTime ) )
				{
					bWaitReaders = true;
				}

				if( _pnWaitReaders )
					*_pnWaitReaders = arrWaitReaders.GetSize();
			}

			if( arrWaitReaders.GetSize() )
			{
				REFERENCE_TIME rtWait = DBL2INT64( (mTime.rtEndTime - mTime.rtStartTime) * 2.0 / 3.0);

				HANDLE* phWait = (HANDLE*)alloca( sizeof(HANDLE) * arrWaitReaders.GetSize() );
				for( int i = 0; i < arrWaitReaders.GetSize(); i++ )
					phWait[i] = arrWaitReaders[i]->m_shHaveSlot;

				if( nSyncWaitType > 0 )
				{
					rtWait += pT->MPropsXML()->GetIntAttribute( L"pull_mode.wait_till_get", 200 ) * DS_MSEC;
				}

				if( _rtMaxWait >= 0 )
					rtWait = MIN( _rtMaxWait, rtWait );

				// Special fix for MMixer sync mode
				// e.g. file in loop - https://medialooks.myjetbrains.com/youtrack/issue/MP-367
				bool bIgnoreFlush = nSyncWaitType > 0 && (rtWait < 1 * DS_ONESEC);

				REFERENCE_TIME rtRealWait = 0;
				DWORD dwRes = m_timerFilesWait.WaitMultipleObjects_WithTimeAndCancel( arrWaitReaders.GetSize(), phWait, bIgnoreFlush ? 0 : 1, &m_shFlush.m_hHandle, rtWait, &rtRealWait );
				
				ATLTRACE2( atlTraceGeneral, 8, _T("IMSourceImpl(%08X,%04X)::Source_PutFrame() %ws WAIT PREV - FILE WAIT:%s/%s (dwRes:%d)\n"), 
					(LONG)this, GetCurrentThreadId(), MTIME2STR( mTime ),
					RT2STR( rtWait ), RT2STR( rtRealWait ), dwRes );

				pT->MPropsXML()->SetAttribute(L"stat::pull.sync_readers", (int)arrWaitReaders.GetSize() );
				if (dwRes == WAIT_TIMEOUT )
				{
					pT->MPropsXML()->IncrementAttribute(L"stat::pull.sync_timeout", 1, true);
				}
				else if (dwRes > WAIT_OBJECT_0 + arrWaitReaders.GetSize())
				{
					pT->MPropsXML()->IncrementAttribute(L"stat::pull.sync_failed", 1, true);
				}
			}
			else if( bWaitReaders && (nSyncWaitType & 0x04) == 0 )
			{
				ATLTRACE2( atlTraceGeneral, 8, _T("IMSourceImpl(%08X,%04X)::Source_PutFrame() %ws NO READERS - WAIT START\n"), 
					(LONG)this, GetCurrentThreadId(), MTIME2STR( mTime ) );

				
				REFERENCE_TIME rtWait = 0;
				HANDLE arrWait[3] = { m_shEOS, m_shFlush, m_shHaveReaders };
				DWORD dwRes = m_timerFilesWait.WaitAnyObjects_WithTime( 3, arrWait, _rtMaxWait, &rtWait );

				ATLTRACE2( atlTraceGeneral, 8, _T("IMSourceImpl(%08X,%04X)::Source_PutFrame() %ws NO READERS - WAIT DONE:%s (dwRes:%d)\n"), 
					(LONG)this, GetCurrentThreadId(), MTIME2STR( mTime ), RT2STR( rtWait ), dwRes );
			}
			else
			{
				pT->MPropsXML()->SetAttribute(L"stat::pull.sync_readers", (int)0);
			}

			{
				CAutoRWS rw( m_rwMap, true );  // True for no reader access

				m_clockPut.Reset();

				CSimpleArray2<int> arrFailed;
				CSimpleArray2<int> arrBuffers;
				CSimpleArray2<double> arrDelay;

				TMapReaders::iterator Iter = m_mapReaders.begin();
				while( Iter != m_mapReaders.end() )
				{
					MReader::TPtr pReader = Iter->second;

					bool bFrameRemoved = false;
					int nFailedLast = 0;
					pReader->PutFrame( _pFrame, &nFailedLast, &bFrameRemoved );
					if( nFailedLast > MAX_FAILED_PUT_CALLS && !pReader->m_bNotAutoClose )
					{
						ATLTRACE2( atlTraceGeneral, 8, _T("WARNING !!! IMSourceImpl(%08X,%04X)::Source_PutFrame(%08X) %ws NOT GET YET (Failed:%d/%d) REMOVE READER\n"), 
							(LONG)this, GetCurrentThreadId(), (LONG)Iter->first, MTIME2STR( mTime ),
							nFailedLast, pReader->m_nFailedTotal );

						pT->MPropsXML()->IncrementAttribute(L"stat::pull.diconnect_readers", 1, true);

						// Delete this reader
						mapRemove.insert( *Iter );
						Iter++;
						continue;
					}

					arrFailed.Add(nFailedLast);
					arrBuffers.Add(pReader->DeqSize());
					arrDelay.Add(pReader->HeadDelay());

					if (bFrameRemoved)
						pT->MPropsXML()->IncrementAttribute(L"stat::pull.frames_dropped", 1, true);

					Iter++;
				}

				m_cpFrameCache = _pFrame;

				pT->MPropsXML()->SetAttribute(L"stat::pull.readers", (int)m_mapReaders.size() );

				if (arrBuffers.GetSize() > 0)
				{
					pT->MPropsXML()->SetAttribute(L"stat::pull.buffers", XMLUtils::ArrayInt2String(arrBuffers.m_aT, arrBuffers.GetSize() ) );
					pT->MPropsXML()->SetAttribute(L"stat::pull.delay", XMLUtils::ArrayDbl2String(arrDelay));
					pT->MPropsXML()->SetAttribute(L"stat::pull.dropped", XMLUtils::ArrayInt2String(arrFailed.m_aT, arrFailed.GetSize()));
				}
				else
				{
					pT->MPropsXML()->RemoveAttribute(L"stat::pull.buffers");
					pT->MPropsXML()->RemoveAttribute(L"stat::pull.delay");
					pT->MPropsXML()->RemoveAttribute(L"stat::pull.dropped");
				}

				if( _pnReaders )
					*_pnReaders = (int)m_mapReaders.size();

				if( m_mapReaders.size() == 0 )
					return S_FALSE;
			}

			if( mapRemove.size() )
			{
				CAutoRWS rw( m_rwMap, true ); 

				TMapReaders::iterator Iter = mapRemove.begin();
				while( Iter != mapRemove.end() )
				{
					m_mapReaders.erase( Iter->first );
					Iter++;
				}

				if( m_mapReaders.size() == 0)
				{
					::ResetEvent( m_shHaveReaders );

					// E.g. if reader crashed or improper closed -> reset (4) mode 
					// TODO: Make normal requests & rate controls
					if( pT->MPropsXML()->GetIntAttribute( L"rate_control" ) == 4 )
					{
						pT->MPropsXML()->SetAttribute( L"rate_control", 3 );
					}
				}

				if( _pnReaders )
					*_pnReaders = (int)m_mapReaders.size();

			}
		}

		return S_OK;
	}

	STDMETHOD(MFSourceFrameGet)( /*[out]*/ IMFFrame** _ppFrame, /*[in]*/ REFERENCE_TIME _rtMaxWaitTime, /*[in]*/ BSTR _bsPropsList )
	{
		T* pT = static_cast<T*>(this);

		return pT->MFSourceFrameGetEx( NULL, _ppFrame, _rtMaxWaitTime, _bsPropsList );
	}

	STDMETHOD(MFSourceFrameGetEx)( /*[in,out]*/ LONGLONG* _plCookie, /*[out]*/ IMFFrame** _ppFrame, /*[in]*/ REFERENCE_TIME _rtMaxWaitTime, /*[in]*/ BSTR _bsPropsList )
	{
		T* pT = static_cast<T*>(this);

		int nBuffers = -1;
		eFrameGetFlags eFlags = eFGT_Normal;
		if( !STR_IS_EMPTY( _bsPropsList ) )
		{
			if( STR_IS_SAME_I( _bsPropsList, L"remove_reader" ) )
			{
				eFlags = eFGT_Remove;
			}
			else 
			{
				if( StrStrIW( _bsPropsList, L"flush_reader" ) != NULL )
					PutFlag( &eFlags, eFGT_FlushBuffers );

				if( StrStrIW( _bsPropsList, L"sync_reader" ) != NULL )
					PutFlag( &eFlags, eFGT_Sync );

				LPCWSTR pszBuffers = FindAttribute( _bsPropsList, L"buffers", NULL );
				if( pszBuffers )
					nBuffers = _wtol( pszBuffers );
			}
		}

		return pT->Source_FrameGetEx_I( _plCookie, _ppFrame, _rtMaxWaitTime, eFlags, nBuffers );
	}

	STDMETHOD(MFSourceInit)(/* [in,out]*/ LONGLONG* _plCookie )
	{
		T* pT = static_cast<T*>(this);

		return pT->SourceInit( _plCookie );
	}

	STDMETHOD(MFSourceClose)( /*[in]*/ LONGLONG _lCookie )
	{
		T* pT = static_cast<T*>(this);

		return pT->SourceClose( _lCookie );
	}

	// For auto destroy live objects
	STDMETHOD(MFSourceRefAdd)( /*[in]*/ DWORD _dwProcessID )
	{
		T* pT = static_cast<T*>(this);

		return pT->SourceRefAdd( _dwProcessID );
	}

	STDMETHOD(MFSourceRefRemove)( /*[in]*/ DWORD _dwProcessID )
	{
		T* pT = static_cast<T*>(this);

		return pT->SourceRefRemove( _dwProcessID );
	}

	STDMETHOD(MFSourceHaveRefs)( /*[out]*/ LONGLONG* _phWaitHandle )
	{
		T* pT = static_cast<T*>(this);

		return pT->SourceHaveRefs( _phWaitHandle );
	}

	STDMETHOD(MFSourcePropsGet)( /*[out]*/ M_AV_PROPS* _pAVProps )
	{
		CComPtrM<IMFFrame> cpMFFrameCache(m_cpFrameCache);
		if( !cpMFFrameCache)
			return HRESULT_FROM_WIN32(ERROR_EMPTY);

		long lFake = 0;
		return cpMFFrameCache->MFAVPropsGet( _pAVProps, &lFake );
	}

	STDMETHOD(SourceFrameGet)( /*[in]*/ REFERENCE_TIME _rtMaxWaitTime, /*[out]*/ IMFrame** _ppFrame, /*[in]*/ int _nFlags )
	{
		T* pT = static_cast<T*>(this);

		CComPtr<IMFFrame> cpMFrame;
		HRESULT hr = pT->Source_FrameGetEx_I( NULL, &cpMFrame, _rtMaxWaitTime, (eFrameGetFlags)_nFlags, (_nFlags >> 8) & 0xFF );
		if (FAILED(hr))
		{
			ATLTRACE2(atlTraceGeneral, 2, _T("###Err### IMSourceImpl.SourceFrameGet() FAILED hr=0x%08X \n"), this, hr);
			return S_FALSE;
		}	

		if( _ppFrame && cpMFrame )
			return cpMFrame.QueryInterface( _ppFrame );

		return hr;
	}

	STDMETHOD(SourceFrameGetEx)( /*[in,out]*/ LONGLONG* _plCookie, /*[in]*/ REFERENCE_TIME _rtMaxWaitTime, /*[out]*/ IMFrame** _ppFrame, /*[in]*/ eFrameGetFlags _eFlags )
	{
		T* pT = static_cast<T*>(this);

		CComPtr<IMFFrame> cpMFrame;
		HRESULT hr = pT->Source_FrameGetEx_I( _plCookie, _ppFrame ? &cpMFrame : NULL, _rtMaxWaitTime, _eFlags, (_eFlags >> 8) & 0xFF );
		if( FAILED( hr ) )
			return hr;

		if( _ppFrame && cpMFrame )
			return cpMFrame.QueryInterface( _ppFrame );

		return hr;
	}

	HRESULT Source_FrameGetEx_I( /*[in,out]*/ LONGLONG* _plCookie, 
		IMFFrame** _ppFrame, 
		REFERENCE_TIME _rtMaxWaitTime, 
		eFrameGetFlags _eFlags, 
		int _nMaxBuffers = -1 )
	{
		
		WCHAR sz[128] = {};
		wsprintf( sz, L"SourceFrameGetEx_I(TID:%04X ReaderID:%08X)", GetCurrentThreadId(), _plCookie ? *_plCookie : 0 );

		CBasicPerfMSRC perf( sz );

		T* pT = static_cast<T*>(this);

		if( !_plCookie )
			_plCookie = &m_llBaseCookie;

		if( _eFlags == eFGT_Remove )
		{
			return SourceClose( *_plCookie );
		}

		int nTakeType = (_eFlags & eFGT_FlushBuffers) ? -1 : 1;
		
		// Each receiver should receive exact one frame
		MReader::TPtr pReader;
		while( true )
		{
			if( *_plCookie == 0 )
			{
				// VVB: Temporary increase output buffers to 4 for allow Live FRC conversion 
				// TODO: Check FRC conversion, make less delay
				int nBuffers = _nMaxBuffers > 0 ? _nMaxBuffers : (_eFlags & eFGT_Sync) ? 1 : pT->MPropsXML()->GetIntAttribute( L"output.buffers", 4 );
				MReader::CreateInstance( &pReader, nBuffers, (_eFlags & eFGT_Sync) ? true : false );
				*_plCookie = (LONGLONG)pReader.p; // Just for have unique ID

				CAutoRWS rw( m_rwMap, true );

				while( !m_mapReaders.insert( make_pair( *_plCookie, pReader) ).second )
				{
					(*_plCookie)++;
				}

// 				pReader->m_cpReadyFrame = m_cpFrameCache;
// 				if( pReader->m_cpReadyFrame )
// 					::SetEvent(pReader->m_shHaveFrame);
// 					

				MTRACE( "Add New Reader:%I64X Total:%zd Flags:%04X", (*_plCookie), m_mapReaders.size(), _eFlags );

				::SetEvent( m_shHaveReaders );

				// Reset flush
				// Used e.g. than internal items disconnected Playlits::ItemSetContainer(NULL)->Source_Flush()
				::ResetEvent( m_shFlush );
			}
			else
			{
				CAutoRWS rw( m_rwMap, false );

				TMapReaders::iterator Iter = m_mapReaders.find( *_plCookie );
				if( Iter == m_mapReaders.end() )
				{
					if( _plCookie == &m_llBaseCookie )
					{
						m_llBaseCookie = 0;
						continue;
					}
				
					return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
				}

				pReader = Iter->second;
			}

			break;
		}

		if( _ppFrame )
		{
			//pReader->m_clockReader.Reset();

			// TODO: Pass flags
			HRESULT hr = pT->Source_OnFrameGet( NULL );
			if( FAILED( hr ) )
			{
				MTRACE_WARN("(%08X,%04X) ReaderID:%08X SKIPPED - Source_OnFrameGet() FAILED hr=%08X", 
					(ULONG)this, GetCurrentThreadId(), (int)*_plCookie, hr );

				return hr;
			}


			DWORD dwMaxWaitLimit = 1000;
#ifdef _DEBUG
			dwMaxWaitLimit = 100000;
#endif
			REFERENCE_TIME rtRealWait = 0;
			HANDLE phWait[4] = { pReader->m_shHaveFrame, m_shFlush, m_shEOS }; 
			DWORD dwRes = pReader->m_timer.WaitAnyObjects_WithTime( 3, phWait, _rtMaxWaitTime, &rtRealWait, dwMaxWaitLimit );
			if( dwRes != WAIT_OBJECT_0  )
			{
				MTRACE_WARN( "(%08X,%04X) ReaderID:%08X (%s) - dwRes=%d TIMEOUT/EOS/ABORT", 
					(ULONG)this, GetCurrentThreadId(), (int)*_plCookie, RT2STR( _rtMaxWaitTime ), dwRes );

				return dwRes == WAIT_OBJECT_0 + 1 ? E_ABORT : 
					dwRes == WAIT_OBJECT_0 + 2 ? HRESULT_FROM_WIN32( ERROR_END_OF_MEDIA ) : 
					HRESULT_FROM_WIN32( ERROR_TIMEOUT );
			}

			CBasicPerfMSRC perf2( L"SourceFrameGetEx(GET)" );

			if( !pReader->TakeFrame( _ppFrame, nTakeType, true ) )
			{
				MTRACE_ERR( "pReader->TakeFrame FAILED (DEVICE CLOSE ?)" );
				return E_ABORT;
			}

			REFERENCE_TIME rtAfterPut = m_clockPut.GetTime();
			//REFERENCE_TIME rtWait = pReader->m_clockReader.GetTime();
			MFSideData_DataSet((*_ppFrame), L"AFPT", rtAfterPut);
			MFSideData_DataSet((*_ppFrame), L"WAIT", rtRealWait);
			MFSideData_DataSet((*_ppFrame), L"_reader_deq_", pReader->DeqSize());
			
			
		}
		
		return S_OK;
	}

	// Next methods for source objects (like live auto-destroy)
	HRESULT Source_OnInit()	{ return S_FALSE; };
	HRESULT Source_OnClose( int _nReceivers ) { return S_FALSE; };
	HRESULT Source_OnFrameGet( BSTR _bsPropsList ) { return S_OK; };

	STDMETHOD(SourceInit)(/* [in,out]*/ LONGLONG* _plCookie )
	{
		ATLASSERT( _plCookie );
		if( !_plCookie ) return E_POINTER;

		{
			bool bNewReceiver = false;
			HRESULT hr = SourceFrameGetEx( _plCookie, 0, (IMFrame**)NULL, eFGT_Normal );
			if( FAILED( hr ) )
			{
				ATLTRACE2( atlTraceGeneral, 2, _T("WARNING !!! IMSourceImpl(%08X,%04X)::SourceInit() - SourceFrameGetEx() FAILED hr=%08X\n"), hr );
			}

			CAutoRWS rw( m_rwMap, true );

			TMapReaders::iterator Iter = m_mapReaders.find( *_plCookie );
			ATLASSERT( Iter != m_mapReaders.end() );
			if( Iter != m_mapReaders.end() )
			{
				Iter->second->m_bNotAutoClose = true;
			}

			m_cpFrameCache.Release();
		}
		
		T* pT = static_cast<T*>(this);

		return pT->Source_OnInit();
	}

	STDMETHOD(SourceClose)( /*[in]*/ LONGLONG _lCookie )
	{
		int nReceivers = 0;
		{
			CAutoRWS rw( m_rwMap, true );

			TMapReaders::iterator Iter = m_mapReaders.find( _lCookie != 0 ? _lCookie : m_llBaseCookie );
			if( Iter == m_mapReaders.end() )
				return HRESULT_FROM_WIN32( ERROR_NOT_FOUND );

			::SetEvent( Iter->second->m_shHaveSlot );
			::SetEvent( Iter->second->m_shHaveFrame );

			m_mapReaders.erase( Iter );

			if( m_mapReaders.size() == 0)
				::ResetEvent( m_shHaveReaders );

			if( _lCookie == 0 || _lCookie == m_llBaseCookie)
				m_llBaseCookie = 0;

			nReceivers = (int)m_mapReaders.size();
		}

		T* pT = static_cast<T*>(this);

		return pT->Source_OnClose( nReceivers );
	}


	STDMETHOD(SourceRefAdd)( /*[in]*/ DWORD _dwProcessID )
	{
		return _IMFExternalImpl::ExternalRefAdd(_dwProcessID);
	}

	STDMETHOD(SourceRefRemove)( /*[in]*/ DWORD _dwProcessID )
	{
		return _IMFExternalImpl::ExternalRefRemove(_dwProcessID);
	}

	STDMETHOD(SourceHaveRefs)( /*[out]*/ LONGLONG* _phWaitHandle )
	{
		return _IMFExternalImpl::ExternalHaveRefs(_phWaitHandle);
	}

// 	CComAutoCriticalSection	m_csProcess;
// 	typedef map<DWORD, pair<HANDLE,volatile long> > TMapProcess;
// 	TMapProcess	m_mapProcess;
// 	CStdHandle  m_shWaitHandle;
// 
// 	// For auto destroy live objects
// 	STDMETHOD(SourceRefAdd)( /*[in]*/ DWORD _dwProcessID )
// 	{
// 		CAutoCS crs(m_csProcess);
// 
// 		if( !m_shWaitHandle )
// 		{
// 			m_shWaitHandle.Attach( ::CreateEvent( NULL, TRUE, FALSE, NULL ) );
// 
// 			_beginthread( CheckProcess_Thread_C, 0, this );
// 		}
// 
// 		TMapProcess::iterator Iter = m_mapProcess.find( _dwProcessID );
// 		if( Iter == m_mapProcess.end() )
// 		{
// 			HANDLE hProcess = ::OpenProcess( SYNCHRONIZE, FALSE, _dwProcessID );
// 
// 			// New process
// 			m_mapProcess.insert( make_pair( _dwProcessID, make_pair(hProcess, 1) ) );
// 			
// 			return S_OK;
// 		}
// 
// 		::InterlockedIncrement( &Iter->second.second );
// 		
// 		return S_FALSE;
// 	}
// 
// 	STDMETHOD(SourceRefRemove)( /*[in]*/ DWORD _dwProcessID )
// 	{
// 		CAutoCS crs(m_csProcess);
// 
// 		TMapProcess::iterator Iter = m_mapProcess.find( _dwProcessID );
// 		if( Iter == m_mapProcess.end() )
// 			return HRESULT_FROM_WIN32( ERROR_NOT_FOUND );
// 
// 		ATLASSERT( m_shWaitHandle );
// 
// 		if( ::InterlockedDecrement( &Iter->second.second ) <= 0 )
// 		{
// 			::CloseHandle( Iter->second.first );
// 			m_mapProcess.erase( Iter );
// 		}
// 
// 		if( m_mapProcess.size() == 0 )
// 		{
// 			::SetEvent( m_shWaitHandle );
// 		}
// 
// 		return S_OK;
// 	}
// 
// 	STDMETHOD(SourceHaveRefs)( /*[out]*/ LONGLONG* _phWaitHandle )
// 	{
// 		CAutoCS crs(m_csProcess);
// 
// 		// Check what all processes is alive
// 		TMapProcess::iterator Iter = m_mapProcess.begin();
// 		while( Iter != m_mapProcess.end() )
// 		{
// 			ATLASSERT( Iter->second.second > 0 );
// 
// 			if( ::WaitForSingleObject( Iter->second.first, 0 ) != WAIT_TIMEOUT )
// 			{
// 				::CloseHandle( Iter->second.first );
// 				Iter = m_mapProcess.erase( Iter );
// 			}
// 			else
// 			{
// 				Iter++;
// 			}
// 		}
// 
// 		if( m_mapProcess.size() == 0 )
// 		{
// 			// We can make SourceHaveRefs() BEFORE SourceRefAdd()
// 			if( m_shWaitHandle.IsValid() )
// 				::SetEvent( m_shWaitHandle );
// 
// 			return S_FALSE;
// 		}
// 
// 		if( _phWaitHandle )
// 			*_phWaitHandle = (LONGLONG)m_shWaitHandle.m_hHandle;
// 
// 		return S_OK;
// 	}
// 
// 	static void __cdecl CheckProcess_Thread_C(LPVOID lpThreadParameter )
// 	{
// 		_IMSourceImpl* pThis = (_IMSourceImpl*)lpThreadParameter;
// 		ATLASSERT( pThis );
// 		while( pThis->SourceHaveRefs(NULL) == S_OK )
// 		{
// 			Sleep(100);
// 		}
// 	}

	// This is for init/close 
	// TODO: Move to other file
// 	CComAutoCriticalSection		m_csInitCount;
// 	typedef set<LONG_PTR>		TSetInit;
// 	TSetInit					setInit;
// 
// 	// Return true for first call
// 	bool Source_OnInit( IUnknown* _pReceiver )
// 	{
// 		CAutoCS crs( m_csInitCount );
// 
// 		setInit.insert( (LONG_PTR)_pReceiver );
// 
// 		return setInit.size() > 1 ? false : true;;
// 	}
// 
// 	bool Source_OnClose( IUnknown* _pReceiver )
// 	{
// 		CAutoCS crs( m_csInitCount );
// 
// 		setInit.erase( (LONG_PTR)_pReceiver );
// 
// 		return setInit.size() == 0 ? true : false;
// 	}

private:

};


//////////////////////////////////////////////////////////////////////////
// Pull model impl.

template <class T>
class CPullImpl
{
	CSimpleThread<CPullImpl<T> >		m_theThread;

	CComAutoCriticalSection m_csSource;
	CComPtr<IUnknown>		m_cpMSource;
	CComBSTR				m_cbsChannelID;
	CComBSTR				m_cbsSourceName; // For reconnect

	DWORD					m_dwNoVideoID;
	CComPtr<IMFrame_I>		m_cpFrameNoVideoOrg;

	CComPtr<IStream>			m_cpMarshalStream;
public:

	CPullImpl()
		: m_dwNoVideoID( 0 )
	{
	}

	~CPullImpl()
	{
		Pull_Stop();
	}

	void SetNoVideoID( DWORD _dwNoVideoID )
	{
		m_dwNoVideoID = _dwNoVideoID;
	}


	HRESULT Pull_Start( IUnknown* _pSource, LPCWSTR _pszChannelID)
	{
		if( !_pSource )
		{
			Pull_Stop();
			return S_OK;
		}

		ATLTRACE2( atlTraceGeneral, 8, _T("CPullImpl(0x%08X)::Pull_Start(%ws)\n"), this, _pszChannelID );


		{
			CAutoCS crs( m_csSource );
			m_cpMSource = _pSource;
			m_cbsSourceName = MF_SourceName(_pSource);
			m_cbsChannelID = STR_IS_EMPTY(_pszChannelID) ? m_cbsSourceName : _pszChannelID;
			
			if( m_cpMarshalStream )
			{
				CComPtr<IMSource> cpMSource;
				MUnmarshallInterface( m_cpMarshalStream, &cpMSource );
				m_cpMarshalStream.Release();
			}

			MMarshallInterface( _pSource, &m_cpMarshalStream );
		}

		if( !m_theThread.IsRunning() )
			m_theThread.Start( this );

		m_theThread.Awake();
		return S_OK;
	}

	void Pull_Stop()
	{
		ATLTRACE2( atlTraceGeneral, 8, _T("CPullImpl(0x%08X)::Pull_Stop()\n"), this);

		m_theThread.Stop(INFINITE, true);
		
		{
			CAutoCS crs( m_csSource );
			m_cpMSource.Release();

			if( m_cpMarshalStream )
			{
				CComPtr<IMSource> cpMSource;
				MUnmarshallInterface( m_cpMarshalStream, &cpMSource );
				m_cpMarshalStream.Release();
			}
		}
	}

	HRESULT RunThread()
	{
		T* pT = static_cast<T*>(this);

		HRESULT hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
		ATLASSERT(hr == S_OK);

		LONGLONG		llCookie = 0;
		DWORD			dwNoVideoID = 0;
		MConverterRC2	vidConverter;

		//CComPtr<IMFrame_I> cpLastFrame;
		bool bSetBreak = true;

		WCHAR szBuffer[512] ={};

		long lConnectCount = 0;

		while(m_theThread.m_bWork)
		{
			DWORD dwRet = ::WaitForSingleObject(m_theThread.m_hEvent, INFINITE);
			if(!m_theThread.m_bWork)
				break;

			ATLASSERT(dwRet == WAIT_OBJECT_0);

			CComPtr<IUnknown> cpSource;
			CComBSTR		cbsChannelID;
			CComBSTR		cbsSourceName;
			{
				CAutoCS crs(m_csSource );

				cpSource = m_cpMSource;
				cbsChannelID = m_cbsChannelID;
			}

			if (!cpSource && ++lConnectCount % 30 == 0 )
			{
				MF_SourceOpen(m_cbsSourceName, &cpSource);
				if (cpSource)
				{
					CAutoCS crs(m_csSource);

					m_cpMSource = cpSource;
					llCookie = 0;

					MTRACE("Source(%ws) RECONNECTED", m_cbsSourceName.m_str);
				}
			}

			// Do something
			CComPtr<IMFrame> cpMFrame;
			HRESULT hr = S_FALSE;
			if( cpSource )
			{
				WCHAR sz[128] = {};
				wsprintf( sz, L"cpSource->SourceFrameGetEx(TID:%04X)", GetCurrentThreadId() );
				CBasicPerfMSRC perf( sz );
				hr = MF_SourceFrameGetEx(cpSource, &llCookie, &cpMFrame, 100 * DS_MSEC, eFGT_Normal);
				//hr = cpSource->SourceFrameGet(0, &cpMFrame, 0 );
			}

			if( hr == HRESULT_FROM_WIN32(ERROR_TIMEOUT) )
			{
				MTRACE_WARN("cpSource->SourceFrameGetEx(%ws, %I64d) - ERROR_TIMEOUT", m_cbsSourceName.m_str, llCookie);
				m_theThread.Awake();
				continue;
			}

			if (hr == E_ABORT)
			{
				// E.g. flushing
				MTRACE_WARN("cpSource->SourceFrameGetEx(%ws, %I64d) - E_ABORT", m_cbsSourceName.m_str, llCookie);
				m_theThread.Awake();

				Sleep(10);
				continue;
			}
			
			if( hr == HRESULT_FROM_WIN32(ERROR_NOT_FOUND) )
			{
				MTRACE_WARN("cpSource->SourceFrameGetEx(%ws, %I64d) - ERROR_NOT_FOUND", m_cbsSourceName.m_str, llCookie);

				// Device changed
				llCookie = 0;
				m_theThread.Awake();
				continue;
			}

			if( hr == HRESULT_FROM_WIN32(ERROR_END_OF_MEDIA) )
			{
				MTRACE("cpSource->SourceFrameGetEx(%ws, %I64d) - ERROR_END_OF_MEDIA", m_cbsSourceName.m_str, llCookie);

				// End of media
				hr = pT->OnChannelEOS( m_cbsChannelID, -1 );
				
				bSetBreak = true;
				m_theThread.Awake();
				Sleep(10);
				continue;
			}

			if( hr == RPC_E_WRONG_THREAD ) // 0x8001010e
			{
				MTRACE("cpSource->SourceFrameGetEx(%ws, %I64d) - RPC_E_WRONG_THREAD", m_cbsSourceName.m_str, llCookie);

				CAutoCS crs(m_csSource );
				if( m_cpMarshalStream )
				{
					cpSource.Release();
					hr = MUnmarshallInterface( m_cpMarshalStream, &cpSource );
					ATLASSERT( SUCCEEDED( hr ) );
					if( SUCCEEDED( hr ) )
					{
						m_cpMSource = cpSource;
					}
				}
			}
			else if (FAILED(hr))
			{
				MTRACE_ERR("cpSource->SourceFrameGetEx(%ws, %I64d)=%08X", m_cbsSourceName.m_str, llCookie, hr);

				m_cpMSource.Release();
			}
			
			

			if( !cpMFrame )
			{
				bSetBreak = true;

				if( dwNoVideoID != m_dwNoVideoID )
				{
					dwNoVideoID = m_dwNoVideoID;
					m_cpFrameNoVideoOrg.Release();
				}

				if( dwNoVideoID && !m_cpFrameNoVideoOrg )
				{
					// No video frame
					LPBYTE pbImageTask = NULL;
					BITMAPINFOHEADER bmiHeader ={0};
					hr = CPicturePersist::LoadFromResource( _Module.GetResourceInstance(), dwNoVideoID, _T("CUSTOM"), &bmiHeader, &pbImageTask );
					ATLASSERT( SUCCEEDED( hr ) );
					if( pbImageTask )
					{
						
						MCreator::FramesCreate_I( bmiHeader.biSizeImage, 0, NULL, &m_cpFrameNoVideoOrg );
						ATLASSERT( m_cpFrameNoVideoOrg );
						if( m_cpFrameNoVideoOrg ) 
						{
							M_AV_PROPS avProps = {};
							avProps.vidProps.fccType = bmiHeader.biBitCount == 32 ? eMFCC_RGB32 : eMFCC_RGB24;
							avProps.vidProps.nHeight = bmiHeader.biHeight;
							avProps.vidProps.nWidth = bmiHeader.biWidth;
							avProps.vidProps.nRowBytes = Img_RowSizeM( &avProps.vidProps );

							m_cpFrameNoVideoOrg->FrameAVPropsSet( &avProps );

							M_TIME mTime = {};
							SET_BREAK( &mTime, TRUE );
							SET_LIVE( &mTime, TRUE );

							m_cpFrameNoVideoOrg->FrameTimeSet( &mTime );

							LONGLONG pbVideo = NULL;
							m_cpFrameNoVideoOrg->FrameVideoGetBytes( NULL, &pbVideo );
							::CopyMemory( (LPBYTE)pbVideo, pbImageTask, bmiHeader.biSizeImage );
						}


						::CoTaskMemFree(pbImageTask);
					}
				}

				if( m_cpFrameNoVideoOrg )
				{
					CComQIPtr<IMFormat> qpMFormat( cpSource );
					if( qpMFormat )
					{
						M_VID_PROPS vidProps = {};
						qpMFormat->FormatVideoGet( eMFT_Input, &vidProps, NULL, NULL );

						if( IsSpecified( &vidProps ) )
							vidConverter.ConvertFrame_Format( m_cpFrameNoVideoOrg, NULL, &vidProps, 2, (IMFrame_I**)&cpMFrame, (XMLParse::XMLNodePtr)NULL );
						else
							m_cpFrameNoVideoOrg->FrameClone( &cpMFrame, eMFC_Reference, eMFCC_Default );

						ATLASSERT( cpMFrame );
						if( cpMFrame )
						{
							M_TIME mTime = {};
							SET_LIVE( &mTime, true );
							SET_BREAK( &mTime, true );

							cpMFrame->FrameTimeSet( &mTime );
						}
					}
				}
			}
			else if( bSetBreak )
			{
				M_TIME mTime = {};
				cpMFrame->FrameTimeGet( &mTime );
				SET_BREAK( &mTime, true );
				cpMFrame->FrameTimeSet( &mTime );

				bSetBreak = false;
			}
// 			else
// 			{
// 				if( Frames_IsSame( cpLastFrame, cpMFrame) )
// 				{
// 					CComPtr<IMFrame> cpMFrameNoAudio;
// 					cpMFrame->FrameClone( &cpMFrameNoAudio, eMFC_VideoRef_Audio, eMFCC_Default );
// 					if( cpMFrameNoAudio )
// 					{
// 						LONG cbAudio = 0;
// 						LPBYTE pbAudio = NULL;
// 						cpMFrameNoAudio->FrameAudioGetBytes( &cbAudio, (LONG_PTR*)&pbAudio );
// 						if( pbAudio && cbAudio > 0 )
// 						{
// 							::ZeroMemory( pbAudio, cbAudio );
// 							cpMFrame = cpMFrameNoAudio;
// 						}
// 					}
// 				}
// 
// 				cpLastFrame = cpMFrame;
// 			}


			if( cpMFrame )
			{
				ATLTRACE2( atlTraceGeneral, 2, _T("CPullImpl(%08X)::RunThread() Cookie:%d OnReceiveAV(%ws) \n"), this,
					(LONG)llCookie, MFFrame_Debug(cpMFrame, szBuffer) );

				CBasicPerfD perfRcv( L"CPullImpl::OnReceiveAV" );

				CComQIPtr<IMFrame_I> qpMFrame_I( cpMFrame );
				ATLASSERT( qpMFrame_I && "CPullImpl()::RunThread() - QI" );
				HRESULT hr =  pT->OnReceiveAV( cbsChannelID, qpMFrame_I );
				if( FAILED( hr ) )
				{
					ATLTRACE2( atlTraceGeneral, 2, _T("###Err### CPullImpl(%08X)::RunThread() OnReceiveAV() FAILED hr=0x%08X \n"), this,
						hr );
				}

				if( hr != HRESULT_FROM_WIN32( ERROR_END_OF_MEDIA ) )
					m_theThread.Awake();

				continue;
			}
			else
			{
				bSetBreak = true;
				Sleep(10);
			}

			m_theThread.Awake();
			continue;
		}

		CAutoCS crs(m_csSource );
	
		// Remove reader
		if( m_cpMSource && llCookie > 0 )
		{
			CComPtr<IMFrame> cpMFrameFake;
			MF_SourceFrameGetEx(m_cpMSource, &llCookie, &cpMFrameFake, -1, eFGT_Remove);
		}

		::CoUninitialize();
		return S_OK;
	}
};


// class CPullSink:
// 	public CPullImpl<CPullSink>,
// 	public IMSourceImpl<CPullSink>
// {
// public:
// 	HRESULT OnReceiveAV( BSTR _bsChannelID, IMFrame* _pFrame )
// 	{
// 		return Source_PutFrame( _pFrame );
// 	}
// };
// 
// class CPullMultiplexor
// 	: public IMSource
// {
// 	typedef map<CComPtr<IMSource>, CPullSink*> TMapSinks;
// 
// 	CAutoRWL		m_rwSinks;
// 	TMapSinks	m_mapSinks;
// 
// public:
// 
// 	HRESULT Pull_Start( IMSource* _pSource, LPCWSTR _pszChannelID = NULL )
// 	{
// 		return S_OK;
// 	}
// 
// 	void Pull_Stop()
// 	{
// 	}
// };