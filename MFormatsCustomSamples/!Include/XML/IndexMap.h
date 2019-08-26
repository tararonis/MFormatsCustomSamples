//---------------------------------------------------------------------------
// This file is part of the Medialooks MPlatform/MFormats
//
// IndexMap.h 
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

#ifndef __INDEX_MAP_H_
#define __INDEX_MAP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../WinAPI/Containers.h"
#include "../WinAPI/wrappers.h"
#include "../WinAPI/threads.h"

#include <map>
#include <vector>
using std::map;
using std::vector;
using std::make_pair;



/////////////////////////////////////////////////////////////////////////////
// CStrHashMap
template <class T, class TVal>
class CStrIndexMapBase
	: public CSimpleMap2<CComBSTR,TVal>
{
public:
	CStrIndexMapBase()
	{
	}
	~CStrIndexMapBase()
	{
	}

	template <class TMap>
	int AddFromKey( const TMap& _rAdd, TVal _valSet )
	{
		T* pT = static_cast<T*>(this);

		int nAdded = 0;
		for( int i = 0; i < _rAdd.GetSize(); i++ )
		{
			nAdded += pT->Add( _rAdd.GetKeyAt(i), _valSet );
		}

		return nAdded;
	}

	template <class TArrayOrMap>
	int AddFromValue( const TArrayOrMap& _rAdd, TVal _valSet )
	{
		T* pT = static_cast<T*>(this);

		int nAdded = 0;
		for( int i = 0; i < _rAdd.GetSize(); i++ )
		{
			nAdded += pT->Add( _rAdd.GetValueAt(i), _valSet );
		}

		return nAdded;
	}

	template <class TMap>
	int AddFromMap( const TMap& _rMapAdd )
	{
		T* pT = static_cast<T*>(this);

		int nAdded = 0;
		for( int i = 0; i < _rMapAdd.GetSize(); i++ )
		{
			nAdded += pT->Add( _rMapAdd.GetKeyAt(i), _rMapAdd.GetValueAt(i) );
		}

		return nAdded;
	}

	//---------------------------------
	// Addition of existing key is forbidden
	BOOL Add(LPCWSTR key, const TVal& val)
	{
		T* pT = static_cast<T*>(this);

		if(pT->FindKey_Dup(key) != -1)
			return FALSE;

		// Increment size
		if(m_nSize == m_nAllocSize)
		{
			// Make sure newElement is not a reference to an element in the array.
			// Or else, it will be invalidated by the reallocation.
// 			ATLASSERT(&key < m_aKey || &key >= m_aKey + m_nAllocSize);
// 			ATLASSERT(&val < m_aVal || &val >= m_aVal + m_nAllocSize);
			if(!Realloc())
				return FALSE;
		}

		InternalSetAtIndex(m_nSize, key, val);
		pT->AddIndex( key, m_nSize );
		m_nSize++;
		
		return TRUE;
	}

	// Insertion of existing key is forbidden
	BOOL InsertAtIndex(int nIndex, LPCWSTR key, const TVal& val)
	{
		// If nIndex == m_nSize: one new element can be added after end of array
		ATLASSERT(0 <= nIndex && nIndex <= m_nSize);
		if(nIndex < 0 || m_nSize < nIndex)
			return FALSE;

		T* pT = static_cast<T*>(this);
		if(pT->FindKey_Dup(key) != -1)
			return FALSE;

		// Increment size
		if(m_nSize == m_nAllocSize)
		{
			// Make sure newElement is not a reference to an element in the array.
			// Or else, it will be invalidated by the reallocation.
// 			ATLASSERT(&key < m_aKey || &key >= m_aKey + m_nAllocSize);
// 			ATLASSERT(&val < m_aVal || &val >= m_aVal + m_nAllocSize);
			if(!Realloc())
				return FALSE;
		}

		// Move higher elements up
		if(nIndex < m_nSize)
		{
			// memmove() correctly copyes overlapped buffers instead of memcpy()
			memmove((void*)(m_aKey + nIndex + 1), (void*)(m_aKey + nIndex), (m_nSize - nIndex) * sizeof(CComBSTR));
			memmove((void*)(m_aVal + nIndex + 1), (void*)(m_aVal + nIndex), (m_nSize - nIndex) * sizeof(TVal));
		}

		InternalSetAtIndex(nIndex, key, val);
		++m_nSize;

		pT->UpdateIndex();
		return TRUE;
	}

	// Setting of existing key (with exception of nIndex key) is forbidden
	BOOL SetAtIndex(int nIndex, LPCWSTR key, const TVal& val)
	{
		ATLASSERT(0 <= nIndex && nIndex < m_nSize);
		if(nIndex < 0 || m_nSize <= nIndex)
			return FALSE;

		T* pT = static_cast<T*>(this);
		int n = pT->FindKey_Dup(key);
		if(n != -1 && n != nIndex)
			return FALSE;

		m_aKey[nIndex].~CComBSTR();
		m_aVal[nIndex].~TVal();
		InternalSetAtIndex(nIndex, key, val);
		pT->UpdateIndex();

		return TRUE;
	}

	//---------------------------------
	BOOL Remove(LPCWSTR key)
	{
		T* pT = static_cast<T*>(this);
		int nIndex = pT->FindKey(key);

		return (nIndex != -1)? pT->RemoveAt(nIndex) : FALSE;
	}

	BOOL RemoveAt(int nIndex)
	{
		ATLASSERT(0 <= nIndex && nIndex < m_nSize);
		if(nIndex < 0 || m_nSize <= nIndex)
			return FALSE;

		m_aKey[nIndex].~CComBSTR();
		m_aVal[nIndex].~TVal();

		if(nIndex != m_nSize - 1)
		{
			memmove((void*)(m_aKey + nIndex), (void*)(m_aKey + nIndex + 1), (m_nSize - (nIndex + 1)) * sizeof(CComBSTR));
			memmove((void*)(m_aVal + nIndex), (void*)(m_aVal + nIndex + 1), (m_nSize - (nIndex + 1)) * sizeof(TVal));
		}

		T* pT = static_cast<T*>(this);
		m_nSize--;
		pT->UpdateIndex();

		return TRUE;
	}
	void RemoveAll()
	{
		if(m_aKey != NULL)
		{
			for(int i = 0; i < m_nSize; i++)
				m_aKey[i].~CComBSTR();

			free(m_aKey);
			m_aKey = NULL;
		}
		if(m_aVal != NULL)
		{
			for(int i = 0; i < m_nSize; i++)
				m_aVal[i].~TVal();

			free(m_aVal);
			m_aVal = NULL;
		}

		m_nSize = 0;
		m_nAllocSize = 0;

		T* pT = static_cast<T*>(this);
		pT->UpdateIndex();
	}
};

