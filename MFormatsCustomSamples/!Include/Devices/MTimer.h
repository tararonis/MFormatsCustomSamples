//---------------------------------------------------------------------------
// This file is part of the Medialooks MPlatform 
//
// MTimer.h 
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

#include "../Base/MFunctions.h"
#include "../WinAPI/threads.h"
#include "../WinAPI/wrappers.h"

#include <map>
using std::map;

#pragma warning(push)
#pragma warning(disable:4189)

// TODO: Check http://www.windowstimestamp.com/description
class MClock 
	: public AutoObjBase<MClock, IUnknown>
{
	LARGE_INTEGER m_liFreq;
	LARGE_INTEGER m_liStart;

	// For speed-up settings
	LONGLONG	  m_llSpeedStartClock;
	LONGLONG	  m_llSpeedStartOut;
	double		  m_dblSpeedUp;

	// For correct QPC - to have always monitionic results (see _GetTime() )
	LONGLONG	  m_llLastCounter; // For monotonic results


	LONGLONG	  m_rtStepStart;		
	

	volatile CComAutoCriticalSection*	 m_pSpeedCS;
public:
	MClock()
		: m_rtStepStart( 0 ),
		  m_llLastCounter( -1 ),
		  m_dblSpeedUp( 0.0 ),
		  m_llSpeedStartClock( -1 ),
		  m_llSpeedStartOut( 0 ),
		  m_pSpeedCS( NULL )
	{
		::QueryPerformanceFrequency( &m_liFreq );
		::QueryPerformanceCounter( &m_liStart );
	}

	~MClock()
	{
		if (m_pSpeedCS)
			delete m_pSpeedCS;
	}

	void Reset( REFERENCE_TIME _rtSetTime = 0 )
	{
		LARGE_INTEGER liStart = {};
		::QueryPerformanceCounter( &liStart );
		liStart.QuadPart -= _RT2Time( _rtSetTime).QuadPart;

		m_liStart = liStart;
		m_llSpeedStartOut = m_llSpeedStartClock = _RT2Time(_rtSetTime).QuadPart;
		
		m_rtStepStart = _rtSetTime; 
	}

	// If _rtResetTime is MINLONGLONG -> continue from last time with changed rate (default)
	void SpeedUp_Set(double _dblSpeedUp, REFERENCE_TIME _rtResetTime = MINLONGLONG )
	{
		// Lazy create CS
		if (!m_pSpeedCS)
		{
			CComAutoCriticalSection *pSpeedCS = new CComAutoCriticalSection();
			if (::InterlockedCompareExchangePointer((volatile PVOID*)&m_pSpeedCS, pSpeedCS, NULL) != NULL)
			{
				// Other thread already set -> delete new obe
				delete pSpeedCS;
			}
		}

		CAutoCS	crs(m_pSpeedCS);
		
		if (_rtResetTime != MINLONGLONG)
		{
			Reset(_rtResetTime != MINLONGLONG);
		}
		else if (m_dblSpeedUp != _dblSpeedUp || m_llSpeedStartOut == 0 || m_llSpeedStartClock == 0)
		{
			LONGLONG llClock = _GetTime();
			m_llSpeedStartOut = _CorrectedTime(llClock, m_dblSpeedUp);
			m_llSpeedStartClock = llClock;
		}
		
		m_dblSpeedUp = _dblSpeedUp;
	}

	double SpeedUp_Get() const { return m_dblSpeedUp; }
	
	REFERENCE_TIME GetTime( REFERENCE_TIME* _prtOriginalTime = NULL)
	{
		LONGLONG llClock = _GetTime();

		// Return original time
		if(_prtOriginalTime )
			*_prtOriginalTime = _Time2RT(llClock);

		// Correct time
		llClock = _CorrectedTime(llClock, m_dblSpeedUp);

		// Convert to REFTIME
		return _Time2RT(llClock);
	}

	REFERENCE_TIME GetStep( bool _bResetStep, REFERENCE_TIME* _prtNow = NULL )
	{
		REFERENCE_TIME rtNow = GetTime();
		REFERENCE_TIME rtStep = rtNow - m_rtStepStart;
		if( _bResetStep )
			m_rtStepStart = rtNow;

		if( _prtNow )
			*_prtNow = rtNow;

		return rtStep;
	}

	REFERENCE_TIME LogTime(LPCTSTR pchPrefix, bool bResetStep = true)
	{
		REFERENCE_TIME rtNow = 0;
		REFERENCE_TIME rtStep = GetStep( bResetStep, &rtNow );

		ATLTRACE2(L"%s: Step:%ws Time:%ws\n", 
			pchPrefix ? pchPrefix : L"Clock", RT2STR( rtStep ), RT2STR( rtNow ) );

		return rtStep;
	}

	// Seems do not used anymore - check and remove 
	// UPD: Uses in MCaptureSCR.h -> RunThreadDXGI() for logging.
	inline REFERENCE_TIME Counter2RT( ULONGLONG _ullPerfCounter )
	{
		return DBL2INT64(((double)_ullPerfCounter * DS_ONESEC) / m_liFreq.QuadPart);
	}

private:

	inline LONGLONG _GetTime()
	{
		LARGE_INTEGER liTime = {};
		{
			// Correct for monotonic results (WARNING !!! TOO FAST calls)
			::QueryPerformanceCounter(&liTime);
			liTime.QuadPart = MAX(m_llLastCounter + 1, liTime.QuadPart);
			m_llLastCounter = liTime.QuadPart;
		}

		// Correct to start time point
		liTime.QuadPart -= m_liStart.QuadPart;

		return liTime.QuadPart;
	}

	inline LONGLONG _CorrectedTime(LONGLONG _llClock, double _dblSpeedUp = 1.0)
	{
		CAutoCS crs(m_pSpeedCS); // Is NULL for normal speed

		if (_dblSpeedUp > 0 && m_llSpeedStartClock != 0)
		{
			_llClock = m_llSpeedStartOut + DBL2INT64((_llClock - m_llSpeedStartClock) * _dblSpeedUp);
		}

		return _llClock;
	}

	inline REFERENCE_TIME _Time2RT(LONGLONG _llTime)
	{
		// Return original value
		return DBL2INT64(((double)_llTime * DS_ONESEC) / m_liFreq.QuadPart);
	}

	inline LARGE_INTEGER _RT2Time( REFERENCE_TIME _rtTime )
	{
		ATLASSERT( m_liStart.QuadPart );
		LARGE_INTEGER liRes = {};
		liRes.QuadPart = DBL2INT64(((double)_rtTime * m_liFreq.QuadPart) / DS_ONESEC);

		return liRes;
	}
};

