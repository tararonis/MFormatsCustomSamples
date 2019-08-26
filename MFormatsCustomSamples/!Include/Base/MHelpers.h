//---------------------------------------------------------------------------
// MHelpers.h : 
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

#ifndef _M_HELPERS_H_
#define _M_HELPERS_H_

#include <math.h>
#include <deque>
using std::deque;
using std::make_pair;

#include <map>
using std::map;

#include "MFormats.h"

#include "MFunctions.h"

#include "../WinAPI/threads.h"
#include "../WinAPI/blob.h"

#include "../Base/MCreator.h"
#include "../Base/MCalls.h"
#include "../Devices/MTimer.h"
// For BT2020_B709 and back color matrix
#include "../Video/ColorMatrix.h"


#include "d3d9types.h"
#include "d3d9.h"
#include "dxgi.h"

#ifndef OLD_XML_PARSE
#include "../Devices/MCounters.h"
#include "XML/XMLUtils.h"
#endif

//////////////////////////////////////////////////////////////////////////
// ID Generation

#define UNIQUE_NAME_FORMAT	L"%ws-%03d"
#define UNIQUE_NAME_DEF_PREFIX	L"MObject"

#define MGSENDER_PREFIX	L"MediaLooks_MSender"

#define M_DENIED_CHARS	L" \n\r\t&;,<>?*'\"[]{}()#"

#ifndef SQR
#define SQR( _val ) ((_val)*(_val))
#endif	// SQR


#ifdef _DEBUG
#define CHECK_FPU \
{ \
	double x  = 1; \
	double y = 3; \
	x/=y; \
	ATLASSERT( x - 0.3333333333333333333 <= 0.00000000001); \
}
#else
#define CHECK_FPU
#endif
// Props

#define MP_MAX_AUDIO_CHANNELS  (32)

#define MVID2STR( _pVid) MVidProps2Str(_pVid, (LPWSTR)alloca(256) )
#define MVID2STR_S( _pVid) MVidProps2StrLog( _pVid, (LPWSTR)alloca(256) )
#define MAUD2STR( _pAud) MAudProps2Str(_pAud, (LPWSTR)alloca(256) )
#define MAUD2STR_S( _pAud) MAudProps2StrLog( _pAud, (LPWSTR)alloca(128) )
#define MDATE2STR( pTime ) (MDateTime2Str( pTime, (LPWSTR)alloca(256)))
#define MTIME2STR( _time ) (MTime2Str( _time, (LPWSTR)alloca(128 *sizeof(WCHAR) ) ))

#ifndef TC2STR
#define TC2STR( _tc ) TC2Str( _tc, (LPWSTR)alloca(64 *sizeof(WCHAR) ) )
#endif

#define MTC2STR( _tc ) TC2Str( _tc, (LPWSTR)alloca(64 *sizeof(WCHAR)), -1 )
#define MTC2STR_F( _tc ) TC2Str( _tc, (LPWSTR)alloca(64 *sizeof(WCHAR)), 1 )
#define PTC2STR( _ptc ) ( _ptc ? TC2STR( *_ptc ) : _T("NULL") )


#define MFRAME2STR( pFrame ) (MFFrame_Debug(pFrame, (LPWSTR)alloca(1024) ))

// Safe functions for threads
#define MVID2STR_SF( _pVid) MVidProps2StrLog( _pVid, (LPWSTR)__STR_BUF )
#define MAUD2STR_SF( _pAud) MAudProps2StrLog( _pAud, (LPWSTR)__STR_BUF )
#define MDATE2STR_SF( pTime ) (MDateTime2Str( pTime, (LPWSTR)__STR_BUF ))
#define MTIME2STR_SF( _time ) (MTime2Str( _time, (LPWSTR)__STR_BUF ))
#define TC2STR_SF( _tc ) TC2Str( _tc, (LPWSTR)__STR_BUF, false )
#define MFRAME2STR_SF( pFrame ) (MFFrame_Debug(pFrame, (LPWSTR)__STR_BUF ))


#ifndef IS_RATE
#define IS_RATE( _dblRate, _dblCheck )  (ABS(_dblRate -_dblCheck) < 0.01)
#endif

// Add +16 for NTSC 480/486 also for some cropped HD standards
#ifndef IS_RES
#define IS_RES( _pVidProps, _eVF ) \
	( _pVidProps->nWidth == VF_Res(_eVF).cx && ABS( _pVidProps->nHeight ) + 16 >= VF_Res(_eVF).cy && ABS( _pVidProps->nHeight ) <= VF_Res(_eVF).cy + VF_VBI_Lines( _eVF ) ) 
#endif

#ifndef IS_RES_FIT
#define IS_RES_FIT( _pVidProps, _eVF ) \
		( _pVidProps->nWidth <= VF_Res(_eVF).cx && ABS( _pVidProps->nHeight ) <= VF_Res(_eVF).cy + VF_VBI_Lines( _eVF ) ) // 49
#endif



#define VF_CLEAR3D( _eVF ) (eMVideoFormat)((_eVF)&0xFF)
#define VF_GET3D( _eVF ) (eM3DFormat)((_eVF)&(eM3D_FullFlag|eM3D_SideBySide|eM3D_TopAndBottom))
#define VF_IS3D( _eVF ) (((_eVF)&(eM3D_SideBySide|eM3D_TopAndBottom))!=0)
#define VF_3DSCALE_X( _eVF ) (((_eVF)&eM3D_FullFlag) ? (((_eVF)&eM3D_SideBySide) ? 2 : 1) : 1)
#define VF_3DSCALE_Y( _eVF ) (((_eVF)&eM3D_FullFlag) ? (((_eVF)&eM3D_TopAndBottom) ? 2 : 1) : 1)



//////////////////////////////////////////////////////////////////////////
//Preview helpers
static LPCWSTR eM_PreviewForwardStr[] = { L"preview.external_process", L"preview.type", L"preview.show_cc", L"maintain_ar", L"full_screen", L"direct3d.pixel_format", 
	L"deinterlace", L"preview.drop_frames", L"preview.skip_fields", L"preview.pull", L"preview.downscale", L"preview.crop", L"preview.audio_channels", L"preview.audio_gain",
	L"audio.buffer_size_msec", L"video_renderer", L"audio_renderer", L"audio_renderer.list", L"audio_renderer.reset", L"preview.mserver_pid", 
	L"preview.background", NULL };

static LPCWSTR eM_FileForwardStr[] = { L"file::info::kbps_avg", L"file::info::kbps_avg_audio", L"file::info::kbps_avg_video", L"file::info::kbps_avg_data", 
	L"file::info::buffer_video", L"file::info::buffer_audio", L"file::info::cc_type", L"file::buffer.audio", L"file::buffer.video", L"file::buffer.data", L"file::buffer.sub", 
	L"file::buffer.frames", L"file::ts_program", NULL };

static LPCWSTR eM_DelayForwardStr[] = { L"mdelay.time", L"mdelay.quality", L"mdelay.split_progressive", 
	L"mdelay.buffer_path", L"mdelay.buffer_duration", L"mdelay.buffer_size_mem", 
	L"mdelay.delete_on_close", L"mdelay.reset_on_open", L"mdelay.static_mute", L"mdelay.static_deinterlace", NULL };

static LPCWSTR eM_DeviceForwardStr[] = {  L"disconnect-type", L"audio.correct-time", L"audio.max-duration", L"video.correct-fps", L"video.correct-time", L"video.ignore-time", 
	L"bmd.locked_video", L"deltacast.locked_video", L"streamlabs.locked_video", L"streamlabs.hw_time", L"audio.delay", L"graph.force_clock", 
	L"vanc.lines", L"vanc.capture", L"vanc.lines_cc", L"vanc.capture_cc", L"vanc.capture_anc", 
	L"bmd.10bit", L"bmd.no_copy", L"bmd.autodetect_format",  L"bmd.max_allocated_frames", L"timecode_format", L"bmd.hw_time", L"drop_frame_tc", L"timecode.drop_frame", 
	L"brightness", L"contrast", L"hue", L"saturation_u", L"saturation_v", L"sharpness",	//For Streamlabs 
	L"thread.priority", L"capture_ts_file", // For H.264 Encoder
	L"device.update_count",
	L"ndi_on_program", L"ndi_on_preview", L"ndi_group", L"ndi_bandwidth", 
	L"mjpg.mfdecoder",
	L"output.buffers",
	L"v_sources.video_in",
	L"directshow.use_filter_name",
	L"directshow.show_blackmagic",
	L"directshow.show_none",
	L"directshow.default_audio",
	L"packer.thread_priority", L"packer.buffer_max", L"packer.buffer_min",
	NULL }; 

//external_access='false' on_frame.sync='false' on_frame.data='false' on_event.sync='false' events.use_window='true' default_name='MFile' preview.pull='false' default_tracks='0' channels_per_track='0' internal.convert_frame='false' pause.fields='1' watch_dog.lock_maxtime='10000' watch_dog.check_time='100' watch_dog.frameget_maxtime='5000' external_process='true' scaling_quality='8' crop='' mirror='' overlay_rms='false' overlay_rms.pos='0.1' overlay_rms.color='green' overlay_waveform='false' overlay_waveform.pos='-0.3' overlay_waveform.color='' mdelay.enabled='false' mdelay.live_preview='false'/>
static LPCWSTR eM_ObjectSkipStr[] = { L"device_sharing", L"external_process", L"external_access", L"on_frame.sync", L"on_frame.data",L"on_event.sync",L"events.use_window",L"preview.pull",L"watch_dog.lock_maxtime",L"watch_dog.check_time",L"watch_dog.frameget_maxtime",NULL};

//////////////////////////////////////////////////////////////////////////
// Video & Audio helpers

#define M_NO_AUDIO				L"<None>"
#define M_NO_VIDEO				L"<None>"
#define M_NOT_SPECIFIED_STR		L"<Auto/Not Specified>"
//#define M_NOT_SPECIFIED_STR2	L"Auto/Not Specified"

// Note: 'none' for compatibility with old code
static LPCWSTR eM_ScaleTypeStr[] = { L"default", L"ignore-ar", L"letter-box", L"crop", L"no-scale", L"none", NULL };
static LPCWSTR eM_InterlaceStr[] = { L"auto", L"top", L"bottom", L"progressive", L"inverse", L"field", NULL };
static LPCWSTR eM_InterlaceStr2[] = { L"Auto", L"TopFirst", L"BottomFirst", L"Progressive", L"Inverse", L"field", L"ERROR", NULL };
static LPCWSTR eM_InterlaceStrS[] = { L"A", L"iT", L"iB", L"p", L"INV", L"OF", L"ER", NULL };
static LPCWSTR eM_ScaleTypeStrS[] = { L"", L"I", L"L", L"C", L"N", NULL };
//static LPCWSTR eM_3DFormatStr[] = { L"none", L"auto", L"side-by-side", L"top-and-bottom", NULL };

// static LPCWSTR eM_VideoFormatStr[] = { L"Custom", 
// 	L"NTSC", L"NTSC-23p", L"NTSC-16x9", L"PAL", L"PAL-16x9", 
// 	L"HD720-50p", L"HD720-59p", L"HD720-60p",
// 	L"HD1080-23p", L"HD1080-24p", L"HD1080-25p", L"HD1080-29p",L"HD1080-30p",
// 	L"HD1080-50i", L"HD1080-59i", L"HD1080-60i", L"HD1080-50p", L"HD1080-59p", L"HD1080-60p",
// 	L"2K-DCI-23p", L"2K-DCI-24p", L"2K-DCI-25p", 
// 	L"2K-DCI-50p", L"2K-DCI-59p", L"2K-DCI-60p", 
// 	L"2K-23p", L"2K-24p", L"2K-25p", L"4K-UHD-50i", L"4K-UHD-5994i", L"4K-UHD-60i", 	
// 	L"4K-UHD-23p", L"4K-UHD-24p", L"4K-UHD-25p", L"4K-UHD-29p", L"4K-UHD-30p", 
// 	L"4K-DCI-23p", L"4K-DCI-24p", L"4K-DCI-25p", 
// 	L"4K-UHD-50p", L"4K-UHD-59p", L"4K-UHD-60p",
// 	L"4K-DCI-50p", L"4K-DCI-59p", L"4K-DCI-60p",
// 	NULL };
// 
// static LPCWSTR eM_VideoFormatStrExt[] = { L"Custom", 
// 	L"eMVF_NTSC", L"eMVF_NTSC_2398p", L"eMVF_NTSC_16x9", L"eMVF_PAL", L"eMVF_PAL_16x9", 
// 	L"eMVF_HD720_50p", L"eMVF_HD720_5994p", L"eMVF_HD720_60p",
// 	L"eMVF_HD1080_2398p", L"eMVF_HD1080_24p", L"eMVF_HD1080_25p", L"eMVF_HD1080_2997p",L"eMVF_HD1080_30p",
// 	L"eMVF_HD1080_50i", L"eMVF_HD1080_5994i", L"eMVF_HD1080_60i", L"eMVF_HD1080_50p", L"eMVF_HD1080_5994p", L"eMVF_HD1080_60p",
// 	L"eMVF_2K_DCI_2398p", L"eMVF_2K_DCI_24p", L"eMVF_2K_DCI_25p",
// 	L"eMVF_2K_DCI_50p", L"eMVF_2K_DCI_5994p", L"eMVF_2K_DCI_60p",
// 	L"eMVF_2K_2398p", L"eMVF_2K_24p", L"eMVF_2K_25p", 	
// 	L"eMVF_4K_UHD_50i", L"eMVF_4K_UHD_5994i", L"eMVF_4K_UHD_60i", 
// 	L"eMVF_4K_UHD_2398p", L"eMVF_4K_UHD_24p", L"eMVF_4K_UHD_25p", L"eMVF_4K_UHD_2997p", L"eMVF_4K_UHD_30p", 
// 	L"eMVF_4K_DCI_2398p", L"eMVF_4K_DCI_24p", L"eMVF_4K_DCI_25p",
// 	L"eMVF_4K_UHD_50p", L"eMVF_4K_UHD_5994p", L"eMVF_4K_UHD_60p",
// 	L"eMVF_4K_DCI_50p", L"eMVF_4K_DCI_5994p", L"eMVF_4K_DCI_60p",
// 	NULL };
// 	


#define Img_RowBytesM	Img_RowSizeM

struct VF_INFO
{
	eMVideoFormat eVF;

	LPCWSTR pszName;
	LPCWSTR pszNameEx;
	MRational frameRate;	
	eMInterlace eInterlace;
	int nWidth;
	int nHeight;	
	int nAspectX;
	int nAspectY;
	int nHDType;
	int nVBILines;
};

static const VF_INFO s_arrFormats[] = {
	//eVF,				pszName,		pszNameEx,			MRationalRate,	 		eInterlace,		nWidth, nHeigh, nARX, nARY,	 nHDType, nVBI_Lines
	{ eMVF_Custom,		L"Custom",		L"eMVF_Custom",			{0, 1},				eMI_Default,		0,		0,		0,		0,		-1,	0 },

	{ eMVF_NTSC,		L"NTSC",		L"eMVF_NTSC",			{30000, 1001},		eMI_Field2First,	720,	486,	4,		3,		0,	39 },
	{ eMVF_NTSC_2398,	L"NTSC-23p",	L"eMVF_NTSC_2398p",		{24000, 1001},   	eMI_Progressive,	720,	486,	4,		3,		0,	39 },
	{ eMVF_NTSC_16x9,	L"NTSC-16x9",	L"eMVF_NTSC_16x9",		{30000, 1001},   	eMI_Field2First,	720,	486,	16,		9,		0,	39 },

	{ eMVF_PAL,			L"PAL",			L"eMVF_PAL",			{25, 1},		 	eMI_Field1First,	720,	576,	4,		3,		0,	49 },
	{ eMVF_PAL_16x9,	L"PAL-16x9",	L"eMVF_PAL_16x9",		{25, 1},		 	eMI_Field1First,	720,	576,	16,		9,		0,	49 },
	
	{ eMVF_HD720_50p,	L"HD720-50p",	L"eMVF_HD720_50p",		{50, 1},		 	eMI_Progressive,	1280,	720,	16,		9,		1, 30 },
	{ eMVF_HD720_5994p,	L"HD720-59p",	L"eMVF_HD720_5994p",	{60000, 1001},   	eMI_Progressive,	1280,	720,	16,		9,		1, 30 },
	{ eMVF_HD720_60p,	L"HD720-60p",	L"eMVF_HD720_60p",		{60, 1},		 	eMI_Progressive,	1280,	720,	16,		9,		1, 30 },

	{ eMVF_HD1080_2398p,L"HD1080-23p",	L"eMVF_HD1080_2398p",	{24000, 1001},  	eMI_Progressive,	1920,	1080,	16,		9,		1, 45 },
	{ eMVF_HD1080_24p,	L"HD1080-24p",	L"eMVF_HD1080_24p",		{24, 1},			eMI_Progressive,	1920,	1080,	16,		9,		1, 45 },
	{ eMVF_HD1080_25p,	L"HD1080-25p",	L"eMVF_HD1080_25p",		{25, 1},			eMI_Progressive,	1920,	1080,	16,		9,		1, 45 },
	{ eMVF_HD1080_2997p,L"HD1080-29p",	L"eMVF_HD1080_2997p",	{30000, 1001},  	eMI_Progressive,	1920,	1080,	16,		9,		1, 45 },
	{ eMVF_HD1080_30p,	L"HD1080-30p",	L"eMVF_HD1080_30p",		{30, 1},			eMI_Progressive,	1920,	1080,	16,		9,		1, 45 },

	{ eMVF_HD1080_50i,	L"HD1080-50i",	L"eMVF_HD1080_50i",		{25, 1},			eMI_Field1First,	1920,	1080,	16,		9,		1, 45 },
	{ eMVF_HD1080_5994i,L"HD1080-59i",	L"eMVF_HD1080_5994i",	{30000, 1001},  	eMI_Field1First,	1920,	1080,	16,		9,		1, 45 },
	{ eMVF_HD1080_60i,	L"HD1080-60i",	L"eMVF_HD1080_60i",		{30, 1},			eMI_Field1First,	1920,	1080,	16,		9,		1, 45 },

	{ eMVF_HD1080_50p,	L"HD1080-50p",	L"eMVF_HD1080_50p",		{50, 1},			eMI_Progressive,	1920,	1080,	16,		9,		1, 45 },
	{ eMVF_HD1080_5994p,L"HD1080-59p",	L"eMVF_HD1080_5994p",	{60000, 1001},  	eMI_Progressive,	1920,	1080,	16,		9,		1, 45 },
	{ eMVF_HD1080_60p,	L"HD1080-60p",	L"eMVF_HD1080_60p",		{60, 1},			eMI_Progressive,	1920,	1080,	16,		9,		1, 45 },

	{ eMVF_2K_DCI_2398p,L"2K-DCI-23p",	L"eMVF_2K_DCI_2398p",	{24000, 1001},  	eMI_Progressive,	2048,	1080,	256,	135,    2, 0 },
	{ eMVF_2K_DCI_24p,	L"2K-DCI-24p",	L"eMVF_2K_DCI_24p",		{24, 1},			eMI_Progressive,	2048,	1080,	256,	135,    2, 0 },
	{ eMVF_2K_DCI_25p,	L"2K-DCI-25p",	L"eMVF_2K_DCI_25p",		{25, 1},			eMI_Progressive,	2048,	1080,	256,	135,    2, 0 },
	{ eMVF_2K_DCI_50p,	L"2K-DCI-50p",	L"eMVF_2K_DCI_50p",		{50, 1},			eMI_Progressive,	2048,	1080,	256,	135,    2, 0 },
	{ eMVF_2K_DCI_5994p,L"2K-DCI-59p",	L"eMVF_2K_DCI_5994p",	{60000, 1001},  	eMI_Progressive,	2048,	1080,	256,	135,    2, 0 },
	{ eMVF_2K_DCI_60p,	L"2K-DCI-60p",	L"eMVF_2K_DCI_60p",		{60, 1},			eMI_Progressive,	2048,	1080,	256,	135,    2, 0 },

	{ eMVF_2K_2398p,	L"2K-23p",		L"eMVF_2K_2398p",		{24000, 1001},  	eMI_Progressive,	2048,	1556,	239,	100,	2, 0 },
	{ eMVF_2K_24p,		L"2K-24p",		L"eMVF_2K_24p",			{24, 1},			eMI_Progressive,	2048,	1556,	239,	100,    2, 0 },
	{ eMVF_2K_25p,		L"2K-25p",		L"eMVF_2K_25p",			{25, 1},			eMI_Progressive,	2048,	1556,	239,	100,    2, 0 },

	{ eMVF_4K_UHD_50i,	L"4K-UHD-50i",	L"eMVF_4K_UHD_50i",		{25, 1},			eMI_Field1First,	3840,	2160,	16,		9,		4, 0 },
	{ eMVF_4K_UHD_5994i,L"4K-UHD-59i",	L"eMVF_4K_UHD_5994i",	{30000, 1001},  	eMI_Field1First,	3840,	2160,	16,		9,		4, 0 },
	{ eMVF_4K_UHD_60i,	L"4K-UHD-60i",	L"eMVF_4K_UHD_60i",		{30, 1},			eMI_Field1First,	3840,	2160,	16,		9,		4, 0 },

	{ eMVF_4K_UHD_2398p,L"4K-UHD-23p",	L"eMVF_4K_UHD_2398p",	{24000, 1001},  	eMI_Progressive,	3840,	2160,	16,		9,		4, 0 },
	{ eMVF_4K_UHD_24p,	L"4K-UHD-24p",	L"eMVF_4K_UHD_24p",		{24, 1},			eMI_Progressive,	3840,	2160,	16,		9,		4, 0 },
	{ eMVF_4K_UHD_25p,	L"4K-UHD-25p",	L"eMVF_4K_UHD_25p",		{25, 1},			eMI_Progressive,	3840,	2160,	16,		9,		4, 0 },
	{ eMVF_4K_UHD_2997p,L"4K-UHD-29p",	L"eMVF_4K_UHD_2997p",	{30000, 1001},  	eMI_Progressive,	3840,	2160,	16,		9,		4, 0 },
	{ eMVF_4K_UHD_30p,	L"4K-UHD-30p",	L"eMVF_4K_UHD_30p",		{30, 1},			eMI_Progressive,	3840,	2160,	16,		9,		4, 0 },

	{ eMVF_4K_DCI_2398p,L"4K-DCI-23p",	L"eMVF_4K_DCI_2398p",	{24000, 1001},  	eMI_Progressive,	4096,	2160,	256,	135,	4, 0 },
	{ eMVF_4K_DCI_24p,	L"4K-DCI-24p",	L"eMVF_4K_DCI_24p",		{24, 1},			eMI_Progressive,	4096,	2160,	256,	135,	4, 0 },
	{ eMVF_4K_DCI_25p,	L"4K-DCI-25p",	L"eMVF_4K_DCI_25p",		{25, 1},			eMI_Progressive,	4096,	2160,	256,	135,	4, 0 },

	{ eMVF_4K_UHD_50p,	L"4K-UHD-50p",	L"eMVF_4K_UHD_50p",		{50, 1},			eMI_Progressive,	3840,	2160,	16,		9,		8, 0 },
	{ eMVF_4K_UHD_5994p,L"4K-UHD-59p",	L"eMVF_4K_UHD_5994p",	{60000, 1001},  	eMI_Progressive,	3840,	2160,	16,		9,		8, 0 },
	{ eMVF_4K_UHD_60p,	L"4K-UHD-60p",	L"eMVF_4K_UHD_60p",		{60, 1},			eMI_Progressive,	3840,	2160,	16,		9,		8, 0 },

	{ eMVF_4K_DCI_50p,	L"4K-DCI-50p",	L"eMVF_4K_DCI_50p",		{50, 1},			eMI_Progressive,	4096,	2160,	256,	135,	8, 0 },
	{ eMVF_4K_DCI_5994p,L"4K-DCI-59p",	L"eMVF_4K_DCI_5994p",	{60000, 1001},  	eMI_Progressive,	4096,	2160,	256,	135,	8, 0 },
	{ eMVF_4K_DCI_60p,	L"4K-DCI-60p",	L"eMVF_4K_DCI_60p",		{60, 1},			eMI_Progressive,	4096,	2160,	256,	135,	8, 0 },

	{ eMVF_Disabled,	L"Disabled",	L"eMVF_Disabled",		{0, 1},				eMI_Default,		0,		0,		0,		0,		-1, 0 }
};

static inline VF_INFO* VF_InfoPtr(eMVideoFormat _eVF)
{
	if( _eVF < 0 || _eVF > SIZEOF_ARRAY( s_arrFormats ) )
		return NULL;

	return (VF_INFO*)&s_arrFormats[_eVF];
}

static inline eMVideoFormat VF_StrToVF(LPCWSTR _videoFormatStr, eMVideoFormat _eVFDefault = eMVF_Custom )
{
	for( int i = 0; i < SIZEOF_ARRAY( s_arrFormats ); i++ )
	{
		if( !lstrcmpW( s_arrFormats[i].pszName, _videoFormatStr ) || 
			!lstrcmpW( s_arrFormats[i].pszNameEx, _videoFormatStr ) )
		{
			return s_arrFormats[i].eVF;
		}
	}
	
	return _eVFDefault;
}

static inline LPCWSTR VF_Str(eMVideoFormat _eVF, LPCWSTR _pszDef = NULL )
{
	eMVideoFormat eVF = VF_CLEAR3D(_eVF);

	if( eVF < 0 || eVF > SIZEOF_ARRAY( s_arrFormats ) )
		return _pszDef;

	return s_arrFormats[eVF].pszName;
}

static inline LPCWSTR VF_StrEx(eMVideoFormat _eVF, LPCWSTR _pszDef = NULL )
{
	eMVideoFormat eVF = VF_CLEAR3D(_eVF);

	if( eVF < 0 || eVF > SIZEOF_ARRAY( s_arrFormats ) )
		return _pszDef;

	return s_arrFormats[eVF].pszNameEx;
}

static eM3DFormat eM_3DFormatIdx[] = { eM3D_None, eM3D_SideBySide, eM3D_TopAndBottom, eM3D_FullSideBySide, eM3D_FullTopAndBottom, eM3D_Tile, eM3D_Auto, eM3D_None };
static LPCWSTR	  eM_3DFormatIdxStr[] = { L"", L"3D-HSbS", L"3D-HTnB", L"3D-SbS", L"3D-TnB", L"3D-Auto", L"3D-Tile", L"none", NULL };
#define M3DF_MAX		SIZEOF_ARRAY(eM_3DFormatIdx)

// Next is for items w/o enum
static LPCWSTR eWaitStartTime_Str[] = { L"pause-in", L"pause-out", L"stop", L"skip", L"play", NULL };
static LPCWSTR eChangeType_Str[] = { L"normal", L"next", L"break", L"replace", L"next_no_cache", NULL };
static LPCWSTR eM_AudioTrackModeStr[] = { L"disabled", L"enabled", L"enabled-exclusive", L"mix-exclusive", L"enabled-add", L"disabled-add", NULL };

// Audio
static int eMAudio_Freq[] = {48000,44100,32000,22050,16000,11025,8000};
static int eMAudio_Channels[] = {2,4,6,8,10,12,14,16,1};
static int eMAudio_Bits[] = {16,/*20,24,*/-32,32};

inline LPCWSTR MState2Name( eMState _eState )
{
	switch( _eState )
	{
	case eMS_Closed: return L"eMS_Close";
	case eMS_Stopped: return L"eMS_Stopped";
	case eMS_Paused: return L"eMS_Paused";
	case eMS_Running: return L"eMS_Running";
	case eMS_Error: return L"eMS_Error";
	case eMS_Break: return L"eMS_Break";
	}

	ATLASSERT( !"MState2Name - WRONG STATE" );
	return L"###eMState###";
}

inline LPCWSTR DX_Mode2Str(DXGI_SWAP_EFFECT _eMode)
{	
	switch (_eMode)
	{
	case DXGI_SWAP_EFFECT_DISCARD: return L"DISCARD";
	case DXGI_SWAP_EFFECT_SEQUENTIAL: return L"SEQUENTIAL";
	case DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL: return L"FLIP_SEQUENTIAL";		
	//case DXGI_SWAP_EFFECT_FLIP_DISCARD: return L"FLIP_DISCARD"; // For Windows SDK Kit > 10.0
	default:
		break;
	}

	return L"#ERR#";
}

template<typename TFormat>
inline LPCWSTR DX_Format2Str(TFormat _eFormat, LPCWSTR _pszNone = L"NONE")
{
	DXGI_FORMAT eFormat = (DXGI_FORMAT)(_eFormat & 0xFF);
	switch (eFormat)
	{
	case DXGI_FORMAT_UNKNOWN: return _pszNone;
	case DXGI_FORMAT_R32G32B32A32_TYPELESS: return L"RGBA128T";
	case DXGI_FORMAT_R32G32B32A32_FLOAT: return L"RGBA128F";
	case DXGI_FORMAT_R32G32B32A32_UINT: return L"RGBA128U";
	case DXGI_FORMAT_R32G32B32A32_SINT: return L"RGBA128S";
	case DXGI_FORMAT_R32G32B32_TYPELESS: return L"RGB128T";
	case DXGI_FORMAT_R32G32B32_FLOAT: return L"RGB128F";
	case DXGI_FORMAT_R32G32B32_UINT: return L"RGB128U";
	case DXGI_FORMAT_R32G32B32_SINT: return L"RGB128S";
	case DXGI_FORMAT_R16G16B16A16_TYPELESS: return L"RGBA64T";
	case DXGI_FORMAT_R16G16B16A16_FLOAT: return L"RGBA64F";
	case DXGI_FORMAT_R16G16B16A16_UNORM: return L"RGBA64UN";
	case DXGI_FORMAT_R16G16B16A16_UINT: return L"RGBA64U";
	case DXGI_FORMAT_R16G16B16A16_SNORM: return L"RGBA64SN";
	case DXGI_FORMAT_R16G16B16A16_SINT: return L"RGBA64S";
	case DXGI_FORMAT_R32G32_TYPELESS: return L"RG64T";
	case DXGI_FORMAT_R32G32_FLOAT: return L"RG64F";
	case DXGI_FORMAT_R32G32_UINT: return L"RG64U";
	case DXGI_FORMAT_R32G32_SINT: return L"RG64S";
	case DXGI_FORMAT_R32G8X24_TYPELESS: return L"R32G8X24T";
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT: return L"D32_FLOAT_S8X24_UINT";
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS: return L"R32_FLOAT_X8X24_TYPELESS";
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT: return L"X32_TYPELESS_G8X24_UINT";
	case DXGI_FORMAT_R10G10B10A2_TYPELESS: return L"R10G10B10A2T";
	case DXGI_FORMAT_R10G10B10A2_UNORM: return L"R10G10B10A2UN";
	case DXGI_FORMAT_R10G10B10A2_UINT: return L"R10G10B10A2U";
	case DXGI_FORMAT_R11G11B10_FLOAT: return L"R11G11B10F";
	case DXGI_FORMAT_R8G8B8A8_TYPELESS: return L"RGB32T";
	case DXGI_FORMAT_R8G8B8A8_UNORM: return L"RGB32UN";
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB: return L"RGB32UNS";
	case DXGI_FORMAT_R8G8B8A8_UINT: return L"RGB32U";
	case DXGI_FORMAT_R8G8B8A8_SNORM: return L"RGB32SN";
	case DXGI_FORMAT_R8G8B8A8_SINT: return L"RGB32S";
	case DXGI_FORMAT_R16G16_TYPELESS: return L"R16G16T";
	case DXGI_FORMAT_R16G16_FLOAT: return L"R16G16F";
	case DXGI_FORMAT_R16G16_UNORM: return L"R16G16UN";
	case DXGI_FORMAT_R16G16_UINT: return L"R16G16U";
	case DXGI_FORMAT_R16G16_SNORM: return L"R16G16SN";
	case DXGI_FORMAT_R16G16_SINT: return L"R16G16S";
	case DXGI_FORMAT_R32_TYPELESS: return L"R32T";
	case DXGI_FORMAT_D32_FLOAT: return L"D32F";
	case DXGI_FORMAT_R32_FLOAT: return L"R32F";
	case DXGI_FORMAT_R32_UINT: return L"R32U";
	case DXGI_FORMAT_R32_SINT: return L"R32S";
	case DXGI_FORMAT_R24G8_TYPELESS: return L"R24G8T";
	case DXGI_FORMAT_D24_UNORM_S8_UINT: return L"D24_UNORM_S8_UINT";
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS: return L"R24_UNORM_X8_TYPELESS";
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT: return L"X24_TYPELESS_G8_UINT";
	case DXGI_FORMAT_R8G8_TYPELESS: return L"R8G8T";
	case DXGI_FORMAT_R8G8_UNORM: return L"R8G8UN";
	case DXGI_FORMAT_R8G8_UINT: return L"R8G8U";
	case DXGI_FORMAT_R8G8_SNORM: return L"R8G8SN";
	case DXGI_FORMAT_R8G8_SINT: return L"R8G8S";
	case DXGI_FORMAT_R16_TYPELESS: return L"R16T";
	case DXGI_FORMAT_R16_FLOAT: return L"R16F";
	case DXGI_FORMAT_D16_UNORM: return L"D16UN";
	case DXGI_FORMAT_R16_UNORM: return L"R16UN";
	case DXGI_FORMAT_R16_UINT: return L"R16U";
	case DXGI_FORMAT_R16_SNORM: return L"R16SN";
	case DXGI_FORMAT_R16_SINT: return L"R16S";
	case DXGI_FORMAT_R8_TYPELESS: return L"R8T";
	case DXGI_FORMAT_R8_UNORM: return L"R8UN";
	case DXGI_FORMAT_R8_UINT: return L"R8U";
	case DXGI_FORMAT_R8_SNORM: return L"R8SN";
	case DXGI_FORMAT_R8_SINT: return L"R8S";
	case DXGI_FORMAT_A8_UNORM: return L"A8U";
	case DXGI_FORMAT_R1_UNORM: return L"R1U";
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP: return L"R9G9B9E5_SHAREDEXP";
	case DXGI_FORMAT_R8G8_B8G8_UNORM: return L"R8G8_B8G8";
	case DXGI_FORMAT_G8R8_G8B8_UNORM: return L"G8R8_G8B8";
	case DXGI_FORMAT_BC1_TYPELESS: return L"BC1T";
	case DXGI_FORMAT_BC1_UNORM: return L"BC1UN";
	case DXGI_FORMAT_BC1_UNORM_SRGB: return L"BC1UNS";
	case DXGI_FORMAT_BC2_TYPELESS: return L"BC2T";
	case DXGI_FORMAT_BC2_UNORM: return L"BC2UN";
	case DXGI_FORMAT_BC2_UNORM_SRGB: return L"BC2UNS";
	case DXGI_FORMAT_BC3_TYPELESS: return L"BC3T";
	case DXGI_FORMAT_BC3_UNORM: return L"BC3UN";
	case DXGI_FORMAT_BC3_UNORM_SRGB: return L"BC3UNS";
	case DXGI_FORMAT_BC4_TYPELESS: return L"BC4T";
	case DXGI_FORMAT_BC4_UNORM: return L"BC4UN";
	case DXGI_FORMAT_BC4_SNORM: return L"BC4SN";
	case DXGI_FORMAT_BC5_TYPELESS: return L"BC5T";
	case DXGI_FORMAT_BC5_UNORM: return L"BC5UN";
	case DXGI_FORMAT_BC5_SNORM: return L"BC5SN";
	case DXGI_FORMAT_B5G6R5_UNORM: return L"BGR555UN";
	case DXGI_FORMAT_B5G5R5A1_UNORM: return L"BGRA5551UN";
	case DXGI_FORMAT_B8G8R8A8_UNORM: return L"BGRA32UN";
	case DXGI_FORMAT_B8G8R8X8_UNORM: return L"BGRX32UN";
	case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM: return L"UNK";
	case DXGI_FORMAT_B8G8R8A8_TYPELESS: return L"BGRA32T";
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB: return L"BGRA32UNS";
	case DXGI_FORMAT_B8G8R8X8_TYPELESS: return L"BGRX32T";
	case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB: return L"BGRX32UNS";
	case DXGI_FORMAT_BC6H_TYPELESS: return L"BC6HT";
	case DXGI_FORMAT_BC6H_UF16: return L"BC6H_UF16";
	case DXGI_FORMAT_BC6H_SF16: return L"BC6H_SF16";
	case DXGI_FORMAT_BC7_TYPELESS: return L"BC7T";
	case DXGI_FORMAT_BC7_UNORM: return L"BC7UN";
	case DXGI_FORMAT_BC7_UNORM_SRGB: return L"BC7UNS";
	case DXGI_FORMAT_AYUV: return L"AYUV";
	case DXGI_FORMAT_Y410: return L"Y410";
	case DXGI_FORMAT_Y416: return L"Y416";
	case DXGI_FORMAT_NV12: return L"NV12";
	case DXGI_FORMAT_P010: return L"P010";
	case DXGI_FORMAT_P016: return L"P016";
	case DXGI_FORMAT_420_OPAQUE: return L"O420";
	case DXGI_FORMAT_YUY2: return L"YUY2";
	case DXGI_FORMAT_Y210: return L"Y210";
	case DXGI_FORMAT_Y216: return L"Y216";
	case DXGI_FORMAT_NV11: return L"NV11";
	case DXGI_FORMAT_AI44: return L"AI44";
	case DXGI_FORMAT_IA44: return L"IA44";
	case DXGI_FORMAT_P8: return L"P8";
	case DXGI_FORMAT_A8P8: return L"A8P8";
	case DXGI_FORMAT_B4G4R4A4_UNORM: return L"BGRA4444UN";
	default:
		break;
	}

	return L"#ERR#";
}

