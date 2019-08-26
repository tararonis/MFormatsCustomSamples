//---------------------------------------------------------------------------
// This file is part of the Medialooks MPlatform/MFormats
//
// XMLParseM.h 
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

#ifdef OLD_XML_PARSE
#error Mixed new and old XMLParse
#endif 

#ifndef _XML_PARSE_M_H_
#define _XML_PARSE_M_H_

#define NEW_XML_PARSE

#include "../Base/DSDefines.h"
#include "../WinAPI/Macros.h"
#include "../WinAPI/wrappers.h"

#include "../Base/MFunctions.h"
#include "../Devices/MTimer.h"

#include "IndexMap.h"
#include "XMLUtils.h"
#include "ObjectsMap.h"

#ifndef MFRAMES_STATIC_FACTORY
#include "../Base/MCreator.h"
#endif

#include <deque>
using std::deque;

//#include "MPlatform.h"

#if !defined(__MFormats_h__) && !defined(__MPlatform_h__)
#error "MFormats.h or MPlatform.h required for XMLParseM.h"
#endif

 
#define CRWSynchroLiteXML CRWSynchroLiteCS

static LPCWSTR	ppszHiddenAttrInfoStr[] = { L"attributes_info", NULL };

//////////////////////////////////////////////////////////////////////////
// XML methods

class XMLParse
{
public:
	typedef enum eSetNodeType
	{
		eSNT_Nodes,
		eSNT_Attributes,
		eSNT_Playout // the top level is nodes, other is attributes
	} eSetNodeType;

	// Create node from attributes list or XML description
	typedef enum eCreateNodeParam
	{
		eCNP_BasicNode,
		eCNP_ParseAttributes,
		eCNP_ParseAttributes_Comlpex, // e.g. list node_name::value=xxx node_name2::value2=xxx etc.
		eCNP_ParseXML,
		eCNP_ParseXML_Complex
	} eCreateNodeParam;

