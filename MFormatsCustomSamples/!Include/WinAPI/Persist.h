//---------------------------------------------------------------------------
// This file is part of the MediaLooks DirectShow Template Library (DSTL)
//
// Persist.h : classes for support of persistence
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

#ifndef __PERSIST_H_
#define __PERSIST_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
template <class T> class IPersistStreamImpl;
template <class T> class IStreamImpl;
template <class ThreadModel = CComMultiThreadModel> class CStreamObject;
template <class ThreadModel = CComMultiThreadModel> class CStreamOnMemory;
template <class ThreadModel = CComMultiThreadModel> class CStreamOnAlloc;

HRESULT __stdcall CreateStream(DWORD cbStartSize, IStream** ppStream);
HRESULT __stdcall CreateStreamOnMalloc(IStream** ppStream);
HRESULT __stdcall GetMallocFromStream(IStream* pStream, void** ppMemory, DWORD* pcbSize);

template <class T> class IPropertyBagImpl;
class CCoPropertyBag;

/////////////////////////////////////////////////////////////////////////////
struct PERSIST_ENTRY
{
	DWORD	cbSize;
	DWORD	cbOffset;
};

#define BEGIN_PERSIST_MAP(theClass) \
	typedef theClass _PersistMapClass; \
	static PERSIST_ENTRY* GetPersistMap() \
	{ \
		static PERSIST_ENTRY arrPersist[] = \
		{

#define PERSIST_MEMBER(member) \
			{ sizeof(((_PersistMapClass*)0)->member), offsetof(_PersistMapClass, member) },

#define END_PERSIST_MAP() \
			{0, 0} \
		}; \
		return arrPersist; \
	}

/////////////////////////////////////////////////////////////////////////////
// IPersistStreamImpl
#ifndef TRACELEVEL_IPersistStreamImpl
	#define TRACELEVEL_IPersistStreamImpl		10
#endif

template <class T>
class ATL_NO_VTABLE IPersistStreamImpl : public IPersistStream
{
// For COM-oblects:
//	COM_INTERFACE_ENTRY(IPersist)
//	COM_INTERFACE_ENTRY(IPersistStream)

// For filters:
//	COM_INTERFACE_ENTRY2(IPersist, IMediaFilter)
//	COM_INTERFACE_ENTRY(IPersistStream)

public:

	bool	m_bDirty;

	//---------------------------------
	virtual LPCTSTR GetClassName() const { return _T("IPersistStreamImpl"); }

	IPersistStreamImpl()
	{
		m_bDirty = true;
	}

	// IPersist
	STDMETHOD(GetClassID)(CLSID *pClassID)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IPersistStreamImpl,
			_T("%s: IPersistStream::GetClassID()\n"), GetClassName());

		ATLASSERT(pClassID != NULL);
		if(pClassID == NULL) return E_INVALIDARG;

		*pClassID = T::GetObjectCLSID();
		return S_OK;
	}

	// IPersistStream
	STDMETHOD(IsDirty)()
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IPersistStreamImpl,
			_T("%s: IPersistStream::IsDirty()\n"), GetClassName());
		return (m_bDirty)? S_OK : S_FALSE;
	}
	STDMETHOD(Load)(LPSTREAM pStm)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IPersistStreamImpl,
			_T("%s: IPersistStream::Load()\n"), GetClassName());

		ATLASSERT(pStm != NULL);
		if(pStm == NULL) return E_INVALIDARG;

		//---------------------------------
		T* pT = static_cast<T*>(this);
		T::ObjectLock lock(pT);

		PERSIST_ENTRY* pMap = T::GetPersistMap();
		ATLASSERT(pMap != NULL);

		HRESULT hr = S_OK;
		ULONG nRead = 0;
		for(int i = 0; pMap[i].cbSize != 0; ++i)
		{
			void* pData = (void*)((BYTE*)pT + pMap[i].cbOffset);
			hr = pStm->Read(pData, pMap[i].cbSize, &nRead);
			if(hr != S_OK || nRead != pMap[i].cbSize)
				return FAILED(hr)? hr : E_FAIL;
		}

		m_bDirty = false;
		return S_OK;
	}
	STDMETHOD(Save)(LPSTREAM pStm, BOOL fClearDirty)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IPersistStreamImpl,
			_T("%s: IPersistStream::Save()\n"), GetClassName());

		ATLASSERT(pStm != NULL);
		if(pStm == NULL) return E_INVALIDARG;

		//---------------------------------
		T* pT = static_cast<T*>(this);
		T::ObjectLock lock(pT);

		PERSIST_ENTRY* pMap = T::GetPersistMap();
		ATLASSERT(pMap != NULL);

		HRESULT hr = S_OK;
		ULONG nWritten = 0;
		for(int i = 0; pMap[i].cbSize != 0; ++i)
		{
			void* pData = (void*)((BYTE*)pT + pMap[i].cbOffset);
			hr = pStm->Write(pData, pMap[i].cbSize, &nWritten);
			if(hr != S_OK || nWritten != pMap[i].cbSize)
				return FAILED(hr)? hr : E_FAIL;
		}

		if(fClearDirty)
			m_bDirty = false;
		return S_OK;
	}
	STDMETHOD(GetSizeMax)(ULARGE_INTEGER FAR* pcbSize)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IPersistStreamImpl,
			_T("%s: IPersistStream::GetSizeMax()\n"), GetClassName());

		ATLASSERT(pcbSize != NULL);
		if(pcbSize == NULL) return E_POINTER;

		//---------------------------------
		PERSIST_ENTRY* pMap = T::GetPersistMap();
		ATLASSERT(pMap != NULL);

		pcbSize->QuadPart = 0;
		for(int i = 0; pMap[i].cbSize != 0; ++i)
			pcbSize->QuadPart += (ULONGLONG)pMap[i].cbSize;

		return S_OK;
	}
};