inline HRESULT MFrameUpdateLUFS( IUnknown* _cpFrame, LUFS_METERS _lufsMeters )
{
	CComQIPtr<IMFFrame> qpMFFrame( _cpFrame );
	if( !qpMFFrame )
		return E_INVALIDARG;

	M_AV_PROPS avProps = MFFrame_AVProps( qpMFFrame );

	avProps.ancData.audOriginal.lufs = _lufsMeters;
	avProps.ancData.audOutput.lufs = _lufsMeters;
	
	HRESULT hr = qpMFFrame->MFAVPropsSet( &avProps, CComBSTR( L"override" ) );
	return hr;
}

inline REFERENCE_TIME Time2RT( LONGLONG _llTime, const M_RATIONAL* _pTimeBase )
{
	if( _llTime != MINLONGLONG && _pTimeBase && _pTimeBase->nNum > 0 && _pTimeBase->nDen > 0 )
	{
		return DBL2INT64( ((double)_llTime * _pTimeBase->nNum / _pTimeBase->nDen) * DS_ONESEC );
	}

	return _llTime;
}

inline REFERENCE_TIME RT2Time( REFERENCE_TIME _rtTime, const M_RATIONAL* _pTimeBase )
{
	if( _rtTime != MINLONGLONG && _pTimeBase && _pTimeBase->nNum > 0 && _pTimeBase->nDen > 0 )
	{
		return DBL2INT64( (double)_rtTime * _pTimeBase->nDen / DS_ONESEC / _pTimeBase->nNum );
	}

	return _rtTime;
}

inline RECT_DBL MFRect2Rect( MF_RECT* _pRect, M_VID_PROPS* _pVidProps, double dblCharAR = 0.55, double* _pdblRowHeight = NULL )
{
	RECT_DBL rcRes = {};
	ATLASSERT( _pRect );
	if( _pRect && _pRect->eRectType == eMFRT_Relative )
	{
		ATLASSERT( _pVidProps );
		if( _pVidProps )
		{
			rcRes.left = _pRect->dblPosX * _pVidProps->nWidth;
			rcRes.top = _pRect->dblPosY * ABS(_pVidProps->nHeight);
			rcRes.right = rcRes.left + _pRect->dblWidth * _pVidProps->nWidth;
			rcRes.bottom = rcRes.top + _pRect->dblHeight * ABS(_pVidProps->nHeight);
		}
	}
	else if( _pRect && _pRect->eRectType == eMFRT_Absolute )
	{
		rcRes.left = _pRect->dblPosX;
		rcRes.top = _pRect->dblPosY;
		rcRes.right = rcRes.left + _pRect->dblWidth;
		rcRes.bottom = rcRes.top + _pRect->dblHeight;
	}
	else 
	{
		int nRows = _pRect->eRectType == eMFRT_FixedCC_15x32 ? 15 : (int)_pRect->eRectType;
		ATLASSERT( nRows >= 2 );

		ATLASSERT( _pVidProps );
		if( _pVidProps )
		{
			double dblOverscanX = _pVidProps->nWidth * 0.07;
			double dblOverscanY = ABS(_pVidProps->nHeight) * 0.07;
			double dblRowHeight = (ABS(_pVidProps->nHeight) - dblOverscanY * 2) / nRows; // 0.9 for overscan 5%
			double dblCharWidth = MAX( 0.1, dblCharAR ) *  dblRowHeight;

			rcRes.left = dblOverscanX + _pRect->dblPosX * dblCharWidth;
			rcRes.top = dblOverscanY + _pRect->dblPosY * dblRowHeight;

			rcRes.right = rcRes.left + _pRect->dblWidth * dblCharWidth;
			rcRes.bottom = rcRes.top + _pRect->dblHeight * dblRowHeight;

			if( _pdblRowHeight )
				*_pdblRowHeight = dblRowHeight;
		}

	}


	return rcRes;
}

// VANC Line
struct VANCLine
{
	short	sLineNo;
	short	cbLine;
	short	pbRow[1];

	static int LineSize( int _cbRowBytes)
	{
		return _cbRowBytes + offsetof( VANCLine, pbRow );
	}

	LPBYTE RowPtr()
	{
		return (LPBYTE)pbRow;
	}
};

inline bool Img_IsSupportM( DWORD _dwFCC, bool _bScaleOrOverlay = false )
{
	switch(_dwFCC)
	{
		// Supported
	case FOURCC_YUY2 :
	case FOURCC_UYVY :
	case FOURCC_HDYC :
	case FOURCC_YVYU :
	case FOURCC_ARGB32 :	
	case FOURCC_RGB32 :
	case FOURCC_RGB24 :
	case FOURCC_NV12 :
		return true;

		// Planned
	case FOURCC_YV12 :
	case FOURCC_I420 :	
		return false;

	case FOURCC_RGB8 :
	case FOURCC_v210 :
	case FOURCC_r210 :
		return true;
	}

	// Unknown 
	return false;
}

inline bool Img_IsSupportM( const PIC_PROPS* _pProps )
{
	return Img_IsSupportM( _pProps->fccType );
}

inline bool Img_IsSupportM( const M_VID_PROPS* _pProps )
{
	return Img_IsSupportM( (DWORD)_pProps->fccType );
}


inline DWORD Img_SizeM( const PIC_PROPS* _pProps )
{
	if( !_pProps) return 0;

	// Special fix fir v210
	if( _pProps->fccType == FOURCC_v210 )
	{
		return (((_pProps->nRowWidth ? _pProps->nRowWidth : _pProps->nWidth) + 47) / 48) * 128 * ABS( _pProps->nHeight );
	}
	// VVB: No reason for alight to 256 bytes 
// 	else if( _pProps->fccType == FOURCC_r210 )
// 	{
// 		return (((_pProps->nRowWidth ? _pProps->nRowWidth : _pProps->nWidth) + 63) / 64) * 256 * ABS( _pProps->nHeight );
// 	}

	if( !IsPlanar(_pProps->fccType) )
	{
		int cbRowSize = ( (_pProps->nRowWidth ? _pProps->nRowWidth : _pProps->nWidth) * Fourcc2Bits(_pProps->fccType ) ) >> 3;
		cbRowSize = ALIGN_UP( cbRowSize, 4 );

		return cbRowSize * ABS( _pProps->nHeight );
	}

	return ( (_pProps->nRowWidth ? _pProps->nRowWidth : _pProps->nWidth) * ABS( _pProps->nHeight ) * 
		Fourcc2Bits(_pProps->fccType ) ) >> 3;
}

inline DWORD Img_RowSizeM( DWORD _dwFCC, int _nWidth )
{
	if( _dwFCC == FOURCC_v210 )
	{
		return ((_nWidth + 47) / 48) * 128;
	}
	// VVB: No reason for alight to 256 bytes 
// 	else if( _dwFCC == FOURCC_r210 )
// 	{
// 		return ((_nWidth + 63) / 64) * 256;
// 	}

	if( IsPlanar( _dwFCC  ) ) 
		return _nWidth;

	if( !Img_IsSupportM( _dwFCC ) )
		return 0;

	DWORD cbSize = ( _nWidth * Fourcc2Bits(_dwFCC) ) >> 3;

	// Special fix for RGB8 - we should have 8-bytes alignment
	return ALIGN_UP( cbSize, _dwFCC == eMFCC_RGB8 ? 8 : 4 );
}

inline DWORD Img_RowSizeM( const PIC_PROPS* _pProps )
{
	if( !_pProps) return 0;

	return Img_RowSizeM( _pProps->fccType, _pProps->nRowWidth ? _pProps->nRowWidth : _pProps->nWidth );
}

inline DWORD Img_RowSizeM( const M_VID_PROPS* _pVidProps )
{
	if( !_pVidProps ) return 0;

	return Img_RowSizeM( _pVidProps->fccType, _pVidProps->nWidth );
}

inline DWORD Img_RowWidthM( DWORD _dwFCC, int _nRowBytes )
{
	if( !_dwFCC )
		return 0;

	// Special fix fir v210
	if( _dwFCC == FOURCC_v210 )
	{
		return (_nRowBytes / 128) * 48;
	}
//	VVB: No reason for alight to 256 bytes 
// 	else if( _dwFCC == FOURCC_r210 )
// 	{
// 		return (_nRowBytes / 256) * 64;
// 	}

	if( IsPlanar( _dwFCC ) ) 
		return _nRowBytes;

	int nBits = Fourcc2Bits(_dwFCC );
	if( nBits == 0 )
		return _nRowBytes;

	return _nRowBytes * 8 / nBits;
}

inline DWORD Img_RowWidthM( const PIC_PROPS* _pProps, int _nRowBytes )
{
	if( !_pProps) return 0;

	return Img_RowWidthM( _pProps->fccType, _nRowBytes );
}
inline DWORD Img_RowWidthM( const M_VID_PROPS* _pProps )
{
	if( !_pProps) return 0;

	return Img_RowWidthM( _pProps->fccType, _pProps->nRowBytes );
}

inline M_VID_PROPS Img_MaskVidProps(const M_VID_PROPS* _pVidProps)
{
	M_VID_PROPS vidPropsMask = {};
	ATLASSERT(_pVidProps);
	if (_pVidProps)
	{
		vidPropsMask = *_pVidProps;
		vidPropsMask.fccType = eMFCC_RGB8;
		vidPropsMask.nHeight = -1 * ABS(vidPropsMask.nHeight); // Mask ALWAYS top-bottom 
		if (!IsRGB(_pVidProps->fccType))
			vidPropsMask.nWidth = ALIGN_DOWN(_pVidProps->nWidth, 2);

		// Note: Use Img_RowWidthM for correct calc mask props for e.g. first/second field vid props
		vidPropsMask.nRowBytes = ALIGN_UP(Img_RowWidthM(_pVidProps), 8); // Mask ALWAYS 8-bytes aligned
	}

	return vidPropsMask;
}


//inline LPCWSTR VF_Str( eMVideoFormat _eVF )
//{
//	return VF_InfoPtr(_eVF)->pszName;
//
//// 	eMVideoFormat eVF = VF_CLEAR3D(_eVF);
//// 	ATLASSERT( (int)eVF < (int)eMVF_Disabled );
//// 	if( (int)eVF >= (int)eMVF_Disabled ) return NULL;
//// 	return eM_VideoFormatStr[ (int)eVF ];
//}

inline int M3D_Idx( eM3DFormat _e3DF ) 
{
	_e3DF = VF_GET3D( _e3DF );

	for( int i = 0; i < M3DF_MAX; i++ )
	{
		if( eM_3DFormatIdx[i] == _e3DF )
			return i;
	}

	return 0;
}

inline LPCWSTR M3D_Str( eM3DFormat _e3DF )
{
	return eM_3DFormatIdxStr[ M3D_Idx(_e3DF) ];
}

#ifdef NEW_XML_PARSE

inline eMVideoFormat Str_VF( LPCWSTR _pszStr, eM3DFormat* _pe3DFormat = NULL )
{
	if( !_pszStr )
		return eMVF_Custom;

	// Check regular formats	
	if( VF_StrToVF(_pszStr) > 0 )
		return VF_StrToVF(_pszStr);	
	
	// Check 3D formats
	int n3DFormat = 1;
	WCHAR szTemp[128]={};
	LPCWSTR ppCheck[] = { L"eMVF_3D_SbS_", L"eMVF_3D_TnB_", L"eMVF_3D_Full_SbS_", L"eMVF_3D_Full_TnB_", NULL };
	LPCWSTR* pCheck = ppCheck;
	while( *pCheck && n3DFormat < M3DF_MAX )
	{
		int nLen = lstrlenW( *pCheck );
		if( StrCmpNW( _pszStr, *pCheck, nLen) == 0 )
		{
			// e.g. eMVF_3D_SbS_1080_2397p -> eMVF_HD1080_2397p
			wsprintfW( szTemp, L"eMVF_HD%ws", _pszStr + nLen );			
			if( VF_StrToVF(szTemp) > 0 )
			{
				if( _pe3DFormat )
				{
					*_pe3DFormat = eM_3DFormatIdx[n3DFormat];
				}

				return (eMVideoFormat)(VF_StrToVF(szTemp)|eM_3DFormatIdx[n3DFormat]);
			}

			return eMVF_Custom;
		}

		pCheck++;
		n3DFormat++;
	}
	
	return eMVF_Custom;
}
#endif // #ifdef NEW_XML_PARSE

inline double VF_Rate( eMVideoFormat _eVF )
{
	const VF_INFO* pVFInfo = VF_InfoPtr( VF_CLEAR3D(_eVF) );
	if( pVFInfo )
		return (double)pVFInfo->frameRate.nNum / (double)pVFInfo->frameRate.nDen;

	return 0;
}

inline int VF_IsDropFrame( eMVideoFormat _eVF )
{
	const VF_INFO* pVFInfo = VF_InfoPtr( VF_CLEAR3D(_eVF) );
	if( pVFInfo )
		return pVFInfo->frameRate.nDen == 1001 ? 1 : 0;

	return 0;
}

inline MRational VF_RateRational( eMVideoFormat _eVF )
{
	MRational res = {};
	const VF_INFO* pVFInfo = VF_InfoPtr( VF_CLEAR3D(_eVF) );
	if( pVFInfo )
	{
		res.nNum = pVFInfo->frameRate.nNum;
		res.nDen = pVFInfo->frameRate.nDen;
	}

	return res;
}

inline eMInterlace VF_Interlace( eMVideoFormat _eVF )
{
	const VF_INFO* pVFInfo = VF_InfoPtr( VF_CLEAR3D(_eVF) );
	if( pVFInfo )
		return pVFInfo->eInterlace;
	
	return eMI_Default;
}

inline SIZE VF_Res( eMVideoFormat _eVF )
{
	SIZE szRes = {0,0};

	const VF_INFO* pVFInfo = VF_InfoPtr( VF_CLEAR3D(_eVF) );
	if( pVFInfo )
	{
		szRes.cx = pVFInfo->nWidth;
		szRes.cy = pVFInfo->nHeight;
	}		

	szRes.cx *= VF_3DSCALE_X( _eVF );
	szRes.cy *= VF_3DSCALE_Y( _eVF );

	return szRes;
}

inline SIZE VF_AR( eMVideoFormat _eVF )
{	
	SIZE szRes = {0,0};

	const VF_INFO* pVFInfo = VF_InfoPtr( VF_CLEAR3D(_eVF) );
	if( pVFInfo )
	{
		szRes.cx = pVFInfo->nAspectX;
		szRes.cy = pVFInfo->nAspectY;
		return szRes;
	}		

	return szRes;
}

inline int VF_IsNTSC( eMVideoFormat _eVF )
{
	switch( _eVF )
	{
	case eMVF_NTSC:
		return 1;
	case eMVF_NTSC_2398:
		return 2;
	case eMVF_NTSC_16x9:
		return 3;
	}

	return 0;
}

inline int VF_IsHD( eMVideoFormat _eVF )
{
	const VF_INFO* pVFInfo = VF_InfoPtr( VF_CLEAR3D(_eVF) );
	if( pVFInfo )
	{
		return pVFInfo->nHDType;
	}		

	return -1;
}

inline bool VF_IsSD( eMVideoFormat _eVF )
{
	return VF_IsHD(_eVF) == 0;
}

inline bool VF_Is4K(eMVideoFormat _eVF)
{

	const VF_INFO* pVFInfo = VF_InfoPtr(VF_CLEAR3D(_eVF));

	return pVFInfo && (pVFInfo->eVF >= eMVF_4K_UHD_50i && pVFInfo->eVF <= eMVF_4K_DCI_60p);
}

inline bool Is4K(M_VID_PROPS* _pVidProps)
{
	if (!_pVidProps)
		return false;

	if (_pVidProps->nWidth >= 3840 && ABS(_pVidProps->nHeight) == 2160)
		return true;

	if (VF_Is4K(_pVidProps->eVideoFormat))
		return true;

	return false;
}

inline bool IsSD( M_VID_PROPS* _pVidProps )
{
	if( !_pVidProps )
		return false;

	if( _pVidProps->nWidth == 720 && 
		(ABS(_pVidProps->nHeight) == 576 ||
		ABS(_pVidProps->nHeight) == 486 ||
		ABS(_pVidProps->nHeight) == 480 ) )
		return true;

	if( VF_IsSD( _pVidProps->eVideoFormat ) )
		return true;

	return false;
}

inline int VF_VBI_Lines( eMVideoFormat _eVF )
{	
	const VF_INFO* pVFInfo = VF_InfoPtr( VF_CLEAR3D(_eVF) );
	if( pVFInfo )
	{
		return pVFInfo->nVBILines;
	}		

	return 0;
}

inline eM3DFormat VF_3DFormat( eMVideoFormat _eVF )
{
	return (eM3DFormat)(_eVF & (eM3D_TopAndBottom|eM3D_SideBySide|eM3D_FullTopAndBottom|eM3D_FullSideBySide));
}

inline M_VID_PROPS VF_MVidProps( eMVideoFormat _eVF, eMFCC _eFCC = eMFCC_Default )
{
	M_VID_PROPS vidProps = {};
	if( _eVF != eMVF_Custom )
	{
		vidProps.eVideoFormat = _eVF;
		vidProps.fccType = _eFCC != eMFCC_Default ? _eFCC : VF_IsHD( _eVF ) ? eMFCC_HDYC : eMFCC_UYVY;
		SIZE szFrame = VF_Res( _eVF );
		vidProps.nWidth = szFrame.cx;
		vidProps.nHeight = IsRGB( vidProps.fccType ) ? -1 * ABS(szFrame.cy) : szFrame.cy;
		vidProps.dblRate = VF_Rate( _eVF );
		vidProps.eInterlace = VF_Interlace( _eVF );
		vidProps.nRowBytes = Img_RowSizeM( vidProps.fccType, vidProps.nWidth );
		vidProps.nAspectX = (short)VF_AR( _eVF ).cx;
		vidProps.nAspectY = (short)VF_AR( _eVF ).cy;
	}

	return vidProps;
}

inline bool VF_IsCorrect( eMVideoFormat _eVF )
{
	// Classic formats
	if( _eVF >= eMVF_Custom && _eVF < eMVF_Disabled )
		return true;

	eM3DFormat e3D = VF_3DFormat(_eVF);
	if( e3D != eM3D_None )
	{
		switch( VF_CLEAR3D(_eVF) )
		{
		case eMVF_HD720_50p:
		case eMVF_HD720_5994p:
		case eMVF_HD720_60p:
		case eMVF_HD1080_2398p:
		case eMVF_HD1080_24p:
			// next format is not standard
		case eMVF_HD1080_25p:
		case eMVF_HD1080_2997p:
		case eMVF_HD1080_30p:
			return true;

		case eMVF_HD1080_50i:
		case eMVF_HD1080_5994i:
		case eMVF_HD1080_60i:
			return e3D != eM3D_TopAndBottom;

		case eMVF_HD1080_50p:
		case eMVF_HD1080_5994p:
		case eMVF_HD1080_60p:
			return (e3D & eM3D_FullFlag) == 0;
		}
	}

	return false;
}

inline int IsInterlaced( const M_VID_PROPS* _pVidProps )
{
	if( !_pVidProps ) return 0;

	eMInterlace eInterlace = VF_Interlace( _pVidProps->eVideoFormat );

	if( _pVidProps->eInterlace == eMI_Progressive || (_pVidProps->eInterlace == eMI_Default && eInterlace == eMI_Progressive))
		return 0;

	if( _pVidProps->eInterlace == eMI_Field1First || (_pVidProps->eInterlace == eMI_Default && eInterlace == eMI_Field1First))
		return 1;

	if(_pVidProps->eInterlace == eMI_Field2First || (_pVidProps->eInterlace == eMI_Default && eInterlace == eMI_Field2First))
		return -1;

	return 0; 
}

inline REFERENCE_TIME MVid2AvgTime( const M_VID_PROPS* _pVidProps, bool _bField, REFERENCE_TIME _rtDefTime = 0)
{
	if (!_pVidProps)
		return _rtDefTime;

	double dblTime = Rate2AvgTimeDbl( _pVidProps->dblRate );
	if( IsInterlaced( _pVidProps ) != 0 && _bField )
		dblTime /= 2;
	return dblTime > 0 ? DBL2INT64( dblTime ) : _rtDefTime;
}

inline double MVid2AvgTimeDbl( const M_VID_PROPS* _pVidProps, bool _bField, double _dblDefTime = 0)
{
	if (!_pVidProps)
		return _dblDefTime;

	double dblTime = Rate2AvgTimeDbl( _pVidProps->dblRate );
	if( IsInterlaced( _pVidProps ) != 0 && _bField )
		dblTime /= 2;
	return dblTime > 0 ? dblTime  : _dblDefTime;
}

inline LONGLONG FrameTime2Index( REFERENCE_TIME _rtTime, double dblRate, REFERENCE_TIME* _prtStartTime = NULL )
{
	LONGLONG llIndex = 0;
	double dblAvgTime = Rate2AvgTimeDbl(dblRate);
	if( dblAvgTime == (int)dblAvgTime )
	{
		// e.g. PAL
		llIndex = (LONGLONG)_rtTime / dblAvgTime;
	}
	else
	{
		llIndex = (LONGLONG)(_rtTime + 1 * DS_MSEC) / dblAvgTime;
	}

	if( _prtStartTime )
		* _prtStartTime = DBL2INT64( llIndex * dblAvgTime );

	return llIndex;
}

inline double MVidProps_AR( const M_VID_PROPS* _pVidProps )
{
	ATLASSERT( _pVidProps );
	if( !_pVidProps ) return 1.0;

	return _pVidProps->nAspectY <= 0 || _pVidProps->nAspectX <= 0 ?
		(_pVidProps->nHeight != 0 ? (double)_pVidProps->nWidth / ABS(_pVidProps->nHeight) : 16.0/9.0 ): 
		(double)_pVidProps->nAspectX / _pVidProps->nAspectY;
}

inline double MVidProps_PixelAR( const M_VID_PROPS* _pVidProps )
{
	// Pixel AR : (AR_X / AR_Y) / ( W / H ) = AR_X * H / AR_Y * W;
	ATLASSERT( _pVidProps );
	if( !_pVidProps ) return 1.0;

	return _pVidProps->nWidth > 0 ? MVidProps_AR( _pVidProps ) * ABS(_pVidProps->nHeight) / _pVidProps->nWidth : 1.0;
}

// _nScaleDir > 0 -> Upscale
// _nScaleDir < 0 -> Downscale
// _nScaleDir = 0 -> Scale both direction
static M_VID_PROPS MVidProps_Scale(M_VID_PROPS* _pVidPropsIn, int _nNewWidth, int _nNewHeight, int _nScaleDir)
{
	M_VID_PROPS vidPropsScale = {};
	if (!_pVidPropsIn)
		return vidPropsScale;

	double dblAr = MVidProps_AR(_pVidPropsIn);

	vidPropsScale = *_pVidPropsIn;
	if ((_nNewHeight <= 0 && _nNewWidth <= 0) || dblAr <= 0)
		return vidPropsScale;


	if (_nNewHeight <= 0)
		_nNewHeight = _nNewWidth / dblAr;

	if (_nNewWidth <= 0)
		_nNewHeight = _nNewHeight * dblAr;

	if (_nScaleDir > 0)
	{
		int nHeight = MAX(_nNewHeight, ABS(_pVidPropsIn->nHeight));
		int nWidth = MAX(_nNewWidth, ABS(_pVidPropsIn->nWidth));

		vidPropsScale.nHeight = MAX(nHeight, DBL2INT(nWidth / dblAr));
		vidPropsScale.nWidth = MAX(nWidth, DBL2INT(nHeight * dblAr));
	}
	else if (_nScaleDir < 0)
	{
		int nHeight = MIN(_nNewHeight, ABS(_pVidPropsIn->nHeight));
		int nWidth = MIN(_nNewWidth, ABS(_pVidPropsIn->nWidth));

		vidPropsScale.nHeight = MIN(nHeight, DBL2INT(nWidth / dblAr));
		vidPropsScale.nWidth = MIN(nWidth, DBL2INT(nHeight * dblAr));
	}
	else
	{
		if (_nNewHeight >= _pVidPropsIn->nWidth && _nNewHeight >= _pVidPropsIn->nHeight)
		{
			vidPropsScale.nHeight = MAX(_nNewHeight, DBL2INT(_nNewWidth / dblAr));
			vidPropsScale.nWidth = MAX(_nNewWidth, DBL2INT(_nNewHeight * dblAr));
		}
		else
		{
			vidPropsScale.nHeight = MIN(_nNewHeight, DBL2INT(_nNewWidth / dblAr));
			vidPropsScale.nWidth = MIN(_nNewWidth, DBL2INT(_nNewHeight * dblAr));
		}
	}

	vidPropsScale.eVideoFormat = eMVF_Custom;
	vidPropsScale.nHeight *= SIGN(_pVidPropsIn->nHeight);
	vidPropsScale.nAspectX = 0;
	vidPropsScale.nAspectY = 0;
	vidPropsScale.nRowBytes = Img_RowBytesM(&vidPropsScale);
	vidPropsScale.eScaleType = eMST_IgnoreAR;

	return vidPropsScale;
}

inline REFERENCE_TIME MVidProps_FieldRT(const M_VID_PROPS* _pVidProps)
{
	ATLASSERT(_pVidProps);
	if (!_pVidProps) return 0;

	REFERENCE_TIME rtTime = Rate2AvgTime(_pVidProps->dblRate, 0);

	if (IsInterlaced(_pVidProps))
		rtTime /= 2;

	return rtTime;
}

inline eMVideoFormat VF_FindByRate( const M_VID_PROPS* _pVidProps, eMVideoFormat* _peFormat = NULL )
{		
	while( *_peFormat >= 0 )
	{
		eMVideoFormat eFormat = *_peFormat;

		if( IS_RATE( _pVidProps->dblRate, VF_Rate( eFormat ) ) && 
			(_pVidProps->eInterlace == eMI_Default || _pVidProps->eInterlace == VF_Interlace( eFormat ) ) )
		{
			return eFormat;
		}

		_peFormat++;
	}	

	return eMVF_Custom;
}

inline eMVideoFormat MVidProps_VF( const M_VID_PROPS* _pVidProps, bool _bExactRes = false )
{
	ATLASSERT( _pVidProps );
	if( !_pVidProps ) return eMVF_Custom;

	eM3DFormat e3D = eM3D_None;

	double dblAR = MVidProps_AR( _pVidProps );

	if( _pVidProps->nWidth == 720 && ABS( _pVidProps->nHeight ) >= 576 && ABS( _pVidProps->nHeight ) <= 576 + (_bExactRes ? 0 : VF_VBI_Lines( eMVF_NTSC ) ) ) // 49
	{
		if( IS_RATE( _pVidProps->dblRate, VF_Rate( eMVF_PAL ) ) )
			return dblAR > 1.4 ? eMVF_PAL_16x9 : eMVF_PAL;
	}

	if( _pVidProps->nWidth == 720 && ABS( _pVidProps->nHeight ) >= 480 && ABS( _pVidProps->nHeight ) <= 486 + (_bExactRes ? 0 : VF_VBI_Lines( eMVF_NTSC ) ) ) // 39
	{
		if( IS_RATE( _pVidProps->dblRate, VF_Rate( eMVF_NTSC ) ) )
			return dblAR > 1.4 ? eMVF_NTSC_16x9 : eMVF_NTSC;

		if( IS_RATE( _pVidProps->dblRate, VF_Rate( eMVF_NTSC_2398 ) ) )
			return eMVF_NTSC_2398;
	}

	if( _pVidProps->nWidth == 1280 && ABS( _pVidProps->nHeight ) >= 720 && ABS( _pVidProps->nHeight ) <= 720 + (_bExactRes ? 0 : VF_VBI_Lines( eMVF_HD720_60p )) ) // 30
	{
		e3D = (eM3DFormat)(e3D & (eM3D_SideBySide|eM3D_TopAndBottom)); // Can't be full with such res

		eMVideoFormat pFormatList[] = { eMVF_HD720_5994p, eMVF_HD720_60p, eMVF_HD720_50p, (eMVideoFormat)-1 };
		eMVideoFormat eFormat = VF_FindByRate( _pVidProps, pFormatList );

		if (eFormat > eMVF_Custom)
			return eMVideoFormat(eFormat|e3D);
	}

	if( _pVidProps->nWidth == 1920 && ABS( _pVidProps->nHeight ) >= 1080 && ABS( _pVidProps->nHeight ) <= 1080 + (_bExactRes ? 0 : VF_VBI_Lines( eMVF_HD1080_50i )) ) // 45
	{
		e3D = (eM3DFormat)(e3D & (eM3D_SideBySide|eM3D_TopAndBottom)); // Can't be full with such res
		
		eMVideoFormat pFormatList[] = { eMVF_HD1080_2398p, eMVF_HD1080_24p, eMVF_HD1080_50i, eMVF_HD1080_25p, eMVF_HD1080_5994i,
			eMVF_HD1080_2997p, eMVF_HD1080_60i, eMVF_HD1080_30p, eMVF_HD1080_50p, eMVF_HD1080_5994p, eMVF_HD1080_60p, (eMVideoFormat)-1 };
		eMVideoFormat eFormat = VF_FindByRate( _pVidProps, pFormatList );

		if (eFormat > eMVF_Custom)
			return eMVideoFormat(eFormat|e3D);
	}

	// 2K formats
	if( _pVidProps->nWidth == 2048 && ABS( _pVidProps->nHeight ) >= 1080 && ABS( _pVidProps->nHeight ) <= 1080 + VF_VBI_Lines(eMVF_2K_DCI_2398p) ) // 45
	{
		eMVideoFormat pFormatList[] = { eMVF_2K_DCI_2398p, eMVF_2K_DCI_24p, eMVF_2K_DCI_25p, eMVF_2K_DCI_50p, eMVF_2K_DCI_5994p, eMVF_2K_DCI_60p, (eMVideoFormat)-1 };
		eMVideoFormat eFormat = VF_FindByRate( _pVidProps, pFormatList );

		if (eFormat > eMVF_Custom)
			return eFormat;
	}	
	if( _pVidProps->nWidth == 2048 && ABS( _pVidProps->nHeight ) >= 1556 && ABS( _pVidProps->nHeight ) <= 1556 + VF_VBI_Lines(eMVF_2K_2398p) ) // 45
	{
		eMVideoFormat pFormatList[] = { eMVF_2K_2398p, eMVF_2K_24p, eMVF_2K_25p, (eMVideoFormat)-1 };
		eMVideoFormat eFormat = VF_FindByRate( _pVidProps, pFormatList );

		if (eFormat > eMVF_Custom)
			return eFormat;
	}

	// 4K formats
	if( _pVidProps->nWidth == 3840 && ABS( _pVidProps->nHeight ) >= 2160 && ABS( _pVidProps->nHeight ) <= 2160 + VF_VBI_Lines(eMVF_4K_UHD_2398p) ) // 45
	{
		eMVideoFormat pFormatList[] = { eMVF_4K_UHD_2398p, eMVF_4K_UHD_24p, eMVF_4K_UHD_25p, eMVF_4K_UHD_2997p, eMVF_4K_UHD_30p,
			eMVF_4K_UHD_50i, eMVF_4K_UHD_5994i, eMVF_4K_UHD_60i, eMVF_4K_UHD_50p, eMVF_4K_UHD_5994p, eMVF_4K_UHD_60p, (eMVideoFormat)-1 };
		eMVideoFormat eFormat = VF_FindByRate( _pVidProps, pFormatList );

		if (eFormat > eMVF_Custom)
			return eFormat;
	}

	if( _pVidProps->nWidth == 4096 && ABS( _pVidProps->nHeight ) >= 2160 && ABS( _pVidProps->nHeight ) <= 2160 + VF_VBI_Lines(eMVF_4K_DCI_2398p)  ) // 45
	{
		eMVideoFormat pFormatList[] = { eMVF_4K_DCI_2398p, eMVF_4K_DCI_24p, eMVF_4K_DCI_25p, eMVF_4K_DCI_50p, eMVF_4K_DCI_5994p, eMVF_4K_DCI_60p, (eMVideoFormat)-1 };
		eMVideoFormat eFormat = VF_FindByRate( _pVidProps, pFormatList );

		if (eFormat > eMVF_Custom)
			return eFormat;
	}

	// Try full formats
	if( _pVidProps->nHeight >= 1440 && 
		_pVidProps->nWidth <= 2048 )
	{
		// Try full TnB
		M_VID_PROPS vidProps = *_pVidProps;
		vidProps.nHeight /= 2;
		//vidProps.e3DFormat = eM3D_None;
		eMVideoFormat eVF_Base = MVidProps_VF( &vidProps );
		if( eVF_Base != eMVF_Custom && !VF_IS3D( eVF_Base ) )
		{
			return (eMVideoFormat)(eVF_Base|eM3D_FullTopAndBottom);
		}
	}
	else if( _pVidProps->nWidth >= 1280 * 2 && 
		ABS(_pVidProps->nHeight) <= 1080 && 
		_pVidProps->nRowBytes < 2 * (int)Img_RowSizeM( _pVidProps )  ) // The last check is for one field 4K formats
	{
		// Try full SbS
		M_VID_PROPS vidProps = *_pVidProps;
		vidProps.nWidth /= 2;
		//vidProps.e3DFormat = eM3D_None;
		eMVideoFormat eVF_Base = MVidProps_VF( &vidProps );
		if( eVF_Base != eMVF_Custom && !VF_IS3D( eVF_Base ) )
		{
			return (eMVideoFormat)(eVF_Base|eM3D_SideBySide);
		}
	}
		
	return eMVF_Custom;
}

// inline eMVideoFormat MVidProps_VF_Fit( M_VID_PROPS* _pVidProps )
// {
// 	if( !_pVidProps )
// 		return eMVF_Custom;
// 
// 	eMVideoFormat eVF = MVidProps_VF( _pVidProps );
// 	if( eVF != eMVF_Custom ) return eVF;
// 	
// 	// Special fix for 3D 720 TnB 1280x1440 (detected as 2K)
// 	M_VID_PROPS vidProps = *_pVidProps;
// 	if( vidProps.nHeight == 1440 && vidProps.nWidth == 1280 )
// 	{
// 		vidProps.nHeight /= 2;
// 		eMVideoFormat eVF = MVidProps_VF_Fit( &vidProps );
// 		if( eVF != eMVF_Custom )
// 		{
// 			eVF = eMVideoFormat(eVF | eM3D_FullSideBySide);
// 			return eVF;
// 		}
// 	}
// 
// 	for( int _nVF = eMVF_Custom + 1; _nVF < eMVF_Disabled; _nVF++ )
// 	{
// 		if( (!_pVidProps->dblRate || IS_RATE(_pVidProps->dblRate, VF_Rate( (eMVideoFormat)_nVF ) ) ) &&
// 			IS_RES_FIT( _pVidProps, (eMVideoFormat)_nVF ) && 
// 			(IsInterlaced(_pVidProps) != 0) == (VF_Interlace( (eMVideoFormat)_nVF ) != eMI_Progressive) )
// 			return (eMVideoFormat)_nVF;
// 	}
// 
// 	// 3D formats
// 	vidProps = *_pVidProps;
// 	if( vidProps.nWidth > 1280 * 2 )
// 	{
// 		vidProps.nWidth /= 2;
// 		eMVideoFormat eVF = MVidProps_VF_Fit( &vidProps );
// 		if( eVF != eMVF_Custom && !VF_IS3D( eVF ) )
// 			eVF = eMVideoFormat(eVF | eM3D_FullSideBySide);
// 
// 		return eVF;
// 	}
// 
// 	if( vidProps.nHeight > 720 * 2 )
// 	{
// 		vidProps.nHeight /= 2;
// 		eMVideoFormat eVF = MVidProps_VF_Fit( &vidProps );
// 		if( eVF != eMVF_Custom && !VF_IS3D( eVF ))
// 			eVF = eMVideoFormat(eVF | eM3D_FullTopAndBottom);
// 
// 		return eVF;
// 	}
// 
// 	return eMVF_Custom;
// }

inline bool IsBottomTop( const M_VID_PROPS* _pVidProps )
{
	return IsRGB( _pVidProps->fccType ) && _pVidProps->fccType != eMFCC_RGB8 && _pVidProps->nHeight > 0 && _pVidProps->nRowBytes >= 0;
}

// nType = 0 -> DShow format (points to first line, negative height, positive row bytes)
// nType = 1 -> IPP format (points to last line, negative row bytes, height sign ignored (positive) )
inline M_VID_PROPS MVidProps_BottomTop( const M_VID_PROPS* _pVidProps, LPBYTE* _ppbVideo, int _nType )
{
	ATLASSERT( _pVidProps && _ppbVideo );

	M_VID_PROPS vidProps = *_pVidProps;
	if( _nType == 1 && IsBottomTop( _pVidProps ) )
	{
		// Convert to point to bottom line & negative row bytes 
		vidProps.nHeight = ABS(vidProps.nHeight);
		vidProps.nRowBytes = -1 * ABS( vidProps.nRowBytes );
		*_ppbVideo += (_pVidProps->nHeight - 1) * _pVidProps->nRowBytes;
	}
	else if( _nType == 0 && _pVidProps->nRowBytes < 0 && ABS(_pVidProps->nHeight) > 0 )
	{
		ATLASSERT( IsRGB( _pVidProps->fccType) );
		vidProps.nHeight = ABS(vidProps.nHeight);
		vidProps.nRowBytes = -1 * vidProps.nRowBytes ;
		*_ppbVideo -= (ABS(_pVidProps->nHeight) - 1) * vidProps.nRowBytes;
	}

	return vidProps;
}