template <class TVal>
class CStrHashMap:
	public CStrIndexMapBase<CStrHashMap<TVal>,TVal>
{
	friend class CStrIndexMapBase<CStrHashMap<TVal>,TVal>;
protected:
	typedef map<UINT64,int> TIndexMap;
	mutable TIndexMap	m_mapIndex;
	bool		m_bIgnoreCase;
	
public:
	CStrHashMap( bool _bIgnoreCase = true )
		: m_bIgnoreCase( _bIgnoreCase )
	{
	}

	int FindKey(LPCWSTR key) const
	{
		UINT64 n64Key = Str2Hash64_I(key, m_bIgnoreCase);
		TIndexMap::iterator Iter = m_mapIndex.find( n64Key );
		if( Iter != m_mapIndex.end() )
		{
			ATLASSERT( Iter->first == Str2Hash64_I(m_aKey[Iter->second], m_bIgnoreCase ) );
			ATLASSERT( !key || StrCmpIW( key, m_aKey[Iter->second] ) == 0 );
			return Iter->second;
		}
		return -1;  // not found
	}

	
	
protected:
	int FindKey_Dup(LPCWSTR key) const
	{
		return FindKey( key );
	}
	inline void AddIndex( LPCWSTR key, int nIndex ) const
	{
		UINT64 n64Key = Str2Hash64_I(key, m_bIgnoreCase);
		ATLASSERT( m_mapIndex.find( n64Key ) == m_mapIndex.end() );
		m_mapIndex.insert( make_pair( n64Key, nIndex ) );
	}

	inline bool UpdateIndex() const
	{
		m_mapIndex.clear();

		for(int i = 0; i < m_nSize; i++)
		{
			AddIndex( m_aKey[i], i );
		}
		return true;
	}
};