/////////////////////////////////////////////////////////////////////////////
// Some implementations of IStream interface

#ifndef TRACELEVEL_IStreamImpl
	#define TRACELEVEL_IStreamImpl	8
#endif

template <class T>
class ATL_NO_VTABLE IStreamImpl : public IStream
{
//	COM_INTERFACE_ENTRY(ISequentialStream)
//	COM_INTERFACE_ENTRY(IStream)
public:
	virtual LPCTSTR GetClassName() const { return _T(""); }

	IStreamImpl() : m_ulPosition(0) {}

	//---------------------------------
	// ISequentialStream
	STDMETHOD(Read)(OUT void* pv, IN ULONG cb, OUT ULONG* pcbRead)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IStreamImpl,
			_T("%s: IStreamWrapper::Read( cb=%u )\n"), GetClassName(), cb);

		if(pcbRead != NULL) *pcbRead = 0;

		ATLASSERT(pv != NULL);
		if(pv == NULL) return STG_E_INVALIDPOINTER;

		if(cb == 0) return S_OK;

		//---------------------------------
		T* pT = static_cast<T*>(this);
		T::ObjectLock lock(pT);

		ULONG cbRead = 0;
		HRESULT hr = pT->ReadData((BYTE*)pv, m_ulPosition, cb, &cbRead);
		if(SUCCEEDED(hr))
		{
			m_ulPosition += cbRead;
			if(pcbRead != NULL) *pcbRead = cbRead;
		}
		return (cbRead == cb)? S_OK : S_FALSE;
	}
	STDMETHOD(Write)(IN const void* pv, IN ULONG cb, OUT ULONG* pcbWritten)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IStreamImpl,
			_T("%s: IStreamWrapper::Write( cb=%u )\n"), GetClassName(), cb);

		if(pcbWritten != NULL) *pcbWritten = 0;

		ATLASSERT(pv != NULL);
		if(pv == NULL) return STG_E_INVALIDPOINTER;

		if(cb == 0) return S_OK;

		//---------------------------------
		T* pT = static_cast<T*>(this);
		T::ObjectLock lock(pT);

		ULONG cbWritten = 0;
		HRESULT hr = pT->WriteData((BYTE*)pv, m_ulPosition, cb, &cbWritten);
		if(SUCCEEDED(hr))
		{
			m_ulPosition += cbWritten;
			if(pcbWritten != NULL) *pcbWritten = cbWritten;
		}
		return hr;
	}

	//---------------------------------
	// IStream
	STDMETHOD(Seek)(IN LARGE_INTEGER dlibMove, IN DWORD dwOrigin,
					OUT ULARGE_INTEGER* plibNewPosition)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IStreamImpl,
			_T("%s: IStreamWrapper::Seek( dlibMove=%I64u, dwOrigin=%u )\n"),
			GetClassName(), dlibMove, dwOrigin);

		ATLASSERT(dwOrigin==STREAM_SEEK_SET || dwOrigin==STREAM_SEEK_CUR || dwOrigin==STREAM_SEEK_END);
		if(dwOrigin != STREAM_SEEK_SET && dwOrigin != STREAM_SEEK_CUR && dwOrigin != STREAM_SEEK_END)
			return STG_E_INVALIDFUNCTION;

		//---------------------------------
		T* pT = static_cast<T*>(this);
		T::ObjectLock lock(pT);

		LONGLONG llShift = dlibMove.QuadPart;
		if(dwOrigin == STREAM_SEEK_CUR)
			llShift += (LONGLONG)m_ulPosition;
		else if(dwOrigin == STREAM_SEEK_END)
			llShift += pT->GetSize();

		ATLASSERT(llShift >= (LONGLONG)0);
		if(llShift < (LONGLONG)0)
			// For STREAM_SEEK_SET dlibMove is unsigned, but this class accepts only
			// DWORD sizes/positions and if dlibMove < 0 then (ULONGLONG)llShift > MAXDWORD
			return STG_E_INVALIDFUNCTION;

		ATLASSERT((ULONGLONG)llShift <= (ULONGLONG)(ULONG)MAXDWORD);
		if((ULONGLONG)llShift > (ULONGLONG)(ULONG)MAXDWORD) return E_UNEXPECTED;

		m_ulPosition = (ULONG)llShift;

		if(plibNewPosition != NULL)
			plibNewPosition->QuadPart = (ULONGLONG)m_ulPosition;
		return S_OK;
	}

	STDMETHOD(SetSize)(IN ULARGE_INTEGER libNewSize)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IStreamImpl,
			_T("%s: IStreamWrapper::SetSize( libNewSize=%I64u )\n"), GetClassName(), libNewSize);

		ATLASSERT(libNewSize.QuadPart <= (ULONGLONG)(ULONG)MAXDWORD);
		if(libNewSize.QuadPart > (ULONGLONG)(ULONG)MAXDWORD) return STG_E_INVALIDFUNCTION;

		//---------------------------------
		T* pT = static_cast<T*>(this);
		T::ObjectLock lock(pT);

		ULONG ulNewSize = (ULONG)libNewSize.QuadPart;
		return pT->ChangeSize(ulNewSize);
	}

	// This method copies the specified bytes from one stream to another.
	// It can also be used to copy a stream to itself.
	STDMETHOD(CopyTo)(IN IStream* pstm, IN ULARGE_INTEGER cb,
					  OUT ULARGE_INTEGER* pcbRead, OUT ULARGE_INTEGER* pcbWritten)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IStreamImpl,
			_T("%s: IStreamWrapper::CopyTo( cb=%I64u )\n"), GetClassName(), cb);

		ATLASSERT(pstm != NULL);
		if(pstm == NULL) return STG_E_INVALIDPOINTER;

		if(pcbRead != NULL) pcbRead->QuadPart = (ULONGLONG)0;
		if(pcbWritten != NULL) pcbWritten->QuadPart = (ULONGLONG)0;

		if(cb.QuadPart == (ULONGLONG)0) return S_OK;

		//---------------------------------
		T* pT = static_cast<T*>(this);
		T::ObjectLock lock(pT);

		ATLASSERT((ULONGLONG)m_ulPosition + cb.QuadPart <= (ULONGLONG)(ULONG)MAXDWORD);
		if((ULONGLONG)m_ulPosition + cb.QuadPart > (ULONGLONG)(ULONG)MAXDWORD) return E_UNEXPECTED;

		ULONG cbRead = (ULONG)cb.QuadPart;
		ULONG cbWritten = 0;
		HRESULT hr = pT->CopyToStream(pstm, m_ulPosition, cbRead, &cbWritten);
		if(FAILED(hr))
			return hr;

		m_ulPosition += cbWritten;

		if(pcbRead != NULL)
			pcbRead->QuadPart = (ULONGLONG)cbWritten;
		if(pcbWritten != NULL)
			pcbWritten->QuadPart = (ULONGLONG)cbWritten;
		return (cbRead == cbWritten)? S_OK : S_FALSE;
	}

	STDMETHOD(Commit)(IN DWORD grfCommitFlags)
	{
		grfCommitFlags;
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IStreamImpl,
			_T("%s: IStreamWrapper::Commit( grfCommitFlags=0x%X )\n"), GetClassName(), grfCommitFlags);
		return S_OK;
	}
	STDMETHOD(Revert)()
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IStreamImpl,
			_T("%s: IStreamWrapper::Revert\n"), GetClassName());
		return E_NOTIMPL;
	}
	STDMETHOD(LockRegion)(IN ULARGE_INTEGER libOffset, IN ULARGE_INTEGER cb,
						  IN DWORD dwLockType)
	{
		libOffset, cb, dwLockType;
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IStreamImpl,
			_T("%s: IStreamWrapper::LockRegion( libOffset=%I64u, cb=%I64u, dwLockType=0x%X )\n"),
			GetClassName(), libOffset, cb, dwLockType);
		return STG_E_INVALIDFUNCTION;
	}
	STDMETHOD(UnlockRegion)(IN ULARGE_INTEGER libOffset, IN ULARGE_INTEGER cb,
							IN DWORD dwLockType)
	{
		libOffset, cb, dwLockType;
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IStreamImpl,
			_T("%s: IStreamWrapper::UnlockRegion( libOffset=%I64u, cb=%I64u, dwLockType=0x%X )\n"),
			GetClassName(), libOffset, cb, dwLockType);
		return STG_E_INVALIDFUNCTION;
	}

	STDMETHOD(Stat)(OUT STATSTG* pstatstg, IN DWORD grfStatFlag)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IStreamImpl,
			_T("%s: IStreamWrapper::Stat\n"), GetClassName());

		ATLASSERT(pstatstg != NULL);
		if(pstatstg == NULL) return STG_E_INVALIDPOINTER;

		ATLASSERT(grfStatFlag == STATFLAG_DEFAULT || grfStatFlag == STATFLAG_NONAME);
		if(grfStatFlag != STATFLAG_DEFAULT && grfStatFlag != STATFLAG_NONAME) return STG_E_INVALIDFLAG;

		//---------------------------------
		T* pT = static_cast<T*>(this);
		T::ObjectLock lock(pT);

		memset(pstatstg, 0, sizeof(STATSTG));
		pstatstg->type = STGTY_LOCKBYTES;
		pstatstg->cbSize.QuadPart = (ULONGLONG)pT->GetSize();
		return S_OK;
	}
	STDMETHOD(Clone)(OUT IStream** /*ppstm*/)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IStreamImpl,
			_T("%s: IStreamWrapper::Clone\n"), GetClassName());
		return E_NOTIMPL;
	}

	//---------------------------------
	ULONG	m_ulPosition;
};

