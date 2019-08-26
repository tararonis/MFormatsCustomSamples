//---------------------------------------------------------------------------
// MTimecode.h : 
//
// Description: Timecode class and aux function implementation
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

#ifndef _MTIMECODE_TC_H_
#define _MTIMECODE_TC_H_

#include "MDefines.h"
#include <XML/XMLParseM.h>
#include "../Base/MHelpers.h"



#define TC_IS_VALID( _pts ) ( (_pts != NULL) ? ((_pts)->eTCFlags & eMTCF_Invalid) == 0 : false )
#define TC_IS_DROP( _pts ) ( (_pts != NULL) ? ((_pts)->eTCFlags & eMTCF_DropFrame) != 0 : false )
#define TC_IS_PROGRESSIVE( _pts ) ( (_pts != NULL) ? ((_pts)->eTCFlags & (eMTCF_Progressive_Even_Frame|eMTCF_Progressive_Odd_Frame)) != 0 : false )
#define TC_HAVE_RATE( _pts ) ( (_pts != NULL) ? ((_pts)->eTCFlags & eMTCF_Rate) != 0 : false )

//#define M_TIME2STR( _time ) MTCHelp::MTime2Str( _time, (LPWSTR)alloca(128 *sizeof(WCHAR) ) )

struct MTCHelp
{
	inline static int Flags2BaseRate( eMTimecodeFlags _eFlags )
	{
		int nBase = 0;
		if( _eFlags & eMTCF_Rate_30 )
			nBase = 30;
		else if( _eFlags & eMTCF_Rate_25 )
			nBase = 25;
		else if( _eFlags & eMTCF_Rate_24 )
			nBase = 24;

		return nBase;
	}

	static double Flags2Rate( eMTimecodeFlags _eFlags )
	{
		int nBase = 0;
		if( _eFlags & eMTCF_Rate_24 )
			nBase = 24;
		else if( _eFlags & eMTCF_Rate_25 )
			nBase = 25;
		else if( _eFlags & eMTCF_Rate_30 )
			nBase = 30;

		if( (_eFlags & eMTCF_Progressive_Even_Frame) || (_eFlags & eMTCF_Progressive_Odd_Frame) )
			nBase *= 2;

		if( (nBase == 30 || nBase == 60) && (_eFlags & eMTCF_DropFrame) )
		{
			return (double)nBase / 1.001;
		}

		return (double)nBase;
	}

	inline static eMTimecodeFlags Rate2Flags( int nRate, bool _bDrop = false, eMTimecodeFlags* _peFlags = NULL )
	{
		eMTimecodeFlags eFlags = eMTCF_NonDropFrame;
		if( nRate > 45 )
		{
			nRate /= 2;
			PutFlag( &eFlags, eMTCF_Progressive_Even_Frame );
		}
		if( nRate == 24 )
			PutFlag( &eFlags, eMTCF_Rate_24 );
		else if( nRate == 25 )
			PutFlag( &eFlags, eMTCF_Rate_25 );
		else if( nRate == 30 )
			PutFlag( &eFlags, eMTCF_Rate_30 );

		if( _bDrop && nRate == 30 )
			PutFlag( &eFlags, eMTCF_DropFrame );

		if( _peFlags )
			PutFlag( _peFlags, eFlags );

		return eFlags;
	}
	
	static double CorrectAvgTime( double& dblAvgTime )
	{
		// Correct rate to 1000/1001 values
		if( dblAvgTime > 417000 && dblAvgTime < 417200 )
		{
			// 29.98 rate
			dblAvgTime = (double)DS_ONESEC * 1001 / 24000;
		}
		else if( dblAvgTime > 333660 && dblAvgTime < 333670 )
		{
			// 29.97 rate
			dblAvgTime = (double)DS_ONESEC * 1001 / 30000;
		}
		else if( dblAvgTime > 166830 && dblAvgTime < 166840 )
		{
			// 59.94 rate
			dblAvgTime = (double)DS_ONESEC * 1001 / 60000;
		}

		return dblAvgTime;
	}

	
	static int Int2Frame( int _nTimecode, REFERENCE_TIME _rtAvgTime, bool _bDropFrame )
	{
		return Tc2Frame( Int2Tc( _nTimecode ), _rtAvgTime, _bDropFrame );
	}

	static int Frame2Int( int _nFrame, REFERENCE_TIME _rtAvgTime, bool _bDropFrame )
	{
		return Tc2Int( Frame2Tc( _nFrame, _rtAvgTime, _bDropFrame ) );
	}