	struct XMLAttribute: 
		public AutoObjBase<XMLAttribute>
	{
		static MClock	s_clockGlobal; // Used for ealpsed time
	public:
		enum Type
		{
			eXAT_String,

			eXAT_Bool,
			eXAT_Int,
			eXAT_UInt,
			eXAT_Int64,
			eXAT_UInt64,

			eXAT_Unit,

			eXAT_Dbl,
			eXAT_GUID,
			eXAT_Fcc,
			eXAT_Color,

			eXAT_DateTime,

			eXAT_Timer
		};

		//typedef XMLAttribute* XMLAttributePtr;

	private:
		CComBSTR	cbsValue;
		bool		m_bStringValid;	// Indicate what string valid for current value
		Type		m_eType;
		union
		{
			bool		bValue;
			int			nValue;
			DWORD		dwValue;
			double		dblValue;
			LONGLONG	llValue;
			ULONGLONG	ullValue;
			FOURCC		fccValue;
			COLORREF	crValue;
			GUID		gValue;
			SYSTEMTIME  timeValue;
			XMLUnit		unValue;
		};
		// Special for timer
		LONGLONG	m_llTimerOffset;

	public:

		XMLAttribute()
		{
			Reset();
		}

		virtual ~XMLAttribute()
		{
			cbsValue.Empty();
			m_bStringValid = false;
			m_eType = (Type)-1;
		}

// 	template <class T>
// 		XMLAttribute( const T& _cr)
// 		{
// 			Reset();
// 			Set( _cr );
// 		}
// 
// 		XMLAttribute( LPCWSTR _cr)
// 		{
// 			Reset();
// 			SetString( _cr ); 
// 		}
// 
// 		XMLAttribute( LPCSTR _cr)
// 		{
// 			Reset();
// 			SetString( _cr ); 
// 		}


		void Reset()
		{
			cbsValue.Empty();
			m_bStringValid = false;
			m_eType = eXAT_String;
			::ZeroMemory( &timeValue, sizeof(timeValue) );
			m_llTimerOffset = s_clockGlobal.GetTime();
		}
		

#define DECLATE_SET_TYPE( cpp_type, xml_type, member ) \
		inline void Set( const cpp_type& _tValue ) \
		{ \
			member = _tValue; \
			m_bStringValid = false;\
			m_eType = eXAT_##xml_type; \
		} \
		inline void Get( cpp_type& _tValue ) \
		{ \
			if( m_eType != eXAT_##xml_type ) \
				UpdateType( eXAT_##xml_type ); \
				\
			_tValue = member; \
		} \
		inline void Set##xml_type( const cpp_type& _tValue ) \
		{ \
			member = _tValue; \
			m_bStringValid = false;\
			m_eType = eXAT_##xml_type; \
		} \

#define DECLATE_GET_TYPE( cpp_type, xml_type, member ) \
		inline cpp_type Get##xml_type() \
		{ \
			if( m_eType == eXAT_##xml_type ) \
				return member; \
			\
			UpdateType( eXAT_##xml_type ); \
			return member; \
		}

#define DECLATE_ATTR_TYPE( cpp_type, xml_type, member ) \
		DECLATE_SET_TYPE( cpp_type, xml_type, member ) \
		DECLATE_GET_TYPE( cpp_type, xml_type, member ) 

		// DECLATE_ATTR_TYPE( LPCWSTR, String, cbsValue ); - special code for not update string
		DECLATE_ATTR_TYPE( XMLUnit, Unit, unValue );
		DECLATE_ATTR_TYPE( bool, Bool, bValue );
		DECLATE_ATTR_TYPE( int, Int, nValue );
		DECLATE_SET_TYPE( long, Int, nValue );
		DECLATE_ATTR_TYPE( UINT, UInt, dwValue );
		DECLATE_SET_TYPE( ULONG, UInt, dwValue );
		DECLATE_ATTR_TYPE( double, Dbl, dblValue );
		DECLATE_ATTR_TYPE( LONGLONG, Int64, llValue );
		DECLATE_ATTR_TYPE( ULONGLONG, UInt64, ullValue );
		DECLATE_ATTR_TYPE( GUID, GUID, gValue );
		DECLATE_ATTR_TYPE( SYSTEMTIME, DateTime, timeValue );

		DECLATE_GET_TYPE( COLORREF, Color, crValue );
		DECLATE_GET_TYPE( FOURCC, Fcc, fccValue );

#undef DECLATE_ATTR_TYPE
#undef DECLATE_GET_TYPE
#undef DECLATE_SET_TYPE

		// For elapsed time
		inline void SetTimer( REFERENCE_TIME _rtNow )
		{
			cbsValue.Empty();
			m_eType = eXAT_Timer;
			m_llTimerOffset = s_clockGlobal.GetTime() + _rtNow;
		}

		inline REFERENCE_TIME GetTimer()
		{
			ATLASSERT(m_eType == eXAT_Timer);
			return s_clockGlobal.GetTime() - m_llTimerOffset;
		}

		// For float 
		inline void Get(float& _tValue)
		{ 
			if (m_eType != eXAT_Dbl)
				UpdateType(eXAT_Dbl);
				
			_tValue = (float)dblValue;
		} 

		inline void Set( const XMLAttribute* _tValue ) 
		{ 
			*this = *_tValue;
		}

// 		inline void Set( const XMLAttribute& _tValue ) 
// 		{ 
// 			*this = _tValue;
// 		}

		inline void Set( const LPCWSTR& str )
		{
			cbsValue = str;
			m_eType = eXAT_String;
			m_bStringValid = true;
		}

		inline void Set( const LPCSTR& str )
		{
			cbsValue = str;
			m_eType = eXAT_String;
			m_bStringValid = true;
		}

		inline void Set( const CComBSTR& str )
		{
			cbsValue = str;
			m_eType = eXAT_String;
			m_bStringValid = true;
		}

		inline void Set( const CComVariant& var )
		{
			switch( var.vt )
			{
			case VT_I1:
				{
					long lVal = var.cVal; // short
					SetInt( lVal );
					break;
				}
			case VT_UI1:
				{
					long lVal = var.bVal; // short
					SetInt( lVal );
					break;
				}
			
			case VT_I2:
				{
					long lVal = var.iVal; // short
					SetInt( lVal);
					break;
				}
			case VT_UI2:
				{
					long lVal = var.uiVal; // short
					SetInt( lVal);
					break;
				}
			case VT_I4:
			case VT_UI4:
			case VT_INT:
			case VT_UINT:
				SetInt( var.lVal);
				break;
			case VT_I8:
			case VT_UI8:
				SetInt64( var.llVal);
				break;
			case VT_R4:
				{
					double dblVal = var.fltVal;
					SetDbl( dblVal);
					break;
				}
			case VT_R8:
				SetDbl( var.dblVal );
				break;
			case VT_BSTR:
			case VT_LPWSTR:
				SetString( var.bstrVal );
				break;
			case VT_BOOL:
				SetBool( var.boolVal != VARIANT_FALSE );
				break;
			case VT_LPSTR:
				SetString( var.pcVal );
				break;	
			}
		}

		inline void SetString( const LPCWSTR& str )
		{
			cbsValue = str;
			m_eType = eXAT_String;
			m_bStringValid = true;
		}
		inline void SetString( const LPCSTR& str )
		{
			cbsValue = str;
			m_eType = eXAT_String;
			m_bStringValid = true;
		}

		inline Type GetType() const { return m_eType; }

		inline CComBSTR& GetString( BSTR* _pbsValue = NULL )
		{
			if( m_eType == eXAT_String || m_bStringValid )
			{
				if( _pbsValue )
					cbsValue.CopyTo( _pbsValue );
				return cbsValue;
			}	

			UpdateString();

			if( _pbsValue )
				cbsValue.CopyTo( _pbsValue );
			return cbsValue;
		}
		inline void Get( LPCWSTR& _rszValue )
		{
			if( m_eType != eXAT_String || !m_bStringValid )
				UpdateString();

			_rszValue = cbsValue.m_str;
		}

		// Special for string
// 		inline void Set( const double& str )
// 		{
// 			dblValue = str;
// 			m_eType = eXAT_Dbl;
// 			m_bStringValid = false;
// 		}
// 		inline void SetDbl( const double& str )
// 		{
// 			dblValue = str;
// 			m_eType = eXAT_Dbl;
// 			m_bStringValid = false;
// 		}
// 		inline double GetDbl()
// 		{
// 			if( m_eType != eXAT_Dbl )
// 				UpdateType( eXAT_Dbl );
// 
// 			return dblValue;
// 		}
// 		inline void Get( double& _rszValue )
// 		{
// 			if( m_eType != eXAT_Dbl )
// 				UpdateType( eXAT_Dbl );
// 
// 			_rszValue = dblValue;
// 		}

		inline void SetFcc( const FOURCC& fcc )
		{
			fccValue = fcc;
			m_eType = eXAT_Fcc;
			m_bStringValid = false;
		}

		inline void SetColor( const COLORREF& col )
		{
			crValue = col;
			m_eType = eXAT_Color;
			m_bStringValid = false;
		}

		inline bool IsEmpty() const
		{
			return cbsValue.m_str == NULL;
		}

		inline operator bool() const 
		{
			return !IsEmpty();
		}
		
	private:
		void UpdateType( Type _eType )
		{
			// Check number conversion
			if( m_eType != eXAT_String && !m_bStringValid )
			{
				// Convert to string
				UpdateString();
			}

			ConvertFromString( _eType );
		}

		void ConvertFromString( Type _eType)
		{
			// Convert from string
			if( !cbsValue.Length() )
			{
				::ZeroMemory( &gValue, sizeof(gValue) );
			}
			else
			{
				// Convert from string to value
				LPWSTR pszEnd = NULL;
				switch( _eType )
				{
				case eXAT_String:
					break;
				case eXAT_Bool:
					bValue = Str2Bool( cbsValue );
					break;
				case eXAT_Int:
					nValue = (INT)_wcstoi64( cbsValue, &pszEnd, 0 );
					if( nValue == 0 && STR_IS_SAME_I( cbsValue, L"true") )
						nValue = 1;
					nValue *= *pszEnd == L'K' ? 1000 : *pszEnd == L'M' ? 1000 * 1000 : *pszEnd == L'G' ? 1000 * 1000 * 1000 : 1;
					break;
				case eXAT_UInt:
					dwValue = (UINT)_wcstoi64( cbsValue, &pszEnd, 0 );
					if( dwValue == 0 && STR_IS_SAME_I( cbsValue, L"true") )
						dwValue = 1;
					dwValue *= *pszEnd == L'K' ? 1000 : *pszEnd == L'M' ? 1000 * 1000 : *pszEnd == L'G' ? 1000 * 1000 * 1000 : 1;
					break;
				case eXAT_Dbl:
					dblValue = Str2Double( cbsValue );
					if( dblValue == 0.0 && STR_IS_SAME_I( cbsValue, L"true") )
						dblValue = 1.0;

					break;
				case eXAT_Int64:
					llValue = _wcstoi64( cbsValue, &pszEnd, 0 );
					if( llValue == 0 && STR_IS_SAME_I( cbsValue, L"true") )
						llValue = 1;
					llValue *= *pszEnd == L'K' ? 1000LL : *pszEnd == L'M' ? 1000LL * 1000 : *pszEnd == L'G' ? 1000LL * 1000 * 1000 : *pszEnd == L'T' ? 1000LL * 1000 * 1000 * 1000 : 1LL;
					break;
				case eXAT_UInt64:
					ullValue = _wcstoui64( cbsValue, &pszEnd, 0 );
					if( ullValue == 0 && STR_IS_SAME_I( cbsValue, L"true") )
						ullValue = 1;
					ullValue *= *pszEnd == L'K' ? 1000LL : *pszEnd == L'M' ? 1000LL * 1000 : *pszEnd == L'G' ? 1000LL * 1000 * 1000 : *pszEnd == L'T' ? 1000LL * 1000 * 1000 * 1000 : 1LL;
					break;
				case eXAT_Unit:
					unValue = XMLUtils::Str2Unit( cbsValue );
					break;
				case eXAT_Color:
					dwValue = Str2Color( cbsValue );
					break;
				case eXAT_Fcc:
					dwValue = Str2Fcc( cbsValue );
					break;
				case eXAT_GUID:
					gValue = GUIDFromStringW( cbsValue );
					break;
				case eXAT_DateTime:
					Str2SysTimeW( cbsValue, &timeValue );
					break;


				default:
					ATLASSERT( !"ConvertFromString(WRONG TYPE)" );
				}
			}

			if( m_eType != eXAT_Timer ) // For timer DO NOT update attribute type
				m_eType = _eType;
		}

		void UpdateString()
		{
			// Convert from string to value
			switch( m_eType )
			{
			case eXAT_String:
				break;
			case eXAT_Bool:
				cbsValue = bValue ? L"true" : L"false";
				break;
			case eXAT_Int:
				cbsValue = INT2STRW( nValue );
				break;
			case eXAT_UInt:
				cbsValue = HEX2STRW( dwValue );
				break;
			case eXAT_Dbl:
				cbsValue = DBL2STRW( dblValue );
				break;
			case eXAT_Unit:
				cbsValue = XMLUtils::Unit2Str( unValue, (LPWSTR)alloca(32) );
				break;
			case eXAT_Int64:
				cbsValue = INT64STRW( llValue );
				break;
			case eXAT_UInt64:
				cbsValue = HEX64STRW( ullValue );
				break;
			case eXAT_Color:
				cbsValue = COLOR2STRW( crValue );
				break;
			case eXAT_Fcc:
				cbsValue = FCC2NAMEW( fccValue );
				break;
			case eXAT_GUID:
				cbsValue = GUID2STR( gValue );
				break;
			case eXAT_DateTime:
				cbsValue = ST2STR( timeValue );
				break;
			case eXAT_Timer:
				{
					LONGLONG llTimer = GetTimer();
					cbsValue = DBL2STRW( (double)llTimer / DS_ONESEC );
					break;
				}
			default:
				ATLASSERT( !"UpdateString(wrong type)" );
			}

			// For elapsed time -> need every time recalc value
			m_bStringValid = (m_eType != eXAT_Timer);
		}
	};

	
	template <class T, class TNodePtr>
	class XMLNodeImpl : 
		public ObjectsMap
	{
		friend class XMLParse;
		
	public:

		typedef deque<pair<CComBSTR,TNodePtr> > TDeqNodes;

		static HRESULT CreateNode( T** _ppItem, LPCWSTR _pszName, T* _pParent = NULL, LPCWSTR _pszSwapID = NULL )
		{
			ATLASSERT(_ppItem != NULL);
			if(_ppItem == NULL) return E_POINTER;

			CComObject<T>* pComObject = NULL;
			HRESULT hr = CComObject<T>::CreateInstance(&pComObject);
			if (SUCCEEDED(hr))
			{
				pComObject->AddRef();
				pComObject->m_bValue = false;
				pComObject->m_pAttrNameOrValue->SetString( _pszName );
				pComObject->m_pParentNode = _pParent;
				pComObject->m_pszSwapID = _pszSwapID;

			//	pComObject->m_shEvent.Attach( ::CreateEvent(NULL, true, true, _pszName) );
				*_ppItem = pComObject;
			}
			else
			{
				*_ppItem = NULL;
			}	

			return hr;
		}


		static HRESULT CreateNode_I( T** _ppItem, XMLNamePart* _pName, T* _pParent = NULL )
		{
			ATLASSERT(_ppItem != NULL);
			if(_ppItem == NULL) return E_POINTER;

			CComObject<T>* pComObject = NULL;
			HRESULT hr = CComObject<T>::CreateInstance(&pComObject);
			if (SUCCEEDED(hr))
			{
				pComObject->AddRef();
				pComObject->m_bValue = false;
				//pComObject->m_pAttrNameOrValue->SetString( L"");
				pComObject->m_pAttrNameOrValue->GetString().Attach( ::SysAllocStringLen( _pName->pszPart, _pName->nLen ) );
				pComObject->m_pParentNode = _pParent;
			//	pComObject->m_shEvent.Attach( ::CreateEvent(NULL, true, true, _pName->pszPart) );
				if( _pParent )
					pComObject->m_pszSwapID = _pParent->m_pszSwapID;

				*_ppItem = pComObject;
			}
			else
			{
				*_ppItem = NULL;
			}	

			return hr;
		}

		static HRESULT CreateValue( T** _ppItem, LPCWSTR _pszName, T* _pParent = NULL )
		{
			ATLASSERT(_ppItem != NULL);
			if(_ppItem == NULL) return E_POINTER;

			CComObject<T>* pComObject = NULL;
			HRESULT hr = CComObject<T>::CreateInstance(&pComObject);
			if (SUCCEEDED(hr))
			{
				pComObject->AddRef();
				pComObject->m_bValue = true;
				pComObject->m_pAttrNameOrValue->SetString( _pszName );
				pComObject->m_pParentNode = _pParent;
			//	pComObject->m_shEvent.Attach( ::CreateEvent(NULL, true, true, _pszName) );
				*_ppItem = pComObject;

			}
			else
			{
				*_ppItem = NULL;
			}	

			return hr;
		}

	protected:
		typedef enum eValueType
		{
			eBasic,
			eParam,
			eXML,
			eRemove
		} eValueType;
	
		bool		m_bPrintOneLine;

		// TODO: If this flag set -> the first m_ppszHiddenNodes[0] - is attributes info node name
		bool		m_bAttributesInfo;
		LPCWSTR*	m_ppszHiddenNodes;
		LPCWSTR*	m_ppszLastNodes;

		class CAttributesMap:
			public CStrHashMap<XMLAttribute::TPtr>
		{
		public:
			CAttributesMap()
				: CStrHashMap( true )
			{
			}

			int FindAttribute(XMLNamePart* _pName) const
			{
				//UpdateIndex();

				UINT64 n64Key = Str2Hash64_I(_pName->pszPart, m_bIgnoreCase, _pName->nLen);
				TIndexMap::iterator Iter = m_mapIndex.find( n64Key );
				if( Iter != m_mapIndex.end() )
				{
					ATLASSERT( Iter->first == Str2Hash64_I(m_aKey[Iter->second], m_bIgnoreCase ) );
					//ATLASSERT( !_pName->pszPart || StrCmpIW( _pName->pszPart, m_aKey[Iter->second] ) == 0 );
					return Iter->second;
				}
				return -1;  // not found
			}
		};

		class CNodesMap:
			public CStrHashMultiMap<TNodePtr>
		{
		public:
			CNodesMap()
				: CStrHashMultiMap( true )
			{
			}

			int FindNode(XMLNamePart* _pName) const
			{
				//UpdateIndex();

				UINT64 n64Key = Str2Hash64_I(_pName->pszPart, m_bIgnoreCase, _pName->nLen );
				TIndexMap::iterator Iter = m_mapIndex.find( n64Key );
				if( Iter != m_mapIndex.end() )
				{
					ATLASSERT( Iter->second.size() > 0 );
					if( Iter->second.size() > (UINT)_pName->nOrder )
					{
						ATLASSERT( Iter->first == Str2Hash64_I(m_aKey[Iter->second[_pName->nOrder]], m_bIgnoreCase ) );
						//ATLASSERT( !key || StrCmpIW( key, m_aKey[ Iter->second[_pName->nOrder] ] ) == 0 ); // The _pName->pszPart could be e.g. lock=true
						return Iter->second[_pName->nOrder];
					}
				}

				return -1;  // not found
			}

			int FindNode(LPCWSTR _pszName, int nOrder = 0) const
			{
				//UpdateIndex();

				UINT64 n64Key = Str2Hash64_I(_pszName, m_bIgnoreCase, -1 );
				TIndexMap::iterator Iter = m_mapIndex.find( n64Key );
				if( Iter != m_mapIndex.end() )
				{
					ATLASSERT( Iter->second.size() > 0 );
					if( Iter->second.size() > (UINT)nOrder )
					{
						ATLASSERT( Iter->first == Str2Hash64_I(m_aKey[Iter->second[nOrder]], m_bIgnoreCase ) );
						//ATLASSERT( !key || StrCmpIW( key, m_aKey[ Iter->second[_pName->nOrder] ] ) == 0 ); // The _pName->pszPart could be e.g. lock=true
						return Iter->second[nOrder];
					}
				}

				return -1;  // not found
			}

			int GetValuesCount() const
			{
				//UpdateIndex();

				TIndexMap::iterator Iter = m_mapIndex.find( 0 );
				if( Iter != m_mapIndex.end() )
				{
					return (int)Iter->second.size();
				}

				return 0;
			}
		};

		// Special functions for get/set attributes values
	private:

		inline XMLAttribute::TPtr GetAttributePtr_Int( XMLNamePart* _pName, bool _bCreateNew, int _nIndex = -1 )
		{
			{
				CAutoRWL rw( m_pRWA, false );

				if( IsSwapID( _pName ) && m_cbsExternName )
				{
					// Store type (only if not stored before)
					if( !m_pAttrNameOrValue->IsEmpty() ) 
						return m_pAttrNameOrValue;
				}

				int nIndex = m_mapAttributes.FindAttribute( _pName );
				if( nIndex >= 0 )
					return m_mapAttributes[nIndex];
			}

			if( _bCreateNew )
			{
				CAutoRWL rw( m_pRWA, true );

				// Check again
				int nIndex = m_mapAttributes.FindAttribute( _pName );
				if( nIndex >= 0 )
					return m_mapAttributes[nIndex];

				CComBSTR cbsName( _pName->nLen, _pName->pszPart );

				XMLAttribute::TPtr pAttribute;
				XMLAttribute::CreateInstance( &pAttribute );

				if (_nIndex >= 0 && _nIndex < m_mapAttributes.GetSize())
				{
					m_mapAttributes.InsertAtIndex(_nIndex, cbsName, pAttribute);
				}
				else
				{
					m_mapAttributes.Add( cbsName, pAttribute );
					_nIndex = m_mapAttributes.GetSize() - 1;
				}

				return pAttribute;
			}

			return XMLAttribute::TPtr(NULL);
		}

		inline XMLAttribute::TPtr GetAttributePtr_Int_R( XMLNamePart* _pName, T** _ppNode )
		{
			{
				CAutoRWL rw( m_pRWA, false );

				if( IsSwapID( _pName ) && m_cbsExternName )
				{
					// Store type (only if not stored before)
					if( !m_pAttrNameOrValue->IsEmpty() ) 
						return m_pAttrNameOrValue;
				}

				int nIndex = m_mapAttributes.FindAttribute( _pName );
				if( nIndex >= 0 )
				{
					if( _ppNode )
					{
						((T*)this)->AddRef();
						*_ppNode = (T*)this;
					}

					return m_mapAttributes[nIndex];
				}
			}

			TNodePtr pParent( m_pParentNode );
			if( pParent )
				return pParent->GetAttributePtr_Int_R( _pName, _ppNode );

			return XMLAttribute::TPtr(NULL);
		}

		template<class T>
		bool AddAttribute_Int( XMLNamePart* _pName, const T& _crVal, bool _bUpdate, int _nIndex /*= -1*/ )
		{
			CAutoRWL rw( m_pRWA, true );

			if( IsSwapID( _pName ) )
			{
				// Store type (only if not stored before)
				if( !m_cbsExternName ) 
					m_cbsExternName = m_pAttrNameOrValue->GetString();
				
				// Change name
				XMLAttribute::TPtr pAttribute;
				XMLAttribute::CreateInstance( &pAttribute );
				pAttribute->Set( _crVal );

				ChangeName( pAttribute->GetString() );
				return true;
			}

			if( _bUpdate )
			{
				int nIndex = m_mapAttributes.FindAttribute( _pName );
				if( nIndex >= 0  )
				{
					m_mapAttributes[nIndex]->Set( _crVal );

					return true;
				}
			}

			CComBSTR cbsName( _pName->nLen, _pName->pszPart );

			XMLAttribute::TPtr pAttribute;
			XMLAttribute::CreateInstance( &pAttribute );
			pAttribute->Set( _crVal );

			if( _nIndex >= 0 && _nIndex < m_mapAttributes.GetSize() )
				return m_mapAttributes.InsertAtIndex( _nIndex, cbsName, pAttribute ) > 0;

			return m_mapAttributes.Add( cbsName, pAttribute ) > 0;
		}

		
		bool RemoveAttribute_Int( XMLNamePart* _pName )
		{
			CAutoRWL rw( m_pRWA, true );

			int nIndex = m_mapAttributes.FindAttribute( _pName );
			if( nIndex >= 0  )
			{
				m_mapAttributes.RemoveAt( nIndex );
				return true;
			}

			return false;
		}
	private:
		// The order for easy check main props
		mutable XMLAttribute::TPtr		m_pAttrNameOrValue;
		CComBSTR						m_cbsExternName;
		
		CAttributesMap			m_mapAttributes;

		CNodesMap				m_mapNodes;

		bool					m_bValue;	// This node is value, not real node, content stored in m_pAttrNameOrValue (set once in constructor)
		CComBSTR				m_cbsEmpty; // Fake: For return CComBSTR& value <-> name 

		// Do not use smart pointer -> for not have cicle refs
		T*						m_pParentNode;
	protected:
		// Protection for attributes, name
		mutable CRWSynchroLiteXML 	m_rwAttributes;
		mutable CRWSynchroLiteXML*	m_pRWA;

		// Protection for nodes 
		mutable CRWSynchroLiteXML	m_rwNodes;

		// This is swap id - not protected as set in constructor

		// Swap ID (if have, used m_cbsExternName)
		LPCWSTR					m_pszSwapID;

	private: 
		XMLNodeImpl(const XMLNodeImpl& src) {};
		XMLNodeImpl& operator=(const XMLNodeImpl& src) {};

		inline int IsSwapID( XMLNamePart* _pNamePart ) const
		{
			if( m_pszSwapID && m_pszSwapID[0] )
			{
				if( !_pNamePart )
					return 2;

				if( memcmp( m_pszSwapID, _pNamePart->pszPart, _pNamePart->nLen * sizeof(WCHAR) ) == 0 )
					return 1;
			}

			return 0;
		}

	public:

		XMLNodeImpl()
			: ObjectsMap( false ),
			  m_bValue( false ),
			  m_pRWA( NULL ),
			  m_pszSwapID( NULL ),
			  m_ppszHiddenNodes( ppszHiddenAttrInfoStr ),
			  m_ppszLastNodes( NULL ),
			  m_bAttributesInfo( true ),
			  m_pParentNode( NULL ),
			  m_rwAttributes( RWLITE_CS_SPIN, false ),
			  m_rwNodes( RWLITE_CS_SPIN, false )
		{
			m_pRWA = &m_rwAttributes;
			m_bPrintOneLine = false;

			XMLAttribute::CreateInstance( &m_pAttrNameOrValue );
		}

		~XMLNodeImpl()
		{
			Clear();
		}

		

		//////////////////////////////////////////////////////////////////////////
		// lock method
		void DisableRWA()
		{
			m_pRWA = NULL;
		}
		void EnableRWA()
		{
			m_pRWA = &m_rwAttributes;
		}
		void LockAttributes( bool _bWrite )
		{
			CRWSynchroLiteXML* pRW = m_pRWA;
			if( _bWrite && pRW )
				pRW->LockWrite();
			else if( pRW )
				pRW->LockRead();
		}

		void UnlockAttributes( bool _bWrite )
		{
			CRWSynchroLiteXML* pRW = m_pRWA;
			if( _bWrite && pRW )
				pRW->UnlockWrite();
			else if( pRW )
				pRW->UnlockRead();
		}



		//////////////////////////////////////////////////////////////////////////
		// aux methods

		LPCWSTR GetID( LPCWSTR* _pszID_Tag = NULL )
		{
			if( m_bValue )
				return false;

			if( !STR_IS_EMPTY(m_pszSwapID) )
			{
				if( _pszID_Tag )
					*_pszID_Tag = m_pszSwapID;

				CAutoRWL rw(m_rwAttributes, false);

				return m_cbsExternName.m_str ? m_pAttrNameOrValue->GetString() : NULL;
			}

			return NULL;
		}

		TNodePtr SetParent( T* _pParent, bool _bRemoveFromParent )
		{
			CAutoRWL rw( m_rwNodes, true );

			TNodePtr pParent( m_pParentNode );
			if( pParent && _bRemoveFromParent )
				pParent->RemoveNodePtr( (T*)this );
// 		
			m_pParentNode = _pParent;
			return pParent;
		}

		//////////////////////////////////////////////////////////////////////////
		// Object impl

		void ReleaseObjectsR( LPCWSTR pszType )
		{
			ReleaseObject( pszType );

			CAutoRWL rw(m_rwNodes, false);

			for( int i = 0; i < m_mapNodes.GetSize(); i++ )
			{
				m_mapNodes[i]->ReleaseObjectsR(pszType);
			}
		}

		void ReleaseObjectsR(const LPCWSTR* ppszRelease, const LPCWSTR* ppszKeep )
		{
			ReleaseObjects( ppszRelease, ppszKeep );

			CAutoRWL rw(m_rwNodes, false);

			for( int i = 0; i < m_mapNodes.GetSize(); i++ )
			{
				m_mapNodes[i]->ReleaseObjectsR(ppszRelease,ppszKeep);
			}
		}

		int CopyObjectsR( T* pNode, const LPCWSTR* ppszTypes = NULL )
		{
			int nCopy = CopyObjects( pNode, ppszTypes );

			CAutoRWL rw(m_rwNodes, false);

			for( int i = 0; i < m_mapNodes.GetSize(); i++ )
			{
				TNodePtr pChild = pNode->GetNodeAt( i );

				if( pChild )
					nCopy += m_mapNodes[i]->CopyObjects( pChild, ppszTypes );
			}

			return nCopy;
		}

		template<class TObject>
		int CollectObjectsR( LPCWSTR pszType, deque<pair<TObject,TNodePtr> >& rDeqCollect )
		{
			TObject pObject = GetObject<TObject>(pszType);
			if( pObject )
				rDeqCollect.push_back( make_pair(pObject,TNodePtr((T*)this)));

			int nCount = pObject ? 1 : 0;
			
			CAutoRWL rw(m_rwNodes, false);

			for( int i = 0; i < m_mapNodes.GetSize(); i++ )
			{
				nCount += m_mapNodes[i]->CollectObjectsR(pszType, rDeqCollect );
			}

			return nCount;
		}

		template<class TObject>
		int CollectObjects( LPCWSTR pszType, deque<pair<TObject,TNodePtr> >& rDeqCollect )
		{
			int nCount = 0;

			CAutoRWL rw(m_rwNodes, false);

			for( int i = 0; i < m_mapNodes.GetSize(); i++ )
			{
				TObject pObject = m_mapNodes[i]->GetObject<TObject>(pszType);
				if( pObject )
				{
					rDeqCollect.push_back( make_pair(pObject,m_mapNodes[i]));
					nCount++;
				}
			}

			return nCount;
		}

		template<class TObject>
		int CollectObjectsPtrR( LPCWSTR pszType, deque<pair<TObject*,TNodePtr> >& rDeqCollect )
		{
			TObject* pObject = GetObjectPtr<TObject>(pszType);
			if( pObject )
				rDeqCollect.push_back( make_pair(pObject,TNodePtr((T*)this)));

			int nCount = pObject ? 1 : 0;

			CAutoRWL rw(m_rwNodes, false);

			for( int i = 0; i < m_mapNodes.GetSize(); i++ )
			{
				nCount += m_mapNodes[i]->CollectObjectsPtrR(pszType, rDeqCollect );
			}

			return nCount;
		}

		// Get object 
		template< class TObject >
		TObject GetObjectR( LPCWSTR pszType, bool _bUpTree = false, T** ppFoundNode = NULL)
		{
			if( !_bUpTree )
			{
				TNodePtr pNext( (T*)this );
				while( pNext ) 
				{
					TObject pObject = pNext->GetObject<TObject>(pszType);
					if( pObject )
					{
						if( ppFoundNode )
							pNext.CopyTo( ppFoundNode );

						return pObject;
					}

					pNext = pNext->GetParentNode();
				}
			}
			else
			{
				ATLASSERT( !L"GetObjectR(up_tree) - NOT IMPL");
			}
			
			return TObject();
		}
		
		TNodePtr GetObject_XMLNode( LPCWSTR pszType, bool _bCreateNew = false, bool* _pbNewCreated = false )
		{
			if( _pbNewCreated )
				*_pbNewCreated = false;

			TNodePtr pObject = GetObject<TNodePtr>( pszType );
			if( !pObject && _bCreateNew)
			{
				if( _pbNewCreated )
					*_pbNewCreated = true;

				T::CreateNode( &pObject, Name(true), (T*)this, m_pszSwapID );
				SetObject(pszType, pObject);
			}

			return pObject;
		}

		bool IsValue() const  { return m_bValue; }

		inline CComBSTR& Value() 
		{ 
			ATLASSERT( m_bValue ); 
			CAutoRWL rw( m_pRWA, false); 

			return m_bValue ? m_pAttrNameOrValue->GetString() : m_cbsEmpty; 
		}

		inline CComBSTR& Name( bool _bRealName = false ) 
		{ 
			CAutoRWL rw( m_pRWA, false);

			return m_bValue ? m_cbsEmpty : (_bRealName && m_cbsExternName.m_str ? m_cbsExternName : m_pAttrNameOrValue->GetString()); 
		}

		void Clear( bool _bClearName = false )
		{
			{
				CAutoRWL rw( m_pRWA, true );

				if( _bClearName )
					m_pAttrNameOrValue->GetString().Empty();

				m_mapAttributes.RemoveAll();
			}

			ReleaseObject(NULL);

			ClearNodes();
		}

		void ClearNodes(const LPCWSTR* ppszClear = NULL, const LPCWSTR* ppszKeep = NULL )
		{
			CAutoRWL rw(m_rwNodes, true);

			if( !ppszClear )
			{
				if( ppszKeep )
				{
					for( int i = m_mapNodes.GetSize() - 1; i >= 0; i-- )
					{
						if( !InStringList( m_mapNodes.GetKeyAt(i), ppszKeep ) )
						{
							m_mapNodes.RemoveAt(i);
						}
					}
				}
				else
				{
					m_mapNodes.RemoveAll();
				}
			}
			else
			{
				while( *ppszClear )
				{
					if( !InStringList( *ppszClear, ppszKeep ) )
					{
						// For remove with duplicated names
						if( !m_mapNodes.Remove( *ppszClear, 0 ) )
							ppszClear++;
					}
					else
					{
						ppszClear++;
					}
				}
			}
		}

		int ClearAttributesByPrefix(LPCWSTR _pszPrefix, const LPCWSTR* ppszKeep = NULL)
		{
			CAutoRWL rw(m_pRWA, true);

			int nClear = 0;
			for (int i = m_mapAttributes.GetSize() - 1; i >= 0; i--)
			{
				if( IsPrefix(m_mapAttributes.GetKeyAt(i), _pszPrefix) && (!ppszKeep || !InStringList(m_mapAttributes.GetKeyAt(i), ppszKeep)))
				{
					m_mapAttributes.RemoveAt(i);
					++nClear;
				}
			}

			return nClear;
		}

		int ClearAttributes(const LPCWSTR* ppszClear = NULL, const LPCWSTR* ppszKeep = NULL )
		{
			CAutoRWL rw( m_pRWA, true );

			int nClear = 0;
			if( !ppszClear )
			{
				if( ppszKeep )
				{
					for( int i = m_mapAttributes.GetSize() - 1; i >= 0; i-- )
					{
						if( !InStringList( m_mapAttributes.GetKeyAt(i), ppszKeep ) )
						{
							m_mapAttributes.RemoveAt(i);
							++nClear;
						}
					}
				}
				else
				{
					nClear = m_mapAttributes.GetSize();
					m_mapAttributes.RemoveAll();
				}
			}
			else
			{
				while( *ppszClear )
				{
					if( !InStringList( *ppszClear, ppszKeep ) )
					{
						m_mapAttributes.Remove( *ppszClear );
						++nClear;
					}

					ppszClear++;
				}
			}

			return nClear;
		}

		inline bool HaveAttributes() const 
		{
			CAutoRWL rw( m_pRWA, true );

			return m_mapAttributes.GetSize() > 0;
		}

		inline bool HaveNodes() const 
		{
			CAutoRWL rw(m_rwNodes, false);

			for( int i = 0; i < m_mapNodes.GetSize(); i++ )
			{
				if( m_mapNodes.GetKeyAt(i) )
				{
					ATLASSERT( !m_mapNodes[i]->m_bValue );
					return true;
				}
			}

			return false;
		}

		// node like <tag>Value</tag> (no attributes, no parent nodes)
		inline bool IsBasicNode() const
		{
			return !HaveAttributes() && !HaveNodes();
		}

		inline TNodePtr GetParentNode( LPCWSTR _pszParentName = NULL ) const 
		{
			TNodePtr pParent = (TNodePtr)m_pParentNode;
			if( !_pszParentName || !_pszParentName[0] )
				return pParent;

			if( pParent )
			{
				if( pParent->IsName( _pszParentName ) )
					return pParent;

				return pParent->GetParentNode( _pszParentName );
			}

			return TNodePtr(NULL);
		}

		inline TNodePtr GetParentNode(const LPCWSTR* _ppszParentNames, WCHAR _wszSeparator = 0 ) const
		{
			TNodePtr pParent = (TNodePtr)m_pParentNode;
			if( pParent )
			{
				LPCWSTR pszSuffix = NULL;
				if( pParent->IsNames( _pszParentName, pszSuffix, _wszSeparator ) )
					return pParent;

				return pParent->GetParentNode( _ppszParentNames, _wszSeparator );
			}

			return TNodePtr(NULL);
		}

		// Note: the returned string NOT PROTECTED
		inline const CComBSTR GetName( BSTR* _pbsName = NULL, bool _bRealName = false )  
		{ 
			if( !m_bValue && _pbsName )
				Name(_bRealName).CopyTo( _pbsName );

			return Name(_bRealName); 
		}

		inline const TNodePtr GetRootNode()
		{
			TNodePtr pParent;
			TNodePtr pNext( (T*)this );
			while( pNext && (pParent = pNext->GetParentNode()) ) // NOT return the top name 
			{
				pNext = pParent;
			}

			return pNext;
		}

		inline const LPCWSTR GetFullName( BSTR* _pbsName, T* pRoot = NULL, IMNode* pMNodeRoot = NULL ) const
		{
			ATLASSERT( _pbsName );
			if( IsValue() || !_pbsName )
				return NULL;

			deque<CComBSTR> deqNames;
			TNodePtr pParent( (T*)this );
			while( pParent && pParent->m_pParentNode && pParent != pRoot ) // NOT return the top name 
			{
				if( pMNodeRoot && pParent.IsEqualObject( pMNodeRoot ) )
					break;

				deqNames.push_front( pParent->GetName() );
				pParent = TNodePtr(pParent->m_pParentNode);
			}

			if( !deqNames.size() )
				return NULL;

			CComBSTR cbsRes = deqNames.front();
			deqNames.pop_front();
			while( deqNames.size() )
			{
				cbsRes.Append( L"::" );
				cbsRes.Append( deqNames.front() );

				deqNames.pop_front();
			}

			*_pbsName = cbsRes.Detach();
			return *_pbsName;
		}

		inline const bool IsName( LPCWSTR _pszName, LPCWSTR* _pszSuffix = NULL, WCHAR wchSeparor = L'.' )  
		{ 
			ATLASSERT( !m_bValue );
			if( m_bValue ) return false;

			CComBSTR cbsName = Name(true);
			
			if( !_pszName )
				return cbsName.m_str ? true : false;

			if( !_pszSuffix )
			{
				if( cbsName.m_str && lstrcmpiW( cbsName.m_str, _pszName) == 0 )
					return true;
			}
			else
			{
				int nLen = lstrlenW( _pszName );
				if( cbsName.m_str && StrCmpNIW( cbsName.m_str, _pszName, nLen ) == 0 && (cbsName.m_str[nLen] == wchSeparor || cbsName.m_str[nLen] == 0))
				{
					if( cbsName.m_str[nLen] != 0)
						*_pszSuffix = cbsName.m_str + nLen;
					return true;
				}
			}

			return false; 
		}

		inline const int IsNames(const LPCWSTR* _ppszNames, BSTR* _pbsSuffix = NULL, WCHAR wchSeparator = L'.' )
		{ 
			if( m_bValue || !_ppszNames ) return -1;

			CComBSTR cbsName = Name(true);

			if( cbsName.m_str )
			{
				int nIndex = 0;
				while( _ppszNames[nIndex] )
				{
					if( !_pbsSuffix)
					{
						if( lstrcmpiW( cbsName.m_str, _ppszNames[nIndex]) == 0 )
							return nIndex;
					}
					else
					{
						int nLen = lstrlenW( _ppszNames[nIndex] );
						if( StrCmpNIW( cbsName.m_str, _ppszNames[nIndex], nLen ) == 0 && (cbsName.m_str[nLen] == wchSeparator || cbsName.m_str[nLen] == 0))
						{
							if( cbsName.m_str[nLen] != 0)
								*_pbsSuffix = CComBSTR(cbsName.m_str + nLen + 1).Detach();

							return nIndex;
						}
					}

					nIndex++;
				}
			}

			return -1; 
		}

		inline const bool HaveNameR( LPCWSTR _pszName, int* _pnDeep = NULL ) const 
		{ 
			if( !m_bValue )
			{
				CAutoRWL rw(m_pRWA, false );

				if( !_pszName )
					return m_pAttrNameOrValue->IsEmpty() ? false : true; 
				
				if( !m_pAttrNameOrValue->IsEmpty() && lstrcmpiW( m_pAttrNameOrValue->GetString(), _pszName) == 0 )
					return true;
			}

			if( _pnDeep )
				(*_pnDeep)++;

			TNodePtr pParent = (TNodePtr)m_pParentNode;
			if( pParent  )
				return pParent->HaveNameR( _pszName, _pnDeep );
		
			if( _pnDeep )
				*_pnDeep = -1;

			return false; 
		}

		inline bool IsEmpty() const
		{
			ATLASSERT( !m_bValue );
			if( m_bValue ) return false;

			{
				CAutoRWL rw( m_pRWA, false );

				if( m_mapAttributes.GetSize() != 0)
					return false;
			}

			CAutoRWL rw(m_rwNodes, false);

			if( m_mapNodes.GetSize() != 0)
				return false;

			return true;
		}

		bool IsEqual( T* _pNode )
		{
			if( !_pNode )
			{
				return IsEmpty();
			}

			ATLASSERT( !m_bValue );
			if( m_bValue ) return false;


			{
				CAutoRWL rw(m_pRWA, false );

				if( m_pAttrNameOrValue->GetString() != _pNode->GetName() )
				{
					return false;
				}

				if( GetAttributesCount() != _pNode->GetAttributesCount() )
				{
					return false;
				}

				for( int i = 0; i < _pNode->GetAttributesCount(); i++ )
				{
					CComBSTR cbsValueLocal = GetStringAttribute_Safe( _pNode->GetAttributeName(i) );
					CComBSTR cbsValueExt = _pNode->GetAttributeValue( i );
					if( cbsValueLocal != cbsValueExt )
					{
						return false;
					}
				}
			}

			CAutoRWL rw(m_rwNodes, false);

			if( m_mapNodes.GetSize() != _pNode->GetNodesCount() )
			{
				return false;
			}

			for( int i = 0; i < _pNode->GetNodesCount(); i++ )
			{
				if( m_mapNodes[i]->IsValue() || _pNode->GetNodeAt(i)->IsValue() )
				{
					if( m_mapNodes[i]->IsValue() != _pNode->GetNodeAt(i)->IsValue() ||
						m_mapNodes[i]->Value() != _pNode->GetNodeAt(i)->Value() )
					{
						return false;
					}
				}

				if( !m_mapNodes[i]->IsEqual( _pNode->GetNodeAt(i) ) )
					return false;
			}

			return true;
		}

		// return value: 
		// -1 -> diffrent name or wronf type
		// 0 -> same node
		// 1 -> diffrence only in attribures
		// 2 -> difference in nodes
		// 3 -> difference in node and attributes
// 		int CompareNode( T* _pNode, bool _bIgnoreName, T** _ppDiffrence )
// 		{
// 			if( !_pNode )
// 			{
// 				return IsEmpty() ? 0 : -1;
// 			}
// 
// 			ATLASSERT( !m_bValue );
// 			if( m_bValue ) return -1;
// 
// 
// 			{
// 				CAutoRWL rw(m_pRWA, false );
// 
// 				if( m_pAttrNameOrValue->GetString() != _pNode->GetName() )
// 				{
// 					return false;
// 				}
// 
// 				for( int i = 0; i < _pNode->GetAttributesCount(); i++ )
// 				{
// 					CComBSTR cbsValueLocal = GetStringAttribute( _pNode->GetAttributeName(i) );
// 					CComBSTR cbsValueExt = _pNode->GetAttributeValue( i );
// 					if( cbsValueLocal != cbsValueExt )
// 					{
// 						return false;
// 					}
// 				}
// 			}
// 
// 			CAutoRWL rw(m_rwNodes, false);
// 
// 			if( m_mapNodes.GetSize() != _pNode->GetNodesCount() )
// 			{
// 				return false;
// 			}
// 
// 			for( int i = 0; i < _pNode->GetNodesCount(); i++ )
// 			{
// 				if( m_mapNodes[i]->IsValue() || _pNode->GetNodeAt(i)->IsValue() )
// 				{
// 					if( m_mapNodes[i]->IsValue() != _pNode->GetNodeAt(i)->IsValue() ||
// 						m_mapNodes[i]->Value() != _pNode->GetNodeAt(i)->Value() )
// 					{
// 						return false;
// 					}
// 				}
// 
// 				if( !m_mapNodes[i]->IsEqual( _pNode->GetNodeAt(i) ) )
// 					return false;
// 			}
// 
// 			return true;
// 		}

		
		//////////////////////////////////////////////////////////////////////////
		// Node values

		bool SetValue( LPCWSTR _pszValue )
		{
			CAutoRWL rw(m_rwNodes, true);

			for( int i = 0; i < m_mapNodes.GetSize(); i++ )
			{
				if( m_mapNodes[i]->m_bValue )
				{
					// Update value
					ATLASSERT( m_mapNodes[i]->m_bValue );

					CAutoRWL rwa( m_mapNodes[i]->m_pRWA, true );

					m_mapNodes[i]->m_pAttrNameOrValue->SetString( _pszValue );
					return true;
				}
			}

			// New value
			TNodePtr pValue;
			CreateValue( &pValue, _pszValue, (T*)this );
			m_mapNodes.Add( NULL, pValue );

			return true;
		}

		bool ClearValue()
		{
			CAutoRWL rw(m_rwNodes, true);

			bool bClear = false;
			for( int i = m_mapNodes.GetSize() - 1; i >= 0 ; i-- )
			{
				if( m_mapNodes[i]->m_bValue )
				{
					// Update value
					m_mapNodes.RemoveAt( i );
					bClear = true;
				}
			}

			return bClear;
		}

		bool AddValue( LPCWSTR _pszValue )
		{
			ATLASSERT( _pszValue && _pszValue[0] );
			if( !_pszValue || !_pszValue[0] )
				return false;

			CAutoRWL rw(m_rwNodes, true);

			if( m_mapNodes.GetSize() > 0 && m_mapNodes.LastValue()->IsValue() )
			{
				CAutoRWL rwa( m_mapNodes.LastValue()->m_pRWA, true );

				m_mapNodes.LastValue()->m_pAttrNameOrValue->GetString().Append( _pszValue );
				return true;
			}

			TNodePtr pValue;
			CreateValue( &pValue, _pszValue, (T*)this );
			return m_mapNodes.Add( NULL, pValue ) > 0;
		}

		// Note: the returned string NOT PROTECTED
		inline CComBSTR GetValue( int _nIndex = 0, BSTR* _pbsValue = NULL ) 
		{
			ATLASSERT( !m_bValue );

			TNodePtr pValue = GetNodeValue_Int( _nIndex );
			if( pValue )
			{
				if( _pbsValue )
					pValue->Value().CopyTo( _pbsValue );

				return pValue->Value();
			}

			return NULL;
		}

		bool SetNodeValue( LPCWSTR _wszName, LPCWSTR _wszValue, bool _bCreateNew = false )
		{
			ATLASSERT( _wszName );
			if( !_wszName ) return false;

			TNodePtr pNewNode = GetNodePtr( _wszName, _bCreateNew );
			if( !pNewNode ) return false;

			pNewNode->SetValue( _wszValue );

			return true;
		}

		LPCWSTR GetNodeValue( LPCWSTR _wszName, LPCWSTR _wsDef = NULL ) 
		{
			const TNodePtr pNode = GetNodePtr( _wszName );
			if( !pNode ) return _wsDef;

			LPCWSTR pszValue = pNode->GetValue();

			return pszValue ? pszValue : _wsDef;
		}

		// Return 
		LPCWSTR GetAutoValue( LPCWSTR _wszName, LPCWSTR _wsDef = NULL ) 
		{
			TNodePtr pNode;
			LPCWSTR pszValue = NULL;
			XMLAttribute::TPtr pAttr = GetAttribute( _wszName, NULL, &pNode);
			if( !pAttr && pNode )
			{
				pszValue = pNode->GetValue();
			}
			else if( pAttr )
			{
				pszValue = pAttr->GetString();
			}

			return pszValue ? pszValue : _wsDef;
		}


		bool GetNodeBoolValue( LPCWSTR _wszName, bool _bDefValue = false, bool* _pbFind = NULL )  
		{
			LPCWSTR pszRes = GetNodeValue( _wszName );
			if( pszRes ) 
			{
				if( _pbFind )
					*_pbFind = true;

				return Str2Bool( pszRes );
			}

			return _bDefValue;
		}

		int GetNodeIntValue( LPCWSTR _wszName, INT _nDef = 0, bool* _pbFind = NULL ) 
		{
			LPCWSTR pszRes = GetNodeValue( _wszName );
			if( pszRes ) 
			{
				if( _pbFind ) *_pbFind = true;

				LPWSTR pEnd = 0;
				int nRes = wcstol( pszRes, &pEnd, 0 );
				return nRes;
			}

			return _nDef;
		}

		//////////////////////////////////////////////////////////////////////////
		// Nodes

		inline bool ChangeName( LPCWSTR _szName )
		{
			ATLASSERT( !m_bValue && _szName);
			if( m_bValue || !_szName ) return false;

			{
				CAutoRWL rw(m_pRWA, true);

				if( m_pAttrNameOrValue->GetString() == _szName )
					return false;

				m_pAttrNameOrValue->SetString( _szName );
			}

			TNodePtr pParent = (TNodePtr)m_pParentNode;
			if( pParent )
			{
				CAutoRWL rwp( pParent->m_rwNodes, true );

				// Not need ASSERT as can be 'fake' parent relation for attributes look-up
				int nIdx = pParent->m_mapNodes.FindVal( TNodePtr((T*)this) );
				if( nIdx >= 0 )
				{
					pParent->m_mapNodes.SetAtIndex( nIdx, _szName, TNodePtr((T*)this) );
					
					return true;
				}
			}

			return pParent ? false : true;
		}

		void RecurseChangeName( LPCWSTR _szName, LPCWSTR _szNewName  )
		{
			if( m_bValue ) return;

			ChangeName( _szNewName );
			
			CAutoRWL rw(m_rwNodes, true); // The write parent lock inside ChangeName()

			for( int i = 0; i < m_mapNodes.GetSize(); i++ )
			{
				m_mapNodes[i]->RecurseChangeName( _szName, _szNewName  );
			}
		}



		//////////////////////////////////////////////////////////////////////////
		// Get/Add/Remove nodes

		int GetNodesCount( LPCWSTR _pszNodeName = NULL ) const
		{
			CAutoRWL rw(m_rwNodes, false);

			if( STR_IS_EMPTY(_pszNodeName))
				return m_mapNodes.GetSize();

			// For do no not return -1 if node not found
			int nDuplicates = m_mapNodes.KeyDuplicates(_pszNodeName);
			return MAX(0, nDuplicates);
		}

		int GetRealNodesCount(const LPCWSTR* _ppszHiddenNodes = NULL) const
		{
			CAutoRWL rw(m_rwNodes, false);

			if( !_ppszHiddenNodes )
				_ppszHiddenNodes = m_ppszHiddenNodes;

			if( !_ppszHiddenNodes )
				return m_mapNodes.GetSize() - m_mapNodes.GetValuesCount();

			int nCount  = 0;
			for( int i = 0; i < m_mapNodes.GetSize(); i++ )
			{
				if( !m_mapNodes[i]->IsValue() && m_mapNodes[i]->IsNames( _ppszHiddenNodes ) < 0 )
				{
					nCount++;
				}
			}

			return nCount;
		}

		TNodePtr operator [](int _nIndex)
		{
			CAutoRWL rw(m_rwNodes, false);

			ATLASSERT( _nIndex >= 0 && _nIndex < m_mapNodes.GetSize() );
			if( _nIndex < 0 || _nIndex >= m_mapNodes.GetSize() )
					return m_mapNodes.LastValue();

			return m_mapNodes[_nIndex];
		}
		TNodePtr GetNodeAt(int _nIndex) 
		{
			CAutoRWL rw(m_rwNodes, false);

			ATLASSERT( _nIndex >= 0 && _nIndex < m_mapNodes.GetSize() );
			if( _nIndex < 0 || _nIndex >= m_mapNodes.GetSize() )
				return m_mapNodes.LastValue();

			return m_mapNodes[_nIndex];
		}
		TNodePtr GetRealNodeAt(int _nIndex, const LPCWSTR* _ppszHiddenNodes = NULL)
		{
			CAutoRWL rw(m_rwNodes, false);

			if( !_ppszHiddenNodes )
				_ppszHiddenNodes = m_ppszHiddenNodes;

			for( int i = 0; i < m_mapNodes.GetSize(); i++ )
			{
				if( !m_mapNodes[i]->IsValue() && m_mapNodes[i]->IsNames( _ppszHiddenNodes ) < 0 )
				{
					if( _nIndex <= 0 )
						return m_mapNodes[i];

					_nIndex--;
				}
			}

			return (TNodePtr)NULL;
		}

		HRESULT Reorder( T* pNode, int _nOrderChange, const LPCWSTR* _ppszHiddenNodes = NULL )
		{
			if( _nOrderChange == 0 )
				return S_FALSE;

			CAutoRWL rw(m_rwNodes, true);

			int nSize = m_mapNodes.GetSize();

			for( int i = 0; i < nSize; i++ )
			{
				if( m_mapNodes[i] == pNode )
				{
					int nInsertAt = -1;
					int nRemoveAt = i;
					while( _nOrderChange > 0 && i < m_mapNodes.GetSize() )
					{
						if( !m_mapNodes[i]->IsValue() && m_mapNodes[i]->IsNames( _ppszHiddenNodes ) < 0 )
						{
							nInsertAt = i + 2;
							nInsertAt = MIN( nInsertAt, nSize );
							_nOrderChange--;
						}

						i++;
					}
					while( _nOrderChange < 0 && i >= 0 )
					{
						if( !m_mapNodes[i]->IsValue() && m_mapNodes[i]->IsNames( _ppszHiddenNodes ) < 0 )
						{
							nInsertAt = i - 1;
							nInsertAt = MAX( 0, nInsertAt );
							_nOrderChange++;
						}

						i--;
					}

					

					if( nInsertAt < 0 || nInsertAt == nRemoveAt )
						return S_FALSE;

					// Add new
					CComBSTR cbsKey = m_mapNodes.GetKeyAt( nRemoveAt );
					TNodePtr pValue = m_mapNodes[nRemoveAt];

					m_mapNodes.InsertAtIndex( nInsertAt, cbsKey, pValue );
					
					RemoveNodeAt(nInsertAt > nRemoveAt ? nRemoveAt : nRemoveAt + 1 );
				}
			}

			return S_OK;
		}

		const TNodePtr GetNodeAt(int _nIndex, LPCWSTR _pszNodeName = NULL ) const
		{
			CAutoRWL rw(m_rwNodes, false);

			if (!STR_IS_EMPTY(_pszNodeName))
			{
				int nNodeIdx = m_mapNodes.FindNode( _pszNodeName, _nIndex );
				if (nNodeIdx < 0 || nNodeIdx >= m_mapNodes.GetSize() )
					return (TNodePtr)NULL;

				return m_mapNodes[nNodeIdx];
			}

			ATLASSERT( _nIndex >= 0 && _nIndex < m_mapNodes.GetSize() );
			if( _nIndex < 0 || _nIndex >= m_mapNodes.GetSize() )
				return m_mapNodes.LastValue();

			return m_mapNodes[_nIndex];
		}

		
		TNodePtr AddNodePtr( LPCWSTR _wszName, LPCWSTR _wszValue = NULL, int _nInsertAt = -1 )
		{
			ATLASSERT( !m_bValue && _wszName );
			if( m_bValue || !_wszName ) return (TNodePtr)NULL;

			XMLNamePart xmlName = {0};
			TNodePtr pNode = FindNodePtr_Int( _wszName, -1, &xmlName, true, -1 );
			if( !pNode )
				return (TNodePtr)NULL;

			return pNode->AddNodePtr_Int( &xmlName, _wszValue, _nInsertAt );
		}

		// if e.g.  GetNodePtr( "none-1::node-2::attr-name" ) -> ppLastNode will be "none-1::node-2" and GetNodePtr return NULL
		// Used "node_name[idx]::attr_name" for access node with INDEX (from 0 to 99999)
		//		"node_name[id]::attr_name" for access node by 'id' - any NOT INDEX like 'id123', 
		//				e.g. "peer[gasSnayqFR]" return node "peer" with attribute 'id' = "gasSnayqFR"
		//				if _bCreateNew is true -> create new node and add 'id' attribute to this node 
		TNodePtr GetNodePtr( LPCWSTR _wszName, bool _bCreateNew = false, int _nInsertAt = -1, T** ppLastNode = NULL, LPCWSTR* ppszLastName = NULL )
		{
			if( !_wszName || !_wszName[0] ) return TNodePtr((T*)this);

			return FindNodePtr_Int( _wszName, -1, NULL, _bCreateNew, _nInsertAt, ppLastNode, ppszLastName );
		}

		TNodePtr FindNodeByAttribute( LPCWSTR _wszAttrName, LPCWSTR _wszAttrValue, int* _pnIndex = 0, bool _bCaseSensetive = false, LPCWSTR _pszNodeName = NULL )
		{
			CComBSTR cbsFindValue = _wszAttrValue;
			if( !_bCaseSensetive )
				::CharLowerW( cbsFindValue.m_str );

			if( _pnIndex )
				*_pnIndex = -1;

			CAutoRWL rw(m_rwNodes, false);

			for( int i = 0; i < m_mapNodes.GetSize(); i++ )
			{
				if( !m_mapNodes[i]->IsValue() )
				{
					// Check for node name
					if (!STR_IS_EMPTY(_pszNodeName) &&
						!STR_IS_SAME_I(m_mapNodes.GetKeyAt(i), _pszNodeName) )
					{
						continue;
					}

					CComBSTR cbsValue = m_mapNodes[i]->GetStringAttribute_Safe( _wszAttrName );
					if( !cbsValue ) 
						continue;

					if( !_bCaseSensetive )
						::CharLowerW( cbsValue.m_str );

					if( cbsValue == cbsFindValue )
					{
						if( _pnIndex )
							*_pnIndex = i;

						return m_mapNodes[i];
					}
				}
			}

			return (TNodePtr)NULL;
		}

		bool HaveTopNode( LPCWSTR _wszName )
		{
			CAutoRWL rw(m_rwNodes, false);

			int nIndex = m_mapNodes.FindKey( _wszName, 0 );
			if( nIndex >= 0 )
			{
				return true;
			}

			return false;
		}

		TNodePtr GetNodePtrIdx( LPCWSTR _wszName, int _nIndex, bool _bCreateNew = false, int _nInsertAt = -1)
		{
			ATLASSERT( !m_bValue );
			if( m_bValue ) return (TNodePtr)NULL;

			if( !_wszName || lstrlenW(_wszName) == 0 ) 
				return TNodePtr((T*)this);

			return FindNodePtr_Int( _wszName, _nIndex, NULL, false, _nInsertAt );
		}

		bool RemoveNodeAt( int nIndex )
		{
			ATLASSERT( !m_bValue );
			if( m_bValue ) return false;

			CAutoRWL rw(m_rwNodes, true);

			//ATLASSERT( nIndex >= 0 && nIndex < m_mapNodes.GetSize() );
			if( nIndex < 0 || nIndex >= m_mapNodes.GetSize() )
				return false;

			m_mapNodes.RemoveAt( nIndex );

			// Check for value merge
			if( nIndex > 1 && nIndex < m_mapNodes.GetSize() - 1 && m_mapNodes[nIndex]->IsValue() && m_mapNodes[nIndex-1]->IsValue() )
			{
				ATLASSERT( m_mapNodes[nIndex]->IsValue() && m_mapNodes[nIndex-1]->IsValue() );

				m_mapNodes[nIndex-1]->Value().AppendBSTR( m_mapNodes[nIndex]->Value() );
				m_mapNodes.RemoveAt( nIndex );
			}

			return true;
		}


		bool RemoveNodePtr( T* pRemove )
		{
			ATLASSERT( !m_bValue );
			if( m_bValue || !pRemove ) return false;

			CAutoRWL rw(m_rwNodes, true);

			int nIndex = m_mapNodes.FindVal( TNodePtr(pRemove) );
			if( nIndex >= 0 )
			{
				return RemoveNodeAt( nIndex );
			}

			return false;
		}

		void RemoveNode_R( LPCWSTR _wszName )
		{
			RemoveNode( _wszName );

			CAutoRWL rw(m_rwNodes, false);
			for( int i = 0; i < m_mapNodes.GetSize(); i++ )
			{
				m_mapNodes[i]->RemoveNode_R( _wszName );
			}
		}

		bool RemoveNode( LPCWSTR _wszName )
		{
			ATLASSERT( !m_bValue );
			if( m_bValue || !_wszName || !_wszName[0] ) return false;

			TNodePtr pNode = FindNodePtr_Int( _wszName, -1, NULL, false, -1 );
			if( !pNode ) return false;

			TNodePtr pParent( pNode->m_pParentNode );
			ATLASSERT( (T*)pNode != (T*)this );
			if( (T*)pNode == (T*)this || !pParent)
				return false;

			return pParent->RemoveNodePtr( pNode );
		}

		
		// _nDepth < 0 -> all tree
		// _nDepth = 0 -> only current level (this node)
		// _nDepth = 1 -> this node + child
		// etc.
		int CollectNodesR( LPCWSTR _pszName, TDeqNodes& _rDeqCollect, int _nDepth = -1, const LPCWSTR* ppszIgnoreNodes = NULL, LPCWSTR _pszPrefix = NULL )
		{
			LPCWSTR ppszNames[] = { _pszName, NULL };
			return CollectNodesR( ppszNames, _rDeqCollect, _nDepth, ppszIgnoreNodes, _pszPrefix );
		}

		int CollectNodesR(const LPCWSTR* _ppszNames, TDeqNodes& rDeqCollect, int _nDepth = -1, const LPCWSTR* ppszIgnoreNodes = NULL, LPCWSTR _pszPrefix = NULL )
		{
			if( ppszIgnoreNodes && IsNames( ppszIgnoreNodes ) >= 0 )
				return 0;

			// We not include name of top node -> for later access
			CComBSTR cbsPrefix = _pszPrefix;
			if( !_pszPrefix )
			{
				cbsPrefix = "";
			}
			else
			{
				if( _pszPrefix[0] )
					cbsPrefix.Append( L"::");

				cbsPrefix.Append( GetName() );
			}

			int nCount = 0;
			if( IsNames( _ppszNames ) >= 0 )
			{
				rDeqCollect.push_back( make_pair( cbsPrefix, TNodePtr((T*)this) ) );
				nCount++;
			}

			if( _nDepth == 0 )
				return nCount;

			CAutoRWL rw(m_rwNodes, false);

			for( int i = 0; i < m_mapNodes.GetSize(); i++ )
			{
				nCount += m_mapNodes[i]->CollectNodesR(_ppszNames, rDeqCollect, _nDepth - 1, ppszIgnoreNodes, cbsPrefix );
			}

			return nCount;
		}

		// _nOverrideType == 0 -> Not override
		// _nOverrideType == 1 -> Override
		// _nOverrideType == 2 -> Override & Remove attributes not presented in source (only with lists)
		int CopyTo( T* pNode, bool _bChangeName = true, int _nOverrideAttributes = 2, const LPCWSTR* _ppszIgnoreNodes = NULL )//, bool _bClearDest = TRUE, bool _bOverrideAttributes = TRUE, bool _bUpdateNodes = TRUE )
		{
			ATLASSERT( !m_bValue );
			if( m_bValue ) return -1;

			ATLASSERT( this != pNode );
			if( this == pNode ) return -1;

			if( !pNode )
				return -1;

			pNode->ClearNodes();
			pNode->m_bPrintOneLine = m_bPrintOneLine;
			
			// Do not sure about need to check m_pszSwapID - but for have like before update XML protection
			if( _bChangeName )
				pNode->ChangeName( Name( !STR_IS_EMPTY(m_pszSwapID) ) );
			
			CopyAttributes( pNode, _nOverrideAttributes );

			CAutoRWL rw(m_rwNodes, false);

			int nNodes = 0;
			for( int i = 0; i < m_mapNodes.GetSize(); i++ )
			{
				if( m_mapNodes[i]->m_bValue )
				{
					pNode->AddValue( m_mapNodes[i]->Value() );
				}
				else
				{
					if( !_ppszIgnoreNodes || !InStringList( m_mapNodes.GetKeyAt(i), _ppszIgnoreNodes ) )
					{
						XMLNamePart xmlName = { m_mapNodes.GetKeyAt(i), (int)m_mapNodes.GetKeyAt(i).Length() };
						TNodePtr pDestNode = pNode->AddNodePtr_Int( &xmlName );
						ATLASSERT( pDestNode );

						// Use change name for extrent_id
						nNodes += m_mapNodes[ i ]->CopyTo( pDestNode, true, _nOverrideAttributes, _ppszIgnoreNodes );//, ppszHiddenAttrInfoStr );
						nNodes++;
					}
					
				}
			}

			return nNodes;
		}

		int AddTo( T* pNode, int _nOverrideAttributes, bool _bAddValue = false, int _nIndex = -1 )
		{
			if( !pNode )
				return -1;

			ATLASSERT( this != pNode );
			if( this == pNode ) return -1;

			CopyAttributes( pNode, _nOverrideAttributes );

			int nNodes = 0;

			CAutoRWL rw(m_rwNodes, false);

			for( int i = 0; i < m_mapNodes.GetSize(); i++ )
			{
				TNodePtr pChild = m_mapNodes[i];
				if( !pChild ) continue;

				if( pChild->IsValue() )
				{
					pNode->AddValue( pChild->Value() );
				}
				else
				{
					XMLNamePart xmlName = { m_mapNodes.GetKeyAt(i), (int)m_mapNodes.GetKeyAt(i).Length() };
					TNodePtr pDestNode = pNode->AddNodePtr_Int( &xmlName, NULL, _nIndex < 0 ? _nIndex : _nIndex++ );

					ATLASSERT( pDestNode );
					nNodes += pChild->CopyTo( pDestNode );
					nNodes++;
				}
			}

			return nNodes;
		}

		void CopyIgnoreTags( T* pCopyTo, const LPCWSTR* ppszTags, const LPCWSTR* ppszIgnoreAttr, bool _bMergeText )
		{
			ATLASSERT( pCopyTo );
			if( !pCopyTo ) return;

			CopyAttributes( pCopyTo, 1, NULL, ppszIgnoreAttr );

			CAutoRWL rw(m_rwNodes, false);

			for( int i = 0; i < m_mapNodes.GetSize(); i++ )
			{
				TNodePtr pNode = m_mapNodes[i];
				if( !pNode ) continue;

				if( pNode->IsValue() )
				{
					pCopyTo->AddValue( pNode->Value() );
				}
				else
				{
					if( InStringList( pNode->GetName(), ppszTags ) )
					{
						pNode->CopyIgnoreTags( pCopyTo, ppszTags, ppszIgnoreAttr, _bMergeText );
					}
					else
					{
						TNodePtr pCopyToNode = pCopyTo->AddNodePtr( pNode->GetName() );
						pNode->CopyIgnoreTags( pCopyToNode, ppszTags, ppszIgnoreAttr, _bMergeText );
					}
				}
			}

			// We can create the value tags even if nodes not empty
		}


		//////////////////////////////////////////////////////////////////////////
		// Attributes: get, set, remove

		inline int GetAttributesCount( MStringList* pList = NULL) const 
		{
			CAutoRWL rw( m_pRWA, false );

			if( pList )
			{
				for( int i = 0; i < m_mapAttributes.GetSize(); i++ )
				{
					pList->AddString( m_mapAttributes.GetKeyAt(i) );
				}
			}

			return m_mapAttributes.GetSize();
		}

		inline LPCWSTR GetAttributeName( int _nIndex, BSTR* _pbsName = NULL ) const 
		{
			CAutoRWL rw( m_pRWA, false );

			if( _nIndex < 0 || _nIndex >= m_mapAttributes.GetSize() )
				return NULL;

			if( _pbsName )
				m_mapAttributes.GetKeyAt(_nIndex).CopyTo( _pbsName );

			return m_mapAttributes.GetKeyAt(_nIndex);
		}

		inline LPCWSTR GetAttributeFullName( int _nIndex, BSTR* _pbsName, T* pRoot = NULL, IMNode* pMNodeRoot = NULL) const 
		{
			ATLASSERT( _pbsName );
			if( !_pbsName ) return NULL;

			CAutoRWL rw( m_pRWA, false );

			if( _nIndex < 0 || _nIndex >= m_mapAttributes.GetSize() )
				return NULL;

			CComBSTR cbsName;
			m_mapAttributes.GetKeyAt(_nIndex).CopyTo( &cbsName );
			
			CComBSTR cbsFullName;
			GetFullName( &cbsFullName, pRoot, pMNodeRoot );
			if( cbsFullName.Length() )
			{
				cbsFullName.Append( L"::" );
				cbsFullName.Append( cbsName );
			}
			else
			{
				cbsFullName = cbsName;
			}

			*_pbsName = cbsFullName.Detach();

			return m_mapAttributes.GetKeyAt(_nIndex);
		}

		inline LPCWSTR GetAttributeValue( int _nIndex, BSTR* _pbsValue = NULL ) const 
		{
			CAutoRWL rw( m_pRWA, false );

			if( _nIndex < 0 || _nIndex >= m_mapAttributes.GetSize() )
				return NULL;

			return m_mapAttributes.GetValueAt(_nIndex)->GetString( _pbsValue );
		}
		inline XMLAttribute::TPtr GetAttributeAt( int _nIndex) 
		{
			CAutoRWL rw( m_pRWA, false );

			if( _nIndex < 0 || _nIndex >= m_mapAttributes.GetSize() )
				return (XMLAttribute::TPtr)NULL;

			return m_mapAttributes.GetValueAt(_nIndex);
		}
		inline bool SetAttributeValue( int _nIndex, BSTR _bsValue )  
		{
			ATLASSERT( _bsValue );
			if( !_bsValue ) return false;

			CAutoRWL rw( m_pRWA, true );

			if( _nIndex < 0 || _nIndex >= m_mapAttributes.GetSize() )
				return false;
			
			m_mapAttributes[_nIndex]->SetString( _bsValue );

			return true;
		}

		inline bool RemoveAttributeAt( int _nIndex )  
		{
			CAutoRWL rw( m_pRWA, true );

			if( _nIndex < 0 || _nIndex >= m_mapAttributes.GetSize() )
				return false;

			m_mapAttributes.RemoveAt( _nIndex );

			return true;
		}


		
		inline int IncrementAttribute( LPCWSTR _wszName, const int& _crAddValue, bool _bCreateNew = true, int _nModule = 0 )
		{
			ATLASSERT( !m_bValue && _wszName && _wszName[0]  );

			CAutoRWL rw( m_pRWA, true );

			bool bFind = false;
			int nValue = GetIntAttribute( _wszName, 0, &bFind );
			if( !bFind && !_bCreateNew)
				return -1;

			nValue += _crAddValue;
			if( _nModule > 0 )
				nValue %= _nModule;

			SetAttribute( _wszName, nValue);
			return nValue;
		}

		inline LONGLONG IncrementAttribute( LPCWSTR _wszName, const LONGLONG& _crAddValue, bool _bCreateNew = true, LONGLONG _llModule = 0 )
		{
			ATLASSERT( !m_bValue && _wszName && _wszName[0]  );

			CAutoRWL rw( m_pRWA, true );

			bool bFind = false;
			LONGLONG llValue = GetInt64Attribute( _wszName, 0, &bFind );
			if( !bFind && !_bCreateNew)
				return -1;

			llValue += _crAddValue;
			if( _llModule > 0 )
				llValue %= _llModule;

			SetAttribute( _wszName, llValue);
			return llValue;
		}

		inline double IncrementAttribute( LPCWSTR _wszName, const double& _crAddValue, bool _bCreateNew = true )
		{
			ATLASSERT( !m_bValue && _wszName && _wszName[0]  );

			CAutoRWL rw( m_pRWA, true );

			bool bFind = false;
			double dblValue = GetDblAttribute( _wszName, 0, &bFind );
			if( !bFind && !_bCreateNew)
				return -1;

			SetAttribute( _wszName, _crAddValue + dblValue);
			return _crAddValue + dblValue;
		}

		template <class T>
		inline bool SetAttribute( LPCWSTR _wszName, const T& _crValue, bool _bCreateMissingNodes = false, bool _bOnlyNewValues = false, int _nInsertAt = -1 )
		{
			ATLASSERT( !m_bValue && _wszName && _wszName[0]  );

			XMLNamePart xmlName = {};
			TNodePtr pNode = FindNodePtr_Int( _wszName, -1, &xmlName, _bCreateMissingNodes, -1 );
			if( !pNode )
				return false;

			return pNode->AddAttribute_Int( &xmlName, _crValue, _bOnlyNewValues ? false : true, _nInsertAt );
		}

		inline bool SetAttribute(LPCWSTR _wszName, LPSTR _crValue, bool _bCreateMissingNodes = false, bool _bOnlyNewValues = false, int _nInsertAt = -1)
		{
			ATLASSERT(!m_bValue && _wszName && _wszName[0]);

			XMLNamePart xmlName = {};
			TNodePtr pNode = FindNodePtr_Int(_wszName, -1, &xmlName, _bCreateMissingNodes, -1);
			if (!pNode)
				return false;

			USES_CONVERSION;

			return pNode->AddAttribute_Int(&xmlName, A2W_CP( _crValue, CP_UTF8), _bOnlyNewValues ? false : true, _nInsertAt);
		}

		
		inline XMLAttribute::TPtr GetAttribute( LPCWSTR _wszName, T** ppLastNode = NULL, T** ppSameNameNode = NULL )
		{
			if( !_wszName || !_wszName[0] )
				return XMLAttribute::TPtr(NULL);
			
			XMLNamePart xmlName = {};
			TNodePtr pNode = FindNodePtr_Int( _wszName, -1, &xmlName, false, -1 );
			if( !pNode ) 
				return XMLAttribute::TPtr(NULL);

			if( ppLastNode ) 
				pNode.CopyTo( ppLastNode );

			if( ppSameNameNode )
			{
				// Try find node
				*ppSameNameNode = pNode->GetNodePtr_Int( &xmlName ).p;
			}
			
			return pNode->GetAttributePtr_Int( &xmlName, false );
		}

		inline XMLAttribute::TPtr GetAttribute_R( LPCWSTR _wszName, T** ppFoundNode = NULL )
		{
			if( !_wszName || !_wszName[0] )
				return XMLAttribute::TPtr(NULL);

			vector<XMLNamePart> vecNames;
			XMLUtils::XMLParseName( _wszName, vecNames );
			if( vecNames.size() != 1 ) return XMLAttribute::TPtr(NULL);

			return GetAttributePtr_Int_R( &vecNames[0], ppFoundNode );
		}

#define DECLATE_GET_ATTRIBUTE( cpp_type, xml_type) \
		inline cpp_type Get##xml_type##Attribute( LPCWSTR _wszName, cpp_type _cppDef = 0, bool* _pbFind = NULL ) \
		{ \
			TNodePtr pAttrNode; \
			XMLAttribute::TPtr pAttribute = GetAttribute( _wszName, &pAttrNode ); \
			if( !pAttribute ) \
			{ \
				if( _pbFind ) *_pbFind = false; \
				return _cppDef; \
			} \
			if( _pbFind ) *_pbFind = true; \
			CAutoRWL rw( pAttrNode->m_pRWA, false ); \
			\
			return pAttribute->Get##xml_type(); \
		} 
#define DECLATE_GET_ATTRIBUTE_R( cpp_type, xml_type) \
	inline cpp_type Get##xml_type##AttributeR( LPCWSTR _wszName, cpp_type _cppDef = 0, bool* _pbFind = NULL ) \
		{ \
		TNodePtr pAttrNode; \
		XMLAttribute::TPtr pAttribute = GetAttribute_R( _wszName, &pAttrNode ); \
		if( !pAttribute ) \
		{ \
		if( _pbFind ) *_pbFind = false; \
		return _cppDef; \
		} \
		if( _pbFind ) *_pbFind = true; \
		CAutoRWL rw( pAttrNode->m_pRWA, false ); \
		\
		return pAttribute->Get##xml_type(); \
		} 

#define DECLATE_LOAD_ATTRIBUTE( cpp_type ) \
		inline bool LoadAttribute( LPCWSTR _wszName, cpp_type& _cppRes) \
		{ \
			TNodePtr pAttrNode; \
			XMLAttribute::TPtr pAttribute = GetAttribute( _wszName, &pAttrNode ); \
			if( !pAttribute ) \
				return false; \
			CAutoRWL rw( pAttrNode->m_pRWA, false ); \
		\
			pAttribute->Get( _cppRes ); \
			return true;	\
		}

#define DECLATE_GET_LOAD_ATTRIBUTE( cpp_type, xml_type) \
	DECLATE_GET_ATTRIBUTE( cpp_type, xml_type) \
	DECLATE_LOAD_ATTRIBUTE( cpp_type ) 

#define DECLATE_GET_ATTRIBUTE_ND( cpp_type, xml_type) \
		inline cpp_type Get##xml_type##Attribute( LPCWSTR _wszName, bool* _pbFind = NULL ) \
		{ \
			cpp_type _cppDef = {}; \
			TNodePtr pAttrNode; \
			XMLAttribute::TPtr pAttribute = GetAttribute( _wszName, &pAttrNode ); \
			if( !pAttribute ) \
			{ \
				if( _pbFind ) *_pbFind = false; \
				return _cppDef; \
			} \
			if( _pbFind ) *_pbFind = true; \
			CAutoRWL rw( pAttrNode->m_pRWA, false ); \
			\
			return pAttribute->Get##xml_type(); \
		} 
		
		DECLATE_LOAD_ATTRIBUTE( float );
		DECLATE_GET_ATTRIBUTE( LPCWSTR, String );
		DECLATE_GET_LOAD_ATTRIBUTE( bool, Bool);
		DECLATE_GET_LOAD_ATTRIBUTE( int, Int);
		DECLATE_LOAD_ATTRIBUTE( LONG );
		DECLATE_GET_LOAD_ATTRIBUTE( ULONG, UInt);
		DECLATE_LOAD_ATTRIBUTE( UINT );
		DECLATE_GET_LOAD_ATTRIBUTE( double, Dbl);
		DECLATE_GET_LOAD_ATTRIBUTE( LONGLONG, Int64);
		DECLATE_GET_LOAD_ATTRIBUTE( ULONGLONG, UInt64);
		DECLATE_GET_ATTRIBUTE_ND( XMLUnit, Unit);
		DECLATE_GET_ATTRIBUTE_ND( GUID, GUID);
		DECLATE_GET_ATTRIBUTE_ND( SYSTEMTIME, DateTime );
		DECLATE_GET_ATTRIBUTE( COLORREF, Color );
		DECLATE_GET_ATTRIBUTE( FOURCC, Fcc );

		DECLATE_GET_ATTRIBUTE_R( LPCWSTR, String );
		DECLATE_GET_ATTRIBUTE_R( COLORREF, Color );
		DECLATE_GET_ATTRIBUTE_R( FOURCC, Fcc );
		DECLATE_GET_ATTRIBUTE_R( bool, Bool);
		DECLATE_GET_ATTRIBUTE_R( double, Dbl );
		DECLATE_GET_ATTRIBUTE_R( int, Int );
		DECLATE_GET_ATTRIBUTE_R( UINT, UInt );
		DECLATE_GET_ATTRIBUTE_R( LONG, Int );
		DECLATE_GET_ATTRIBUTE_R( LONGLONG, Int64 );
		DECLATE_GET_ATTRIBUTE_R( ULONGLONG, UInt64 );
		
#undef DECLATE_GET_ATTRIBUTE_ND
#undef DECLATE_GET_ATTRIBUTE_R
#undef DECLATE_GET_LOAD_ATTRIBUTE

#define DECLATE_SET_ATTRIBUTE( cpp_type, xml_type) \
		bool Set##xml_type##Attribute( LPCWSTR _wszName, const cpp_type& _crValue, bool _bCreateMissingNodes = false, bool _bOnlyNewValues = false, int _nInsertAt = -1 ) \
		{ \
			ATLASSERT( !m_bValue && _wszName && _wszName[0] ); \
		\
			XMLNamePart xmlName = {}; \
			TNodePtr pNode = FindNodePtr_Int( _wszName, -1, &xmlName, _bCreateMissingNodes, -1 ); \
			if( !pNode ) \
				return false; \
		\
			CAutoRWL rw( pNode->m_pRWA, true ); \
			XMLAttribute::TPtr pAttr = pNode->GetAttributePtr_Int( &xmlName, _bOnlyNewValues ? false : true, _nInsertAt ); \
			if( !pAttr ) \
				return false; \
		\
			pAttr->Set##xml_type( _crValue ); \
			return true; \
		}
		DECLATE_SET_ATTRIBUTE( COLORREF, Color );
		DECLATE_SET_ATTRIBUTE( FOURCC, Fcc );
		DECLATE_SET_ATTRIBUTE( bool, Bool );
#undef DECLATE_SET_ATTRIBUTE

		bool IsSameAttribute(LPCWSTR _wszName, LPCWSTR _pszCompare, bool _bCaseSensetive = false, bool _bTrueIfNotSpecified = false)
		{
			TNodePtr pAttrNode;
			XMLAttribute::TPtr pAttribute = GetAttribute(_wszName, &pAttrNode);
			if (!pAttribute)
				return _pszCompare == NULL ? true : _bTrueIfNotSpecified;

			CAutoRWL rw(pAttrNode->m_pRWA, false);

			LPCWSTR pszValue = pAttribute->GetString();
			return _bCaseSensetive ? STR_IS_SAME(pszValue, _pszCompare) : STR_IS_SAME_I(pszValue, _pszCompare);
		}

		CComBSTR GetStringAttribute_Safe( LPCWSTR _wszName, LPCWSTR _wszDef = NULL, bool* _pbFind = NULL )
		{
			TNodePtr pAttrNode;
			XMLAttribute::TPtr pAttribute = GetAttribute( _wszName, &pAttrNode ); 
			if( !pAttribute ) 
			{ 
				if( _pbFind ) *_pbFind = false; 
				return _wszDef; 
			} 

			CAutoRWL rw( pAttrNode->m_pRWA, false ); 
			
			if( _pbFind ) *_pbFind = true; 
			
			return pAttribute->GetString(); 
		}	 

		// Some special type handling
		bool SetTimerAttribute(LPCWSTR _wszName, REFERENCE_TIME _rtNow = 0, bool _bCreateMissingNodes = false, bool _bOnlyNewValues = false, int _nInsertAt = -1)
		{
			XMLNamePart xmlName = {};
			TNodePtr pNode = FindNodePtr_Int(_wszName, -1, &xmlName, _bCreateMissingNodes, -1);
			if (!pNode)
				return false;

			CAutoRWL rw(pNode->m_pRWA, true);

			XMLAttribute::TPtr pAttr = pNode->GetAttributePtr_Int(&xmlName, _bOnlyNewValues ? false : true, _nInsertAt);
			if (!pAttr)
				return false;

			pAttr->SetTimer(_rtNow);
			return true;
		}

		bool SetUnitAttribute( LPCWSTR _wszName, const double& _crValue, double _dblBase, bool _bCreateMissingNodes = false, bool _bOnlyNewValues = false, int _nInsertAt = -1 )
		{
			XMLNamePart xmlName = {}; 
			TNodePtr pNode = FindNodePtr_Int( _wszName, -1, &xmlName, _bCreateMissingNodes, -1 ); 
			if( !pNode ) 
				return false; 

			CAutoRWL rw( pNode->m_pRWA, true);

			XMLAttribute::TPtr pAttr = pNode->GetAttributePtr_Int( &xmlName, _bOnlyNewValues ? false : true, _nInsertAt ); 
			if( !pAttr ) 
				return false; 

			pAttr->Set( XMLUtils::MakePercent( _crValue, _dblBase ) );
			return true; 
		}

		double GetUnitAttribute( LPCWSTR _wszName, double _dblBase, double _dblDef = 0, bool* _pbFind = NULL )
		{
			TNodePtr pAttrNode;
			XMLAttribute::TPtr pAttribute = GetAttribute( _wszName, &pAttrNode ); 
			if( !pAttribute )
			{
				if( _pbFind ) *_pbFind = false;
				return _dblDef;
			}

			if( _pbFind ) *_pbFind = true;

			CAutoRWL rw( pAttrNode->m_pRWA, false ); 

			return XMLUtils::Unit2Value( pAttribute->GetUnit(), _dblBase );
		}

		int GetBoolAttributeAsInt( LPCWSTR _wszName, int _nDefValue = -1, bool* _pbFind = NULL )
		{
			bool bFind = false;
			CComBSTR cbsRes = GetStringAttribute_Safe( _wszName, NULL, &bFind );
			if( _pbFind )
				*_pbFind = bFind;

			if( !bFind )
				return _nDefValue;

			return Str2BoolInt( cbsRes );
		}

		LONGLONG GetBytesAttribute( LPCWSTR _wszName, LPCWSTR _pszDefValue = NULL, bool* _pbFind = NULL )
		{
			CComBSTR cbsRes = GetStringAttribute_Safe( _wszName, _pszDefValue, _pbFind );

			return Str2Bytes( cbsRes );
		}

		LONGLONG GetBytesAttributeLL(LPCWSTR _wszName, LONGLONG _llDefValue = NULL, bool* _pbFind = NULL)
		{
			CComBSTR cbsRes = GetStringAttribute_Safe(_wszName, NULL, _pbFind);

			return STR_IS_EMPTY(cbsRes.m_str) ? _llDefValue : Str2Bytes(cbsRes);
		}

		eMFCC GetFCCAttribute( LPCWSTR _wszName, eMFCC _eFCCDefault = eMFCC_Default, bool* _pbFind = NULL )
		{
			CComBSTR cbsRes = GetStringAttribute_Safe( _wszName, NULL, _pbFind );
			eMFCC eRes = (eMFCC)Str2Fcc( cbsRes );
			if( eRes == eMFCC_Default && !STR_IS_SAME_I(cbsRes, L"default") )
				return _eFCCDefault;

			return eRes;
		}

		template <class TEnum>
		bool SetAttribute( LPCWSTR _wszName, const LPCWSTR _checkArray[], const TEnum& _crValue, bool _bCreateMissingNodes = false, bool _bOnlyNewValues = false, int _nInsertAt = -1 )
		{
			ATLASSERT( !m_bValue && _wszName && _wszName[0]  );

			XMLNamePart xmlName = {}; 
			TNodePtr pNode = FindNodePtr_Int( _wszName, -1, &xmlName, _bCreateMissingNodes, -1 );
			if( !pNode )
				return false;

			LPCWSTR pszValue = XMLUtils::Enum2Str( _checkArray, (int)_crValue );
			ATLASSERT( pszValue );
			if( !pszValue ) return false;

			return pNode->AddAttribute_Int( &xmlName, pszValue, _bOnlyNewValues ? false : true, _nInsertAt );
		}


		template <class TEnum>
		TEnum GetEnumAttribute( LPCWSTR _wszName, const LPCWSTR _checkArray[], const TEnum& def, bool* _pbFind = NULL )
		{
			TNodePtr pAttrNode;
			XMLAttribute::TPtr pAttribute = GetAttribute( _wszName, &pAttrNode ); 
			if( !pAttribute )
				return def;

			CAutoRWL rw(pAttrNode->m_pRWA, false);

			TEnum ret = def;
			XMLUtils::StrGetEnum( _checkArray, pAttribute->GetString(), ret );
			return ret;
		}

		template <class TEnum>
		TEnum GetEnumAttributeR( LPCWSTR _wszName, const LPCWSTR _checkArray[], const TEnum& def, bool* _pbFind = NULL )
		{
			CComBSTR cbsValue = GetStringAttributeR( _wszName, NULL, _pbFind  );
			if( !cbsValue )
				return def;
			TEnum ret = def;
			XMLUtils::StrGetEnum( _checkArray, cbsValue, ret );
			return ret;
		}

		int GetEnumAttribute( LPCWSTR _wszName, const LPCWSTR _checkArray[], bool* _pbFind = NULL )
		{
			TNodePtr pAttrNode;
			XMLAttribute::TPtr pAttribute = GetAttribute( _wszName, &pAttrNode ); 
			if( !pAttribute )
			{
				if( _pbFind )
					*_pbFind = false;

				return -1;
			}

			if( _pbFind )
				*_pbFind = true;

			CAutoRWL rw(pAttrNode->m_pRWA,false);

			return XMLUtils::Str2Enum( _checkArray, pAttribute->GetString() );
		}

		int GetEnumAttributeR( LPCWSTR _wszName, const LPCWSTR _checkArray[], bool* _pbFind = NULL )
		{
			CComBSTR cbsValue = GetStringAttributeR( _wszName, NULL, _pbFind );
			return XMLUtils::Str2Enum( _checkArray, cbsValue );
		}

		template <class TEnum>
		bool LoadAttribute( LPCWSTR _wszName, const LPCWSTR _checkArray[], TEnum& res )
		{
			TNodePtr pAttrNode;
			XMLAttribute::TPtr pAttribute = GetAttribute( _wszName, &pAttrNode ); 
			if( !pAttribute )
				return false;

			CAutoRWL rw(pAttrNode->m_pRWA,false);

			return XMLUtils::StrGetEnum( _checkArray, pAttribute->GetString(), res );
		}

		bool RemoveAttribute( LPCWSTR _wszName )
		{
			XMLNamePart xmlName = {}; 
			TNodePtr pNode = FindNodePtr_Int( _wszName, -1, &xmlName, false, -1 );
			if( !pNode )
				return false;

			return pNode->RemoveAttribute_Int( &xmlName );
		}

		bool HaveAttribute( LPCWSTR _wszName, bool _bIgnoreEmptyStrings = false)
		{
			XMLNamePart xmlName = {}; 
			TNodePtr pNode = FindNodePtr_Int( _wszName, -1, &xmlName, false, -1 );
			if( !pNode )
				return false;

			XMLAttribute::TPtr pAttribute = pNode->GetAttributePtr_Int(&xmlName, false);
			if (!pAttribute)
				return false;

			if (_bIgnoreEmptyStrings )
			{
				CComBSTR cbsStrValue;
				pAttribute->GetString(&cbsStrValue);

				return !STR_IS_EMPTY(cbsStrValue);
			}
				
			return true;
		}

		bool HaveInfo()
		{
			LPCWSTR pszInfoNode = (m_bAttributesInfo && m_ppszHiddenNodes) ? m_ppszHiddenNodes[0] : L"attributes_info";
			TNodePtr pInfo = GetNodePtr( pszInfoNode );
			if( pInfo )
				return true;

			return false;
		}

		// Note: If _ppszOptionsHelp used -> the help values used till NULL -> { L"Help1", NULL, L"Help2", NULL } is not good, 
		//		 use { L"Help1", L"", L"Help2", NULL }, but the help list may contain less strings
		TNodePtr InfoSetOptionsList( LPCWSTR _pszValue, 
			const LPCWSTR* _ppszOptionsValues,
			const LPCWSTR* _ppszOptionsHelp = NULL,
			bool _bFixedOption = false,
			CSimpleArray2<TNodePtr>* _pArrValues = NULL )
		{
			if( !_pszValue || !_pszValue[0] )
				return (TNodePtr)NULL;

			TNodePtr pXMLValue = GetAttributeInfo( _pszValue, NULL, true );
			pXMLValue->Clear();
			if( _ppszOptionsHelp || _pArrValues )
			{
				ATLASSERT( pXMLValue->GetStringAttribute( L"values" ) == NULL );

				// Use complex type e.g.
				// 	<line-in type='option_fixed' default='sdi'>
				// 		<sdi>SDI Video & SDI Audio</sdi>
				// 		<hdmi>SDI Video & SDI Audio</hdmi>
				// 	</line-in>
				const LPCWSTR* ppszValue = _ppszOptionsValues;
				const LPCWSTR* ppszHelp = _ppszOptionsHelp;
				while( *ppszValue != NULL )
				{
					TNodePtr pNode = pXMLValue->AddNodePtr( *ppszValue, *ppszHelp && (*ppszHelp)[0] ? *ppszHelp : NULL );
					if( _pArrValues )
						_pArrValues->Add( pNode );

					ppszValue++;

					if( *ppszHelp )
						ppszHelp++;
				}

				pXMLValue->RemoveAttribute( L"values" );
			}
			else
			{
				// Basic type e.g. values = 'option1|option2|option3' etc.
				// <b type='option_fixed' default='90M' values='185M, 175M, 145M, 120M, 115M, 90M, 45M, 36M'>Set bitrate (in bits/s). Possible values: 185M, 175M, 145M, 120M, 115M, 45M, 36M.</b>
				CComBSTR cbsValues = pXMLValue->GetStringAttribute_Safe( L"values" );
				const LPCWSTR* ppszValue = _ppszOptionsValues;
				while( *ppszValue )
				{
					if( cbsValues.m_str )
						cbsValues.Append( L"|" );

					cbsValues.Append( *ppszValue );
					
					ppszValue++;
				}

				pXMLValue->SetAttribute( L"values", cbsValues );
			}

			pXMLValue->SetAttribute( L"type", _bFixedOption ? L"option_fixed" : L"option" );
			

			return pXMLValue;
		}

		TNodePtr InfoSet( LPCWSTR _pszValue, LPCWSTR pszType, LPCWSTR _pszDescription = NULL)
		{
			if( !_pszValue || !_pszValue[0] )
				return (TNodePtr)NULL;

			TNodePtr pXMLValue = GetAttributeInfo( _pszValue, NULL, true );
			if( pXMLValue )
			{
				pXMLValue->Clear();
				pXMLValue->SetAttribute( L"type", pszType );
				if( !STR_IS_EMPTY(_pszDescription) )
					pXMLValue->SetValue( _pszDescription );
			}

			return pXMLValue;
		}

		TNodePtr InfoReset( LPCWSTR _pszValue )
		{
			if( !_pszValue || !_pszValue[0] )
				return (TNodePtr)NULL;

			TNodePtr pXMLValue = GetAttributeInfo( _pszValue, NULL, true );
			if( pXMLValue )
				pXMLValue->SetParent( NULL, true );

			return pXMLValue;
		}

		TNodePtr GetInfo( int* _pnCount = NULL, int* _pnIndex = NULL, CStrHashMap<bool>* _pAttributesList = NULL, bool _bUpdateInfo = false )
		{
			if( _pnCount )
				*_pnCount = 0;

			LPCWSTR pszInfoNode = (m_bAttributesInfo && m_ppszHiddenNodes) ? m_ppszHiddenNodes[0] : L"attributes_info";
			TNodePtr pInfo = GetNodePtr( pszInfoNode );
			if( pInfo && _bUpdateInfo )
			{
				CAutoRWL rwA( m_pRWA, false );

				for( int i = 0; i < m_mapAttributes.GetSize(); i++ )
				{
					if( pInfo->GetNodePtr( m_mapAttributes.GetKeyAt(i), false ) )
						continue;

					if( pInfo->GetBoolAttribute(L"inherit", true ) )
					{
						// Check the parent
						TNodePtr pBaseInfo;
						TNodePtr pParent = GetParentNode();
						if( pParent && (pBaseInfo = pParent->GetNodePtr( pszInfoNode ) ) )
						{
							if( pBaseInfo->GetNodePtr( m_mapAttributes.GetKeyAt(i), false ) )
								continue;
						}

						// TODO: Check the recursive paranet
					}

					pInfo->AddNodePtr( m_mapAttributes.GetKeyAt(i) );
				}
			}


			if( pInfo )
			{
				if( _pnCount )
					*_pnCount += pInfo->GetNodesCount();

				if( _pAttributesList )
				{
					CAutoRWL rw( pInfo->m_rwNodes, false );

					_pAttributesList->AddFromKey( pInfo->m_mapNodes, true );
				}
			}

			if( !pInfo || pInfo->GetBoolAttribute(L"inherit", true ) )
			{
				TNodePtr pBaseInfo;
				TNodePtr pParent = GetParentNode();
				if( pParent && (pBaseInfo = pParent->GetNodePtr( pszInfoNode ) ) )
				{
					if( _pnCount )
						*_pnCount += pBaseInfo->GetNodesCount();

					if( _pAttributesList )
					{
						CAutoRWL rw( pBaseInfo->m_rwNodes, false );

						_pAttributesList->AddFromKey( pBaseInfo->m_mapNodes, true );
					}

					// We enumerate attributes - first parent(inherit), after child(current)
					if( !pInfo || (_pnIndex && *_pnIndex < pBaseInfo->GetNodesCount() ) )
					{
						// Use parent(inherit) 
						pInfo = pBaseInfo;
					}
					else
					{
						// Use child(current) but correct index (as base is first)
						if( _pnIndex )
							*_pnIndex -= pBaseInfo->GetNodesCount();
					}
				}
			}

			// Check root node 
			if( !pInfo )
			{
				TNodePtr pCommonInfo = GetObjectR<TNodePtr>( L"attributes_info" );
				if( pCommonInfo )
				{
					deque<TNodePtr> deqInherit; // For reverse arguments

					CComBSTR cbsName;
					GetName(&cbsName, true);
					pInfo = pCommonInfo->GetNodePtr( cbsName.m_str );
					if( !pInfo && cbsName.m_str )
					{
						// Check for e.g. ticker.img
						LPWSTR pszPostifix = StrChrW( cbsName.m_str, L'.' );
						if( pszPostifix )
						{
							*pszPostifix = 0;
							pInfo = pCommonInfo->GetNodePtr( cbsName.m_str );
						}
					}

					while( pInfo )
					{
						deqInherit.push_front( pInfo );
 
						if( _pnCount )
							*_pnCount += pInfo->GetRealNodesCount();

						if( _pAttributesList )
						{
							CAutoRWL rw( pInfo->m_rwNodes, false );

							_pAttributesList->AddFromKey( pInfo->m_mapNodes, true );
						}

						// Check inherited types
						CComBSTR cbsInherit = pInfo->GetStringAttribute_Safe( L"inherit" );
						if( !STR_IS_EMPTY(cbsInherit) )
							pInfo = pCommonInfo->GetNodePtr( cbsInherit );
						else
							break;
					}

					// Correct index (for reverse order) base, inherit(base), inherit(inherit(base)) ...
					if( _pnIndex )
					{
						while( !deqInherit.empty() )
						{
							if( deqInherit.front()->GetRealNodesCount() > *_pnIndex )
							{
								pInfo = deqInherit.front();
								break;
							}

							*_pnIndex -= deqInherit.front()->GetRealNodesCount();
							deqInherit.pop_front();
						}
					}
				}
			}

			if( _pAttributesList && pInfo )
			{
				CAutoRWL rw( m_pRWA, false );

				_pAttributesList->AddFromKey( m_mapAttributes, false );
			}

			return pInfo;
		}

		// e.g. <attributes_info><line-in><sdi...></sdi><hdmi..>..</hdmi>...
		// -> _bsName == 'line-in' -> return <line_in> node, _ppInfoNode  point to <attributes_info>
		TNodePtr GetAttributeInfo( LPCWSTR _bsAttrName, T** _ppInfoNode, bool _bCreateNew = false )
		{
			LPCWSTR pszInfoNode = (m_bAttributesInfo && m_ppszHiddenNodes) ? m_ppszHiddenNodes[0] : L"attributes_info";

			// Check for node attributes e.g. object::external_audio
			XMLNamePart xmlName = {};
			TNodePtr pNode = FindNodePtr_Int( _bsAttrName, -1, &xmlName, false, -1 );
			if( !pNode ) 
				return (TNodePtr)NULL;

			TNodePtr pDescAttr;
			TNodePtr pInfo = pNode->GetNodePtr( pszInfoNode, _bCreateNew );
			if( pInfo )
				pDescAttr = pInfo->GetNodePtr( xmlName.pszPart, _bCreateNew );

			USES_SF_MACROS;

			if( !pDescAttr )
			{
				// Try parent
				TNodePtr pParent = pNode->GetParentNode();
				if( pParent && 
					pParent->GetBoolAttribute(STR_APPEND_SF( pszInfoNode, L"::inherit" ), true) &&
					(!pInfo || pInfo->GetBoolAttribute(L"inherit", true ) ) )
				{
					if( pInfo = pParent->GetNodePtr( pszInfoNode ) )
						pDescAttr = pInfo->GetNodePtr( xmlName.pszPart );
				}
			}


			// Try root
			if( !pDescAttr )
			{
				TNodePtr pCommonInfo = pNode->GetObjectR<TNodePtr>( L"attributes_info" );
				if( pCommonInfo )
				{
					CComBSTR cbsName;
					pNode->GetName(&cbsName, true);

					pInfo = pCommonInfo->GetNodePtr( cbsName.m_str );
					if( !pInfo && cbsName.m_str )
					{
						// Check for e.g. ticker.img
						LPWSTR pszPostifix = StrChrW( cbsName.m_str, L'.' );
						if( pszPostifix )
						{
							*pszPostifix = 0;
							pInfo = pCommonInfo->GetNodePtr( cbsName.m_str );
						}
					}

					// Check nested (most priority - top)
					while( pInfo )
					{
						pDescAttr = pInfo->GetNodePtr( xmlName.pszPart );
						if( pDescAttr )
							break;

						// Check inherited types
						CComBSTR cbsInherit = pInfo->GetStringAttribute_Safe( L"inherit" );
						if( !STR_IS_EMPTY(cbsInherit) )
							pInfo = pCommonInfo->GetNodePtr( cbsInherit );
						else
							break;
					}
				}
			}

			if( pInfo && _ppInfoNode )
				pInfo.CopyTo( _ppInfoNode );

			return pDescAttr;
		}

		// e.g. <attributes_info><line-in option='sdi,hdmi'><sdi...></sdi><hdmi..>..</hdmi>...
		// -> _bsName == 'line-in', _pszName ='hdmi' -> return <hdmi..> node
		TNodePtr GetAttributeOptionInfo( /*[in]*/ LPCWSTR _pszName, /*[in]*/ LPCWSTR _pszOption )
		{
			if( STR_IS_EMPTY(_pszName) )
				return (TNodePtr)NULL;

			CComBSTR cbsOption = _pszOption;
			if( STR_IS_EMPTY(_pszOption) )
				cbsOption = GetStringAttribute_Safe( _pszName );
			

			TNodePtr pInfo;		// <attributes_info>
			TNodePtr pAttrNode;  // The desired attribute node e.g. <attributes_info><line-in><sdi...><sdi> -> <sdi>
			TNodePtr pDescAttr = GetAttributeInfo( _pszName, &pInfo ); // return <line-in>
			if( pDescAttr )
			{
				pAttrNode = pDescAttr->GetNodePtr( cbsOption );
				if( !pAttrNode )
				{
					CComBSTR cbsSelFrom = pDescAttr->GetStringAttribute_Safe( L"select_from" );
					if( cbsSelFrom )
					{
						pDescAttr = pInfo->GetNodePtr( cbsSelFrom);
						if( pDescAttr )
							pAttrNode = pDescAttr->GetNodePtr( cbsOption );
					}
				}
			}

			return pAttrNode;
		}

		// _nOverrideType == 0 -> Not override
		// _nOverrideType == 1 -> Override
		// _nOverrideType == 2 -> Override & Remove attributes not presented in source (only with lists)

		// Note: For copy ALL nested attributes use ppszCopyNodes point to empty list
		int CopyAttributes(T* pNode, int _nOverrideType, const LPCWSTR* ppszCopy = NULL, const LPCWSTR* ppszIgnore = NULL, const LPCWSTR* ppszCopyNodes = NULL)
		{
			if (!pNode)
				return -1;

			ATLASSERT(this != pNode);
			if (this == pNode) return -1;

			int nCount = 0;
			{
				CAutoRWL rw(m_pRWA, false);

				if (_nOverrideType == 2)
					pNode->ClearAttributes();

				if (m_pszSwapID && m_pszSwapID[0] && m_cbsExternName &&
					(!ppszCopy || InStringList(m_pszSwapID, ppszCopy)) &&
					(!ppszIgnore || !InStringList(m_pszSwapID, ppszIgnore)))
				{
					pNode->SetAttribute(m_pszSwapID, Name());
				}

				for (int i = 0; i < m_mapAttributes.GetSize(); i++)
				{
					if ((!ppszCopy || InStringList(m_mapAttributes.GetKeyAt(i), ppszCopy)) &&
						(!ppszIgnore || !InStringList(m_mapAttributes.GetKeyAt(i), ppszIgnore)))
					{
						XMLNamePart xmlName = { m_mapAttributes.GetKeyAt(i), (int)m_mapAttributes.GetKeyAt(i).Length() };
						pNode->AddAttribute_Int(&xmlName, m_mapAttributes.GetValueAt(i)->GetString(), _nOverrideType == 1, -1);
						nCount++;
					}
				}
			}

			if (ppszCopyNodes)
			{
				CAutoRWL rw(m_rwNodes, false);

				for (int i = 0; i < m_mapNodes.GetSize(); i++)
				{
					LPCWSTR pszName = m_mapNodes.GetKeyAt(i);
					if (pszName && (!(*ppszCopyNodes) || MStrInList(pszName, ppszCopyNodes)))
					{
						TNodePtr pChildNode = pNode->GetNodePtr(m_mapNodes.GetKeyAt(i), TRUE);

						nCount += m_mapNodes[i]->CopyAttributes(pChildNode, _nOverrideType, ppszCopy, ppszIgnore, ppszCopyNodes);
					}
				}
			}

			return nCount;
		}

		// Note: 
		// If _pszChnagePrefix is NULL - keep prefix else prefix changed to _pszChangePrefix string 
		// e.g. for cut-off prefix use L"" for _pszChangePrefix   
		int CopyAttributesByPrefix(T* pNode, int _nOverrideType, LPCWSTR _pszPrefix, LPCWSTR _pszChangePrefix, const LPCWSTR* ppszIgnore = NULL)
		{
			if (!pNode)
				return -1;

			ATLASSERT(this != pNode);
			if (this == pNode) return -1;

			int nCount = 0;
			{
				CAutoRWL rw(m_pRWA, false);

				if (_nOverrideType == 2)
					pNode->ClearAttributes();

				if (!STR_IS_EMPTY(m_pszSwapID) &&
					m_cbsExternName &&
					(!ppszIgnore || !InStringList(m_pszSwapID, ppszIgnore)))
				{
					pNode->SetAttribute(m_pszSwapID, Name());
				}

				for (int i = 0; i < m_mapAttributes.GetSize(); i++)
				{
					LPCWSTR pszRest = NULL;
					LPCWSTR pszName = m_mapAttributes.GetKeyAt(i);
					if (IsPrefix(pszName, _pszPrefix, &pszRest) && (!ppszIgnore || !InStringList(pszName, ppszIgnore)))
					{
						CComBSTR cbsNewName = _pszChangePrefix ? _pszChangePrefix : pszName;
						if (_pszChangePrefix)
							cbsNewName.Append(pszRest);

						XMLNamePart xmlName = { cbsNewName, (int)cbsNewName.Length() };

						pNode->AddAttribute_Int(&xmlName, m_mapAttributes.GetValueAt(i)->GetString(), _nOverrideType == 1, -1);
						nCount++;
					}
				}
			}

			return nCount;
		}

		template<class TFind>
		TNodePtr FindNodeByPtr( TFind* _pFind )
		{
			CAutoRWL rw(m_rwNodes, false);

			for( int i = 0; i < m_mapNodes.GetSize(); i++ )
			{
				if( m_mapNodes[i].IsEqualObject( _pFind) )
					return m_mapNodes[i];
			}

			return (TNodePtr)(NULL);
		}

		template<class TFind>
		TNodePtr FindNodeByPtr_R( TFind* _pFind )
		{
			CAutoRWL rw(m_rwNodes, false);

			for( int i = 0; i < m_mapNodes.GetSize(); i++ )
			{
				if( m_mapNodes[i].IsEqualObject( _pFind) )
					return m_mapNodes[i];

				TNodePtr pRes = m_mapNodes[i]->FindNodeByPtr_R( _pFind );
				if( pRes )
					return pRes;
			}

			return (TNodePtr)(NULL);
		}

		TNodePtr TreeGetNodeEx( LPCWSTR _pszType, LPCWSTR _pszKey )
		{
			ATLASSERT( _pszType && _pszKey );
			if( !_pszType || !_pszKey ) return (TNodePtr)NULL;

			vector<XMLNamePart> vecNames;
			XMLUtils::XMLParseName( _pszType, vecNames );
			ATLASSERT( vecNames.size() > 0 );

			return TreeGetNodeEx_R( vecNames, _pszKey, 0 );
		}
		/*
<m-device>
	<video name='Decklink HD'>
		<device name='Decklink HD' moniker='{....}'>
			<line-in current='SDI'>
				<line-in name='SDI'/>
				<line-in name='HDMI'/>
				<line-in name='Y,Cb,Cr'/>
			</line-in>

			<format name='1920x1080@50i'>
				<format name='1920x1080@50i'>
					<colorspace>
						<colorspace name='HDYC'/>
					</colorspace>
				</format>
				<format name='720x576@50i' interlace='top'/>
				<format name='720x576@50i'/>
			</format>

			<colorspace>
				<colorspace name='RGB32'/>
				<colorspace name='UYVY'/>
				<colorspace name='HDYC'/>
				<colorspace name='v210'/>
				<colorspace name='r210'/>
			</colorspace>

			<audio>
				<device name='<From Video>' moniker='{....}'/>
			</audio>
		</device>
	</video>
	<audio name='Realtek'>
		<device name='<From Video>' moniker='{....}'/>
		<device name='Realtek'/>
			<format>
			</format>
		</device>
		<device name='Decklink HD' moniker='{....}'>
			<line-in>
				<line-in name='SDI'/>
				<line-in name='HDMI'/>
				<line-in name='XLR Analog'/>
			</line-in>
		</device>
	</audio>
</m-device>
*/
		TNodePtr TreeGetSpecified( LPCWSTR _pszKey )
		{
			CComBSTR cbsCurrent = GetStringAttribute_Safe( _pszKey );
			if( !cbsCurrent )
				return (TNodePtr)NULL;

			// video, audio etc
			TNodePtr pCur = FindNodeByAttribute( _pszKey, cbsCurrent );
			if( !pCur )
			{
				RemoveAttribute( _pszKey );
				return (TNodePtr)NULL;
			}

			return pCur;
		}
		TNodePtr TreeGetNodeEx_R( vector<XMLNamePart>& vecNames, LPCWSTR _pszKey, int _nLevel )
		{
			ATLASSERT( _pszKey );
			if( !_pszKey ) return (TNodePtr)NULL;

			
			// audio, video, etc.
			TNodePtr pType;

			// Check first level SPECIFIED nodes
			if( _nLevel == 0 )
			{
				CAutoRWL rw(m_rwNodes, false);

				for( int i = 0; i < m_mapNodes.GetSize(); i++ )
				{
					TNodePtr pChild = m_mapNodes[i];
					if( !pChild || pChild->IsValue() )
						continue;

					TNodePtr pCur = pChild->TreeGetSpecified( _pszKey );
					if( pCur )
					{
						// audio, video, etc.
						pType = pCur->GetNodePtr_Int( &vecNames[0] );
						if( pType )
							break;
					}
				}
			}

			// audio, video, etc.
			if( !pType )
				pType = GetNodePtr_Int( &vecNames[0] );
			if( !pType )
				return (TNodePtr)NULL;

			if( vecNames.size() == 1 )
				return pType;

			vecNames.erase( vecNames.begin() );
			
			TNodePtr pCur = pType->TreeGetSpecified( _pszKey );
			if( !pCur )
				return (TNodePtr)NULL;

			return pCur->TreeGetNodeEx_R( vecNames, _pszKey, _nLevel + 1 );
		}

		LPCWSTR GetFreeNodeName( CComBSTR& cbsBaseName, bool _bAlwaysIndex )
		{
			int _nIndex = 1;
			WCHAR* pszName = (WCHAR*)alloca( cbsBaseName.Length() * 2 + 32 );

			XMLNamePart xmlName = {pszName, 0};
			xmlName.nLen = wsprintfW( pszName, L"%ws%ws", cbsBaseName, _bAlwaysIndex ? L"_000" : L"" );
			while( GetNodePtr_Int( &xmlName ) != NULL )
			{
				xmlName.nLen = wsprintfW( pszName, L"%ws_%03d", cbsBaseName, _nIndex++ );
			}

			cbsBaseName = pszName;
			return cbsBaseName.m_str;
		}
		
			
	protected:
		inline TNodePtr GetNodePtr_Int( XMLNamePart* _pName )
		{
			CAutoRWL rw(m_rwNodes, false);

			int nFind = m_mapNodes.FindNode( _pName );
			if( nFind >= 0 ) 
				return m_mapNodes[nFind];

			return (TNodePtr)NULL;				
		}

	

		// _pLastName != NULL -> not get last node (for Attributes)

		inline TNodePtr FindNodePtr_Int( LPCWSTR _wszName, int _nIndex, XMLNamePart* _pLastName,  bool _bCreateNew, int _nInsertAt, 
			T** ppLastNode = NULL, LPCWSTR* ppszLastName = NULL )
		{
			if( !_wszName || !_wszName[0] ) return TNodePtr( (T*)this );

			vector<XMLNamePart> vecNames;
			XMLUtils::XMLParseName( _wszName, vecNames );

			int nNames = (int)vecNames.size();
			if( _pLastName )
			{
				*_pLastName = vecNames[ nNames - 1];
				nNames--;
			}

			if( _nIndex > 0 && nNames > 0 )
				vecNames[ nNames - 1].nOrder = _nIndex;

			TNodePtr pNode( (T*)this);
			for( int i = 0; i < nNames; i++ )
			{
				TNodePtr pNextNode;
				if (vecNames[i].HaveId() )
					//Select node from current node, not from root only
					pNextNode = pNode->FindNodeByAttribute(L"id", vecNames[i].Id(), NULL, true, vecNames[i].Name());
				else
					pNextNode = pNode->GetNodePtr_Int(&vecNames[i]);

				if( !pNextNode )
				{
					if( _bCreateNew )
					{
						T::CreateNode_I( &pNextNode, &vecNames[i], pNode); // Set correct parent, not always root - (T*)this

						if (vecNames[i].HaveId())
							pNextNode->SetAttribute(L"id", vecNames[i].Id() );

						CAutoRWL rw(pNode->m_rwNodes, true);

						if( _nInsertAt >= 0 && _nInsertAt < pNode->m_mapNodes.GetSize() )
						{
							pNode->m_mapNodes.InsertAtIndex( MIN( _nInsertAt, pNode->m_mapNodes.GetSize() ), (LPCWSTR)pNextNode->Name(), pNextNode );
						}
						else
						{
							pNode->m_mapNodes.Add( (LPCWSTR)pNextNode->Name(), pNextNode );
						}
					}
					else
					{
						if( i == nNames - 1 )
						{
							if( ppLastNode )
								pNode.CopyTo( ppLastNode );

							if( ppszLastName )
								*ppszLastName = vecNames[i].pszPart;
						}

						return (TNodePtr)NULL;
					}
				}

				pNode = pNextNode;
			}

			return pNode;
		}

		

		inline TNodePtr GetNodeValue_Int( int _nIndex = 0 ) const 
		{
			CAutoRWL rw(m_rwNodes, false);

			int nFind = m_mapNodes.FindKey( NULL, _nIndex );
			if( nFind >= 0 ) 
				return m_mapNodes[nFind];

			return (TNodePtr)NULL;				
		}
		static inline eValueType GetValueType_Int( LPCWSTR _wszValue )
		{
			if( STR_IS_EMPTY(_wszValue) ) return eRemove;

			// Check for '=' (not in quotes)
			if( !XMLUtils::StrChrW_Quotes( _wszValue, L'=' ) )
				return eBasic;

			if( !XMLUtils::StrChrW_Quotes( _wszValue, L'<' ) )
				return eParam;

			return eXML;
		}

		TNodePtr AddNodePtr_Int( XMLNamePart* _pName, LPCWSTR _wszValue = NULL, int _nInsertAt = -1 )
		{
			ATLASSERT( _pName && _pName->pszPart );

			TNodePtr pXMLChildNode;
			CreateNode_I( &pXMLChildNode, _pName, (T*)this );
			if( !STR_IS_EMPTY(_wszValue) )
				pXMLChildNode->SetValue( _wszValue );

			AttachNodePtr_Int( pXMLChildNode, _nInsertAt );
			return pXMLChildNode;
		}

		bool AttachNodePtr_Int( TNodePtr _pNode, int _nInsertAt = -1 )
		{
			ATLASSERT( _pNode );
			if( !_pNode ) return false;

			CAutoRWL rw(m_rwNodes, true);

			if( _nInsertAt >= 0 && _nInsertAt < m_mapNodes.GetSize() )
			{
				m_mapNodes.InsertAtIndex( _nInsertAt, _pNode->GetName(), _pNode );
			}
			else
			{
				m_mapNodes.Add( (LPCWSTR)_pNode->GetName(), _pNode );
			}

			return true;
		}


		//////////////////////////////////////////////////////////////////////////
		// Complex methods
	public:
		//////////////////////////////////////////////////////////////////////////
		// Parse / Save

		HRESULT SaveToFile( LPCWSTR _wszFileName, bool _bUnicode, BSTR* _pbsContent = NULL, int _nHTMLType = 0, LPCWSTR _pszFileHeader = NULL )
		{
			CComBSTR cbsDesc;
			HRESULT hr = SaveXML( &cbsDesc, 0, _nHTMLType );
			if( FAILED( hr ) ) return hr;

			HANDLE hFile = NULL;
			if( _wszFileName && _wszFileName[0] )
			{
				hFile = ::CreateFileW( _wszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL );
				if( !hFile ) return HRESULT_FROM_WIN32( GetLastError() );
			}

			if( hFile )
			{
				int nChars = cbsDesc.Length();
				if( !nChars ) return E_UNEXPECTED;

				if( !_bUnicode )
				{
					DWORD dwWritten = 0;
					if( !STR_IS_EMPTY( _pszFileHeader ) )
					{
						USES_CONVERSION;

						LPCSTR pszStr = W2CA_CP(_pszFileHeader, CP_UTF8);
						::WriteFile( hFile, pszStr, lstrlenA(pszStr), &dwWritten, NULL );
					}

					CBlobNew blobNew;
					blobNew.AllocMem( cbsDesc.Length() + 2 );
					char* szFile = (char*)blobNew.Ptr();
					::WideCharToMultiByte(CP_UTF8, 0, cbsDesc.m_str, -1, szFile, nChars + 1, NULL, NULL);

					::WriteFile( hFile, szFile, nChars, &dwWritten, NULL );
				}
				else
				{
					DWORD dwWritten = 0;
					if( _pszFileHeader && _pszFileHeader[0] )
					{
						::WriteFile( hFile, _pszFileHeader, lstrlenW(_pszFileHeader), &dwWritten, NULL );
					}

					::WriteFile( hFile, cbsDesc.m_str, nChars * sizeof(WCHAR), &dwWritten, NULL );
				}

				::CloseHandle(hFile);
			}

			if( _pbsContent ) *_pbsContent = cbsDesc.Detach();

			return S_OK;
		}

		HRESULT LoadAuto( LPCWSTR _wszValue )
		{
			if( !_wszValue ) 
				return E_INVALIDARG;

			if( XMLUtils::IsFile(_wszValue) && GetFileAttributesW( _wszValue ) != INVALID_FILE_ATTRIBUTES )
			{
				return LoadFromFile( _wszValue );
			}
			else
			{
				if( ParseXML( _wszValue, NULL, false) != S_OK )
				{
					return ParseAttributes( _wszValue, NULL );
				}
			}

			return S_OK;
		}

		HRESULT LoadFromFile( LPCWSTR _wszFileName, int nUnicode = -1 )
		{
			//		CBasicPerf perfXML(_T("PERF: XMLPrace::LoadFromFile") );

			USES_CONVERSION;

			CBlobNew blobData;
			HRESULT hr = blobData.LoadFromFile( W2CT(_wszFileName ), 2 );
			if( FAILED( hr ) ) return hr;

			CComBSTR cbsContent = Text2WideStr( blobData.Ptr(), blobData.GetSize() );

// 			LPWSTR pszContent = NULL;
// 			CBlobHeap blobConvert;
// 			if( nUnicode > 0 || ( nUnicode < 0 && IsUnicode( blobData.Ptr(), blobData.GetSize()) ) )
// 			{
// 				int nPos = 0;
// 				if( (blobData[0] & 0xFE) == 0xFE && (blobData[1] & 0xFE) == 0xFE)
// 					nPos = 2;
// 
// 				pszContent = (LPWSTR)blobData.Ptr(nPos);
// 			}
// 			else
// 			{
// 				// TODO: correct correction 
// 
// 				blobConvert.AllocMem( blobData.GetSize() * 2 + 2);
// 				pszContent = (LPWSTR)blobConvert.Ptr();
// 
// 				int nChars = ::MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, (LPCSTR)blobData.Ptr(), -1, pszContent, blobConvert.GetSize() );
// 				if( !nChars )
// 					nChars = ::MultiByteToWideChar( CP_ACP, 0, (LPCSTR)blobData.Ptr(), -1, pszContent, blobConvert.GetSize() );
// 
// 				pszContent[nChars] = 0;
// 			}

			XMLUtils::PrepareForParse(cbsContent);

			return ParseXML( cbsContent, NULL );
		}

		HRESULT ParseXML( LPCWSTR pszStart, LPCWSTR* ppszEnd, bool _bComplex = false, bool _bKeepBlank = false, bool _bKeepName = false, bool _bKeepSpecialChars = false )
		{
			if( !pszStart ) return E_INVALIDARG;

			// Clear nodes & attributes
			// Note: !!! Keep object (used e.g. in VM: SceneSet_Item_I)
			ClearNodes();
			ClearAttributes();

			LPCWSTR pEnd = ppszEnd && *ppszEnd ? *ppszEnd : pszStart + lstrlenW(pszStart);

			CComBSTR cbsOpenTag;
			LPCWSTR pStr = pszStart;
			int nTagType = 0;
			if( !_bComplex )
			{
				CComBSTR cbsParam;
				LPCWSTR pNodeStart = pszStart;
				while( pNodeStart )
				{
					// < tag />
					LPCWSTR pTagEnd = pEnd;
					pNodeStart = XMLUtils::NextTag( pStr, &pTagEnd, cbsOpenTag, &cbsParam, &nTagType );
					pStr = pTagEnd;

					// Check for </tag> -> IGNORE
					if( nTagType >= 0 ) break; 
				}
				if( !pNodeStart )
				{
					ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("###Err### [XMLParse] ParseXML - TOP NODE NOT FOUND:'%ws'\n"), pStr );
					return E_INVALIDARG;
				}

				ATLASSERT( cbsOpenTag.Length() );
				ChangeName( cbsOpenTag );		

				if( nTagType == 2 )
				{
					// <![CDATA[Some Text <TEXT xxx>]]>
					if( ppszEnd )
						*ppszEnd = pStr;

					AddValue( XMLUtils::XML2Value( cbsParam ) );	


					ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("[XMLParse] ParseXML - SIMPLE TAG:'%ws'\n"), GetName() );
					return S_OK;
				}

				if( cbsParam )
				{
					ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("[XMLParse] ParseXML - TAG NAME:'%ws' ATTR:'%ws'\n"), GetName(), cbsParam );
					ParseAttributes( cbsParam, NULL);
				}
				else
				{
					ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("[XMLParse] ParseXML - TAG NAME:'%ws' NO ATTR\n"), GetName() );
				}

				if( nTagType == 0 )
				{
					// <tag ... />

					if( ppszEnd )
						*ppszEnd = pStr;

					ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("[XMLParse] ParseXML - SIMPLE TAG:'%ws'\n"), GetName() );
					return S_OK;
				}
			}

			while( pStr < pEnd )
			{
				CComBSTR cbsNextTag;
				CComBSTR cbsParamInner;
				LPCWSTR pTagEnd = pEnd;
				LPCWSTR pNodeStart = XMLUtils::NextTag( pStr, &pTagEnd, cbsNextTag, &cbsParamInner, &nTagType );
				if( !pNodeStart )
				{
					if( pTagEnd == pEnd )
					{
						ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("!!!WARNING!!! [XMLParse] ParseXML - NOT FOUND CLOSE TAG FOR:'%ws'\n"), GetName() );

						pNodeStart = pEnd; // For put value
					}
					else
					{
						ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("!!!WARNING!!! [XMLParse] ParseXML - WRONG SYNTAX:'%ws'\n"), pStr );

						pStr = pTagEnd;
						continue;
					}
				}


