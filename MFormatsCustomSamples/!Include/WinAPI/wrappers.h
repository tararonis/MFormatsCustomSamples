//---------------------------------------------------------------------------
// This file is part of the MediaLooks Media Platform (MMP)
//
// Wrappers.h : wrapper classes for some Windows primitives
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

#ifndef __WRAPPERS_H_
#define __WRAPPERS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Macros.h"
#include "Containers.h"
#include "Basetsd.h"

#include <map>
using std::make_pair;
//#include <CS.h>

#pragma warning(push)
#pragma warning(disable:4189)

#if _WIN32_WINNT < 0x600 
#pragma error( "Need _WIN32_WINNT >= 0x600 for SRWLock" );
#endif

/////////////////////////////////////////////////////////////////////////////
template <class T> class CAutoObj;

class CVAlloc;

// Simple holder for objects (see declaration for more details)
// Some specializations of CSimpleHolder
// CStdHandle - wrapper around HANDLE - can be used with CreateFile, CreateEvent, CreateProcess etc.
// CLibraryHandle - wrapper around HINSTANCE - can be used with LoadLibrary
// CFindHandle - wrapper around HINSTANCE - can be used with FindFirstFile, FindNextFile etc.
template <class THandle, class TAction, class TValues > class CSimpleHolder;

class CHandle;
class CEvent;
class CTimer;
class CAutoCS;
class CInterLock;
class CLibrary;
class CRWSynchro;
class CAutoRW;

// Added by VVB
class CMutex;
class CAutoMutex;
class CRWSynchroLite;
class CAutoRWL;

template <class T> class DVCreator;
template <class T> HRESULT CreateComObject(T** ppObject);
static HRESULT __stdcall CreateInternalInstance(REFCLSID rclsid, REFIID riid, void** ppv);

/////////////////////////////////////////////////////////////////////////////
// CComPtr<> for class objects

template <class T>
class _NoAddRefReleaseOnAutoObj : public T
{
private:
	// For disable calls : CAutoObj<CMyClass> apClass; apClass->AddRef()/Release() :
	//		error C2248: 'Release' : cannot access private member declared in class
	//		'_NoAddRefReleaseOnAutoObj<class CMyClass>'
	STDMETHOD_(ULONG, AddRef)()=0;
	STDMETHOD_(ULONG, Release)()=0;
};

/////////////////////////////////////////////////////////////////////////////
// Abstract class for notifications
class CNotifierBase
{
public:
	virtual void Notify(int nParam1 = 0, int nParam2 = 0) = 0;
};

//////////////////////////////////////////////////////////////////////////

#define AUTOOBJ_DECLARE_INTERFACE(_top_interface) \
STDMETHOD_(ULONG, AddRef)()	\
{\
	return _AutoObjBase::AddRef();\
}\
STDMETHOD_(ULONG, Release)()\
{\
	return _AutoObjBase::Release();\
}\
STDMETHOD(QueryInterface)(\
	_In_ REFIID iid,\
	_Deref_out_ void** ppvObject) throw()\
{\
	if (IsEqualIID(iid, __uuidof(_top_interface)))\
	{\
		AddRef();\
		*ppvObject = static_cast<_top_interface*>(this);\
		return S_OK;\
	}\
	return _AutoObjBase::QueryInterface(iid, ppvObject);\
}


template <class T, class TInterface = IUnknown>
class AutoObjBase
	: public TInterface
{
	volatile LONG	m_lRef;
protected:
	typedef AutoObjBase<T, TInterface> _AutoObjBase;

public:
	typedef CAutoObj<T> TPtr;

public:
	AutoObjBase(): m_lRef( 0 )
	{
	}
	virtual ~AutoObjBase() {}

	template <class TCreate>
	static HRESULT CreateInstance( TCreate** _ppObject )
	{
		ATLASSERT( _ppObject );
		if( !_ppObject ) return E_POINTER;

		TCreate* pT = new TCreate();
		pT->AddRef();
		*_ppObject = pT;
		return S_OK;
	}

	LONG RefCount() const { return m_lRef; } 

	IUnknown* GetUnknown() throw() {return static_cast<IUnknown*>(this);}

	STDMETHOD_(ULONG, AddRef)() 
	{
		return ::InterlockedIncrement( &m_lRef );
	}
	STDMETHOD_(ULONG, Release)()
	{
		ULONG l = ::InterlockedDecrement( &m_lRef );
		if (l == 0)
			delete this;
		return l;
	}
	STDMETHOD(QueryInterface)(
		_In_ REFIID iid, 
		_Deref_out_ void** ppvObject) throw()
	{
		if( !ppvObject )
			return E_POINTER;

		if( IsEqualIID( iid, IID_IUnknown) )
		{
			AddRef();
			*ppvObject = static_cast<IUnknown*>(this);
			return S_OK;
		}

		if( IsEqualIID( iid, __uuidof(TInterface) ) )
		{
			AddRef();
			*ppvObject = static_cast<TInterface*>(this);
			return S_OK;
		}

		T* pT = static_cast<T*>(this);
		return pT->QueryInterface_I( iid, ppvObject );
	}

	HRESULT QueryInterface_I( REFIID iid,  void** ppvObject) throw()
	{
		return E_NOINTERFACE;
	}

	bool NeedAsyncRelease() { return true; }
};

template <class T, class TInterface = IUnknown>
class AutoObjBaseGC
	: public AutoObjBase<T, TInterface>
{
	// Last AddRef() / Release() time (QueryPerformanceCounter)
	LONGLONG		m_llQPCLast;
	LONGLONG		m_llQPCFreq;
	REFERENCE_TIME	m_rtQPCOffset;
public:
	AutoObjBaseGC()
		: m_rtQPCOffset( 0 )
	{
		::QueryPerformanceCounter( (LARGE_INTEGER*)&m_llQPCLast );
		::QueryPerformanceFrequency( (LARGE_INTEGER*)&m_llQPCFreq );
	}
	virtual ~AutoObjBaseGC() {}

	// Return last access time  
	LONGLONG QPCLast( REFERENCE_TIME _rtOneRefTime = DS_ONESEC * 1000, LONGLONG* _pllQPCFreq = NULL )
	{
		if( _pllQPCFreq )
			*_pllQPCFreq = m_llQPCFreq;

		return m_llQPCLast + DBL2INT64( ((double)(RefCount() - 1) * _rtOneRefTime + m_rtQPCOffset) * m_llQPCFreq / DS_ONESEC );
	}

	void QPCOffset_Set(REFERENCE_TIME _rtOffset)
	{
		m_rtQPCOffset = _rtOffset;
	}

	STDMETHOD_(ULONG, AddRef)() 
	{
		::QueryPerformanceCounter( (LARGE_INTEGER*)&m_llQPCLast );

		return _AutoObjBase::AddRef();
	}
	STDMETHOD_(ULONG, Release)()
	{
		::QueryPerformanceCounter( (LARGE_INTEGER*)&m_llQPCLast );
		return _AutoObjBase::Release();
	}
};

/*

//---------------------------------
template <class T>
class CAutoObj
{
public:
	typedef typename T TContent;

	CAutoObj()
	{
		p = NULL;
	}
	explicit CAutoObj(T* pT)
	{
		p = pT;
		if(p)
			p->AddRef();
	}
	CAutoObj(const CAutoObj<T>& apT)
	{
		p = apT.p;
		if(p)
			p->AddRef();
	}
	~CAutoObj()
	{
		if(p)
			p->Release();
	}
	void Release()
	{
		T* pT = p;
		if(pT)
		{
			p = NULL;
			pT->Release();
		}
	}
	operator T*() const
	{
		return p;
	}
	T& operator*() const
	{
		ATLASSERT(p!=NULL);
		return *p;
	}
	T** operator&()
	{
		ATLASSERT(p==NULL);
		return &p;
	}
	_NoAddRefReleaseOnAutoObj<T>* operator->() const
	{
		ATLASSERT(p!=NULL);
		return (_NoAddRefReleaseOnAutoObj<T>*)p;
	}
	T* operator=(T* pT)
	{
		if(pT != NULL)
			pT->AddRef();
		if(p)
			p->Release();
		p = pT;
		return p;
	}
	T* operator=(const CAutoObj<T>& apT)
	{
		return (*this = apT.p);
	}
	bool operator!() const
	{
		return (p == NULL);
	}
	bool operator==(T* pT) const
	{
		return p == pT;
	}
	void Attach(T* pT)
	{
		if(p)
			p->Release();
		p = pT;
	}
	T* Detach()
	{
		T* pT = p;
		p = NULL;
		return pT;
	}
	HRESULT CopyTo(T** ppT)
	{
		ATLASSERT(ppT != NULL);
		if(ppT == NULL)
			return E_POINTER;

		*ppT = p;
		if(p)
			p->AddRef();
		return S_OK;
	}
	HRESULT Create()
	{
		ATLASSERT(p == NULL);

		CComObject<T>* pObj = NULL;
		HRESULT hr = CComObject<T>::CreateInstance(&pObj);
		if(SUCCEEDED(hr))
		{
			pObj->AddRef();
			p = static_cast<T*>(pObj);
		}
		return hr;
	}

	bool IsEqualObject( IUnknown* pOther) 
	{
		if (p == NULL && pOther == NULL)
			return true;	// They are both NULL objects

		if (p == NULL || pOther == NULL)
			return false;	// One is NULL the other is not

		CComPtr<IUnknown> punk1;
		CComPtr<IUnknown> punk2;
		p->QueryInterface(__uuidof(IUnknown), (void**)&punk1);
		pOther->QueryInterface(__uuidof(IUnknown), (void**)&punk2);
		return punk1 == punk2;
	}

	bool IsEqualObject( const CAutoObj<T>& pOther) 
	{
		if (p == NULL && pOther == NULL)
			return true;	// They are both NULL objects

		if (p == NULL || pOther == NULL)
			return false;	// One is NULL the other is not

		CComPtr<IUnknown> punk1;
		CComPtr<IUnknown> punk2;
		p->QueryInterface(__uuidof(IUnknown), (void**)&punk1);
		pOther->QueryInterface(__uuidof(IUnknown), (void**)&punk2);
		return punk1 == punk2;
	}



	template <class Q>
	HRESULT QueryInterface(Q** pp) const
	{
		ATLASSERT(pp != NULL && *pp == NULL);
		ATLASSERT(p != NULL);
		if( p )
			return p->QueryInterface(__uuidof(Q), (void**)pp);

		return E_NOINTERFACE;
	}

	T* p;
};

*/

//---------------------------------
template <class T>
class CAutoObj
{
public:
	T* p;
protected:

	typedef typename CAutoObj<T> _AutoObj;

	mutable SRWLOCK srw_lock;

	void Lock( bool _bWrite ) const throw()
	{
		if( _bWrite )
			::AcquireSRWLockExclusive( &srw_lock );
		else
			::AcquireSRWLockShared( &srw_lock );
	}
	void Unlock( bool _bWrite ) const throw()
	{
		if( _bWrite )
			::ReleaseSRWLockExclusive( &srw_lock );
		else
			::ReleaseSRWLockShared( &srw_lock );
	}

	T* GetPtr_With_AddRef() const throw()
	{
		Lock(false);
		T* pT = p;
		if( pT )
			pT->AddRef();
		Unlock(false);
		return pT;
	}
public:
	typedef typename T TContent;

	CAutoObj()
		: p( NULL )
	{
		::InitializeSRWLock( &srw_lock );
	}
	explicit CAutoObj(T* pT)
		: p( pT )
	{
		::InitializeSRWLock( &srw_lock );
		if(p)
			p->AddRef();
	}
	CAutoObj(const CAutoObj<T>& apT)
	{
		::InitializeSRWLock( &srw_lock );
		p = apT.GetPtr_With_AddRef();
	}
	~CAutoObj()
	{
		T* pT = (T*)::InterlockedExchangePointer((void**)&p, NULL);
		if(pT)
			pT->Release();
	}
	void Release()
	{
		Lock(true);
		T* pT = (T*)::InterlockedExchangePointer((void**)&p, NULL);
		Unlock(true);

		if(pT)
			pT->Release();
	}

	bool Release_Async()
	{
		Lock(true);
		T* pT = (T*)::InterlockedExchangePointer((void**)&p, NULL);
		Unlock(true);

		if (pT)
		{
			if (pT->NeedAsyncRelease())
			{
				::QueueUserWorkItem(_Release_Async_Static, pT, 0);
				return true;
			}

			pT->Release();
		}
		return false;
	}