	static M_TIMECODE Int2Tc( int _nTimecode )
	{
		if( _nTimecode == -1 ) 
		{
			M_TIMECODE tcInvalid = {};
			tcInvalid.eTCFlags = eMTCF_Invalid;
			return tcInvalid;
		}

		M_TIMECODE mgRes = {0};
		mgRes.nHours = (~0x80000000 & _nTimecode) >> 24;
		mgRes.nHours = (mgRes.nHours & 0xF) + (mgRes.nHours >> 4) * 10;
		mgRes.nMinutes = (0x00FF0000 & _nTimecode) >> 16;
		mgRes.nMinutes = (mgRes.nMinutes & 0xF) + (mgRes.nMinutes >> 4) * 10;
		mgRes.nSeconds = (0x0000FF00 & _nTimecode) >> 8;
		mgRes.nSeconds= (mgRes.nSeconds & 0xF) + (mgRes.nSeconds >> 4) * 10;
		mgRes.nFrames = (0x000000FF & _nTimecode);
		mgRes.nFrames = (mgRes.nFrames & 0xF) + (mgRes.nFrames >> 4) * 10;
		return mgRes;
	};

	static M_TIMECODE Dword2Tc( int _nTimecode )
	{
		if( _nTimecode == -1 ) 
		{
			M_TIMECODE tcInvalid = {};
			tcInvalid.eTCFlags = eMTCF_Invalid;
			return tcInvalid;
		}

		M_TIMECODE mgRes = {};
		mgRes.nHours = ((~0x80000000 & _nTimecode) >> 24);
		mgRes.nMinutes = (0x00FF0000 & _nTimecode) >> 16;
		mgRes.nSeconds = (0x0000FF00 & _nTimecode) >> 8;
		mgRes.nFrames = (0x000000FF & _nTimecode);
		return mgRes;
	};

	static M_TIMECODE Int2TcPacked( int _nTimecode )
	{
		if( _nTimecode == -1 ) 
		{
			M_TIMECODE tcInvalid = {};
			tcInvalid.eTCFlags = eMTCF_Invalid;
			return tcInvalid;
		}


		// 9:5:6:6:6 T:H:M:S:F
		M_TIMECODE mgRes = {};
		mgRes.nFrames = (_nTimecode & 0x3F);
		mgRes.nMinutes = ((_nTimecode >> 6) & 0x3F);
		mgRes.nSeconds = ((_nTimecode >> 12) & 0x3F);
		mgRes.nHours = ((_nTimecode >> 18) & 0x1F);
		mgRes.nExtraCounter = ((_nTimecode >> 23) & 0x1FF);
		return mgRes;
	};

	static int Tc2IntPacked( const M_TIMECODE& _rTimecode )
	{
		// 9:5:6:6:6 T:H:M:S:F
		if( !TC_IS_VALID( &_rTimecode ) )
		{
			return -1;
		}

		UINT nTimeCode = UINT(_rTimecode.nExtraCounter & 0x1FF);
		nTimeCode <<= 9;
		nTimeCode |= UINT( _rTimecode.nHours % 24);
		nTimeCode <<= 5;
		nTimeCode |= UINT( _rTimecode.nMinutes % 60);
		nTimeCode <<= 6;
		nTimeCode |= UINT( _rTimecode.nMinutes % 60);
		nTimeCode <<= 6;
		nTimeCode |= UINT( _rTimecode.nFrames & 0x3F);
		
		return nTimeCode;
	};

	static int Tc2Int( const M_TIMECODE& _rTimecode )
	{
		if( !TC_IS_VALID( &_rTimecode ) )
		{
			return -1;
		}

		int nRes = 0;
		LPBYTE pbRes = (LPBYTE)&nRes;
		pbRes[3] = _rTimecode.nHours % 10 + ((_rTimecode.nHours / 10) * 0x10) ;
		pbRes[2] = _rTimecode.nMinutes % 10 + ((_rTimecode.nMinutes / 10) * 0x10) ;
		pbRes[1] = _rTimecode.nSeconds % 10 + ((_rTimecode.nSeconds / 10) * 0x10) ;
		pbRes[0] = _rTimecode.nFrames % 10 + ((_rTimecode.nFrames / 10) * 0x10) ;
		return nRes;
	}

