#pragma once

// Classes for Average Counters, use in MStat and for fps/average values calculations

#include "../WinAPI/Macros.h"
#include "../WinAPI/wrappers.h"
#include "../Devices/MTimer.h"
#include "../DSTL/ippMerge.h"
#include "../XML/XMLParseM.h"
#include <deque>
using std::deque;

#define DEBUG_COUNTERS( _avgCalc ) ((_avgCalc).DebugCounters(alloca(256), true))

#define DEBUG_COUNTERS_SF( _avgCalc ) ((_avgCalc).DebugCounters((LPWSTR)__STR_BUF, true))

#define DEBUG_COUNTERS_NOFPS( _avgCalc ) ((_avgCalc).DebugCounters(alloca(256), false))

#define DEBUG_COUNTERS_NOFPS_SF( _avgCalc ) ((_avgCalc).DebugCounters((LPWSTR)__STR_BUF, false))

struct AVG_INFO
{
	Ipp64f  average;
	Ipp64f  stddev;
	Ipp64f  min;
	Ipp64f  max;
	int		counters;
};

// Base class for average counters
class MAverageCalc :
	public AutoObjBase<MAverageCalc>
{
	Ipp64f*			m_pRTTimes;
	int				m_nMaxFrames;
	int				m_nCurIndex;

	CRWSynchroSRW	m_rwSync;
public:
	MAverageCalc(int _nAvg = 64)
		: m_nMaxFrames(-1),
		m_nCurIndex(-1),
		m_pRTTimes(NULL)
	{
		SetAverageCounter(_nAvg);
	}

	~MAverageCalc()
	{
		if (m_pRTTimes)
			delete[] m_pRTTimes;
	}

	void SetAverageCounter(int _nAvg)
	{
		CAutoRWS rw(m_rwSync, true);

		_nAvg = MAX(_nAvg, 1);
		if (m_nMaxFrames != _nAvg)
		{
			if (m_pRTTimes)
				delete[] m_pRTTimes;

			m_nMaxFrames = _nAvg;
			m_pRTTimes = new Ipp64f[m_nMaxFrames * 2];
			m_nCurIndex = -1;
		}
	}

	void Reset()
	{
		CAutoRWS rw(m_rwSync, true);

		m_nCurIndex = -1;
	}

	inline int AddValue(Ipp64f _val)
	{
		ATLASSERT(m_pRTTimes);
		if (!m_pRTTimes) return -1;

		int nAnalizeTime = 0;
		{
			CAutoRWS rw(m_rwSync, true);

			ATLASSERT(m_pRTTimes);
			if (!m_pRTTimes) return -1;

			m_nCurIndex++;
			if (m_nCurIndex >= m_nMaxFrames * 2)
			{
				::MoveMemory(m_pRTTimes, m_pRTTimes + m_nMaxFrames, sizeof(Ipp64f) * m_nMaxFrames);
				m_nCurIndex = m_nMaxFrames;
			}

			ATLASSERT(0 <= m_nCurIndex && m_nCurIndex < m_nMaxFrames * 2);
			m_pRTTimes[m_nCurIndex] = _val;

			nAnalizeTime = MIN(m_nMaxFrames, m_nCurIndex) + 1;
		}

		return nAnalizeTime;
	}

	bool IsFull(int* _pnCount = NULL, int* _pnMax = NULL)
	{
		CAutoRWS rw(m_rwSync, false);

		if (!m_pRTTimes)
			return false;

		if (_pnCount)
			*_pnCount = MIN(m_nMaxFrames, m_nCurIndex) + 1;

		if (_pnMax)
			*_pnMax = m_nMaxFrames;

		return m_nCurIndex >= m_nMaxFrames;
	}

	bool IsEmpty(int* _pnCount = NULL, int* _pnMax = NULL)
	{
		CAutoRWS rw(m_rwSync, false);

		if (!m_pRTTimes)
			return true;

		if (_pnCount)
			*_pnCount = MIN(m_nMaxFrames, m_nCurIndex) + 1;

		if (_pnMax)
			*_pnMax = m_nMaxFrames;

		return m_nCurIndex < 0;
	}

	AVG_INFO GetAvgCounters()
	{
		AVG_INFO avgInfo = {};
		avgInfo.counters = GetCounters(&avgInfo.average, &avgInfo.stddev, &avgInfo.min, &avgInfo.max);
		return avgInfo;
	}

	int GetCounters(Ipp64f* _pfAvg = NULL, Ipp64f* _pfStdDev = NULL, Ipp64f* _pfMin = NULL, Ipp64f* _pfMax = NULL)
	{
		CAutoRWS rw(m_rwSync, false);

		ATLASSERT(m_pRTTimes);
		if (!m_pRTTimes || m_nMaxFrames <= 0 || m_nCurIndex < 0) return -1;

		int nAnalizeTime = MIN(m_nMaxFrames, m_nCurIndex) + 1;
		int nStartIdx = MAX(0, m_nCurIndex - m_nMaxFrames);
	
		if( nAnalizeTime > 1 )
		{
			if (_pfAvg || _pfStdDev)
			{
				Ipp64f dblAvgTimeRT = 0;
				Ipp64f dblStdDevRT = 0;
				ippsMeanStdDev_64f(m_pRTTimes + nStartIdx, nAnalizeTime, &dblAvgTimeRT, &dblStdDevRT);
				if (_pfAvg)
					*_pfAvg = dblAvgTimeRT;
				if (_pfStdDev)
					*_pfStdDev = dblStdDevRT;
			}

			if (_pfMin)
				ippsMin_64f(m_pRTTimes + nStartIdx, nAnalizeTime, _pfMin);

			if (_pfMax)
				ippsMax_64f(m_pRTTimes + nStartIdx, nAnalizeTime, _pfMax);
		}
		else
		{
			if (_pfAvg)
				*_pfAvg = m_pRTTimes[nStartIdx];
			if (_pfStdDev)
				*_pfStdDev = 0;
			if (_pfMin)
				*_pfMin = m_pRTTimes[nStartIdx];
			if (_pfMax)
				*_pfMax = m_pRTTimes[nStartIdx];
		}

		return nAnalizeTime;
	}

	LPCWSTR DebugCounters(LPVOID _pszDebug, bool _bFps = true)
	{
		ATLASSERT(_pszDebug);

		Ipp64f dblAvgTimeRT = 0;
		Ipp64f dblStdDevRT = 0;
		Ipp64f dblTimeMin = 0;
		Ipp64f dblTimeMax = 0;
		GetCounters(&dblAvgTimeRT, &dblStdDevRT, &dblTimeMin, &dblTimeMax);

		WCHAR szDebug[128] = {};
		if (_bFps)
		{
			_swprintf((LPWSTR)_pszDebug, L"Avg:%.03f (%.3f fps) Min:%.03f Max:%.03f Jitter:%.03f (msec)", dblAvgTimeRT / DS_MSEC, dblAvgTimeRT > 0 ? (double)DS_ONESEC / dblAvgTimeRT : 0.0,
				dblTimeMin / DS_MSEC, dblTimeMax / DS_MSEC, dblStdDevRT / DS_MSEC);
		}
		else
		{
			_swprintf((LPWSTR)_pszDebug, L"Avg:%.03f Min:%.03f Max:%.03f Jitter:%.03f", dblAvgTimeRT, dblTimeMin, dblTimeMax, dblStdDevRT);
		}

		return (LPCWSTR)_pszDebug;
	}

	int GetUtmostValues(REFERENCE_TIME* _prtFirst, REFERENCE_TIME* _prtLast)
	{
		CAutoRWS rw(m_rwSync, false);

		int nAnalizeTime = MIN(m_nMaxFrames, m_nCurIndex);
		int nStartIdx = MAX(0, m_nCurIndex - m_nMaxFrames);

		if (nAnalizeTime >= 0)
		{
			if (_prtFirst)
				*_prtFirst = m_pRTTimes[nStartIdx];

			if (_prtLast)
				*_prtLast = m_pRTTimes[nStartIdx + nAnalizeTime];
		}

		return nAnalizeTime + 1;
	}

	double GetFps(bool _bFromStartStop)
	{
		if (_bFromStartStop)
		{
			double dblAvgT = 0.0;
			if (GetCounters(&dblAvgT, NULL, NULL, NULL) >= 1)
			{
				return (double)DS_ONESEC / dblAvgT;
			}
		}
		else
		{
			REFERENCE_TIME rtValues[2] = {};
			int nCounts = GetUtmostValues(&rtValues[0], &rtValues[1]);
			if (nCounts > 1 && rtValues[0] != rtValues[1])
			{
				return (double)DS_ONESEC * (nCounts - 1) / (rtValues[1] - rtValues[0]);
			}
		}

		return 0.0;
	}

	double GetAverage(Ipp64f* _pMin = NULL, Ipp64f* _pMax = NULL)
	{
		double dblAvgT = 0.0;
		GetCounters(&dblAvgT, NULL, _pMin, _pMax);
		return dblAvgT;
	}
};