//---------------------------------
MIDL_INTERFACE("9039B690-2744-48D0-A7ED-7A3344EF9F9A")
IStreamMemory : public IUnknown
{
public:
	STDMETHOD(GetMemory)(OUT BLOB* pBlob) PURE;
};

//---------------------------------
// Base pure virtual class for stream allocators
class ATL_NO_VTABLE CStreamAllocator : public IUnknown
{
public : 
	STDMETHOD(GetMemory)(OUT BLOB* pBlob) PURE;
	STDMETHOD(ChangeSize)(IN ULONG ulWanted) PURE;
	STDMETHOD(ReadData)(OUT BYTE* pBuf, IN ULONG cbPosition, IN ULONG cb, OUT ULONG* pcbRead) PURE;
	STDMETHOD(WriteData)(IN BYTE* pBuf, IN ULONG cbPosition, IN ULONG cb, OUT ULONG* pcbWritten) PURE;
	STDMETHOD(CopyToStream)(IN IStream* pstm, IN ULONG cbPosition, IN ULONG cb, OUT ULONG* pcbWritten) PURE;
};

//---------------------------------
template <class ThreadModel /*= CComMultiThreadModel*/>
class ATL_NO_VTABLE CStreamObject : 
	public CComObjectRootEx<ThreadModel>,
	public CComCoClass<CStreamObject<ThreadModel> >,
	public IStreamImpl<CStreamObject<ThreadModel> >,
	public IStreamMemory,
	public CStreamAllocator
{
	CComPtr<IUnknown> m_pUnkMarshaler;
	CStreamAllocator* m_pAllocator;

	//---------------------------------
public:
	LPCTSTR GetClassName() const { return _T("CStreamObject"); }

	CStreamObject()
	{
		m_pUnkMarshaler = NULL;
		m_pAllocator = NULL;
	}

	//---------------------------------
	static HRESULT CreateStream(IStream** ppStream, CStreamAllocator* pAllocator, ULONG ulPosition = 0)
	{
		ATLASSERT(ppStream != NULL);
		if(ppStream == NULL) return E_POINTER;
		ATLASSERT(pAllocator != NULL);
		if(pAllocator == NULL) return E_INVALIDARG;

		//---------------------------------
		CStreamObject<ThreadModel>* pStream = NULL;
		HRESULT hr = CreateComObject(&pStream);
		if(SUCCEEDED(hr))
		{
			pStream->InitStream(pAllocator, ulPosition);
			hr = pStream->QueryInterface(__uuidof(IStream), (void**)ppStream);
			ATLASSERT(*ppStream != NULL);

			pStream->Release();
		}
		return hr;
	}
	
	void InitStream(CStreamAllocator* pAllocator, ULONG ulPosition)
	{
		if(pAllocator != NULL)
			pAllocator->AddRef();

		if(m_pAllocator != NULL)
			m_pAllocator->Release();

		m_pAllocator = pAllocator;
		m_ulPosition = ulPosition;
	}

	//---------------------------------
	HRESULT FinalConstruct()
	{
		ATLTRACE2(atlTraceGeneral, 2, _T("CStreamObject::FinalConstruct\n"));
		return CoCreateFreeThreadedMarshaler(GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease()
	{
		ATLTRACE2(atlTraceGeneral, 2, _T("CStreamObject::FinalRelease\n"));
		if(m_pAllocator != NULL)
		{
			m_pAllocator->Release();
			m_pAllocator = NULL;
		}
		m_pUnkMarshaler.Release();
	}

	//---------------------------------
	DECLARE_NOT_AGGREGATABLE(CStreamObject)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CStreamObject)
		COM_INTERFACE_ENTRY(ISequentialStream)
		COM_INTERFACE_ENTRY(IStream)
		COM_INTERFACE_ENTRY(IStreamMemory)
		COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
	END_COM_MAP()

	//---------------------------------
	STDMETHOD_(ULONG, GetSize)()
	{
		ATLASSERT(m_pAllocator != NULL);

		BLOB rgBlob = {0};
		HRESULT hr = m_pAllocator->GetMemory(&rgBlob);
		return (hr == S_OK)? rgBlob.cbSize : 0;
	}
	STDMETHOD(ChangeSize)(IN ULONG ulWanted)
	{
		ATLASSERT(m_pAllocator != NULL);
		return m_pAllocator->ChangeSize(ulWanted);
	}
	STDMETHOD(ReadData)(OUT BYTE* pBuf, IN ULONG cbPosition, IN ULONG cb, OUT ULONG* pcbRead)
	{
		ATLASSERT(m_pAllocator != NULL);
		return m_pAllocator->ReadData(pBuf, cbPosition, cb, pcbRead);
	}
	STDMETHOD(WriteData)(IN BYTE* pBuf, IN ULONG cbPosition, IN ULONG cb, OUT ULONG* pcbWritten)
	{
		ATLASSERT(m_pAllocator != NULL);
		return m_pAllocator->WriteData(pBuf, cbPosition, cb, pcbWritten);
	}
	STDMETHOD(CopyToStream)(IN IStream* pstm, IN ULONG cbPosition, IN ULONG cb, OUT ULONG* pcbWritten)
	{
		ATLASSERT(m_pAllocator != NULL);
		return m_pAllocator->CopyToStream(pstm, cbPosition, cb, pcbWritten);
	}

	//---------------------------------
	STDMETHOD(Clone)(OUT IStream** ppstm)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IStreamImpl, _T("%s: Clone\n"), GetClassName());

		ObjectLock lock(this);
		return CreateStream(ppstm, m_pAllocator, m_ulPosition);
	}

	//---------------------------------
	// IStreamMemory
	STDMETHOD(GetMemory)(OUT BLOB* pBlob)
	{
		ATLASSERT(m_pAllocator != NULL);
		return m_pAllocator->GetMemory(pBlob);
	}
};