	static int Tc2Frame( const M_TIMECODE& _rTimecode, REFERENCE_TIME _rtAvgTimePerFrame, int _nDropFrame = -1 )
	{
		return Tc2Frame( _rTimecode, _rtAvgTimePerFrame > 0 ? (double)DS_ONESEC / _rtAvgTimePerFrame : 0, _nDropFrame );
	}

	static M_TIMECODE NextTC( const M_TIMECODE& _rTimecode, REFERENCE_TIME _rtAvgTimePerFrame, int _nDropFrame = -1 )
	{
		return NextTC( _rTimecode, _rtAvgTimePerFrame > 0 ? (double)DS_ONESEC / _rtAvgTimePerFrame : 0, _nDropFrame );
	}

	static M_TIMECODE NextTC( const M_TIMECODE& _rTimecode, double _dblRate = 0, int _nDropFrame = -1 )
	{
		int nFrame = Tc2Frame( _rTimecode, _dblRate, _nDropFrame );
		if( _dblRate <= 0 )
			_dblRate = TC_HAVE_RATE( &_rTimecode ) ? Flags2BaseRate( _rTimecode.eTCFlags ) : 30;

		return Frame2Tc( nFrame, _dblRate, _nDropFrame >= 0 ? _nDropFrame != 0: TC_IS_DROP( &_rTimecode ) );
	}

	static int Tc2Frame( const M_TIMECODE& _rTimecode, double _dblRate = 0, int _nDropFrame = -1 )
	{
		if( !TC_IS_VALID( &_rTimecode ) ) return -1;

		int nRate = DBL2INT( _dblRate );
		if( nRate > 45 ) 
			nRate /= 2;

		if( nRate <= 0 )
			nRate = TC_HAVE_RATE( &_rTimecode ) ? Flags2BaseRate( _rTimecode.eTCFlags ) : 30;
		
		if (_nDropFrame < 0)
		{
			// Detect drop frame 
			_nDropFrame = (nRate % 30 == 0 && TC_IS_DROP(&_rTimecode));
		}
		else if ( nRate % 30 != 0 || (nRate - _dblRate) < 0.005)
		{
			// Reset drop frame for incorrect rates
			_nDropFrame = 0;
		}

		int nFrames = 0;
		if( _nDropFrame > 0 )
		{
			// Drop two frame every minutes and not drop every 10 minutes
			nFrames = ((_rTimecode.nHours * 3600 + _rTimecode.nMinutes * 60 + _rTimecode.nSeconds) * nRate + _rTimecode.nFrames - 
					  ((_rTimecode.nHours * 60 + _rTimecode.nMinutes - ( (_rTimecode.nHours * 60 + _rTimecode.nMinutes) / 10)) * 2));
		}
		else
		{
			nFrames = (_rTimecode.nHours * 3600 + _rTimecode.nMinutes * 60 + _rTimecode.nSeconds) * nRate + _rTimecode.nFrames;
		}

		if( TC_IS_PROGRESSIVE( &_rTimecode) || _dblRate > 45.0 )
		{
			nFrames *= 2;

			if( _rTimecode.eTCFlags & eMTCF_Progressive_Odd_Frame )
				nFrames++;
		}

		return nFrames;
	}

	static M_TIMECODE Frame2Tc( int _nFrame, REFERENCE_TIME _rtAvgTimePerFrame, bool _bDropFrame = false )
	{
		return Frame2Tc( _nFrame, _rtAvgTimePerFrame > 0 ? (double)DS_ONESEC / _rtAvgTimePerFrame : 0, _bDropFrame );
	}

