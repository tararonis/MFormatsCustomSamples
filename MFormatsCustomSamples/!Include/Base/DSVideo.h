//---------------------------------------------------------------------------
// This file is part of the MediaLooks DirectShow Template Library (DSTL)
//
// DSVideo.h : base class and functions for video conversions
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

#ifndef __DSVIDEO_H_
#define __DSVIDEO_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\WinAPI\Guards.h"
#include "..\Base\DSDefines.h"

/////////////////////////////////////////////////////////////////////////////
SIZE __stdcall MinAspectRatio(LONG nWidth, LONG nHeight);

/////////////////////////////////////////////////////////////////////////////
typedef struct VID_TYPE
{
	// First DWORD code from subtype GUID (also for RGB).
	// Can be equal to FOURCC_UNK for compressed formats (depends from function)
	DWORD			fccType;

	// Width of picture in pixels
	ULONG			nWidth;

	// Height of picture in pixels
	// For RGB:
	//	positive value: bottom-up orientation
	//	negative value: top-down orientation
	LONG			nHeight;

	// Interlace flags
	DWORD			dwInterlace;

	// Average time per frame
	REFERENCE_TIME	tAvg;

	// Aspect ratio data
	SIZE			ar;

	// Bitrate
	DWORD			nBitRate;
} VID_TYPE;

/////////////////////////////////////////////////////////////////////////////
// #define MIN_HORZ_ALIGNMENT		4
// #define MIN_VERT_ALIGNMENT		2

typedef struct tagPictureFormat2
{
	const GUID*	pSubtype;
	WORD		wBitCount;				// == BITMAPINFOHEADER.biBitCount

	BYTE		nHorzYtoUV;				// Numbers of Y data to U or V data at horizontal
	BYTE		nVertYtoUV;				// Numbers of Y data to U or V data at vertical

	BYTE		anLayerShift[3];		// YUV or RGB bytes initial shift
	BYTE		anLayerStep[3];			// YUV or RGB bytes step

	bool		bPlanarFormat;
	bool		bYUVFormat;				// true : YUV, false : RGB
} PICTURE_FORMAT_2;

/////////////////////////////////////////////////////////////////////////////
class CPictureFormats
{
public:
	static const PICTURE_FORMAT_2 s_arrSupportedFormats[];
	static const int s_cFormats;

	//---------------------------------
	// Alignment checked from nHorzYtoUV and nVertYtoUV parameters
	static int Test(FOURCC fccType, DWORD nWidth, DWORD nHeight, DWORD nRowWidth)
	{
		if(nWidth == 0 || nHeight == 0)
			return -1;
		if(nRowWidth != 0 && nRowWidth < nWidth)
			return -1;

		int nFormat = Find(fccType);
		if(nFormat == -1)
			return -1;

		const PICTURE_FORMAT_2& rgFormat = s_arrSupportedFormats[nFormat];
		if( (nWidth%rgFormat.nHorzYtoUV) != 0 || (nRowWidth != 0 && (nRowWidth%rgFormat.nHorzYtoUV) != 0) )
			return -1;
		if( (nHeight%rgFormat.nVertYtoUV) != 0)
			return -1;

		return nFormat;
	}

	//---------------------------------
	// Alignment checked from nHorzYtoUV and nVertYtoUV parameters
	static int TestMT(const AM_MEDIA_TYPE* pmt, long* pnWidth = NULL, long* pnHeight = NULL, long* pnRowWidth = NULL)
	{
		ATLASSERT(pmt != NULL);
		if(pmt == NULL) return -1;

		if(pmt->majortype != MEDIATYPE_Video)
			return -1;
		if(pmt->cbFormat == 0 || pmt->pbFormat == NULL)
			return -1;

		//---------------------------------
		int nWidth = 0;
		int nHeight = 0;
		int nRowWidth = 0;

		if(pmt->formattype == FORMAT_VideoInfo)
		{
			ATLASSERT(pmt->cbFormat >= sizeof(VIDEOINFOHEADER));
			if(pmt->cbFormat < sizeof(VIDEOINFOHEADER)) return -1;

			VIDEOINFOHEADER* pHeader = (VIDEOINFOHEADER*)pmt->pbFormat;
			nWidth = pHeader->rcTarget.right - pHeader->rcTarget.left;
			nRowWidth = pHeader->bmiHeader.biWidth;
			nHeight = pHeader->bmiHeader.biHeight;
		}
		else if(pmt->formattype == FORMAT_VideoInfo2)
		{
			ATLASSERT(pmt->cbFormat >= sizeof(VIDEOINFOHEADER2));
			if(pmt->cbFormat < sizeof(VIDEOINFOHEADER2)) return -1;

			VIDEOINFOHEADER2* pHeader = (VIDEOINFOHEADER2*)pmt->pbFormat;
			nWidth = pHeader->rcTarget.right - pHeader->rcTarget.left;
			nRowWidth = pHeader->bmiHeader.biWidth;
			nHeight = pHeader->bmiHeader.biHeight;
		}
		else
			return -1;

		//---------------------------------
		if(nRowWidth == 0 || nHeight == 0)
			return -1;

		if(nWidth == 0)
			nWidth = nRowWidth;
		else if(nWidth > nRowWidth)
			return -1;

		int nFormat = Find(pmt->subtype);
		if(nFormat == -1)
			return -1;

		//---------------------------------
		const PICTURE_FORMAT_2& rgFormat = s_arrSupportedFormats[nFormat];

		// For YUV formats use positive heights
		if(rgFormat.bYUVFormat && nHeight < 0)
			nHeight = -nHeight;

		if((nWidth%rgFormat.nHorzYtoUV) != 0 || (nRowWidth%rgFormat.nHorzYtoUV) != 0)
			return -1;
		if((nHeight%rgFormat.nVertYtoUV) != 0)
			return -1;

		if(pnWidth != NULL)
			*pnWidth = nWidth;
		if(pnHeight != NULL)
			*pnHeight = nHeight;
		if(pnRowWidth != NULL)
			*pnRowWidth = nRowWidth;
		return nFormat;
	}

	// Alignment checked from nHorzYtoUV and nVertYtoUV parameters
	static int TestMT(const AM_MEDIA_TYPE* pmt, PIC_PROPS* pPicProps)
	{
		ATLASSERT(pPicProps != NULL);
		if(pPicProps == NULL) return E_POINTER;

		int nType = TestMT(pmt, (long*)&pPicProps->nWidth, (long*)&pPicProps->nHeight, (long*)&pPicProps->nRowWidth);
		if(nType != -1)
			pPicProps->fccType = GUID2FOURCC(*s_arrSupportedFormats[nType].pSubtype);
		return nType;
	}

	//---------------------------------
	// Alignment checked from nHorzAlign and nVertAlign parameters
	static int TestAlign(FOURCC fccType, DWORD nWidth, int nHeight, DWORD nRowWidth)
	{
		if(nWidth == 0 || nHeight == 0)
			return -1;
		if(nRowWidth != 0 && nRowWidth < nWidth)
			return -1;

		int nFormat = Find(fccType);
		if(nFormat == -1)
			return -1;

		const PICTURE_FORMAT_2& rgFormat = s_arrSupportedFormats[nFormat];
		if( (nWidth%rgFormat.nHorzYtoUV) != 0 || (nRowWidth != 0 && (nRowWidth%rgFormat.nHorzYtoUV) != 0) )
			return -1;
		if( (nHeight%rgFormat.nVertYtoUV) != 0)
			return -1;

		return nFormat;
	}

	static DWORD AlignedWidth(FOURCC fccType, DWORD nWidth)
	{
		DWORD nAlignedWidth = 0;
		int nFormat = Find(fccType);
		if(nFormat != -1)
			nAlignedWidth = ALIGN_UP(nWidth, s_arrSupportedFormats[nFormat].nHorzYtoUV);
		return nAlignedWidth;
	}

	// Alignment tests and corrections of sizes makes from nHorzAlign and nVertAlign parameters
	static int CorrectAlign(FOURCC fccType, DWORD& nWidth, DWORD& nHeight, DWORD& nRowWidth)
	{
		if(nWidth == 0 || nHeight == 0)
			return -1;
		if(nRowWidth != 0 && nRowWidth < nWidth)
			return -1;

		int nFormat = Find(fccType);
		if(nFormat == -1)
			return -1;

		//---------------------------------
		const PICTURE_FORMAT_2& rgFormat = s_arrSupportedFormats[nFormat];

		if(nWidth % rgFormat.nHorzYtoUV != 0)
			nWidth = ALIGN_UP(nWidth, rgFormat.nHorzYtoUV);

		if(nRowWidth != 0 && nRowWidth % rgFormat.nHorzYtoUV != 0)
			nRowWidth = ALIGN_UP(nRowWidth, rgFormat.nHorzYtoUV);
        if(nRowWidth == 0)
			nRowWidth = nWidth;

		if(nHeight%rgFormat.nVertYtoUV != 0)
		{
			if((LONG)nHeight > 0)
				nHeight = ALIGN_DOWN(nHeight, rgFormat.nVertYtoUV);
			else
				nHeight = ALIGN_UP(nHeight, rgFormat.nVertYtoUV);
		}

		return nFormat;
	}