//---------------------------------
// Stream on fixed memory block: READ ONLY !
// ATTENTION : only for exchange between CLSCTX_INPROC_SERVER COM-objects !

template <class ThreadModel /*= CComMultiThreadModel*/>
class ATL_NO_VTABLE CStreamOnMemory :
	public CComObjectRootEx<ThreadModel>,
	public CComCoClass<CStreamOnMemory<ThreadModel> >,
	public CStreamAllocator
{
	BYTE*	m_pBuffer;
	ULONG	m_ulSize;

public :
	CStreamOnMemory() : m_pBuffer(NULL), m_ulSize(0) {}
	~CStreamOnMemory() {}

	//---------------------------------
	static HRESULT CreateStream(IStream** ppStream, BYTE* pBuffer, ULONG ulSize)
	{
		ATLASSERT(ppStream != NULL);
		if(ppStream == NULL) return E_POINTER;

		*ppStream = NULL;

		ATLASSERT(pBuffer != NULL);
		if(pBuffer == NULL) return E_INVALIDARG;
		ATLASSERT(ulSize != 0);
		if(ulSize == 0) return E_INVALIDARG;

		//---------------------------------
		CStreamOnMemory<ThreadModel>* pAllocObject = NULL;
		HRESULT hr = CreateComObject(&pAllocObject);
		if(SUCCEEDED(hr))
		{
			pAllocObject->InitAllocator(pBuffer, ulSize);
			hr = CStreamObject<ThreadModel>::CreateStream(ppStream, pAllocObject);
			pAllocObject->Release();
		}
		return hr;
	}

	void InitAllocator(BYTE* pBuffer, ULONG ulSize)
	{
		ATLASSERT(pBuffer != NULL && ulSize != 0);
		m_pBuffer = pBuffer;
		m_ulSize = ulSize;
	}

	//---------------------------------
	DECLARE_NOT_AGGREGATABLE(CStreamOnMemory)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CStreamOnMemory)
		COM_INTERFACE_ENTRY(IUnknown)
	END_COM_MAP()

	//---------------------------------
	STDMETHOD(GetMemory)(OUT BLOB* pBlob)
	{
		ATLASSERT(pBlob != NULL);
		if(pBlob == NULL) return E_POINTER;

		pBlob->cbSize = m_ulSize;
		pBlob->pBlobData = m_pBuffer;
		return S_OK;
	}
	STDMETHOD(ChangeSize)(IN ULONG /*ulWanted*/)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(ReadData)(OUT BYTE* pBuf, IN ULONG cbPosition, IN ULONG cb, OUT ULONG* pcbRead)
	{
		ATLASSERT(pBuf != NULL);
		if(pBuf == NULL) return STG_E_INVALIDPOINTER;

		ATLASSERT(pcbRead != NULL);
		if(pcbRead == NULL) return E_POINTER;

		*pcbRead = 0;
		if(cb == 0) return S_OK;

		ULONG ulEnd = cbPosition + cb;
		ATLASSERT(ulEnd >= cbPosition && _T("Stream allocator overfloat"));
		if(ulEnd < cbPosition) return E_UNEXPECTED;

		//---------------------------------
		ObjectLock lock(this);
		if(m_pBuffer == NULL || cbPosition >= m_ulSize) return S_FALSE;

		if(ulEnd > m_ulSize) ulEnd = m_ulSize;

		ULONG cbRead = ulEnd - cbPosition;
		// memmove for copy a stream to itself
		memmove(pBuf, m_pBuffer + cbPosition, cbRead);

		*pcbRead = cbRead;
		return (cbRead == cb)? S_OK : S_FALSE;
	}
	STDMETHOD(WriteData)(IN BYTE* /*pBuf*/, IN ULONG /*cbPosition*/, IN ULONG /*cb*/, OUT ULONG* /*pcbWritten*/)
	{
		return E_NOTIMPL;
	}
	STDMETHOD(CopyToStream)(IN IStream* /*pstm*/, IN ULONG /*cbPosition*/, IN ULONG /*cb*/, OUT ULONG* /*pcbWritten*/)
	{
		return E_NOTIMPL;
	}
};