				if( pNodeStart > pStr )
				{
					LPCWSTR pValueStart = _bKeepBlank ? pStr : XMLUtils::SkipBlank( pStr, true );
					LPCWSTR pValueEnd = _bKeepBlank ? pNodeStart - 1 : XMLUtils::SkipBlankRev( pNodeStart - 1, true );
					if( pValueEnd >= pValueStart )
					{
						CComBSTR cbsValue( pValueEnd - pValueStart + 1, pValueStart );

						// Check for the empty value (all space) here, if empty -> NOT add
						LPCWSTR pszValue = XMLUtils::XML2Value( cbsValue, _bKeepBlank, _bKeepSpecialChars );
						if( pszValue )
							AddValue( pszValue );

						ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("[XMLParse] ParseXML - Element '%ws' NEXT Value:'%ws'\n"), cbsOpenTag, pszValue );		
					}
				}

				if( nTagType < 0 )
				{
					// Check for current tag 
					// NOTE: The name could be changes -> use cbsOpenTag
					if( cbsNextTag == cbsOpenTag )
					{
						ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("[XMLParse] ParseXML - CLOSE TAG FOR:'%ws'\n"), cbsOpenTag );
						if( ppszEnd )
							*ppszEnd = pTagEnd;

						break;
					}

					ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("[XMLParse] ParseXML - UNPROPER CLOSE TAG:'%ws' - SKIP THIS\n"), cbsOpenTag );

					// Skip this tag as unknown
					pStr = pTagEnd;
				}
				else if( nTagType == 2 )
				{
					AddValue( XMLUtils::XML2Value( cbsParamInner ) );

					ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("[XMLParse] ParseXML - Element '%ws' NEXT CDATA Value:'%ws'\n"), cbsOpenTag, cbsParamInner );		

					pStr = pTagEnd;
				}
				else if( pNodeStart < pEnd )
				{
					TNodePtr pChildNode;
					T::CreateNode( &pChildNode, L"__temp__", NULL, NULL );
					pChildNode->m_pszSwapID = m_pszSwapID;
					
					ATLASSERT( pChildNode );
					if( !pChildNode ) return E_UNEXPECTED;

					LPCWSTR pNodeEnd = pEnd;
					if( pChildNode->ParseXML( pNodeStart, &pNodeEnd, false, _bKeepBlank, false, _bKeepSpecialChars ) != S_OK )
					{
						ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("[XMLParse] ParseXML - ERROR PARSE:'%ws' - SKIP THIS\n"), pNodeStart );

						// Error - skip unknown or wrong tag
						//RemoveNodePtr( pChildNode );

						pStr = pTagEnd;
					}
					else
					{
						pChildNode->SetParent( (T*)this, false );
						AttachNodePtr_Int( pChildNode );

						pStr = pNodeEnd;
					}
				}
				else
				{
					if( ppszEnd )
						*ppszEnd = pEnd;

					break;
				}
			}

			return S_OK;
		}

		int GetAllNodes( LPCWSTR _wszName, deque<TNodePtr>& _deqNodes, bool _bNested )
		{
			if( STR_IS_EMPTY( _wszName ) )
				return 0;

			CAutoRWL( m_rwNodes, false );

			int nCounter = _deqNodes.size();
			for( int i = 0; i < m_mapNodes.GetSize(); ++i )
			{
				if( !m_mapNodes[i]->IsValue() )
				{
					if( m_mapNodes[i]->IsName( _wszName ) )
					{
						_deqNodes.push_back( m_mapNodes[i] );
						++nCounter;
					}

					if( _bNested )
						nCounter += m_mapNodes[i]->GetAllNodes( _wszName, _deqNodes, _bNested );
				}
			}

			return nCounter;
		}

		HRESULT SaveXML( BSTR* _pbsContent, int _nTabs = 0, int _nHTMLType = 0 )
		{
			if( !_pbsContent ) return E_POINTER;

			CComPtr<IStream> cpStream;
			::CreateStreamOnHGlobal( NULL, TRUE, &cpStream );

			HRESULT hr = SaveXML( cpStream, _nTabs, true, _nHTMLType );
			if( FAILED( hr ) ) return hr;

			CBlobNew blobTest;
			blobTest.ConvertFromStream(cpStream);

			CComBSTR cbsDesc = (WCHAR*)blobTest.Ptr();

			*_pbsContent = cbsDesc.Detach();
			return S_OK;
		}


		inline int TagType()
		{
			if( IsBasicNode() ) return 4;

			CComBSTR cbsName; 
			GetName( &cbsName );

			if( !cbsName ) return 0;

			if( lstrcmpiW( cbsName, L"br" ) == 0 ) return 1;

			if( lstrcmpiW( cbsName, L"tab" ) == 0 ) return 2;

			if( lstrcmpiW( cbsName, L"i" ) == 0 ||
				lstrcmpiW( cbsName, L"b" ) == 0 ||
				lstrcmpiW( cbsName, L"u" ) == 0 ||
				lstrcmpiW( cbsName, L"p" ) == 0 ||
				lstrcmpiW( cbsName, L"font" ) == 0 ||
				lstrcmpiW( cbsName, L"span" ) == 0 ) return 3;

			return 0;
		}

		// Save node
		// <tag>value</tag>
		// <tag attr='xxx'>value</tag>
		// <tag attr='xxx'>
		//		value-1
		//		<next>XXXX</next>
		//		value-x
		// </tag>
		// Exception

		HRESULT SaveXML( IStream* _pStream, int nTabs, bool _bPutEOS = true, int _nHTMLType = 0)
		{
			if( IsValue() ) return false;

			if( !_pStream ) return E_INVALIDARG;

			int nTagType = _nHTMLType ?  TagType() : 0;

			// WARNING !!! Dangerous 512 bytes alloc + TABSTR()
			WCHAR szTABBuffer[512]={0};

			ULONG ulRes = 0;
			if( nTabs >= 0 ) 
				_pStream->Write( TABSTR(nTabs), nTabs * sizeof(WCHAR), &ulRes );

			CComBSTR cbsNodeName = Name(true);
			
			//CharLowerW(cbsNodeName);
			_pStream->Write( L"<", sizeof(WCHAR), &ulRes );
			_pStream->Write( cbsNodeName, cbsNodeName.Length() * sizeof(WCHAR), &ulRes );

			LPWSTR pszDelimer = L" ";
			if( _nHTMLType == 2 )
			{
				pszDelimer = (LPWSTR)alloca( (nTabs + 8) * sizeof(WCHAR) );
				wsprintfW( pszDelimer, L"\r\n%ws", TABSTR( nTabs + 3) );
			}

			SaveAttributes( _pStream, false, false, NULL, NULL, NULL, pszDelimer );

			CAutoRWL rw( m_rwNodes, false );

			if( GetNodesCount() == 0 )
			{
				_pStream->Write( L"/>", sizeof(WCHAR) * 2, &ulRes );
			}
			else
			{
				_pStream->Write( L">", sizeof(WCHAR), &ulRes );

				if( !HaveNodes() )
				{
					ATLASSERT( GetNodesCount() == 1 );

					CComBSTR cbsValue = GetValue();
					XMLUtils::Value2XML( cbsValue, 0 );
					_pStream->Write( cbsValue, cbsValue.Length() * sizeof(WCHAR), &ulRes );
				}
				else
				{
					bool bNextNL = false;
					for( int i = 0; i < GetNodesCount(); i++ )
					{
						TNodePtr pNode = GetNodeAt(i);
						int nNextTextTag = _nHTMLType ? pNode->TagType() : 0;

						bool bPutNL = nNextTextTag <= 1 || (i == 0 && !nTagType) || bNextNL;
						if( bPutNL && nTabs >= 0 && !pNode->m_bPrintOneLine )
							_pStream->Write( L"\r\n", sizeof(WCHAR) * 2, &ulRes );

						bNextNL = (nNextTextTag == 0); // Put new line after normal block

						if( pNode->IsValue() )
						{
							if( !nTagType && !pNode->m_bPrintOneLine ) 
							{
								nTabs = MIN( nTabs, SIZEOF_ARRAY(szTABBuffer) - 2 );
								_pStream->Write( XMLUtils::PrintTab(nTabs+1, szTABBuffer), (nTabs + 1)* sizeof(WCHAR), &ulRes );
							}

							CComBSTR cbsValue = pNode->Value();
							XMLUtils::Value2XML( cbsValue, 0 );
							_pStream->Write( cbsValue, cbsValue.Length() * sizeof(WCHAR), &ulRes );
						}
						else
						{
							pNode->SaveXML( _pStream,  (nTabs < 0 || pNode->m_bPrintOneLine) ? -1 : (bPutNL ? nTabs + 1 : nTabs), false, _nHTMLType );
						}
					}

					// Before end tag (e.g. </text> )
					if( !nTagType && nTabs >= 0 && !m_bPrintOneLine )
					{
						_pStream->Write( L"\r\n", sizeof(WCHAR) * 2, &ulRes );

						nTabs = MIN( nTabs, SIZEOF_ARRAY(szTABBuffer) - 2 );
						_pStream->Write( XMLUtils::PrintTab(nTabs, szTABBuffer), (nTabs)* sizeof(WCHAR), &ulRes );
					}
				}

				_pStream->Write( L"</", sizeof(WCHAR)*2, &ulRes );
				_pStream->Write( cbsNodeName, cbsNodeName.Length() * sizeof(WCHAR), &ulRes );
				_pStream->Write( L">", sizeof(WCHAR), &ulRes );
			}


			if( _bPutEOS ) _pStream->Write( L"\0", sizeof(WCHAR), &ulRes );

			return S_OK;
		}

		static void SaveValue( IStream* _pStream, LPWSTR pszValue )
		{
			ULONG ulRes = 0;
			int nLen = lstrlenW( pszValue );
			bool bQuotes = StrChrW( pszValue, L'"' ) != NULL;
			bool bApos = StrChrW( pszValue, L'\'' ) != NULL;
			if( bApos )
			{
				_pStream->Write( L"\"", sizeof(WCHAR), &ulRes );
				// Replace " by ' 
				// e.g. This is Raphael's "Foligno" Madonna ... ->  This is Raphael's 'Foligno' Madonna
				if( bQuotes ) XMLUtils::ReplaceChars( pszValue, L"\"", L'\'' );  
				_pStream->Write( pszValue, nLen * sizeof(WCHAR), &ulRes );
				_pStream->Write( L"\"", sizeof(WCHAR), &ulRes );
			}
			else if( true ) // bQuotes || !nLen || StrCSpnIW( pszValue, L" \n\r\t`~!@#$%^&*()_-+={}[]\\|;:',.<>/? ;,:.)(+?!*`~=^#@&$[]{}|\\/" ) != lstrlenW( pszValue ) )
			{
				_pStream->Write( L"'", sizeof(WCHAR), &ulRes );
				_pStream->Write( pszValue, nLen * sizeof(WCHAR), &ulRes );
				_pStream->Write( L"'", sizeof(WCHAR), &ulRes );				
			}
		}

		HRESULT SaveAttributes( IStream* _pStream, 
								bool _bPutEOS = false, 
								bool _bSaveNodeValue = false, 
							    LPCWSTR _pszPrefix = NULL, 
								const LPCWSTR* _ppszIgnoreList = NULL,
								const LPCWSTR* _ppszSaveList = NULL,
								LPCWSTR  _pszDelimer = L" " ) 
		{
			ULONG ulRes = 0;

			ATLASSERT( _pszDelimer );
			int nDelLen = lstrlenW(_pszDelimer);

			CAutoRWL rw(m_pRWA,false);

			bool bFirstAttribute = true;
			if( IsSwapID(NULL) && m_cbsExternName )
			{
				// Save ID
				_pStream->Write( L" ", sizeof(WCHAR), &ulRes );
				_pStream->Write( m_pszSwapID, lstrlenW(m_pszSwapID) * sizeof(WCHAR), &ulRes );
				_pStream->Write( L"=", sizeof(WCHAR), &ulRes );

				bFirstAttribute = false;

				SaveValue( _pStream, Name() );
			}

			CComBSTR cbsValue = GetValue();
			if( _bSaveNodeValue && cbsValue.Length() )
			{
				_pStream->Write( L" _node_value=", lstrlenW(L" _node_value=") * sizeof(WCHAR), &ulRes );

				SaveValue( _pStream, cbsValue );
			}
			
			if( _ppszSaveList )
			{
				ATLASSERT( !_ppszIgnoreList );
				const LPCWSTR* ppszSaveName = _ppszSaveList;
				while( *ppszSaveName )
				{
					XMLNamePart xmlName = { *ppszSaveName, lstrlenW(*ppszSaveName) };
					int nFind = m_mapAttributes.FindAttribute( &xmlName );
					if( nFind >= 0 )
					{
						CComBSTR cbsKey = m_mapAttributes.GetKeyAt(nFind);
						_pStream->Write( L" ", sizeof(WCHAR), &ulRes );
						if( _pszPrefix && _pszPrefix[0] )
							_pStream->Write( _pszPrefix, lstrlenW(_pszPrefix) * sizeof(WCHAR), &ulRes );
						_pStream->Write( cbsKey.m_str, cbsKey.Length() * sizeof(WCHAR), &ulRes );
						_pStream->Write( L"=", sizeof(WCHAR), &ulRes );

						CComBSTR cbsValue = m_mapAttributes.GetValueAt(nFind)->GetString();
						SaveValue( _pStream, cbsValue );
					}

					ppszSaveName++;
				}
			}
			else
			{
				for( int i = 0; i < m_mapAttributes.GetSize(); i++ )
				{
					CComBSTR cbsKey = m_mapAttributes.GetKeyAt(i);
					if( !InStringListI( cbsKey, _ppszIgnoreList ) )
					{
						if( bFirstAttribute )
							_pStream->Write( L" ", sizeof(WCHAR), &ulRes );
						else 
							_pStream->Write( _pszDelimer, sizeof(WCHAR) * nDelLen, &ulRes );
						if( _pszPrefix && _pszPrefix[0] )
							_pStream->Write( _pszPrefix, lstrlenW(_pszPrefix) * sizeof(WCHAR), &ulRes );
						_pStream->Write( cbsKey.m_str, cbsKey.Length() * sizeof(WCHAR), &ulRes );
						_pStream->Write( L"=", sizeof(WCHAR), &ulRes );

						CComBSTR cbsValue = m_mapAttributes.GetValueAt(i)->GetString();
						SaveValue( _pStream, cbsValue );

						bFirstAttribute = false;
					}
				}
			}

			

			if( _bPutEOS ) _pStream->Write( L"\0", sizeof(WCHAR), &ulRes );

			return m_mapAttributes.GetSize() ? S_OK : S_FALSE;
		}

		HRESULT SaveAttributes( BSTR* _pbsContent, bool _bSaveNodeValue = false, LPCWSTR _pszPrefix = NULL, const LPCWSTR* _ppszIgnoreList = NULL, const LPCWSTR* _ppszSaveList = NULL )
		{
			if( !_pbsContent ) return E_POINTER;

			CComPtr<IStream> cpStream;
			::CreateStreamOnHGlobal( NULL, TRUE, &cpStream );
			if( !cpStream ) return E_FAIL;

			HRESULT hr = SaveAttributes( cpStream, TRUE, _bSaveNodeValue, _pszPrefix, _ppszIgnoreList, _ppszSaveList );
			if( FAILED( hr ) ) return hr;

			CBlobNew blobTest;
			blobTest.ConvertFromStream(cpStream);

			CComBSTR cbsDesc = (WCHAR*)blobTest.Ptr();

			*_pbsContent = cbsDesc.Detach();
			return S_OK;
		}

		HRESULT SaveAttributes_R( IStream* _pStream, bool _bSaveNodeValue = false, LPCWSTR _pszPrefix = NULL, const LPCWSTR* _ppszIgnore = NULL)
		{
			SaveAttributes( _pStream, FALSE, _bSaveNodeValue, _pszPrefix, _ppszIgnore );

			CAutoRWL rw(m_rwNodes, false );

			for( int i = 0; i < m_mapNodes.GetSize(); i++ )
			{
				TNodePtr pNode = m_mapNodes[i];
				if( pNode && !pNode->IsValue() )
				{
					CComBSTR cbsName;
					pNode->GetName( &cbsName );

					CComBSTR cbsPrefix = _pszPrefix;
					cbsPrefix.Append( cbsName );
					cbsPrefix.Append( L"::" );

					pNode->SaveAttributes_R( _pStream, _bSaveNodeValue, cbsPrefix, _ppszIgnore );
				}
			}

			return S_OK;
		} 
		HRESULT SaveAttributes_R( BSTR* _pbsContent, bool _bSaveNodeValue = false, LPCWSTR _pszPrefix = NULL, const LPCWSTR* _ppszIgnore = NULL )
		{
			if( !_pbsContent ) return E_POINTER;

			CComPtr<IStream> cpStream;
			::CreateStreamOnHGlobal( NULL, TRUE, &cpStream );
			if( !cpStream ) return E_FAIL;

			HRESULT hr = SaveAttributes_R( cpStream, _bSaveNodeValue, _pszPrefix, _ppszIgnore );
			if( FAILED( hr ) ) return hr;

			cpStream->Write( L"\0", sizeof(WCHAR), NULL );

			CBlobNew blobTest;
			blobTest.ConvertFromStream(cpStream);

			CComBSTR cbsDesc = (WCHAR*)blobTest.Ptr();

			*_pbsContent = cbsDesc.Detach();
			return S_OK;
		}
		
		inline CComBSTR AttributesList(bool _bSaveNodeValue = false, LPCWSTR _pszPrefix = NULL, const LPCWSTR* _ppszIgnoreList = NULL, const LPCWSTR* _ppszSaveList = NULL)
		{
			CComBSTR cbsRes;
			SaveAttributes(&cbsRes, _bSaveNodeValue, _pszPrefix, _ppszIgnoreList, _ppszSaveList);
			return cbsRes;
		}

		inline CComBSTR AttributesList_R(bool _bSaveNodeValue = false, LPCWSTR _pszPrefix = NULL, const LPCWSTR* _ppszIgnoreList = NULL)
		{
			CComBSTR cbsRes;
			SaveAttributes_R(&cbsRes, _bSaveNodeValue, _pszPrefix, _ppszIgnoreList);
			return cbsRes;
		}

		// _bComplex -> allow to have attibutes like node::value=xxx
		bool ParseAttributes( LPCWSTR _wszContent, LPCWSTR _wszContentEnd, bool _bAddFlags = false, int _nOverrideType = 1, 
			bool _bComplex = false, MStringList* _pAttrList = NULL )
		{
			if( !_wszContent || !_wszContent[0] ) return false;

			if( _nOverrideType == 2 )
				ClearAttributes();

			bool bHaveAttributes = false;
			LPCWSTR pStr = _wszContent;
			LPCWSTR pEnd = _wszContentEnd ? _wszContentEnd : pStr + lstrlenW(pStr);
			while( pStr < pEnd )
			{
				// Skip spaces
				while( pStr < pEnd && XMLUtils::IsBlank(*pStr) ) pStr++;

				// Get the variable name (key)
				const WCHAR* pVarStart = pStr;
				while( pStr < pEnd && !XMLUtils::IsBlank(*pStr) && *pStr != L'=' ) pStr++;

				XMLNamePart xmlKey ={};
				if( pVarStart < pStr )
				{
					//cbsKey.Attach( ::SysAllocStringLen( pVarStart, pStr - pVarStart ) );
					xmlKey.pszPart = pVarStart;
					xmlKey.nLen = pStr - pVarStart;
					//if( _bLowerCase ) CharLowerW( cbsKey.m_str );
				}
				else if( pStr >= pEnd ) 
				{
					break;
				}
				// else the ' .. key=val  =something ..' string 

				// Skip spaces
				while( pStr < pEnd && XMLUtils::IsBlank(*pStr) ) pStr++;

				// Search '='
				if( *pStr != L'=' )
				{
					if( _bAddFlags )
					{
						AddAttribute_Int( &xmlKey, L"", true, -1 );
						bHaveAttributes = true;

						if( _pAttrList )
							_pAttrList->AddString( xmlKey.pszPart, xmlKey.nLen );
						
					}

					continue;
				}

				// Skip '='
				pStr++;

				// Skip spaces
				while( pStr < pEnd && XMLUtils::IsBlank(*pStr) ) pStr++;

				if( pStr >= pEnd ) break;

				// Add to list
				if( _pAttrList )
					_pAttrList->AddString( xmlKey.pszPart, xmlKey.nLen );

				// Get value
				CComBSTR cbsValue;
				XMLUtils::_ParamStr_Value(pStr, &cbsValue, pEnd, &pStr);
				
// 				int nSkip = 0;
// 				const WCHAR* pVarEnd = NULL;
// 				if( *pStr == L'\'' )
// 				{
// 					pStr++;
// 					// Find end 
// 					pVarEnd = StrChrW( pStr, L'\'' );
// 					nSkip = 1;
// 				}
// 				else if( *pStr == L'\"' )
// 				{
// 					pStr++;
// 					// Find end 
// 					pVarEnd = StrChrW( pStr, L'\"' );
// 					nSkip = 1;
// 				}
// 				else
// 				{
// 					// Find blank
// 					pVarEnd = pStr;
// 					while( pVarEnd < pEnd && !XMLUtils::IsBlank(*pVarEnd) ) pVarEnd++;
// 				}
// 
// 				if( !pVarEnd ) 
// 					pVarEnd = pEnd;
// 				else
// 					pVarEnd = MIN( pVarEnd, pEnd );
// 
// 
// 				cbsValue.Attach( ::SysAllocStringLen( pStr, pVarEnd - pStr ) );
// 
// 				pStr = pVarEnd + nSkip;

				ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("[XMLParse] ParseAttributes - Attribute:'%ws' Value:'%ws'\n"), 
					xmlKey.pszPart, cbsValue );

				if( _bComplex )
				{
					SetAttribute( CComBSTR(xmlKey.nLen, xmlKey.pszPart), cbsValue, true );
				}
				else
				{
					AddAttribute_Int( &xmlKey, (LPCWSTR)cbsValue.m_str, _nOverrideType > 0, -1 );
				}

				bHaveAttributes = true;
			}

			return bHaveAttributes;
		}

		// This method set the node or attributes value
		// for basic names 
		HRESULT LoadNode( LPCWSTR _wszName, LPCWSTR _wszValue, eSetNodeType _eSetNodeValue = eSNT_Attributes, bool _bCreateNew = TRUE )
		{
			eValueType eType = GetValueType_Int( _wszValue );
			bool bBasicName = StrStrW( _wszName, L"::" ) ==  NULL;

			// Check value type
			if( _wszName && _wszName[0] != 0 )
			{
				if( eType == eRemove )
				{
					if( _eSetNodeValue == eSNT_Nodes || (bBasicName && _eSetNodeValue == eSNT_Playout) )
						return RemoveNode( _wszName ) ? S_OK : HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );
					else
						return RemoveAttribute( _wszName ) ? S_OK : HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );
				}
				else if( eType == eBasic )
				{
					if( _eSetNodeValue == eSNT_Nodes || (bBasicName && _eSetNodeValue == eSNT_Playout) )
						return SetNodeValue( _wszName, _wszValue, _bCreateNew ) ? S_OK : HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );
					else
						return SetAttribute( _wszName, _wszValue, _bCreateNew ) ? S_OK : HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );
				}
				else if( eType == eParam )
				{
					TNodePtr pNode = GetNodePtr( _wszName, _bCreateNew );
					if( !pNode ) return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );

					return pNode->ParseAttributes( _wszValue, NULL );
				}
				else if( eType == eXML )
				{
					TNodePtr pNode = GetNodePtr( _wszName, _bCreateNew );
					if( !pNode ) return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );

					// Check for correct name
					TNodePtr pTest;
					CreateNode( &pTest, L"test" );
					HRESULT hr = pTest->ParseXML( _wszValue, NULL );
					if( FAILED( hr ) ) return hr;

					if( pTest->Name() != pNode->Name() ) return HRESULT_FROM_WIN32( ERROR_INVALID_PARAMETER );

					pTest->CopyTo( pNode );
					return S_OK;
				}
			}
			else
			{
				if( eType == eRemove )
				{
					Clear( false );
				}
				else if( eType == eBasic )
				{
					SetValue( _wszValue );
				}
				else if( eType == eParam )
				{
					if( !ParseAttributes( _wszValue, NULL, false, 1, true ) )
						return E_FAIL;
				}
				else if( eType == eXML )
				{
					// Check for correct name
					HRESULT hr = ParseXML( _wszValue, NULL );
					if( FAILED( hr ) ) return hr;

// 					TNodePtr pTest; // For ID swap
// 					CreateNode( &pTest, L"test" );
// 					HRESULT hr = XMLParse::ParseXML( _wszValue, NULL, pTest);
// 					if( FAILED( hr ) ) return hr;
// 
// 					//if( xmlTest.m_cbsNameOrValue != m_cbsNameOrValue ) return HRESULT_FROM_WIN32( ERROR_INVALID_PARAMETER );
// 
// 					pTest->CopyTo( (T*)this, false );
				}

				return S_OK;
			}

			return E_INVALIDARG;
		}

		HRESULT SaveNode( LPCWSTR _wszName, BSTR* _pbsValue )
		{
			if( !_pbsValue ) return E_POINTER;

			TNodePtr pTargetNode(this);
			if( _wszName && _wszName[0] != 0 )
			{
				pTargetNode = GetNodePtr( _wszName );
				if( !pTargetNode )
				{
					CComBSTR cbsValue = GetStringAttribute_Safe( _wszName, NULL );
					*_pbsValue = cbsValue.Detach();
					return S_OK;
				}
			}


			// For <node/> -> return <node/>
			// For <node>Value</node> -> return Value
			// For <node attr='Some'/> -> return attr='Some'
			// For <node attr='Some'>Value</node> (and other cases)-> return <node attr='Some'>Value</node>


			if( pTargetNode->GetNodesCount() == 0 )
			{
				if( pTargetNode->GetAttributesCount() )
				{
					// For <node attr='Some'/> -> return attr='Some'
					return pTargetNode->SaveAttributes( _pbsValue );
				}
			}
			else if( pTargetNode->GetNodesCount() == 1 && pTargetNode->GetAttributesCount() == 0  )
			{
				TNodePtr pTargetValue = pTargetNode->GetNodeAt(0);
				// only value 
				if( pTargetValue && pTargetValue->IsValue() )
					return pTargetNode->Value().CopyTo( _pbsValue );
			}

			return pTargetNode->SaveXML( _pbsValue );
		}
	}; // XMLNode