	// Alignment tests and corrections of sizes makes from nHorzAlign and nVertAlign parameters
	static bool CorrectRectAlign(FOURCC fccType, RECT& rcSrc)
	{
		int nFormat = Find(fccType);
		if(nFormat == -1)
			return false;

		const PICTURE_FORMAT_2& rgFormat = s_arrSupportedFormats[nFormat];

		if(rcSrc.left%rgFormat.nHorzYtoUV != 0)
			rcSrc.left = ALIGN_UP(rcSrc.left, rgFormat.nHorzYtoUV);
		if(rcSrc.right%rgFormat.nHorzYtoUV != 0)
			rcSrc.right = ALIGN_DOWN(rcSrc.right, rgFormat.nHorzYtoUV);

		if(rcSrc.top%rgFormat.nVertYtoUV != 0)
			rcSrc.top = ALIGN_UP(rcSrc.top, rgFormat.nVertYtoUV);
		if(rcSrc.bottom%rgFormat.nVertYtoUV != 0)
			rcSrc.bottom = ALIGN_DOWN(rcSrc.bottom, rgFormat.nVertYtoUV);

		return true;
	}

	//---------------------------------
	typedef enum tagFormatGroup
	{
		FG_YUV = 0,
		FG_YUV_PLANAR,
		FG_RGB
	} FORMATGROUP;

	static int GetFormats(FORMATGROUP eForm, int* pArrIdx = NULL)
	{
		bool bYUVFormat = (eForm != FG_RGB);
		bool bPlanarFormat = (eForm == FG_YUV_PLANAR);
		int nIndex = 0;
		for(int i = 0; i < s_cFormats; ++i)
		{
			const PICTURE_FORMAT_2& rgFormat = s_arrSupportedFormats[i];

			if(rgFormat.bYUVFormat == bYUVFormat && rgFormat.bPlanarFormat == bPlanarFormat)
			{
				if(pArrIdx != NULL) pArrIdx[nIndex] = i;
				++nIndex;
			}
		}
		return nIndex;
	}

	//---------------------------------
	static FOURCC Fourcc(const PICTURE_FORMAT_2& rgFormat)
	{
		return GUID2FOURCC(*(rgFormat.pSubtype));
	}
	static int Find(FOURCC fccType)
	{
		for(int i = 0; i < s_cFormats; ++i)
		{
			if(Fourcc(s_arrSupportedFormats[i]) == fccType)
				return i;
		}
		return -1;
	}
	static int Find(const GUID& guidSubtype)
	{
		for(int i = 0; i < s_cFormats; ++i)
		{
			if(IsEqualGUID(guidSubtype, *s_arrSupportedFormats[i].pSubtype))
				return i;
		}
		return -1;
	}

	static WORD BitCount(FOURCC fccType)
	{
		int nFormat = Find(fccType);
		if(nFormat != -1)
			return s_arrSupportedFormats[nFormat].wBitCount;
		return 0;
	}

	static FOURCC FccFromBmp(const BITMAPINFOHEADER* pBmp)
	{
		ATLASSERT(pBmp != NULL);
		if(pBmp == NULL) return FOURCC_UNK;

		// Can't use bmps with color table
		ATLASSERT(pBmp->biClrUsed == 0);
		if(pBmp->biClrUsed != 0) return FOURCC_UNK;

		// Can't use bmps with planes != 1
		ATLASSERT(pBmp->biPlanes == 1);
		if(pBmp->biPlanes != 1) return FOURCC_UNK;

		//---------------------------------
		if(pBmp->biCompression == BI_RGB)
		{
			switch(pBmp->biBitCount)
			{
				case 32:	return GUID2FOURCC(MEDIASUBTYPE_RGB32);
				case 24:	return GUID2FOURCC(MEDIASUBTYPE_RGB24);
				case 16:	return GUID2FOURCC(MEDIASUBTYPE_RGB565);
				case 15:	return GUID2FOURCC(MEDIASUBTYPE_RGB555);
			}

			ATLASSERT(!_T("CPictureFormats::FccFromBmp(): Unsupported BMP::biBitCount"));
			return FOURCC_UNK;
		}

		if( pBmp->biCompression == BI_RLE8 ||
			pBmp->biCompression == BI_RLE4 ||
			pBmp->biCompression == BI_BITFIELDS)
		{
			ATLASSERT(!_T("CPictureFormats::FccFromBmp(): Unsupported BMP::biCompression"));
			return FOURCC_UNK;
		}

		// For known video subtypes dwFCC > MAXWORD
		if(pBmp->biCompression <= MAXWORD)
		{
			ATLASSERT(!_T("CPictureFormats::FccFromBmp(): Unknown BMP::biCompression"));
			return FOURCC_UNK;
		}

		return pBmp->biCompression;
	}

	//---------------------------------
	// Calculation of row size in bytes
	// nWidth - picture width in pixel
	static HRESULT CalcRowSize(FOURCC fccType, DWORD nWidth, DWORD* pcbRowSize)
	{
		ATLASSERT(pcbRowSize != NULL);
		if(pcbRowSize == NULL) return E_POINTER;

		if( fccType == FOURCC_v210)
		{
			*pcbRowSize = ((nWidth + 47) / 48) * 128;
		}
		else if( fccType == FOURCC_r210 )
		{
			*pcbRowSize = ((nWidth + 63) / 64) * 256;
		}
		else
		{
			*pcbRowSize = 0;
			int nFormat = Find(fccType);
			if(nFormat == -1)
				return E_INVALIDARG;
			*pcbRowSize = CalcRowSize(s_arrSupportedFormats[nFormat], nWidth);
		}
		return S_OK;
	}
	static DWORD CalcRowSize(const PICTURE_FORMAT_2& rgFormat, DWORD nWidth)
	{
		ATLASSERT(nWidth % rgFormat.nHorzYtoUV == 0);

		if(rgFormat.bYUVFormat && rgFormat.bPlanarFormat)
			return nWidth;
		
		// Special fix for v210
		if( GUID2FOURCC( *rgFormat.pSubtype ) == FOURCC_v210 )
		{
			nWidth = ((nWidth + 47) / 48) * 128;
		}
		else if( GUID2FOURCC( *rgFormat.pSubtype ) == FOURCC_r210 )
		{
			nWidth = ((nWidth + 63) / 64) * 256;
		}
		else
		{
			nWidth = (nWidth * rgFormat.wBitCount)/8;
		}

		// For correct alignment of RGB24 with nWidth%4 != 0
		return ALIGN_UP(nWidth, sizeof(DWORD));
	}

	//---------------------------------
	// Calculation of image size in bytes
	// nWidth, nHeight - picture width/height in pixel
	static HRESULT CalcImageSize(FOURCC fccType, DWORD nWidth, DWORD nHeight, DWORD* pcbImageSize)
	{
		ATLASSERT(pcbImageSize != NULL);
		if(pcbImageSize == NULL) return E_POINTER;

		*pcbImageSize = 0;
		int nFormat = Find(fccType);
		if(nFormat == -1)
			return E_INVALIDARG;

		*pcbImageSize = CalcImageSize(s_arrSupportedFormats[nFormat], nWidth, nHeight);
		return S_OK;
	}
	static DWORD CalcImageSize(const PICTURE_FORMAT_2& rgFormat, DWORD nWidth, DWORD nHeight)
	{
		DWORD cbRowSize = CalcRowSize(rgFormat, nWidth);

		ATLASSERT(nHeight % rgFormat.nVertYtoUV == 0);

		if((LONG)nHeight < 0)
			nHeight = (DWORD)(-(LONG)nHeight);
		DWORD cbImageSize = cbRowSize * nHeight;
		if(rgFormat.bYUVFormat && rgFormat.bPlanarFormat)
		{
			//cbImageSize += cbRowSize * (nHeight/rgFormat.nVertYtoUV);
			// U and V Plane
			cbImageSize += (nWidth/rgFormat.nHorzYtoUV) * (nHeight/rgFormat.nVertYtoUV) * 2;
		}
			
		return cbImageSize;
	}

	static DWORD PlanarColorShift(const PICTURE_FORMAT_2& rgFormat, bool bUColor,
								  DWORD nWidth, DWORD nHeight)
	{
		ATLASSERT(rgFormat.bYUVFormat && rgFormat.bPlanarFormat);
		ATLASSERT(nWidth%rgFormat.nHorzYtoUV == 0 && nHeight%rgFormat.nVertYtoUV == 0);

		DWORD cbRowSize = CalcRowSize(rgFormat, nWidth);
		if((LONG)nHeight < 0)
			nHeight = (DWORD)(-(LONG)nHeight);
		DWORD cbShift = cbRowSize * nHeight;
		if(!bUColor)
			cbShift += (cbRowSize/rgFormat.nHorzYtoUV) * (nHeight/rgFormat.nVertYtoUV);
		return cbShift;
	}

