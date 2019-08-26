//---------------------------------------------------------------------------
// Perf.h : 
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

#ifndef __PERF_H_
#define __PERF_H_

#ifdef __PERF_ONLY_H_
#error __PERF_ONLY_H_ DEFINED
#endif

// Time functions
#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")

#include "../WinAPI/Functions.h"
#include "../WinAPI/Containers.h"

#pragma warning(push)
#pragma warning(disable:4189)


/////////////////////////////////////////////////////////////////////////////
class CPerf
{
protected:
	static DWORD s_dwFreq;		// CPU frequency in MHz
	static HWND s_hWnd;

	LPCTSTR		m_pszPrefix;

	LONGLONG	m_llSumm;
	LONGLONG	m_llStart;
	LONGLONG	m_llFirst;
	LONGLONG	m_llLast;
	DWORD		m_nTests;

	//---------------------------------

public:
	CPerf(LPCTSTR pszPrefix = NULL)
	{
		Clear();

		m_pszPrefix = (pszPrefix != NULL)? pszPrefix : _T("");

		if(s_dwFreq == 0)
		{
			bool b = GetProcFreq(&s_dwFreq);
			ATLASSERT(b && _T("CPerf::CPerf(): GetProcFreq() FAILED"));
		}
	}

	void SetWindow(HWND hWnd)
	{
		s_hWnd = hWnd;
	}

	void Clear()
	{
		m_llSumm = m_llStart = (LONGLONG)0;
		m_llFirst = m_llLast = (LONGLONG)0;
		m_nTests = 0;
	}

	void Start()
	{
		m_llStart = TimeStamp();
		if(m_nTests == 0)
			m_llFirst = m_llStart;
	}

	void Stop()
	{
		m_llLast = TimeStamp();
		m_llSumm += m_llLast - m_llStart;
		++m_nTests;
	}

	void Mark()
	{
		if(m_llStart != (LONGLONG)0)
			Stop();

		if(m_nTests == 100)
		{
			m_llSumm = m_llStart = (LONGLONG)0;
			m_llFirst = m_llLast = (LONGLONG)0;
			m_nTests = 0;
		}

		Start();
	}

	void Out(LPCTSTR pszPrefix = NULL)
	{
		if(m_nTests == 0)
			return;

		ATLASSERT(s_dwFreq != 0);

		LPCTSTR psz = (pszPrefix != NULL)? pszPrefix : m_pszPrefix;

		//---------------------------------
		DWORD msAvg = (DWORD)(m_llSumm/((LONGLONG)s_dwFreq * (LONGLONG)m_nTests));
		DWORD msAvgH = msAvg/1000;
		msAvg -= msAvgH * 1000;

		DWORD msSumm = (DWORD)(m_llSumm/(LONGLONG)s_dwFreq);
		DWORD msSummH = msSumm/1000;
		msSumm -= msSummH * 1000;

		DWORD nTotal = (DWORD)(100000 * m_llSumm/(m_llLast - m_llFirst));
		DWORD nTotalH = nTotal/1000;
		nTotal -= nTotalH * 1000;

		ATLTRACE2(atlTraceGeneral, 2, _T("%s: avg=%u.%03u ms (%u calls), summ=%u.%03u ms, CPU: %u.%03u%%\n"),
			psz, msAvgH, msAvg, m_nTests, msSummH, msSumm, nTotalH, nTotal);
	}

	void OutWin(LPCTSTR pszPrefix = NULL)
	{
		if(m_nTests == 0)
			return;

		ATLASSERT(s_dwFreq != 0);
		ATLASSERT(::IsWindow(s_hWnd));

		//---------------------------------
		LPCTSTR psz = (pszPrefix != NULL)? pszPrefix : m_pszPrefix;

		DWORD msAvg = (DWORD)(m_llSumm/((LONGLONG)s_dwFreq * (LONGLONG)m_nTests));
		DWORD msAvgH = msAvg/1000;
		msAvg -= msAvgH * 1000;

		TCHAR szMsg[128];
		wsprintf(szMsg, _T("%sAvg=%u.%03u ms (%u calls)"), psz, msAvgH, msAvg, m_nTests);
		SetWindowText(s_hWnd, szMsg);
	}