inline bool IsDisabled( const M_AUD_PROPS* _pAudProps )
{
	return _pAudProps != NULL && _pAudProps->nChannels < 0 ;
}

inline bool IsDisabled( const M_VID_PROPS* _pVidProps )
{
	return _pVidProps != NULL && _pVidProps->eVideoFormat == eMVF_Disabled; 
}

inline bool IsValid( const M_VID_PROPS* _pVidProps )
{
	if( IsDisabled( _pVidProps ) )
		return true;

	if( VF_IsCorrect(_pVidProps->eVideoFormat ) )
		return true;

	if( _pVidProps->nWidth > 0 && _pVidProps->nHeight != 0 && Img_IsSupportM( _pVidProps ) )
		return true;

	return false;
}

inline bool IsValid( const M_AUD_PROPS* _pAudProps )
{
	if( IsDisabled( _pAudProps ) )
		return true;

	return _pAudProps != NULL && 
		_pAudProps->nChannels >= 1 && 
		_pAudProps->nChannels <= MP_MAX_AUDIO_CHANNELS &&
		_pAudProps->nSamplesPerSec > 800 &&
		( _pAudProps->nBitsPerSample == -32 ||
		_pAudProps->nBitsPerSample == 16 ||
		_pAudProps->nBitsPerSample == 20 ||
		_pAudProps->nBitsPerSample == 24 ||
		_pAudProps->nBitsPerSample == 32 );
}

// Note: DISABLED audio is NOT Specified !!! (historically)
inline bool IsSpecified( const M_AUD_PROPS* _pAudProps, bool _bFull = true )
{
	if( _bFull )
		return IsValid( _pAudProps ) && !IsDisabled( _pAudProps );
	
	return _pAudProps != NULL && (_pAudProps->nChannels > 0 || _pAudProps->nSamplesPerSec > 0 || _pAudProps->nBitsPerSample != 0);
}

// Note: DISABLED video is NOT Specified !!! (historically)
inline bool IsSpecified( const M_VID_PROPS* _pVidProps, bool _bFull = true )
{
	if( _bFull )
	{
		return _pVidProps != NULL && _pVidProps->eVideoFormat != eMVF_Disabled &&
			( (_pVidProps->eVideoFormat != eMVF_Custom && (int)_pVidProps->eVideoFormat >= 0 ) || 
			( _pVidProps->nWidth > 0 && _pVidProps->nHeight != 0 /*&& _pVidProps->nRowBytes != 0*/ && _pVidProps->fccType != eMFCC_Default ) ); 
	}

	return _pVidProps != NULL && 
		(_pVidProps->dblRate > 0 || 
		_pVidProps->eVideoFormat != eMVF_Custom ||
		_pVidProps->fccType != eMFCC_Default || 
		(_pVidProps->nAspectX > 0 && _pVidProps->nAspectY > 0) ||
		(_pVidProps->nWidth > 0 && _pVidProps->nHeight != 0) ||
		_pVidProps->eInterlace != eMI_Default);
}

inline bool IsSpecified( const M_AUDIO_LOUDNESS* _pAudLoudProps, bool _bFull = true )
{
	return _pAudLoudProps != NULL &&
		(_pAudLoudProps->lufs.fI != 0.0 ||
		 _pAudLoudProps->lufs.fM != 0.0 ||
		 _pAudLoudProps->lufs.fS != 0.0);
}

inline eMVideoFormat MVidProps_VF_Fit(M_VID_PROPS* _pVidProps, eMVideoFormat* _pVF_List = NULL)
{
	if (!_pVidProps || (!IsSpecified(_pVidProps)))
		return eMVF_Custom;

	eMVideoFormat eVF = MVidProps_VF(_pVidProps);
	if (eVF != eMVF_Custom) return eVF;

	// Special fix for 3D 720 TnB 1280x1440 (detected as 2K)
	M_VID_PROPS vidProps = *_pVidProps;
	if (vidProps.nHeight == 1440 && vidProps.nWidth == 1280)
	{
		vidProps.nHeight /= 2;
		eMVideoFormat eVF = MVidProps_VF_Fit(&vidProps);
		if (eVF != eMVF_Custom)
		{
			eVF = eMVideoFormat(eVF | eM3D_FullSideBySide);
			return eVF;
		}
	}

	eMVideoFormat* pVF_List = _pVF_List;
	if (pVF_List == NULL)
	{
		pVF_List = (eMVideoFormat*)alloca(sizeof(eMVideoFormat) * (int)eMVF_Disabled);
		for (int i = 0; i < (int)eMVF_Disabled; i++)
		{
			// First is eMVF_Custom
			pVF_List[i] = (eMVideoFormat)(i + 1);
		}
	}

	eMVideoFormat eVF_Closest = eMVF_Custom;
	while (*pVF_List != eMVF_Disabled && (int)*pVF_List > 0)
	{
		if (IS_RES_FIT(_pVidProps, *pVF_List) &&
			(IsInterlaced(_pVidProps) != 0) == (VF_Interlace(*pVF_List) != eMI_Progressive))
		{
			if (!_pVidProps->dblRate || IS_RATE(_pVidProps->dblRate, VF_Rate(*pVF_List)))
				return *pVF_List;

			if (eVF_Closest == eMVF_Custom && ABS(_pVidProps->dblRate - VF_Rate(*pVF_List)) < ABS(_pVidProps->dblRate - VF_Rate(eVF_Closest)))
			{
				eVF_Closest = *pVF_List;
			}
		}

		pVF_List++;
	}

	// 3D formats
	vidProps = *_pVidProps;
	if (vidProps.nWidth > 1280 * 2)
	{
		vidProps.nWidth /= 2;
		eMVideoFormat eVF = MVidProps_VF_Fit(&vidProps, _pVF_List);
		if (eVF != eMVF_Custom && !VF_IS3D(eVF))
			eVF = eMVideoFormat(eVF | eM3D_FullSideBySide);

		return eVF;
	}

	if (vidProps.nHeight > 720 * 2)
	{
		vidProps.nHeight /= 2;
		eMVideoFormat eVF = MVidProps_VF_Fit(&vidProps, _pVF_List);
		if (eVF != eMVF_Custom && !VF_IS3D(eVF))
			eVF = eMVideoFormat(eVF | eM3D_FullTopAndBottom);

		return eVF;
	}

	// Need to find the closest format


	return eVF_Closest;
}

inline bool IsPartiallySpecified( const M_AUD_PROPS* _pAudProps )
{
	return IsSpecified( _pAudProps, false );
}

inline bool IsPartiallySpecified( const M_VID_PROPS* _pVidProps )
{
	return IsSpecified( _pVidProps, false );
}

inline bool IsSameFCC( eMFCC _fcc1, eMFCC _fcc2 )
{
	if( _fcc1 == eMFCC_HDYC )
		_fcc1 = eMFCC_UYVY;
	if( _fcc1 == eMFCC_RGB32 )
		_fcc1 = eMFCC_ARGB32;

	if( _fcc2 == eMFCC_HDYC )
		_fcc2 = eMFCC_UYVY;
	if( _fcc2 == eMFCC_RGB32 )
		_fcc2 = eMFCC_ARGB32;

	return _fcc1 == _fcc2;
}

inline int IsSpecified( const M_AV_PROPS* _pAVProps, bool _bFullAudio = true )
{
	if( !_pAVProps )
		return 0;

	bool bVideo = IsSpecified( &_pAVProps->vidProps );
	bool bAudio = IsSpecified( &_pAVProps->audProps, _bFullAudio );

	return (bVideo ? 1 : 0) + (bAudio ? 2 : 0);
}

inline eMVideoFormat UpdateMVidProps( M_VID_PROPS* _pVidProps, bool _bUpdateAllFromVF_or_CorrectCustom = false )
{
	ATLASSERT( _pVidProps );
	if( !_pVidProps ) return eMVF_Custom;

	if( _pVidProps->eVideoFormat == eMVF_Disabled )
		return _pVidProps->eVideoFormat;
	
	if( _pVidProps->nAspectX == 0 || _pVidProps->nAspectY == 0 )
	{
		_pVidProps->nAspectX = 0;
		_pVidProps->nAspectY = 0;
	}
	
	if( _pVidProps->eVideoFormat == eMVF_Custom )
	{
		// Use true for have ability to specify perform or NOT perform VBI line cutoff/add in MConverter::CorrectFormats()
		_pVidProps->eVideoFormat = MVidProps_VF( _pVidProps, _bUpdateAllFromVF_or_CorrectCustom ? false : true );
	}
	else
	{
		M_VID_PROPS vidProps = VF_MVidProps( _pVidProps->eVideoFormat, _pVidProps->fccType );
		if( !vidProps.dblRate ) return eMVF_Custom;

		// For negative rate - used e.g. in DXV FRC converter
		vidProps.dblRate *= SIGN(_pVidProps->dblRate);

		// Check for correct props
		vidProps.eScaleType = _pVidProps->eScaleType;
	
		if( !_bUpdateAllFromVF_or_CorrectCustom )
		{
			if( _pVidProps->dblRate != 0 && _pVidProps->nWidth != 0 && _pVidProps->nHeight != 0 )
				vidProps.eVideoFormat = MVidProps_VF( _pVidProps );

			// Update aspect ratio, row len, 
			if( _pVidProps->nRowBytes ) 
				vidProps.nRowBytes = _pVidProps->nRowBytes;

			if( _pVidProps->nAspectX && _pVidProps->nAspectY )
			{
				vidProps.nAspectX = _pVidProps->nAspectX;
				vidProps.nAspectY = _pVidProps->nAspectY;
			}
			if( ABS(_pVidProps->nHeight) > 1)
			{
				vidProps.nHeight = _pVidProps->nHeight;
			}
			else if( ABS(_pVidProps->nHeight) == 1)
			{
				vidProps.nHeight = ABS(vidProps.nHeight) * SIGN(_pVidProps->nHeight);
			}
			if( _pVidProps->nWidth )
			{
				vidProps.nWidth = _pVidProps->nWidth;
				if( !_pVidProps->nRowBytes )
					vidProps.nRowBytes = Img_RowSizeM(vidProps.fccType, vidProps.nWidth );
			}
			if( _pVidProps->eInterlace != eMI_Default )
				vidProps.eInterlace = _pVidProps->eInterlace;
		}
		else
		{
			// Update some parameters
			if( _pVidProps->nHeight != 0 )
			{
				if( (ABS(vidProps.nHeight) <= ABS(_pVidProps->nHeight) || (VF_IsNTSC(vidProps.eVideoFormat) && ABS(_pVidProps->nHeight) == 480) ) && 
					ABS(_pVidProps->nHeight) <= ABS(vidProps.nHeight) + VF_VBI_Lines(vidProps.eVideoFormat) )
				{
					vidProps.nHeight = _pVidProps->nHeight;
				}
				else
				{
					vidProps.nHeight = SIGN(_pVidProps->nHeight) * ABS(vidProps.nHeight);
				}
			}
		}

		
		*_pVidProps = vidProps;
	}

// 	if( _pVidProps->eVideoFormat & (eM3D_SideBySide|eM3D_TopAndBottom))
// 	{
// 		_pVidProps->e3DFormat = eM3DFormat(_pVidProps->eVideoFormat & (eM3D_FullFlag|eM3D_SideBySide|eM3D_TopAndBottom));
// 	}

	if( _bUpdateAllFromVF_or_CorrectCustom && _pVidProps->nWidth > 0 && _pVidProps->nHeight != 0 )
	{
		if( _pVidProps->eInterlace != eMI_Progressive && _pVidProps->eInterlace != eMI_Field1First && _pVidProps->eInterlace != eMI_Field2First )
			_pVidProps->eInterlace = eMI_Progressive;

		if( _pVidProps->fccType == eMFCC_Default )
			_pVidProps->fccType = ABS(_pVidProps->nHeight) > 625 ? eMFCC_HDYC : eMFCC_UYVY;
	}

	if( !IsRGB(_pVidProps->fccType) )
	{
		_pVidProps->nWidth = ALIGN_DOWN( _pVidProps->nWidth, 2 );
		_pVidProps->nHeight = ABS(_pVidProps->nHeight );
	}
	

	if( IsInterlaced( _pVidProps ) )
		_pVidProps->nHeight = ALIGN_DOWN_SIGN(_pVidProps->nHeight, 2 );

	ATLASSERT( _pVidProps->nRowBytes == 0 || ABS(_pVidProps->nRowBytes) >= (int)Img_RowSizeM( _pVidProps ) );
	_pVidProps->nRowBytes = SIGN(_pVidProps->nRowBytes) * MAX( ABS(_pVidProps->nRowBytes), (int)Img_RowSizeM( _pVidProps ) );

	return _pVidProps->eVideoFormat;
}

inline bool MVidIsValid( const M_VID_PROPS* _pVidProps )
{
	if( !_pVidProps )
		return false;

	M_VID_PROPS vidProps = *_pVidProps;
	UpdateMVidProps( &vidProps );
	return vidProps.nWidth > 0 && vidProps.nHeight != 0 && vidProps.nRowBytes != 0 && vidProps.fccType != eMFCC_Default;
}

inline DWORD Img_SizeM( const M_VID_PROPS* _pPropsIn, bool _bIgnoreRowBytes = false )
{
	if( !_pPropsIn) return 0;

	M_VID_PROPS vidProps = *_pPropsIn;
	if( !vidProps.nRowBytes || !vidProps.nHeight )
		UpdateMVidProps( &vidProps );

	int nRowBytes = vidProps.nRowBytes;
	if( _bIgnoreRowBytes )
		nRowBytes = Img_RowSizeM( &vidProps );

	if( !IsPlanar( vidProps.fccType ) )
		return nRowBytes * ABS(vidProps.nHeight);

	// Now we support only YV12, NV12 images -> size is Y + Y / 4 + Y / 4 = Y + Y / 2
	return nRowBytes * ABS(vidProps.nHeight) * 3 / 2;
}

inline MF_VID_PTR Img_VidPtrM( M_VID_PROPS* _pVidProps, void* _pvVideo, bool _bNegativeStrideForRGB)
{
	MF_VID_PTR vidPtr = {};
	if( _pVidProps )
	{
		vidPtr.szVideoPlanes[0].cx = _pVidProps->nWidth;
		vidPtr.szVideoPlanes[0].cy = ABS(_pVidProps->nHeight);

		vidPtr.lpVideoPlanes[0] = (LONGLONG)_pvVideo;
		vidPtr.cbVideoRowBytes[0] = MAX( _pVidProps->nRowBytes, (int)Img_RowSizeM( _pVidProps ) );

		if( IsPlanar( _pVidProps->fccType ) )
		{
			if(_pvVideo)
				vidPtr.lpVideoPlanes[1] = vidPtr.lpVideoPlanes[0] + vidPtr.cbVideoRowBytes[0] * vidPtr.szVideoPlanes[0].cy;

			if( _pVidProps->fccType == eMFCC_NV12 )
			{
				vidPtr.szVideoPlanes[1].cx = vidPtr.szVideoPlanes[0].cx; // Size for U & V
				vidPtr.szVideoPlanes[1].cy = vidPtr.szVideoPlanes[0].cy / 2;
				vidPtr.cbVideoRowBytes[1] = vidPtr.cbVideoRowBytes[0];
			}
			else
			{
				vidPtr.szVideoPlanes[2].cx = vidPtr.szVideoPlanes[1].cx = vidPtr.szVideoPlanes[0].cx / 2; // Size for U & V
				vidPtr.szVideoPlanes[2].cy = vidPtr.szVideoPlanes[1].cy = vidPtr.szVideoPlanes[0].cy / 2;
				vidPtr.cbVideoRowBytes[2] = vidPtr.cbVideoRowBytes[1] = vidPtr.cbVideoRowBytes[0] / 2;

				if(_pvVideo)
					vidPtr.lpVideoPlanes[2] = vidPtr.lpVideoPlanes[1] + vidPtr.cbVideoRowBytes[1] * vidPtr.szVideoPlanes[1].cy;

				if( _pVidProps->fccType == eMFCC_I420 )
					SwapVal( vidPtr.lpVideoPlanes[1], vidPtr.lpVideoPlanes[2] );
			}
		}
		else if (_bNegativeStrideForRGB && IsBottomTop(_pVidProps))
		{
			vidPtr.lpVideoPlanes[0] += vidPtr.cbVideoRowBytes[0] * (vidPtr.szVideoPlanes[0].cy - 1);
			vidPtr.cbVideoRowBytes[0] *= -1;
		}
	}
	
	return vidPtr;
}

inline DWORD Img_SizeM( const MF_VID_PTR* _pVidPtr, DWORD* _pnPlanesOffset = NULL )
{
	DWORD cbSize = 0;
	ATLASSERT( _pVidPtr );
	if( _pVidPtr )
	{
		for( int i = 0; i < 4; i++ )
		{
			 cbSize += _pVidPtr->szVideoPlanes[i].cy * ABS(_pVidPtr->cbVideoRowBytes[i]);
			 if( _pnPlanesOffset )
				 _pnPlanesOffset[i] = cbSize;
		}
	}

	return cbSize;
}

inline M_VID_PROPS PIC2MVidProps( PIC_PROPS* _pPicProps )
{
	M_VID_PROPS vidProps = {};
	vidProps.fccType = (eMFCC)_pPicProps->fccType;
	vidProps.nWidth = _pPicProps->nWidth;
	vidProps.nHeight = _pPicProps->nHeight;
	vidProps.nRowBytes = Img_RowSizeM( _pPicProps );
	if( !IsRGB( vidProps.fccType ) )
		vidProps.nHeight = ABS(vidProps.nHeight);

	UpdateMVidProps( &vidProps );

	return vidProps;
}



// _nCompareType = 0 -> all
// _nCompareType = 1 -> only size
// _nCompareType = 2 -> only size & fcc
// _nCompareType = 3 -> only size & fcc & row_bytes
// _nSameIfOneNotSpecified = 1 -> if input not specified -> is same
// _nSameIfOneNotSpecified = 2 -> if output not specified -> is same
// _nSameIfOneNotSpecified = 3 -> if input or output not specified -> is same
inline bool IsSameFormat( const M_VID_PROPS* _pVidProps1, const M_VID_PROPS* _pVidProps2, int _nCompareType = 0, int _nSameIfOneNotSpecified = 0 )
{
	// e.g. for conversion check
	if ((_nSameIfOneNotSpecified & 1) && !IsSpecified(_pVidProps1))
		return true;

	if ((_nSameIfOneNotSpecified & 2) && !IsSpecified(_pVidProps2))
		return true;

	if( !_pVidProps1 || !_pVidProps2 )
		return _pVidProps1 == _pVidProps2;

	M_VID_PROPS vidProps1 = *_pVidProps1;
	M_VID_PROPS vidProps2 = *_pVidProps2;

	// The next check is for prevent updating to 3D formats 
	UpdateMVidProps( &vidProps1 );
	UpdateMVidProps( &vidProps2 );

	if( _nCompareType != 0 )
	{
		if( (_nCompareType >= 2 && vidProps1.fccType != vidProps2.fccType) ||
			(_nCompareType >= 3 && vidProps1.nRowBytes != vidProps2.nRowBytes) ||
			vidProps1.nWidth != vidProps2.nWidth ||
			ABS(vidProps1.nHeight) != ABS(vidProps2.nHeight) )
		{
			return false;
		}

		return true;
	}

	bool bChangeAR = ( vidProps1.nAspectX && vidProps1.nAspectY && 
		vidProps2.nAspectX && vidProps2.nAspectY && 
		vidProps1.eScaleType != eMST_IgnoreAR && vidProps2.eScaleType != eMST_IgnoreAR &&
		(vidProps1.nAspectX != vidProps2.nAspectX || vidProps1.nAspectY != vidProps2.nAspectY ) );

	if( vidProps1.eVideoFormat != eMVF_Custom && 
		vidProps1.fccType == vidProps2.fccType &&
		vidProps1.eVideoFormat == vidProps2.eVideoFormat &&
		vidProps1.nWidth == vidProps2.nWidth && 
		vidProps1.nHeight == vidProps2.nHeight &&
		vidProps1.eInterlace == vidProps2.eInterlace &&
		!bChangeAR )
	{
		return true;
	}

	if( !IS_RATE(vidProps1.dblRate, vidProps2.dblRate ) ||
		vidProps1.fccType != vidProps2.fccType ||
		vidProps1.nWidth != vidProps2.nWidth ||
		vidProps1.nHeight != vidProps2.nHeight ||
		vidProps1.fccType != vidProps2.fccType ||
		vidProps1.eInterlace != vidProps2.eInterlace ||
		/*vidProps1.e3DFormat != vidProps2.e3DFormat ||*/
		bChangeAR )
	{
		return false;
	}

	// Do not compare scale-type
// 	if( vidProps1.eScaleType != vidProps2.eScaleType )
// 		return false;

	return true;
}


// _nSameIfOneNotSpecified = 1 -> if input not specified -> is same
// _nSameIfOneNotSpecified = 2 -> if output not specified -> is same
// _nSameIfOneNotSpecified = 3 -> if input or output not specified -> is same
inline bool IsSameFormat( const M_AUD_PROPS* _pAudProps1, const M_AUD_PROPS* _pAudProps2, int _nSameIfOneNotSpecified = 0)
{
	// e.g. for conversion check
	if ((_nSameIfOneNotSpecified & 1) && !IsSpecified(_pAudProps1))
		return true;

	if ((_nSameIfOneNotSpecified & 2) && !IsSpecified(_pAudProps2))
		return true;

	if( !_pAudProps1 || !_pAudProps2 )
		return _pAudProps1 == _pAudProps2;

	if( (_pAudProps1->nChannels || _pAudProps2->nChannels) && _pAudProps1->nChannels != _pAudProps2->nChannels )
		return false;

	if( (_pAudProps1->nBitsPerSample || _pAudProps2->nBitsPerSample) && _pAudProps1->nBitsPerSample != _pAudProps2->nBitsPerSample )
		return false;

	if( (_pAudProps1->nSamplesPerSec || _pAudProps2->nSamplesPerSec) && _pAudProps1->nSamplesPerSec != _pAudProps2->nSamplesPerSec )
		return false;

	return true;
}

// Return 1 -> same format, 
// -1 - different audio
// -2 - different video
// -3 - different both
inline int IsSameFormatAV( const M_AV_PROPS* _pAVProps1, const M_AV_PROPS* _pAVProps2, int _nCompareVideoType = 0 )
{
	bool bSameVideo = IsSameFormat( &_pAVProps1->vidProps, &_pAVProps2->vidProps, _nCompareVideoType );
	bool bSameAudio = IsSameFormat( &_pAVProps1->audProps, &_pAVProps2->audProps );

	if( bSameAudio && bSameVideo )
		return 1;

	return (bSameVideo ? 0 : -1) + (bSameAudio ? 0 : -2);
}

inline M_VID_PROPS MVidPropsFromPic( PIC_PROPS* _pPicProps )
{
	M_VID_PROPS vidRes = {};
	ATLASSERT( _pPicProps );
	if( _pPicProps )
	{
		vidRes.fccType = (eMFCC)_pPicProps->fccType;
		vidRes.nWidth = _pPicProps->nWidth;
		vidRes.nHeight = _pPicProps->nHeight;
		vidRes.nRowBytes = Img_RowSizeM( _pPicProps );

		UpdateMVidProps( &vidRes, false );
	}

	return vidRes;
}

inline PIC_PROPS MVidPropsToPic( M_VID_PROPS* _pVidProps )
{
	PIC_PROPS picRes = { _pVidProps->fccType, _pVidProps->nWidth, _pVidProps->nHeight, 
		(int)Img_RowWidthM( _pVidProps->fccType, _pVidProps->nRowBytes ) };

	return picRes;
}

inline M_VID_PROPS MVidProps_Make( eMFCC _fccFormat, int _nSizeX, int _nSizeY, int _nAspectX = 0 , int _nAspectY = 0, double _dblRate = 0 )
{
	M_VID_PROPS vidRes = {};
	vidRes.fccType = _fccFormat;
	vidRes.nWidth = _nSizeX;
	vidRes.nHeight = _nSizeY;
	vidRes.dblRate = _dblRate;
	if( _nAspectX && _nAspectY )
	{
		vidRes.nAspectX = _nAspectX;
		vidRes.nAspectY = _nAspectY;
	}
	if( vidRes.fccType != eMI_Default )
		vidRes.nRowBytes = Img_RowSizeM( vidRes.fccType, vidRes.nWidth );

	UpdateMVidProps( &vidRes, false );

	return vidRes;
}

inline M_VID_PROPS MVidPropsMerge( const M_VID_PROPS* _pVidProps, const M_VID_PROPS* _pVidPropsDef )
{
	M_VID_PROPS vidRes = {};
	if( !_pVidProps )
		return _pVidPropsDef ? *_pVidPropsDef : vidRes;

	if (!_pVidPropsDef)
		return *_pVidProps;

	
	vidRes = *_pVidProps;

	// Check for disabled formats (in this case do not update props)
	if( vidRes.eVideoFormat == eMVF_Disabled )
		return vidRes;
	
	if( vidRes.fccType == eMFCC_Default )
		vidRes.fccType = _pVidPropsDef->fccType;

	if( vidRes.eScaleType == eMST_Default )
		vidRes.eScaleType = _pVidPropsDef->eScaleType;

	if( vidRes.eInterlace == eMI_Default )
		vidRes.eInterlace = _pVidPropsDef->eInterlace;

	if( vidRes.nWidth == 0 || vidRes.nHeight == 0)
	{
		vidRes.nWidth = _pVidPropsDef->nWidth;
		vidRes.nHeight = _pVidPropsDef->nHeight;
		vidRes.nRowBytes = vidRes.fccType != eMFCC_Default ? Img_RowSizeM( vidRes.fccType, vidRes.nWidth ) : 0;

		// Special fix for specifi only FCC (e.g. RGB w/o height )
		if (IsRGB(_pVidProps->fccType) && !IsRGB(_pVidPropsDef->fccType) && _pVidProps->nHeight <= 0 )
			vidRes.nHeight = ABS(vidRes.nHeight) * -1;
	}
	if( vidRes.nAspectX == 0 || vidRes.nAspectY == 0)
	{
		vidRes.nAspectX = _pVidPropsDef->nAspectX;
		vidRes.nAspectY = _pVidPropsDef->nAspectY;
	}

	if( vidRes.dblRate <= 0 )
		vidRes.dblRate = _pVidPropsDef->dblRate;

	if( vidRes.fccType == eMFCC_Default && vidRes.nWidth > 0 && ABS(vidRes.nHeight) > 0 )
		vidRes.fccType = ABS(vidRes.nHeight) >= 720 ? eMFCC_HDYC : eMFCC_UYVY;

	if( !vidRes.nRowBytes )
		vidRes.nRowBytes = Img_RowSizeM( &vidRes );

	if( !IsRGB(vidRes.fccType))
		vidRes.nWidth = ALIGN_DOWN( vidRes.nWidth, 2 );
	
	return vidRes;
}

inline int MVidPropsOverride( M_VID_PROPS* _pVidProps, const M_VID_PROPS* _pVidPropsOverride )
{
	if( !_pVidProps || !_pVidPropsOverride )
		return _pVidProps ? 0 : -1;

	int nRes = 0;
	if( _pVidPropsOverride->eInterlace == eMI_Inverse )
	{
		_pVidProps->eInterlace = _pVidProps->eInterlace == eMI_Field1First ? eMI_Field2First :
		_pVidProps->eInterlace == eMI_Field2First ? eMI_Field1First : _pVidProps->eInterlace;

		_pVidProps->eVideoFormat = eMVF_Custom;
		nRes = 1;
	}
	else if( _pVidPropsOverride->eInterlace == eMI_OneField ) // Also in this exclusive case we CHANGE OUTPUT FORMAT
	{
		_pVidProps->eInterlace = eMI_Field1First ;
		_pVidProps->eVideoFormat = eMVF_Custom;
		nRes = 1;
	}
	else if( _pVidPropsOverride->eInterlace != eMI_Default )
	{
		if( _pVidPropsOverride->eInterlace != eMI_Progressive && _pVidProps->dblRate < 35.0 )
		{
			_pVidProps->eInterlace = _pVidPropsOverride->eInterlace;
			_pVidProps->eVideoFormat = eMVF_Custom;
			nRes = 1;
		}
	}

	if( _pVidPropsOverride->nAspectX && _pVidPropsOverride->nAspectY )
	{
		_pVidProps->nAspectX = _pVidPropsOverride->nAspectX;
		_pVidProps->nAspectY = _pVidPropsOverride->nAspectY;
		nRes |= 2;
	}

	if( _pVidPropsOverride->eScaleType != eMST_Default )
	{
		_pVidProps->eScaleType = _pVidPropsOverride->eScaleType;
		nRes |= 4;
	}

	if( _pVidPropsOverride->dblRate != 0 )
	{
		_pVidProps->dblRate = _pVidPropsOverride->dblRate;
		_pVidProps->eVideoFormat = eMVF_Custom;
		nRes |= 8;
	}

	if(_pVidPropsOverride->fccType != _pVidProps->fccType &&
		(_pVidPropsOverride->fccType == eMFCC_ARGB32 || _pVidPropsOverride->fccType == eMFCC_RGB32) &&
		(_pVidProps->fccType == eMFCC_ARGB32 || _pVidProps->fccType == eMFCC_RGB32) )
	{
		_pVidProps->fccType = _pVidPropsOverride->fccType;
		nRes |= 16;
	}

	
	if (_pVidPropsOverride->fccType != _pVidProps->fccType && 
		(_pVidPropsOverride->fccType == eMFCC_HDYC || _pVidPropsOverride->fccType == eMFCC_UYVY) &&
		(_pVidProps->fccType == eMFCC_HDYC || _pVidProps->fccType == eMFCC_UYVY))
	{
		_pVidProps->fccType = _pVidPropsOverride->fccType;
		nRes |= 32;
	}

	return nRes;
}

inline LPCWSTR MVidProps2Str( const M_VID_PROPS* _pVidProps, LPWSTR _pszStr, bool _bShort = false )
{
	if( !_pVidProps || !_pszStr )
		return NULL;

	M_VID_PROPS vidProps = *_pVidProps;
	UpdateMVidProps( &vidProps );

	if( IsDisabled(&vidProps) )
	{
		swprintf( _pszStr, _bShort ? L"-" : M_NO_VIDEO );
	}
	else if( !IsSpecified(&vidProps) )
	{
		swprintf( _pszStr, _bShort ? L"-" : M_NOT_SPECIFIED_STR );
	}
	else
	{
		// 640x480 @ 30.00Hz
		// YUY2 640x480 @ 30.00Hz Progressive
		// PAL v210 720x576@50iT 4:3 TopFirst v210
		// HD1080-5994i HDYC 1920x1080 @ 59.94Hz BottomFirst 16:9
		// 3D-Full-TnB HD1080-5994i HDYC 1920x1080 @ 59.94Hz BottomFirst 16:9
		WCHAR szAr[16]={0};
		if( vidProps.nAspectX && vidProps.nAspectY )
		{
			wsprintfW(szAr, L" %d:%d", vidProps.nAspectX, vidProps.nAspectY );
		}
		WCHAR sz3D[16]={0};
// 		if( VF_IS3D(vidProps.e3DFormat) )
// 		{
// 			//wsprintfW(sz3D, L"3D-%s", (vidProps.e3DFormat & eM3D_SideBySide) ? L"SbS" : L"TnB" );
// 			wsprintfW(sz3D, L"%ws ", M3D_Str(vidProps.e3DFormat) );
// 		}

		ATLASSERT( _pVidProps->eInterlace <= eMI_OneField );

		if( _bShort )
		{
			// YUY2 720(1440)x576@29.97p 4:3
			swprintf( _pszStr, L"%ws %d(%d)x%d@%.3f%ws%ws",
				vidProps.fccType != eMFCC_Default ? FCC2NAMEW(vidProps.fccType) : L"DEF", 
				vidProps.nWidth, vidProps.nRowBytes, vidProps.nHeight, vidProps.dblRate,
				ARRAYN_VAL( eM_InterlaceStrS, _pVidProps->eInterlace ), szAr);
		}
		else
		{
			swprintf( _pszStr, L"%ws%ws%ws%ws%ws%dx%d@%.2f%ws%ws",
				sz3D,
				vidProps.eVideoFormat != eMVF_Custom ? VF_Str(vidProps.eVideoFormat) : L"", vidProps.eVideoFormat != eMVF_Custom ? L" " : L"",
				vidProps.fccType != eMFCC_Default ? FCC2NAMEW(vidProps.fccType) : L"", vidProps.fccType != eMFCC_Default ? L" " : L"",
				vidProps.nWidth, vidProps.nHeight, vidProps.dblRate,
				vidProps.eInterlace != eMI_Default ? ARRAYN_VAL( eM_InterlaceStrS, _pVidProps->eInterlace ) : L"",
				szAr);
		}
	
	}

	return _pszStr;
}

inline LPCWSTR MAudProps2Str( const M_AUD_PROPS* _pAudProps, LPWSTR _pszStr )
{
	if( IsDisabled(_pAudProps) )
	{
		swprintf( _pszStr, M_NO_AUDIO );
	}
	else if( !IsSpecified(_pAudProps) )
	{
		swprintf( _pszStr, M_NOT_SPECIFIED_STR );
	}
	else
	{
		if (_pAudProps->nBitsPerSample < 0 )
			wsprintfW( _pszStr, L"%d Hz, %d Ch, %d-bit (float)", 
			_pAudProps->nSamplesPerSec, 
			(int)_pAudProps->nChannels, 
			ABS(_pAudProps->nBitsPerSample) );
		else
			wsprintfW( _pszStr, L"%d Hz, %d Ch, %d-bit", 
			_pAudProps->nSamplesPerSec, 
			(int)_pAudProps->nChannels, 
			_pAudProps->nBitsPerSample );
		// N64STR( DBL2INT((double)_pAudProps->nBitsPerSample * _pAudProps->nBitsPerSample * _pAudProps->nChannels / 1024) ) );
	}

	return _pszStr;
}

inline LPCWSTR MVidProps2StrLog( const M_VID_PROPS* _pVidProps, LPWSTR _pszBuff )
{
	if( IsPartiallySpecified( _pVidProps ) )
	{
		WCHAR szAR[64]={};
		if( _pVidProps->nAspectX != 0 && _pVidProps->nAspectY != 0 )
		{
			swprintf( szAR, L" (%d:%d%ws)", _pVidProps->nAspectX, _pVidProps->nAspectY, ARRAYN_VAL(eM_ScaleTypeStrS, _pVidProps->eScaleType) );
		}

		swprintf( _pszBuff, L"%d(%d)x%d(%s)@%.3f%ws%ws", _pVidProps->nWidth, _pVidProps->nRowBytes, _pVidProps->nHeight, 
 			FCC2NAMEW( _pVidProps->fccType), _pVidProps->dblRate, ARRAYN_VAL(eM_InterlaceStrS, _pVidProps->eInterlace ), szAR );
	}
	else if( IsDisabled( _pVidProps ) )
	{
		swprintf( _pszBuff, L"disabled" );
	}
	else
	{
		swprintf( _pszBuff, L"" );
	}

	return _pszBuff;
}

inline LPCWSTR MAudProps2StrLog( const M_AUD_PROPS* _pAudProps, LPWSTR _pszBuff )
{
	if(IsPartiallySpecified( _pAudProps ) )
	{
		char buffer[33]={};
		_itoa(_pAudProps->nTrackSplitBits,buffer,2);

		swprintf( _pszBuff, L"%d,%d,%d%s:%hs", _pAudProps->nChannels, _pAudProps->nSamplesPerSec, ABS(_pAudProps->nBitsPerSample), 
			_pAudProps->nBitsPerSample > 0 ? L"i" : L"f", buffer );
	}
	else if( IsDisabled( _pAudProps ) )
	{
		swprintf( _pszBuff, L"disabled" );
	}
	else 
	{
		swprintf( _pszBuff, L"" );
	}

	return _pszBuff;
}

inline WAVEFORMATEX MAudProps2WF( const M_AUD_PROPS* _pAudProps )
{
	WAVEFORMATEX wf = {0};
	if( !IsSpecified( _pAudProps ) )
		return wf;

	int nBits = ALIGN_UP( ABS(_pAudProps->nBitsPerSample), 8 );

	wf.wFormatTag = WAVE_FORMAT_PCM;//_pAudProps->nBitsPerSample > 0 ? WAVE_FORMAT_PCM : WAVE_FORMAT_IEEE_FLOAT;
	wf.nChannels = _pAudProps->nChannels;
	wf.nSamplesPerSec = _pAudProps->nSamplesPerSec;
	wf.wBitsPerSample = nBits;
	if( wf.wBitsPerSample == 20 )
		wf.wBitsPerSample = 24;

	wf.nBlockAlign = (wf.nChannels * wf.wBitsPerSample ) >> 3;	 
	wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;

	return wf;
}