template <class TVal>
class CStrHashMultiMap:
	public CStrIndexMapBase<CStrHashMultiMap<TVal>,TVal>
{
	friend class CStrIndexMapBase<CStrHashMultiMap<TVal>,TVal>;

protected:
	typedef vector<int> TDupVector;
	typedef map<UINT64,TDupVector> TIndexMap;
	mutable TIndexMap	m_mapIndex;
	bool		m_bIgnoreCase;

	
public:
	CStrHashMultiMap( bool _bIgnoreCase = true )
		: m_bIgnoreCase( _bIgnoreCase )
	{
	}

	BOOL Remove(LPCWSTR key, UINT _nOrder )
	{
		int nIndex = FindKey(key, _nOrder );

		return (nIndex != -1) ? RemoveAt(nIndex) : FALSE;
	}

	int FindKey(LPCWSTR key, UINT _nOrder ) const
	{
		UINT64 n64Key = Str2Hash64_I(key, m_bIgnoreCase);
		TIndexMap::iterator Iter = m_mapIndex.find( n64Key );
		if( Iter != m_mapIndex.end() )
		{
			ATLASSERT( Iter->second.size() > 0 );
			if( Iter->second.size() > _nOrder )
			{
				ATLASSERT( Iter->first == Str2Hash64_I(m_aKey[Iter->second[_nOrder]], m_bIgnoreCase ) );
				ATLASSERT( !key || StrCmpIW( key, m_aKey[ Iter->second[_nOrder] ] ) == 0 );
				return Iter->second[_nOrder];
			}
		}

		return -1;  // not found
	}

	int KeyDuplicates(LPCWSTR key) const
	{
		UINT64 n64Key = Str2Hash64_I(key, m_bIgnoreCase);
		TIndexMap::iterator Iter = m_mapIndex.find(n64Key);
		if (Iter != m_mapIndex.end())
			return (int)Iter->second.size();
		
		return -1;  // not found
	}

protected:

	int FindKey_Dup(LPCWSTR key) const
	{
		return -1;
	}

	inline void AddIndex( const CComBSTR& key, int nIndex ) const
	{
		UINT64 n64Key = Str2Hash64_I(key, m_bIgnoreCase);
		ATLASSERT( n64Key || !key.m_str );
		TIndexMap::iterator Iter = m_mapIndex.find( n64Key );
		if( Iter == m_mapIndex.end() )
		{
			Iter = m_mapIndex.insert( make_pair( n64Key, TDupVector() ) ).first;
		}

		Iter->second.push_back( nIndex );
	}

	inline bool UpdateIndex() const
	{
		m_mapIndex.clear();

		for(int i = 0; i < m_nSize; i++)
		{
			AddIndex( m_aKey[i], i );
		}
		return true;
	}
};


