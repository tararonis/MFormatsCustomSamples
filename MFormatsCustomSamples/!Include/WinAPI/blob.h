//---------------------------------------------------------------------------
// This file is part of the MediaLooks DirectShow Template Library (DSTL)
//
// Blob.h : 
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

#if !defined(BLOB_H)
#define BLOB_H

#include "wrappers.h"
#include "Macros.h"

#pragma warning(push)
#pragma warning(disable:4189)

//////////////////////////////////////////////////////////////////////////
// Define DS_STL_USAGE macros if you want using STL with BLOBS
//
#ifdef DS_STL_USAGE

#include <set>
using std::set;

#include <map>
using std::map;

#endif

//////////////////////////////////////////////////////////////////////////
// Some Additional defines and typedefs
//
#ifndef BYTEBLOB
struct BYTEBLOB
{
    DWORD   cbData;
    LPBYTE  pbData;
};
#else 
#error "BYTEBLO defined"
#endif

#ifndef _LPCBYTE_DEFINED
#define _LPCBYTE_DEFINED
typedef const BYTE *LPCBYTE;
#endif

#ifndef VERIFY
#ifndef _DEBUG
#define VERIFY( f ) ( f )
#else
#define VERIFY( f ) ATLASSERT( f )
#endif // _DEBUG
#endif // VERIFY


#ifdef __IStream_INTERFACE_DEFINED__
//////////////////////////////////////////////////////////////////////////
// Simple helpers for read/write from stream
//
template< class T >
HRESULT SimpleWriteToStream( const T* pT, IStream* _pStream )
{
    ATLASSERT( _pStream );
    
    DWORD cbWritten = 0;
    HRESULT hr = _pStream->Write( pT , sizeof(T), &cbWritten );
    ATLASSERT( SUCCEEDED( hr ) );
    ATLASSERT( sizeof(T) == cbWritten );
    
    return hr;
}

template< class T >
HRESULT SimpleReadFromStream( T* pT, IStream* _pStream )
{
    ATLASSERT( _pStream );
    
    DWORD cbRead = 0;
    HRESULT hr = _pStream->Read( pT , sizeof(T), &cbRead );
    ATLASSERT( SUCCEEDED( hr ) );
    ATLASSERT( sizeof(T) == cbRead );
    
    return hr;
}

#endif // __IStream_INTERFACE_DEFINED__

//////////////////////////////////////////////////////////////////////////
// Some classes for BLOB memory allocation
// Note: You can write you own classes for specified memory allocation.
//

#ifdef _OBJBASE_H_

class CCoTaskAlloc
{
public:
    static LPBYTE Alloc( DWORD& _rdwSize )
    {
		if( !_rdwSize )
			return NULL;

        LPBYTE pbRes =  (LPBYTE)::CoTaskMemAlloc( _rdwSize );
        ATLASSERT( pbRes );
        if( !pbRes ) _rdwSize =0;

        return pbRes;
    }
    static void Free( LPBYTE _pbData, DWORD _dwSize )
    {
		if( _pbData )
			::CoTaskMemFree( _pbData );
    }
    static bool IsMarshaled()
    {
        return true;
    }
};

#endif // _OBJBASE_H_

class CNewAlloc
{
public:
    static LPBYTE Alloc( DWORD& _rdwSize )
    {
        LPBYTE pbRes =  new BYTE[ _rdwSize ];
        ATLASSERT( pbRes );
        if( !pbRes ) _rdwSize =0;

        return pbRes;
    }
    static void Free( LPBYTE _pbData, DWORD _dwSize  )
    {
        delete[] _pbData;
    }
    static bool IsMarshaled()
    {
        return false;
    }
};

class CAlignedAlloc
{
public:
    static LPBYTE Alloc( DWORD& _rdwSize )
    {
        // Align size
        _rdwSize = (_rdwSize + 7 ) & ~0x7; 
        LPBYTE pbRes =  (LPBYTE)new UINT64[ _rdwSize >> 3 ];
        ATLASSERT( pbRes );
        if( !pbRes ) _rdwSize =0;

        return pbRes;
    }
    static void Free( LPBYTE _pbData, DWORD _dwSize )
    {
        delete[] _pbData;
    }
    static bool IsMarshaled()
    {
        return false;
    }
};

class CFakeAlloc
{
public:
    static LPBYTE Alloc( DWORD& _rdwSize )
    {
        ATLASSERT( FALSE );
        return NULL;
    }
    static void Free( LPBYTE _pbData, DWORD _dwSize )
    {
        //ATLASSERT( FALSE );
    }
    static bool IsMarshaled()
    {
        return false;
    }
};

class CVirtualAlloc
{
public:
    static LPBYTE Alloc( DWORD& _rdwSize )
    {
		if( !_rdwSize ) return (LPBYTE)&_rdwSize;

        LPBYTE pbRes = (LPBYTE)::VirtualAlloc( NULL, _rdwSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE );
        //_ASSERTE( pbRes );
        if( !pbRes )
        {
            _rdwSize =0;
            return NULL;
        }

        _rdwSize = (_rdwSize + 0xFFF  ) & ~0xFFF;

        return pbRes;
    }
    static void Free( LPBYTE _pbData, DWORD _dwSize )
    {
        if( _pbData && _dwSize )
        {
            BOOL bSuccess = ::VirtualFree( _pbData, 0, MEM_RELEASE );
            _ASSERTE( bSuccess );
        }
    }
    static bool IsMarshaled()
    {
        return false;
    }
};

class CHeapAlloc
{
public:
    static LPBYTE Alloc( DWORD& _rdwSize )
    {
		if( !_rdwSize ) return (LPBYTE)&_rdwSize;

        LPBYTE pbRes = (LPBYTE)::HeapAlloc( ::GetProcessHeap(), 0, ALIGN_UP( _rdwSize + 24, 8 ) );
		ATLASSERT( pbRes );
        if( !pbRes )
        {
            _rdwSize = 0;
            return NULL;
        }

        return Real2Ptr( pbRes );
    }
    static void Free( LPBYTE _pbData, DWORD _dwSize )
    {
        if( _pbData && _dwSize )
        {
            BOOL bSuccess = ::HeapFree( ::GetProcessHeap(), 0, Ptr2Real(_pbData) );
            _ASSERTE( bSuccess );
        }
    }
    static bool IsMarshaled()
    {
        return false;
    }

	static LPBYTE Ptr2Real( LPBYTE _pbPtr )
	{
		BYTE btOffset  = *(_pbPtr-1);
		return _pbPtr - btOffset;
	}

	static LPBYTE Real2Ptr( LPBYTE _pbReal )
	{
		// Align up to 16 bytes
		LPBYTE pbPtr = (LPBYTE)((LONG_PTR)(_pbReal + 1 + 16) & ~((LONG_PTR)0xF));
		ATLASSERT( pbPtr >= _pbReal + 1 );
		BYTE offset = (BYTE)(pbPtr - _pbReal);
		*(pbPtr-1) = offset;
		return pbPtr;
	}
};