	// Calculate offset to point in picture buffer in bytes
	// size: size of picture in pixels
	// pt: position of point on picture in pixels
	// nPlane: number of color plane for planar formats (0 -:- 2)
	static DWORD CalcOffset(const PICTURE_FORMAT_2& rgFormat, const SIZE& size,
		const POINT& pt, int nPlane = 0)
	{
		ATLASSERT(pt.x >= 0 && pt.y >= 0);
		ATLASSERT(pt.x < size.cx);
		ATLASSERT(pt.y < size.cy);
		ATLASSERT(size.cx % rgFormat.nHorzYtoUV == 0);
		ATLASSERT(size.cy % rgFormat.nVertYtoUV == 0);

		// Offset to point in picture buffer
		DWORD cbOffset = 0;
		if(rgFormat.bPlanarFormat)
		{
			if(nPlane == 0)
			{
				cbOffset = pt.x + pt.y * size.cx;
			}
			else
			{
				ATLASSERT(nPlane == 1 || nPlane == 2);

				// Size of Y plane
				cbOffset = size.cx * size.cy;

				if(nPlane == 2)
				{
					// Add size of UV plane
					cbOffset += (size.cx/rgFormat.nHorzYtoUV) * (size.cy/rgFormat.nVertYtoUV);
				}

				// Vert offset
				cbOffset += (pt.y/rgFormat.nVertYtoUV) * (size.cx/rgFormat.nHorzYtoUV);
				// Horz offset
				cbOffset += pt.x/rgFormat.nHorzYtoUV;
			}
		}
		else
		{
			ATLASSERT(nPlane == 0);
			// Vert offset
			cbOffset = CalcRowSize(rgFormat, size.cx) * pt.y;
			// Horz offset
			cbOffset += ((pt.x * rgFormat.wBitCount) >> 3);
		}
		return cbOffset;
	}

	// Calculate offset to point in picture buffer in bytes
	// size: size of picture in pixels
	// pt: position of point on picture in pixels
	// nPlane: number of color plane for planar formats (0 -:- 2)
	static HRESULT CalcOffset(FOURCC fccType, const SIZE& size, const POINT& pt,
		int nPlane, DWORD* pcbOffset)
	{
		ATLASSERT(pcbOffset != NULL);
		if(pcbOffset == NULL) return E_POINTER;

		*pcbOffset = 0;
		int nFormat = Find(fccType);
		if(nFormat == -1)
			return E_INVALIDARG;

		*pcbOffset = CalcOffset(s_arrSupportedFormats[nFormat], size, pt, nPlane);
		return S_OK;
	}

	// Calculate offset to point in picture buffer in bytes
	// pt: position of point on picture in pixels
	// nPlane: number of color plane for planar formats (0 -:- 2)
	static DWORD CalcOffset(const PIC_PROPS& rgProps, const POINT& pt, int nPlane = 0)
	{
		ATLASSERT(rgProps.nRowWidth == 0 || rgProps.nRowWidth >= rgProps.nWidth);

		SIZE size;
		size.cx = (rgProps.nRowWidth == 0)? rgProps.nWidth : rgProps.nRowWidth;
		size.cy = (rgProps.nHeight > 0)? rgProps.nHeight : -rgProps.nHeight;

		ATLASSERT(size.cx > 0);
		ATLASSERT(size.cy > 0);
		ATLASSERT(0 <= pt.x && pt.x < size.cx);
		ATLASSERT(0 <= pt.y && pt.y < size.cy);

		int nFormat = Find(rgProps.fccType);
		if(nFormat == -1)
		{
			ATLASSERT(!_T("CPictureFormats::CalcOffset(): Unsupported FOURCC"));
			return 0;
		}

		return CalcOffset(s_arrSupportedFormats[nFormat], size, pt, nPlane);
	}

	//---------------------------------
	static HRESULT CopyData(BYTE* pDst, BYTE* pSrc, int nFormat, DWORD nPictureWidth,
		DWORD nPictureHeight, DWORD nWidthDst = 0, DWORD nWidthSrc = 0)
	{
		ATLASSERT(pSrc != NULL && pDst != NULL);
		if(pSrc == NULL || pDst == NULL) return E_INVALIDARG;

		ATLASSERT(nFormat >= 0 && nFormat < s_cFormats);
		if(nFormat < 0 || nFormat >= s_cFormats) return E_INVALIDARG;

		ATLASSERT(nPictureWidth > 0 && nPictureHeight != 0);
		if(nPictureWidth == 0 || nPictureHeight == 0) return E_INVALIDARG;

		if((LONG)nPictureHeight < 0)
			nPictureHeight = (DWORD)(-(LONG)nPictureHeight);

		if(nWidthSrc == 0)
			nWidthSrc = nPictureWidth;
		if(nWidthDst == 0)
			nWidthDst = nPictureWidth;

		ATLASSERT(nPictureWidth <= nWidthSrc && nPictureWidth <= nWidthDst);
		if(nPictureWidth > nWidthSrc || nPictureWidth > nWidthDst) return E_INVALIDARG;

		//---------------------------------
		const PICTURE_FORMAT_2& rgFormat = s_arrSupportedFormats[nFormat];
		DWORD cbSrcRowSize = CalcRowSize(rgFormat, nWidthSrc);
		DWORD cbDstRowSize = CalcRowSize(rgFormat, nWidthDst);
		DWORD cbPictureRowSize = CalcRowSize(rgFormat, nPictureWidth);

		if(cbSrcRowSize == cbDstRowSize && cbSrcRowSize <= cbPictureRowSize + 32)
		{
			// More optimal make copy of all source buffer
			if(rgFormat.bPlanarFormat)
			{
				ATLASSERT(nPictureHeight % rgFormat.nVertYtoUV == 0);
				if(!SEH_CopyMemory(pDst, pSrc, cbSrcRowSize * (nPictureHeight + nPictureHeight/2)))
					return E_UNEXPECTED;
			}
			else
			{
				if(!SEH_CopyMemory(pDst, pSrc, cbSrcRowSize * nPictureHeight))
					return E_UNEXPECTED;
			}
		}
		else
		{
			DWORD h = 0;
			for(; h < nPictureHeight; ++h)
			{
				if(!SEH_CopyMemory(pDst, pSrc, cbPictureRowSize))
					return E_UNEXPECTED;

				pSrc += cbSrcRowSize;
				pDst += cbDstRowSize;
			}

			if(rgFormat.bPlanarFormat)
			{
				ATLASSERT(nWidthSrc % rgFormat.nHorzYtoUV == 0 && nWidthDst % rgFormat.nHorzYtoUV == 0);

				// copy UV
				for(h = 0; h < nPictureHeight; ++h)
				{
					if(!SEH_CopyMemory(pDst, pSrc, cbPictureRowSize/2))
						return E_UNEXPECTED;

					pSrc += cbSrcRowSize/2;
					pDst += cbDstRowSize/2;
				}
			}
		}
		return S_OK;
	}
};

//---------------------------------
const __declspec(selectany) PICTURE_FORMAT_2 CPictureFormats::s_arrSupportedFormats[] = 
{
	// YUV 4:2:2 as for UYVY but with different component ordering within the u_int32 macropixel
	{ &MEDIASUBTYPE_YUY2,  16, 2, 1, {0, 1, 3}, {2, 4, 4}, false, true },
	
	// YUV 4:2:2 (Y sample at every pixel, U and V sampled at every second pixel horizontally on each line).
	// A macropixel contains 2 pixels in 1 u_int32.
	{ &MEDIASUBTYPE_UYVY,  16, 2, 1, {1, 0, 2}, {2, 4, 4}, false, true },

	// 8 bit Y plane followed by 8 bit 2x2 subsampled V and U planes
	{ &MEDIASUBTYPE_YV12,  12, 2, 2, {0, 0, 0}, {1, 1, 1}, true,  true },

	// YUV 4:2:2 as for UYVY but with different component ordering within the u_int32 macropixel.
	// Note: This format usually not support by renderer's, tust it's after YV12 format
	{ &MEDIASUBTYPE_YVYU,  16, 2, 1, {0, 3, 1}, {2, 4, 4}, false, true },

	// 8 bit Y plane followed by 8 bit 2x2 subsampled U and V planes
	{ &MEDIASUBTYPE_I420,  12, 2, 2, {0, 0, 0}, {1, 1, 1}, true,  true },
	
	// 24 bit format used in mode 2 of the IEEE 1394 Digital Camera 1.04 spec
	{ &MEDIASUBTYPE_IYU2,  24, 1, 1, {2, 1, 0}, {3, 3, 3}, false, true },

	// RGB Formats
	{ &MEDIASUBTYPE_RGB32,  32, 1, 1, {2, 1, 0}, {4, 4, 4}, false, false },
	{ &MEDIASUBTYPE_ARGB32, 32, 1, 1, {2, 1, 0}, {4, 4, 4}, false, false },
	{ &MEDIASUBTYPE_RGB24,  24, 1, 1, {2, 1, 0}, {3, 3, 3}, false, false },
	{ &MEDIASUBTYPE_RGB565, 16, 1, 1, {1, 0, 0}, {2, 2, 2}, false, false },
	{ &MEDIASUBTYPE_RGB555, 16, 1, 1, {1, 0, 0}, {2, 2, 2}, false, false },

	// DeckLink HDYC (1080i) - the same as UYVY for HD video 
	{ &MEDIASUBTYPE_HDYC,	16, 2, 1, {1, 0, 2}, {2, 4, 4}, false, true },

	// 'v210' 4:2:2 Compression
	// Twelve 10-bit unsigned components are packed into four 32-bit little-endian words.
	{ &MEDIASUBTYPE_v210,	20, 2, 1, {1, 0, 2}, {2, 4, 4}, false, true },

	// 'e210' 4:2:2 Compression
	// Twelve 10-bit unsigned components are packed into four 32-bit little-endian words.
	{ &MEDIASUBTYPE_r210,	32, 1, 1, {0, 1, 2}, {4, 4, 4}, false, false },

	{ &MEDIASUBTYPE_NV12,	12, 2, 2, {0, 0, 0}, {1, 1, 1}, true, true },

};