/////////////////////////////////////////////////////////////////////////////
// CIndexMap
template <class TKey, class TVal>
class CIndexMap
	: public CSimpleMap2<TKey,TVal,false>
{
	typedef map<TKey,int> TIndexMap;
	mutable TIndexMap	m_mapIndex;
public:
	CIndexMap()
	{
	}
	~CIndexMap()
	{
		RemoveAll();
	}

	//---------------------------------
	// Addition of existing key is forbidden
	BOOL Add(const TKey& key, const TVal& val)
	{
		if(FindKey(key) != -1)
			return FALSE;

		// Increment size
		if(m_nSize == m_nAllocSize)
		{
			// Make sure newElement is not a reference to an element in the array.
			// Or else, it will be invalidated by the reallocation.
			ATLASSERT(&key < m_aKey || &key >= m_aKey + m_nAllocSize);
			ATLASSERT(&val < m_aVal || &val >= m_aVal + m_nAllocSize);
			if(!Realloc())
				return FALSE;
		}

		InternalSetAtIndex(m_nSize, key, val);
		m_mapIndex.insert( make_pair(key, m_nSize) );
		++m_nSize;
		return TRUE;
	}

	// Insertion of existing key is forbidden
	BOOL InsertAtIndex(int nIndex, const TKey& key, const TVal& val)
	{
		// If nIndex == m_nSize: one new element can be added after end of array
		ATLASSERT(0 <= nIndex && nIndex <= m_nSize);
		if(nIndex < 0 || m_nSize < nIndex)
			return FALSE;

		if(FindKey(key) != -1)
			return FALSE;

		// Increment size
		if(m_nSize == m_nAllocSize)
		{
			// Make sure newElement is not a reference to an element in the array.
			// Or else, it will be invalidated by the reallocation.
			ATLASSERT(&key < m_aKey || &key >= m_aKey + m_nAllocSize);
			ATLASSERT(&val < m_aVal || &val >= m_aVal + m_nAllocSize);
			if(!Realloc())
				return FALSE;
		}

		// Move higher elements up
		if(nIndex < m_nSize)
		{
			// memmove() correctly copyes overlapped buffers instead of memcpy()
			memmove((void*)(m_aKey + nIndex + 1), (void*)(m_aKey + nIndex), (m_nSize - nIndex) * sizeof(TKey));
			memmove((void*)(m_aVal + nIndex + 1), (void*)(m_aVal + nIndex), (m_nSize - nIndex) * sizeof(TVal));
		}

		InternalSetAtIndex(nIndex, key, val);
		++m_nSize;


		UpdateIndex();
		
		return TRUE;
	}

	// Setting of existing key (with exception of nIndex key) is forbidden
	BOOL SetAtIndex(int nIndex, const TKey& key, const TVal& val)
	{
		ATLASSERT(0 <= nIndex && nIndex < m_nSize);
		if(nIndex < 0 || m_nSize <= nIndex)
			return FALSE;

		int n = FindKey(key);
		if(n != -1 && n != nIndex)
			return FALSE;

		// Remove old key
		m_mapIndex.erase( m_aKey[nIndex ] );
		
		m_aKey[nIndex].~TKey();
        m_aVal[nIndex].~TVal();
		InternalSetAtIndex(nIndex, key, val);

		m_mapIndex.insert( make_pair(key, nIndex) );

		return TRUE;
	}

	//---------------------------------
	BOOL Remove(const TKey& key)
	{
		int nIndex = FindKey(key);
		
		return (nIndex != -1)? RemoveAt(nIndex) : FALSE;
	}
	BOOL RemoveAt(int nIndex)
	{
		ATLASSERT(0 <= nIndex && nIndex < m_nSize);
		if(nIndex < 0 || m_nSize <= nIndex)
			return FALSE;

        m_aKey[nIndex].~TKey();
        m_aVal[nIndex].~TVal();

		if(nIndex != m_nSize - 1)
		{
			memmove((void*)(m_aKey + nIndex), (void*)(m_aKey + nIndex + 1), (m_nSize - (nIndex + 1)) * sizeof(TKey));
		    memmove((void*)(m_aVal + nIndex), (void*)(m_aVal + nIndex + 1), (m_nSize - (nIndex + 1)) * sizeof(TVal));
		}

		--m_nSize;

		UpdateIndex();

		return TRUE;
	}
	void RemoveAll()
	{
		if(m_aKey != NULL)
		{
			for(int i = 0; i < m_nSize; i++)
				m_aKey[i].~TKey();

			free(m_aKey);
			m_aKey = NULL;
		}
		if(m_aVal != NULL)
		{
			for(int i = 0; i < m_nSize; i++)
				m_aVal[i].~TVal();

			free(m_aVal);
			m_aVal = NULL;
		}

		UpdateIndex();

		m_nSize = 0;
		m_nAllocSize = 0;
	}


	int FindKey(const TKey& key) const
	{
		TIndexMap::iterator Iter = m_mapIndex.find( key );
		if( Iter != m_mapIndex.end() )
		{
			ATLASSERT( key == m_aKey[Iter->second] );
			return Iter->second;
		}
		return -1;  // not found
	}
};


template<class TAutoObj, class TRWSync = CRWSynchroLite>
class CNamedPtrMap
{
protected:
	TRWSync						m_rwNamedObj;
	typedef typename TAutoObj::TPtr TAutoObjPtr;

	CStrHashMap<TAutoObjPtr>	m_mapNamedObj;
	typedef CNamedPtrMap<TAutoObj, TRWSync> _NamedPtrMap;
public:

	CNamedPtrMap( bool _bUseTLS = false )
		: m_rwNamedObj( RWLITE_CS_SPIN, _bUseTLS )
	{
	}