	static M_TIMECODE Frame2Tc( int _nFrame, double _dblRate, bool _bDropFrame = false )
	{
		M_TIMECODE tcRes = {};
		if( _nFrame < 0 )
		{
			tcRes.eTCFlags = eMTCF_Invalid;
			return tcRes;
		}

		tcRes.nExtraCounter = _nFrame;

		int nBase = DBL2INT( _dblRate );
		if( (nBase != 30 && nBase != 60) || (nBase - _dblRate) < 0.005 )
			_bDropFrame = false;

		if( nBase > 45 )
		{
			// Progressive
			SET_FLAG( &tcRes.eTCFlags, (_nFrame % 2) ? (eMTCF_Progressive_Odd_Frame|eMTCF_Progressive_Even_Frame) : eMTCF_Progressive_Even_Frame, true );
			nBase /= 2;
			_nFrame /= 2;
		}
		
		// Put rate flags
		Rate2Flags( nBase, _bDropFrame, &tcRes.eTCFlags );

		if( nBase == 0 )
			nBase = 30;

		// Convert from mTime to timecode
		int nSeconds = (int)(_nFrame / nBase );
		if( _bDropFrame )
		{
			// Frames in ten minutes 600 * DBL2INT( dblRate ) - 20 + 2 (17982 for 29.97)
			int nFramesIn10Min = 600 * nBase - 10 * 2 + 2;
			int nTenMinutes = _nFrame / nFramesIn10Min;
			_nFrame %= nFramesIn10Min;
			// Frames in one minute 60 * DBL2INT( dblRate ) - 2  (1798 for 29.97, 1800 for first minute)
			if( _nFrame < nBase * 60 )
			{
				// First minute
				nSeconds = _nFrame / nBase + nTenMinutes * 10 * 60;
				_nFrame %= nBase;
			}
			else
			{
				// Next minutes (1798 frames in minute)
				int nMinutes = 1 + (_nFrame - nBase * 60) / (nBase * 60 - 2);
				_nFrame = (_nFrame - nBase * 60) % (nBase * 60 - 2);
				// 28 frames in first second
				if( _nFrame < nBase - 2 )
				{
					// First second
					nSeconds = 60 * nMinutes + nTenMinutes * 10 * 60;
					_nFrame += 2;
				}
				else
				{
					// Next seconds
					nSeconds = 60 * nMinutes + nTenMinutes * 10 * 60 + 1;
					nSeconds += (_nFrame - (nBase - 2)) / nBase;
					_nFrame = (_nFrame - (nBase - 2)) % nBase;
				}
			}

			tcRes.nHours = nSeconds / 3600;
			tcRes.nMinutes = (((nSeconds % 3600) + 3600) % 3600 ) / 60;
			tcRes.nSeconds = ((nSeconds % 60) + 60) % 60;
			tcRes.nFrames = _nFrame;
		}
		else
		{
			tcRes.nHours = nSeconds / 3600;
			tcRes.nMinutes = (((nSeconds % 3600) + 3600) % 3600 ) / 60;
			tcRes.nSeconds = ((nSeconds % 60) + 60) % 60;
			tcRes.nFrames = ((_nFrame % nBase) + nBase) % nBase; 
		}

		return tcRes;
	}

	static REFERENCE_TIME Tc2Ref( const M_TIMECODE& _rTimecode, REFERENCE_TIME _rtAvgTime, int _nDropFrame = -1 )
	{
		return Tc2Ref( _rTimecode, _rtAvgTime > 0 ? (double)(DS_ONESEC / _rtAvgTime) : 0, _nDropFrame );
	}

	static REFERENCE_TIME Tc2Ref( const M_TIMECODE& _rTimecode, double _dblRate = 0, int _nDropFrame = -1 )
	{
		if( !TC_IS_VALID( &_rTimecode ) ) return -1;

		int nFrames = Tc2Frame( _rTimecode, _dblRate, _nDropFrame );

		int nRate = DBL2INT( _dblRate );
		if( nRate > 45 ) 
			nRate /= 2;

		if( nRate == 0 )
			nRate = TC_HAVE_RATE( &_rTimecode ) ? Flags2BaseRate( _rTimecode.eTCFlags ) : 30;

		if( _nDropFrame < 0 || nRate != 30 )
			_nDropFrame = (nRate == 30 && TC_IS_DROP( &_rTimecode ));

		// Convert progressive rate back
		if( _dblRate > 45.0 || TC_IS_PROGRESSIVE( &_rTimecode) )
			nRate *= 2;

		if( _nDropFrame > 0 )
		{
			return DBL2INT64((double)nFrames * DS_ONESEC * 1000 / (nRate * 1001));
		}
		
		return DBL2INT64((double)nFrames * DS_ONESEC / nRate);
	}

	static M_TIMECODE Ref2Tc( REFERENCE_TIME _rtTime, double _dblRate, bool _bDropFrame = false )
	{
		return Ref2Tc( _rtTime, _dblRate > 0 ? DBL2INT64( DS_ONESEC / _dblRate ) : 0, _bDropFrame );
	}

	static M_TIMECODE Ref2Tc( REFERENCE_TIME _rtTime, REFERENCE_TIME _rtAvgTime, bool _bDropFrame = false )
	{
		if( _rtTime < 0 )
		{
			M_TIMECODE tcInvalid = {};
			tcInvalid.eTCFlags = eMTCF_Invalid;
			return tcInvalid;
		}

		double dblAvgTime = (double)(_rtAvgTime > 0 ? _rtAvgTime : 333667);
		CorrectAvgTime( dblAvgTime );

		// Add 2 msec for correct rounding during .97 rates
		int nFrames = (int)((_rtTime + 2 * DS_MSEC)/ dblAvgTime);
		return Frame2Tc( nFrames, (double)DS_ONESEC / dblAvgTime, _bDropFrame );
	}