class MClock_Fake
{
public:
	MClock_Fake()
	{
	}

	inline void Reset( REFERENCE_TIME _rtSetTime = 0 )	{}

	inline REFERENCE_TIME GetTime()	{ return 0;	}

	inline REFERENCE_TIME LogTime( LPCTSTR pchPrefix, bool bResetStep = true ) { return 0; }
};

#ifdef _DEBUG 
#define MClockD MClock
#else
#define MClockD MClock_Fake
#endif

class MTimer
{
	MClock m_clock;
	HANDLE m_hTimer;
public:

	MTimer( bool _bAllocAtFirstWait = false )
		: m_hTimer( NULL )
	{
		if( !_bAllocAtFirstWait )
			m_hTimer = ::CreateWaitableTimer( NULL, TRUE, NULL );
	}
	~MTimer()
	{
		if( m_hTimer )
			::CloseHandle( m_hTimer );
	}

	void Close()
	{
		if( m_hTimer )
			::CloseHandle( m_hTimer );
		m_hTimer = NULL;
	}
	
	HRESULT WaitTime( REFERENCE_TIME _rtWait, REFERENCE_TIME* _pRealWait = NULL, bool _bUpdateTimePeriod = false )
	{
		if( _rtWait <= 0 )
		{
			if( _pRealWait )
				*_pRealWait = -1;

			return S_FALSE;
		}

		if( !m_hTimer ) // For lazy alloc
			m_hTimer = ::CreateWaitableTimer( NULL, TRUE, NULL );

		ATLASSERT(m_hTimer);

		m_clock.Reset();

		if( _bUpdateTimePeriod )
			timeBeginPeriod(1);

		REFERENCE_TIME rtWait = -1 * _rtWait;
		::SetWaitableTimer( m_hTimer, (LARGE_INTEGER*)&rtWait, 0, NULL, NULL, FALSE );
		DWORD dwRes = ::WaitForSingleObject( m_hTimer, (_rtWait / DS_MSEC) * 2 + 1000 );
		ATLASSERT( dwRes == WAIT_OBJECT_0 );

		if( _bUpdateTimePeriod )
			timeEndPeriod(1);

		if( _pRealWait )
			*_pRealWait = m_clock.GetTime();

		return dwRes == WAIT_OBJECT_0 ? S_OK : HRESULT_FROM_WIN32( ERROR_TIMEOUT );
	}