//---------------------------------
// malloc/realloc/free support for streams
// ATTENTION : only for exchange between CLSCTX_INPROC_SERVER COM-objects !

template <class ThreadModel /*= CComMultiThreadModel*/>
class ATL_NO_VTABLE CStreamOnAlloc :
	public CComObjectRootEx<ThreadModel>,
	public CComCoClass<CStreamOnAlloc<ThreadModel> >,
	public CStreamAllocator
{
	BYTE*	m_pBuffer;
	ULONG	m_ulSize;

public :
	CStreamOnAlloc() : m_pBuffer(NULL), m_ulSize(0) {}
	~CStreamOnAlloc()
	{
		free(m_pBuffer);
	}

	//---------------------------------
	static HRESULT CreateStream(IStream** ppStream)
	{
		ATLASSERT(ppStream != NULL);
		if(ppStream == NULL) return E_POINTER;

		*ppStream = NULL;

		//---------------------------------
		CStreamOnAlloc<ThreadModel>* pAllocObject = NULL;
		HRESULT hr = CreateComObject(&pAllocObject);
		if(SUCCEEDED(hr))
		{
			hr = CStreamObject<ThreadModel>::CreateStream(ppStream, pAllocObject);
			pAllocObject->Release();
		}
		return hr;
	}

	//---------------------------------
	DECLARE_NOT_AGGREGATABLE(CStreamOnAlloc)
	DECLARE_GET_CONTROLLING_UNKNOWN()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CStreamOnAlloc)
		COM_INTERFACE_ENTRY(IUnknown)
	END_COM_MAP()

	//---------------------------------
	STDMETHOD(GetMemory)(OUT BLOB* pBlob)
	{
		ATLASSERT(pBlob != NULL);
		if(pBlob == NULL) return E_POINTER;

		pBlob->cbSize = m_ulSize;
		pBlob->pBlobData = m_pBuffer;
		return S_OK;
	}
	STDMETHOD(ChangeSize)(IN ULONG ulWanted)
	{
		ObjectLock lock(this);

		if(ulWanted == m_ulSize) return S_OK;

		HRESULT hr = S_OK;
		if(ulWanted == 0)
		{
			free(m_pBuffer);
			m_pBuffer = NULL;
			m_ulSize = 0;
		}
		else
		{
			BYTE* pBuffer = (BYTE*)realloc(m_pBuffer, ulWanted);
			if(pBuffer == NULL)
				hr = STG_E_MEDIUMFULL;
			else
			{
				m_pBuffer = pBuffer;
				m_ulSize = ulWanted;
			}
		}
		return hr;
	}
	STDMETHOD(ReadData)(OUT BYTE* pBuf, IN ULONG cbPosition, IN ULONG cb, OUT ULONG* pcbRead)
	{
		ATLASSERT(pBuf != NULL);
		if(pBuf == NULL) return STG_E_INVALIDPOINTER;

		ATLASSERT(pcbRead != NULL);
		if(pcbRead == NULL) return E_POINTER;

		*pcbRead = 0;
		if(cb == 0) return S_OK;

		ULONG ulEnd = cbPosition + cb;
		ATLASSERT(ulEnd >= cbPosition && _T("Stream allocator overfloat"));
		if(ulEnd < cbPosition) return E_UNEXPECTED;

		//---------------------------------
		ObjectLock lock(this);
		if(m_pBuffer == NULL || cbPosition >= m_ulSize) return S_FALSE;

		if(ulEnd > m_ulSize) ulEnd = m_ulSize;

		ULONG cbRead = ulEnd - cbPosition;
		// memmove for copy a stream to itself
		memmove(pBuf, m_pBuffer + cbPosition, cbRead);

		*pcbRead = cbRead;
		return (cbRead == cb)? S_OK : S_FALSE;
	}
	STDMETHOD(WriteData)(IN BYTE* pBuf, IN ULONG cbPosition, IN ULONG cb, OUT ULONG* pcbWritten)
	{
		ATLASSERT(pBuf != NULL);
		if(pBuf == NULL) return STG_E_INVALIDPOINTER;

		ATLASSERT(pcbWritten != NULL);
		if(pcbWritten == NULL) return E_POINTER;

		*pcbWritten = 0;
		if(cb == 0) return S_OK;

		ULONG ulEnd = cbPosition + cb;
		ATLASSERT(ulEnd >= cbPosition && _T("Stream allocator overfloat"));
		if(ulEnd < cbPosition) return E_UNEXPECTED;

		//---------------------------------
		ObjectLock lock(this);

		if(ulEnd > m_ulSize)
		{
			HRESULT hr = ChangeSize(ulEnd);
			if(FAILED(hr)) return hr;
		}
		ATLASSERT(m_pBuffer != NULL);

		// memmove for copy a stream to itself
		memmove(m_pBuffer + cbPosition, pBuf, cb);
		*pcbWritten = cb;
		return S_OK;
	}
	STDMETHOD(CopyToStream)(IN IStream* pstm, IN ULONG cbPosition, IN ULONG cb, OUT ULONG* pcbWritten)
	{
		ATLASSERT(pstm != NULL);
		if(pstm == NULL) return STG_E_INVALIDPOINTER;

		ATLASSERT(pcbWritten != NULL);
		if(pcbWritten == NULL) return E_POINTER;

		*pcbWritten = 0;
		if(cb == 0) return S_OK;

		ULONG ulEnd = cbPosition + cb;
		ATLASSERT(ulEnd >= cbPosition && _T("Stream allocator overfloat"));
		if(ulEnd < cbPosition) return E_UNEXPECTED;

		//---------------------------------
		ObjectLock lock(this);
		if(m_pBuffer == NULL || cbPosition >= m_ulSize) return S_FALSE;

		if(ulEnd > m_ulSize) ulEnd = m_ulSize;

		ULONG cbWritten = 0;
		HRESULT hr = pstm->Write(m_pBuffer + cbPosition, ulEnd - cbPosition, &cbWritten);
		if(SUCCEEDED(hr))
			*pcbWritten = cbWritten;
		return hr;
	}
};