inline WAVEFORMATEXTENSIBLE MAudProps2WFX( const M_AUD_PROPS* _pAudProps, int _nChannelMask = -1 )
{
	WAVEFORMATEXTENSIBLE wfx = {0};
	if( !IsSpecified( _pAudProps ) )
		return wfx;

	wfx.Format = MAudProps2WF( _pAudProps );
	wfx.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
	wfx.Format.cbSize = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
	wfx.Samples.wValidBitsPerSample = ABS(_pAudProps->nBitsPerSample);
	if( _nChannelMask == -1 )
		wfx.dwChannelMask = (1 << _pAudProps->nChannels) - 1;
	else
		wfx.dwChannelMask = _nChannelMask;
	wfx.SubFormat = _pAudProps->nBitsPerSample > 0 ? KSDATAFORMAT_SUBTYPE_PCM : KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;

	return wfx;
}

inline M_AUD_PROPS MAudPropsFromWF( const WAVEFORMATEX* _pWaveFormat )
{
	M_AUD_PROPS audProps = { _pWaveFormat->nChannels, (int)_pWaveFormat->nSamplesPerSec, _pWaveFormat->wBitsPerSample, 0 };
	if( _pWaveFormat->cbSize >= sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX) )
	{
		// Check for 32-bit float and integer
		WAVEFORMATEXTENSIBLE* pWFX = (WAVEFORMATEXTENSIBLE*)_pWaveFormat;
		if( pWFX->SubFormat == KSDATAFORMAT_SUBTYPE_IEEE_FLOAT )
			audProps.nBitsPerSample = -1 * ABS(audProps.nBitsPerSample);
	}
	else if( _pWaveFormat->wFormatTag == WAVE_FORMAT_IEEE_FLOAT )
	{
		audProps.nBitsPerSample = -1 * ABS(audProps.nBitsPerSample);
	}

	return audProps;
}

inline M_AUD_PROPS MAudPropsMerge( M_AUD_PROPS* _pAudProps, M_AUD_PROPS* _pAudPropsDef )
{
	M_AUD_PROPS audRes = {};
	if (_pAudPropsDef)
		audRes = *_pAudPropsDef;

	if( !_pAudProps )
		return audRes;

	if( !IsSpecified(_pAudPropsDef) || IsDisabled(_pAudProps) ) // Disabled
		return _pAudProps ? *_pAudProps : audRes;
	
	 audRes = *_pAudProps;
	 if (audRes.nChannels == 0)
		 audRes.nChannels = _pAudPropsDef->nChannels;

	 if (audRes.nChannels < 0)
	 {
		 audRes.nSamplesPerSec = 0;
		 audRes.nBitsPerSample = 0;
		 audRes.nTrackSplitBits = 0;
	 }
	 else
	 {
		 if (audRes.nSamplesPerSec <= 0)
			 audRes.nSamplesPerSec = _pAudPropsDef->nSamplesPerSec;
		 if (audRes.nBitsPerSample == 0)
			 audRes.nBitsPerSample = _pAudPropsDef->nBitsPerSample;

		 if (audRes.nChannels == _pAudPropsDef->nChannels && audRes.nTrackSplitBits == 0)
			 audRes.nTrackSplitBits = _pAudPropsDef->nTrackSplitBits;
	 }

	 return audRes;
}

inline int MAudBits2Bytes( int _nBits, bool _bChannelBuffer = false )
{
	int nBits = ALIGN_UP( ABS(_nBits), 8 );
	int nBytes = nBits / 8;
	if( _bChannelBuffer && nBytes == 3 )
		nBytes = 4;

	return nBytes;
}

// Note: Allow negative numbers for convert offsets
inline LONGLONG MAudBytes2Samples( M_AUD_PROPS* _pAudProps, LONGLONG _cbAudio )
{
	ATLASSERT( _pAudProps );
	if( !IsSpecified( _pAudProps ) )
		return 0;

	int nBlock = MAudBits2Bytes( _pAudProps->nBitsPerSample, false ) * _pAudProps->nChannels;
	ATLASSERT( nBlock > 0 && _cbAudio % nBlock == 0 );
	if( nBlock <= 0 )
		return 0;

	return _cbAudio / nBlock;
}

inline LONGLONG MAudTime2Samples( M_AUD_PROPS* _pAudProps, REFERENCE_TIME _rtAudio )
{
	ATLASSERT( _pAudProps );
	if( !IsSpecified( _pAudProps ) )
		return 0;

	return DBL2INT64( (double)_pAudProps->nSamplesPerSec * _rtAudio / DS_ONESEC );
}

inline LONGLONG MAudTimeSec2Samples( M_AUD_PROPS* _pAudProps, double _dblAudioSec )
{
	ATLASSERT( _pAudProps );
	if( !IsSpecified( _pAudProps ) )
		return 0;

	return DBL2INT64( (double)_pAudProps->nSamplesPerSec * _dblAudioSec );
}

inline LONGLONG MAudTime2Bytes( M_AUD_PROPS* _pAudProps, REFERENCE_TIME _rtAudio )
{
	ATLASSERT( _pAudProps );
	if( !IsSpecified( _pAudProps ) )
		return 0;

	int nBlock = MAudBits2Bytes( _pAudProps->nBitsPerSample, false ) * _pAudProps->nChannels;

	return MAudTime2Samples( _pAudProps, _rtAudio ) * nBlock;
}

// Note: Allow negative numbers for convert offsets
inline LONGLONG MAudSamples2Bytes( M_AUD_PROPS* _pAudProps, LONGLONG _lSamples )
{
	ATLASSERT( _pAudProps );
	if( !IsSpecified( _pAudProps ) )
		return 0;

	int nBlock = MAudBits2Bytes( _pAudProps->nBitsPerSample, false ) * _pAudProps->nChannels;
	return _lSamples * nBlock;
}

inline REFERENCE_TIME MAudSamples2Time( M_AUD_PROPS* _pAudProps, LONGLONG _llSamples )
{
	ATLASSERT( _pAudProps );
	if( !IsSpecified( _pAudProps ) )
		return 0;

	return DBL2INT64( (double)_llSamples * DS_ONESEC / (double)_pAudProps->nSamplesPerSec  );
}

// Return in 100 nsec units (like REFERENCE_TIME)
inline double MAudSamples2TimeDbl( M_AUD_PROPS* _pAudProps, LONGLONG _llSamples )
{
	ATLASSERT( _pAudProps );
	if( !IsSpecified( _pAudProps ) )
		return 0;

	return (double)_llSamples * DS_ONESEC / (double)_pAudProps->nSamplesPerSec;
}

inline double MAudSamples2TimeSec( M_AUD_PROPS* _pAudProps, LONGLONG _llSamples )
{
	ATLASSERT( _pAudProps );
	if( !IsSpecified( _pAudProps ) )
		return 0;

	ATLASSERT( _pAudProps->nSamplesPerSec > 0 );
	if( _pAudProps->nSamplesPerSec <= 0 )
		return 0;

	return (double)_llSamples / (double)_pAudProps->nSamplesPerSec;
}

inline REFERENCE_TIME MAudBytes2Time( M_AUD_PROPS* _pAudProps, LONGLONG _llBytes )
{
	ATLASSERT( _pAudProps );
	if( !IsSpecified( _pAudProps ) )
		return 0;

	int nBlock = MAudBits2Bytes( _pAudProps->nBitsPerSample, false ) * _pAudProps->nChannels;
	ATLASSERT( nBlock > 0 && _llBytes % nBlock == 0 );
	if( nBlock <= 0 ) return 0;

	return DBL2INT64( (double)_llBytes * DS_ONESEC / (double)(_pAudProps->nSamplesPerSec * nBlock) );
}

inline double MAudBytes2TimeSec( M_AUD_PROPS* _pAudProps, LONGLONG _llBytes )
{
	ATLASSERT( _pAudProps );
	if( !IsSpecified( _pAudProps ) )
		return 0;

	int nBlock = MAudBits2Bytes( _pAudProps->nBitsPerSample, false ) * _pAudProps->nChannels;
	ATLASSERT( nBlock > 0 && _llBytes % nBlock == 0 );
	if( nBlock <= 0 ) return 0;

	return (double)_llBytes / (double)(_pAudProps->nSamplesPerSec * nBlock);
}

#define WF2MAudProps MAudPropsFromWF 
// inline M_AUD_PROPS WF2MAudProps( const WAVEFORMATEX* _pWF )
// {
// 	M_AUD_PROPS audProps = {0};
// 	audProps.nChannels = _pWF->nChannels;
// 	audProps.nBitsPerSample = _pWF->wBitsPerSample;
// 	audProps.nSamplesPerSec = _pWF->nSamplesPerSec;
// 
// 	return audProps;
// }

inline HRESULT FillMAudioProps( const AM_MEDIA_TYPE* _pMT, 
	M_AUD_PROPS* _pAudProps )
{
	if( !_pMT || !_pAudProps ) return E_INVALIDARG;

	if( _pMT->formattype != FORMAT_WaveFormatEx || !_pMT->pbFormat )
		return E_INVALIDARG;

	ATLASSERT( _pMT->cbFormat >= sizeof(WAVEFORMATEX) );

	*_pAudProps = MAudPropsFromWF( (WAVEFORMATEX*)_pMT->pbFormat );
	
	return S_OK;
}

inline HRESULT FillMVideoProps( const AM_MEDIA_TYPE* _pMT, 
	M_VID_PROPS* _pVidProps )
{
	if( !_pMT || !_pVidProps ) return E_INVALIDARG;

	PIC_PROPS picProps = {0};
	int nFormat = CPictureFormats::TestMT( _pMT, &picProps );
	if( nFormat == -1 )
	{
		CMTHelp::MTParseVideo( _pMT, &picProps );
	}

	_pVidProps->fccType = (eMFCC)picProps.fccType;
	_pVidProps->nWidth = picProps.nWidth;
	_pVidProps->nHeight = picProps.nHeight;
	_pVidProps->nRowBytes = Img_RowSizeM( &picProps );
	if( _pVidProps->nRowBytes == 0 )
		_pVidProps->nRowBytes = _pVidProps->nWidth * 4;

	REFERENCE_TIME rtTime = MTAverageTime( _pMT );
	double dblCorrect = CorrectAverageTimeM( rtTime );
	if( dblCorrect > 0 ) 
		_pVidProps->dblRate = (double)DS_ONESEC / dblCorrect;
	else 
		_pVidProps->dblRate = 0;

	bool bDefinedAR = false;
	if( _pMT->formattype == FORMAT_VideoInfo2 )
	{
		VIDEOINFOHEADER2* pVH2 = (VIDEOINFOHEADER2*)_pMT->pbFormat;
		ATLASSERT( _pMT->cbFormat >= sizeof(VIDEOINFOHEADER2) );
		if( _pMT->cbFormat < sizeof(VIDEOINFOHEADER2) ) return E_UNEXPECTED;

		if( pVH2->dwInterlaceFlags & AMINTERLACE_IsInterlaced )
		{
			_pVidProps->eInterlace = (pVH2->dwInterlaceFlags & AMINTERLACE_Field1First) ? eMI_Field1First : eMI_Field2First;
		}
		else
		{
			_pVidProps->eInterlace = eMI_Progressive;
		}
		int nDiv = (int)MLGCD( pVH2->dwPictAspectRatioX, pVH2->dwPictAspectRatioY );
		_pVidProps->nAspectX = INT2SHORT(pVH2->dwPictAspectRatioX / nDiv);
		_pVidProps->nAspectY = INT2SHORT(pVH2->dwPictAspectRatioY / nDiv);

		// Special fix for H.264 Decoder (set 4:3 by default)
		if( ((_pVidProps->nWidth > 720 || _pVidProps->nHeight > 576) && _pVidProps->nAspectX == 4 && _pVidProps->nAspectY == 3) ||
			_pVidProps->nAspectX == _pVidProps->nWidth && _pVidProps->nAspectY == ABS(_pVidProps->nHeight) )
		{
			int nDiv = (int)MLGCD( picProps.nWidth, ABS( picProps.nHeight ) );
			_pVidProps->nAspectX = short(picProps.nWidth / nDiv);
			_pVidProps->nAspectY = short(ABS( picProps.nHeight ) / nDiv);
		}
		else
		{
			bDefinedAR = true;
		}
	}
	else
	{
		_pVidProps->eInterlace = eMI_Progressive;
		int nDiv = (int)MLGCD( picProps.nWidth, ABS( picProps.nHeight ) );
		_pVidProps->nAspectX = short(picProps.nWidth / nDiv);
		_pVidProps->nAspectY = short(ABS( picProps.nHeight ) / nDiv);

		// Special fix for 720x576 / 720x480 / 720x486 AVI Files
		if( IsSD( _pVidProps) )
		{
			//if( _pbPropsChanged ) *_pbPropsChanged = true;

			// For PAL/NTSC set interlacing flags
			if( rtTime > 333000 && rtTime < 334000 )
			{
				_pVidProps->eInterlace = eMI_Field2First;	
			}
			else if( rtTime == 400000 ) 
			{
				_pVidProps->eInterlace = eMI_Field1First;	
			}

			_pVidProps->nAspectX = 4;
			_pVidProps->nAspectY = 3;

			bDefinedAR = true;
		}
	}

	// Check after chnage 
	if( ( _pVidProps->nWidth == 960 && ABS(_pVidProps->nHeight) == 720 ) ||
		( _pVidProps->nWidth == 1440 && ABS(_pVidProps->nHeight) == 1080 ) )
	{
		if( _pVidProps->nAspectX != 16 && _pVidProps->nAspectY != 9 )
		{
		//	if( _pbPropsChanged ) *_pbPropsChanged = true;
		}
		_pVidProps->nAspectX = 16;
		_pVidProps->nAspectY = 9;
	}

	UpdateMVidProps( _pVidProps );

	return S_OK;
}

// _nVI2 > 0 -> Video Info Header 2 (with AR & interlace)
// _nVI2 < 0 -> Video Info Header (no AR)
// _nVI2 == 0 -> Auto (VI1/VI2)
inline HRESULT MTCreateFromMProps( M_VID_PROPS* _pVidProps, CMType& _mtRes, M_VID_PROPS* _pVidPropsDef = NULL, int _nVI2 = 0, int _nRowAligment = 0 )
{
	_mtRes.Reset();

	if( !_pVidProps ) 
		return E_POINTER;

	M_VID_PROPS mgVidProps = *_pVidProps;
	UpdateMVidProps( &mgVidProps );

	if( !mgVidProps.fccType ) mgVidProps.fccType = _pVidPropsDef ? _pVidPropsDef->fccType : eMFCC(FOURCC_UYVY);
	if( !mgVidProps.nWidth ) mgVidProps.nWidth = _pVidPropsDef ? _pVidPropsDef->nWidth : 720;
	if( !mgVidProps.nHeight ) mgVidProps.nHeight = _pVidPropsDef ? _pVidPropsDef->nHeight : 576;
	if( !mgVidProps.dblRate ) mgVidProps.dblRate = _pVidPropsDef ? _pVidPropsDef->dblRate : 25.0;
	if( !mgVidProps.nAspectX || !mgVidProps.nAspectY )
	{
		mgVidProps.nAspectX = _pVidPropsDef ? _pVidPropsDef->nAspectX : 0;
		mgVidProps.nAspectY = _pVidPropsDef ? _pVidPropsDef->nAspectX : 0;
	}
	if( !mgVidProps.eInterlace )
	{
		mgVidProps.eInterlace = _pVidPropsDef ? _pVidPropsDef->eInterlace : eMI_Default;
	}

	bool bVI2 = _nVI2 > 0 ? true : 
		_nVI2 < 0 ? false : 
		(mgVidProps.eInterlace != eMI_Default && mgVidProps.eInterlace != eMI_Progressive) || (mgVidProps.nAspectX > 0 && mgVidProps.nAspectY > 0);

	PIC_PROPS picProps = {mgVidProps.fccType, mgVidProps.nWidth, mgVidProps.nHeight, mgVidProps.nWidth };

	HRESULT hr = CMTHelp::MTCreateVideo( _mtRes, mgVidProps.fccType, bVI2 ? FORMAT_VideoInfo2 : FORMAT_VideoInfo, 
		mgVidProps.nWidth, mgVidProps.nHeight, _nRowAligment <= 0 ? Img_RowWidthM( &picProps, mgVidProps.nRowBytes ) : ALIGN_UP(mgVidProps.nWidth, _nRowAligment ), TRUE );

	REFERENCE_TIME rtAvgTime = mgVidProps.dblRate > 0 ? DBL2INT64( DS_ONESEC / mgVidProps.dblRate ) : 0;
	MTSetAverageTime( &_mtRes, rtAvgTime );
	VIDEOINFOHEADER2* pVH2 = _mtRes;
	if( pVH2 )
	{
		if( mgVidProps.eInterlace == eMI_Field1First )
		{
			pVH2->dwInterlaceFlags = AMINTERLACE_IsInterlaced | AMINTERLACE_Field1First | AMINTERLACE_FieldPatBothRegular | AMINTERLACE_DisplayModeBobOrWeave;
		}
		else if( mgVidProps.eInterlace == eMI_Field2First )
		{
			pVH2->dwInterlaceFlags = AMINTERLACE_IsInterlaced | AMINTERLACE_FieldPatBothRegular | AMINTERLACE_DisplayModeBobOrWeave;
		}
		if( mgVidProps.nAspectX && mgVidProps.nAspectY )
		{
			pVH2->dwPictAspectRatioX = mgVidProps.nAspectX;
			pVH2->dwPictAspectRatioY = mgVidProps.nAspectY;
		}
		else
		{
			int  nDiv = (int)MLGCD( mgVidProps.nWidth, ABS(mgVidProps.nHeight) );
			ATLASSERT( nDiv );
			if( nDiv )
			{
				pVH2->dwPictAspectRatioX = mgVidProps.nWidth / nDiv;
				pVH2->dwPictAspectRatioY = ABS(mgVidProps.nHeight) / nDiv;
			}
		}
		pVH2->dwBitRate = Img_SizeM( &mgVidProps ) * DBL2INT( mgVidProps.dblRate ) * 8;
	}

	VIDEOINFOHEADER* pVH1 = _mtRes;
	if( pVH1 )
		pVH1->dwBitRate = Img_SizeM( &mgVidProps ) * DBL2INT( mgVidProps.dblRate ) * 8;

	return S_OK;
}

inline HRESULT MTCreateFromMProps( M_AUD_PROPS* _pAudProps, CMType& _mtRes, M_AUD_PROPS* _pAudPropsDef = NULL ) 
{
	_mtRes.Reset();

	if( !_pAudProps ) 
		return E_POINTER;

	return CMTHelp::MTCreatePCM( _mtRes, _pAudProps->nSamplesPerSec ? _pAudProps->nSamplesPerSec : _pAudPropsDef ? _pAudPropsDef->nSamplesPerSec : 48000,
		_pAudProps->nChannels ? _pAudProps->nChannels : _pAudPropsDef ? _pAudPropsDef->nChannels : 2,
		_pAudProps->nBitsPerSample ? _pAudProps->nBitsPerSample : _pAudPropsDef ? _pAudPropsDef->nBitsPerSample : 16);
}

inline SYSTEMTIME MTime2SysTime( M_DATETIME* _pTime )
{
	SYSTEMTIME sysTime = {};
	sysTime.wYear = _pTime->nYear;
	sysTime.wDay = _pTime->nDay;
	sysTime.wMonth = _pTime->nMonth;
	sysTime.wHour = _pTime->nHour;
	sysTime.wMinute = _pTime->nMinute;
	sysTime.wSecond = _pTime->nSecond;
	sysTime.wMilliseconds = _pTime->nMilliseconds;

	return sysTime;
}

inline M_DATETIME SysTime2MTime( SYSTEMTIME* _pTime )
{
	M_DATETIME mTime = {};
	mTime.nYear = _pTime->wYear;
	mTime.nDay = _pTime->wDay;
	mTime.nMonth = _pTime->wMonth;
	mTime.nHour = _pTime->wHour;
	mTime.nMinute = _pTime->wMinute;
	mTime.nSecond = _pTime->wSecond;
	mTime.nMilliseconds = _pTime->wMilliseconds;

	return mTime;
}

// inline ULONGLONG MTime2UTC64( M_DATETIME* _pTime )
// {
// 	SYSTEMTIME sysTime = MTime2SysTime( _pTime );
// 	ULONGLONG llTime = 0;
// 	SystemTimeToFileTime( &sysTime, (FILETIME*)&llTime );
// 
// 	return llTime;
// }

inline REFERENCE_TIME FrameDuration( M_TIME* _pTime, REFERENCE_TIME _rtDefTime = 333667 )
{
	if( _pTime && _pTime->rtEndTime > _pTime->rtStartTime + 10 )
		return _pTime->rtEndTime - _pTime->rtStartTime;

	return _rtDefTime;
}


inline M_TIME MTimeFromNum( M_VID_PROPS* _pVidProps, LONGLONG _llFrameNum )
{
	double dblTimeFrame = MVid2AvgTimeDbl(_pVidProps, false, (double)DS_ONESEC / 25.0 );
	M_TIME timeRes = {};
	SET_BREAK( &timeRes, _llFrameNum == 0 );
	timeRes.rtStartTime = DBL2INT64( dblTimeFrame * _llFrameNum );
	timeRes.rtEndTime = DBL2INT64( dblTimeFrame * (_llFrameNum + 1) );

	return timeRes;
}

inline M_TIME MTimeField(const M_TIME& _rTime, int _nField )
{
	M_TIME mTime = _rTime;
	if (_nField == 0)
		mTime.rtEndTime = (mTime.rtEndTime + mTime.rtStartTime) / 2;
	else if (_nField == 1)
		mTime.rtStartTime = (mTime.rtEndTime + mTime.rtStartTime) / 2;

	return mTime;
}

inline LPCWSTR MDateTime2Str( M_DATETIME* _pTime, LPWSTR _pszBuffer )
{
	if( _pTime->nDay > 0 )
	{
		if( _pTime->nMilliseconds > 0 )
			wsprintfW( _pszBuffer, L"%04d/%02d/%0d %02d:%02d:%02d.%03d", _pTime->nYear, _pTime->nMonth, _pTime->nDay,
			_pTime->nHour, _pTime->nMinute, _pTime->nSecond, _pTime->nMilliseconds );
		else
			wsprintfW( _pszBuffer, L"%04d/%02d/%0d %02d:%02d:%02d", _pTime->nYear, _pTime->nMonth, _pTime->nDay,
			_pTime->nHour, _pTime->nMinute, _pTime->nSecond );
	}
	else
	{
		if( _pTime->nMilliseconds > 0 )
			wsprintfW( _pszBuffer, L"%02d:%02d:%02d.%03d", _pTime->nHour, _pTime->nMinute, _pTime->nSecond, _pTime->nMilliseconds );
		else
			wsprintfW( _pszBuffer, L"%02d:%02d:%02d", _pTime->nHour, _pTime->nMinute, _pTime->nSecond );
	}

	return _pszBuffer;
}

static LPCWSTR TC2Str( const M_TIMECODE& _rTimecode, LPWSTR _szBuffer, int _nPrintField = -1 )
{
	// VVB: TODO: Make normal fields flag

	LPCWSTR pszDelimer = (_rTimecode.eTCFlags & eMTCF_DropFrame) ? L";" : L":";
	if(_nPrintField == 1 || ((_rTimecode.eTCFlags & 0xF00) && _nPrintField == -1) )
	{
		int nField = ((_rTimecode.eTCFlags & 0xE00) >> 8);
		nField = MAX(0, nField - 1);
		// 50p, 59.94p, 60p HD video
		wsprintf( _szBuffer, _T("%02d:%02d:%02d%ws%02d.%d"), _rTimecode.nHours, _rTimecode.nMinutes, _rTimecode.nSeconds, pszDelimer, _rTimecode.nFrames, 
			nField );
	}
	else
	{
		wsprintf( _szBuffer, _T("%02d:%02d:%02d%ws%02d"), _rTimecode.nHours, _rTimecode.nMinutes, _rTimecode.nSeconds, pszDelimer, _rTimecode.nFrames );
	}

	return _szBuffer;
}

static LPCWSTR MTime2Str( const M_TIME& _rTime, LPWSTR _szBuffer, bool _bPrintField = false )
{
	wsprintfW( _szBuffer, L"%s..%s(%ws)F%05X", RT2STR( _rTime.rtStartTime ), RT2STR( _rTime.rtEndTime ), MTC2STR( _rTime.tcFrame ), _rTime.eFFlags );
	return _szBuffer;
}

inline M_DATETIME Str2MTime( LPCWSTR _pszTime )
{
	M_DATETIME mTime = {};
	if( !_pszTime )
		return mTime;
	
	LPCWSTR pTime = _pszTime;
	if( StrChrW( _pszTime, L'/' ) && (pTime = StrChrW( _pszTime, L' ' )) )
	{
		pTime++;
		// parse date
		swscanf( _pszTime, L"%d/%d/%d", &mTime.nYear, &mTime.nMonth, &mTime.nDay );
	}
	else if( StrChrW( _pszTime, L'-' ) && (pTime = StrChrW( _pszTime, L' ' )) )
	{
		pTime++;
		// parse date
		swscanf( _pszTime, L"%d-%d-%d", &mTime.nYear, &mTime.nMonth, &mTime.nDay );
	}

	swscanf( pTime, L"%d:%d:%d", &mTime.nHour, &mTime.nMinute, &mTime.nSecond );

	LPCWSTR pMsec = StrChrW( pTime, L'.' );
	if( pMsec )
	{
		pMsec++;
		mTime.nMilliseconds = _wtol( pMsec );
	}
	
	return mTime;
}


inline M_DATETIME UTC64MTime( LONGLONG _llTime )
{
// 	M_DATETIME mTime  = {};
// 	LONGLONG nTime = _llTime / DS_MSEC;
// 	mTime.nMilliseconds = nTime % 1000;
// 	nTime /= 1000;
// 	mTime.nSecond = nTime % 60;
// 	nTime /= 60;
// 	mTime.nMinute = nTime % 60;
// 	nTime /= 60;
// 	mTime.nHour = nTime;

	SYSTEMTIME sysTime = {};
	FileTimeToSystemTime( (FILETIME*)&_llTime, &sysTime );
	if( sysTime.wYear < 1800 )
	{
		sysTime.wDay = 0;
		sysTime.wMonth = 0;
		sysTime.wYear = 0;
	}
	return SysTime2MTime( &sysTime );
}



inline M_DATETIME CorrectTime( M_DATETIME* _pTime, LONGLONG* _pllTime = NULL, bool _bCorrectForward = true )
{
	SYSTEMTIME sysTime = {};
	::GetLocalTime( &sysTime );

	if( !_pTime )
	{
		LONGLONG llTime = GetLocalTime();
		if( _pllTime )
			*_pllTime = llTime;

		return UTC64MTime( llTime );
	}

	M_DATETIME dtRes = *_pTime;

	// Check time
	int nNextDay = 0;
	if( !dtRes.nDay && !dtRes.nMonth && !dtRes.nYear )
	{
		LONGLONG llSystem = (sysTime.wHour * 3600 + sysTime.wMinute * 60 + sysTime.wSecond) * 1000 + sysTime.wMilliseconds;
		LONGLONG llCorrect = (dtRes.nHour * 3600 + dtRes.nMinute * 60 + dtRes.nSecond) * 1000 + dtRes.nMilliseconds;

		if( _bCorrectForward && llSystem > llCorrect ) // e.g. for scheduling
			nNextDay = 1; // Next date
		else if( !_bCorrectForward && llSystem < llCorrect ) // e.g. for time-buffer
			nNextDay = -1; // Prev date
	}
		

	if( !dtRes.nDay )
		dtRes.nDay = sysTime.wDay;

	if( !dtRes.nMonth )
		dtRes.nMonth = sysTime.wMonth;

	if( !dtRes.nYear )
		dtRes.nYear = sysTime.wYear;

	if( nNextDay )
	{
		LONGLONG llTime = 0;
		SYSTEMTIME sysTime = MTime2SysTime( &dtRes );
		SystemTimeToFileTime( &sysTime, (FILETIME*)&llTime );

		llTime += nNextDay * 24 * 3600 * DS_ONESEC;
		dtRes = UTC64MTime( llTime );
	}

	if( _pllTime )
	{
		SYSTEMTIME sysTime = MTime2SysTime( &dtRes );
		SystemTimeToFileTime( &sysTime, (FILETIME*)_pllTime );
	}
	

	return dtRes;
}

inline LONGLONG MTime2UTC64( M_DATETIME* _pTime )
{
	LONGLONG llTime = 0;
	if( _pTime->nYear || _pTime->nMonth || _pTime->nDay )
	{
		CorrectTime( _pTime, &llTime );
	}
	else
	{
		llTime = (_pTime->nHour * 3600 + _pTime->nMinute * 60 + _pTime->nSecond) *  DS_ONESEC + _pTime->nMilliseconds * DS_MSEC;
	}

	return llTime;
}




inline LPCWSTR GenerateUniqueID( CComBSTR& _cbsUniqueID, int _nUseIndex, HANDLE* _pnHandle, LPCWSTR _pszDefPrefix = NULL )
{
	if( !_pszDefPrefix )
		_pszDefPrefix = UNIQUE_NAME_DEF_PREFIX;

	LPCWSTR pszPrefix = _cbsUniqueID.Length() ? _cbsUniqueID.m_str : _pszDefPrefix;
	LPWSTR pszName = (LPWSTR)alloca( (lstrlenW(pszPrefix) + lstrlenW( UNIQUE_NAME_FORMAT )) * sizeof(WCHAR) + 16 );
	int nCount = 0;
	while( true )
	{
		if( nCount == 0 && _nUseIndex <= 0 )
			wsprintfW( pszName, L"%ws", pszPrefix );
		else
			wsprintfW( pszName, UNIQUE_NAME_FORMAT, pszPrefix, nCount );

		CComBSTR cbsHandleName = pszName;
		::CharLowerW( cbsHandleName.m_str );

		StrReplaceW( cbsHandleName, L"\\", L"|" );
		StrReplaceW( cbsHandleName, L"/", L"|" );

		// The name can't have backslash
		HANDLE hEvent = ::CreateEventW( NULL, TRUE, TRUE, cbsHandleName );
		ATLASSERT( hEvent );
		if( !hEvent ) return NULL;

		if( GetLastError() == ERROR_ALREADY_EXISTS )
		{
			::CloseHandle( hEvent );
			nCount++;
		}
		else
		{
			*_pnHandle = hEvent;
			_cbsUniqueID = pszName;

			return _cbsUniqueID.m_str;
		}

		if( _nUseIndex < 0 )
		{
			_cbsUniqueID.Empty();
			return NULL;
		}
	}

	return NULL;
}

class CUniqueName
{
	CStdHandle	m_shNameHandle;
	CComBSTR	m_cbsName;

public:

	CUniqueName( LPCWSTR _pszName = NULL )
	{
		if( _pszName )
			GenerateName( _pszName );
	}

	operator LPCWSTR() { return m_cbsName.m_str; };

	LPCWSTR GenerateName( LPCWSTR _pszBaseName, int _nUseIndex = 0 )
	{
		Reset();

		return GenerateUniqueID( m_cbsName, _nUseIndex, &m_shNameHandle.m_hHandle, _pszBaseName );
	}

	void Reset()
	{
		m_cbsName.Empty();
		m_shNameHandle.Close();
	}

	bool IsValid()
	{
		return !STR_IS_EMPTY( m_cbsName.m_str ) && m_shNameHandle;
	}

	const CComBSTR& Name() const { return m_cbsName; }
};

#ifdef NEW_XML_PARSE // VVB: Fix for CG (Grr... :) )

static bool MVidProps_CalcARRects(M_VID_PROPS* pVidPropsIn, 
	M_VID_PROPS* pVidPropsOut, 
	IN OUT RECT* pRectIn, 
	IN OUT RECT* pRectOut, 
	LPCWSTR pszCrop = NULL, 
	LPCWSTR pszBorders = NULL)
{
	if (!pVidPropsIn || !pVidPropsOut || !pRectIn || !pRectOut)
		return false;

	// Forced input scale-type
	eMScaleType eScaleType = pVidPropsIn->eScaleType != eMST_Default ? pVidPropsIn->eScaleType :
		pVidPropsOut->eScaleType;

	double dblPixelAR_In = MVidProps_PixelAR(pVidPropsIn);
	double dblPixelAR_Out = MVidProps_PixelAR(pVidPropsOut);

	int nFieldScale = 1;
	if (eScaleType == eMST_Default)
	{
		double dblAR_In = MVidProps_AR(pVidPropsIn);
		double dblAR_Out = MVidProps_AR(pVidPropsOut);

		
		// Fix for HD/3K/4K movies with similar size
		if (pVidPropsIn->nWidth >= 1100 &&
			pVidPropsOut->nWidth >= 1280 &&
			ABS((double)(pVidPropsIn->nWidth - pVidPropsOut->nWidth) / pVidPropsOut->nWidth) <= 0.10 &&
			ABS(dblPixelAR_In - dblPixelAR_Out) < 0.01)
		{
			if (ABS(pVidPropsIn->nHeight) == ABS(pVidPropsOut->nHeight) / 2)
				nFieldScale = 2;// Fix for pause / fields conversion

			eScaleType = eMST_NoScale;
		}
		// HD -> SD letterbox (add +60 as could be overscan)
		else if (dblAR_In >= dblAR_Out || ABS(pVidPropsIn->nHeight) > ABS(pVidPropsOut->nHeight) + 60)
		{
			eScaleType = eMST_LetterBox;
		}
		else // SD -> HD/SD crop
		{
			eScaleType = eMST_Crop;
		}
	}

	int nFlipIn = 0;
	RECT rcIn = Rect_Correct(pRectIn, true, pVidPropsIn->nWidth, ABS(pVidPropsIn->nHeight), NULL, &nFlipIn);
	int nFlipOut = 0;
	RECT rcOut = Rect_Correct(pRectOut, true, pVidPropsOut->nWidth, ABS(pVidPropsOut->nHeight), NULL, &nFlipOut);

	if (!STR_IS_EMPTY(pszCrop))
	{
		RECT rcBorders = {};
		XMLUtils::String2Borders(pszCrop, rcBorders);
		rcIn.left += rcBorders.left;
		rcIn.right -= rcBorders.right;
		rcIn.top += rcBorders.top;
		rcIn.bottom += rcBorders.bottom;
	}

	if (!STR_IS_EMPTY(pszBorders))
	{
		RECT rcBorders = {};
		XMLUtils::String2Borders(pszBorders, rcBorders);
		rcOut.left += rcBorders.left;
		rcOut.right -= rcBorders.right;
		rcOut.top += rcBorders.top;
		rcOut.bottom += rcBorders.bottom;
	}


	if (eScaleType == eMST_Crop || eScaleType == eMST_LetterBox)
	{
		// Correct using AR
		//double dblWidthIn = MVidProps_AR(pVidPropsIn) * ABS(pVidPropsIn->nHeight);
		//double dblWidthOut = MVidProps_AR(pVidPropsOut) * ABS(pVidPropsOut->nHeight);
		//dblPixelAR = MVidProps_AR( _pVidProps ) * ABS(_pVidProps->nHeight) / _pVidProps->nWidth
		double dblWidthIn = dblPixelAR_In * RECT_W(rcIn);
		double dblWidthOut = dblPixelAR_Out * RECT_W(rcOut);

		double dblScaleX = dblWidthOut / dblWidthIn;
		double dblScaleY = (double)RECT_H(rcOut) / RECT_H(rcIn);

		double dblScaleAR = (eScaleType == eMST_Crop ? dblScaleX / dblScaleY : dblScaleY / dblScaleX);
		RECT* pRectCorrect = (eScaleType == eMST_Crop ? &rcIn : &rcOut);
		if (dblScaleAR < 1.0)
		{
			// Cut by input width 
			int nOffsetX = DBL2INT(pRectCorrect->right * (1.0 - dblScaleAR) / 2.0);
			pRectCorrect->left += ALIGN_UP(nOffsetX, 2);
			pRectCorrect->right -= ALIGN_UP(nOffsetX, 2);
		}
		else
		{
			int nOffsetY = DBL2INT(pRectCorrect->bottom * (1.0 - 1.0 / dblScaleAR) / 2.0);
			pRectCorrect->top += ALIGN_UP(nOffsetY, 2);
			pRectCorrect->bottom -= ALIGN_UP(nOffsetY, 2);
		}

	}
	else if (eScaleType == eMST_NoScale)
	{
		// No scale
		int nWidth = MIN(rcIn.right, rcOut.right);
		int nHeight = MIN(rcIn.bottom * nFieldScale, rcOut.bottom);

		rcIn.left = ALIGN_UP((rcIn.right - nWidth) / 2, 2);
		rcIn.right -= rcIn.left;
		rcIn.top = ALIGN_UP((rcIn.bottom - nHeight) / 2, 2);
		rcIn.bottom -= rcIn.top;

		rcOut.left = ALIGN_UP((rcOut.right - nWidth) / 2, 2);
		rcOut.right -= rcOut.left;
		rcOut.top = ALIGN_UP((rcOut.bottom - nHeight) / 2, 2);
		rcOut.bottom -= rcOut.top;
	}


	nFlipOut ^= nFlipIn;
	*pRectIn = rcIn;

	rcOut = Rect_Flip(rcOut, nFlipOut, true);
	*pRectOut = rcOut;


	if (RECT_H(rcIn) == RECT_H(rcOut) &&
		RECT_H(rcIn) == ABS(pVidPropsIn->nHeight) &&
		RECT_H(rcOut) == ABS(pVidPropsOut->nHeight) &&
		RECT_W(rcIn) == RECT_W(rcOut) &&
		RECT_W(rcIn) == pVidPropsIn->nWidth &&
		RECT_W(rcOut) == pVidPropsOut->nWidth)
	{
		return false;
	}

	return true;
}