	DWORD WaitAnyObjects_WithTime( int _nWaitCount, HANDLE* _phWait, REFERENCE_TIME _rtWait, REFERENCE_TIME* _pRealWait = NULL, DWORD _dwNoTimerWaitLimit = INFINITE )
	{
		m_clock.Reset();

		HANDLE* phWait = _phWait;
		if( _rtWait > 0 )
		{
			if( !m_hTimer ) // For lazy alloc
				m_hTimer = ::CreateWaitableTimer( NULL, TRUE, NULL );
			ATLASSERT( m_hTimer );

			REFERENCE_TIME rtWait = -1 * _rtWait;
			::SetWaitableTimer( m_hTimer, (LARGE_INTEGER*)&rtWait, 0, NULL, NULL, FALSE );

			phWait = (HANDLE*)alloca( (_nWaitCount + 1) * sizeof(HANDLE) );
			for( int i = 0; i < _nWaitCount; i++ )
				phWait[i] = _phWait[i];

			phWait[_nWaitCount] = m_hTimer;

			_nWaitCount++;
		}

		DWORD dwWaitMsec = _rtWait > 0 ? (_rtWait / DS_MSEC) * 2 + 1000 : _rtWait < 0 ? _dwNoTimerWaitLimit : 0;
		DWORD dwRes = ::WaitForMultipleObjects( _nWaitCount, phWait, FALSE, dwWaitMsec );
		
		if( _pRealWait )
			*_pRealWait = m_clock.GetTime();

		return (_rtWait > 0 && dwRes == WAIT_OBJECT_0 + _nWaitCount - 1) ? WAIT_TIMEOUT : dwRes;
	}

	DWORD WaitMultipleObjects_WithTimeAndCancel( int _nWaitAllCount, HANDLE* _phWaitAllEvents, int _nWaitCancelCount, HANDLE* _phWaitCancelEvents, REFERENCE_TIME _rtWait, REFERENCE_TIME* _pRealWait = NULL )
	{
		m_clock.Reset();

		ATLASSERT(_nWaitCancelCount > 0 ? _phWaitCancelEvents != NULL : _nWaitCancelCount == 0 );
		if (!_phWaitCancelEvents)
			_nWaitCancelCount = 0;

		_nWaitCancelCount = MAX(_nWaitCancelCount, 0);

		HANDLE* phWaitCancel = _phWaitCancelEvents;
		if( _rtWait > 0 )
		{
			if( !m_hTimer ) // For lazy alloc
				m_hTimer = ::CreateWaitableTimer( NULL, TRUE, NULL );
			ATLASSERT( m_hTimer );

			REFERENCE_TIME rtWait = -1 * _rtWait;
			::SetWaitableTimer( m_hTimer, (LARGE_INTEGER*)&rtWait, 0, NULL, NULL, FALSE );

			phWaitCancel = (HANDLE*)alloca( (_nWaitCancelCount + 1) * sizeof(HANDLE) );
			for( int i = 0; i < _nWaitCancelCount; i++ )
				phWaitCancel[i] = _phWaitCancelEvents[i];

			phWaitCancel[_nWaitCancelCount] = m_hTimer;

			_nWaitCancelCount++;
		}

		DWORD dwRes = WaitForMultipleObjects_WithCancel( _nWaitAllCount, _phWaitAllEvents, _nWaitCancelCount, phWaitCancel, _rtWait > 0 ? (_rtWait / DS_MSEC) * 2 + 1000 : _rtWait < 0 ? INFINITE : 0 );

		if( _pRealWait )
			*_pRealWait = m_clock.GetTime();

		return (_rtWait > 0 && dwRes == WAIT_OBJECT_0 + _nWaitCancelCount - 1) ? WAIT_TIMEOUT : dwRes;
	}
};

