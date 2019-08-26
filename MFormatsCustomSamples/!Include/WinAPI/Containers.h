//---------------------------------------------------------------------------
// This file is part of the MediaLooks Media Platform (MMP)
//
// Containers.h : corrected container classes from ATL, container class for
//				  pointers, some container classes for strings
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

#ifndef __CONTAINERS_H_
#define __CONTAINERS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <limits.h>

/////////////////////////////////////////////////////////////////////////////
template <class T> class CSimpleArray2;
template <class TKey, class TVal, bool bTUniqueKeys = true> class CSimpleMap2;
template <class T> class CInterfaceArray;
template <class T> class CSimpleRing;
template <class T> class CSimpleList;

class CStrBlock;
class CStringBuf;

/////////////////////////////////////////////////////////////////////////////
// CSimpleArray2
template <class T>
class CSimpleArray2
	{
public:
	T* m_aT;
	int m_nSize;
	int m_nAllocSize;

	//---------------------------------
	CSimpleArray2()
	{
		m_aT = NULL;
		m_nSize = 0;
		m_nAllocSize = 0;
	}

	CSimpleArray2(const CSimpleArray2<T>& src)
	{
		m_aT = NULL;
		m_nSize = 0;
		m_nAllocSize = 0;

		if(src.GetSize() > 0 && Alloc(src.GetSize()))
        {
			for(int i = 0; i < src.GetSize(); ++i)
				Add(src[i]);
		}
	}
	CSimpleArray2<T>& operator=(const CSimpleArray2<T>& src)
	{
		if(this == &src)
			return *this;

		if(GetSize() != src.GetSize())
		{
			RemoveAll();

			if(!Alloc(src.GetSize()))
				return *this;
		}
		else
		{
			for(int i = GetSize(); i > 0; --i)
				RemoveAt(i - 1);
		}

		for(int i = 0; i < src.GetSize(); ++i)
			Add(src[i]);

		return *this;
	}

	~CSimpleArray2()
	{
		RemoveAll();
	}

	//---------------------------------
	int GetSize() const
	{
		return m_nSize;
	}

	//---------------------------------
	BOOL Add(const T& t)
	{
		// Increment size
		if(m_nSize == m_nAllocSize)
		{
			// Make sure newElement is not a reference to an element in the array.
			// Or else, it will be invalidated by the reallocation.
			ATLASSERT(&t < m_aT || &t >= m_aT + m_nAllocSize);
			if(!Realloc())
				return FALSE;
		}

		SetAtIndex(m_nSize, t);
		++m_nSize;
		return TRUE;
	}

	BOOL InsertAt(int nIndex, const T& t)
	{
		// If nIndex == m_nSize: one new element can be added after end of array
		ATLASSERT(0 <= nIndex && nIndex <= m_nSize);
		if(nIndex < 0 || m_nSize < nIndex)
			return FALSE;

		// Increment size
		if(m_nSize == m_nAllocSize)
		{
			// Make sure newElement is not a reference to an element in the array.
			// Or else, it will be invalidated by the reallocation.
			ATLASSERT(&t < m_aT || &t >= m_aT + m_nAllocSize);
			if(!Realloc())
				return FALSE;
		}

		// Move higher elements up
		if(nIndex < m_nSize)
		{
			// memmove() correctly copyes overlapped buffers instead of memcpy()
			memmove((void*)(m_aT + nIndex + 1), (void*)(m_aT + nIndex), (m_nSize - nIndex) * sizeof(T));
		}

		SetAtIndex(nIndex, t);
		++m_nSize;
		return TRUE;
	}
	
	// SetAt(int nIndex, const T& t) is the same as arr[nIndex] = t
	// if T& operator=(T&) is defined for T
	BOOL SetAt(int nIndex, const T& t)
	{
		ATLASSERT(0 <= nIndex && nIndex < m_nSize);
		if(nIndex < 0 || m_nSize <= nIndex)
			return FALSE;

		m_aT[nIndex].~T();

		SetAtIndex(nIndex, t);
		return TRUE;
	}

	//---------------------------------
	BOOL Remove(const T& t)
	{
		int nIndex = Find(t);
		return (nIndex != -1)? RemoveAt(nIndex) : FALSE;
	}
	BOOL RemoveAt(int nIndex)
	{
		ATLASSERT(0 <= nIndex && nIndex < m_nSize);
		if(nIndex < 0 || m_nSize <= nIndex)
			return FALSE;

		m_aT[nIndex].~T();

		if(nIndex != (m_nSize - 1))
			memmove((void*)(m_aT + nIndex), (void*)(m_aT + nIndex + 1), (m_nSize - (nIndex + 1)) * sizeof(T));

		--m_nSize;
		return TRUE;
	}
	void RemoveAll()
	{
		if(m_aT != NULL)
		{
			for(int i = 0; i < m_nSize; ++i)
				m_aT[i].~T();

			free(m_aT);
			m_aT = NULL;
		}
		m_nSize = 0;
		m_nAllocSize = 0;
	}

	//---------------------------------
	const T& operator[] (int nIndex) const
	{
		ATLASSERT(0 <= nIndex && nIndex < m_nSize);
		return m_aT[nIndex];
	}
	T& operator[] (int nIndex)
	{
		ATLASSERT(0 <= nIndex && nIndex < m_nSize);
		return m_aT[nIndex];
	}
	T* GetData() const
	{
		return m_aT;
	}

	int Find(const T& t) const
	{
		for(int i = 0; i < m_nSize; ++i)
		{
			if(m_aT[i] == t)
				return i;
		}
		return -1;
	}

	//---------------------------------
private:

	class Wrapper
	{
	public:
		Wrapper(const T& _t) : t(_t) {}
		template <class _Ty>
		void *operator new(size_t, _Ty* p)
		{
			return p;
		}
		template <class _Ty>
		void operator delete(void*, _Ty*)
		{
			ATLASSERT(!_T("CSimpleArray2::Wrapper(): operator delete"));
		}
		T t;
	};

	// This function don't call destructor for old object and hidden from usage
	void SetAtIndex(int nIndex, const T& t)
	{
		new( m_aT + nIndex ) Wrapper(t);
	}

	//---------------------------------
	bool Alloc(int nSize)
	{
		ATLASSERT(m_aT == NULL);

		if(nSize == 0)
			return false;

		m_aT = (T*)malloc(nSize * sizeof(T));
		if(m_aT == NULL)
			return false;

		m_nAllocSize = nSize;
		return true;
	}

	bool Realloc()
	{
		int nNewAllocSize = (m_nAllocSize == 0) ? 1 : (2 * m_nSize);
		if(nNewAllocSize < 0 || nNewAllocSize > INT_MAX/sizeof(T))
			return false;

		T* aT = (T*)realloc(m_aT, nNewAllocSize * sizeof(T));
		if(aT == NULL)
			return false;

		m_nAllocSize = nNewAllocSize;
		m_aT = aT;
		return true;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CSimpleMap2
template <class TKey, class TVal, bool bTUniqueKeys /*= true*/>
class CSimpleMap2
{
public:
	TKey*	m_aKey;
	TVal*	m_aVal;
	int		m_nSize;
	int		m_nAllocSize;

	CSimpleMap2()
	{
		m_aKey = NULL;
		m_aVal = NULL;
		m_nSize = 0;
		m_nAllocSize = 0;
	}
	CSimpleMap2(const CSimpleMap2<TKey, TVal>& src)
	{
		m_aKey = NULL;
		m_aVal = NULL;
		m_nSize = 0;
		m_nAllocSize = 0;

		if(src.GetSize() > 0 && Alloc(src.GetSize()))
        {
			for(int i = 0; i < src.GetSize(); ++i)
				Add(src.GetKeyAt(i), src[i]);
		}
	}
	CSimpleMap2<TKey, TVal>& operator=(const CSimpleMap2<TKey, TVal>& src)
	{
		if(this != &src)
		{
			if(GetSize() != src.GetSize())
			{
				RemoveAll();

				if(!Alloc(src.GetSize()))
					return *this;
			}
			else
			{
				for(int i = GetSize(); i > 0; --i)
					RemoveAt(i - 1);
			}

			for(int i = 0; i < src.GetSize(); ++i)
				Add(src.GetKeyAt(i), src[i]);
		}
		return *this;
	}

	~CSimpleMap2()
	{
		RemoveAll();
	}

	//---------------------------------
	int GetSize() const
	{
		return m_nSize;
	}

	int AddMap(const CSimpleMap2<TKey, TVal>& src)
	{
		int nAdded = 0; // Used if bTUniqueKeys is true
		for (int i = 0; i < src.GetSize(); ++i)
		{
			if (Add(src.GetKeyAt(i), src[i]))
				nAdded++;
		}

		return nAdded;
	}

	//---------------------------------
	// Addition of existing key is forbidden
	BOOL Add(const TKey& key, const TVal& val)
	{
		if(bTUniqueKeys && FindKey(key) != -1)
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

		if(bTUniqueKeys && FindKey(key) != -1)
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
		return TRUE;
	}

	// Setting of existing key (with exception of nIndex key) is forbidden
	BOOL SetAtIndex(int nIndex, const TKey& key, const TVal& val)
	{
		ATLASSERT(0 <= nIndex && nIndex < m_nSize);
		if(nIndex < 0 || m_nSize <= nIndex)
			return FALSE;

		if(bTUniqueKeys)
		{
			int n = FindKey(key);
			if(n != -1 && n != nIndex)
				return FALSE;
		}

		m_aKey[nIndex].~TKey();
        m_aVal[nIndex].~TVal();
		InternalSetAtIndex(nIndex, key, val);
		return TRUE;
	}

	BOOL SetAt(const TKey& key, const TVal& val)
	{
		int nIndex = FindKey(key);
		if(nIndex == -1)
			return FALSE;

		m_aKey[nIndex].~TKey();
        m_aVal[nIndex].~TVal();
		InternalSetAtIndex(nIndex, key, val);
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

		m_nSize = 0;
		m_nAllocSize = 0;
	}

	//---------------------------------
	TKey& GetKeyAt(int nIndex) const
	{
		ATLASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_aKey[nIndex];
	}
	TVal& GetValueAt(int nIndex) const
	{
		ATLASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_aVal[nIndex];
	}

	TKey& LastKey() const
	{
		ATLASSERT(m_nSize > 0 );
		return m_aKey[m_nSize - 1];
	}
	TVal& LastValue() const
	{
		ATLASSERT(m_nSize > 0 );
		return m_aVal[m_nSize - 1];
	}

	const TVal& operator[] (int nIndex) const
	{
		ATLASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_aVal[nIndex];
	}
	TVal& operator[] (int nIndex)
	{
		ATLASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_aVal[nIndex];
	}

	int FindKey(const TKey& key) const
	{
		for(int i = 0; i < m_nSize; i++)
		{
			if(m_aKey[i] == key)
				return i;
		}
		return -1;  // not found
	}
	int FindVal(const TVal& val) const
	{
		for(int i = 0; i < m_nSize; i++)
		{
			if(m_aVal[i] == val)
				return i;
		}
		return -1;  // not found
	}

	//---------------------------------
protected:

	template <typename T>
	class Wrapper
	{
	public:
		Wrapper(const T& _t) : t(_t)
		{
		}
		template <typename _Ty>
		void *operator new(size_t, _Ty* p)
		{
			return p;
		}
		template <class _Ty>
		void operator delete(void*, _Ty*)
		{
			ATLASSERT(!_T("CSimpleMap2::Wrapper(): operator delete"));
		}
		T t;
	};

	void InternalSetAtIndex(int nIndex, const TKey& key, const TVal& val)
	{
		new( m_aKey + nIndex ) Wrapper<TKey>(key);
		new( m_aVal + nIndex ) Wrapper<TVal>(val);
	}

	//---------------------------------
	bool Alloc(int nSize)
	{
		ATLASSERT(m_aKey == NULL && m_aVal == NULL);

		if(nSize == 0)
			return false;

		m_aKey = (TKey*)malloc(nSize * sizeof(TKey));
		if(m_aKey == NULL)
			return false;

		m_aVal = (TVal*)malloc(nSize * sizeof(TVal));
		if(m_aVal == NULL)
		{
			free(m_aKey);
			m_aKey = NULL;
			return false;
		}

		m_nAllocSize = nSize;
		return true;
	}

	bool Realloc()
	{
		int nNewAllocSize = (m_nAllocSize == 0) ? 1 : (2 * m_nSize);
		if( nNewAllocSize < 0 || nNewAllocSize > INT_MAX/sizeof(TKey) ||
			nNewAllocSize > INT_MAX/sizeof(TVal))
			return false;

		TKey* aKey = (TKey*)realloc(m_aKey, nNewAllocSize * sizeof(TKey));
		if(aKey == NULL)
			return false;
		m_aKey = aKey;

		TVal* aVal = (TVal*)realloc(m_aVal, nNewAllocSize * sizeof(TVal));
		if(aVal == NULL)
			return false;
		m_aVal = aVal;

		m_nAllocSize = nNewAllocSize;
		return true;
	}
};


/////////////////////////////////////////////////////////////////////////////
// Container class for interface pointers.
// CSimpleArray and CComDynamicUnkArray have some problems - coding new class

template <class T>
class CInterfaceArray
{
public:
	T** m_aT;
	int m_nSize;
	int m_nAllocSize;

	//---------------------------------
	CInterfaceArray()
	{
		m_aT = NULL;
		m_nSize = 0;
		m_nAllocSize = 0;
	}

	~CInterfaceArray()
	{
		RemoveAll();
	}

	//---------------------------------
	int GetSize() const
	{
		return m_nSize;
	}

	BOOL Add(T* pT)
	{
		if(m_nSize == m_nAllocSize)
		{
			int nNewAllocSize = (m_nAllocSize == 0) ? 1 : (m_nSize * 2);
			T** aT = (T**)realloc(m_aT, nNewAllocSize * sizeof(T*));
			if(aT == NULL)
				return FALSE;

			m_nAllocSize = nNewAllocSize;
			m_aT = aT;
		}

		m_aT[m_nSize++] = pT;
		if(pT != NULL)
			pT->AddRef();
		return TRUE;
	}
	BOOL Add(CInterfaceArray<T>& array)
	{
		for(int i = 0; i < array.GetSize(); ++i)
		{
			if(!Add(array[i]))
				return FALSE;
		}
		return TRUE;
	}
	bool Remove(T* pT)
	{
		int i = FindInArray(pT);
		if(i == -1)
			return false;

		RemoveAt(i);
		return true;
	}
	void RemoveAt(int nIndex)
	{
		ATLASSERT(nIndex < m_nSize);

		if(m_aT[nIndex])
			m_aT[nIndex]->Release();
		if(nIndex != --m_nSize)
			memmove((void*)(m_aT + nIndex), (void*)(m_aT + nIndex + 1), (m_nSize - nIndex) * sizeof(T*));
	}
	void RemoveAll()
	{
		if(m_aT != NULL)
		{
			for(int i = 0; i < m_nSize; i++)
			{
				if(m_aT[i]) m_aT[i]->Release();
			}
			free(m_aT);
			m_aT = NULL;
		}
		m_nSize = 0;
		m_nAllocSize = 0;
	}
	T* operator[] (int nIndex) const
	{
		ATLASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_aT[nIndex];
	}
	bool IsInArray(T* pT) const
	{
		int i = FindInArray(pT);
		return (i != -1);
	}
	int FindInArray(T* pT) const
	{
		ATLASSERT(pT != NULL);
		if(pT == NULL) return -1;

		CComPtr<IUnknown> pUnk1;
		pT->QueryInterface(IID_IUnknown, (void**)&pUnk1);
		for(int i = 0; i < m_nSize; ++i)
		{
			ATLASSERT(m_aT[i] != NULL);

			CComPtr<IUnknown> pUnk2;
			m_aT[i]->QueryInterface(IID_IUnknown, (void**)&pUnk2);
			if(pUnk1 == pUnk2) return i;
		}
		return -1;
	}
};

/////////////////////////////////////////////////////////////////////////////
// Ring container

template <class T>
class CSimpleRing
{
public:
	T*			m_aT;		// Ring buffer
	DWORD		m_nSize;	// Allocated size of buffer (in T objects)
	DWORD		m_nFirst;	// Position of oldest data
	DWORD		m_nNew;		// Position for put new data
	ULONGLONG	m_ulTotal;	// Total quantity of the data put in ring

	//---------------------------------
	CSimpleRing()
	{
		m_aT = NULL;
		m_nSize = 0;
		m_nFirst = (DWORD)-1;
		m_nNew = 0;
		m_ulTotal = (ULONGLONG)0;
	}
	~CSimpleRing()
	{
		RemoveAll();
		free(m_aT);
	}
	bool Init(DWORD nSize)
	{
		ATLASSERT(nSize > 0);

		Clear();
		free(m_aT);
		m_nSize = 0;

		m_aT = (T*)malloc(nSize * sizeof(T));
		if(m_aT == NULL)
			return false;

		m_nSize = nSize;
		return true;
	}

	// nExtend : number of additional items
	bool Extend(DWORD nExtend)
	{
		if(nExtend == 0)
			return true;
		if(m_nSize == 0)
			return Init(nExtend);

		T* p = (T*)malloc((nExtend + m_nSize) * sizeof(T));
		if(p == NULL)
			return false;

		if(!IsEmpty())
		{
			if(m_nFirst < m_nNew)
			{
				m_nNew -= m_nFirst;
				memcpy(p, m_aT + m_nFirst, m_nNew * sizeof(T));
			}
			else
			{
				DWORD n = m_nSize - m_nFirst;
				memcpy(p, m_aT + m_nFirst, n * sizeof(T));
				if(m_nNew != 0)
					memcpy(p + n, m_aT, m_nNew * sizeof(T));
				m_nNew += n;
			}

			m_nFirst = 0;
		}

		free(m_aT);
		m_aT = p;
		m_nSize += nExtend;
		return true;
	}
	DWORD GetCapacity() const
	{
		return m_nSize;
	}
	DWORD GetSize() const
	{
		DWORD n = 0;
		if(!IsEmpty())
		{
			n = m_nNew - m_nFirst;
			if(m_nNew <= m_nFirst)
				n += m_nSize;
		}
		return n;
	}
	bool IsEmpty() const
	{
		return m_nFirst == (DWORD)-1;
	}
	bool IsFull() const
	{
		return m_nNew == m_nFirst;
	}
	ULONGLONG Start() const
	{
		return m_ulTotal - (ULONGLONG)GetSize();
	}
	ULONGLONG Total() const
	{
		return m_ulTotal;
	}
	bool Add(const T& t)
	{
		if(IsEmpty())
		{
			if(!Init(1))
				return false;
		}
		else if(IsFull())
		{
			if(!Extend(m_nSize))
				return false;
		}

		Put(t);
		return true;
	}
	void Put(const T& t)
	{
		ATLASSERT(m_aT != NULL);

		SetAtIndex(m_nNew, t);

		if(IsEmpty() || (IsFull() && ++m_nFirst == m_nSize))
			m_nFirst = 0;
		if(++m_nNew == m_nSize)
			m_nNew = 0;
		++m_ulTotal;
	}

	// ATTENTION. Method not tested yet !
	//
	// Insert new element at position nIndex from current ring start.
	// All ring elements after insertion position shifted.
	// If IsFull() && bExtend = true: ring will be extended for put new element
	bool InsertAt(DWORD nIndex, const T& t, bool bExtend = false)
	{
		DWORD nBusy = GetSize();
		ATLASSERT(nIndex <= nBusy);
		if(nBusy < nIndex) return false;

		ATLASSERT(bExtend || !IsFull());
		if(!bExtend && IsFull()) return false;

		//---------------------------------
		if(IsEmpty())
			return Add(t);

		if(IsFull())
		{
			T* p = (T*)malloc((2 * m_nSize) * sizeof(T));
			if(p == NULL)
				return false;

			DWORD nFirstPartSize = m_nSize - m_nFirst;
			if(nIndex < nFirstPartSize)
			{
				if(nIndex > 0)
					memcpy(p, m_aT + m_nFirst, nIndex * sizeof(T));

				memcpy(p + nIndex + 1, m_aT + m_nFirst + nIndex, (nFirstPartSize - nIndex) * sizeof(T));

				if(m_nNew > 0)
					memcpy(p + nFirstPartSize + 1, m_aT, m_nNew * sizeof(T));
			}
			else
			{
				memcpy(p, m_aT + m_nFirst, nFirstPartSize * sizeof(T));

				DWORD n = nIndex - nFirstPartSize;
				ATLASSERT(n <= m_nNew);

				if(nIndex > 0)
					memcpy(p + nFirstPartSize, m_aT, n * sizeof(T));

				if(nIndex < m_nNew)
					memcpy(p + nFirstPartSize + n + 1, m_aT + n, (m_nNew - n) * sizeof(T));
			}

			// nIndex is right place for put new item

			m_nFirst = 0;
			m_nNew = m_nSize + 1;
			free(m_aT);
			m_aT = p;
			m_nSize *= 2;
		}
		else if(nIndex < nBusy)
		{
			DWORD nFirstPartSize = (m_nFirst < m_nNew)? m_nNew - m_nFirst : m_nSize - m_nFirst;
			if(nIndex < nFirstPartSize)
			{
				if(nIndex > 0)
				{
					if(m_nFirst > 0)
					{
						memmove(m_aT + m_nFirst - 1, m_aT + m_nFirst, nIndex * sizeof(T));
					}
					else
					{
						memmove(m_aT + m_nSize - 1, m_aT, sizeof(T));
						if(nIndex > 1)
							memmove(m_aT, m_aT + 1, (nIndex - 1) * sizeof(T));
					}
				}
				
				nIndex += m_nFirst;

				if(m_nFirst == 0)
					m_nFirst = m_nSize;
				--m_nFirst;
			}
			else
			{
				nIndex -= nFirstPartSize;
				ATLASSERT(nIndex < m_nNew);

				memmove(m_aT + nIndex + 1, m_aT + nIndex, (m_nNew - nIndex) * sizeof(T));
				++m_nNew;
			}
		}

		SetAtIndex(nIndex, t);
		return true;
	}
	T& Get() const
	{
		ATLASSERT(!IsEmpty());
		return m_aT[m_nFirst];
	}
	T& First() const
	{
		ATLASSERT(!IsEmpty());
		return m_aT[m_nFirst];
	}
	T& Last() const
	{
		ATLASSERT(!IsEmpty());
		return m_aT[(m_nNew > 0)? m_nNew - 1 : m_nSize - 1];
	}
	T& operator[] (DWORD nIndex) const
	{
		ATLASSERT(nIndex >= 0 && nIndex < GetSize());
		nIndex += m_nFirst;
		if(nIndex >= m_nSize)
			nIndex -= m_nSize;
		return m_aT[nIndex];
	}
	bool RemoveFirst()
	{
		if(IsEmpty())
			return false;

		m_aT[m_nFirst].~T();

		if(++m_nFirst == m_nSize)
			m_nFirst = 0;
		if(m_nFirst == m_nNew)
		{
			m_nFirst = (DWORD)-1;
			m_nNew = 0;
		}
		return true;
	}
	bool RemoveLast()
	{
		if(IsEmpty())
			return false;

		DWORD nLast = (m_nNew > 0)? m_nNew - 1 : m_nSize - 1;
		m_aT[nLast].~T();

		m_nNew = nLast;

		if(m_nNew == m_nFirst)
		{
			m_nFirst = (DWORD)-1;
			m_nNew = 0;
		}
		return true;
	}
	void RemoveAll()
	{
		while(RemoveFirst());
	}
	void Clear()
	{
		RemoveAll();
		m_ulTotal = (ULONGLONG)0;
	}

	//---------------------------------
private:
	class Wrapper
	{
	public:
		Wrapper(const T& _t) : t(_t) {}
		template <class _Ty> void *operator new(size_t, _Ty* p) { return p; }
		T t;
	};
	void SetAtIndex(DWORD nIndex, const T& t)
	{
		ATLASSERT(nIndex < m_nSize);
		new(m_aT + nIndex) Wrapper(t);
	}
};

/////////////////////////////////////////////////////////////////////////////
// 1-connected list
// TODO: insert Wrapper class for new(size_t, _Ty* p)

template <class T>
class CSimpleList
{
	struct TWrap
	{
		TWrap()
		{
			pNext = NULL;
		}

		T	elem;
		T*	pNext;
	};

	// Start of busy elements list
	T*		m_pFirst;
	T*		m_pLast;
	// Start of free elements list
	TWrap*	m_pFree;
	// Number of busy elements
	int		m_nSize;
	// Number of allocated elements
	int		m_nAllocSize;

	//---------------------------------
public:

	CSimpleList()
	{
		m_pFirst = m_pLast = NULL;
		m_pFree = NULL;
		m_nSize = m_nAllocSize = 0;
	}

	~CSimpleList()
	{
		Clear();
	}

	HRESULT Init(int nElements = 16)
	{
		Clear();
		return (Extend(nElements))? S_OK : E_OUTOFMEMORY;
	}

	void Clear()
	{
		while(m_pFirst != NULL)
		{
			ATLASSERT(m_pFirst != m_pLast || m_nSize == 1);

			TWrap* pTemp = (TWrap*)m_pFirst;
			m_pFirst = pTemp->pNext;
			delete pTemp;

			--m_nAllocSize;
			ATLASSERT(m_nAllocSize >= 0);
			--m_nSize;
			ATLASSERT(m_nSize >= 0);
		}
		ATLASSERT(m_nSize == 0);
		m_pLast = NULL;

		Squeeze();

		ATLASSERT(m_nAllocSize == 0);
	}

	// Delete all free elements
	void Squeeze()
	{
		while(m_pFree != NULL)
		{
			TWrap* pTemp = m_pFree;
			m_pFree = (TWrap*)pTemp->pNext;
			delete pTemp;

			--m_nAllocSize;
			ATLASSERT(m_nAllocSize >= 0);
		}
	}

	//---------------------------------
	int GetSize() const
	{
		return m_nSize;
	}

	// Add after last element
	BOOL Add(const T& t)
	{
		TWrap* pNew = GetElement();
		if(pNew == NULL)
			return FALSE;

		pNew->elem = t;

		if(m_pFirst == NULL)
			m_pFirst = (T*)pNew;
		else
			((TWrap*)m_pLast)->pNext = (T*)pNew;
		m_pLast = (T*)pNew;
		return TRUE;
	}

	// Insert after pPrev element
	// If pPrev = NULL: insert before first element
	// For insert after end: InsertAfter(t, Last());
	BOOL InsertAfter(const T& t, T* pPrev = NULL)
	{
		TWrap* pNew = GetElement();
		if(pNew == NULL)
			return FALSE;

		pNew->elem = t;

		//---------------------------------
		if(pPrev == NULL)
		{
			pNew->pNext = m_pFirst;
			m_pFirst = (T*)pNew;
			if(m_pLast == NULL)
				m_pLast = (T*)pNew;
		}
		else
		{
			pNew->pNext = ((TWrap*)pPrev)->pNext;
			((TWrap*)pPrev)->pNext = (T*)pNew;

			if(m_pLast == pPrev)
			{
				ATLASSERT(pNew->pNext == NULL);
				m_pLast = (T*)pNew;
			}
			else
				ATLASSERT(pNew->pNext != NULL);
		}
		return TRUE;
	}

	BOOL Remove(T* pElement)
	{
		ATLASSERT(pElement != NULL);
		if(pElement == NULL) return FALSE;

		TWrap* p = (TWrap*)pElement;
		if(pElement == m_pFirst)
		{
			m_pFirst = p->pNext;
			if(m_pLast == pElement)
				m_pLast = m_pFirst;
		}
		else
		{
			TWrap* pPrev = (TWrap*)m_pFirst;
			while(pPrev->pNext != pElement)
			{
				pPrev = (TWrap*)pPrev->pNext;
				ATLASSERT(pPrev != NULL);
				if(pPrev == NULL) return FALSE;
			}

			pPrev->pNext = p->pNext;
			if(m_pLast == pElement)
			{
				ATLASSERT(pPrev->pNext == NULL);
				m_pLast = (T*)pPrev;
			}
			else
				ATLASSERT(pPrev->pNext != NULL);
		}

		// For free resources in T object
		p->elem = T();

		--m_nSize;
		PutToFree(p);
		return TRUE;
	}

	//---------------------------------
	T* First() const
	{
		return m_pFirst;
	}
	T* Last() const
	{
		return m_pLast;
	}
	T* Next(const T* pCurr) const
	{
		TWrap* p = (TWrap*)pCurr;
		return (p != NULL)? (T*)p->pNext : NULL;
	}

	//---------------------------------
private:

	TWrap* GetElement()
	{
		if(m_pFree == NULL)
		{
			ATLASSERT(m_nSize == m_nAllocSize);
			if(!Extend())
				return NULL;
		}

		TWrap* pNew = m_pFree;
		m_pFree = (TWrap*)pNew->pNext;
		pNew->pNext = NULL;
		++m_nSize;
		return pNew;
	}
	bool Extend(int nNewElements = 0)
	{
		if(nNewElements <= 0)
			nNewElements = (m_nAllocSize > 0)? m_nAllocSize : 1;

		TWrap* p = NULL;
		for(int n = 0; n < nNewElements; ++n)
		{
			ATLTRY(p = new TWrap());
			if(p == NULL)
				return false;

			PutToFree(p);
			++m_nAllocSize;
		}
		return true;
	}
	void PutToFree(TWrap* p)
	{
		ATLASSERT(p != NULL);
		p->pNext = (T*)m_pFree;
		m_pFree = p;
	}
};

/////////////////////////////////////////////////////////////////////////////
// 2-connected list on initial reserved memory

template <class T>
class CDoubleList
{
	struct TWrap
	{
		TWrap()
		{
			pPrev = pNext = NULL;
		}

		// Set elem as first member for execute InsertAfter(T* pTPrev, const T& t)
		// In this case TWrap* pPrev = (TWrap*)pTPrev is correct
		T	elem;
		TWrap*	pPrev;
		TWrap*	pNext;
	};

	// Start of busy elements list
	// m_pFirst->pNext->...->pNext = m_pLast, m_pLast->pNext = NULL
	// m_pLast->pPrev->...->pPrev = m_pFirst, m_pFirst->pPrev = NULL
	TWrap*	m_pFirst;
	TWrap*	m_pLast;
	// Start of free elements single list (only TWrap::pNext is used as pointer to next element)
	TWrap*	m_pFree;
	// Number of busy elements
	int		m_nSize;
	// Number of allocated elements
	int		m_nAllocSize;

	//---------------------------------
public:

	CDoubleList()
	{
		m_pFirst = m_pLast = m_pFree = NULL;
		m_nSize = m_nAllocSize = 0;
	}

	~CDoubleList()
	{
		Delete();
	}

	bool Init(int nElements = 32)
	{
		Delete();
		return Extend(nElements);
	}

	void Delete()
	{
		// Delete all used elements
		while(m_pFirst != NULL)
		{
			ATLASSERT(m_pFirst != m_pLast || m_nSize == 1);

			TWrap* pTemp = m_pFirst;
			m_pFirst = pTemp->pNext;
			delete pTemp;

			--m_nAllocSize;
			ATLASSERT(m_nAllocSize >= 0);
			--m_nSize;
			ATLASSERT(m_nSize >= 0);
		}
		ATLASSERT(m_nSize == 0);
		m_pLast = NULL;

		// Delete all free elements
		while(m_pFree != NULL)
		{
			TWrap* pTemp = m_pFree;
			m_pFree = pTemp->pNext;
			delete pTemp;

			--m_nAllocSize;
			ATLASSERT(m_nAllocSize >= 0);
		}

		ATLASSERT(m_nAllocSize == 0);
	}

	// Move all busy elements to list of free
	void Clear()
	{
		while(m_pFirst != NULL)
		{
			ATLASSERT(m_pFirst != m_pLast || m_nSize == 1);

			TWrap* pTemp = m_pFirst;
			m_pFirst = pTemp->pNext;

			// For free resources in T object
			pTemp->elem = T();

			--m_nSize;
			ATLASSERT(m_nSize >= 0);

			PutToFree(pTemp);
		}
		ATLASSERT(m_nSize == 0);
		m_pLast = NULL;
	}

	//---------------------------------
	bool IsEmpty() const
	{
		return m_nSize == 0;
	}
	bool IsFull() const
	{
		return m_nSize == m_nAllocSize;
	}
	int GetSize() const
	{
		return m_nSize;
	}

	bool AddFirst(const T& t)
	{
		TWrap* pNew = GetElement();
		if(pNew == NULL)
			return false;

		pNew->elem = t;

		if(m_pFirst == NULL)
		{
			ATLASSERT(m_pLast == NULL);
			m_pFirst = m_pLast = pNew;
		}
		else
		{
			pNew->pNext = m_pFirst;
			m_pFirst->pPrev = pNew;
			m_pFirst = pNew;
		}
		return true;
	}

	bool AddLast(const T& t)
	{
		TWrap* pNew = GetElement();
		if(pNew == NULL)
			return false;

		pNew->elem = t;

		if(m_pLast == NULL)
		{
			ATLASSERT(m_pFirst == NULL);
			m_pFirst = m_pLast = pNew;
		}
		else
		{
			m_pLast->pNext = pNew;
			pNew->pPrev = m_pLast;
			m_pLast = pNew;
		}
		return true;
	}

	// Insert after pPrev element (pPrev->pNext = pNew)
	// If pPrev = NULL: AddFirst() is used
	bool InsertAfter(T* pTPrev, const T& t)
	{
		if(pTPrev == NULL)
			return AddFirst(t);

		//---------------------------------
		TWrap* pNew = GetElement();
		if(pNew == NULL)
			return FALSE;

		pNew->elem = t;

		TWrap* pPrev = (TWrap*)pTPrev;
		pNew->pPrev = pPrev;
		pNew->pNext = pPrev->pNext;

		if(pPrev->pNext == NULL)
		{
			ATLASSERT(m_pLast == pPrev);
			m_pLast = pNew;
		}
		else
		{
			ATLASSERT(pPrev->pNext->pPrev == pPrev);
			pPrev->pNext->pPrev = pNew;
		}

		pPrev->pNext = pNew;
		return true;
	}

	// Insert before pNext element (pNext->pPrev = pNew)
	// If pNext = NULL: AddTLast() is used
	bool InsertBefore(T* pTNext, const T& t)
	{
		if(pTNext == NULL)
			return AddLast(t);

		//---------------------------------
		TWrap* pNew = GetElement();
		if(pNew == NULL)
			return FALSE;

		pNew->elem = t;

		TWrap* pNext = (TWrap*)pTNext;
		pNew->pPrev = pNext->pPrev;
		pNew->pNext = pNext;

		if(pNext->pPrev == NULL)
		{
			ATLASSERT(m_pFirst == pNext);
			m_pFirst = pNew;
		}
		else
		{
			ATLASSERT(pNext->pPrev->pNext == pNext);
			pNext->pPrev->pNext = pNew;
		}

		pNext->pPrev = pNew;
		return true;
	}

	void Exchange(T* pT1, T* pT2)
	{
		if(pT1 == NULL || pT2 == NULL || pT1 == pT2)
			return;

		TWrap* p1 = (TWrap*)pT1;
		TWrap* p1Prev = p1->pPrev;
		TWrap* p1Next = p1->pNext;

		TWrap* p2 = (TWrap*)pT2;
		TWrap* p2Prev = p2->pPrev;
		TWrap* p2Next = p2->pNext;

		if(p1Next == p2)
		{
			ATLASSERT(p2Prev == p1);
			p1Next = p1;
			p2Prev = p2;
		}
		if(p2Next == p1)
		{
			ATLASSERT(p1Prev == p2);
			p2Next = p2;
			p1Prev = p1;
		}

		//---------------------------------
		p1->pPrev = p2Prev;
		if(p2Prev == NULL)
		{
			ATLASSERT(p2 == m_pFirst);
			m_pFirst = p1;
		}
		else
			p2Prev->pNext = p1;

		p1->pNext = p2Next;
		if(p2Next == NULL)
		{
			ATLASSERT(p2 == m_pLast);
			m_pLast = p1;
		}
		else
			p2Next->pPrev = p1;

		p2->pPrev = p1Prev;
		if(p1Prev == NULL)
		{
			ATLASSERT(p1 == m_pFirst);
			m_pFirst = p2;
		}
		else
			p1Prev->pNext = p2;

		p2->pNext = p1Next;
		if(p1Next == NULL)
		{
			ATLASSERT(p1 == m_pLast);
			m_pLast = p2;
		}
		else
			p1Next->pPrev = p2;
	}

	bool Remove(T* pTElem)
	{
		ATLASSERT(pTElem != NULL);
		if(pTElem == NULL) return false;

		TWrap* pElem = (TWrap*)pTElem;
		if(pElem == m_pFirst)
		{
			ATLASSERT(pElem->pPrev == NULL);

			if(pElem == m_pLast)
			{
				ATLASSERT(pElem->pNext == NULL);
				m_pFirst = m_pLast = NULL;
			}
			else
			{
				ATLASSERT(pElem->pNext != NULL);
				ATLASSERT(pElem->pNext->pPrev == pElem);
				m_pFirst = pElem->pNext;
				m_pFirst->pPrev = NULL;
			}
		}
		else
		{
			ATLASSERT(pElem->pPrev != NULL);
			ATLASSERT(pElem->pPrev->pNext == pElem);

			if(pElem == m_pLast)
			{
				ATLASSERT(pElem->pNext == NULL);
				m_pLast = pElem->pPrev;
				m_pLast->pNext = NULL;
			}
			else
			{
				ATLASSERT(pElem->pNext != NULL);
				ATLASSERT(pElem->pNext->pPrev == pElem);
				pElem->pPrev->pNext = pElem->pNext;
				pElem->pNext->pPrev = pElem->pPrev;
			}
		}

		// For free resources in T object
		pElem->elem = T();

		--m_nSize;
		PutToFree(pElem);
		return true;
	}

	//---------------------------------
	T* First() const
	{
		return (m_pFirst != NULL)? &m_pFirst->elem : NULL;
	}
	T* Last() const
	{
		return (m_pLast != NULL)? &m_pLast->elem : NULL;
	}
	T* Next(const T* pTCurr) const
	{
		if(pTCurr == NULL)
			return NULL;

		TWrap* pCurr = (TWrap*)pTCurr;
		return (pCurr->pNext != NULL)? &pCurr->pNext->elem : NULL;
	}
	T* Prev(const T* pTCurr) const
	{
		if(pTCurr == NULL)
			return NULL;

		TWrap* pCurr = (TWrap*)pTCurr;
		return (pCurr->pPrev != NULL)? &pCurr->pPrev->elem : NULL;
	}

	//---------------------------------
private:

	TWrap* GetElement()
	{
		if(m_pFree == NULL)
		{
			ATLASSERT(m_nSize == m_nAllocSize);
			if(!Extend())
				return NULL;
		}

		TWrap* pNew = m_pFree;
		m_pFree = pNew->pNext;
		pNew->pPrev = pNew->pNext = NULL;
		++m_nSize;
		return pNew;
	}
	bool Extend(int nNewElements = 0)
	{
		if(nNewElements <= 0)
			nNewElements = (m_nAllocSize > 0)? m_nAllocSize : 1;

		TWrap* p = NULL;
		for(int n = 0; n < nNewElements; ++n)
		{
			ATLTRY(p = new TWrap());
			if(p == NULL)
				return false;

			PutToFree(p);
			++m_nAllocSize;
		}
		return true;
	}
	void PutToFree(TWrap* p)
	{
		// Don't change p->pPrev - its not used in m_pFree list
		ATLASSERT(p != NULL);
		p->pNext = m_pFree;
		m_pFree = p;
	}
};

/////////////////////////////////////////////////////////////////////////////
// Self-extending array of string.
//
// ATTENTION : 
//	1. Functions Get() and Duplicate() allocates new struct STRARR and
//	   strings in continuous memory block with CoTaskMemFree().
//	2. For create copy of STRARR contents you should use Duplicate() only !

typedef struct tagStrArr
{
	DWORD	nElem;
	LPTSTR	arrStr[1];	// [size_is(nElem)]
} STRARR;

#define STRBLOCK_CHUNK		(0x1000 * sizeof(TCHAR))
#define STRARR_CHUNK		(0x100 * sizeof(LPCTSTR))

class CStrBlock
{
public:
	CStrBlock() :
	  m_pszBuf(NULL), m_nBufAlloc(0), m_nBufBusy(0),
	  m_pStrArr(NULL), m_cbArrAlloc(0), m_cbArrBusy(0)
	  {}

	~CStrBlock() { Clear(); }

	bool Add(LPCTSTR psz)
	{
		ATLASSERT(psz != NULL);
		return Add(psz, lstrlen(psz));
	}

	// Ending zero appended automatically after nSym symbols
	bool Add(LPCTSTR psz, DWORD nSym)
	{
		ATLASSERT(psz != NULL);

		if(m_pszBuf == NULL || m_nBufAlloc <= m_nBufBusy + nSym)
		{
			ATLASSERT(m_pszBuf != NULL || (m_nBufAlloc == 0 && m_nBufBusy == 0));
			ATLASSERT(m_pszBuf == NULL || (m_nBufAlloc > 0 && m_nBufBusy > 0));

			DWORD nAlloc = ((m_nBufBusy + nSym)/STRBLOCK_CHUNK + 1) * STRBLOCK_CHUNK;
			LPTSTR pszBuf = (LPTSTR)::CoTaskMemRealloc(m_pszBuf, nAlloc * sizeof(TCHAR));
			if(pszBuf == NULL)
			{
				Clear();
				return false;
			}

			m_pszBuf = pszBuf;
			m_nBufAlloc = nAlloc;
		}
		if(m_pStrArr == NULL || m_cbArrAlloc < m_cbArrBusy + sizeof(LPTSTR))
		{
			ATLASSERT((m_pStrArr == NULL && m_cbArrAlloc == 0) || (m_pStrArr != NULL && m_cbArrAlloc > 0));

			DWORD nAlloc = ((m_cbArrBusy + sizeof(LPTSTR))/STRARR_CHUNK + 1) * STRARR_CHUNK;
			STRARR* pArr = (STRARR*)::CoTaskMemRealloc(m_pStrArr, nAlloc);
			if(pArr == NULL)
			{
				Clear();
				return false;
			}

			if(m_pStrArr == NULL)
			{
				pArr->nElem = 0;
				m_cbArrBusy = sizeof(DWORD) * 2; // For x64 8 Bytes alignment !!!
			}

			m_pStrArr = pArr;
			m_cbArrAlloc = nAlloc;
		}

		LPTSTR pszEnd = m_pszBuf + m_nBufBusy;
		memcpy(pszEnd, psz, nSym * sizeof(TCHAR));
		pszEnd[nSym] = (TCHAR)0;

		// Save shifts to strings, not pointers !
		m_pStrArr->arrStr[m_pStrArr->nElem++] = (LPTSTR)(pszEnd - m_pszBuf);

		m_nBufBusy += nSym + 1;
		m_cbArrBusy += sizeof(LPTSTR);
		return true;
	}

	// Returned pointer should be freed with CoTaskMemFree()
	STRARR* GetData()
	{
		DWORD cbLen = m_cbArrBusy + m_nBufBusy * sizeof(TCHAR);
		if(cbLen == 0) cbLen = sizeof(DWORD);

		STRARR* pArr = (STRARR*)::CoTaskMemAlloc(cbLen);
		if(pArr == NULL) return NULL;

		if(cbLen == sizeof(DWORD))
		{
			pArr->nElem = 0;
			return pArr;
		}

		//---------------------------------
		LPTSTR psz = (LPTSTR)((BYTE*)pArr + m_cbArrBusy);
		memcpy(psz, m_pszBuf, m_nBufBusy * sizeof(TCHAR));

		// Copy STRARR data with conversion from shifts to pointers
		pArr->nElem = m_pStrArr->nElem;
		for(DWORD n = 0; n < pArr->nElem; ++n)
			pArr->arrStr[n] = psz + (DWORD_PTR)m_pStrArr->arrStr[n];

		return pArr;
	}

	static STRARR* Duplicate(const STRARR* pSrc, DWORD nStringsForElement = 1)
	{
		ATLASSERT(pSrc != NULL);
		if(pSrc == NULL) return NULL;
		ATLASSERT(nStringsForElement > 0);
		if(nStringsForElement == 0) return NULL;

		//---------------------------------
		LPTSTR pMax = NULL;
		DWORD nElem = pSrc->nElem * nStringsForElement;
		DWORD n = 0;
		for(; n < nElem; ++n)
		{
			if((DWORD_PTR)pSrc->arrStr[n] > (DWORD_PTR)pMax) pMax = pSrc->arrStr[n];
		}

		DWORD_PTR cbSize = sizeof(STRARR);
		if(pMax != NULL)
			cbSize = (DWORD_PTR)pMax + lstrlen(pMax) + sizeof(TCHAR) - (DWORD_PTR)pSrc;

		STRARR* pDst = (STRARR*)::CoTaskMemAlloc(cbSize);
		if(pDst == NULL) return NULL;

		memcpy(pDst, pSrc, cbSize);

		DWORD_PTR dwDiff = (DWORD_PTR)pDst - (DWORD_PTR)pSrc;
		DWORD_PTR* pdw = (DWORD_PTR*)pDst->arrStr;
		for(n = 0; n < nElem; ++n, ++pdw)
		{
			if(*pdw != 0)
				*pdw += dwDiff;
		}

		return pDst;
	}

	void Clear()
	{
		if(m_pszBuf != NULL)
		{
			::CoTaskMemFree(m_pszBuf);
			m_pszBuf = NULL;
		}
		m_nBufAlloc = m_nBufBusy = 0;

		if(m_pStrArr != NULL)
		{
			::CoTaskMemFree(m_pStrArr);
			m_pStrArr = NULL;
		}
		m_cbArrAlloc = m_cbArrBusy = 0;
	}

private:
	LPTSTR	m_pszBuf;		// Current buffer for strings
	DWORD	m_nBufAlloc;	// Size of m_pszBuf in TCHAR's
	DWORD	m_nBufBusy;		// Busy part of m_pszBuf in TCHAR's

	STRARR*	m_pStrArr;		// Current buffer for struct STRARR
	DWORD	m_cbArrAlloc;	// Size of m_pStrArr in bytes
	DWORD	m_cbArrBusy;	// Busy part of m_pStrArr in bytes
};

/////////////////////////////////////////////////////////////////////////////
#define STRBUF_SIZE		0x1000

class CStringBuf
{
public:
	CStringBuf() : m_pszBuf(NULL), m_nAlloc(0), m_nSize(0) {}
	~CStringBuf() { Clear(); }

	bool Assign(DWORD nSym = STRBUF_SIZE)
	{
		if(m_pszBuf == NULL || m_nAlloc < nSym)
		{
			DWORD nAlloc = (nSym/STRBUF_SIZE + 1) * STRBUF_SIZE;
			LPTSTR pszBuf = (LPTSTR)::CoTaskMemRealloc(m_pszBuf, nAlloc * sizeof(TCHAR));
			if(pszBuf == NULL)
			{
				Clear();
				return false;
			}

			m_pszBuf = pszBuf;
			m_nAlloc = nAlloc;
			m_pszBuf[m_nSize] = 0;
		}
		return true;
	}

	bool Append(LPCTSTR psz)
	{
		ATLASSERT(psz != NULL);

		DWORD nLen = lstrlen(psz);
		if(nLen == 0) return true;

		if(!Assign(m_nSize + nLen + 1)) return false;

		lstrcpy(m_pszBuf + m_nSize, psz);
		m_nSize += nLen;
		return true;
	}

	bool Put(LPCTSTR psz)
	{
		ATLASSERT(psz != NULL);

		DWORD nLen = lstrlen(psz);
		if(nLen == 0) return true;

		if(m_pszBuf == NULL || m_nAlloc <= nLen)
		{
			Clear();
			if(!Assign(nLen + 1)) return false;
		}

		lstrcpy(m_pszBuf, psz);
		m_nSize = nLen;
		return true;
	}

	LPCTSTR Get() const
	{
		ATLASSERT(m_pszBuf != NULL);
		return (m_pszBuf != NULL)? m_pszBuf : _T("");
	}

	LPTSTR Detach()
	{
		LPTSTR psz = m_pszBuf;
		m_pszBuf = NULL;
		m_nAlloc = m_nSize = 0;
		return psz;
	}

	void Clear()
	{
		if(m_pszBuf != NULL)
		{
			::CoTaskMemFree(m_pszBuf);
			m_pszBuf = NULL;
		}
		m_nAlloc = m_nSize = 0;
	}

private:
	LPTSTR	m_pszBuf;
	DWORD	m_nAlloc;		// Size of allocated buffer in TCHAR's
	DWORD	m_nSize;		// Size of filled part in TCHAR's without ending zero
};

/////////////////////////////////////////////////////////////////////////////
#endif	// __CONTAINERS_H_
