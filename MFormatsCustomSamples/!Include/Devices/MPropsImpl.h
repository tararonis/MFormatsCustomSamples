//---------------------------------------------------------------------------
// This file is part of the Medialooks MPlatform & MFormats SDKs
//
// MPropsImpl.h 
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

#include "MFormats.h"

#include "../WinAPI/Diagnostics.h"
#include "../xml/XMLParseM.h"



template <class TObject, class TInterface =  IMProps>
class ATL_NO_VTABLE IMPropsImpl:
	public TInterface
#ifdef _PROPS_HW_
	, public MPropsHW
#endif
{
protected:
	CRWSynchroLite	m_rwMProps;

	//int						m_nInfoEnum;
	bool					m_bOverridePropsForIndex; // For override props even for GetByIndex()
	bool					m_bIgnoreSameProps;

	CStrHashMap<bool>		m_mapAttributesInfo;

private:
	XMLParse::XMLNodePtr		m_pXMLProps;


public:
	typedef IMPropsImpl<TObject,TInterface> _IMPropsImpl;

	
public:

	IMPropsImpl()
		: m_pXMLProps( L"m-props", NULL ),
		  m_bOverridePropsForIndex( false ),
		  m_bIgnoreSameProps( false )
	{
	}

	~IMPropsImpl()
	{
		m_pXMLProps.Release();
		m_mapAttributesInfo.RemoveAll();
	}

	HRESULT OnSetMPropsEx(LPCWSTR _pszPropName, LPCWSTR _pszPropValue, LPCWSTR _pszPrevValue )
	{
		return E_NOTIMPL;
	}

	HRESULT OnSetMProps( LPCWSTR _pszPropName, LPCWSTR _pszPropValue )
	{
		return S_OK;
	}

	inline XMLParse::XMLNodePtr MPropsXML( LPCWSTR _pszObject = NULL )
	{
		ATLASSERT( m_pXMLProps );

		if( !_pszObject || !_pszObject[0] || !m_pXMLProps )
			return m_pXMLProps;

		return m_pXMLProps->GetNodePtr( _pszObject, true );
	}

	// Should be override in top class for have e.g. "object::xxx" props
	// (like in MFile, MPlaylist)
	inline XMLParse::XMLNodePtr MObjectXML()
	{
		ATLASSERT(m_pXMLProps);
		return m_pXMLProps;
	}

	// This method used for keep default attributes and user-defined 
	// e.g. Open( ... "some_props=xxx") and Open( ... "other_props=yyy")
	// cause "some_props=xxx other_props=yyy" what is not right
	inline XMLParse::XMLNodePtr MPropsXML_UpdateDefaults( LPCWSTR _pszObject = L"current", int _nUpdateType = 2 )
	{
		ATLASSERT( m_pXMLProps );

		if( STR_IS_EMPTY(_pszObject) || !m_pXMLProps )
			return m_pXMLProps;

		XMLParse::XMLNodePtr pNode = m_pXMLProps->GetNodePtr( _pszObject, true );
		ATLASSERT( pNode );
		if( pNode )
			m_pXMLProps->CopyAttributes( pNode, _nUpdateType );

		return pNode;
	}

	XMLParse::XMLNodePtr MPropsXML_Set( XMLParse::XMLNodePtr _pProps )
	{
		ATLASSERT( _pProps );
		if( _pProps )
			m_pXMLProps = _pProps;

		return m_pXMLProps;
	}

	inline XMLParse::XMLNodePtr MPropsXML_StatEx( LPCWSTR _pszDefStat = L"stat" )
	{
		XMLParse::XMLNodePtr pXMLObject = MPropsXML()->GetNodePtr( L"object" );
		if( !pXMLObject )
			pXMLObject = MPropsXML();

		LPCWSTR pszStatEx = pXMLObject->GetStringAttribute( L"object::statistics.extended", L"stat" );
		if( STR_IS_EMPTY(pszStatEx) && !_pszDefStat )
			return (XMLParse::XMLNodePtr)NULL;

		return MPropsXML( STR_IS_EMPTY(pszStatEx) ? _pszDefStat : pszStatEx );
	}

	

	// _nType == 1 -> Set, _nType == 2 -> Remove, _nType == 0 -> Get, _nType == -1 -> Enum (node as props_name), _nType == -2 -> All Info
	template<class T>
	HRESULT GetRedirector_MProps( CComBSTR& _rbsPropsName, int _nType, T** _ppMProps, BOOL* _pbSetCurrent = NULL )
	{
		return S_FALSE;
	}

	// _nType == 1 -> Set, _nType == 2 -> Remove, _nType == 0 -> Get, _nType == -1 -> Enum (node as props_name), _nType == -2 -> All Info
	template<class T>
	HRESULT _GetRedirector_MProps(CComBSTR& _rbsPropsName, int _nType, T** _ppMProps, BOOL* _pbSetCurrent = NULL)
	{
		TObject* pT = static_cast<TObject*>(this);
		HRESULT hr = pT->GetRedirector_MProps(_rbsPropsName, _nType, _ppMProps, _pbSetCurrent);
		if (SUCCEEDED(hr) && _ppMProps && *_ppMProps)
			return hr;

		// Special code for MRuntime props
		if (!STR_IS_EMPTY( _rbsPropsName.m_str) )
		{
			LPCWSTR pszName = NULL;
			if ((lstrcmpiW(_rbsPropsName, L"runtime") == 0 && _nType != 0) || IsPrefix(_rbsPropsName, L"runtime::", &pszName))
			{
				CComQIPtr<IMFProps> qpMFProps(MCreator::GetMFFactory());
				if (qpMFProps)
				{
					_rbsPropsName.Attach(::SysAllocString(pszName));

					return qpMFProps.QueryInterface(_ppMProps);
				}
			}
		}

		return S_FALSE;
	}

	STDMETHOD(PropsSet)( /*[in]*/ BSTR _bsPropName,
		/*[in]*/ BSTR _bsPropValue)
	{

		{
			CComBSTR cbsPropsName = _bsPropName;
			BOOL bSetCurrent = FALSE;
			TObject* pT =static_cast<TObject*>(this);
			CComPtr<IMProps> cpMProps;
			pT->_GetRedirector_MProps( cbsPropsName, 1, &cpMProps, &bSetCurrent );
			if( cpMProps )
			{
				HRESULT hr = cpMProps->PropsSet( cbsPropsName, _bsPropValue );
				if( FAILED( hr ) || !bSetCurrent )
					return hr;
			}
#ifdef MFORMATS_INTERFACES
			else
			{
				cbsPropsName = _bsPropName;
				CComPtr<IMFProps> cpMProps;
				pT->_GetRedirector_MProps( cbsPropsName, 1, &cpMProps, &bSetCurrent );
				if( cpMProps )
				{
					HRESULT hr = cpMProps->PropsSet( cbsPropsName, _bsPropValue );
					if( FAILED( hr ) || !bSetCurrent )
						return hr;
				}
			}
#endif
		}

		TObject* pT = static_cast<TObject*>(this);

		CComBSTR cbsPrevValue;

		{
			CAutoRWL rw(m_rwMProps, true);

			XMLParse::XMLNodePtr pProps = pT->MPropsXML();
			if (!pProps) return E_FAIL;

			if (_bsPropName && _bsPropName[0])
			{
				// If have node with same name -> change it's name
				XMLParse::XMLNodePtr pNode = pProps->GetNodePtr(_bsPropName);
				if (pNode)
				{
					pNode->SetValue(_bsPropValue);
				}
				else
				{
					cbsPrevValue = pProps->GetStringAttribute_Safe(_bsPropName);

					if (!pProps->SetAttribute(_bsPropName, _bsPropValue, TRUE, FALSE))
						return E_INVALIDARG;

					// The next code remove attribute with same name as node and set node value to prev attr name
					// e.g. have attributes video='1080i', after set attribute video::codec=h264, remove 'video' attribute
					// and set 'video' node value to '1080i'
					// Have: 
					//		<props video=1080i/>
					// Call: 
					//		PropsSet( "video::codec", "h264")
					// Result:
					//		<props>
					//			<video codec=h264>1080i</video>
					//		</props>
					CSimpleArray2<CComBSTR> arrNames;
					XMLParse::AttName2Array(_bsPropName, arrNames);
					if (arrNames.GetSize() > 1)
					{
						// Check for attribute with same name
						arrNames.RemoveAt(arrNames.GetSize() - 1);

						CComBSTR cbsNode;
						XMLParse::Array2AttName(arrNames, &cbsNode);

						bool bHave = false;
						LPCWSTR pszAttributeOld = pProps->GetStringAttribute(cbsNode, NULL, &bHave);
						if (bHave)
						{
							XMLParse::XMLNodePtr pNewNode = pProps->GetNodePtr(cbsNode);
							ATLASSERT(pNewNode);
							if (pNewNode)
								pNewNode->SetValue(pszAttributeOld);

							pProps->RemoveAttribute(cbsNode);
						}
					}
				}
			}
			else
			{
				if (!_bsPropValue || !_bsPropValue[0])
					return E_INVALIDARG;

				HRESULT hr = pProps->LoadNode(NULL, _bsPropValue);
				if (FAILED(hr)) return hr;
			}

			if (m_bIgnoreSameProps && !STR_IS_EMPTY(_bsPropValue) && !STR_IS_EMPTY(cbsPrevValue) && cbsPrevValue == _bsPropValue)
			{
				return S_FALSE;
			}
		}

		// Try new callback with previous value
		HRESULT hr = pT->OnSetMPropsEx(_bsPropName, _bsPropValue, cbsPrevValue );
		if( hr == E_NOTIMPL )
			return pT->OnSetMProps( _bsPropName, _bsPropValue );

		return hr;
	}

	STDMETHOD(PropsGet)( /*[in]*/ BSTR _bsPropName, 
		/*[out]*/ BSTR* _pbsPropValue )
	{
		{
			CComBSTR cbsPropsName = _bsPropName;
			TObject* pT =static_cast<TObject*>(this);
			CComPtr<IMProps> cpMProps;
			pT->_GetRedirector_MProps( cbsPropsName, 0, &cpMProps, NULL );
			if (cpMProps)
			{
				// Could be RPC error -> and crash in .Net if return failed result
				HRESULT hr = cpMProps->PropsGet(cbsPropsName, _pbsPropValue);
				return FAILED(hr) ? S_FALSE : hr;
			}
#ifdef MFORMATS_INTERFACES
			{
				cbsPropsName = _bsPropName;
				CComPtr<IMFProps> cpMProps;
				pT->_GetRedirector_MProps( cbsPropsName, 0, &cpMProps, NULL );
				if (cpMProps)
				{
					HRESULT hr = cpMProps->PropsGet(cbsPropsName, _pbsPropValue);
					return FAILED(hr) ? S_FALSE : hr;
				}
			}
#endif
		}

		if( !_pbsPropValue ) return E_POINTER;

		CAutoRWL rw( m_rwMProps, false );

		TObject* pT =static_cast<TObject*>(this);
		XMLParse::XMLNodePtr pProps = pT->MPropsXML();

		if( _bsPropName && _bsPropName[0] )
		{
			TCHAR szModuleName[MAX_PATH*4] = {0};
			__if_exists(_Module)
			{
				::GetModuleFileName( _Module.GetModuleInstance(), szModuleName, SIZEOF_ARRAY(szModuleName) );
			}
			__if_not_exists(_Module)
			{
				::GetModuleFileName( _AtlBaseModule.GetModuleInstance(), szModuleName, SIZEOF_ARRAY(szModuleName) );
			}
			if( lstrcmpiW( _bsPropName, L"_process_name_" ) == 0)
			{
				::GetModuleFileName( NULL, szModuleName, SIZEOF_ARRAY(szModuleName) );
				*_pbsPropValue = CComBSTR(szModuleName).Detach();
			}
			else if( lstrcmpiW( _bsPropName, L"_process_pid_" ) == 0)
			{
				*_pbsPropValue = CComBSTR(INT2STRW(GetCurrentProcessId())).Detach();
			}
			else if( lstrcmpiW( _bsPropName, L"dll-version" ) == 0)
			{
				VERINFO verInfo = {0};
				CDiag::GetVerInfo(szModuleName, &verInfo);

				TCHAR szVerString[MAX_PATH] = {0};
				wsprintf( szVerString, _T("%d.%d.%d.%d"), HIWORD( verInfo.dwProdVerMS ), LOWORD( verInfo.dwProdVerMS ), 
					HIWORD( verInfo.dwProdVerLS ), LOWORD( verInfo.dwProdVerLS ) );

				*_pbsPropValue = CComBSTR( szVerString ).Detach();
			}
			else if( lstrcmpiW( _bsPropName, L"dll-file-version" ) == 0)
			{
				VERINFO verInfo = {0};
				CDiag::GetVerInfo(szModuleName, &verInfo);

				TCHAR szVerString[MAX_PATH] = {0};
				wsprintf( szVerString, _T("%d.%d.%d.%d"), HIWORD( verInfo.dwFileVerMS ), LOWORD( verInfo.dwFileVerMS ), 
					HIWORD( verInfo.dwFileVerLS ), LOWORD( verInfo.dwFileVerLS ) );

				*_pbsPropValue = CComBSTR( szVerString ).Detach();
			}
			else if( lstrcmpiW( _bsPropName, L"dll-path" ) == 0)
			{
				*_pbsPropValue = CComBSTR( szModuleName ).Detach();
			}
			else if( lstrcmpiW( _bsPropName, L"dll-date" ) == 0)
			{
				CComBSTR cbsDate = __DATE__;
				cbsDate.Append( L" ");
				cbsDate.Append( __TIME__ );

				*_pbsPropValue = cbsDate.Detach();
			}
			else
			{
				if( !pProps ) return E_FAIL;

				LPCWSTR pszValue = pProps->GetStringAttribute( _bsPropName );
				if( !pszValue ) 
				{
#ifdef _PROPS_HW_
					HRESULT hr = OnGetHW( _bsPropName, _pbsPropValue );
					if( hr == S_OK )
						return S_OK;
#endif
					// The next code for resolve conflict between same nodes name and attributes
					// e.g. have attributes video='1080i', after set attribute video::codec=h264, remove 'video' attribute
					// and set 'video' node value to '1080i'
					// Have: 
					//		<props video=1080i/>
					// Call: 
					//		PropsSet( "video::codec", "h264")
					// Result:
					//		<props>
					//			<video codec=h264>1080i</video>
					//		</props>

					// Check node
					XMLParse::XMLNodePtr pNode = pProps->GetNodePtr(_bsPropName);
					if (!pNode)
						return S_FALSE;

					if( !pNode->GetValue( 0, _pbsPropValue ) )
					{
						// If node is empty -> save whole node to XML 
						pNode->SaveAttributes( _pbsPropValue );
					}
				}
				else
				{
					*_pbsPropValue = CComBSTR(pszValue).Detach();
				}
			}
		}
		else
		{	
			XMLParse::SaveXML( pProps, _pbsPropValue );
		}	

		return S_OK;
	}


	STDMETHOD(PropsGetCount)( /*[in]*/ BSTR _bsNodeName, /*[out]*/ int* _pnCount )
	{
		if( !_pnCount )
			return E_POINTER;

		TObject* pT =static_cast<TObject*>(this);

		{
			CComBSTR cbsPropsName = _bsNodeName;
			TObject* pT =static_cast<TObject*>(this);
			CComPtr<IMProps> cpMProps;
			pT->_GetRedirector_MProps( cbsPropsName, -1, &cpMProps, NULL );
			if (cpMProps)
			{
				HRESULT hr = cpMProps->PropsGetCount(cbsPropsName, _pnCount);
				return FAILED(hr) ? S_FALSE : hr;
			}
#ifdef MFORMATS_INTERFACES
			{
				cbsPropsName = _bsNodeName;
				CComPtr<IMFProps> cpMProps;
				pT->_GetRedirector_MProps( cbsPropsName, -1, &cpMProps, NULL );
				if (cpMProps)
				{
					HRESULT hr = cpMProps->PropsGetCount(cbsPropsName, _pnCount);
					return FAILED(hr) ? S_FALSE : hr;
				}
			}
#endif
		}

		CAutoRWL rw( m_rwMProps, true );

		XMLParse::XMLNodePtr pNode = GetTargetNode_I( _bsNodeName );
		if( !pNode )
		{
			return E_INVALIDARG;
		}

		// Return the info props + user props 
		m_mapAttributesInfo.RemoveAll();
		XMLParse::XMLNodePtr pInfo = pNode->GetInfo( NULL, NULL, &m_mapAttributesInfo );
		if( pInfo )
		{
			*_pnCount = m_mapAttributesInfo.GetSize() + pNode->GetRealNodesCount();
		}
		else
		{
			*_pnCount = pNode->GetAttributesCount() + pNode->GetRealNodesCount();
		}

		return S_OK;
	}

	STDMETHOD(PropsGetByIndex)( /*[in]*/ BSTR _bsNodeName, /*[in]*/ int _nIndex, /*[out]*/ BSTR* _pbsPropName, /*[out]*/ BSTR* _pbsPropValue, /*[out]*/ BOOL* _pbNode )
	{
		TObject* pT =static_cast<TObject*>(this);

		{
			CComBSTR cbsPropsName = _bsNodeName;
			TObject* pT =static_cast<TObject*>(this);
			CComPtr<IMProps> cpMProps;
			pT->_GetRedirector_MProps( cbsPropsName, -1, &cpMProps, NULL );
			if (cpMProps)
			{
				HRESULT hr =  cpMProps->PropsGetByIndex(cbsPropsName, _nIndex, _pbsPropName, _pbsPropValue, _pbNode);
				return FAILED(hr) ? S_FALSE : hr;
			}
#ifdef MFORMATS_INTERFACES
			{
				cbsPropsName = _bsNodeName;
				CComPtr<IMFProps> cpMProps;
				pT->_GetRedirector_MProps( cbsPropsName, -1, &cpMProps, NULL );
				if (cpMProps)
				{
					HRESULT hr = cpMProps->PropsGetByIndex(cbsPropsName, _nIndex, _pbsPropName, _pbsPropValue, _pbNode);
					return FAILED(hr) ? S_FALSE : hr;
				}
			}
#endif
		}

		CAutoRWL rw( m_rwMProps, true );

		if( _pbNode )
			*_pbNode = false;

		
		XMLParse::XMLNodePtr pNode = GetTargetNode_I( _bsNodeName );
		if( !pNode )
		{
			// Check for info
			if( _bsNodeName && lstrcmpiW( _bsNodeName, L"_info_") == 0 )
			{
				CComBSTR cbsName;
				pT->MPropsXML()->AttributesInfoGetByIndex( _nIndex, &cbsName );
				if( cbsName.Length() )
				{
					pT->MPropsXML()->AttributesStringGet( cbsName, _pbsPropValue );

					return S_OK;
				}
			}

			return E_INVALIDARG;
		}

		if( _nIndex < 0 )
			return HRESULT_FROM_WIN32( ERROR_INVALID_INDEX );

		if( _pbNode )
			*_pbNode = false;

		// Check for attributes enum
		if( m_mapAttributesInfo.GetSize() == 0 && pNode->HaveInfo() )
		{
			pNode->GetInfo( NULL, NULL, &m_mapAttributesInfo );
		}

		if( m_mapAttributesInfo.GetSize() )
		{
			if( _nIndex < m_mapAttributesInfo.GetSize() )
			{
				CComBSTR cbsName = m_mapAttributesInfo.GetKeyAt( _nIndex );

				if( _pbsPropValue )
				{
					CComBSTR cbsValue = pNode->GetStringAttribute_Safe( cbsName );
					*_pbsPropValue = cbsValue.Detach();
				}

				if( _pbsPropName )
					*_pbsPropName = cbsName.Detach();

				return S_OK;
			}

			_nIndex -= m_mapAttributesInfo.GetSize();
		}
		else 
		{
			if( _nIndex < pNode->GetAttributesCount() )
			{
				if( _pbsPropName )
					pNode->GetAttributeName(_nIndex, _pbsPropName );

				if( _pbsPropValue )
				{
					if( m_bOverridePropsForIndex )
					{
						CComBSTR cbsFullName;
						pNode->GetAttributeFullName(_nIndex, &cbsFullName );

						pT->PropsGet( cbsFullName, _pbsPropValue );
					}
					else
					{
						pNode->GetAttributeValue(_nIndex, _pbsPropValue );
					}
				}


				return S_OK;
			}

			_nIndex -= pNode->GetAttributesCount();
		}

		if( _nIndex < pNode->GetRealNodesCount() )
		{
			XMLParse::XMLNodePtr pChildNode = pNode->GetRealNodeAt( _nIndex );
			if( !pChildNode )
				return E_UNEXPECTED;

			if( _pbsPropName )
				pChildNode->GetName( _pbsPropName );

			if( _pbsPropValue )
				pChildNode->GetValue(0, _pbsPropValue );

			if( _pbNode )
				*_pbNode = true;

			return S_OK;
		}

		return HRESULT_FROM_WIN32( ERROR_INVALID_INDEX );
	}

	STDMETHOD(PropsRemove)( /*[in]*/ BSTR _bsPropName )
	{
		if( !_bsPropName || !_bsPropName[0] )
			return E_INVALIDARG;

		TObject* pT =static_cast<TObject*>(this);

		{
			CComBSTR cbsPropsName = _bsPropName;
			BOOL bSetCurrent = FALSE;
			TObject* pT =static_cast<TObject*>(this);
			CComPtr<IMProps> cpMProps;
			pT->_GetRedirector_MProps( cbsPropsName, 2, &cpMProps, &bSetCurrent );
			if( cpMProps )
			{
				HRESULT hr = cpMProps->PropsRemove( cbsPropsName );
				if( !bSetCurrent )
					return FAILED(hr) ? S_FALSE : hr;
			}
#ifdef MFORMATS_INTERFACES
			{
				cbsPropsName = _bsPropName;
				CComPtr<IMFProps> cpMProps;
				pT->_GetRedirector_MProps( cbsPropsName, 2, &cpMProps, &bSetCurrent );
				if( cpMProps )
				{
					HRESULT hr = cpMProps->PropsRemove( cbsPropsName );
					if( !bSetCurrent )
						return FAILED(hr) ? S_FALSE : hr;
				}
			}
#endif
		}

		CAutoRWL rw( m_rwMProps, true );

		m_mapAttributesInfo.RemoveAll();

		XMLParse::XMLNodePtr pProps = pT->MPropsXML();
		if( !pProps ) return E_FAIL;

		if( _bsPropName && lstrcmpiW(_bsPropName, L"_all_props_" ) == 0 )
		{
			pProps->ClearAttributes();
		}
		else if( _bsPropName && lstrcmpiW(_bsPropName, L"_all_nodes_" ) == 0 )
		{
			pProps->ClearNodes();
		}
		else
		{
			if( !pProps->RemoveAttribute( _bsPropName ) )
			{
				if( !pProps->RemoveNode(_bsPropName) )
				{
					return E_INVALIDARG;
				}
			}
		}


		return S_OK;
	}

	STDMETHOD(PropsInfoGet)( /*[in]*/ BSTR _bsPropName, /*[in]*/ eMInfoType _eType, /*[out]*/ BSTR* _pbsInfo )
	{
		if( !_bsPropName || !_bsPropName[0] )
			return E_INVALIDARG;

		TObject* pT =static_cast<TObject*>(this);

		{
			CComBSTR cbsPropsName = _bsPropName;
			TObject* pT =static_cast<TObject*>(this);
#ifdef MFORMATS_INTERFACES
			{
				CComPtr<IMFProps> cpMProps;
				pT->_GetRedirector_MProps( cbsPropsName, -2, &cpMProps, NULL );
				if (cpMProps)
				{
					HRESULT hr = cpMProps->PropsInfoGet(cbsPropsName, _eType, _pbsInfo);
					return FAILED(hr) ? S_FALSE : hr;
				}
			}
#endif
		}

		CAutoRWL rw( m_rwMProps, false );

		XMLParse::XMLNodePtr pProps = pT->MPropsXML();
		if( !pProps ) return E_FAIL;

		return pProps->AttributesInfoGet( _bsPropName, _eType, _pbsInfo );
	}


	STDMETHOD(PropsOptionGetCount)( /*[in]*/ BSTR _bsPropName, /*[out]*/ int* _pnOptionCount )
	{
		if( !_pnOptionCount )
			return E_POINTER;

		TObject* pT =static_cast<TObject*>(this);

		{
			CComBSTR cbsPropsName = _bsPropName;
			CComPtr<IMFProps> cpMProps;
			pT->_GetRedirector_MProps( cbsPropsName, -2, &cpMProps, NULL );
			if (cpMProps)
			{
				HRESULT hr = cpMProps->PropsOptionGetCount(cbsPropsName, _pnOptionCount);
				return FAILED(hr) ? S_FALSE : hr;
			}
		}

		XMLParse::XMLNodePtr pProps = pT->MPropsXML();
		if( !pProps ) return E_FAIL;

		CComBSTR cbsValues;
		CSimpleArray2<CComBSTR> arrValues;
		HRESULT hr = pProps->AttributesInfoGet( _bsPropName, eMIT_Values, &cbsValues );
		if( SUCCEEDED( hr ) && cbsValues.m_str)
		{
			if( StrChrW(cbsValues.m_str, L'|') )
			{
				XMLUtils::StringSplit( cbsValues, arrValues, L'|' );
			}
			else
			{
				XMLUtils::String2Array( cbsValues, arrValues );
			}
		}

		*_pnOptionCount = arrValues.GetSize();

		return S_OK;
	}

	STDMETHOD(PropsOptionGetByIndex)( /*[in]*/ BSTR _bsPropName, /*[in]*/ int _nOptionIdx, /*[out]*/ BSTR* _pbsOption, /*[out]*/ BSTR* _pbsHelp )
	{
		TObject* pT =static_cast<TObject*>(this);
		
		{
			CComBSTR cbsPropsName = _bsPropName;
			CComPtr<IMFProps> cpMProps;
			pT->_GetRedirector_MProps( cbsPropsName, -2, &cpMProps, NULL );
			if (cpMProps)
			{
				HRESULT hr = cpMProps->PropsOptionGetByIndex(cbsPropsName, _nOptionIdx, _pbsOption, _pbsHelp);
				return FAILED(hr) ? S_FALSE : hr;
			}
		}

		XMLParse::XMLNodePtr pProps = pT->MPropsXML();
		if( !pProps ) return E_FAIL;

		CComBSTR cbsValues;
		CSimpleArray2<CComBSTR> arrValues;
		HRESULT hr = pProps->AttributesInfoGet( _bsPropName, eMIT_Values, &cbsValues );
		if( SUCCEEDED( hr ) && cbsValues.m_str)
		{
			if( StrChrW(cbsValues.m_str, L'|') )
			{
				XMLUtils::StringSplit( cbsValues, arrValues, L'|' );
			}
			else
			{
				XMLUtils::String2Array( cbsValues, arrValues );
			}
		}

		if( _nOptionIdx < 0 || _nOptionIdx >= arrValues.GetSize() )
			return HRESULT_FROM_WIN32( ERROR_INVALID_INDEX );

		if( _pbsOption )
			arrValues[_nOptionIdx].CopyTo( _pbsOption );

		if( _pbsHelp )
			pProps->AttributesValuesInfoGet( _bsPropName, arrValues[_nOptionIdx], NULL, _pbsHelp );

		return S_OK;
	}

	STDMETHOD(PropsOptionSetByIndex)( /*[in]*/ BSTR _bsPropName, /*[in]*/ int _nOptionIdx )
	{
		TObject* pT =static_cast<TObject*>(this);
		
		{
			CComBSTR cbsPropsName = _bsPropName;
			CComPtr<IMFProps> cpMProps;
			pT->_GetRedirector_MProps(cbsPropsName, -2, &cpMProps, NULL);
			if (cpMProps)
			{
				HRESULT hr = cpMProps->PropsOptionSetByIndex(cbsPropsName, _nOptionIdx);
				return FAILED(hr) ? S_FALSE : hr;
			}
		}

		XMLParse::XMLNodePtr pProps = pT->MPropsXML();
		if( !pProps ) return E_FAIL;

		CComBSTR cbsValues;
		CSimpleArray2<CComBSTR> arrValues;
		HRESULT hr = pProps->AttributesInfoGet( _bsPropName, eMIT_Values, &cbsValues );
		if( SUCCEEDED( hr ) && cbsValues.m_str)
		{
			if( StrChrW(cbsValues.m_str, L'|') )
			{
				XMLUtils::StringSplit( cbsValues, arrValues, L'|' );
			}
			else
			{
				XMLUtils::String2Array( cbsValues, arrValues );
			}
		}

		if( _nOptionIdx < 0 || _nOptionIdx >= arrValues.GetSize() )
			return HRESULT_FROM_WIN32( ERROR_INVALID_INDEX );

		return pT->PropsSet( _bsPropName, arrValues[_nOptionIdx] );
	}

	STDMETHOD(PropsOptionGet)( /*[in]*/ BSTR _bsPropName, /*[out]*/ int* _pnOptionIdx, /*[out]*/ BSTR* _pbsOptionValue, /*[out]*/ BSTR* _pbsHelp )
	{
		TObject* pT =static_cast<TObject*>(this);

		if( _pnOptionIdx )
			*_pnOptionIdx = -1;

		{
			CComBSTR cbsPropsName = _bsPropName;
			CComPtr<IMFProps> cpMProps;
			pT->_GetRedirector_MProps( cbsPropsName, -2, &cpMProps, NULL );
			if (cpMProps)
			{
				HRESULT hr = cpMProps->PropsOptionGet(cbsPropsName, _pnOptionIdx, _pbsOptionValue, _pbsHelp);
				return FAILED(hr) ? S_FALSE : hr;
			}
		}

		XMLParse::XMLNodePtr pProps = pT->MPropsXML();
		if( !pProps ) return E_FAIL;

		CComBSTR cbsOptionValue = pProps->GetStringAttribute_Safe( _bsPropName );
		if( !cbsOptionValue )
			return S_FALSE;//HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND ); // For avoid error message in End-User applications.

		if( _pnOptionIdx )
		{
			CComBSTR cbsValues;
			CSimpleArray2<CComBSTR> arrValues;
			HRESULT hr = pProps->AttributesInfoGet( _bsPropName, eMIT_Values, &cbsValues );
			if( SUCCEEDED( hr ) && cbsValues.m_str)
			{
				if( StrChrW(cbsValues.m_str, L'|') )
				{
					XMLUtils::StringSplit( cbsValues, arrValues, L'|' );
				}
				else
				{
					XMLUtils::String2Array( cbsValues, arrValues );
				}
			}

			*_pnOptionIdx = arrValues.Find( cbsOptionValue );
		}

		if( _pbsHelp )
			pProps->AttributesValuesInfoGet( _bsPropName, cbsOptionValue, NULL, _pbsHelp );

		return S_OK;
	}



private:

	XMLParse::XMLNodePtr GetTargetNode_I( LPCWSTR _pszNodeName )
	{
		TObject* pT =static_cast<TObject*>(this);
		XMLParse::XMLNodePtr pProps = pT->MPropsXML();
		if( pProps && _pszNodeName && _pszNodeName[0] )
		{
			return pProps->GetNodePtr( _pszNodeName );
		}

		return pProps;
	}
};