//////////////////////////////////////////////////////////////////////
// Standard implementation
inline HRESULT __stdcall CreateStream(DWORD cbStartSize, IStream** ppStream)
{
	ATLASSERT(ppStream != NULL);
	if(ppStream == NULL) return E_POINTER;

	*ppStream = NULL;

	HGLOBAL hGlob = ::GlobalAlloc(GHND, cbStartSize);
	if(hGlob == NULL)
		return ERROR_FROM_WIN32(::GetLastError(), E_FAIL);

	HRESULT hr = ::CreateStreamOnHGlobal(hGlob, TRUE, ppStream);
	if(FAILED(hr))
		::GlobalFree(hGlob);
	return hr;
}

//---------------------------------
// Implementation with usage of CStreamOnAlloc<>
// Allocated memory for this stream automatically freed when
// the last reference to the stream object is released
inline HRESULT __stdcall CreateStreamOnMalloc(IStream** ppStream)
{
	HRESULT hr = CStreamOnAlloc<CComMultiThreadModel>::CreateStream(ppStream);
	return hr;
}

inline HRESULT __stdcall GetMallocFromStream(IStream* pStream, void** ppMemory, DWORD* pcbSize)
{
	ATLASSERT(pStream != NULL);
	if(pStream == NULL) return E_INVALIDARG;
	ATLASSERT(ppMemory != NULL);
	if(ppMemory == NULL) return E_POINTER;
	ATLASSERT(pcbSize != NULL);
	if(pcbSize == NULL) return E_POINTER;

	*ppMemory = NULL;
	*pcbSize = 0;

	CComPtr<IStreamMemory> spStreamMem;
	HRESULT hr = pStream->QueryInterface(&spStreamMem);
	if(hr != S_OK || spStreamMem == NULL)
		return FAILED(hr)? hr : E_FAIL;

	BLOB rgBlob = {0};
	hr = spStreamMem->GetMemory(&rgBlob);
	if(hr != S_OK)
		return FAILED(hr)? hr : E_FAIL;

	*ppMemory = rgBlob.pBlobData;
	*pcbSize = rgBlob.cbSize;
	return S_OK;
}