	static DWORD WINAPI _Release_Async_Static(LPVOID lpThreadParameter)
	{
		T* pT = reinterpret_cast<T*>(lpThreadParameter);
		ATLASSERT(pT);
		return pT->Release();
	}

	operator T*() const
	{
		return p;
	}
	
	T& operator*() const
	{
		ATLASSERT(p!=NULL);
		return *p;
	}
	T** operator&()
	{
		ATLASSERT(p==NULL);
		return &p;
	}
	_NoAddRefReleaseOnAutoObj<T>* operator->() const
	{
		ATLASSERT(p!=NULL);
		return (_NoAddRefReleaseOnAutoObj<T>*)p;
	}
	T* operator=(T* _pT)
	{
		// WARNING !!! Method is MT safe BUT AddRef() is NOT locked 
		if(_pT != NULL)
			_pT->AddRef();

		Lock(true);
		T* pT = (T*)::InterlockedExchangePointer((void**)&p, _pT);
		Unlock(true);

		if(pT)
			pT->Release();
		return _pT;
	}
	T* operator=(const CAutoObj<T>& apT)
	{
		T* pSet = apT.GetPtr_With_AddRef();

		Lock(true);
		T* pT = (T*)::InterlockedExchangePointer((void**)&p, pSet);
		Unlock(true);

		if(pT)
			pT->Release();
		return pSet;
	}
	bool operator!() const
	{
		return (p == NULL);
	}
	bool operator==(T* pT) const
	{
		return p == pT;
	}
	void Attach(T* _pT)
	{
		Lock(true);
		T* pT = (T*)::InterlockedExchangePointer((void**)&p, _pT);
		Unlock(true);

		if(pT)
			pT->Release();
	}
	T* Detach()
	{
		Lock(true);
		T* pT = (T*)::InterlockedExchangePointer((void**)&p, NULL);
		Unlock(true);
		return pT;
	}
	HRESULT CopyTo(T** ppT) const
	{
		ATLASSERT(ppT != NULL);
		if(ppT == NULL)
			return E_POINTER;

		*ppT = GetPtr_With_AddRef();
		return S_OK;
	}
	HRESULT Create()
	{
		ATLASSERT(p == NULL);
		if( p )
			Release();

		CComObject<T>* pObj = NULL;
		HRESULT hr = CComObject<T>::CreateInstance(&pObj);
		if(SUCCEEDED(hr))
		{
			pObj->AddRef();
			p = static_cast<T*>(pObj);
		}
		return hr;
	}

	bool IsEqualObject( IUnknown* pOther) const
	{
		T* pThis = GetPtr_With_AddRef();
		if (pThis == NULL && pOther == NULL)
			return true;	// They are both NULL objects

		if (pThis == NULL || pOther == NULL)
		{
			if( pThis )
				pThis->Release();

			return false;	// One is NULL the other is not
		}

		CComPtr<IUnknown> punk1;
		CComPtr<IUnknown> punk2;
		pThis->QueryInterface(__uuidof(IUnknown), (void**)&punk1);
		pOther->QueryInterface(__uuidof(IUnknown), (void**)&punk2);
		pThis->Release();
		return punk1 == punk2;
	}

	bool IsEqualObject( const CAutoObj<T>& _pOther) const
	{
		T* pThis = GetPtr_With_AddRef();
		T* pOther = _pOther.GetPtr_With_AddRef();

		if (pThis == NULL && pOther == NULL)
			return true;	// They are both NULL objects

		if (pThis == NULL || pOther == NULL)
		{
			if( pThis )
				pThis->Release();
			if( pOther )
				pOther->Release();

			return false;	// One is NULL the other is not
		}

		CComPtr<IUnknown> punk1;
		CComPtr<IUnknown> punk2;
		pThis->QueryInterface(__uuidof(IUnknown), (void**)&punk1);
		pOther->QueryInterface(__uuidof(IUnknown), (void**)&punk2);

		pThis->Release();
		pOther->Release();

		return punk1 == punk2;
	}

	template <class Q>
	HRESULT QueryInterface(Q** pp) const
	{
		ATLASSERT(pp != NULL && *pp == NULL);
		T* pThis = GetPtr_With_AddRef();
		ATLASSERT( pThis );
		if( pThis )
		{
			HRESULT hr = pThis->QueryInterface(__uuidof(Q), (void**)pp);
			pThis->Release();
			return hr;
		}

		return E_NOINTERFACE;
	}
};

//////////////////////////////////////////////////////////////////////////
class CMAlloc
{
	void*	m_pMemory;
	void*	m_pAlignedMemory;
	DWORD	m_cbSize;

	//---------------------------------
public:
	CMAlloc()
	{
		m_pMemory = m_pAlignedMemory = NULL;
		m_cbSize = 0;
	}

	~CMAlloc()
	{
		Free();
	}

	//---------------------------------
	bool operator!() const
	{
		return m_pAlignedMemory == NULL;
	}
	operator void*() const
	{
		return m_pAlignedMemory;
	}
	BYTE* Memory() const
	{
		return (BYTE*)m_pAlignedMemory;
	}
	DWORD Size() const
	{
		return m_cbSize;
	}

	//---------------------------------
	bool Alloc(DWORD cbSize, DWORD cbAlignment = 0)
	{
		// cbAlignment must be a power of 2 or 0
		ATLASSERT((-(LONG)cbAlignment & cbAlignment) == cbAlignment);

		Free();

		if(cbAlignment != 0)
			--cbAlignment;

		// Alloc also if cbSize = 0
		m_pMemory = malloc(cbAlignment + (cbSize > 0)? cbSize : 1);
		if(m_pMemory == NULL)
			return false;

		m_pAlignedMemory = (void*)(((DWORD_PTR)m_pMemory + (DWORD_PTR)cbAlignment) & ~(DWORD_PTR)cbAlignment);
		m_cbSize = cbSize;
		return true;
	}
	void Free()
	{
		free(m_pMemory);
		m_pMemory = m_pAlignedMemory = NULL;
		m_cbSize = 0;
	}
	void MoveTo(CMAlloc& mem)
	{
		mem.Free();
		mem.m_pMemory = m_pMemory;
		mem.m_cbSize = m_cbSize;
		mem.m_pAlignedMemory = m_pAlignedMemory;
		m_pMemory = m_pAlignedMemory = NULL;
		m_cbSize = 0;
	}
};



//////////////////////////////////////////////////////////////////////////
class CVAlloc
{
	static DWORD s_dwAllocationGranularity;
	void*	m_pMemory;
	DWORD	m_cbSize;

public:

	//---------------------------------
	CVAlloc()
	{
		m_pMemory = NULL;
		m_cbSize = 0;
	}

	~CVAlloc() { Free(); }

	//---------------------------------
	bool operator!() const
	{
		return m_pMemory == NULL;
	}
	operator void*() const
	{
		return m_pMemory;
	}
	BYTE* Memory() const
	{
		return (BYTE*)m_pMemory;
	}
	DWORD Size() const
	{
		return m_cbSize;
	}
	DWORD AllocatedSize() const
	{
		if(m_pMemory == NULL)
			return 0;

		if(s_dwAllocationGranularity == 0)
		{
			SYSTEM_INFO rgSysInfo;
			GetSystemInfo(&rgSysInfo);
			s_dwAllocationGranularity = rgSysInfo.dwAllocationGranularity;
		}

		if(m_cbSize == 0)
			return s_dwAllocationGranularity;

		return (m_cbSize + s_dwAllocationGranularity - 1) & ~(s_dwAllocationGranularity - 1);
	}

	//---------------------------------
	bool Alloc(DWORD cbSize)
	{
		Free();

		// Alloc also if cbSize = 0
		m_pMemory = ::VirtualAlloc(NULL, (cbSize > 0)? cbSize : 1, MEM_COMMIT, PAGE_READWRITE);
		if(m_pMemory == NULL)
			return false;

		m_cbSize = cbSize;
		return true;
	}
	void Free()
	{
		if(m_pMemory != NULL)
		{
			::VirtualFree(m_pMemory, 0, MEM_RELEASE);
			m_pMemory = NULL;
			m_cbSize = 0;
		}
	}
	void MoveTo(CVAlloc& mem)
	{
		mem.Free();
		mem.m_pMemory = m_pMemory;
		mem.m_cbSize = m_cbSize;
		m_pMemory = NULL;
		m_cbSize = 0;
	}
};

__declspec(selectany) DWORD CVAlloc::s_dwAllocationGranularity = 0;

//////////////////////////////////////////////////////////////////////////
// Simple objects holder
// Template arguments:
//		THandle - holdered class
//		TAction - static class for simplest actions
//				  Set( THandle& _hTarget, const THandle& _hValue ) - set new value ( without duplicate )
//				  Copy( THandle& _hTarget, const THandle& _hValue ) - copy value ( with duplicate )
//				  Close( THandle& _hTarget ) - close objects
//
//		TValues - static class for objects values - see impl BaseValues<THandle>

template <class THandle, class TAction, class TValues >
class CSimpleHolder
{
public:
	// Holdered handle
	THandle		m_hHandle;

public:
	// Simple constructor
	CSimpleHolder( const THandle& _hHandle = TValues::NullValue() ) : m_hHandle( _hHandle ) {}

	// Copy constructor
	CSimpleHolder( const CSimpleHolder& _rHandleHolder )
	{
		m_hHandle = TValues::NullValue();

		if( TValues::IsValid(_rHandleHolder) )
		{
			TAction::Copy( m_hHandle, _rHandleHolder );
		}
	}

	// Simple destructor
	~CSimpleHolder()
	{
		Close();
	}

	inline CSimpleHolder& operator = ( const CSimpleHolder& _rHandleHolder )
	{
		if( _rHandleHolder.Ptr() != this )
		{
			if (m_hHandle != NULL)
			{
				TAction::Close( m_hHandle );
				m_hHandle = NULL;
			}
			if( TValues::IsValid(_rHandleHolder) )
			{
				TAction::Copy( m_hHandle, _rHandleHolder );
			}
		}
		
		return *this;
	}

	inline HRESULT Attach( const THandle& _hHandle ) 
	{
		// Check for the same handle or both invalid handles
		ATLASSERT( (*this) != _hHandle || !TValues::IsValid( m_hHandle ) );
		if( (*this) == _hHandle ) 
		{
			return TValues::IsValid( m_hHandle ) ? E_INVALIDARG : S_OK;
		}
		
		TAction::Close( m_hHandle );
		TAction::Set( m_hHandle, _hHandle );
	
		return S_OK;
	}

	inline THandle Detach()	
	{
		THandle hTmp = TValues::NullValue();

		TAction::Set( hTmp, m_hHandle );
		TAction::Set( m_hHandle, TValues::NullValue() );

		return hTmp;
	}

	inline operator THandle() const	
	{
		if( !TValues::IsValid(m_hHandle) )
			return TValues::NullValue();
		
		return m_hHandle;
	}

// 	inline const THandle* operator &() const	
// 	{
// 		return &m_hHandle;
// 	}

	inline THandle* operator &()
	{
		// ATLASSERT( !TValues::IsValid(m_hHandle) ); -> Used for WAIT_XXX functions

		return &m_hHandle;
	}

	inline HRESULT Close()
	{
		HRESULT hr = S_OK;
		if( TValues::IsValid( m_hHandle ) )
		{
			hr = TAction::Close( m_hHandle );
		}
		
		TAction::Set( m_hHandle, TValues::NullValue() );

		return hr;
	}

	inline bool operator == ( const THandle& _hHandle ) const
	{
		if( !TValues::IsValid( m_hHandle ) && !TValues::IsValid( _hHandle ) )
			return true;

		return TValues::IsEqual( *this, _hHandle );
	}

	inline bool operator != ( const THandle& _hHandle ) const
	{
		return !(*this == _hHandle);
	}

	inline bool IsValid() const
	{
		return TValues::IsValid( m_hHandle );
	}

	inline bool operator !() const
	{
		return !TValues::IsValid( m_hHandle );
	}

private:
	// Only for using in operator = ( const CSimpleHolder& )
	inline const CSimpleHolder* Ptr() const	{
		return this;
	}
};

//////////////////////////////////////////////////////////////////////////
// Base (common) values definition
template< class THandle >
struct BaseValues
{
	// Return initial (always invalid value of handle)
	static inline THandle NullValue()
	{ 
		return NULL;
	}
	// Compare two values ( true if equal, false overwise )
	static inline bool IsEqual( const THandle& _hFirst, const THandle& _hSecond )
	{
		return _hFirst == _hSecond;
	}
	// Return true if _hCheck have valid value and false overwise
	static inline bool IsValid( const THandle& _hCheck )
	{
		return IsEqual( NullValue(), _hCheck ) == false;
	}
};