	bool OutStr(LPTSTR pszBuf, LPTSTR pszPrefix = NULL)
	{
		ATLASSERT(pszBuf != NULL);
		if(pszBuf == NULL) return false;

		ATLASSERT(s_dwFreq != 0);
		
		*pszBuf = (TCHAR)0;
		if(m_nTests == 0)
			return false;

		LPCTSTR psz = (pszPrefix != NULL)? pszPrefix : m_pszPrefix;

		DWORD msAvg = (DWORD)(m_llSumm/((LONGLONG)s_dwFreq * (LONGLONG)m_nTests));
		DWORD msAvgH = msAvg/1000;
		msAvg -= msAvgH * 1000;

		wsprintf(pszBuf, _T("%sAvg=%u.%03u ms (%u tests)"), psz, msAvgH, msAvg, m_nTests);
		return true;
	}

	bool OutAvg(LPTSTR pszBuf, LPTSTR pszPrefix = NULL)
	{
		ATLASSERT(pszBuf != NULL);
		if(pszBuf == NULL) return false;

		ATLASSERT(s_dwFreq != 0);

		*pszBuf = (TCHAR)0;
		if(m_nTests == 0)
			return false;

		LPCTSTR psz = (pszPrefix != NULL)? pszPrefix : m_pszPrefix;

		DWORD msAvg = (DWORD)(m_llSumm/((LONGLONG)s_dwFreq * (LONGLONG)m_nTests));
		DWORD msAvgH = msAvg/1000;
		msAvg -= msAvgH * 1000;

		wsprintf(pszBuf, _T("%s%u.%03u"), psz, msAvgH, msAvg);
		return true;
	}
};

__declspec(selectany) DWORD CPerf::s_dwFreq = 0;
__declspec(selectany) HWND CPerf::s_hWnd = NULL;


class CPerfFake
{

public:

	CPerfFake(LPCTSTR pszPrefix = NULL)
	{
	}

	void SetWindow(HWND hWnd)
	{
	}

	void Clear()
	{
	}

	void Start()
	{
	}

	void Stop()
	{
	}

	void Mark()
	{
	}

	void Out(LPCTSTR pszPrefix = NULL)
	{

	}

	void OutWin(LPCTSTR pszPrefix = NULL)
	{
	}

	bool OutStr(LPTSTR pszBuf, LPTSTR pszPrefix = NULL)
	{
		return true;
	}

	bool OutAvg(LPTSTR pszBuf, LPTSTR pszPrefix = NULL)
	{
		return true;
	}
};


#ifdef _DEBUG
#define CPerfD CPerf
#else
#define CPerfD CPerfFake
#endif

//---------------------------------
class CAutoPerf
{
	CPerf&	m_perf;
	LPCTSTR	m_pszPrefix;

public:

	CAutoPerf(CPerf& perf, LPCTSTR _pPerfix = NULL )
		: m_perf(perf),
		  m_pszPrefix( _pPerfix )
	{
		m_perf.Start();
	}
	~CAutoPerf()
	{
		m_perf.Stop();
		m_perf.Out( m_pszPrefix );
	}
};


class CAutoPerfFake
{
public:
	CAutoPerfFake(CPerf& perf, LPCTSTR _pPerfix = NULL )
	{
	}

	CAutoPerfFake(CPerfFake& perf, LPCTSTR _pPerfix = NULL )
	{
	}
};

#ifdef _DEBUG
#define CAutoPerfD CAutoPerf
#else
#define CAutoPerfD CAutoPerfFake
#endif

#define PERF_STR_SIZE	(128)
//---------------------------------
class CBasicPerf:
	public CPerf
{
	TCHAR	m_szText[PERF_STR_SIZE];
public:

	CBasicPerf(LPCTSTR szPrefix)
	{
		m_pszPrefix = szPrefix;
		Start();
	}

	CBasicPerf(int _nFake, LPCTSTR szPrefix, ... )
	{
		va_list arg_ptr = NULL;
		va_start(arg_ptr, szPrefix);
		_vstprintf( m_szText, szPrefix, arg_ptr );
		va_end(arg_ptr);

		m_pszPrefix = m_szText;
		Start();
	}

	~CBasicPerf()
	{
		Stop();
		Out();
	}
};