const __declspec(selectany) int CPictureFormats::s_cFormats = sizeof(s_arrSupportedFormats)/sizeof(s_arrSupportedFormats[0]);

/////////////////////////////////////////////////////////////////////////////
//  Note: as on Intel site
// 
//	R = 1.164(Y - 16) + 1.596(V - 128)
//	G = 1.164(Y - 16) - 0.813(V - 128) - 0.391(U - 128)
//	B = 1.164(Y - 16)                  + 2.018(U - 128)
inline DWORD __stdcall YUV2RGB(DWORD dwYUV)
{
	int Y = (int)(dwYUV & 0xFF) - 16;
	Y *= 38142;										// 1,164 * 32768 = 38142
	int U = (int)((dwYUV & 0xFF00)>>8) - 128;
	int V = (int)((dwYUV & 0xFF0000)>>16) - 128;

	int R = (Y + 52298 * V) >> 15;					// 1,596 * 32768 = 52298
	R = (R < 0)? 0 : (R > 255)? 255 : R;
	
	int G = (Y - 26640 * V - 12812 * U) >> 15;		// 0,813 * 32768 = 26640; 0,391 * 32768 = 12812
	G = (G < 0)? 0 : (G > 255)? 255 : G;

	int B = (Y + 66126 * U) >> 15;					// 2,018 * 32768 = 66126
	B = (B < 0)? 0 : (B > 255)? 255 : B;

	return RGB(R,G,B);
}

//---------------------------------
// From Intel
// 
// Y = [(9798 R + 19235G + 3736 B) / 32768] Equations used by code.
// U = [(-4784 R - 9437 G + 4221 B) / 32768] + 128
// V = [(20218R - 16941G - 3277 B) / 32768] + 128
//
// Y = [(9798 R + 19235G + 3736 B) >>15] Modified integer equations
// U = [(16122 (B - Y))>>15]
// V = [(25203 (R - Y))>>15]

inline DWORD __stdcall RGB2YUV(DWORD dwRGB)
{
	int R = (dwRGB & 0xFF);
	int G = (dwRGB & 0xFF00)>>8;
	int B = (dwRGB & 0xFF0000)>>16;
	
	int y = ((9798 * R + 19235 * G + 3736 * B) >> 15) + 15;
	y = (y < 0)? 0 : (y > 255)? 255 : y;
	
	int u = ((16122 *(B - y))>>15) + 128;
	u = (u < 0)? 0 : (u > 255)? 255 : u;
	
	int v = ((25203 * (R - y))>>15) + 128;
	v = (v < 0)? 0 : (v > 255)? 255 : v;
	

	return YUV(y,u,v);
}

// Convert YUV16 (YUY2, UYVY and YVYU) to RGB32
inline HRESULT YUV16toRGB32(FOURCC fccType, DWORD nWidth, LONG nHeight, DWORD nRowWidth,
							const BYTE* pbSrc, BYTE* pbDst)
{
	ATLASSERT(fccType == FOURCC_YUY2 || fccType == FOURCC_UYVY || fccType == FOURCC_YVYU);
	if(fccType != FOURCC_YUY2 && fccType != FOURCC_UYVY && fccType != FOURCC_YVYU) return E_INVALIDARG;
	ATLASSERT(nWidth > 0);
	if(nWidth == 0) return E_INVALIDARG;
	ATLASSERT(nHeight != 0);
	if(nHeight == 0) return E_INVALIDARG;
	ATLASSERT(nRowWidth == 0 || nRowWidth >= nWidth);
	if(nRowWidth != 0 && nRowWidth < nWidth) return E_POINTER;
	ATLASSERT(pbSrc != NULL);
	if(pbSrc == NULL) return E_INVALIDARG;
	ATLASSERT(pbDst != NULL);
	if(pbDst == NULL) return E_INVALIDARG;

	//---------------------------------
	int n = CPictureFormats::Test(fccType, nWidth, nHeight, nRowWidth);
	if(n == -1)
		 return E_INVALIDARG;

	//---------------------------------
	DWORD nH = ABS(nHeight);
	const DWORD* pdwS = (const DWORD*)pbSrc;
	DWORD* pdwD = (DWORD*)(pbDst + 4 * nWidth * (nH - 1));
	DWORD nAdd = 0;
	if(nRowWidth > 0)
		nAdd = (nRowWidth - nWidth)/2;		// * 2 (to bytes) /4 (to YUYV DWORD)

	for(DWORD h = 0; h < nH; ++h)
	{
		for(DWORD w = 0; w < nWidth; w += 2, ++pdwS)
		{
			// YUY2: Y0 V  Y1 U  (in byte order)
			// UYVY: U  Y0 V  Y1 (in byte order)
			// YVYU: Y0 U  Y1 V  (in byte order)
			DWORD y, u, v;
			BYTE* pb = (BYTE*)pdwS;
			if(fccType == FOURCC_YUY2)
			{
				y = (DWORD)pb[0];
				u = (DWORD)pb[3];
				v = (DWORD)pb[1];
			}
			else if(fccType == FOURCC_UYVY)
			{
				y = (DWORD)pb[1];
				u = (DWORD)pb[2];
				v = (DWORD)pb[0];
			}
			else
			{
				y = (DWORD)pb[0];
				u = (DWORD)pb[1];
				v = (DWORD)pb[3];
			}

			*pdwD++ = YUV2RGB(YUV(y, u, v));

			y = (fccType == FOURCC_UYVY)? (DWORD)pb[3] : (DWORD)pb[2];

			*pdwD++ = YUV2RGB(YUV(y, u, v));
		}

		pdwD -= 2 * nWidth;
		pdwS += nAdd;
	}

	return S_OK;
}

// Use Euclidean algorithm (http://en.wikipedia.org/wiki/Euclidean_algorithm)
inline SIZE __stdcall MinAspectRatio(LONG nWidth, LONG nHeight)
{
	SIZE ar;
	ar.cx = ABS(nWidth);
	ar.cy = ABS(nHeight);

	if(nWidth != 0 && nHeight != 0)
	{
		LONG a = ar.cx;
		LONG b = ar.cy;
		while(b > 0)
		{
			LONG t = b;
			b = a % b;
			a = t;
		}

		// a is greatest common divisor
		ar.cx /= a;
		ar.cy /= a;
	}
	return ar;
}

/////////////////////////////////////////////////////////////////////////////
// CVideoFPS

class CVideoFPS
{
public :

	static const double s_arrAviFPS[];
	static const int s_cAviFPS;
	static const double s_arrDvdFPS[];
	static const int s_cDvdFPS;

	//---------------------------------
	static DWORD GetAviFPS(const double** ppArrFPS)
	{
		ATLASSERT(ppArrFPS != NULL);
		if(ppArrFPS == NULL) return 0;

		// Array without first and last values (this values are up/down limits, not FPS)
		*ppArrFPS = &s_arrAviFPS[1];
		return s_cAviFPS - 2;
	}

	static bool IsValidAviFPS(double fps)
	{
		return (s_arrAviFPS[0] >= fps && fps >= s_arrAviFPS[s_cAviFPS - 1]);
	}

	//--------------------------------- Conversions with discretization
	static double PeriodToAviFps(REFERENCE_TIME tPeriod)
	{
		double fps = PeriodToFps(tPeriod);
		return FpsToAvi(fps);
	}
	static REFERENCE_TIME FpsToAviPeriod(double fps)
	{
		double fpsDiscrete = FpsToAvi(fps);
		return FpsToPeriod(fpsDiscrete);
	}

	static REFERENCE_TIME DiscretizeAviPeriod(REFERENCE_TIME tPeriod)
	{
		double fps = PeriodToFps(tPeriod);
		return FpsToAviPeriod(fps);
	}
	static REFERENCE_TIME DiscretizeDvdPeriod(REFERENCE_TIME tPeriod)
	{
		double fps = PeriodToFps(tPeriod);
		fps = FpsToDvd(fps);
		tPeriod = FpsToPeriod(fps);
		return tPeriod;
	}

	//--------------------------------- Conversions
	static double PeriodToFps(REFERENCE_TIME tPeriod)
	{
		ATLASSERT(tPeriod >= (REFERENCE_TIME)0);

		double fps = 0.0;
		if(tPeriod > (REFERENCE_TIME)0)
		{
			fps = (double)DS_ONESEC/(double)tPeriod;
		}
		return fps;
	}
	static REFERENCE_TIME FpsToPeriod(double fps)
	{
		REFERENCE_TIME tPeriod = (REFERENCE_TIME)0;
		if(fps > 0.0)
		{
			tPeriod = (REFERENCE_TIME)((double)DS_ONESEC/fps + 0.5);
		}
		return tPeriod;
	}

