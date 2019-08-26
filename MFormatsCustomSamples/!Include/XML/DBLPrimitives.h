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

#ifndef _DBL_PRIMITIVES_H_
#define _DBL_PRIMITIVES_H_

#pragma once

#ifndef GetAlphaValue
#define GetAlphaValue(argb)      (LOBYTE(((DWORD)(argb))>>24))
#endif

#ifndef RGBA
#define RGBA(r,g,b, alpha) ((COLORREF)( ((DWORD)r & 0xFF)|(((DWORD)g & 0xFF)<<8)|(((DWORD)b & 0xFF)<<16)|(((DWORD)alpha & 0xFF)<<24) ) )
#endif

#ifndef MAX
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN 
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif


struct POINT_DBL
{
	double x;
	double y;

	operator POINT () const 
	{ 
		POINT ptRet = {DBL2INT(x), DBL2INT(y)};
		return ptRet;
	}

	POINT Fix()
	{
		POINT ptRet = {int(x), int(y)};
		return ptRet;
	}

	bool IsEqualInt( POINT_DBL& _crPos ) const 
	{
		return DBL2INT(x) == DBL2INT(_crPos.x) && DBL2INT(y) == DBL2INT(_crPos.y);
	}

	bool AddDelta( POINT_DBL& _delta, const POINT_DBL& _dest, bool* _pbUpdate )
	{
		POINT_DBL prev = *this;

		x += _delta.x; 
		if( (_delta.x > 0 && x >= _dest.x) || (_delta.x < 0 && x <= _dest.x) )
		{
			_delta.x = 0;
			x = _dest.x;
		}
		y += _delta.y; 
		if( (_delta.y > 0 && y >= _dest.y) || (_delta.y < 0 && y <= _dest.y) )
		{
			_delta.y = 0;
			y = _dest.y;
		}

		if( _pbUpdate )
			*_pbUpdate = !IsEqualInt( prev );

		return _delta.x != 0 || _delta.y != 0;
	}

	POINT_DBL CalcDelta( const POINT_DBL& _dest, int nSteps )
	{
		ATLASSERT( nSteps > 0 );

		POINT_DBL delta = 
		{
			(_dest.x - x) / nSteps,
			(_dest.y - y) / nSteps,
		};

		return delta;
	}

	bool operator != ( POINT_DBL& _crPos ) const 
	{
		return x != _crPos.x || y != _crPos.y;
	}

	POINT_DBL& operator += ( POINT& _crPos )
	{
		x += _crPos.x;
		y += _crPos.y;

		return *this;
	}

	POINT_DBL& operator -= ( POINT& _crPos )
	{
		x -= _crPos.x;
		y -= _crPos.y;

		return *this;
	}

	inline void Reset()
	{
		::ZeroMemory( this, sizeof(*this) );
	}
};

struct SIZE_DBL
{
	double cx;
	double cy;

	operator SIZE () const 
	{ 
		SIZE szRet = {DBL2INT(cx), DBL2INT(cy)};
		return szRet;
	}

	bool IsEqualInt( SIZE_DBL& _crSize ) const 
	{
		return DBL2INT(cx) == DBL2INT(_crSize.cx) && DBL2INT(cy) == DBL2INT(_crSize.cy);
	}

	bool AddDelta( SIZE_DBL& _delta, const SIZE_DBL& _dest, bool* _pbUpdate )
	{
		SIZE_DBL prev = *this;

		cx += _delta.cx; 
		if( (_delta.cx > 0 && cx >= _dest.cx) || (_delta.cx < 0 && cx <= _dest.cx) )
		{
			_delta.cx = 0;
			cx = _dest.cx;
		}
		cy += _delta.cy; 
		if( (_delta.cy > 0 && cy >= _dest.cy) || (_delta.cy < 0 && cy <= _dest.cy) )
		{
			_delta.cy = 0;
			cy = _dest.cy;
		}
		
		if( _pbUpdate )
			*_pbUpdate = !IsEqualInt( prev );

		return _delta.cx != 0 || _delta.cy != 0;
	}

	SIZE_DBL CalcDelta( const SIZE_DBL& _dest, int nSteps )
	{
		ATLASSERT( nSteps > 0 );
		
		SIZE_DBL delta = 
		{
			(_dest.cx - cx) / nSteps,
			(_dest.cy - cy) / nSteps,
		};

		return delta;
	}