//////////////////////////////////////////////////////////////////////////
// Class for simplifier work with BLOBS 
//
template< class TAllocator >
class CBLOBHolder
	: public AutoObjBase<CBLOBHolder<TAllocator > >
{
	void Zero_Members()
	{
		::ZeroMemory( &m_blobData, sizeof( m_blobData ) );
		m_dwAllocSize = 0;
#if defined(_DEBUG)
		m_pbDifferDataOne = m_pbDifferDataTwo = NULL;
		m_nDiffShift = 0;
#endif // defined(_DEBUG)
	}

public:
	BYTEBLOB m_blobData;

protected:

	

	DWORD    m_dwAllocSize;
#if defined(_DEBUG)
	mutable LPBYTE  m_pbDifferDataOne;
	mutable LPBYTE  m_pbDifferDataTwo;
	mutable DWORD   m_nDiffShift;
#endif // defined(_DEBUG)
public:

	static HRESULT CreateInstance( CBLOBHolder<TAllocator>** _ppObject, int _cbAllocSize = 0, int _cbResize = -1 )
	{
		HRESULT hr = _AutoObjBase::CreateInstance( _ppObject );
		if( FAILED( hr ) )
			return hr;

		ATLASSERT( _ppObject );
		if( _cbAllocSize > 0 )
			return (*_ppObject)->AllocMem( _cbAllocSize, true, 0, _cbResize );

		return S_OK;
	}

	static HRESULT CreateInstance( CBLOBHolder<TAllocator>** _ppObject, LPBYTE _pbData, int _cbData )
	{
		HRESULT hr = _AutoObjBase::CreateInstance( _ppObject );
		if( FAILED( hr ) )
			return hr;

		ATLASSERT( _ppObject );
		if( _cbData > 0 )
			return (*_ppObject)->Init( _cbData, _pbData );

		return S_OK;
	}

    inline CBLOBHolder()
    {
        Zero_Members();

		ATLTRACE2( atlTraceGeneral, 12, _T("CBLOBHolder::CBLOBHolder( Ptr:%08X, Size=%d, allsize=%d)\n"), this, sizeof( CBLOBHolder ), m_dwAllocSize );
    }


	inline CBLOBHolder( const BYTE* _pbData, DWORD _cbData, bool _bFillZero = false )
	{
		m_blobData.cbData = _cbData;
		m_dwAllocSize = _cbData;
		m_blobData.pbData = TAllocator::Alloc( m_dwAllocSize );

		ATLASSERT( m_blobData.pbData );
		if( _pbData )
		{
			::CopyMemory( m_blobData.pbData, _pbData, m_blobData.cbData );
		}
		else if( _bFillZero )
		{
			::ZeroMemory( m_blobData.pbData, m_blobData.cbData );
		}
	}

	inline CBLOBHolder( const BYTEBLOB& _blobData )
    {
        m_blobData.cbData = _blobData.cbData;
        m_dwAllocSize = m_blobData.cbData;
		if( m_dwAllocSize )
		{
			m_blobData.pbData = TAllocator::Alloc( m_dwAllocSize );
			
			ATLASSERT( m_blobData.pbData );
			if( m_blobData.pbData )
			{
				::CopyMemory( m_blobData.pbData, _blobData.pbData, m_blobData.cbData );
			}
			else
			{
				Zero_Members();
			}
		}
		else
		{
			m_blobData.pbData = 0;
		}
    }

    inline CBLOBHolder( const CBLOBHolder& _crBLOBHolder )
    {
        m_blobData.cbData = _crBLOBHolder.m_blobData.cbData;
        m_dwAllocSize = m_blobData.cbData;
		if( m_dwAllocSize )
		{
			m_blobData.pbData = TAllocator::Alloc( m_dwAllocSize );
			
			ATLASSERT( m_blobData.pbData );
			if( m_blobData.pbData )
			{
				::CopyMemory( m_blobData.pbData, _crBLOBHolder.m_blobData.pbData, m_blobData.cbData );
			}
			else
			{
				Zero_Members();
			}
		}
		else
		{
			m_blobData.pbData = 0;
		}

    }

    inline CBLOBHolder( const BSTR& _rbsInitString )
    {
        if( _rbsInitString )
        {
            m_blobData.cbData = ::SysStringByteLen( _rbsInitString  ) + 2;
            m_dwAllocSize = m_blobData.cbData;
            m_blobData.pbData = TAllocator::Alloc( m_dwAllocSize );
            
            
            ATLASSERT( m_blobData.pbData );
            if( m_blobData.pbData )
            {
                ::CopyMemory( m_blobData.pbData, _rbsInitString , m_blobData.cbData );
            }
            else
            {
                Zero_Members();
            }
        }
        else
        {
            Zero_Members();
        }
    }

    inline ~CBLOBHolder()
    {
        TAllocator::Free( m_blobData.pbData, m_dwAllocSize );
    }

    inline CBLOBHolder& operator = ( const CBLOBHolder& _crBLOBHolder )
    {
        if( this == _crBLOBHolder.Addr() ) return *this;

        HRESULT hr = InternalAlloc( _crBLOBHolder.m_blobData.cbData );
        ATLASSERT( SUCCEEDED( hr ) );
        if( SUCCEEDED( hr ) )
        {
            ::CopyMemory( m_blobData.pbData, _crBLOBHolder.m_blobData.pbData, m_blobData.cbData );
        }
        else
        {
            Zero_Members();
        }
        
        return *this;
    }

    inline CBLOBHolder& operator = ( const BYTEBLOB& _crBLOBHolder )
    {
        HRESULT hr = InternalAlloc( _crBLOBHolder.cbData );
        ATLASSERT( SUCCEEDED( hr ) );
        if( SUCCEEDED( hr ) )
        {
            ::CopyMemory( m_blobData.pbData, _crBLOBHolder.pbData, m_blobData.cbData );
        }
        else
        {
            Zero_Members();
        }
                
        return *this;
    }

    inline HRESULT PutBLOB( const BYTEBLOB& _crBlobData )
    {
        HRESULT hr = InternalAlloc( _crBlobData.cbData );
        ATLASSERT( SUCCEEDED( hr ) );
        if( FAILED( hr ) ) return hr;

        ::CopyMemory( m_blobData.pbData, _crBlobData.pbData, m_blobData.cbData );
        return S_OK;
    }

    inline BYTEBLOB Detach()
    {
        BYTEBLOB blobRes = m_blobData;
        Zero_Members();
        
        return blobRes;
    }

    inline CBLOBHolder& Attach( const BYTEBLOB& _blobData )
    {
        if( m_blobData.pbData )
            TAllocator::Free( m_blobData.pbData , m_dwAllocSize );

        m_blobData.pbData = _blobData.pbData;
		m_blobData.cbData = _blobData.pbData ? _blobData.cbData : 0;
        m_dwAllocSize = m_blobData.cbData;

        return *this;
    }

	inline CBLOBHolder& Attach( LPCVOID _pbData, DWORD _cbData )
    {
        if( m_blobData.pbData )
            TAllocator::Free( m_blobData.pbData , m_dwAllocSize );

        m_blobData.pbData = (LPBYTE)_pbData;
		m_blobData.cbData = _pbData ? _cbData : 0;
        m_dwAllocSize = m_blobData.cbData;

        return *this;
    }

	inline CBLOBHolder& SetData( LPCVOID _pbData, DWORD _cbData )
	{
		AllocMem( _cbData, _pbData == NULL );
		if( _pbData )
			MemCopy( _pbData, _cbData );
		
		return *this;
	}

    inline CBLOBHolder& Clear()
    {
        m_blobData.cbData = 0;

        return *this;
    }

    inline CBLOBHolder& FreeMem()
    {
        TAllocator::Free( m_blobData.pbData, m_dwAllocSize );
        Zero_Members();

        return *this;
    }

	inline HRESULT Init( DWORD _cbData, LPCVOID _pbData )
	{
		HRESULT hr = AllocMem( _cbData, _pbData == NULL );
		if( FAILED( hr ) )
			return hr;

		if( _pbData )
			MemCopy( _pbData, _cbData );

		return hr;
	}

	inline LPBYTE ShiftData( int _nShiftRight, DWORD _dwShiftStartPos = 0 )
	{
		if( _dwShiftStartPos > GetSize() )
			return NULL;

		DWORD cbMove = GetSize(_dwShiftStartPos);
		
		LPBYTE pbShiftStart = NULL;
		if( _nShiftRight > 0 )
		{
			ReallocMem( GetSize() + _nShiftRight );
			pbShiftStart = Ptr(_dwShiftStartPos);
			::MoveMemory( pbShiftStart + _nShiftRight, pbShiftStart, cbMove );
		}
		else if( _nShiftRight < 0 )
		{
			pbShiftStart = Ptr(_dwShiftStartPos);
			::MoveMemory( pbShiftStart, pbShiftStart - _nShiftRight, cbMove + _nShiftRight );
			ReallocMem( GetSize() + _nShiftRight );			
		}

		return pbShiftStart;
	}

    inline CBLOBHolder& MemSet( BYTE _btValue, DWORD _dwOffset = 0, int _cbSize = -1 )
    {
        ATLASSERT( m_blobData.pbData != NULL );
		ATLASSERT( _dwOffset <= m_blobData.cbData );
		_dwOffset = MIN( _dwOffset, m_blobData.cbData );

		_cbSize = MIN( _cbSize, (int)(m_blobData.cbData - _dwOffset) );
		DWORD cbFill = _cbSize >= 0 ? (DWORD)_cbSize : m_blobData.cbData - _dwOffset;

		if( cbFill )
        ::FillMemory( m_blobData.pbData + _dwOffset, cbFill, _btValue );

        return *this;
    }

    inline CBLOBHolder& MemCopy( LPCVOID _cpbSource, DWORD _cbSize, DWORD _dwOffset = 0 )
    {
        ATLASSERT( _cpbSource );
        ATLASSERT( m_blobData.pbData != NULL );
        ATLASSERT( m_blobData.cbData - _dwOffset >= _cbSize  );
		__try
		{
			::CopyMemory( m_blobData.pbData + _dwOffset, _cpbSource, MIN( _cbSize, m_blobData.cbData - _dwOffset ) );
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			ATLTRACE2( atlTraceGeneral, 2, _T("###Err### CBLOBHolder::MemCopy - HANDLED EXCEPTION\n") );
		}
        

        return *this;
    }

	inline int MemRemove( DWORD _cbSize )
	{
		ATLASSERT( _cbSize <= GetSize() );
		if( _cbSize > GetSize() ) return -1;

		HRESULT hr = MemMove( m_blobData.pbData + _cbSize, GetSize() - _cbSize );
		ATLASSERT( SUCCEEDED( hr ) );

		hr = Resize( GetSize() - _cbSize );
		ATLASSERT( SUCCEEDED( hr ) );

		return GetSize();
	}

	inline CBLOBHolder& MemAppend( LPCVOID _cpbSource, DWORD _cbSize, DWORD* _pdwDataOffset = NULL, DWORD _cbMinAppendBlock = 0 )
    {
        if( _cbSize )
		{
			if( m_blobData.cbData + _cbSize > m_dwAllocSize )
			{
				DWORD dwOffset = _pdwDataOffset ? *_pdwDataOffset : 0;
				DWORD cbOldSize = m_blobData.cbData - dwOffset;
				ReallocMem( cbOldSize + MAX(_cbMinAppendBlock, _cbSize), false, 0, &dwOffset );
				ATLASSERT( dwOffset == 0 );
				Resize( cbOldSize );

				if( _pdwDataOffset )
					*_pdwDataOffset = 0;
			}

			ATLASSERT( m_blobData.pbData != NULL );
			ATLASSERT( m_dwAllocSize >= m_blobData.cbData + _cbSize  );
			if( _cpbSource )
				::CopyMemory( m_blobData.pbData + m_blobData.cbData, _cpbSource, MIN( _cbSize, m_dwAllocSize - m_blobData.cbData ) );
			else
				::ZeroMemory( m_blobData.pbData + m_blobData.cbData, MIN( _cbSize, m_dwAllocSize - m_blobData.cbData ) );

			m_blobData.cbData += MIN( _cbSize, m_dwAllocSize - m_blobData.cbData );
		}

        return *this;
    }

	inline CBLOBHolder& MemMove( LPCVOID _cpbSource, DWORD _cbSize, DWORD _dwOffset = 0 )
    {
        ATLASSERT( _cpbSource );
        ATLASSERT( m_blobData.pbData != NULL );
        ATLASSERT( m_blobData.cbData - _dwOffset >= _cbSize  );
        ::MoveMemory( m_blobData.pbData + _dwOffset, _cpbSource, MIN( _cbSize, m_blobData.cbData - _dwOffset ) );

        return *this;
    }

	inline int MemCompare( LPCVOID _cpbSource, DWORD _cbSize, DWORD _dwOffset = 0 ) const
    {
		if (_dwOffset > m_blobData.cbData || !m_blobData.pbData)
			return -1;

		if (_cbSize == 0)
			_cbSize = m_blobData.cbData - _dwOffset;

        ATLASSERT( _cpbSource );
        ATLASSERT( m_blobData.pbData != NULL );
        ATLASSERT( m_blobData.cbData - _dwOffset >= _cbSize  );

        return memcmp( m_blobData.pbData + _dwOffset, _cpbSource, MIN( _cbSize, m_blobData.cbData - _dwOffset ) );
    }

	inline void CopyTo( LPVOID _pvBuffer, DWORD _cbBuffer = 0, DWORD _dwOffset = 0 ) const
	{
		ATLASSERT( _pvBuffer );
        ATLASSERT( m_blobData.pbData != NULL );
		ATLASSERT( m_blobData.cbData >= _dwOffset + _cbBuffer );

		_cbBuffer = _cbBuffer ? _cbBuffer : m_blobData.cbData - _dwOffset;

		::CopyMemory( _pvBuffer, m_blobData.pbData + _dwOffset, MIN( _cbBuffer, m_blobData.cbData - _dwOffset ) );
	}

    inline HRESULT AllocMem( DWORD _dwSize, bool _bFillMem = false, BYTE _btValue = 0, int _nResize = -1 )
    {
        HRESULT hr = InternalAlloc( _dwSize );
        if( FAILED( hr ) ) return hr;

        if( _bFillMem )
            ::FillMemory( m_blobData.pbData, m_blobData.cbData, _btValue );

		if( _nResize >= 0 )
			Resize(_nResize);

        return hr;
    }

	inline HRESULT ReallocMem( DWORD _dwSize, bool _bFillMem = false, BYTE _btValue = 0, DWORD* _pdwStartOffset = 0 )
    {
		DWORD dwDataOffset = _pdwStartOffset ? *_pdwStartOffset : 0;
		ATLASSERT( dwDataOffset <= m_blobData.cbData );
		dwDataOffset = MIN( dwDataOffset, m_blobData.cbData );


		DWORD dwLastSize = m_blobData.cbData;
        HRESULT hr = InternalRealloc( _dwSize, _pdwStartOffset );
        if( FAILED( hr ) ) return hr;

		if( _bFillMem )
		{
			if( hr == S_FALSE )
			{
				// Just increase size, offset is NOT changed
				if( dwLastSize < m_blobData.cbData )
					::FillMemory( m_blobData.pbData + dwLastSize, m_blobData.cbData - dwLastSize, _btValue );
			}
			else
			{
				// No data moved from [offset][data] -> [data]...
				DWORD cbLastData = dwLastSize - dwDataOffset;
				if( m_blobData.cbData > cbLastData )
					::FillMemory( m_blobData.pbData + cbLastData, m_blobData.cbData - cbLastData, _btValue );
			}
		}
		
        return hr;
    }

    inline LPBYTE Ptr( DWORD _dwOffset = 0)
    {
        ATLASSERT( m_dwAllocSize > _dwOffset && m_blobData.cbData >= _dwOffset || _dwOffset == 0 );
        if( m_dwAllocSize <= _dwOffset ) return NULL;

        return m_blobData.pbData + _dwOffset;
    }

	inline LPCBYTE Ptr( DWORD _dwOffset = 0) const
    {
		ATLASSERT( m_dwAllocSize > _dwOffset && m_blobData.cbData >= _dwOffset || _dwOffset == 0 );
		if( m_dwAllocSize <= _dwOffset ) return NULL;

		return m_blobData.pbData + _dwOffset;
    }

	inline LPBYTE EndPtr( DWORD _dwBackOffset = 0 )
    {
		ATLASSERT( m_blobData.cbData >= _dwBackOffset || _dwBackOffset == 0 );
        if( m_blobData.cbData < _dwBackOffset ) return NULL;

        return m_blobData.pbData + m_blobData.cbData - _dwBackOffset;
    }

	inline LPCBYTE EndPtr( DWORD _dwBackOffset = 0 ) const
    {
		ATLASSERT( m_blobData.cbData >= _dwBackOffset || _dwBackOffset == 0 );
        if( m_blobData.cbData < _dwBackOffset ) return NULL;

        return m_blobData.pbData + m_blobData.cbData - _dwBackOffset;
    }

    inline BYTE& operator[] ( DWORD _dwPos ) 
    {
        static BYTE zeroByte = 0;

        ATLASSERT( m_blobData.cbData > _dwPos );
        if( m_blobData.cbData <= _dwPos  ) return zeroByte;

        return *(m_blobData.pbData + _dwPos);
    }

    inline const BYTE& operator[] ( DWORD _dwPos ) const
    {
        static const BYTE zeroByte = 0;

        ATLASSERT( m_blobData.cbData > _dwPos );
        if( m_blobData.cbData <= _dwPos  ) return zeroByte;

        return *(m_blobData.pbData + _dwPos);
    }

    inline HRESULT Resize( DWORD _dwSize )
    {
        ATLASSERT( m_dwAllocSize >= _dwSize );
        if( m_dwAllocSize >= _dwSize )
        {
            m_blobData.cbData = _dwSize;
            return S_OK;
        }

        return E_INVALIDARG;
    }

	inline HRESULT IncreaseSize( DWORD _dwSize )
	{
		ATLASSERT( m_dwAllocSize >= m_blobData.cbData + _dwSize );
		if( m_dwAllocSize >= m_blobData.cbData + _dwSize )
		{
			m_blobData.cbData += _dwSize;
			return S_OK;
		}

		return E_INVALIDARG;
	}

    inline DWORD GetAllocSize() const 
    {
        return m_dwAllocSize;
    }

    inline DWORD GetSize( DWORD _dwOffset = 0 ) const 
	{ 
        return m_blobData.cbData >= _dwOffset ? m_blobData.cbData - _dwOffset : 0;
    }

	inline DWORD GetFreeSize() const 
	{
		ATLASSERT( m_dwAllocSize >= m_blobData.cbData );
		return m_dwAllocSize - m_blobData.cbData;
	}

#ifdef __IStream_INTERFACE_DEFINED__
    HRESULT WriteToStream( IStream* _pStream ) const 
    {
        ATLASSERT( _pStream );
        if( !_pStream ) return E_POINTER;

        HRESULT hr = SimpleWriteToStream( &m_blobData.cbData, _pStream );
        ATLASSERT( SUCCEEDED( hr ) );
        if( FAILED( hr ) ) return hr;

        if( m_blobData.cbData )
        {
            DWORD cbWritten = 0;
            hr = _pStream->Write( m_blobData.pbData , m_blobData.cbData , &cbWritten );
            ATLASSERT( SUCCEEDED( hr ) );
            ATLASSERT( m_blobData.cbData  == cbWritten );
        }

        return hr;
    }

    HRESULT ConvertToStream2( IStream** _ppStream, BOOL _bFree = FALSE ) const 
    {
        if( !_ppStream ) return E_POINTER;
        
        HGLOBAL hgData = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_DISCARDABLE , m_blobData.cbData  );
        LPVOID pvDestData = ::GlobalLock( hgData );
        ATLASSERT( pvDestData );
        if( !pvDestData ) return E_OUTOFMEMORY;
        
        ::CopyMemory( pvDestData , m_blobData.pbData , m_blobData.cbData );
        
        if( _bFree )
        {
            TAllocator::Free( m_blobData.pbData, m_dwAllocSize );           
        }
        
        ::GlobalUnlock( hgData );
        
        CComPtr<IStream> cpStream;
        HRESULT hr = ::CreateStreamOnHGlobal( hgData , TRUE, &cpStream );
        ATLASSERT( SUCCEEDED( hr ) );
        if( FAILED( hr ) ) return hr;
        
        *_ppStream = cpStream.Detach();

        return S_OK;
    }

	HRESULT ConvertToStream( IStream** _ppStream, BOOL _bFree = FALSE ) const 
    {
        if( !_ppStream ) return E_POINTER;
        
        CComPtr<IStream> cpStream;
        HRESULT hr = ::CreateStreamOnHGlobal( NULL, TRUE, &cpStream );
        ATLASSERT( SUCCEEDED( hr ) );
        if( FAILED( hr ) ) return hr;

		hr = cpStream->Write( m_blobData.pbData, m_blobData.cbData, NULL );
		ATLASSERT( SUCCEEDED( hr ) );
        if( FAILED( hr ) ) return hr;

		LARGE_INTEGER liSet = {0};
		cpStream->Seek( liSet, STREAM_SEEK_SET, NULL );
        
        if( _bFree )
        {
            TAllocator::Free( m_blobData.pbData, m_dwAllocSize );           
        }
        
        *_ppStream = cpStream.Detach();

        return S_OK;
    }

    HRESULT ConvertFromStream( IStream* _pStream ) 
    {
        ATLASSERT( _pStream );
        if( !_pStream ) return E_POINTER;

		LARGE_INTEGER liZero ={0};
		ULARGE_INTEGER liPos ={0};
		HRESULT hr = _pStream->Seek( liZero, STREAM_SEEK_END, &liPos );
		ATLASSERT( SUCCEEDED( hr ) );
		if( FAILED( hr ) ) return hr;

		ATLASSERT( liPos.QuadPart < MAXDWORD );

        hr = InternalAlloc( liPos.LowPart ); // was QuadPart: msvc7: warning C4244: 'argument' : conversion from 'ULONGLONG' to 'DWORD', possible loss of data
        ATLASSERT( SUCCEEDED( hr ) );
        if( FAILED( hr ) ) return hr;

		hr = _pStream->Seek( liZero, STREAM_SEEK_SET, &liPos );
		ATLASSERT( SUCCEEDED( hr ) );
		if( FAILED( hr ) ) return hr;

		DWORD cbRead = 0;
		ATLASSERT( m_blobData.pbData );
		hr = _pStream->Read( m_blobData.pbData, m_blobData.cbData, &cbRead );
		ATLASSERT( SUCCEEDED( hr ) );
		if( FAILED( hr ) ) return hr;

		ATLASSERT( cbRead == m_blobData.cbData );
        
        return S_OK;
    }


    HRESULT ReadFromStream( IStream* _pStream )
    {
        ATLASSERT( _pStream );
        if( !_pStream ) return E_POINTER;

        DWORD dwSize = 0;
        HRESULT hr = SimpleReadFromStream( &dwSize, _pStream );
        ATLASSERT( SUCCEEDED( hr ) );
        if( FAILED( hr ) ) return hr;

        hr = InternalAlloc( dwSize );
        ATLASSERT( SUCCEEDED( hr ) );
        if( FAILED( hr ) ) return hr;

        if( dwSize )
        {
            DWORD cbRead = 0;
            hr = _pStream->Read( m_blobData.pbData , m_blobData.cbData , &cbRead );
            ATLASSERT( SUCCEEDED( hr ) );
            ATLASSERT( m_blobData.cbData  == cbRead );
        }

        return hr;
    }
