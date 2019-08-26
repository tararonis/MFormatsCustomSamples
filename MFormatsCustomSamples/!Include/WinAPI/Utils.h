//---------------------------------------------------------------------------
// Utils.h : 
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

#ifndef _WINAPI_I_UTILS_H_
#define _WINAPI_I_UTILS_H_

#include "../WinAPI/Wrappers.h"

#include "Wininet.h"
#pragma comment(lib,"Wininet.lib" )

#define INTERNET_READ_ALLOC_CHUNK (10 * 1024 * 1024 ) // (10MB)

struct InternetAction
{
	static inline void Set( HANDLE& _hTarget, const HANDLE& _hValue )
	{
		_hTarget = _hValue;
	}
	static inline HRESULT Close( const HANDLE& _hTarget ) 
	{
		if( !::InternetCloseHandle( _hTarget ) )
		{
			return ERROR_FROM_WIN32( GetLastError(), E_FAIL );
		}

		return S_OK;
	}
};

typedef CSimpleHolder< HANDLE, InternetAction, BaseValues<HANDLE> > CInternetHandle;

// inline int String2WideX( LPCSTR _pszChar, LPWSTR _pszWide, int _nLen = -1 )
// {
// 	ATLASSERT( _pszWide && _pszChar );
// 	if( !_pszWide || !_pszChar ) return 0;
// 
// 	int nLen = _nLen > 0 ? _nLen : lstrlenA(_pszChar);
// 	int nChars = 0;
// 
// 	// Check for UTF-8
// 	if( nLen > 2 && _pszChar[0] == (char)0xEF && _pszChar[1] == (char)0xBB && _pszChar[2] == (char)0xBF )
// 	{
// 		nChars = ::MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, _pszChar, _nLen, _pszWide, nLen * 4 + 2 );
// 	}
// 	else if( nLen > 2 && _pszChar[0] == (char)0x2B && _pszChar[1] == (char)0x2F && _pszChar[2] == (char)0x76 )
// 	{
// 		nChars = ::MultiByteToWideChar( CP_UTF7, MB_ERR_INVALID_CHARS, _pszChar, _nLen, _pszWide, nLen * 4 + 2 );
// 	}
// 	else if( nLen > 2 && _pszChar[0] == (char)0xFF && _pszChar[1] == (char)0xFE )
// 	{
// 		if( _nLen >= 0 )
// 			lstrcpynW( _pszWide, (LPCWSTR)(_pszChar + 2), (_nLen - 2 ) / sizeof(WCHAR) );	
// 		else 
// 			lstrcpyW( _pszWide, (LPCWSTR)(_pszChar + 2) );	
// 
// 		nChars = lstrlenW( _pszWide );
// 	}
// 	else if( _nLen > 0 && IsUnicode( _pszChar, _nLen, 0.9 ) ) 
// 	{
// 		if( _nLen >= 0 )
// 			lstrcpynW( _pszWide, (LPCWSTR)(_pszChar), _nLen / sizeof(WCHAR) );	
// 		else 
// 			lstrcpyW( _pszWide, (LPCWSTR)(_pszChar) );	
// 
// 		nChars = lstrlenW( _pszWide );
// 	}
// 	
// 	if( nChars == 0 )
// 	{
// 		nChars = ::MultiByteToWideChar( CP_ACP, 0, _pszChar, _nLen, _pszWide, nLen * 4 + 2 );
// 	}
// 
// 	_pszWide[nChars] = 0;
// 	return nChars;
// }