	//--------------------------------- Discretizations
	static double FpsToAvi(double fps)
	{
		return DiscretizeFPS(fps, s_arrAviFPS, s_cAviFPS);
	}
	static double FpsToDvd(double fps)
	{
		return DiscretizeFPS(fps, s_arrDvdFPS, s_cDvdFPS);
	}
	static double DiscretizeFPS(double fps, const double* pFPS, int cFPS)
	{
		ATLASSERT(pFPS != NULL && cFPS > 0);

		if(fps > pFPS[0] || pFPS[cFPS - 1] > fps) 
			return 0.0;

		int i = 1;
		for(; i < cFPS - 2; ++i)
		{
			double d = (pFPS[i] + pFPS[i + 1])/2.0;
			if(fps > d)
				break;
		}
		return pFPS[i];
	}
};

//---------------------------------
// First and last values are up/down limits only (not FPS)
const __declspec(selectany) double CVideoFPS::s_arrAviFPS[] = 
{
	60.0, 30.0, 30.0/1.001, 25.0, 24.0, 24.0/1.001, 15, 15/1.001, 12.5,
	12.0, 12.0/1.001, 10.0, 8.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.5
};
const __declspec(selectany) int CVideoFPS::s_cAviFPS = SIZEOF_ARRAY(s_arrAviFPS);

// Only the most essential FPS for DVD
// First and last values are up/down limits only (not FPS)
const __declspec(selectany) double CVideoFPS::s_arrDvdFPS[] = 
{
	60.0, 30.0/1.001, 25.0, 24.0/1.001, 20.0
};
const __declspec(selectany) int CVideoFPS::s_cDvdFPS = SIZEOF_ARRAY(s_arrDvdFPS);

/////////////////////////////////////////////////////////////////////////////
// CPicturePersist
//
// ATTENTION. Code in this class written only for standard BMP headers (BITMAPINFOHEADER structure)

class CPicturePersist
{
public:
	// Loads BITMAPINFO and picture data from file pszFileName
	// After usage *ppBmpInfo and *ppPicture should be freed with ::CoTaskMemFree()
	// Size of *ppPicture buffer is equal to (*ppBmpInfo)->bmiHeader.biSizeImage
	static HRESULT LoadFromFile(LPCTSTR pszFileName, BITMAPINFO** ppBmpInfo,
		DWORD* pcbBmpInfo, BYTE** ppPicture);

	// Loads BITMAPINFOHEADER and pixels from file pszFileName
	static HRESULT LoadFromFile(LPCTSTR pszFileName, BITMAPINFOHEADER* pBmpHeader, BYTE** ppPicture);
	
	// Loads BITMAPINFOHEADER and picture data from resource
	static HRESULT LoadFromResource(LPCTSTR pszModuleName, WORD wID, LPCTSTR pszResType,
									BITMAPINFOHEADER* pBmpHeader, BYTE** ppPicture);
	static HRESULT LoadFromResource(HINSTANCE hInst, WORD wID, LPCTSTR pszResType,
									BITMAPINFOHEADER* pBmpHeader, BYTE** ppPicture);

	// Loads BITMAPINFOHEADER and pixels from file pszFileName
	static HRESULT LoadFromHBITMAP(HBITMAP hBitmap, BITMAPINFOHEADER* pBmpHeader, BYTE** ppPicture);

	//---------------------------------
	// Save image to file.
	// If type of image is YUV16 (YUY2, UYVY and YVYU): convert to RGB32
	static HRESULT SaveToFileAsBmp(LPCTSTR pszFileName, const BYTE* pPicture, const AM_MEDIA_TYPE* pmt);
	static HRESULT SaveToFileAsBmp(LPCTSTR pszFileName, FOURCC fccType, DWORD nWidth, LONG nHeight,
		DWORD nRowWidth, const BYTE* pPicture);

	// Creates BITMAPINFO struct from fccType, nWidth,and nHeight.
	// Saves BITMAPINFO () and picture data to file pszFileName
	// For RGB pictures used biCompression = BI_RGB.
	static HRESULT SaveToFile(LPCTSTR pszFileName, FOURCC fccType, DWORD nWidth, LONG nHeight,
		const BYTE* pPicture)
	{
		BITMAPINFO bmi = {0};
		HRESULT hr = FillBmpInfoHeader(fccType, nWidth, nHeight, &bmi.bmiHeader);
		if(hr == S_OK)
			hr = SaveToFile(pszFileName, &bmi, pPicture);
		return hr;
	}

	// Saves BITMAPINFO and picture data to file pszFileName
	static HRESULT SaveToFile(LPCTSTR pszFileName, const BITMAPINFO* pBmpInfo, const BYTE* pPicture);

	//---------------------------------
	// Helpers
	static DWORD BitmapInfoSize(DWORD biCompression = BI_RGB)
	{
		DWORD cbSize = sizeof(BITMAPINFOHEADER);
		if(biCompression == BI_BITFIELDS)
		{
			// MSDN, BITMAPINFOHEADER:
			//	"If the biCompression member of the BITMAPINFOHEADER is BI_BITFIELDS,
			//	 the bmiColors member contains three DWORD color masks that specify
			//	 the red, green, and blue components, respectively, of each pixel.
			//	 Each DWORD in the bitmap array represents a single pixel."
			cbSize += 2 * sizeof(RGBQUAD);	// add 2 only: first RGBQUAD already in BITMAPINFO struct
		}

		// DWORD alignment removed: Paint and ACDSee don't use DWORD alignment for pixel buffer!
		// return ALIGN_UP(cbSize, sizeof(DWORD));
		return cbSize;
	}
	static DWORD BmpFileHeaderSize(DWORD biCompression = BI_RGB)
	{
		return sizeof(BITMAPFILEHEADER) + BitmapInfoSize(biCompression);
	}

	//---------------------------------
	// Checks and corrects BITMAPINFOHEADER structure
	// ATTENTION. Some data can be corrected.
	// For sample, Photoshop doesn't set image size correctly and we correct this value
	static bool TestAndCorrectBmpHeader(BITMAPINFOHEADER* pBmpHeader);

	static HRESULT FillBmpHeaders(FOURCC fccType, DWORD nWidth, LONG nHeight, BYTE* pBuf);
	static HRESULT FillBmpInfoHeader(FOURCC fccType, DWORD nWidth, LONG nHeight,
									 BITMAPINFOHEADER* pBmpHeader);
	static bool MakeFileHeader(const BITMAPINFO* pBmpInfo, BYTE* pHeader);
};

// Loads BITMAPINFO and picture data from file pszFileName
// After usage *ppBmpInfo and *ppPicture should be freed with ::CoTaskMemFree()
// Size of *ppPicture buffer is equal to (*ppBmpInfo)->bmiHeader.biSizeImage
inline HRESULT CPicturePersist::LoadFromFile(LPCTSTR pszFileName, BITMAPINFO** ppBmpInfo,
											 DWORD* pcbBmpInfo, BYTE** ppPicture)
{
	
	ATLASSERT(pszFileName != NULL);
	if(pszFileName == NULL) return E_INVALIDARG;
	ATLASSERT(ppBmpInfo != NULL);
	if(ppBmpInfo == NULL) return E_POINTER;
	ATLASSERT(pcbBmpInfo != NULL);
	if(pcbBmpInfo == NULL) return E_POINTER;
	ATLASSERT(ppPicture != NULL);
	if(ppPicture == NULL) return E_POINTER;

	*ppBmpInfo = NULL;
	*pcbBmpInfo = 0;
	*ppPicture = NULL;

	//---------------------------------
	HRESULT hr = S_OK;
	::SetLastError(ERROR_SUCCESS);

	// Open exist file for read
	HANDLE hFile = ::CreateFile(pszFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return ERROR_FROM_WIN32(::GetLastError(), E_FAIL);

	//---------------------------------
	// Reads and checks BITMAPFILEHEADER
	DWORD cbRead = 0;
	BITMAPFILEHEADER rgBmpFileHdr = {0};
	BOOL bRet = ::ReadFile(hFile, &rgBmpFileHdr, sizeof(BITMAPFILEHEADER), &cbRead, NULL);
	if(!bRet || cbRead != sizeof(BITMAPFILEHEADER))
	{
		hr = (bRet)? E_FAIL : ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
		::CloseHandle(hFile);
		return hr;
	}

	if( rgBmpFileHdr.bfType != 0x4d42 ||
		(LONG)rgBmpFileHdr.bfOffBits < sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) ||
		rgBmpFileHdr.bfOffBits >= rgBmpFileHdr.bfSize )
	{
		::CloseHandle(hFile);
		return E_FAIL;
	}

	//---------------------------------
	// Reads and checks BITMAPINFO
	DWORD cbSize = rgBmpFileHdr.bfOffBits - sizeof(BITMAPFILEHEADER);
	DWORD cbBmpInfoSize = cbSize;
	if(cbBmpInfoSize < sizeof(BITMAPINFO))
	{
		// If biCompression == BI_RGB, then RGBQUAD can be skipped.
		// Add memory for this structure.
		cbBmpInfoSize = sizeof(BITMAPINFO);
	}

	BITMAPINFO* pBI = (BITMAPINFO*)::CoTaskMemAlloc(cbBmpInfoSize);
	if(pBI == NULL)
	{
		::CloseHandle(hFile);
		return E_OUTOFMEMORY;
	}
	memset(pBI, 0, cbBmpInfoSize);

	cbRead = 0;
	bRet = ::ReadFile(hFile, pBI, cbSize, &cbRead, NULL);
	if(!bRet || cbRead != cbSize)
	{
		hr = (bRet)? E_FAIL : ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
		::CoTaskMemFree(pBI);
		::CloseHandle(hFile);
		return hr;
	}

	if( !TestAndCorrectBmpHeader(&pBI->bmiHeader) ||
		rgBmpFileHdr.bfSize < rgBmpFileHdr.bfOffBits + pBI->bmiHeader.biSizeImage)
	{
		::CoTaskMemFree(pBI);
		::CloseHandle(hFile);
		return E_FAIL;
	}

	//---------------------------------
	// Create picture buffer
	BYTE* pBuf = (BYTE*)::CoTaskMemAlloc(pBI->bmiHeader.biSizeImage);
	if(pBuf == NULL)
	{
		::CoTaskMemFree(pBI);
		::CloseHandle(hFile);
		return E_OUTOFMEMORY;
	}

	// Read data from file into buffer
	cbRead = 0;
	bRet = ::ReadFile(hFile, pBuf, pBI->bmiHeader.biSizeImage, &cbRead, NULL);
	if(!bRet || cbRead != pBI->bmiHeader.biSizeImage)
	{
		hr = (bRet)? E_FAIL : ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
		::CoTaskMemFree(pBuf);
		::CoTaskMemFree(pBI);
		::CloseHandle(hFile);
		return hr;
	}

	//---------------------------------
	::CloseHandle(hFile);

	*ppBmpInfo = pBI;
	*pcbBmpInfo = cbBmpInfoSize;
	*ppPicture = pBuf;
	return S_OK;
}

