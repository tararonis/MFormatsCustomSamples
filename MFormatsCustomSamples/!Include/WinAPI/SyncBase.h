//---------------------------------------------------------------------------
// This file is part of the MediaLooks DirectShow Template Library (DSTL)
//
// SyncBase.h : wrapper classes for some Windows sync primitives
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

#ifndef __SYNCBASE_H_
#define __SYNCBASE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
using std::map;
using std::make_pair;

class CMutex
{
	friend class CAutoMutex;

	CMutex( int nFake )
	{
		m_hMutex = NULL;
	}
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

class CAutoMutex
{
public:
	CAutoMutex( CMutex& _rMutex )
		: m_theMutex( -1 )
	{
		m_pMutex = &_rMutex;

		::WaitForSingleObject( m_pMutex->m_hMutex, INFINITE );
	}
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
	CMutex	m_theMutex;
	CMutex* m_pMutex;
};


#define RWLITE_DEFAULT_SPIN		(500)

// Call in the same thread
// Lock Read -> Lock Read -> Ok
// Lock Write -> Lock Read -> Ok
// Lock Write -> Lock Write -> Ok
// Lock Read -> Lock Write -> DEADLOCK
class CRWSynchroLite
{
	CRITICAL_SECTION	m_csWrite;
	volatile	long	m_lReaders;
#ifdef _DEBUG
	typedef std::map<DWORD,int> TMapReaders;
	CComAutoCriticalSection		m_csMap;
	TMapReaders					m_mapReaders;
#endif

	//---------------------------------
public:
	CRWSynchroLite( DWORD _dwSpinCount = RWLITE_DEFAULT_SPIN )
		: m_lReaders( 0 )
	{
		::InitializeCriticalSectionAndSpinCount( &m_csWrite, _dwSpinCount );
	}

	~CRWSynchroLite()
	{
		::DeleteCriticalSection( &m_csWrite );
	}

	void LockRead()
	{
		::EnterCriticalSection( &m_csWrite ); // <- Wait here till write complete

#ifdef _DEBUG
		{
			CAutoCS crs(m_csMap);
			// If m_lReaders < 0 -> current thread already write, do nothing (the interlocked is not required)
			long lReaders = InterlockedIncrementGreaterEqualThan( &m_lReaders, 0 );
			if( lReaders > 0 )
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
				ATLASSERT( m_csWrite.OwningThread == GetCurrentThreadId() );
			}
		}
#else
		// If m_lReaders < 0 -> current thread already write, do nothing (the interlocked is not required)
		InterlockedIncrementGreaterEqualThan( &m_lReaders, 0 );
#endif

		::LeaveCriticalSection( &m_csWrite );
	}

	void UnlockRead()
	{
#ifdef _DEBUG
		{
			CAutoCS crs(m_csMap);

			TMapReaders::iterator Iter = m_mapReaders.find( GetCurrentThreadId() );
			if( Iter == m_mapReaders.end() )
			{
				// Not found -> begin read after write
				ATLASSERT( m_mapReaders.size() == 0 );
				ATLASSERT( m_csWrite.OwningThread == GetCurrentThreadId() );

				long lRes = InterlockedDecrementGreaterThan( &m_lReaders, 0 );
				ATLASSERT( lRes < 0 );
			}
			else
			{
				if( Iter->second == 1 )
				{
					// Remove reader
					m_mapReaders.erase( Iter );
				}
				else
				{
					ATLASSERT( Iter->second > 1 );
					Iter->second--;
				}

				long lRes = InterlockedDecrementGreaterThan( &m_lReaders, 0 );
				ATLASSERT( lRes > 0 || ( lRes == 0 && m_mapReaders.size() == 0 ));
			}
		}
		
#else
		// If m_lReaders < 0 -> current thread already write, do nothing
		// If m_lReaders == 0 -> error
		InterlockedDecrementGreaterThan( &m_lReaders, 0 );
#endif
	}