	bool operator != ( SIZE_DBL& _crPos ) const 
	{
		return cx != _crPos.cx || cy != _crPos.cy;
	}

	inline void Reset()
	{
		::ZeroMemory( this, sizeof(*this) );
	}
};

struct COLOR_DBL
{
	double r;
	double g;
	double b;
	double a;

	COLOR_DBL( double _a, double _r, double _g, double _b ):
		a( _a ), r( _r ), g( _g ), b( _b )
	{
	}
	COLOR_DBL( COLORREF _nColor = 0)
	{
		a = GetAlphaValue(_nColor);
		r = GetRValue(_nColor);
		g = GetGValue(_nColor);
		b = GetBValue(_nColor);
	}

	operator int() const 
	{
		return RGBA( DBL2INT( r ), DBL2INT( g ),DBL2INT( b ),DBL2INT( a ) );
	}

	bool IsEqualInt( COLOR_DBL& _crPos ) const 
	{
		return DBL2INT(a) == DBL2INT(_crPos.a) && DBL2INT(r) == DBL2INT(_crPos.r) &&
			DBL2INT(g) == DBL2INT(_crPos.g) && DBL2INT(b) == DBL2INT(_crPos.b);
	}

	bool AddDelta( COLOR_DBL& _delta, const COLOR_DBL& _dest, bool* _pbUpdate )
	{
		COLOR_DBL prev = *this;

		a += _delta.a; 
		if( (_delta.a > 0 && a >= _dest.a) || (_delta.a < 0 && a <= _dest.a) )
		{
			_delta.a = 0;
			a = _dest.a;
		}
		r += _delta.r; 
		if( (_delta.r > 0 && r >= _dest.r) || (_delta.r < 0 && r <= _dest.r) )
		{
			_delta.r = 0;
			r = _dest.r;
		}
		g += _delta.g; 
		if( (_delta.g > 0 && g >= _dest.g) || (_delta.g < 0 && g <= _dest.g) )
		{
			_delta.g = 0;
			g = _dest.g;
		}
		b += _delta.b; 
		if( (_delta.b > 0 && b >= _dest.b) || (_delta.b < 0 && b <= _dest.b) )
		{
			_delta.b = 0;
			b = _dest.b;
		}

		if( _pbUpdate )
			*_pbUpdate = !IsEqualInt( prev );

		return _delta.a != 0 || _delta.r != 0;
	}

	COLOR_DBL CalcDelta( const COLOR_DBL& _dest, int nSteps )
	{
		ATLASSERT( nSteps > 0 );

		COLOR_DBL delta( (_dest.a - a) / nSteps, (_dest.r - r) / nSteps, (_dest.g - g) / nSteps, (_dest.b - b) / nSteps );

		return delta;
	}

	inline void Reset()
	{
		::ZeroMemory( this, sizeof(*this) );
	}
};


struct RECT_DBL
{
	double left;
	double top;
	double right;
	double bottom;			

	RECT_DBL& operator = ( const RECT& _crRect )  
	{ 
		left = _crRect.left; 
		top = _crRect.top; 
		right = _crRect.right; 
		bottom = _crRect.bottom; 
		return *this;
	}

	bool operator == ( const RECT_DBL& _crRect ) const 
	{ 
		return left == _crRect.left &&
			top == _crRect.top &&
			right == _crRect.right && 
			bottom == _crRect.bottom;  
	}

	bool operator != ( const RECT_DBL& _crRect ) const 
	{ 
		return !(*this == _crRect);
	}

	inline RECT_DBL Intersect( const RECT_DBL& _crRect ) const 
	{
		RECT_DBL rcRes = {0};
		rcRes.left = MIN( MAX( left, _crRect.left ), _crRect.right );
		rcRes.right = MAX( MIN( right, _crRect.right ), _crRect.left );
		rcRes.top = MIN( MAX( top, _crRect.top ), _crRect.bottom );
		rcRes.bottom = MAX( MIN( bottom, _crRect.bottom ), _crRect.top );

		return rcRes;
	}

	inline RECT_DBL Union( const RECT_DBL& _crRect ) const 
	{
		RECT_DBL rcRes = {0};
		rcRes.left = MIN( left, _crRect.left );
		rcRes.right = MAX( right, _crRect.right );
		rcRes.top = MIN( top, _crRect.top );
		rcRes.bottom = MAX( bottom, _crRect.bottom );

		return rcRes;
	}

