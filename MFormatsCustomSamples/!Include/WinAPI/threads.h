//---------------------------------------------------------------------------
// This file is part of the MediaLooks DirectShow Template Library (DSTL)
//
// Threads.h : wrapper classes for threads. Should be linked with multithread RTL
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

#ifndef __THREADS_H_
#define __THREADS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <process.h>
#include "Wrappers.h"

/////////////////////////////////////////////////////////////////////////////
#ifndef TRACELEVEL_Threads
	#define TRACELEVEL_Threads	8
#endif

#ifndef TRACELEVEL_Error
#define TRACELEVEL_Error		2
#endif

#ifndef TRACELEVEL_Fatal
#define TRACELEVEL_Fatal		0
#endif

#pragma warning(push)
#pragma warning(disable:4189 4302 4311)


/////////////////////////////////////////////////////////////////////////////
HRESULT __stdcall DSWaitWithMessageLoop(HANDLE hHandle, DWORD dwWait = INFINITE,
										bool bExitOnWMQuit = true);
DWORD __stdcall DSMultipleWaitWithMessageLoop(DWORD nCount, HANDLE* phHandles,
	  BOOL bWaitAll = FALSE, DWORD dwWait = INFINITE);


template <class T> class CSimpleThread;
template <class T, COINIT ThreadModel = COINIT_MULTITHREADED> class CSimpleThreadEx;
template <class T> class CBaseThread;
template <class T, COINIT ThreadModel = COINIT_MULTITHREADED> class CComBaseThread;
template <class T, COINIT ThreadModel = COINIT_MULTITHREADED> class CComControlThread;
template <class T, COINIT ThreadModel = COINIT_MULTITHREADED> class CComWinThread;
template <class T, COINIT ThreadModel = COINIT_MULTITHREADED> class CComUnifiedThread;

/////////////////////////////////////////////////////////////////////////////
inline HRESULT __stdcall DSWaitWithMessageLoop(HANDLE hHandle, DWORD dwWait /*= INFINITE*/,
											   bool bExitOnWMQuit /*= true*/)
{
	ATLASSERT(hHandle != NULL);
	if(hHandle == NULL) return E_INVALIDARG;

	DWORD dwStart = 0;
	if(dwWait != INFINITE)
		dwStart = ::GetTickCount();

	DWORD dw = dwWait;
	MSG msg;
	DWORD dwRet;
	while(1)
	{
		::SetLastError(ERROR_SUCCESS);

		dwRet = ::MsgWaitForMultipleObjects(1, &hHandle, FALSE, dw, QS_ALLINPUT);
		if(dwRet == WAIT_OBJECT_0)
			return S_OK;

		if(dwRet == WAIT_TIMEOUT)
			return S_FALSE;

		if(dwRet != WAIT_OBJECT_0 + 1)
			return ERROR_FROM_WIN32(::GetLastError(), E_FAIL);

		//---------------------------------
		while(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			if(bExitOnWMQuit && msg.message == WM_QUIT)
			{
				// MSDN: "All message loops exit when they receive a WM_QUIT message.
				// Furthermore, a nested message loop, if it receives a WM_QUIT
				// message, must break the loop and then re-post the quit message
				// so that the next outer layer can process it."
				int nExitCode = (int)msg.wParam;	// Fix for remove warning C4244
				::PostQuitMessage(nExitCode);
				return S_FALSE;
			}

			::TranslateMessage(&msg);
			::DispatchMessage(&msg);

			dwRet = ::WaitForSingleObject(hHandle, 0);
			if(dwRet == WAIT_OBJECT_0)
				return S_OK;

			if(dwRet != WAIT_TIMEOUT)
				return ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
			
			if(dwWait != INFINITE)
			{
				dw = ::GetTickCount() - dwStart;
				if(dwWait > dw)
					dw = dwWait - dw;
				else
					return S_FALSE;
			}
		}
	}

	return E_UNEXPECTED;
}


/////////////////////////////////////////////////////////////////////////////
inline DWORD __stdcall DSMultipleWaitWithMessageLoop(DWORD nCount, HANDLE* phHandles,
							 BOOL bWaitAll /*= FALSE*/, DWORD dwWait /*= INFINITE*/)
{
	ATLASSERT(phHandles != NULL && nCount > 0);
	if(phHandles == NULL || nCount == 0)
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		return WAIT_FAILED;
	}

	DWORD dwStart = 0;
	if(dwWait != INFINITE)
		dwStart = ::GetTickCount();

	DWORD dw = dwWait;
	MSG msg;
	DWORD dwRet;
	while(1)
	{
		::SetLastError(ERROR_SUCCESS);

		dwRet = ::MsgWaitForMultipleObjects(nCount, phHandles, bWaitAll, dw, QS_ALLINPUT);
		if(dwRet >= WAIT_OBJECT_0 && dwRet < WAIT_OBJECT_0 + nCount )
			return dwRet;

		if(dwRet == WAIT_TIMEOUT)
			return dwRet;

		if(dwRet != WAIT_OBJECT_0 + nCount)
			return dwRet;

		//---------------------------------
		while(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				// MSDN: "All message loops exit when they receive a WM_QUIT message.
				// Furthermore, a nested message loop, if it receives a WM_QUIT
				// message, must break the loop and then re-post the quit message
				// so that the next outer layer can process it."
				int nExitCode = (int)msg.wParam;	// Fix for remove warning C4244
				::PostQuitMessage(nExitCode);
				return WAIT_FAILED;
			}

			::TranslateMessage(&msg);
			::DispatchMessage(&msg);

			dwRet = ::WaitForMultipleObjects(nCount, phHandles, bWaitAll, 0);
			if(dwRet >= WAIT_OBJECT_0 && dwRet < WAIT_OBJECT_0 + nCount)
				return dwRet;

			if(dwRet != WAIT_TIMEOUT)
				return dwRet;
			
			if(dwWait != INFINITE)
			{
				dw = ::GetTickCount() - dwStart;
				if(dwWait > dw)
					dw = dwWait - dw;
				else
					return WAIT_TIMEOUT;
			}
		}
	}

	ATLASSERT(FALSE);
	return WAIT_FAILED;
}

/////////////////////////////////////////////////////////////////////////////
// CSimpleThread
//
// Signature of thread function of class T:
//		HRESULT RunThread()
//
//---------------------------------
// Class can be extended for use thread function of class T, different from RunThread():
//
//	typedef HRESULT (T::*_ThreadRun)();
//	T* m_pT;
//	_ThreadRun m_pfnThreadRun;
//	m_pT = NULL;
//	m_pfnThreadRun = &T::RunThread;
//	HRESULT Start(T* pObject, _ThreadRun pfnThreadRun = m_pfnThreadRun)
//	{
//		m_pT = pObject;
//		m_pfnThreadRun = pfnThreadRun;
//		...
//	}
//	static unsigned __stdcall StartThread(void* pv)
//	{
//		CSimpleThread<T>* pThread = reinterpret_cast<CSimpleThread<T>*>(pv);
//		return ((pThread->m_pT)->*(pThread->m_pfnThreadRun))();
//	}
//---------------------------------

enum eThreadState
{
	eTS_Stopped,			// Thread is closed
	eTS_Running,			// Thread is running
	eTS_StopPending		// Stop pending (m_bWork = false but thread is alive)
};