class CBasicPerfFake
{
public:
	CBasicPerfFake(LPCTSTR szPrefix)	{}
	CBasicPerfFake(int _nFake, LPCTSTR szPrefix, ... ) {}
};

#if defined(_DEBUG) || defined(_LOG_DEBUG)
#define CBasicPerfD CBasicPerf
#else
#define CBasicPerfD CBasicPerfFake
#endif


//---------------------------------
class CMultiPerf
{
	static HWND s_hWnd;

	CSimpleArray2<CPerf> m_arrPerfs;

	//---------------------------------
public:
	
	void Init(int nElem)
	{
		m_arrPerfs.RemoveAll();
		for(int i = 0; i < nElem; ++i) m_arrPerfs.Add(CPerf());
	}

	void SetWindow(HWND hWnd)
	{
		s_hWnd = hWnd;
	}

	void Clear(int idx)
	{
		ATLASSERT(idx < m_arrPerfs.GetSize());
		if(idx < m_arrPerfs.GetSize())
			m_arrPerfs[idx].Clear();
	}
	void ClearAll()
	{
		for(int i = 0; i < m_arrPerfs.GetSize(); ++i)
			m_arrPerfs[i].Clear();
	}

	void Start(int idx)
	{
		ATLASSERT(idx <= m_arrPerfs.GetSize());
		
		if(idx == m_arrPerfs.GetSize())
			m_arrPerfs.Add(CPerf());

		if(idx < m_arrPerfs.GetSize())
			m_arrPerfs[idx].Start();
	}

	void Stop(int idx)
	{
		ATLASSERT(idx < m_arrPerfs.GetSize());
		if(idx < m_arrPerfs.GetSize())
			m_arrPerfs[idx].Stop();
	}

	void Out(LPCTSTR pszPrefix = NULL)
	{
		TCHAR szMsg[1024];
		if(pszPrefix == NULL)
			szMsg[0] = (TCHAR)0;
		else
			lstrcpy(szMsg, pszPrefix);

		TCHAR sz[128];
		TCHAR szName[32];
		bool bFirst = true;
		for(int i = 0; i < m_arrPerfs.GetSize(); ++i)
		{
			wsprintf(szName, _T("[%i] "), i);

			if(m_arrPerfs[i].OutStr(sz, szName))
			{
				if(bFirst)
					bFirst = false;
				else
					lstrcat(szMsg, _T("   "));

				int	nBusy = lstrlen(szMsg);
				lstrcpyn(szMsg + nBusy, sz, SIZEOF_ARRAY(szMsg) - (nBusy + 1 + 3));
			}
		}

		if(::IsWindow(s_hWnd))
			SetWindowText(s_hWnd, szMsg);

		ATLTRACE2(atlTraceGeneral, 2, szMsg);
	}

	void OutAll(LPCTSTR pszPrefix = NULL)
	{
		TCHAR szMsg[1024];
		if(pszPrefix == NULL)
			szMsg[0] = (TCHAR)0;
		else
			lstrcpy(szMsg, pszPrefix);

		TCHAR sz[128];
		TCHAR szName[32];
		bool bFirst = true;
		for(int i = 0; i < m_arrPerfs.GetSize(); ++i)
		{
			wsprintf(szName, _T("[%i] "), i);

			if(m_arrPerfs[i].OutAvg(sz, szName))
			{
				if(bFirst)
					bFirst = false;
				else
					lstrcat(szMsg, _T("  "));

				int	nBusy = lstrlen(szMsg);
				lstrcpyn(szMsg + nBusy, sz, SIZEOF_ARRAY(szMsg) - (nBusy + 1 + 3));
			}
		}

		if(::IsWindow(s_hWnd))
			SetWindowText(s_hWnd, szMsg);

		ATLTRACE2(atlTraceGeneral, 2, szMsg);
	}
};

__declspec(selectany) HWND CMultiPerf::s_hWnd = NULL;

