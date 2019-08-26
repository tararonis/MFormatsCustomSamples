//---------------------------------------------------------------------------
// This file is part of the MediaLooks DirectShow Template Library (DSTL)
//
// DSGuids.h : some DirectShow guids and guid => name mapping
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

#ifndef __DSGUIDS_H_
#define __DSGUIDS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// C1F400A4-3F08-11d3-9F0B-006008039E37		Null Renderer
extern "C" const __declspec(selectany) CLSID  CLSID_NullRenderer = 
	{0xC1F400A4,0x3F08,0x11d3,{0x9f,0x0b,0x00,0x60,0x08,0x03,0x9e,0x37}};

// D51BD5A1-7548-11CF-A520-0080C77EF58A		Wave Parser
extern "C" const __declspec(selectany) CLSID  CLSID_WaveParser = 
	{0xD51BD5A1,0x7548,0x11CF,{0xA5,0x20,0x00,0x80,0xC7,0x7E,0xF5,0x8A}};

// ATTENTION. Overlay Mixer 2 returns clsid = CLSID_OverlayMixer in IPersist::GetClsID()
// A0025E90-E45B-11D1-ABE9-00A0C905F375		Overlay Mixer 2
extern "C" const __declspec(selectany) CLSID CLSID_OverlayMixer2 = 
	{0xA0025E90,0xE45B,0x11D1,{0xAB,0xE9,0x00,0xA0,0xC9,0x05,0xF3,0x75}};

// 51B4ABF3-748F-4E3B-A276-C828330E926A		Video Mixing Renderer 9
extern "C" const __declspec(selectany) CLSID CLSID_VideoMixingRenderer9 =
	{0x51B4ABF3,0x748F,0x4E3B,{0xA2,0x76,0xC8,0x28,0x33,0x0E,0x92,0x6A}};

// 7DF62B50-6843-11D2-9EEB-006008039E37		Generate Still Video Source 
extern "C" const __declspec(selectany) CLSID CLSID_StillVideoSource =
	{0x7DF62B50,0x6843,0x11D2,{0x9E,0xEB,0x00,0x60,0x08,0x03,0x9E,0x37}};

// AA20215C-B047-4702-BA13-57376113AAD0		Windows Media Player DMO Container
extern "C" const __declspec(selectany) CLSID CLSID_WMPDMOContainer =
	{0xAA20215C,0xB047,0x4702,{0xBA,0x13,0x57,0x37,0x61,0x13,0xAA,0xD0}};

// 33156162-81D6-11D3-8006-00C04FA30A73		Windows Media Encoder Sink (???)
extern "C" const __declspec(selectany) CLSID CLSID_WME_Sink =
	{0x33156162,0x81D6,0x11D3,{0x80,0x06,0x00,0xC0,0x4F,0xA3,0x0A,0x73}};

/////////////////////////////////////////////////////////////////////////////
// Subtype for input pin of "Mpeg4 Decoder DMO" filter
extern "C" const __declspec(selectany) GUID MEDIASUBTYPE_MPG4_DMO =
	{0x3447504D,0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}};

extern "C" const __declspec(selectany) GUID MEDIASUBTYPE_mpg4_DMO =
	{0x3467706D,0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}};

extern "C" const __declspec(selectany) GUID MEDIASUBTYPE_mp42_DMO =
	{0x3234706D,0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}};

extern "C" const __declspec(selectany) GUID MEDIASUBTYPE_MP42_DMO =
	{0x3234504D,0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}};

//---------------------------------
// Subtype for input pin of "Mpeg43 Decoder DMO" filter
extern "C" const __declspec(selectany) GUID MEDIASUBTYPE_mp43_DMO =
	{0x3334706D,0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}};

extern "C" const __declspec(selectany) GUID MEDIASUBTYPE_MP43_DMO =
	{0x3334504D,0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}};

//---------------------------------
// Subtype for input pin of "Mpeg4s Decoder DMO" filter
extern "C" const __declspec(selectany) GUID MEDIASUBTYPE_mp4s_DMO =
	{0x7334706D,0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}};

extern "C" const __declspec(selectany) GUID MEDIASUBTYPE_MP4S_DMO =
	{0x5334504D,0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}};

extern "C" const __declspec(selectany) GUID MEDIASUBTYPE_m4s2_DMO =
	{0x3273346D,0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}};

extern "C" const __declspec(selectany) GUID MEDIASUBTYPE_M4S2_DMO =
	{0x3253344D,0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}};