// Loads BITMAPINFOHEADER and picture data from file pszFileName
// After usage *ppPicture should be freed with ::CoTaskMemFree(*ppPicture)
inline HRESULT CPicturePersist::LoadFromFile(LPCTSTR pszFileName, BITMAPINFOHEADER* pBmpHeader,
											 BYTE** ppPicture)
{
	ATLASSERT(pszFileName != NULL);
	if(pszFileName == NULL) return E_INVALIDARG;
	ATLASSERT(pBmpHeader != NULL);
	if(pBmpHeader == NULL) return E_INVALIDARG;
	ATLASSERT(ppPicture != NULL);
	if(ppPicture == NULL) return E_POINTER;

	memset(pBmpHeader, 0, sizeof(BITMAPINFOHEADER));
	*ppPicture = NULL;

	//---------------------------------
	BITMAPFILEHEADER rgBmpFileHdr = {0};
	BITMAPINFOHEADER rgBmpInfo = {0};
	DWORD cbRead = 0;
	HRESULT hr = S_OK;

	::SetLastError(ERROR_SUCCESS);

	//---------------------------------
	// Open exist file for read
	HANDLE hFile = ::CreateFile(pszFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return ERROR_FROM_WIN32(::GetLastError(), E_FAIL);

	// Reads and checks BITMAPFILEHEADER
	BOOL bRet = ::ReadFile(hFile, &rgBmpFileHdr, sizeof(BITMAPFILEHEADER), &cbRead, NULL);
	if(!bRet || cbRead != sizeof(BITMAPFILEHEADER))
	{
		hr = (bRet)? E_FAIL : ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
		::CloseHandle(hFile);
		return hr;
	}

	if( rgBmpFileHdr.bfType != 0x4d42 ||
		(LONG)rgBmpFileHdr.bfOffBits < sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) ||
		rgBmpFileHdr.bfOffBits >= rgBmpFileHdr.bfSize )
	{
		::CloseHandle(hFile);
		return E_FAIL;
	}

	// Reads and checks BITMAPINFOHEADER
	bRet = ::ReadFile(hFile, &rgBmpInfo, sizeof(BITMAPINFOHEADER), &cbRead, NULL);
	if(!bRet || cbRead != sizeof(BITMAPINFOHEADER))
	{
		hr = (bRet)? E_FAIL : ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
		::CloseHandle(hFile);
		return hr;
	}

	//---------------------------------
	if( !TestAndCorrectBmpHeader(&rgBmpInfo) ||
		rgBmpFileHdr.bfSize < rgBmpFileHdr.bfOffBits + rgBmpInfo.biSizeImage)
	{
		::CloseHandle(hFile);
		return E_FAIL;
	}

	//---------------------------------
	// Shifts the file pointer to picture block
	if(rgBmpFileHdr.bfOffBits > sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER))
	{
		LONG lDistanceToMoveHigh = 0;
		LONG lShift = ::SetFilePointer(hFile, (LONG)rgBmpFileHdr.bfOffBits, &lDistanceToMoveHigh, FILE_BEGIN);
		if(lShift == -1)
			hr = ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
		else if(lDistanceToMoveHigh != 0 || lShift != (LONG)rgBmpFileHdr.bfOffBits)
			hr = E_FAIL;

		if(FAILED(hr))
		{
			::CloseHandle(hFile);
			return hr;
		}
	}

	// Create picture buffer
	BYTE* pBuf = (BYTE*)::CoTaskMemAlloc(rgBmpInfo.biSizeImage);
	if(pBuf == NULL)
	{
		::CloseHandle(hFile);
		return E_OUTOFMEMORY;
	}

	// Read data from file into buffer
	bRet = ::ReadFile(hFile, pBuf, rgBmpInfo.biSizeImage, &cbRead, NULL);
	if(!bRet || cbRead != rgBmpInfo.biSizeImage)
		hr = (bRet)? E_FAIL : ERROR_FROM_WIN32(::GetLastError(), E_FAIL);

	//---------------------------------
	// Close file
	bRet = ::CloseHandle(hFile);
	if(SUCCEEDED(hr) && !bRet)
		hr = ERROR_FROM_WIN32(::GetLastError(), E_FAIL);

	if(FAILED(hr))
	{
		::CoTaskMemFree(pBuf);
		return hr;
	}

	//---------------------------------
	memcpy(pBmpHeader, &rgBmpInfo, sizeof(BITMAPINFOHEADER));
	*ppPicture = pBuf;
	return S_OK;
}

// Loads BITMAPINFOHEADER and picture data from resource
// ATTENTION : Windows 95 cannot handle executable files that contain
// resources whose size is 64K or larger.
// After usage *ppPicture should be freed with ::CoTaskMemFree(*ppPicture)
inline HRESULT CPicturePersist::LoadFromResource(LPCTSTR pszModuleName, WORD wID, LPCTSTR pszResType,
												 BITMAPINFOHEADER* pBmpHeader, BYTE** ppPicture)
{
	ATLASSERT(pszModuleName != NULL);
	if(pszModuleName == NULL) return E_INVALIDARG;
	ATLASSERT(pBmpHeader != NULL);
	if(pBmpHeader == NULL) return E_INVALIDARG;
	ATLASSERT(ppPicture != NULL);
	if(ppPicture == NULL) return E_POINTER;

	memset(pBmpHeader, 0, sizeof(BITMAPINFOHEADER));
	*ppPicture = NULL;

	//---------------------------------
	HRESULT hr = S_OK;
	::SetLastError(ERROR_SUCCESS);

	HINSTANCE hInst = ::LoadLibraryEx(pszModuleName, NULL, LOAD_LIBRARY_AS_DATAFILE);
	if(hInst == NULL)
		hr = ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
	else
		hr = LoadFromResource(hInst, wID, pszResType, pBmpHeader, ppPicture);
	return hr;
}

// Loads BITMAPINFOHEADER and picture data from resource
// If hInst == NULL: load resource from the file, which was used
// to create the calling process
// ATTENTION : Windows 95 cannot handle executable files that contain
// resources whose size is 64K or larger.
// After usage *ppPicture should be freed with ::CoTaskMemFree(*ppPicture)
inline HRESULT CPicturePersist::LoadFromResource(HINSTANCE hInst, WORD wID, LPCTSTR pszResType,
												 BITMAPINFOHEADER* pBmpHeader, BYTE** ppPicture)
{
	ATLASSERT(pBmpHeader != NULL);
	if(pBmpHeader == NULL) return E_INVALIDARG;
	ATLASSERT(ppPicture != NULL);
	if(ppPicture == NULL) return E_POINTER;

	memset(pBmpHeader, 0, sizeof(BITMAPINFOHEADER));
	*ppPicture = NULL;

	::SetLastError(ERROR_SUCCESS);

	//---------------------------------
	if(hInst == NULL)
	{
		// ATTENTION. Loads resource from the file, which was used to create the calling process
		hInst = ::GetModuleHandle(NULL);
		if(hInst == NULL)
			return ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
	}

	HRSRC hResInfo = ::FindResource(hInst, MAKEINTRESOURCE(wID), pszResType);
	if(hResInfo == NULL) 
		return ERROR_FROM_WIN32(::GetLastError(), E_FAIL);

	DWORD cbSize = ::SizeofResource(hInst, hResInfo);
	if(cbSize == 0)
		return ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
	if(cbSize < sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER))
		return E_FAIL;

	//---------------------------------
	HGLOBAL hResData = ::LoadResource(hInst, hResInfo);
	if(hResData == NULL)
		return ERROR_FROM_WIN32(::GetLastError(), E_FAIL);

	LPVOID pBmp = ::LockResource(hResData);
	if(pBmp == NULL)
		return E_FAIL;

	//---------------------------------
	const BITMAPFILEHEADER* pBmpFileHdr = (const BITMAPFILEHEADER*)pBmp;
	if( pBmpFileHdr->bfType != 0x4d42 ||
		(LONG)pBmpFileHdr->bfOffBits < sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) ||
		pBmpFileHdr->bfOffBits >= pBmpFileHdr->bfSize )
		return E_FAIL;

	const BITMAPINFOHEADER* pBmpIH = (const BITMAPINFOHEADER*)((BYTE*)pBmp + sizeof(BITMAPFILEHEADER));

	// Works only with standard BMP headers (BITMAPINFOHEADER structure)
	if(pBmpIH->biSize < sizeof(BITMAPINFOHEADER))
		return E_FAIL;

	//---------------------------------
	BITMAPINFOHEADER rgBmpInfo = {};
	memcpy(&rgBmpInfo, pBmpIH, sizeof(BITMAPINFOHEADER));

	if( !TestAndCorrectBmpHeader(&rgBmpInfo) ||
		pBmpFileHdr->bfSize < pBmpFileHdr->bfOffBits + rgBmpInfo.biSizeImage ||
		cbSize < pBmpFileHdr->bfOffBits + rgBmpInfo.biSizeImage)
		return E_FAIL;

	*ppPicture = (BYTE*)::CoTaskMemAlloc(rgBmpInfo.biSizeImage);
	if(*ppPicture == NULL)
		return E_OUTOFMEMORY;

	//---------------------------------
	// Copy all data
	memcpy(*ppPicture, (BYTE*)pBmp + pBmpFileHdr->bfOffBits, rgBmpInfo.biSizeImage);
	memcpy(pBmpHeader, &rgBmpInfo, sizeof(BITMAPINFOHEADER));
	return S_OK;
}