#endif __IStream_INTERFACE_DEFINED__

template< class T >
    HRESULT PutSimpleObject( const T& _crObject )
    {   
        DWORD dwSize = sizeof( _crObject );

        {
            // Allocate memory ( or use already allocated )
            HRESULT hr = InternalAlloc( dwSize );
            ATLASSERT( SUCCEEDED( hr ) );
            if( FAILED( hr ) ) return hr;
        }

        ::CopyMemory( m_blobData.pbData, &_crObject, m_blobData.cbData );

        return S_OK;
    }

template< class T > 
    HRESULT PutObject( const T& _crObject )
    {
        CComPtr<IStream> cpStream;
        VERIFY( SUCCEEDED( ::CreateStreamOnHGlobal( NULL ,TRUE, &cpStream ) ) );
        HRESULT hr = _crObject.WriteToStream( cpStream );
        ATLASSERT( SUCCEEDED( hr ) );
        if( FAILED( hr ) ) return hr;

            
        HGLOBAL hgData = NULL;
        VERIFY( SUCCEEDED( ::GetHGlobalFromStream( cpStream, &hgData) ) );
            
        LPBYTE pbData = (LPBYTE)::GlobalLock( hgData );
        ATLASSERT( pbData );
            
        DWORD dwSize = ::GlobalSize( hgData );
        
        {
            // Allocate memory ( or use already allocated )
            HRESULT hr = InternalAlloc( dwSize );
            ATLASSERT( SUCCEEDED( hr ) );
            if( FAILED( hr ) ) return hr;
        }

        ::CopyMemory( m_blobData.pbData, pbData, m_blobData.cbData );
     
        return S_OK;
    }