typedef void (WINAPI *PFOnTimeChanged)( LPVOID _lpCookie, LONGLONG _llCalcTime, LONGLONG _llLocalTime );

class MTimeSpy
{
	CSimpleThread<MTimeSpy>				m_theThread;

	MClock								m_clock;
	REFERENCE_TIME						m_rtMaxDiff;
	int									m_nMSecCheck;

	// For callback	
	// If NULL -> use APC queue (TODO)
	PFOnTimeChanged						m_pfOnChange;
	LPVOID								m_lpCookie;
public:

	MTimeSpy()
	{
		m_pfOnChange = NULL;
		m_lpCookie = NULL;
		m_rtMaxDiff = 0;
		m_nMSecCheck = 0;
	}

	~MTimeSpy()
	{
		StopTimeSpy();
	}

	void StartTimeSpy( PFOnTimeChanged _pfOnChnage, LPVOID _lpCookie, LONGLONG _rtMaxDiff = 100 * DS_MSEC, int _nMSecWait = 1000 )
	{
		m_theThread.Stop();

		m_pfOnChange = _pfOnChnage;
		m_lpCookie = _lpCookie;
		m_rtMaxDiff = _rtMaxDiff;
		m_nMSecCheck = _nMSecWait;

		m_theThread.Start(this);
		m_theThread.Awake();
	}

	void StopTimeSpy()
	{
		m_theThread.Stop();

		m_pfOnChange = NULL;
		m_lpCookie = NULL;
		m_rtMaxDiff = 0;
		m_nMSecCheck = 0;
	}

	HRESULT RunThread()
	{
		HRESULT hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
		ATLASSERT(hr == S_OK);

		LONGLONG llTime = GetLocalTime();
		m_clock.Reset( llTime );

		TCHAR szBuff[4][128] = {};
		while(m_theThread.m_bWork)
		{
			DWORD dwRet = ::WaitForSingleObject(m_theThread.m_hEvent, INFINITE);
			if(!m_theThread.m_bWork)
				break;

			LONGLONG llLocalTime = GetLocalTime();
			LONGLONG llCalcTime = m_clock.GetTime();
			if( m_rtMaxDiff > 0 && ABS(llLocalTime - llCalcTime ) > m_rtMaxDiff )
			{
#ifdef _DEBUG
				{
					SYSTEMTIME sysTimeLocal = {};
					::FileTimeToSystemTime( (FILETIME*)&llLocalTime, &sysTimeLocal );
					SYSTEMTIME sysTimeCalc = {};
					::FileTimeToSystemTime( (FILETIME*)&llCalcTime, &sysTimeCalc );

					ATLTRACE2( atlTraceGeneral, 2, _T("WARNING !!! MTimeSpy::RunThread() - TIME CHANGE DETECTED Diff:%s (Max:%s)"),
						//SysTime2StrW( sysTimeLocal, szBuff[0], true), SysTime2StrW( sysTimeCalc, szBuff[1], true),
						RefTime2Str( llCalcTime - llLocalTime, szBuff[2] ), RefTime2Str( m_rtMaxDiff, szBuff[3] ) );

					::MessageBoxW( NULL, RT2STR( llCalcTime - llLocalTime), L"MTimeSpy", 0 );
				}
#endif

				if( m_pfOnChange )
					m_pfOnChange( m_lpCookie, llCalcTime, llLocalTime );
			}

// 			static int nTest = 0;
// 			if( ++nTest % 10 == 0 )
// 			{
// 				::MessageBoxW( NULL, RT2STR( llCalcTime - llLocalTime), L"MTimeSpy", 0 );
// 			}

			llLocalTime = GetLocalTime();
			m_clock.Reset( llLocalTime );

			Sleep( MAX( 1, m_nMSecCheck ) );

			m_theThread.Awake();
		}

		::CoUninitialize();
		return S_OK;
	}

};