//////////////////////////////////////////////////////////////////////////
// Simple wrapper around HANDLE, ::CloseHandle, ::DuplicateHandle
// Use with OpenFile, CreateEvent and many others
struct StdHandleAction
{
	static inline void Set( HANDLE& _hTarget, const HANDLE& _hValue )
	{
		_hTarget = _hValue;
	}
	static inline HRESULT Close( const HANDLE& _hTarget ) 
	{
		if( _hTarget && _hTarget != INVALID_HANDLE_VALUE )
		{
			if( !::CloseHandle( _hTarget ) )
			{
				return ERROR_FROM_WIN32( GetLastError(), E_FAIL );
			}
		}

		return  S_OK;
	}

	static inline void Copy( HANDLE& _hTarget, const HANDLE& _hValue ) 
	{
		BOOL bSuccess = ::DuplicateHandle( ::GetCurrentProcess(), 
						                   _hValue,
						                   ::GetCurrentProcess(), 
						                   &_hTarget,
						                   0,
						                   FALSE,
						                   DUPLICATE_SAME_ACCESS );
		bSuccess;
		ATLASSERT( bSuccess );
	}
};

struct StdHandleValues
	: public BaseValues<HANDLE>
{
	static inline bool IsValid( const HANDLE& _hCheck )
	{
		return _hCheck != NULL && _hCheck != INVALID_HANDLE_VALUE;
	}
};

typedef CSimpleHolder< HANDLE, StdHandleAction, StdHandleValues > CStdHandle;




//////////////////////////////////////////////////////////////////////////
// Simple wrapper around HINSTANCE, ::LoadLibrary etc.
// NOTE !!! (**) Copy disallowed
struct LibraryAction
{
	static inline void Set( HINSTANCE& _hTarget, const HINSTANCE& _hValue )
	{
		_hTarget = _hValue;
	}
	static inline HRESULT Close( const HINSTANCE& _hTarget ) 
	{
		if( (_hTarget != NULL) && !::FreeLibrary( _hTarget ) )
		{
			return ERROR_FROM_WIN32( GetLastError(), E_FAIL );
		}

		return S_OK;
	}
};

typedef CSimpleHolder< HINSTANCE, LibraryAction, BaseValues<HINSTANCE> > CLibraryHandle;

//////////////////////////////////////////////////////////////////////////
// Simple wrapper around HANDLE, ::FindFirstFile etc.
// NOTE !!! (**) Copy disallowed
struct FindAction
{
	static inline void Set( HANDLE& _hTarget, const HANDLE& _hValue )
	{
		_hTarget = _hValue;
	}
	static inline HRESULT Close( const HANDLE& _hTarget ) 
	{
		if( !::FindClose( _hTarget ) )
		{
			return ERROR_FROM_WIN32( GetLastError(), E_FAIL );
		}

		return S_OK;
	}
};

typedef CSimpleHolder< HANDLE, FindAction, BaseValues<HANDLE> > CFindHandle;

//////////////////////////////////////////////////////////////////////////
// For use with Graphics Objects

template <class T>
struct GDIAction
{
	static inline void Set( T& _hTarget, const T& _hValue )
	{
		_hTarget = _hValue;
	}
	static inline HRESULT Close( const T& _hTarget ) 
	{
		if( !::DeleteObject( _hTarget ) )
		{
			return ERROR_FROM_WIN32( GetLastError(), E_FAIL );
		}

		return S_OK;
	}
};

template <class T> 
class CGDIHandle:	public CSimpleHolder< T, GDIAction<T>, BaseValues<T> >
{
public:
	CGDIHandle( const T& _val = BaseValues<T>::NullValue() ) 
		: CSimpleHolder< T, GDIAction<T>, BaseValues<T> >( _val )
	{
	}
};

struct DCAction
{
	static inline void Set( HDC& _hTarget, const HDC& _hValue )
	{
		_hTarget = _hValue;
	}
	static inline HRESULT Close( const HDC& _hTarget ) 
	{
		if( !::DeleteDC( _hTarget ) )
		{
			return ERROR_FROM_WIN32( GetLastError(), E_FAIL );
		}

		return S_OK;
	}
};

typedef CSimpleHolder< HDC, DCAction, BaseValues<HDC> > SDCHandle;

/////////////////////////////////////////////////////////////////////////////
// CHandle: creates or holds external created handle and close it automatically.
class CHandle
{
public:
	CHandle(HANDLE h = NULL): m_h(h) {}
	virtual ~CHandle() { Close(); }

	void Close()
	{
		if(m_h != NULL)
		{
			::CloseHandle(m_h);
			m_h = NULL;
		}
	}
	void Attach(HANDLE h)
	{
		if(h != m_h)
		{
			Close();
			m_h = h;
		}
	}
	HANDLE Detach()
	{
		HANDLE h = m_h;
		m_h = NULL;
		return h;
	}
	operator HANDLE() const { return m_h; }
	bool operator!() const { return m_h == NULL; }

protected:
	HANDLE	m_h;
};

/////////////////////////////////////////////////////////////////////////////
// CEvent
class CEvent : public ::CHandle
{
public:
	CEvent(HANDLE h = NULL): CHandle(h) {}

	bool Create(BOOL bManualReset = FALSE)
	{
		Close();

		m_h = ::CreateEvent(NULL, bManualReset, FALSE, NULL);
		return (m_h != NULL);
	}
	bool SetEvent()
	{
		ATLASSERT(m_h != NULL);
		if(m_h == NULL) return false;
		return (::SetEvent(m_h) == TRUE);
	}
	bool ResetEvent()
	{
		ATLASSERT(m_h != NULL);
		if(m_h == NULL) return false;
		return (::ResetEvent(m_h) == TRUE);
	}
};

/////////////////////////////////////////////////////////////////////////////
// CTimer
class CTimer : public ::CHandle
{
	BOOL m_bManual;

public:
	CTimer() : m_bManual(FALSE) {}

	bool Create(BOOL bManualReset = FALSE)
	{
		Close();

		m_h = ::CreateWaitableTimer(NULL, bManualReset, NULL);
		m_bManual = bManualReset;
		return (m_h != NULL);
	}
	bool Reset()
	{
		if(m_h == NULL)
			return false;

		if(m_bManual)
		{
			// SetTime(): resets manual timer to non-signaled state
			SetTime(0x8000000000000000);
			::CancelWaitableTimer(m_h);
		}
		else
		{
			::CancelWaitableTimer(m_h);
			::WaitForSingleObject(m_h, 0);
		}
		return true;
	}
	bool SetPeriod(LONG lPeriod)
	{
		ATLASSERT(m_h != NULL);
		if(m_h == NULL) return false;

		LARGE_INTEGER liDueTime;
		liDueTime.QuadPart = 0;
		BOOL bRet = ::SetWaitableTimer(m_h, &liDueTime, lPeriod, NULL, NULL, FALSE);
		return (bRet == TRUE);
	}
	bool SetTime(LONGLONG llTime)
	{
		ATLASSERT(m_h != NULL);
		if(m_h == NULL) return false;

		LARGE_INTEGER liDueTime;
		liDueTime.QuadPart = llTime;
		BOOL bRet = ::SetWaitableTimer(m_h, &liDueTime, NULL, NULL, NULL, FALSE);
		return (bRet == TRUE);
	}
};


//////////////////////////////////////////////////////////////////////////
// CMutex

class CMutex
{
	friend class CAutoMutex;

	// Used in CAutoMutex
	CMutex( int nFake )	{ m_hMutex = NULL;	}

public:
	HANDLE m_hMutex;
	CMutex( LPCWSTR _pszName = NULL )
	{
		m_hMutex = ::CreateMutexW( NULL, FALSE, _pszName );
	}
	~CMutex()
	{
		if( m_hMutex )
			::CloseHandle( m_hMutex );
	}
};

//////////////////////////////////////////////////////////////////////////
// CAutoMutex - Locks a mutex by reference or name, and unlocks it automatically

class CAutoMutex
{
public:

	// Lock mutex by reference
	CAutoMutex( CMutex& _rMutex )
		: m_theMutex( -1 )
	{
		m_pMutex = &_rMutex;

		::WaitForSingleObject( m_pMutex->m_hMutex, INFINITE );
	}

	// Lock mutex by process-wide name
	CAutoMutex( LPCWSTR _pszMutex )
		: m_theMutex( _pszMutex )
	{
		m_pMutex = &m_theMutex;
		::WaitForSingleObject( m_pMutex->m_hMutex, INFINITE );
	}
	~CAutoMutex()
	{
		::ReleaseMutex( m_pMutex->m_hMutex );
	}

private:
	// The new/fake mutex object
	CMutex	m_theMutex;
	CMutex* m_pMutex;
};



/////////////////////////////////////////////////////////////////////////////
// Locks a critical section, and unlocks it automatically
// 

#ifdef _DEBUG
#define VERIFY_CS( cs ) ATLASSERT( (DWORD)cs.m_sec.OwningThread == ::GetCurrentThreadId() );
#else
#define VERIFY_CS( cs )
#endif

class CAutoCS
{
	CAutoCS(const CAutoCS& autoCS);
	CAutoCS &operator=(const CAutoCS& autoCS);

public:
	CAutoCS(volatile CComAutoCriticalSection* pCS, LPCWSTR _pszFakeComment = NULL) : m_pCS(pCS) { if (m_pCS) ((CComAutoCriticalSection*)m_pCS)->Lock(); }
	CAutoCS(CComAutoCriticalSection* pCS, LPCWSTR _pszFakeComment = NULL) : m_pCS(pCS) { if(m_pCS) ((CComAutoCriticalSection*)m_pCS)->Lock(); }
	CAutoCS(CComAutoCriticalSection& cs, LPCWSTR _pszFakeComment = NULL ) : m_pCS(&cs) { ((CComAutoCriticalSection*)m_pCS)->Lock(); }
	CAutoCS(CComFakeCriticalSection& cs, LPCWSTR _pszFakeComment = NULL) : m_pCS(NULL) {}
	~CAutoCS() { if(m_pCS) ((CComAutoCriticalSection*)m_pCS)->Unlock(); }
private:
	volatile CComAutoCriticalSection* m_pCS;
};

#ifndef TRACELEVEL_MutexDbg
#define TRACELEVEL_MutexDbg (10)
#endif

#define AUTO_MTX_INFO_SIZE	(256)

class CAutoMTX
{
	CAutoMTX(const CAutoMTX& autoCS);
	CAutoMTX &operator=(const CAutoMTX& autoCS);

public:
	CAutoMTX(HANDLE _hMutex, LPDWORD _pdwLockedThreadID = NULL, LPSTR _pszInfo = NULL, LPCSTR _pszLockInfo = NULL ) : m_hMutex(_hMutex), m_pdwThreadID( _pdwLockedThreadID ), m_pszInfo(_pszInfo)
	{ 
		if (_hMutex)
		{
			if (m_pszInfo && _pszLockInfo)
				lstrcpynA(m_pszInfo, _pszLockInfo, AUTO_MTX_INFO_SIZE);

			MTRACE_L(TRACELEVEL_MutexDbg, "Lock Mutex:%I64X Thread:%04X(%d) - WAIT(%hs)", (ULONGLONG)_hMutex, GetCurrentThreadId(), GetCurrentThreadId(), m_pszInfo);

			::WaitForSingleObject(m_hMutex, INFINITE);
			if (m_pdwThreadID)
				*m_pdwThreadID = GetCurrentThreadId();
			
			MTRACE_L(TRACELEVEL_MutexDbg, "Lock Mutex:%I64X Thread:%04X(%d) - LOCK(%hs)", (ULONGLONG)_hMutex, GetCurrentThreadId(), GetCurrentThreadId(), m_pszInfo);
		}
	}
	~CAutoMTX() 
	{ 
		if (m_hMutex)
		{
			MTRACE_L(TRACELEVEL_MutexDbg, "Lock Mutex:%I64X Thread:%04X(%d) - UNLOCK(%hs)", (ULONGLONG)m_hMutex, GetCurrentThreadId(), GetCurrentThreadId(), m_pszInfo);
			if (m_pdwThreadID)
				*m_pdwThreadID = NULL;
			::ReleaseMutex(m_hMutex);
		}
	}
private:
	HANDLE m_hMutex;
	LPDWORD m_pdwThreadID;
	LPSTR	m_pszInfo;
	
};