	inline RECT_DBL Add( double _dX, double _dY ) const 
	{
		RECT_DBL rcRes = {0};
		rcRes.left += _dX; 
		rcRes.top += _dY; 
		rcRes.right += _dX; 
		rcRes.bottom += _dY; 

		return rcRes;
	}

	inline void Reset()
	{
		left = 0;
		right = 0;
		top = 0;
		bottom = 0;
	}

	inline bool IsEmpty() const 
	{
		if( left >= right ) return true;
		if( top >= bottom ) return true;
		return false;
	}

	inline bool IsZero() const 
	{
		if( DBL2INT( left ) == 0 && DBL2INT( right ) == 0 && DBL2INT( top ) == 0 && DBL2INT( bottom ) == 0 )
			return true;

		return false;
	}

	inline operator bool () const 
	{
		return !IsEmpty();
	}

	int ClearMirror()
	{
		double temp = 0;
		int nRes = 0;
		if( right < left )
		{
			temp = right; right = left; left = temp;
			nRes |= 1;
		}
		if( top > bottom )
		{
			temp = bottom; bottom = top; top = temp;
			nRes |= 2;
		}

		return nRes;
	}

	bool AddDelta( RECT_DBL& _crDelta, const RECT_DBL& _crDest, bool* _pbUpdate )
	{
		RECT_DBL rcPrev = *this;

		left += _crDelta.left; 
		if( (_crDelta.left > 0 && left >= _crDest.left) || (_crDelta.left < 0 && left <= _crDest.left) )
		{
			_crDelta.left = 0;
			left = _crDest.left;
		}
		top += _crDelta.top; 
		if( (_crDelta.top > 0 && top >= _crDest.top) || (_crDelta.top < 0 && top <= _crDest.top) )
		{
			_crDelta.top = 0;
			top = _crDest.top;
		}
		right += _crDelta.right; 
		if( (_crDelta.right > 0 && right >= _crDest.right) || (_crDelta.right < 0 && right <= _crDest.right) )
		{
			_crDelta.right = 0;
			right = _crDest.right;
		}
		bottom += _crDelta.bottom; 
		if( (_crDelta.bottom > 0 && bottom >= _crDest.bottom) || (_crDelta.bottom < 0 && bottom <= _crDest.bottom) )
		{
			_crDelta.bottom = 0;
			bottom = _crDest.bottom;
		}

		if( _pbUpdate )
			*_pbUpdate = !IsEqualInt( rcPrev );

		return _crDelta.left != 0 || _crDelta.top != 0 || _crDelta.right != 0 || _crDelta.bottom != 0;
	}

	RECT_DBL CalcDelta( const RECT_DBL& _crDest, int nSteps, const PIC_PROPS* _pVideoProps )
	{
		ATLASSERT( nSteps > 0 );
		RECT_DBL rcDest = _crDest;
		if( IsEmpty() && _pVideoProps ) 
		{
			left = 0;
			right = _pVideoProps->nWidth;
			top = 0;
			bottom = ABS(_pVideoProps->nHeight);
		}

		if( rcDest.IsEmpty() && _pVideoProps )				
		{
			rcDest.left = 0;
			rcDest.right = _pVideoProps->nWidth;
			rcDest.top = 0;
			rcDest.bottom = ABS(_pVideoProps->nHeight);
		}

		RECT_DBL rcDelta = 
		{
			(rcDest.left - left) / nSteps,
			(rcDest.top - top) / nSteps,
			(rcDest.right - right) / nSteps,
			(rcDest.bottom - bottom) / nSteps,
		};

		return rcDelta;
	}

	operator RECT() const
	{
		RECT rc = {0};
		rc.left = DBL2INT( left );
		rc.top = DBL2INT( top );
		rc.right = DBL2INT( right );
		rc.bottom = DBL2INT( bottom );
		return rc;
	}

	bool IsEqualInt( RECT_DBL& _crRect ) const 
	{
		return DBL2INT(left) == DBL2INT(_crRect.left) && 
			DBL2INT(top) == DBL2INT(_crRect.top) && 
			DBL2INT(right) == DBL2INT(_crRect.right) && 
			DBL2INT(bottom) == DBL2INT(_crRect.bottom);
	}
};

#endif _DBL_PRIMITIVES_H_