/////////////////////////////////////////////////////////////////////////////
class CPerfMark
{
	static DWORD s_dwFreq;		// CPU frequency in MHz

	class CPerfData
	{
		LONGLONG	m_llStart;
		LONGLONG	m_llSumm;
		DWORD		m_nTests;
		bool		m_bAccumulated;

		//---------------------------------
	public:
		CPerfData()
		{
			Clear();
		}
		void Clear()
		{
			m_llStart = m_llSumm = 0;
			m_nTests = 0;
			m_bAccumulated = false;
		}
		void Start()
		{
			m_bAccumulated = false;
			m_llStart = TimeStamp();
		}
		void Accumulate()
		{
			m_llSumm += TimeStamp() - m_llStart;
			m_bAccumulated = true;
		}
		void StopAccumulate()
		{
			if(m_bAccumulated)
			{
				++m_nTests;
				m_bAccumulated = false;
			}
		}
		void End()
		{
			Accumulate();
			++m_nTests;
		}
		LONGLONG Average() const
		{
			return (m_nTests == 0)? 0 : m_llSumm/(LONGLONG)m_nTests;
		}
		DWORD NumTests() const
		{
			return m_nTests;
		}
	};

	CSimpleArray2<CPerfData>	m_arrData;

	//---------------------------------

public:
	CPerfMark()
	{
		if(s_dwFreq == 0)
		{
			bool b = GetProcFreq(&s_dwFreq);
			ATLASSERT(b && _T("CPerfMark::CPerfMark(): GetProcFreq() FAILED"));
		}
	}

	void Init(int nMarks)
	{
		ATLASSERT(nMarks > 0);

		m_arrData.RemoveAll();
		for(int i = 0; i < nMarks; ++i)
			m_arrData.Add(CPerfData());
	}

	void Clear()
	{
		for(int i = 0; i < m_arrData.GetSize(); ++i)
			m_arrData[i].Clear();
	}

	//---------------------------------
	void Start(int i)
	{
		m_arrData[i].Start();
	}
	void Accumulate(int i)
	{
		m_arrData[i].Accumulate();
	}
	void StopAccumulate(int i)
	{
		m_arrData[i].StopAccumulate();
	}
	void End(int i)
	{
		m_arrData[i].End();
	}

	//---------------------------------
	void Out(LPCTSTR pszPrefix = NULL)
	{
		ATLASSERT(s_dwFreq != 0);

		ATLTRACE2(atlTraceGeneral, 2, _T("%s Performance data\n"),
			(pszPrefix != NULL)? pszPrefix : _T(""));

		for(int i = 0; i < m_arrData.GetSize(); ++i)
		{
			DWORD msAvg = (DWORD)( (m_arrData[i].Average() * 1000)/(LONGLONG)s_dwFreq );
			DWORD msAvgH = msAvg/1000000;
			msAvg -= msAvgH * 1000000;

			ATLTRACE2(atlTraceGeneral, 2, _T("Mark %i: duration=%u.%06u ms, num tests=%u\n"),
				i, msAvgH, msAvg, m_arrData[i].NumTests());
		}
	}
};

__declspec(selectany) DWORD CPerfMark::s_dwFreq = 0;

/////////////////////////////////////////////////////////////////////////////
#ifdef _PERF_CYCLE

class CPerfCycle
{
	static DWORD s_dwFreq;		// CPU frequency in MHz

	class CPerfData
	{
		LONGLONG	m_llStart;
		LONGLONG	m_llSumm;

		//---------------------------------
	public:
		CPerfData()
		{
			Clear();
		}
		void Clear()
		{
			m_llStart = m_llSumm = 0;
		}
		void Start(LONGLONG llStart)
		{
			m_llStart = llStart;
		}
		void Stop(LONGLONG llStop)
		{
			m_llSumm += llStop - m_llStart;
		}
		LONGLONG Average(DWORD nTests) const
		{
			return (nTests == 0)? 0 : m_llSumm/(LONGLONG)nTests;
		}
	};

	CSimpleArray2<CPerfData>	m_arrData;
	DWORD						m_nCycles;