// class CComMultiThreadModel_ML
// {
// public:
// 	static ULONG WINAPI Increment( LPLONG p) throw()
// 	{
// 		return InterlockedIncrement(p);
// 	}
// 	static ULONG WINAPI Decrement( LPLONG p) throw()
// 	{
// 		return InterlockedDecrement(p);
// 	}
// 	typedef CComAutoCriticalSection AutoCriticalSection;
// 	typedef CComAutoCriticalSection AutoDeleteCriticalSection;
// 	typedef CComAutoCriticalSection CriticalSection;
// 	typedef CComMultiThreadModelNoCS ThreadModelNoCS;
// };
// 
// class CDbgAutoCS
// {
// 	CDbgAutoCS(const CDbgAutoCS& autoCS);
// 	CDbgAutoCS &operator=(const CDbgAutoCS& autoCS);
// 
// public:
// 	CDbgAutoCS(CComAutoCriticalSection& cs) : m_cs(cs) 
// 	{ 
// 		int nThread = GetCurrentThreadId();
// 		ATLTRACE2( atlTraceGeneral, 2, _T("[CS 0x%08x] %04X> : LockCount=%d,%d,%d (%d) RecursionCount=%d OwningThread:%04X Count:%d \n"), &m_cs.m_sec, nThread, 
// 			(m_cs.m_sec.LockCount & 0x1) ? 0 : 1,
// 			((m_cs.m_sec.LockCount & 0x2) >> 1 ) ? 0 : 1,
// 			(-1 - m_cs.m_sec.LockCount) >> 2,
// 			m_cs.m_sec.LockCount,
// 			m_cs.m_sec.RecursionCount, 
// 			m_cs.m_sec.OwningThread,
// 			m_cs.m_sec.DebugInfo->ContentionCount );
// 		m_cs.Lock(); 
// 		ATLTRACE2( atlTraceGeneral, 2, _T("[CS 0x%08x] %04X>>: LockCount=%d,%d,%d (%d) RecursionCount=%d OwningThread:%04X Count:%d\n"), &m_cs.m_sec, nThread, 
// 			(m_cs.m_sec.LockCount & 0x1) ? 0 : 1,
// 			((m_cs.m_sec.LockCount & 0x2) >> 1 ) ? 0 : 1,
// 			(-1 - m_cs.m_sec.LockCount) >> 2,
// 			m_cs.m_sec.LockCount,
// 			m_cs.m_sec.RecursionCount, 
// 			m_cs.m_sec.OwningThread,
// 			m_cs.m_sec.DebugInfo->ContentionCount );
// 	}
// 	~CDbgAutoCS() 
// 	{ 
// 		int nThread = GetCurrentThreadId();
// 		ATLTRACE2( atlTraceGeneral, 2, _T("[CS 0x%08x] %04X<<: LockCount=%d,%d,%d (%d) RecursionCount=%d OwningThread:%04X Count:%d\n"), &m_cs.m_sec, nThread, 
// 			(m_cs.m_sec.LockCount & 0x1) ? 0 : 1,
// 			((m_cs.m_sec.LockCount & 0x2) >> 1 ) ? 0 : 1,
// 			(-1 - m_cs.m_sec.LockCount) >> 2,
// 			m_cs.m_sec.LockCount,
// 			m_cs.m_sec.RecursionCount, 
// 			m_cs.m_sec.OwningThread,
// 			m_cs.m_sec.DebugInfo->ContentionCount );
// 		m_cs.Unlock(); 
// 		ATLTRACE2( atlTraceGeneral, 2, _T("[CS 0x%08x] %04X< : LockCount=%d,%d,%d (%d) RecursionCount=%d OwningThread:%04X Count:%d\n"), &m_cs.m_sec, nThread, 
// 			(m_cs.m_sec.LockCount & 0x1) ? 0 : 1,
// 			((m_cs.m_sec.LockCount & 0x2) >> 1 ) ? 0 : 1,
// 			(-1 - m_cs.m_sec.LockCount) >> 2,
// 			m_cs.m_sec.LockCount,
// 			m_cs.m_sec.RecursionCount, 
// 			m_cs.m_sec.OwningThread,
// 			m_cs.m_sec.DebugInfo->ContentionCount );
// 	}
// private:
// 	CComAutoCriticalSection& m_cs;
// };

// class CComDbgCriticalSection : 
// 	public CComAutoCriticalSection
// {
// public:
// 	HRESULT Lock() throw()
// 	{
// 		ATLTRACE2( atlTraceGeneral, 2, _T("[CS.Lock %08X] Before: LockCount=%d RecursionCount=%d OwningThread:%04X\n"), this, m_sec.LockCount, m_sec.RecursionCount, m_sec.OwningThread );
// 		EnterCriticalSection(&m_sec);
// 		ATLTRACE2( atlTraceGeneral, 2, _T("[CS.Lock %08X] After: LockCount=%d RecursionCount=%d OwningThread:%04X\n"), this, m_sec.LockCount, m_sec.RecursionCount, m_sec.OwningThread );
// 		return S_OK;
// 	}
// 	HRESULT Unlock() throw()
// 	{
// 		ATLTRACE2( atlTraceGeneral, 2, _T("[CS.Unlock %08X] Before: LockCount=%d RecursionCount=%d OwningThread:%04X\n"), this, m_sec.LockCount, m_sec.RecursionCount, m_sec.OwningThread );
// 
// 		LeaveCriticalSection(&m_sec);
// 
// 		ATLTRACE2( atlTraceGeneral, 2, _T("[CS.Unlock %08X] After: LockCount=%d RecursionCount=%d OwningThread:%04X\n"), this, m_sec.LockCount, m_sec.RecursionCount, m_sec.OwningThread );
// 		return S_OK;
// 	}
// private :
// 	HRESULT Init(); // Not implemented. CComAutoCriticalSection::Init should never be called
// 	HRESULT Term(); // Not implemented. CComAutoCriticalSection::Term should never be called
// };

/////////////////////////////////////////////////////////////////////////////
// Wrapper class for InterlockedIncrement/Decrement

class CInterLock
{
	volatile LONG* m_pl;

public :
	CInterLock(volatile LONG* pl) : m_pl(pl)
	{
		ATLASSERT(m_pl != NULL);
		if(m_pl != NULL)
			::InterlockedIncrement((LPLONG)m_pl);
	}
	~CInterLock()
	{
		if(m_pl)
			::InterlockedDecrement((LPLONG)m_pl);
	}
	void Reset()
	{
		if(m_pl)
		{
			::InterlockedDecrement((LPLONG)m_pl);
			m_pl = NULL;
		}
	}
};

/////////////////////////////////////////////////////////////////////////////
// Wrapper class for library handle

class CLibrary
{
public :
	CLibrary(LPCTSTR pszLibrary = NULL) : m_hLibrary(NULL)
	{
		Load(pszLibrary);
	}
	~CLibrary()
	{
		Load(NULL);
	}

	// If Load(NULL) : only free previous library 
	HRESULT Load(LPCTSTR pszLibrary, DWORD dwFlags = 0)
	{
		HRESULT hr = S_OK;
		__try
		{
			if(m_hLibrary != NULL)
			{
				::FreeLibrary(m_hLibrary);
				m_hLibrary = NULL;
			}
			if(pszLibrary != NULL)
			{
				SetLastError(0);
				m_hLibrary = ::LoadLibraryEx(pszLibrary, NULL, dwFlags);
				if(m_hLibrary == NULL)
					hr = ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
			}
		}
		__except( EXCEPTION_EXECUTE_HANDLER )
		{
			m_hLibrary = NULL;
			hr = E_FAIL;
		}
		return hr;
	}

	operator HINSTANCE() const { return m_hLibrary; }

	bool operator!() const { return m_hLibrary == NULL; }

private:
	HINSTANCE m_hLibrary;
};

/////////////////////////////////////////////////////////////////////////////
// Classes for synchronize read and write operations
//	Several threads can read simultaneously, but only one thread can write
//	(and read disabled in write time)
//
// Usage:
//
//	CRWSynchro	m_rwSynch;
//
//	void ReadFun()
//	{
//		CAutoRW lock(m_rwSynch, false);
//		....
//	}
//
//	void WriteFun()
//	{
//		CAutoRW lock(m_rwSynch, true);
//		....
//	}
//
// Recursive calls from the same thread allowed for the same method:
// LockRead() call not blocked by previous call of LockRead() for the same
// thread (LockWrite() has the same behavior).
//
// ATTENTION.
// If LockRead() (LockWrite()) recursive called from the same thread which
// already call opposite method LockWrite() (LockRead()): DEAD LOCK will be reached.
//
//---------------------------------
// TODO. Next extention possible:
//
// LockWrite() -> LockRead(): possible for all cases: if LockWrite() called
// then LockRead() calls from different threads will be blocked.
//
// LockRead() -> LockWrite(): possible if LockRead() called from one thread only
// If LockRead() called from several threads then LockWrite() should be blocked
// while all another threads not call UnlockRead(). After call LockWrite()
// all LockRead() calls from different threads will be blocked.
//
// ATTENTION.
//	If LockWrite() called after LockRead() from 2 or more threads: DEAD LOCK will be reached.
//
//---------------------------------

class CRWSynchro
{
	CComAutoCriticalSection	m_csRW;
	CComAutoCriticalSection	m_csWrite;

	HANDLE	m_hWriteEnabled;
	DWORD	m_cRead;
	HANDLE	m_hReadEnabled;
	DWORD	m_cWrite;

#ifdef _DEBUG
	CSimpleArray2<DWORD>	m_arrReadThreads;
	CSimpleArray2<DWORD>	m_arrWriteThreads;
	DWORD					m_dwCurrentWriteThread;
#endif

	//---------------------------------
public:
	CRWSynchro()
	{
		m_hWriteEnabled = ::CreateEvent(NULL, TRUE, TRUE, NULL);
		ATLASSERT(m_hWriteEnabled != NULL);
		m_hReadEnabled = ::CreateEvent(NULL, TRUE, TRUE, NULL);
		ATLASSERT(m_hReadEnabled != NULL);
		m_cRead = 0;
		m_cWrite = 0;

#ifdef _DEBUG
		m_dwCurrentWriteThread = 0;
#endif
	}

	~CRWSynchro()
	{
		if(m_hWriteEnabled != NULL)
			::CloseHandle(m_hWriteEnabled);
		if(m_hReadEnabled != NULL)
			::CloseHandle(m_hReadEnabled);
	}

	void LockRead()
	{
#ifdef _DEBUG
		{
			CAutoCS lock(m_csRW);
			DWORD dwThreadID = GetCurrentThreadId();
			m_arrReadThreads.Add(dwThreadID);
		}
#endif
		//---------------------------------
		while(1)
		{
			::WaitForSingleObject(m_hReadEnabled, INFINITE);

			//---------------------------------
			CAutoCS lock(m_csRW);
			if(m_cWrite == 0)
			{
				++m_cRead;
				::ResetEvent(m_hWriteEnabled);
				break;
			}
		}
	}

	void UnlockRead()
	{
		CAutoCS lock(m_csRW);

		ATLASSERT(m_cRead > 0);
		if(--m_cRead == 0)
			::SetEvent(m_hWriteEnabled);

		//---------------------------------
#ifdef _DEBUG
		DWORD dwThreadID = GetCurrentThreadId();
		int idx = m_arrReadThreads.Find(dwThreadID);
		ATLASSERT(idx != -1);
		m_arrReadThreads.RemoveAt(idx);
#endif
	}

	void LockWrite()
	{
#ifdef _DEBUG
		{
			CAutoCS lock(m_csRW);
			DWORD dwThreadID = GetCurrentThreadId();
			m_arrWriteThreads.Add(dwThreadID);
		}
#endif
		//---------------------------------
		while(1)
		{
			::WaitForSingleObject(m_hWriteEnabled, INFINITE);

			//---------------------------------
			CAutoCS lock(m_csRW);
			if(m_cRead == 0)
			{
				++m_cWrite;
				::ResetEvent(m_hReadEnabled);
				break;
			}
		}

		//---------------------------------
		// For lock write (only one thread can enter into)
		m_csWrite.Lock();

#ifdef _DEBUG
		ATLTRACE2( atlTraceGeneral, 2, _T("LockWrite(%08X)\n"), this );
		ATLASSERT(m_dwCurrentWriteThread == 0);
		m_dwCurrentWriteThread = GetCurrentThreadId();
#endif
	}

// 	void LockWrite()
// 	{
// #ifdef _DEBUG
// 		DWORD dwThreadID = GetCurrentThreadId();
// #endif
// 
// 		{
// 			CAutoCS lock(m_csRW);
// 			++m_cWrite;
// 			::ResetEvent(m_hReadEnabled);
// 
// #ifdef _DEBUG
// 			m_arrWriteThreads.Add(dwThreadID);
// #endif
// 		}
// 
// 		::WaitForSingleObject(m_hWriteEnabled, INFINITE);
// 
// 		//---------------------------------
// 		// For lock write (only one thread can enter into)
// 		m_csWrite.Lock();
// 
// #ifdef _DEBUG
// 		ATLASSERT(m_dwCurrentWriteThread == 0);
// 		m_dwCurrentWriteThread = dwThreadID;
// #endif
//	}