	static M_TIMECODE& NormilizeTC( M_TIMECODE& _rTimecode, double _dblRate = 0.0 )
	{
		if( !TC_IS_VALID( &_rTimecode ) )
		{
			return _rTimecode;
		}
		
		int nFrames = Tc2Frame( _rTimecode, _dblRate );

		int nRate = _dblRate > 0 ? DBL2INT( _dblRate ) : 
			TC_HAVE_RATE( &_rTimecode ) ? Flags2BaseRate( _rTimecode.eTCFlags ) : 30;

		bool bDropFrame = nRate == 30 && TC_IS_DROP( &_rTimecode );

		if( TC_IS_PROGRESSIVE( &_rTimecode) )
			nRate *= 2;

		_rTimecode = Frame2Tc( nFrames, nRate / (bDropFrame ? 1.001 : 1.0), bDropFrame );
	
		return _rTimecode;
	}

	static LPCWSTR Tc2Str( const M_TIMECODE& _rTimecode, LPWSTR _szBuffer, bool _bPrintField = false )
	{
		LPCWSTR pszDelimer = TC_IS_DROP( &_rTimecode ) ? L";" : L":";
		if( _bPrintField )
		{
			// 50p, 59.94p, 60p HD video
			wsprintf( _szBuffer, _T("%02d:%02d:%02d%ws%02d.%d"), _rTimecode.nHours, _rTimecode.nMinutes, _rTimecode.nSeconds, pszDelimer, _rTimecode.nFrames, 
				_rTimecode.nFrames % 2 );
		}
		else
		{
			wsprintf( _szBuffer, _T("%02d:%02d:%02d%ws%02d"), _rTimecode.nHours, _rTimecode.nMinutes, _rTimecode.nSeconds, pszDelimer, _rTimecode.nFrames );
		}

		return _szBuffer;
	}

	static REFERENCE_TIME Str2Ref( LPCTSTR szTime, REFERENCE_TIME _rtAvgTime, int _nDropFrame = -1 )
	{
		return Tc2Ref( Str2Tc(szTime, _rtAvgTime, _nDropFrame), _rtAvgTime, _nDropFrame );
	}

	static REFERENCE_TIME Str2Ref( LPCTSTR szTime, double _dblRate, int _nDropFrame = -1 )
	{
		return Tc2Ref( Str2Tc(szTime, _dblRate, _nDropFrame), _dblRate, _nDropFrame );
	}
	
	static M_TIMECODE Str2Tc( LPCWSTR _pszTime, REFERENCE_TIME _rtAvgTime, int _nDropFrame = -1 )
	{
		return Str2Tc( _pszTime, _rtAvgTime > 0 ? (double)DS_ONESEC / _rtAvgTime : 0.0, _nDropFrame );
	}