inline bool IsPullMode( IUnknown* _pObject )
{
	CComQIPtr<IMProps> qpMProps( _pObject );
	if( qpMProps )
	{
		CComBSTR cbsMode;
		qpMProps->PropsGet( CComBSTR(L"pull"), &cbsMode );

		if( cbsMode && Str2Bool( cbsMode ) )
		{
			return true;
		}
	}

	return false;
}

inline LPCWSTR MCorrectID( CComBSTR& _cbsNewID, bool _bLowerCase = false )
{
	if( _bLowerCase ) 
		::CharLowerW( _cbsNewID );

	return XMLUtils::ReplaceChars( _cbsNewID.m_str, M_DENIED_CHARS, L'-', true );
}

inline HRESULT XMLStoreMVidProps( XMLParse::XMLNodePtr _pXMLNode, M_VID_PROPS* _pVidProps, bool _bIgnoreVideoFormat = false, bool _bClearUnspecified = false )
{
	ATLASSERT( _pXMLNode );
	if( !_pXMLNode ) return E_POINTER;

	if (_pVidProps && _pVidProps->eVideoFormat != eMVF_Custom && !_bIgnoreVideoFormat)
	{
		_pXMLNode->SetAttribute(L"video_format", VF_Str(_pVidProps->eVideoFormat));
	}
	else if (_bClearUnspecified)
	{
		_pXMLNode->RemoveAttribute(L"video_format");
	}

	if (_pVidProps && _pVidProps->nWidth != 0)
		_pXMLNode->SetAttribute(L"width", _pVidProps->nWidth);
	else if (_bClearUnspecified)
		_pXMLNode->RemoveAttribute(L"width");

	if (_pVidProps && _pVidProps->nHeight != 0)
		_pXMLNode->SetAttribute(L"height", _pVidProps->nHeight);
	else if (_bClearUnspecified)
		_pXMLNode->RemoveAttribute(L"height");

	if (_pVidProps && _pVidProps->dblRate > 0)
		_pXMLNode->SetAttribute(L"rate", _pVidProps->dblRate);
	else if (_bClearUnspecified)
		_pXMLNode->RemoveAttribute(L"rate");

	if (_pVidProps && _pVidProps->fccType != eMFCC_Default)
		_pXMLNode->SetAttribute(L"pixel_format", _pVidProps->fccType ? FCC2NAMEW(_pVidProps->fccType) : L"Default");
	else if (_bClearUnspecified)
		_pXMLNode->RemoveAttribute(L"pixel_format");

	if (_pVidProps && _pVidProps->nAspectX > 0 && _pVidProps->nAspectY > 0)
		_pXMLNode->SetAttribute(L"aspect_ratio", PAIR2STR(_pVidProps->nAspectX, _pVidProps->nAspectY));
	else if (_bClearUnspecified)
		_pXMLNode->RemoveAttribute(L"aspect_ratio");

	if (_pVidProps && _pVidProps->eInterlace != eMI_Default)
		_pXMLNode->SetAttribute(L"interlace", eM_InterlaceStr, _pVidProps->eInterlace);
	else if (_bClearUnspecified)
		_pXMLNode->RemoveAttribute(L"interlace");

	if (_pVidProps && _pVidProps->eScaleType != eMST_Default)
		_pXMLNode->SetAttribute(L"scale_type", eM_ScaleTypeStr, _pVidProps->eScaleType);
	else if (_bClearUnspecified)
		_pXMLNode->RemoveAttribute(L"scale_type");
// 	if( _pVidProps->e3DFormat != eM3D_None  )
// 		_pXMLNode->SetAttribute( L"format_3d", M3D_Str(_pVidProps->e3DFormat) );

	return S_OK;
}

// Override aspect-ration, scale-type and interlace
inline HRESULT XMLOverrideMVidProps(XMLParse::XMLNodePtr _pXMLNode, M_VID_PROPS* _pVidProps)
{
	ATLASSERT(_pVidProps);
	if (!_pXMLNode || !_pVidProps) return E_POINTER;

	bool bUpdate = _pXMLNode->LoadAttribute(L"scale_type", eM_ScaleTypeStr, _pVidProps->eScaleType);
	if( !bUpdate ) // For compatibility with old props
		bUpdate = _pXMLNode->LoadAttribute(L"keep-aspect", eM_ScaleTypeStr, _pVidProps->eScaleType);

	// Fix for 'none' scale-type
	if (_pVidProps->eScaleType >= 5)
		_pVidProps->eScaleType = eMST_IgnoreAR;

	// Note: The "0:0" is valid value -> e.g. reset AR
	LPCWSTR pszAR = _pXMLNode->GetStringAttribute(L"aspect_ratio");
	if (!STR_IS_EMPTY(pszAR))
	{
		pair<int, int> pairAR = XMLUtils::Str2Pair(pszAR);
		
		_pVidProps->nAspectX = pairAR.first;
		_pVidProps->nAspectY = pairAR.second;
		bUpdate = true;
	}

	bUpdate |= _pXMLNode->LoadAttribute(L"interlace", eM_InterlaceStr, _pVidProps->eInterlace);

	return bUpdate ? S_OK : S_FALSE;
}

inline HRESULT XMLLoadMVidProps( XMLParse::XMLNodePtr _pXMLNode, M_VID_PROPS* _pVidProps, bool _bIgnoreVideoFormat = false )
{
	ATLASSERT(  _pVidProps );
	if( !_pXMLNode || !_pVidProps ) return E_POINTER;

	if( _pXMLNode->HaveAttribute( L"video_format") && !_bIgnoreVideoFormat )
	{
		_pVidProps->eVideoFormat = VF_StrToVF( _pXMLNode->GetStringAttribute(L"video_format" ), eMVF_Custom );
		UpdateMVidProps( _pVidProps, true );
	}

	_pXMLNode->LoadAttribute( L"width", _pVidProps->nWidth );
	_pXMLNode->LoadAttribute( L"height", _pVidProps->nHeight );
	_pXMLNode->LoadAttribute( L"rate", _pVidProps->dblRate );

	if( _pXMLNode->HaveAttribute( L"pixel_format" ) )
		_pVidProps->fccType = (eMFCC)Str2Fcc( _pXMLNode->GetStringAttribute( L"pixel_format", FCC2NAMEW(_pVidProps->fccType) ) );
	else 
		_pVidProps->fccType = (eMFCC)Str2Fcc( _pXMLNode->GetStringAttribute( L"colorspace", FCC2NAMEW(_pVidProps->fccType) ) );

	XMLOverrideMVidProps(_pXMLNode, _pVidProps);

	int n3DIdx = 0; 
// 	if( _pXMLNode->LoadAttribute( L"format_3d", eM_3DFormatIdxStr, n3DIdx ) && n3DIdx < M3DF_MAX )
// 		_pVidProps->e3DFormat = eM_3DFormatIdx[ n3DIdx ];

	_pVidProps->nRowBytes = 0;
	UpdateMVidProps( _pVidProps, false );
	

	return S_OK;
}

inline HRESULT XMLStoreMAudProps( XMLParse::XMLNodePtr _pXMLNode, M_AUD_PROPS* _pAudProps )
{
	ATLASSERT( _pXMLNode && _pAudProps );
	if( !_pXMLNode || !_pAudProps ) return E_POINTER;

	if( _pAudProps->nChannels != 0 ) 
		_pXMLNode->SetAttribute( L"channels", _pAudProps->nChannels );
	if( _pAudProps->nSamplesPerSec != 0 ) 
		_pXMLNode->SetAttribute( L"rate", _pAudProps->nSamplesPerSec );
	if( _pAudProps->nBitsPerSample != 0 ) 
		_pXMLNode->SetAttribute( L"bits", _pAudProps->nBitsPerSample );
	

	return S_OK;
}

inline HRESULT XMLLoadMAudProps( XMLParse::XMLNodePtr _pXMLNode, M_AUD_PROPS* _pAudProps )
{
	ATLASSERT( _pAudProps );
	if( !_pXMLNode || !_pAudProps ) return E_POINTER;

	_pXMLNode->LoadAttribute( L"channels", _pAudProps->nChannels );
	_pXMLNode->LoadAttribute( L"rate", _pAudProps->nSamplesPerSec );
	_pXMLNode->LoadAttribute( L"bits", _pAudProps->nBitsPerSample );

	return S_OK;
}

// For XML props
inline int XMLCopyMProps( IUnknown* _pSource, IUnknown* _pDest, LPCWSTR* _ppszProps )
{
	CComQIPtr<IMProps> qpMPropsSrc( _pSource );
	if( !qpMPropsSrc )
		_pSource->QueryInterface( __uuidof(IMFProps), (void**)&qpMPropsSrc.p );

	CComQIPtr<IMProps> qpMPropsDst( _pDest );
	if( !qpMPropsDst )
		_pDest->QueryInterface( __uuidof(IMFProps), (void**)&qpMPropsDst.p );

	if( !qpMPropsSrc || !qpMPropsDst )
		return 0;

	int nCount = 0;
	if( !_ppszProps )
	{
		// Copy all
		int nCount = 0;
		qpMPropsSrc->PropsGetCount( NULL, &nCount );
		for( int i = 0; i < nCount; i++ )
		{
			CComBSTR cbsName;
			CComBSTR cbsValue;
			BOOL bNode = FALSE;
			HRESULT hr = qpMPropsSrc->PropsGetByIndex( NULL, i, &cbsName, &cbsValue, &bNode );
			if( hr == S_OK && !bNode )
			{
				qpMPropsDst->PropsSet( cbsName, cbsValue );
			}
		}
	}
	else
	{
		while( *_ppszProps )
		{
			CComBSTR cbsValue;
			HRESULT hr = qpMPropsSrc->PropsGet( CComBSTR(*_ppszProps), &cbsValue );
			if( hr == S_OK )
			{
				qpMPropsDst->PropsSet( CComBSTR(*_ppszProps), cbsValue );
				nCount++;
			}

			_ppszProps++;
			
		}
	}
	
	return nCount;
}

// Note: if _ppszProps - the limit for property name is near 512 symbols
inline int XMLCopyMProps( IUnknown* _pSource, XMLParse::XMLNodePtr pXMLNode, LPCWSTR* _ppszProps, LPCWSTR _pszSrcPrefix = NULL, LPCWSTR _pszDstPrefix = NULL )
{
	ATLASSERT( pXMLNode );
	if( !pXMLNode ) return 0;

	CComQIPtr<IMProps> qpMPropsSrc( _pSource );
	if( !qpMPropsSrc )
		_pSource->QueryInterface( __uuidof(IMFProps), (void**)&qpMPropsSrc.p );

	if( !qpMPropsSrc )
		return 0;

	int nCount = 0;
	if( !_ppszProps )
	{
		// Copy all
		int nCount = 0;
		qpMPropsSrc->PropsGetCount( NULL, &nCount );
		for( int i = 0; i < nCount; i++ )
		{
			BOOL bNode = FALSE;
			CComBSTR cbsName;
			CComBSTR cbsValue;
			HRESULT hr = qpMPropsSrc->PropsGetByIndex( NULL, i, &cbsName, &cbsValue, &bNode );
			if( hr == S_OK && !bNode )
			{
				pXMLNode->SetAttribute( cbsName, cbsValue );
			}
		}
	}
	else
	{
		USES_SF_MACROS;

		while( *_ppszProps )
		{
			CComBSTR cbsValue;
			HRESULT hr = qpMPropsSrc->PropsGet( CComBSTR( STR_APPEND_SF(_pszSrcPrefix, *_ppszProps) ), &cbsValue );
			if( hr == S_OK )
			{
				pXMLNode->SetAttribute(STR_APPEND_SF(_pszDstPrefix, *_ppszProps), cbsValue );
				nCount++;
			}

			_ppszProps++;

		}
	}

	return nCount;
}

inline int XMLCopyToMFSideData(XMLParse::XMLNodePtr _pXMLData, IUnknown* _pDataTo, LPCWSTR* _ppszAttributesList = NULL, LPCWSTR _pszAddPrefix = NULL, bool _bConvertToLong64 = false)
{
	if (!_pXMLData)
		return -1;

	int nCount = 0;
	if(_ppszAttributesList)
	{
		while (*_ppszAttributesList)
		{
			CComBSTR cbsName = _pszAddPrefix ? _pszAddPrefix : L"";
			cbsName.Append(*_ppszAttributesList);
			XMLParse::XMLAttribute::TPtr pAttr = _pXMLData->GetAttribute(*_ppszAttributesList);
			if (pAttr)
			{
				CComBSTR cbsValue = pAttr->GetString();
				if (_bConvertToLong64)
					MFSideData_DataSet(_pDataTo, cbsName, Str2Bytes(cbsValue) );
				else
					MFSideData_StrSet(_pDataTo, cbsName, cbsValue );
				
				nCount++;
			}
			
			_ppszAttributesList++;
		}
	}
	else
	{
		for (int i = 0; i < _pXMLData->GetAttributesCount(); i++)
		{
			CComBSTR cbsName = _pszAddPrefix ? _pszAddPrefix : L"";
			cbsName.Append(_pXMLData->GetAttributeName(i));

			XMLParse::XMLAttribute::TPtr pAttr = _pXMLData->GetAttributeAt(i);
			if (pAttr)
			{
				if (_bConvertToLong64)
					MFSideData_DataSet(_pDataTo, cbsName, pAttr->GetInt64());
				else
					MFSideData_StrSet(_pDataTo, cbsName, pAttr->GetString());

				nCount++;
			}
		}
	}

	return nCount;
}

inline HRESULT __stdcall SEH_PropsSet(IMProps* pThis, BSTR _bsName, BSTR _bsVal )
{
	HRESULT hr = E_UNEXPECTED;
	__try
	{
		if( pThis )
			hr = pThis->PropsSet( _bsName, _bsVal );
	}
	__except(DS_SEH_HANDLER)
	{
		DWORD dwExcept = GetExceptionCode();
		ATLTRACE2(atlTraceGeneral, TRACELEVEL_SEH_QueryInterface,
			_T("###Err### PropsSet(%ws,%ws): Exception 0x%X\n"), _bsName, _bsVal, dwExcept );
	}
	return hr;
}

inline int XMLCopyMProps( XMLParse::XMLNodePtr _pXMLNode, IUnknown* _pDest, LPCWSTR* _ppszProps, LPCWSTR _pszPrefix = NULL, bool _bCopyNestedNodes = NULL, bool _bOverwrite = true )
{
	ATLASSERT( _pXMLNode );
	if( !_pXMLNode || !_pDest ) return 0;

	CComQIPtr<IMAttributes> qpMAttributes( _pDest );
	CComQIPtr<IMProps> qpMPropsDst( _pDest );
	if( !qpMPropsDst )
		_pDest->QueryInterface( __uuidof(IMFProps), (void**)&qpMPropsDst.p );

	if( !qpMPropsDst && !qpMAttributes )
		return 0;

	int nCount = 0;
	if( !_ppszProps )
	{
		int nCount = _pXMLNode->GetAttributesCount();
		for( int i = 0; i < nCount; i++ )
		{
			CComBSTR cbsName;
			CComBSTR cbsValue;
			if( _pXMLNode->GetAttributeName( i, &cbsName ) && _pXMLNode->GetAttributeValue( i, &cbsValue ) )
			{
				CComBSTR cbsComposedName = _pszPrefix;
				cbsComposedName.Append(cbsName);

				if( qpMPropsDst )
				{
					CComBSTR cbsPropPrevValue;
					qpMPropsDst->PropsGet( cbsComposedName, &cbsPropPrevValue );
					if( _bOverwrite || !cbsPropPrevValue )
						qpMPropsDst->PropsSet( cbsComposedName, cbsValue );
				}
				else if( qpMAttributes )
				{
					qpMAttributes->AttributesStringSet( cbsComposedName, cbsValue );
				}
			}
		}

		if (_bCopyNestedNodes)
		{
			int nNodes = _pXMLNode->GetNodesCount();
			for (int i = 0; i < nNodes; i++)
			{
				XMLParse::XMLNodePtr pXMLNode = _pXMLNode->GetNodeAt(i);
				if (!pXMLNode->IsValue())
				{
					CComBSTR cbsPrefix = _pszPrefix;
					cbsPrefix.Append(_pXMLNode->GetNodeAt(i)->Name());
					cbsPrefix.Append(L"::");

					XMLCopyMProps(pXMLNode, _pDest, NULL, cbsPrefix);
				}
			}
		}
	}
	else
	{
		while( *_ppszProps )
		{
			CComBSTR cbsValue = _pXMLNode->GetStringAttribute_Safe( *_ppszProps );
			if( cbsValue.m_str )
			{
				CComBSTR cbsName = *_ppszProps;

				ATLTRACE2( atlTraceGeneral, 10, _T("XMLCopyMProps(%ws,%ws)\n"), cbsName, cbsValue );
				if( qpMPropsDst )
					SEH_PropsSet( qpMPropsDst, cbsName, cbsValue );
				else if( qpMAttributes )
					qpMAttributes->AttributesStringSet( cbsName, cbsValue );

				nCount++;
			}

			_ppszProps++;
		}
	}

	return nCount;
}

inline int XMLRemoveMProps( IUnknown* _pProps, LPCWSTR* _ppszPropsRemove, LPCWSTR* _ppszPropsKeep )
{
	CComQIPtr<IMAttributes> qpMAttributes( _pProps );
	CComQIPtr<IMProps> qpMPropsDst( _pProps );
	if( !qpMPropsDst )
		_pProps->QueryInterface( __uuidof(IMFProps), (void**)&qpMPropsDst.p );

	if( !qpMPropsDst && !qpMAttributes )
		return 0;

	int nCount = 0;
	if( !_ppszPropsRemove && !_ppszPropsKeep )
	{
		// Remove all 
		if( qpMPropsDst )
			qpMPropsDst->PropsRemove( CComBSTR( L"_all_props_") );
		else if( qpMAttributes )
			qpMAttributes->AttributesRemoveAll();
	}
	else if( _ppszPropsRemove)
	{
		while( *_ppszPropsRemove )
		{
			if( !_ppszPropsKeep || !InStringList( *_ppszPropsRemove, _ppszPropsKeep) )
			{
				if( qpMPropsDst )
					qpMPropsDst->PropsRemove( CComBSTR( *_ppszPropsRemove ) );
				else if( qpMAttributes )
					qpMAttributes->AttributesRemove( CComBSTR( *_ppszPropsRemove ) );
			}
			_ppszPropsRemove++;
		}
	}
	else 
	{
		ATLASSERT( _ppszPropsKeep );

		int nCount = 0;
		qpMPropsDst->PropsGetCount( NULL, &nCount );

		CSimpleArray2<CComBSTR> arrRemove;
		for( int i = 0; i < nCount; i++ )
		{
			BOOL bNode = FALSE;
			CComBSTR cbsName;
			CComBSTR cbsValue; // For proxy/stub 
			qpMPropsDst->PropsGetByIndex( NULL, i, &cbsName, &cbsValue, &bNode );

			if( !bNode && !InStringList( cbsName, _ppszPropsKeep ) )
				arrRemove.Add( cbsName );
		}

		for( int i = 0; i < arrRemove.GetSize(); i++ )
		{
			qpMPropsDst->PropsRemove( arrRemove[i] );
		}
	}


	return nCount;
}

#endif // NEW_XML_PARSE

// Note: This function could
// 0 - full frames, 1 - top field, 2 - bottom field
inline int MFFrame_IsField( IUnknown* _pFrame, int* _pnOffset = NULL )
{
	CComQIPtr<IMFFrame> qpMFFrame( _pFrame );
	ATLASSERT( qpMFFrame || !_pFrame );
	if( qpMFFrame )
	{
		MF_FRAME_INFO mfInfo = {};
		qpMFFrame->MFAllGet( &mfInfo );

		// Check the field
		if( mfInfo.avProps.vidProps.nRowBytes == Img_RowSizeM( &mfInfo.avProps.vidProps ) * 2 )
		{
			LONG cbDataFake = 0;
			LONGLONG lpVideoSrc = 0;
			qpMFFrame->MFDataGet( CComBSTR( L"__vptr_"), &cbDataFake, &lpVideoSrc );

			int nOffset = mfInfo.lpVideo - lpVideoSrc;

			if( _pnOffset )
				*_pnOffset = nOffset;

			if( nOffset == mfInfo.avProps.vidProps.nRowBytes / 2 )
				return 2;

			if( nOffset == 0 )
				return 1;
		}
		else if( _pnOffset )
		{
			LONG cbDataFake = 0;
			LONGLONG lpVideoSrc = 0;
			qpMFFrame->MFDataGet( CComBSTR( L"__vptr_"), &cbDataFake, &lpVideoSrc );

			*_pnOffset = mfInfo.lpVideo - lpVideoSrc;
		}

	}

	return 0;
}

inline HRESULT MFFrame_UpdateAudioLevel(IUnknown* _pFrame, bool _bRecalculate)
{
	CComQIPtr<IMFFrame> qpMFFrame(_pFrame);
	ATLASSERT(qpMFFrame || !_pFrame);
	if (!qpMFFrame)
		return S_FALSE;

	return qpMFFrame->MFAudioGain(_bRecalculate ? CComBSTR("recalc_vu_meters") : NULL, 0.0, 0.0);
}



//////////////////////////////////////////////////////////////////////////
// For frame debug

inline LPCWSTR MFFrame2Str(IUnknown* _pFrame, LPWSTR _pszBuffer)
{
	CComQIPtr<IMFFrame> qpMFFrame(_pFrame);
	if (!qpMFFrame)
	{
		wsprintfW(_pszBuffer, L"IMFFrame=NULL (0x%08X)", (int)_pFrame);
	}
	else
	{
		MF_FRAME_INFO info = MFFrame_Info(qpMFFrame);
		
		REFERENCE_TIME rtAudio = info.cbAudio > 0 ? MAudBytes2Time(&info.avProps.audProps, info.cbAudio) : 0;

		LONG cb608Data = 0, cb708Data = 0;
		LONGLONG lpData = 0;
		qpMFFrame->MFDataGet(CComBSTR("C608"), &cb608Data, &lpData);
		qpMFFrame->MFDataGet(CComBSTR("C708"), &cb708Data, &lpData);

		double dblBufferPos = BufferPosDbl(info.mTime.eFFlags);

		double dblDealyMsec = MFFrame_DelayMsec(_pFrame);

		_snwprintf(_pszBuffer, 512, L"t:%s..%s(%s) #%d (%I64d) f:%05X%ws B:%.2f %d(%d)x%d(%s)@%.3f%ws %ws%ws(%d:%d%ws) %d:%d:%d:%X(%d/%s) delay=%.3f Dt:%d(CC:%d,%d) Obj:%d Str:%d ID:%I64X V:0x%08X(%d) A:0x%08X(%d) ",
			RT2STR(info.mTime.rtStartTime), RT2STR(info.mTime.rtEndTime), MTC2STR(info.mTime.tcFrame), info.mTime.tcFrame.nExtraCounter,
			info.mTime.rtEndTime - info.mTime.rtStartTime,
			info.mTime.eFFlags, info.avProps.bLocked ? L"L" : L"U", dblBufferPos,
			info.avProps.vidProps.nWidth, info.avProps.vidProps.nRowBytes, info.avProps.vidProps.nHeight,
			FCC2NAMEW(info.avProps.vidProps.fccType), info.avProps.vidProps.dblRate, ARRAY_VAL(eM_InterlaceStrS, info.avProps.vidProps.eInterlace),
			DX_Format2Str(info.avProps.vidProps.eGPUFlags, L""), info.avProps.vidProps.eGPUFlags & eGTF_GPU_SeparateFields ? L"x2" : L"",
			info.avProps.vidProps.nAspectX, info.avProps.vidProps.nAspectY, ARRAY_VAL(eM_ScaleTypeStrS, info.avProps.vidProps.eScaleType),
			info.avProps.audProps.nChannels, info.avProps.audProps.nSamplesPerSec, info.avProps.audProps.nBitsPerSample,
			info.avProps.audProps.nTrackSplitBits, info.lAudioSamples, RT2STR(rtAudio),
			dblDealyMsec, info.nDataCount, cb608Data, cb708Data,
			info.nObjCount, info.nStrCount, info.llVideoID, (LONG)info.lpVideo, info.cbVideo, (LONG)info.lpAudio, info.cbAudio);

	}

	return _pszBuffer;
}

inline CComBSTR MFFrame2Str(IUnknown* _pFrame)
{
	WCHAR szBuffer[512] = {};
	return MFFrame2Str(_pFrame, szBuffer);
}

#ifdef _DEBUG
inline LPCWSTR MFFrame_Debug_I( IMFFrame* _pFrame, LPWSTR _psz )
{
	return MFFrame2Str(_pFrame, _psz);
}

inline LPCWSTR MFFrame_Debug( IUnknown* _pFrame, LPWSTR _psz )
{
	CComPtr<IMFFrame> cpMFFrame;
	if (_pFrame)
	{
		HRESULT hr = _pFrame->QueryInterface( __uuidof(IMFFrame), (void**)&cpMFFrame);
		if (!cpMFFrame)
		{
			wsprintfW(_psz, L"IMFFrame(0x%08X) QI FAILED hr=0x%08X", (int)_pFrame, hr);
			return _psz;
		}
	}
	return MFFrame_Debug_I(cpMFFrame, _psz );
}

#else
inline LPCWSTR MFFrame_Debug( IUnknown* _pFrame, LPWSTR _psStr ) {return NULL;}
inline LPCWSTR MFFrame_Debug_I( IMFFrame* _pFrame, LPWSTR _psStr ) {return NULL;}
#endif

inline HRESULT MFFrame_CopyAudio(IUnknown* _pFrameFrom, IUnknown* _pFrameTo)
{
	CComQIPtr<IMFFrame> qpMFrameFrom(_pFrameFrom);
	CComQIPtr<IMFFrame> qpMFrameTo(_pFrameTo);
	if (!qpMFrameTo || !qpMFrameTo )
		return E_INVALIDARG;

	return qpMFrameFrom->MFAudioCopyTo(NULL, qpMFrameTo);
// 	long lAudioSamples = 0;
// 	LPBYTE pbAudio = NULL;
// 	M_AV_PROPS avPropsIn = MFFrame_AVProps(_pFrameFrom, &lAudioSamples, NULL, &pbAudio);
// 	if (IsSpecified(&avPropsIn.audProps) && pbAudio && lAudioSamples > 0)
// 	{
// 		CComQIPtr<IMFFrame> qpMFrameTo(_pFrameTo);
// 		if (!qpMFrameTo)
// 			return E_INVALIDARG;
// 
// 		qpMFrameTo->MFAudioSet(&avPropsIn.audProps, MAudSamples2Bytes(&avPropsIn.audProps, lAudioSamples), (LONGLONG)pbAudio);
// 
// 		M_AV_PROPS avPropsTo = MFFrame_AVProps(qpMFrameTo);
// 		avPropsTo.ancData = avPropsIn.ancData;
// 		qpMFrameTo->MFAVPropsSet(&avPropsTo, NULL);
// 		return S_OK;
// 	}
// 
// 	return S_FALSE;
}

inline M_VID_PROPS MVidProps_Limit(M_VID_PROPS* _pVidProps, int _nLimitWidth )
{
	M_VID_PROPS vidProps = {};
	ATLASSERT(_pVidProps);
	if (!IsSpecified(_pVidProps) )
		return _pVidProps ? *_pVidProps : vidProps;

	vidProps = *_pVidProps;

	_nLimitWidth = MAX(32, _nLimitWidth);
	
	if (_pVidProps->nWidth)
	{
		vidProps.nWidth = ALIGN_UP(MIN(_nLimitWidth, vidProps.nWidth), 2);
		vidProps.nHeight = DBL2INT(vidProps.nWidth * ABS(_pVidProps->nHeight) / _pVidProps->nWidth);
		vidProps.nHeight = ALIGN_UP(ABS(vidProps.nHeight), 2) * SIGN( _pVidProps->nHeight);
		vidProps.nRowBytes = 0;
		UpdateMVidProps(&vidProps, false);
	}

	return vidProps;
}


#ifdef NEW_XML_PARSE

// _pszExtraPrefix used e.g. for Preview - for preview.audio_channels & preview.audio_gain
inline int MFFrame_UpdateAudio( IUnknown* _pFrame, XMLParse::XMLNodePtr _pXMLProps, LPCWSTR _pszExtraPrefix = NULL )
{
	CComQIPtr<IMFFrame> qpMFFrame( _pFrame );
	if( !qpMFFrame )
		return -1;

	int nUpdate = 0;

	LPCWSTR pszAudCH = NULL;
	if( !STR_IS_EMPTY(_pszExtraPrefix) )
	{
		CComBSTR cbsName = _pszExtraPrefix;
		cbsName.Append( L"audio_channels" );
		pszAudCH = _pXMLProps->GetStringAttribute( cbsName );
	}
	if( STR_IS_EMPTY(pszAudCH) )
		pszAudCH = _pXMLProps->GetStringAttribute( L"audio_channels" );
	if( !STR_IS_EMPTY(pszAudCH) )
	{
		qpMFFrame->MFAVPropsSet( NULL, CComBSTR( pszAudCH ) );
		nUpdate |= 1;
	}

	LPCWSTR pszAudGain = NULL;
	if( !STR_IS_EMPTY(_pszExtraPrefix) )
	{
		CComBSTR cbsName = _pszExtraPrefix;
		cbsName.Append( L"audio_gain" );
		pszAudGain = _pXMLProps->GetStringAttribute( cbsName );
	}
	if( STR_IS_EMPTY(pszAudGain) )
		pszAudGain = _pXMLProps->GetStringAttribute( L"audio_gain" );
	if( !STR_IS_EMPTY(pszAudGain) )
	{
		CSimpleArray2<double> arrGain;
		XMLUtils::String2ArrayDbl( pszAudGain, arrGain );

		if( arrGain.GetSize() == 1  )
		{
			qpMFFrame->MFAudioGain( NULL, arrGain[0], arrGain[0] );
		}
		else
		{
			for ( int i = 0; i < arrGain.GetSize(); i++ )
			{
				qpMFFrame->MFAudioGain( CComBSTR( INT2STRW(i) ), arrGain[i], arrGain[i] );
			}
		}

		nUpdate |= 2;
	}

	return nUpdate;
}

// Update crop & Mirror
template <class TInterface>
inline int MFFrame_UpdateCropMirror( IUnknown* _pFrame, TInterface** _ppFrameOut, XMLParse::XMLNodePtr _pXMLProps, bool _bCopyForMirror = true )
{
	ATLASSERT( _ppFrameOut );
	if( !_ppFrameOut ) return -1;

	CComQIPtr<IMFFrame> qpMFFrame( _pFrame );
	if( !qpMFFrame )
		return -1;

	int nUpdate = 0;
	long lAudio = 0;
	M_AV_PROPS avPropsIn = {};
	qpMFFrame->MFAVPropsGet( &avPropsIn, &lAudio );

	LPCWSTR pszBorders = _pXMLProps->GetStringAttribute( L"crop" );
	RECT rcBorders = {};
	XMLUtils::String2Borders( pszBorders, rcBorders );
	if( rcBorders.left || rcBorders.right || rcBorders.top || rcBorders.bottom )
	{
		RECT rcCut = {rcBorders.left, rcBorders.top,
			avPropsIn.vidProps.nWidth - rcBorders.right, ABS(avPropsIn.vidProps.nHeight) - rcBorders.bottom };

		CComPtr<IMFFrame> cpCut;
		qpMFFrame->MFCut( -1, &rcCut, &cpCut );
		if( cpCut )
		{
			qpMFFrame = cpCut;
			nUpdate |= 1;
		}
	}

	LPCWSTR ppszMirror[] = {L"", L"horz", L"vert", L"both", NULL };
	int nMirror = _pXMLProps->GetEnumAttribute( L"mirror", ppszMirror );
	if( nMirror > 0 )
	{
		if( _bCopyForMirror )
		{
			CComPtr<IMFFrame> cpMFrameOut;
			qpMFFrame->MFClone( &cpMFrameOut, eMFC_Full, eMFCC_Default );
			ATLASSERT( cpMFrameOut );
			if( cpMFrameOut )
				qpMFFrame = cpMFrameOut;
		}

		LONG cbVideo = 0;
		LONGLONG pbVideo = NULL;
		qpMFFrame->MFVideoGetBytes( &cbVideo, (LONGLONG*)&pbVideo );
		MConverter::MirrorFrame( (LPBYTE)pbVideo, &avPropsIn.vidProps, nMirror );

		nUpdate |= 2;
	}

	qpMFFrame.QueryInterface( _ppFrameOut );

	return nUpdate;
}

inline CComBSTR MFFrame_ApplyRGBMatrix(IUnknown* _pFrame, XMLParse::XMLNodePtr _pXMLProps)
{
	CComBSTR cbsColorMatrix = _pXMLProps->GetStringAttribute_Safe(L"gpu.rgb_transform_matrix");

	LPCWSTR ppszColorMatrix[] = { L"BT2020_BT709", L"BT709_BT2020", 
		L"RGB_YUV_601", L"RGB_YUV_709", L"RGB_YUV_2020", 
		L"YUV_RGB_601", L"YUV_RGB_709", L"YUV_RGB_2020",
		L"Full_To_16_235", L"Full_From_16_235",
		NULL };
	int nSpecialMatrix = -1;
	vector<float> vecMaxtrix(12);
	if (InStringListI(cbsColorMatrix, ppszColorMatrix, &nSpecialMatrix))
	{
		if (nSpecialMatrix == 0)
			::CopyMemory(vecMaxtrix.data(), matConvertRGB_2020_709, sizeof(float) * MIN(SIZEOF_ARRAY(matConvertRGB_2020_709), vecMaxtrix.size()));
		else if (nSpecialMatrix == 1)
			::CopyMemory(vecMaxtrix.data(), matConvertRGB_709_2020, sizeof(float) * MIN(SIZEOF_ARRAY(matConvertRGB_709_2020), vecMaxtrix.size()));
		else if(nSpecialMatrix == 2)
			::CopyMemory(vecMaxtrix.data(), matConvertRGB_YUV_601, sizeof(float) * MIN(SIZEOF_ARRAY(matConvertRGB_YUV_601), vecMaxtrix.size()));
		else if (nSpecialMatrix == 3)
			::CopyMemory(vecMaxtrix.data(), matConvertRGB_YUV_709, sizeof(float) * MIN(SIZEOF_ARRAY(matConvertRGB_YUV_709), vecMaxtrix.size()));
		else if (nSpecialMatrix == 4)
			::CopyMemory(vecMaxtrix.data(), matConvertRGB_YUV_2020, sizeof(float) * MIN(SIZEOF_ARRAY(matConvertRGB_YUV_2020), vecMaxtrix.size()));
		else if (nSpecialMatrix == 5)
			::CopyMemory(vecMaxtrix.data(), matConvertYUV_RGB_601, sizeof(float) * MIN(SIZEOF_ARRAY(matConvertYUV_RGB_601), vecMaxtrix.size()));
		else if (nSpecialMatrix == 6)
			::CopyMemory(vecMaxtrix.data(), matConvertYUV_RGB_709, sizeof(float) * MIN(SIZEOF_ARRAY(matConvertYUV_RGB_709), vecMaxtrix.size()));
		else if (nSpecialMatrix == 7)
			::CopyMemory(vecMaxtrix.data(), matConvertYUV_RGB_2020, sizeof(float) * MIN(SIZEOF_ARRAY(matConvertYUV_RGB_2020), vecMaxtrix.size()));
		else if (nSpecialMatrix == 8)
			::CopyMemory(vecMaxtrix.data(), matConvertRGB_Full_16_235, sizeof(float) * MIN(SIZEOF_ARRAY(matConvertRGB_Full_16_235), vecMaxtrix.size()));
		else //if (nSpecialMatrix == 9)
			::CopyMemory(vecMaxtrix.data(), matConvertRGB_16_235_Full, sizeof(float) * MIN(SIZEOF_ARRAY(matConvertRGB_16_235_Full), vecMaxtrix.size()));
	}
	else
	{
		CSimpleArray2<float> arrUserMatrix;
		XMLUtils::String2ArrayDbl(cbsColorMatrix, arrUserMatrix);
		if (!MFFrame_IsGPU(_pFrame) || arrUserMatrix.GetSize() == 0)
			return NULL;



		// Size 1 -> gain (level = 0)
		// Size 2 -> gain + level
		// Size 3..6 -> gain (R,G,B), level (R,G,B)
		// Size 9 -> matrix (level = 0)
		// Size 10..12 -> matrix, level (R,G,B)
		if (arrUserMatrix.GetSize() <= 2)
		{
			vecMaxtrix[0] = vecMaxtrix[5] = vecMaxtrix[10] = arrUserMatrix[0];
			if (arrUserMatrix.GetSize() == 2)
				vecMaxtrix[3] = vecMaxtrix[7] = vecMaxtrix[11] = arrUserMatrix[1];
		}
		else if (arrUserMatrix.GetSize() <= 6)
		{
			ATLASSERT(arrUserMatrix.GetSize() >= 3);
			vecMaxtrix[0] = arrUserMatrix[0];
			vecMaxtrix[5] = arrUserMatrix[1];
			vecMaxtrix[10] = arrUserMatrix[2];
			if (arrUserMatrix.GetSize() > 3)
				vecMaxtrix[3] = arrUserMatrix[3];
			if (arrUserMatrix.GetSize() > 4)
				vecMaxtrix[7] = arrUserMatrix[4];
			if (arrUserMatrix.GetSize() > 5)
				vecMaxtrix[11] = arrUserMatrix[5];
		}
		else
		{
			for (int i = 0; i < arrUserMatrix.GetSize(); ++i)
				vecMaxtrix[i] = arrUserMatrix[i];
		}
	}

	// Print matrix
	WCHAR szMatrix[128] = {};
	LPWSTR pszMatrix = szMatrix;
	for (UINT i = 0; i < vecMaxtrix.size(); ++i)
	{
		if (i % 4 == 0)
			pszMatrix += _snwprintf(pszMatrix, SIZEOF_ARRAY(szMatrix) - (pszMatrix - szMatrix), L"\r\n\t");

		pszMatrix += _snwprintf(pszMatrix, SIZEOF_ARRAY(szMatrix) - (pszMatrix - szMatrix), L"%.4f ", vecMaxtrix[i]);
	}

	MFSideData_StrSet(_pFrame, L"__rgb_transform_matrix_", szMatrix);

	return szMatrix;
}