// Loads BITMAPINFOHEADER and picture data from HBITMAP
// After usage *ppPicture should be freed with ::CoTaskMemFree(*ppPicture)
inline HRESULT CPicturePersist::LoadFromHBITMAP(HBITMAP hBitmap, BITMAPINFOHEADER* pBmpHeader, BYTE** ppPicture)
{
	if( !hBitmap ) return E_INVALIDARG;
	if( !pBmpHeader && !ppPicture ) return E_POINTER;



	HDC hDC  = ::GetDC((HWND)NULL);
	if( !hDC ) return ERROR_FROM_WIN32( ::GetLastError(), E_FAIL );

	// Get number of lines
	BITMAPINFO* pInfo = (BITMAPINFO*)alloca( sizeof( BITMAPINFO ) + 4 * sizeof(RGBQUAD) );
	::ZeroMemory( pInfo, sizeof( BITMAPINFO ) + 4 * sizeof(RGBQUAD) );
	pInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	if( ::GetDIBits( hDC, hBitmap, 0, 0, NULL, pInfo, DIB_RGB_COLORS ) == 0 )
	{
		DWORD dwErr = ::GetLastError();

		::ReleaseDC((HWND)NULL, hDC);

		return ERROR_FROM_WIN32( dwErr, E_FAIL );
	}

	// Dirty fix for compression (by unknown reason pInfo->bmiHeader.biCompression == BI_BITFIELDS for 24 and 32 bits images)
	if( pInfo->bmiHeader.biBitCount == 24 || pInfo->bmiHeader.biBitCount == 32 )
	{
		pInfo->bmiHeader.biCompression = 0;
	}

	if( pBmpHeader ) ::SEH_CopyMemory( pBmpHeader, &pInfo->bmiHeader, sizeof(BITMAPINFOHEADER) );
	
	if( ppPicture )
	{
		*ppPicture = (LPBYTE)::CoTaskMemAlloc( pInfo->bmiHeader.biSizeImage );
		if( ::GetDIBits( hDC, hBitmap, 0, pInfo->bmiHeader.biHeight, *ppPicture, pInfo, DIB_RGB_COLORS ) == 0 )
		{
			DWORD dwErr = ::GetLastError();

			::ReleaseDC((HWND)NULL, hDC);

			return ERROR_FROM_WIN32( dwErr, E_FAIL );
		}
	}
	::ReleaseDC((HWND)NULL, hDC);

	return S_OK;
}

// Save image to file.
// If type of image is YUV16 (YUY2, UYVY and YVYU): convert to RGB32
inline HRESULT CPicturePersist::SaveToFileAsBmp(LPCTSTR pszFileName, const BYTE* pPicture,
												const AM_MEDIA_TYPE* pmt)
{
	ATLASSERT(pmt != NULL);
	if(pmt == NULL) return E_INVALIDARG;

	//---------------------------------
	if(pmt->majortype != MEDIATYPE_Video)
		return VFW_E_INVALIDMEDIATYPE;
	if(pmt->pbFormat == NULL)
		return VFW_E_INVALIDMEDIATYPE;

	//---------------------------------
	static const GUID* arrSubtypes[] =
	{ 
		&MEDIASUBTYPE_ARGB32, &MEDIASUBTYPE_RGB32, &MEDIASUBTYPE_RGB24,
		&MEDIASUBTYPE_YUY2, &MEDIASUBTYPE_UYVY, &MEDIASUBTYPE_YVYU
	};
	int i;
	for(i = 0; i < SIZEOF_ARRAY(arrSubtypes); ++i)
	{
		if(pmt->subtype == *arrSubtypes[i])
			break;
	}
	if(i == SIZEOF_ARRAY(arrSubtypes))
	{
		ATLASSERT(!_T("CPicturePersist::SaveToFileAsBmp(): unsupported subtype"));
		return VFW_E_INVALIDSUBTYPE;
	}

	//---------------------------------
	const BITMAPINFOHEADER* pbmi = NULL;
	DWORD nWidth = 0;

	if(pmt->formattype == FORMAT_VideoInfo)
	{
		if(pmt->cbFormat < sizeof(VIDEOINFOHEADER))
			return VFW_E_INVALIDMEDIATYPE;

		pbmi = &((const VIDEOINFOHEADER*)pmt->pbFormat)->bmiHeader;
	}
	else if(pmt->formattype == FORMAT_VideoInfo2)
	{
		if(pmt->cbFormat < sizeof(VIDEOINFOHEADER2))
			return VFW_E_INVALIDMEDIATYPE;

		const VIDEOINFOHEADER2* pvih = (const VIDEOINFOHEADER2*)pmt->pbFormat;
		pbmi = &pvih->bmiHeader;
		if(pvih->rcSource.right - pvih->rcSource.left > 0)
		{
			nWidth = (DWORD)(pvih->rcSource.right - pvih->rcSource.left);
			ATLASSERT(nWidth <= (DWORD)pbmi->biWidth);
		}
	}
	else
	{
		ATLASSERT(!_T("CPicturePersist::SaveToFileAsBmp(): unsupported formattype"));
		return VFW_E_INVALIDMEDIATYPE;
	}

	FOURCC fccType = pbmi->biCompression;
	ATLASSERT(fccType == GUID2FOURCC(pmt->subtype));

	if(nWidth == 0)
		nWidth = (DWORD)pbmi->biWidth;

	DWORD nHeight = ABS(pbmi->biHeight);

	//---------------------------------
	HRESULT hr = SaveToFileAsBmp(pszFileName, fccType, nWidth, nHeight, nWidth, pPicture);
	return hr;
}

// Save image to file.
// If type of image is YUV16 (YUY2, UYVY and YVYU): convert to RGB32
inline HRESULT CPicturePersist::SaveToFileAsBmp(LPCTSTR pszFileName, FOURCC fccType, DWORD nWidth,
												LONG nHeight, DWORD nRowWidth, const BYTE* pPicture)
{
	ATLASSERT(pszFileName != NULL);
	if(pszFileName == NULL) return E_INVALIDARG;
	ATLASSERT(nWidth > 0);
	if(nWidth == 0) return E_INVALIDARG;
	ATLASSERT(nHeight != 0);
	if(nHeight == 0) return E_INVALIDARG;
	ATLASSERT(pPicture != NULL);
	if(pPicture == NULL) return E_INVALIDARG;

	//---------------------------------
	int n = CPictureFormats::Test(fccType, nWidth, nHeight, nRowWidth);
	if(n == -1)
		 return E_INVALIDARG;

	//---------------------------------
	const BYTE* pImage = pPicture;
	BYTE* pConverted = NULL;

	HRESULT hr = S_OK;
	if(fccType != FOURCC_RGB24 && fccType != FOURCC_ARGB32 && fccType != FOURCC_RGB32)
	{
		ATLASSERT(fccType == FOURCC_YUY2 || fccType == FOURCC_UYVY || fccType == FOURCC_YVYU);
		if(fccType != FOURCC_YUY2 && fccType != FOURCC_UYVY && fccType != FOURCC_YVYU) return E_INVALIDARG;

		DWORD cbImageSize = 0;
		hr = CPictureFormats::CalcImageSize(FOURCC_RGB32, nWidth, nHeight, &cbImageSize);
		ATLASSERT(hr == S_OK);

		pConverted = (BYTE*)malloc(cbImageSize);
		if(pConverted == NULL)
			return E_OUTOFMEMORY;

		hr = YUV16toRGB32(fccType, nWidth, nHeight, nRowWidth, pPicture, pConverted);
		if(hr != S_OK)
		{
			free(pConverted);
			return hr;
		}

		pImage = pConverted;
		fccType = FOURCC_RGB32;
		nHeight = ABS(nHeight);
	}

	//---------------------------------
	BITMAPINFO bmi = {0};
	hr = FillBmpInfoHeader(fccType, nWidth, nHeight, &bmi.bmiHeader);
	if(hr == S_OK)
		hr = SaveToFile(pszFileName, &bmi, pImage);

	if(pConverted != NULL)
		free(pConverted);

	return hr;
}