#ifndef LITE_XML_PARSE

	class CoXMLNode;

	class XMLNodePtr: public CAutoObj<CoXMLNode>
	{
	public:
		XMLNodePtr()
		{
			p = NULL;
		}

		~XMLNodePtr()
		{
			if(p)
				p->Release();
			p = NULL;
		}

// 		XMLNodePtr( LPCWSTR _pszNodeName, CoXMLNode* pParent )
// 		{
// 			CoXMLNode::CreateNode( &p, _pszNodeName );
// 		}

		XMLNodePtr( LPCWSTR _pszNodeName, CoXMLNode* pParent, LPCWSTR _pszSwapID = NULL)
		{
			CoXMLNode::CreateNode( &p, _pszNodeName, pParent, _pszSwapID );
		}

		XMLNodePtr(LPCWSTR _pszParametets, eCreateNodeParam _eParamType )
		{
			CoXMLNode::CreateNode(&p, _eParamType == eCNP_BasicNode ? _pszParametets : L"node_param", NULL, NULL);
			if( _eParamType == eCNP_ParseAttributes )
				p->ParseAttributes( _pszParametets, NULL, false, 2, false );
			else if( _eParamType == eCNP_ParseAttributes_Comlpex )
				p->ParseAttributes( _pszParametets, NULL, false, 2, false );
			else if( _eParamType == eCNP_ParseXML )
				p->ParseXML( _pszParametets, NULL, false, false, false, false );
			else if( _eParamType == eCNP_ParseXML_Complex )
				p->ParseXML( _pszParametets, NULL, true );
		}


		explicit XMLNodePtr(CoXMLNode* pT)
		{
			p = pT;
			if(p)
				p->AddRef();
		}

		XMLNodePtr operator [] (int _nIndex )
		{
			ATLASSERT( p );
			if( p )
				return p->GetNodeAt(_nIndex);

			return (XMLNodePtr)NULL;
		}

		XMLNodePtr(const XMLNodePtr& apT)
		{
			p = apT.p;
			if(p)
				p->AddRef();
		}
	};

	template <class T, class TNodePtr, class TInterface>
	class IMAttributesImpl
		: public TInterface
	{
	protected:
		T*		m_pNodeThis;
		int		m_nInfoCount;

		void EnableAttributesInfo( bool _bEnable )
		{
			if( _bEnable )
			{
				m_pNodeThis->m_bAttributesInfo = true;
				if( !m_pNodeThis->m_ppszHiddenNodes )
					m_pNodeThis->m_ppszHiddenNodes = ppszHiddenAttrInfoStr;
			}
			else
			{
				m_pNodeThis->m_bAttributesInfo = true;
				if( m_pNodeThis->m_ppszHiddenNodes == ppszHiddenAttrInfoStr )
					m_pNodeThis->m_ppszHiddenNodes = NULL;
			}
		}

		
	public:

		IMAttributesImpl( T* _pThisNode = NULL )
			: m_pNodeThis( _pThisNode )
		{
			m_nInfoCount = 0;
		}
		// [helpstring)("Get attributes count")] 
		STDMETHOD(AttributesGetCount)( /*[out]*/ int* _pnCount )
		{
			ATLASSERT( m_pNodeThis );

			int nCount = m_pNodeThis->GetAttributesCount();
			if( m_pNodeThis->m_ppszHiddenNodes )
			{
				const LPCWSTR* ppszNode = m_pNodeThis->m_ppszHiddenNodes;
				if( m_pNodeThis->m_bAttributesInfo )
					ppszNode++;
				
				while( *ppszNode )
				{
					TNodePtr pNode = m_pNodeThis->GetNodePtr( *ppszNode );
					if( pNode )
						nCount += pNode->GetAttributesCount();

					ppszNode++;
				}
			}

			if( _pnCount )
				*_pnCount = nCount;

			return nCount ? S_OK : S_FALSE;
		}

		// [helpstring)("Get attributes count")] 
		STDMETHOD(AttributesGetByIndex)( /*[in]*/ int _nIndex, /*[out]*/ BSTR* _pbsName, /*[out]*/ BSTR* _pbsValue )
		{
			CAutoRWL rw( m_pNodeThis->m_pRWA, false );

			
			CComBSTR cbsNodeName;
			int nIndex = _nIndex;
			TNodePtr pNode = TNodePtr( m_pNodeThis );
			if( m_pNodeThis->m_ppszHiddenNodes && nIndex >= m_pNodeThis->GetAttributesCount() )
			{
				nIndex -= m_pNodeThis->GetAttributesCount();

				const LPCWSTR* ppszNode = m_pNodeThis->m_ppszHiddenNodes;
				if( m_pNodeThis->m_bAttributesInfo )
					ppszNode++;
				while( *ppszNode  )
				{
					pNode = m_pNodeThis->GetNodePtr( *ppszNode );
					if( pNode )
					{
						if( nIndex < pNode->GetAttributesCount() )
						{
							cbsNodeName = *ppszNode;
							cbsNodeName.Append( L"::" );
							break;
						}

						nIndex -= pNode->GetAttributesCount();
					}

					ppszNode++;
				}
			}

			CComBSTR cbsName = pNode->GetAttributeName(nIndex);
			if( !cbsName ) return HRESULT_FROM_WIN32(ERROR_INVALID_INDEX);

			if( cbsNodeName )
			{
				cbsNodeName.Append( cbsName );
				cbsName = cbsNodeName;
			}

			CComBSTR cbsValue = pNode->GetAttributeValue( nIndex );

			if( _pbsName)
				*_pbsName = cbsName.Detach();

			if( _pbsValue)
				*_pbsValue = cbsValue.Detach();

			return S_OK;
		}

		// [helpstring)("Remove attribute")] 
		STDMETHOD(AttributesRemove)( /*[in]*/ BSTR _bsName )
		{
			if( !m_pNodeThis->RemoveAttribute( _bsName ) )
				return HRESULT_FROM_WIN32( ERROR_NOT_FOUND );

			return S_OK;
		}

		// [helpstring)("Remove all attribute")] 
		STDMETHOD(AttributesRemoveAll)()
		{
			m_pNodeThis->ClearAttributes();
			return S_OK;
		}

		// [helpstring)("Set attribute value")] 
		STDMETHOD(AttributesStringSet)( /*[in]*/ BSTR _bsName, /*[in]*/ BSTR _bsValue )
		{
			if( !m_pNodeThis->SetAttribute( _bsName, _bsValue, true ) )
				return E_INVALIDARG;

			return S_OK;
		}

		// [helpstring)("Set attribute int value")] 
		STDMETHOD(AttributesIntSet)( /*[in]*/ BSTR _bsName, /*[in]*/ int _nValue )
		{
			if( !m_pNodeThis->SetAttribute( _bsName, _nValue, true ) )
				return E_INVALIDARG;

			return S_OK;
		}

		STDMETHOD(AttributesBoolSet)( /*[in]*/ BSTR _bsName, /*[in]*/ BOOL _bValue )
		{
			if( !m_pNodeThis->SetAttribute( _bsName, _bValue ? true : false, true ) )
				return E_INVALIDARG;

			return S_OK;
		}

		// [helpstring)("Set attribute double value")] 
		STDMETHOD(AttributesDoubleSet)( /*[in]*/ BSTR _bsName, /*[in]*/ double _dblValue )
		{
			if( !m_pNodeThis->SetAttribute( _bsName, _dblValue, true ) )
				return E_INVALIDARG;

			return S_OK;
		}

		// [helpstring)("Get attribute value")] 
		STDMETHOD(AttributesStringGet)( /*[in]*/ BSTR _bsName, /*[out]*/ BSTR* _pbsValue )
		{
			if( !_pbsValue )
				return E_POINTER;

			bool bFind = false;
			CComBSTR cbsValue = m_pNodeThis->GetStringAttribute_Safe( _bsName, NULL, &bFind );
			if( !bFind )
				return HRESULT_FROM_WIN32( ERROR_NOT_FOUND );


			*_pbsValue = cbsValue.Detach();
			return S_OK;
		}

		// [helpstring)("Check existence of attribute")] 
		STDMETHOD(AttributesHave)( /*[in]*/ BSTR _bsName, /*[out]*/ BOOL* _pbHave, /*[out]*/ BSTR* _pbsValue )
		{
			bool bFind = false;
			CComBSTR cbsValue = m_pNodeThis->GetStringAttribute_Safe( _bsName, NULL, &bFind );

			if( _pbHave )
				*_pbHave = bFind;

			if( !bFind )
			{
				AttributesInfoGet( _bsName, eMIT_Default, _pbsValue );
			}
			else if( _pbsValue )
				*_pbsValue = cbsValue.Detach();

			return bFind ? S_OK : S_FALSE;
		}

		// [helpstring)("Get attribute int value )(conversion to int from string performed)")] 
		STDMETHOD(AttributesIntGet)( /*[in]*/ BSTR _bsName, /*[out]*/ int* _pnValue )
		{
			if( !_pnValue )
				return E_POINTER;

			bool bFind = false;
			*_pnValue = m_pNodeThis->GetIntAttribute( _bsName, 0, &bFind );
			if( !bFind )
				return HRESULT_FROM_WIN32( ERROR_NOT_FOUND );

			return S_OK;
		}

		// [helpstring)("Get attribute int value )(conversion to int from string performed)")] 
		STDMETHOD(AttributesBoolGet)( /*[in]*/ BSTR _bsName, /*[out]*/ BOOL* _pbValue )
		{
			if( !_pbValue )
				return E_POINTER;

			bool bFind = false;
			*_pbValue = m_pNodeThis->GetBoolAttribute( _bsName, 0, &bFind );
			if( !bFind )
				return HRESULT_FROM_WIN32( ERROR_NOT_FOUND );

			return S_OK;
		}

		// [helpstring)("Get attribute double value )(conversion to int from string performed)")] 
		STDMETHOD(AttributesDoubleGet)( /*[in]*/ BSTR _bsName, /*[out]*/ double* _pdblValue )
		{
			if( !_pdblValue )
				return E_POINTER;

			bool bFind = false;
			*_pdblValue = m_pNodeThis->GetDblAttribute( _bsName, 0, &bFind );
			if( !bFind )
				return HRESULT_FROM_WIN32( ERROR_NOT_FOUND );

			return S_OK;
		}

		// [helpstring)("Save all attributes to string")] 
		STDMETHOD(AttributesSave)( /*[out]*/ BSTR* _pbsAttributesList )
		{
			if( !_pbsAttributesList )
				return E_POINTER;

			CComBSTR cbsAttributesList;
			m_pNodeThis->SaveAttributes( &cbsAttributesList );

			WCHAR szPrefix[MAX_PATH]={};
			if( m_pNodeThis->m_ppszHiddenNodes )
			{
				const LPCWSTR* ppszNode = m_pNodeThis->m_ppszHiddenNodes;
				if( m_pNodeThis->m_bAttributesInfo )
					ppszNode++;
				while( *ppszNode )
				{
					TNodePtr pNode = m_pNodeThis->GetNodePtr( *ppszNode );
					if( pNode )
					{
						wsprintfW( szPrefix, L"%ws::", *ppszNode );
						CComBSTR cbsNodeAttr;
						pNode->SaveAttributes( &cbsNodeAttr, false, szPrefix );

						cbsAttributesList.Append( L" " );
						cbsAttributesList.Append( cbsNodeAttr );
					}

					ppszNode++;
				}
			}

			*_pbsAttributesList = cbsAttributesList.Detach();
			return S_OK;
		}

		// [helpstring)("Parse attibutes string and update current attributes")] 
		STDMETHOD(AttributesMultipleSet)( /*[in]*/ BSTR _bsAttributesList, /*[in]*/ eMUpdateType _eUpdateType )
		{
			if( !_bsAttributesList )
				return E_INVALIDARG;

			return m_pNodeThis->ParseAttributes( _bsAttributesList, NULL, false, _eUpdateType, true );
		}

		// [helpstring)("Clone attibutes")] 
		STDMETHOD(AttributesClone)( /*[out]*/ IMAttributes** _ppClone )
		{
			TNodePtr pNode;
			HRESULT hr = T::CreateNode( &pNode, L"m_attributes");
			if( FAILED( hr ) )
				return hr;

			m_pNodeThis->CopyAttributes( pNode, 0 );

			return pNode.QueryInterface( _ppClone );
		}

		

		// [helpstring)("Set attibutes")] 
		STDMETHOD(AttributesSet)( /*[in]*/ IMAttributes* _pAttibutesSet, /*[in]*/ eMUpdateType _eUpdateType )
		{
			if( !_pAttibutesSet )
				return E_INVALIDARG;

			if( _eUpdateType == eMUT_ClearDest )
				m_pNodeThis->ClearAttributes();

			int nCount = 0;
			_pAttibutesSet->AttributesGetCount( &nCount );
			for( int i = 0; i < nCount; i++ )
			{
				CComBSTR cbsName;
				CComBSTR cbsValue;
				HRESULT hr = _pAttibutesSet->AttributesGetByIndex( i, &cbsName, &cbsValue );
				if( FAILED( hr ) )
					continue;

				if( _eUpdateType == eMUT_AddNew && m_pNodeThis->HaveAttribute( cbsName ) )
					continue;

				m_pNodeThis->SetAttribute( cbsName, cbsValue );
			}

			return S_OK;
		}

		
		// [helpstring)("Get attribute information count")] 
		STDMETHOD(AttributesInfoGetCount)( /*[out]*/ int* _pnCount )
		{
			if( !_pnCount )
				return E_POINTER;

			m_pNodeThis->GetInfo( _pnCount );
// 			LPCWSTR pszInfoNode = m_pNodeThis->m_bAttributesInfo && m_pNodeThis->m_ppszHiddenNodes ? m_pNodeThis->m_ppszHiddenNodes[0] : L"attributes_info";
// 			TNodePtr pInfo = m_pNodeThis->GetNodePtr( pszInfoNode );
// 			if( pInfo )
// 				nCount += pInfo->GetNodesCount();
// 
// 			if( !pInfo || pInfo->GetBoolAttribute(L"inherit", true ) )
// 			{
// 				TNodePtr pParent = m_pNodeThis->GetParentNode();
// 				if( pParent )
// 					pInfo = pParent->GetNodePtr( pszInfoNode );
// 				
// 				if( pInfo )
// 					nCount += pInfo->GetNodesCount();
// 			}
// 
// 			if( _pnCount )
// 				*_pnCount = nCount;

			return *_pnCount > 0 ? S_OK : S_FALSE;
		}

		// [helpstring)("Get attribute information by index")] 
		STDMETHOD(AttributesInfoGetByIndex)( /*[in]*/ int _nIndex, /*[out]*/ BSTR* _pbsName )
		{
			if( _nIndex < 0 )
				return E_INVALIDARG;

			if( !_pbsName )
				return E_POINTER;

			TNodePtr pInfo = m_pNodeThis->GetInfo( NULL, &_nIndex );
			ATLASSERT( _nIndex >= 0 );
			if( !pInfo || _nIndex > pInfo->GetRealNodesCount() )
				return HRESULT_FROM_WIN32( ERROR_INVALID_INDEX );

			TNodePtr pAttrDesc = pInfo->GetRealNodeAt(_nIndex);
			if( !pAttrDesc )
				return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);

			pAttrDesc->GetName( _pbsName );

			return S_OK;

			
// 			LPCWSTR pszInfoNode = m_pNodeThis->m_bAttributesInfo && m_pNodeThis->m_ppszHiddenNodes ? m_pNodeThis->m_ppszHiddenNodes[0] : L"attributes_info";
// 
// 			TNodePtr pInfo = m_pNodeThis->GetNodePtr( pszInfoNode );
// 			if( !pInfo || pInfo->GetBoolAttribute(L"inherit", true ) )
// 			{
// 				TNodePtr pParent = m_pNodeThis->GetParentNode();
// 				if( pParent )
// 				{
// 					TNodePtr pInfo = pParent->GetNodePtr( pszInfoNode );
// 					if( pInfo )
// 					{
// 						if( _nIndex < pInfo->GetNodesCount() )
// 						{
// 							TNodePtr pAttrDesc = pInfo->GetNodeAt(_nIndex);
// 							if( pAttrDesc )
// 								pAttrDesc->GetName( _pbsName );
// 
// 							return pAttrDesc ? S_OK : E_FAIL;
// 						}
// 
// 						_nIndex -= pInfo->GetNodesCount();
// 					}
// 				}
// 			}
// 
// 			
// 			if( pInfo )
// 			{
// 				if( _nIndex < pInfo->GetNodesCount() )
// 				{
// 					TNodePtr pAttrDesc = pInfo->GetNodeAt(_nIndex);
// 					if( pAttrDesc )
// 						pAttrDesc->GetName( _pbsName );
// 
// 					return pAttrDesc ? S_OK : E_FAIL;
// 				}
// 
// 				_nIndex -= pInfo->GetNodesCount();
// 			}
			
//			return HRESULT_FROM_WIN32(ERROR_INVALID_INDEX);
		}

		// [helpstring)("Get attribute information )(see eMInfoType for details)")] 
		STDMETHOD(AttributesInfoGet)( /*[in]*/ BSTR _bsName, /*[in]*/ eMInfoType _eType, /*[out]*/ BSTR* _pbsInfo )
		{
			// 				eMIT_Help,		// Help string	
			// 				eMIT_Type,		// Atrribute type (int, double, string, flags, option)
			// 				eMIT_Default,   // Default value
			// 				eMIT_Values,	// Allowed values - comma separated  (for flags and options)
			// 				eMIT_Min,		// Minimum value (for int, double)
			// 				eMIT_Max		// Maximum value (for int, double)
			// 				eMIT_Dynamic	// Dynamic value (need to be update - usual for options/option_fixed)

			if( !_bsName || !_bsName[0] )
				return E_INVALIDARG;

			const LPCWSTR ppszInfoStr[] = { NULL, L"type", L"default", L"values", L"min", L"max", L"dynamic" };
			if( (int)_eType < 0 || (int)_eType >= SIZEOF_ARRAY(ppszInfoStr) )
				return E_INVALIDARG;

			TNodePtr pInfo;
			TNodePtr pDescAttr = m_pNodeThis->GetAttributeInfo( _bsName, &pInfo );

			// Get info
			CComBSTR cbsValue;
			if( pDescAttr )
			{
				// CAutoRWL rw_lock(pDescAttr->m_rwNodes, true);

				if( !ppszInfoStr[_eType] )
					pDescAttr->GetValue( 0, &cbsValue );
				else
				{
					cbsValue = pDescAttr->GetStringAttribute_Safe( ppszInfoStr[_eType] );
					if( !cbsValue && _eType == eMIT_Values )
					{
						CComBSTR cbsSelFrom = pDescAttr->GetStringAttribute_Safe( L"select_from" );
						if( cbsSelFrom && !pDescAttr->GetRealNodesCount() )
						{
							pDescAttr = pInfo->GetNodePtr( cbsSelFrom );
							if( pDescAttr )
							{
								cbsValue = pDescAttr->GetStringAttribute_Safe( ppszInfoStr[_eType] );
							}
						}
						
						if( !cbsValue && pDescAttr )
						{
							for( int i = 0 ; i < pDescAttr->GetRealNodesCount(); i++ )
							{
								TNodePtr pNode = pDescAttr->GetRealNodeAt(i);
								//ATLASSERT(pNode);
								if (pNode)
								{
									cbsValue.Append(pNode->GetName());
									cbsValue.Append(L"|"); // Append ALWAYS for have '|' for correct split string
								}
							}
						}
					}
				}

				cbsValue.CopyTo(_pbsInfo);
			}

			return cbsValue ? S_OK : S_FALSE;
		}

		STDMETHOD(AttributesValuesInfoGet)( /*[in]*/ BSTR _bsName, /*[in]*/ BSTR _bsOption, /*[in]*/ BSTR _bsType, /*[out]*/ BSTR* _pbsInfo )
		{
			if( !_bsName || !_bsName[0] || !_bsOption || !_bsOption[0] )
				return E_INVALIDARG;

			TNodePtr pAttrNode = m_pNodeThis->GetAttributeOptionInfo( _bsName, _bsOption );
			if( pAttrNode )
			{
				CComBSTR cbsInfo;

				if( !_bsType || !_bsType[0] )
					pAttrNode->GetValue( 0, &cbsInfo );
				else
					cbsInfo = pAttrNode->GetStringAttribute_Safe( _bsType );

				return cbsInfo.CopyTo(_pbsInfo);
			}

			return S_FALSE; // For C#
		}
	};

	template <class T, class TNodePtr, class TInterface = IMNode>
	class IMNodeImpl
		: public IMAttributesImpl<T,TNodePtr,TInterface>
	{
	public:

		IMNodeImpl( T* _pThisNode = NULL )
			: IMAttributesImpl( _pThisNode )
		{
		}

		// [helpstring)("Get node name")] 
		STDMETHOD(NodeNameGet)( /*[out]*/ BSTR* _pbsName )
		{
			if( !_pbsName )
				return E_POINTER;

			m_pNodeThis->GetName(_pbsName);

			return S_OK;
		}

		// [helpstring)("Set node name")] 
		STDMETHOD(NodeNameSet)( /*[in]*/ BSTR _bsName )
		{
			if( !_bsName || !_bsName[0] )
				return E_INVALIDARG;

			m_pNodeThis->ChangeName( _bsName );
			return S_OK;
		}

		// [helpstring)("Get node value")] 
		STDMETHOD(NodeValueGet)( /*[out]*/ BSTR* _pbsValue )
		{
			if( !_pbsValue )
				return E_POINTER;

			int nHiddenNode = 0;
			if( m_pNodeThis->m_ppszLastNodes && (nHiddenNode = m_pNodeThis->IsNames( m_pNodeThis->m_ppszHiddenNodes ) ) >= 0 )
			{
				// ??? Not understanf this block

				if( !m_pNodeThis->GetNodesCount() )
					*_pbsValue = CComBSTR("").Detach();
				else
				{
					CComBSTR cbsName;
					m_pNodeThis->GetName( &cbsName );

					CComBSTR cbsContent;
					m_pNodeThis->SaveXML( &cbsContent );
					if( !cbsContent )
						return E_UNEXPECTED;

					LPCWSTR pszStart = XMLUtils::StrChrW_Quotes( cbsContent, L'>' );
					LPCWSTR pszEnd = StrRChrW( cbsContent, NULL, L'<' );
					if( !pszEnd || !pszEnd )
						return E_UNEXPECTED;

					*_pbsValue = ::SysAllocStringLen( pszStart + 1, pszEnd - pszStart - 1 );
				}
			}
			else
			{
				m_pNodeThis->GetValue( 0, _pbsValue );
			}

			return S_OK;
		}

		// [helpstring)("Set node value")] 
		STDMETHOD(NodeValueSet)( /*[in]*/ BSTR _bsValue )
		{
			if( m_pNodeThis->m_ppszLastNodes && m_pNodeThis->IsNames( m_pNodeThis->m_ppszHiddenNodes ) >= 0 )
			{
				if( !_bsValue || !_bsValue[0] )
				{
					m_pNodeThis->ClearNodes();
				}
				else
				{
					CBlobNew blobContent;
					blobContent.AllocMem( lstrlenW(_bsValue) * 2 + 64 );
					LPWSTR pszContent = (LPWSTR)blobContent.Ptr();
					wsprintfW( pszContent, L"<temp>%ws</temp>", _bsValue );

					TNodePtr pNode;
					T::CreateNode( &pNode, L"temp");
					HRESULT hr = pNode->ParseXML( pszContent, NULL );
					if( FAILED( hr ) )
						return hr;

					m_pNodeThis->ClearNodes();
					pNode->CopyTo( (T*)this, false, 0 );
				}
			}
			else
			{
				m_pNodeThis->SetValue( _bsValue );
			}

			return S_OK;
		}

		// [helpstring)("Get node full path )(optionally you can specify the root node - this is node from which one you can reach current via path)")] 
		STDMETHOD(NodePathGet)( /*[out]*/ BSTR* _bsPath, /*[in]*/ IMNode* _pRoot )
		{
			m_pNodeThis->GetFullName( _bsPath, NULL, _pRoot );
			return S_OK;
		}

		// [helpstring)("Get node parent )(if have)")] 
		STDMETHOD(NodeParentGet)( /*[out]*/ IMNode** _ppParent )
		{
			TNodePtr pNode( m_pNodeThis->m_pParentNode );
			if( !pNode )
				return S_FALSE;

			return pNode.QueryInterface( _ppParent );
		}

		// [helpstring)("Destroy this node and remove it's from parents list")] 
		STDMETHOD(NodeDetach)()
		{
			TNodePtr pNode( m_pNodeThis->m_pParentNode );
			if( !pNode )
				return S_FALSE;

			m_pNodeThis->Clear();

			pNode->RemoveNodePtr( (T*)this );

			return S_OK;
		}

		// [helpstring)("Get nodes count")] 
		STDMETHOD(NodeGetCount)( /*[out]*/ int* _pnCount )
		{
			if( !_pnCount )
				return E_POINTER;

			if( m_pNodeThis->m_ppszLastNodes && m_pNodeThis->IsNames( m_pNodeThis->m_ppszHiddenNodes ) >= 0 )
				*_pnCount = 0;
			else
				*_pnCount = m_pNodeThis->GetRealNodesCount( m_pNodeThis->m_ppszHiddenNodes );

			return S_OK;
		}

		// [helpstring)("Get node by it's index")] 
		STDMETHOD(NodeGetByIndex)( /*[in]*/ int _nIndex, /*[out]*/ BSTR* _pbsName, /*[out]*/ IMNode** _ppNode )
		{
			TNodePtr pNode = m_pNodeThis->GetRealNodeAt( _nIndex, m_pNodeThis->m_ppszHiddenNodes );
			if( !pNode )
				return HRESULT_FROM_WIN32( ERROR_INVALID_INDEX );

			if( _pbsName )
				pNode->GetName( _pbsName );

			if( _ppNode )
				pNode.QueryInterface( _ppNode );

			return S_OK;
		}

		// [helpstring)("Add new node by path - the middle nodes will be created.")] 
		STDMETHOD(NodeAdd)( /*[in]*/ BSTR _bsPath, /*[in]*/ BSTR _bsValue, /*[out]*/ IMNode** _ppNode )
		{
			if( !_bsPath || !_bsPath[0] )
				return E_INVALIDARG;

			TNodePtr pNode = m_pNodeThis->AddNodePtr( _bsPath, _bsValue );
			if( !pNode )
				return E_FAIL;

			if( _ppNode )
				pNode.QueryInterface( _ppNode );

			return S_OK;
		}

		// [helpstring)("Get existing or create new node - the middle nodes will be created")] 
		STDMETHOD(NodeGet)( /*[in]*/ BSTR _bsPath, /*[in]*/ BOOL _bCreateNew, /*[out]*/ IMNode** _ppNode )
		{
			if( !_bsPath || !_bsPath[0] )
				return E_INVALIDARG;

			TNodePtr pNode = m_pNodeThis->GetNodePtr( _bsPath, _bCreateNew ? true : false );
			if( !pNode )
				return HRESULT_FROM_WIN32(ERROR_NOT_FOUND);

			if( _ppNode )
				pNode.QueryInterface( _ppNode );

			return S_OK;
		}

		// [helpstring)("Remove child node")] 
		STDMETHOD(NodeRemove)( /*[in]*/ BSTR _bsPath )
		{
			if( !_bsPath || !_bsPath[0] )
				return E_INVALIDARG;

			return m_pNodeThis->RemoveNode( _bsPath ) ? S_OK : S_FALSE;
		}

		// [helpstring)("Remove all child nodes, values and )(optionally) atibutes")] 
		STDMETHOD(NodeRemoveAll)( /*[in]*/ BOOL _bRemoveAttributes )
		{
			m_pNodeThis->ClearNodes();
			if( _bRemoveAttributes )
				m_pNodeThis->ClearAttributes();

			return S_OK;
		}

		// [helpstring)("Parse XML string to this node")] 
		STDMETHOD(NodeParse)( /*[in]*/ BSTR _pbsXMLString, /*[in]*/ BOOL _bUpdateName )
		{
			CComBSTR cbsName;
			if( !_bUpdateName )
				m_pNodeThis->GetName( &cbsName );

			HRESULT hr = m_pNodeThis->ParseXML( _pbsXMLString, NULL );
			if( FAILED( hr ) )
				return hr;

			if( cbsName.Length() )
				m_pNodeThis->ChangeName( cbsName );

			return S_OK;
		}

		// [helpstring)("Save this node as XML string")] 
		STDMETHOD(NodeSave)( /*[out]*/ BSTR* _pbsXMLString )
		{
			if( !_pbsXMLString )
				return E_POINTER;

			return m_pNodeThis->SaveXML( _pbsXMLString );
		}

		// [helpstring)("Clone node")] 
		STDMETHOD(NodeClone)( /*[out]*/ IMNode** _ppClone )
		{
			if( !_ppClone )
				return E_POINTER;

			TNodePtr pNode;
			T::CreateNode( &pNode, L"clone");
			m_pNodeThis->CopyTo( pNode );

			return pNode.QueryInterface( _ppClone );
		}

		// [helpstring)("Set node")] 
		STDMETHOD(NodeSet)( /*[in]*/ IMNode* _pNodeSet, /*[in]*/ eMUpdateType _eUpdateType )
		{
			return E_NOTIMPL;
		}

		STDMETHOD(NodeInfoGet)( /*[in]*/ BSTR _bsInfoType, /*[out]*/ BSTR* _pbsDecription)
		{
			if( !_pbsDecription )
				return E_POINTER;

			TNodePtr pInfo;
			TNodePtr pCommonInfo = m_pNodeThis->GetObjectR<TNodePtr>( L"attributes_info" );
			if( pCommonInfo && (pInfo = pCommonInfo->GetNodePtr( m_pNodeThis->GetName(NULL, true) ) ) )
			{
				if( !_bsInfoType || !_bsInfoType[0] )
				{
					pInfo->GetValue( 0, _pbsDecription );
					return S_OK;
				}


				// Check nested (most priority - top)
				while( pInfo )
				{
					CComBSTR cbsInfo = pInfo->GetStringAttribute_Safe( _bsInfoType );
					if( cbsInfo )
					{
						*_pbsDecription = cbsInfo.Detach();
						return S_OK;
					}

					// Check inherited types
					CComBSTR pszInherit = pInfo->GetStringAttribute_Safe( L"inherit" );
					if( pszInherit )
						pInfo = pCommonInfo->GetNodePtr( pszInherit );
					else
						break;
				}
			}

			// Return S_FALSE for easy use in C#
			return S_FALSE;
		}

	};

	class CoXMLNode:
		public CComObjectRootEx<CComMultiThreadModel>,
		public CComCoClass<CoXMLNode>,
		public XMLNodeImpl<CoXMLNode, XMLNodePtr>,
		public IMNodeImpl<CoXMLNode,XMLNodePtr,IMNode>

	{
	public:

		CoXMLNode()
			: IMNodeImpl( this )
		{
		}

		DECLARE_GET_CONTROLLING_UNKNOWN()

		DECLARE_PROTECT_FINAL_CONSTRUCT()

		DECLARE_AGGREGATABLE(CoXMLNode)

		BEGIN_COM_MAP(CoXMLNode)
			COM_INTERFACE_ENTRY(IMNode)
			COM_INTERFACE_ENTRY(IMAttributes)
		END_COM_MAP()

		

	};