	static M_TIMECODE Str2Tc( LPCWSTR _pszTime, double _dblRate = 0, int _nDropFrame = -1 )
	{
		M_TIMECODE mTimeCode = {};
		if( !_pszTime ) 
		{
			mTimeCode.eTCFlags = eMTCF_Invalid;
			return mTimeCode;
		}

		int nValue = 0;
		int nDigit = 1;
		int nOrder = 0;
		int n60FPS_Flags = -1;

		bool bNoSeparator = StrSpn( _pszTime, L"012345679" ) == lstrlen(_pszTime);

		const TCHAR* pDuration = _pszTime + lstrlen(_pszTime);
		while( pDuration >= _pszTime )
		{
			if( *pDuration >= _T('0') && *pDuration <= _T('9') )
			{
				nValue += (*pDuration - _T('0')) * nDigit;
				nDigit *= 10;
			}

			if( *pDuration == _T('.') )
			{
				n60FPS_Flags = nValue > 0 ? 1 : 0;
				nValue = 0;
				nDigit = 1;
			}

			if( *pDuration == _T(';') && _nDropFrame < 0 )
				_nDropFrame = 1;

			if( *pDuration == _T(':') || *pDuration == _T(';') ||
				*pDuration == _T('H') || *pDuration == _T('h') ||
				*pDuration == _T('M') || *pDuration == _T('m') ||
				*pDuration == _T('S') || *pDuration == _T('s') ||
				*pDuration == _T('F') || *pDuration == _T('f') ||
				pDuration == _pszTime || (nDigit >= 100 && bNoSeparator) )
			{
				if( nOrder == 0 )
					mTimeCode.nFrames = nValue;
				else if( nOrder == 1 )
					mTimeCode.nSeconds = nValue;
				else if( nOrder == 2 )
					mTimeCode.nMinutes = nValue;
				else if( nOrder == 3 )
					mTimeCode.nHours = nValue;

				if( *pDuration == _T('H') || *pDuration == _T('h') )
					nOrder = 3;
				else if( *pDuration == _T('M') || *pDuration == _T('m') )
					nOrder = 2;
				else if( *pDuration == _T('S') || *pDuration == _T('s') )
					nOrder = 1;
				else if( *pDuration == _T('F') || *pDuration == _T('f') )
					nOrder = 0;
				else // ':' or ';' separator
				{
					if( nOrder == 0 && *pDuration == _T(':') && _nDropFrame < 0 )
						_nDropFrame = 0;

					nOrder++;
				}

				nValue = 0;
				nDigit = 1;
			}

			pDuration--;
		}

		if( _dblRate > 0 )
		{
			int nBase = DBL2INT( _dblRate );
			if( (nBase != 30 && nBase != 60) || (nBase - _dblRate) < 0.005 )
				_nDropFrame = 0;

			Rate2Flags( nBase, _nDropFrame > 0, &mTimeCode.eTCFlags );

			if( nBase > 45 && n60FPS_Flags >= 0)
				SET_FLAG( &mTimeCode.eTCFlags, n60FPS_Flags > 0 ? (eMTCF_Progressive_Odd_Frame|eMTCF_Progressive_Even_Frame) : eMTCF_Progressive_Even_Frame, 1 );

			// Correct values
			NormilizeTC( mTimeCode );
		}
		else if( n60FPS_Flags >= 0 )
		{
			SET_FLAG(&mTimeCode.eTCFlags, n60FPS_Flags > 0 ? (eMTCF_Progressive_Odd_Frame|eMTCF_Progressive_Even_Frame) : eMTCF_Progressive_Even_Frame, 1 );
		}


		if( _nDropFrame != 0 )
			PutFlag( &mTimeCode.eTCFlags, eMTCF_DropFrame );
		else
			RemoveFlag( &mTimeCode.eTCFlags, eMTCF_DropFrame );
		
		
		return mTimeCode;
	}

	static CComBSTR TC2StrLog(M_TIMECODE* _pTimecode)
	{
		if (!_pTimecode)
			return L"NULL";

		if (!TC_IS_VALID(_pTimecode))
			return L"no";

		WCHAR szTimecodeInfo[128] = {};

		WCHAR szField[16] = {};
		int nField = ((eMTCF_Rate & _pTimecode->eTCFlags) >> 8);
		if (nField > 0)
			_snwprintf(szField, SIZEOF_ARRAY(szField), L".%d", nField - 1);

		int nBaseRate = Flags2BaseRate(_pTimecode->eTCFlags);

		int nFrameNo = Tc2Frame(*_pTimecode);

		// 10:00:00;10.0 (12344) 50NDF SRC UserBits:0x12344/Extra:12344
		_snwprintf(szTimecodeInfo, SIZEOF_ARRAY(szTimecodeInfo), L"%ws%ws (%d) %ws%ws %ws%ws%ws", 
			TC2STR(*_pTimecode), szField, nFrameNo,
			nBaseRate > 0 ? INT2STRW(nBaseRate) : L"",
			TC_IS_DROP(_pTimecode) ? L"DF" : L"NDF",
			_pTimecode->eTCFlags & eMTCF_SrcTC ? L"SRC " :
			_pTimecode->eTCFlags & eMTCF_UserTC ? L"USER " :
			_pTimecode->eTCFlags & eMTCF_LocalTimeTC ? L"TIME " : 
			_pTimecode->eTCFlags & eMTCF_LocalTimeSyncTC ? L"SYNC-TIME " : L"",
			_pTimecode->eTCFlags & eMTCF_SrcTCUserbits ? L"UserBits:" : nFrameNo != _pTimecode->nExtraCounter ? L"ExtCnt:" : L"",
			_pTimecode->eTCFlags & eMTCF_SrcTCUserbits ? HEX2STRW(_pTimecode->nExtraCounter) : nFrameNo != _pTimecode->nExtraCounter ? INT2STRW(_pTimecode->nExtraCounter) : L"");

		return szTimecodeInfo;
	}