template <class T>
class CSimpleThreadBase
{
public:
	typedef HRESULT(T::*_ThreadMethod)(LPVOID _pvParam);
	typedef unsigned(__stdcall *_ThreadProc)(void *);
	typedef CSimpleThreadBase<T> _SimpleThread;

protected:

	struct START_CB
	{
		_ThreadMethod	pThreadMethod;
		LPVOID			pvParam;
		T*				pObject;

		START_CB(T* _pObject, _ThreadMethod _pThreadMethod, LPVOID _pvParam)
			: pObject(_pObject), pThreadMethod(_pThreadMethod), pvParam(_pvParam)
		{
		}
	};

public:

	ATL::CComAutoCriticalSection	m_csStartStop;
	ATL::CComAutoCriticalSection	m_csThread;
	HANDLE	m_hThread;
	DWORD	m_dwThreadID;
	HANDLE	m_hEvent;

	volatile bool	m_bWork;		// true: thread continues to work in a loop
									// false: thread should leave from a loop

	//---------------------------------
	CSimpleThreadBase()
	{
		m_hThread = NULL;
		m_dwThreadID = 0;
		m_hEvent = NULL;
		m_bWork = false;

	}
	~CSimpleThreadBase()
	{
		// If thread is running at this moment - this very dangerous !!!
		// - for class memebers could be already called destructor, while they can be accesable in RunThread()
		//ATLASSERT( !IsRunning() && "~CSimpleThread" );

		// Temp code for detect the not closed threads 		
		if (IsRunning())
		{
			WCHAR szEXEName[1024] = {};
			WCHAR szDLLName[1204] = {};
			GetModuleFileNameW(NULL, szEXEName, SIZEOF_ARRAY(szEXEName));
			DLLModuleName(szDLLName, SIZEOF_ARRAY(szDLLName));

			WCHAR szInfo[4096] = {};
			_snwprintf(szInfo, SIZEOF_ARRAY(szInfo), L"WARNING !!! Thread %04X(%d) NOT STOPPED before destroying.\nCurrent:%04X(%d) %hs\nEXE: %ws\nDLL: %ws",
				m_dwThreadID, m_dwThreadID,
				GetCurrentThreadId(), GetCurrentThreadId(),
				__FUNCTION__, szEXEName, szDLLName);

			::MessageBoxW(NULL, szInfo, L"WARNING !!! Thread not stopped.", MB_SYSTEMMODAL);
		}

		Stop();
	}

	bool IsCurrentThread(DWORD* _pdwThreadID)
	{
		CAutoCS crs(m_csThread);

		if (_pdwThreadID)
			*_pdwThreadID = m_dwThreadID;

		return GetCurrentThreadId() == m_dwThreadID;
	}

	eThreadState IsRunning(HANDLE* _phThread = NULL)
	{
		CAutoCS crs(m_csThread);

		if (_phThread)
			*_phThread = m_hThread;

		if (!m_hThread)
			return eTS_Stopped;

		DWORD dwRes = ::WaitForSingleObject(m_hThread, 0);
		if (dwRes == WAIT_TIMEOUT)
		{
			ATLASSERT(m_dwThreadID);
			return m_bWork ? eTS_Running : eTS_StopPending;
		}

		MTRACE_WARN("(%08X)::IsRunning() Thread:%d(%08X) ID:%d(%08X) FINISHED BUT NOT CLOSED",
			(ULONG)this, (DWORD)m_hThread, (DWORD)m_hThread, m_dwThreadID, m_dwThreadID);

		ATLASSERT(m_dwThreadID);

		if (m_hEvent != NULL)
		{
			::CloseHandle(m_hEvent);
			m_hEvent = NULL;
		}
		if (m_hThread != NULL)
			::CloseHandle(m_hThread);

		m_hThread = NULL;
		m_dwThreadID = 0;
		m_bWork = false;
		return eTS_Stopped;
	}

	DWORD WaitWhileThreadComplete(DWORD _dwMsec)
	{
		HANDLE hThread = NULL;
		if (IsRunning(&hThread) != eTS_Stopped)
		{
			DWORD dwRes = ::WaitForSingleObject(hThread, _dwMsec);
			MTRACE("(%d msec) dwRes = %d", _dwMsec, dwRes);
			return dwRes;
		}

		MTRACE("(%d msec) NOT RUNNING", _dwMsec);
		return WAIT_OBJECT_0;
	}

	HRESULT StartEx(T* _pObject, _ThreadMethod _pfnThreadMethod, LPVOID _pvParam = NULL, bool _bRestart = false, DWORD _dwStackSize = 0)
	{
		ATLASSERT(_pObject && _pfnThreadMethod);
		if (!_pObject || !_pfnThreadMethod) return E_INVALIDARG;

		if (IsRunning() && !_bRestart)
		{
			MTRACE("Thread:%08X(%d) ALREADY RUNNING", m_dwThreadID, m_dwThreadID);
			return S_FALSE;
		}

		START_CB* pStart = new START_CB(_pObject, _pfnThreadMethod, _pvParam);

		return _Start(_pObject, StartThreadCPP, pStart, _dwStackSize);
	}

	HRESULT Stop(DWORD dwWait = INFINITE, bool bTerminateThread = false, bool _bMsgLoop = true)
	{
		CAutoCS crsS(m_csStartStop);

		m_bWork = false;

		if (IsRunning() == eTS_Stopped)
		{
			MTRACE("(%08X)::Stop() Thread:%d(%08X) ID:%d(%08X) ALREADY STOPPED",
				(ULONG)this, (DWORD)m_hThread, (DWORD)m_hThread, m_dwThreadID, m_dwThreadID);

			return S_FALSE;
		}

		MTRACE("(%08X)::Stop() Thread:%d(%08X) ID:%d(%08X) Running",
			(ULONG)this, (DWORD)m_hThread, (DWORD)m_hThread, m_dwThreadID, m_dwThreadID);

		ATLASSERT(m_dwThreadID != GetCurrentThreadId());
		if (m_dwThreadID == GetCurrentThreadId()) return HRESULT_FROM_WIN32(ERROR_THREAD_ALREADY_IN_TASK);

		HANDLE hThread = NULL;
		if (Awake(&hThread, 0))
		{
			DWORD dwRet = 0;
			if (_bMsgLoop)
				dwRet = DSWaitWithMessageLoop(hThread, dwWait);
			else
				dwRet = ::WaitForSingleObject(hThread, dwWait);
			if (dwRet != WAIT_OBJECT_0)
			{
				MTRACE_ERR("WaitForSingleObject( wait time=%u )=0x%X for thread 0x%X", dwWait, dwRet, m_dwThreadID);

				if (bTerminateThread)
				{
					MTRACE_ERR("call TerminateThread() for thread 0x%X", m_dwThreadID);

					ATLASSERT(!"TerminateThread(SimpleThread)");
					::TerminateThread(hThread, (DWORD)-1);

					// For do not have error MsgBox, check m_hThread == hThread for prevent situation Stop()->Start()
					CAutoCS crs(m_csThread);

					if (m_hThread == hThread)
					{
						::CloseHandle(m_hThread);

						m_dwThreadID = 0;
						m_hThread = 0;
					}
				}
			}
		}

		if (IsRunning() != eTS_Stopped)
		{
			return HRESULT_FROM_WIN32(ERROR_TIMEOUT);
		}

		return S_OK;
	}