template <class T>
class IMPropsHostRedirector
	: public IMPropsImpl<T, IMFProps>
{
	CComPtrM<IUnknown> m_cpHost;
public:

	template<class T>
	HRESULT GetRedirector_MProps( CComBSTR& _rbsPropsName, int _nType, T** _ppMProps, BOOL* _pbSetCurrent = NULL )
	{
		CComPtrM<IUnknown> cpHost = m_cpHost;

		if( cpHost && _rbsPropsName  )
		{
			// For playlist use: host::current -> current file
			//					 host::next -> next file
			CComQIPtr<IMPlaylist> qpMPlaylist( cpHost );

			if( qpMPlaylist )
			{
				LPCWSTR ppszForward[] = {L"host::current", L"host::next", L"host::background", NULL };

				int nType = 0;
				LPCWSTR pszName = NULL;
				if( (nType = IsPrefix( _rbsPropsName, ppszForward, &pszName)) > 0 )
				{
					// Need 
					if( STR_IS_EMPTY(pszName) || (pszName[0] == L':' && pszName[1] == ':') )
					{
						_rbsPropsName = STR_IS_EMPTY(pszName) ? NULL : pszName + 2;

						CComBSTR cbsFake;
						double dblFake = 0.0;
						CComPtr<IMItem> cpMItem;
						if( nType == 1 || nType == 2 ) // 1 - current, 2 - next
						{
							qpMPlaylist->PlaylistGetByIndex( -1 * nType, &dblFake, &cbsFake, &cpMItem );
						}
						else
						{
							ATLASSERT( nType == 2 );
							qpMPlaylist->PlaylistBackgroundGet( &cbsFake, &cpMItem );
						}

						if( cpMItem )
							return cpMItem.QueryInterface( _ppMProps );
					}

					return S_OK;
				}
			}


			// For mixer use: host::stream.xxx -> mixer stream
			//				  host::[stream-id] -> mixer stream (TODO ?)
			CComQIPtr<IMStreams> qpMStreams( cpHost );
			if( qpMStreams )
			{
				LPCWSTR ppszForward[] = {L"host::stream.", L"host::background", NULL };

				int nType = 0;
				LPCWSTR pszName = NULL;
				if( (nType = IsPrefix( _rbsPropsName, ppszForward, &pszName)) >= 0 )
				{
					// Need 
					if( nType == 1 && ( STR_IS_EMPTY(pszName) || (pszName[0] == L':' && pszName[1] == ':') ) )
					{
						_rbsPropsName = STR_IS_EMPTY(pszName) ? NULL : pszName + 2;

						CComBSTR cbsFake;
						CComPtr<IMItem> cpMItem;
						qpMStreams->StreamsBackgroundGet( &cbsFake, &cpMItem );

						if( cpMItem )
							return cpMItem.QueryInterface( _ppMProps );
					}
					else if( nType == 0 && !STR_IS_EMPTY(pszName) )
					{
						LPWSTR pszNameEnd = 0;
						int nStreamIdx = wcstoul( pszName, &pszNameEnd, 0 );
						if( STR_IS_EMPTY(pszNameEnd) || (pszNameEnd[0] == L':' && pszNameEnd[1] == ':') )
						{
							_rbsPropsName = STR_IS_EMPTY(pszNameEnd) ? NULL : pszNameEnd + 2;

							CComBSTR cbsFake;
							CComPtr<IMItem> cpMItem;
							qpMStreams->StreamsGetByIndex( nStreamIdx, &cbsFake, &cpMItem );

							if( cpMItem )
								return cpMItem.QueryInterface( _ppMProps );
						}
					}

					return S_OK;
				}
			}

			LPCWSTR pszName = NULL;
			if( STR_IS_SAME_I( _rbsPropsName, L"host" ) && IsPrefix( _rbsPropsName, L"host::", &pszName) )
			{
				_rbsPropsName = pszName;
				return cpHost.QueryInterface( _ppMProps );
			}
		}

		return S_OK;
	}

	void Redirector_MPropsHostSet( IUnknown* _pHost )
	{
		m_cpHost = _pHost;
	}
};