	// Convert system time to timecode 
	// Note:	- if _pSysTime is NULL -> use localtime
	//			- number of frames returned in M_TIMECODE::nExtraCounter
	static M_TIMECODE SysTime2TC(SYSTEMTIME* _pSysTime, REFERENCE_TIME _rtAvgTimePerFrame, bool _bDropFrame )
	{
		SYSTEMTIME sysTime = {};
		if (_pSysTime)
			sysTime = *_pSysTime;
		else
			GetLocalTime(&sysTime);

		int nFrames = _rtAvgTimePerFrame > 0 ? DBL2INT((double)DS_MSEC * sysTime.wMilliseconds / _rtAvgTimePerFrame) : 0;
		M_TIMECODE mTC_Time = { sysTime.wHour, sysTime.wMinute, sysTime.wSecond, nFrames };
		mTC_Time.nExtraCounter = MTCHelp::Tc2Frame(mTC_Time, _rtAvgTimePerFrame, _bDropFrame);
		M_TIMECODE mTC_Check = MTCHelp::Frame2Tc(mTC_Time.nExtraCounter, _rtAvgTimePerFrame, _bDropFrame);
		return mTC_Time;
	}
};

class MTCFiller : public AutoObjBase<MTCFiller>
{
private:
	int	m_nStartTC;
	int	m_nNextTC;

public:
	MTCFiller()
	{
		ResetTC();
	}
	~MTCFiller()
	{
	}