#else
	
class CoXMLNode;

class XMLNodePtr: public CAutoObj<CoXMLNode>
{
public:
	XMLNodePtr()
	{
		p = NULL;
	}

	~XMLNodePtr()
	{
		if(p)
			p->Release();
		p = NULL;
	}

	// 		XMLNodePtr( LPCWSTR _pszNodeName, CoXMLNode* pParent )
	// 		{
	// 			CoXMLNode::CreateNode( &p, _pszNodeName );
	// 		}

	static HRESULT CreateNode( LPCWSTR _pszNodeName, CoXMLNode* pParent, CoXMLNode** _ppNode, LPCWSTR _pszSwapID = NULL )
	{
		return CoXMLNode::CreateNode( _ppNode, _pszNodeName, pParent, _pszSwapID );
	}

	XMLNodePtr(LPCWSTR _pszParametets, eCreateNodeParam _eParamType)
	{
		CoXMLNode::CreateNode(&p, _eParamType == eCNP_BasicNode ? _pszNodeName : L"node_param", NULL, NULL);
		if (_eParamType == eCNP_ParseAttributes)
			p->ParseAttributes(_pszParametets, NULL, false, 2, false);
		else if (_eParamType == eCNP_ParseAttributes_Comlpex)
			p->ParseAttributes(_pszParametets, NULL, false, 2, false);
		else if (_eParamType == eCNP_ParseXML)
			p->ParseXML(_pszParametets, NULL, false, false, false, false);
	}