	int NamedPtr_Remove( LPCWSTR _pszName, bool _bAllowEmptyNames = false)
	{
		int nRemoved = 0;

		if( (!_pszName || !_pszName[0]) && !_bAllowEmptyNames )
		{
			return NamedPtr_RemoveAll();
		}
		
		if( !_pszName )
			_pszName = L"";

		CAutoRWL rw(m_rwNamedObj, true);

		int nIndex = m_mapNamedObj.FindKey( _pszName );
		if( nIndex >= 0 )
		{
			ATLTRACE2( atlTraceGeneral, 8, _T("CNamedPtrMap(%08X)::NamedPtr_Remove( %ws ) Size:%d Idx:%d\n"),
				(_NamedPtrMap*)this, _pszName, m_mapNamedObj.GetSize(), nIndex );

			m_mapNamedObj.RemoveAt(nIndex);
			nRemoved = 1;
		}

		return nRemoved;
	}

	int NamedPtr_RemovePtr(TAutoObjPtr _pRemoveObject)
	{
		int nRemoved = 0;

		CAutoRWL rw(m_rwNamedObj, true);

		int nIndex = m_mapNamedObj.FindVal(_pRemoveObject);
		if (nIndex >= 0)
		{
			MTRACE("(%08X)::NamedPtr_Remove( pObj ) Size:%d Idx:%d",
				(ULONG)(_NamedPtrMap*)this,  m_mapNamedObj.GetSize(), nIndex);

			m_mapNamedObj.RemoveAt(nIndex);
			nRemoved = 1;
		}

		return nRemoved;
	}

	int NamedPtr_RemoveAll()
	{
		CAutoRWL rw(m_rwNamedObj, true);

		MTRACE("(%08X)::NamedPtr_RemoveAll() Size:%d",
			(ULONG)(_NamedPtrMap*)this, m_mapNamedObj.GetSize() );

		int nRemoved = m_mapNamedObj.GetSize();
		m_mapNamedObj.RemoveAll();
		return nRemoved;
	}

	TAutoObjPtr NamedPtr_Get( LPCWSTR _pszName, bool _bCreateNew, bool _bAllowEmptyNames )
	{
		if( !_bAllowEmptyNames )
		{
			if( !_pszName || !_pszName[0] )
				return (TAutoObjPtr)NULL;
		}
		
		if( !_pszName )
			_pszName = L"";
		
		int nAttempt = 32;
		while( nAttempt-- > 0  )
		{
			{
				CAutoRWL rw(m_rwNamedObj, false);

				int nIndex = m_mapNamedObj.FindKey( _pszName );
				if( nIndex >= 0 )
				{
					return m_mapNamedObj[nIndex];
				}
			}

			if( !_bCreateNew )
				break;

			CAutoRWL rw(m_rwNamedObj, true);

			TAutoObj::TPtr pObject;
			TAutoObj::CreateInstance( &pObject );

			ATLTRACE2( atlTraceGeneral, 8, _T("CNamedPtrMap(%08X)::NamedPtr_Get( %ws ) Size:%d CREATE NEW\n"),
				(_NamedPtrMap*)this, _pszName, m_mapNamedObj.GetSize() );

			if( m_mapNamedObj.Add( _pszName, pObject ) )
				return pObject;
		}

		ATLASSERT( nAttempt > 0 );

		return (TAutoObjPtr)NULL;
	}

	TAutoObjPtr operator [] (LPCWSTR _pszNamed )
	{
		return NamedPtr_Get( _pszNamed, true, true );
	}
};


