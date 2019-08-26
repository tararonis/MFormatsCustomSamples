//---------------------------------------------------------------------------
// This file is part of the Medialooks Media Platform (MMP)
//
// ObjectsMap.h 
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

#include "../WinAPI/Wrappers.h"
#include "../Base/MFunctions.h"
#include "IndexMap.h"

using std::pair;


class ObjectBase
	: public AutoObjBase<ObjectBase>
{
public:
	virtual ~ObjectBase() {};
};

typedef CAutoObj<ObjectBase> TObjectBasePtr;

template <class T>
class ObjectPtr
	: public ObjectBase
{
	T*		m_pContent;
public:
	ObjectPtr( T* pContent )
		: m_pContent( pContent )
	{
	}

	virtual ~ObjectPtr()
	{
		if( m_pContent )
			delete m_pContent;
		m_pContent = 0;
	}

	T* GetContent() const { return m_pContent; }
};

template <class T>
class ObjectVal
	: public ObjectBase
{
	T	m_content;
public:
	ObjectVal( T pContent )
		: m_content( pContent )
	{
	}

	virtual ~ObjectVal()
	{
	}

	T GetContent() const { return m_content; }
};

class ObjectsMap
{
	// Protection for objects
	mutable CRWSynchroLite	m_rwObjects;
	CStrHashMap<TObjectBasePtr> m_mapObjects;

public:

	ObjectsMap( bool _bUseTLS ) // For XMLNode DO NOT use TLS !!! (too many instances)
		: m_rwObjects( RWLITE_CS_SPIN, _bUseTLS )
	{
	}

	~ObjectsMap()
	{
		ReleaseObject(NULL);
	}

	bool LockObjects( bool _bWrite, bool _bWait = true )
	{
		if( _bWrite )
			return m_rwObjects.LockWrite(_bWait);

		return m_rwObjects.LockRead(_bWait);
	}

	void UnlockObjects( bool _bWrite )
	{
		if( _bWrite )
			m_rwObjects.UnlockWrite();
		else
			m_rwObjects.UnlockRead();
	}

	//////////////////////////////////////////////////////////////////////////
	// Object impl

	template< class T >
	bool SetObjectPtr( LPCWSTR pszType, T* pT)
	{
		ATLASSERT( pszType );
		if( !pszType ) return false;

		CAutoRWL rw(m_rwObjects, true);

		int nIdx = m_mapObjects.FindKey( pszType );
		if( nIdx >= 0 )
		{
			if( pT == ((ObjectPtr<T>*)m_mapObjects[nIdx].p)->GetContent() )
				return false;

			ObjectBase* pXMLObject = new ObjectPtr<T>( pT );
			m_mapObjects[nIdx] = pXMLObject;
		}
		else
		{
			ObjectBase* pXMLObject = new ObjectPtr<T>( pT );
			m_mapObjects.Add( pszType, TObjectBasePtr(pXMLObject) );
		}

		return true;
	}

	template< class T >
	bool SetObject( LPCWSTR pszType, T pT)
	{
		ATLASSERT( pszType );
		if( !pszType ) return false;

		CAutoRWL rw(m_rwObjects, true);

		int nIdx = m_mapObjects.FindKey( pszType );
		if( nIdx >= 0 )
		{
			if( pT == ((ObjectVal<T>*)m_mapObjects[nIdx].p)->GetContent() )
				return false;


			ObjectBase* pXMLObject = new ObjectVal<T>( pT );
			m_mapObjects[nIdx] = pXMLObject;
		}
		else
		{
			ObjectBase* pXMLObject = new ObjectVal<T>( pT );
			m_mapObjects.Add( pszType, TObjectBasePtr(pXMLObject) );
		}

		return true;
	}

	bool ReleaseObject( LPCWSTR pszType )
	{
		CAutoRWL rw(m_rwObjects, true);

		if( !pszType )
		{
			m_mapObjects.RemoveAll();
			return true;
		}

		int nIdx = m_mapObjects.FindKey( pszType );
		if( nIdx >= 0 )
		{
			m_mapObjects.RemoveAt( nIdx );
			return true;
		}

		return false;
	}

	bool ReleaseObjects( const LPCWSTR* ppszRelease, const LPCWSTR* ppszKeep )
	{
		CAutoRWL rw(m_rwObjects, true);
		if( !ppszRelease && !ppszKeep )
			return ReleaseObject( NULL );

		bool bRemove = false;
		int i = 0;
		while( i < m_mapObjects.GetSize() )
		{
			if( (!ppszKeep || !MStrInList( m_mapObjects.GetKeyAt(i), ppszKeep )) ||
				(ppszRelease && MStrInList( m_mapObjects.GetKeyAt(i), ppszRelease ) ) )
			{
				//delete m_mapObjects[i];
				m_mapObjects.RemoveAt(i);
				bRemove = true;
			}
			else
			{
				i++;
			}
		}

		return bRemove;
	}