	XMLNodePtr( LPCWSTR _pszNodeName, CoXMLNode* pParent, LPCWSTR _pszSwapID = NULL)
	{
		CoXMLNode::CreateNode( &p, _pszNodeName, pParent, _pszSwapID );
	}

	explicit XMLNodePtr(CoXMLNode* pT)
	{
		p = pT;
		if(p)
			p->AddRef();
	}

	XMLNodePtr operator [] (int _nIndex )
	{
		ATLASSERT( p );
		if( p )
			return p->GetNodeAt(_nIndex);

		return (XMLNodePtr)NULL;
	}

	XMLNodePtr(const XMLNodePtr& apT)
	{
		p = apT.p;
		if(p)
			p->AddRef();
	}
};

class CoXMLNode:
	public AutoObjBase<CoXMLNode>,
	public XMLNodeImpl<CoXMLNode, XMLNodePtr>
{
};

#endif

	
	/*

// Access: video, video::line, audio::format etc.

<m-device>
	<video name='Decklink HD'>
		<device name='Decklink HD' moniker='{....}'>
			<line name='SDI'>
				<line name='SDI'/>
				<line name='HDMI'/>
				<line name='Y,Cb,Cr'/>
			</line>

			<format>
				<format name='720x576@50i' interlace='top'/>
				<format name='720x576@50i'/>
			</format>

			<colorspace>
				<colorspace name='RGB32'/>
				<colorspace name='UYVY'/>
				...
			</colorspace>
		</device>
	</video>
	<audio name='Realtek'>
		<device name='<From Video>' moniker='{....}'/>
		<device name='Realtek'/>
			<format>
			</format>
		</device>
		<device name='Decklink HD' moniker='{....}'>
			<line>
				<line name='SDI'/>
				<line name='HDMI'/>
				<line name='XLR Analog'/>
			</line>
		</device>
	</audio>
</m-device>
*/
		

		

