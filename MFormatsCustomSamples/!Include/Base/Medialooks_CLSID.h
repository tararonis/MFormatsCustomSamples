//---------------------------------------------------------------------------
// Medialooks_CLSID.h : 
//
// Description: 
//
//---------------------------------------------------------------------------
// Copyright (c) 2017, Medialooks Ltd.
// http://www.medialooks.com (dev@medialooks.com)
//
// Author:  Vsevolod V. Burkutsky (VVB)
// Version:	1.0 (2017-01-11)
//---------------------------------------------------------------------------
// CONFIDENTIAL INFORMATION
//
// This file is Intellectual Property (IP) of Medialooks Ltd. and is
// strictly confidential. You can gain access to this file only if you
// sign a License Agreement and a Non-Disclosure Agreement (NDA) with
// Medialooks Ltd. If you had not signed any of these documents, please
// contact <dev@medialooks.com> immediately.
//
//---------------------------------------------------------------------------
// $Id$
//---------------------------------------------------------------------------

#pragma once

//////////////////////////////////////////////////////////////////////////
// MRuntime
class DECLSPEC_UUID("4F365177-473F-4ad5-A5C2-428FE0C8E03E")
	MFFactory;

class DECLSPEC_UUID("C08B164D-5CCF-4251-B22F-27AAA7ECABF3")
	MFSink;

class DECLSPEC_UUID("0792B97B-E4CB-4140-B848-EE96C6660837")
	MFConverter;

class DECLSPEC_UUID("0792B96A-E4CB-4140-B848-EE96C6660837")
	MFAudioBuffer;

class DECLSPEC_UUID("5902F643-CCFF-4EE5-8440-82D903492749")
	MSourceDS;

class DECLSPEC_UUID("8D54AB24-D639-4336-9C3A-17F2D0EAAFA0")
	MFSideData;

class DECLSPEC_UUID("8D54AB26-D639-4336-9C3A-17F2D0EAAFA0")
	MFPipe;

class DECLSPEC_UUID("21AFEE0C-F30E-4D29-B496-1DA4DA212A84")
	MSinkDS;

//////////////////////////////////////////////////////////////////////////
// MPlatform

class DECLSPEC_UUID("D6FC798C-B43E-43EB-B135-5F87CF93B984")
	MFileFFM_I;

class DECLSPEC_UUID("21AFEE0C-F30E-4D29-B496-1DA4DA212A84")
	MSinkDS;

//////////////////////////////////////////////////////////////////////////
// Plugins

class DECLSPEC_UUID("877DD976-9AB6-4366-A3DA-89A76FD8B547")
	CoCEFObject;

class DECLSPEC_UUID("40CD2744-06D8-4D57-ACD9-E1D523660AD4")
	MDelay;

class DECLSPEC_UUID("B69932C1-DE12-4C17-A205-7CF52B719636")
	CoMFilterFFM;

class DECLSPEC_UUID("92864720-72D1-4BF8-B860-D9600BD8A104")
	CoMAudioFFM;

//////////////////////////////////////////////////////////////////////////
// MFDevices

class DECLSPEC_UUID("604BE365-B73E-4FCF-88EA-45FE1A2A0A0F")	
	MFLiveBMD;

class DECLSPEC_UUID("03AC4D14-7C4C-410C-8429-FC58BDB54FFE")
	MFLiveDS;

class DECLSPEC_UUID("604BE36A-B73E-4FCF-88EA-45FE1A2A0A0F")
	MFLiveAJA;

class DECLSPEC_UUID("604BE376-B73E-4FCF-88EA-45FE1A2A0A0F")
	MFLiveBMDS;

class DECLSPEC_UUID("C631082E-DBE4-48E0-AA89-672C5B10CCDE")
	MFCaptureSL2;

class DECLSPEC_UUID("452E0371-E29E-4F2B-9810-9AD7A6C48102")
	MFRenderSL2;

class DECLSPEC_UUID("FDB76080-E089-41D5-9C04-545B9E07992F")
	MFCaptureBF;

class DECLSPEC_UUID("C5997D38-884C-49D5-A430-699B573836C4")
	MFRenderBF;

class DECLSPEC_UUID("4EF4FB7B-D93F-44B7-B6A8-5869CCEC8F37")
	MFCaptureDT;