//////////////////////////////////////////////////////////////////////
// IPropertyBagImpl

#ifndef TRACELEVEL_IPropertyBagImpl
	#define TRACELEVEL_IPropertyBagImpl	8
#endif

template <class T>
class ATL_NO_VTABLE IPropertyBagImpl : public IPropertyBag
{
public:

	CSimpleMap2<CComBSTR,CComVariant> m_mapValues;
	BOOL                              m_bDirty;

	//---------------------------------
	virtual LPCTSTR GetClassName() const { return _T("IPropertyBagImpl<>"); }

	IPropertyBagImpl() : m_bDirty( FALSE ){}
	~IPropertyBagImpl() {}

	STDMETHOD(Read)(IN LPCOLESTR pszPropName, IN OUT VARIANT* pVar, IN IErrorLog* /*pErrorLog*/)
	{
		USES_CONVERSION;
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IPropertyBagImpl, _T("%s: IPropertyBag::Read( Key=%s )\n"),
				   GetClassName(), pszPropName ? W2CT(pszPropName) : _T(" !!! NULL"));

		// Verify pointer
		ATLASSERT( pszPropName && pVar );
		if( !pszPropName || !pVar ) return E_POINTER;
    
		//---------------------------------
		T::ObjectLock lock(static_cast<T*>(this));

		// Find key
		int nIndex = m_mapValues.FindKey( CComBSTR(pszPropName) );
		if( nIndex < 0 ) return E_INVALIDARG;
    
		// Retrieve value
		CComVariant varRes = m_mapValues.GetValueAt( nIndex );
		if( pVar->vt == VT_EMPTY || pVar->vt == varRes.vt )
		{
			// Set pVar->vt to VT_EMPTY ( only this feild intialized )
			pVar->vt = VT_EMPTY;
			
			// Required type is the same or any
			return varRes.Detach( pVar );
		}

		// Convert to required type
		HRESULT hr = varRes.ChangeType( pVar->vt );
		if( FAILED( hr ) ) return E_FAIL;

		// Set pVar->vt to VT_EMPTY ( only this feild intialized )
		pVar->vt = VT_EMPTY;
    
		return varRes.Detach( pVar );
	}

	STDMETHOD(Write)(IN LPCOLESTR pszPropName, IN VARIANT* pVar)
	{
#if defined(_DEBUG)
		{
			USES_CONVERSION;
        
			CComVariant varValue;
			if( pVar )
			{
				varValue = *pVar;
				if( FAILED( varValue.ChangeType( VT_BSTR ) ) )
					varValue = L"Unknown type";
			}
        
			ATLTRACE2(atlTraceCOM, TRACELEVEL_IPropertyBagImpl,
				_T("%s: IPropertyBag::Write( Key=%s , Value=%s)\n"),
				GetClassName(),
				pszPropName ? W2CT(pszPropName) : _T(" !!! NULL"),
				pVar ? W2T( varValue.bstrVal ) : _T(" !!! NULL"));
		}
#endif // defined(_DEBUG)
    
		// Verify pointer
		ATLASSERT( pszPropName && pVar );
		if( !pszPropName || !pVar ) return E_POINTER;

		//---------------------------------
		T::ObjectLock lock(static_cast<T*>(this));

		// Remove old key
		m_mapValues.Remove( CComBSTR(pszPropName) );
		// Add new key
		if( !m_mapValues.Add( CComBSTR(pszPropName), *pVar ) ) return E_FAIL;
    
		m_bDirty = TRUE;
		return S_OK;
	}
};

//---------------------------------
class ATL_NO_VTABLE CCoPropertyBag :
	public CComObjectRootEx<CComMultiThreadModel>,
	public IPropertyBagImpl<CCoPropertyBag>,
	public IErrorLog
{
public:
	virtual LPCTSTR GetClassName() const { return _T("CCoPropertyBag"); }

	CCoPropertyBag() {}
	~CCoPropertyBag() {}

	//---------------------------------
	static HRESULT CreatePropertyBag(IPropertyBag** ppPropertyBag)
	{
		ATLASSERT(ppPropertyBag != NULL);
		if(ppPropertyBag == NULL) return E_POINTER;

		*ppPropertyBag = NULL;

		//---------------------------------
		CCoPropertyBag* pPropBagObject = NULL;
		HRESULT hr = CreateComObject(&pPropBagObject);
		if(SUCCEEDED(hr))
		{
			hr = pPropBagObject->QueryInterface(__uuidof(IPropertyBag), (void**)ppPropertyBag);
			ATLASSERT(*ppPropertyBag != NULL);
			pPropBagObject->Release();
		}
		return hr;
	}

	//---------------------------------
	DECLARE_NO_REGISTRY()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CCoPropertyBag)
		COM_INTERFACE_ENTRY(IPropertyBag)
		COM_INTERFACE_ENTRY(IErrorLog)
	END_COM_MAP()

	// IErrorLog
	STDMETHOD(AddError)( LPCOLESTR pszPropName, EXCEPINFO *pExcepInfo)
	{
		return S_OK;
	}
};