// 	static LPCWSTR GetNodeAttribute( LPCWSTR _wszName, LPCWSTR _pszTopNode, BSTR* _pbsNode = NULL, int nCheckNodes = 1 )
// 	{
// 		CSimpleArray2<SNodeInfo> arrNames;
// 		CSimpleArray2<LPCWSTR> arrParts;
// 		XMLParse::AttName2Array( _wszName, arrNames, true, &arrParts );
// 
// 		if( !arrNames.GetSize() ) return NULL;
// 
// 		if( _pszTopNode )
// 		{
// 			CSimpleArray2<SNodeInfo> arrNodeNames;
// 			if( StrStrW( _pszTopNode, L"::" ) )
// 			{
// 				XMLParse::AttName2Array( _pszTopNode, arrNodeNames, true );
// 			}
// 			else
// 			{
// 				arrNodeNames.Add( SNodeInfo(_pszTopNode,0) );
// 			}
// 
// 			if( arrNodeNames.GetSize() >= arrNames.GetSize() )
// 				return NULL;
// 
// 			for( int i = 0; i < arrNodeNames.GetSize(); i++ )
// 			{
// 				if( arrNames[i].cbsName != arrNodeNames[i].cbsName ||
// 					arrNames[i].nIndex != arrNodeNames[i].nIndex )
// 					return NULL;
// 			}
// 
// 			ATLASSERT( arrNodeNames.GetSize() < arrParts.GetSize() );
// 			return arrParts[ arrNodeNames.GetSize() ];
// 		}
// 		else if( nCheckNodes < arrParts.GetSize() )
// 		{
// 			if( _pbsNode )
// 			{
// 				CComBSTR cbsNode;
// 				for( int i = 0; i < nCheckNodes; i++ )
// 				{
// 					cbsNode.Append( arrNames[i].cbsName );
// 					if( i + 1 < nCheckNodes )
// 						cbsNode.Append( L"::" );
// 				}
// 
// 				*_pbsNode = cbsNode.Detach();
// 			}
// 
// 			return arrParts[ nCheckNodes ];
// 		}
// 
// 		return NULL;
// 	}



	// 	static inline void CorrectNames( XMLNode& _rNode )
	// 	{
	// 		_rNode->cbsName = CorrectName( _rNode->cbsName );
	// 		_rNode->cbsValue = CorrectName( _rNode->cbsValue );
	// 		for( int i = 0; i < _rNode->mapAttrib.GetSize(); i++ )
	// 		{
	// 			_rNode->mapAttrib.GetKeyAt(i) = CorrectName( _rNode->mapAttrib.GetKeyAt(i) );
	// 			//_rNode->mapAttrib.GetValueAt(i) = CorrectName( _rNode->mapAttrib.GetValueAt(i) );
	// 		}
	// 
	// 		for( int i = 0; i < _rNode->m_mapNodes.GetSize(); i++ )
	// 		{
	// 			_rNode->m_mapNodes.GetKeyAt(i) = CorrectName( _rNode->m_mapNodes.GetKeyAt(i) );
	// 
	// 			CorrectNames( _rNode->m_mapNodes.GetValueAt(i) );
	// 		}
	// 	}




	// Swap names
	// { L"id", L"type", L"def-type", NULL };
	// <block-000 type=block...> -> <block id=block-000>
	// <block-000 ...> -> <def-type id=block-000>
template <class T>
	static HRESULT SaveXML( T pXMLNode, BSTR* _pbsContent, int _nTabs = 0, int _nHTMLType = 0 )
	{
		return pXMLNode->SaveXML( _pbsContent, _nTabs, _nHTMLType );
	}


	// Save node
	// <tag>value</tag>
	// <tag attr='xxx'>value</tag>
	// <tag attr='xxx'>
	//		value-1
	//		<next>XXXX</next>
	//		value-x
	// </tag>
	// Exception
	template <class T>
	static HRESULT SaveXML( T pXMLNode, IStream* _pStream, int nTabs, bool _bPutEOS = true, int _nHTMLType = 0)
	{
		return pXMLNode->SaveXML( _pStream, nTabs, _bPutEOS, _nHTMLType );	
	}
	
	template <class T>
	static HRESULT ParseXML( LPCWSTR pszStart, LPCWSTR* ppszEnd, T pXMLNode, bool _bComplex = false, bool _bKeepBlank = false)
	{
		return pXMLNode->ParseXML( pszStart, ppszEnd, _bComplex, _bKeepBlank );
	}
// 	
// 		USES_CONVERSION;
// 
// 		if( !pszStart ) return E_INVALIDARG;
// 		LPCWSTR pEnd = ppszEnd && *ppszEnd ? *ppszEnd : pszStart + lstrlenW(pszStart);
// 
// 		ATLASSERT( pXMLNode );
// 		if( !pXMLNode ) return E_POINTER;
// 			
// 
// 		CComBSTR cbsOpenTag;
// 		LPCWSTR pStr = pszStart;
// 		int nTagType = 0;
// 		if( !_bComplex )
// 		{
// 			CComBSTR cbsParam;
// 			LPCWSTR pNodeStart = pszStart;
// 			while( pNodeStart )
// 			{
// 				// < tag />
// 				LPCWSTR pTagEnd = pEnd;
// 				pNodeStart = XMLUtils::NextTag( pStr, &pTagEnd, cbsOpenTag, &cbsParam, &nTagType );
// 				pStr = pTagEnd;
// 
// 				// Check for </tag> -> IGNORE
// 				if( nTagType >= 0 ) break; 
// 			}
// 			if( !pNodeStart )
// 			{
// 				ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("###Err### [XMLParse] ParseXML - TOP NODE NOT FOUND:'%ws'\n"), pStr );
// 				return E_INVALIDARG;
// 			}
// 
// 			ATLASSERT( cbsOpenTag.Length() );
// 			pXMLNode->ChangeName( cbsOpenTag );		
// 
// 			if( nTagType == 2 )
// 			{
// 				// <![CDATA[Some Text <TEXT xxx>]]>
// 				if( ppszEnd )
// 					*ppszEnd = pStr;
// 
// 				pXMLNode->AddValue( XMLUtils::XML2Value( cbsParam ) );	
// 
// 
// 				ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("[XMLParse] ParseXML - SIMPLE TAG:'%ws'\n"), pXMLNode->GetName() );
// 				return S_OK;
// 			}
// 
// 			if( cbsParam )
// 			{
// 				ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("[XMLParse] ParseXML - TAG NAME:'%ws' ATTR:'%ws'\n"), pXMLNode->GetName(), cbsParam );
// 				pXMLNode->ParseAttributes( cbsParam, NULL);
// 			}
// 			else
// 			{
// 				ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("[XMLParse] ParseXML - TAG NAME:'%ws' NO ATTR\n"), pXMLNode->GetName() );
// 			}
// 
// 			if( nTagType == 0 )
// 			{
// 				// <tag ... />
// 
// 				if( ppszEnd )
// 					*ppszEnd = pStr;
// 
// 				ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("[XMLParse] ParseXML - SIMPLE TAG:'%ws'\n"), pXMLNode->GetName() );
// 				return S_OK;
// 			}
// 		}
// 
// 		while( pStr < pEnd )
// 		{
// 			CComBSTR cbsNextTag;
// 			CComBSTR cbsParamInner;
// 			LPCWSTR pTagEnd = pEnd;
// 			LPCWSTR pNodeStart = XMLUtils::NextTag( pStr, &pTagEnd, cbsNextTag, &cbsParamInner, &nTagType );
// 			if( !pNodeStart )
// 			{
// 				if( pTagEnd == pEnd )
// 				{
// 					ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("!!!WARNING!!! [XMLParse] ParseXML - NOT FOUND CLOSE TAG FOR:'%ws'\n"), pXMLNode->GetName() );
// 
// 					pNodeStart = pEnd; // For put value
// 				}
// 				else
// 				{
// 					ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("!!!WARNING!!! [XMLParse] ParseXML - WRONG SYNTAX:'%ws'\n"), pStr );
// 
// 					pStr = pTagEnd;
// 					continue;
// 				}
// 			}
// 
// 			
// 			if( pNodeStart > pStr )
// 			{
// 				LPCWSTR pValueStart = _bKeepBlank ? pStr : XMLUtils::SkipBlank( pStr, true );
// 				LPCWSTR pValueEnd = _bKeepBlank ? pNodeStart - 1 : XMLUtils::SkipBlankRev( pNodeStart - 1, true );
// 				if( pValueEnd >= pValueStart )
// 				{
// 					CComBSTR cbsValue( pValueEnd - pValueStart + 1, pValueStart );
// 
// 					// Check for the empty value (all space) here, if empty -> NOT add
// 					LPCWSTR pszValue = XMLUtils::XML2Value( cbsValue, _bKeepBlank );
// 					if( pszValue )
// 						pXMLNode->AddValue( pszValue );
// 
// 					ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("[XMLParse] ParseXML - Element '%ws' NEXT Value:'%ws'\n"), cbsOpenTag, pszValue );		
// 				}
// 			}
// 
// 			if( nTagType < 0 )
// 			{
// 				// Check for current tag 
// 				// NOTE: The name could be changes -> use cbsOpenTag
// 				if( cbsNextTag == cbsOpenTag )
// 				{
// 					ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("[XMLParse] ParseXML - CLOSE TAG FOR:'%ws'\n"), cbsOpenTag );
// 					if( ppszEnd )
// 						*ppszEnd = pTagEnd;
// 
// 					break;
// 				}
// 
// 				ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("[XMLParse] ParseXML - UNPROPER CLOSE TAG:'%ws' - SKIP THIS\n"), cbsOpenTag );
// 
// 				// Skip this tag as unknown
// 				pStr = pTagEnd;
// 			}
// 			else if( nTagType == 2 )
// 			{
// 				pXMLNode->AddValue( XMLUtils::XML2Value( cbsParamInner ) );
// 
// 				ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("[XMLParse] ParseXML - Element '%ws' NEXT CDATA Value:'%ws'\n"), cbsOpenTag, cbsParamInner );		
// 
// 				pStr = pTagEnd;
// 			}
// 			else if( pNodeStart < pEnd )
// 			{
// 				XMLNodePtr pChildNode = pXMLNode->AddNodePtr(L"__temp__");
// 				ATLASSERT( pChildNode );
// 				if( !pChildNode ) return E_UNEXPECTED;
// 
// 				LPCWSTR pNodeEnd = pEnd;
// 				if( ParseXML( pNodeStart, &pNodeEnd, pChildNode, false ) != S_OK )
// 				{
// 					ATLTRACE2( atlTraceGeneral, TRACELEVEL_XMLParse, _T("[XMLParse] ParseXML - ERROR PARSE:'%ws' - SKIP THIS\n"), pNodeStart );
// 
// 					// Error - skip unknown or wrong tag
// 					pXMLNode->RemoveNodePtr( pChildNode );
// 					
// 					pStr = pTagEnd;
// 				}
// 				else
// 				{
// 					pStr = pNodeEnd;
// 				}
// 			}
// 			else
// 			{
// 				if( ppszEnd )
// 					*ppszEnd = pEnd;
// 
// 				break;
// 			}
// 		}
// 
// 
// 		return S_OK;
//	}