class DECLSPEC_UUID("A0A21412-A5F3-471B-87D8-A7AE4F5B4367")
	MFRenderDT;

class DECLSPEC_UUID("B8E10465-DDD9-42EC-9F3A-8B5D2954F48F")
	MFCaptureSCR;

class DECLSPEC_UUID("E674F74E-5F25-4DA0-8F72-EB62C1B39B2E")
	MFCaptureDC;

class DECLSPEC_UUID("3376E267-34DF-4825-8207-3D30E99FF6CA")
	MFRendererDC;

class DECLSPEC_UUID("7B1C4AB9-A967-4EC1-BBD2-C10806D32682")
	MFRendererNDI;

class DECLSPEC_UUID("A896A5FB-126E-4D11-98F0-13EBC6BE2023")
	MFReceiverNDI;

class DECLSPEC_UUID("70E3F3BF-00E1-4DC2-BE5D-8FF15C877C9C")
	MFReceiverCEF;

class DECLSPEC_UUID("3DE42C28-40E8-45BF-8949-22CF44523C02")
	MFCaptureMW;

class DECLSPEC_UUID("23FACFC8-45A2-4AF6-97E8-C075056A55E8")
	MFCaptureAJA2;

class DECLSPEC_UUID("298AEA80-B023-4B6E-B6F9-BB9E55911852")
	MFRendererAJA2;

//////////////////////////////////////////////////////////////////////////
// MFCodecs

class DECLSPEC_UUID("96EB1C14-4CC0-4830-9CC3-F064196B2626")
	MFSplitter;

class DECLSPEC_UUID("96EB1C64-4CC0-4830-9CC3-F064196B2626")
	MFDecoder;

class DECLSPEC_UUID("96EB1C74-4CC0-4830-9CC3-F064196B2626")
	MFMuxer;

class DECLSPEC_UUID("96EB1C94-4CC0-4830-9CC3-F064196B2626")
	MFEncoder;

class DECLSPEC_UUID("96EB1F14-4CC0-4830-9CC3-F064196B2626")
	MFSplitterFFM;

class DECLSPEC_UUID("96EB1F64-4CC0-4830-9CC3-F064196B2626")
	MFDecoderFFM;

class DECLSPEC_UUID("96EB1F23-4CC0-4830-9CC3-F064196B2626")
	MFMuxerFFM;

class DECLSPEC_UUID("96EB1F26-4CC0-4830-9CC3-F064196B2626")
	MFEncoderFFM;

class DECLSPEC_UUID("BF23862C-7C7B-4CE3-8FE7-4CA5171D624E")
	MFDecoderQS;

class DECLSPEC_UUID("BE272839-CA83-42CA-931A-8273EFBDA9AE")
	MFEncoderQS;

class DECLSPEC_UUID("2CBD671C-8106-49EC-8ABB-9D67CF025301")
	MFDecoderNV;

class DECLSPEC_UUID("D0C31F85-8131-48FD-82DA-1D2249FFBD53")
	MFEncoderNV;

class DECLSPEC_UUID("ECFBFE56-F923-44B6-91D4-C9E3EA04131B")
	MFSplitterMXF_Nablet;

class DECLSPEC_UUID("A6C0D9AD-A719-4FDC-901A-246BC5211309")
	MFSplitterMPEG2_Nablet;

class DECLSPEC_UUID("090AD36C-B9C2-483C-97F7-4F7023D238DB")
	MFMuxerMPEG2_Nablet;

class DECLSPEC_UUID("9285699B-E779-4B2E-92CA-26DDEE01AA2A")
	MFCaptionsDecoder;

//////////////////////////////////////////////////////////////////////////
// WebRTC

class DECLSPEC_UUID("B9E821B4-D77D-4B28-B13E-62C0235F8C5C")
	MWebRTC_Core;

class DECLSPEC_UUID("B9E821B7-D77D-4B28-B13E-62C0235F8C5C")
	MWebRTC_MFLive;

class DECLSPEC_UUID("B9E821B8-D77D-4B28-B13E-62C0235F8C5C")
	MWebRTC_MFRenderer;

//////////////////////////////////////////////////////////////////////////
// MLProxy
class DECLSPEC_UUID("988CDFB8-09F0-43D4-BB86-B1613F41D940")
	CoMLProxy;