	//---------------------------------
	operator HANDLE() const { return m_hThread; }
	bool operator!() const { return m_hThread == NULL; }

	bool Awake(HANDLE* _phThread = NULL, int _nUpdateWorkStatus = -1)
	{
		CAutoCS crs(m_csThread);

		if (_phThread)
			*_phThread = m_hThread;

		if (m_hThread == NULL)
			return false;

		if (_nUpdateWorkStatus >= 0)
			m_bWork = _nUpdateWorkStatus > 0 ? 1 : 0;

		ATLASSERT(m_hEvent);
		BOOL bRet = ::SetEvent(m_hEvent);
		return true;
	}

protected:

	HRESULT _Start(T* pObject, _ThreadProc pfnThreadProc, LPVOID _pvParam, DWORD _dwStackSize = 0)
	{
		ATLASSERT(pObject != NULL);
		if (pObject == NULL) return E_INVALIDARG;
		ATLASSERT(pfnThreadProc != NULL);
		if (pfnThreadProc == NULL) return E_INVALIDARG;

		CAutoCS crsS(m_csStartStop);

		HRESULT hr = Stop();
		if (FAILED(hr))
		{
			MTRACE_ERR("Can't stop thread (ThreadID:%08X)", m_dwThreadID);
			return hr;
		}

		//---------------------------------
		::SetLastError(0);

		CAutoCS crs(m_csThread);

		ATLASSERT(m_hThread == NULL);
		if (m_hThread != NULL) return HRESULT_FROM_WIN32(ERROR_FILE_EXISTS);

		HANDLE hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		if (hEvent == NULL)
		{
			hr = ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
			ATLASSERT(!_T("CSimpleThread::InitThread(): can't create event"));
			return hr;
		}

		m_hThread = (HANDLE)::_beginthreadex(
			NULL, _dwStackSize, pfnThreadProc, _pvParam ? _pvParam : (void*)pObject,
			CREATE_SUSPENDED, (unsigned*)&m_dwThreadID);

		if (m_hThread == NULL)
		{
			hr = ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
			::CloseHandle(hEvent);
			ATLASSERT(!_T("CSimpleThread::ThreadStart(): can't create thread"));
			return hr;
		}

		//---------------------------------
		m_hEvent = hEvent;
		m_bWork = true;

		MTRACE("(%08X)::Start() Thread:%d(%08X) ID:%d(%08X) Started",
			(ULONG)this, (DWORD)m_hThread, (DWORD)m_hThread, m_dwThreadID, m_dwThreadID);

		DWORD dwRet = ::ResumeThread(m_hThread);
		dwRet;
		ATLASSERT(dwRet == 1);
		return S_OK;
	}


	
	//////////////////////////////////////////////////////////////////////////
	// New method for direct use class ThreadProps, not HRESULT RunThread()
	static unsigned __stdcall StartThreadCPP(void* pv)
	{
		MTRACE(" (THREADPROC) thread 0x%04X START", ::GetCurrentThreadId());

		ATLASSERT(pv != NULL);
		START_CB* pStart = reinterpret_cast<START_CB*>(pv);
		ATLASSERT(pStart->pObject && pStart->pThreadMethod);
		HRESULT uiRes = (pStart->pObject->*pStart->pThreadMethod)(pStart->pvParam);

		MTRACE(" (THREADPROC) thread 0x%04X DONE, result=0x%X", ::GetCurrentThreadId(), uiRes);

		delete pStart;

		return uiRes;
	}

};

template <class T>
class CSimpleThread
	: public CSimpleThreadBase<T>
{

public:
	HRESULT Start(T* pObject, _ThreadProc pfnThreadProc = StartThread, DWORD _dwStackSize = 0 )
	{
		return _Start(pObject, pfnThreadProc, NULL, _dwStackSize);
	}

protected:

	//---------------------------------
	static unsigned __stdcall StartThread(void* pv)
	{
		MTRACE( " (THREADPROC) thread 0x%04X START", ::GetCurrentThreadId());

		ATLASSERT(pv != NULL);
		T* pT = reinterpret_cast<T*>(pv);
		unsigned int uiRes = pT->RunThread();

		MTRACE( " (THREADPROC) thread 0x%04X DONE, result=0x%X", ::GetCurrentThreadId(), uiRes);
		return uiRes;
	}

	// Example of CMyClass::RunThread() if class CMyClass includes CSimpleThread as member:
	//
	//		CSimpleThread<CMyClass> m_theThread;
	//
	//		HRESULT RunThread()
	//		{
	//			HRESULT hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	//			ATLASSERT(hr == S_OK);
	//
	//			while(m_theThread.m_bWork)
	//			{
	//				DWORD dwRet = ::WaitForSingleObject(m_theThread.m_hEvent, INFINITE);
	//				if(!m_theThread.m_bWork)
	//					break;
	//
	//				ATLASSERT(dwRet == WAIT_OBJECT_0);
	//
	//				// Do something
	//			}
	//
	//			::CoUninitialize();
	//			return S_OK;
	//		}

	//////////////////////////////////////////////////////////////////////////
	// New method for direct use class ThreadProps, not HRESULT RunThread()
	static unsigned __stdcall StartThreadCPP(void* pv)
	{
		MTRACE(" (THREADPROC) thread 0x%04X START", ::GetCurrentThreadId());

		ATLASSERT(pv != NULL );
		START_CB* pStart = reinterpret_cast<START_CB*>(pv);
		ATLASSERT(pStart->pObject && pStart->pThreadMethod);
		HRESULT uiRes = (pStart->pObject->*pStart->pThreadMethod)(pStart->pvParam);

		MTRACE(" (THREADPROC) thread 0x%04X DONE, result=0x%X", ::GetCurrentThreadId(), uiRes);

		delete pStart;

		return uiRes;
	}
	
};