inline HRESULT CopyInternetFile( LPCWSTR _szSourceURL, LPCWSTR _szDestFile, DWORD _dwFlag = 0)
{
	HRESULT hr = S_OK;

	// Open internet conection
	CInternetHandle shInternet = ::InternetOpen(_T("Mozilla/4.0 (compatible; MSIE 5.0; Windows 98)"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if( !shInternet )
	{
		DWORD dwErr = ::GetLastError();
 		
 		ATLTRACE2(atlTraceGeneral, 2, _T("###Err### CopyInternetFile(%s->%s) - ")
 			_T("InternetOpen() FAILED dwRes = %d\n"), _szSourceURL, _szDestFile, dwErr );
 		
 		return HRESULT_FROM_WIN32( dwErr );
	}

	// Open request
	CInternetHandle shRequest = ::InternetOpenUrlW(shInternet, _szSourceURL, NULL, 0, INTERNET_FLAG_NO_UI | _dwFlag, 0);
	if( !shRequest )
	{
		DWORD dwErr = GetLastError();

		DWORD dwInetError = 0;
        WCHAR tchBuff[ 512 ] = {0};
        DWORD dwLen = 511;

        InternetGetLastResponseInfoW( &dwInetError, tchBuff, &dwLen );

		ATLTRACE2(atlTraceGeneral, 2, _T("###Err### CopyInternetFile(%ws->%ws) - ")
 			_T("InternetOpenUrl() FAILED dwRes = %d\nRESPONCE:[%08X - %ws]"), 
			_szSourceURL, _szDestFile, dwErr, dwInetError, tchBuff );

 		return HRESULT_FROM_WIN32( dwErr );
	}

	// Open destination file
	CStdHandle shFile = ::CreateFileW( _szDestFile , 
		GENERIC_WRITE, 
		0,
		NULL,
		CREATE_ALWAYS, 
		0,
		NULL );

	if( !shFile )
	{
		DWORD dwErr = ::GetLastError();
		
		ATLTRACE2(atlTraceGeneral, 2, _T("###Err### CopyInternetFile(%ws->%ws) - ")
			_T("CreateFile() FAILED dwRes = %d\n"), _szSourceURL, _szDestFile, dwErr );
		
		return HRESULT_FROM_WIN32( dwErr );
	}

	TCHAR szBuff[64]={0};
	// Buffer for data (128K)
	// Buffer for data (128K)
	DWORD cbReadBuffer = 128 * 1024;
	//BYTE arrBuffer[ 128 * 1024 ] = {0};
	CBlobVirtual blobRead;
	hr = blobRead.AllocMem( cbReadBuffer );
	if( FAILED( hr ) ) return hr;

	DWORD cbRead = 0;
	DWORD cbWholeRead = 0;
	while( ::InternetReadFile(shRequest, blobRead.Ptr(), cbReadBuffer, &cbRead) )
	{

		DWORD cbWritten = 0;
		BOOL bRes = ::WriteFile( shFile, blobRead.Ptr(), cbRead, &cbWritten, NULL );
		ATLASSERT( bRes && cbWritten == cbRead );
		if( !bRes || cbWritten != cbRead )
		{
			DWORD dwErr = ::GetLastError();
			
			ATLTRACE2(atlTraceGeneral, 2, _T("###Err### CopyInternetFile(%s->%s) - ")
				_T("WriteFile() FAILED dwRes = %d\n"), _szSourceURL, _szDestFile, dwErr );
			
			return HRESULT_FROM_WIN32( dwErr );
		}

		cbWholeRead += cbRead;

		ATLTRACE2(atlTraceGeneral, 2, _T("CopyInternetFile(%s->%s) - [%s bytes copied (%d read)]\n"),
				_szSourceURL, _szDestFile, N64ToStr( cbWholeRead, szBuff ), cbRead );

		if( cbRead != cbReadBuffer)
		{
			// End of file reached
			return S_OK;
		}
	}


	DWORD dwErr = ::GetLastError();
	
	ATLTRACE2(atlTraceGeneral, 2, _T("###Err### CopyInternetFile(%s->%s) - ")
		_T("InternetReadFile() FAILED dwRes = %d\n"), _szSourceURL, _szDestFile, dwErr );
	
	return HRESULT_FROM_WIN32( dwErr );
}

inline HRESULT ReadInternetFile( LPCWSTR _szSourceURL, CBlobHeap&  _crBlobFile, bool _bConvertText, DWORD _dwFlag = 0 )
{
	HRESULT hr = S_OK;

	// Open internet conection
	CInternetHandle shInternet = ::InternetOpenW(L"Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.0)", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if( !shInternet )
	{
		DWORD dwErr = ::GetLastError();

		ATLTRACE2(atlTraceGeneral, 2, _T("###Err### ReadInternetFile(%ws) - ")
			_T("InternetOpen() FAILED dwRes = %d\n"), _szSourceURL, dwErr );

		return HRESULT_FROM_WIN32( dwErr );
	}

	// Open request
	CInternetHandle shRequest = ::InternetOpenUrlW(shInternet, _szSourceURL, NULL, 0, INTERNET_FLAG_NO_UI | _dwFlag, 0);
	if( !shRequest )
	{
		DWORD dwErr = GetLastError();

		DWORD dwInetError = 0;
		WCHAR tchBuff[ 512 ] = {0};
		DWORD dwLen = 511;

		InternetGetLastResponseInfoW( &dwInetError, tchBuff, &dwLen );

		ATLTRACE2(atlTraceGeneral, 2, _T("###Err### ReadInternetFile(%s) - ")
			_T("InternetOpenUrl() FAILED dwRes = %d\nRESPONCE:[%08X - %ws]"), 
			_szSourceURL, dwErr, dwInetError, tchBuff );

		return HRESULT_FROM_WIN32( dwErr );
	}

	TCHAR szBuff[64]={0};
	// Buffer for data (128K)
	DWORD cbReadBuffer = 128 * 1024;
	//BYTE arrBuffer[ 128 * 1024 ] = {0};
	CBlobVirtual blobRead;
	hr = blobRead.AllocMem( cbReadBuffer );
	if( FAILED( hr ) ) return hr;

	DWORD cbRead = 0;
	DWORD cbWholeRead = 0;
	while( ::InternetReadFile(shRequest, blobRead.Ptr(), cbReadBuffer, &cbRead) )
	{
		if( cbRead == 0 )
		{
			// End of file reached
			break;
		}

		if( cbWholeRead + cbRead > _crBlobFile.GetAllocSize() )
		{
			HRESULT hr = _crBlobFile.ReallocMem( MAX( cbWholeRead + cbRead + 2, _crBlobFile.GetAllocSize() + INTERNET_READ_ALLOC_CHUNK ) );
			if( FAILED( hr ) )  return hr;
		}

		_crBlobFile.Resize( _crBlobFile.GetAllocSize() );
		_crBlobFile.MemCopy( blobRead.Ptr(), cbRead, cbWholeRead );
		cbWholeRead += cbRead;
		_crBlobFile.Resize( cbWholeRead + 2);
		_crBlobFile.MemSet( 0, cbWholeRead );
		_crBlobFile.Resize( cbWholeRead );
		

		ATLTRACE2(atlTraceGeneral, 2, _T("ReadInternetFile(%s) - [%s bytes copied (%d read)]\n"),
			_szSourceURL, N64ToStr( cbWholeRead, szBuff ), cbRead );
	}

	DWORD dwErr = ::GetLastError();

	ATLTRACE2(atlTraceGeneral, 2, _T("###Err### ReadInternetFile(%ws) - ")
		_T("InternetReadFile() FAILED dwRes = %d\n"), _szSourceURL, dwErr );


	if( cbWholeRead && _bConvertText)
	{
		CBlobHeap blobBuffer;
		HRESULT hr = blobBuffer.AllocMem( cbWholeRead * 4 + 2, true);
		if( FAILED( hr ) ) return hr;

		int cbConvert = Text2Wide( _crBlobFile.Ptr(), cbWholeRead, (LPWSTR)blobBuffer.Ptr() );
			
		if( cbConvert <= 0 )
			cbConvert =::MultiByteToWideChar( CP_UTF8, MB_ERR_INVALID_CHARS, (LPCSTR)_crBlobFile.Ptr(), cbWholeRead, (LPWSTR)blobBuffer.Ptr(), cbWholeRead );
		if( cbConvert <= 0 )
			cbConvert = ::MultiByteToWideChar( CP_ACP, MB_ERR_INVALID_CHARS, (LPCSTR)_crBlobFile.Ptr(), cbWholeRead, (LPWSTR)blobBuffer.Ptr(), cbWholeRead );
		if( cbConvert > 0 )
		{
			_crBlobFile.AllocMem( cbConvert * 2 + 2, true );
			_crBlobFile.MemCopy( blobBuffer.Ptr(), cbConvert * 2 );
			_crBlobFile.Resize( cbConvert * 2 );
		}
	}
	
	

	return cbWholeRead ? S_OK : HRESULT_FROM_WIN32( dwErr );
}

#endif // _WINAPI_I_UTILS_H_