template<class TAutoObj, class TRWSync = CRWSynchroLite>
class CNamedPtrMap_GC
	: public CNamedPtrMap<TAutoObj, TRWSync>
{
	// GC thread
	friend class CSimpleThread<CNamedPtrMap_GC>;
	CSimpleThread<CNamedPtrMap_GC>		m_theThread;
protected:
	typedef CNamedPtrMap_GC<TAutoObj, TRWSync> _NamedPtrMap_GC;

	typedef typename _NamedPtrMap::TAutoObjPtr TAutoObjPtr;
	typedef std::map<LONGLONG, std::pair<CComBSTR,TAutoObjPtr> > TMapOrder;
public:
	int				m_nMinObjects;
	int				m_nMaxObjects;
	REFERENCE_TIME	m_rtUnusedTime;
public:

	CNamedPtrMap_GC( int _nMin = 3, int _nMax = 16, REFERENCE_TIME _rtUnusedTime = 3 * DS_ONESEC )
		: m_nMinObjects( _nMin ), m_nMaxObjects( _nMax ), m_rtUnusedTime( _rtUnusedTime )
	{
		m_theThread.Start(this);
	}

	~CNamedPtrMap_GC()
	{
		m_theThread.Stop();
	}

	void GC_Collect()
	{
		m_theThread.Awake();
	}

	TAutoObjPtr NamedPtr_Get(LPCWSTR _pszName, bool _bCreateNew, bool _bAllowEmptyNames, REFERENCE_TIME _rtExtraOffsetForKeepFromGC = 0 )
	{
		TAutoObjPtr pRes = _NamedPtrMap::NamedPtr_Get(_pszName, _bCreateNew, _bAllowEmptyNames);
		if (pRes)
			pRes->QPCOffset_Set(_rtExtraOffsetForKeepFromGC);

		return pRes;
	}

	int GC_GetByTimeOrder( TMapOrder* _pMapOrder, int _nMaxRef = 1 )
	{
		CAutoRWL rw(m_rwNamedObj, false);

		for( int i = 0; i < m_mapNamedObj.GetSize(); i++ )
		{
			if( _nMaxRef < 0 || m_mapNamedObj[i]->RefCount() <= _nMaxRef )
				_pMapOrder->insert( make_pair(m_mapNamedObj[i]->QPCLast(), make_pair( m_mapNamedObj.GetKeyAt(i), m_mapNamedObj[i] ) ) );
		}

		return (int)_pMapOrder->size();
	}

protected:
	
	int GC_RemoveOld( int _nMin, int _nMax, REFERENCE_TIME _rtUnusedTime )
	{
		LONGLONG llQPCFreq = 0;
		::QueryPerformanceFrequency( (LARGE_INTEGER*)&llQPCFreq );
		LONGLONG llQPCMin = 0;
		::QueryPerformanceCounter( (LARGE_INTEGER*)&llQPCMin );
		llQPCMin -= DBL2INT64( (double)_rtUnusedTime * llQPCFreq / DS_ONESEC);

		if( m_mapNamedObj.GetSize() <= _nMin )
			return 0;

		TMapOrder mapTimes;
		GC_GetByTimeOrder( &mapTimes, -1 );

		int nRemoved = 0;
		{
			CAutoRWL rw(m_rwNamedObj, true);

			// Remove all objects (the object will be released out-of RW scope in mapTimes destructor - as release(close) could use a enough of time)
			for( auto Iter = mapTimes.begin(); Iter != mapTimes.end(); Iter++ )
			{
				if( (int)m_mapNamedObj.GetSize() > _nMax || Iter->first < llQPCMin )
				{
					ATLTRACE2( atlTraceGeneral, 8, _T("CNamedPtrMap_GC(%08X)::GC_RemoveOld( Size:%d Min:%d Max:%d Time:%s ) - REMOVE:%ws QPCTime:%I64d (Min:%I64d) \n"),
						(_NamedPtrMap*)this, m_mapNamedObj.GetSize(), _nMin, _nMax, RT2STR( _rtUnusedTime ), Iter->second.first, Iter->first, llQPCMin );

					m_mapNamedObj.Remove( Iter->second.first );
					if( m_mapNamedObj.GetSize() <= _nMin )
						break;

					nRemoved++;
				}
			}
		}

		return nRemoved;
	}

	HRESULT RunThread()
	{
		HRESULT hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
		ATLASSERT(hr == S_OK);

		while(m_theThread.m_bWork)
		{
			DWORD dwRet = ::WaitForSingleObject(m_theThread.m_hEvent, 1000 );
			if(!m_theThread.m_bWork)
				break;

			// Do something
			GC_RemoveOld( m_nMinObjects, m_nMaxObjects, m_rtUnusedTime );
		}

		::CoUninitialize();
		return S_OK;
	}
};



/////////////////////////////////////////////////////////////////////////////
#endif	// __INDEX_MAP_H_