	void UnlockWrite()
	{
#ifdef _DEBUG
		DWORD dwThreadID = GetCurrentThreadId();
		ATLASSERT(m_dwCurrentWriteThread == dwThreadID);
		m_dwCurrentWriteThread = 0;
#endif

		// For unlock write
		m_csWrite.Unlock();

		//---------------------------------
		CAutoCS lock(m_csRW);

		ATLASSERT(m_cWrite > 0);
		if(--m_cWrite == 0)
			::SetEvent(m_hReadEnabled);

#ifdef _DEBUG
		int idx = m_arrWriteThreads.Find(dwThreadID);
		ATLASSERT(idx != -1);
		m_arrWriteThreads.RemoveAt(idx);

		ATLTRACE2( atlTraceGeneral, 2, _T("UnlockWrite(%08X)\n"), this );
#endif
	}
};

//---------------------------------
class CAutoRW
{
	CAutoRW(const CAutoRW& autoSR);
	CAutoRW& operator=(const CAutoRW& autoSR);

public:
	CAutoRW(CRWSynchro& rwSynchro, bool bWrite) : m_rwSynchro(rwSynchro), m_bWrite(bWrite)
	{
		if(m_bWrite)
			m_rwSynchro.LockWrite();
		else
			m_rwSynchro.LockRead();
	}
	~CAutoRW()
	{
		if(m_bWrite)
			m_rwSynchro.UnlockWrite();
		else
			m_rwSynchro.UnlockRead();
	}

private:
	CRWSynchro& m_rwSynchro;
	const bool m_bWrite;
};


//////////////////////////////////////////////////////////////////////////
// VVB: RW Synchro Lite -> less resources, fast enter
#define TRACELEVEL_RWSync		(10) 

#define RWLITE_CS_SPIN			(1024)
#define RWLITE_ZEROSLEEP_SPIN	(128)
#ifndef _DEBUG
#undef DEBUG_RWL
#endif

class CRWSynchroLiteBase
{
	friend class CAutoRWL;
public:

	CRWSynchroLiteBase()
	{
	}

	virtual ~CRWSynchroLiteBase()
	{
	}

	virtual bool LockRead( bool _bWait = true, LPCWSTR* _ppszWrite = NULL ) = 0;
	
	virtual void UnlockRead() = 0;
	
	// WARNING !!! Lock Read -> Lock Write NOT ALLOWED -> Dead lock
	virtual bool LockWrite( bool _bWait = true, LPCWSTR _pszWrite = NULL, LPCWSTR* _ppszWrite = NULL ) = 0;
	
	virtual void UnlockWrite() = 0;

	virtual DWORD WriteTID() = 0;
};




//////////////////////////////////////////////////////////////////////////
// Based on SRWLOCK: https://msdn.microsoft.com/en-us/library/windows/desktop/ms683483(v=vs.85).aspx
// NO Recursion !!!
class CRWSynchroLiteSRW
	: public CRWSynchroLiteBase
{
	friend class CAutoRWL;
	SRWLOCK m_srwLock;
public:

	CRWSynchroLiteSRW()
	{
	}

	virtual ~CRWSynchroLiteSRW()
	{
	}

	virtual bool LockRead( bool _bWait = true, LPCWSTR* _ppszWrite = NULL )
	{
		if( !_bWait )
			return ::TryAcquireSRWLockShared( &m_srwLock ) != 0;

		::AcquireSRWLockShared( &m_srwLock );
		return true;
	}

	virtual void UnlockRead()
	{
		::ReleaseSRWLockShared( &m_srwLock );
	}

	// WARNING !!! Lock Read -> Lock Write NOT ALLOWED -> Dead lock
	virtual bool LockWrite( bool _bWait = true, LPCWSTR _pszWrite = NULL, LPCWSTR* _ppszWrite = NULL )
	{
		if( !_bWait )
			return ::TryAcquireSRWLockExclusive( &m_srwLock ) != 0;

		::AcquireSRWLockExclusive( &m_srwLock );
		return true;
	}

	virtual void UnlockWrite()
	{
		::ReleaseSRWLockExclusive( &m_srwLock );
	}

	virtual DWORD WriteTID() { return (DWORD)-1; }
};


//////////////////////////////////////////////////////////////////////////
// Based on SRWLOCK: https://msdn.microsoft.com/en-us/library/windows/desktop/ms683483(v=vs.85).aspx
// NO Recursion !!!
class CRWSynchroSRW
{
	SRWLOCK m_srwLock;
public:

	CRWSynchroSRW()
	{
		::InitializeSRWLock( &m_srwLock );
	}

	bool LockRead( bool _bWait = true, LPCWSTR* _ppszWrite = NULL )
	{
		if( !_bWait )
			return ::TryAcquireSRWLockShared( &m_srwLock ) != 0;

		::AcquireSRWLockShared( &m_srwLock );
		return true;
	}

	void UnlockRead()
	{
		::ReleaseSRWLockShared( &m_srwLock );
	}

	bool LockWrite( bool _bWait = true, LPCWSTR _pszWrite = NULL, LPCWSTR* _ppszWrite = NULL )
	{
		if( !_bWait )
			return ::TryAcquireSRWLockExclusive( &m_srwLock ) != 0;

		::AcquireSRWLockExclusive( &m_srwLock );
		return true;
	}

	void UnlockWrite()
	{
		::ReleaseSRWLockExclusive( &m_srwLock );
	}
};
 
// Lock Read -> Lock Read -> Ok
// Lock Write -> Lock Read -> Lock Write ... Ok
// Lock Write -> Lock Write -> Lock Read -> Ok
// Lock Read -> Lock Write -> DEADLOCK
class CRWSynchroLiteRW
	: public CRWSynchroLiteBase
{
	friend class CAutoRWL;

	// TLS storage
	DWORD				m_dwTLSStorage;

#ifdef DEBUG_RWL
	CComAutoCriticalSection			m_csMap;
	typedef std::map<DWORD,int>		TMapReaders;
	TMapReaders						m_mapReaders; // For easy debugging
#endif

	//---------------------------------
	

public:
	CRWSynchroLiteRW( DWORD _dwSpinCount = RWLITE_CS_SPIN, bool _bUseTLS = false )
		: m_lReaders( 0 ), 
		m_lWriters( 0 ),
		m_pszWrite( NULL ),
		m_pszDebugInOut( NULL )
	{
		::InitializeCriticalSectionAndSpinCount( &m_csWrite, _dwSpinCount );
		
		if( false && _bUseTLS ) // Too many problem with TLS
		{
			m_dwTLSStorage = TlsAlloc();
			ATLASSERT( m_dwTLSStorage != TLS_OUT_OF_INDEXES );
		}
		else
		{
			m_dwTLSStorage = TLS_OUT_OF_INDEXES;
		}
	}

	~CRWSynchroLiteRW()
	{
		if( m_dwTLSStorage != TLS_OUT_OF_INDEXES )
			TlsFree(m_dwTLSStorage);

		m_dwTLSStorage = TLS_OUT_OF_INDEXES;

		::DeleteCriticalSection( &m_csWrite );
	}

	inline bool LockRead( bool _bWait = true, LPCWSTR* _ppszWrite = NULL )
	{
		if( m_dwTLSStorage != TLS_OUT_OF_INDEXES )
		{
			//int nReadersTLS = (int)TlsGetValue(m_dwTLSStorage);
			int nReadersTLS = *static_cast<const int*>(TlsGetValue(m_dwTLSStorage));
			TlsSetValue(m_dwTLSStorage, (LPVOID)(nReadersTLS + 1) );
			if( nReadersTLS > 0 )
				return true; // Read already locked -> do nothing
		}

		if( _ppszWrite )
			*_ppszWrite = m_pszWrite;

#ifdef DEBUG_RWL
		// Note: !!! use checking m_lWriters < 0 for not allow new reader when writer wait till readers complete
		//           BUT in this case Thread1 Read->..->Read Thread2: Write -> CAUSE THE FREEZE !!!
		if( m_lWriters <= 0 )
		{
			// If m_lReaders < 0 -> current thread already write, do nothing
			// Interlocked as reader may change count outside CS scope
			long lReaders = InterlockedIncrementGreaterEqualThan( &m_lReaders, 0 );

			CAutoCS crs(m_csMap);
			if( lReaders >= 0 )
			{
				TMapReaders::iterator Iter = m_mapReaders.find( GetCurrentThreadId() );
				if( Iter == m_mapReaders.end() )
					m_mapReaders.insert( make_pair( GetCurrentThreadId(), 1 ) );
				else
					Iter->second++;
				
				return true;
			}
			else
			{
				// Writer -> no readers (except current thread) allowed
				ATLASSERT( m_mapReaders.size() == 0 );
			}
		}
#else
		// Check for fast enter (w/o CS)
		// If no writers now -> add new reader 
		// If writers or writer appear right after check m_lWriters -> change reader counter (writer will wait) or goto CS (this reader wait)
		// Note: !!! use checking m_lWriters <= 0 for not allow new reader if writer wait till readers complete
		//           BUT  Thread1 Read->.(*).->Read Thread2: Write (*)-> CAUSE THE FREEZE !!! -> USED TLS
		if( (m_lWriters <= 0 || m_dwTLSStorage == TLS_OUT_OF_INDEXES) && InterlockedIncrementGreaterEqualThan( &m_lReaders, 0 ) >= 0 )
		{
			return true;
		}
#endif

		if( !_bWait )
		{
			if( !::TryEnterCriticalSection( &m_csWrite ) )
			{
				// If we enter to reader lock -> zero recuorsive readers  
				TlsSetValue(m_dwTLSStorage, (LPVOID)0 );

				return false;
			}
		}
		else
		{
			::EnterCriticalSection( &m_csWrite ); // <- Wait here till write complete (if not same thread as writer)
		}

#ifdef DEBUG_RWL
		{
			// If m_lReaders < 0 -> current thread already write, do nothing
			// Interlocked as reader may change count outside CS scope
			long lReaders = InterlockedIncrementGreaterEqualThan( &m_lReaders, 0 );

			CAutoCS crs(m_csMap);
			if( lReaders >= 0 )
			{
				TMapReaders::iterator Iter = m_mapReaders.find( GetCurrentThreadId() );
				if( Iter == m_mapReaders.end() )
					m_mapReaders.insert( make_pair( GetCurrentThreadId(), 1 ) );
				else
					Iter->second++;
			}
			else
			{
				// Writer -> no readers (except current thread) allowed
				ATLASSERT( m_mapReaders.size() == 0 );
			}
		}
#else
		// If m_lReaders < 0 -> current thread already write, do nothing
		// Interlocked as reader may change count outside CS scope
		InterlockedIncrementGreaterEqualThan( &m_lReaders, 0 );
#endif

		// Not lock critical section 
		::LeaveCriticalSection( &m_csWrite );
		return true;
	}

	inline void UnlockRead()
	{
		if( m_dwTLSStorage != TLS_OUT_OF_INDEXES )
		{
			//int nReadersTLS = (int)TlsGetValue(m_dwTLSStorage);
			int nReadersTLS = *static_cast<const int*>(TlsGetValue(m_dwTLSStorage));
			ATLASSERT( nReadersTLS > 0 );
			TlsSetValue(m_dwTLSStorage, (LPVOID)(nReadersTLS - 1) );
			if( nReadersTLS > 1 )
				return; // This is recursive call -> do nothing
		}

#ifdef DEBUG_RWL
		{
			CAutoCS crs(m_csMap);

			TMapReaders::iterator Iter = m_mapReaders.find( GetCurrentThreadId() );
			if( Iter == m_mapReaders.end() )
			{
				// Not found -> begin read after write
				ATLASSERT( m_lWriters > 0 );
				ATLASSERT( m_mapReaders.size() == 0 );
				ATLASSERT( m_csWrite.OwningThread == (HANDLE)GetCurrentThreadId() );

				ATLASSERT( m_lReaders < 0 );
			}
			else
			{
				if( Iter->second == 1 )
				{
					// Remove reader
					m_mapReaders.erase(Iter);
				}
				else
				{
					ATLASSERT( Iter->second > 1 );
					Iter->second--;
				}

				ATLASSERT( m_lReaders > 0 );
			}
		}

		// If m_lReaders < 0 -> current thread already write, do nothing
		// If m_lReaders == 0 -> error
		// If m_lReaders > 0 -> decrease reader count
		long lRes = InterlockedDecrementGreaterThan( &m_lReaders, 0 );
		ATLASSERT( lRes != 0 );

#else
		// If m_lReaders < 0 -> current thread already write, do nothing
		// If m_lReaders == 0 -> NOT EXPECTED -> error
		// If m_lReaders > 0 -> decrease reader count
		long lRes = InterlockedDecrementGreaterThan( &m_lReaders, 0 );
		ATLASSERT( lRes != 0 );
#endif
	}

	// WARNING !!! Lock Read -> Lock Write NOT ALLOWED -> Dead lock
	inline bool LockWrite( bool _bWait = true, LPCWSTR _pszWrite = NULL, LPCWSTR* _ppszWrite = NULL )
	{
		if( _ppszWrite )
			*_ppszWrite = m_pszWrite;

#ifdef _DEBUG
		if( m_dwTLSStorage != TLS_OUT_OF_INDEXES )
		{
			//int nReadersTLS = (int)TlsGetValue(m_dwTLSStorage); 
			int nReadersTLS = *static_cast<const int*>(TlsGetValue(m_dwTLSStorage));
			ATLASSERT( (nReadersTLS == 0 || (ULONGLONG)m_csWrite.OwningThread == (ULONGLONG)GetCurrentThreadId()) && "LockRead->LockWrite");
			nReadersTLS++;
		}
#endif

		if( !_bWait )
		{
			if( !::TryEnterCriticalSection( &m_csWrite ) )
				return false;

			if( InterlockedDecrementLessEqualThan( &m_lReaders, 0 ) <= 0 )
			{
				::InterlockedIncrement( &m_lWriters ); // Increase writer counters -> new readers will wait the CS
				return true;
			}
			else
			{
				::LeaveCriticalSection( &m_csWrite );
				return false;
			}
		}
		else
		{
			::EnterCriticalSection( &m_csWrite ); // <- Wait here till write complete (if not same thread as writer)
		}

#ifdef DEBUG_RWL
		{
			CAutoCS crs(m_csMap);

			TMapReaders::iterator Iter = m_mapReaders.find( GetCurrentThreadId() );
			if( Iter != m_mapReaders.end() )
			{
				ATLASSERT( !"CRWSynchroLite::LockWrite() - Call after LockRead() in same thread" );
			}
		}
#endif

		// At this point NO new reader allowed, wait till all other readers finish read.
		// If enter in same thread -> increase writer counter
		// 
		::InterlockedIncrement( &m_lWriters ); // Increase writer counters -> new readers will wait the CS

		int nCount = 0;
		while( InterlockedDecrementLessEqualThan( &m_lReaders, 0 ) > 0 )
			Sleep(++nCount % RWLITE_ZEROSLEEP_SPIN ? 0 : 1);

		m_pszWrite = _pszWrite;

		// Now all reader finish read operation, and new readers wait in CS
#ifdef DEBUG_RWL
		ATLASSERT( m_mapReaders.size() == 0 );
#endif

		return true;
	}
	inline void UnlockWrite()
	{
		// The readers not allowed at this point
#ifdef DEBUG_RWL
		ATLASSERT( m_lReaders < 0 );
		ATLASSERT( m_mapReaders.size() == 0 );
		ATLASSERT( m_csWrite.OwningThread == (HANDLE)GetCurrentThreadId() );
#endif

		// Increase readers count 
		InterlockedIncrementLessThan( &m_lReaders, 0 ); 
		
		::InterlockedDecrement( &m_lWriters ); // Decrease writers counter -> now fast eneter (w/o CS) allowed 
		// (the interlocked is not required: writer > 0, other changes blocked by CS)

		::LeaveCriticalSection( &m_csWrite ); // Leave CS -> now waiting CS readers may process
	}

	//virtual DWORD WriteTID() { return (DWORD)m_csWrite.OwningThread; }
	virtual DWORD WriteTID() { return HandleToUlong(m_csWrite.OwningThread); }

public:
	// Add value ONLY if greater or equal specified value 
	inline static long InterlockedAddGreaterEqualThan( long volatile *Destination, long Greater, long Add )
	{
		
		// lValue == Greater -> Add value -> Return 
		// lValue <  Greater -> Can't decrease -> Return 
		// lValue >  Greater -> Remember current value, add in next call 
		long lCurrent = ::InterlockedCompareExchange( Destination, Greater + Add, Greater );
		if( lCurrent == Greater )
			return lCurrent;

		while( lCurrent >= Greater ) 
		{
			// If value change from last call, try next cycle
			long lChanged = ::InterlockedCompareExchange( Destination, lCurrent + Add, lCurrent );
			if( lChanged == lCurrent ) 
				break;

			lCurrent = lChanged;
		}

		return lCurrent;
	}

	inline static long InterlockedAddLessEqualThan( long volatile *Destination, long Less, long Add )
	{
		// lValue == Less -> increase from Less to 1 -> Return 
		// lValue >  Less -> Can't decrease -> Return 
		// lValue <  Less -> Remember current value, decrease in next call 
		long lCurrent = ::InterlockedCompareExchange( Destination, Less + Add, Less );
		if( lCurrent == Less )
			return lCurrent;

		while( lCurrent <= Less ) 
		{
			// If value change from last call, try next cycle
			long lChanged = ::InterlockedCompareExchange( Destination, lCurrent + Add, lCurrent );
			if( lChanged == lCurrent ) 
				break;

			lCurrent = lChanged;
		}

		return lCurrent;
	}


	inline static long InterlockedIncrementGreaterEqualThan( long volatile *Destination, long Greater )
	{
		return InterlockedAddGreaterEqualThan( Destination, Greater, 1 );
	}

	inline static long InterlockedDecrementGreaterThan( long volatile *Destination, long Greater )
	{
		return InterlockedAddGreaterEqualThan( Destination, Greater + 1, -1 );
	}

	inline static long InterlockedDecrementLessEqualThan( long volatile *Destination, long Less )
	{
		return InterlockedAddLessEqualThan( Destination, Less, -1 );
	}

	inline static long InterlockedIncrementLessThan( long volatile *Destination, long Less )
	{
		return InterlockedAddLessEqualThan( Destination, Less - 1, 1 );
	}
protected:
	// Writer critical section
	CRITICAL_SECTION		m_csWrite;

	// Readers/Writer counter
	// Greater zero -> readers
	// Less zero -> writers
	volatile		long		m_lReaders;

	// Writers counter (used for fast-enter reader)
	volatile		long		m_lWriters;

	LPCWSTR				m_pszWrite;

public:
	LPCWSTR				m_pszDebugInOut;  // For debug enter/leave, work with CAutoRWL ONLY
};