	//---------------------------------

public:
	CPerfCycle()
	{
		if(s_dwFreq == 0)
		{
			bool b = GetProcFreq(&s_dwFreq);
			ATLASSERT(b && _T("CPerfCycle::CPerfCycle(): GetProcFreq() FAILED"));
		}

		m_nCycles = 0;
	}

	void Init(int nMarks)
	{
		ATLASSERT(nMarks > 0);

		m_arrData.RemoveAll();
		for(int i = 0; i < nMarks; ++i)
			m_arrData.Add(CPerfData());
		m_nCycles = 0;
	}

	void Clear()
	{
		for(int i = 0; i < m_arrData.GetSize(); ++i)
			m_arrData[i].Clear();
		m_nCycles = 0;
	}

	//---------------------------------
	void StartCycle()
	{
		++m_nCycles;
		Start(0);
	}
	void StopCycle()
	{
		Stop(0);
	}
	void Start(int i)
	{
		m_arrData[i].Start(TimeStamp());
	}
	void StopStart(int iStop, int iStart)
	{
		LONGLONG ll = TimeStamp();
		m_arrData[iStop].Stop(ll);
		m_arrData[iStart].Start(ll);
	}
	void Stop(int i)
	{
		m_arrData[i].Stop(TimeStamp());
	}

	//---------------------------------
	void Out(LPCTSTR pszPrefix = NULL)
	{
		ATLASSERT(s_dwFreq != 0);
		if(m_nCycles == 0)
		{
			ATLTRACE2(atlTraceGeneral, 2, _T("%s No performance data\n"), (pszPrefix != NULL)? pszPrefix : _T(""));
			return;
		}

		ATLTRACE2(atlTraceGeneral, 2, _T("%s Performance data\n"),
			(pszPrefix != NULL)? pszPrefix : _T(""));

		LONGLONG llBase = m_arrData[0].Average(m_nCycles);
		ATLASSERT(llBase != 0);

		DWORD msAvg = (DWORD)( (llBase * 1000)/(LONGLONG)s_dwFreq );
		DWORD msAvgH = msAvg/1000000;
		msAvg -= msAvgH * 1000000;

		ATLTRACE2(atlTraceGeneral, 2, _T("Mark 0: duration=%u.%06u ms, num tests=%u\n"),
			msAvgH, msAvg, m_nCycles);

		for(int i = 1; i < m_arrData.GetSize(); ++i)
		{
			LONGLONG ll = m_arrData[i].Average(m_nCycles);
			msAvg = (DWORD)( (ll * 1000)/(LONGLONG)s_dwFreq );
			msAvgH = msAvg/1000000;
			msAvg -= msAvgH * 1000000;

			double dblRel = (double)(ll * 100)/(double)llBase;
			ATLTRACE2(atlTraceGeneral, 2, _T("Mark %i: duration=%u.%06u ms (%5.2f%%)\n"),
				i, msAvgH, msAvg, dblRel);
		}
	}
};

__declspec(selectany) DWORD CPerfCycle::s_dwFreq = 0;

#else

class CPerfCycle
{
public:
	CPerfCycle() {}
	void Init(int) {}
	void Clear() {}
	void StartCycle() {}
	void StopCycle() {}
	void Start(int) {}
	void StopStart(int, int) {}
	void Stop(int) {}
	void Out(LPCTSTR pszPrefix = NULL) {}
};

#endif	// _PERF_CYCLE

// class CFramePerf:
// 	public CPerf
// {
// public:
// 	CFramePerf( LPCTSTR szPrefix = NULL, int nFPSFrames = 100 )
// 		: CPerf(szPrefix)
// 	{
// 	}
// 
// 	void CheckPoint( bool _bFrame )
// 	{
// 		if( !m_llStart )
// 		{
// 			Start();
// 			m_llLast = m_llStart;
// 		}
// 		else
// 		{
// 			LONGLONG llPos = TimeStamp();
// 			LONGLONG llTime = m_llLast - llPos;
// 			m_llLast = llPos;
// 
// 			double dblTime = (double)llTime / s_dwFreq;
// 		}
// 	}
// };

#pragma warning(pop)

/////////////////////////////////////////////////////////////////////////////
#endif	// __PERF_H_