template <class T, class TCallbackParam = LONGLONG>
class MThread
	: public AutoObjBase<MThread<T, TCallbackParam>>,
	  public CSimpleThreadBase<T>
{
	friend class CAutoObj< MThread<T, TCallbackParam> >;
public:
	typedef MThread<T, TCallbackParam> _MThread;
	typedef int (T::*_ThreadCallbackCPP)(TCallbackParam _llParam);
	
	MThread()
	{
	}

	~MThread()
	{
		_SimpleThread::Stop(INFINITE, true, true);
	}

	static HRESULT StartThread(_MThread** _ppObject, T* pClassObject, _ThreadCallbackCPP _pfnClassMethod, TCallbackParam _theThreadParam = NULL, DWORD _dwStackSize = 0)
	{
		ATLASSERT(_ppObject);
		if (!_ppObject) return E_POINTER;

		if (!pClassObject || !_pfnClassMethod)
			return E_INVALIDARG;

		_MThread* pT = new _MThread();
		pT->AddRef();
		HRESULT hr = pT->_Start(pClassObject, _pfnClassMethod, _theThreadParam, _dwStackSize);
		if (FAILED(hr))
		{
			pT->Release();
			return hr;
		}

		*_ppObject = pT;
		return S_OK;
	}

private:


	bool NeedAsyncRelease() { return IsRunning(); }

	struct THREAD_CB
	{
		MThread* 			pThis;
		T*					pClass;
		_ThreadCallbackCPP	pThreadCallback;
		TCallbackParam		tParamCB;

		THREAD_CB(MThread* _pThis, T* _pClass, _ThreadCallbackCPP	_pThreadCallback, TCallbackParam _tParamCB)
			: pThis(_pThis), pClass(_pClass), pThreadCallback(_pThreadCallback), tParamCB(_tParamCB)
		{
		}
	};

	//---------------------------------
	static unsigned __stdcall _StartThreadEx(void* pv)
	{
		MTRACE(" (THREADPROC) thread 0x%04X START", ::GetCurrentThreadId());

		ATLASSERT(pv != NULL);
		THREAD_CB* pThreadCB = reinterpret_cast<THREAD_CB*>(pv);
		unsigned int uiRes = pThreadCB->pThis->_RunThreadEx(pThreadCB);

		MTRACE(" (THREADPROC) thread 0x%04X DONE, result=0x%X", ::GetCurrentThreadId(), uiRes);

		delete pThreadCB;
		return uiRes;
	}


	// int ThreadCallback(TCallbackParam _theThreadParam)
	// < 0 (-1) -> Stop
	// > 0 -> Msec wait for specifed time 
	
	HRESULT _Start(T* pObject, _ThreadCallbackCPP _pfnThreadMethod, TCallbackParam _theThreadParam = NULL, DWORD _dwStackSize = 0)
	{
		THREAD_CB* pThreadCB = new THREAD_CB( this, pObject, _pfnThreadMethod, _theThreadParam);
		HRESULT hr = _SimpleThread::_Start(pObject, _StartThreadEx, pThreadCB, _dwStackSize);
		ATLASSERT(SUCCEEDED(hr));
		if (FAILED(hr))
		{
			MTRACE_ERR("_SimpleThread::Start FAILED");
		}

		return hr;
	}

	HRESULT _RunThreadEx(THREAD_CB* pCallback)
	{
		ATLASSERT(pCallback && pCallback->pClass && pCallback->pThreadCallback);

		HRESULT hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
		ATLASSERT(hr == S_OK);

		while (m_bWork)
		{
			int nWait = (pCallback->pClass->*pCallback->pThreadCallback)(pCallback->tParamCB);
			if (nWait < 0)
			{
				MTRACE("Thread Stopped nRes=%d", nWait);
				break;
			}

			DWORD dwRet = ::WaitForSingleObject(m_hEvent, nWait);
			if (!m_bWork)
				break;

			ATLASSERT(dwRet == WAIT_OBJECT_0 || dwRet == WAIT_TIMEOUT);
		}

		::CoUninitialize();
		return S_OK;
	}

};

/////////////////////////////////////////////////////////////////////////////
// CSimpleThreadEx
//
// Signature of thread function of class T:
//		HRESULT RunThreadEx(const CSimpleThread<CMyClass>* pThread, DWORD dwParam)
// where:
//		pThread: pointer to object of the thread which has clled this method
//		dwParam: parameter of method CSimpleThreadEx::Start(T* pObject, DWORD dwParam)

template <class T, COINIT ThreadModel /*= COINIT_MULTITHREADED*/>
class CSimpleThreadEx : public CSimpleThread<T>
{
	T*		m_pObject;
	DWORD	m_dwParam;

	//---------------------------------

public:

	typedef CSimpleThreadEx<T> TSimpleThreadEx;

	HRESULT Start(T* pObject, DWORD dwParam)
	{
		ATLASSERT(pObject != NULL);
		if(pObject == NULL) return E_INVALIDARG;
		
		Stop();

		//---------------------------------
		HRESULT hr = S_OK;
		::SetLastError(0);

		HANDLE hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		if(hEvent == NULL)
		{
			hr = ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
			ATLASSERT(!_T("CSimpleThread::InitThread(): can't create event"));
			return hr;
		}

		m_hThread = (HANDLE)::_beginthreadex(
			NULL, 0, StartThreadEx, (void*)this,
			CREATE_SUSPENDED, (unsigned*)&m_dwThreadID);

		if(m_hThread == NULL)
		{
			hr = ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
			::CloseHandle(hEvent);
			ATLASSERT(!_T("CBaseThread::ThreadStart(): can't create thread"));
			return hr;
		}

		//---------------------------------
		m_pObject = pObject;
		m_dwParam = dwParam;

		m_hEvent = hEvent;
		m_bWork = true;

		DWORD dwRet = ::ResumeThread(m_hThread);
		dwRet;
		ATLASSERT(dwRet == 1);
		return S_OK;
	}

	static unsigned __stdcall StartThreadEx(void* pv)
	{
		ATLASSERT(pv != NULL);
		HRESULT hr = ::CoInitializeEx(NULL, ThreadModel);
		ATLASSERT(hr == S_OK);

		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Threads,
			_T("CSimpleThreadEx::StartThreadEx(): thread 0x%04X started\n"), ::GetCurrentThreadId());

		CSimpleThreadEx<T, ThreadModel>* pThread = reinterpret_cast<CSimpleThreadEx<T, ThreadModel>*>(pv);
		hr = pThread->m_pObject->RunThreadEx(pThread, pThread->m_dwParam);

		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Threads,
			_T("CSimpleThreadEx::StartThreadEx(): thread 0x%04X stopped, hr=0x%X\n"), ::GetCurrentThreadId(), hr);

		::CoUninitialize();
		return hr;
	}

	//---------------------------------
	// Example of CMyClass::RunThreadEx() if class CMyClass includes array of CSimpleThreadEx objects:
	//
	//		CSimpleThreadEx<CMyClass> m_theThreads[16];
	//
	//		HRESULT RunThreadEx(const TSimpleThreadEx* pThread, DWORD dwParam)
	//		{
	//			while(pThread->m_bWork)
	//			{
	//				DWORD dwRet = ::WaitForSingleObject(pThread->m_hEvent, INFINITE);
	//				if(!pThread->m_bWork)
	//					break;
	//
	//				ATLASSERT(dwRet == WAIT_OBJECT_0);
	//
	//				// Do something
	//				DoSomething(dwParam);			
	//			}
	//			return S_OK;
	//		}
};



/////////////////////////////////////////////////////////////////////////////
// Signature of thread function of class T
//		void ThreadFun(DWORD nID)
// where nID:
//	- number of signaled handle accordingly position in pHandles array (see Start());
//	- (DWORD)-1 if thread ending.

template <class T>
class CBaseThread
{
protected:

	// Variables for remove unnecessary waiting at call of Start() or Stop()
	// from some threads simultaneously
	volatile LONG		m_lStart;		// 1: some thread already in Start()
	volatile LONG		m_lStop;		// 1: some thread already in Stop()

	CComAutoCriticalSection	m_cs;

	HANDLE				m_hThread;
	DWORD				m_dwThreadId;
	bool				m_bAutoClose;
	bool				m_bEnd;
	bool				m_bWaitThreadEndWithMsgLoop;
	bool				m_bExitOnWMQuit;

	DWORD				m_nCount;		// 1 + nCount from Start()
	HANDLE*				m_pHandles;		// m_pHandles[0] : our event for stop thread,
										// another: handles from pHandles array (see Start())

