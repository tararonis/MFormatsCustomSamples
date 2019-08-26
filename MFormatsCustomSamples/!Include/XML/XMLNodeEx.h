//---------------------------------------------------------------------------
// This file is part of the Medialooks Media Platform (MPlatform)
//
// XMLParseEx.h 
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

#include "XMLParseM.h"

template<class TNodeBase, class TNodePtr>
class XMLNodeExT: 
	public TNodeBase
{
public:
	// (hide->show) on-show: 0->1
	// (show->hide) on-hide: -1->0
	// default: 1

	// Return: 1 -> Trans done
	//		   0 -> No trans
	//		  -1 -> Transition now


	int IsTransDone( double _dblStreamTime, double* _pdblTransPos = NULL, int* _pnShowHide = NULL )
	{
		if( _dblStreamTime < 0 )
			return 0;

		if( _pnShowHide )
			*_pnShowHide = 0;

		if( _pdblTransPos )
			*_pdblTransPos = 0;

		TNodePtr pFromNode = GetObject<TNodePtr>(L"node_from");
		if( !pFromNode ) return 0;

		double dblChangePos = GetTransPros( pFromNode, _dblStreamTime );
		if( dblChangePos > 1.0 )
		{
			ReleaseObject( L"node_from" );
			ReleaseObject( L"node_hide" );
			return 1;
		}

		if( _pdblTransPos )
			*_pdblTransPos = dblChangePos;

		if( _pnShowHide )
		{
			bool bFrom = pFromNode->GetBoolAttribute( L"show", true );
			bool bTo = TNodeBase::GetBoolAttribute( L"show", true );
			if( bFrom != bTo )
				*_pnShowHide = bFrom ? -1 : 1;
		}

		return -1;
	}

	

	bool GetBoolAttribute( LPCWSTR _pszName, bool _bDef, bool* _pbFind, bool _bAllTrue )
	{
		 TNodePtr pFromNode = GetObject<TNodePtr>(L"node_from");
		if( pFromNode && pFromNode->HaveAttribute( _pszName ) )
		{
			bool bFrom = pFromNode->GetBoolAttribute( _pszName, _bDef );
			bool bTo = TNodeBase::GetBoolAttribute( _pszName, _bDef, _pbFind );
			return _bAllTrue ? (bFrom && bTo) : (bFrom | bTo);
		}

		return TNodeBase::GetBoolAttribute( _pszName, _bDef, _pbFind );
	}

	int GetIntAttribute( LPCWSTR _pszName, 
		double _dblStreamTime,
		int* _pnFrom,
		double* _pdblPos,
		int _nDef = 0 )
	{
		bool bRev = false;
		double dblChangePos = -1;
		TNodePtr pFromNode = GetObject<TNodePtr>(L"node_from");
		if( pFromNode && pFromNode->HaveAttribute( _pszName ) )
		{
			dblChangePos = GetTransPros( pFromNode, _dblStreamTime );
			if( dblChangePos > 1.0 )
			{
				// Not clear as need to catch in trans check
				pFromNode = (TNodePtr)NULL;
				dblChangePos = -1;
			}
		}

		int nRes = ((TNodePtr)this)->GetIntAttribute( _pszName, _nDef );
		if( _pnFrom )
			*_pnFrom = pFromNode ? pFromNode->GetIntAttribute( _pszName, _nDef ) : nRes;
		if( _pdblPos )
			*_pdblPos = dblChangePos;
		return nRes;
	}

	CComBSTR GetStringAttribute_Safe( LPCWSTR _pszName, 
		double _dblStreamTime,
		CComBSTR& _cbsFrom,
		double* _pdblPos,
		LPCWSTR _pszDef = NULL )
	{
		bool bRev = false;
		double dblChangePos = -1;
		TNodePtr pFromNode = GetObject<TNodePtr>(L"node_from");
		if( pFromNode && pFromNode->HaveAttribute( _pszName ) )
		{
			dblChangePos = GetTransPros( pFromNode, _dblStreamTime );
			if( dblChangePos > 1.0 )
			{
				// Not clear as need to catch in trans check
				pFromNode = (TNodePtr)NULL;
				dblChangePos = -1;
			}
		}

		CComBSTR cbsRes = ((TNodePtr)this)->GetStringAttribute_Safe( _pszName, _pszDef );
		_cbsFrom = pFromNode ? pFromNode->GetStringAttribute_Safe( _pszName, _pszDef ) : cbsRes;
		if( _pdblPos )
			*_pdblPos = dblChangePos;
		return cbsRes;
	}

	double GetDblAttribute( LPCWSTR _pszName, 
		double _dblDef,
		double _dblStreamTime,
		double _dblAddSegment = -1,
		double _dblFPS = 0 )
	{
		bool bRev = false;
		double dblChangePos = -1;
		TNodePtr pFromNode = GetObject<TNodePtr>(L"node_from");
		if( pFromNode && pFromNode->HaveAttribute( _pszName ) )
		{
			dblChangePos = GetTransPros( pFromNode, _dblStreamTime );
			if( dblChangePos > 1.0 )
			{
				// Not clear as need to catch in trans check
				pFromNode = (TNodePtr)NULL;
				dblChangePos = 0;
			}
			// 				else if( dblChangePos == 0 )
			// 				{
			// 					pFromNode = NULL;
			// 				}
		}

		TNodePtr pHideNode = GetObject<TNodePtr>(L"node_hide");

		return XMLGetDblAttribute( _pszName, _dblDef, pHideNode ? pHideNode : (TNodePtr)this, pFromNode, dblChangePos, _dblStreamTime, _dblAddSegment, _dblFPS );
	}

	COLORREF GetColorAttribute( LPCWSTR _pszName, 
		COLORREF _crDef,
		double _dblStreamTime )
	{
		bool bRev = false;
		double dblChangePos = 0;
		TNodePtr pFromNode = GetObject<TNodePtr>(L"node_from");
		if( pFromNode )
		{
			dblChangePos = GetTransPros( pFromNode, _dblStreamTime );
			if( dblChangePos > 1.0 )
			{
				// Not clear as need to catch in trans check
				pFromNode = (TNodePtr)NULL;
				dblChangePos = 0;
			}
		}

		TNodePtr pHideNode = GetObject<TNodePtr>(L"node_hide");

		return XMLGetColorAvg( _pszName,  pHideNode ? pHideNode : (TNodePtr)this, pFromNode, dblChangePos, _crDef );
	}

//private:

	double GetTransPros(  TNodePtr pFromNode, double _dblStreamTime,  TNodeBase** ppFromNode = NULL )
	{
		if( !pFromNode )
		{
			pFromNode = GetObject<TNodePtr>(L"node_from");
			if( !pFromNode ) return 0;
		}

		if( ppFromNode )
			pFromNode.CopyTo(ppFromNode);

		if( _dblStreamTime < 0 )
			_dblStreamTime = pFromNode->GetDblAttribute( L"_stream_last_", -1 );
		else
			pFromNode->SetAttribute( L"_stream_last_", _dblStreamTime );

		if( _dblStreamTime < 0 )
			return 0;

		double dblChangePos = 0;

		double dblStartTime = pFromNode->GetDblAttribute( L"_start_", -1 );
		double dblChangeTime = pFromNode->GetDblAttribute( L"_change_time_" );
		double dblDelayTime = pFromNode->GetDblAttribute( L"_delay_time_" );
		if( dblStartTime < 0 )
		{
			pFromNode->SetAttribute( L"_start_", _dblStreamTime );
		}
		else if( dblStartTime >= 0 && dblChangeTime > 0 )
		{
			_dblStreamTime -= dblStartTime;
			if( _dblStreamTime  > dblDelayTime )
				dblChangePos = (_dblStreamTime - dblDelayTime) / dblChangeTime;

			ATLASSERT( dblChangePos >= 0 );
			if( dblChangePos <= 1 )
			{
				dblChangePos = Accel( dblChangePos, pFromNode->GetDblAttribute( L"_change_accel_", 1.0 ) );
			}
		}

		

		return dblChangePos;
	}
	private:


		static COLORREF XMLGetColorAvg( LPCWSTR _pszName, 
			TNodePtr _pDestNode, 
			TNodePtr _pSrcNode, 
			double _dblPos, 
			COLORREF crDef = -1 )
		{
			if( !_pSrcNode && !_pDestNode )
				return crDef;

			if( !_pSrcNode )
				return _pDestNode->GetColorAttribute( _pszName, crDef );

			COLORREF crFirst = _pSrcNode ? _pSrcNode->GetColorAttribute( _pszName, crDef ) : crDef;
			COLORREF crSecond = _pDestNode ? _pDestNode->GetColorAttribute( _pszName, crDef ) : crDef;

			COLORREF crRes = 0;
			LPBYTE pbRes = (LPBYTE)&crRes;
			LPBYTE pbFirst = (LPBYTE)&crFirst;
			LPBYTE pbSecond = (LPBYTE)&crSecond;

			pbRes[0] = INT2BYTE( DBL2INT( pbFirst[0] * (1.0 - _dblPos) + pbSecond[0] * _dblPos ) );
			pbRes[1] = INT2BYTE( DBL2INT( pbFirst[1] * (1.0 - _dblPos) + pbSecond[1] * _dblPos ) );
			pbRes[2] = INT2BYTE( DBL2INT( pbFirst[2] * (1.0 - _dblPos) + pbSecond[2] * _dblPos ) );
			pbRes[3] = INT2BYTE( DBL2INT( pbFirst[3] * (1.0 - _dblPos) + pbSecond[3] * _dblPos ) );

			return crRes;
		}

		

	static double XMLGetDblAttribute( LPCWSTR _pszName, 
		double _dblDef,
		TNodePtr pToNode,	// Original pos
		TNodePtr pFromNode,	// Dest pos
		double _dblTransPos, 
		double _dblStreamTime,
		double _dblAddSegment = 0,
		double _dblFPS = 0 )
	{
		double dblValue = _dblDef;
		// Update current
		if( pFromNode && _dblTransPos >= 0 )
		{
			// Add
			TNodePtr pToAdd = pToNode->GetNodePtr( L"add" );
			TNodePtr pFromAdd = pFromNode->GetNodePtr( L"add" );

			if( pToAdd || pFromAdd )
			{
				double dblFrom = XMLParse::XMLDoubleInc(_pszName, pFromNode, pFromAdd, true, _dblDef, _dblAddSegment, _dblFPS > 0 ? 1.0 / _dblFPS : 1/30.0  );
				double dblTo = XMLParse::XMLDoubleInc(_pszName, pToNode, pToAdd, true, _dblDef, _dblAddSegment, _dblFPS > 0 ? 1.0 / _dblFPS : 1/30.0 );

				dblValue = dblFrom * (1 - _dblTransPos) + dblTo * _dblTransPos;
			}
			else
			{
				dblValue = XMLParse::XMLGetDoubleAvg( _pszName, pFromNode, pToNode, _dblTransPos, _dblDef );
			}

			// Vary
			TNodePtr pToVary = pToNode->GetNodePtr( L"vary" );
			//TNodePtr pFromVary= pFromNode->GetNodePtr( L"vary" );
			if( pToVary /*|| pFromVary*/ )
			{
				double dblVaryMax = pToVary ? pToVary->GetDblAttribute( _pszName ) : 0;
// 				double dblVaryMaxFrom = pFromVary ? pFromVary->GetDblAttribute( _pszName ) : 0;
// 				dblVaryMax = MAX( dblVaryMax, dblVaryMaxFrom );
				if( ABS(dblVaryMax) >= 0.0001 )
				{
					double dblVaryTimeMax = pToVary->GetDblAttribute( L"time", 10.0 );// XMLParse::XMLGetDoubleAvg( L"time", pFromVary, pToVary, _dblTransPos, 10.0 );
					TNodePtr pToVaryTime = pToVary ? pToVary->GetNodePtr( L"time" ) : (TNodePtr)NULL;
					//TNodePtr pFromVaryTime = pFromVary ? pFromVary->GetNodePtr( L"time" ) : (TNodePtr)NULL;
					if( pToVaryTime )
						dblVaryTimeMax = pToVaryTime->GetDblAttribute( _pszName, dblVaryTimeMax );
// 					{
// 						dblVaryTimeMax = XMLParse::XMLGetDoubleAvg( _pszName, pFromVaryTime, pToVaryTime, _dblTransPos, dblVaryTimeMax );
// 					}


					double dblVaryTime = (_dblStreamTime - dblVaryTimeMax * (int)(_dblStreamTime / dblVaryTimeMax)) / dblVaryTimeMax; // [0..1]
					double dblVaryAdd = dblVaryMax * cos(2 * M_PI * dblVaryTime);

					dblValue += dblVaryAdd;
				}
			}
		}
		else
		{
			// Add
			TNodePtr pToAdd = pToNode->GetNodePtr( L"add" );
			if( pToAdd )
			{
				dblValue = XMLParse::XMLDoubleInc(_pszName, pToNode, pToAdd, true, _dblDef, _dblAddSegment, _dblFPS > 0 ? 1.0 / _dblFPS : 1/30.0 );
			}
			else
			{
				dblValue = pToNode->GetDblAttribute( _pszName, _dblDef );
			}


			// Vary
			TNodePtr pToVary = pToNode->GetNodePtr( L"vary" );
			if( pToVary )
			{
				double dblVaryMax = pToVary->GetDblAttribute( _pszName );
				if( ABS(dblVaryMax) >= 0.0001 )
				{
					double dblVaryTimeMax = pToVary->GetDblAttribute( L"time", 10.0 );
					TNodePtr pToVaryTime = pToVary->GetNodePtr( L"time" );
					if( pToVaryTime )
						dblVaryTimeMax = pToVaryTime->GetDblAttribute( _pszName, dblVaryTimeMax );


					double dblVaryTime = (_dblStreamTime - dblVaryTimeMax * (int)(_dblStreamTime / dblVaryTimeMax)) / dblVaryTimeMax; // [0..1]
					double dblVaryAdd = dblVaryMax * cos(2 * M_PI * dblVaryTime);

					dblValue += dblVaryAdd;
				}
			}
		}

		return dblValue;
	}
};

typedef XMLNodeExT<XMLParse::CoXMLNode, XMLParse::XMLNodePtr> XMLNodeEx;