template<class TFrameObjPtr>
inline HRESULT MFFrame_MakeCut(TFrameObjPtr& _rFrame, XMLParse::XMLNodePtr _pXMLProps, LPCWSTR _pszXMLRectName = L"src_rect")
{
	if (!_rFrame || !_pXMLProps)
		return E_INVALIDARG;

	RECT rcSrc = XMLUtils::String2Rect(_pXMLProps->GetStringAttribute(_pszXMLRectName), NULL);
	if (IsRectEmpty(&rcSrc))
		return S_FALSE;

	CComQIPtr<IMFFrame> qpMFFrameIn(_rFrame);
	if (!qpMFFrameIn)
		return E_NOINTERFACE;

	CComPtr<IMFFrame> cpFrameCut;
	qpMFFrameIn->MFCut(-1, &rcSrc, &cpFrameCut);
	if (cpFrameCut)
		_rFrame = cpFrameCut;

	return S_OK;
}

#endif // NEW_XML_PARSE

// Check via CDiag class
inline unsigned int GetOSVersion()
{
	const wchar_t* kernel32 = L"kernel32.dll";
	wchar_t path[MAX_PATH];
	UINT n = GetSystemDirectory(path, MAX_PATH);
	if (n > MAX_PATH - wcslen(kernel32)) {
		return 0xffffffff;
	}
	swprintf_s(path, L"%s\\%s", path, kernel32);
	DWORD versionSize = GetFileVersionInfoSize(path, NULL);
	if (!versionSize) {
		return 0xffffffff;
	}

	BYTE* version = new BYTE[versionSize];
	BOOL ret = GetFileVersionInfo(path, 0, versionSize, (void*)version);
	if (!ret) {
		delete[] version;
		return 0xffffffff;
	}

	BYTE* buffer = NULL;
	UINT bufferSize = 0;
	ret = VerQueryValue(version, L"\\", (void**)&buffer, &bufferSize);
	if (!ret || bufferSize < sizeof(VS_FIXEDFILEINFO)) {
		delete[] version;
		return 0xffffffff;
	}

	VS_FIXEDFILEINFO *vinfo = (VS_FIXEDFILEINFO*)buffer;
	int verOS = HIWORD(vinfo->dwProductVersionMS);
	delete[] version;
	return verOS;
}

// Obsolete - have to be moved to MCalls.h or changed to MFProps_GetBool( NULL, xxx );
inline int MFFactory_GetBool(LPCWSTR _pszName, int _nWrong = 0)
{
	CComBSTR cbsRes = MFProps_Get(MCreator::GetMFFactory(), _pszName);
	return Str2BoolInt(cbsRes, _nWrong);
}

template<class TComPtr>
inline void Obj_SafeRelease(TComPtr& _rCloseObj )
{
	while (_rCloseObj)
	{
		CComQIPtrM<IMFObject> qpMFObject(_rCloseObj);
		if (qpMFObject)
		{
			qpMFObject->MFClose();
			break;
		}

		CComQIPtrM<IMFDevice> qpMFDevice(_rCloseObj);
		if (qpMFDevice)
		{
			qpMFDevice->DeviceClose();
			break;
		}

		CComQIPtrM<IMFSink> qpSink(_rCloseObj);
		if (qpSink)
		{
			qpSink->SinkClose();
			break;
		}

		CComQIPtrM<IMFPipe> qpPipe(_rCloseObj);
		if (qpPipe)
		{
			qpPipe->PipeClose();
			break;
		}

		CComQIPtrM<IMFDecoder> qpMFDecoder(_rCloseObj);
		if (qpMFDecoder)
		{
			qpMFDecoder->DecodeClose();
			break;
		}

		CComQIPtrM<IMFEncoder> qpMFEncoder(_rCloseObj);
		if (qpMFEncoder)
		{
			qpMFEncoder->EncodeClose();
			break;
		}

		CComQIPtrM<IMFSplitter> qpMFSplitter(_rCloseObj);
		if (qpMFSplitter)
		{
			qpMFSplitter->SplitterClose();
			break;
		}

		CComQIPtrM<IMFMuxer> qpMFMuxer(_rCloseObj);
		if (qpMFMuxer)
		{
			qpMFMuxer->MuxerClose();
			break;
		}

		// Now for e.g. MFSinkReader (TODO: Make support for IMFObject in MFSinkReader)
		CComQIPtrM<IMFReader> qpMFReader(_rCloseObj);
		if (qpMFReader)
		{
			qpMFReader->ReaderClose();
			break;
		}


		ATLASSERT("Obj_SafeRelease(UNK INTERFACE)" && !_rCloseObj);
		break;
	}

	_rCloseObj.Release();
}

template<class TInterface>
inline HRESULT Obj_Create(CLSID gClassID, LPCWSTR _pszDLLName, TInterface** _ppObject)
{
	// Try CLSID
	HRESULT hr = ::CoCreateInstance(gClassID, NULL, CLSCTX_ALL, __uuidof(TInterface), (void**)_ppObject);
	if (FAILED(hr))
	{
		// Try DLL
		MTRACE_WARN("Create Objects:%ws FAILED hr=%08X - Try DLL:%ws", CComBSTR(gClassID).m_str, hr, _pszDLLName);
		hr = CreateObjectDirect(_pszDLLName, gClassID, _ppObject);
	}

	if (*_ppObject)
		MFProps_Get(*_ppObject, s_pszCheckUUID);

	return hr;
}



template< class TInterface>
static HRESULT Object_GetExternalByName( LPCWSTR _pszObjName, LPCWSTR _pszExtraPrifix, TInterface** _ppObject )
{
	ATLASSERT( _ppObject );
	if( !_pszObjName ) return E_INVALIDARG;

	CComBSTR cbsObjName = _pszObjName;
	::CharLowerW( cbsObjName.m_str );

	CComBSTR cbsPrefix = _pszExtraPrifix;
	cbsPrefix.Append( MGSENDER_PREFIX );

	// Get all tee sinks
	CSimpleArray2<CComBSTR> arrNames;
	HRESULT hr = ROTHelpers::GetAllObjects( arrNames, cbsPrefix  );
	if( FAILED( hr ) )
	{
		ATLTRACE2( atlTraceGeneral, 2, _T("###Err### Object_GetExternalByName(%ws,Prefix:%ws) - ")
			_T("ROTHelpers::GetAllObjects() FAILED hr = 0x%08X\n"), _pszObjName, _pszExtraPrifix, hr );

		return hr;
	}

	ATLTRACE2( atlTraceGeneral, 2, _T("Object_GetExternalByName(%ws,Prefix:%ws) - ")
		_T("ROTHelpers::GetAllObjects() - %d OBJECT\n"), _pszObjName, _pszExtraPrifix, arrNames.GetSize() );

	CComPtr<IUnknown> cpExtObjectFound;
	for( int i = 0; i < arrNames.GetSize(); i++ )
	{
		// Create object
		CComPtr<IUnknown> cpExtObject;
		hr = ROTHelpers::GetObjectByName( arrNames[i].m_str, &cpExtObject );
		if( FAILED( hr ) )
		{
			ATLTRACE2( atlTraceGeneral, 2, _T("###Err### Object_GetExternalByName(%ws,Prefix:%ws) - ")
				_T("ROTHelpers::GetObject([%ws]) FAILED hr = 0x%08X\n"), 
				_pszObjName, _pszExtraPrifix, arrNames[i].m_str, hr );

			continue;
		}

		// Check for MLTeeSink
		CComQIPtr<IMFProps> qpMFProps = cpExtObject;
		CComQIPtr<IMSource_I> qpMSource_I = cpExtObject;
		CComQIPtr<IMFSource_I> qpMFSource_I = cpExtObject;
		CComQIPtr<IMSender_I> qpMSender_I = cpExtObject;
		if( !qpMSource_I && !qpMFSource_I && !qpMSender_I && !qpMFProps )
		{
			ATLTRACE2( atlTraceGeneral, 2, _T("WARNING !!! Object_GetExternalByName(%ws,Prefix:%ws) - ")
				_T("ROTHelpers::GetObject([%ws]) No IMSource_I / IMFSource_I / IMSender_I interface\n"), 
				_pszObjName, _pszExtraPrifix, arrNames[i].m_str );

			continue;
		}

		CComQIPtr<IMObject> qpMObject( cpExtObject );
		if( qpMObject )
		{
			CComBSTR cbsName;
			hr = qpMObject->ObjectNameGet( &cbsName );
			::CharLowerW( cbsName.m_str );
			if( cbsName == cbsObjName )
			{
				ATLTRACE2( atlTraceGeneral, 2, _T("Object_GetExternalByName(%ws,Prefix:%ws) - ")
					_T("ROTHelpers::GetAllObjects() - FOUND:%ws\n"), _pszObjName, _pszExtraPrifix, cbsName );

				cpExtObjectFound = cpExtObject;
				break;
			}

			ATLTRACE2( atlTraceGeneral, 2, _T("Object_GetExternalByName(%ws,Prefix:%ws) - ")
				_T("ROTHelpers::GetAllObjects() Object:%ws\n"), _pszObjName, _pszExtraPrifix, cbsName );
		}
		else if( qpMFProps )
		{
			CComBSTR cbsName;
			hr = qpMFProps->PropsGet( CComBSTR( L"object_name" ), &cbsName );
			::CharLowerW( cbsName.m_str );
			if( cbsName == cbsObjName )
			{
				ATLTRACE2( atlTraceGeneral, 2, _T("Object_GetExternalByName(%ws,Prefix:%ws) - ")
					_T("ROTHelpers::GetAllObjects() - FOUND:%ws\n"), _pszObjName, _pszExtraPrifix, cbsName );

				cpExtObjectFound = cpExtObject;
				break;
			}

			ATLTRACE2( atlTraceGeneral, 2, _T("Object_GetExternalByName(%ws,Prefix:%ws) - ")
				_T("ROTHelpers::GetAllObjects() Object:%ws\n"), _pszObjName, _pszExtraPrifix, cbsName );
		}
		else
		{
			ATLTRACE2( atlTraceGeneral, 2, _T("###Err### Object_GetExternalByName(%ws,Prefix:%ws) - ")
				_T("ROTHelpers::GetObject([%ws]) No IMObject interface\n"), 
				_pszObjName, _pszExtraPrifix, arrNames[i].m_str );
		}
	}

	if( !cpExtObjectFound )
	{
		ATLTRACE2( atlTraceGeneral, 2, _T("WARNING !!! Object_GetExternalByName(%ws,Prefix:%ws) - ")
			_T("ROTHelpers::GetObject() NOT FOUND\n"), 
			_pszObjName, _pszExtraPrifix );

		return HRESULT_FROM_WIN32( ERROR_NOT_FOUND );
	}

	return cpExtObjectFound.QueryInterface( _ppObject );
}



class MTimeCorrector
{
	REFERENCE_TIME	m_rtStreamOffset;
	double	m_dblLastEndTimeRT;
public:
	MTimeCorrector()
		: m_dblLastEndTimeRT( 0 ),
		  m_rtStreamOffset( MINLONGLONG )
	{
	}


	// Offset chnaged: 

	// 1 - After reset/first
	// 2 - Break
	// 3 - Last
	// 4 - Exceed Max
	REFERENCE_TIME CalcTime( M_VID_PROPS* _pVidProps, M_TIME* _pTime, REFERENCE_TIME* _prtEnd = NULL, REFERENCE_TIME _rtMaxDiff = -1, int* _pnOffsetChanged = NULL )
	{
		REFERENCE_TIME rtEnd = 0;
		REFERENCE_TIME rtStart = 0;

		double dblFrameTimeRT = MVid2AvgTimeDbl(_pVidProps, false, 400000 );

		if( !_pTime || (_pTime->rtStartTime == 0 && _pTime->rtEndTime == _pTime->rtStartTime && _pTime->eFFlags == 0 ) )
		{
			// Generate time sequence
			rtStart = DBL2INT64( m_dblLastEndTimeRT );
			m_dblLastEndTimeRT += dblFrameTimeRT;
			rtEnd = DBL2INT64( m_dblLastEndTimeRT );
		}
		else 
		{
			// Check for max skip
			bool bExceedMax = false;
			if( !IS_BREAK(_pTime) && 
				m_rtStreamOffset != MINLONGLONG && 
				_rtMaxDiff > 0 && 
				ABS( (_pTime->rtStartTime + m_rtStreamOffset) - m_dblLastEndTimeRT) > _rtMaxDiff )
			{
				ATLTRACE2(atlTraceGeneral, 8, _T("MTimeCorrector(%08X)::CalcTime(In:%s..%s Break:%d Last:%s) TOO BIG DIFF - RESET [TIME]\n"), this,
					RT2STR(_pTime->rtStartTime), RT2STR(_pTime->rtEndTime), IS_BREAK(_pTime), RT2STR(m_dblLastEndTimeRT) );

				bExceedMax = true;
			}
		
			if( bExceedMax || m_rtStreamOffset == MINLONGLONG || IS_BREAK(_pTime) || IS_LAST(_pTime ) )
			{
				if( _pnOffsetChanged )
					*_pnOffsetChanged = m_rtStreamOffset == MINLONGLONG ? 1 : IS_BREAK( _pTime ) ? 2 : IS_LAST( _pTime ) ? 3 : 4;

				m_rtStreamOffset = m_dblLastEndTimeRT - _pTime->rtStartTime;

				ATLTRACE2(atlTraceGeneral, 8, _T("MTimeCorrector(%08X)::CalcTime(%ws In:%s..%s Last:%s Break:%d Offset:%s Out:%s) UPDATE OFFSET [TIME]\n"), this,
					IS_BREAK( _pTime ) ? L"BREAK" : L"", RT2STR(_pTime->rtStartTime), RT2STR(_pTime->rtEndTime), RT2STR(m_dblLastEndTimeRT), IS_BREAK(_pTime),
					RT2STR( m_rtStreamOffset), RT2STR(_pTime->rtStartTime + m_rtStreamOffset) );
			}
			else
			{
				ATLTRACE2(atlTraceGeneral, 8, _T("MTimeCorrector(%08X)::CalcTime(NORMAL In:%s..%s Last:%s Break:%d Offset:%s Out:%s) [TIME]\n"), this,
					RT2STR(_pTime->rtStartTime), RT2STR(_pTime->rtEndTime), RT2STR(m_dblLastEndTimeRT), IS_BREAK(_pTime),
					RT2STR( m_rtStreamOffset), RT2STR(_pTime->rtStartTime + m_rtStreamOffset) );
			}

			rtStart = _pTime->rtStartTime + m_rtStreamOffset;
			if( _pTime->rtEndTime > _pTime->rtStartTime + 1000 )
				rtEnd = _pTime->rtEndTime + m_rtStreamOffset;
			else
				rtEnd = rtStart + DBL2INT64(dblFrameTimeRT);

			m_dblLastEndTimeRT = rtEnd;
		}

		if( _prtEnd )
			*_prtEnd = rtEnd;

		return rtStart;
	}

	M_TIME CalcMTime(IUnknown* _pMFFrame, REFERENCE_TIME _rtMaxDiff = -1, int* _pnOffsetChanged = NULL)
	{
		M_TIME mTime = MFFrame_Time(_pMFFrame);
		M_AV_PROPS avProps = MFFrame_AVProps(_pMFFrame);
		
		return CalcMTime(&avProps.vidProps, &mTime, _rtMaxDiff, _pnOffsetChanged);
	}

	// Offset changed: 

	// 1 - After reset/first
	// 2 - Break
	// 3 - Last
	// 4 - Exceed Max
	M_TIME CalcMTime( M_VID_PROPS* _pVidProps, M_TIME* _pTime, REFERENCE_TIME _rtMaxDiff = -1, int* _pnOffsetChanged = NULL )
	{
		M_TIME timeRes = {};
		if( _pTime ) 
			timeRes = *_pTime;

		int nOffsetChanged = false;
		bool bAdded = IS_ADDED( _pTime );
		bool bSkipped = IS_SKIPPED( _pTime );
		bool bPause = IS_PAUSE( _pTime );
		bool bLast = IS_LAST( _pTime );
		SET_BREAK( &timeRes, m_rtStreamOffset == MINLONGLONG ? true : false );
		timeRes.rtStartTime = CalcTime( _pVidProps, _pTime, &timeRes.rtEndTime, _rtMaxDiff, &nOffsetChanged );
		if( bPause )
			SET_PAUSE( &timeRes, true );
		if( bLast )
			SET_LAST( &timeRes, true );
	
		// For not lost frame corrections flags
		if( nOffsetChanged == 1 ) // First frame in correction sequence
			SET_BREAK( &timeRes, true );
		else if( bAdded ) 
			SET_ADDED( &timeRes, true );
		else if( bSkipped || (nOffsetChanged == 2) || (nOffsetChanged == 4) ) // nTimeCorrected -> Break or time break (or last - not used for Live)
			SET_SKIPPED( &timeRes, true );
		

		if( _pnOffsetChanged )
			*_pnOffsetChanged = nOffsetChanged;

		return timeRes;
	}

	void ResetTime( REFERENCE_TIME _rtLastTime  = 0 )
	{
		ATLTRACE2(atlTraceGeneral, 8, _T("MTimeCorrector(%08X)::ResetTime( Last:%s->%s Offset:%s->NULL )\n"),
			(LONG)this, RT2STR( m_dblLastEndTimeRT ), RT2STR( _rtLastTime ), RT2STR( m_rtStreamOffset ) );

		m_dblLastEndTimeRT = _rtLastTime;
		m_rtStreamOffset = MINLONGLONG;
	}

	M_TIME UpdateMTime( IUnknown* _pFrame, REFERENCE_TIME _rtMaxDiff = -1 )
	{
		M_TIME mTime = {};

		CComQIPtr<IMFFrame> qpMFFrame( _pFrame );
		if( qpMFFrame )
		{
			MF_FRAME_INFO mfInfo = {};
			qpMFFrame->MFAllGet( &mfInfo );

			mTime = CalcMTime( &mfInfo.avProps.vidProps, &mfInfo.mTime, _rtMaxDiff );
			qpMFFrame->MFTimeSet( &mTime );
		}

		return mTime;
	}

};

class MAudioCalc
	: public AutoObjBase<MAudioCalc>
{
	double			m_dblAudioDiff;
	REFERENCE_TIME  m_rtLastTimeEnd;
public:
	MAudioCalc()
	{
		Reset();
	}

	inline int GetNextBytes_Time( M_AUD_PROPS* _pAudProps, M_TIME* _pTime, long* _pnSamples = NULL, REFERENCE_TIME _rtMaxDiff = 10 * DS_MSEC )
	{
		if( IS_BREAK( _pTime ) || (_pTime->rtStartTime < m_rtLastTimeEnd) || (_rtMaxDiff > 0 && (_pTime->rtStartTime - m_rtLastTimeEnd) > _rtMaxDiff) )
			m_rtLastTimeEnd = MINLONGLONG;

		REFERENCE_TIME rtStart = m_rtLastTimeEnd != MINLONGLONG ? m_rtLastTimeEnd : _pTime->rtStartTime;
		REFERENCE_TIME rtEnd = _pTime->rtEndTime > _pTime->rtStartTime + DS_MSEC ? _pTime->rtEndTime : _pTime->rtStartTime + 333667;
		REFERENCE_TIME rtLen = rtEnd - rtStart;

		m_rtLastTimeEnd = MAX( m_rtLastTimeEnd, rtEnd );
		

// 		if( _pTime->rtEndTime > rtStart + 5 * DS_MSEC )
// 		{
// 			rtLen = _pTime->rtEndTime - rtStart;
// 			m_rtLastTimeEnd = _pTime->rtEndTime;
// 		}
// 		else 
// 		{
// 			// End time not set
// 			if( m_rtLastTimeEnd != MINLONGLONG ) //
// 			{
// 				rtLen = _pTime->rtStartTime + 333667 - m_rtLastTimeEnd; // ???
// 				if( rtLen > 0 )
// 					m_rtLastTimeEnd = _pTime->rtStartTime + ;
// 				else
// 					rtLen = 0;
// 
// 				ATLASSERT( rtLen < DS_ONESEC );
// 			}
// 			else
// 			{
// 				// Break and last end not set
// 				m_rtLastTimeEnd = 
// 			}
// 		}
// 		else
// 		{
// 			rtLen = 333667;
// 		}
// 
// 		m_rtLastTimeEnd = _pTime->rtStartTime + rtLen;

		return GetNextBytes( _pAudProps, rtLen > 0 ? rtLen : 0 , _pnSamples, false );
	}

	inline int GetNextBytes_FPS( M_AUD_PROPS* _pAudProps, double _dblFrameRate, long* _pnSamples = NULL, bool _bResetDiff = false )
	{
		if( !_dblFrameRate ) return 0;

		REFERENCE_TIME rtTime  = DBL2INT64(DS_ONESEC / _dblFrameRate);
		double dblAvgTime = CorrectAverageTimeM( rtTime );

		return GetNextBytes( _pAudProps, dblAvgTime, _pnSamples, _bResetDiff );
	}

	inline int GetNextBytes( M_AUD_PROPS* pAudProps, double _dblFrameLenRT, long* _pnSamples = NULL, bool _bResetDiff = false )
	{
		if( _bResetDiff )
			m_dblAudioDiff = 0;

		if( !pAudProps->nSamplesPerSec ) return 0;

		double dblGet = _dblFrameLenRT + m_dblAudioDiff;

		LONGLONG llSamples = (REFERENCE_TIME(dblGet) / DS_ONESEC) * (REFERENCE_TIME)pAudProps->nSamplesPerSec;
		LONGLONG llRest = (REFERENCE_TIME(dblGet) % DS_ONESEC) * (REFERENCE_TIME)pAudProps->nSamplesPerSec / DS_ONESEC;
		long lSamples = (LONG)(llSamples + llRest);

		double dblReal = (double)lSamples * DS_ONESEC / pAudProps->nSamplesPerSec;
		m_dblAudioDiff = dblGet - dblReal;

		if( _pnSamples )
			*_pnSamples = lSamples;

		return MAudSamples2Bytes( pAudProps, lSamples);
	}

	double AddNextBytes( M_AUD_PROPS* pAudProps, int _nBytes )
	{
		return AddNextSamples( pAudProps, MAudBytes2Samples( pAudProps, _nBytes) );
	}

	double AddNextSamples( M_AUD_PROPS* pAudProps, int _nSamples )
	{
		double dblTimeDiff = (double)(_nSamples * DS_ONESEC ) / pAudProps->nSamplesPerSec;
		m_dblAudioDiff += dblTimeDiff;
		return m_dblAudioDiff;
	}

	void Reset()
	{
		m_dblAudioDiff = 0;
		m_rtLastTimeEnd = MINLONGLONG;
	}

	
};



// Class calculate frame time based on frame start & duration
// On the output the result time is monotonic increased and rtEndTime = rtStartTime(next)
class MInputTimeCorrector
{
	CComAutoCriticalSection m_csTime;
	M_TIME			m_timeLast;
	REFERENCE_TIME	m_rtTimeOffset;
public:
	MInputTimeCorrector()
	{
		Reset();
	}

	M_TIME FrameTime( REFERENCE_TIME _rtInputTime, REFERENCE_TIME _rtFrameDuration, int* _pnDropped )
	{
		CAutoCS crs( m_csTime );

		// Correct start time according to last end time
		M_TIME mTime = {};
		REFERENCE_TIME rtStart = m_rtTimeOffset != MINLONGLONG ? _rtInputTime - m_rtTimeOffset : MINLONGLONG;
		if( m_rtTimeOffset == MINLONGLONG || 
			ABS(m_timeLast.rtEndTime - rtStart) + 5 * DS_MSEC >= _rtFrameDuration  )
		{
			SET_BREAK( &mTime, true );
			REFERENCE_TIME rtTimeOffsetNew = _rtInputTime - m_timeLast.rtEndTime;

			int nDropped = 0;
			if( rtStart != MINLONGLONG )
			{
				// Frame dropped 
				nDropped = (ABS(m_timeLast.rtEndTime - rtStart) + 5 * DS_MSEC) / _rtFrameDuration;
			}

			ATLTRACE2( atlTraceGeneral, 8, _T("MInputTimeCorrector(%08X)::FrameTime() - CORRECT START Dropped:%d Last:%s In:%s(%s) Offset:%s->%s\n"), 
				(LONG)this,
				nDropped, RT2STR( m_timeLast.rtEndTime ), RT2STR( rtStart ),
				RT2STR( _rtInputTime ), 
				RT2STR( m_rtTimeOffset ), RT2STR( rtTimeOffsetNew ) );

			m_rtTimeOffset = rtTimeOffsetNew;

			if( _pnDropped )
				*_pnDropped = nDropped;
		}

		mTime.rtStartTime = m_timeLast.rtEndTime;
		mTime.rtEndTime = _rtInputTime - m_rtTimeOffset + _rtFrameDuration;

		m_timeLast = mTime;
		return mTime;
	}

	REFERENCE_TIME TimeOffset() const { return m_rtTimeOffset; }

	void Reset()
	{
		CAutoCS crs( m_csTime );

		ATLTRACE2( atlTraceGeneral, 8, _T("MInputTimeCorrector(%08X)::Reset() - Last:%s Offset:%s\n"), 
			(LONG)this, RT2STR( m_timeLast.rtEndTime), RT2STR( m_rtTimeOffset ) );

		m_rtTimeOffset = MINLONGLONG;
		::ZeroMemory( &m_timeLast, sizeof(m_timeLast) );
	}
};


//////////////////////////////////////////////////////////////////////////
// Obsolete methods 

class CTimeCorrectorObsolete
{
	REFERENCE_TIME	m_rtTimeOffset;
	REFERENCE_TIME	m_rtLastEnd;
public:

	CTimeCorrectorObsolete()
	{
		ResetTime();
	}

	M_TIME AvoidSkips( M_TIME* _pTime, bool _bMin = false )
	{
		ATLASSERT( _pTime );

		M_TIME mTime = *_pTime;
		if( !IS_BREAK(&mTime) )
		{
			mTime.rtStartTime = _bMin ? MIN( m_rtLastEnd, mTime.rtStartTime ) : m_rtLastEnd;
		}

		m_rtLastEnd = mTime.rtEndTime;
		return mTime;
	}

	REFERENCE_TIME CorrectTime( REFERENCE_TIME _rtStart, REFERENCE_TIME& _rtEnd, bool _bNewSegment )
	{
		if( _bNewSegment || m_rtTimeOffset == MINLONGLONG )
		{
			m_rtTimeOffset = _rtStart - m_rtLastEnd;
		}

		_rtStart -= m_rtTimeOffset;
		_rtEnd -= m_rtTimeOffset;

		m_rtLastEnd = _rtEnd;
		return _rtStart;
	}

	void ResetTime( REFERENCE_TIME _rtLast = 0 )
	{
		m_rtTimeOffset = MINLONGLONG;
		m_rtLastEnd = _rtLast;
	}
};

class CRateCorrectorObsolete
	: public CTimeCorrectorObsolete
{
	double	m_dblLastOut;
public:

	struct RCField
	{
		double		dblStart;
		double		dblOutLen;
		double		dblOrgTime;
		int			nFields;
		M_VID_PROPS	vidProps;
		LPBYTE		pbVideo;
	};

public:

	CRateCorrectorObsolete()
	{
		m_dblLastOut = MINLONG;
		ResetTime( MINLONGLONG );
	}

	// Use fields 
	int CalcOutTimes( M_TIME* _pTime, M_VID_PROPS* _pInProp, M_VID_PROPS* _pOutProps, double _dblOutRate, RCField* _pField1, RCField* _pField2 )
	{
		ATLASSERT( _pInProp && _pOutProps );
		
		bool bUseFields = true;//_dblOutRate > 1.5 || IsInterlaced(_pInProp) == 0 || IsInterlaced(_pOutProps) == 0;

		// Correct time
		M_TIME mTime  = AvoidSkips( _pTime );
		double dblOutLen = GetDuration( _pOutProps, bUseFields ) / _dblOutRate;

		if( IS_BREAK(&mTime) )
			m_dblLastOut = MINLONG;//mTime.rtStartTime;
		
		int nInputFields = bUseFields && (_pInProp->eInterlace != eMI_Progressive && _pInProp->eInterlace != eMI_Default) ? 2 : 1;

		double dblInLen = mTime.rtEndTime - mTime.rtStartTime;
		if( dblInLen < 10 )
			dblInLen = GetDuration( _pInProp, bUseFields );
		else if( nInputFields > 1 && bUseFields )
			dblInLen /= 2;

		double dblStartTime = mTime.rtStartTime;
		int nOutFrames = GetRCField( dblStartTime, dblStartTime + dblInLen, dblOutLen, _pField1 );
		if( nInputFields > 1 )
		{
			dblStartTime += dblInLen;
			nOutFrames += GetRCField( dblStartTime, dblStartTime + dblInLen, dblOutLen, _pField2 );
		}
		
		return nOutFrames;
	}

	static double GetDuration( M_VID_PROPS* _pVidProps, bool _bUseFields )
	{
		ATLASSERT( _pVidProps );
		if( !_pVidProps->dblRate )
			return _bUseFields ? 166833.333 : 333666.666;

		double dblLen = DS_ONESEC / _pVidProps->dblRate;
		UpdateMVidProps( _pVidProps );
		if( _bUseFields && _pVidProps->eInterlace != eMI_Progressive && _pVidProps->eInterlace != eMI_Default )
			dblLen /= 2;

		return dblLen;
	}
private:

	int GetRCField( double _dblStartTime, double _dblEndTime, double _dblOutLen, RCField* _pField )
	{
		int nOutIndex = (int)(_dblStartTime / _dblOutLen);
		double dblOutStart = m_dblLastOut;
		if( m_dblLastOut <= MINLONG )
			dblOutStart = nOutIndex * _dblOutLen;

		_pField->nFields = 0;
		while( dblOutStart + _dblOutLen / 2.0 < _dblEndTime )
		{
			if( dblOutStart + _dblOutLen / 2.0 > _dblStartTime )
			{
				if( _pField->nFields == 0 )
				{
					_pField->dblStart = dblOutStart;
					_pField->dblOrgTime = _dblStartTime;
					_pField->dblOutLen = _dblOutLen;
					_pField->pbVideo = NULL;
				}

				_pField->nFields++;
				m_dblLastOut = dblOutStart;
			}

			dblOutStart += _dblOutLen;
		}

		return _pField->nFields;
	}
};

// For prevent processing same frame
class MFrameComparer
{
	CComPtr<IMFrame_I>		m_qpLastFrameIn;
	M_VID_PROPS				m_vidPropsLastOut;

	CComBSTR					m_cbsLastRotate;
	CComBSTR					m_cbsLastCrop;
	CComBSTR					m_cbsLastMirror;

public:

	MFrameComparer()
	{
		Reset();
	}

	void Reset()
	{
		::ZeroMemory( &m_vidPropsLastOut, sizeof(m_vidPropsLastOut) );
		m_cbsLastCrop.Empty();
		m_cbsLastMirror.Empty();
		m_qpLastFrameIn.Release();
	}

	bool IsSameFrameObj( IUnknown* _pFrame )
	{
		return m_qpLastFrameIn ? m_qpLastFrameIn.IsEqualObject( _pFrame ) : false; 
	}

	
	template< class TNodePtr>
	bool IsSameFrame( IUnknown* _pFrame, M_VID_PROPS* _pVidOutput, TNodePtr pProps )
	{
		CComQIPtr<IMFrame_I> qpMFrameNext( _pFrame );
		ATLASSERT( qpMFrameNext );
		if( !qpMFrameNext )
			return false;

		// Check for same video
		bool bSameVideo = false;
		LONGLONG lID_Last = 0;
		LONGLONG lID_In = 0;
		if( m_qpLastFrameIn && (!_pVidOutput || IsSameFormat( &m_vidPropsLastOut, _pVidOutput ) ) )
		{
			m_qpLastFrameIn->GetID( &lID_Last );
			qpMFrameNext->GetID( &lID_In );
			if( lID_In == lID_Last )
			{
				// Check video format (e.g. override)
				M_AV_PROPS avLast = {};
				m_qpLastFrameIn->FrameAVPropsGet( &avLast );
				M_AV_PROPS avNext = {};
				qpMFrameNext->FrameAVPropsGet( &avNext );
				if( IsSameFormat( &avLast.vidProps, &avNext.vidProps) )
				{
					ATLTRACE2( atlTraceGeneral, 8, _T("MFrameComparer(%08X)::IsSameFrame() [FRC-X] - SAME FRAME IN (DUP)\n"), (LONG)this );
					bSameVideo = true;
				}
			}
		}

		if( !bSameVideo )
		{
			m_qpLastFrameIn = qpMFrameNext;
			if( _pVidOutput )
				m_vidPropsLastOut = *_pVidOutput;
			else
				::ZeroMemory( &m_vidPropsLastOut, sizeof(m_vidPropsLastOut) );
		}

		// Check crop & mirrow
		bool bSameProps = true;
		if( pProps )
		{
			CComBSTR cbsCrop = pProps->GetStringAttribute_Safe( L"crop" );
			if( cbsCrop != m_cbsLastCrop )
			{
				m_cbsLastCrop = cbsCrop;
				bSameProps = false;
			}

			CComBSTR cbsMirror = pProps->GetStringAttribute_Safe( L"mirror" );
			if( cbsMirror != m_cbsLastMirror )
			{
				m_cbsLastMirror = cbsMirror;
				bSameProps = false;
			}

			CComBSTR cbsRotate = pProps->GetStringAttribute_Safe( L"rotate" );
			if( cbsRotate != m_cbsLastRotate )
			{
				m_cbsLastRotate = cbsRotate;
				bSameProps = false;
			}
		}

		return bSameVideo && bSameProps;
	}
};