	// WARNING !!! Lock Read -> Lock Write NOT ALLOWED -> Dead lock
	void LockWrite()
	{
		::EnterCriticalSection( &m_csWrite ); // <- Wait here till write from other thread complete

#ifdef _DEBUG
		{
			CAutoCS crs(m_csMap);

			TMapReaders::iterator Iter = m_mapReaders.find( GetCurrentThreadId() );
			if( Iter != m_mapReaders.end() )
			{
				ATLASSERT( !"CRWSynchroLite::LockWrite() - Call after LockRead() in same thread" );
			}
		}
#endif
		// At this point no new reader allowed, wait till all readers finish read
		// The second writer (in same thread) decrease readers counter
		while( InterlockedDecrementLessEqualThan( &m_lReaders, 0 ) > 0 )
			Sleep(1);

		// Now all reader finish read operation, and new wait in CS
		ATLASSERT( m_mapReaders.size() == 0 );
	}
	void UnlockWrite()
	{
		// The readers not allowed in this point
#ifdef _DEBUG
		ATLASSERT( m_lReaders < 0 );
		ATLASSERT( m_mapReaders.size() == 0 );
		ATLASSERT( m_csWrite.OwningThread == GetCurrentThreadId() );
#endif
		
		// The interlocked is not required
		InterlockedIncremntLessThan( &m_lReaders, 0 );

		::LeaveCriticalSection( &m_csWrite );
	}

public:
	// Add value ONLY if greater or equal specified value 
	inline static long InterlockedAddGreaterEqualThan( long volatile *Destination, long Greater, long Add )
	{
		while( true ) 
		{
			// lValue == Greater -> Add value -> Return 
			// lValue <  Greater -> Can't decrease -> Return 
			// lValue >  Greater -> Remember current value, add in next call 
			long lValue = ::InterlockedCompareExchange( Destination, Greater + Add, Greater );
			if( lValue <= Greater ) 
				return lValue;

			// If value change from last call, try next cycle
			long lCheck = ::InterlockedCompareExchange( Destination, lValue + Add, lValue );
			if( lCheck == lValue ) 
				return lValue;
		}
	}

	inline static long InterlockedAddLessEqualThan( long volatile *Destination, long Less, long Add )
	{
		while( true ) 
		{
			// lValue == Greater -> increase from Greater to 1 -> Return 
			// lValue >  Greater -> Can't decrease -> Return 
			// lValue <  Greater -> Remember current value, decrease in next call 
			long lValue = ::InterlockedCompareExchange( Destination, Less + Add, Less );
			if( lValue >= Less ) 
				return lValue;

			// If value change from last call, try next cycle
			long lCheck = ::InterlockedCompareExchange( Destination, lValue + Add, lValue );
			if( lCheck == lValue ) 
				return lValue;
		}
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
		return InterlockedAddGreaterEqualThan( Destination, Less, -1 );
	}

	inline static long InterlockedIncremntLessThan( long volatile *Destination, long Less )
	{
		return InterlockedAddGreaterEqualThan( Destination, Less, 1 );
	}

};

//---------------------------------
class CAutoRWL
{
	CAutoRWL(const CAutoRWL& autoSR);
	CAutoRWL& operator=(const CAutoRWL& autoSR);

public:
	CAutoRWL(CRWSynchroLite& rwSynchro, bool bWrite) : m_rwSynchro(rwSynchro), m_bWrite(bWrite)
	{
		if(m_bWrite)
			m_rwSynchro.LockWrite();
		else
			m_rwSynchro.LockRead();
	}
	~CAutoRWL()
	{
		if(m_bWrite)
			m_rwSynchro.UnlockWrite();
		else
			m_rwSynchro.UnlockRead();
	}

private:
	CRWSynchroLite& m_rwSynchro;
	const bool m_bWrite;
};


/////////////////////////////////////////////////////////////////////////////
#endif	 // __SYNCBASE_H_ 