#ifdef DS_STL_USAGE

template< class T > 
    HRESULT PutSet( const set<T>& _crSetObjects )
    {
        CComPtr<IStream> cpStream;
        VERIFY( SUCCEEDED( ::CreateStreamOnHGlobal( NULL ,TRUE, &cpStream ) ) );

        DWORD dwCount = _crSetObjects.size();
        VERIFY( SUCCEEDED( SimpleWriteToStream( &dwCount, cpStream ) ) );

        std::set<T>::const_iterator Iter = _crSetObjects.begin();
        while( Iter != _crSetObjects.end() )
        {
            VERIFY( SUCCEEDED( Iter->WriteToStream( cpStream ) ) );

            Iter++;
        }
            
        HGLOBAL hgData = NULL;
        VERIFY( SUCCEEDED( ::GetHGlobalFromStream( cpStream, &hgData) ) );
            
        LPBYTE pbData = (LPBYTE)::GlobalLock( hgData );
        ATLASSERT( pbData );
            
        DWORD dwSize = ::GlobalSize( hgData );
        {
            // Allocate memory ( or use already allocated )
            HRESULT hr = InternalAlloc( dwSize );
            ATLASSERT( SUCCEEDED( hr ) );
            if( FAILED( hr ) ) return hr;
        }

        ::CopyMemory( m_blobData.pbData, pbData, m_blobData.cbData );
     
        return S_OK;
    }