// Can be used in ONE thread ONLY !!!
class MAverageTime
	: public MAverageCalc
{
	MClock m_clock;

	int AddValue(Ipp64f _val) { return MAverageCalc::AddValue(_val); }
public:

	class AutoPerf
	{
	public:
		AutoPerf(MAverageTime* pAvgTime)
			: m_pAvgTime(pAvgTime)
		{
			if (m_pAvgTime)
				m_pAvgTime->StartMark();
		}

		AutoPerf(MAverageTime* pAvgTime, XMLParse::XMLNodePtr _pXMLStat, LPCWSTR _pszStatName)
			: m_pAvgTime(pAvgTime), m_pXMLStat(_pXMLStat), m_cbsStatName(_pszStatName)
		{
			if (m_pAvgTime)
				m_pAvgTime->StartMark();
		}

		void Cancel(bool _bRemoveStat)
		{
			m_pAvgTime = NULL;

			if (_bRemoveStat && m_pXMLStat && m_cbsStatName.Length())
				m_pXMLStat->RemoveAttribute(m_cbsStatName);
		}

		void Done(bool _bOutputStat)
		{
			if (m_pAvgTime)
			{
				m_pAvgTime->StopMark();

				if (_bOutputStat && m_pXMLStat && m_cbsStatName.Length())
					m_pXMLStat->SetAttribute(m_cbsStatName, DEBUG_COUNTERS(*m_pAvgTime));

				m_pAvgTime = NULL;
			}
		}

		~AutoPerf()
		{
			Done(true);
		}

	private:
		MAverageTime*			m_pAvgTime;
		XMLParse::XMLNodePtr	m_pXMLStat;
		CComBSTR				m_cbsStatName;
	};

	typedef CAutoObj<MAverageTime> TPtr;
public:

	MAverageTime(int _nAvg = 30)
		: MAverageCalc(_nAvg)
	{
	}

	// For caluculate executionm time -> use Start() ... Stop()
	inline REFERENCE_TIME StartMark()
	{
		return m_clock.GetStep(true);
	}

	inline REFERENCE_TIME StopMark(Ipp64f _div = 1.0)
	{
		REFERENCE_TIME rtStep = m_clock.GetStep(true);
		AddValue((Ipp64f)rtStep / (_div != 0.0 ? _div : 1.0));
		return rtStep;
	}

	inline REFERENCE_TIME OnTime()
	{
		REFERENCE_TIME rtTime = m_clock.GetTime();
		AddValue((Ipp64f)rtTime);
		return rtTime;
	}

	inline int PutTime(double _dblTimeRT)
	{
		return AddValue((Ipp64f)_dblTimeRT);
	}
};