	int CopyObjects( ObjectsMap* pCopyTo, const LPCWSTR* ppszTypes )
	{
		// Collect objects
		deque<pair<CComBSTR,TObjectBasePtr> > deqCopy;
		{
			CAutoRWL rw( m_rwObjects, false );

			for( int i = 0; i < m_mapObjects.GetSize(); i++ )
			{
				if( MStrInList( m_mapObjects.GetKeyAt(i), ppszTypes ) )
				{
					deqCopy.push_back( make_pair( m_mapObjects.GetKeyAt(i), m_mapObjects.GetValueAt(i) ) );
				}
			}
		}

		CAutoRWL rw( pCopyTo->m_rwObjects, true );

		pCopyTo->m_mapObjects.RemoveAll();

		while( !deqCopy.empty() )
		{
			pCopyTo->m_mapObjects.Add( deqCopy.front().first, deqCopy.front().second );
			deqCopy.pop_front();
		}

		return pCopyTo->m_mapObjects.GetSize();
	}

	template< class T >
	T* GetObjectPtr( LPCWSTR pszType )
	{
		CAutoRWL rw(m_rwObjects, false);

		int nIdx = m_mapObjects.FindKey( pszType );
		if( nIdx >= 0 )
		{
			ObjectPtr<T>* pObjectPtr = dynamic_cast<ObjectPtr<T>*>(m_mapObjects[nIdx].p);
			if( !pObjectPtr )
				return NULL;

			return pObjectPtr->GetContent();
		}

		return NULL;
	}

	template< class T >
	T GetObject( LPCWSTR pszType )
	{
		CAutoRWL rw(m_rwObjects, false);

		int nIdx = m_mapObjects.FindKey( pszType );
		if( nIdx >= 0 )
		{
			ObjectVal<T>* pObjectPtr = dynamic_cast<ObjectVal<T>*>(m_mapObjects[nIdx].p);
			if( !pObjectPtr )
				return T();

			return pObjectPtr->GetContent();
		}

		return T();
	}

	template< class T >
	T* GetObjectPtr_New( LPCWSTR pszType, bool* _pbNewCreated = false )
	{
		if( _pbNewCreated )
			*_pbNewCreated = false;

		T* pObject = NULL;
		while( !pObject )
		{
			pObject = GetObjectPtr<T>( pszType );
			if( !pObject )
			{
				CAutoRWL rw(m_rwObjects, true);

				pObject = new T();
				ObjectPtr<T>* pObjectPtr = new ObjectPtr<T>( pObject );
				if( !m_mapObjects.Add( pszType, pObjectPtr ) )
				{
					delete pObjectPtr;
					pObject = NULL;
				}
				else 
				{
					if( _pbNewCreated )
						*_pbNewCreated = true;
				}
			}
		}

		return pObject;
	}

	template< class T >
	CAutoObj<T> GetObjectAutoObj_New( LPCWSTR pszType, bool* _pbNewCreated = false )
	{
		CAutoObj<T> pObject;
		while( !pObject )
		{
			if( _pbNewCreated )
				*_pbNewCreated = false;

			CAutoRWL rw(m_rwObjects, true);

			pObject = GetObject<CAutoObj<T> >( pszType );
			if( !pObject )
			{
				T::CreateInstance( &pObject );
				if( pObject )
				{
				 	TObjectBasePtr pObjectPtr( new ObjectVal<CAutoObj<T> >( pObject ) );
					if( !m_mapObjects.Add( pszType, pObjectPtr ) )
					{
						ATLASSERT( !"GetObjectAutoObj_New");
						pObject.Release();
					}
					else 
					{
						if( _pbNewCreated )
							*_pbNewCreated = true;
					}
				}
			}
		}

		return pObject;
	}
};

template <class TKey, class TVal>
class CAutoObjMap
{
	typedef CAutoObj<TVal> TValPtr;
	CRWSynchroLite		m_rwMap;
	typedef map<TKey,TValPtr> TMapObjects;
	TMapObjects			m_mapObjects;


public:

	CAutoObjMap()
	{
	}

	virtual ~CAutoObjMap()
	{
		MapClear();
	}

	TValPtr MapGet( TKey _key )
	{
		{
			CAutoRWL rw(m_rwMap,false);

			TMapObjects::iterator Iter = m_mapObjects.find( _key );
			if( Iter != m_mapObjects.end() )
				return Iter->second;
		}

		TValPtr pObject;
		HRESULT hr = TVal::CreateInstance( &pObject );
		if( FAILED( hr ) )
			return pObject;

		CAutoRWL rw(m_rwMap,true);

		TMapObjects::iterator Iter = m_mapObjects.insert( make_pair(_key, pObject) ).first;
		return Iter->second;
	}

	bool MapRemove( TKey _key )
	{
		CAutoRWL rw(m_rwMap, true);

		return m_mapObjects.erase(_key);
	}

	void MapClear()
	{
		CAutoRWL rw(m_rwMap, true);

		m_mapObjects.clear();
	}
};