template< class TKey, class TValue > 
    HRESULT PutMap( const map<TKey,TValue>& _crMapObjects, DWORD _dwFlag = -1 )
    {
        CComPtr<IStream> cpStream;
        VERIFY( SUCCEEDED( ::CreateStreamOnHGlobal( NULL ,TRUE, &cpStream ) ) );

        DWORD dwCount = _crMapObjects.size();
        VERIFY( SUCCEEDED( SimpleWriteToStream( &dwCount, cpStream ) ) );

        std::map<TKey,TValue>::const_iterator Iter = _crMapObjects.begin();
        while( Iter != _crMapObjects.end() )
        {
            if( _dwFlag & 1 ) 
                VERIFY( SUCCEEDED( Iter->first.WriteToStream( cpStream ) ) );

            if( _dwFlag & 2 )
                VERIFY( SUCCEEDED( Iter->second.WriteToStream( cpStream ) ) );

            Iter++;
        }
            
        HGLOBAL hgData = NULL;
        VERIFY( SUCCEEDED( ::GetHGlobalFromStream( cpStream, &hgData) ) );
            
        LPBYTE pbData = (LPBYTE)::GlobalLock( hgData );
        ATLASSERT( pbData );
            
        DWORD dwSize = ::GlobalSize( hgData );
        {
            // Allocate memory ( or use already allocated )
            HRESULT hr = InternalAlloc( dwSize );
            ATLASSERT( SUCCEEDED( hr ) );
            if( FAILED( hr ) ) return hr;
        }
        

        ::CopyMemory( m_blobData.pbData, pbData, m_blobData.cbData );
     
        return S_OK;
    }

template< class T > 
    HRESULT GetSet( set<T>& _rSetOfObjects ) const 
    {
        HGLOBAL hgData = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_DISCARDABLE , m_blobData.cbData );
        ATLASSERT( hgData );
        LPVOID pvDestData = ::GlobalLock( hgData );
    
        ::CopyMemory( pvDestData, m_blobData.pbData, m_blobData.cbData);

        VERIFY( ::GlobalUnlock( hgData ) == NO_ERROR );

        CComPtr<IStream> cpStream;
        VERIFY( SUCCEEDED( ::CreateStreamOnHGlobal( hgData ,TRUE, &cpStream ) ) );
     
        DWORD dwCount = 0;
        HRESULT hr = SimpleReadFromStream( &dwCount, cpStream );
        if( SUCCEEDED( hr ) )
        {
            for( int i = 0 ; i < dwCount; i++ )
            {
                T tObjValue;
                hr = tObjValue.ReadFromStream( cpStream ); 
                if( FAILED( hr ) ) break;
                
                _rSetOfObjects.insert( tObjValue );
            }
        }
        
        
        return hr;
    }

template< class TKey , class TValue> 
    HRESULT GetMap( map<TKey,TValue>& _rMapOfObjects ) const 
    {
        HGLOBAL hgData = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_DISCARDABLE , m_blobData.cbData );
        ATLASSERT( hgData );
        LPVOID pvDestData = ::GlobalLock( hgData );
    
        ::CopyMemory( pvDestData, m_blobData.pbData, m_blobData.cbData);

        VERIFY( ::GlobalUnlock( hgData ) == NO_ERROR );

        CComPtr<IStream> cpStream;
        VERIFY( SUCCEEDED( ::CreateStreamOnHGlobal( hgData ,TRUE, &cpStream ) ) );
     
        DWORD dwCount = 0;
        HRESULT hr = SimpleReadFromStream( &dwCount, cpStream );
        if( SUCCEEDED( hr ) )
        {
            for( int i = 0 ; i < dwCount; i++ )
            {
                TKey tKey;
                TValue tValue;
                hr = tKey.ReadFromStream( cpStream ); 
                if( FAILED( hr ) ) break;
                hr = tValue.ReadFromStream( cpStream ); 
                if( FAILED( hr ) ) break;
                
                _rMapOfObjects.insert( tObjValue );
            }
        }
        
        return hr;
    }
#endif // DS_STL_USAGE