/////////////////////////////////////////////////////////////////////////////
// Some __uuidof(), not defined in DirectX idl
struct __declspec(uuid("{6E8D4A21-310C-11d0-B79A-00AA003767A7}")) IAMLine21Decoder;
struct __declspec(uuid("{593cdde1-0759-11d1-9e69-00c04fd7c15b}")) IMixerPinConfig;
struct __declspec(uuid("{ebf47182-8764-11d1-9e69-00c04fd7c15b}")) IMixerPinConfig2;
struct __declspec(uuid("{15e65ec0-3b9c-11d2-b92f-00609797ea5b}")) IDirectDraw7;
struct __declspec(uuid("{36d39eb0-dd75-11ce-bf0e-00aa0055595a}")) IDirectDrawVideo;

// From Win2k DDK (W2KDDK\inc\UUIDS.H)
struct __declspec(uuid("{C76794A1-D6C5-11D0-9E69-00C04FD7C15B}")) IVPNotify;
struct __declspec(uuid("{EBF47183-8764-11D1-9E69-00C04FD7C15B}")) IVPNotify2;
struct __declspec(uuid("{BC29A660-30E3-11D0-9E69-00C04FD7C15B}")) IVPConfig;

// Some definitions for old DirectX (6.0)
#ifdef __OLD_DS__

struct __declspec(uuid("{56a868b4-0ad4-11ce-b03a-0020af0ba770}")) IVideoWindow;
struct __declspec(uuid("{56a868b6-0ad4-11ce-b03a-0020af0ba770}")) IMediaEvent;
struct __declspec(uuid("{56a868c0-0ad4-11ce-b03a-0020af0ba770}")) IMediaEventEx;
struct __declspec(uuid("{56a868b1-0ad4-11ce-b03a-0020af0ba770}")) IMediaControl;
struct __declspec(uuid("{56a868b2-0ad4-11ce-b03a-0020af0ba770}")) IMediaPosition;

#endif	// __OLD_DS__

///////////////////////////////////////////////////////////////////////////
// 2476CBD2-51B6-4593-B26F-637D90378FCA		AAC audio
extern "C" const __declspec(selectany) GUID MEDIASUBTYPE_AAC =
	{0x2476CBD2,0x51B6,0x4593,{0xB2,0x6F,0x63,0x7D,0x90,0x37,0x8F,0xCA}};

// 30323449-0000-0010-8000-00AA00389B71		'I420' == MEDIASUBTYPE_I420
extern "C" const __declspec(selectany) GUID MEDIASUBTYPE_I420 =
	{0x30323449,0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}};

// 32555949-0000-0010-8000-00AA00389B71		'IYU2' == MEDIASUBTYPE_IYU2
extern "C" const __declspec(selectany) GUID MEDIASUBTYPE_IYU2 =
	{0x32555949,0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}};

//---------------------------------
// From DeckLink SDK

// 43594448-0000-0010-8000-00AA00389B71		'HDYC' == MEDIASUBTYPE_HDYC
#ifndef __MEDIASUBTYPE_HDYC_FWD_DEFINED__
#define __MEDIASUBTYPE_HDYC_FWD_DEFINED__
extern "C" const __declspec(selectany) GUID MEDIASUBTYPE_HDYC =
	{0x43594448,0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}};
#endif	// __MEDIASUBTYPE_HDYC_FWD_DEFINED__

// 30313276-0000-0010-8000-00AA00389B71		'v210' == MEDIASUBTYPE_v210
#ifndef __MEDIASUBTYPE_v210_FWD_DEFINED__
#define __MEDIASUBTYPE_v210_FWD_DEFINED__
extern "C" const __declspec(selectany) GUID MEDIASUBTYPE_v210 =
	{0x30313276,0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}};
#endif	// __MEDIASUBTYPE_v210_FWD_DEFINED__

// 30313272-0000-0010-8000-00AA00389B71		'r210' == MEDIASUBTYPE_r210
#ifndef __MEDIASUBTYPE_r210_FWD_DEFINED__
#define __MEDIASUBTYPE_r210_FWD_DEFINED__
extern "C" const __declspec(selectany) GUID MEDIASUBTYPE_r210 =
	{0x30313272,0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}};
#endif	// __MEDIASUBTYPE_r210_FWD_DEFINED__

//---------------------------------
// H264/AVC color spaces

// 32323459-0000-0010-8000-00AA00389B71		YUV 4:2:2 planar format
extern "C" const __declspec(selectany) GUID	MEDIASUBTYPE_Y422 =
	{0x32323459,0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}}; 

// 34343459-0000-0010-8000-00AA00389B71		YUV 4:4:4 planar format
extern "C" const __declspec(selectany) GUID	MEDIASUBTYPE_Y444 =
	{0x34343459,0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}}; 

//---------------------------------
// H264/AVC subtypes