class CRWSynchroLiteCS
	: public CRWSynchroLiteBase
{
	friend class CAutoRWL;

public:
	CRWSynchroLiteCS( DWORD _dwSpinCount = RWLITE_CS_SPIN, bool _bUseTLS = false )
		: m_lReaders( 0 ), 
		m_lWriters( 0 ),
		m_pszWrite( NULL ),
		m_pszDebugInOut( NULL )
	{
		::InitializeCriticalSectionAndSpinCount( &m_csWrite, _dwSpinCount );
	}

	~CRWSynchroLiteCS()
	{
		::DeleteCriticalSection( &m_csWrite );
	}

	inline bool LockRead( bool _bWait = true, LPCWSTR* _ppszWrite = NULL )
	{
		if( !_bWait )
		{
			if( !::TryEnterCriticalSection( &m_csWrite ) )
			{
				return false;
			}
		}
		else
		{
			::EnterCriticalSection( &m_csWrite ); // <- Wait here till write complete (if not same thread as writer)
		}

		return true;
	}

	inline void UnlockRead()
	{
		::LeaveCriticalSection( &m_csWrite ); // Leave CS -> now waiting CS readers may process
	}

	inline bool LockWrite( bool _bWait = true, LPCWSTR _pszWrite = NULL, LPCWSTR* _ppszWrite = NULL )
	{
		if( !_bWait )
		{
			if( !::TryEnterCriticalSection( &m_csWrite ) )
				return false;
		}
		else
		{
			::EnterCriticalSection( &m_csWrite ); // <- Wait here till write complete (if not same thread as writer)
		}


		return true;
	}
	inline void UnlockWrite()
	{
		::LeaveCriticalSection( &m_csWrite ); // Leave CS -> now waiting CS readers may process
	}
	//virtual DWORD WriteTID() { return (DWORD)m_csWrite.OwningThread; }
	virtual DWORD WriteTID() { return HandleToUlong(m_csWrite.OwningThread); }
protected:
	// Writer critical section
	CRITICAL_SECTION		m_csWrite;

	// Readers/Writer counter
	// Greater zero -> readers
	// Less zero -> writers
	volatile		long		m_lReaders;

	// Writers counter (used for fast-enter reader)
	volatile		long		m_lWriters;

	LPCWSTR				m_pszWrite;

public:
	LPCWSTR				m_pszDebugInOut;  // For debug enter/leave, work with CAutoRWL ONLY
};

// https://www.codeproject.com/Articles/3023/A-solution-to-the-Readers-Writers-Problem-using-se

class CRWSyncroLiteIPC: 
	public AutoObjBase<CRWSyncroLiteIPC>,
	public CRWSynchroLiteBase
{
public:
	static HRESULT CreateInstance( CRWSyncroLiteIPC** _ppObject, volatile long* _plReaderCountSharedMem, LONGLONG _llSharedID, LPCWSTR _pszPrefix = L"__mfbuffer_rwsync" )
	{
		ATLASSERT( _ppObject );
		if( !_ppObject ) return E_POINTER;

		CRWSyncroLiteIPC* pT = new CRWSyncroLiteIPC( _plReaderCountSharedMem, _llSharedID, _pszPrefix );
		pT->AddRef();
		*_ppObject = pT;
		return S_OK;
	}

protected:
	HANDLE					m_semReaders;
	HANDLE					m_semWriters;
	volatile long&			m_lReaders;

	STDMETHOD(QueryInterface)(_In_ REFIID iid, _Deref_out_ void** ppvObject) 
	{
		return E_NOINTERFACE;
	}
public:
	CRWSyncroLiteIPC( volatile long* _plReaderCountSharedMem, LONGLONG _llSharedID, LPCWSTR _pszPrefix = L"__mfbuffer_rwsync" ) :
			m_semReaders(NULL),
			m_semWriters(NULL),
			m_lReaders(*_plReaderCountSharedMem)
	  {
		  // initialize the Readers & Writers variables

		  WCHAR szObjectName[128] = {};
		  wsprintfW( szObjectName, L"_sr_%ws%I64X", _pszPrefix, _llSharedID );
		  m_semReaders    = ::CreateSemaphoreW(NULL, 1, 1, szObjectName );
		  wsprintfW( szObjectName, L"_sw_%ws%I64X", _pszPrefix, _llSharedID );
		  m_semWriters    = ::CreateSemaphoreW(NULL, 1, 1, szObjectName );

		  ATLASSERT( m_semReaders && m_semWriters );
	  };

	  virtual ~CRWSyncroLiteIPC()
	  {
		  if (m_semWriters)
			  ::CloseHandle(m_semWriters);    
		  m_semWriters = NULL;

		  if (m_semReaders)
			  ::CloseHandle(m_semReaders);    
		  m_semReaders = NULL;
	  }


	  virtual bool LockRead( bool _bWait = true, LPCWSTR* _ppszWrite = NULL )
	  {
		  // P( semReaders )
		  DWORD dwEvent = ::WaitForSingleObject( m_semReaders, _bWait ? INFINITE : 0 );
		  ATLASSERT(dwEvent == WAIT_OBJECT_0 || dwEvent == WAIT_TIMEOUT);
		  if(dwEvent != WAIT_OBJECT_0)
			  return false;

		  // Note: Need atomic increment and wait !!! - thus use m_semReaders, interlocked is not required
		  if( ::InterlockedIncrement( &m_lReaders ) == 0 )
		  {
			  // P( semWriters )
			  dwEvent = ::WaitForSingleObject( m_semWriters, INFINITE );
			  ATLASSERT(dwEvent == WAIT_OBJECT_0);
		  }

		  // V( semReaders )
		  ::ReleaseSemaphore( m_semReaders, 1, NULL );
		  return true;
	  };

	  virtual void UnlockRead()
	  {
		  // P( semReaders )
		  DWORD dwEvent = ::WaitForSingleObject( m_semReaders, INFINITE );
		  ATLASSERT(dwEvent == WAIT_OBJECT_0);

		  // Note: Does need atomic decrement and wait ?
		  if( ::InterlockedDecrement( &m_lReaders ) == 1 )
		  {
			  // V( semWriters )
			  ::ReleaseSemaphore(m_semWriters, 1, NULL );
		  }
		  // V( semReaders )
		  ::ReleaseSemaphore( m_semReaders, 1, NULL );
	  };

	  virtual bool LockWrite( bool _bWait = true, LPCWSTR _pszWrite = NULL, LPCWSTR* _ppszWrite = NULL )
	  {
		  // P( semWriters )
		  DWORD dwEvent = ::WaitForSingleObject( m_semWriters, _bWait ? INFINITE : 0 );
		  ATLASSERT(dwEvent == WAIT_OBJECT_0 || dwEvent == WAIT_TIMEOUT);
		  if(dwEvent != WAIT_OBJECT_0)
			  return false;

		  return true;
	  }

	  virtual void UnlockWrite()
	  {
		  // V( semWriters )
		  ::ReleaseSemaphore(m_semWriters, 1, NULL);
	  };

	  virtual DWORD WriteTID() { return (DWORD)-1; }
};