template< class T > 
    HRESULT GetObject( T& _rObject ) const 
    {
        HGLOBAL hgData = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_DISCARDABLE , m_blobData.cbData );
        ATLASSERT( hgData );
        LPVOID pvDestData = ::GlobalLock( hgData );
    
        ::CopyMemory( pvDestData, m_blobData.pbData, m_blobData.cbData);

        VERIFY( ::GlobalUnlock( hgData ) == NO_ERROR );

        CComPtr<IStream> cpStream;
        VERIFY( SUCCEEDED( ::CreateStreamOnHGlobal( hgData ,TRUE, &cpStream ) ) );
        
        HRESULT hr = _rObject.ReadFromStream( cpStream );
        ATLASSERT( SUCCEEDED( hr ) );
        
        return hr;
    }


template< class T > 
    HRESULT GetSimpleObject( T& _crObject ) const 
    {
        ATLASSERT( sizeof( _crObject ) == m_blobData.cbData );
        if( sizeof( _crObject ) != m_blobData.cbData ) return E_INVALIDARG; 

        ::CopyMemory( &_crObject, m_blobData.pbData, m_blobData.cbData );

        return S_OK;
    }

    HRESULT SaveToFile( LPCTSTR _lpFileName, bool _bFlushBuffers = false, bool _bAppendFile = false ) const
    {
        HANDLE hFile = ::CreateFile( _lpFileName, 
                                     GENERIC_WRITE, 
                                     0, 
                                     NULL, 
                                     _bAppendFile ? OPEN_ALWAYS : CREATE_ALWAYS, 
                                     FILE_ATTRIBUTE_NORMAL, 
                                     NULL );

        if( hFile == INVALID_HANDLE_VALUE )
        {
            DWORD dwErr = ::GetLastError();
            _RPTF1( 0 , "Error CreateFile : hr = 0x%08X\n" , dwErr );

            return HRESULT_FROM_WIN32(dwErr);
        }

		if( _bAppendFile )
			::SetFilePointer(hFile, 0, NULL, FILE_END);

        DWORD dwWritten = 0 ;
        if( !::WriteFile( hFile, m_blobData.pbData, m_blobData.cbData, &dwWritten, NULL ) )
        {
            DWORD dwErr = ::GetLastError();
            _RPTF1( 0 , "Error WriteFile: hr = 0x%08X\n" , dwErr );
            
            return HRESULT_FROM_WIN32(dwErr);
        }

        ATLASSERT( dwWritten == m_blobData.cbData );

		if( _bFlushBuffers )
		{
			BOOL bOK = ::FlushFileBuffers( hFile );
			ATLASSERT( bOK );
		}

        VERIFY( ::CloseHandle( hFile ) );

        return S_OK;
    }

    HRESULT LoadFromFile( LPCTSTR _lpFileName, DWORD dwZeroPostfix = 0, DWORD dwSize = 0 )
    {

        HANDLE hFile = ::CreateFile( _lpFileName, 
                                     GENERIC_READ, 
                                     FILE_SHARE_READ | FILE_SHARE_WRITE, 
                                     NULL, 
                                     OPEN_EXISTING, 
                                     FILE_ATTRIBUTE_NORMAL, 
                                     NULL );

        if( hFile == INVALID_HANDLE_VALUE )
        {
			DWORD dwErr = ::GetLastError();
            _RPTF1( 0 , "Error CreateFile : hr = 0x%08X\n" , dwErr );
            
            return HRESULT_FROM_WIN32(dwErr);
        }

		DWORD cbFileSize = ::GetFileSize( hFile, NULL );
        DWORD cbSize = dwSize ? dwSize : cbFileSize;
        HRESULT hr = InternalAlloc( cbSize + dwZeroPostfix );
        ATLASSERT( SUCCEEDED( hr ) );
        if( FAILED( hr ) ) return hr;

		if( dwZeroPostfix )
			MemSet( 0, 0, cbSize + dwZeroPostfix );
		
        DWORD dwRead = 0 ;
        if( !::ReadFile( hFile, m_blobData.pbData, MIN( cbFileSize, cbSize ), &dwRead, NULL ) )
        {
            DWORD dwErr = ::GetLastError();
            _RPTF1( 0 , "Error ReadFile : hr = 0x%08X\n" , dwErr );
            
            return HRESULT_FROM_WIN32(dwErr);
        }

        ATLASSERT( dwRead == MIN( cbFileSize, cbSize ) );
		Resize(dwRead);

        VERIFY( ::CloseHandle( hFile ) );

        return S_OK;
    }

	LPCWSTR SaveToStringHEX( CComBSTR& _cbsString, int _nStrWidth = 96 )
	{
		_cbsString.Empty();

		if( m_blobData.cbData == 0 )
			return NULL;

		static const char s_chHexChars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
			'A', 'B', 'C', 'D', 'E', 'F'};

		char* pszValue = new char[m_blobData.cbData * 4 + 1];
		char* psz = pszValue;
		for( int i = 0; i < (int)m_blobData.cbData; i++ )
		{
			*psz++ = s_chHexChars[m_blobData.pbData[i] >> 4];
			*psz++ = s_chHexChars[m_blobData.pbData[i] & 0xF];

			if( _nStrWidth > 0 && (i + 1) % _nStrWidth == 0 )
				*psz++ = '\n';
		}

		*psz = '\0';

		_cbsString = pszValue;
		delete [] pszValue;

		return _cbsString;
	}

	inline char GetHexValueW(LPCWSTR& psz)
	{
		while( *psz != 0 )
		{
			if (*psz >= L'0' && *psz <= L'9')
				return (*psz - L'0');
			if (*psz >= L'A' && *psz <= L'F')
				return (*psz - L'A' + 10);
			if (*psz >= L'a' && *psz <= L'f')
				return (*psz - L'a' + 10);

			psz++;
		}

		return -1;
	}

	int LoadFromStringHEX( LPCWSTR _pszSting, int nStrLen = 0 )
	{
		if( !_pszSting )
		{
			Clear();
			return -1;
		}

		if( nStrLen <= 0 )
			nStrLen = lstrlenW( _pszSting );

		HRESULT hr = AllocMem( nStrLen );
		if( FAILED( hr ) )
			return -1;

		m_blobData.cbData = 0;
		LPBYTE pbDest = m_blobData.pbData;

		LPCWSTR psz = _pszSting;
		while( *psz && psz < _pszSting + nStrLen )
		{
			char ch1 = GetHexValueW( _pszSting );
			_pszSting++;
			char ch2 = GetHexValueW( _pszSting );
			_pszSting++;
			if( ch1 < 0 || ch2 < 0 )
				break;

			*pbDest++ = (BYTE)(ch1 << 4 | ch2);
			m_blobData.cbData++;
		}
		
		return m_blobData.cbData;
	}

    inline BYTEBLOB* operator&()
    {
        return &m_blobData;
    }

	inline CBLOBHolder* GetOwnPtr()
	{
		return this;
	}

    inline operator BYTEBLOB() const
    {
        return m_blobData;
    }