	// Return:
	// -1 - Error
	//  0 - do not update, 
	//	1(5) - time_sync(sync start, monotonic increase), 
	//	2(6) - time_sync(sync start, use frame time), 
	//	3(7) - time_sync(sync each frame, max 4 frames diff), 
	//  +4 - offset updated (could be combined with 1,2,3) e.g. 5,6,7 
	int UpdateTC(IUnknown* _pFrameIn, XMLParse::XMLNodePtr _pXMLProps)
	{
		ATLASSERT(_pXMLProps);
		CComQIPtr<IMFFrame> qpMFFrameIn = _pFrameIn;
		if (!qpMFFrameIn || !_pXMLProps )
			return -1;

		M_TIME mTime = MFFrame_Time(qpMFFrameIn);
		REFERENCE_TIME rtFrame = MFFrame_AvgTimeRT( qpMFFrameIn );

		//	1 - time_sync(sync start, monotonic increase), 
		//	2 - time_sync(sync start, use frame time), 
		//	3 - time_sync(sync each frame, max 4 frames diff), 
		int nTimeSync = _pXMLProps->GetIntAttribute(L"timecode.time_sync");
		if (nTimeSync > 2) // Sync by clock and do not have difference more than 4 frames
		{
			// Use next frame TC
			M_TIMECODE mTC_Time = MTCHelp::SysTime2TC(NULL, rtFrame, true);
			if (m_nNextTC < 0 || ABS(m_nNextTC - mTC_Time.nExtraCounter) >= 4 || _pXMLProps->RemoveAttribute(L"timecode.restart") )
			{
				// First frame, of difference too much -> use TC from system time
				MTRACE("CORRECT TIMECODE (time_sync=2) Time:%s(%d) TC:%s(%d)",
					MTC2STR(mTC_Time), mTC_Time.nExtraCounter, MTC2STR(MTCHelp::Frame2Tc(m_nNextTC, rtFrame, true)), m_nNextTC);

				// Do not reduce timecode -> use several frames with same timecode
				// - check start TC for changes from 1 to 2
				if (m_nStartTC < 0)
					m_nNextTC = MAX(m_nNextTC - 1, mTC_Time.nExtraCounter);
				else
					m_nNextTC = mTC_Time.nExtraCounter;
			}

			mTime.tcFrame = MTCHelp::Frame2Tc(m_nNextTC, rtFrame, true); // Always use drop-frame timecode as more closed to real time
			SET_FLAG(&mTime.tcFrame.eTCFlags, eMTCF_LocalTimeSyncTC, true);

			++m_nNextTC;
			m_nStartTC = -1; // For reset TC counter if user change from 2 to 1
		}
		else if (nTimeSync > 0 || (mTime.tcFrame.eTCFlags == eMTCF_Invalid) ) // Sync start by clock, next timecode is monotonic increased
		{
			bool bDropFrameTC = _pXMLProps->GetBoolAttribute(L"timecode.drop_frame", true);
			// Update TC
			if ( m_nStartTC < 0 || m_nNextTC < 0 || _pXMLProps->RemoveAttribute( L"timecode.restart" ) )
			{
				if (IS_LIVE(&mTime) || nTimeSync > 0)
				{
					M_TIMECODE mTC_Time = MTCHelp::SysTime2TC(NULL, rtFrame, bDropFrameTC);

					int nFrameIdx = DBL2INT64((double)mTime.rtStartTime / rtFrame);
					if( nTimeSync == 1 )
						m_nNextTC = m_nStartTC = mTC_Time.nExtraCounter;
					else
						m_nNextTC = m_nStartTC = mTC_Time.nExtraCounter - nFrameIdx;
						
					// First frame, or restart -> use TC from system time
					MTRACE("START TIMECODE (time_sync=1) Time:%s(%d) TC:%s(%d) Frame:%d",
						MTC2STR(mTC_Time), mTC_Time.nExtraCounter, MTC2STR(MTCHelp::Frame2Tc(m_nNextTC, rtFrame, true)), m_nNextTC, nFrameIdx);
				}
				else
				{
					m_nNextTC = m_nStartTC = 0;
				}
			}

			int nTCFrame = nTimeSync == 1 ? m_nNextTC : (m_nStartTC + DBL2INT64((double)mTime.rtStartTime / rtFrame));

			// Try to have monotonic TC 
			// e.g. instead of 12:00:00:00, 12:00:00:00, 12:00:00:02 -> 12:00:00:00, 12:00:00:01, 12:00:00:02
			if (ABS(nTCFrame - m_nNextTC) <= 1)
				nTCFrame = m_nNextTC;

			mTime.tcFrame = MTCHelp::Frame2Tc(nTCFrame, rtFrame, bDropFrameTC);
			SET_FLAG(&mTime.tcFrame.eTCFlags, eMTCF_LocalTimeTC, true);

			m_nNextTC = nTCFrame + 1;
		}
		else
		{
			m_nNextTC = -1;
			m_nStartTC = -1;
		}


		mTime.tcFrame.nHours = ((mTime.tcFrame.nHours % 24) + 24) % 24;


		LPCWSTR pszTCOffset = _pXMLProps->GetStringAttribute(L"timecode.offset");
		if ( !STR_IS_EMPTY( pszTCOffset ) )
		{
			bool bNegative = false;
			if (pszTCOffset[0] == L'-')
			{
				bNegative = true;
				pszTCOffset++;
			}

			bool bWrapTC = true;
			if (pszTCOffset[0] == L'+')
			{
				bWrapTC = false;
				pszTCOffset++;
			}

			bool bDrop = TC_IS_DROP(&mTime.tcFrame);
			int nFramesTC = MTCHelp::Tc2Frame(mTime.tcFrame, rtFrame, bDrop);
			M_TIMECODE tcOffset = MTCHelp::Str2Tc(pszTCOffset, rtFrame, bDrop);
			int nFramesOffset = MTCHelp::Tc2Frame(tcOffset);
			if (bNegative)
			{
				M_TIMECODE tc24Offset = MTCHelp::Str2Tc(L"24:00:00:00", rtFrame, bDrop);
				int nFramesOffset24 = MTCHelp::Tc2Frame(tc24Offset);

				nFramesTC += nFramesOffset24 - nFramesOffset;
			}
			else
			{
				nFramesTC += nFramesOffset;
			}

			mTime.tcFrame = MTCHelp::Frame2Tc(nFramesTC, rtFrame, bDrop);
			if (bWrapTC)
				mTime.tcFrame.nHours = ((mTime.tcFrame.nHours % 24) + 24) % 24;

			nTimeSync |= 4;
		}

		if (nTimeSync > 0 || (mTime.tcFrame.eTCFlags == eMTCF_Invalid) )
		{
			MTRACE("Update TC:%ws->%ws", MTCHelp::TC2StrLog(&MFFrame_Time(qpMFFrameIn).tcFrame).m_str, MTCHelp::TC2StrLog(&mTime.tcFrame).m_str);

			qpMFFrameIn->MFTimeSet(&mTime);
		}

		return nTimeSync;
	}

	void ResetTC()
	{
		m_nNextTC = -1;
		m_nStartTC = -1;
	}
	
};

#endif // _MTIMECODE_TC_H_