// For multiple threads
class MAverageTimeMT
	: public MAverageCalc
{
	CRWSynchroSRW	m_rwSync;
	typedef std::map<DWORD, MClock::TPtr>  TMapThreads;

	TMapThreads m_mapThreads;

	int AddValue(Ipp64f _val) { return MAverageCalc::AddValue(_val); }

public:
	typedef CAutoObj<MAverageTimeMT> TPtr;

public:

	MAverageTimeMT(int _nAvg = 30)
		: MAverageCalc(_nAvg)
	{
	}

	MClock::TPtr GetClock(bool _bCreateNew, DWORD _dwThreadId = -1)
	{
		if (_dwThreadId == -1)
			_dwThreadId = GetCurrentThreadId();

		CAutoRWS rw(m_rwSync, true);

		TMapThreads::iterator Iter = m_mapThreads.find(_dwThreadId);
		if (Iter == m_mapThreads.end() && _bCreateNew)
		{
			MClock::TPtr pClock;
			MClock::CreateInstance(&pClock);
			Iter = m_mapThreads.insert(make_pair(_dwThreadId, pClock)).first;
		}

		return Iter == m_mapThreads.end() ? (MClock::TPtr)NULL : Iter->second;
	}

	// For caluculate executionm time -> use Start() ... Stop()
	inline REFERENCE_TIME StartMark(DWORD _dwThreadId = -1)
	{
		MClock::TPtr pClock = GetClock(true);
		if (!pClock)
			return -1;

		return pClock->GetStep(true);
	}

	inline REFERENCE_TIME StopMark(DWORD _dwThreadId = -1)
	{
		MClock::TPtr pClock = GetClock(true);
		if (!pClock)
			return -1;

		REFERENCE_TIME rtStep = pClock->GetStep(true);
		AddValue((Ipp64f)rtStep);
		return rtStep;
	}

	inline REFERENCE_TIME OnTime(DWORD _dwThreadId = -1)
	{
		MClock::TPtr pClock = GetClock(true);
		if (!pClock)
			return -1;

		REFERENCE_TIME rtNow = pClock->GetTime();
		AddValue((Ipp64f)rtNow);
		return rtNow;
	}

	bool FreeThread(DWORD _dwThreadId = -1)
	{
		if (_dwThreadId == -1)
			_dwThreadId = GetCurrentThreadId();

		CAutoRWS rw(m_rwSync, true);

		TMapThreads::iterator Iter = m_mapThreads.find(_dwThreadId);
		if (Iter != m_mapThreads.end())
		{
			m_mapThreads.erase(Iter);
			return true;
		}

		return false;
	}
};