// 31435641-0000-0010-8000-00AA00389B71		'AVC1' subtype from FOURCC_Base
// AVC file format according to ISO/IEC 14496-15, used in QT
extern "C" const __declspec(selectany) GUID	MEDIASUBTYPE_AVC1 =
	{0x31435641,0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}}; 

// 8D2D71CB-243F-45E3-B2D8-5FD7967EC09B		ITU-T Rec. H.264 (03/2005), Annex.B: Byte stream format
// Used in MainConcept H.264 splitter and MainConcept MPEG-2 Demultiplexer for H264 in TS
extern "C" const __declspec(selectany) GUID	MEDIASUBTYPE_H264ByteStream = 
	{0x8D2D71CB,0x243F,0x45E3,{0xB2,0xD8,0x5F,0xD7,0x96,0x7E,0xC0,0x9B}};

// F038FB38-36B4-4CE0-AD08-8EA7CB60110F		H.264/AVC sybtype from
// "MainConcept (Adobe2) H.264/AVC Video Encoder" filter
// Filter CLSID = {FF890B61-A4C5-4B19-87CF-65D86EC12F1C}
// Media type not includes seq_parameter_set() and pic_parameter_set()
extern "C" const __declspec(selectany) GUID	MEDIASUBTYPE_H264_MainConcept_Adobe2 = 
	{0xF038FB38,0x36B4,0x4CE0,{0xAD,0x08,0x8E,0xA7,0xCB,0x60,0x11,0x0F}};

// 34363248-0000-0010-8000-00AA00389B71		'H264' subtype from FOURCC_Base
// Output of ffdshow video encoder {4DB2B5D9-4556-4340-B189-AD20110D953F}
// Media type not includes seq_parameter_set() and pic_parameter_set()
extern "C" const __declspec(selectany) GUID	MEDIASUBTYPE_H264 =
	{0x34363248,0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}}; 
// 34363268-0000-0010-8000-00AA00389B71		'h264' subtype from FOURCC_Base
// Media type not includes seq_parameter_set() and pic_parameter_set()
extern "C" const __declspec(selectany) GUID	MEDIASUBTYPE_h264 =
	{0x34363268,0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}}; 

//---------------------------------
// Additional (non-standard) DV subtypes

// 32687664-0000-0010-8000-00AA00389B71		'dvh2' subtype: 720p24
extern "C" const __declspec(selectany) GUID	MEDIASUBTYPE_dvh2 =
	{0x32687664,0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}};

// 33687664-0000-0010-8000-00AA00389B71		'dvh3' subtype: 720p25
extern "C" const __declspec(selectany) GUID	MEDIASUBTYPE_dvh3 =
	{0x33687664,0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}};

// 34687664-0000-0010-8000-00AA00389B71		'dvh4' subtype: 720p30 (fps = 29.97)
extern "C" const __declspec(selectany) GUID	MEDIASUBTYPE_dvh4 =
	{0x34687664,0x0000,0x0010,{0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71}};

/////////////////////////////////////////////////////////////////////////////
// Some CLSID/IID

extern "C" const __declspec(selectany) CLSID CLSID_HTMLViewer =
	{0x25336920,0x03F9,0x11CF,{0x8F,0xD0,0x00,0xAA,0x00,0x68,0x6F,0x13}};

extern "C" const __declspec(selectany) IID IID_IActiveScript =
	{0xBB1A2AE1,0xA4F9,0x11CF,{0x8F,0x20,0x00,0x80,0x5F,0x2C,0xD0,0x64}};
extern "C" const __declspec(selectany) IID IID_IHTMLDocument2 =
	{0x332C4425,0x26CB,0x11D0,{0xB4,0x83,0x00,0xC0,0x4F,0xD9,0x01,0x19}};
extern "C" const __declspec(selectany) IID IID_IHTMLLocation =
	{0x163BB1E0,0x6E00,0x11CF,{0x83,0x7A,0x48,0xDC,0x04,0xC1,0x00,0x00}};
extern "C" const __declspec(selectany) IID IID_IHTMLWindow2 =
	{0x332C4427,0x26CB,0x11D0,{0xB4,0x83,0x00,0xC0,0x4F,0xD9,0x01,0x19}};
extern "C" const __declspec(selectany) IID IID_IObjectSafety =
	{0xCB5BDC81,0x93C1,0x11CF,{0x8F,0x20,0x00,0x80,0x5F,0x2C,0xD0,0x64}};
extern "C" const __declspec(selectany) IID IID_IOleCommandTarget =
	{0xB722BCCB,0x4E68,0x101B,{0xA2,0xBC,0x00,0xAA,0x00,0x40,0x47,0x70}};
extern "C" const __declspec(selectany) IID IID_IProxyManager =
	{0x00000008,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}};