//---------------------------------
class ATL_NO_VTABLE CCoStmPropertyBag :
	public CComObjectRootEx<CComMultiThreadModel>,
	public IPropertyBagImpl<CCoStmPropertyBag>,
    public IPersistStream
{
public:
	virtual LPCTSTR GetClassName() const { return _T("CCoStmPropertyBag"); }

	CCoStmPropertyBag() {}
	~CCoStmPropertyBag() {}

	//---------------------------------
	static HRESULT CreatePropertyBag(IPropertyBag** ppPropertyBag)
	{
		ATLASSERT(ppPropertyBag != NULL);
		if(ppPropertyBag == NULL) return E_POINTER;

		*ppPropertyBag = NULL;

		//---------------------------------
		CCoStmPropertyBag* pPropBagObject = NULL;
		HRESULT hr = CreateComObject(&pPropBagObject);
		if(SUCCEEDED(hr))
		{
			hr = pPropBagObject->QueryInterface(__uuidof(IPropertyBag), (void**)ppPropertyBag);
			ATLASSERT(*ppPropertyBag != NULL);
			pPropBagObject->Release();
		}
		return hr;
	}

	//---------------------------------
	DECLARE_NO_REGISTRY()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CCoStmPropertyBag)
		COM_INTERFACE_ENTRY(IPropertyBag)
		COM_INTERFACE_ENTRY(IPersistStream)
	END_COM_MAP()

	//---------------------------------
	// IPersist
	STDMETHOD(GetClassID)(OUT CLSID* /*pClassID*/)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IPropertyBagImpl,
			_T("%s: IPersistStream::GetClassID(): NOT IMPLEMENTED\n"), GetClassName());

		return E_NOTIMPL;
	}

	//---------------------------------
	// IPersistStream
	STDMETHOD(IsDirty)()
	{ 
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IPropertyBagImpl, _T("%s: IPersistStream::IsDirty() = %s\n"), 
			GetClassName(),m_bDirty ? _T("S_OK") : _T("S_FALSE"));

		return m_bDirty ? S_OK : S_FALSE; 
	}

	STDMETHOD(Load)(IN IStream* pStm)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IPropertyBagImpl,
			_T("%s: IPersistStream::Load( IStream [0x%08X] )\n"), GetClassName(), (DWORD_PTR)pStm);

		// Verify pointer
		ATLASSERT( pStm );
		if( !pStm ) return E_POINTER;

		m_mapValues.RemoveAll();

		DWORD dwSize = 0;
		HRESULT hr = pStm->Read( &dwSize, sizeof( dwSize ), NULL );
		ATLASSERT( SUCCEEDED( hr ) );

		//---------------------------------
		ObjectLock lock(this);
    
		for( UINT i = 0 ; i < dwSize; i++ )
		{
			CComBSTR cbsKey;
			hr = cbsKey.ReadFromStream( pStm );
			ATLASSERT( SUCCEEDED( hr ) );
			if( FAILED( hr ) ) break;

			CComVariant varValue;
			hr = varValue.ReadFromStream( pStm );
			ATLASSERT( SUCCEEDED( hr ) );
			if( FAILED( hr ) ) break;
        
			// Remove old key
			m_mapValues.Remove( cbsKey );
			// Add new key
			BOOL bSuccess = m_mapValues.Add( cbsKey, varValue );
			ATLASSERT( bSuccess );
			if( !bSuccess )
			{
				hr = E_FAIL;
				break;
			}
		}

		if( FAILED( hr ) )
		{
			m_mapValues.RemoveAll();
			return hr;
		}

		m_bDirty = FALSE;

		return S_OK;
	}

	STDMETHOD(Save)(IN IStream* pStm, IN BOOL fClearDirty)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IPropertyBagImpl,
			_T("%s: IPersistStream::Save( IStream [0x%08X], %s )\n"),
			GetClassName(), (DWORD_PTR)pStm, fClearDirty ? _T("TRUE") : _T("FALSE"));

		 // Verify pointer
		ATLASSERT( pStm );
		if( !pStm ) return E_POINTER;

		ObjectLock lock(this);

		DWORD dwSize = m_mapValues.GetSize();
		HRESULT hr = pStm->Write( &dwSize, sizeof( dwSize ), NULL );
		ATLASSERT( SUCCEEDED( hr ) );
		if( FAILED( hr ) ) return hr;
        
		for( UINT i = 0 ; i < dwSize; i++ )
		{
			hr = m_mapValues.GetKeyAt( i ).WriteToStream( pStm );
			ATLASSERT( SUCCEEDED( hr ) );
			if( FAILED( hr ) ) return hr;
                
			hr = m_mapValues.GetValueAt( i ).WriteToStream( pStm );
			ATLASSERT( SUCCEEDED( hr ) );
			if( FAILED( hr ) ) return hr;
		}

		if( fClearDirty )
			m_bDirty = FALSE;

		return S_OK;
	}

	STDMETHOD(GetSizeMax)(OUT ULARGE_INTEGER* /*pcbSize*/)
	{
		ATLTRACE2(atlTraceCOM, TRACELEVEL_IPropertyBagImpl,
			_T("%s: IPersistStream::GetSizeMax()\n"), GetClassName());
		return E_NOTIMPL;
	}
};

/////////////////////////////////////////////////////////////////////////////
#endif	 // __PERSIST_H_