// 
// struct ANCData
// {
// 	static LPBYTE GetDataPtr( M_VID_PROPS* _pVidProps, LPBYTE _pbVideo, DWORD _cbVideo, int* _pcbAnc )
// 	{
// 		if( !_pbVideo || !IsSpecified(_pVidProps) )
// 		{
// 			if( _pcbAux )
// 				*_pcbAux = 0;
// 			return NULL;
// 		}
// 
// 		DWORD cbVideo = Img_SizeM( _pVidProps );
// 		if( _cbVideo <= cbVideo )
// 			return NULL;
// 
// 		if( _pcbAnc )
// 			*_pcbAnc = _cbVideo - cbVideo;
// 
// 		return _pbVideo + cbVideo;
// 	}
// 	static bool PutDataPtr( M_VID_PROPS* _pVidProps, LPBYTE _pbVideo, LPBYTE _pbANC, DWORD _cbANC, DWORD _cbVideo = 0 )
// 	{
// 		ATLASSERT( _pbVideo && IsSpecified(_pVidProps) );
// 		DWORD cbVideo = Img_SizeM( _pVidProps );
// 		if( !_pbANC || !_pbVideo || (_cbVideo > 0 && _cbVideo < cbVideo + _cbANC) )
// 			return false;
// 
// 		if( _pbANC)
// 			SEH_CopyMemory( _pbVideo + cbVideo, _pbANC, _cbANC );
// 
// 		return true;
// 	}
// 	static HRESULT PutANCData( M_VID_PROPS* _pVidProps, LPBYTE _pbVideo, DWORD _cbVideo, DWORD _dwDataFCC, void* _pvANC, DWORD _cbANC )
// 	{
// 		int cbAuxMax = 0;
// 		DWORD* pdwAux = (DWORD*)GetDataPtr( _pVidProps, _pbVideo, _cbVideo, &cbAuxMax );
// 		if( !pdwAux ) return E_OUTOFMEMORY;
// 
// 		while( *pdwAux != 0 && cbAuxMax > 9 )
// 		{
// 			DWORD dwFCC = pdwAux[0];
// 			DWORD cbSize = pdwAux[1];
// 			if( cbSize + 8 > (DWORD)cbAuxMax )
// 				break;
// 
// 			if( dwFCC == _dwDataFCC )
// 			{
// 				ATLASSERT( cbAuxMax > 8 + (int)cbSize );
// 				// Remove data
// 				::memmove( pdwAux, (LPBYTE)pdwAux + 8 + cbSize, cbAuxMax - 8 - cbSize );
// 				::ZeroMemory( (LPBYTE)pdwAux + cbAuxMax - 8 - cbSize, 8 + cbSize );
// 				continue;
// 			}
// 			pdwAux = (DWORD*)((LPBYTE)pdwAux + 8 + cbSize);
// 			cbAuxMax -= 8 + cbSize;
// 		}
// 
// 		if( _cbANC && _pvANC )
// 		{
// 			if( cbAuxMax < 8 + (int)_cbANC )
// 				return E_OUTOFMEMORY;
// 
// 			*pdwAux++ = _dwDataFCC;
// 			*pdwAux++ = _cbANC;
// 
// 			::SEH_CopyMemory( pdwAux, _pvANC, _cbANC );
// 		}
// 
// 		return S_OK;
// 	}
// 
// 	static void* GetANCData( M_VID_PROPS* _pVidProps, LPBYTE _pbVideo, DWORD _cbVideo, DWORD _dwDataFCC_or_Index, DWORD* _pcbANC )
// 	{
// 		int cbAuxMax = 0;
// 		DWORD* pdwAux = (DWORD*)GetDataPtr( _pVidProps, _pbVideo, _cbVideo, &cbAuxMax );
// 		if( !pdwAux ) return NULL;
// 
// 		int nIndex = 0;
// 		while( *pdwAux != 0 && cbAuxMax > 9 )
// 		{
// 			DWORD dwFCC = pdwAux[0];
// 			DWORD cbSize = pdwAux[1];
// 			if( cbSize + 8 > (DWORD)cbAuxMax )
// 				break;
// 
// 			if( (_dwDataFCC_or_Index > 0xFF && dwFCC == _dwDataFCC_or_Index) || 
// 				(_dwDataFCC_or_Index <= 0xFF && nIndex == _dwDataFCC_or_Index ) )
// 			{
// 				if( _pcbANC )
// 					*_pcbANC = cbSize;
// 				return pdwAux + 2;
// 			}
// 
// 			pdwAux = (DWORD*)((LPBYTE)pdwAux + 8 + cbSize);
// 			cbAuxMax -= 8 + cbSize;
// 			nIndex++;
// 		}
// 
// 		return NULL;
// 	}
// 
// 	class MDeqANC
// 	{
// 		CComAutoCriticalSection m_csDeq;
// 		typedef deque<BYTEBLOB> TDeqANC;
// 		TDeqANC			m_deqANCData;
// 		int				m_cbANCMax;
// 
// 	public:
// 		MDeqANC()
// 			: m_cbANCMax( 0 )
// 		{
// 		}
// 
// 		~MDeqANC()
// 		{
// 			ResetANC();
// 		}
// 
// 		int GetMaxDataSize()
// 		{
// 			return m_cbANCMax;
// 		}
// 
// 		HRESULT PutANC( IMFrame_I* _pFrame )
// 		{
// 			ATLASSERT( _pFrame );
// 			if( !_pFrame )
// 				return E_POINTER;
// 
// 			M_AV_PROPS avProps = {};
// 			DWORD cbVideo = 0;
// 			LPBYTE pbVideo = NULL;
// 			_pFrame->GetFramePropsAll( NULL, &avProps, NULL, NULL, &cbVideo, &pbVideo );
// 		
// 			int cbANC = 0;
// 			LPBYTE pbDataANC = ANCData::GetDataPtr( &avProps.vidProps, pbVideo, cbVideo, &cbANC );
// 
// 			if( pbDataANC && ANCData::GetANCData( &avProps.vidProps, pbVideo, cbVideo, 0, NULL ) )
// 			{
// 				BYTEBLOB blobData = {};
// 				blobData.pbData = new BYTE[cbANC];
// 				blobData.cbData = cbANC;
// 				::CopyMemory( blobData.pbData, pbDataANC, cbANC );
// 				
// 				CAutoCS crs(m_csDeq);
// 
// 				m_cbANCMax = MAX( cbANC, m_cbANCMax );
// 				m_deqANCData.push_back( blobData );
// 
// 				return S_OK;
// 			}
// 
// 			return S_FALSE;
// 		}
// 
// 		HRESULT FillANC( IMFrame_I* _pFrame )
// 		{
// 			ATLASSERT( _pFrame );
// 			if( !_pFrame )
// 				return E_POINTER;
// 
// 			BYTEBLOB blobData = {};
// 			{
// 				CAutoCS crs( m_csDeq );
// 
// 				if( !m_deqANCData.size() )
// 					return S_FALSE;
// 
// 				blobData = m_deqANCData.front();
// 				m_deqANCData.pop_front();
// 			}
// 
// 			M_AV_PROPS avProps = {};
// 			DWORD cbVideo = 0;
// 			LPBYTE pbVideo = NULL;
// 			_pFrame->GetFramePropsAll( NULL, &avProps, NULL, NULL, &cbVideo, &pbVideo );
// 
// 			int cbANC = 0;
// 			LPBYTE pbDataANC = ANCData::GetDataPtr( &avProps.vidProps, pbVideo, cbVideo, &cbANC );
// 
// 			ATLASSERT( cbANC >= blobData.cbData );
// 			if( cbANC >= blobData.cbData )
// 			{
// 				::CopyMemory( pbDataANC, blobData.pbData, blobData.cbData );
// 			}
// 
// 			delete [] blobData.pbData;
// 
// 			return cbANC >= blobData.cbData ? S_OK : HRESULT_FROM_WIN32( ERROR_OUTOFMEMORY );
// 		}
// 
// 		void ResetANC()
// 		{
// 			CAutoCS crs(m_csDeq);
// 
// 			TDeqANC::iterator Iter = m_deqANCData.begin();
// 			while( Iter != m_deqANCData.end() )
// 			{
// 				delete [] Iter->pbData;
// 				Iter++;
// 			}
// 			m_deqANCData.clear();
// 			m_cbANCMax = 0;
// 		}
// 	};
// };




#define CHUNK_ID( pData )  (*(DWORD*)(pbChunkStart))
#define CHUNK_SIZE( pData )  (*((DWORD*)(pbChunkStart) + 1))

class CWAVFile
{
	CStdHandle		m_shFile;

public:
	M_AUD_PROPS		m_audProps;
	DWORD			m_dwDataSize;
	DWORD			m_dwDataOffset;
	CComBSTR			m_cbsFileName;

	template <class T>
	struct WAVHeader
	{
		DWORD dwRIFF; // 'RIFF'
		DWORD cbRIFF; //   
		DWORD dwWAVE;	// 'WAVE'
		DWORD dwChunkFMT;	// 'fmt '
		DWORD cbChunkFMT;		
		T waveFormat;
		DWORD dwChunkData;	// 'data'
		DWORD cbChunkData;

		WAVHeader( DWORD _cbAudioSize, void* _pWaveFormat )
		{
			::ZeroMemory( this,sizeof(WAVHeader) );
			dwRIFF = FCC('RIFF');
			cbRIFF = sizeof(WAVHeader) - 8 + _cbAudioSize;
			dwWAVE = FCC('WAVE');
			if( _pWaveFormat )
				::CopyMemory( &waveFormat, _pWaveFormat, sizeof(waveFormat) );

			dwChunkFMT = FCC('fmt ');
			cbChunkFMT = sizeof(T);
			dwChunkData = FCC('data');
			cbChunkData = _cbAudioSize;
		}

		bool IsValid(int _nMaxSize )
		{
			if( dwRIFF == FCC('RIFF') &&
				dwWAVE == FCC('WAVE') &&
				dwChunkFMT == FCC('fmt ') && 
				DataPtr( _nMaxSize ) != NULL )
			{
				return true;
			}

			return false;
		}

		LPBYTE DataChunk( int _nMaxSize )
		{
			LPBYTE pbChunkStart = (LPBYTE)&waveFormat + cbChunkFMT;
			while( pbChunkStart - (LPBYTE)this + 8 < _nMaxSize )
			{
				if( CHUNK_ID(pbChunkStart) == FCC('data') )
					return pbChunkStart;

				pbChunkStart += CHUNK_SIZE(pbChunkStart); // chunk size
			}

			return NULL;
		}

		DWORD DataSize( int _nMaxSize )
		{
			LPBYTE pbDataChunk = DataChunk( _nMaxSize );
			if( !pbDataChunk )
				return 0;

			return *(DWORD*)(pbDataChunk + 4);
		}

		LPBYTE DataPtr( int _nMaxSize, int* _pnDataAvalable = NULL )
		{
			LPBYTE pbDataChunk = DataChunk( _nMaxSize );
			if( !pbDataChunk )
				return NULL;

			if( _pnDataAvalable )
				*_pnDataAvalable = (pbDataChunk - (LPBYTE)this) - 8;

			return pbDataChunk + 8;
		}

		DWORD DataOffset( int _nMaxSize, DWORD _dwDataPos )
		{
			LPBYTE pbData = DataPtr( _nMaxSize );
			if( !pbData )
				return NULL;

			return (pbData + _dwDataPos) - (LPBYTE)this;
		}
	};


public:

	CWAVFile()
		: m_dwDataOffset(0),
		  m_dwDataSize(0)
	{
		::ZeroMemory( &m_audProps, sizeof(m_audProps) );
	}

	~CWAVFile()
	{
		CloseFile();
	}

	HRESULT WriteToFile( LPCWSTR _pszFileName, IUnknown* _pFrame, int _nAppendType = 0 )
	{
		if( !_pFrame )
		{
			if( m_shFile )
				::FlushFileBuffers( m_shFile );
			
			return S_FALSE;
		}

		MF_FRAME_INFO mfInfo = MFFrame_Info(_pFrame);
		if( !IsSpecified(&mfInfo.avProps.audProps) || mfInfo.cbAudio <= 0 || !mfInfo.lpAudio )
			return S_FALSE;

		return WriteToFile( _pszFileName, (LPBYTE)mfInfo.lpAudio, mfInfo.cbAudio, &mfInfo.avProps.audProps, _nAppendType );
	}

	void CloseFile()
	{
		m_shFile.Close();
		m_cbsFileName.Empty();
		m_dwDataOffset = 0;
		m_dwDataSize = 0;
		::ZeroMemory( &m_audProps, sizeof(m_audProps) );
	}

	HRESULT OpenFile( LPCWSTR _pszFileName, M_AUD_PROPS* _pAudProps, DWORD* _pdwDataSize )
	{
		if( !_pszFileName )
			return E_INVALIDARG;

		HANDLE hFile = ::CreateFileW( _pszFileName, 
			GENERIC_WRITE|GENERIC_READ, 
			FILE_SHARE_READ, 
			NULL,
			OPEN_EXISTING,
			0,
			NULL );

		DWORD dwErr = GetLastError();

		if( !hFile || hFile == INVALID_HANDLE_VALUE )
			return HRESULT_FROM_WIN32( dwErr );

		DWORD dwDataSize = 0;
		DWORD dwDataOffset = 0;
		M_AUD_PROPS audProps = {};
		HRESULT hr = ReadHeader( hFile, &audProps, &dwDataSize, &dwDataOffset );
		if( FAILED( hr ) )
			return hr;

		m_audProps = *_pAudProps;
		m_dwDataOffset = dwDataOffset;
		m_dwDataSize = dwDataSize;
		m_shFile.Attach( hFile );

		if( _pAudProps )
			*_pAudProps = m_audProps;

		if( _pdwDataSize )
			*_pdwDataSize = dwDataSize;

		return S_OK;
	}

	HRESULT ReadFromFile( IN LPCWSTR _pszFileName, IN DWORD _dwOffset, IN LPBYTE _pbAudio, IN OUT DWORD* _pcbAudio, OUT M_AUD_PROPS* _pAudProps )
	{
		ATLASSERT( _pcbAudio && _pbAudio );

		if( !m_shFile || (_pszFileName && m_cbsFileName != _pszFileName) )
		{
			HRESULT hr = OpenFile( _pszFileName, NULL, NULL );
			if( FAILED( hr ) )
				return hr;
		}

		DWORD dwPos = m_dwDataOffset + _dwOffset;
		DWORD dwRes = ::SetFilePointer( m_shFile, dwPos, NULL, FILE_BEGIN );
		if( dwRes == INVALID_SET_FILE_POINTER )
		{
			DWORD dwErr = ::GetLastError();
			ATLTRACE2( atlTraceGeneral, 2, _T("###Err### CCoWavWriter::ReadFromFile - ")
				_T("SetFilePointer() FAILED dwErr = %d\n"), dwErr );

			return HRESULT_FROM_WIN32( dwErr );
		}

		DWORD cbRead = 0;
		BOOL bRes = ::ReadFile( m_shFile, _pbAudio, *_pcbAudio, &cbRead, NULL );
		if( !bRes )
		{
			DWORD dwErr = ::GetLastError();
			ATLTRACE2( atlTraceGeneral, 2, _T("###Err### CCoWavWriter::ReadFromFile - ")
				_T("ReadFile() FAILED dwErr = %d\n"), dwErr );

			return HRESULT_FROM_WIN32( dwErr );
		}

		*_pcbAudio = cbRead;
		return S_OK;
	}

	// -1 -> Create new (reset even for same name)
	//  0 -> Create new only for different name (not append)
	//  1 -> Try to append file (if format is the same)
	//  2 -> Write even if format is not same
	HRESULT WriteToFile( LPCWSTR _pszFileName, LPBYTE _pbAudio, DWORD _cbAudio, M_AUD_PROPS* _pAudProps, int _nAppendType )
	{
		if( _nAppendType < 0 || !m_shFile || (_pszFileName && m_cbsFileName != _pszFileName) )
		{
			HANDLE hFile = ::CreateFileW( _pszFileName, 
				GENERIC_WRITE|GENERIC_READ, 
				FILE_SHARE_READ, 
				NULL,
				_nAppendType <= 0 ? CREATE_ALWAYS : OPEN_ALWAYS,
				0,
				NULL );

			DWORD dwErr = GetLastError();

			if( !hFile || hFile == INVALID_HANDLE_VALUE )
				return HRESULT_FROM_WIN32( dwErr );

			m_shFile.Attach( hFile );
			if( _nAppendType > 0 && dwErr == ERROR_ALREADY_EXISTS )
			{
				// Check file
				HRESULT hr = ReadHeader( m_shFile, &m_audProps, &m_dwDataSize, &m_dwDataOffset );
				if( FAILED( hr ) || !(IsSameFormat( _pAudProps, &m_audProps ) && _nAppendType < 2 ) )
				{
					::SetFilePointer(hFile, 0 , NULL, FILE_BEGIN );
					::SetEndOfFile(hFile);
					::ZeroMemory( &m_audProps, sizeof(m_audProps) );
				}
				else
				{
					::SetFilePointer( hFile, 0, NULL, FILE_END );
				}
			}
			
			if( !IsSpecified(&m_audProps) )
			{
				m_audProps = *_pAudProps;
				WriteHeader( m_shFile, &m_audProps, 0, &m_dwDataOffset );
			}
			
			m_cbsFileName = _pszFileName;
		}
		else
		{
			::SetFilePointer( m_shFile, 0, NULL, FILE_END );
		}
		
		DWORD dwWritten = 0;
		BOOL bRes = ::WriteFile( m_shFile, _pbAudio, _cbAudio, &dwWritten, NULL );
		if( !bRes )
			return HRESULT_FROM_WIN32( GetLastError() );

		UpdateSize();

		return S_OK;
	}

	void UpdateSize( DWORD _dwDataSize = 0 )
	{
		if( _dwDataSize == 0 )
		{
			_dwDataSize = ::SetFilePointer( m_shFile, 0, NULL, FILE_CURRENT ) - m_dwDataOffset;
		}

		m_dwDataSize = _dwDataSize;

		WriteHeader( m_shFile, &m_audProps, _dwDataSize, NULL );
	}

	static HRESULT ReadHeader( HANDLE _hFile, M_AUD_PROPS* _pAudProps, DWORD* _pdwDataSize, DWORD* _pdwDataOffset )
	{
		if( !_hFile )
			return E_UNEXPECTED;

		DWORD dwRes = ::SetFilePointer( _hFile, 0 , NULL, FILE_BEGIN );
		if( dwRes == INVALID_SET_FILE_POINTER )
		{
			DWORD dwErr = ::GetLastError();
			ATLTRACE2( atlTraceGeneral, 2, _T("###Err### CCoWavWriter::WriteHeader - ")
				_T("SetFilePointer() FAILED dwErr = %d\n"), dwErr );

			return HRESULT_FROM_WIN32( dwErr );
		}

		DWORD cbRead = 0;
		CBlobHeap blobData;
		blobData.AllocMem( 1024 );
		::ReadFile( _hFile, blobData.Ptr(), blobData.GetAllocSize(), &cbRead, NULL );
		
		WAVHeader<WAVEFORMATEXTENSIBLE>* pWavHeader = (WAVHeader<WAVEFORMATEXTENSIBLE>*)blobData.Ptr();
		if( !pWavHeader->IsValid( cbRead) )
			return E_INVALIDARG;

		ATLASSERT( _pAudProps );
		*_pAudProps = MAudPropsFromWF( (WAVEFORMATEX*)&pWavHeader->waveFormat );
		
		if( _pdwDataOffset )
			*_pdwDataOffset = pWavHeader->DataOffset(cbRead, 0 );

		if( _pdwDataSize )
			*_pdwDataSize = pWavHeader->DataSize(cbRead);

		return S_OK;
	}

	static HRESULT WriteHeader( HANDLE _hFile, M_AUD_PROPS* _pAudProps, DWORD _cbAudioBytes, DWORD* _pdwDataOffset )
	{
		if( !_hFile )
			return E_UNEXPECTED;
		
		DWORD dwRes = SetFilePointer( _hFile, 0 , NULL, FILE_BEGIN );
		if( dwRes == INVALID_SET_FILE_POINTER )
		{
			DWORD dwErr = ::GetLastError();
			ATLTRACE2( atlTraceGeneral, 2, _T("###Err### CCoWavWriter::WriteHeader - ")
				_T("SetFilePointer() FAILED dwErr = %d\n"), dwErr );

			return HRESULT_FROM_WIN32( dwErr );
		}
		
		WAVEFORMATEXTENSIBLE wfx = MAudProps2WFX(_pAudProps);
		wfx.Format.wFormatTag = _pAudProps->nBitsPerSample > 0 ? WAVE_FORMAT_PCM : WAVE_FORMAT_IEEE_FLOAT;
	
		if( _pAudProps->nChannels <= 2 && ABS(_pAudProps->nBitsPerSample) <= 16 )
		{
			WAVHeader<PCMWAVEFORMAT> wavHeader( _cbAudioBytes, &wfx );

			DWORD cbWritten = 0;
			BOOL bRes = ::WriteFile( _hFile, &wavHeader, sizeof( wavHeader ), &cbWritten, NULL );
			if( !bRes || cbWritten != sizeof( wavHeader ) ) return HRESULT_FROM_WIN32( GetLastError() );

			if( _pdwDataOffset )
				*_pdwDataOffset = sizeof( wavHeader );
		}
		else
		{
			WAVHeader<WAVEFORMATEXTENSIBLE> wavHeader( _cbAudioBytes, &wfx );

			DWORD cbWritten = 0;
			BOOL bRes = ::WriteFile( _hFile, &wavHeader, sizeof( wavHeader ), &cbWritten, NULL );
			if( !bRes || cbWritten != sizeof( wavHeader ) ) return HRESULT_FROM_WIN32( GetLastError() );

			if( _pdwDataOffset )
				*_pdwDataOffset = sizeof( wavHeader );
		}

		return S_OK;
	}
};

class CTracer
	: public CComAutoCriticalSection
{
public:
	LPCWSTR m_pLast;

	CTracer()
	{
		m_pLast = NULL;
	}

	void ResetLast()
	{
		ATLTRACE2( atlTraceGeneral, 8, _T("CAutoTracer(%ws) S:%08X Owner:%08X RESET LAST\n"), m_pLast, 
			(DWORD)&m_sec, (DWORD)m_sec.OwningThread );

		m_pLast = NULL;
	}
};

class CAutoTracer
{
	CTracer& m_tracer;
	CAutoCS* m_pAutoCS;
public:
	CAutoTracer( CTracer& _trace, LPCWSTR _pszComment )
		: m_tracer( _trace )
	{
		ATLTRACE2( atlTraceGeneral, 8, _T("CAutoTracer(%ws) pThis:%08X CS:%08X Owner:%08X ENTER\n"), _pszComment, 
			this, (DWORD)&_trace.m_sec, (DWORD)_trace.m_sec.OwningThread );

		m_pAutoCS = new CAutoCS( _trace );

		ATLASSERT( m_tracer.m_pLast == NULL );
		m_tracer.m_pLast = _pszComment;

		ATLTRACE2( atlTraceGeneral, 8, _T("CAutoTracer(%ws) pThis:%08X CS:%08X Owner:%08X AT_LOCKED\n"), _pszComment, 
			this, (DWORD)&_trace.m_sec, (DWORD)_trace.m_sec.OwningThread );
	}
	~CAutoTracer()
	{
		ATLTRACE2( atlTraceGeneral, 8, _T("CAutoTracer(%ws) pThis:%08X CS:%08X AT_UNLOCKED\n"), m_tracer.m_pLast, 
			this, (DWORD)&m_tracer.m_sec );

		m_tracer.m_pLast = NULL;

		delete m_pAutoCS;
	}
};

#ifndef TRACELEVEL_DeqActive
#define TRACELEVEL_DeqActive (8)
#endif

#ifdef NEW_XML_PARSE

struct DEQ_ITEM_MP
{
	CComPtr<IMFrame_I> cpMFrame;
	CComBSTR cbsChannel;
	CComPtr<IUnknown> cpSender;

	DEQ_ITEM_MP() {}

	DEQ_ITEM_MP( BSTR _bsChannelID,					
		IUnknown* _pSender,				
		IMFrame_I* _pFrameIn )
		: cbsChannel( _bsChannelID ),
		  cpMFrame( _pFrameIn ),
		  cpSender(_pSender)
	{
	}
};

template <class TObject>
class MDeqObjects
	: public AutoObjBase<MDeqObjects<TObject>>
{
	CComAutoCriticalSection	m_csDeq;
	typedef std::deque<TObject>	TObjectDeq;
	TObjectDeq				m_deqObjects;

	CStdHandle		m_shFlush;
	CStdHandle		m_shCanPut;
	CStdHandle		m_shCanGet;
	int				m_nMaxDeq;
	int				m_nDropped;
public:

	MDeqObjects( int _nMaxDeq = 0 )
		: m_nMaxDeq(_nMaxDeq)
	{
		m_shCanPut.Attach(::CreateEvent(NULL, TRUE, TRUE, FALSE));
		m_shCanGet.Attach(::CreateEvent(NULL, TRUE, FALSE, FALSE));
		m_shFlush.Attach(::CreateEvent(NULL, TRUE, FALSE, FALSE));
	}

	~MDeqObjects()
	{
		DeqFlush(true);
	}

	void DeqMaxSet(int _nMaxDeq)
	{
		CAutoCS crs(m_csDeq);

		m_nMaxDeq = _nMaxDeq;
		if (m_nMaxDeq <= 0 || (int)m_deqObjects.size() < m_nMaxDeq)
			::SetEvent(m_shCanPut);
		else if (m_nMaxDeq > 0 || (int)m_deqObjects.size() >= m_nMaxDeq)
			::ResetEvent(m_shCanPut);
	}

	int DeqMax() const { return m_nMaxDeq; }

	int DeqDropped() const { return m_nDropped; }
	
	int DeqSize(int* _pnMax = NULL, int* _pnDropped = NULL) 
	{ 
		CAutoCS crs(m_csDeq);

		if (_pnMax)
			*_pnMax = m_nMaxDeq;

		if (_pnDropped)
			*_pnDropped = m_nDropped;

		return (int)m_deqObjects.size();
	}

	HANDLE HandleCanGet() { return m_shCanGet; }

	HANDLE HandleCanPut() { return m_shCanPut; }

	bool DeqPut(const TObject& _rObject, int _nMaxWaitMsec, bool _bForceNew)
	{
		::ResetEvent(m_shFlush);

		if (m_nMaxDeq > 0)
		{
			HANDLE phWait[2] = { m_shCanPut, m_shFlush };
			DWORD dwRes = ::WaitForMultipleObjects(2, phWait, FALSE, _nMaxWaitMsec >= 0 ? (DWORD)_nMaxWaitMsec : INFINITE );
			if (dwRes != WAIT_OBJECT_0)
				return false;
		}

		CAutoCS crs(m_csDeq);

		if (m_nMaxDeq > 0 && (int)m_deqObjects.size() >= m_nMaxDeq)
		{
			if (!_bForceNew)
				return false;

			m_deqObjects.pop_front();
			++m_nDropped;
		}

		m_deqObjects.push_back(_rObject);
		::SetEvent(m_shCanGet);

		if (m_nMaxDeq > 0 && (int)m_deqObjects.size() >= m_nMaxDeq)
		{
			::ResetEvent(m_shCanPut);
		}

		return true;
	}

	TObject DeqGet(int _nMaxWaitMsec)
	{
		int nMaxWaitMSec = _nMaxWaitMsec;
		TObject objResult;
		while (true)
		{
			{
				CAutoCS crs(m_csDeq);

				if (m_deqObjects.size())
				{
					objResult = m_deqObjects.front();
					m_deqObjects.pop_front();

					::SetEvent(m_shCanPut);

					if (!m_deqObjects.size())
						::ResetEvent(m_shCanGet);

					break;
				}
			}

			if (nMaxWaitMSec == 0)
				break;

			HANDLE phWait[2] = { m_shCanGet, m_shFlush };
			DWORD dwRes = ::WaitForMultipleObjects(2, phWait, FALSE, nMaxWaitMSec >= 0 ? (DWORD)nMaxWaitMSec : INFINITE );
			if( dwRes != WAIT_OBJECT_0 )
				break;

			// Wait only one time
			nMaxWaitMSec = 0;
		}

		return objResult;
	}

	void DeqFlush(bool _bFlushDropped)
	{
		CAutoCS crs(m_csDeq);

		m_deqObjects.clear();

		::ResetEvent(m_shCanGet);
		::SetEvent(m_shCanPut);
		::SetEvent(m_shFlush);

		if (_bFlushDropped)
			m_nDropped = 0;
	}
};

template <class T, class TObject>
class CDeqActive
{
	CSimpleThread<CDeqActive< T, TObject > >	m_theThread;
	CStdHandle									m_shEventFree;
	int											m_nMaxFrames;
	int											m_nMinFrames;
	bool										m_bForceNew;
		
	CStdHandle									m_shTimer;
	XMLParse::XMLNodePtr						m_pXMLStat;
	int											m_nThreadPriority;

protected:
	CComAutoCriticalSection						m_csDeq;
	typedef deque<TObject>						TObjectDeq;
	TObjectDeq									m_deqObjects;
public:

	CDeqActive()
		: m_nMaxFrames( 0 ),
		  m_nMinFrames( 0 ),
		  m_bForceNew( true ),
		  m_nThreadPriority( THREAD_PRIORITY_NORMAL )
	{
		m_shEventFree.Attach( ::CreateEvent(NULL, TRUE, TRUE, NULL) );
	}

	~CDeqActive()
	{
		DeqFree(true);

		m_shEventFree.Close();
	}

	void DeqSetStatisticsXML( XMLParse::XMLNodePtr _pXMLStat )
	{
		CAutoCS crs(m_csDeq);

		m_pXMLStat = _pXMLStat;
	}

	
	// Note: return E_ABORT -> claer the deque
	//		 return VFW_E_ENUM_OUT_OF_SYNC -> repeat with latest object
	HRESULT OnActiveDeq( const TObject& _object, int _nDeqSize )
	{
		METHOD_MUST_BE_OVERRIDEN( OnActiveDeq ); 
	}

	// Here could be overrided how packets taked from deque
	bool OnDeqReady(TObjectDeq& _deqInput, TObject& _objectReady )
	{
		return false;
	}

	HRESULT OnFrameDrop( const TObject& _object )
	{
		return S_OK;
	}

	int DeqMaxFrames() const { return m_nMaxFrames; }

	int DeqMinFrames() const { return m_nMinFrames; }

	void DeqSetPriority( int _nPriority )
	{
		m_nThreadPriority = _nPriority;
	}

	int DeqGetPriority() { return m_nThreadPriority; }

	void DeqSetMinMax( int _nMin, int _nMax, bool _bForceNew = false )
	{
		CAutoCS crs(m_csDeq);

		m_nMinFrames = MIN( _nMin, _nMax );
		m_nMaxFrames = MAX( _nMin, _nMax );
		m_bForceNew = _bForceNew;

		T* pT = static_cast<T*>(this);

		if( pT->DeqSize() < m_nMaxFrames && m_shEventFree )
		{
			ATLTRACE2( atlTraceGeneral, TRACELEVEL_DeqActive, _T("CDeqActive(%08X:%04X)::DeqSetMinMax() Deq:%d/%d Min:%d Max:%d) SET FREE\n"), 
				this, GetCurrentThreadId(), m_deqObjects.size(), pT->DeqSize(), m_nMinFrames, m_nMaxFrames );

			::SetEvent( m_shEventFree );
		}

		if( m_pXMLStat )
		{
			m_pXMLStat->SetAttribute( L"buffer.max", _nMax );
			m_pXMLStat->SetAttribute( L"buffer.min", _nMin );
			m_pXMLStat->SetAttribute( L"buffer.current", -1, false, true );
			m_pXMLStat->SetAttribute( L"buffer.overflow", 0, false, true );
		}
	}

	void DeqSetMax( int _nMax, bool _bForceNew = false )
	{
		DeqSetMinMax( 0, _nMax, _bForceNew );
	}

	int DeqPut( const TObject& _object, DWORD _dwWaitMS = INFINITE )
	{
		return DeqPutRT( _object, _dwWaitMS != INFINITE ? _dwWaitMS * DS_MSEC : -1);
	}

	int DeqPutRT( const TObject& _object, REFERENCE_TIME _rtWait )
	{
		if( !m_theThread.IsRunning() )
			m_theThread.Start( this );

		T* pT = static_cast<T*>(this);

		DWORD dwRes = WAIT_TIMEOUT;
		if(_rtWait < 0 )
		{
			dwRes = ::WaitForSingleObject( m_shEventFree, INFINITE );
		}
		else if(_rtWait > 0 )
		{
			if( !m_shTimer )
				m_shTimer.Attach( ::CreateWaitableTimer( NULL, TRUE, NULL ) );

			_rtWait *= -1; // For timer

			HANDLE arrWait[2] = { m_shEventFree, m_shTimer };
			::SetWaitableTimer( m_shTimer, (LARGE_INTEGER*)&_rtWait, 0, NULL, NULL, FALSE );

			ATLTRACE2( atlTraceGeneral, TRACELEVEL_DeqActive, _T("CDeqActive(%08X:%04X)::DeqPut() (%d(%d)/%d/%d) WAIT FREE\n"), 
				this, GetCurrentThreadId(), m_deqObjects.size(), pT->DeqSize(), m_nMinFrames, m_nMaxFrames );
			dwRes = ::WaitForMultipleObjects( 2, arrWait, FALSE, (ABS(_rtWait) / DS_MSEC) * 2 + 10 );
		}
		
		CAutoCS crs(m_csDeq);

		if( !m_theThread.m_bWork || !m_theThread.IsRunning() )
			return -2;

		if( m_nMaxFrames > 0 && pT->DeqSize() >= m_nMaxFrames )
		{
			if( m_pXMLStat )
				m_pXMLStat->IncrementAttribute( L"buffer.overflow", (int)1, true );

			if( m_bForceNew )
			{
				ATLTRACE2( atlTraceGeneral, 8, _T("WARNING !!! CDeqActive(%08X:%04X)::DeqPut() POP FRONT (Exeed Max:%d(%d)/%d/%d) dwRes=%d\n"), 
					this, GetCurrentThreadId(), m_deqObjects.size(), pT->DeqSize(), m_nMinFrames, m_nMaxFrames, dwRes );

				m_deqObjects.pop_front();
			}
			else
			{
				ATLTRACE2( atlTraceGeneral, 8, _T("WARNING !!! CDeqActive(%08X:%04X)::DeqPut() SKIP FRAME (Exeed Max:%d(%d)/%d/%d) dwRes=%d\n"), 
					this, GetCurrentThreadId(), m_deqObjects.size(), pT->DeqSize(), m_nMinFrames, m_nMaxFrames, dwRes );

				pT->OnFrameDrop( _object );

				return -1;
			}
		}

		m_deqObjects.push_back( _object );

		if( pT->DeqSize() >= m_nMinFrames )
		{
			ATLTRACE2( atlTraceGeneral, TRACELEVEL_DeqActive, _T("CDeqActive(%08X:%04X)::DeqPut() PUT FRAME (%d(%d)/%d/%d) AWAKE\n"), 
				this, GetCurrentThreadId(), m_deqObjects.size(), pT->DeqSize(), m_nMinFrames, m_nMaxFrames );

			m_theThread.Awake();
		}
		else
		{
			ATLTRACE2( atlTraceGeneral, TRACELEVEL_DeqActive, _T("CDeqActive(%08X:%04X)::DeqPut() PUT FRAME (%d(%d)/%d/%d) WAIT MIN\n"), 
				this, GetCurrentThreadId(), m_deqObjects.size(), pT->DeqSize(), m_nMinFrames, m_nMaxFrames );
		}

		if( m_nMaxFrames > 0 && pT->DeqSize() >= m_nMaxFrames )
		{
			ATLTRACE2( atlTraceGeneral, TRACELEVEL_DeqActive, _T("CDeqActive(%08X:%04X)::DeqPut() (%d(%d)/%d/%d) RESET FREE\n"), 
				this, GetCurrentThreadId(), m_deqObjects.size(), pT->DeqSize(), m_nMinFrames, m_nMaxFrames );

			::ResetEvent( m_shEventFree );
		}


		if( m_pXMLStat )
			m_pXMLStat->SetAttribute( L"buffer.current", (int)m_deqObjects.size() );

		return (int)m_deqObjects.size();
	}

	// Could be overridden in top class - for custom size calculation (e.g. for calculate only video packets)
	int DeqSize( int* _pnMax = NULL )
	{
		CAutoCS crs(m_csDeq);

		if( _pnMax )
			*_pnMax = m_nMaxFrames;

		return (int)m_deqObjects.size();
	}

	void DeqWait( int _nSleep = 1 )
	{
		while( true )
		{
			{
				CAutoCS crs(m_csDeq);

				if( m_deqObjects.empty() )
					break;
			}

			Sleep( MAX(_nSleep,1) );
		}
	}

	bool DeqFree( bool _bStopThread = false, DWORD _dwMaxWait = 3000 )
	{
		bool bFlushFrames = false;

		{
			CAutoCS crs(m_csDeq);

			bFlushFrames = !m_deqObjects.empty();

			m_deqObjects.clear();

			if( _bStopThread )
			{
				m_theThread.m_bWork = false;
				m_theThread.Awake();
			}

			::SetEvent( m_shEventFree );

			if( m_pXMLStat )
				m_pXMLStat->SetAttribute( L"buffer.current", (int)m_deqObjects.size() );
		}

		if( _bStopThread )
		{
#ifdef _DEBUG
			m_theThread.Stop(INFINITE, true);
#else
			m_theThread.Stop(_dwMaxWait, true);
#endif
		}

		return bFlushFrames;
	}