	//---------------------------------
	// Pointer to extern object with next method:
	//		void ThreadFun(DWORD nID)
	// where nID:
	//	- number of signaled handle accordingly position in pHandles array (see Start());
	//	- (DWORD)-1 if thread ending.
	T*					m_pObject;

	//---------------------------------
public:
	CBaseThread() : m_lStart(0), m_lStop(0)
	{
		m_bWaitThreadEndWithMsgLoop = false;
		m_bExitOnWMQuit = true;
		m_hThread = NULL;
		m_dwThreadId = 0;
		m_bEnd = true;
		m_bAutoClose = false;
		m_nCount = 0;
		m_pHandles = NULL;
		m_pObject = NULL;
	}

	virtual ~CBaseThread()
	{
		if(m_dwThreadId == ::GetCurrentThreadId())
		{
			// Self-murder.
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Fatal,
				_T("###Err### ~CBaseThread(): call from internal thread\n"));

			// Can't make nothing: thread object isn't valid anymore and in any case
			// we will see access violation or some another catastrophic failure
			ATLASSERT(!_T("~CBaseThread(): call from internal thread"));
		}
		else if(::InterlockedIncrement((PLONG)&m_lStart) > 1)
		{
			// Some thread creating internal thread in this moment
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Fatal,
				_T("###Err### ~CBaseThread(): some thread in Start()\n"));

			// Can't make nothing: thread object isn't valid after destructor and in any case
			// we will see access violation or some another catastrophic failure
			ATLASSERT(!_T("~CBaseThread(): some thread in Start()"));
		}
		else if(::InterlockedIncrement((PLONG)&m_lStop) > 1 && m_hThread != NULL)
		{
			::InterlockedDecrement((PLONG)&m_lStop);

			// Some external thread closing internal thread in this moment
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Fatal,
				_T("###Err### ~CBaseThread(): some thread in Stop()\n"));

			// We waiting end of internal thread, but in this case exist
			// very high possibility of second call of destructor from external thread
			// with access violation or some another catastrophic failure

			// Waits while internal thread is ending
			WaitForThreadEnd(m_hThread, INFINITE);

			// Waits while external thread leave Stop() method
			while(m_lStop > 0)
				Sleep(10);

			// Clear() should be called from external or internal thread
			ATLASSERT(m_hThread == NULL);
		}
		else
		{
			FinalStop();
		}
	}

	HRESULT Start(T* pObject, HANDLE handle)
	{
		ATLASSERT(handle != NULL);
		if(handle == NULL) return E_INVALIDARG;

		return Start(pObject, 1, &handle);
	}

	HRESULT Start(T* pObject, DWORD nCount = 0, const HANDLE* pHandles = NULL)
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Threads, _T("CBaseThread<>::Start()\n"));

		ATLASSERT(pObject != NULL);
		if(pObject == NULL) return E_INVALIDARG;
		ATLASSERT((nCount == 0 && pHandles == NULL) || (nCount > 0 && pHandles != NULL));
		if((nCount == 0 && pHandles != NULL) || (nCount > 0 && pHandles == NULL)) return E_INVALIDARG;

		//---------------------------------
		// If two (or more) threads attempts to create the same new thread,
		// then call of Start() can be succeeded only for one from theirs.
		// Use Interlocked functions for restrict access without waiting.

		HRESULT hr = S_OK;
		if(::InterlockedIncrement((PLONG)&m_lStart) > 1)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error,
				_T("###Err### CBaseThread::Start(): already starting in another thread\n"));
			hr = S_FALSE;
		}
		else if(m_hThread != NULL)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error,
				_T("###Err### CBaseThread::Start(): already started\n"));
			hr = S_FALSE;
		}
		else
		{
			hr = ThreadStart(pObject, nCount, pHandles);
		}

		::InterlockedDecrement((PLONG)&m_lStart);
		return hr;
	}

	// S_OK:	Thread ended
	// S_FALSE: If thread don't ended at return from this function
	//			(timeout or call from internal thread)
	//
	// ATTENTION. Don't call Stop() from DllMain:
	//	MSDN: "Waiting on synchronization objects in DllMain can cause a deadlock"
	HRESULT Stop(DWORD dwWait = INFINITE)
	{
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Threads, _T("CBaseThread<>::Stop()\n"));

		// If two (or more) threads attempts to destroy the same thread,
		// then call of Stop() can be succeeded only for one from theirs.
		// Use Interlocked functions for restrict access without waiting.

		HRESULT hr = S_OK;
		if(::InterlockedIncrement((PLONG)&m_lStop) > 1)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error,
				_T("###Err### CBaseThread::Stop(): already closing in another thread\n"));
			hr = S_FALSE;
		}
		else
		{
			hr = ThreadStop(dwWait);
			if(hr == S_FALSE)
			{
				ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error,
					_T("CBaseThread::Stop(): can't wait for the ending of a thread\n"));
			}
			else if(hr != S_OK)
			{
				ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error,
					_T("###Err### CBaseThread::Stop(): ThreadStop()=0x%X\n"), hr);
			}
		}

		::InterlockedDecrement((PLONG)&m_lStop);
		return hr;
	}

	operator HANDLE() const { return m_hThread; }
	bool operator!() const { return m_hThread == NULL; }

	DWORD ThreadID() const
	{
		return m_dwThreadId;
	}

	//---------------------------------
	virtual HRESULT ThreadInitialize() { return S_OK; }
	virtual void ThreadUninitialize() {}

	//---------------------------------