extern "C" const __declspec(selectany) IID IID_ITimerService =
	{0x3050F35F,0x98B5,0x11CF,{0xBB,0x82,0x00,0xAA,0x00,0xBD,0xCE,0x0B}};
extern "C" const __declspec(selectany) IID IID_IWebBrowser2 =
	{0xD30C1661,0xCDAF,0x11d0,{0x8A,0x3E,0x00,0xC0,0x4F,0xC9,0xE2,0x6E}};
extern "C" const __declspec(selectany) IID IID_IWebBrowserApp =
	{0x0002DF05,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}};

struct __declspec(uuid("618F8AD4-8B7A-11d0-8FCC-00C04FD9189D")) IDirectDraw3;
extern "C" const __declspec(selectany) IID IID_IDirectDraw3 =
	{0x618F8AD4,0x8B7A,0x11d0,{0x8F,0xCC,0x00,0xC0,0x4F,0xD9,0x18,0x9D}};

struct __declspec(uuid("6C14DB81-A733-11CE-A521-0020AF0BE560")) IDirectDrawSurface;
extern "C" const __declspec(selectany) IID IID_IDirectDrawSurface =
	{0x6C14DB81,0xA733,0x11CE,{0xA5,0x21,0x00,0x20,0xAF,0x0B,0xE5,0x60}};

/////////////////////////////////////////////////////////////////////////////
// 1D0D0809-3513-244F-4B3D-2A0A1D131B17		Category GUID for MediaLooks filters
extern "C" const __declspec(selectany) CLSID CLSID_MediaLooksCategory =
	{0x1D0D0809, 0x3513, 0x244F, {0x4B, 0x3D, 0x2A, 0x0A, 0x1D, 0x13, 0x1B, 0x17}};