// Saves BITMAPINFOHEADER and picture data to file pszFileName
inline HRESULT CPicturePersist::SaveToFile(LPCTSTR pszFileName, const BITMAPINFO* pBmpInfo,
										   const BYTE* pPicture)
{
	ATLASSERT(pszFileName != NULL && pBmpInfo != NULL && pPicture != NULL);
	if(pszFileName == NULL || pBmpInfo == NULL || pPicture == NULL) return E_INVALIDARG;

	// Works only with standard BMP headers (BITMAPINFOHEADER structure)
	ATLASSERT(pBmpInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));
	if(pBmpInfo->bmiHeader.biSize != sizeof(BITMAPINFOHEADER))
		 return E_INVALIDARG;

	//---------------------------------
	// Create file header
	DWORD cbSize = BmpFileHeaderSize(pBmpInfo->bmiHeader.biCompression);
	BYTE* pHeader = (BYTE*)_alloca(cbSize);
	if(!MakeFileHeader(pBmpInfo, pHeader))
		return E_FAIL;

	//---------------------------------
	// Create new file
	HRESULT hr = S_OK;
	::SetLastError(ERROR_SUCCESS);

	HANDLE hFile = ::CreateFile(pszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return ERROR_FROM_WIN32(::GetLastError(), E_FAIL);

	// Write header to file
	DWORD cbWritten = 0;
	BOOL bRet = ::WriteFile(hFile, pHeader, cbSize, &cbWritten, NULL);
	if(!bRet || cbWritten != cbSize)
	{
		hr = (bRet)? E_FAIL : ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
	}
	else
	{
		// Copy pixels to file
		BITMAPINFOHEADER* pBIH = (BITMAPINFOHEADER*)(pHeader + sizeof(BITMAPFILEHEADER));

		bRet = ::WriteFile(hFile, pPicture, pBIH->biSizeImage, &cbWritten, NULL);
		if(!bRet || cbWritten != pBIH->biSizeImage)
			hr = (bRet)? E_FAIL : ERROR_FROM_WIN32(::GetLastError(), E_FAIL);
	}

	bRet = ::FlushFileBuffers(hFile);
	if(SUCCEEDED(hr) && !bRet)
		hr = ERROR_FROM_WIN32(::GetLastError(), E_FAIL);

	bRet = ::CloseHandle(hFile);
	if(SUCCEEDED(hr) && !bRet)
		hr = ERROR_FROM_WIN32(::GetLastError(), E_FAIL);

	return hr;
}

// Size of pHeader buffer must be at least BmpFileHeaderSize(pBmpInfo) bytes
inline bool CPicturePersist::MakeFileHeader(const BITMAPINFO* pBmpInfo, BYTE* pHeader)
{
	ATLASSERT(pBmpInfo != NULL && pHeader != NULL);
	if(pBmpInfo == NULL || pHeader == NULL) return false;

	// Works only with standard BMP headers (BITMAPINFOHEADER structure)
	ATLASSERT(pBmpInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));

	//---------------------------------
	// Fill BITMAPINFOHEADER and check/correct it
	BITMAPINFO* pBmp = (BITMAPINFO*)(pHeader + sizeof(BITMAPFILEHEADER));
	memcpy(&pBmp->bmiHeader, &pBmpInfo->bmiHeader, sizeof(BITMAPINFOHEADER));

	if(!TestAndCorrectBmpHeader(&pBmp->bmiHeader))
		return false;

	//---------------------------------
	// Fill BITMAPFILEHEADER
	BITMAPFILEHEADER* pFileHdr = (BITMAPFILEHEADER*)pHeader;
	memset(pFileHdr, 0, sizeof(BITMAPFILEHEADER));
	pFileHdr->bfType = 0x4d42;
	pFileHdr->bfOffBits = BmpFileHeaderSize(pBmpInfo->bmiHeader.biCompression);
	pFileHdr->bfSize = pFileHdr->bfOffBits + pBmpInfo->bmiHeader.biSizeImage; 

	//---------------------------------
	// Fill RGBQUAD if biCompression == BI_BITFIELDS
	if(pBmpInfo->bmiHeader.biCompression == BI_BITFIELDS)
	{
		ATLASSERT(pBmpInfo->bmiHeader.biBitCount == 16 || pBmpInfo->bmiHeader.biBitCount == 32);
		memcpy(&pBmp->bmiColors, pBmpInfo->bmiColors, 3 * sizeof(RGBQUAD));
	}

	return true;
}

// Checks and corrects BITMAPINFOHEADER structure
// For sample, Photoshop doesn't set image size correctly and we correct this value
inline bool CPicturePersist::TestAndCorrectBmpHeader(BITMAPINFOHEADER* pBmpHeader)
{
	ATLASSERT(pBmpHeader != NULL);
	if(pBmpHeader == NULL) return false;

	// Works only with standard BMP headers (BITMAPINFOHEADER structure)
	if(pBmpHeader->biSize < sizeof(BITMAPINFOHEADER))
		return false;

	//---------------------------------
	FOURCC fccType = pBmpHeader->biCompression;
	if(fccType == BI_RGB || fccType == BI_BITFIELDS)
	{
		// Change FOURCC for RGB bitmaps
		if(pBmpHeader->biBitCount == 32)
			fccType = FOURCC_RGB32;
		else if(pBmpHeader->biBitCount == 24)
			fccType = FOURCC_RGB24;
		else if(pBmpHeader->biBitCount == 16)
			fccType = FOURCC_RGB565;
		else
			return false;
	}

	//---------------------------------
	int nFormat = CPictureFormats::Find(fccType);
	if(nFormat == -1)
		return false;
	if(pBmpHeader->biPlanes != 1)
		return false;
	if(pBmpHeader->biWidth == 0 || pBmpHeader->biHeight == 0)
		return false;
	if(pBmpHeader->biClrImportant != 0 || pBmpHeader->biClrUsed != 0)
		return false;

	if(CPictureFormats::TestAlign(fccType, (DWORD)pBmpHeader->biWidth,
							(DWORD)pBmpHeader->biHeight, 0) == -1)
		return false;

	DWORD cbSize = 0;
	HRESULT hr = CPictureFormats::CalcImageSize(fccType, (DWORD)pBmpHeader->biWidth,
							(DWORD)pBmpHeader->biHeight, &cbSize);

	if(hr == S_OK && pBmpHeader->biSizeImage != cbSize)
	{
		// Old versions of Photoshop set bad image size == 0
		// New version of Photoshop (CS) set image size == cbSize + 2

		ATLTRACE2(atlTraceGeneral, TRACELEVEL_Warning,
			_T("WARNING. CPicturePersist::TestAndCorrectBmpHeader(): ")
			_T("correcting bad image size: old=%u, new=%u\n"), pBmpHeader->biSizeImage, cbSize);

		pBmpHeader->biSizeImage = cbSize;
	}

	return (hr == S_OK);
}

inline HRESULT CPicturePersist::FillBmpHeaders(FOURCC fccType, DWORD nWidth, LONG nHeight, BYTE* pBuf)
{
	ATLASSERT(pBuf != NULL);
	if(pBuf == NULL) return E_POINTER;

	BITMAPINFO rgBmpInfo = {0};
	HRESULT hr = FillBmpInfoHeader(fccType, nWidth, nHeight, &rgBmpInfo.bmiHeader);
	if(hr == S_OK)
	{
		if(!MakeFileHeader(&rgBmpInfo, pBuf))
			hr = E_FAIL;
	}
	return hr;
}

inline HRESULT CPicturePersist::FillBmpInfoHeader(FOURCC fccType, DWORD nWidth, LONG nHeight,
												  BITMAPINFOHEADER* pBmpHeader)
{
	ATLASSERT(pBmpHeader != NULL);
	if(pBmpHeader == NULL) return E_POINTER;

	int nFormat = CPictureFormats::Find(fccType);
	if(nFormat == -1)
		return E_INVALIDARG;

	//---------------------------------
	const PICTURE_FORMAT_2& rgFormat = CPictureFormats::s_arrSupportedFormats[nFormat];

	memset(pBmpHeader, 0, sizeof(BITMAPINFOHEADER));

	pBmpHeader->biSize = sizeof(BITMAPINFOHEADER);
	pBmpHeader->biWidth = (LONG)nWidth;
	pBmpHeader->biHeight = nHeight;
	pBmpHeader->biPlanes = 1;
	pBmpHeader->biBitCount = rgFormat.wBitCount;
	pBmpHeader->biCompression = (!rgFormat.bYUVFormat)? BI_RGB : fccType;
	pBmpHeader->biSizeImage = CPictureFormats::CalcImageSize(rgFormat, nWidth, nHeight);

	// We don't set next values
	//	pBmpHeader->biXPelsPerMeter = 0;
	//	pBmpHeader->biYPelsPerMeter = 0;
	//	pBmpHeader->biClrUsed = 0;
	//	pBmpHeader->biClrImportant = 0;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
#endif	// __DSVIDEO_H_