template <class T>
class IMFConfigImpl
	: public IMFConfig
{
	CStdHandle					m_shMutex;
	XMLParse::XMLNodePtr		m_pXMLConfig;
	CComBSTR					m_cbsConfigPath;
private:
	
	// Skip Software\Medialooks\ 

	LPCWSTR CorrectKeyName( LPCWSTR _pszName )
	{
		if( STR_IS_EMPTY( _pszName ) )
			return _pszName;

		{
			LPCWSTR pszCmp = L"Software";
			int nLen = lstrlenW(pszCmp);
			if( StrCmpNIW( _pszName, pszCmp, nLen ) == 0 &&
				(_pszName[nLen] == L'\\' || _pszName[nLen] == '/') )
			{
				_pszName += nLen + 1;
			}
			else
			{
				return _pszName;
			}
		}

		{
			LPCWSTR pszCmp = L"Medialooks";
			int nLen = lstrlenW(pszCmp);
			if( StrCmpNIW( _pszName, pszCmp, nLen ) == 0 &&
				(_pszName[nLen] == L'\\' || _pszName[nLen] == '/') )
			{
				_pszName += nLen + 1;
			}
		}

		return _pszName;
	}

	

public:

	IMFConfigImpl()
		: m_pXMLConfig( L"config", NULL )
	{
		m_shMutex.Attach( ::CreateMutex( NULL, FALSE, L"__MFConfig_Sync_Mutex__" ) );
	}

	void Config_SetPersistFile( LPCWSTR _pszPath )
	{
		CAutoMTX crs( m_shMutex );

		m_cbsConfigPath = _pszPath;
	}

	STDMETHOD(SaveSettings)( /*[in]*/ BSTR _bsKeyName, /*[in]*/ BSTR _bsAttributesList )
	{
		CAutoMTX crs( m_shMutex );

		XMLParse::XMLNodePtr pXMLNode;
		if( STR_IS_EMPTY(_bsKeyName) )
		{
			pXMLNode = m_pXMLConfig->GetNodePtr( L"default::attributes", true, 0 );
		}
		else
		{
			LPCWSTR pszKey = CorrectKeyName( _bsKeyName );
			pXMLNode = m_pXMLConfig->FindNodeByAttribute( L"key", pszKey );
			if( !pXMLNode )
			{
				pXMLNode = m_pXMLConfig->AddNodePtr( L"data" );
				pXMLNode->SetAttribute( L"key", pszKey );

				ATLASSERT( m_pXMLConfig->FindNodeByAttribute( L"key", pszKey ) == pXMLNode );
			}

			if( pXMLNode )
				pXMLNode = pXMLNode->GetNodePtr( L"attributes", true );
		}

		if( !pXMLNode )
			return E_UNEXPECTED;

		pXMLNode->ParseAttributes( _bsAttributesList, NULL, false, 1, true );

		if( STR_IS_EMPTY( m_cbsConfigPath.m_str ) )
			return S_FALSE;

		HRESULT hr = m_pXMLConfig->SaveToFile( m_cbsConfigPath, true, NULL, 2 );
		if( FAILED( hr ) )
		{
			MTRACE_ERR( "m_pXMLConfig->SaveToFile(%ws) FAILED hr=%08X", m_cbsConfigPath.m_str, hr );

			return hr;
		}

		return S_OK;
	}
	
	STDMETHOD(LoadSettings)( /*[in]*/ BSTR _bsKeyName, /*[out]*/ BSTR* _pbsAttributesList )
	{
		CAutoMTX crs( m_shMutex );

		if( !STR_IS_EMPTY( m_cbsConfigPath.m_str ) )
		{
			HRESULT hr = m_pXMLConfig->LoadFromFile( m_cbsConfigPath );
			if( FAILED( hr ) )
			{
				MTRACE_WARN( "m_pXMLConfig->LoadFromFile(%ws) FAILED hr=%08X", m_cbsConfigPath.m_str, hr );
			}
		}

		XMLParse::XMLNodePtr pXMLNode;
		if( STR_IS_EMPTY(_bsKeyName) )
		{
			pXMLNode = m_pXMLConfig->GetNodePtr( L"default::attributes" );
		}
		else
		{
			LPCWSTR pszKey = CorrectKeyName( _bsKeyName );

			pXMLNode = m_pXMLConfig->FindNodeByAttribute( L"key", pszKey );
			if( pXMLNode )
				pXMLNode = pXMLNode->GetNodePtr( L"attributes", true );
		}

		if( pXMLNode )
			pXMLNode->SaveAttributes_R( _pbsAttributesList, false );

		if( STR_IS_EMPTY( m_cbsConfigPath.m_str ) )
			return S_FALSE;
		
		return pXMLNode ? S_OK : HRESULT_FROM_WIN32( ERROR_INVALID_INDEX );
	}

};