template<class T>
class MTimerCallback
{
	typedef bool (T::*Callback)(LPVOID _lvParam, LONGLONG _llParam);

	class IMTimerCB: IUnknown
	{
	public:
		virtual REFERENCE_TIME Invoke() = 0;
	};

	class MTimerCB:
		public AutoObjBase<MTimerCB,IMTimerCB>
	{
	public:

		static HRESULT CreateInstance(MTimerCB** _ppObject, T* _pObject, REFERENCE_TIME _rtRepeat, Callback _callback, LPVOID _lvParam, LONGLONG _llParam, bool _bAsync)
		{
			ATLASSERT(_ppObject);
			if (!_ppObject) return E_POINTER;

			MTimerCB* pT = new MTimerCB(_pObject, _rtRepeat, _callback, _lvParam, _llParam, _bAsync);
			pT->AddRef();
			*_ppObject = pT;
			return S_OK;
		}

		MTimerCB(T* _pObject, Callback _callback, REFERENCE_TIME _rtRepeat, LPVOID _lvParam, LONGLONG _llParam, bool _bAsync)
			: m_pObject(_pObject), m_rtPeriod(_rtRepeat), m_pCallback(_callback), m_lvParam(_lvParam), m_llParam(_llParam), m_bAsync(_bAsync)
		{
		}

		virtual REFERENCE_TIME Invoke()
		{
			bool bRes = (m_pObject->*m_pCallback)(m_lvParam, m_llParam);

			return bRes ? ABS(m_rtPeriod) : 0;
		}

	private:
		T*			m_pObject;
		Callback	m_pCallback;
		LPVOID		m_lvParam;
		LONGLONG	m_llParam;
		REFERENCE_TIME	m_rtPeriod;
		bool		m_bAsync;
	};

	typedef map<REFERENCE_TIME, CComPtr<IMTimerCB> > TMapEvents;

	CComAutoCriticalSection m_csMap;
	TMapEvents	m_mapEvents;

	T*			m_pObject;

	CSimpleThread<MTimerCallback>	m_theThread;
public:

	// Note: _rtRepeat > 0, the events period is _rtRepeat
	//		 _rtRepeat < 0, the events period is _rtRepeat
	HRESULT AddEvent(T* _pClass, Callback _method,  REFERENCE_TIME _rtStartTime, REFERENCE_TIME _rtRepeat, LPVOID _lvParam = NULL, LONGLONG _llParam = 0, bool _bAsync = false)
	{
		REFERENCE_TIME rtTime = GetLocalTime();

		// Check for relative time
		if (_rtStartTime < 0)
		{
			// Relative time
			_rtStartTime = rtTime + ABS(_rtStartTime);
		}

		if (_rtStartTime < rtTime)
			return HRESULT_FROM_WIN32(ERROR_TIMER_RESUME_IGNORED);

		MTimerCB::TPtr pTimerCB;
		MTimerCB::CreateInstance(&pTimerCB, _pClass, _method, _rtRepeat, _lvParam, _llParam, _bAsync);
		ATLASSERT(pTimerCB);
		if (!pTimerCB) return E_OUTOFMEMORY;

		CAutoCS crs(m_csMap);

		while (!m_mapEvents.insert(make_pair(_rtStartTime, pTimerCB)).second)
		{
			_rtStartTime++;
		}

		if (m_mapEvents.begin()->second.IsEqualObject(pTimerCB))
		{
			// This is first event -> restart timer
			_UpdateTimer();
		}


		return S_OK;
	}

	REFERENCE_TIME NextEvent() 
	{ 
		CAutoCS crs(m_csMap);

		return m_mapEvents.empty() ? -1 : m_mapEvents.begin()->first;
	}

private:

	void _UpdateTimer()
	{

	}
};

#pragma warning(pop)