#ifndef RWLITE_CS_RW
#define CRWSynchroLite CRWSynchroLiteRW
#else
#define CRWSynchroLite CRWSynchroLiteCS
#endif

//---------------------------------
class CAutoRWL
{
	CAutoRWL(const CAutoRWL& autoSR);
	CAutoRWL& operator=(const CAutoRWL& autoSR);

public:
	inline CAutoRWL(CRWSynchroLiteCS& rwSynchro, bool bWrite, LPCWSTR _pszWrite = NULL) : m_pRWSynchro(&rwSynchro), m_bWrite(bWrite), m_pszDebug( _pszWrite )
	{
		if(m_bWrite)
		{
// 			if( m_pRWSynchro->m_pszDebugInOut ) 
// 				ATLTRACE2( atlTraceGeneral, TRACELEVEL_RWSync, _T("CAutoRWL(TID:%04X,%ws:%08X) -> LOCK WRITE R:%d W:%d(%04X) %ws\n"), GetCurrentThreadId(), m_pRWSynchro->m_pszDebugInOut, (LONG)m_pRWSynchro, m_pRWSynchro->m_lReaders, m_pRWSynchro->m_lWriters, m_pRWSynchro->m_csWrite.OwningThread, m_pszDebug );

			m_pRWSynchro->LockWrite(true, _pszWrite);
		}
		else
		{
// 			if( m_pRWSynchro->m_pszDebugInOut ) 
// 				ATLTRACE2( atlTraceGeneral, TRACELEVEL_RWSync, _T("CAutoRWL(TID:%04X,%ws:%08X) -> LOCK READ R:%d W:%d(%04X) %ws\n"), GetCurrentThreadId(), m_pRWSynchro->m_pszDebugInOut, (LONG)m_pRWSynchro, m_pRWSynchro->m_lReaders, m_pRWSynchro->m_lWriters, m_pRWSynchro->m_csWrite.OwningThread, m_pszDebug );

			m_pRWSynchro->LockRead();
		}
	}
	inline CAutoRWL(CRWSynchroLiteRW& rwSynchro, bool bWrite, LPCWSTR _pszWrite = NULL) : m_pRWSynchro(&rwSynchro), m_bWrite(bWrite), m_pszDebug( _pszWrite )
	{
		if(m_bWrite)
		{
// 			if( m_pRWSynchro->m_pszDebugInOut ) 
// 				ATLTRACE2( atlTraceGeneral, TRACELEVEL_RWSync, _T("CAutoRWL(TID:%04X,%ws:%08X) -> LOCK WRITE R:%d W:%d(%04X) %ws\n"), GetCurrentThreadId(), m_pRWSynchro->m_pszDebugInOut, (LONG)m_pRWSynchro, m_pRWSynchro->m_lReaders, m_pRWSynchro->m_lWriters, m_pRWSynchro->m_csWrite.OwningThread, m_pszDebug );

			m_pRWSynchro->LockWrite(true, _pszWrite);
		}
		else
		{
// 			if( m_pRWSynchro->m_pszDebugInOut ) 
// 				ATLTRACE2( atlTraceGeneral, TRACELEVEL_RWSync, _T("CAutoRWL(TID:%04X,%ws:%08X) -> LOCK READ R:%d W:%d(%04X) %ws\n"), GetCurrentThreadId(), m_pRWSynchro->m_pszDebugInOut, (LONG)m_pRWSynchro, m_pRWSynchro->m_lReaders, m_pRWSynchro->m_lWriters, m_pRWSynchro->m_csWrite.OwningThread, m_pszDebug );

			m_pRWSynchro->LockRead();
		}
	}
	inline CAutoRWL(CRWSynchroLiteBase* prwSynchro, bool bWrite, LPCWSTR _pszWrite = NULL) : m_pRWSynchro(prwSynchro), m_bWrite(bWrite), m_pszDebug( _pszWrite )
	{
		if(m_bWrite && m_pRWSynchro )
		{
// 			if( m_pRWSynchro->m_pszDebugInOut ) 
// 				ATLTRACE2( atlTraceGeneral, TRACELEVEL_RWSync, _T("CAutoRWL(TID:%04X,%ws:%08X) -> LOCK WRITE R:%d W:%d(%04X) %ws\n"), GetCurrentThreadId(), m_pRWSynchro->m_pszDebugInOut, (LONG)m_pRWSynchro, m_pRWSynchro->m_lReaders, m_pRWSynchro->m_lWriters, m_pRWSynchro->m_csWrite.OwningThread, m_pszDebug );

			m_pRWSynchro->LockWrite(true, _pszWrite);
		}
		else if( m_pRWSynchro )
		{
// 			if( m_pRWSynchro->m_pszDebugInOut ) 
// 				ATLTRACE2( atlTraceGeneral, TRACELEVEL_RWSync, _T("CAutoRWL(TID:%04X,%ws:%08X) -> LOCK READ R:%d W:%d(%04X) %ws\n"), GetCurrentThreadId(), m_pRWSynchro->m_pszDebugInOut, (LONG)m_pRWSynchro, m_pRWSynchro->m_lReaders, m_pRWSynchro->m_lWriters, m_pRWSynchro->m_csWrite.OwningThread, m_pszDebug );

			m_pRWSynchro->LockRead();
		}
	}
	inline ~CAutoRWL()
	{
		if(m_bWrite && m_pRWSynchro)
		{
			m_pRWSynchro->UnlockWrite();

// 			if( m_pRWSynchro->m_pszDebugInOut ) 
// 				ATLTRACE2( atlTraceGeneral, TRACELEVEL_RWSync, _T("CAutoRWL(TID:%04X,%ws:%08X) <- UNLOCK WRITE R:%d W:%d(%04X) %ws\n"), GetCurrentThreadId(), m_pRWSynchro->m_pszDebugInOut, (LONG)m_pRWSynchro, m_pRWSynchro->m_lReaders, m_pRWSynchro->m_lWriters, m_pRWSynchro->m_csWrite.OwningThread, m_pszDebug );
		}
		else if( m_pRWSynchro )
		{
			m_pRWSynchro->UnlockRead();

// 			if( m_pRWSynchro->m_pszDebugInOut ) 
// 				ATLTRACE2( atlTraceGeneral, TRACELEVEL_RWSync, _T("CAutoRWL(TID:%04X,%ws:%08X) <- UNLOCK READ R:%d W:%d(%04X) %ws\n"), GetCurrentThreadId(), m_pRWSynchro->m_pszDebugInOut, (LONG)m_pRWSynchro, m_pRWSynchro->m_lReaders, m_pRWSynchro->m_lWriters, m_pRWSynchro->m_csWrite.OwningThread, m_pszDebug );
		}
	}

private:
	CRWSynchroLiteBase* m_pRWSynchro;
	const bool	m_bWrite;
	LPCWSTR		m_pszDebug;
};


class CAutoRWS
{
	bool	m_bWrite;
	CRWSynchroSRW* m_pRWSynchro;

private:
	CAutoRWS(const CAutoRWS& autoSR);
	CAutoRWS& operator=(const CAutoRWS& autoSR);

public:
	
	inline CAutoRWS(CRWSynchroSRW& rwSynchro, bool bWrite) : m_pRWSynchro(&rwSynchro), m_bWrite(bWrite)
	{
		if(m_bWrite)
		{
			m_pRWSynchro->LockWrite(true);
		}
		else
		{
			m_pRWSynchro->LockRead();
		}
	}
	inline CAutoRWS(CRWSynchroSRW* prwSynchro, bool bWrite) : m_pRWSynchro(prwSynchro), m_bWrite(bWrite)
	{
		if(m_bWrite && m_pRWSynchro )
		{
			m_pRWSynchro->LockWrite(true);
		}
		else if( m_pRWSynchro )
		{
			m_pRWSynchro->LockRead();
		}
	}
	inline ~CAutoRWS()
	{
		if(m_bWrite && m_pRWSynchro)
		{
			m_pRWSynchro->UnlockWrite();

		}
		else if( m_pRWSynchro )
		{
			m_pRWSynchro->UnlockRead();
		}
	}

private:
	
};

/////////////////////////////////////////////////////////////////////////////
// MSVC can't correctly instantiate 2 CreateComObjectEx's in one scope
// with different T

template<class T>
class DVCreator
{
public:
	static HRESULT Create(T** ppObject)
	{
		ATLASSERT(ppObject != NULL);
		if(ppObject == NULL) return E_POINTER;

		ATLASSERT(*ppObject == NULL);

		CComObject<T>* p = NULL;
		HRESULT hr = CComObject<T>::CreateInstance(&p);
		if(SUCCEEDED(hr))
		{
			p->AddRef();
			*ppObject = static_cast<T*>(p);
		}
		else
			*ppObject = NULL;
		return hr;
	}

	// ATTENTION.
	// Compiler don't checks type of _ppInterface.
	// Sample (compiled without errors) :
	//
	//		IBaseFilter** ppFilter;
	//		HRESULT hr = DVCreator<CCoDVDCorrector>::CreateEx(&ppFilter);
	//
	// The same problem also in CComCoClass::CreateInstance(Q** pp);

	template <class TInterface>
	static HRESULT CreateEx(TInterface** ppInterface)
	{
		ATLASSERT(ppInterface != NULL);
		if(ppInterface == NULL) return E_POINTER;
		ATLASSERT(*ppInterface == NULL);

		CComObject<T>* pComObject = NULL;
		HRESULT hr = CComObject<T>::CreateInstance(&pComObject);
		if (SUCCEEDED(hr))
		{
			pComObject->AddRef();
			hr = pComObject->QueryInterface(ppInterface);
			pComObject->Release();
		}
		else
			*ppInterface = NULL;
		return hr;
	}
};

/////////////////////////////////////////////////////////////////////////////
// Used for creating of COM object without overhead of CoCreateInstance.
template <class T>
HRESULT CreateComObject(T** ppObject)
{
	ATLASSERT(ppObject != NULL);
	if(ppObject == NULL) return E_POINTER;
	ATLASSERT(*ppObject == NULL);

	CComObject<T>* p = NULL;
	HRESULT hr = CComObject<T>::CreateInstance(&p);
	if(SUCCEEDED(hr))
	{
		p->AddRef();
		*ppObject = static_cast<T*>(p);
	}
	else
		*ppObject = NULL;
	return hr;
}

inline HRESULT __stdcall CreateInternalInstance(REFCLSID rclsid, REFIID riid, void** ppv)
{
	CComPtr<IClassFactory> spCF;
	HRESULT hr = ::DllGetClassObject(rclsid, __uuidof(IClassFactory), (void**)&spCF);
	if(hr == S_OK)
		hr = spCF->CreateInstance(NULL, riid, ppv);
	return hr;
}

//#define _FAKE_COM_PTR_M_

#ifdef _FAKE_COM_PTR_M_

#define CComPtrM	CComPtr
#define CComQIPtrM	CComQIPtr

#else
//////////////////////////////////////////////////////////////////////////
// Multithreaded CComPtr & CComQIPtr
// 

//CComPtrBase provides the basis for all other smart pointers
//The other smartpointers add their own constructors and operators
template <class T>
class CComPtrBaseM
{
public:
	T*		p;
protected:

	mutable SRWLOCK srw_lock;


	CComPtrBaseM() throw()
	{
		::InitializeSRWLock( &srw_lock );
		p = NULL;
		
	}
	CComPtrBaseM(_Inout_opt_ T* lp) throw()
	{
		::InitializeSRWLock( &srw_lock );

		if (lp != NULL)
			lp->AddRef();
		p = lp;
	}