// Designed for calculate bytes per second rate for input/output streams, have options for specify average period (in seconds)
// Logic:
// Keep data size received per period (e.g. one second), based on this calculate bytes/second and make average for last N calls.
// TODO: Do not use different modes, make two different methods e.g. OnData()/OnReceived()
class MBitrateCalculator
	: public MAverageCalc
{
public:

	enum eBitrateMode
	{
		eBM_Packets,	// e.g. packets size, audio samples
		eBM_Received    // e.g. received bytes
	};

	typedef CAutoObj<MBitrateCalculator> TPtr;

private:
	CRWSynchroSRW	m_rwSync;

	// Period for bitrate calculate
	REFERENCE_TIME	m_rtPeriod;
	// For mode = eBM_Received
	LONGLONG		m_llLastValue; 
	// Total data size 
	LONGLONG		m_llDeqSumm;
	// Mode: packets size/received bytes
	eBitrateMode	m_eMode;
	MClock			m_clock;
	// External clock offset, also used as flag for external_clock mode
	// m_rtExtClockOffset == DS_NOPTR -> internal clock used
	REFERENCE_TIME	m_rtExtClockOffset;

	int AddValue(Ipp64f _val) { return MAverageCalc::AddValue(_val); }

	typedef deque<pair<REFERENCE_TIME, LONGLONG> > TDeqValues;
	TDeqValues	m_deqValues;
	
public:

	// _nMode == 0 -> Delta values (e.g. audio samples, packets size)
	// _nMode == 1 -> Increased values (e.g. bytes received)
	MBitrateCalculator(eBitrateMode _eMode = eBM_Packets, int _nAvgPoints = 16, REFERENCE_TIME _rtPeriod = DS_ONESEC)
		: m_rtExtClockOffset(DS_NOPTS)
	{
		SetAverageCounter(_eMode, _nAvgPoints, _rtPeriod);
	}

	void SetAverageCounter(eBitrateMode _eMode, int _nAvgPoints = 16, REFERENCE_TIME _rtPeriod = DS_ONESEC)
	{
		CAutoRWS rw(m_rwSync, true);

		MAverageCalc::SetAverageCounter(_nAvgPoints);
		m_rtPeriod = _rtPeriod;
		m_eMode = _eMode;

		_Reset();
	}

	void Reset()
	{
		CAutoRWS rw(m_rwSync, true);

		_Reset();
	}

	// Mode = 0 -> _cbDataValue is packet size, audio samples 
	// Mode = 1 -> _cbDataValue is increased value (e.g. bytes received etc.)
	// Return average rate
	double OnNextData(LONGLONG _cbDataValue, REFERENCE_TIME _rtExtClock = DS_NOPTS, REFERENCE_TIME* _prtClock = NULL)
	{
		if (_cbDataValue <= 0)
			return GetAverage();

		CAutoRWS rw(m_rwSync, true);

		LONGLONG cbDataSize = _cbDataValue;

		if (m_eMode == eBM_Received)
		{
			if (_cbDataValue < m_llLastValue)
				_Reset();

			if (m_llLastValue != DS_NOPTS)
				cbDataSize -= m_llLastValue;
			
			m_llLastValue = _cbDataValue;
		}
		
		ATLASSERT(cbDataSize >= 0);

		// Check for external/internal clock mode change
		
		REFERENCE_TIME rtClock = 0;
		if (_rtExtClock != DS_NOPTS)
		{
			if (m_rtExtClockOffset == DS_NOPTS)
			{
				// Switch from internal clock mode
				_Reset();

				m_rtExtClockOffset = 0;
			}

			rtClock = _rtExtClock - m_rtExtClockOffset;

			// Get time from last call (used for calculate offset if time of packets changed)
			REFERENCE_TIME rtStep = m_clock.GetStep(true);
			if (m_deqValues.size() && rtClock < m_deqValues.back().first)
			{
				// Change offset: last clock time + time from last call (step)
				rtClock = m_deqValues.back().first + rtStep;
				m_rtExtClockOffset = _rtExtClock - rtClock;
			}
		}
		else
		{
			if (m_rtExtClockOffset != DS_NOPTS)
				_Reset(); // Switch from external clock mode
		
			rtClock = m_clock.GetTime();
		}

		// Check last time
		if (m_deqValues.size() && rtClock <= m_deqValues.back().first)
		{
			// The same clock value (this is could be, too fast call or e.g. packets with same time)
			// Just increase last time data size
			ATLASSERT(rtClock == m_deqValues.back().first);
			m_deqValues.back().second += cbDataSize;
		}
		else
		{
			// Add new pair - time + data size
			m_deqValues.push_back(make_pair(rtClock, cbDataSize));
		}

		// Increase sum by last value (for optimization)
		m_llDeqSumm += cbDataSize;
		ATLASSERT(_DataSumm() == m_llDeqSumm);
		ATLASSERT(m_deqValues.back().first == rtClock);

		// Check for period, remove old values
		while (m_deqValues.size() > 2 && rtClock - m_deqValues.front().first > m_rtPeriod)
		{
			m_llDeqSumm -= m_deqValues.front().second;
			m_deqValues.pop_front();
		}

		if (_prtClock)
			*_prtClock = rtClock;

		double dblRate = _GetMomentRate();
		if (dblRate > 0)
			AddValue(dblRate);

		return GetAverage();
	}

private:
	void _Reset()
	{
		MAverageCalc::Reset();
		m_deqValues.clear();
		m_llLastValue = DS_NOPTS;
		m_rtExtClockOffset = DS_NOPTS;
		m_llDeqSumm = 0;
		m_clock.Reset();
	}

	double _GetMomentRate()
	{
		if (m_deqValues.size() < 2)
			return 0.0;

		// Do not take into account first data size 
		LONGLONG llDataSize = m_llDeqSumm - m_deqValues.front().second;
		REFERENCE_TIME rtTime = m_deqValues.back().first - m_deqValues.front().first;

		ATLASSERT(rtTime >  0);
		if (rtTime > 0)
		{
			return (double)llDataSize * DS_ONESEC / rtTime;
		}

		return 0.0;
	}

	LONGLONG _DataSumm()
	{
		LONGLONG llSumm = 0;
		for (auto iterDeq = m_deqValues.begin(); iterDeq != m_deqValues.end(); ++iterDeq )
			llSumm += iterDeq->second;
		
		return llSumm;
	}

};