/////////////////////////////////////////////////////////////////////////////
__declspec(selectany) GUID_NAME s_rgDSGuidName[] =
{
	//---------------------------------
	__GUIDSTR(GUID_NULL),

	//--------------------------------- Filters/graphs
	{ &CLSID_FilterGraph,				_T("Filter Graph") },
    { &CLSID_DvdGraphBuilder,			_T("DVD Graph Builder") },
    { &CLSID_CaptureGraphBuilder,		_T("Capture Graph Builder") },

	{ &CLSID_SystemClock,				_T("System Reference clock") },

	{ &CLSID_DVDNavigator,				_T("DVD Navigator") },
    { &CLSID_AsyncReader,				_T("Async Reader") },
	{ &CLSID_URLReader,					_T("File Source (URL)") },

	{ &CLSID_Colour,					_T("Colour space convertor") },
    { &CLSID_InfTee,					_T("Infinite Pin Tee Filter") },
    { &CLSID_SmartTee,					_T("Smart Tee Filter") },
    { &CLSID_VfwCapture,				_T("Vfw Capture Filter") },

    { &CLSID_CMpegAudioCodec,			_T("MPEG-1 Audio Decoder") },
    { &CLSID_CMpegVideoCodec,			_T("MPEG-1 Video Decoder") },

	{ &CLSID_ACMWrapper,				_T("ACM wrapper") },
	{ &CLSID_AviDest,					_T("Avi Mux") },
	{ &CLSID_AviSplitter,				_T("Avi Splitter") },
	{ &CLSID_AVICo,						_T("AVI Compressor") },
    { &CLSID_AVIDec,					_T("AVI Decompressor") },
    { &CLSID_AVIDraw,					_T("AVI Draw Filter") },
    { &CLSID_MPEG1Splitter,				_T("MPEG-1 Stream Splitter") },
	{ &CLSID_MPEG2Demultiplexer,		_T("MPEG-2 Demultiplexer") },
	{ &CLSID_MMSPLITTER,				_T("MPEG-2 Splitter") },
	{ &CLSID_WaveParser,				_T("Wave Parser") },

    { &CLSID_DVMux,						_T("DV Mux") },
    { &CLSID_DVSplitter,				_T("DV Splitter") },
    { &CLSID_DVVideoCodec,				_T("DV Video Codec") },
    { &CLSID_DVVideoEnc,				_T("DV Video Encoder") },

	{ &CLSID_Line21Decoder,				_T("Line21 Decoder") },
	{ &CLSID_OverlayMixer,				_T("Overlay Mixer") },
	{ &CLSID_OverlayMixer2,				_T("Overlay Mixer 2") },
	{ &CLSID_VideoRenderer,				_T("Video Renderer") },
	{ &CLSID_VideoMixingRenderer,		_T("VMR") },
	{ &CLSID_VideoMixingRenderer9,		_T("Video Mixing Renderer 9") },

    { &CLSID_AudioRender,				_T("Audio Render") },
	{ &CLSID_DSoundRender,				_T("DirectSound Renderer") },
    { &CLSID_AVIMIDIRender,				_T("MIDI Renderer") },

	{ &CLSID_NullRenderer,				_T("Null Renderer") },

	{ &CLSID_FileWriter,				_T("File Writer") },

    { &CLSID_WMAsfReader,				_T("Asf Reader") },
    { &CLSID_WMAsfWriter,				_T("Asf Writer") },

	{ &CLSID_WMPDMOContainer,			_T("WMP DMO Container")	},
	{ &CLSID_WME_Sink,					_T("WME Sink (???)")  },

	{ &CLSID_StillVideoSource,			_T("Still Video Source") },

	//--------------------------------- Direct Show Property pages
	{ &CLSID_TVTunerFilterPropertyPage,			_T("TV Tuner Filter Property Page") },
	{ &CLSID_CrossbarFilterPropertyPage,		_T("Crossbar Filter Property Page") },
	{ &CLSID_TVAudioFilterPropertyPage,			_T("TV Audio Filter Property Page") },
	{ &CLSID_VideoProcAmpPropertyPage,			_T("Video Proc Amp Property Page") },
	{ &CLSID_CameraControlPropertyPage,			_T("Camera Control Property Page") },
	{ &CLSID_AnalogVideoDecoderPropertyPage,	_T("Analog Video Decoder Property Page") },
	{ &CLSID_VideoStreamConfigPropertyPage,		_T("Video Stream Config Property page") },
	{ &CLSID_AudioRendererAdvancedProperties,	_T("Audio Renderer Advanced Property page") },

	//--------------------------------- Filter categories
	{ &CLSID_ActiveMovieCategories,		_T("Active Movie Categories") },
    { &CLSID_LegacyAmFilterCategory,	_T("DirectShow Filters category") },
    { &CLSID_AudioCompressorCategory,	_T("Audio Compressor Category") },
    { &CLSID_AudioInputDeviceCategory,	_T("Audio Input Device Category") },
    { &CLSID_AudioRendererCategory,		_T("Audio Renderer Category") },
    { &CLSID_DVDHWDecodersCategory,		_T("DVD HW Decoders Category") },
    { &CLSID_MidiRendererCategory,		_T("Midi Renderer Category") },
    { &CLSID_VideoCompressorCategory,	_T("Video Compressor Category") },
    { &CLSID_VideoInputDeviceCategory,	_T("Video Input Device Category") },
	{ &AM_KSCATEGORY_CAPTURE,			_T("WDM Streaming Capture Devices") },
	{ &AM_KSCATEGORY_RENDER,			_T("WDM Streaming Rendering Devices") },

	//---------------------------------
	// Interfaces
	__UUIDSTR( IAMBufferNegotiation ),
	__UUIDSTR( IAMDeviceRemoval ),
	__UUIDSTR( IAMFilterMiscFlags ),
	__UUIDSTR( IAMLatency ),
	__UUIDSTR( IAMLine21Decoder ),
	__UUIDSTR( IAMOpenProgress ),
    __UUIDSTR( IAMPushSource ),
	__UUIDSTR( IAMStreamConfig ),
	__UUIDSTR( IBaseFilter  ),
	__UUIDSTR( IBasicAudio ),
	__UUIDSTR( IBasicVideo ),
	__UUIDSTR( IBasicVideo2 ),
	__UUIDSTR( IDvdControl ),
	__UUIDSTR( IDvdControl2 ),
	__UUIDSTR( IDvdGraphBuilder ),
	__UUIDSTR( IDvdInfo ),
	__UUIDSTR( IDvdInfo2 ),
	__UUIDSTR( IDvdState ),
	__UUIDSTR( IEnumFilters ),
	__UUIDSTR( IEnumMediaTypes ),
	__UUIDSTR( IEnumPins ),
	__UUIDSTR( IFileSinkFilter ),
	__UUIDSTR( IFileSourceFilter ),
	__UUIDSTR( IFilterGraph ),
	__UUIDSTR( IFilterGraph2 ),
	__UUIDSTR( IGraphBuilder ),
	__UUIDSTR( IIPDVDec ),
	__UUIDSTR( IKsPin ),
	__UUIDSTR( IKsPropertySet ),
	__UUIDSTR( IMediaControl ),
	__UUIDSTR( IMediaEvent ),
	__UUIDSTR( IMediaEventEx ),
	__UUIDSTR( IMediaFilter ),
	__UUIDSTR( IMediaPosition ),
	__UUIDSTR( IMediaSample ),
	__UUIDSTR( IMediaSample2 ),
	__UUIDSTR( IMediaSeeking ),
	__UUIDSTR( IMemAllocator ),
	__UUIDSTR( IMemInputPin ),
	__UUIDSTR( IMixerPinConfig ),
	__UUIDSTR( IMixerPinConfig2 ),
	__UUIDSTR( IOverlay ),
	__UUIDSTR( IPin ),
	__UUIDSTR( IQualityControl ),
	__UUIDSTR( IReferenceClock ),
	__UUIDSTR( IVideoWindow ),

	//---------------------------------
	// Windows interfaces
	{ &CLSID_HTMLViewer,				_T("CLSID_HTMLViewer") },

	__UUIDSTR( IAdviseSink ),
	__UUIDSTR( IAdviseSinkEx ),
	__UUIDSTR( IBindHost ),
	__UUIDSTR( IClientSecurity ),
	__UUIDSTR( IConnectionPointContainer ),
	__UUIDSTR( IDirectDraw3 ),
	__UUIDSTR( IDirectDrawSurface ),
	__UUIDSTR( IDispatch ),
	__UUIDSTR( IObjectWithSite ),
	__UUIDSTR( IOleClientSite ),
	__UUIDSTR( IOleControl ),
	__UUIDSTR( IOleControlSite ),
	__UUIDSTR( IOleInPlaceFrame ),
	__UUIDSTR( IOleInPlaceObject ),
	__UUIDSTR( IOleInPlaceObjectWindowless ),
	__UUIDSTR( IOleInPlaceSite ),
	__UUIDSTR( IOleInPlaceSiteEx ),
	__UUIDSTR( IOleInPlaceSiteWindowless ),
	__UUIDSTR( IOleInPlaceUIWindow ),
	__UUIDSTR( IOleObject ),
	__UUIDSTR( IOleWindow ),
	__UUIDSTR( IPersistPropertyBag ),
	__UUIDSTR( IPersistPropertyBag2 ),
	__UUIDSTR( IPersistStream ),
	__UUIDSTR( IPersistStreamInit ),
	__UUIDSTR( IPersistStorage ),
	__UUIDSTR( IPropertyNotifySink ),
	__UUIDSTR( IQuickActivate ),
	__UUIDSTR( IRunnableObject ),
	__UUIDSTR( IServiceProvider ),
	__UUIDSTR( ISpecifyPropertyPages ),
	__UUIDSTR( IViewObject ),
	__UUIDSTR( IViewObjectEx ),

	{ &IID_IActiveScript,				_T("IActiveScript") },
	{ &IID_IHTMLDocument2,				_T("IHTMLDocument2") },
	{ &IID_IHTMLLocation,				_T("IHTMLLocation") },
	{ &IID_IHTMLWindow2,				_T("IHTMLWindow2") },
	{ &IID_IObjectSafety,				_T("IObjectSafety") },
	{ &IID_IOleCommandTarget,			_T("IOleCommandTarget") },
	{ &IID_IProxyManager,				_T("IProxyManager") },
	{ &IID_ITimerService,				_T("ITimerService") },
	{ &IID_IWebBrowserApp,				_T("IWebBrowserApp") },
	{ &IID_IWebBrowser2,				_T("IWebBrowser2") },

	//--------------------------------- AM_MEDIA_TYPE::majortype
	__GUIDSTR( MEDIATYPE_AnalogAudio ),
	__GUIDSTR( MEDIATYPE_AnalogVideo ),
	__GUIDSTR( MEDIATYPE_Video ),
	__GUIDSTR( MEDIATYPE_Audio ),
	__GUIDSTR( MEDIATYPE_Stream ),
	__GUIDSTR( MEDIATYPE_DVD_ENCRYPTED_PACK ),
	__GUIDSTR( MEDIATYPE_DVD_NAVIGATION ),
	__GUIDSTR( MEDIATYPE_MPEG2_PES ),
	__GUIDSTR( MEDIATYPE_MPEG2_PACK ),
	__GUIDSTR( MEDIATYPE_AUXLine21Data ),
	__GUIDSTR( MEDIATYPE_File ),
	__GUIDSTR( MEDIATYPE_Interleaved ),
	__GUIDSTR( MEDIATYPE_LMRT ),
	__GUIDSTR( MEDIATYPE_MPEG1SystemStream ),
	__GUIDSTR( MEDIATYPE_Midi ),
	__GUIDSTR( MEDIATYPE_ScriptCommand ),
	__GUIDSTR( MEDIATYPE_Text ),
	__GUIDSTR( MEDIATYPE_Timecode ),
	__GUIDSTR( MEDIATYPE_URL_STREAM ),
	__GUIDSTR( MEDIATYPE_VBI ),

	//---------------------------------	AM_MEDIA_TYPE::subtype
	// Audio
	__GUIDSTR( MEDIASUBTYPE_PCM ),
	__GUIDSTR( MEDIASUBTYPE_WAVE ),
	__GUIDSTR( MEDIASUBTYPE_DOLBY_AC3 ),
	__GUIDSTR( MEDIASUBTYPE_DOLBY_AC3_SPDIF ),
	__GUIDSTR( MEDIASUBTYPE_AAC ),
	__GUIDSTR( MEDIASUBTYPE_RAW_SPORT ),
	__GUIDSTR( MEDIASUBTYPE_SPDIF_TAG_241h ),

	// RGB
	__GUIDSTR( MEDIASUBTYPE_RGB1 ),
	__GUIDSTR( MEDIASUBTYPE_RGB4 ),
	__GUIDSTR( MEDIASUBTYPE_RGB8 ),
	__GUIDSTR( MEDIASUBTYPE_RGB555 ),
	__GUIDSTR( MEDIASUBTYPE_RGB565 ),
	__GUIDSTR( MEDIASUBTYPE_RGB24 ),
	__GUIDSTR( MEDIASUBTYPE_RGB32 ),
	__GUIDSTR( MEDIASUBTYPE_ARGB32 ),

	// YUV
	__GUIDSTR( MEDIASUBTYPE_CLPL ),
	__GUIDSTR( MEDIASUBTYPE_YUYV ),
	__GUIDSTR( MEDIASUBTYPE_IYUV ),
	__GUIDSTR( MEDIASUBTYPE_YVU9 ),
	__GUIDSTR( MEDIASUBTYPE_Y411 ),
	__GUIDSTR( MEDIASUBTYPE_Y41P ),
	__GUIDSTR( MEDIASUBTYPE_YUY2 ),
	__GUIDSTR( MEDIASUBTYPE_YVYU ),
	__GUIDSTR( MEDIASUBTYPE_UYVY ),
	__GUIDSTR( MEDIASUBTYPE_Y211 ),
	__GUIDSTR( MEDIASUBTYPE_YV12 ),
	__GUIDSTR( MEDIASUBTYPE_CLJR ),
	__GUIDSTR( MEDIASUBTYPE_IF09 ),
	__GUIDSTR( MEDIASUBTYPE_CPLA ),

	// Defined in current file
	__GUIDSTR( MEDIASUBTYPE_I420 ),
	__GUIDSTR( MEDIASUBTYPE_IYU2 ),
	__GUIDSTR( MEDIASUBTYPE_Y422 ),
	__GUIDSTR( MEDIASUBTYPE_Y444 ),

	// MPEG1
	__GUIDSTR( MEDIASUBTYPE_MPEG1System ),
	__GUIDSTR( MEDIASUBTYPE_MPEG1Audio ),
	__GUIDSTR( MEDIASUBTYPE_MPEG1AudioPayload ),
	__GUIDSTR( MEDIASUBTYPE_MPEG1Packet ),
	__GUIDSTR( MEDIASUBTYPE_MPEG1Payload ),
	__GUIDSTR( MEDIASUBTYPE_MPEG1Video ),
	__GUIDSTR( MEDIASUBTYPE_MPEG1VideoCD ),

	// MPEG2
	__GUIDSTR( MEDIASUBTYPE_MPEG2_PROGRAM ),
	__GUIDSTR( MEDIASUBTYPE_MPEG2_TRANSPORT ),
	__GUIDSTR( MEDIASUBTYPE_MPEG2_AUDIO ),
	__GUIDSTR( MEDIASUBTYPE_MPEG2_VIDEO ),

	// H264: defined in current file
	__GUIDSTR( MEDIASUBTYPE_AVC1 ),
	__GUIDSTR( MEDIASUBTYPE_H264ByteStream ),
	__GUIDSTR( MEDIASUBTYPE_H264 ),
	__GUIDSTR( MEDIASUBTYPE_h264 ),

	// DVD
	__GUIDSTR( MEDIASUBTYPE_DVD_SUBPICTURE ),
	__GUIDSTR( MEDIASUBTYPE_DVD_LPCM_AUDIO ),
	__GUIDSTR( MEDIASUBTYPE_DVD_NAVIGATION_DSI ),
	__GUIDSTR( MEDIASUBTYPE_DVD_NAVIGATION_PCI ),
	__GUIDSTR( MEDIASUBTYPE_DVD_NAVIGATION_PROVIDER ),

	// VBI, etc.
	__GUIDSTR( KSDATAFORMAT_SUBTYPE_RAW8),
#if (NTDDI_VERSION >= NTDDI_WINXP)
	__GUIDSTR( KSDATAFORMAT_SUBTYPE_CC ),
#endif 
	__GUIDSTR( KSDATAFORMAT_SUBTYPE_NABTS ),
	__GUIDSTR( MEDIASUBTYPE_TELETEXT ),
	__GUIDSTR( MEDIASUBTYPE_Line21_BytePair ),
	__GUIDSTR( MEDIASUBTYPE_Line21_GOPPacket ),
	__GUIDSTR( MEDIASUBTYPE_Line21_VBIRawData ),

	// QuickTime
	__GUIDSTR( MEDIASUBTYPE_QTJpeg ),
	__GUIDSTR( MEDIASUBTYPE_QTMovie ),
	__GUIDSTR( MEDIASUBTYPE_QTRle ),
	__GUIDSTR( MEDIASUBTYPE_QTRpza ),
	__GUIDSTR( MEDIASUBTYPE_QTSmc ),

	// DSS
	__GUIDSTR( MEDIASUBTYPE_DssAudio ),
	__GUIDSTR( MEDIASUBTYPE_DssVideo ),
	__GUIDSTR( MEDIASUBTYPE_VPVBI ),
	__GUIDSTR( MEDIASUBTYPE_VPVideo ),

	// Another
	__GUIDSTR( MEDIASUBTYPE_Overlay ),
	__GUIDSTR( MEDIASUBTYPE_Avi ),
	__GUIDSTR( MEDIASUBTYPE_AIFF ),
	__GUIDSTR( MEDIASUBTYPE_Asf ),
	__GUIDSTR( MEDIASUBTYPE_AU ),
	__GUIDSTR( MEDIASUBTYPE_None ),

	//--------------------------------- AM_MEDIA_TYPE::format
	__GUIDSTR( FORMAT_VideoInfo ),
	__GUIDSTR( FORMAT_VideoInfo2 ),
	__GUIDSTR( FORMAT_WaveFormatEx ),
	__GUIDSTR( FORMAT_MPEGStreams ),
	__GUIDSTR( FORMAT_MPEGVideo ),
	__GUIDSTR( FORMAT_MPEG2Audio ),
	__GUIDSTR( FORMAT_MPEG2Video ),
	__GUIDSTR( FORMAT_AnalogVideo ),
	__GUIDSTR( FORMAT_DVD_LPCMAudio ),
	__GUIDSTR( FORMAT_DolbyAC3 ),
	__GUIDSTR( FORMAT_DvInfo ),
	__GUIDSTR( FORMAT_None ),
	
	// From ksmedia.h
	__GUIDSTR( KSDATAFORMAT_SPECIFIER_VBI ),
	__GUIDSTR( PROPSETID_VIDCAP_VIDEOCOMPRESSION ),

	//--------------------------------- Another
	// KSPROPSETID
	__GUIDSTR( KSPROPSETID_CopyProt ),
	__GUIDSTR( KSPROPSETID_TSRateChange ),
	__GUIDSTR( KSPROPSETID_DvdSubPic),

	// Time Format GUIDs
	__GUIDSTR( TIME_FORMAT_FRAME ),
	__GUIDSTR( TIME_FORMAT_BYTE ),
	__GUIDSTR( TIME_FORMAT_SAMPLE ),
	__GUIDSTR( TIME_FORMAT_FIELD ),
	__GUIDSTR( TIME_FORMAT_MEDIA_TIME),

	//--------------------------------- Pin categories
	__GUIDSTR( AMPROPSETID_Pin ),
	__GUIDSTR( PIN_CATEGORY_ANALOGVIDEOIN ),
	__GUIDSTR( PIN_CATEGORY_CAPTURE ),
	__GUIDSTR( PIN_CATEGORY_CC ),
	__GUIDSTR( PIN_CATEGORY_EDS ),
	__GUIDSTR( PIN_CATEGORY_NABTS ),
	__GUIDSTR( PIN_CATEGORY_PREVIEW ),
	__GUIDSTR( PIN_CATEGORY_STILL ),
	__GUIDSTR( PIN_CATEGORY_TELETEXT ),
	__GUIDSTR( PIN_CATEGORY_TIMECODE ),
	__GUIDSTR( PIN_CATEGORY_VBI ),
	__GUIDSTR( PIN_CATEGORY_VIDEOPORT ),
	__GUIDSTR( PIN_CATEGORY_VIDEOPORT_VBI ),
};

//---------------------------------
GUID_NAME_NODE(s_nodeDSGuidName, s_rgDSGuidName)

#undef PREV_GUID_NAME_NODE
#define PREV_GUID_NAME_NODE	&s_nodeDSGuidName

/////////////////////////////////////////////////////////////////////////////
#endif	// __DSGUIDS_H_