	void Lock( bool _bWrite ) const throw()
	{
		if( _bWrite )
			::AcquireSRWLockExclusive( &srw_lock );
		else
			::AcquireSRWLockShared( &srw_lock );
	}
	void Unlock( bool _bWrite ) const throw()
	{
		if( _bWrite )
			::ReleaseSRWLockExclusive( &srw_lock );
		else
			::ReleaseSRWLockShared( &srw_lock );
	}

	T* GetPtr_With_AddRef() const throw()
	{
		Lock(false);
		T* pT = p;
		if( pT )
			pT->AddRef();
		Unlock(false);
		return pT;
	}

	IUnknown* GetQIPtr_With_AddRef( REFIID riid ) const throw()
	{
		IUnknown* pRes = NULL;
		Lock(false);
		T* pT = p;
		if( pT )
			pT->QueryInterface( riid, (void**)&pRes );
		Unlock(false);
		return pRes;
	}


public:
	typedef T _PtrClass;

	~CComPtrBaseM() throw()
	{
		Release();
	}
	operator T*() const throw()
	{
		return p;
	}
	T& operator*() const
	{
		ATLENSURE(p!=NULL);
		return *p;
	}
	//The assert on operator& usually indicates a bug.  If this is really
	//what is needed, however, take the address of the p member explicitly.
	T** operator&() throw()
	{
		ATLASSERT(p==NULL);
		return &p;
	}
	_NoAddRefReleaseOnCComPtr<T>* operator->() const throw()
	{
		ATLASSERT(p!=NULL);
		return (_NoAddRefReleaseOnCComPtr<T>*)p;
	}
	bool operator!() const throw()
	{	
		return (p == NULL);
	}
	bool operator<(_In_opt_ T* pT) const throw()
	{
		return p < pT;
	}
	bool operator!=(_In_opt_ T* pT) const
	{
		return !operator==(pT);
	}
	bool operator==(_In_opt_ T* pT) const throw()
	{
		return p == pT;
	}

	// Release the interface and set to NULL
	void Release() throw()
	{
		Lock(true);
		T* pT = (T*)::InterlockedExchangePointer( (void**)&p, NULL ) ;
		Unlock(true);
		if (pT)
			pT->Release();
	}
	// Compare two objects for equivalence
	bool IsEqualObject(_Inout_opt_ IUnknown* pOther) throw()
	{
		T* pThis = GetPtr_With_AddRef();
		if (pThis == NULL && pOther == NULL)
			return true;	// They are both NULL objects

		if (pThis == NULL || pOther == NULL)
		{
			if( pThis )
				pThis->Release();
			return false;	// One is NULL the other is not
		}

		CComPtr<IUnknown> punk1;
		CComPtr<IUnknown> punk2;
		pThis->QueryInterface(__uuidof(IUnknown), (void**)&punk1);
		pOther->QueryInterface(__uuidof(IUnknown), (void**)&punk2);
		pThis->Release();
		return punk1 == punk2;
	}

	// Compare two objects for equivalence
	bool IsEqualObject(const CComPtrBaseM<T>& _pOther) throw()
	{
		T* pOther = _pOther.GetPtr_With_AddRef();
		T* pThis = GetPtr_With_AddRef();
		if (pThis == NULL && pOther == NULL)
			return true;	// They are both NULL objects

		if (pThis == NULL || pOther == NULL)
		{
			if( pThis )
				pThis->Release();
			if( pOther )
				pOther->Release();
			return false;	// One is NULL the other is not
		}

		CComPtr<IUnknown> punk1;
		CComPtr<IUnknown> punk2;
		pThis->QueryInterface(__uuidof(IUnknown), (void**)&punk1);
		pOther->QueryInterface(__uuidof(IUnknown), (void**)&punk2);
		pThis->Release();
		pOther->Release();

		return punk1 == punk2;
	}
	// Attach to an existing interface (does not AddRef)
	void Attach(_In_opt_ T* pSet) throw()
	{
		Lock(true);
		T* pT = (T*)::InterlockedExchangePointer((void**)&p, pSet);
		Unlock(true);

		if(pT)
			pT->Release();
	}
	// Detach the interface (does not Release)
	T* Detach() throw()
	{
		Lock(true);
		T* pT = (T*)::InterlockedExchangePointer((void**)&p, NULL);
		Unlock(true);
		return pT;
	}
	_Check_return_ HRESULT CopyTo(_Deref_out_opt_ T** ppT) throw()
	{
		ATLASSERT(ppT != NULL);
		if(ppT == NULL)
			return E_POINTER;

		*ppT = GetPtr_With_AddRef();
		return S_OK;
	}
	_Check_return_ HRESULT SetSite(_Inout_opt_ IUnknown* punkParent) throw()
	{
		return AtlSetChildSite(p, punkParent);
	}
	_Check_return_ HRESULT Advise(
		_Inout_ IUnknown* pUnk, 
		_In_ const IID& iid, 
		_Out_ LPDWORD pdw) throw()
	{
		return AtlAdvise(p, pUnk, iid, pdw);
	}
	_Check_return_ HRESULT CoCreateInstance(
		_In_ REFCLSID rclsid, 
		_Inout_opt_ LPUNKNOWN pUnkOuter = NULL, 
		_In_ DWORD dwClsContext = CLSCTX_ALL) throw()
	{
		ATLASSERT(p == NULL);
		
		T* pSet = NULL;
		HRESULT hr = ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&pSet);
		if( FAILED( hr ) )
			return hr;

		Lock(true);
		T* pT = (T*)::InterlockedExchangePointer((void**)&p, pSet);
		Unlock(true);

		if(pT)
			pT->Release();

		return hr;
	}
	_Check_return_ HRESULT CoCreateInstance(
		_In_z_ LPCOLESTR szProgID, 
		_Inout_opt_ LPUNKNOWN pUnkOuter = NULL, 
		_In_ DWORD dwClsContext = CLSCTX_ALL) throw()
	{
		CLSID clsid = {};
		HRESULT hr = CLSIDFromProgID(szProgID, &clsid);
		if( FAILED( hr ) )
			return hr;

		return CoCreateInstance( clsid, pUnkOuter, dwClsContext);
	}
	template <class Q>
	_Check_return_ HRESULT QueryInterface(_Deref_out_ Q** pp) const throw()
	{
		ATLASSERT(pp != NULL && *pp == NULL);
		T* pThis = GetPtr_With_AddRef();
		ATLASSERT( pThis );
		if( pThis )
		{
			HRESULT hr = pThis->QueryInterface(__uuidof(Q), (void**)pp);
			pThis->Release();
			return hr;
		}

		return E_UNEXPECTED;
	}

};

template <class T>
class CComPtrM : 
	public CComPtrBaseM<T>
{
	typedef CComPtrM<T> _CComPtrM;
public:
	CComPtrM() throw()
	{
	}
	CComPtrM(_Inout_opt_ T* lp) throw() :
		CComPtrBaseM<T>(lp)
	{
	}
	CComPtrM(_Inout_ const CComPtrM<T>& lp) throw()
	{	
		p = lp.GetPtr_With_AddRef();
	}
	T* operator=(_Inout_opt_ T* lp) throw()
	{
		if(*this!=lp)
		{
			if(lp != NULL)
				lp->AddRef();

			Lock(true);
			T* pT = (T*)::InterlockedExchangePointer((void**)&p, lp);
			Unlock(true);

			if(pT)
				pT->Release();
		}
		return *this;
	}
	template <typename Q>
	T* operator=(_Inout_ const CComPtrM<Q>& lp) throw()
	{
		if( !IsEqualObject(lp) )
		{
			T* pSet = lp.GetQIPtr_With_AddRef(__uuidof(T));

			Lock(true);
			T* pT = (T*)::InterlockedExchangePointer((void**)&p, pSet);
			Unlock(true);

			if(pT)
				pT->Release();
		}
		return *this;
	}
	T* operator=(_Inout_ const CComPtrM<T>& lp) throw()
	{
		if(*this!=lp)
		{
			T* pSet = lp.GetPtr_With_AddRef();

			Lock(true);
			T* pT = (T*)::InterlockedExchangePointer((void**)&p, pSet);
			Unlock(true);

			if(pT)
				pT->Release();
		}
		return *this;
	}	
	CComPtrM(_Inout_ CComPtrM<T>&& lp) throw() :	
		CComPtrBaseM<T>()
	{	
		p = lp.GetPtr_With_AddRef();
		lp.Release();
	}	
	T* operator=(_Inout_ CComPtrM<T>&& lp) throw()
	{			
		if (*this != lp)
		{
			T* pSet = lp.GetPtr_With_AddRef();

			Lock(true);
			T* pT = (T*)::InterlockedExchangePointer((void**)&p, pSet);
			Unlock(true);

			if(pT)
				pT->Release();

			lp.Release();
		}
		return *this;		
	}
};

template <class T, const IID* piid = &__uuidof(T)>
class CComQIPtrM : 
	public CComPtrM<T>
{
public:
	CComQIPtrM() throw()
	{
	}
// 	CComQIPtrM(_Inout_opt_ T* lp) throw() :
// 		CComPtrM<T>(lp)
// 	{
// 	}
	CComQIPtrM(_Inout_ const CComQIPtrM<T,piid>& lp) throw() :
		CComPtrM<T>(lp)
	{
	}
	CComQIPtrM(_Inout_opt_ IUnknown* lp) throw()
	{
		if (lp != NULL)
			lp->QueryInterface(*piid, (void **)&p);
	}
// 	T* operator=(_Inout_opt_ T* lp) throw()
// 	{
// 		if(*this!=lp)
// 		{
// 			if(lp != NULL)
// 				lp->AddRef();
// 
// 			Lock(true);
// 			T* pT = (T*)::InterlockedExchangePointer((void**)&p, lp);
// 			Unlock(true);
// 
// 			if(pT)
// 				pT->Release();
// 		}
// 		return *this;
// 	}
	template <typename Q>
	T* operator=(_Inout_ const CComQIPtrM<Q,__uuidof(Q)>& lp) throw()
	{
		if(*this!=lp)
		{
			T* pSet = lp.GetQIPtr_With_AddRef(__uuidof(T));

			Lock(true);
			T* pT = (T*)::InterlockedExchangePointer((void**)&p, pSet);
			Unlock(true);

			if(pT)
				pT->Release();
		}
		return *this;
	}
	T* operator=(_Inout_opt_ IUnknown* lp) throw()
	{
		if(*this!=lp)
		{
			T* pSet = NULL;
			if( lp )
				lp->QueryInterface( *piid, (void**)&pSet );

			Lock(true);
			T* pT = (T*)::InterlockedExchangePointer((void**)&p, pSet);
			Unlock(true);

			if(pT)
				pT->Release();
		}
		return *this;
	}
};

//Specialization to make it work
template<>
class CComQIPtrM<IUnknown, &IID_IUnknown> : 
	public CComPtrM<IUnknown>
{
public:
	CComQIPtrM() throw()
	{
	}
	CComQIPtrM(_Inout_opt_ IUnknown* lp) throw()
	{
		//Actually do a QI to get identity
		if (lp != NULL)
			lp->QueryInterface(__uuidof(IUnknown), (void **)&p);
	}
	CComQIPtrM(_Inout_ const CComQIPtrM<IUnknown,&IID_IUnknown>& lp) throw() :
		CComPtrM<IUnknown>(lp.p)
	{
	}
	IUnknown* operator=(_Inout_opt_ IUnknown* lp) throw()
	{
		if(*this!=lp)
		{
			IUnknown* pSet = NULL;
			if( lp )
				lp->QueryInterface( IID_IUnknown, (void**)&pSet );

			Lock(true);
			IUnknown* pT = (IUnknown*)::InterlockedExchangePointer((void**)&p, pSet);
			Unlock(true);

			if(pT)
				pT->Release();
		}
		return *this;
	}

	IUnknown* operator=(_Inout_ const CComQIPtrM<IUnknown,&IID_IUnknown>& lp) throw()
	{
		if(*this!=lp)
		{
			IUnknown* pSet = lp.GetQIPtr_With_AddRef(IID_IUnknown);

			Lock(true);
			IUnknown* pT = (IUnknown*)::InterlockedExchangePointer((void**)&p, pSet);
			Unlock(true);

			if(pT)
				pT->Release();
		}
		return *this;
	}
};

#endif // CComPtrM

#pragma warning(pop)

/////////////////////////////////////////////////////////////////////////////
#endif	 // __WRAPPERS_H_ 