// Similar to MBitrateCalculator 
// Looks to be osolete, TODO: remove and switch to MBitrateCalculator
class MRateCalculator
	: public MAverageCalc
{
	CRWSynchroSRW	m_rwSync;

	LONGLONG	 m_llLastValue;
	int			 m_nMode;
	MClock		 m_clock;

	int AddValue(Ipp64f _val) { return MAverageCalc::AddValue(_val); }

public:
	MAverageCalc m_avgTime;
	MAverageCalc m_avgValues;

	typedef CAutoObj<MRateCalculator> TPtr;

public:

	// _nMode == 0 -> Delta values (e.g. audio samples)
	// _nMode == 1 -> Increased values (e.g. bytes received)
	MRateCalculator(int _nAvgFirst = 30, int _nMode = 0, int _nAvgSecond = 8)
		: MAverageCalc(_nAvgSecond),
		m_avgTime(_nAvgFirst),
		m_avgValues(_nAvgFirst),
		m_llLastValue(MINLONGLONG),
		m_nMode(_nMode)
	{
	}

	// _nMode == 0 -> Delta values (e.g. audio samples)
	// _nMode == 1 -> Increased values (e.g. bytes received)
	void SetAverageCounter(int _nAvgFirst, int _nMode, int _nAvgSecond = 8)
	{
		CAutoRWS rw(m_rwSync, true);

		MAverageCalc::SetAverageCounter(_nAvgSecond);
		m_avgTime.SetAverageCounter(_nAvgFirst);
		m_avgValues.SetAverageCounter(_nAvgFirst);
		m_llLastValue = MINLONGLONG;
		m_nMode = _nMode;

		m_clock.Reset();
	}

	void Reset()
	{
		CAutoRWS rw(m_rwSync, true);

		m_avgTime.Reset();
		m_avgValues.Reset();
		m_llLastValue = MINLONGLONG;

		m_clock.Reset();
	}

	// _nMode == 0 -> _llValue = Delta values (e.g. audio samples)
	// _nMode == 1 -> _llValue = Increased values (e.g. bytes received)
	double OnNextData(LONGLONG _llValue, LONGLONG* _pllLastValue = NULL, REFERENCE_TIME* _prtClock = NULL, REFERENCE_TIME _rtClock = MINLONGLONG)
	{
		CAutoRWS rw(m_rwSync, true);

		if (m_nMode == 1)
		{
			if (_llValue < m_llLastValue)
			{
				m_avgTime.Reset();
				m_avgValues.Reset();

				m_clock.Reset();
			}

			m_llLastValue = _llValue;
		}
		else if (m_llLastValue == MINLONGLONG)
		{
			m_clock.Reset();
			m_llLastValue = 0;
		}
		else
		{
			m_llLastValue += _llValue;
		}

		REFERENCE_TIME rtClock = _rtClock != MINLONGLONG ? _rtClock : m_clock.GetTime();

		m_avgTime.AddValue(rtClock);
		m_avgValues.AddValue(m_llLastValue);

		if (_pllLastValue)
			*_pllLastValue = m_llLastValue;

		if (_prtClock)
			*_prtClock = rtClock;

		double dblRate = _GetRate();
		if (dblRate > 0)
			AddValue(dblRate);

		return GetAverage();
	}

private:
	double _GetRate()
	{
		REFERENCE_TIME rtTimes[2] = {};
		LONGLONG llValues[2] = {};
		int nTime = m_avgTime.GetUtmostValues(&rtTimes[0], &rtTimes[1]);
		int nValues = m_avgValues.GetUtmostValues(&llValues[0], &llValues[1]);

		ATLASSERT(nTime == nValues);
		if (nTime > 1 && rtTimes[0] != rtTimes[1])
		{
			return (double)(llValues[1] - llValues[0]) * DS_ONESEC / (rtTimes[1] - rtTimes[0]);
		}

		return 0.0;
	}

};