protected:

	// Create new thread in suspended state and resume its before return.
	HRESULT ThreadStart(T* pObject, DWORD nCount, const HANDLE* pHandles)
	{
		ATLASSERT(pObject != NULL);
		ATLASSERT((nCount == 0 && pHandles == NULL) || (nCount > 0 && pHandles != NULL));

		ATLASSERT(m_hThread == NULL);
		ATLASSERT(m_bEnd);
		ATLASSERT(m_pHandles == NULL);

		//---------------------------------
		CAutoCS lock(m_cs);

		HRESULT hr = S_OK;
		::SetLastError(0);

		HANDLE hExit = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		if(hExit == NULL)
		{
			hr = ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
			ATLASSERT(!_T("CBaseThread::ThreadStart(): can't create event"));
			return hr;
		}

		m_pHandles = (HANDLE*)malloc((nCount + 1) * sizeof(HANDLE));
		if(m_pHandles == NULL)
		{
			::CloseHandle(hExit);
			return E_OUTOFMEMORY;
		}

		m_pHandles[0] = hExit;
		if(nCount > 0)
		{
			memcpy(m_pHandles + 1, pHandles, nCount * sizeof(HANDLE));
		}
		m_nCount = nCount + 1;

		//---------------------------------
		m_hThread = (HANDLE)::_beginthreadex(NULL, 0, _ThreadMain, (void*)this,
											 CREATE_SUSPENDED, (unsigned*)&m_dwThreadId);
		if(m_hThread == NULL)
		{
			hr = ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
			ATLASSERT(!_T("CBaseThread::ThreadStart(): can't create thread"));
			Clear();
			return hr;
		}

		//---------------------------------
		m_bEnd = false;
		m_pObject = pObject;

		DWORD dwRet = ::ResumeThread(m_hThread);
		dwRet;
		ATLASSERT(dwRet == 1);
		return S_OK;
	}

	HRESULT ThreadStop(DWORD dwWait)
	{
		{
			CAutoCS lock(m_cs);

			if(m_hThread == NULL)
			{
				ATLASSERT(m_bEnd);
				ATLTRACE2(atlTraceGeneral, TRACELEVEL_Threads,
					_T("CBaseThread::ThreadStop(): already stopped\n"));
				return S_OK;
			}
			if(m_bEnd)
			{
				ATLASSERT(!m_bAutoClose);

				// If internal thread already ended, but variables not cleared yet
				// (m_bAutoClose == false in _ThreadMain()): only clears state
				Clear();
				return S_OK;
			}

			ATLASSERT(m_pHandles != NULL && m_pHandles[0] != NULL);
			::SetEvent(m_pHandles[0]);

			if(m_dwThreadId == ::GetCurrentThreadId())
			{
				// In this case internal thread will be clear all data on exit.
				m_bAutoClose = true;
				return S_FALSE;
			}

			m_bAutoClose = false;
		}

		//---------------------------------
		// ATTENTION. Don't attempts stop thread in DllMain:
		//	MSDN: "Waiting on synchronization objects in DllMain can cause a deadlock"
		HRESULT hr = WaitForThreadEnd(m_hThread, dwWait);

		//---------------------------------
		CAutoCS lock(m_cs);

		if(hr == S_OK)
		{
			ATLASSERT(m_bEnd);
			Clear();
		}
		else if(hr != S_FALSE)
		{
			ATLASSERT(!_T("CBaseThread::ThreadStop(): WaitForThreadEnd() failed"));
			Clear();
		}
		else
		{
			// If wait period ended or received WM_QUIT, and thread not signaled
			if(!m_bEnd)
			{
				// If internal thread not ended yet: set auto close flag
				m_bAutoClose = true;
			}
			else
			{
				// If internal thread ended between return from WaitForThreadEnd()
				// and enter to current critical section: clears variables
				Clear();
			}
			hr = S_FALSE;
		}
		return hr;
	}

	void FinalStop()
	{
		{
			CAutoCS lock(m_cs);

			if(m_hThread == NULL)
			{
				ATLASSERT(m_bEnd);
				return;
			}
			if(m_bEnd)
			{
				ATLASSERT(!m_bAutoClose);
				Clear();
				return;
			}
	
			m_bAutoClose = false;
		}

		//---------------------------------
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Threads,
			_T("CBaseThread::FinalStop(): close thread 0x%X\n"), m_dwThreadId);
		
		ATLASSERT(m_pHandles != NULL && m_pHandles[0] != NULL);
		::SetEvent(m_pHandles[0]);

		::WaitForSingleObject(m_hThread, INFINITE);

		CAutoCS lock(m_cs);
		Clear();
	}

	//---------------------------------
	static unsigned __stdcall _ThreadMain(void* pv)
	{
		ATLASSERT(pv != NULL);
		CBaseThread<T>* pThis = reinterpret_cast<CBaseThread<T>*>(pv);

		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Threads,
			_T("CBaseThread::_ThreadMain(): thread 0x%04X started\n"), pThis->m_dwThreadId);

		HRESULT hr = pThis->ThreadInitialize();
		if(hr == S_OK)
		{
			hr = pThis->ThreadMain();
			pThis->ThreadUninitialize();
		}

		//---------------------------------
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Threads,
			_T("CBaseThread::_ThreadMain(): thread 0x%X ended, hr=0x%X\n"), pThis->m_dwThreadId, hr);

		pThis->Exit();
		return hr;
	}

	void Exit()
	{
		CAutoCS lock(m_cs);
		m_bEnd = true;
		if(m_bAutoClose)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Threads, _T("CBaseThread::Exit(): auto close\n"));
			Clear();
		}
	}

	virtual HRESULT ThreadMain()
	{
		HRESULT hr = S_OK;
		while(WaitObjects(hr));

		SEH_ThreadFun((DWORD)-1);
		return hr;
	}

	bool WaitObjects(HRESULT& hr)
	{
		ATLASSERT(m_pHandles != NULL && m_nCount > 0);

		DWORD dwRet = ::WaitForMultipleObjects(m_nCount, m_pHandles, FALSE, INFINITE);
		if(dwRet == WAIT_OBJECT_0)
		{
			// Exit from extern while() and close current thread
			return false;
		}

		if(WAIT_OBJECT_0 < dwRet && dwRet < WAIT_OBJECT_0 + m_nCount)
		{
			dwRet -= WAIT_OBJECT_0 + 1;
			hr = SEH_ThreadFun(dwRet);
		}
		else
		{
			hr = ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### CBaseThread::WaitObjects(): ")
				_T("WaitForMultipleObjects()=0x%X, hr=0x%X\n"), dwRet, hr);
		}

		// If error: return false, exit from extern while() and close current thread
		return hr == S_OK;
	}

	HRESULT WaitForThreadEnd(HANDLE hHandle, DWORD dwWait = INFINITE)
	{
		if(!m_bWaitThreadEndWithMsgLoop)
		{
			ATLASSERT(hHandle != NULL);
			if(hHandle == NULL) return E_INVALIDARG;

			DWORD dwRet = ::WaitForSingleObject(hHandle, dwWait);
			if(dwRet == WAIT_OBJECT_0)
				return S_OK;

			if(dwRet == WAIT_TIMEOUT)
				return S_FALSE;

			return ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
		}

		return DSWaitWithMessageLoop(hHandle, dwWait, m_bExitOnWMQuit);
	}

	HRESULT SEH_ThreadFun(DWORD nID)
	{
		ATLASSERT(m_pObject != NULL);

		HRESULT hr = S_OK;
		__try
		{
			m_pObject->ThreadFun(nID);
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error,
				_T("###Err### CBaseThread::SEH_ThreadFun(): Exception 0x%X in thread 0x%X\n"),
				GetExceptionCode(), ::GetCurrentThreadId());
			hr = E_UNEXPECTED;
		}
		return hr;
	}

	void Clear()
	{
		if(m_hThread != NULL)
		{
			::CloseHandle(m_hThread);
			m_hThread = NULL;
			m_dwThreadId = 0;
		}
		if(m_pHandles != NULL)
		{
			::CloseHandle(m_pHandles[0]);
			free(m_pHandles);
			m_pHandles = NULL;
			m_nCount = 0;
		}

		m_pObject = NULL;
		m_bAutoClose = false;
	}
};

/////////////////////////////////////////////////////////////////////////////
template <class T, COINIT ThreadModel /*= COINIT_MULTITHREADED*/>
class CComBaseThread : public CBaseThread<T>
{
public:
	virtual HRESULT ThreadInitialize()
	{
		HRESULT hr = ::CoInitializeEx(NULL, ThreadModel);
		ATLASSERT(hr == S_OK);
		return hr;
	}
	virtual void ThreadUninitialize()
	{
		::CoUninitialize();
	}
};

/////////////////////////////////////////////////////////////////////////////
// Signature of thread function of class T
//		void ThreadFun(DWORD nID)
// where nID:
//	- number of signaled handle accordingly position in pHandles array (see CBaseThread::Start());
//	- WAIT_TIMEOUT if wait period expired;
//	- (DWORD)-1 if thread ending.