template <class T>
	static HRESULT SaveToFile( T pXMLNode, LPCWSTR _wszFileName, bool _bUnicode, BSTR* _pbsContent = NULL, int _nHTMLType = 0, LPCWSTR _pszFileHeader = NULL )
	{
		CComBSTR cbsDesc;
		HRESULT hr = SaveXML( pXMLNode, &cbsDesc, 0, _nHTMLType );
		if( FAILED( hr ) ) return hr;

		HANDLE hFile = NULL;
		if( _wszFileName && _wszFileName[0] )
		{
			hFile = ::CreateFileW( _wszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL );
			if( !hFile ) return HRESULT_FROM_WIN32( GetLastError() );
		}

		if( hFile )
		{
			int nChars = cbsDesc.Length();
			if( !nChars ) return E_UNEXPECTED;

			if( !_bUnicode )
			{
				DWORD dwWritten = 0;
				if( _pszFileHeader && _pszFileHeader[0] )
				{
					USES_CONVERSION;

					LPCSTR pszStr = W2CA(_pszFileHeader);
					::WriteFile( hFile, pszStr, lstrlenA(pszStr), &dwWritten, NULL );
				}

				CBlobNew blobNew;
				blobNew.AllocMem( cbsDesc.Length() + 2 );
				char* szFile = (char*)blobNew.Ptr();
				::WideCharToMultiByte(CP_ACP, 0, cbsDesc.m_str, -1, szFile, nChars + 1, NULL, NULL);

				::WriteFile( hFile, szFile, nChars, &dwWritten, NULL );
			}
			else
			{
				DWORD dwWritten = 0;
				if( _pszFileHeader && _pszFileHeader[0] )
				{
					::WriteFile( hFile, _pszFileHeader, lstrlenW(_pszFileHeader), &dwWritten, NULL );
				}

				::WriteFile( hFile, cbsDesc.m_str, nChars * sizeof(WCHAR), &dwWritten, NULL );
			}

			::CloseHandle(hFile);
		}

		if( _pbsContent ) *_pbsContent = cbsDesc.Detach();

		return S_OK;
	}

	template <class T>
	static HRESULT SaveToRegistry_I( T pXMLNode, HKEY hKeyParent, LPCWSTR lpszKeyName, LPCWSTR _pszNodePrefix = NULL )
	{
		USES_CONVERSION;

		ATLASSERT( pXMLNode );
		if( !pXMLNode ) return E_POINTER;

		CRegKey keyNode;
		if( keyNode.Create( hKeyParent, W2CT(lpszKeyName) ) != ERROR_SUCCESS )
		{
			return HRESULT_FROM_WIN32( GetLastError() );
		}

		CComBSTR cbsValue;
		pXMLNode->GetValue( 0, &cbsValue );
		if( cbsValue.Length() )
		{
			keyNode.SetStringValue( NULL, W2T(cbsValue) );
		}

		for( int i = 0; i < pXMLNode->GetAttributesCount(); i++ )
		{
			CComBSTR cbsKey = _pszNodePrefix;
			if( cbsKey.Length() )
				cbsKey.Append( L"." );

			cbsKey.Append( pXMLNode->GetAttributeName( i ) );

			// Check for integer
			CComBSTR cbsValue = pXMLNode->GetAttributeValue(i);
			if( !cbsValue.Length() )
			{
				keyNode.SetStringValue( W2T(cbsKey ), _T(""), REG_SZ );
			}
			else
			{
				int nValue = _wtoi(cbsValue); // DWORD for reg
				LPWSTR pEnd = 0;
				LONGLONG n64Value = _wcstoui64( cbsValue, &pEnd, 0 );
				CComBSTR cbsIntValue = INT2STRW(nValue);
				CComBSTR cbsN64Value = HEX64STRW(n64Value);
				if( cbsValue == cbsIntValue && nValue >= 0 )
				{
					keyNode.SetDWORDValue( W2T( cbsKey ), nValue );
				}
				else if( cbsValue == cbsN64Value && n64Value >= 0 )
				{
					keyNode.SetQWORDValue( W2T(cbsKey ), n64Value );
				}
				else
				{
					keyNode.SetStringValue( W2T(cbsKey ), W2T(cbsValue), REG_SZ );
				}
			}

		}

		if( _pszNodePrefix )
		{
			for( int i = 0; i < pXMLNode->GetNodesCount(); i++ )
			{
				if( pXMLNode->GetNodeAt(i)->IsValue() )
					continue;

				CComBSTR cbsKey = _pszNodePrefix;
				if( cbsKey.Length() )
					cbsKey.Append( L"." );

				cbsKey.Append( pXMLNode->GetNodeAt(i)->GetName()  );

				CComBSTR cbsValue = pXMLNode->GetNodeAt(i)->GetValue();

				if( cbsValue.Length() )
				{
					int nValue = _wtoi(cbsValue); // DWORD for reg

					LPWSTR pEnd = 0;
					LONGLONG n64Value = _wcstoui64( cbsValue, &pEnd, 0 );
					CComBSTR cbsIntValue = INT2STRW(nValue);
					CComBSTR cbsN64Value = HEX64STRW(n64Value);
					if( cbsValue == cbsIntValue && nValue >= 0 )
					{
						keyNode.SetDWORDValue( W2T( cbsKey ), nValue );
					}
					else if( cbsValue == cbsN64Value && n64Value >= 0)
					{
						keyNode.SetQWORDValue( W2T(cbsKey ), n64Value );
					}
					else
					{
						keyNode.SetStringValue( W2T(cbsKey ), W2T(cbsValue), REG_SZ );
					}
				}

				if( !pXMLNode->GetNodeAt(i)->IsValue() )
					SaveToRegistry_I( pXMLNode->GetNodeAt(i), hKeyParent, lpszKeyName, cbsKey );
			}
		}

		keyNode.Close();

		return S_OK;
	}

	template <class T>
	static HRESULT LoadFromResources( T pXMLNode, UINT resID, LPCWSTR _pszType = L"XML", int _nUnicode = -1 )
	{

		#if (_ATL_VER >= 0x0700)
				HINSTANCE hInstance = ATL::_AtlBaseModule.GetResourceInstance();
		#else	// !(_ATL_VER >= 0x0700)
				HINSTANCE hInstance = _Module.GetResourceInstance();
		#endif	// !(_ATL_VER >= 0x0700)
		
		HRSRC hResource = ::FindResourceW(hInstance, MAKEINTRESOURCE(resID), _pszType);
		if (!hResource)
			return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
	
		HGLOBAL hLoadedResource = LoadResource(hInstance, hResource);
		if (!hLoadedResource)
			return E_FAIL;

		LPBYTE pbLockedResource = (LPBYTE)LockResource(hLoadedResource);
		if (!pbLockedResource)
			return HRESULT_FROM_WIN32( ERROR_FILE_INVALID );
				
		DWORD dwResourceSize = SizeofResource(hInstance, hResource);
		if (!dwResourceSize)
			return HRESULT_FROM_WIN32( ERROR_FILE_INVALID );
					
		CBlobNew blobConvert;
		LPWSTR pszContent = NULL;
		LPCWSTR pszContentEnd = NULL;
			
		if( _nUnicode > 0 )
		{
			int nPos = 0;
			if( (pbLockedResource[0] & 0xFE) == 0xFE && (pbLockedResource[1] & 0xFE) == 0xFE)
				nPos = 2;

			pszContent = (LPWSTR)pbLockedResource;
			pszContentEnd = (LPWSTR)(pbLockedResource + dwResourceSize);

			// TODO: XMLUtils::PrepareForParse(pszContent);
		}
		else
		{
			// TODO: correct correction 
			blobConvert.AllocMem( dwResourceSize * 2 + 2);
			pszContent = (LPWSTR)blobConvert.Ptr();

			if( Text2Wide( pbLockedResource, dwResourceSize, pszContent ) <= 0 )
				return E_INVALIDARG;
		}
		
		XMLUtils::PrepareForParse(pszContent);

		return pXMLNode->ParseXML( pszContent, &pszContentEnd );			
	}

	// IMFConfig for use in MFFactory::FinalConstruct()
	template <class T>
	static HRESULT LoadFromRegistry( T pXMLNode, HKEY hKeyParent, LPCWSTR lpszKeyName, bool _bSetDefaults, bool _bFlat = false, IMFConfig* _pConfig = NULL )
	{
		USES_CONVERSION;

		// Try local settings
		CComBSTR cbsAttributes;
#ifndef MFRAMES_STATIC_FACTORY
		HRESULT hr = S_OK;
		if( _pConfig ) 
			hr = _pConfig->LoadSettings( CComBSTR(lpszKeyName), &cbsAttributes );
		else
			hr = MCreator::LoadSettings( lpszKeyName, &cbsAttributes );
		if( hr != S_FALSE && hr != E_NOINTERFACE ) // Could be HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND ) if settings is empty
		{
			// Local only settings
			pXMLNode->ParseAttributes( cbsAttributes, NULL, false, 1, true );

			if( _bSetDefaults )
			{
				CComBSTR cbsAttributes_Update;
				pXMLNode->SaveAttributes( &cbsAttributes_Update);
				if( _pConfig )
					_pConfig->SaveSettings( CComBSTR(lpszKeyName), cbsAttributes_Update);
				else
					MCreator::SaveSettings( lpszKeyName, cbsAttributes_Update);
			}

			Debug_XML( 2, pXMLNode, L"[LoadFromRegistry(MConfig)]" );

			return S_OK;
		}
#endif

		CRegKey keyNode;
		if( keyNode.Open( hKeyParent, W2CT(lpszKeyName), KEY_READ ) == ERROR_SUCCESS )
		{
			int nIndex = 0;
			WCHAR szXMLName[MAX_PATH*2] = {0};
			DWORD dwSize = SIZEOF_ARRAY(szXMLName);
			DWORD dwType = 0;
			while( ::RegEnumValueW( keyNode.m_hKey, nIndex++, szXMLName, &dwSize, NULL, &dwType, NULL, 0 ) == ERROR_SUCCESS )
			{
				dwSize = SIZEOF_ARRAY(szXMLName);

				if( !szXMLName[0] )
					continue;

				CComBSTR cbsRegName = szXMLName;
				bool bNode = false;
				if( _bFlat )
				{
					bNode = StrChrW( szXMLName, L'.' ) == NULL;
					if( !bNode )
					{
						StrReplaceW( szXMLName, L".", L"::", SIZEOF_ARRAY(szXMLName) );
					}
				}

				if( dwType == REG_DWORD )
				{
					DWORD dwValue = 1;
					keyNode.QueryDWORDValue( W2T(cbsRegName), dwValue );

					if( bNode )
						pXMLNode->SetNodeValue( szXMLName, INT2STRW(dwValue) );
					else
						pXMLNode->SetAttribute( szXMLName, INT2STRW(dwValue), 1, 0 );
				}
				else if( dwType == REG_QWORD )
				{
					ULONGLONG llValue = 1;
					keyNode.QueryQWORDValue( W2T(cbsRegName), llValue );

					if( bNode )
						pXMLNode->SetNodeValue( szXMLName, INT64STRW(llValue) );
					else
						pXMLNode->SetAttribute( szXMLName, INT64STRW(llValue), 1 );
				}
				else
				{
					ULONG ulChars = 0;
					keyNode.QueryStringValue( W2T(cbsRegName), NULL, &ulChars );

					LPTSTR pszValue = (LPTSTR)alloca( (ulChars + 2 )* sizeof(TCHAR) );
					if( keyNode.QueryStringValue( W2T(cbsRegName), pszValue, &ulChars ) == ERROR_SUCCESS )
					{
						if( bNode )
							pXMLNode->SetNodeValue( szXMLName, T2W(pszValue) );
						else
							pXMLNode->SetAttribute( szXMLName, T2W(pszValue), 1 );
					}
				}
			}
		}

		keyNode.Close();

		if( _bSetDefaults )
		{
			SaveToRegistry_I( pXMLNode, hKeyParent, lpszKeyName, _bFlat ? L"" : NULL );
		}

		if( cbsAttributes )
		{
			pXMLNode->ParseAttributes( cbsAttributes, NULL, false, 1, _bFlat );

// 			CComBSTR cbsAttrUpdate;
// 			pXMLNode->SaveAttributes( &cbsAttributes );
// 			MCreator::SaveSettings( lpszKeyName, cbsAttributes );
		}

		Debug_XML( 2, pXMLNode, L"[LoadFromRegistry]" );

		return S_OK;
	}

	

	static HRESULT LoadAuto( LPCWSTR _wszValue, XMLNodePtr pXMLNode )
	{
		return pXMLNode->LoadAuto( _wszValue );
	}

	static HRESULT LoadFromFile( LPCWSTR _wszFileName, XMLNodePtr pXMLNode, int nUnicode = -1 )
	{
		return pXMLNode->LoadFromFile( _wszFileName, -1 );
	}

	//////////////////////////////////////////////////////////////////////////
	// Some primitives

	static HRESULT AttName2Array( LPCWSTR _wszName, CSimpleArray2<CComBSTR>& _arrString, LPCWSTR _pszDelimer = L"::" )
	{
		if( !_wszName ) return E_INVALIDARG;
		// effects::blur
		// effects::blur(1)::size-x
		// font
		// 

		int nDelimerLen = lstrlenW( _pszDelimer );
		LPCWSTR pStr = _wszName;
		LPCWSTR pEnd = _wszName + lstrlenW(_wszName);
		while( pStr < pEnd )
		{
			LPCWSTR pSeparator = StrStrW( pStr, _pszDelimer );
			if( !pSeparator ) pSeparator = pEnd;

			CComBSTR cbsName( pSeparator - pStr, pStr );
			_arrString.Add( cbsName );

			pStr = pSeparator + nDelimerLen;
		}

		return _arrString.GetSize() ? S_OK : S_FALSE;
	}

	static HRESULT Array2AttName( const CSimpleArray2<CComBSTR>& _rArray, BSTR* _pbsRes, LPCWSTR _pszDelimer = L"::" )
	{
		if( !_pbsRes ) return E_POINTER;

		if( !_rArray.GetSize() ) return E_INVALIDARG;

		CComBSTR cbsRes;
		for( int i = 0; i < _rArray.GetSize() ; i++ )
		{
			cbsRes.Append( _rArray[i] );

			if( i < _rArray.GetSize() - 1 )
				cbsRes.Append( _pszDelimer );
		}

		*_pbsRes = cbsRes.Detach();
		return S_OK;
	}


	

	// 	static HRESULT String2Borders( LPCWSTR _wsString, RECT_DBL& _rcBorders )
	// 	{
	// 		RECT rcBorders = {0};
	// 		HRESULT hr = String2Borders( _wsString, rcBorders )
	// 		if( hr == S_OK )
	// 			_rcBorders = rcBorders;
	// 
	// 		return hr;
	// 	}

	template <class TNodePtr>
	static double XMLGetDoubleAvg( LPCWSTR _pszName, 
		TNodePtr _pSrcNode, 
		TNodePtr _pDestNode, 
		double _dblPos, 
		double _dblValueDef = 0 )
	{
		if( _dblPos >= 1.0 )
			return _pDestNode ? _pDestNode->GetDblAttribute( _pszName, _dblValueDef ) : _dblValueDef;

		double dblFirst = _pSrcNode ? _pSrcNode->GetDblAttribute( _pszName, _dblValueDef ) : _dblValueDef;
		if( _dblPos <= 0 )
			return dblFirst;

		double dblSecond = _pDestNode ? _pDestNode->GetDblAttribute( _pszName, _dblValueDef ) : _dblValueDef;

		return dblFirst * (1 - _dblPos) + dblSecond * _dblPos;
	}

	

	template <class TNodePtr>
	static double XMLDoubleInc( LPCWSTR _pszName, 
		TNodePtr _pSrcNode, 
		TNodePtr _pAddNode, 
		bool _bUpdateSrc = false, 
		double _dblValueDef = 0,
		double _dblSegment = 0,
		double _dblScaleValue = 0)
	{
		if( !_pSrcNode ) return _dblValueDef;

		double dblValue = _pSrcNode->GetDblAttribute( _pszName, _dblValueDef );
		double dblAdd = _pAddNode ? _pAddNode->GetDblAttribute( _pszName ) : 0;

		dblValue += dblAdd * _dblScaleValue;

		if( dblAdd != 0 && _dblSegment > 0 )
		{
			// e.g. limit [0...1]
			// -5.5 -> -6
			dblValue -= _dblSegment * (int)(dblValue /_dblSegment);
		}
		else if( dblAdd != 0 && _dblSegment < 0 )
		{
			// e.g. limit [-1...1]
			_dblSegment *= -2;
			dblValue += _dblSegment / 2.0;
			dblValue -= _dblSegment * (int)(dblValue /_dblSegment);  // [0...2]
			dblValue -= _dblSegment / 2.0;		// [-1...1]
		}


		if( _bUpdateSrc && dblAdd != 0 )
		{
			_pSrcNode->SetAttribute( _pszName, dblValue );
		}

		return dblValue;
	}

	template <class TNodePtr>
	static double XMLGetDoubleAdd( LPCWSTR _pszName, TNodePtr _pSrcNode, TNodePtr _pAddNode, double _dblPos, double _dblSrcDef = 0)
	{
		ATLASSERT( _pSrcNode );
		double dblFirst = _pSrcNode->GetDblAttribute( _pszName, _dblSrcDef );
		if( !_pAddNode )
			return dblFirst;

		double dblSecond = _pAddNode->GetDblAttribute( _pszName );

		return dblFirst + dblSecond * _dblPos;
	}
	

	static bool XMLStoreRect( XMLParse::XMLNodePtr _pXMLNode, const RECT& _crRect, SIZE* pSizeBase = NULL )
	{
		if( !_pXMLNode ) return false;

		_pXMLNode->SetUnitAttribute( L"left", _crRect.left, pSizeBase ? pSizeBase->cx : 0 );
		_pXMLNode->SetUnitAttribute( L"right",_crRect.right, pSizeBase ? pSizeBase->cx : 0 );
		_pXMLNode->SetUnitAttribute( L"top", _crRect.top, pSizeBase ? pSizeBase->cy : 0 );
		_pXMLNode->SetUnitAttribute( L"bottom", _crRect.bottom, pSizeBase ? pSizeBase->cy : 0 );

		return true;
	}
	static bool XMLGetRect( XMLParse::XMLNodePtr _pXMLNode, RECT& _rRect, SIZE* pSizeBase  = NULL)
	{
		if( !_pXMLNode ) return false;

		if( pSizeBase )
		{
			RECT_DBL rcRes = {0};
			rcRes = _rRect;
			SIZE_DBL szBase = {(double)pSizeBase->cx, (double)pSizeBase->cy };
			XMLGetRect( _pXMLNode, rcRes, &szBase );
			_rRect = rcRes;
		}
		else
		{
			_rRect.left = _pXMLNode->GetIntAttribute( L"left" );
			_rRect.right = _pXMLNode->GetIntAttribute( L"right" );
			_rRect.top = _pXMLNode->GetIntAttribute( L"top" );
			_rRect.bottom = _pXMLNode->GetIntAttribute( L"bottom" );
		}

		return true;
	}
	static bool XMLStoreRect( XMLParse::XMLNodePtr _pXMLNode, const RECT_DBL& _crRect, SIZE_DBL* pSizeBase = NULL)
	{
		if( !_pXMLNode ) return false;

		_pXMLNode->SetUnitAttribute( L"left", _crRect.left, pSizeBase ? pSizeBase->cx : 0 );
		_pXMLNode->SetUnitAttribute( L"right",_crRect.right, pSizeBase ? pSizeBase->cx : 0 );
		_pXMLNode->SetUnitAttribute( L"top", _crRect.top, pSizeBase ? pSizeBase->cy : 0 );
		_pXMLNode->SetUnitAttribute( L"bottom", _crRect.bottom, pSizeBase ? pSizeBase->cy : 0 );

		return true;
	}
	static bool XMLGetRect( XMLParse::XMLNodePtr _pXMLNode, RECT_DBL& _rRect, SIZE_DBL* pSizeBase  = NULL)
	{
		if( !_pXMLNode ) return false;

		_rRect.left = _pXMLNode->GetUnitAttribute( L"left", pSizeBase ? pSizeBase->cx : -100 );
		_rRect.right = _pXMLNode->GetUnitAttribute( L"right", pSizeBase ? pSizeBase->cx : -100 );
		_rRect.top = _pXMLNode->GetUnitAttribute( L"top", pSizeBase ? pSizeBase->cy : -100 );
		_rRect.bottom = _pXMLNode->GetUnitAttribute( L"bottom", pSizeBase ? pSizeBase->cy : -100 );

		return true;
	}
	static bool XMLStoreSize( XMLParse::XMLNodePtr _pXMLNode, const SIZE& _crSize, SIZE* pSizeBase  = NULL)
	{
		if( !_pXMLNode ) return false;

		_pXMLNode->SetUnitAttribute( L"width", _crSize.cx, pSizeBase ? pSizeBase->cx : 0 );
		_pXMLNode->SetUnitAttribute( L"height",_crSize.cy, pSizeBase ? pSizeBase->cy : 0 );

		return true;
	}
	static bool XMLGetSize( XMLParse::XMLNodePtr _pXMLNode, SIZE& _crSize, SIZE* pSizeBase  = NULL)
	{
		if( !_pXMLNode ) return false;

		if( pSizeBase )
		{
			SIZE_DBL szRes = {(double)_crSize.cx, (double)_crSize.cy};
			SIZE_DBL szBase = {(double)pSizeBase->cx, (double)pSizeBase->cy };
			XMLGetSize( _pXMLNode, szRes, &szBase );
			_crSize = szRes;
		}
		else
		{
			_crSize.cx = _pXMLNode->GetIntAttribute( L"width" );
			_crSize.cy = _pXMLNode->GetIntAttribute( L"height" );
		}

		return true;
	}
	static bool XMLStoreSize( XMLParse::XMLNodePtr _pXMLNode, const SIZE_DBL& _crSize, SIZE_DBL* pSizeBase  = NULL)
	{
		if( !_pXMLNode ) return false;

		_pXMLNode->SetUnitAttribute( L"width",  _crSize.cx, pSizeBase ? pSizeBase->cx : 0 );
		_pXMLNode->SetUnitAttribute( L"height", _crSize.cy, pSizeBase ? pSizeBase->cy : 0 );


		return true;
	}
	static bool XMLGetSize( XMLParse::XMLNodePtr _pXMLNode, SIZE_DBL& _crSize, SIZE_DBL* pSizeBase  = NULL)
	{
		if( !_pXMLNode ) return false;

		_crSize.cx = _pXMLNode->GetUnitAttribute( L"width", pSizeBase ? pSizeBase->cx : -100  );
		_crSize.cy = _pXMLNode->GetUnitAttribute( L"height", pSizeBase ? pSizeBase->cy : -100  );

		return true;
	}

	static BSTR XMLGetString( XMLParse::XMLNodePtr pXMLNode, CComBSTR& _cbsString, bool bUseTags )
	{
		CComBSTR cbsNewLine = bUseTags ? L"<br/>" : L"\r\n";
		// Collect text
		CComBSTR cbsTemp;
		for( int i = 0; i < pXMLNode->GetNodesCount(); i++ )
		{
			XMLNodePtr pNode = pXMLNode->GetNodeAt(i);
			if( pNode->IsValue() )
			{
				cbsTemp = pNode->Value();
				XMLUtils::ReplaceChars( cbsTemp.m_str, L"\r\n\t", L' ' );
				_cbsString.Append( cbsTemp );
			}
			else
			{
				if( pNode->GetName() == L"br" )
					_cbsString.Append( cbsNewLine );
				else if( pNode->GetName() == L"tab" )
					_cbsString.Append( bUseTags ? L"<tab/>": L"\t" );
				else if( pNode->GetName()  == L"p" )
				{
					_cbsString.Append( cbsNewLine );
					if( pNode->GetNodesCount()  )
					{
						XMLGetString( pNode, _cbsString, bUseTags );
						_cbsString.Append( cbsNewLine );
					}
				}
				else if( pNode->GetNodesCount() )
				{
					XMLGetString( pNode, _cbsString, bUseTags );
				}
			}
		}

		return _cbsString;
	}

	template <class TNode>
	static LPCWSTR XMLUpdatePath( CComBSTR& _rcbsPath, TNode _pNode, LPCWSTR _pszPathTag, TNode _pParent = (TNode)NULL )
	{
		if( _rcbsPath.Length() && !IsFullPathW( _rcbsPath ) )
		{
			TNode pParent = _pNode->GetParentNode();
			if( !pParent )
			{
				pParent = _pParent;
				_pParent = (TNode)NULL;
			}

			while( pParent )
			{
				CComBSTR cbsDefPath = pParent->GetStringAttribute_Safe( _pszPathTag );
				if( !cbsDefPath || !IsFullPathW(cbsDefPath) )
					pParent = pParent->GetParentNode();
				else  
				{
					MakeFullPath( _rcbsPath, cbsDefPath );
					break;
				}

				if( !pParent )
				{
					pParent = _pParent;
					_pParent = (TNode)NULL;
				}
			}

			// Try module name 
			GetFullPath_Module( _rcbsPath );
		}

		return _rcbsPath;
	}

	template <class TNode>
	static LPCWSTR XMLGetPath( TNode _pNode, LPCWSTR _pszPathTag, CComBSTR& _rcbsPath, TNode _pParent = (TNode)NULL )
	{
		_rcbsPath = _pNode->GetStringAttribute_Safe( _pszPathTag );

		return XMLUpdatePath( _rcbsPath, _pNode, _pszPathTag, _pParent );
	}



	//////////////////////////////////////////////////////////////////////////
	// Custom struct load/save

// 	static void XMLStoreVidProps( XMLParse::XMLNodePtr _pXMLNode, const MG_VID_PROPS& _crVidProps, bool _bStoreAll = true )
// 	{
// 		ATLASSERT( _pXMLNode );
// 		if( _pXMLNode )
// 		{
// 			USES_CONVERSION;
// 
// 			if( _crVidProps.nWidth != 0 || _bStoreAll) 
// 				_pXMLNode->SetAttribute( L"width", _crVidProps.nWidth );
// 			if( _crVidProps.nHeight != 0 || _bStoreAll) 
// 				_pXMLNode->SetAttribute( L"height", _crVidProps.nHeight );
// 			if( _crVidProps.dblRate > 0 || _bStoreAll) 
// 				_pXMLNode->SetAttribute( L"rate", _crVidProps.dblRate );
// 			if( _crVidProps.fccType != eMGFCC_Default || _bStoreAll) 
// 				_pXMLNode->SetAttribute( L"colorspace",  _crVidProps.fccType ? FCC2NAMEW(_crVidProps.fccType) : L"Default" );
// 			if( ( _crVidProps.nAspectX > 0 && _crVidProps.nAspectY > 0 ) || _bStoreAll )
// 				_pXMLNode->SetAttribute( L"aspect_ratio",  PAIR2STR( _crVidProps.nAspectX, _crVidProps.nAspectY ) );
// 
// 			if( _crVidProps.eInterlace != eMGI_Default || _bStoreAll )
// 				_pXMLNode->SetAttribute( L"interlace",  eMG_InterlaceStr, _crVidProps.eInterlace  );
// 			if( _crVidProps.eScaleType != eMGKA_Default || _bStoreAll )
// 				_pXMLNode->SetAttribute( L"maintain_ar",  eMG_KeepAspectStr, _crVidProps.eScaleType  );
// 		}
// 	}
// 
// 	static bool XMLGetVidProps( XMLParse::XMLNodePtr _pXMLNode, MG_VID_PROPS& _rVidProps )
// 	{
// 		USES_CONVERSION;
// 
// 		if( _pXMLNode )
// 		{
// 			_pXMLNode->LoadAttribute( L"width", _rVidProps.nWidth );
// 			_pXMLNode->LoadAttribute( L"height", _rVidProps.nHeight );
// 			if( !_pXMLNode->LoadAttribute( L"rate", _rVidProps.dblRate ) )
// 			{
// 				// Old variant (e.g. Live Source)
// 				_pXMLNode->LoadAttribute( L"frame-rate", _rVidProps.dblRate );
// 			}
// 
// 			_rVidProps.fccType = (eMGFCC)_pXMLNode->GetFccAttribute( L"colorspace", _rVidProps.fccType );
// 
// 			pair<int,int> pairAR( _rVidProps.nAspectX, _rVidProps.nAspectY );
// 			pairAR = XMLUtils::Str2Pair( _pXMLNode->GetStringAttribute(L"aspect_ratio"), pairAR );
// 			_rVidProps.nAspectX = pairAR.first;
// 			_rVidProps.nAspectY = pairAR.second;
// 
// 			_pXMLNode->LoadAttribute( L"interlace", eMG_InterlaceStr, _rVidProps.eInterlace );
// 
// 			if( !_pXMLNode->LoadAttribute(L"maintain_ar", eMG_KeepAspectStr, _rVidProps.eScaleType ) )
// 			{
// 				// Old variant
// 				_pXMLNode->LoadAttribute(L"keep-aspect", eMG_KeepAspectStr, _rVidProps.eScaleType );
// 			}
// 
// 			return true;
// 		}
// 		return false;
// 	}
// 
// 	static void XMLStoreAudProps( XMLParse::XMLNodePtr _pXMLNode, const MG_AUD_PROPS& _crAudProps )
// 	{
// 		ATLASSERT( _pXMLNode );
// 		if( _pXMLNode )
// 		{
// 			if( _crAudProps.nChannels < 0 )
// 			{
// 				_pXMLNode->SetAttribute( L"noaudio", true );
// 			}
// 			else
// 			{
// 				_pXMLNode->SetAttribute( L"channels", _crAudProps.nChannels );
// 				_pXMLNode->SetAttribute( L"bits", _crAudProps.nBitsPerSample);
// 				_pXMLNode->SetAttribute( L"rate", _crAudProps.nSamplesPerSec );
// 			}
// 		}
// 	}
// 
// 	static bool XMLGetAudProps( XMLParse::XMLNodePtr _pXMLNode, MG_AUD_PROPS& _rAudProps )
// 	{
// 		if( _pXMLNode )
// 		{
// 			bool bNoAudio = false;
// 			_pXMLNode->LoadAttribute( L"noaudio", bNoAudio );
// 			if( bNoAudio ) 
// 			{
// 				_rAudProps.nChannels = -1;
// 				_rAudProps.nBitsPerSample = 0;
// 				_rAudProps.nSamplesPerSec = 0;
// 			}
// 			else
// 			{
// 				_pXMLNode->LoadAttribute( L"channels", _rAudProps.nChannels );
// 				_pXMLNode->LoadAttribute( L"bits", _rAudProps.nBitsPerSample);
// 				_pXMLNode->LoadAttribute( L"rate", _rAudProps.nSamplesPerSec );
// 			}
// 
// 			return true;
// 		}
// 		return false;
// 	}

#ifdef __DSTRACE_H__
#ifndef _LIB
#	ifdef _DEBUG 
template <class TNode_Ptr>
	static void Debug_XML( UINT level, TNode_Ptr pNode, LPCWSTR pszPrefix = NULL)
	{
		if(level > ATL_TRACE_LEVEL) return;

		USES_CONVERSION;

		CComBSTR cbsRes;
		if( pNode )
			XMLParse::SaveXML( pNode, &cbsRes );
		else
			cbsRes = "NULL";

		CComBSTR cbsPrint = pszPrefix ? pszPrefix : L"[XML] ";
		cbsPrint.AppendBSTR( cbsRes );

		char* pszText = new char[ cbsPrint.Length() + 3 ];
		int nChars = ::WideCharToMultiByte( CP_ACP, WC_NO_BEST_FIT_CHARS, cbsPrint, cbsPrint.Length(), pszText, cbsPrint.Length() + 2, NULL, FALSE );
		pszText[nChars] = 0;

		_LogOutBuffer( cbsPrint.Length(), (BYTE*)pszText );

		delete [] pszText;
	}

template <class TNode_Ptr>
	static void Debug_Attributes( UINT level, TNode_Ptr pNode, LPCWSTR pszPrefix = NULL )
	{
		if(level > ATL_TRACE_LEVEL) return;

		USES_CONVERSION;

		CComBSTR cbsRes;
		if( pNode )
			pNode->SaveAttributes( &cbsRes );
		else
			cbsRes = "NULL";

		ATLTRACE2( atlTraceGeneral, level, _T("%ws %ws\n"), pszPrefix ? pszPrefix : L"[XMLAttributes] ", cbsRes );
	}
#	else // _DEBUG 
template <class TNode_Ptr>
	static void Debug_XML( int atlLevel, TNode_Ptr pNode, LPCWSTR pszPrefix = NULL) {}
template <class TNode_Ptr>
	static void Debug_Attributes( UINT level, TNode_Ptr pNode, LPCWSTR pszPrefix = NULL ) {}
#	endif // _DEBUG 
#else // _LIB
template <class TNode_Ptr>
	static void Debug_XML( int atlLevel, TNode_Ptr pNode, LPCWSTR pszPrefix = NULL) {}
	template <class TNode_Ptr>
	static void Debug_Attributes( UINT level, TNode_Ptr pNode, LPCWSTR pszPrefix = NULL ) {}
#endif // _LIB
#else // __DS_TRACE_H__
template <class TNode_Ptr>
	static void Debug_XML( int atlLevel, TNode_Ptr pNode, LPCWSTR pszPrefix = NULL) {}
template <class TNode_Ptr>
	static void Debug_Attributes( UINT level, TNode_Ptr pNode, LPCWSTR pszPrefix = NULL ) {}
#endif // __DS_TRACE_H__

};

_declspec(selectany) MClock XMLParse::XMLAttribute::s_clockGlobal;

#endif //_PARSE_MPL_H_