	HRESULT RunThread()
	{
		HRESULT hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
		ATLASSERT(hr == S_OK);

		T* pT = static_cast<T*>(this);

		
		int nThreadPriority = m_nThreadPriority;
		::SetThreadPriority( GetCurrentThread(), nThreadPriority );
		if (m_pXMLStat)
			m_pXMLStat->SetAttribute(L"deq.thread_priority", nThreadPriority);

		while(m_theThread.m_bWork)
		{
			if( m_nThreadPriority != nThreadPriority )
			{
				::SetThreadPriority( ::GetCurrentThread(), m_nThreadPriority );
				nThreadPriority = m_nThreadPriority; 

				if (m_pXMLStat)
					m_pXMLStat->SetAttribute(L"deq.thread_priority", m_nThreadPriority);
			}

			DWORD dwRet = ::WaitForSingleObject(m_theThread.m_hEvent, INFINITE);
			if(!m_theThread.m_bWork)
				break;

			ATLASSERT(dwRet == WAIT_OBJECT_0);
			ATLASSERT( GetCurrentThreadId() == m_theThread.m_dwThreadID );


			
			// Do something
			TObject object;
			int	nDeqSize = 0;
			{
				CAutoCS crs(m_csDeq);

				if( m_deqObjects.size() )
				{
					// Check for custom order
					if (!pT->OnDeqReady(m_deqObjects, object))
					{
						object = m_deqObjects.front();
						m_deqObjects.pop_front();
					}
				}
				else
				{
					continue;
				}

				if( m_nMaxFrames <= 0 || pT->DeqSize() < m_nMaxFrames )
				{
					::SetEvent(m_shEventFree);

					ATLTRACE2( atlTraceGeneral, TRACELEVEL_DeqActive, _T("CDeqActive(%08X:%04X)::RunThread() (%d(%d)/%d/%d) SET FREE\n"), 
						this, GetCurrentThreadId(), m_deqObjects.size(), pT->DeqSize(), m_nMinFrames, m_nMaxFrames );
				}
				else
				{
					ATLTRACE2( atlTraceGeneral, TRACELEVEL_DeqActive, _T("CDeqActive(%08X:%04X)::RunThread() (%d(%d)/%d/%d) KEEP BUSY\n"), 
						this, GetCurrentThreadId(), m_deqObjects.size(), pT->DeqSize(), m_nMinFrames, m_nMaxFrames );
				}

				nDeqSize = (int)m_deqObjects.size();
				if( nDeqSize > 0 )
				{
					m_theThread.Awake();
				}
				
				if( m_pXMLStat )
					m_pXMLStat->SetAttribute( L"buffer.current", (int)nDeqSize );

				ATLTRACE2( atlTraceGeneral, TRACELEVEL_DeqActive, _T("CDeqActive(%08X:%04X)::RunThread() FRAME Ready (%d(%d)/%d/%d)\n"), 
					this, GetCurrentThreadId(), m_deqObjects.size(), pT->DeqSize(), m_nMinFrames, m_nMaxFrames );
			}
			
			HRESULT hr = pT->OnActiveDeq( object, nDeqSize );
			if( hr == VFW_E_ENUM_OUT_OF_SYNC )
			{
				{
					CAutoCS crs(m_csDeq);

					MTRACE("OnActiveDeq() == VFW_E_ENUM_OUT_OF_SYNC - REPEAT (Deq:%zd +1)", m_deqObjects.size());

					m_deqObjects.push_front( object );

					if( m_pXMLStat )
						m_pXMLStat->SetAttribute( L"buffer.current", (int)m_deqObjects.size() );
				}

				m_theThread.Awake();
				continue;
			}
			else if (hr == E_ABORT) // Note special code for clear the deq
			{
				MTRACE("OnActiveDeq() == E_ABORT - CLEAR (Deq:%zd)", m_deqObjects.size());

				CAutoCS crs(m_csDeq);

				m_deqObjects.clear();
			}
		}

		::CoUninitialize();
		return S_OK;
	}
};

typedef HRESULT (WINAPI *PFOnActiveDeq)(void* pvCallbackPtr, LONGLONG llCallbackData, void* _pvObject, int _nDeqSize );

template <class TInterface>
class CDeqActiveObj :  
	public AutoObjBase<CDeqActiveObj<TInterface> >,
	public CDeqActive<CDeqActiveObj<TInterface>, CComPtr<TInterface> >
{
	PFOnActiveDeq m_pfCallback;
	void*  m_pvCallbackPtr;
	LONGLONG m_llCallbackData;



public:

	CDeqActiveObj( PFOnActiveDeq _pfCallback, void* pvCallbackPtr, LONGLONG llCallbackData )
		: m_pfCallback( _pfCallback ), m_pvCallbackPtr( pvCallbackPtr ), m_llCallbackData( llCallbackData )
	{
	}

	static HRESULT CreateInstance( PFOnActiveDeq _pfCallback, void* pvCallbackPtr, LONGLONG llCallbackData, CDeqActiveObj<TInterface>** _ppObj )
	{
		if( !_ppObj || !_pfCallback )
			return E_INVALIDARG;

		CDeqActiveObj* pObj = new CDeqActiveObj(_pfCallback, pvCallbackPtr, llCallbackData);
		pObj->AddRef();

		*_ppObj = pObj;
		return S_OK;
	}

public:
	HRESULT OnActiveDeq( TInterface* _object, int _nDeqSize )
	{
		return m_pfCallback( m_pvCallbackPtr, m_llCallbackData, (void*)_object, _nDeqSize);
	}

	HRESULT OnFrameDrop( TInterface* _object )
	{
		return m_pfCallback( m_pvCallbackPtr, m_llCallbackData, (void*)_object, -1);
	}

};

template <class T, class TInterface>
class CDeqActiveMT
{
	CComAutoCriticalSection						m_csThreads;
	
	typedef typename CDeqActiveObj<TInterface>::TPtr  TDeqActiveObjPtr;

	CSimpleArray2<TDeqActiveObjPtr>				m_arrDeques;
	
public:

	CDeqActiveMT()
	{
	}

	~CDeqActiveMT()
	{
		DeqFree(true);
	}

	HRESULT OnActiveDeq( int _nThread, TInterface* _object, int _nDeqSize )
	{
		METHOD_MUST_BE_OVERRIDEN( OnActiveDeq ); 
	}

	HRESULT OnFrameDrop( int _nThread, TInterface* _object )
	{
		return S_OK;
	}


	// Deq methods
	bool DeqSetThreads( int _nWorkThreads )
	{
		ATLASSERT( _nWorkThreads <= 128 );
		if( _nWorkThreads > 128 )
			return false;

		if( _nWorkThreads < 0 )
		{
			// TODO: CPU count
			_nWorkThreads = 8;
		}

		CAutoCS crs(m_csThreads);

		while( m_arrDeques.GetSize() > _nWorkThreads )
		{
			RemoveThread();
		}

		for( int i = m_arrDeques.GetSize(); i < _nWorkThreads; i++ )
		{
			AddThread();
		}
	}

	void DeqSetStatisticsXML( XMLParse::XMLNodePtr _pXMLStat )
	{
		CAutoCS crs(m_csDeq);

		m_pXMLStat = _pXMLStat;
	}
	
	int DeqMaxFrames() const { return DeqPtr()->DeqMaxFrames(); }

	int DeqMinFrames() const { return DeqPtr()->DeqMinFrames(); }

	int DeqGetPriority() { return DeqPtr()->DeqGetPriority(); }

	void DeqSetPriority( int _nPriority )
	{ 
		for( int i = 0; i < MAX(1, DeqThreads() ); i++ )
		{
			TDeqActiveObjPtr pDeq = DeqPtr(i);
			if( pDeq )
				pDeq->DeqSetPriority(_nPriority);
		}
	}

	void DeqSetMinMax( int _nMin, int _nMax, bool _bForceNew = false )
	{
		for( int i = 0; i < MAX(1, DeqThreads() ); i++ )
		{
			TDeqActiveObjPtr pDeq = DeqPtr(i);
			if( pDeq )
				pDeq->DeqSetMinMax(_nMin, _nMax, _bForceNew );
		}
	}

	void DeqSetMax( int _nMax, bool _bForceNew = false )
	{
		DeqSetMinMax( 0, _nMax, _bForceNew );
	}

	int DeqPut( int _nThread, TInterface* _object, DWORD _dwWaitMS = INFINITE )
	{
		return DeqPutRT( _nThread, _object, _dwWaitMS != INFINITE ? _dwWaitMS * DS_MSEC : -1);
	}

	int DeqPutRT( int _nThread, TInterface* _object, REFERENCE_TIME _rtWaitMS )
	{
		if( _nThread < 0 )
		{
			// Take minimum lane
			int nMin = MAXLONG;
			for( int i = 0 ; i < DeqThreads(); i++ )
			{
				int nSize = DeqSize( i );
				if( nSize < nMin )
				{
					_nThread = i;
					nMin = nSize;
				}
			}
		}

		ATLASSERT( _nThread <= 128 );

		TDeqActiveObjPtr pDeq = DeqPtr(_nThread, true);
		if( !pDeq )
			return -1;
		
		return pDeq->DeqPutRT( _object, _rtWaitMS );
	}

	int DeqSize( int _nThread, int* _pnMax = NULL )
	{
		TDeqActiveObjPtr pDeq = DeqPtr(_nThread);
		if( !pDeq )
			return -1;

		return pDeq->DeqSize( _pnMax );
	}

	int DeqThreads()
	{
		CAutoCS crs(m_csThreads);

		return m_arrDeques.GetSize();
	}

	ULONGLONG DeqFree( bool _bStopThread = false, DWORD _dwMaxWait = 3000 )
	{
		ULONGLONG llFlash = 0; 
		int nThread = 0;
		TDeqActiveObjPtr pDeq;
		while( pDeq = DeqPtr(nThread))
		{
			if( pDeq->DeqFree(_bStopThread) )
				llFlash |= (((ULONGLONG)1)<<nThread);

			++nThread;
		}

		if( _bStopThread )
		{
			while( DeqThreads() )
			{
				RemoveThread(true);
			}
		}

		return llFlash;
	}

private:

	bool AddThread()
	{
		T* pT = static_cast<T*>(this);

		TDeqActiveObjPtr pDeq;
		CDeqActiveObj<TInterface>::CreateInstance( OnActiveDeq_StaticCB, pT, m_arrDeques.GetSize(), &pDeq );
		ATLASSERT( pDeq );

		CAutoCS crs(m_csThreads);

		if( m_arrDeques.GetSize() > 0 )
		{
			pDeq->DeqSetMinMax( m_arrDeques[0]->DeqMinFrames(), m_arrDeques[0]->DeqMaxFrames() );
			pDeq->DeqSetPriority( m_arrDeques[0]->DeqGetPriority() );
		}

		m_arrDeques.Add( pDeq );

		return true;
	}

	TDeqActiveObjPtr DeqPtr(int _nThread = 0, bool _bCreateThread = false)
	{
		CAutoCS crs(m_csThreads);

		while( (_bCreateThread || _nThread == 0) && _nThread >= m_arrDeques.GetSize() )
		{
			AddThread();
		}

		if( _nThread >= m_arrDeques.GetSize() )
			return TDeqActiveObjPtr(NULL);

		return m_arrDeques[0];
	}

	bool RemoveThread( bool _bFinalRemove = false )
	{
		TDeqActiveObjPtr pDeq;

		{
			CAutoCS crs(m_csThreads);

			if( m_arrDeques.GetSize() > (_bFinalRemove ? 0 : 1 ) )
			{
				int nLast = m_arrDeques.GetSize() - 1;
				pDeq = m_arrDeques[nLast];
				m_arrDeques.RemoveAt( nLast );
			}
		}

		if( pDeq )
		{
			pDeq->DeqFree(true);

			return true;
		}

		return false;
	}

	static HRESULT WINAPI OnActiveDeq_StaticCB(void* pvCallbackPtr, LONGLONG llCallbackData, void* _pvObject, int _nDeqSize )
	{
		ATLASSERT( pvCallbackPtr );
		if( pvCallbackPtr )
		{
			T* pT = static_cast<T*>(pvCallbackPtr);
			if( _nDeqSize >= 0 )
				return pT->OnActiveDeq( (INT)llCallbackData, (TInterface*)_pvObject, _nDeqSize );

			return pT->OnFrameDrop( (INT)llCallbackData, (TInterface*)_pvObject );
		}

		return -1;
	}

};

#endif //NEW_XML_PARSE

class MDeqANC
{
	CComAutoCriticalSection		m_csDeq;

	typedef deque< pair<CComPtr<IMFrame>,DWORD> > TDeqANC;
	TDeqANC			m_deqANC;
	bool			m_bSkipXDS;

public:

	MDeqANC()
		: m_bSkipXDS( false )
	{
	}

	~MDeqANC()
	{
		Reset();
	}

	void Reset()
	{
		CAutoCS crs(m_csDeq);

		m_deqANC.clear();
	}

	int Size()
	{
		CAutoCS crs(m_csDeq);

		return (int)m_deqANC.size();
	}

	bool IsValidC608( DWORD _dwC608, bool _bIgnoreXDS )
	{
		if( _bIgnoreXDS )
		{
			// Ignore channel 3 & 4
			if( (_dwC608|0xFFFF8080) == 0xFFFF8080 )
				return false;
		}
		else
		{
			if( (_dwC608|0x80808080) == 0x80808080 )
				return false;
		}
	
		return true;
	}

	bool IsXDSStart( DWORD _dwC608 )
	{
		BYTE btXDS_0 = ((_dwC608 >> 16) & 0x7F);
		return btXDS_0 >= 0x01 && btXDS_0 <= 0x0E;
	}

	int Put(IUnknown* _pFrame, int _nMaxSize = 128 )
	{
		MF_FRAME_INFO mfInfo = MFFrame_Info(_pFrame);
		if (mfInfo.nDataCount == 0 && mfInfo.nObjCount == 0 && mfInfo.nStrCount == 0)
			return 0;
		
		// Get C608
		LONG cbData = 0;
		LONGLONG pbData = MFSideData_DataGet(_pFrame, L"C608", &cbData );
		DWORD dwCC608 = 0x80808080;
		if( cbData >= 4 && pbData )
		{
			dwCC608 = *(DWORD*)pbData;
		}

		if( m_bSkipXDS && !IsXDSStart(dwCC608) )
		{
			dwCC608 &= 0x0000FFFF;
			dwCC608 |= 0x808F0000;
		}
		else
		{
			m_bSkipXDS = false;
		}
		

		CComPtr<IMFrame> cpFrame;
		MCreator::MFFrameClone(_pFrame, &cpFrame, eMFC_Props);

#ifdef _DEBUG
		LONGLONG lpVideo = 0;
		cpFrame->FrameVideoGetBytes( NULL, &lpVideo );
		ATLASSERT( lpVideo == 0 );
#endif

		char pszCC[5] = {};
		*(DWORD*)pszCC = dwCC608 & ~0x80808080;

		int nSize = 0;
		{
			CAutoCS crs(m_csDeq);

			// Check for overflow (e.g. NTSC->PAL conversion)
			if( (int)m_deqANC.size() > _nMaxSize * 2 || 
				((int)m_deqANC.size() > _nMaxSize && !IsValidC608( dwCC608, true ) ) || 
				(m_deqANC.size() > 3 && !IsValidC608( m_deqANC.back().second, true ) && !IsValidC608( dwCC608, true ) ) )
			{
				ATLTRACE2( atlTraceGeneral, 8, _T("MDeqANC::Put( %hs (%08X) ,Deq:%d/%d) - SKIP\n"), pszCC, dwCC608, m_deqANC.size(), _nMaxSize );

				// Skip this
				nSize = (int)m_deqANC.size();
				m_bSkipXDS = true;
			}
			else
			{
				ATLTRACE2( atlTraceGeneral, 8, _T("MDeqANC::Put( %hs (%08X) ,Deq:%d/%d) - PUT\n"), pszCC, dwCC608, m_deqANC.size(), _nMaxSize );

				m_deqANC.push_back( make_pair(cpFrame,dwCC608) );
				nSize = (int)m_deqANC.size();
			}
		}

		return nSize;
	}

	bool Read( IUnknown* _pDest, bool _bRemove = true )
	{
		CComPtr<IMFrame> cpMFrameRead;
		{
			CAutoCS crs(m_csDeq);

			if (!m_deqANC.size())
				return false;

			cpMFrameRead = m_deqANC.front().first;
			ATLASSERT(cpMFrameRead);

			char pszCC[5] = {};
			*(DWORD*)pszCC = m_deqANC.front().second & ~0x80808080;

			ATLTRACE2(atlTraceGeneral, 8, _T("MDeqANC::Read( %hs (%08X),Deq:%d)\n"), pszCC, m_deqANC.front().second, m_deqANC.size());

			if (_bRemove)
				m_deqANC.pop_front();

		}

		MFSideData_Copy(cpMFrameRead, _pDest, -1);

		return true;
	}
};

// VVB: Obsolete, have to be removed 
// Use MBitrateCalculator instead
class MBitrateCalc:
	public AutoObjBase<MBitrateCalc>
{
	struct STimeSize
	{
		REFERENCE_TIME  rtTime;
		LONG			lSize;
	};

	CComAutoCriticalSection m_csDeq;
	deque<STimeSize>		m_deqTime;
	LONGLONG				m_llDataSize;

public:

	MBitrateCalc()
	{
		Reset();
	}

	~MBitrateCalc()
	{
		Reset();
	}

	int OnPacket( REFERENCE_TIME _rtTime, LONG _lSize, double _dblSecAvg = 1.0 )
	{
		CAutoCS crs(m_csDeq );

		STimeSize pktInfo = { _rtTime, _lSize };
		if( pktInfo.rtTime == MINLONGLONG && m_deqTime.size() )
			pktInfo.rtTime = m_deqTime.back().rtTime;

		m_deqTime.push_back( pktInfo );
		m_llDataSize += pktInfo.lSize;
		while( m_deqTime.size() > 1 && m_deqTime.front().rtTime + DBL2INT64( _dblSecAvg * DS_ONESEC ) < pktInfo.rtTime  )
		{
			m_llDataSize -= m_deqTime.front().lSize;
			m_deqTime.pop_front();
		}

		return GetBitrate();
	}

	int GetBitrate()
	{
		CAutoCS crs(m_csDeq );

		if( m_deqTime.size() < 2 )
			return 0;

		REFERENCE_TIME rtTime = m_deqTime.back().rtTime - m_deqTime.front().rtTime;
		if( rtTime <= 0 )
			return 0;
		double dblRate = (m_llDataSize - m_deqTime.back().lSize) * 8.0 * DS_ONESEC / rtTime;

		return DBL2INT( dblRate );
	}

	void Reset()
	{
		CAutoCS crs(m_csDeq );
		m_deqTime.clear();
		m_llDataSize = 0;
	}
};

// Dirty fix for CG
#ifdef _XML_PARSE_M_H_

// Timer method, for rate control, support MFFrame rate control by time and rate control by 'slots' including 
// live mode with slot timer corrections and 'slot' base time detection
class MRateTimer
	: public AutoObjBase<MRateTimer>
{
	MTimer				m_timer;
	MTimeCorrector		m_timeCorrector;
	MClock				m_clock;
	CComAutoCriticalSection m_csClock;
	CStdHandle			m_shFlush;

	// For reset clock after long periods of times
	// e.g. https://medialooks.myjetbrains.com/youtrack/issue/MP-1057
	REFERENCE_TIME		m_rtLastWait;

	// For WaitTimeSlot
	LONGLONG			m_llSlotNext;
	double				m_dblWaitPeriodRT;
	REFERENCE_TIME		m_rtSlotOffset;
	MAverageCalc		m_avgSlotSync;
	
public:
	MRateTimer()
	{
		m_avgSlotSync.SetAverageCounter(16);
		Reset();
	}
	~MRateTimer()
	{
	}

	void Clock_SpeedUp_Set(double  _dblSpeedUp)
	{
		m_clock.SpeedUp_Set(_dblSpeedUp);
	}

	double Clock_SpeedUp_Get()
	{
		return m_clock.SpeedUp_Get();
	}

	// Time slot waiting methods
	double TimeSlot(LONGLONG* _pllSlotNext = NULL, double* _pdblPeriod = NULL, REFERENCE_TIME* _prtSlotClock = NULL)
	{
		if (m_dblWaitPeriodRT <= 0)
			return -1.0;

		REFERENCE_TIME rtClock = m_clock.GetTime() - m_rtSlotOffset;

		if (_pllSlotNext)
			*_pllSlotNext = m_llSlotNext;

		if (_prtSlotClock)
			*_prtSlotClock = rtClock;

		if (_pdblPeriod)
			*_pdblPeriod = m_dblWaitPeriodRT;

		return rtClock / m_dblWaitPeriodRT;
	}

	MAverageCalc& TimeSlotAvgSync() { return m_avgSlotSync; }

	HRESULT WaitTimeSlot( IUnknown* _pFrame, HANDLE* _phFlush = NULL, LONG* _pSkipSlots = NULL, LONGLONG* _pllSlotIdx = NULL )
	{
		return _WaitTimeSlotEx( _pFrame, -1,  _phFlush, _pSkipSlots, _pllSlotIdx );
	}

	HRESULT WaitTimeSlot( double _dblWaitPeriodRT, HANDLE* _phFlush = NULL, LONG* _pSkipSlots = NULL, LONGLONG* _pllSlotIdx = NULL )
	{
		return _WaitTimeSlotEx(_dblWaitPeriodRT, -1, _phFlush, _pSkipSlots, _pllSlotIdx);
	}

	// Added _nMaxSlotsStep option:
	// _nMaxSlotsStep
	// _nMaxSlotsStep == -2 -> Live sync mode wait, do not skip slots, but shift slot timer
	// _nMaxSlotsStep == -1 -> the same as WaitTimeSlot (not limited number of skipped slots), if slot skipped wait next slot
	// Next option used for MMixer - FrameGet_Callback
	// _nMaxSlotsStep == 0 -> calculate slots stat offset (accoring to call time), set slot index to zero, do not wait
	// _nMaxSlotsStep == N -> wait not more then N skipped slots, if N == 1 - mean what wait only next slot, if slot skipped -> do not wait 
	
	HRESULT WaitTimeSlotEx(IUnknown* _pFrame, int _nMaxSlotsStep, HANDLE* _phFlush = NULL, LONG* _pSkipSlots = NULL, LONGLONG* _pllSlotIdx = NULL)
	{
		return _WaitTimeSlotEx(_pFrame, _nMaxSlotsStep, _phFlush, _pSkipSlots, _pllSlotIdx);
	}

	HRESULT WaitTimeSlotEx(double _dblWaitPeriodRT, int _nMaxSlotsStep, HANDLE* _phFlush = NULL, LONG* _pSkipSlots = NULL, LONGLONG* _pllSlotIdx = NULL)
	{
		return _WaitTimeSlotEx(_dblWaitPeriodRT, _nMaxSlotsStep, _phFlush, _pSkipSlots, _pllSlotIdx);
	}

	// Sample time rate control

	HRESULT WaitSampleTime( IUnknown* _pFrame, HANDLE* _phFlush, REFERENCE_TIME _rtOffset = 0, REFERENCE_TIME _rtMaxWait = -1 )
	{
		CComQIPtr<IMFrame> qpMFrame( _pFrame );

		ATLASSERT( qpMFrame );
		if( !qpMFrame ) return E_POINTER;

		LONG lAudioSamples = 0;
		M_AV_PROPS avProps = MFFrame_AVProps( _pFrame, &lAudioSamples );
		
		M_TIME mTime = {};
		qpMFrame->FrameTimeGet( &mTime );

		return WaitSampleTime( &avProps, &mTime, _phFlush, MAudSamples2Bytes( &avProps.audProps, lAudioSamples), _rtOffset, _rtMaxWait );
	}

	HRESULT WaitSampleTime( M_AV_PROPS* _pAVProps, M_TIME* _pTimeIn, HANDLE* _phFlush, int _cbAudio = -1, REFERENCE_TIME _rtOffset = 0, REFERENCE_TIME _rtMaxWait = -1 )
	{
		ATLASSERT( _pTimeIn );
		if( !_pTimeIn ) return E_INVALIDARG;

		M_TIME mTimeIn = {};
		if (_pTimeIn)
			mTimeIn = *_pTimeIn;

		// WARNING !!! Could be slight time shift due to sample rate to reference time correction
		// - for precise timing have to specify the _pTimeIn  
		if (_pAVProps &&_cbAudio > 0 && mTimeIn.rtEndTime == mTimeIn.rtStartTime)
			mTimeIn.rtEndTime = mTimeIn.rtStartTime + MAudBytes2Time(&_pAVProps->audProps, _cbAudio);

		// Correct time
		M_TIME mTime = m_timeCorrector.CalcMTime( _pAVProps ? &_pAVProps->vidProps : NULL, &mTimeIn, DS_MSEC * 10 );

		// Check for clock correction
		//m_clock.AdjustByTime(&mTime);

		// TODO: correction time by audio len
		REFERENCE_TIME rtWait = 0;
		{
			CAutoCS crs( m_csClock );

			// Special fix for buffer overflow
			// TODO: Adjust speed up coefficients, use for buffer underflow 
			if (BUFFER_POS_HAVE(_pTimeIn->eFFlags) )
			{
				double dblBuffer = BufferPosDbl(_pTimeIn->eFFlags);
				if (dblBuffer > 0.5)
					m_clock.SpeedUp_Set(1.20);
				else if (dblBuffer > 0.1)
					m_clock.SpeedUp_Set(1.0 + (dblBuffer - 0.1) / 2.0 ); // from 0% at 10% buffer to 20% at 50% buffer
				else if (dblBuffer < -0.5)
					m_clock.SpeedUp_Set(0.95);
				else	 
					m_clock.SpeedUp_Set(1.0);

				MTRACE("Rate Speed-up:%.3f Buffer:%.3f", m_clock.SpeedUp_Get(), dblBuffer);
			}

			rtWait = mTime.rtStartTime - m_clock.GetTime(); 
			if( rtWait > DS_ONESEC || rtWait < -5 * DS_ONESEC || ABS(m_clock.GetTime() - m_rtLastWait) > DS_ONESEC )
			{
				ATLTRACE2( atlTraceGeneral, 2, _T("MRateTimer::WaitSampleTime(%s..%s) - RESET clock:%s (last:%ws) (Original:%ws Corr:%.5f ) (wait:%s)\n"),
					RT2STR( mTime.rtStartTime ), RT2STR( mTime.rtEndTime ), RT2STR( m_clock.GetTime()), RT2STR(m_rtLastWait),
					RT2STR(m_clock.GetTime(0)), m_clock.SpeedUp_Get(), RT2STR( -1 * rtWait ) );

				m_clock.Reset(mTime.rtStartTime - _rtOffset);

				// Wait 1 frame time
				rtWait = mTime.rtEndTime - mTime.rtStartTime; 
			}
		}


		DWORD dwRes = 0;
		if( rtWait > 0 && _rtMaxWait != 0 )
		{
			ATLTRACE2( atlTraceGeneral, 8, _T("MRateTimer::WaitSampleTime(%s..%s) - WAIT clock:%s (Wait:%s Max:%s)\n"),
				RT2STR( mTime.rtStartTime ), RT2STR( mTime.rtEndTime ), RT2STR( m_clock.GetTime()), RT2STR( -1 * rtWait ), RT2STR( _rtMaxWait) );

			if( _rtMaxWait > 0 )
				rtWait = MIN( rtWait, _rtMaxWait );

			if( !_phFlush && !m_shFlush )
				m_shFlush.Attach( ::CreateEvent( NULL, TRUE, FALSE, NULL ) );

			REFERENCE_TIME rtRealWait = 0;
			DWORD dwRes = m_timer.WaitAnyObjects_WithTime( 1, _phFlush ? _phFlush : &m_shFlush.m_hHandle, rtWait, &rtRealWait );

			MTRACE("(%s..%s) - WAIT DONE clock:%s (Original:%ws Corr:%.5f) (wait:%s, Max:%s) res=%d real:%s",
				RT2STR( mTime.rtStartTime ), RT2STR( mTime.rtEndTime ), RT2STR( m_clock.GetTime()), 
				RT2STR(m_clock.GetTime(0)), m_clock.SpeedUp_Get(), RT2STR( rtWait ), RT2STR( _rtMaxWait), dwRes, RT2STR( rtRealWait ) );
		}
		else
		{
			MTRACE( "(%s..%s) - WAIT SKIP clock:%s (Original:%ws Corr:%.5f)  (wait:%s)\n",
				RT2STR( mTime.rtStartTime ), RT2STR( mTime.rtEndTime ), RT2STR( m_clock.GetTime()), 
				RT2STR(m_clock.GetTime(0)), m_clock.SpeedUp_Get(), RT2STR( -1 * rtWait ) );
		}

		m_rtLastWait = m_clock.GetTime();

		return dwRes;
	}

	void Reset( REFERENCE_TIME _rtSet = MINLONG )
	{
		CAutoCS crs(m_csClock);

		ATLTRACE2( atlTraceGeneral, 8, _T("MRateTimer:Reset(%ws)\n"), RT2STR( _rtSet ) );

		m_clock.Reset( _rtSet );
		m_timeCorrector.ResetTime(0);

		if( m_shFlush.IsValid() )
			::PulseEvent( m_shFlush );

		m_llSlotNext = -1;
		m_dblWaitPeriodRT = 0;
		m_rtSlotOffset = 0;
		m_rtLastWait = -100 * DS_24HOUR;
		m_avgSlotSync.Reset();
	}

	REFERENCE_TIME GetTime() { return m_clock.GetTime();  }

private:

	HRESULT _WaitTimeSlotEx(IUnknown* _pFrame, int _nMaxSlotsStep, HANDLE* _phFlush, LONG* _pSkipSlots, LONGLONG* _pllSlotIdx)
	{
		double dblAvgTimeRT = MFFrame_AvgTimeRT(_pFrame, NULL, 0.0);
		if (dblAvgTimeRT <= 0)
			return E_INVALIDARG;

		return _WaitTimeSlotEx(dblAvgTimeRT, _nMaxSlotsStep, _phFlush, _pSkipSlots, _pllSlotIdx);
	}

	HRESULT _WaitTimeSlotEx(double _dblWaitPeriodRT, int _nMaxSlotsStep, HANDLE* _phFlush, LONG* _pSkipSlots, LONGLONG* _pllSlotIdx)
	{
		ATLASSERT(_dblWaitPeriodRT > 0);
		if (_dblWaitPeriodRT <= 0) 
			return E_INVALIDARG;

		if (_pSkipSlots)
			*_pSkipSlots = -1;

		if (_pllSlotIdx)
			*_pllSlotIdx = 0;

		REFERENCE_TIME rtClock = m_clock.GetTime();

		if (m_dblWaitPeriodRT != _dblWaitPeriodRT)
		{
			MTRACE("(%08X) (Period:%s->%s) - RESET(Period Step:%ws) Idx:%I64d Offset:%s->%s", (ULONG)this,
				RT2STR(m_dblWaitPeriodRT), RT2STR(_dblWaitPeriodRT), RT2STR(m_clock.GetStep(true)), m_llSlotNext, RT2STR(m_rtSlotOffset), RT2STR(rtClock));

			_SlotsReset(_dblWaitPeriodRT, rtClock);
			m_avgSlotSync.Reset();
			// Reset step
			m_clock.GetStep(true);
			return S_FALSE;
		}

		ATLASSERT(m_dblWaitPeriodRT > 0);
		ATLASSERT(m_llSlotNext > 0);

		rtClock -= m_rtSlotOffset;
		double dblCurSlot = rtClock / m_dblWaitPeriodRT;
		double dblNextSlotTimeRT = m_llSlotNext * m_dblWaitPeriodRT;

		// Sync time slot (for Live mode / sync calc)
		if ( _nMaxSlotsStep == 0 || (_nMaxSlotsStep == -2 && (LONGLONG)dblCurSlot >= m_llSlotNext) )
		{
			REFERENCE_TIME rtNewOffset = m_rtSlotOffset + rtClock;
			double dblMsecDelay = (rtClock - dblNextSlotTimeRT) / DS_MSEC;

			// Check for reasonable delay (less the period at least), if not just update start time 
			// e.g. WaitTimeSlot not called for period of time
			if (ABS(dblMsecDelay) * DS_MSEC < m_dblWaitPeriodRT)
			{
				// Move start offset by 1/2 of changes 
				rtNewOffset = DBL2INT64(m_rtSlotOffset + (dblNextSlotTimeRT + rtClock) / 2.0);
				m_avgSlotSync.AddValue(dblMsecDelay);
			}
			
			MTRACE("(%08X) (Period:%s Slot:%.3f/%I64d Time:%ws Next:%ws) - RESET(CALC Step:%ws) Idx:%I64d Offset:%ws->%ws Delay:%.3f AVG:%ws", (ULONG)this,
				RT2STR(m_dblWaitPeriodRT), dblCurSlot, m_llSlotNext, RT2STR(rtClock), DRT2STR(dblNextSlotTimeRT), RT2STR(m_clock.GetStep(true)), m_llSlotNext, RT2STR(m_rtSlotOffset), RT2STR(rtNewOffset),
				dblMsecDelay, DEBUG_COUNTERS_NOFPS(m_avgSlotSync) );

			_SlotsReset(_dblWaitPeriodRT, rtNewOffset );
			
			return S_FALSE;
		}

		LONGLONG llWaitSlot = LONGLONG(dblCurSlot) + 1;
		// Could be prev slot as wait time could be for few microsec less
		ATLASSERT(llWaitSlot + 1 >= m_llSlotNext);
		llWaitSlot = MAX(llWaitSlot, m_llSlotNext);

		if( _nMaxSlotsStep > 0 )
			llWaitSlot = MIN(llWaitSlot, m_llSlotNext + _nMaxSlotsStep - 1);

		// Wait time slot
		REFERENCE_TIME rtSlotTime = DBL2INT64(llWaitSlot * m_dblWaitPeriodRT);
		REFERENCE_TIME rtWait = rtSlotTime - rtClock;
		// Next assert could be wrong if FLUSH event signaled
		// ATLASSERT(rtWait < DBL2INT64(m_dblWaitPeriodRT + DS_MSEC) || m_llSlotNext < 2 );

		if (!_phFlush && !m_shFlush)
			m_shFlush.Attach(::CreateEvent(NULL, TRUE, FALSE, NULL));

		REFERENCE_TIME rtRealWait = 0;
		DWORD dwRes = -1;
		if( rtWait > DS_MSEC / 2 ) // Do not wait less then 0.5 msec
			dwRes = m_timer.WaitAnyObjects_WithTime(1, _phFlush ? _phFlush : &m_shFlush.m_hHandle, rtWait, &rtRealWait);

		MTRACE("(%08X) (Period:%s Slot:%.3f MaxStep:%d) - Time:%s (Step:%s) Slot:%I64d->%I64d (Skip:%d) Wait:%s(%s)=%d", (ULONG)this,
			RT2STR(m_dblWaitPeriodRT), dblCurSlot, _nMaxSlotsStep, RT2STR(rtClock), RT2STR(m_clock.GetStep(true)),
			m_llSlotNext, llWaitSlot + 1, (int)(llWaitSlot - m_llSlotNext), RT2STR(rtWait), RT2STR(rtRealWait - rtWait), dwRes);

		if (_pSkipSlots)
			*_pSkipSlots = (int)(llWaitSlot - m_llSlotNext);

		if (_pllSlotIdx)
			*_pllSlotIdx = llWaitSlot;

		m_llSlotNext = llWaitSlot + 1;

		return S_OK;
	}

	inline void _SlotsReset(double _dblPeriodRT, REFERENCE_TIME _rtSlotStartOffset )
	{
		m_llSlotNext = 1;
		m_rtSlotOffset = _rtSlotStartOffset;
		m_dblWaitPeriodRT = _dblPeriodRT;
	}
};

#endif

class MEventsReceiver
{
public:
	virtual HRESULT OnFrame( 
		LPCWSTR	_pszChannelID,
		IUnknown* _pFrame,
		BOOL _bSync,
		BOOL _bSendData ) = 0;


	virtual HRESULT OnEvent( 
		LPCWSTR	_pszChannelID,
		LPCWSTR _pszEventName,
		LPCWSTR _pszEventParam,
		IUnknown* _pObject,
		BOOL _bSync ) = 0;
};

template <class T> 
class IMFProcessRedir : public IMFProcess
{
public:
	STDMETHOD(ProcessFrame)( /*[in]*/ IMFFrame* _pFrameIn, /*[out]*/ IMFFrame** _ppFrameOut, /*[out]*/ int* _pnFramesRest, /*[in]*/ BSTR _bsPropsList )
	{
		if( _pnFramesRest )
			_pnFramesRest = 0;

		CComQIPtr<IMFrame> qpFrameIn(_pFrameIn);
		if( !qpFrameIn && _pFrameIn )
			return E_NOINTERFACE;

		T* pT = static_cast<T*>(this);

		CComPtr<IMFrame> cpFrameRes;
		HRESULT hr = pT->OnMediaReceive( 0, NULL, qpFrameIn, _ppFrameOut ? &cpFrameRes : NULL );
		if( FAILED( hr ) )
			return hr;

		if( _ppFrameOut )
		{
			if( !cpFrameRes )
				cpFrameRes = qpFrameIn;

			if( cpFrameRes )
			{
				hr = cpFrameRes.QueryInterface( _ppFrameOut );
				if( FAILED( hr ) )
					return hr;
			}
		}
		
		return S_OK;
	}
};

template <class T> 
class IMPluginRedir : public IMPlugin
{
public:

	STDMETHOD(OnMediaReceive)( 
		/* [in] */ LONGLONG _llCallbackCookie,
		/* [in] */ IUnknown *_pSender,
		/* [in] */ IMFrame *_pFrameIn,
		/* [out] */ IMFrame **_ppFrameRes)
	{
		CComQIPtr<IMFFrame> qpFrameIn(_pFrameIn);
		if( !qpFrameIn && _pFrameIn )
			return E_NOINTERFACE;

		T* pT = static_cast<T*>(this);

		CComPtr<IMFFrame> cpFrameRes;
		HRESULT hr = pT->ProcessFrame( qpFrameIn, _ppFrameRes ? &cpFrameRes : NULL, NULL, NULL );
		if( FAILED( hr ) )
			return hr;

		if( _ppFrameRes )
		{
			if( !cpFrameRes )
				cpFrameRes = qpFrameIn;

			if( cpFrameRes )
			{
				hr = cpFrameRes.QueryInterface( _ppFrameRes );
				if( FAILED( hr ) )
					return hr;
			}
		}

		return S_OK;
	}

	STDMETHOD(OnMediaEnd)( 
		/* [in] */ LONGLONG _llCallbackCookie,
		/* [in] */ IUnknown *_pSender,
		/* [in] */ DWORD _dwFlags)
	{
		return S_OK;
	}

	STDMETHOD(OnMediaFlush)( 
		/* [in] */ LONGLONG _llCallbackCookie,
		/* [in] */ IUnknown *_pSender,
		/* [in] */ DWORD _dwFlags)
	{
		
		return S_OK;
	}
};




#endif // _M_HELPERS_H_