template <class T, COINIT ThreadModel /*= COINIT_MULTITHREADED*/>
class CComControlThread : public CComBaseThread<T, ThreadModel>
{
public:
	CComControlThread(): m_dwWait(INFINITE) {}

	void SetWait(DWORD dwWait = INFINITE)
	{
		CAutoCS lock(m_cs);

		m_dwWait = dwWait;
		if(m_hThread != NULL && ::GetCurrentThreadId() != m_dwThreadId)
		{
			ATLASSERT(m_pHandles != NULL);
			::SetEvent(m_pHandles[0]);
		}
	}

	DWORD GetWait() const { return m_dwWait; }

	//---------------------------------
protected:

	virtual HRESULT ThreadMain()
	{
		HRESULT hr = S_OK;
		while(WaitObjectsWithTimer(hr));

		SEH_ThreadFun((DWORD)-1);
		return hr;
	}

	bool WaitObjectsWithTimer(HRESULT& hr)
	{
		ATLASSERT(m_pHandles != NULL && m_nCount > 0);

		DWORD dwRet = ::WaitForMultipleObjects(m_nCount, m_pHandles, FALSE, m_dwWait);
		if(dwRet == WAIT_OBJECT_0)
		{
			CAutoCS lock(m_cs);

			if(m_lStop != 0 || m_bAutoClose || m_hThread == NULL)
			{
				// Exit from extern while() and close current thread
				// Conditions:
				// if m_lStop != 0: we in TreadStop()
				// if m_bAutoClose: we already exit from TreadStop() without waiting
				// if m_hThread == NULL : was error
				return false;
			}
			else
			{
				// Only set new m_dwWait
			}
		}
		else if((WAIT_OBJECT_0 < dwRet && dwRet < WAIT_OBJECT_0 + m_nCount) || dwRet == WAIT_TIMEOUT)
		{
			if(dwRet != WAIT_TIMEOUT)
				dwRet -= WAIT_OBJECT_0 + 1;
			hr = SEH_ThreadFun(dwRet);
		}
		else
		{
			hr = ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
			ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error,
				_T("###Err### CComControlThread::WaitObjectsWithTimer(): ")
				_T("WaitForMultipleObjects()=0x%X, hr=0x%X\n"), dwRet, hr);
		}

		// If error: return false, exit from extern while() and close current thread
		return hr == S_OK;
	}

	//---------------------------------
	DWORD	m_dwWait;
};

/////////////////////////////////////////////////////////////////////////////
// Signature of thread function of class T
//		void ThreadFun(DWORD nID)
// where nID:
//	- number of signaled handle accordingly position in pHandles array (see CBaseThread::Start());
//	- (DWORD)-1 if thread ending.

template <class T, COINIT ThreadModel /*= COINIT_MULTITHREADED*/>
class CComWinThread : public CComBaseThread<T, ThreadModel>
{
public:
	CComWinThread(bool bExitOnWMQuit = true)
	{
		m_bWaitThreadEndWithMsgLoop = true;
		m_bExitOnWMQuit = bExitOnWMQuit;
	}

	//---------------------------------
protected:

	virtual HRESULT ThreadMain()
	{
		// First call in message loop is PeekMessage(): we force the system to create
		// the message queue (see MSDN, PostThreadMessage()).
		HRESULT hr = S_OK;
		while(MsgPeek(hr) && MsgWait(hr));

		SEH_ThreadFun((DWORD)-1);
		return hr;
	}

	bool MsgPeek(HRESULT& hr)
	{
		ATLASSERT(m_pHandles != NULL && m_nCount > 0);

		DWORD dwRet;
		MSG msg;
		while(hr == S_OK && PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				ATLTRACE2(atlTraceGeneral, TRACELEVEL_High, _T("### CComWinThread::MsgPeek(): ")
					_T("WM_QUIT for thread 0x%x ###\n"), ::GetCurrentThreadId());

				// Don't converting wParam to HRESULT (with HRESULT_FROM_WIN32()), sending as is
				hr = (HRESULT)msg.wParam;

				// Don't posts message: this thread loop is upper.
				// Exits from extern while() and close current thread
				return false;
			}

			::TranslateMessage(&msg);
			::DispatchMessage(&msg);

			dwRet = ::WaitForMultipleObjects(m_nCount, m_pHandles, FALSE, 0);
			if(dwRet == WAIT_OBJECT_0)
			{
				// Exit from extern while() and close current thread
				return false;
			}

			if(WAIT_OBJECT_0 < dwRet && dwRet < WAIT_OBJECT_0 + m_nCount)
			{
				dwRet -= WAIT_OBJECT_0 + 1;
				hr = SEH_ThreadFun(dwRet);
			}
			else if(dwRet != WAIT_TIMEOUT)
			{
				hr = ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
				ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### CComWinThread::MsgPeek(): ")
					_T("WaitForMultipleObjects()=0x%X, hr=0x%X\n"), dwRet, hr);
			}
		}

		// If error: return false, exit from extern while() and close current thread
		return hr == S_OK;
	}

	bool MsgWait(HRESULT& hr)
	{
		ATLASSERT(m_pHandles != NULL && m_nCount > 0);

		while(hr == S_OK)
		{
			DWORD dwRet = ::MsgWaitForMultipleObjects(m_nCount, m_pHandles, FALSE, INFINITE, QS_ALLINPUT);
			if(dwRet == WAIT_OBJECT_0)
			{
				// Exit from extern while() and close current thread
				return false;
			}
			if(dwRet == WAIT_OBJECT_0 + m_nCount)
			{
				// Go to MsgPeek(): PeekMessage()
				return true;
			}

			if(WAIT_OBJECT_0 < dwRet && dwRet < WAIT_OBJECT_0 + m_nCount)
			{
				dwRet -= WAIT_OBJECT_0 + 1;
				hr = SEH_ThreadFun(dwRet);
			}
			else
			{
				hr = ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
				ATLTRACE2(atlTraceGeneral, TRACELEVEL_Error, _T("###Err### CComWinThread::MsgWait(): ")
					_T("MsgWaitForMultipleObjects()=0x%X, hr=0x%X\n"), dwRet, hr);
			}
		}

		// If error: return false, exit from extern while() and close current thread
		return hr == S_OK;
	}
};

/////////////////////////////////////////////////////////////////////////////
// Signature of thread function of class T
//		void ThreadFun(DWORD nID)
// where nID:
//	- number of signaled handle accordingly position in pHandles array (see CBaseThread::Start());
//	- (DWORD)-1 if thread ending.

template <class T, COINIT ThreadModel /*= COINIT_MULTITHREADED*/>
class CComUnifiedThread : public CComWinThread<T, ThreadModel>
{
	const bool m_bWinThread;

	//---------------------------------
public:
	CComUnifiedThread(bool bWinThread = false, bool bExitOnWMQuit = true) :
	  m_bWinThread(bWinThread), CComWinThread<T, ThreadModel>(bExitOnWMQuit)
	{
		m_bWaitThreadEndWithMsgLoop = m_bWinThread;
	}

	//---------------------------------
protected:
	virtual HRESULT ThreadMain()
	{
		HRESULT hr = S_OK;
		if(m_bWinThread)
			hr = CComWinThread<T, ThreadModel>::ThreadMain();
		else
			hr = CComBaseThread<T, ThreadModel>::ThreadMain();
		return hr;
	}
};