//     inline const BYTEBLOB* operator&() const
//     {
//         return &m_blobData;
//     }
    inline bool operator!() const
    {
        return !(bool)(*this);
    }

    inline operator bool() const
    {
        if( !m_blobData.cbData )
        {
            return false;
        }
        else
        {
            ATLASSERT( m_blobData.pbData );
            return true;
        }
    }

	inline bool IsEqual(const CBLOBHolder& _crCompareBlob) const
	{
		if (m_blobData.cbData != _crCompareBlob.m_blobData.cbData) return false;

		if (memcmp(m_blobData.pbData, _crCompareBlob.m_blobData.pbData, m_blobData.cbData) == 0) return true;

#if defined(_DEBUG)
		m_pbDifferDataOne = m_blobData.pbData;
		m_pbDifferDataTwo = _crCompareBlob.m_blobData.pbData;
		m_nDiffShift = 0;
		while (m_nDiffShift < m_blobData.cbData && (*m_pbDifferDataOne++ == *m_pbDifferDataTwo++))
		{
			m_nDiffShift++;
		}
#endif // defined(_DEBUG)

		return false;
	}

    inline bool operator == ( const CBLOBHolder& _crCompareBlob ) const
    {
		return IsEqual(_crCompareBlob);
    }

	inline bool operator != (const CBLOBHolder& _crCompareBlob) const
	{
		return !IsEqual(_crCompareBlob);
	}
	

	inline HRESULT SetWritePos( DWORD _dwPos )
	{
		return ReallocMem( _dwPos );
	}

	inline HRESULT SetReadPos( DWORD _dwPos )
	{
		return Resize( _dwPos );
	}

	inline DWORD GetReadWritePos()
	{
		return GetSize();
	}

	// Method for emulate save/load like stream
	HRESULT Write( const void* _pvData, DWORD _cbData, DWORD* _pcbWritten = NULL )
	{
		HRESULT hr = MemAppend( _pvData, _cbData, NULL, 1024 * 100 ); // Minimum 100 Kb appends (for better performance)
		if( FAILED( hr ) )
			return hr;

		if( _pcbWritten )
			*_pcbWritten = _cbData;

		return S_OK;
	}

	template<class T>
	HRESULT WriteT( const T* _pvData, DWORD* _pcbWritten = NULL )
	{
		HRESULT hr = MemAppend( (void*)_pvData, sizeof(T), NULL, 1024 * 100 ); // Minimum 100 Kb appends (for better performance)
		if( FAILED( hr ) )
			return hr;

		if( _pcbWritten )
			*_pcbWritten = sizeof(T);

		return S_OK;
	}

	template<class T>
	HRESULT WriteVal( const T& _rData, DWORD* _pcbWritten = NULL )
	{
		HRESULT hr = MemAppend( (void*)&_rData, sizeof(T), NULL, 1024 * 100 ); // Minimum 100 Kb appends (for better performance)
		if( FAILED( hr ) )
			return hr;

		if( _pcbWritten )
			*_pcbWritten = sizeof(T);

		return S_OK;
	}

	HRESULT WriteString( LPCWSTR _pszString )
	{
		
		DWORD cbSize = _pszString ? (lstrlenW( _pszString ) + 1) * sizeof(WCHAR) : 0;
		HRESULT hr = Write( &cbSize, sizeof(cbSize) );
		if( FAILED( hr ) )
			return hr;

		return Write( _pszString, cbSize );
	}

	HRESULT Read( void* _pvData, DWORD _cbData, DWORD* _pcbRead = NULL )
	{
		DWORD cbRead = MIN( _cbData, GetFreeSize() );
		if( cbRead == 0 )
			return HRESULT_FROM_WIN32( ERROR_HANDLE_EOF );

		::CopyMemory( _pvData, EndPtr(), cbRead );
		IncreaseSize( cbRead );
		if( _pcbRead )
			*_pcbRead = cbRead;
		return S_OK;
	}

	template<class T>
	HRESULT ReadT( T* _pvData, DWORD* _pcbRead = NULL )
	{
		DWORD cbRead = MIN( sizeof(T), GetFreeSize() );
		if( cbRead == 0 )
			return HRESULT_FROM_WIN32( ERROR_HANDLE_EOF );

		::CopyMemory( _pvData, EndPtr(), cbRead );
		IncreaseSize( cbRead );
		if( _pcbRead )
			*_pcbRead = cbRead;
		return S_OK;
	}

	HRESULT ReadString( CComBSTR& _cbsString )
	{
		DWORD cbSize = 0;
		HRESULT hr = Read( &cbSize, sizeof(cbSize) );
		if( FAILED( hr ) )
			return hr;

		if( cbSize )
			_cbsString = (LPCWSTR)EndPtr();
		else
			_cbsString.Empty();

		IncreaseSize( cbSize );

		return S_OK;
	}

private:
    inline const CBLOBHolder* Addr() const 
    {
        return this;
    }

    inline HRESULT InternalAlloc( DWORD _dwSize )
    {
		ATLASSERT( m_dwAllocSize < 1 * 1024 * 1024 * 1024 );
        m_blobData.cbData = _dwSize;
        if( m_dwAllocSize < _dwSize )
        {
            TAllocator::Free( m_blobData.pbData, m_dwAllocSize );
            m_dwAllocSize = m_blobData.cbData;
            m_blobData.pbData = TAllocator::Alloc( m_dwAllocSize );

            if( !m_blobData.pbData )
            {
                Zero_Members();
                return E_OUTOFMEMORY;
            }

			return S_OK;
        }

        return S_FALSE;
    }

	
	// Return: 
	// S_FALSE -> already allocated data is bigger -> do not change data_offset - SIZE is _dwDataSize + dwDataOffset
	// S_OK -> Buffer realloced AND data copied FROM data_offset, new data_offset is Zero !!!! SIZE is _dwDataSize
	// Added _nDataOffset -> the start of valid data
	inline HRESULT InternalRealloc( IN DWORD _dwDataSize, IN OUT DWORD* _pdwDataOffset )
    {
		ATLASSERT( m_dwAllocSize >= m_blobData.cbData );
		DWORD dwDataOffset = _pdwDataOffset ? *_pdwDataOffset : 0;
		ATLASSERT( dwDataOffset <= m_blobData.cbData );
		dwDataOffset = MIN( dwDataOffset, m_blobData.cbData );

		DWORD cbDataHave = m_blobData.cbData - dwDataOffset;

		// Check alloc zero & and zero data (reset offset in this case)
		if( _dwDataSize + dwDataOffset <= m_dwAllocSize && 
			_dwDataSize > 0 && 
			cbDataHave > 0  )
		{
			m_blobData.cbData = _dwDataSize + dwDataOffset;

			return S_FALSE; // Do nothing, NOT change offset
		}

		if( _dwDataSize > m_dwAllocSize )
		{
			// WARNING !!! Alloc size could be changed (increased) during alloc
			DWORD dwAllocSize = _dwDataSize;
			LPBYTE pbNewData = TAllocator::Alloc( dwAllocSize );
			if( !pbNewData )
			{
				TAllocator::Free( m_blobData.pbData, m_dwAllocSize );

				Zero_Members();
				return E_OUTOFMEMORY;
			}

			if( cbDataHave )
			{
				ATLASSERT( m_blobData.pbData );
				ATLASSERT( cbDataHave < _dwDataSize );
				::CopyMemory( pbNewData, m_blobData.pbData, cbDataHave );
			}

			TAllocator::Free( m_blobData.pbData, m_dwAllocSize );
			m_blobData.pbData = pbNewData;
			m_dwAllocSize = dwAllocSize;
		}
		else
		{
			// Do not need to reallocate mem -> just move to start
			if( cbDataHave > 0 )
				::MoveMemory( m_blobData.pbData, m_blobData.pbData + dwDataOffset, cbDataHave );
		}

		m_blobData.cbData = _dwDataSize;
		if( _pdwDataOffset )
			*_pdwDataOffset = 0;
		
		return S_OK;
    }



};