#define TRACELEVEL_ParallelThread   (10)

template <class T, class TData>
class CParallelThreads
{
	typedef CParallelThreads<T, TData> TParallelThreads;

	CSimpleThreadEx<TParallelThreads>* m_pThreads;
	int				m_nThreads;
	CStdHandle*		m_pEvents;

	CComAutoCriticalSection m_csData;
	TData*					m_pData;	

	friend class CSimpleThreadEx<TParallelThreads>;
public:

	CParallelThreads()
		: m_pThreads( NULL ),
		m_pEvents( NULL ),
		m_nThreads( 0 ),
		m_pData( NULL )
	{
	}

	~CParallelThreads()
	{
		ATLASSERT( m_pThreads == NULL && "CParallelThreads");
		ParallelStop();
	}

	// Method could be overridden
	HRESULT OnParallelComplete( TData* _pData )
	{
		return S_OK;
	}

	// Method MUST be overridden
	HRESULT OnParallelProcess( TData* _pData, int _nThread, int _nMaxThreads )
	{
		METHOD_MUST_BE_OVERRIDEN( OnParallelProcess );
	}

	HRESULT ParallelProcess( TData* _pData, bool _bAsync, int _nThreads = -1, int _nThreadMax = 4 )
	{
		ATLASSERT( _pData );
		if( !_pData ) return E_INVALIDARG;

		if( _nThreads <= 0 )
		{
			SYSTEM_INFO sysInfo ={};
			::GetSystemInfo( &sysInfo );
			_nThreads = MIN( (int)sysInfo.dwNumberOfProcessors, MAX( _nThreadMax, 1 ) );
// #ifdef DEBUG
// 			_nThreads = 63;//rand() % 27 + 1;
// #endif
		}

		if( _bAsync )
			WaitCompletion_I();

		{
			CAutoCS crs(m_csData);

			ATLASSERT( m_pData == NULL );
			m_pData = _pData;

			ATLTRACE2( atlTraceGeneral, TRACELEVEL_ParallelThread, _T("CParallelThreads::ParallelProcess() Set Data:%08X\n"), m_pData );
		}

		ParallelStart_I( _nThreads );

		if( !_bAsync )
		{
			WaitCompletion_I();
			ATLASSERT( m_pData == NULL );
		}

		return S_OK;
	}

	inline void ParallelStop(DWORD dwWait = INFINITE, bool bTerminateThread = false)
	{
		if( m_pThreads )
		{
			ATLTRACE2( atlTraceGeneral, TRACELEVEL_ParallelThread, _T("CParallelThreads::ParallelStop() Data:%08X\n"), m_pData );

			for( int i = 0; i < m_nThreads; i++ )
			{
				m_pThreads[i].Stop(dwWait, bTerminateThread);
			}

			delete [] m_pThreads;
			m_pThreads = NULL;
		}

		if( m_pEvents )
		{
			delete [] m_pEvents;
			m_pEvents = NULL;
		}
	}

private:

	HRESULT ParallelStart_I( int _nThreads )
	{
		ATLASSERT( _nThreads >= 0 );
		if( _nThreads <= 0 )
			return E_INVALIDARG;

		if( !m_pThreads || m_nThreads != _nThreads )
		{
			ATLTRACE2( atlTraceGeneral, TRACELEVEL_ParallelThread, _T("CParallelThreads::ParallelStart_I(%d) Data:%08X\n"), m_nThreads, m_pData );

			ParallelStop();

			m_pThreads = new CSimpleThreadEx<TParallelThreads>[_nThreads];
			m_pEvents = new CStdHandle[_nThreads];
			m_nThreads = _nThreads;

			for( int i = 0; i < m_nThreads; i++ )
			{
				m_pThreads[i].Start(this,i);

				m_pEvents[i].Attach( ::CreateEvent( NULL, TRUE, FALSE, NULL ) );
			}
		}


		ATLASSERT( m_pEvents );
		for( int i = 0; i < m_nThreads; i++ )
		{
			ATLTRACE2( atlTraceGeneral, TRACELEVEL_ParallelThread, _T("CParallelThreads::ParallelStart_I() AWAKE %d/%d Data:%08X\n"), i, m_nThreads, m_pData );
			::ResetEvent( m_pEvents[i] );
			m_pThreads[i].Awake();
		}

		return S_OK;
	}

	HRESULT RunThreadEx(const CSimpleThreadEx<TParallelThreads>* pThread, DWORD dwParam)
	{
		while(pThread->m_bWork)
		{
			DWORD dwRet = ::WaitForSingleObject(pThread->m_hEvent, INFINITE);
			if(!pThread->m_bWork)
				break;

			ATLASSERT(dwRet == WAIT_OBJECT_0);

			// Do something
			ATLASSERT( m_pData );
			T* pT = static_cast<T*>(this);
			HRESULT hr = pT->OnParallelProcess( m_pData, dwParam, m_nThreads );
			ATLASSERT( SUCCEEDED( hr ) );

			ATLTRACE2( atlTraceGeneral, TRACELEVEL_ParallelThread, _T("CParallelThreads::RunThreadEx() Thread %d/%d DONE Data:%08X\n"), dwParam, m_nThreads, m_pData );
			::SetEvent( m_pEvents[dwParam] );

			// Check for complete
			// DO NOT CALL THIS -> AS MAY CAUSE SET m_pData to NULL if thread come to this section after common complete is DONE.
			//WaitCompletion_I( 0 );
		}

		return S_OK;
	}

	HRESULT WaitCompletion_I( DWORD dwWait = INFINITE )
	{
		ATLASSERT( m_pEvents && m_nThreads > 0 );
		if( !m_pEvents || m_nThreads <= 0 ) return E_UNEXPECTED;

		DWORD dwRes = ::WaitForMultipleObjects( m_nThreads, (HANDLE*)m_pEvents, TRUE, dwWait );
		if( dwRes >= WAIT_OBJECT_0 + m_nThreads )
		{
			ATLTRACE2( atlTraceGeneral, TRACELEVEL_ParallelThread, _T("CParallelThreads::WaitCompletion_I(%d) NOT READY Data:%08X\n"), m_nThreads, m_pData );

			ATLASSERT( dwRes == WAIT_TIMEOUT );
			return HRESULT_FROM_WIN32( ERROR_TIMEOUT );
		}

		ATLTRACE2( atlTraceGeneral, TRACELEVEL_ParallelThread, _T("CParallelThreads::WaitCompletion_I(%d) READY:%d Data:%08X\n"), m_nThreads, dwRes, m_pData );

		HRESULT hr = S_FALSE;

		CAutoCS crs(m_csData);

		if( m_pData )
		{
			T* pT = static_cast<T*>(this);
			hr = pT->OnParallelComplete( m_pData );

			ATLTRACE2( atlTraceGeneral, TRACELEVEL_ParallelThread, _T("CParallelThreads::WaitCompletion_I(%d) DONE:%d Data:%08X\n"), m_nThreads, dwRes, m_pData );
			m_pData = NULL;
		}

		return hr;
	}
};

#pragma warning(pop)

/////////////////////////////////////////////////////////////////////////////
#endif	 // __THREADS_H_ 