// Special class for using in buffers
template <class TAllocator> 
class CBlobBufferX
	: public CBLOBHolder<TAllocator>
{
public:
	CBlobBufferX() {
	}

	CBlobBufferX( DWORD _cbSize ) {
		HRESULT hr = AllocMem( _cbSize );
		ATLASSERT( SUCCEEDED( hr ) );
		Resize( 0 );
	}

	inline HRESULT Alloc( DWORD _cbSize )	{
		HRESULT hr = CBLOBHolder<TAllocator>::AllocMem( _cbSize );
		ATLASSERT( SUCCEEDED( hr ) );
		if( FAILED( hr ) ) return hr;

		Resize( 0 );
		return S_OK;
	}

	inline HRESULT AppendData( LPVOID _pvData, DWORD _cbData )	{
		return CBLOBHolder<TAllocator>::MemAppend( _pvData, _cbData );
	}

	inline operator LPCVOID () const {
		return CBLOBHolder<TAllocator>::Ptr();
	}

	inline LPVOID GetWritePtr( DWORD _cbWrite ) {
		LPVOID pvRes = m_blobData.pbData + m_blobData.cbData;
		CBLOBHolder<TAllocator>::Resize( CBLOBHolder<TAllocator>::GetSize() + _cbWrite );
		
		return pvRes;
	}

	inline HRESULT SetWritePtr( DWORD _cbShift ) {
		return CBLOBHolder<TAllocator>::Resize( _cbShift );
	}

	inline DWORD GetBufferedSize() const {
		return CBLOBHolder<TAllocator>::GetSize();
	}

	inline DWORD GetBufferSize() const {
		return CBLOBHolder<TAllocator>::GetAllocSize();
	}

	inline DWORD GetRestSize() const {
		return (CBLOBHolder<TAllocator>::GetAllocSize() - CBLOBHolder<TAllocator>::GetSize());
	}

	inline bool IsFull() const {
		return (CBLOBHolder<TAllocator>::GetAllocSize() == CBLOBHolder<TAllocator>::GetSize());
	}

	inline bool IsEmpty() const {
		return (CBLOBHolder<TAllocator>::GetSize() == 0);
	}

	inline void Clear() {
		CBLOBHolder<TAllocator>::Clear();
	}

	inline HRESULT ShiftLeft( DWORD _dwShift )
	{
		ATLASSERT( _dwShift <= CBLOBHolder<TAllocator>::GetSize() );
		if( _dwShift > CBLOBHolder<TAllocator>::GetSize() ) return false;

		HRESULT hr = CBLOBHolder<TAllocator>::MemMove( m_blobData.pbData + _dwShift, CBLOBHolder<TAllocator>::GetSize() - _dwShift );
		ATLASSERT( SUCCEEDED( hr ) );

		hr = CBLOBHolder<TAllocator>::Resize( CBLOBHolder<TAllocator>::GetSize() - _dwShift );
		ATLASSERT( SUCCEEDED( hr ) );

		return hr;
	}
};

template< int TTotalSize, int TSegnemtSize >
class CSegmentAllocX
{
	template< int TTotalSize, int TSegnemtSize >
	class CSegmentPool
	{
		CSegmentPool()
			: m_pbMemory( NULL )
		{
			::InitializeCriticalSection( &m_csProtect );
		}
		~CSegmentPool()
		{
			Uninitialize();

			::DeleteCriticalSection( &m_csProtect )
		}

		inline LPBYTE GetBuffer()
		{
			::EnterCriticalSection( &m_csProtect );

			// Check for initialization
			if( !m_ppbFreeStart )
			{
				// If initialization failed m_ppbFreeCur will be NULL
				Initialize();
			}

			// Get free buffer
			LPBYTE pbBuffer = m_ppbFreeCur ? (*m_ppbFreeCur) : NULL;

			// Move next free buffer pointer
			if( m_ppbFreeCur )
			{
				if( m_ppbFreeCur == m_ppbFreeStart )
				{
					// End of free buffer reached
					m_ppbFreeCur = NULL;
				}
				else
				{
					// Move pointer to next free buffer pointer
					m_ppbFreeCur--;
				}
			}

			::LeaveCriticalSection( &m_csProtect );

			return pbBuffer;
		}

		inline void FreeBuffer( LPBYTE _pbBuffer )
		{
			::EnterCriticalSection( &m_csProtect );

			if( m_ppbFreeCur )
			{
				m_ppbFreeCur++;
			}
			else
			{
				m_ppbFreeCur = m_ppbFreeStart;
			}

			(*m_ppbFreeCur) = _pbBuffer;

			::LeaveCriticalSection( &m_csProtect );
		}
	private:

		bool Initialize()
		{
			// Allocate memory for pool
			ATLASSERT( m_pbMemory == NULL );
			m_pbMemory = (LPBYTE)::VirtualAlloc( NULL, TTotalSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE );
			if( !m_pbMemory ) return false;

			// Allocate memory for the free buffers pointers
			DWORD cbSegments = TTotalSize / TSegnemtSize;
			m_ppbFreeStart = new LPBYTE[ cbSegments ];
			ATLASSERT( m_ppbFreeStart );

			// Fill free buffer pointers
			LPBYTE pbTemp = m_pbMemory;
			for( int i = 0; i < cbSegments; i++ )
			{
				m_ppbFreeStart[ i ] = pbTemp;
				pbTemp += TSegnemtSize;
			}

			// Set pointer to the next free buffer to the 
			// end of free buffers sequence
			m_ppbFreeCur = m_ppbFreeStart + cbSegments - 1;
			return true;
		}

		void Uninitialize()
		{
			::EnterCriticalSection( &m_csProtect );

			if( m_ppbFreeStart )
			{
				// Check for all segents was freed
				ATLASSERT( m_ppbFreeCur = m_ppbFreeStart + (TTotalSize / TSegnemtSize) - 1 );

				delete[] m_ppbFreeStart;
				m_ppbFreeStart = NULL;
				m_ppbFreeCur = NULL;

				BOOL bSuccess = ::VirtualFree( m_pbMemory, 0, MEM_RELEASE );
				ATLASSERT( bSuccess );
				m_pbMemory = NULL;
			}
			else
			{
				ATLASSERT( m_pbMemory == NULL );
				ATLASSERT( m_ppbFreeStart );
			}

			::LeaveCriticalSection( &m_csProtect );
		}

	private:
		// Pointer for pool memory 
		LPBYTE				m_pbMemory;
	
		// Pointer to the free buffer pointers
		LPBYTE*				m_ppbFreeStart;
		// Pointer to the next free buffer pointer
		LPBYTE*				m_ppbFreeCur;

		CRITICAL_SECTION		m_csProtect;
	};

public:
    static LPBYTE Alloc( DWORD& _rdwSize )
    {
		ATLASSERT( _rdwSize <= TSegnemtSize );

		_rdwSize = TSegnemtSize;

        return m_poolSegment.GetBuffer();
    }
    static void Free( LPBYTE _pbData, DWORD _dwSize )
    {
        if( _pbData )
        {
           m_poolSegment.FreeBuffer( _pbData );
        }
    }
    static bool IsMarshaled()
    {
        return false;
    }

private:
	static CSegmentPool<TTotalSize, TSegnemtSize>	m_poolSegment;
};

#ifdef _OBJBASE_H_
typedef CBLOBHolder<CCoTaskAlloc>   CBlobTask;
#endif

typedef CBLOBHolder<CNewAlloc>      CBlobNew;
typedef CBLOBHolder<CAlignedAlloc>  CBlobAligned;
typedef CBLOBHolder<CFakeAlloc>     CBlobFake;
typedef CBLOBHolder<CVirtualAlloc>  CBlobVirtual;
typedef CBLOBHolder<CHeapAlloc>		CBlobHeap;

typedef CAutoObj<CBLOBHolder<CHeapAlloc> >	TBlobHeapPtr;



typedef CBlobBufferX<CVirtualAlloc>  CBufferVirtualX;
typedef CBlobBufferX<CNewAlloc>		CBufferNewX;
typedef CBlobBufferX<CAlignedAlloc>  CBufferAligmentX;

#pragma warning(pop)

#endif // !defined(BLOB_H)
